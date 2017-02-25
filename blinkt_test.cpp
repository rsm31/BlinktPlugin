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

/*
 * Simple test of blinkt_functions library.
 * Assumes that "blinkt_setup" or equivalent has been run.
 */

#include "blinkt_functions.h"
#include <stdlib.h>
#include <wiringPi.h>

int main(int argc, char** argv) {

	(void) wiringPiSetupSys();

	// Set rainbow colours + white
	blinkt_set_led(0, 0xff, 0x00, 0x00); // red
	blinkt_set_led(1, 0xff, 0x7f, 0x00); // orange
	blinkt_set_led(2, 0xff, 0xff, 0x00); // yellow
	blinkt_set_led(3, 0x00, 0xff, 0x00); // blue
	blinkt_set_led(4, 0x00, 0x00, 0xff); // green
	blinkt_set_led(5, 0x20, 0x00, 0x40); // indigo
	blinkt_set_led(6, 0x80, 0x00, 0xff); // violet
	blinkt_set_led(7, 0x80, 0x80, 0x80); // white
	blinkt_set_intensity(1.0);
	blinkt_refresh();

	delay(2000);

	// Step the intensity down and back up again
	for (float i = 1.0; i >= 0; i -= 0.01) {
		blinkt_set_intensity(i);
		blinkt_refresh();
		delay(25);
	}
	delay(500);
	for (float i = 0; i <= 1.0; i += 0.01) {
		blinkt_set_intensity(i);
		blinkt_refresh();
		delay(25);
	}

	delay(2000);

	// Do the same thing with the RGB values
	blinkt_set_intensity(1.0);
	for (int i = 0; i <= 0xff; i++) {
		blinkt_set_all(i, i, i);
		blinkt_refresh();
		delay(10);
	}
	delay(500);
	for (int i = 0xff; i >= 0; i--) {
		blinkt_set_all(i, i, i);
		blinkt_refresh();
		delay(10);
	}

	delay(2000);

	// Turn everything off	
	blinkt_reset();
	blinkt_refresh();

	exit(0);
}
