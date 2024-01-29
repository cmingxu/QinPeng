#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stdio.h>
#include "opm.h"

union FloatValue {
  float f;
  uint8_t bytes[4];
};

void display_as_hex(const char *s, size_t len){
  int i;
  for(i = 0; i < len; i++){
    printf("%02x ", (unsigned char)s[i]);
  }
  printf("\n");
}

void display_as_asicc(const char *s, size_t len){
  int i;
  for(i = 0; i < len; i++){
    printf("%c", s[i]);
  }
  printf("\n");
}

void print_out_frame_t(out_frame_t *frame) {
  printf("FS: %02x %02x\n", frame->FS[0], frame->FS[1]);
  printf("SAddr: %02x %02x %02x %02x\n", frame->SAddr[0], frame->SAddr[1], frame->SAddr[2], frame->SAddr[3]);
  printf("B: %02x\n", frame->B);
  printf("DataLen: %02x\n", frame->DataLen);
  printf("DataField: ");
  printf("FR: %02x\n", frame->FR);
}

void print_in_frame_t(in_frame_t *frame) {
  printf("FS: %02x %02x\n", frame->FS[0], frame->FS[1]);
  printf("SAddr: %02x %02x %02x %02x\n", frame->SAddr[0], frame->SAddr[1], frame->SAddr[2], frame->SAddr[3]);
  printf("B: %02x\n", frame->B);
  printf("DataLen: %02x\n", frame->DataLen);
  printf("DataField: ");
  printf("FR: %02x\n", frame->FR);
}

void bytes_to_float(unsigned char *bytes, float *f) {
  union FloatValue fv;
  fv.bytes[0] = bytes[3];
  fv.bytes[1] = bytes[2];
  fv.bytes[2] = bytes[1];
  fv.bytes[3] = bytes[0];

  *f = fv.f;
}

void bytes_to_multiple_float(unsigned char *bytes, float *f, size_t count) {
  for(int i = 0; i < count; i ++) {
    union FloatValue fv;
    fv.bytes[0] = bytes[4 * i + 3];
    fv.bytes[1] = bytes[4 * i + 2];
    fv.bytes[2] = bytes[4 * i + 1];
    fv.bytes[3] = bytes[4 * i + 0];
    *(f + i) = fv.f;
  }
}

void float_to_bytes(float f, unsigned char *bytes) {
  union FloatValue fv;
  fv.f = f;
  bytes[0] = fv.bytes[3];
  bytes[1] = fv.bytes[2];
  bytes[2] = fv.bytes[1];
  bytes[3] = fv.bytes[0];
}

void multiple_float_to_bytes(float *f, unsigned char *bytes, size_t num_of_float) {
  for(int i = 0; i < num_of_float; i ++) {
    union FloatValue fv;
    fv.f = *(f + i);
    bytes[4 * i + 3] = fv.bytes[0];
    bytes[4 * i + 2] = fv.bytes[1];
    bytes[4 * i + 1] = fv.bytes[2];
    bytes[4 * i + 0] = fv.bytes[3];
  }
}

#endif /* ifndef  */
