# BlinktPlugin

Apama plugin for Raspberry Pi Blinkt! LED module


## Introduction

The Blinkt! is a strip of 8 APA102C RGB LED modules that can be attached to the 40-pin GPIO connector on a Raspberry Pi Model B and controlled using a simple two-wire SPI protocol. The `BlinktPlugin` software makes the Blinkt! accessible to Apama EPL developers through an EPL plugin and helper object. The goal is to demonstrate how to straightforwardly integrate real hardware with Apama and control it from EPL.

The APA102C LED modules used in the Blinkt! are controllable for both colour and overall intensity or brightness. Colour values for the LEDs are RGB (red, green, blue) triplets with a range of zero to 255 inclusive. The plugin handles values outside this range by simply using the least significant 8 bits of the integer as the colour value and ignoring the rest.

Intensity values for the LEDs are represented as floating point numbers with a range of 0.0 to 1.0 inclusive. Values less than zero will cause the intensity to be left unchanged, although using the `setRGB()` or `setAllRGB()` actions on the helper object is an easier way to change the colour of the LEDs without affecting the intensity. Values greater than 1.0 are treated as though they were 1.0, i.e. maximum intensity.

The plugin keeps an internal colour/intensity state for each LED that is only transferred to the real LEDs when the `refresh()` action is called. This is the only way to change the state of the physical LEDs. The effects of any number of preceding `set*()` and `reset()` actions will become visible only when `refresh()` is called.

For more information on the Blinkt! hardware and other language APIs see:
- https://shop.pimoroni.com/products/blinkt (Blinkt! product page)
- https://github.com/pimoroni/blinkt (Blinkt! Python library GitHub project)
- https://pinout.xyz/pinout/blinkt (Blinkt! pinout diagram)
- https://cdn-shop.adafruit.com/datasheets/APA102.pdf (APA102C datasheet)


## Files

- [`README.md`](README.md) - This file.
- [`LICENCE`](LICENCE) - Licence and copyright information.
- [`blinkt_functions.h`](blinkt_functions.h), [`blinkt_functions.cpp`](blinkt_functions.cpp) - A set of C++ functions for controlling the Blinkt! hardware, inspired by the Blinkt Python API.
- [`BlinktPlugin.h`](BlinktPlugin.h), [`BlinktPlugin.cpp`](BlinktPlugin.cpp) - Apama EPL plugin to expose the `blinkt_functions` functionality to EPL developers.
- [`BlinktHelper.mon`](BlinktHelper.mon) - Apama EPL helper/wrapper object for the `BlinktPlugin`. In general the helper should be used in preference to directly calling the plugin functions.
- [`blinkt_setup`](blinkt_setup) - Script to configure the Raspberry Pi GPIO pins for the Blinkt! hardware.
- [`blinkt_reset.cpp`](blinkt_reset.cpp), [`blinkt_test.cpp`](blinkt_test.cpp) - Simple C++ programs using `blinkt_functions` to reset the Blinkt! or display some basic test patterns on the LEDs. The `blinkt_setup` script should be run before running either of these programs.
- [`Makefile`](Makefile) - Build and install support for `blinkt_functions`, `BlinktPlugin`, test programs and documentation.
- [`build.xml`](build.xml) - Ant script to build the ApamaDoc API documentation for the `BlinktHelper` object. 
- [`tests/`](tests) - Tests and samples implemented as `PySys` test cases.
- [`doc/`](doc) - Pre-built ApamaDoc API documentation for the `BlinktHelper` object.


## Requirements

This software requires the following:

1. A Raspberry Pi with a 40-pin GPIO header. This software was developed on a Raspberry Pi 3 Model B Rev 1.2, and any Pi 3, 4 or Zero model should also work.

2. A Blinkt! RGB LED board attached to the Raspberry Pi GPIO connector.

3. A Linux OS distribution compatible with Apama on the Raspberry Pi. This software was developed using Raspberry Pi OS (December 2020 build, based on Debian Buster, kernel version 5.4, gcc 8.3.0) and other similar distributions should also work. Raspberry Pi OS images can be downloaded from https://www.raspberrypi.org/downloads/

4. The `wiringPi` library. This is installed by default on Raspberry Pi OS and may be in the package repositories for other Raspberry Pi Linux distributions. Unfortunately the original `wiringPi` project at http://wiringpi.com/ has been deprecated but an unofficial mirror at https://github.com/WiringPi/ is being maintained.

5. The Apama streaming analytics engine, version 10.5 or higher. The Apama Core Community Edition for Raspberry Pi can be downloaded from https://www.apamacommunity.com/downloads/.

6. The `GPIOPlugin` for Apama 10.x, available from https://github.com/CallumAttryde/apama_GPIO. The plugin should be built and installed somewhere that it can be loaded by your Apama installation, typically under the `$APAMA_WORK` directory.  Note that `$APAMA_WORK/lib` is on the default Apama shared library path so plugins installed here can be used without any additional configuration. More information about the plugin can be found at https://www.apamacommunity.com/innovation-week-results/.


## Build & Installation

Building and installing the `BlinktPlugin` is straightforward. All of the shell commands below can be run as a normal user; root privileges are not required.

1. Ensure that all the required hardware and software listed in the previous section is installed and working.

2. Set up the Apama environment, where `$APAMA_HOME` is the root of your Apama installation:
  ```
  $ source $APAMA_HOME/bin/apama_env
  ```

3. Change directory to the root of the `BlinktPlugin` distribution and build everything:
  ```
  $ cd BlinktPlugin
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
```
$ cd tests
```

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

By default the tests will use the BlinktPlugin library and EPL wrapper from the project directory, which should be the latest, not necessarily installed, version. To use the version that has been installed under `$APAMA_WORK`, add the `-XTEST_AGAINST_INSTALL` option to your `pysys run` command, for example:

  ```
  $ pysys run -XTEST_AGAINST_INSTALL BlinktPlugin_002
  ```

Use `pysys -h` or `pysys <command> -h`, e.g. `pysys run -h` for more information about using PySys see https://github.com/pysys-test/pysys-test, or the detailed API documentation at https://pysys-test.github.io/pysys-test.


## Documentation

API documentation for the `BlinktHelper` object can be found in the [`doc`](doc) subdirectory of the `BlinktPlugin` distribution. Point your web browser at the [`index.html`](doc/index.html) file in this directory. The API documentation is built from the ApamaDoc comments in the `BlinkHelper` source code. If necessary this documentation can be rebuilt by running:

  ```
  $ ant blinkt-doc
  ```

from the top level of the `BlinktPlugin` distribution, in a shell with the Apama environment configured.

**NOTE:** You will need a full installation of Apama (commercial or community, but _not_ the Core Community edition) to run the documentation build, as it requires some components that are only available in the full edition. The full edition of Apama is not currently available for the Raspberry Pi, so any rebuild of the documentation will need to be done on a different machine.

The `BlinktPlugin` itself and the underlying `blinkt_functions` are documented in their respective source code. The documentation comments should be compatible with Doxygen although this documentation is not currently built automatically.


## Copyright & Licence

Copyright (c) 2016-2020 Scott Mitchell.
All rights reserved.

Licenced under the BSD 3-Clause licence (the "Licence"); you may not use this file except in compliance with the Licence. You may obtain a copy of the Licence from the LICENCE file in the top level of this software distribution or from:

https://opensource.org/licenses/BSD-3-Clause

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the License for the specific language governing permissions and limitations under the License.
