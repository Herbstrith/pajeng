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
#ifndef __PAJERASTROREADER_H__
#define __PAJERASTROREADER_H__
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
#include "PajeRastroTraceEvent.h"
#include "PajeDefinitions.h"
#define __STDC_FORMAT_MACROS
#include <inttypes.h>

extern "C"
{
  #include <rastro.h>
}

class PajeRastroReader : public PajeComponent {
private:
  enum { OUT_DEF, IN_DEF } defStatus;
  PajeEventDefinition *eventBeingDefined;
  std::map<u_int32_t,PajeEventDefinition*> eventDefinitions;
  rst_rastro_t rastro;
  rst_event_t rst_event;
  bool moreData;
  void scanDefinitionLine(u_int32_t definitionArray[], u_int32_t size);
  PajeRastroTraceEvent* scanEventLine (rst_event_t *event);
  void AddToParamList(rst_event_t *reference);
  long long currentEvent;
  PajeDefinitions *defs;

  struct StringParamsList{
    char* string;
    short string_position;
    struct  StringParamsList* next;
  };
  StringParamsList *stringList;

public:
  PajeRastroReader (PajeDefinitions *definitions, char *file_rst);
  ~PajeRastroReader (void);
  
  bool hasMoreData (void);
  void readNextChunk ();
};
#endif
