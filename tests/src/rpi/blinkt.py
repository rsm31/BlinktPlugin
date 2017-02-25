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
# Base class for all BlinktPlugin PySys tests.
#

from pysys.constants import *
from pysys.basetest import BaseTest
from apama.correlator import CorrelatorHelper
from os.path import join

ApamaHomeMonitors = join(PROJECT.APAMA_HOME, 'monitors')
ApamaWorkMonitors = join(PROJECT.APAMA_WORK, 'monitors')
BlinktTestObjects = join(PROJECT.ROOTDIR, 'objects')

class BlinktBaseTest(BaseTest):

	def __init__(self, descriptor, outdir, runner):
		BaseTest.__init__(self, descriptor, outdir, runner)

		# Initialise a correlator with the Blinkt plugin, the plugin
		# wrapper and other required supporting code.
		self.correlator = CorrelatorHelper(self, name='BlinktCorrelator')
		process = self.correlator.start()
		self.correlatorLog = process.stdout
		self.correlatorOut = process.stdout
		self.correlatorErr = process.stderr
		self.correlator.injectEPL(filenames=[
			join(ApamaHomeMonitors, 'TimeFormatEvents.mon'),
			join(ApamaWorkMonitors, 'GPIOPlugin.mon'),
			join(ApamaWorkMonitors, 'BlinktHelper.mon')
		])
		self.waitForSignal(self.correlatorLog, expr="Blinkt initialised")

