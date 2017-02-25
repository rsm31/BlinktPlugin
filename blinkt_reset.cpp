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
 * Clear all Blinkt output and set intensity to zero.
 * Assumes that "blinkt_setup" or equivalent has been run.
 */

#include "blinkt_functions.h"
#include <stdlib.h>
#include <wiringPi.h>

int main(int argc, char** argv) {

	(void) wiringPiSetupSys();

	blinkt_reset();
	blinkt_refresh();

	exit(0);
}
