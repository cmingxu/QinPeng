#include <ch9326.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include "lib/libch9326_x86/ch9326.h"
#include "opm.h"
#include "utils.h"

#define BUFSIZE 128
#define FRAME_HEADER_SIZE 9

const uint8_t DEFAULT_FS[2] = {0x55, 0xAA};
const uint8_t DEFAULT_SAddr[4] = {0x00, 0x00, 0x00, 0x01};
const uint8_t DEFAULT_CRC = 0x00;

static out_frame_t *create_out_frame(enum_cmd_t t, uint8_t *data,  uint8_t dataLen){
  out_frame_t *frame = (out_frame_t *)malloc(sizeof(out_frame_t));
  memcpy(frame->FS, DEFAULT_FS, 2);
  memcpy(frame->SAddr, DEFAULT_SAddr, 4);
  frame->B = t;
  frame->DataLen = dataLen;
  frame->DataField = data;
  frame->FR = DEFAULT_CRC;
  return frame;
}

static size_t out_frame_buf(out_frame_t *frame, char *buf) {
  memcpy(buf, frame->FS, 2);
  memcpy(buf + 2, frame->SAddr, 4);
  buf[6] = frame->B;
  buf[7] = frame->DataLen;
  memcpy(buf + 8, frame->DataField, frame->DataLen);
  buf[8 + frame->DataLen] = frame->FR;

  return frame->DataLen + FRAME_HEADER_SIZE;
}

static size_t out_frame_size(out_frame_t *frame) {
  return frame->DataLen + FRAME_HEADER_SIZE;
}

static void free_out_frame(out_frame_t *frame){
  free(frame);
}

static in_frame_t *create_in_frame(const char *buf){
  in_frame_t *frame = (in_frame_t *)malloc(sizeof(in_frame_t));
  memcpy(frame->FS, buf, 2);
  memcpy(frame->SAddr, buf + 2, 4);
  frame->B = buf[6];
  frame->DataLen = buf[7];
  frame->DataField = (unsigned char*)malloc(frame->DataLen);
  memcpy(frame->DataField, buf + 8, frame->DataLen);
  frame->FR = buf[8 + frame->DataLen] & 0xFF;
  return frame;
}

static void free_in_frame(in_frame_t *frame){
  free(frame);
}

static size_t in_frame_size(in_frame_t *frame) {
  return frame->DataLen + FRAME_HEADER_SIZE;
}

static float send_and_recv_single_float(opm_device_t *dev, enum_cmd_t cmd){
  out_frame_t *out = create_out_frame(
    cmd,
    NULL,
    0
  );
  in_frame_t *in = NULL;

  unsigned char buf[128];
  memset(buf, 0, 128);
  int out_size = out_frame_buf(out, (char *)buf);
  int nWrite = ch9326_send(dev->index, buf, out_size);

  int nRead = ch9326_recv(dev->index, buf, 128);
  for(int r = 0; r < 1000; r++){
    if(nRead != 0)
      break;

    usleep(100);
    nRead = ch9326_recv(dev->index, buf, 128);
  }

  in = create_in_frame((char *)buf);

  float f;
  bytes_to_float(in->DataField, &f);

  free_out_frame(out);
  free_in_frame(in);
  return f;
}

static int get_para_common(opm_device_t *dev, enum_cmd_t cmd, float *f, size_t size){
  out_frame_t *out = create_out_frame(
    cmd,
    NULL,
    0
  );
  in_frame_t *in = NULL;

  unsigned char buf[128];
  memset(buf, 0, 128);
  int out_size = out_frame_buf(out, (char *)buf);
  int nWrite = ch9326_send(dev->index, buf, out_size);

  int nRead = ch9326_recv(dev->index, buf, 128);
  for(int r = 0; r < 1000; r++){
    if(nRead != 0)
      break;

    usleep(100);
    nRead = ch9326_recv(dev->index, buf, 128);
  }

  in = create_in_frame((char *)buf);

  bytes_to_multiple_float(in->DataField, f, size);

  free_out_frame(out);
  free_in_frame(in);
  return 0;
}

static int set_para_common(opm_device_t *dev, enum_cmd_t cmd, unsigned char *f, size_t size){
  out_frame_t *out = create_out_frame(
    cmd,
    f,
    size
  );
  in_frame_t *in = NULL;

  unsigned char buf[128];
  memset(buf, 0, 128);
  int out_size = out_frame_buf(out, (char *)buf);
  int nWrite = ch9326_send(dev->index, buf, out_size);

  int nRead = ch9326_recv(dev->index, buf, 128);
  for(int r = 0; r < 1000; r++){
    if(nRead != 0)
      break;

    usleep(100);
    nRead = ch9326_recv(dev->index, buf, 128);
  }

  in = create_in_frame((char *)buf);

  assert(in->B == out->B);
  assert(in->DataLen == size);

  free_out_frame(out);
  free_in_frame(in);
  return 0;
}

int open_opm_devices(opm_device_t **devs){
  int i;
  int r = ch9326_find();
  if(r < 0){
    return r;
  }
  *devs = (opm_device_t *)malloc(sizeof(opm_device_t) * r);
  for(i = 0; i < r; i++){
    (*devs)[i].index = i;
  }

  for(i = 0; i < r; i ++) {
    ch9326_open(i);
  }

  for(i = 0; i < r; i++){
    (*devs)[i].product = (unsigned char*)malloc(BUFSIZE);
    (*devs)[i].manufacturer = (unsigned char*)malloc(BUFSIZE);
    (*devs)[i].serial_number = (unsigned char*)malloc(BUFSIZE);

    ch9326_get_product_string(i, (*devs)[i].product, BUFSIZE);
    ch9326_get_manufacturer_string(i, (*devs)[i].manufacturer, BUFSIZE);
    ch9326_get_serial_number_string(i, (*devs)[i].serial_number, BUFSIZE);
  }

  return r;
}

void close_opm_devices(opm_device_t **devs, size_t size){
  for(int i = 0; i < size; i++){
    ch9326_close(i);
  }
}

float get_pd_vol(opm_device_t *dev){
  return send_and_recv_single_float(dev, GetPDVolCMD);
}

float get_l1550_opt_pwr(opm_device_t *dev){
  return send_and_recv_single_float(dev, GetL1550OptPwrCMD);
}

float get_l1310_opt_pwr(opm_device_t *dev){
  return send_and_recv_single_float(dev, GetL1310OptPwrCMD);
}

float get_l850_opt_pwr(opm_device_t *dev){
  return send_and_recv_single_float(dev, GetL850OptPwrCMD);
}

int get_l1550_para(opm_device_t *dev, float *para){
  return get_para_common(dev, GetL1550JZParaCMD, para, 4);
}

int get_l1310_para(opm_device_t *dev, float *para){
  return get_para_common(dev, GetL1310JZParaCMD, para, 4);
}

int get_l850_para(opm_device_t *dev, float *para){
  return get_para_common(dev, GetL850JZParaCMD, para, 4);
}

int set_l1550_para(opm_device_t *dev, float *para){
  unsigned char buf[16];
  multiple_float_to_bytes(para, buf, 4);
  return set_para_common(dev, SetL1550JZParaCMD, buf, 16);
}

int set_l1310_para(opm_device_t *dev, float *para){
  unsigned char buf[16];
  multiple_float_to_bytes(para, buf, 4);
  return set_para_common(dev, SetL1310JZParaCMD, buf, 16);
}

int set_l850_para(opm_device_t *dev, float *para){
  unsigned char buf[16];
  multiple_float_to_bytes(para, buf, 4);
  return set_para_common(dev, SetL850JZParaCMD, buf, 16);
}
