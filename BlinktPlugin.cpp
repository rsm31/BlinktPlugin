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

// See BlinktPlugin.h for details of the public API of this plugin.

#include "BlinktPlugin.h"
#include "blinkt_functions.h"
#include <wiringPi.h>


unsigned BlinktPlugin::RefCount = 0;
bool BlinktPlugin::ResetOnUnload = true;
pthread_mutex_t BlinktPlugin::Mutex;


void BlinktPlugin::Lock() {
	pthread_mutex_lock(&Mutex);
}

void BlinktPlugin::Unlock() {
	pthread_mutex_unlock(&Mutex);
}


void BlinktPlugin::IncrementRefCount() {
	RefCount++;
}

// Reset Blinkt when refcount reaches zero, if enabled
void BlinktPlugin::DecrementRefCount() {
	if (RefCount > 0 && --RefCount == 0 && ResetOnUnload) {
		blinkt_reset();
		blinkt_refresh();
	}
}


// Plugin functions available through EPL
// Mostly just map through to wiringPi or blinkt_functions

void BlinktPlugin::setLED(int64_t num, int64_t red, int64_t green, int64_t blue, double intensity) {
	Lock();
	blinkt_set_led(num, red, green, blue, intensity);
	Unlock();
}

void BlinktPlugin::setAll(int64_t red, int64_t green, int64_t blue, double intensity) {
	Lock();
	blinkt_set_all(red, green, blue, intensity);
	Unlock();
}

void BlinktPlugin::setIntensity(int64_t num, double intensity) {
	Lock();
	blinkt_set_intensity(num, intensity);
	Unlock();
}

void BlinktPlugin::setIntensityAll(double intensity) {
	Lock();
	blinkt_set_intensity(intensity);
	Unlock();
}

void BlinktPlugin::refresh() {
	Lock();
	blinkt_refresh();
	Unlock();
}

void BlinktPlugin::reset() {
	Lock();
	blinkt_reset();
	Unlock();
}

void BlinktPlugin::delay(int64_t millis) {
	::delay((unsigned)millis);
}

bool BlinktPlugin::enableDebug(bool enable) {
	Lock();
	bool rval = blinkt_enable_debug(enable);
	Unlock();
	return rval;
}

bool BlinktPlugin::enableResetOnUnload(bool enable) {
	Lock();
	bool rval = ResetOnUnload;
	ResetOnUnload = enable;
	Unlock();
	return rval;
}

// Return the GPIO pin number of the Blinkt DAT (data) pin
int64_t BlinktPlugin::getDAT() {
	return BLINKT_DAT;
}

// Return the GPIO pin number of the Blinkt CLK (clock) pin
int64_t BlinktPlugin::getCLK() {
	return BLINKT_CLK;
}


APAMA_DECLARE_EPL_PLUGIN(BlinktPlugin)
