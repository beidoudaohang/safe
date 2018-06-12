/**
 * \file common_api.h
 * \brief Contains type definitions and prototype declarations for common.c
 */

#ifndef _COMMON_AD9370_H_
#define _COMMON_AD9370_H_

#ifdef __cplusplus
extern "C" {
#endif

/* build project settings include the path to the desired platform folder for correct includes */
// #include "stdint.h"
#include "helios.h"

#define SPIARRAYSIZE 1024

/* assuming 3 byte SPI message - integer math enforces floor() */
#define SPIARRAYTRIPSIZE ((SPIARRAYSIZE / 3) * 3)

/*========================================
 * Enums and structures
 *=======================================*/
/* proposed to increase number of error return values to make unique for each return */
typedef enum
{
    ADIERR_OK=0,
    ADIERR_INV_PARM,
    ADIERR_FAILED
} ADI_ERR;

/*!< \brief COMMON layer error reporting enumerated types */
typedef enum
{
    COMMONERR_OK=0,
    COMMONERR_FAILED
} commonErr_t;

/* bit 0 is MESSAGE, bit 1 is WARNING, bit 2 is ERROR */
typedef enum
{
    ADIHAL_LOG_NONE    = 0x0,
    ADIHAL_LOG_MESSAGE = 0x1,
    ADIHAL_LOG_WARNING = 0x2,
    ADIHAL_LOG_ERROR   = 0x4,
	ADIHAL_LOG_SPI     = 0x8,
	ADIHAL_LOG_AXI_REG = 0x10,
	ADIHAL_LOG_AXI_MEM = 0x20,
	ADIHAL_LOG_ALL     = 0x3F
} ADI_LOGLEVEL;

/**
 * \brief Data structure to hold SPI settings for all system device types
 */
typedef struct
{
	u8 chipSelectIndex;        ///< valid 1~8
	u8 writeBitPolarity;       ///< the level of the write bit of a SPI write instruction word, value is inverted for SPI read operation
	u8 longInstructionWord;    ///< 1 = 16bit instruction word, 0 = 8bit instruction word
	u8 MSBFirst;               ///< 1 = MSBFirst, 0 = LSBFirst
	u8 CPHA;                   ///< clock phase, sets which clock edge the data updates (valid 0 or 1)
	u8 CPOL;                   ///< clock polarity 0 = clock starts low, 1 = clock starts high
    u8 enSpiStreaming;         ///< Not implemented. SW feature to improve SPI throughput.
    u8 autoIncAddrUp;          ///< Not implemented. For SPI Streaming, set address increment direction. 1= next addr = addr+1, 0:addr = addr-1
    u8 fourWireMode;           ///< 1: Use 4-wire SPI, 0: 3-wire SPI (SDIO pin is bidirectional). NOTE: ADI's FPGA platform always uses 4-wire mode.
} spiSettings_t;

/* global variable so application layer can set the log level */
extern ADI_LOGLEVEL CMB_LOGLEVEL;

/* close hardware pointers */
commonErr_t CMB_closeHardware(void);

/* GPIO function */
commonErr_t CMB_setGPIO(u32 GPIO);

/* hardware reset function */
commonErr_t CMB_hardReset(u8 spiChipSelectIndex);

/* SPI read/write functions */
commonErr_t CMB_setSPIOptions(spiSettings_t *spiSettings); /* allows the platform HAL to work with devices with various SPI settings */
commonErr_t CMB_setSPIChannel(u16 chipSelectIndex );  /* value of 0 deasserts all chip selects */
commonErr_t CMB_SPIWriteByte(spiSettings_t *spiSettings, u16 addr, u8 data); /* single SPI byte write function */
commonErr_t CMB_SPIWriteBytes(spiSettings_t *spiSettings, u16 *addr, u8 *data, u32 count);
commonErr_t CMB_SPIReadByte (spiSettings_t *spiSettings, u16 addr, u8 *readdata); /* single SPI byte read function */
commonErr_t CMB_SPIWriteField(spiSettings_t *spiSettings, u16 addr, u8  field_val, u8 mask, u8 start_bit); /* write a field in a single register */
commonErr_t CMB_SPIReadField (spiSettings_t *spiSettings, u16 addr, u8 *field_val, u8 mask, u8 start_bit);	/* read a field in a single register */

/* platform timer functions */
commonErr_t CMB_wait_ms(u32 time_ms);
commonErr_t CMB_wait_us(u32 time_us);
commonErr_t CMB_setTimeout_ms(u32 timeOut_ms);
commonErr_t CMB_setTimeout_us(u32 timeOut_us);
commonErr_t CMB_hasTimeoutExpired();

/* platform logging functions */
commonErr_t CMB_openLog(const char *filename);
commonErr_t CMB_closeLog(void);
commonErr_t CMB_writeToLog(ADI_LOGLEVEL level, u8 deviceIndex, u32 errorCode, const char *comment);
commonErr_t CMB_flushLog(void);

/* platform FPGA AXI register read/write functions */
commonErr_t CMB_regRead(u32 offset, u32 *data);
commonErr_t CMB_regWrite(u32 offset, u32 data);

/* platform DDR3 memory read/write functions */
commonErr_t CMB_memRead(u32 offset, u32 *data, u32 len);
commonErr_t CMB_memWrite(u32 offset, u32 *data, u32 len);

#ifdef __cplusplus
}
#endif
#endif

