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

// Plugin function parameter types
const char8* BlinktPlugin::setLEDParamTypes[] = { "integer", "integer", "integer", "integer", "float" };
const char8* BlinktPlugin::setAllParamTypes[] = { "integer", "integer", "integer", "float" };
const char8* BlinktPlugin::setIntensityParamTypes[] = { "integer", "float" };
const char8* BlinktPlugin::setIntensityAllParamTypes[] = { "float" };
const char8* BlinktPlugin::refreshParamTypes[] = { };
const char8* BlinktPlugin::resetParamTypes[] = { };
const char8* BlinktPlugin::delayParamTypes[] = { "integer" };
const char8* BlinktPlugin::enableDebugParamTypes[] = { "boolean" };
const char8* BlinktPlugin::enableResetOnUnloadParamTypes[] = { "boolean" };
const char8* BlinktPlugin::getDATParamTypes[] = { };
const char8* BlinktPlugin::getCLKParamTypes[] = { };

// Plugin function table
AP_Function BlinktPlugin::Functions[] = {
	{"setLED", &setLED, 5, setLEDParamTypes, "void"},
	{"setAll",  &setAll, 4, setAllParamTypes,  "void"},
	{"setIntensity", &setIntensity, 2, setIntensityParamTypes, "void"},
	{"setIntensityAll", &setIntensityAll, 1, setIntensityAllParamTypes, "void"},
	{"refresh", &refresh, 0, refreshParamTypes, "void"},
	{"reset",  &reset, 0, resetParamTypes,  "void"},
	{"delay", &delay, 1, delayParamTypes, "void"},
	{"enableDebug", &enableDebug, 1, enableDebugParamTypes, "boolean"},
	{"enableResetOnUnload", &enableResetOnUnload, 1, enableResetOnUnloadParamTypes, "boolean"},
	{"getDAT", &getDAT, 0, getDATParamTypes, "integer"},
	{"getCLK", &getCLK, 0, getCLKParamTypes, "integer"},
};

unsigned BlinktPlugin::RefCount = 0;
bool BlinktPlugin::ResetOnUnload = true;
pthread_mutex_t BlinktPlugin::Mutex;
static bool initialised = false;

// Singleton instance to ensure the constructor gets called
static BlinktPlugin plugin;

// Constructor just does static mutex initialisation
BlinktPlugin::BlinktPlugin() {
	if (!initialised) {
		initialised = true;
		pthread_mutex_init(&Mutex, NULL);
	}
}

BlinktPlugin::~BlinktPlugin() {
	// Nothing to do here
}

void BlinktPlugin::Lock() {
	pthread_mutex_lock(&Mutex);
}

void BlinktPlugin::Unlock() {
	pthread_mutex_unlock(&Mutex);
}

AP_Function* BlinktPlugin::GetFunctions() {
	return Functions;
}

