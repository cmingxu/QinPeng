#include <ch9326.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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
  frame->FR = buf[8 + frame->DataLen];
  return frame;
}

static void free_in_frame(in_frame_t *frame){
  free(frame);
}

static size_t in_frame_size(in_frame_t *frame) {
  return frame->DataLen;
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
  out_frame_t *out = create_out_frame(
    GetPDVolCMD,
    NULL,
    0
  );
  in_frame_t *in = NULL;

  unsigned char buf[128];
  memset(buf, 0, 128);
  int nWrite = ch9326_send(dev->index, (unsigned char*)out, out_frame_size(out));

  int nRead = ch9326_recv(dev->index, buf, 128);
  for(int r = 0; r < 10900; r++){
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
