/*
    FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

 ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
 ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

 ***************************************************************************
 *                                                                       *
 *    FreeRTOS provides completely free yet professionally developed,    *
 *    robust, strictly quality controlled, supported, and cross          *
 *    platform software that is more than just the market leader, it     *
 *    is the industry's de facto standard.                               *
 *                                                                       *
 *    Help yourself get started quickly while simultaneously helping     *
 *    to support the FreeRTOS project by purchasing a FreeRTOS           *
 *    tutorial book, reference manual, or both:                          *
 *    http://www.FreeRTOS.org/Documentation                              *
 *                                                                       *
 ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
 */

/* Scheduler includes. */
#include "FreeRTOS.h"
/*task incluede*/
#include "task.h"
/* Demo app includes. */
#include "partest.h"

#define ptOUTPUT 	0
#define ptALL_OFF	0

/*-----------------------------------------------------------
 * Simple parallel port IO routines.
 *-----------------------------------------------------------*/
void vParTestInitialise(void) {
    /* The explorer 16 board has LED's on port A.  All bits are set as output
    so PORTA is read-modified-written directly. */
    //    _TRISA1 = _TRISB15 = ptOUTPUT;
    _TRISE7 = _TRISE5 = 0;
    _LATE7 = _LATE5 = 0;
    //    _RA1 = _RB15 = 1
    _TRISD7 = 1; //设置为输入//C3INA/SEG26/CN16/RD7
    _ANSD7 = 0; //设置为数字输入
    //    _CN16PUE = 1; //使能CN16的上拉
    _CNIP = 1; //优先级为2
    _CNIF = 0; //清除标志位
    _CN16IE = 1; //使能CNT16
    _CNIE = 1; //使能中断
}

/*-----------------------------------------------------------*/

void vParTestSetLED(unsigned portBASE_TYPE uxLED, signed portBASE_TYPE xValue) {
    unsigned portBASE_TYPE uxLEDBit;

    /* Which port A bit is being modified? */
    uxLEDBit = 1 << uxLED;

    if (xValue) {
        /* Turn the LED on. */
        portENTER_CRITICAL();
        {
            _LATE5 = 1;
            //            PORTA |= uxLEDBit;
        }
        portEXIT_CRITICAL();
    } else {
        /* Turn the LED off. */
        portENTER_CRITICAL();
        {
            _LATE5 = 0;
            //            PORTA &= ~uxLEDBit;
        }
        portEXIT_CRITICAL();
    }
}

/*-----------------------------------------------------------*/

void vParTestToggleLED(unsigned portBASE_TYPE uxLED) {
    unsigned portBASE_TYPE uxLEDBit;

    uxLEDBit = 1 << uxLED;
    portENTER_CRITICAL();
    {
        /* If the LED is already on - turn it off.  If the LED is already
        off, turn it on. */
        //        if (PORTA & uxLEDBit) {
        //            PORTA &= ~uxLEDBit;
        //        } else {
        //            PORTA |= uxLEDBit;
        //    }
        _LATE5 ^= 1;
    }
    portEXIT_CRITICAL();
}
extern UBaseType_t xEntryTickLessMode;

void __attribute__((__interrupt__, auto_psv)) _CNInterrupt(void) {//_CNInterrupt _CNIInterrupt
    UBaseType_t status_value;

    if (!_RD7) {//如果KEY1按下
        status_value = taskENTER_CRITICAL_FROM_ISR();
        xEntryTickLessMode ^= 1;
        taskEXIT_CRITICAL_FROM_ISR(status_value);
    }
    _CNIF = 0; //清除标志位;
}



