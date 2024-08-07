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
#include <string.h>

/*
 * This is a periodic thread that does absolutely nothing except flashing
 * a LED.
 */
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (true) {
    palSetPad(GPIOD, GPIOD_LED3);       /* Orange.  */
    palSetPad(GPIOD, GPIOD_LED4);       /* Orange.  */
    chThdSleepMilliseconds(1500);
    palClearPad(GPIOD, GPIOD_LED3);     /* Orange.  */
    palClearPad(GPIOD, GPIOD_LED4);     /* Orange.  */
    chThdSleepMilliseconds(1500);
  }
}
uint8_t rxbuffer[20];
uint8_t txbuffer[20];
uint8_t received;

static THD_WORKING_AREA(waThread2, 128);
static THD_FUNCTION(Thread2, arg) {
  (void) arg;
  uint8_t start[] = "Hi from STM32F0:blinker \n";
  sdWrite(&SD2, start, sizeof(start));
  while (true) {
    sdRead(&SD2, rxbuffer, sizeof(rxbuffer));
    sdWrite(&SD2, rxbuffer, sizeof(rxbuffer));
}
}
/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  /*
   * Activates the serial driver 2 using the driver default configuration.
   * PA2(TX) and PA3(RX) are routed to USART2.
   */
  sdStart(&SD2, NULL);
  palSetPadMode(GPIOA, 2, PAL_MODE_ALTERNATE(7));
  palSetPadMode(GPIOA, 3, PAL_MODE_ALTERNATE(7));
/*
   * Activates the serial driver 1 using the driver default configuration.
   */
 // sdStart(&SD2, NULL);
//  palSetPadMode(GPIOA, 2, PAL_MODE_ALTERNATE(1));       /* USART1 TX.       */
//  palSetPadMode(GPIOA, 3, PAL_MODE_ALTERNATE(1));      /* USART1 RX.       */
  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);
  chThdCreateStatic(waThread2, sizeof(waThread2), NORMALPRIO, Thread2, NULL);

	while(true){
		chThdSleepMilliseconds(500);

	}
	return 0;
}
