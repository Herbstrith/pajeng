/*
    This file is part of PajeNG

    PajeNG is free software: you can redistribute it and/or modify
    it under the terms of the GNU Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    PajeNG is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Public License for more details.

    You should have received a copy of the GNU Public License
    along with PajeNG. If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __PAJEEVENTDECODER_H__
#define __PAJEEVENTDECODER_H__
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "PajeComponent.h"
#include "PajeEventDefinition.h"
#include "PajeTraceEvent.h"
#include "PajeDefinitions.h"
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
/*
extern "C"
{
  #include <rastro.h>

}
*/



class PajeBinaryReader : public PajeComponent {
private:
  enum { OUT_DEF, IN_DEF } defStatus;
  PajeEventDefinition *eventBeingDefined;
  std::map<std::string,PajeEventDefinition*> eventDefinitions;
  rst_rastro_t rastro;  
  
  void scanDefinitionLine (u_int32_t definitionArray[]);
  PajeTraceEvent *scanEventLine (rst_event_t event);
  long long currentEvent;
  PajeDefinitions *defs;

public:
  PajeBinaryReader (PajeDefinitions *definitions);
  ~PajeBinaryReader (void);

  void readNextChunk ();
};
#endif
