#ifndef _CH9326_LIB_H
#define _CH9326_LIB_H
#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************  
* �� �� ��: ch9326_find
* ��������: ̽��ch9326�豸�����豸ǰ�������
* ����˵��: 
* ���ú���: libusb_init(from libusb.a)
*			libusb_get_device_list(from libusb.a)
*			libusb_get_device_descriptor(from libusb.a)
* ȫ�ֱ���: ch9326[]
* ��    ��: ��
* ��    ��: �����ɹ� : �ҵ����豸��(>0)�����֧��16���豸ͬʱ����, ����ʧ�� : 0
* �� �� ��: tech32                         ����: 2011-11
* �� �� ��:                                ����:
* ��    ��: VER 1.0.0.0
***********************************************************************/
extern
unsigned char
ch9326_find(
    void
    );

/*********************************************************************
* �� �� ��: ch9326_open
* ��������: ���豸���������������߳�
* ����˵��:
* ���ú���: libusb_open(from libusb.a)
* ȫ�ֱ���: ch9326[]
* ��    ��: �豸�����ţ�0��Ӧ��1���豸��1��Ӧ��2���豸... n��Ӧ��n+1���豸(n<=16)
* ��    ��: �����ɹ� : ����1, ����ʧ�� : ����0
* �� �� ��: tech32                         ����: 2011-11
* �� �� ��: tech36                         ����: 2017-11
* ��    ��: VER 1.0.0.0
***********************************************************************/
extern
unsigned char
ch9326_open(
    unsigned char index
    );

/*********************************************************************
* �� �� ��: ch9326_close
* ��������: �ر��豸
* ����˵��: 
* ���ú���: libusb_close(from libusb.a)
* ȫ�ֱ���: 
* ��    ��: �豸�����ţ�0��Ӧ��1���豸��1��Ӧ��2���豸... n��Ӧ��n+1
			���豸(n<=16)
* ��    ��: 
			�����ɹ� : 1
			����ʧ�� : 0
* �� �� ��: tech32                         ����: 2011-11
* �� �� ��:                                ����:
* ��    ��: VER 1.0.0.0
***********************************************************************/
extern
unsigned char
ch9326_close(
    unsigned char index
    );

/*********************************************************************
* �� �� ��: ch9326_set
* ��������: ��������ͨѶ��ʽ
* ����˵��: 
* ���ú���: libusb_control_transfer(from libusb.a)
* ȫ�ֱ���: 
* ��    ��: unsigned char index : �豸�����ţ�0��Ӧ��1���豸��1��Ӧ��2���豸... n��Ӧ��n+1���豸(n<=16)
*			unsigned char rate : 01 = 300bps, 02 = 600bps, 03 = 1200bps, 04 = 2400bps, 05 = 4800bps, 06 = 9600(default)bps,
*								 07 = 14400bps, 08 = 19200bps, 09 = 28800bps, 10 = 38400bps, 11 = 57600bps, 12 = 76800bps,
*								 13 = 115200bps
*			unsigned char check : 01 = odd, 02 = even, 03 = space, 04 = none(default)
*			unsigned char stop_bits : 01 = 1bit stop bit(default)
*									  02 = 2bit stop bit
*			unsigned char data_bits : 01 = 5bit data bit, 02 = 6bit data bit, 03 = 7bit data bit, 04 = 8bit data bit(default)
*			unsigned char interval :	0x10 = 3ms(default), 0x20 = 6ms, 0x30 = 9ms
* ��    ��: �����ɹ� : ����1, ����ʧ�� : ����0
* �� �� ��: tech32                         ����: 2011-11
* �� �� ��:                                ����:
* ��    ��: VER 1.0.0.0
***********************************************************************/
extern
unsigned char
ch9326_set(
	unsigned char index,
	unsigned char rate,
	unsigned char check,
	unsigned char stop_bits,
	unsigned char data_bits,
	unsigned char interval
	);

/*********************************************************************
* �� �� ��: ch9326_send
* ��������: ��������
* ����˵��: 
* ���ú���: libusb_interrupt_transfer(from libusb.a)
* ȫ�ֱ���: 
* ��    ��: unsigned char index : �豸�����ţ�0��Ӧ��1���豸��1��Ӧ��2���豸... n��Ӧ��n+1���豸(n<=16)
			unsigned char *data : �������ݻ�����		
			unsigned long length : �������ݳ���
* ��    ��: �����ɹ� : ���ط��͵����ݳ���, ����ʧ�� : ����0
* �� �� ��: tech32                         ����: 2011-11
* �� �� ��:                                ����:
* ��    ��: VER 1.0.0.0
***********************************************************************/
extern
unsigned long
ch9326_send(
	unsigned char index,
	unsigned char *data,
	unsigned long length
	);

