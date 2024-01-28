#ifndef OPM_H
#define OPM_H

#include <stdint.h>
#include <stdlib.h>

typedef enum{
  GetPDVolCMD = 0x01,
  GetL1550OptPwrCMD = 0x02,
  GetL1310OptPwrCMD = 0x03,
  GetL850OptPwrCMD = 0x04,
  GetL1550JZParaCMD = 0x05,
  GetL1310JZParaCMD = 0x06,
  GetL850JZParaCMD = 0x07,
  SetL1550JZParaCMD = 0x08,
  SetL1310JZParaCMD = 0x09,
  SetL850JZParaCMD = 0x10,
} enum_cmd_t;

typedef struct out_frame{
  uint8_t FS[2];
  uint8_t SAddr[4];
  uint8_t B;
  uint8_t DataLen;
  uint8_t *DataField;
  uint8_t FR;
} out_frame_t;

typedef out_frame_t in_frame_t;

typedef struct device{
  unsigned char* manufacturer;
  unsigned char* product;
  unsigned char* serial_number;
  int index;
} opm_device_t;


int open_opm_devices(opm_device_t **);
void close_opm_devices(opm_device_t **, size_t len);
float get_pd_vol(opm_device_t *);
#endif
