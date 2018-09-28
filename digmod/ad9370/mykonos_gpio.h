/*
 * mykonos_gpio.h
 *
 */

#ifndef MYKONOSGPIO_H_
#define MYKONOSGPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "t_mykonos.h"
#include "t_mykonos_gpio.h"

/*
 *****************************************************************************
 * GPIO functions
 *****************************************************************************
 */
/* Monitor output Function */
mykonosGpioErr_t MYKONOS_setGpioMonitorOut(mykonosDevice_t *device, uint8_t monitorIndex, uint8_t monitorMask);
mykonosGpioErr_t MYKONOS_getGpioMonitorOut(mykonosDevice_t *device, uint8_t *monitorIndex, uint8_t *monitorMask);

/* RX GPIO Functions */
mykonosGpioErr_t MYKONOS_setRx1GainCtrlPin(mykonosDevice_t *device, uint8_t incStep, uint8_t decStep, mykonosGpioSelect_t rx1GainIncPin, mykonosGpioSelect_t rx1GainDecPin, uint8_t enable);
mykonosGpioErr_t MYKONOS_setRx2GainCtrlPin(mykonosDevice_t *device, uint8_t incStep, uint8_t decStep, mykonosGpioSelect_t rx2GainIncPin, mykonosGpioSelect_t rx2GainDecPin, uint8_t enable);
mykonosGpioErr_t MYKONOS_getRx1GainCtrlPin(mykonosDevice_t *device, uint8_t *incStep, uint8_t *decStep, mykonosGpioSelect_t *rx1GainIncPin, mykonosGpioSelect_t *rx1GainDecPin, uint8_t *enable);
mykonosGpioErr_t MYKONOS_getRx2GainCtrlPin(mykonosDevice_t *device, uint8_t *incStep, uint8_t *decStep, mykonosGpioSelect_t *rx2GainIncPin, mykonosGpioSelect_t *rx2GainDecPin, uint8_t *enable);

/* TX GPIO Functions */
mykonosGpioErr_t MYKONOS_setTx1AttenCtrlPin(mykonosDevice_t *device, uint8_t stepSize, mykonosGpioSelect_t tx1AttenIncPin, mykonosGpioSelect_t tx1AttenDecPin, uint8_t enable, uint8_t useTx1ForTx2);
mykonosGpioErr_t MYKONOS_setTx2AttenCtrlPin(mykonosDevice_t *device, uint8_t stepSize, mykonosGpioSelect_t tx2AttenIncPin, mykonosGpioSelect_t tx2AttenDecPin, uint8_t enable);
mykonosGpioErr_t MYKONOS_getTx1AttenCtrlPin(mykonosDevice_t *device, uint8_t *stepSize, mykonosGpioSelect_t *tx1AttenIncPin, mykonosGpioSelect_t *tx1AttenDecPin, uint8_t *enable, uint8_t *useTx1ForTx2);
mykonosGpioErr_t MYKONOS_getTx2AttenCtrlPin(mykonosDevice_t *device, uint8_t *stepSize, mykonosGpioSelect_t *tx2AttenIncPin, mykonosGpioSelect_t *tx2AttenDecPin, uint8_t *enable, uint8_t *useTx1ForTx2);

/* GPIO memory source Functions */
mykonosGpioErr_t MYKONOS_setGpioPinLevel(mykonosDevice_t *device, uint32_t gpioPinLevel);
mykonosGpioErr_t MYKONOS_getGpioPinLevel(mykonosDevice_t *device, uint32_t *gpioPinLevel);
mykonosGpioErr_t MYKONOS_getGpioSetLevel(mykonosDevice_t *device, uint32_t *gpioPinSetLevel);

/* Gain Compensation Functions */
mykonosGpioErr_t MYKONOS_setRxGainCompensation (mykonosDevice_t *device, mykonosGainComp_t *gainComp, uint8_t enable);
mykonosGpioErr_t MYKONOS_getRxGainCompensation (mykonosDevice_t *device, mykonosGainComp_t *gainComp, uint8_t *enabled);

/* SLICER Functions */
mykonosGpioErr_t MYKONOS_setRxSlicerCtrl(mykonosDevice_t *device, uint8_t slicerStep, mykonosRxSlicer_t rx1Pins, mykonosRxSlicer_t rx2Pins, uint8_t enable);
mykonosGpioErr_t MYKONOS_getRxSlicerCtrl(mykonosDevice_t *device, uint8_t *slicerStep, mykonosRxSlicer_t *rx1Pins, mykonosRxSlicer_t *rx2Pins, uint8_t *enable);

