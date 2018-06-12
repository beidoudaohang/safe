/**************************
* Created by:     XMAN
* Created date:   2016-06-06
* Version:        V1.0
* Descriptions:  
**************************/
#include "helios.h"
#include "ad9528.h"
#include "device.h"
#include "log.h"


static const AD9528_REG_STRUC ad9528_conf[] =
{
	{0x000,0x99,1},
	{0x00f,0x01,1},
	{0x001,0x00,1},
	{0x100,0x1E,1},
	{0x101,0x00,1},
	{0x102,0x96,1},
	{0x103,0x00,1},
	{0x104,0x78,1},
	{0x105,0x00,1},
	{0x106,0x1A,1},
	{0x107,0x03,1},
	{0x108,0x59,1},
	{0x109,0x04,1},
	{0x10a,0x0A,1},
	{0x10b,0x00,1},
	{0x200,0xE6,1},
	{0x201,0x87,1},
	{0x202,0x03,1},
	{0x203,0x04,1},
	{0x00f,0x01,1},
	{0x203,0x05,1},
	{0x00f,0x01,1},
	{0x204,0x13,1},
	{0x205,0x2A,1},
	{0x206,0x00,1},
	{0x207,0x01,1},
	{0x208,0x09,1},
	{0x209,0x00,1},
	{0x300,0x20,1},
	{0x301,0x80,1},
	{0x302,0x04,1},
	{0x303,0x40,1},
	{0x304,0x00,1},
	{0x305,0x00,1},
	{0x306,0x00,1},
	{0x307,0x80,1},
	{0x308,0x04,1},
	{0x309,0x40,1},
	{0x30a,0x00,1},
	{0x30b,0x00,1},
	{0x30c,0x00,1},
	{0x30d,0x80,1},
	{0x30e,0x04,1},
	{0x30f,0x40,1},
	{0x310,0x00,1},
	{0x311,0x00,1},
	{0x312,0x00,1},
	{0x313,0x80,1},
	{0x314,0x04,1},
	{0x315,0x40,1},
	{0x316,0x00,1},
	{0x317,0x00,1},
	{0x318,0x40,1},
	{0x319,0x00,1},
	{0x31a,0x04,1},
	{0x31b,0x00,1},
	{0x31c,0x00,1},
	{0x31d,0x09,1},
	{0x31e,0x00,1},
	{0x31f,0x00,1},
	{0x320,0x09,1},
	{0x321,0x00,1},
	{0x322,0x00,1},
	{0x323,0x09,1},
	{0x324,0x40,1},
	{0x325,0x00,1},
	{0x326,0x00,1},
	{0x327,0x00,1},
	{0x328,0x00,1},
	{0x329,0x07,1},
	{0x32a,0x00,1},
	{0x32b,0x00,1},
	{0x32c,0x00,1},
	{0x32d,0x00,1},
	{0x32e,0x00,1},
	{0x400,0x64,1},
	{0x401,0x00,1},
	{0x402,0x00,1},
	{0x403,0x90,1},
	{0x404,0x04,1},
	{0x500,0x10,1},
	{0x501,0x00,1},
	{0x502,0x00,1},
	{0x503,0xFF,1},
	{0x504,0xFF,1},
	{0x505,0x02,1},
	{0x506,0x03,1},
	{0x507,0x0C,1},
	{0x508,0xE4,1},
	{0x509,0x00,1},
	{0xffe,0x00,1},
	{0x00f,0x01,1},
//	{0x203,0x04,1},
//	{0x00f,0x01,1},
//	{0x203,0x05,1},
//	{0x00f,0x01,1},
	{0x32a,0x00,1},
	{0x00f,0x01,1},
	{0x32a,0x01,1},
	{0x00f,0x01,1},
	{0x32a,0x00,1},
	{0x00f,0x01,1},
	{0x403,0x91,1},
	{0x00f,0x01,1},

};


u8 bspAd9528Reset(void)
{
    RLDEBUG("Reset AD9528...... \n");
    //  在reset引脚产生一个1ms的低电平脉冲
    DevFpgaWrite(AD9528_RST_REG, 1);
    usleep(10); 
    DevFpgaWrite(AD9528_RST_REG, 0);
    usleep(1000);
    DevFpgaWrite(AD9528_RST_REG, 1);
    usleep(1000);

    return (RET_SUCC);
}

u8 bspAd9528Write ( u16 addr, u32 data, u8 width)
{

	DevAD9528Write(addr, data, width);

	return (RET_SUCC);
}


u32 bspAd9528Read ( u16 addr,  u8 width)
{
	return (DevAD9528Read(addr, width));
}

/*
  Manual Sync All Outputs
    a. Write REG0x32A=0x01, Write REG0x00F=0x01
    b. Write REG0x32A=0x00, Write REG0x00F=0x01
*/
u8 bspAd9528SynSet(void)
{
    bspAd9528Write(0x32a, 0x01, 1);
    bspAd9528Write(0x00f, 0x01, 1);

    bspAd9528Write(0x32a, 0x00, 1);
    bspAd9528Write(0x00f, 0x01, 1);
    return RET_SUCC;
}

