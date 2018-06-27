/**************************
* Created by:     XMAN
* Created date:   2016-06-06
* Version:        V1.0
* Descriptions:   device init
**************************/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include "helios.h"
#include "device.h"
#include "common_ad9370.h" 
#include "log.h"

static s32 g_iFpgaDevFd;
static u8 g_bExternalDevValue;

s32 FpgaDevOpen(void) 
{
	s32 fd;
	
	fd = open(FPGA_DEV_NAME, O_RDWR, S_IRUSR | S_IWUSR);
	if(fd < 0) 
	{
		RLDEBUG("unable to open /dev/fpga, exit!\n");
		return (FALSE);
	}	
	
	return fd;
}

void GetKoVersion(s8* koVerBuf)
{
	u32 arg[10] = {0,0,0,0,0,0,0,0,0,0};
	s8 data[10];
	
	ioctl(g_iFpgaDevFd, IOCTL_GET_VERSION, (unsigned long*)arg);
	strncpy(koVerBuf, (char*)arg, 10);
}

void DevFpgaWrite(u16 usAddr, u16 uData)
{
	long fd;
	u32 arg[2] = {0,0};
	
	arg[0] = usAddr;
	arg[1] = uData;
	//if(0xb7!=usAddr)
	//printf("fpga write:addr=0x%x,data=0x%x\n",usAddr,uData);
	ioctl(g_iFpgaDevFd, IOCTL_FPGA_WRITE, (u32*)arg);
}

u16 DevFpgaRead(u16 usAddr)
{
	long fd;
	u32 arg[2] = {0,0};
	
	arg[0] = usAddr;
	ioctl(g_iFpgaDevFd, IOCTL_FPGA_READ, (u32*)arg);
	return (arg[0]);		
}

u32 FpgaDataLoad(u32 len, u8* pData)
{
	u32 uRet = 0; 
	uRet = write(g_iFpgaDevFd, (void*)pData, len);
	return (uRet);
}

void DevFpgaBitMaskSet(u16 usAddr, u16 uOffset)
{
	long fd;
	u32 arg[3] = {0,0,0};
	
	arg[0] = usAddr;
	arg[1] = uOffset;
	ioctl(g_iFpgaDevFd, IOCTL_FPGA_SET_BITMASK, (u32*)arg);		
}

void DevFpgaBitMaskClear(u16 usAddr, u16 uOffset)
{
	long fd;
	u32 arg[3] = {0,0,0};
	
	arg[0] = usAddr;
	arg[1] = uOffset;
	ioctl(g_iFpgaDevFd, IOCTL_FPGA_CLS_BITMASK, (u32*)arg);
}

void DevGpioOutputSet(u32 pin, s32 value)
{
	u32  arg[2] = {0,0};
	u16  data;
	
	arg[0] = pin;
	arg[1] = value;
	ioctl(g_iFpgaDevFd, IOCTL_SET_GPIO_OUTPUT, (unsigned long*)arg);
}

void DevGpioValueSet(u32 pin, s32 value)
{
	u32  arg[2] = {0,0};
	u16  data;
	
	arg[0] = pin;
	arg[1] = value;
	ioctl(g_iFpgaDevFd, IOCTL_SET_GPIO_VALUE, (unsigned long*)arg);
}

u32 DevGpioValueGet(unsigned int pin)
{
	u32  arg[2] = {0,0};
	u16  data;
	
	arg[0] = pin;
	ioctl(g_iFpgaDevFd, IOCTL_GET_GPIO_VALUE, (unsigned long*)arg);
	data = (u32)arg[0];
	//RLDEBUG("get_gpio_value(%d) = %d\n", pin, data);
	return data;	
}

/**************************
* Function: InputHandler  
* Input:   
* Output:        
* return:
* Descriptions: ???????ж??????????????????ò??fpga???????ж?  
**************************/
void InputHandler(s32 signum)
{
	u8 uIntFlag = 0;

	uIntFlag = DevFpgaRead(0x15C);
	//RLDEBUG("uIntFlag=%d\n",uIntFlag);
	if(uIntFlag&0x02)
	{
		g_bExternalDevValue = 1;
	}
	else if(0 == uIntFlag&0x02)
	{
		g_bExternalDevValue = 0;
	}
}

