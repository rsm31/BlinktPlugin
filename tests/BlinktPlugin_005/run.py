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
from rpi.blinkt import BlinktBaseTest

class PySysTest(BlinktBaseTest):

	def execute(self):
		self.correlator.injectEPL(filenames=['Test.mon'])
		self.waitForSignal('BlinktCorrelator.out', expr='TEST COMPLETE')

	def validate(self):
		for i in range(1, 4):
			self.assertGrep('BlinktCorrelator.out', expr="Test step %i complete" % i)

