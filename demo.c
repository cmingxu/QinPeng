#include <stdio.h>
#include <stdlib.h>
#include <ch9326.h>
#include <string.h>
#include <unistd.h>

void main()
{
	char data;
	char buf[128];
	int i;

	memset(buf, 0, sizeof(buf));

	int r = ch9326_find();
	printf("found %d 9326\n", r);

	r = ch9326_open(0);
	printf("open : %d\n", r);

	r = ch9326_set(0, 13, 4, 1, 4, 30);
	printf("set : %d\n", r);

	r = ch9326_set_gpiodata(0, 0);
	printf("set gpio data: %d\n", r);

	r = ch9326_set_gpiodir(0, 0);
	printf("set gpio dir: %d\n", r);

	r = ch9326_set_gpio(0, 0, 0);
	printf("set gpio: %d\n", r);

	r = ch9326_get_gpio(0, &data);
	printf("get gpio: %d\n", r);

	r = ch9326_connected(0);
	printf("connected : %d\n", r);

	r = ch9326_get_manufacturer_string(0, buf, 128);
	if (r == 0)
		printf("get manu error\n");
	else if (r == 1) {
		for (i = 0; i < 128; i++)
			printf("%x ", buf[i]);

    printf("%s\n", buf);
		printf("\n");
	}

	r = ch9326_get_product_string(0, buf, 128);
	if (r == 0)
		printf("get prod error\n");
	else if (r == 1) {
		for (i = 0; i < 128; i++)
			printf("%x ", buf[i]);
		printf("\n");
	}

	r = ch9326_get_serial_number_string(0, buf, 128);
	if (r == 0)
		printf("get serial error\n");
	else if (r == 1) {
		for (i = 0; i < 128; i++)
			printf("%x ", buf[i]);
		printf("\n");
	}

	for (i = 0; i < 20; i++) {
		r = ch9326_send(0, buf, 128);
		printf("send : %d\n", r);

		sleep(1);

		r = ch9326_recv(0, buf, 128);
		printf("recv : %d \n", r);
	}

	r = ch9326_close(0);
	printf("close : %d\n", r);
	return;
}