/* Floating Point Formatter Functions */
mykonosGpioErr_t MYKONOS_setFloatPointFrmt (mykonosDevice_t *device, mykonosFloatPntFrmt_t *floatFrmt);
mykonosGpioErr_t MYKONOS_getFloatPointFrmt (mykonosDevice_t *device, mykonosFloatPntFrmt_t *floatFrmt);

mykonosGpioErr_t MYKONOS_setRxEnFloatPntFrmt (mykonosDevice_t *device, uint8_t rx1Att, uint8_t rx2Att, uint8_t enable);
mykonosGpioErr_t MYKONOS_getRxEnFloatPntFrmt (mykonosDevice_t *device, uint8_t *rx1Att, uint8_t *rx2Att, uint8_t *enable);

mykonosGpioErr_t MYKONOS_setOrxEnFloatPntFrmt (mykonosDevice_t *device, uint8_t orxAtt, uint8_t enable);
mykonosGpioErr_t MYKONOS_getOrxEnFloatPntFrmt (mykonosDevice_t *device, uint8_t *orxAtt, uint8_t *enable);

/*
 *****************************************************************************
 * General Purpose Interrupt functions
 *****************************************************************************
 */
mykonosGpioErr_t MYKONOS_configGpInterrupt(mykonosDevice_t *device, uint16_t gpMask);
mykonosGpioErr_t MYKONOS_readGpInterruptStatus(mykonosDevice_t *device, uint16_t *status);

/*
 *****************************************************************************
 * ARM GPIO usage functions
 *****************************************************************************
 */
mykonosGpioErr_t MYKONOS_setArmGpioPins(mykonosDevice_t *device);
mykonosGpioErr_t MYKONOS_setRadioControlPinMode(mykonosDevice_t *device);

/*
 *****************************************************************************
 * AuxADC/AuxDAC functions
 *****************************************************************************
 */
mykonosGpioErr_t MYKONOS_setupAuxAdcs(mykonosDevice_t *device, uint8_t adcDecimation, uint8_t enable);
mykonosGpioErr_t MYKONOS_setAuxAdcChannel(mykonosDevice_t *device, uint8_t auxAdcChannel);
mykonosGpioErr_t MYKONOS_readAuxAdc(mykonosDevice_t *device, uint16_t *adcCode);
mykonosGpioErr_t MYKONOS_setupAuxDacs(mykonosDevice_t *device);
mykonosGpioErr_t MYKONOS_writeAuxDac(mykonosDevice_t *device, uint8_t auxDacIndex, uint16_t auxDacCode);

/*
 *****************************************************************************
 * GPIO3v3 functions
 *****************************************************************************
 */
mykonosGpioErr_t MYKONOS_setupGpio3v3(mykonosDevice_t *device);

mykonosGpioErr_t MYKONOS_setGpio3v3PinLevel(mykonosDevice_t *device, uint16_t gpio3v3PinLevel);
mykonosGpioErr_t MYKONOS_getGpio3v3SetLevel(mykonosDevice_t *device, uint16_t *gpio3v3SetLevel);

mykonosGpioErr_t MYKONOS_getGpio3v3PinLevel(mykonosDevice_t *device, uint16_t *gpio3v3PinLevel);

mykonosGpioErr_t MYKONOS_setGpio3v3Oe(mykonosDevice_t *device, uint16_t gpio3v3OutEn);
mykonosGpioErr_t MYKONOS_getGpio3v3Oe(mykonosDevice_t *device, uint16_t *gpio3v3OutEn);

mykonosGpioErr_t MYKONOS_setGpio3v3SourceCtrl(mykonosDevice_t *device, uint16_t gpio3v3SrcCtrl);
mykonosGpioErr_t MYKONOS_getGpio3v3SourceCtrl(mykonosDevice_t *device, uint16_t *gpio3v3SrcCtrl);

/*
 *****************************************************************************
 * GPIO helper functions
 *****************************************************************************
 */
mykonosGpioErr_t MYKONOS_setupGpio(mykonosDevice_t *device);
mykonosGpioErr_t MYKONOS_setGpioOe(mykonosDevice_t *device, uint32_t gpioOutEn, uint32_t gpioUsedMask);
mykonosGpioErr_t MYKONOS_getGpioOe(mykonosDevice_t *device, uint32_t *gpioOutEn);

mykonosGpioErr_t MYKONOS_setGpioSourceCtrl(mykonosDevice_t *device, uint32_t gpioSrcCtrl);
mykonosGpioErr_t MYKONOS_getGpioSourceCtrl(mykonosDevice_t *device, uint32_t *gpioSrcCtrl);

const char* getGpioMykonosErrorMessage(mykonosGpioErr_t errorCode);

#ifdef __cplusplus
}
#endif

#endif /* MYKONOSGPIO_H_ */
