#ifndef  __MAIN_H__
#define  __MAIN_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "DSP2833x.h"

#define LED_RUN_ON() GpioDataRegs.GPACLEAR.bit.GPIO14 = 1

#define LED_STOP_ON() GpioDataRegs.GPACLEAR.bit.GPIO15 = 1

#define LED_RUN_OFF() GpioDataRegs.GPASET.bit.GPIO14 = 1

#define LED_STOP_OFF() GpioDataRegs.GPASET.bit.GPIO15 = 1

#define BLINK_LED() GpioDataRegs.GPATOGGLE.bit.GPIO26 = 1


#define ENABLE_ALL_PWM() GpioDataRegs.GPASET.bit.GPIO12 = 1

#define DISABLE_ALL_PWM() GpioDataRegs.GPACLEAR.bit.GPIO12 = 1


#define CLOSE_RELAY_1() GpioDataRegs.GPASET.bit.GPIO13 = 1

#define OPEN_RELAY_1() GpioDataRegs.GPACLEAR.bit.GPIO13 = 1


#define CLOSE_RELAY_2() GpioDataRegs.GPASET.bit.GPIO24 = 1

#define OPEN_RELAY_2() GpioDataRegs.GPACLEAR.bit.GPIO24 = 1


#define Set_check_pin() GpioDataRegs.GPASET.bit.GPIO25 = 1

#define Clear_check_pin() GpioDataRegs.GPACLEAR.bit.GPIO25 = 1




#ifdef __cplusplus
}
#endif /* extern "C" */

#endif
