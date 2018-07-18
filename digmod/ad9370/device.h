/**************************
* Created by:     XMAN
* Created date:   2016-06-06
* Version:        V1.0
* Descriptions:   
**************************/
#ifndef  _DEVICE_H
#define  _DEVICE_H
                   
#include "helios.h"

#define FPGA_DEV_NAME "/dev/fpga"

enum FPGA_IOCTL_FUNCTION{
	/*basic ioctl*/
	EMTPY,
	IOCTL_SET_GPIO_OUTPUT,
	IOCTL_SET_GPIO_VALUE,
	IOCTL_GET_GPIO_VALUE,
	IOCTL_GET_VERSION,
	IOCTL_FPGA_WRITE,
	IOCTL_FPGA_READ,
	IOCTL_FPGA_SET_BITMASK,
	IOCTL_FPGA_CLS_BITMASK,
	IOCTL_FEED_DOG,
	IOCTL_EEPROM_WRITE,
	IOCTL_EEPROM_READ,
	IOCTL_AD9370_WRITE,
	IOCTL_AD9370_READ,
	IOCTL_AD9528_WRITE,
	IOCTL_AD9528_READ
};

u8 DevInit(void);
s32 FpgaDevOpen(void);
void GetKoVersion(s8* koVerBuf);
void DevFpgaWrite(u16 usAddr, u16 uData);
u16 DevFpgaRead(u16 usAddr);
u32 FpgaDataLoad(u32 len, u8* pData);
void DevFeedDog(u8 uVal);
u32 DevEepromByteWrite(u16 uAddr, u8 uData);
u8 DevEepromByteRead(u16 uAddr);
u32 DevEepromWordWrite(u16 usAddr, u16 usData);
u16 DevEepromWordRead(u16 usAddr);
void DevFpgaBitMaskSet(u16 usAddr, u16 uOffset);
void DevFpgaBitMaskClear(u16 usAddr, u16 uOffset);
void DevGpioOutputSet(u32 pin, s32 value);
void DevGpioInputSet(u32 pin, s32 value);
u32 DevGpioValueGet(unsigned int pin);
u8 DevThunderAlarmGet(void);
void DevAd9370Write(u16 addr, u8 data);
u8 DevAd9370Read(u16 addr);

#endif

