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
#include <linux/spi/spidev.h>  
#include <string.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include "helios.h"
#include "ad9370.h"
#include "t_mykonos.h"
#include "mykonos.h"
#include "ad9370_config_A.h"
#include "ad9370_config_B.h"
#include "mykonos_macros.h"
#include "log.h"
#include "ad_dev_api.h"
#include "porting.h"
#include "para_table.h"
#include "dig_band0.h"
// #define AD9370_IO_CONF 

#define FPGA_DELAY 1000*10
#define AD9370_ARMCODE "/datafile/Mykonos_M3.bin"
#define AD9370_CHIP_NUMBER 2
//ad9370 cmd
//ioctl cmd
#define AD9370_IOC_MAGIC 'k' //majic
//cmd
#define AD9370_CMD_RESET _IO(AD9370_IOC_MAGIC, 0x30)

struct ad9370_dev
{
	int index;
	char spidev[64];
	char dev[64];
	int spi_fd;
	int ad_fd;
};

static struct ad_spi_cfg cfg = {
	.speed = 10000000,
	.bits = 24,
	.mode = SPI_CPHA | SPI_CPOL,
	.delay = 1,
};

static struct ad9370_dev ad9370_d[2] = {
	{
		.index = 0,
		.spidev = "/dev/spidev0.0",
		.dev = "/dev/ad9370-ctl.0",
		.spi_fd = 0,
		.ad_fd = 0
	},
	{
		.index = 1,
		.spidev = "/dev/spidev1.0",
		.dev = "/dev/ad9370-ctl.1",
		.spi_fd = 0,
		.ad_fd = 0
	}
};