/*********************************************************************
* �� �� ��: ch9326_recv
* ��������: ��������
* ����˵��: 
* ���ú���: 
* ȫ�ֱ���: 
* ��    ��: unsigned char index : �豸�����ţ�0��Ӧ��1���豸��1��Ӧ��2���豸... n��Ӧ��n+1���豸(n<=16)
			unsigned char *data : �������ݻ�����
			unsigned long length : �������ݳ���
* ��    ��: �����ɹ� : ���ط��͵����ݳ���, ����ʧ�� : ����0
* �� �� ��: tech32                         ����: 2011-11
* �� �� ��:                                ����:
* ��    ��: VER 1.0.0.0
***********************************************************************/
extern
unsigned long
ch9326_recv(
	unsigned char index,
    char *data,
	unsigned long length
	);

/*********************************************************************
* �� �� ��: ch9326_set_gpiodata
* ��������:
* ����˵��:
* ���ú���: libusb_control_transfer(from libusb.a)
* ȫ�ֱ���:
* ��    ��: unsigned char index : �豸�����ţ�0��Ӧ��1���豸��1��Ӧ��2���豸... n��Ӧ��n+1���豸(n<=16)
*			unsigned char data : λ0��������IO1��ƽ��0Ϊ����͵�ƽ��1Ϊ����ߵ�ƽ
*								 λ1��������IO2��ƽ��0Ϊ����͵�ƽ��1Ϊ����ߵ�ƽ
* ��    ��:
            �����ɹ� : 1
            ����ʧ�� : 0
* �� �� ��: tech32                         ����: 2011-11
* �� �� ��: tech33                         ����: 2014-11
* ��    ��: VER 1.0.0.0
***********************************************************************/
extern unsigned char
ch9326_set_gpiodata(
    unsigned char index,
    unsigned char data
    );

/*********************************************************************
* �� �� ��: ch9326_set_gpiodir
* ��������:
* ����˵��:
* ���ú���: libusb_control_transfer(from libusb.a)
* ȫ�ֱ���:
* ��    ��: unsigned char index : �豸�����ţ�0��Ӧ��1���豸��1��Ӧ��2���豸... n��Ӧ��n+1���豸(n<=16)
*			unsigned char dir : λ0��������IO1����0Ϊ���롢1Ϊ���
*						 		λ1��������IO2����0Ϊ���롢1Ϊ���
* ��    ��:
            �����ɹ� : 1
            ����ʧ�� : 0
* �� �� ��: tech32                         ����: 2011-11
* �� �� ��: tech33                         ����: 2014-11
* ��    ��: VER 1.0.0.0
***********************************************************************/
extern unsigned char
ch9326_set_gpiodir(
    unsigned char index,
    unsigned char dir
    );

/*********************************************************************
* �� �� ��: ch9326_set_gpio
* ��������:
* ����˵��:
* ���ú���: libusb_control_transfer(from libusb.a)
* ȫ�ֱ���:
* ��    ��: unsigned char index : �豸�����ţ�0��Ӧ��1���豸��1��Ӧ��2���豸... n��Ӧ��n+1���豸(n<=16)
*			unsigned char dir : λ0��������IO1����0Ϊ���롢1Ϊ���
*						 		λ1��������IO2����0Ϊ���롢1Ϊ���
*			unsigned char data : λ0��������IO1��ƽ��0Ϊ����͵�ƽ��1Ϊ����ߵ�ƽ
*								 λ1��������IO2��ƽ��0Ϊ����͵�ƽ��1Ϊ����ߵ�ƽ
* ��    ��:
            �����ɹ� : 1
            ����ʧ�� : 0
* �� �� ��: tech32                         ����: 2011-11
* �� �� ��:                                ����:
* ��    ��: VER 1.0.0.0
***********************************************************************/
extern unsigned char
ch9326_set_gpio(
    unsigned char index,
    unsigned char dir,
    unsigned char data
    );

