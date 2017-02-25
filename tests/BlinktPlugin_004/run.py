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

from pysys.constants import *
from apama.correlator import CorrelatorHelper
from rpi.blinkt import *

class PySysTest(BlinktBaseTest):

	def execute(self):
		self.correlator.injectEPL(filedir=BlinktTestObjects, filenames=['BlinktRainbow.mon'])
		self.correlator.injectEPL(filenames=['Test.mon'])
		self.waitForSignal(self.correlatorLog, expr='TEST COMPLETE', timeout=90)

	def validate(self):
		self.assertLineCount(self.correlatorLog, expr="Forward BlinktRainbow\(.*\) started", condition="==3")
		self.assertLineCount(self.correlatorLog, expr="Reverse BlinktRainbow\(.*\) started", condition="==3")
		self.assertLineCount(self.correlatorLog, expr="BlinktRainbow stopped", condition="==6")

		for i in range(1, 9):
			self.assertGrep(self.correlatorLog, expr="Test step %i complete" % i)

