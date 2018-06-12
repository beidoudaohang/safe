/**
 * \file common.c
 * \brief Contains Mykonos API common wrapper functions for user hardware platform drivers
 */

#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include "common_ad9370.h" 
#include "device.h"
#include "log.h"


static u8 _writeBitPolarity = 1;
static u8 _longInstructionWord = 1;
static u8 _chipSelectIndex = 0;

ADI_LOGLEVEL CMB_LOGLEVEL = ADIHAL_LOG_NONE;




extern void Ad9370Reset(void);
extern u8 Ad9370SpiWrite( u16 addr, u8 data);
extern u8 Ad9370SpiRead(u16 addr);

commonErr_t CMB_closeHardware(void)
{
    //HAL_closeSpi();
   // HAL_closeLogFile();

    return(COMMONERR_OK);
}

commonErr_t CMB_setGPIO(u32 GPIO)
{
    return(COMMONERR_OK);
}

commonErr_t CMB_hardReset(u8 spiChipSelectIndex)
{

    //if(1 == spiChipSelectIndex)
	{
		Ad9370Reset();
	}
    /*else if(2 == spiChipSelectIndex)
	{
		Ad9528Reset();
	}*/
    return(COMMONERR_OK);
}

commonErr_t CMB_setSPIOptions(spiSettings_t *spiSettings)
{
    long error = 0;
    _writeBitPolarity = spiSettings->writeBitPolarity & 0x01;
    _longInstructionWord = spiSettings->longInstructionWord & 0x01;

    //error = HAL_initSpi((spiSettings->chipSelectIndex), (u8)((spiSettings->CPOL <<1) | (spiSettings->CPHA)));

    if(error < 0)
    {
        return(COMMONERR_FAILED);
    }

    return(COMMONERR_OK);
}

commonErr_t CMB_setSPIChannel(u16 chipSelectIndex )
{
    /* 0 = all chip selects de-asserted */
    _chipSelectIndex = (u8)chipSelectIndex;

    return(COMMONERR_OK);
}

commonErr_t CMB_SPIWriteByte(spiSettings_t *spiSettings, u16 addr, u8 data)
{
	long retval = 0;
	u8 Temp1 =0;
	u8 txbuf[] = {0x00,0x00,0x00};

/*
    if (_chipSelectIndex != spiSettings->chipSelectIndex)
    {
        if(CMB_setSPIOptions(spiSettings))
        {
            return(COMMONERR_FAILED);
        }

        if(CMB_setSPIChannel(spiSettings->chipSelectIndex))
        {
            return(COMMONERR_FAILED);
        }
    }

    if(CMB_LOGLEVEL & LOG_SPI)
    {
        RLDEBUG("SPIWrite: CS:%2d, ADDR:0x%03X, DATA:0x%02X \n", spiSettings->chipSelectIndex,addr, data);
    }
*/
	Ad9370SpiWrite(addr, data);

    return(COMMONERR_OK);
}

commonErr_t CMB_SPIWriteBytes(spiSettings_t *spiSettings, u16 *addr, u8 *data, u32 count)
{
	u8 usData = 0;
	u16 usAddr =0;
	u32 i = 0;
	long retval = 0;

/*
    if (_chipSelectIndex != spiSettings->chipSelectIndex)
    {
        if(CMB_setSPIOptions(spiSettings))
        {
            return(COMMONERR_FAILED);
        }

        if(CMB_setSPIChannel(spiSettings->chipSelectIndex))
        {
            return(COMMONERR_FAILED);
        }
    }
*/
        for (i = 0; i < count; i++)
        {
		usAddr = addr[i];
		usData = data[i];
		Ad9370SpiWrite(usAddr, usData);	
    	}

    return(COMMONERR_OK);
}

commonErr_t CMB_SPIReadByte(spiSettings_t *spiSettings, u16 addr, u8 *readdata)
{
    u8 data=0;
    long retval = 0;
	u8 TEMP_value =0; //zzw
	
    u8 txbuf[] = {0x00,0x00,0x00};

/*
    if(_chipSelectIndex != spiSettings->chipSelectIndex)
    {
        if(CMB_setSPIOptions(spiSettings))
        {
            return(COMMONERR_FAILED);
        }
        if(CMB_setSPIChannel(spiSettings->chipSelectIndex))
        {
            return(COMMONERR_FAILED);
        }
    }
	*/
	
   *readdata = Ad9370SpiRead(addr);
	
    if(CMB_LOGLEVEL & ADIHAL_LOG_SPI)
    {
        RLDEBUG("SPIRead: CS:%2d, ADDR:0x%03X, ReadData:0x%02X\n", spiSettings->chipSelectIndex, addr, *readdata);
    }

    return(COMMONERR_OK);
}