/*********************************************************************
* �� �� ��: ch9326_get_gpio
* ��������:
* ����˵��:
* ���ú���: libusb_control_transfer(from libusb.a)
* ȫ�ֱ���:
* ��    ��: unsigned char index : �豸�����ţ�0��Ӧ��1���豸��1��Ӧ��2���豸... n��Ӧ��n+1���豸(n<=16)
*			unsigned char data : λ5��ʾIO1�������ƽ״̬��Ϊ1���ʾ�ߵ�ƽ��Ϊ0��ʾ�͵�ƽ
                                 λ3��ʾIO2�������ƽ״̬��Ϊ1���ʾ�ߵ�ƽ��Ϊ0��ʾ�͵�ƽ
* ��    ��:
            �����ɹ� : 1
            ����ʧ�� : 0
* �� �� ��: tech32                         ����: 2011-11
* �� �� ��:                                ����:
* ��    ��: VER 1.0.0.0
***********************************************************************/
extern unsigned char
ch9326_get_gpio(
    unsigned char index,
    char *data
    );

/*********************************************************************
* �� �� ��: ch9326_connected
* ��������: ��������
* ����˵��:
* ���ú���:
* ȫ�ֱ���:
* ��    ��: unsigned char index : �豸�����ţ�0��Ӧ��1���豸��1��Ӧ��2���豸... n��Ӧ��n+1���豸(n<=16)
* ��    ��: �����ɹ� : ����1, ����ʧ�� : ����0
* �� �� ��: tech33                        ����: 2014-11
* �� �� ��:                                      ����:
* ��    ��: VER 1.0.0.0
***********************************************************************/
extern unsigned char ch9326_connected(unsigned char index);

/*********************************************************************
* �� �� ��: ch9326_get_manufacturer_string
* ��������: ��ȡ�����ַ���������
* ����˵��: 
* ���ú���: libusb_get_string_descriptor
* ȫ�ֱ���: 
* ��    ��: unsigned char index : �豸�����ţ�0��Ӧ��1���豸��1��Ӧ��2���豸... n��Ӧ��n+1���豸(n<=16)
			unsigned char *data : ���ݻ������׵�ַ
			unsigned long length : ��������С
* ��    ��: �����ɹ� : ����1, ����ʧ�� : ����0
* �� �� ��: tech36                         ����: 2017-5
* �� �� ��:                                      ����:
* ��    ��: VER 1.0.0.0
***********************************************************************/
extern unsigned char
ch9326_get_manufacturer_string(
	unsigned char index,
	unsigned char *data,
	unsigned long length
	);

/*********************************************************************
* �� �� ��: ch9326_get_product_string
* ��������: ��ȡ��Ʒ�ַ���������
* ����˵��: 
* ���ú���: libusb_get_string_descriptor
* ȫ�ֱ���: 
* ��    ��: unsigned char index : �豸�����ţ�0��Ӧ��1���豸��1��Ӧ��2���豸... n��Ӧ��n+1���豸(n<=16)
			unsigned char *data : ���ݻ������׵�ַ
			unsigned long length : ��������С
* ��    ��: �����ɹ� : ����1, ����ʧ�� : ����0
* �� �� ��: tech36                         ����: 2017-5
* �� �� ��:                                      ����:
* ��    ��: VER 1.0.0.0
***********************************************************************/
extern unsigned char
ch9326_get_product_string(
	unsigned char index,
	unsigned char *data,
	unsigned long length
	);

/*********************************************************************
* �� �� ��: ch9326_get_serial_number_string
* ��������: ��ȡ���к��ַ���������
* ����˵��: 
* ���ú���: libusb_get_string_descriptor
* ȫ�ֱ���: 
* ��    ��: unsigned char index : �豸�����ţ�0��Ӧ��1���豸��1��Ӧ��2���豸... n��Ӧ��n+1���豸(n<=16)
			unsigned char *data : ���ݻ������׵�ַ
			unsigned long length : ��������С
* ��    ��: �����ɹ� : ����1, ����ʧ�� : ����0
* �� �� ��: tech36                         ����: 2017-5
* �� �� ��:                                      ����:
* ��    ��: VER 1.0.0.0
***********************************************************************/
extern unsigned char
ch9326_get_serial_number_string(
	unsigned char index,
	unsigned char *data,
	unsigned long length
	);

#ifdef __cplusplus
}
#endif

#endif // _CH9326_LIB_H
