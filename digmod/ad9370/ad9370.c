/**************************
* Created by:     XMAN
* Created date:   2016-06-06
* Version:        V1.0
* Descriptions:   9370 config
**************************/
/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "helios.h"
#include "ad9370.h"
#include "t_mykonos.h"
#include "mykonos.h"
#include "ad9370_config.h"
#include "mykonos_macros.h"
#include "device.h"
#include "log.h"


#define AD9370_ARMCODE "datafile/Mykonos_M3.bin"

u8 ReadFile(const u8* fileName, u32 maxLen, u8* pData, u32* len)
{
  	long fd;

  	if(NULL ==pData)
  		return (RET_FAIL);
  	
  	if ((fd = open (fileName, O_RDONLY, 0)) == ERROR)
  	{

		RLDEBUG("Error: can't open file %s\n", fileName);
      	return (RET_FAIL);
  	}

	*(u32*)len = read(fd, (char *)pData, maxLen);

  	close (fd);
  	
  	if(*(u32*)len <= maxLen)
  	{
  		return (RET_SUCC);
  	}
  	else
  	{
		RLDEBUG("Error: read file %s\n", fileName);
		return (RET_FAIL);
  	}
}


void Ad9370Reset(void )
{
	RLDEBUG("Reset AD9370 .......\n");
   	DevFpgaWrite(AD9370_RST_REG, 1);
	usleep(100);
       DevFpgaWrite(AD9370_RST_REG, 0);
	usleep(100);
       DevFpgaWrite(AD9370_RST_REG, 1);
}

u8 Ad9370SpiWrite( u16 addr, u8 data)
{
	DevAd9370Write(addr, data);

    return RET_SUCC;
}

u8 Ad9370SpiRead(u16 addr)
{
	return (DevAd9370Read(addr));		

}

void ATTSet(u8 chn, f32 att)
{
	u16 reg,lereg;
				
	if((chn>=2) ||(att<0.0||att>31.5))
	{	
		RLDEBUG("input para error!\n");
		return ;
	}

	reg = (0==chn)?CHN1_ATT_REG:CHN2_ATT_REG;
	lereg = (0==chn)?CHN1_ATT_LE_REG:CHN2_ATT_LE_REG;
	DevFpgaWrite(lereg, 0);
	DevFpgaWrite(reg, (u16) (att*2));
	DevFpgaWrite(lereg, 1);
	usleep(1);
	DevFpgaWrite(lereg, 0);
	
}