/******************************************************************
AD9528 Initialization Sequence:
    1. Load registers from REG0x00 to REG0x509 in AD9528evb_software_generated_configuration.stp file
    2. Do VCO Calibration
        a. Write REG0x203.bit0=0, Write REG0x00F=0x01
        b. Write REG0x203.bit0=1, Write REG0x00F=0x01
    3. Wait and check PLL2 locked
        a. Write REG0x00F=0x01
        b. Check REG0x508.bit1=1?
        c. If yes, continue step 4
        d. If no, wait and go to 3.a.
    4. Manual Sync All Outputs
        a. Write REG0x32A=0x01, Write REG0x00F=0x01
        b. Write REG0x32A=0x00, Write REG0x00F=0x01
input:    u8  VSW_BYTE refSelect, u32 refFrqKhz, u32 vcxoFrqKhz
output:    none
return:    BSP_OK, BSP_ERR_HARDWARE_VERSION, BSP_ERR_PARAMETER_INVALID,
        BSP_ERR_CLK_NOT_LOCK
description:
config ad952x.
******************************************************************/
u8 ad9528_config (void)
{
	u8 typeflag;
    u32 ad9528_data = 0x0;
    int i;
    int temp = 0;
    u32 timeout = 0;
    RLDEBUG("Start to config AD9528 ......\n");

    bspAd9528Reset();   /*Ad9528 Reset*/

    RLDEBUG("config AD9528...... \n");
    /*
      1. Load registers from REG0x00 to REG0x509 in
        AD9528evb_software_generated_configuration.stp file
    */
    temp = sizeof(ad9528_conf)/sizeof(AD9528_REG_STRUC);
    for(i=0; i<temp ; i++)
    {
        bspAd9528Write(ad9528_conf[i].reg, ad9528_conf[i].val, ad9528_conf[i].width);
        //DEBUG("bspAd9528Write(%03X, %02x, %d)", ad9528_conf[i].reg,ad9528_conf[i].val, ad9528_conf[i].width);
    }
#if (0)
    for(i=0; i<temp ; i++)
        RLDEBUG("bspAd9528Read(%03X)=%02x\n", ad9528_conf[i].reg,bspAd9528Read(ad9528_conf[i].reg, ad9528_conf[i].width));

    // 2.Do VCO Calibration
    // a. Write REG0x203.bit0=0,  Write REG0x00F=0x01
    // b. Write REG0x203.bit0=1,  Write REG0x00F=0x01
    ad9528_data = bspAd9528Read(0x203, 1);
    ad9528_data &= ~0x01;
    //ad9528_data = 0x10;
    bspAd9528Write(0x203, ad9528_data, 1);
    bspAd9528Write(0x00f, 0x01, 1); //update

    ad9528_data |= 0x01;
    bspAd9528Write(0x203, ad9528_data, 1);
    bspAd9528Write(0x00f, 0x01, 1);  //update


    //再写一次，0.6144MHz 才有输出
    bspAd9528Write(0x403, 0x91, 1);
    bspAd9528Write(0x00f, 0x01, 1); //update
#endif
    // 3.Wait and check PLL2 locked
    // a. Write REG0x00F=0x01
    // b. Check REG0x508.bit1=1?
    // c. If yes, continue step 4
    // d. If no, wait and go to 3.a.
    do{
        bspAd9528Write(0x00f, 0x01, 1);
        temp = bspAd9528Read(0x508, 1);  // & 0x02
        usleep(100);
    }while(!(temp & 0x02) && (timeout++ < 100));  // || (temp == 0xff)

    RLDEBUG("bspAd9528Read(0x508)=0x%02x\n", temp);


    if(!(temp & 0x02) && (timeout >= 100))
    {
        RLDEBUG("Fail!!.\n\n");
        return RET_FAIL;
    }

	RLDEBUG("bspAd9528Read(0x403)=0x%02x\n",bspAd9528Read(0x403, 1));
    // 4. Manual Sync All Outputs
    // a. Write REG0x32A=0x01, Write REG0x00F=0x01
    // b. Write REG0x32A=0x00, Write REG0x00F=0x01
    //bspAd9528SynSet();

    //return bspAd9528LockStat();    /*锁定 or 失锁*/

    RLDEBUG("Done.\n");
	typeflag = DevEepromByteRead(EEPROM_ADDR_MAINOPT_FLAG);

	if(1==typeflag)
	{
		bspAd9528Write(0x10a, 0xb, 1);
		bspAd9528Write(0x0f, 0x1, 1);
	}
    return RET_SUCC;
}


u32 bspAd9528ReadPrint (  u16 addr, u8 width)
{
	u32      retVal;

	retVal = bspAd9528Read (addr, width); //&ad9528_data,

	RLDEBUG("Read AD9528 addr = 0x%x, data = 0x%x, width = %d.\n", addr, retVal, width);
	return retVal;
}




