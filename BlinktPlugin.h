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

#ifndef _BLINKT_PLUGIN_H
#define _BLINKT_PLUGIN_H

#include <correlator_plugin.hpp>
#include <pthread.h>


/**
 * Correlator plugin to control the Pimoroni Blinkt APA102 LED board. It is
 * recommended to use the rpi.blinkt.BlinktHelper wrapper event in your EPL
 * code whenever possible, rather than invoking the plugin directly. See the
 * ApamaDoc for the BlinktHelper wrapper for full documentation of the
 * available functionality and correct use of the plugin.
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
 *
 * The plugin implementation is entirely static and implemented within a class
 * only to keep things tidy by having it in its own namespace. The plugin is
 * thread-safe and can be accessed from multiple EPL contexts in parallel.
 *
 * Before using the plugin to control the Blinkt, the appropriate GPIO pins
 * for the Blinkt data and clock lines must be configured as outputs. This
 * can be done using the external 'gpio' program or the
 * com.apamax.rpi.gpio.Setup object provided by the GPIO plugin. This setup is
 * done automatically when the BlinktHelper wrapper is injected into the
 * correlator. The plugin provides getDAT() and getCLK() functions to retrieve
 * the relevant GPIO pin numbers.
 */
class BlinktPlugin {

public:

	/**
	 * Constructor, called once in a static initialiser to create and
	 * initialise the mutex.
	 */
	BlinktPlugin();

	/**
	 * Destructor
	 */
	~BlinktPlugin();

	/**
	 * Get the plugin function table.
	 *
	 * @return A pointer to the plugin function table.
	 */
	static AP_Function* GetFunctions();

	/**
	 * Get the size of the plugin function table.
	 *
	 * @return The number of entries in the plugin function table.
	 */
	static unsigned GetNumFunctions();

	/**
	 * Increment the plugin reference count.
	 */
	static void IncrementRefCount();

	/**
	 * Decrement the plugin reference count. If the count reaches zero and
	 * ResetOnUnload is true, this method will attempt to reset and
	 * refresh the Blinkt LEDs. Note that this might fail if for example
	 * the GPIO pin settings have been reset before it is called.
	 */
	static void DecrementRefCount();


	// Plugin functions available to EPL code

	/**
	 * Set the colour and intensity of a Blinkt LED. This function just
	 * changes internal plugin state. Use the refresh() function to
	 * actually update the Blinkt LEDs.
	 *
	 * @param led (integer) The LED number to set, starting from zero.
	 * @param red (integer) The red component of the RGB colour value.
	 * @param green (integer) The green component of the RGB colour value.
	 * @param blue (integer) The blue component of the RGB colour value.
	 * @param intensity (float) The global intensity of the LED.
	 */
	static void AP_PLUGIN_CALL setLED(const AP_Context& ctx, const AP_TypeList& args, AP_Type& rval, AP_TypeDiscriminator rtype);

	/**
	 * Set the colour and intensity of all Blinkt LEDs. This function just
	 * changes internal plugin state. Use the refresh() function to
	 * actually update the Blinkt LEDs.
	 *
	 * @param red (integer) The red component of the RGB colour value.
	 * @param green (integer) The green component of the RGB colour value.
	 * @param blue (integer) The blue component of the RGB colour value.
	 * @param intensity (float) The global intensity of the LEDs.
	 */
	static void AP_PLUGIN_CALL setAll(const AP_Context& ctx, const AP_TypeList& args, AP_Type& rval, AP_TypeDiscriminator rtype);

	/**
	 * Set the intensity of a Blinkt LED, leaving the colour unchanged.
	 * This function just changes internal plugin state. Use the refresh()
	 * function to actually update the Blinkt LEDs.
	 *
	 * @param led (integer) The LED number to set, starting from zero.
	 * @param intensity (float) The global intensity of the LED.
	 */
	static void AP_PLUGIN_CALL setIntensity(const AP_Context& ctx, const AP_TypeList& args, AP_Type& rval, AP_TypeDiscriminator rtype);

