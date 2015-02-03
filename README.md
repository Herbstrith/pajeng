PajeNG - Trace Visualization Tool
==================================

PajeNG (Paje Next Generation) is a re-implementation (in C++) and
direct heir of the well-known [Paje visualization
tool](http://paje.sf.net) for the analysis of execution traces (in the
[Paje File
Format](http://paje.sourceforge.net/download/publication/lang-paje.pdf))
through trace visualization (space/time view).  The tool is released
under the [GNU General Public License
3](http://www.gnu.org/licenses/gpl.html). PajeNG comprises the
__libpaje__ library, the space-time visualization tool in __pajeng__
and a set of auxiliary tools to manage Paje trace files (such as
__pj_dump__ and __pj_validate__). It was started as part of the french
[INFRA-SONGS ANR
project](http://infra-songs.gforge.inria.fr/). Development has
continued at [INF/UFRGS](http://www.inf.ufrgs.br/en/).


### Detailed Installation and everything else

[Check the PajeNG's wiki](https://github.com/schnorr/pajeng/wiki/).


What i am working at:
==================================
Make PajeNG work with the new binary trace format(librastro+poti), which aims to increase the performance of the tool.
To use the new format as input, one must use 'r' and the .rst file as parameters.Ex "./pj_validate -r rastro_file.rst"
--working with pj_validate.
--better performance if using the slightly modified librastro from my repository.