/*
	name:
		ad9370_dev_open(void)
	description:
		open ad9370 dev
*/
int ad9370_dev_open(void)
{
	int ret;

	RLDEBUG("ad9370 init start\n");

	//ad9370 link a
	if (!strstr(ad9370_d[AD9370_A].spidev, "/dev")) {
		RLDEBUG("ad9370_a spi str false\n");
		return -ERR_NODEV;
	}
	if (!strstr(ad9370_d[AD9370_A].dev, "/dev")){
		RLDEBUG("ad9370_a dev str false\n");
		return -ERR_NODEV;
	}

	ad9370_d[AD9370_A].spi_fd = open(ad9370_d[AD9370_A].spidev, O_RDWR);
	if (ad9370_d[AD9370_A].spi_fd < 0) {
		RLDEBUG("spidev open false\n");
		return -ERR_OPENFALSE;
	}
	ad9370_d[AD9370_A].ad_fd = open(ad9370_d[AD9370_A].dev, O_RDWR);
	if (ad9370_d[AD9370_A].ad_fd < 0) {
		RLDEBUG("ad9370 dev open false\n");
		return -ERR_OPENFALSE;
	}

	//reset chip
	ad9370_dev_reset(AD9370_A);

	//set spi para
	ret  = ioctl(ad9370_d[AD9370_A].spi_fd, SPI_IOC_WR_MODE, &(cfg.mode));
	if (-1 == ret) {
		RLDEBUG("spidev mode set false\n");
	}
	ret  = ioctl(ad9370_d[AD9370_A].spi_fd, SPI_IOC_RD_MODE, &(cfg.mode));
	if (-1 == ret) {
		RLDEBUG("spidev mode set false\n");
	}

	ret  = ioctl(ad9370_d[AD9370_A].spi_fd, SPI_IOC_WR_BITS_PER_WORD, &(cfg.bits));
	if (-1 == ret) {
		RLDEBUG("spidev BITS set false\n");
	}
	ret  = ioctl(ad9370_d[AD9370_A].spi_fd, SPI_IOC_RD_BITS_PER_WORD, &(cfg.bits));
	if (-1 == ret) {
		RLDEBUG("spidev BITS set false\n");
	}

	ret  = ioctl(ad9370_d[AD9370_A].spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &(cfg.speed));
	if (-1 == ret) {
		RLDEBUG("spidev speed set false\n");
	}
	ret  = ioctl(ad9370_d[AD9370_A].spi_fd, SPI_IOC_RD_MAX_SPEED_HZ, &(cfg.speed));
	if (-1 == ret) {
		RLDEBUG("spidev speed set false\n");
	}



	//ad9370 link b
	if (!strstr(ad9370_d[AD9370_B].spidev, "/dev")) {
		RLDEBUG("ad9370_b spi str false\n");
		return -ERR_NODEV;
	}
	if (!strstr(ad9370_d[AD9370_B].dev, "/dev")){
		RLDEBUG("ad9370_b dev str false\n");
		return -ERR_NODEV;
	}

	ad9370_d[AD9370_B].spi_fd = open(ad9370_d[AD9370_B].spidev, O_RDWR);
	if (ad9370_d[AD9370_B].spi_fd < 0) {
		RLDEBUG("ad9370b spidev open false\n");
		return -ERR_OPENFALSE;
	}
	ad9370_d[AD9370_B].ad_fd = open(ad9370_d[AD9370_B].dev, O_RDWR);
	if (ad9370_d[AD9370_B].ad_fd < 0) {
		RLDEBUG("ad9370b dev open false\n");
		return -ERR_OPENFALSE;
	}

	//reset chip
	ad9370_dev_reset(AD9370_B);


	//set spi para
	ret  = ioctl(ad9370_d[AD9370_B].spi_fd, SPI_IOC_WR_MODE, &(cfg.mode));
	if (-1 == ret) {
		RLDEBUG("spidev mode set false\n");
	}
	ret  = ioctl(ad9370_d[AD9370_B].spi_fd, SPI_IOC_RD_MODE, &cfg.mode);
		if (-1 == ret) {
			RLDEBUG("spidev mode set false\n");
		}

	ret  = ioctl(ad9370_d[AD9370_B].spi_fd, SPI_IOC_WR_BITS_PER_WORD, &cfg.bits);
	if (-1 == ret) {
		RLDEBUG("spidev BITS set false\n");
	}
	ret  = ioctl(ad9370_d[AD9370_B].spi_fd, SPI_IOC_RD_BITS_PER_WORD, &cfg.bits);
	if (-1 == ret) {
		RLDEBUG("spidev BITS set false\n");
	}

	ret  = ioctl(ad9370_d[AD9370_B].spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &cfg.speed);
	if (-1 == ret) {
		RLDEBUG("spidev speed set false\n");
	}
	ret  = ioctl(ad9370_d[AD9370_B].spi_fd, SPI_IOC_RD_MAX_SPEED_HZ, &cfg.speed);
	if (-1 == ret) {
		RLDEBUG("spidev speed set false\n");
	}

	//for chip communicate test
/*	ad_dev_write(ad9370_d[AD9370_A].spi_fd,0x0000,0x3c,&cfg);
	ret = ad_dev_read(ad9370_d[AD9370_A].spi_fd,0x0004,&cfg);
	RLDEBUG("ad9370_a init ok,ad9370_A chip id=%x\n",ret);
	
	ad_dev_write(ad9370_d[AD9370_B].spi_fd,0x0000,0x3c,&cfg);
	ret = ad_dev_read(ad9370_d[AD9370_B].spi_fd,0x0004,&cfg);
	RLDEBUG("ad9370_b init ok,ad9370_b chip id=%x\n",ret);	*/
	char dt;
	ad9370_dev_reg_set(AD9370_A,0x0000,0x3c);
	ad9370_dev_reg_read(AD9370_A,0x0004,&dt);
	RLDEBUG("ad9370_a init ok,ad9370_A chip id=%x\n",dt);
	//if(dt == 0xff) return -1;

	ad9370_dev_reg_set(AD9370_B,0x0000,0x3c);
	ad9370_dev_reg_read(AD9370_B,0x0004,&dt);
	RLDEBUG("ad9370_a init ok,ad9370_B chip id=%x\n",dt);
	//if(dt == 0xff) return -1;

	RLDEBUG("ad9370 init ok\n");

	return 0;

}

/*
	name:
		ad9370_dev_close(void)
	description:
		close ad9370 dev
*/
int ad9370_dev_close(void)
{
	//ad9370 link a
	close(ad9370_d[AD9370_A].spi_fd);
	close(ad9370_d[AD9370_A].ad_fd);

	//ad9370 link b
	close(ad9370_d[AD9370_B].spi_fd);
	close(ad9370_d[AD9370_B].ad_fd);

	return 0;
}

