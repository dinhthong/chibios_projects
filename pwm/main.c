/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "ch.h"
#include "hal.h"
#include "rt_test_root.h"
#include "oslib_test_root.h"
static PWMConfig pwmcfg = {
	200000, /* 200Khz PWM clock frequency*/
	1024, /* PWM period of 1024 ticks ~ 0.005 second */
	NULL, /* No callback */

	/* Only channel 1 enabled */
	{
		{PWM_OUTPUT_ACTIVE_HIGH, NULL},
		{PWM_OUTPUT_ACTIVE_HIGH, NULL},
		{PWM_OUTPUT_ACTIVE_HIGH, NULL},
		{PWM_OUTPUT_ACTIVE_HIGH, NULL}
	}
};

static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {
	(void)arg;

	pwmcnt_t duty = 1024;
	bool direction = true;
	while(true) {
		pwmEnableChannel(&PWMD4, 0, (pwmcnt_t)duty);
		chThdSleepMilliseconds(5);
		if (direction) {
			duty -= 1;
		}
		else {
			duty += 1;
		}
		if (duty < 1) {
			direction = false;

		}
		if (duty > 1023) {
			direction = true;
		}



	}


}
int main(void)
{
	halInit();
	chSysInit();

	pwmStart(&PWMD4, &pwmcfg);
  palSetPadMode(GPIOD, GPIOD_LED4, PAL_MODE_ALTERNATE(2));      /* Green.   */

	chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

	while(true){
		chThdSleepMilliseconds(500);

	}
	return 0;
}
