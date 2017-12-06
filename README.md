# BlinktPlugin
Apama plugin for Raspberry Pi Blinkt! LED module


## Introduction

The Blinkt! is a strip of 8 APA102 RGB LED modules that can be attached to the 40-pin GPIO connector on a Raspberry Pi Model B and controlled using a simple two-wire SPI protocol. The `BlinktPlugin` software makes the Blinkt! accessible to Apama EPL developers through an EPL plugin and helper object. The goal is to demonstrate how to straightforwardly integrate real hardware with Apama and control it from EPL.

The APA102 LED modules used in the Blinkt! are controllable for both colour and overall intensity or brightness. Colour values for the LEDs are RGB (red, green, blue) triplets with a range of zero to 255 inclusive. The plugin handles values outside this range by simply using the least significant 8 bits of the integer as the colour value and ignoring the rest.

Intensity values for the LEDs are represented as floating point numbers with a range of 0.0 to 1.0 inclusive. Values less than zero will cause the intensity to be left unchanged, although using the `setRGB()` or `setAllRGB()` actions on the helper object is an easier way to change the colour of the LEDs without affecting the intensity. Values greater than 1.0 are treated as though they were 1.0, i.e. maximum intensity.

The plugin keeps an internal colour/intensity state for each LED that is only transferred to the real LEDs when the `refresh()` action is called. This is the only way to change the state of the physical LEDs. The effects of any number of preceding `set*()` and `reset()` actions will become visible only when `refresh()` is called.

For more information on the Blinkt! hardware and other language APIs see:
- https://github.com/pimoroni/blinkt (Blinkt! GitHub project)
- https://cdn-shop.adafruit.com/product-files/2343/APA102C.pdf (APA102 datasheet)


## Files