	/**
	 * Set the intensity of all Blinkt LEDs, leaving the colour unchanged.
	 * This function just changes internal plugin state. Use the refresh()
	 * function to actually update the Blinkt LEDs.
	 *
	 * @param intensity (float) The global intensity of the LEDs.
	 */
	static void AP_PLUGIN_CALL setIntensityAll(const AP_Context& ctx, const AP_TypeList& args, AP_Type& rval, AP_TypeDiscriminator rtype);

	/**
	 * Update all the Blinkt LEDs to match the internal colour and
	 * intensity state of the plugin, making the effects of all previous
	 * set*() calls visible.
	 */
	static void AP_PLUGIN_CALL refresh(const AP_Context& ctx, const AP_TypeList& args, AP_Type& rval, AP_TypeDiscriminator rtype);

	/**
	 * Set all Blinkt LEDs to no colour (red, green and blue all zero) and
	 * zero intensity. The refresh() action must be call to make the
	 * effects of a reset visible.
	 */
	static void AP_PLUGIN_CALL reset(const AP_Context& ctx, const AP_TypeList& args, AP_Type& rval, AP_TypeDiscriminator rtype);

	/**
	 * Sleep (busy wait) for a specified time.
	 *
	 * @param millis (integer) The number of milliseconds to sleep for.
	 */
	static void AP_PLUGIN_CALL delay(const AP_Context& ctx, const AP_TypeList& args, AP_Type& rval, AP_TypeDiscriminator rtype);

	/**
	 * Enable or disable debugging output from the plugin. When enabled,
	 * debug output is sent to stdout.
	 *
	 * @param enable (boolean) True to enable debug output, false to
	 * disable it.
	 * @return (boolean) The previous value of the debug flag.
	 */
	static void AP_PLUGIN_CALL enableDebug(const AP_Context& ctx, const AP_TypeList& args, AP_Type& rval, AP_TypeDiscriminator rtype);

	/**
	 * Enable or disable debugging output from the plugin. When enabled,
	 * debug output is sent to stdout.
	 *
	 * @param enable (boolean) True to enable debug output, false to
	 * disable it.
	 * @return (boolean) The previous value of the debug flag.
	 */
	static void AP_PLUGIN_CALL enableResetOnUnload(const AP_Context& ctx, const AP_TypeList& args, AP_Type& rval, AP_TypeDiscriminator rtype);

	/**
	 * Get the GPIO pin number of the Blinkt DAT (data) pin. Can be passed
	 * to the GPIO plugin Setup object to configure this pin as an output.
	 *
	 * @return (integer) The GPIO number of the DAT pin.
	 */
	static void AP_PLUGIN_CALL getDAT(const AP_Context& ctx, const AP_TypeList& args, AP_Type& rval, AP_TypeDiscriminator rtype);

	/**
	 * Get the GPIO pin number of the Blinkt CLK (clock) pin. Can be
	 * passed to the GPIO plugin Setup object to configure this pin as an
	 * output.
	 *
	 * @return (integer) The GPIO number of the CLK pin.
	 */
	static void AP_PLUGIN_CALL getCLK(const AP_Context& ctx, const AP_TypeList& args, AP_Type& rval, AP_TypeDiscriminator rtype);

private:
	// Plugin function parameter types
	static const char8* setLEDParamTypes[];
	static const char8* setAllParamTypes[];
	static const char8* setIntensityParamTypes[];
	static const char8* setIntensityAllParamTypes[];
	static const char8* refreshParamTypes[];
	static const char8* resetParamTypes[];
	static const char8* delayParamTypes[];
	static const char8* enableDebugParamTypes[];
	static const char8* enableResetOnUnloadParamTypes[];
	static const char8* getDATParamTypes[];
	static const char8* getCLKParamTypes[];

	// Plugin function table
	static AP_Function Functions[];

	// Plugin reference count
	static unsigned RefCount;

	// Reset-on-unload flag
	static bool ResetOnUnload;

	// Global lock for all plugin functions
	static pthread_mutex_t Mutex;

	// Lock the global mutex
	static void Lock();

	// Unlock the global mutex
	static void Unlock();
};

#endif // _BLINKT_PLUGIN_H
