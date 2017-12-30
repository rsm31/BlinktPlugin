#
# Copyright (c) 2016-2017 Scott Mitchell.
# All rights reserved.
#
# Licenced under the BSD 3-Clause licence (the "Licence"); you may not use
# this file except in compliance with the Licence. You may obtain a copy of
# the Licence from the LICENCE file in the top level of this software
# distribution or from:
#
# 	https://opensource.org/licenses/BSD-3-Clause
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
# License for the specific language governing permissions and limitations
# under the License.
#

#
# Makefile for the BlinktPlugin and associated support code.
#

LDLIBS = -lwiringPi
PLUGIN_LIBS = -lapclient

PLUGIN_CPPFLAGS = -I$(APAMA_HOME)/include
PLUGIN_CXXFLAGS = --std=c++11 -fPIC
PLUGIN_LDFLAGS = -shared -L$(APAMA_HOME)/lib

LIBDIR = $(APAMA_WORK)/lib
MONDIR = $(APAMA_WORK)/monitors


all: blinkt_test blinkt_reset libBlinktPlugin.so


blinkt_test: blinkt_test.o blinkt_functions.o

blinkt_reset: blinkt_reset.o blinkt_functions.o

libBlinktPlugin.so: BlinktPlugin.o blinkt_functions.o
	$(CC) $(PLUGIN_LDFLAGS) $+ $(LDLIBS) $(PLUGIN_LIBS) -o $@


blinkt_test.o: blinkt_test.cpp

blinkt_reset.o: blinkt_reset.cpp

blinkt_functions.o: blinkt_functions.cpp blinkt_functions.h

BlinktPlugin.o: BlinktPlugin.cpp BlinktPlugin.h
	$(CXX) $(PLUGIN_CPPFLAGS) $(PLUGIN_CXXFLAGS) -c $< -o $@


install: all
	mkdir -p $(LIBDIR)
	mkdir -p $(MONDIR)
	cp libBlinktPlugin.so $(LIBDIR)
	cp BlinktHelper.mon $(MONDIR)

doc: .dummy
	ant blinkt-doc

clean:
	-rm *.o blinkt_test blinkt_reset libBlinktPlugin.so
	-rm apamadoc_output.log
	-rmdir logs
	-rm *~

.dummy:
