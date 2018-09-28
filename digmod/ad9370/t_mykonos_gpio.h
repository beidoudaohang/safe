/*
 * \file t_mykonosGpio.h
 * \brief Mykonos GPIO error handling and MYK_GPIO defines
 */

#ifndef T_MYKONOSGPIO_H_
#define T_MYKONOSGPIO_H_

#ifdef __cplusplus
extern "C" {
#endif


// #include "common.h"

#define MYK_GPIO0    0x1
#define MYK_GPIO1    0x2
#define MYK_GPIO2    0x4
#define MYK_GPIO3    0x8
#define MYK_GPIO4    0x10
#define MYK_GPIO5    0x20
#define MYK_GPIO6    0x40
#define MYK_GPIO7    0x80
#define MYK_GPIO8    0x100
#define MYK_GPIO9    0x200
#define MYK_GPIO10   0x400
#define MYK_GPIO11   0x800
#define MYK_GPIO12   0x1000
#define MYK_GPIO13   0x2000
#define MYK_GPIO14   0x4000
#define MYK_GPIO15   0x8000
#define MYK_GPIO16   0x10000
#define MYK_GPIO17   0x20000
#define MYK_GPIO18   0x40000

/**
 *  \brief Enum of unique error codes from the Mykonos GPIO API functions.
 * Each error condition in the library should get its own enum value to allow
 * easy debug of errors.
 */
typedef enum
{
	MYKONOS_ERR_GPIO_OK = 0,
	MYKONOS_ERR_EN_MONITOR_OUT_NOT_ENABLED,
    MYKONOS_ERR_EN_MONITOR_OUT_SRC_CTRL,
    MYKONOS_ERR_MONITOR_OUT_INDEX_RANGE,
    MYKONOS_ERR_GETGPIOMON_INDEX_NULL_PARM,
    MYKONOS_ERR_GETGPIOMON_MONITORMASK_NULL_PARM,

    MYKONOS_ERR_MGCRX2_GPIO_DECPIN_INV_PARAM,
    MYKONOS_ERR_MGCRX2_GPIO_INCPIN_INV_PARAM,
    MYKONOS_ERR_MGCRX2_SOURCE_CONFIG,
    MYKONOS_ERR_MGCRX2_STEP_INV_PARAM,
    MYKONOS_ERR_MGCRX1_GPIO_DECPIN_INV_PARAM,
    MYKONOS_ERR_MGCRX1_GPIO_INCPIN_INV_PARAM,
    MYKONOS_ERR_MGCRX1_STEP_INV_PARAM,
    MYKONOS_ERR_GETRX2PIN_INCSTEP_NULL_PARM,
    MYKONOS_ERR_GETRX2PIN_DECSTEP_NULL_PARM,
    MYKONOS_ERR_GETRX2PIN_INCPIN_NULL_PARM,
    MYKONOS_ERR_GETRX2PIN_DECPIN_NULL_PARM,
    MYKONOS_ERR_GETRX2PIN_EN_NULL_PARM,
    MYKONOS_ERR_GETRX1PIN_INCSTEP_NULL_PARM,
    MYKONOS_ERR_GETRX1PIN_DECSTEP_NULL_PARM,
    MYKONOS_ERR_GETRX1PIN_INCPIN_NULL_PARM,
    MYKONOS_ERR_GETRX1PIN_DECPIN_NULL_PARM,
    MYKONOS_ERR_GETRX1PIN_EN_NULL_PARM,

    MYKONOS_ERR_TPCTX2_GPIO_DECPIN_INV_PARAM,
    MYKONOS_ERR_TPCTX2_GPIO_INCPIN_INV_PARAM,
    MYKONOS_ERR_TPCTX2_GPIO_STEP_INV_PARAM,
    MYKONOS_ERR_TPCTX1_GPIO_DECPIN_INV_PARAM,
    MYKONOS_ERR_TPCTX1_GPIO_INCPIN_INV_PARAM,
    MYKONOS_ERR_TPCTX1_GPIO_STEP_INV_PARAM,
    MYKONOS_ERR_GETTX2PIN_STEP_NULL_PARM,
    MYKONOS_ERR_GETTX2PIN_INC_NULL_PARM,
    MYKONOS_ERR_GETTX2PIN_DEC_NULL_PARM,
    MYKONOS_ERR_GETTX2PIN_EN_NULL_PARM,
    MYKONOS_ERR_GETTX2PIN_TX1TX2_NULL_PARM,
    MYKONOS_ERR_GETTX1PIN_STEP_NULL_PARM,
    MYKONOS_ERR_GETTX1PIN_INC_NULL_PARM,
    MYKONOS_ERR_GETTX1PIN_DEC_NULL_PARM,
    MYKONOS_ERR_GETTX1PIN_EN_NULL_PARM,
    MYKONOS_ERR_GETTX1PIN_TX1TX2_NULL_PARM,

	MYKONOS_ERR_SPI2_INV_GPIO,
	MYKONOS_ERR_SPI2_INV_SETUP,

	MYKONOS_ERR_SET_GPIO_1V8_INV_POINTER,

	MYKONOS_ERR_GETGPIOSETLEVEL_NULL_PARM,
	MYKONOS_ERR_READGPIOSPI_NULL_PARM,
    MYKONOS_ERR_SET_GPIO_1V8_INV_MODE,
    MYKONOS_ERR_GETGPIO_OE_NULL_PARM,
    MYKONOS_ERR_GPIO_OE_INV_PARAM,
    MYKONOS_ERR_GPIO_SRC_PARAM_INV,

    MYKONOS_ERR_INV_GP_INT_MASK_PARM,
    MYKONOS_ERR_GP_INT_STATUS_NULL_PARAM,

    MYKONOS_ERR_GETGPIO3V3OUT_NULL_PARM,
    MYKONOS_ERR_SET_GPIO_3V3_INV_POINTER,
    MYKONOS_ERR_GETGPIO3V3SPI_NULL_PARM,
    MYKONOS_ERR_GPIO3V3OUTEN_NULL_PARM,
    MYKONOS_ERR_SET_GPIO_3V3_INV_MODE,
    MYKONOS_ERR_SET_GPIO_3V3_INV_SRC_CTRL,

    MYKONOS_ERR_SET_ARMGPIO_INV_POINTER,
    MYKONOS_ERR_SET_ARMGPIO_PINS_ARMERROR,
    MYKONOS_ERR_SET_ARMGPIO_PINS_INV_SIGNALID,
    MYKONOS_ERR_SET_ARMGPIO_PINS_INV_GPIOPIN,
    MYKONOS_ERR_SET_RADIOCTRL_PINS_ARMERROR,

    MYKONOS_ERR_SETUPAUXDAC_NULL_PARAM,
    MYKONOS_ERR_WRITEAUXDAC_NULL_AUXIO,
    MYKONOS_ERR_READAUXADC_NULL_PARAM,
    MYKONOS_ERR_SETUPAUXDAC_INV_AUXDACCODE,
    MYKONOS_ERR_WRITEAUXDAC_INV_AUXDACCODE,
    MYKONOS_ERR_WRITEAUXDAC_INV_AUXDACINDEX,
    MYKONOS_ERR_INV_AUX_ADC_CHAN_PARM,
    MYKONOS_ERR_SETUPAUXADC_INV_VCODIV,
    MYKONOS_ERR_INV_AUX_ADC_DEC_PARM,

    MYKONOS_ERR_GAINCOMP_NULL_STRUCT,
    MYKONOS_ERR_GAINCOMP_SET_NULL_STRUCT,
    MYKONOS_ERR_GAINCOMP_INV_DELAY,
    MYKONOS_ERR_GAINCOMP_INV_RX1_OFFSET,
    MYKONOS_ERR_GAINCOMP_INV_RX2_OFFSET,
    MYKONOS_ERR_GAINCOMP_INV_DURATION,
    MYKONOS_ERR_GAINCOMP_INV_RAMP,
    MYKONOS_ERR_GAINCOMP_INV_STEP,
    MYKONOS_ERR_GAINCOMP_INV_EN,

    MYKONOS_ERR_SLICER_STEP_OUT_OF_RANGE,
    MYKONOS_ERR_SLICER_INV_RX1_SEL,
    MYKONOS_ERR_SLICER_INV_RX2_SEL,
    MYKONOS_ERR_SLICER_EN_INV,

    MYKONOS_ERR_SLICER_RX1PIN_NULL_PARM,
    MYKONOS_ERR_SLICER_RX2PIN_NULL_PARM,
    MYKONOS_ERR_SLICER_STEP_NULL_PARM,
    MYKONOS_ERR_SLICER_EN_NULL_PARM,

    MYKONOS_ERR_FLOATFRMT_NULL_STRUCT,
    MYKONOS_ERR_FLOATFRMT_SET_NULL_STRUCT,
    MYKONOS_ERR_FLOATFRMT_INV_ROUND_MODE,
    MYKONOS_ERR_FLOATFRMT_INV_DATA_FORMAT,
    MYKONOS_ERR_FLOATFRMT_INV_ENC_NAN,
    MYKONOS_ERR_FLOATFRMT_INV_EXP_BITS,
    MYKONOS_ERR_FLOATFRMT_INV_LEADING,
    MYKONOS_ERR_FLOATFRMT_INV_RX1ATT,
    MYKONOS_ERR_FLOATFRMT_INV_RX2ATT,
    MYKONOS_ERR_FLOATFRMT_INV_EN,
    MYKONOS_ERR_FLOATFRMT_NULL_RX1ATT,
    MYKONOS_ERR_FLOATFRMT_NULL_RX2ATT,
    MYKONOS_ERR_FLOATFRMT_NULL_ENABLE,
    MYKONOS_ERR_FLOATFRMT_SET_INV_EN,
    MYKONOS_ERR_FLOATFRMT_SET_INV_RX1ATT,
    MYKONOS_ERR_FLOATFRMT_SET_INV_RX2ATT,

    MYKONOS_ERR_FLOATFRMT_INV_ORXATT,
    MYKONOS_ERR_FLOATFRMT_INV_ORXEN,
    MYKONOS_ERR_FLOATFRMT_NULL_ORXATT,
    MYKONOS_ERR_FLOATFRMT_NULL_ORXENABLE
} mykonosGpioErr_t;

/**
 *  \brief Enum of possible Rx Slicer pin combinations
 */
typedef enum
{
    GPIO_0_1_2 = 0, /*!< GPIO combination for RX1 */
    GPIO_5_6_7 = 1, /*!< GPIO combination for RX1 or RX2 */
    GPIO_8_9_10 = 2, /*!< GPIO combination for RX1 */
    GPIO_11_12_13 = 3 /*!< GPIO combination for RX2 */
} mykonosRxSlicer_t;


/**
 *  \brief Data structure to hold low voltage GPIO settings
 *
 * uint8_t delay, This parameter contains the delay (in clkRout cycles) from when the gain
 * was changed in the front end to when the compensation is applied in the digital section.
 * The value varies for different data-path configurations.
 * It has a range of 0 to 0xFF.
 *
 * uint8_t rx1Offset, These parameter contains the Rx1 offset word used for the gain compensation
 * when the gain index is at its maximum setting.
 * It has a range of 0 to 0x1F with a resolution is 0.5dB per LSB.
 *
 * uint8_t rx2Offset, These parameter contains the Rx2 offset word used for the gain compensation
 * when the gain index is at its maximum setting.
 * It has a range of 0 to 0x1F with a resolution is 0.5dB per LSB.
 *
 * uint8_t compDuration, These bits configure the number of clock cycles (clkRout cycles/IQ sample rate)
 * between consecutive steps in the ramp that is used to compensate for the gain in the digital section.
 * It has a range of 0 to 0x3.
 *
 * uint8_t compRamp, These bits configure the number of total steps in the ramp
 * that is generated by the gain compensation block according to the following settings.
 * compRamp |  dB ramp
 * ---------|------------
 *     0    |    1dB
 *     1    |    2dB
 *     2    |    3dB
 *     3    |    4dB
 *
 * uint8_t compStep, These bits contains the value in dB that the total Rx gain changes
 * when there is an LSB change in the gain index according to the following settings:
 * compStep |  dB ramp
 * ---------|------------
 *    0     |   0.25dB
 *    1     |   0.5dB
 *    2     |   1.0dB
 *    3     |   2.0dB
 *    4     |   3.0dB
 *    5     |   4.0dB
 *    6     |   5.0dB
 *    7     |   6.0dB
 *
 **/
typedef struct
{
    uint8_t delayCycles;
    uint8_t rx1Offset;
    uint8_t rx2Offset;
    uint8_t compDuration;
    uint8_t compRamp;
    uint8_t compStep;
} mykonosGainComp_t;


/**
 *  \brief Data structure to hold floating point formatter settings for the floating point
 *  number generation
 *
 * uint8_t roundMode, These parameter set the round modes for the significand.
 * The following settings are defined in the IEEE754 specification:
 * roundMode |  Round type
 * ----------|-----------------
 *      0    |  RoundTiesToEven
 *      1    |  RoundTowardsPositive
 *      2    |  RoundTowardsNegative
 *      3    |  RoundTowardsZero
 *      4    |  RoundTiesToAway
 *
 *
 * uint8_t dataFormat, These parameter sets the format of the 16-bit output on the JESD interface:
 * Setting this to 1 then the format is from MSB to LSB to {sign, significand, exponent}.
 * Clearing this bit sets the format to {sign, exponent, significand}.
 *
 * uint8_t encNan, if this parameter is set to 1 then the floating point formatter reserves the highest value
 * of exponent for NaN to be compatible to the IEEE754 specification.
 * Clearing this parameter increases the range of the exponent by one.
 *
 * uint8_t expBits, These parameter is used to indicate the number of exponent bits in the floating point number
 * according to the following settings:
 *  expBits|  Round type
 * --------|------------------------------------------------
 *    0    |  2 bit exponent, 13 bit significand, 1 bit sign
 *    1    |  3 bit exponent, 12 bit significand, 1 bit sign
 *    2    |  4 bit exponent, 11 bit significand, 1 bit sign
 *    3    |  5 bit exponent, 10 bit significand, 1 bit sign
 *
 * uint8_t leading, Setting this parameter hides the leading one in the the significand to be compatible to the IEEE754 specification.
 * Clearing this parameter causes the leading one to be at the MSB of the significand.
 *
 **/
typedef struct
{
   uint8_t roundMode;
   uint8_t dataFormat;
   uint8_t encNan;
   uint8_t expBits;
   uint8_t leading;
}mykonosFloatPntFrmt_t;

#ifdef __cplusplus
}
#endif

#endif /* T_MYKONOSGPIO_H_ */