- [`README.md`](README.md) - This file.
- [`LICENCE`](LICENCE) - Licence and copyright information.
- [`blinkt_functions.h`](blinkt_functions.h), [`blinkt_functions.cpp`](blinkt_functions.cpp) - A set of C++ functions for controlling the Blinkt! hardware. Inspired by the Blinkt Python API (http://docs.pimoroni.com/blinkt/)
- [`BlinktPlugin.h`](BlinktPlugin.h), [`BlinktPlugin.cpp`](BlinktPlugin.cpp) - Apama EPL plugin to expose the `blinkt_functions` functionality to EPL developers.
- [`BlinktHelper.mon`](BlinktHelper.mon) - Apama EPL helper/wrapper object for the `BlinktPlugin`. In general the helper should be used in preference to directly calling the plugin functions.
- [`blinkt_setup`](blinkt_setup) - Script to configure the Raspberry Pi GPIO pins for the Blinkt! hardware.
- [`blinkt_reset.cpp`](blinkt_reset.cpp), [`blinkt_test.cpp`](blinkt_test.cpp) - Simple C++ programs using `blinkt_functions` to reset the Blinkt! or display some basic test patterns on the LEDs. The `blinkt_setup` script should be run before running either of these programs.
- [`Makefile`](Makefile) - Build and install support for `blinkt_functions`, `BlinktPlugin`, test programs and documentation.
- [`build.xml`](build.xml) - Ant script to build the ApamaDoc API documentation for the `BlinktHelper` object. 
- [`tests/`](tests) - Tests and samples implemented as PySys test cases.
- [`doc/`](doc) - API documentation for the `BlinktHelper` object.


## Requirements

This software requires the following:

1. A Raspberry Pi Model B, generation 2 or 3.

2. A Blinkt! RGB LED board attached to the Raspberry Pi GPIO connector.

3. A Linux OS distribution compatible with Apama on the Raspberry Pi. This software was developed and tested on a Rapberry Pi 3 running Raspian (based on Debian Stretch, kernel version 4.9.59, gcc 6.3.0) but other distributions should also work. Raspian images can be downloaded from https://www.raspberrypi.org/downloads/

4. The `wiringPi` library, which should be available by default on any modern Linux distribution for the Raspberry Pi. If necessary `wiringPi` can be downloaded from from http://wiringpi.com/

5. Apama version 10.1 or higher. The Apama Core Community Edition for Raspberry Pi can be downloaded from http://www.apamacommunity.com/downloads/.

6. The `GPIOPlugin` for Apama, compatible with the version of Apama you are using. The `GPIOPlugin` for Apama 9.12 can be downloaded from http://www.apamacommunity.com/gpio-plugin-for-apama-core-community-edition-on-a-raspberry-pi/. This version of the GPIOPlugin will also run on Apama 10.1. The plugin should be built and installed in your Apama installation.


## Build & Installation

Building and installing the `BlinktPlugin` is straightforward. All of the shell commands below can be run as a normal user; root privileges are not required.

1. Ensure that all the required hardware and software listed in the previous section is installed and working.

2. Set up the Apama environment, where `<APAMA_HOME>` is the root of your Apama installation:
  ```
  $ . <APAMA_HOME>/bin/apama_env
  ```

3. Change directory to the root of the `BlinktPlugin` distribution and build everything:
  ```
  $ make all
  ```
 
4. Assuming the build was successful, test basic Blinkt! control:
  ```
  $ ./blinkt_setup
  $ ./blinkt_test
  $ ./blinkt_reset
  ```

5. Install the EPL plugin and helper object under `$APAMA_WORK`:
  ```
  $ make install
  ```

Testing of the EPL components is covered in the next section.


## Tests & Samples

Tests and samples for the `BlinktPlugin` and `BlinktHelper` are provided as PySys test cases. PySys is a Python-based system testing framework distributed with Apama, along with extensions for testing Apama applications.

1. Move into the [`tests`](tests) subdirectory of the `BlinktPlugin` distribution. This should be done in the same shell used to build the plugin, or another shell with the Apama environment configured

2. To see the available tests with brief or verbose detail:
  ```
  $ pysys print
  $ pysys print -f
  ```
  
3. To run a single test, in this case `BlinktPlugin_001`:
  ```
  $ pysys run BlinktPlugin_001
  ```
  
4. To run all the tests:
  ```
  $ pysys run
  ```
  
Each test directory contains a `run.py` script that controls and verifies the test. The `Input` directory of most of the `BlinktPlugin` tests will contain the EPL code executed by the test (usually called `Test.mon`) and some tests will make use of the EPL samples in the `objects` directory, such as `BlinktRainbow.mon`.

Use `pysys -h` or `pysys <command> -h`, e.g. `pysys run -h` for more information about using PySys, or see the detailed API documentation at http://pysys.sourceforge.net/.


## Documentation

API documentation for the `BlinktHelper` object can be found in the [`doc`](doc) subdirectory of the `BlinktPlugin` distribution. Point your web browser at the [`index.html`](doc/index.html) file in this directory. The API documentation is built from the ApamaDoc comments in the `BlinkHelper` source code. If necessary this documentation can be rebuilt by running:

  ```
  $ ant blinkt-doc
  ```

from the top level of the `BlinktPlugin` distribution, in a shell with the Apama environment configured.

Note that you will need a full installation of Apama (commercial or community, but not the Core Community edition) to run the documentation build, as it requires some components that are only available in the full edition. The full edition of Apama is not currently available for the Raspberry Pi, so any rebuild of the documentation will need to be done on a different machine.

The `BlinktPlugin` itself and the underlying `blinkt_functions` are documented in their respective source code. The documentation comments should be compatible with Doxygen but built documentation for the C++ components is not currently provided with the `BlinktPlugin` distribution.


## Copyright & Licence

Copyright (c) 2016-2017 Scott Mitchell.
All rights reserved.

Licenced under the BSD 3-Clause licence (the "Licence"); you may not use this file except in compliance with the Licence. You may obtain a copy of the Licence from the LICENCE file in the top level of this software distribution or from:

https://opensource.org/licenses/BSD-3-Clause

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the License for the specific language governing permissions and limitations under the License.