unsigned BlinktPlugin::GetNumFunctions() {
	return sizeof(Functions) / sizeof(AP_Function);
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

void AP_PLUGIN_CALL BlinktPlugin::setLED(const AP_Context& ctx, const AP_TypeList& args, AP_Type& rval, AP_TypeDiscriminator rtype) {
	AP_int64 num = args[0].integerValue();
	AP_int64 red = args[1].integerValue();
	AP_int64 green = args[2].integerValue();
	AP_int64 blue = args[3].integerValue();
	AP_float64 intensity = args[4].floatValue();

	Lock();
	blinkt_set_led(num, red, green, blue, intensity);
	Unlock();
}

void AP_PLUGIN_CALL BlinktPlugin::setAll(const AP_Context& ctx, const AP_TypeList& args, AP_Type& rval, AP_TypeDiscriminator rtype) {
	AP_int64 red = args[0].integerValue();
	AP_int64 green = args[1].integerValue();
	AP_int64 blue = args[2].integerValue();
	AP_float64 intensity = args[3].floatValue();

	Lock();
	blinkt_set_all(red, green, blue, intensity);
	Unlock();
}

void AP_PLUGIN_CALL BlinktPlugin::setIntensity(const AP_Context& ctx, const AP_TypeList& args, AP_Type& rval, AP_TypeDiscriminator rtype) {
	AP_int64 num = args[0].integerValue();
	AP_float64 intensity = args[1].floatValue();

	Lock();
	blinkt_set_intensity(num, intensity);
	Unlock();
}

void AP_PLUGIN_CALL BlinktPlugin::setIntensityAll(const AP_Context& ctx, const AP_TypeList& args, AP_Type& rval, AP_TypeDiscriminator rtype) {
	AP_float64 intensity = args[0].floatValue();

	Lock();
	blinkt_set_intensity(intensity);
	Unlock();
}

void AP_PLUGIN_CALL BlinktPlugin::refresh(const AP_Context& ctx, const AP_TypeList& args, AP_Type& rval, AP_TypeDiscriminator rtype) {
	Lock();
	blinkt_refresh();
	Unlock();
}

void AP_PLUGIN_CALL BlinktPlugin::reset(const AP_Context& ctx, const AP_TypeList& args, AP_Type& rval, AP_TypeDiscriminator rtype) {
	Lock();
	blinkt_reset();
	Unlock();
}

void AP_PLUGIN_CALL BlinktPlugin::delay(const AP_Context& ctx, const AP_TypeList& args, AP_Type& rval, AP_TypeDiscriminator rtype) {
	AP_int64 millis = args[0].integerValue();

	::delay((unsigned)millis);
}

void AP_PLUGIN_CALL BlinktPlugin::enableDebug(const AP_Context& ctx, const AP_TypeList& args, AP_Type& rval, AP_TypeDiscriminator rtype) {
	AP_bool enable = args[0].booleanValue();

	Lock();
	rval.booleanValue(blinkt_enable_debug(enable));
	Unlock();
}

void AP_PLUGIN_CALL BlinktPlugin::enableResetOnUnload(const AP_Context& ctx, const AP_TypeList& args, AP_Type& rval, AP_TypeDiscriminator rtype) {
	AP_bool enable = args[0].booleanValue();

	Lock();
	rval.booleanValue(ResetOnUnload);
	ResetOnUnload = enable;
	Unlock();
}

// Return the GPIO pin number of the Blinkt DAT (data) pin
void AP_PLUGIN_CALL BlinktPlugin::getDAT(const AP_Context& ctx, const AP_TypeList& args, AP_Type& rval, AP_TypeDiscriminator rtype) {
	rval.integerValue(BLINKT_DAT);
}

// Return the GPIO pin number of the Blinkt CLK (clock) pin
void AP_PLUGIN_CALL BlinktPlugin::getCLK(const AP_Context& ctx, const AP_TypeList& args, AP_Type& rval, AP_TypeDiscriminator rtype) {
	rval.integerValue(BLINKT_CLK);
}



/*
 * Boilerplate functions required by the correlator plugin loader.
 * Must be declared extern "C" and with the exact names and signatures
 * defined in the correlator_plugin.hpp header.
 */
extern "C" {

	AP_PLUGIN_DLL_SYM AP_ErrorCode AP_PLUGIN_CALL AP_INIT_FUNCTION_NAME(const AP_Context& ctx, uint32& version, uint32& nFunctions, AP_Function*& functions) {
		// Reject if loading API is older than plugin
		if (version < AP_PLUGIN_VERSION) {
			version	= AP_PLUGIN_VERSION;
			return AP_VERSION_MISMATCH_ERROR;
		}
		else {
			// Export the plugin's version
			version	= AP_PLUGIN_VERSION;
			// Export function declarations
			functions = BlinktPlugin::GetFunctions();
			nFunctions = BlinktPlugin::GetNumFunctions();
			BlinktPlugin::IncrementRefCount();
			return AP_NO_ERROR;
		}
	}

	AP_PLUGIN_DLL_SYM AP_ErrorCode AP_PLUGIN_CALL AP_SHUTDOWN_FUNCTION_NAME(const AP_Context& ctx) {
		// Maybe reset Blinkt if refcount reaches zero
		BlinktPlugin::DecrementRefCount();
		return AP_NO_ERROR;
	}

	AP_PLUGIN_DLL_SYM AP_ErrorCode AP_PLUGIN_CALL AP_LIBRARY_VERSION_FUNCTION_NAME(const AP_Context& ctx, uint32& version) {
		// Just	return the version number
		version	= AP_PLUGIN_VERSION;
		return AP_NO_ERROR;
	}

	AP_PLUGIN_DLL_SYM AP_Capabilities AP_PLUGIN_CALL AP_PLUGIN_GET_CAPABILITIES_FUNCTION_NAME(const AP_Context& ctx) {
		return AP_CAPABILITIES_NONE;
	}

} // extern "C"
