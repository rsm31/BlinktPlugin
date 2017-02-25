/*
 * Copyright (c) 2016-2017 Scott Mitchell.
 * All rights reserved.
 *
 * Licenced under the BSD 3-Clause licence (the "Licence"); you may not use
 * this file except in compliance with the Licence. You may obtain a copy of
 * the Licence from the LICENCE file in the top level of this software
 * distribution or from:
 *
 *	 https://opensource.org/licenses/BSD-3-Clause
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */

#ifndef _BLINKT_FUNCTIONS_H
#define _BLINKT_FUNCTIONS_H

#include <stdint.h>


/**
 * Support functions for the BlinktPlugin. This is the code that actually
 * interacts with the wiringPi library to control the Blinkt! hardware. These
 * functions can be used outside of the Apama correlator to control the
 * Blinkt!, see for example the blinkt_test and blinkt_reset programs.
 *
 * For more information on wiringPi see: http://wiringpi.com/
 *
 * The Blinkt! is a strip of 8 APA102 RGB LED modules that can be attached to
 * the 40-pin GPIO connector on a Raspberry Pi Model B and controlled using a
 * simple two-wire SPI protocol. The BlinktPlugin software makes the Blinkt!
 * accessible to Apama EPL developers through an EPL plugin and helper
 * object. The goal is to demonstrate how to straightforwardly integrate real
 * hardware with Apama and control it from EPL.
 *
 * The APA102 LED modules used in the Blinkt! are controllable for both colour
 * and overall intensity or brightness. Colour values for the LEDs are RGB
 * (red, green, blue) triplets with a range of zero to 255 inclusive. The
 * plugin handles values outside this range by simply using the least
 * significant 8 bits of the integer as the colour value and ignoring the
 * rest.
 *
 * Intensity values for the LEDs are represented as floating point numbers
 * with a range of 0.0 to 1.0 inclusive. Values less than zero will cause the
 * intensity to be left unchanged, although using the setRGB() or setAllRGB()
 * actions on the helper object is an easier way to change the colour of the
 * LEDs without affecting the intensity. Values greater than 1.0 are treated
 * as though they were 1.0, i.e. maximum intensity.
 *
 * The plugin keeps an internal colour/intensity state for each LED that is
 * only transferred to the real LEDs when the refresh() action is called. This
 * is the only way to change the state of the physical LEDs. The effects of
 * any number of preceding set*() and reset() actions will become visible only
 * when refresh() is called.
 *
 * For more information on the Blinkt! hardware and other language APIs see:
 * https://github.com/pimoroni/blinkt (Blinkt! GitHub project)
 * https://cdn-shop.adafruit.com/product-files/2343/APA102C.pdf (APA102 datasheet)
 */


/**
 * The number of LEDs on a Blinkt! device.
 */
const unsigned BLINKT_NUM_LEDS = 8;

/**
 * GPIO pin number for the Blinkt! DAT (data) line, using the Broadcom pin
 * numbering scheme.
 */
const unsigned BLINKT_DAT = 23;

/**
 * GPIO pin number for the Blinkt! CLK (clock) line, using the Broadcom pin
 * numbering scheme.
 */
const unsigned BLINKT_CLK = 24;


/**
 * Set the colour and intensity of a Blinkt! LED. This function just changes
 * internal state. Use the refresh() function to actually update the Blinkt!
 * LEDs. The intensity paramter is optional and the intensity of the LED will
 * be left unchanged if this parameter is not present.
 *
 * @param led The LED number to set, starting from zero.
 * @param red The red component of the RGB colour value.
 * @param green The green component of the RGB colour value.
 * @param blue The blue component of the RGB colour value.
 * @param intensity The global intensity of the LED (optional).
 */
void blinkt_set_led(unsigned num, uint8_t red, uint8_t green, uint8_t blue, float intensity = -1.0);

/**
 * Set the colour and intensity of all Blinkt! LEDs. This function just
 * changes internal state. Use the refresh() action to actually update the
 * Blinkt! LEDs. The intensity parameter is optional and the intensity of the
 * LEDs will be left unchanged if this parameter is not present.
 *
 * @param red The red component of the RGB colour value.
 * @param green The green component of the RGB colour value.
 * @param blue The blue component of the RGB colour value.
 * @param intensity The global intensity of the LEDs (optional).
 */
void blinkt_set_all(uint8_t red, uint8_t green, uint8_t blue, float intensity = -1.0);

/**
 * Set the intensity of a Blinkt! LED, leaving the colour unchanged. This
 * function just changes internal state. Use the refresh() function to
 * actually update the Blinkt! LEDs.
 *
 * @param led The LED number to set, starting from zero.
 * @param intensity The global intensity of the LED.
 */
void blinkt_set_intensity(unsigned num, float intensity);

/**
 * Set the intensity of all Blinkt! LEDs, leaving the colour unchanged. This
 * function just changes internal state. Use the refresh() function to
 * actually update the Blinkt! LEDs.
 *
 * @param intensity The global intensity of the LEDs.
 */
void blinkt_set_intensity(float intensity);

/**
 * Update all the Blinkt! LEDs to match the internal colour and intensity
 * state, making the effects of all previous blinkt_set*() calls visible.
 */
void blinkt_refresh();

/**
 * Set all Blinkt! LEDs to no colour (red, green and blue all zero) and zero
 * intensity. The refresh() function must be call to make the effects of a
 * reset visible.
 */
void blinkt_reset();

/**
 * Enable or disable debugging output from this module. When enabled, debug
 * output is sent to stdout.
 *
 * @param enable True to enable debug output, false to disable it.
 * @return The previous value of the debug flag.
 */
bool blinkt_enable_debug(bool enable);

#endif // _BLINKT_FUNCTIONS_H
