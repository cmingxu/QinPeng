#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#include <ch9326.h>
#include "opm.h"

#define MIN(a,b) (((a)<(b))?(a):(b))

static void print_usage() {
  fprintf(stderr, "\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "Usage: opm-cli [options]\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "Options:\n");
  fprintf(stderr, "  -l list all available devices\n");
  fprintf(stderr, "  -s set para\n");
  fprintf(stderr, "  -g get para\n");
  fprintf(stderr, "  -o get opt power\n");
  fprintf(stderr, "  -v get voltage\n");
  fprintf(stderr, "  -i <index>  Index of the device to use (default: 0)\n");
  fprintf(stderr, "  -w <wavelen>  Wavelength to set (valid values are 1550 1310 850)\n");
  fprintf(stderr, "  -p <floats>  Floats to set (eg.  1.1,2.2,3.3,4.4)\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "\n");
}

static bool is_root() {
  return getuid() == 0;
}

static void print_device_list(opm_device_t *devs, size_t cnt) {
  int i;
  for(i = 0; i < cnt; i++) {
    printf("%d: Manufacturer %s, Product %s, Serial No, %s\n", i, devs[i].manufacturer, devs[i].product, devs[i].serial_number);
  }
}

typedef enum {
  ACTION_NONE,
  ACTION_SET_PARA,
  ACTION_GET_PARA,
  ACTION_GET_OPT_PWR,
  ACTION_GET_VOL,
} action_t;


static float valid_floats[] = {1550, 1310, 850};
static bool wavelen_invalid(int wavelen) {
  int i;
  for(i = 0; i < sizeof(valid_floats) / sizeof(float); i++) {
    if(wavelen == valid_floats[i]) {
      return false;
    }
  }
  return true;
}

static void parse_floats(char *floats, float *f1, float *f2, float *f3, float *f4) {
  char *p = strtok(floats, ",");
  *f1 = atof(p);
  p = strtok(NULL, ",");
  *f2 = atof(p);
  p = strtok(NULL, ",");
  *f3 = atof(p);
  p = strtok(NULL, ",");
  *f4 = atof(p);
}


int main(int argc, char** argv)
{
  int wavelen = 1550;
  int index = 0;
  char floats[256];
  memset(floats, 0, 256);
  action_t action = ACTION_NONE;

  if(!is_root()) {
    fprintf(stderr, "\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "You must be root to run this program.\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "\n");
    exit(1);
  }

  opm_device_t *devs;
  int i, r;
  r = open_opm_devices(&devs);

  char c;
  while ((c = getopt (argc, argv, "li:w:p:vsgho")) != -1) {
    switch (c)
    {
      case 'l':
        print_device_list(devs, r);
        break;
      case 'i':
        index = atoi(optarg);
        break;
      case 'w':
        wavelen = atoi(optarg);
        break;
      case 'p':
        strncpy(floats, optarg, MIN(strlen(optarg), 127));
        floats[MIN(strlen(optarg), 127)] = '\0';
        break;
      case 's':
        action = ACTION_SET_PARA;
        break;
      case 'g':
        action = ACTION_GET_PARA;
        break;
      case 'v':
        action = ACTION_GET_VOL;
        break;
      case 'o':
        action = ACTION_GET_OPT_PWR;
        break;
      case 'h':
        print_usage();
        return 0;
      case '?':
        print_usage();
        return 1;
      default:
        abort ();
    }
  }

  if(action == ACTION_NONE) {
    print_usage();
    return 1;
  }

  if(index >= r || index == -1) {
    fprintf(stderr, "\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "Invalid index %d\n", index);
    fprintf(stderr, "\n");
    fprintf(stderr, "\n");
    return 1;
  }

  if(action == ACTION_GET_VOL) {
    float vol;
    vol = get_pd_vol(devs + index);
    printf("Voltage of index %d is %f\n", index, vol);
    return 0;
  }

  if(action == ACTION_GET_OPT_PWR) {
    if(wavelen_invalid(wavelen)) {
      fprintf(stderr, "\n");
      fprintf(stderr, "\n");
      fprintf(stderr, "Invalid wavelength %d\n", wavelen);
      fprintf(stderr, "\n");
      fprintf(stderr, "\n");
      return 1;
    }

    float opt_pwr;
    switch (wavelen) {
      case 1550:
        opt_pwr = get_l1550_opt_pwr(devs + index);
        break;
      case 1310:
        opt_pwr = get_l1310_opt_pwr(devs + index);
        break;
      case 850:
        opt_pwr = get_l850_opt_pwr(devs + index);
        break;
      default:
        return 1;
    }

    printf("Opt Power for index %d is %f\n", index, opt_pwr);
  }

  if(action == ACTION_GET_PARA) {
    if(wavelen_invalid(wavelen)) {
      fprintf(stderr, "\n");
      fprintf(stderr, "\n");
      fprintf(stderr, "Invalid wavelength %d\n", wavelen);
      fprintf(stderr, "\n");
      fprintf(stderr, "\n");
      return 1;
    }

    float params[4];
    switch (wavelen) {
      case 1550:
        get_l1550_para(devs + index, params);
        break;
      case 1310:
        get_l1310_para(devs + index, params);
        break;
      case 850:
        get_l850_para(devs + index, params);
        break;
      default:
        fprintf(stderr, "\n");
        fprintf(stderr, "\n");
        fprintf(stderr, "Invalid wavelength %d\n", wavelen);
        fprintf(stderr, "\n");
        fprintf(stderr, "\n");
        return 1;
    }

    printf("%f %f %f %f\n", params[0], params[1], params[2], params[3]);

    return 0;
  }

  if(action == ACTION_SET_PARA) {
    if(wavelen_invalid(wavelen)) {
      fprintf(stderr, "\n");
      fprintf(stderr, "\n");
      fprintf(stderr, "Invalid wavelength %d\n", wavelen);
      fprintf(stderr, "\n");
      fprintf(stderr, "\n");
      return 1;
    }

    if(strlen(floats) == 0) {
      fprintf(stderr, "\n");
      fprintf(stderr, "\n");
      fprintf(stderr, "Invalid params %s for -p\n", floats);
      fprintf(stderr, "\n");
      fprintf(stderr, "\n");
      return 1;
    }

    float params[4];
    parse_floats(floats, 
                 &params[0] ,
                 &params[1] ,
                 &params[2] ,
                 &params[3]);
    switch (wavelen) {
      case 1550:
        set_l1550_para(devs + index, params);
        break;
      case 1310:
        set_l1310_para(devs + index, params);
        break;
      case 850:
        set_l850_para(devs + index, params);
        break;
      default:
        return 1;
    }


    printf("set params for index %d\n", index);
    for(int i=0; i < 4; i ++) {
      printf("%f ", params[i]);
    }
    printf("\n");

    return 0;
  }




  close_opm_devices(&devs, r);
}