/*
	name:
		ad9370_dev_reset(int index)
	para:
		ch--->chip index
	description:
		reset ad9370 dev
*/

int ad9370_dev_reset(int index)
{
	int ret;

	//reset ad9528
	if (AD9370_A == index) {
		ret = ioctl(ad9370_d[AD9370_A].ad_fd, AD9370_CMD_RESET);
		if (ret < 0)
			RLDEBUG("cant reset ad9370");
	} else if (AD9370_B == index) {
		ret = ioctl(ad9370_d[AD9370_B].ad_fd, AD9370_CMD_RESET);
		if (ret < 0)
			RLDEBUG("cant reset ad9370");
	} else {
		-ERR_NODEV;
	}

	return 0;
}

/*
	name:
		int ad9370_dev_reg_set(int reg,char dat)
	para:
		reg --- ad9370 reg addr
		dat --- para
	description:
		set ad9370 reg
*/

int ad9370_dev_reg_set(AD9370_CH ch, int reg, char dat)
{
	int ret = -1;

	//PRINTF_DEBUG("ad9370_set:ch = %d,reg=%4x,dat=%2x \n",ch,reg,dat);

	if ((AD9370_A == ch) || (AD9370_B== ch)) {
		//spi_transfer(ad9370_d[ch].spi_fd, reg, dat);
		ad_dev_write(ad9370_d[ch].spi_fd, reg, dat, &cfg);
		ret = 0;
	}

	return ret;
}

/*
	name:
		int ad9370_dev_reg_read(int reg,char *dat)
	para:
		reg --- ad9370 reg addr
		dat --- para
	description:
		read ad9370 reg
*/

int ad9370_dev_reg_read(AD9370_CH ch, int reg, char *dat)
{
	int ret = -1;

	//PRINTF_DEBUG("ad9370_read:ch = %d,reg=%4x \n",ch,reg);

	if (NULL == dat)
		return ret;

	if ((AD9370_A == ch) || (AD9370_B== ch)) {
		//*dat = spi_transfer(ad9370_d[ch].spi_fd, reg, dat);
		*dat = ad_dev_read(ad9370_d[ch].spi_fd, reg, &cfg);
		ret = 0;
	}
	
	//PRINTF_DEBUG("ad9370_read:ch = %d,reg=%4x ,back dat=%x\n",ch,reg,*dat);

	return ret;

}


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