commonErr_t CMB_SPIWriteField(spiSettings_t *spiSettings, u16 addr, u8 field_val, VSW_BYTE mask, VSW_BYTE start_bit)
{
    u8 Val=0;

    if(CMB_LOGLEVEL & ADIHAL_LOG_SPI)
    {
        RLDEBUG("SPIWriteField: CS:%2d, ADDR:0x%03X, FIELDVAL:0x%02X, MASK:0x%02X, STARTBIT:%d \n", spiSettings->chipSelectIndex,addr, field_val, mask, start_bit);
    }
    if(CMB_SPIReadByte(spiSettings, addr, &Val))
    {
        return(COMMONERR_FAILED);
    }
    Val = (Val & ~mask) | ((field_val << start_bit) & mask);
    if(CMB_SPIWriteByte(spiSettings, addr, Val))
    {
        return(COMMONERR_FAILED);
    }

    return(COMMONERR_OK);
}

/* read a field in a single register space (not multibyte fields) */
commonErr_t CMB_SPIReadField(spiSettings_t *spiSettings, u16 addr, u8 *field_val, VSW_BYTE mask, VSW_BYTE start_bit)
{
    u8 data;

    if(CMB_SPIReadByte(spiSettings, addr, &data))
    {
        return(COMMONERR_FAILED);
    }
    *field_val =(u8)((data & mask) >> start_bit);

    if(CMB_LOGLEVEL & ADIHAL_LOG_SPI)
    {
        RLDEBUG("SPIReadField: CS:%2d, ADDR:0x%03X, MASK:0x%02X, STARTBIT:%d, FieldVal:0x%02X\n", spiSettings->chipSelectIndex,addr, mask, start_bit, *field_val);
    }

    return(COMMONERR_OK);
}
/*************************************************
��������:CMB_writeToLog()
��������: ��ӡ����
�������: ��
�������:��
��������ֵ:
���� : ���
*************************************************/
commonErr_t CMB_writeToLog(ADI_LOGLEVEL level, u8 deviceIndex, u32 errorCode, const char *comment)
{

	/*
    if((CMB_LOGLEVEL & LOG_ERROR) && (level == LOG_ERROR))
    {
        RLDEBUG("ERROR: %d: %s", (int)errorCode, comment);
    }
    else if((CMB_LOGLEVEL & LOG_WARNING) && (level == LOG_WARNING))
    {
        RLDEBUG("WARNING: %d: %s",(int)errorCode, comment);
    }
    else if((CMB_LOGLEVEL & LOG_MESSAGE) && (level == LOG_MESSAGE))
    {
        RLDEBUG("MESSAGE: %d: %s",(int)errorCode, comment);
    }
    else
    {
        RLDEBUG("Undefined Log Level: %0x\n", level);
    }
	*/

	//printf("Ad9370 congfig value = %d\n", deviceIndex);


	switch(level)
	{
		//������Ϣ
		case ADIHAL_LOG_ERROR:
		{
			RLDEBUG("ERROR: %d: %s", (int)errorCode, comment);
			break;
		}

		//������Ϣ
		case ADIHAL_LOG_WARNING:
		{
			RLDEBUG("WARNING: %d: %s",(int)errorCode, comment);
			break;
		}


		//������Ϣ
		case ADIHAL_LOG_MESSAGE:
		{
			RLDEBUG("MESSAGE: %d: %s",(int)errorCode, comment);
			break;
		}

		default:
		{
 			RLDEBUG("Undefined Log Level: %0x\n", level);
			break;
		}

	}

    return(COMMONERR_OK);
}

/* if filename null, a default path will be used in logging.c */
#if 0
commonErr_t CMB_openLog(const char *filename)
{
    HAL_openLogFile(filename);

    return(COMMONERR_OK);
}
#endif
#if 0
commonErr_t CMB_closeLog(void)
{
    HAL_closeLogFile();

    return(COMMONERR_OK);
}
#endif
#if 0
commonErr_t CMB_flushLog(void)
{
    HAL_flushLogFile();

    return(COMMONERR_OK);
}
#endif
#if 0
commonErr_t CMB_wait_ms(u32 time_ms)
{
    /* nanosleep can be interrupted, if so, call until total time has completed */
    struct timespec t0;
    struct timespec t1;
    struct timespec *temp;
    struct timespec *waitTime = &t0;
    struct timespec *remaining = &t1;

    waitTime->tv_sec = time_ms/1000;
    waitTime->tv_nsec = (time_ms % 1000) * (1000000);

    while((nanosleep(waitTime, remaining) == (-1)) && (errno == EINTR))
    {
        /* if nanosleep returned early due to a interrupt, nanosleep again using remaining time */
        temp = waitTime;
        waitTime = remaining;
        remaining = temp;
    }

    return(COMMONERR_OK);
}
#endif