u8 DevThunderAlarmGet(void)
{
	u8 data;
	data=(DevFpgaRead(0x15C)&0x02);
	if(data)
	{
		g_bExternalDevValue=1;
	}
	else if(0 == data)
	{
		g_bExternalDevValue=0;
	}

	//RLDEBUG("DevThunder alarm status:%d.(0:ok)\n",g_bExternalDevValue);
	return g_bExternalDevValue;
}

/* 
void ExternalDevInit(void)
{
	long oflags;
	u8 mac0[100];

	if(-1!=g_iFpgaDevFd)
	{
		signal(SIGIO, InputHandler);
		fcntl(g_iFpgaDevFd, F_SETOWN, getpid());
		oflags = fcntl(g_iFpgaDevFd, F_GETFL);
		fcntl(g_iFpgaDevFd, F_SETFL, oflags | FASYNC);
	}
	else
	{
		RLDEBUG("\r\nfpga open failure!");
	}
 
} */

void DevFeedDog(u8 uVal)
{
	u32 arg[3] = {0,0,0};
	arg[0] = uVal;
	
	ioctl(g_iFpgaDevFd, IOCTL_FEED_DOG, (u32*)arg);
}

u32 DevEepromByteWrite(u16 uAddr, u8 uData)
{
	u32 arg[3] = {0,0,0};
	
	arg[0] = uAddr;
	arg[1] = uData;

	ioctl(g_iFpgaDevFd, IOCTL_EEPROM_WRITE, (u32*)arg);
	return(arg[0]);
}

u8 DevEepromByteRead(u16 uAddr)
{
	u32 arg[3] = {0,0,0};
	u8 data;
	
	arg[0] = uAddr;

	ioctl(g_iFpgaDevFd, IOCTL_EEPROM_READ, (u32*)arg);
	data = (u8)arg[0];
	
	return (data);
}

u32 DevEepromWordWrite(u16 usAddr, u16 usData)
{
	u8 uTmpDat1;
    u8 uTmpDat2;
	u32 retVal;
	
	uTmpDat1 = (u8)usData;
	uTmpDat2 = (usData>>8);
	retVal = DevEepromByteWrite(usAddr,uTmpDat1);
	retVal = DevEepromByteWrite(usAddr+1,uTmpDat2);	
	RLDEBUG("\neeprom write(%d) = %d\n", usAddr, usData);
	
	return retVal;
}

u16 DevEepromWordRead(u16 usAddr)
{
	u16 data;
	
	data = DevEepromByteRead(usAddr);
	data |= DevEepromByteRead(usAddr+1)<<8;

	RLDEBUG("\neeprom read(%d) = %d\n", usAddr, data);
	
	return (data);
}

void DevAD9528Write(u16 addr, u32 data, u8 width)
{
	u32 arg[3] = {0};
	
	arg[0] = addr;
	arg[1] = data;
	arg[2] = width;

	ioctl(g_iFpgaDevFd, IOCTL_AD9528_WRITE, (u32*)arg);

}

u32 DevAD9528Read(u16 addr, u8 width)
{
	u32 arg[3] = {0};
	
	arg[0] = addr;
	arg[1] = width;
	
	ioctl(g_iFpgaDevFd, IOCTL_AD9528_READ, (u32*)arg);
	return (arg[0]);		

}


void DevAd9370Write(u16 addr, u8 data)
{
	u32 arg[2] = {0};
	
	
	arg[0] = addr;
	arg[1] = data;
	

	ioctl(g_iFpgaDevFd, IOCTL_AD9370_WRITE, (u32*)arg);

}

u8 DevAd9370Read(u16 addr)
{
	u32 arg[1] = {0};
	u8 data;

	arg[0] = addr;

	
	ioctl(g_iFpgaDevFd, IOCTL_AD9370_READ, (u32*)arg);
	data = (u8)arg[0];
	return (data);		

}


u8 DevInit(void)
{
	g_iFpgaDevFd = FpgaDevOpen();

	//ExternalDevInit();
	
	return RET_SUCC;
}


