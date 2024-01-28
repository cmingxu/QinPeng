#include <stdio.h>
#include <stdlib.h>
#include <ch9326.h>
#include "opm.h"

void main()
{
  opm_device_t *devs;
  int i, r;
  r = open_opm_devices(&devs);
  printf("found %d opm devices\n", r);
  for(i = 0; i < r; i++){
    printf("index: %d\n", devs[i].index);
    printf("product: %s\n", devs[i].product);
    printf("manufacturer: %s\n", devs[i].manufacturer);
    printf("serial_number: %s\n", devs[i].serial_number);
  }

  for (int i = 0;  i < r;  i++) {
    printf("ch9326 device %d connected: %d \n ", i, ch9326_connected(i));
  }

  for (int i = 0;  i < r;  i++) {
    float v = get_pd_vol(&devs[i]);
    printf("pd voltage: %f\n", v);
  }

  close_opm_devices(&devs, r);
}