commonErr_t CMB_wait_us(u32 time_us)
{

        volatile unsigned int i, j;
	for (i = 0; i < time_us; i++)
		for (j = 0; j < 17; j++);
       return(COMMONERR_OK);
}

commonErr_t CMB_wait_ms(u32 time_ms)
{
      volatile unsigned int i, j;
	for (i = 0; i < time_ms; i++)
		for (j = 0; j < 30000; j++);
      return(COMMONERR_OK);
}

commonErr_t CMB_setTimeout_ms(u32 timeOut_ms)
{
	    return(COMMONERR_OK);
           #if 0
	    u8 retval;
           retval = 0;
	     retval= CMB_wait_ms(timeOut_ms);
	    if (retval > 0)
	    {
	        return (COMMONERR_FAILED);
	    }
	    else 
	    {
	        return(COMMONERR_OK);
	    }
	    #endif
		 
}

commonErr_t CMB_setTimeout_us(u32 timeOut_us)
{
	     return(COMMONERR_OK);
#if 0
	    u8 retval;
           retval = 0;
	     retval= CMB_wait_us(timeOut_us);
	    if (retval > 0)
	    {
	        return (COMMONERR_FAILED);
	    }
	    else 
	    {
	        return(COMMONERR_OK);
	    }
#endif 
		 
}

commonErr_t HAL_hasTimeoutExpired()
{
    return(COMMONERR_OK);
    #if 0
    u8 retval;
    retval = 0;

     if (++retval>100)   //��ʱʱ����Ҫ�������Ż�
    {
          return (COMMONERR_FAILED);
    }
    else 
    {
          return(COMMONERR_OK);
    }
	#endif 
}

commonErr_t CMB_hasTimeoutExpired()
{
    return(COMMONERR_OK);
    #if 0
    u8 retval = 0;
    retval = HAL_hasTimeoutExpired();

    if (retval > 0)
    {
        return (COMMONERR_FAILED);
    }
    else 
    {
        return(COMMONERR_OK);
    }
   #endif
}
#if 0
commonErr_t CMB_regRead(u32 offset, u32 *data)
{
    char *uiod = "/dev/uio0";
    const u32 map_size = 0x1000;
    u32 error = 0;

    error = fpgaAxiReadWrite(uiod, map_size, 1, offset, 0, data);

    if(CMB_LOGLEVEL & LOG_AXI_REG)
    {
        RLDEBUG("FPGA Register Read: OFFSET_ADDR:0x%08X, READ_DATA:0x%08X\n", offset, *data);
    }
  
    if (error != 0)
    {
        return (COMMONERR_FAILED);
    }
    else 
    {
        return(COMMONERR_OK);
    }
    
}
#endif
#if 0
commonErr_t CMB_regWrite(u32 offset, u32 data)
{
    char *uiod = "/dev/uio0";
    const u32 map_size = 0x1000;
    u32 error = 0;

    error = fpgaAxiReadWrite(uiod, map_size, 1, offset, 1, &data);

    if(CMB_LOGLEVEL & LOG_AXI_REG)
    {
        RLDEBUG("FPGA Register Write: OFFSET_ADDR:0x%08X, WRITE_DATA:0x%08X\n", offset, data);
    }

    if (error != 0)
    {
        return (COMMONERR_FAILED);
    }
    else 
    {
        return(COMMONERR_OK);
    }
}
#endif

#if 0
commonErr_t CMB_memRead(u32 offset, u32 *data, VSW_USWORD32 len)
{
    char *uiod = "/dev/uio1";
    const u32 map_size = 0x40000000;
    u32 error = 0;

    error = fpgaAxiReadWrite(uiod, map_size, len, offset, 0, data);

    if(CMB_LOGLEVEL & LOG_AXI_MEM)
    {
        RLDEBUG("Memory Read: OFFSET_ADDR:0x%08X, LENGTH:0x%08X\n", offset, len);
    }

    if (error != 0)
    {
        return (COMMONERR_FAILED);
    }
    else 
    {
        return(COMMONERR_OK);
    }
}
#endif
#if 0
commonErr_t CMB_memWrite(u32 offset, u32 *data, VSW_USWORD32 len)
{
    char *uiod = "/dev/uio1";
    const u32 map_size = 0x40000000;
    u32 error = 0;

    error = fpgaAxiReadWrite(uiod, map_size, len, offset, 1, data);

    if(CMB_LOGLEVEL & LOG_AXI_MEM)
    {
        RLDEBUG("Memory Write: OFFSET_ADDR:0x%08X, LENGTH:0x%08X\n", offset, len);
    }

    if (error != 0)
    {
        return (COMMONERR_FAILED);
    }
    else 
    {
        return(COMMONERR_OK);
    }
}
#endif