/* u8 Ad9370SpiWrite( u16 addr, u8 data)
{
	DevAd9370Write(addr, data);

    return RET_SUCC;
}

u8 Ad9370SpiRead(u16 addr)
{
	return (DevAd9370Read(addr));		

}
 */


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
	u16 ullo,dllo;
	// u8 temp1,temp2,temp3;
	u8 retVal;
	u32 len_armcode;
	u8 deframerStatus = 0, obsFramerStatus = 0, framerStatus = 0;
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
	mykonosDevice_t *mykDevice = NULL;
	mykonosDevice_t *mykDevice_AB[AD9370_CHIP_NUMBER] = {&AD9370_Device_A, &AD9370_Device_B};
	u8 i;
	mykonosErr_t mykError = MYKONOS_ERR_OK;
	RLDEBUG("Start to config Ad9370 ......\n");

	if(ad9370_dev_open()) return RET_FAIL;



	for(i=0; i<AD9370_CHIP_NUMBER; ++i){
		mykDevice = mykDevice_AB[i];
		
		#ifdef AD9370_IO_CONF
		DevFpgaWrite(0xc8, i);
		usleep(100);
		DevFpgaWrite(0xc0, 0);
		DevFpgaWrite(0xc0, 1);
		usleep(100);
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
		#else
		DevFpgaWrite(0xc0, 0);
		DevFpgaWrite(0xc0, 1);
		usleep(100);
		DevFpgaWrite(0xc0, 0);

		band_dynamic_para_a[0].ad9370_conf.chip_select = i;
		// band_dynamic_para_a[0].ad9370_conf.reset_fpga = 1;
		band_dynamic_para_a[0].ad9370_conf.gtp_rx = 1;
		band_dynamic_para_a[0].ad9370_conf.gtp_tx = 1;
		band_dynamic_para_a[0].ad9370_conf.jesd204b_fb_rst = 1;
		band_dynamic_para_a[0].ad9370_conf.jesd204b_rx_rst = 1;
		band_dynamic_para_a[0].ad9370_conf.jesd204b_tx_rst = 1;
		band0_set_adr_add(0xf4);
		//usleep(500*1000);
		dig_cmd_set_deal();
		usleep(FPGA_DELAY);

		// band_dynamic_para_a[0].ad9370_conf.reset_fpga = 0;
		band_dynamic_para_a[0].ad9370_conf.gtp_rx = 0;
		band_dynamic_para_a[0].ad9370_conf.gtp_tx = 0;
		band_dynamic_para_a[0].ad9370_conf.jesd204b_fb_rst = 0;
		band_dynamic_para_a[0].ad9370_conf.jesd204b_rx_rst = 0;
		band_dynamic_para_a[0].ad9370_conf.jesd204b_tx_rst = 0;
		band0_set_adr_add(0xf4);
		// usleep(500*1000);
		dig_cmd_set_deal();
		usleep(FPGA_DELAY);
		#endif
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
		mykError = MYKONOS_resetDevice(mykDevice);

		/* MYKONOS_initialize() loads the Mykonos device data structure
		* settings for the Rx/Tx/ORx/Sniffer profiles, digital
		* filter enables, calibrates the CLKPLL, and loads the user provided Rx
		* gain tables.
		*/
		RLDEBUG("Ad9370: Start to initialize Device......\n");

		mykError = MYKONOS_initialize(mykDevice);
		if (mykError != MYKONOS_ERR_OK)
		{
			/*** < User log error return value to debug why init failed > ***/
			RLDEBUG("MYKONOS_initialize failed!\n");
		}
		
		/*******************************/
		/***** CLKPLL Status Check *****/
		/*******************************/
		RLDEBUG("Ad9370: Start to Check CLKPLL Status ......\n");
		mykError = MYKONOS_checkPllsLockStatus(mykDevice, &pllLockStatus);

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
		mykError = MYKONOS_enableMultichipSync(mykDevice, 1, &mcsStatus);

		/*** < Request minimum 3 SYSREF pulses from Clock Device - user code here > ***/

		/*******************/
		/**** Verify MCS ***/
		/*******************/
		RLDEBUG("Ad9370: Start to unenable Multichip Sync ......\n");
		mykError = MYKONOS_enableMultichipSync(mykDevice, 0, &mcsStatus);
		if ((mcsStatus & 0x0B) == 0x0B)
		{
			/*** < MCS successful - user code here > ***/
			RLDEBUG("MCS  set success!\n");
		}
		else
		{
			/*** < MCS failed - ensure MCS before proceeding - user code here > ***/
			RLDEBUG("MCS set fail! mcsStatus=%d\n", mcsStatus);
		}

		/*************************/
		/**** Load  ARM ***/
		/*************************/
		RLDEBUG("Ad9370: Start to Load  ARM ......\n");
		if (pllLockStatus & 0x01)
		{
			mykError = MYKONOS_initArm(mykDevice);
			RLDEBUG("start to load arm code DONE\n");

			/*** < user code here: user must load ARM binary byte array into variable binary[98304] before calling next command > ***/
			retVal = ReadFile(AD9370_ARMCODE, 98304, binary, &len_armcode);
			if(retVal !=0 )
			{
				RLDEBUG("Get arm code error!\n");
				return RET_FAIL;
			}

			mykError = MYKONOS_loadArmFromBinary(mykDevice, &binary[0], count);
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

		/*******************************/
		/****���� PLL  ***/
		/*******************************/
		RLDEBUG("Ad9370: Start to set  PLL ......\n");

/* 		if(i == 0){
			mykDevice->rx->rxPllLoFrequency_Hz = (u64)band_para_a[0].md_sundry.dig_sundry.center_freq.dl*1000*1000;
			mykDevice->tx->txPllLoFrequency_Hz = (u64)band_para_a[0].md_sundry.dig_sundry.center_freq.ul*1000*1000;
		}else{
			mykDevice->rx->rxPllLoFrequency_Hz = (u64)band_para_a[0].md_sundry.dig_sundry.center_freq.ul*1000*1000;
			mykDevice->tx->txPllLoFrequency_Hz = (u64)band_para_a[0].md_sundry.dig_sundry.center_freq.dl*1000*1000;	
		} */


		RLDEBUG("mykDevice->rx->rxPllLoFrequency_Hz==%llu\n", mykDevice->rx->rxPllLoFrequency_Hz);
		RLDEBUG("mykDevice->tx->txPllLoFrequency_Hz==%llu\n", mykDevice->tx->txPllLoFrequency_Hz);
		mykError = MYKONOS_setRfPllFrequency(mykDevice, RX_PLL, mykDevice->rx->rxPllLoFrequency_Hz);
		mykError = MYKONOS_setRfPllFrequency(mykDevice, TX_PLL, mykDevice->tx->txPllLoFrequency_Hz);
		mykError = MYKONOS_setRfPllFrequency(mykDevice, SNIFFER_PLL, mykDevice->obsRx->snifferPllLoFrequency_Hz);

		/*** ��ʱ 200ms �ȴ� PLL ���� ***/
		usleep(200*1000);

		RLDEBUG("Ad9370: Start to check PLL Lock Statu......\n");
		mykError = MYKONOS_checkPllsLockStatus(mykDevice, &pllLockStatus);
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
		mykError = MYKONOS_runInitCals(mykDevice, initCalMask);
		mykError = MYKONOS_waitInitCals(mykDevice, 60000, &errorFlag, &errorCode);

		if ((errorFlag != 0) || (errorCode != 0)) 
		{                     
			RLDEBUG("initial cals error!\n");
			mykError = MYKONOS_getInitCalStatus(mykDevice, &initCalStatus);
			if(mykError)
			{
				/*** < User log error return value to debug why init failed > ***/
			}
			
			//abort init calibrations      
			mykError = MYKONOS_abortInitCals(mykDevice, &initCalsCompleted);
			if(mykError)
			{
				/*** < User log error return value to debug why init failed > ***/

			}
			if(initCalsCompleted)
			{
				/*** < which calls had completed, per the mask > ***/ 
			}
			
			mykError = MYKONOS_readArmCmdStatus(mykDevice, &errorWord, &statusWord);
			if(mykError)
			{
				/*** < User log error return value to debug why init failed > ***/
			}

			mykError = MYKONOS_readArmCmdStatusByte(mykDevice, 2, &status);
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

		#ifdef AD9370_IO_CONF
		DevFpgaWrite(0x60, 0x02);	
		usleep(100);
		DevFpgaWrite(0x60, 0x00);	
		DevFpgaWrite(0xc5, 0x01);
		DevFpgaWrite(0xc7, 0x01);
		#else

		band_dynamic_para_a[0].ad9370_conf.gtp_rx = 1;
		band_dynamic_para_a[0].ad9370_conf.gtp_tx = 1;
		band_dynamic_para_a[0].ad9370_conf.jesd204b_fb_rst = 1;
		band_dynamic_para_a[0].ad9370_conf.jesd204b_rx_rst = 1;
		band0_set_adr_add(0xf4);
		// usleep(500*1000);
		dig_cmd_set_deal();
		usleep(FPGA_DELAY);

		band_dynamic_para_a[0].ad9370_conf.gtp_rx = 0;
		band_dynamic_para_a[0].ad9370_conf.gtp_tx = 0;
		band0_set_adr_add(0xf4);
		// usleep(500*1000);
		dig_cmd_set_deal();
		usleep(FPGA_DELAY);
		#endif


		mykError = MYKONOS_enableSysrefToRxFramer(mykDevice, 1);
		if (mykError)
		{
			/*** < Read error message to determine what went wrong  - user code here > ***/
		}
		/*** < User: Mykonos is actively transmitting CGS from the RxFramer> ***/
		#ifdef AD9370_IO_CONF
		DevFpgaWrite(0xc5, 0x0);
		DevFpgaWrite(0xc7, 0x0);
		#else
		band_dynamic_para_a[0].ad9370_conf.jesd204b_fb_rst = 0;
		band_dynamic_para_a[0].ad9370_conf.jesd204b_rx_rst = 0;
		band0_set_adr_add(0xf4);
		// usleep(500*1000);
		dig_cmd_set_deal();
		usleep(FPGA_DELAY);
		#endif
		mykError = MYKONOS_enableSysrefToObsRxFramer(mykDevice, 1);
		if (mykError)
		{
			/*** < Read error message to determine what went wrong  - user code here > ***/
		}
		/*** < User: Mykonos is actively transmitting CGS from the ObsRxFramer> ***/
		#ifdef AD9370_IO_CONF
		DevFpgaWrite(0x60, 0x01);	
		usleep(100);
		DevFpgaWrite(0xc6, 0x01);
		DevFpgaWrite(0x60, 0x00);
		#else

		band_dynamic_para_a[0].ad9370_conf.gtp_tx = 1;
		band_dynamic_para_a[0].ad9370_conf.jesd204b_tx_rst = 1;
		band0_set_adr_add(0xf4);
		// usleep(500*1000);
		dig_cmd_set_deal();
		usleep(FPGA_DELAY);

		band_dynamic_para_a[0].ad9370_conf.gtp_tx = 0;
		band0_set_adr_add(0xf4);
		// usleep(500*1000);
		dig_cmd_set_deal();
		usleep(FPGA_DELAY);
		#endif

		/***************************************************/
		/**** Enable SYSREF to Mykonos JESD204B Deframer ***/
		/***************************************************/
		/*** < User: Make sure SYSREF is stopped/disabled > ***/
		RLDEBUG("Ad9370: Enable SYSREF to Mykonos JESD204B Deframer......\n");
		mykError = MYKONOS_enableSysrefToDeframer(mykDevice, 0);
		mykError = MYKONOS_resetDeframer(mykDevice);

		/*** < User: make sure BBIC JESD framer is actively transmitting CGS> ***/
		#ifdef AD9370_IO_CONF
		DevFpgaWrite(0xc6, 0x00);
		#else

		band_dynamic_para_a[0].ad9370_conf.jesd204b_tx_rst = 0;
		band0_set_adr_add(0xf4);
		// usleep(500*1000);
		dig_cmd_set_deal();
		usleep(FPGA_DELAY);
		#endif
		mykError = MYKONOS_enableSysrefToDeframer(mykDevice, 1);

		/*** < User Sends SYSREF Here > ***/

		/*** <  User: Insert User BBIC JESD Sync Verification Code Here > ***/

		/************************************/
		/**** Check Mykonos Framer Status ***/
		/************************************/
		RLDEBUG("Ad9370: Check Mykonos Framer Status......\n");
		mykError = MYKONOS_readRxFramerStatus(mykDevice, &framerStatus);
		mykError = MYKONOS_readOrxFramerStatus(mykDevice, &obsFramerStatus);

		/**************************************/
		/**** Check Mykonos Deframer Status ***/
		/**************************************/
		RLDEBUG("Ad9370: Check Mykonos Deframer Status......\n");
		mykError = MYKONOS_readDeframerStatus(mykDevice, &deframerStatus);

		/*** < User: When links have been verified, proceed > ***/

		/* Allow Rx1/2 QEC tracking and Tx1/2 QEC tracking to run when in the radioOn state         */
		/* Tx calibrations will only run if radioOn and the obsRx path is set to OBS_INTERNAL_CALS  */
		RLDEBUG("Ad9370: Allow Rx1/2 QEC tracking and Tx1/2 QEC tracking to run when in the radioOn state......\n");
		mykError = MYKONOS_enableTrackingCals(mykDevice, trackingCalMask);

		/* Function to turn radio on, Enables transmitters and receivers */
		/* that were setup during MYKONOS_initialize() */
		RLDEBUG("Ad9370: Function to turn radio on, Enables transmitters and receivers......\n");
		mykError = MYKONOS_radioOn(mykDevice);

		/* Allow TxQEC to run when user is not actively using ORx receive path */
		RLDEBUG("Ad9370: Allow TxQEC to run when user is not actively using ORx receive path......\n");
		mykError = MYKONOS_setObsRxPathSource(mykDevice, OBS_INTERNALCALS);
		mykError = MYKONOS_setObsRxPathSource(mykDevice, OBS_RX1_TXLO);
		// mykError = MYKONOS_setObsRxPathSource(mykDevice, OBS_RX2_TXLO);

		#ifdef AD9370_IO_CONF
		DevFpgaWrite(0x60,0x03);
		usleep(100);
		DevFpgaWrite(0x60,0x00);
		sleep(2);
		#else
		band_dynamic_para_a[0].ad9370_conf.gtp_rx = 1;
		band_dynamic_para_a[0].ad9370_conf.gtp_tx = 1;
		band0_set_adr_add(0xf4);
		// usleep(500*1000);
		dig_cmd_set_deal();
		usleep(FPGA_DELAY);

		band_dynamic_para_a[0].ad9370_conf.gtp_rx = 0;
		band_dynamic_para_a[0].ad9370_conf.gtp_tx = 0;
		band0_set_adr_add(0xf4);
		dig_cmd_set_deal();
		sleep(2);

		#endif

		//RX framer states
		mykError = MYKONOS_readRxFramerStatus(mykDevice, &framerStatus);
		//ORX framer states
		mykError = MYKONOS_readOrxFramerStatus(mykDevice, &obsFramerStatus);
		//TX deframer states
		mykError = MYKONOS_readDeframerStatus(mykDevice, &deframerStatus);
		RLDEBUG("ad9370 reg value 0x069:0x%x,0xDD0:0x%x,0x08A:0x%x.\n", framerStatus, obsFramerStatus, deframerStatus);
		if ((0x3e != (0x3e & framerStatus)) || (0x3e != (0x3e & obsFramerStatus)) || (0x28 != (0x28 & deframerStatus)))
		{
			#ifdef AD9370_IO_CONF
			DevFpgaWrite(0x60,0x03);
			usleep(100);
			DevFpgaWrite(0x60,0x00);
			sleep(2);
			#else
			band_dynamic_para_a[0].ad9370_conf.gtp_rx = 1;
			band_dynamic_para_a[0].ad9370_conf.gtp_tx = 1;
			band0_set_adr_add(0xf4);
			// usleep(500*1000);
			dig_cmd_set_deal();
			usleep(FPGA_DELAY);

			band_dynamic_para_a[0].ad9370_conf.gtp_rx = 0;
			band_dynamic_para_a[0].ad9370_conf.gtp_tx = 0;
			band0_set_adr_add(0xf4);
			dig_cmd_set_deal();
			sleep(2);
			#endif

			//RX framer states
			mykError = MYKONOS_readRxFramerStatus(mykDevice, &framerStatus);
			//ORX framer states
			mykError = MYKONOS_readOrxFramerStatus(mykDevice, &obsFramerStatus);
			//TX deframer states
			mykError = MYKONOS_readDeframerStatus(mykDevice, &deframerStatus);
			RLDEBUG("ad9370 reg value 0x069:0x%x,0xDD0:0x%x,0x08A:0x%x.\n", framerStatus, obsFramerStatus, deframerStatus);
		}

/* 		mykError = MYKONOS_enableTxNco(mykDevice, 1, 5000, 5000);
		if(mykError)
		{
			RLDEBUG("MYKONOS_enableTxNco  ERROR!\n");
		} */
	}
	ad9370_dev_close();
	return RET_SUCC;
}


//AD9370״̬���
/* void bspAD9370StateCheck(void)
{
	mykonosErr_t mykError = MYKONOS_ERR_OK;
	u8 deframerStatus = 0;

	mykError = MYKONOS_readDeframerStatus(mykDevice, &deframerStatus);
	if((deframerStatus&0x0f) != 0x08)
	{
		RLDEBUG("------(bspAD9370StateCheck)deframerStatus = 0x%x \n", deframerStatus);
		RLDEBUG("bspAD9370StateCheck: JESD ERROR PREPARE TO REBOOT......\n");
		system("reboot");
	}

} */

