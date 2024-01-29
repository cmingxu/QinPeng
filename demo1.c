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

  for (int i = 0;  i < r;  i++) {
    float v = get_l1550_opt_pwr(&devs[i]);
    printf("index %d 1550 opt pwr: %f\n",i,  v);
  }

  for (int i = 0;  i < r;  i++) {
    float v = get_l1310_opt_pwr(&devs[i]);
    printf("index %d 1310 opt pwr: %f\n", i, v);
  }

  for (int i = 0;  i < r;  i++) {
    float v = get_l850_opt_pwr(&devs[i]);
    printf("index %d 850 opt pwr: %f\n", i, v);
  }

  for (int i = 0;  i < r;  i++) {
    float para[4] = {1, 2, 3, 4};
    int r = set_l1550_para(&devs[i], para);
    for(int j = 0; j < 4; j ++) {
      printf("index %d set 1550 para: %f\n",j, para[j] );
    }
  }

  for (int i = 0;  i < r;  i++) {
    float para[4] = {0};
    int r = get_l1550_para(&devs[i], para);
    for(int j = 0; j < 4; j ++) {
      printf("get 1550 para %d: %f\n",j, para[j] );
    }
  }

  close_opm_devices(&devs, r);
}

