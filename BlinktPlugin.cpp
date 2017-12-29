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


bool BlinktPlugin::initialised = false;
unsigned BlinktPlugin::RefCount = 0;
bool BlinktPlugin::ResetOnUnload = true;
std::mutex BlinktPlugin::Mutex;


BlinktPlugin::BlinktPlugin(): base_plugin_t("BlinktPlugin") {
	// No one-off initialisation needed currently
	if (!initialised) {
		initialised = true;
	}
	BlinktPlugin::IncrementRefCount();
}

BlinktPlugin::~BlinktPlugin() {
	// Maybe reset Blinkt if refcount reaches zero
	BlinktPlugin::DecrementRefCount();
}


void BlinktPlugin::IncrementRefCount() {
	std::lock_guard<std::mutex> lock(Mutex);
	RefCount++;
}

// Reset Blinkt when refcount reaches zero, if enabled
void BlinktPlugin::DecrementRefCount() {
	std::lock_guard<std::mutex> lock(Mutex);
	if (RefCount > 0 && --RefCount == 0 && ResetOnUnload) {
		blinkt_reset();
		blinkt_refresh();
	}
}


// Plugin functions available through EPL
// Mostly just map through to wiringPi or blinkt_functions

void BlinktPlugin::setLED(int64_t num, int64_t red, int64_t green, int64_t blue, double intensity) {
	std::lock_guard<std::mutex> lock(Mutex);
	blinkt_set_led(num, red, green, blue, intensity);
}

void BlinktPlugin::setAll(int64_t red, int64_t green, int64_t blue, double intensity) {
	std::lock_guard<std::mutex> lock(Mutex);
	blinkt_set_all(red, green, blue, intensity);
}

void BlinktPlugin::setIntensity(int64_t num, double intensity) {
	std::lock_guard<std::mutex> lock(Mutex);
	blinkt_set_intensity(num, intensity);
}

void BlinktPlugin::setIntensityAll(double intensity) {
	std::lock_guard<std::mutex> lock(Mutex);
	blinkt_set_intensity(intensity);
}

void BlinktPlugin::refresh() {
	std::lock_guard<std::mutex> lock(Mutex);
	blinkt_refresh();
}

void BlinktPlugin::reset() {
	std::lock_guard<std::mutex> lock(Mutex);
	blinkt_reset();
}

void BlinktPlugin::delay(int64_t millis) {
	::delay((unsigned)millis);
}

bool BlinktPlugin::enableDebug(bool enable) {
	std::lock_guard<std::mutex> lock(Mutex);
	return blinkt_enable_debug(enable);
}

bool BlinktPlugin::enableResetOnUnload(bool enable) {
	std::lock_guard<std::mutex> lock(Mutex);
	bool rval = ResetOnUnload;
	ResetOnUnload = enable;
	return rval;
}

int64_t BlinktPlugin::getDAT() {
	return BLINKT_DAT;
}

int64_t BlinktPlugin::getCLK() {
	return BLINKT_CLK;
}