u8 ad9370_config(void)
{
	u8 mcsStatus = 0;
	u8 pllLockStatus = 0;
	u8 binary[98304] = {0}; /*** < contains ARM binary file as array - set by user > ***/
	u32 count = sizeof(binary);
	u8 errorFlag = 0;
	u8 errorCode = 0;
	u32 initCalsCompleted = 0;
	u16 errorWord = 0;
	u16 statusWord = 0;
	u8 status = 0;
	mykonosInitCalStatus_t initCalStatus = {0};
	//u8 i;
	//u8 gainIndex;
	//mykonosObsRxChannels_t obs_rx_source = 5;
	//u8 ADC_XBAR;
	//u16 obsRxDecPower_mdBFS;

	u16 ullo,dllo;
	u8 temp1,temp2,temp3;
	u8 retVal;
	u32 len_armcode;
	u8 deframerStatus = 0;
	u8 obsFramerStatus = 0;
	u8 framerStatus = 0;
	u32 initCalMask = TX_BB_FILTER | ADC_TUNER | TIA_3DB_CORNER | DC_OFFSET |
	TX_ATTENUATION_DELAY | RX_GAIN_DELAY | FLASH_CAL |
	PATH_DELAY | TX_LO_LEAKAGE_INTERNAL | TX_QEC_INIT |
	LOOPBACK_RX_LO_DELAY | LOOPBACK_RX_RX_QEC_INIT |
	RX_LO_DELAY | RX_QEC_INIT;

	u32 trackingCalMask = 	TRACK_RX1_QEC |
	TRACK_RX2_QEC |
	TRACK_TX1_QEC |
	TRACK_TX2_QEC |
	TRACK_ORX1_QEC|
	TRACK_ORX2_QEC;

	mykonosErr_t mykError = MYKONOS_ERR_OK;
	RLDEBUG("Start to config Ad9370 ......\n");
	DevFpgaWrite(0xc0, 0);

	DevFpgaWrite(0x60, 0x00);
	DevFpgaWrite(0x60, 0x03);
	usleep(100);
	DevFpgaWrite(0x60, 0x00);

	DevFpgaWrite(0xc5, 0x01);
	DevFpgaWrite(0xc6, 0x01);
	DevFpgaWrite(0xc7, 0x01);
	usleep(100);
	DevFpgaWrite(0xc5, 0x0);
	DevFpgaWrite(0xc6, 0x0);
	DevFpgaWrite(0xc7, 0x0);

	/**********************************************************/
	/**********************************************************/
	/************ Mykonos Initialization Sequence *************/
	/**********************************************************/
	/**********************************************************/

	/*** < Insert User System Clock(s) Initialization Code Here > ***/

	/*** < Insert User BBIC JESD204B Initialization Code Here > ***/

	/*******************************/
	/****��ʼ�� ***/
	/*******************************/
	RLDEBUG("Ad9370: Start to rese Device......\n");
	mykError = MYKONOS_resetDevice(&mykDevice);

	/* MYKONOS_initialize() loads the Mykonos device data structure
	 * settings for the Rx/Tx/ORx/Sniffer profiles, digital
	 * filter enables, calibrates the CLKPLL, and loads the user provided Rx
	 * gain tables.
	 */
	RLDEBUG("Ad9370: Start to initialize Device......\n");

	mykError = MYKONOS_initialize(&mykDevice);
	if (mykError != MYKONOS_ERR_OK)
	{
		/*** < User log error return value to debug why init failed > ***/
		RLDEBUG("MYKONOS_initialize failed!\n");
	}
	


	/*******************************/
	/***** CLKPLL Status Check *****/
	/*******************************/
	RLDEBUG("Ad9370: Start to Check CLKPLL Status ......\n");
	mykError = MYKONOS_checkPllsLockStatus(&mykDevice, &pllLockStatus);

	if (pllLockStatus & 0x01)
	{
		/*** < CLKPLL locked - user code here > ***/
		RLDEBUG("CLKPLL is locked\n");
	}
	else
	{
		/*** < CLKPLL not locked - ensure lock before proceeding - user code here > ***/
		RLDEBUG("CLKPLL is unlocked\n");
	}
      
	/*******************************************************/
	/**** Perform MultiChip Sync (MCS) on Mykonos Device ***/
	/*******************************************************/
	RLDEBUG("Ad9370: Start to enable Multichip Sync ......\n");
	mykError = MYKONOS_enableMultichipSync(&mykDevice, 1, &mcsStatus);

	/*** < Request minimum 3 SYSREF pulses from Clock Device - user code here > ***/

	/*******************/
	/**** Verify MCS ***/
	/*******************/
	RLDEBUG("Ad9370: Start to unenable Multichip Sync ......\n");
	mykError = MYKONOS_enableMultichipSync(&mykDevice, 0, &mcsStatus);
	if ((mcsStatus & 0x0B) == 0x0B)
	{
		/*** < MCS successful - user code here > ***/
		RLDEBUG("MCS  set success!\n");
	}
	else
	{
		/*** < MCS failed - ensure MCS before proceeding - user code here > ***/
		RLDEBUG("MCS set fail!\n");
	}

	/*************************/
	/**** Load  ARM ***/
	/*************************/
	RLDEBUG("Ad9370: Start to Load  ARM ......\n");
	if (pllLockStatus & 0x01)
	{
		mykError = MYKONOS_initArm(&mykDevice);
             RLDEBUG("start to load arm code DONE\n");

		/*** < user code here: user must load ARM binary byte array into variable binary[98304] before calling next command > ***/
		retVal = ReadFile(AD9370_ARMCODE, 98304, binary, &len_armcode);
		if(retVal !=0 )
		{
			RLDEBUG("Get arm code error!\n");
			return RET_FAIL;
		}

		mykError = MYKONOS_loadArmFromBinary(&mykDevice, &binary[0], count);
		if (mykError != MYKONOS_ERR_OK)
		{
			/*** < ARM did not load properly - check binary and settings - user code here > ***/
			RLDEBUG(" Load  ARM  code  failed!\n");

		}
		else
		{
			RLDEBUG("Load  ARM  code   successful!\n");
		}

	}
	else
	{
		/*** < check settings for proper CLKPLL lock - user code here > ***/
	}

	//DevFpgaWrite(0x60, 1);
	//usleep(100);
	//DevFpgaWrite(0x60, 0);
	

	/*******************************/
	/****���� PLL  ***/
	/*******************************/
	RLDEBUG("Ad9370: Start to set  PLL ......\n");
	//ullo = DevEepromWordRead(EEPROM_FREQ_FREQSTART);//TODO:
	if(0xffff != ullo)
	{
		mykDevice.rx->rxPllLoFrequency_Hz = (u64)ullo*1000*1000;
	}
	//dllo = DevEepromWordRead(EEPROM_FREQ_FREQSTOP);//TODO:
	if(0xffff != dllo)
	{
		mykDevice.tx->txPllLoFrequency_Hz = (u64)dllo*1000*1000;
	}
	RLDEBUG("mykDevice.rx->rxPllLoFrequency_Hz==%llu\n", mykDevice.rx->rxPllLoFrequency_Hz);
	RLDEBUG("mykDevice.tx->txPllLoFrequency_Hz==%llu\n", mykDevice.tx->txPllLoFrequency_Hz);
	mykError = MYKONOS_setRfPllFrequency(&mykDevice, RX_PLL, mykDevice.rx->rxPllLoFrequency_Hz);
	mykError = MYKONOS_setRfPllFrequency(&mykDevice, TX_PLL, mykDevice.tx->txPllLoFrequency_Hz);
	mykError = MYKONOS_setRfPllFrequency(&mykDevice, SNIFFER_PLL, mykDevice.obsRx->snifferPllLoFrequency_Hz);

	/*** ��ʱ 200ms �ȴ� PLL ���� ***/
	usleep(200*1000);

	RLDEBUG("Ad9370: Start to check PLL Lock Statu......\n");
	mykError = MYKONOS_checkPllsLockStatus(&mykDevice, &pllLockStatus);
	if ((pllLockStatus & 0x0F) == 0x0F)
	{
		/*** < All PLLs locked - user code here > ***/
		RLDEBUG("All PLLs locked\n");
	}
	else
	{
		/*** < PLLs not locked - ensure lock before proceeding - user code here > ***/
		RLDEBUG("All PLLs not locked\n");

	}
	


	/*****************************************************/
	/*** Mykonos ARM Initialization Calibrations       ***/
	/*****************************************************/
	RLDEBUG("Ad9370: Mykonos ARM Initialization Calibrations......\n");
	mykError = MYKONOS_runInitCals(&mykDevice, initCalMask);
	mykError = MYKONOS_waitInitCals(&mykDevice, 60000, &errorFlag, &errorCode);

	if ((errorFlag != 0) || (errorCode != 0)) 
	{                     
		RLDEBUG("initial cals error!\n");
		mykError = MYKONOS_getInitCalStatus(&mykDevice, &initCalStatus);
		if(mykError)
		{
			/*** < User log error return value to debug why init failed > ***/
		}
         
		//abort init calibrations      
		mykError = MYKONOS_abortInitCals(&mykDevice, &initCalsCompleted);
		if(mykError)
		{
			/*** < User log error return value to debug why init failed > ***/

		}
		if(initCalsCompleted)
		{
			/*** < which calls had completed, per the mask > ***/ 
		}
		
		mykError = MYKONOS_readArmCmdStatus(&mykDevice, &errorWord, &statusWord);
		if(mykError)
		{
			/*** < User log error return value to debug why init failed > ***/
		}

		mykError = MYKONOS_readArmCmdStatusByte(&mykDevice, 2, &status);
		if(status!=0)
		{
			/*** < Arm Mailbox Status Error errorWord > ***/
			/*** < Pending Flag per opcode statusWord, this follows the mask > ***/
		}
            
		if(mykError)
		{
			/*** < User log error return value to debug why init failed > ***/
		}
	}
	else
	{
		RLDEBUG("initial cals successfully!\n");
		/*** < Calibrations completed successfully - user code here > ***/
	}
	
	/*************************************************/
	/**** Enable SYSREF to Mykonos JESD204B Framers ***/
	/*************************************************/
	/*** < User: Make sure SYSREF is stopped/disabled > ***/
	/*** < User: make sure BBIC JESD is reset and ready to recieve CGS chars> ***/
	
	
	RLDEBUG("Ad9370: Enable SYSREF to Mykonos JESD204B Framers......\n");

	DevFpgaWrite(0x60, 0x02);	
	usleep(100);
	DevFpgaWrite(0x60, 0x00);	
	
	DevFpgaWrite(0xc5, 0x01);
	DevFpgaWrite(0xc7, 0x01);
		
	mykError = MYKONOS_enableSysrefToRxFramer(&mykDevice, 1);
	if (mykError)
	{
		/*** < Read error message to determine what went wrong  - user code here > ***/
	}
	/*** < User: Mykonos is actively transmitting CGS from the RxFramer> ***/

	DevFpgaWrite(0xc5, 0x0);
	DevFpgaWrite(0xc7, 0x0);

	mykError = MYKONOS_enableSysrefToObsRxFramer(&mykDevice, 1);
	if (mykError)
	{
		/*** < Read error message to determine what went wrong  - user code here > ***/
	}
	/*** < User: Mykonos is actively transmitting CGS from the ObsRxFramer> ***/

	DevFpgaWrite(0x60, 0x01);	
	usleep(100);
	DevFpgaWrite(0xc6, 0x01);
	DevFpgaWrite(0x60, 0x00);


	/***************************************************/
	/**** Enable SYSREF to Mykonos JESD204B Deframer ***/
	/***************************************************/
	/*** < User: Make sure SYSREF is stopped/disabled > ***/
	RLDEBUG("Ad9370: Enable SYSREF to Mykonos JESD204B Deframer......\n");
	mykError = MYKONOS_enableSysrefToDeframer(&mykDevice, 0);
	mykError = MYKONOS_resetDeframer(&mykDevice);

	/*** < User: make sure BBIC JESD framer is actively transmitting CGS> ***/
	DevFpgaWrite(0xc6, 0x00);
	
	mykError = MYKONOS_enableSysrefToDeframer(&mykDevice, 1);

	/*** < User Sends SYSREF Here > ***/

	/*** <  User: Insert User BBIC JESD Sync Verification Code Here > ***/

	/************************************/
	/**** Check Mykonos Framer Status ***/
	/************************************/
	RLDEBUG("Ad9370: Check Mykonos Framer Status......\n");
	mykError = MYKONOS_readRxFramerStatus(&mykDevice, &framerStatus);
	mykError = MYKONOS_readOrxFramerStatus(&mykDevice, &obsFramerStatus);

	/**************************************/
	/**** Check Mykonos Deframer Status ***/
	/**************************************/
	RLDEBUG("Ad9370: Check Mykonos Deframer Status......\n");
	mykError = MYKONOS_readDeframerStatus(&mykDevice, &deframerStatus);
	/*if((deframerStatus&0x0f) != 0x08)
	{
		RLDEBUG("Ad9370: JESD ERROR PREPARE TO REBOOT......\n");
		system("reboot");
	}*/
	/*** < User: When links have been verified, proceed > ***/

	/* Allow Rx1/2 QEC tracking and Tx1/2 QEC tracking to run when in the radioOn state         */
	/* Tx calibrations will only run if radioOn and the obsRx path is set to OBS_INTERNAL_CALS  */
   	RLDEBUG("Ad9370: Allow Rx1/2 QEC tracking and Tx1/2 QEC tracking to run when in the radioOn state......\n");
	mykError = MYKONOS_enableTrackingCals(&mykDevice, trackingCalMask);

	/* Function to turn radio on, Enables transmitters and receivers */
	/* that were setup during MYKONOS_initialize() */
   	RLDEBUG("Ad9370: Function to turn radio on, Enables transmitters and receivers......\n");
	mykError = MYKONOS_radioOn(&mykDevice);

	/* Allow TxQEC to run when user is not actively using ORx receive path */
   	RLDEBUG("Ad9370: Allow TxQEC to run when user is not actively using ORx receive path......\n");
	mykError = MYKONOS_setObsRxPathSource(&mykDevice, OBS_INTERNALCALS);
	mykError = MYKONOS_setObsRxPathSource(&mykDevice, OBS_RX1_TXLO);

	DevFpgaWrite(0x60,0x03);
	usleep(100);
	DevFpgaWrite(0x60,0x00);
	sleep(2);
	/*DevFpgaWrite(0xc5,0x1);
	DevFpgaWrite(0xc7,0x1);
	usleep(100);
	DevFpgaWrite(0xc5,0x00);
	DevFpgaWrite(0xc7,0x00);
      	DevFpgaWrite(0xc6,0x1);
	usleep(100);
	DevFpgaWrite(0xc6,0x00);*/

	//DevFpgaWrite(0xc0, 1);

	//RX framer states
	Ad9370SpiWrite(0x068,0x1); 
	sleep(1);
	Ad9370SpiWrite(0x068,0x0);
	sleep(1);
	temp1 = Ad9370SpiRead(0x069);
	//ORX framer states
	Ad9370SpiWrite(0xDCF,0x1);
	sleep(1);
	Ad9370SpiWrite(0xDCF,0x0);
	sleep(1);
	temp2 = Ad9370SpiRead(0xDD0);
	//TX deframer states
	Ad9370SpiWrite(0x08A,0x1);
	sleep(1);
	Ad9370SpiWrite(0x08A,0x0);
	sleep(1);
	temp3 = Ad9370SpiRead(0x08B);
	if((0x3e!= (0x3e&temp1))||(0x3e!= (0x3e&temp2))||(0x28!= (0x28&temp3)))
	{
		RLDEBUG("ad9370 reg value 0x069:0x%x,0xDD0:0x%x,0x08A:0x%x.\n", temp1, temp2, temp3);
		DevFpgaWrite(0x60,0x3);
		usleep(100);
		DevFpgaWrite(0x60,0x0);
		sleep(1);

		//RX framer states
		Ad9370SpiWrite(0x068,0x1); 
		sleep(1);
		Ad9370SpiWrite(0x068,0x0);
		sleep(1);
		temp1 = Ad9370SpiRead(0x069);
		//ORX framer states
		Ad9370SpiWrite(0xDCF,0x1);
		sleep(1);
		Ad9370SpiWrite(0xDCF,0x0);
		sleep(1);
		temp2 = Ad9370SpiRead(0xDD0);
		//TX deframer states
		Ad9370SpiWrite(0x08A,0x1);
		sleep(1);
		Ad9370SpiWrite(0x08A,0x0);
		sleep(1);
		temp3 = Ad9370SpiRead(0x08B);
		RLDEBUG("ad9370 reg value 0x069:0x%x,0xDD0:0x%x,0x08A:0x%x.\n", temp1, temp2, temp3);
	} 
	

	return RET_SUCC;
}


//AD9370״̬���
void bspAD9370StateCheck(void)
{
	mykonosErr_t mykError = MYKONOS_ERR_OK;
	u8 deframerStatus = 0;

	mykError = MYKONOS_readDeframerStatus(&mykDevice, &deframerStatus);
	if((deframerStatus&0x0f) != 0x08)
	{
		RLDEBUG("------(bspAD9370StateCheck)deframerStatus = 0x%x \n", deframerStatus);
		RLDEBUG("bspAD9370StateCheck: JESD ERROR PREPARE TO REBOOT......\n");
		system("reboot");
	}

}

