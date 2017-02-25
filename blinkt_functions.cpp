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

// See blinkt_functions.h for details of the public API of this module.

#include "blinkt_functions.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <wiringPi.h>

static const unsigned BLINKT_BYTES_PER_LED = 4;
static const unsigned BLINKT_BUFFER_LENGTH = BLINKT_BYTES_PER_LED * BLINKT_NUM_LEDS;
static const uint8_t BLINKT_INTENSITY_MAX = 31;
static const uint8_t BLINKT_INTENSITY_MASK = (uint8_t)~BLINKT_INTENSITY_MAX;

/*
 * Buffer sent to the Blinkt when refresh() is called
 * Byte order = IBGR, LED order = L->R
 */
static uint8_t BLINKT_BUFFER[BLINKT_BUFFER_LENGTH];

static bool BLINKT_DEBUG = false;

/*
 * Write a single byte to the Blinkt.  Each bit (msb first) is written to the
 * BLINKT_DAT line then the BLINKT_CLK line is toggled 0->1->0.
 */
void blinkt_write_byte(uint8_t byte) {
	for (int b = 0; b < 8; b++) {
		int d = (byte & 0x80) != 0;
		digitalWrite(BLINKT_DAT, d);
		digitalWrite(BLINKT_CLK, 1);
		byte <<= 1;
		digitalWrite(BLINKT_CLK, 0);
	}
}

/*
 * The start frame is 32 zero bits.
 */
void blinkt_start_frame() {
	blinkt_write_byte(0x00);
	blinkt_write_byte(0x00);
	blinkt_write_byte(0x00);
	blinkt_write_byte(0x00);
}

/*
 * The end frame is 32 one bits.
 */
void blinkt_end_frame() {
	blinkt_write_byte(0xff);
	blinkt_write_byte(0xff);
	blinkt_write_byte(0xff);
	blinkt_write_byte(0xff);
}

/*
 * Debug logging of Blinkt buffer.
 */
void blinkt_dump_buffer() {
	uint8_t* p = BLINKT_BUFFER;
	fprintf(stdout, "Blinkt buffer contents:\n");
	fprintf(stdout, " N:  I  B  G  R\n");
	for (int n = 0; n < BLINKT_NUM_LEDS; n++) {
		fprintf(stdout, "%2d: %0.2x %0.2x %0.2x %0.2x\n", n, *p++, *p++, *p++, *p++);
	}
	fprintf(stdout, "\n");
	fflush(stdout);
}


// Public API functions

void blinkt_refresh() {
	if (BLINKT_DEBUG) {
		blinkt_dump_buffer();
	}
	blinkt_start_frame();
	uint8_t* p = BLINKT_BUFFER;
	for (int n = 0; n < BLINKT_NUM_LEDS; n++) {
		// Bits not used in intensity byte must be set to 1
		blinkt_write_byte(*p++ | BLINKT_INTENSITY_MASK);	// intensity
		blinkt_write_byte(*p++);				// blue
		blinkt_write_byte(*p++);				// green
		blinkt_write_byte(*p++);				// red
	}
	blinkt_end_frame();
}

void blinkt_reset() {
	for (int i = 0; i < BLINKT_BUFFER_LENGTH; i++) {
		BLINKT_BUFFER[i] = (uint8_t)0;
	}
}

void blinkt_set_led(unsigned num, uint8_t red, uint8_t green, uint8_t blue, float intensity) {
	if (num > BLINKT_NUM_LEDS) {
		return;
	}

	uint8_t* p = BLINKT_BUFFER + (num * BLINKT_BYTES_PER_LED);

	if (intensity >= 0.0) {
		*p = (uint8_t)(BLINKT_INTENSITY_MAX * (intensity > 1.0 ? 1.0 : intensity));
	}
	*(++p) = blue;
	*(++p) = green;
	*(++p) = red;
}

void blinkt_set_all(uint8_t red, uint8_t green, uint8_t blue, float intensity) {
	for (int i = 0; i < BLINKT_NUM_LEDS; i++) {
		blinkt_set_led(i, red, green, blue, intensity);
	}
}

void blinkt_set_intensity(unsigned num, float intensity) {
	if (num > BLINKT_NUM_LEDS) {
		return;
	}
	BLINKT_BUFFER[num * BLINKT_BYTES_PER_LED] = (uint8_t)(BLINKT_INTENSITY_MAX * (intensity > 1.0 ? 1.0 : intensity));
}

void blinkt_set_intensity(float intensity) {
	for (int i = 0; i < BLINKT_NUM_LEDS; i++) {
		blinkt_set_intensity(i, intensity);
	}
}

bool blinkt_enable_debug(bool enable) {
	bool ret = BLINKT_DEBUG;
	BLINKT_DEBUG = enable;
	return ret;
}
