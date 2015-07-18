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
#ifndef __PAJERASTROTRACEEVENT_H__
#define __PAJERASTROTRACEEVENT_H__
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "PajeTraceEvent.h"
#include "PajeEventDefinition.h"
#include "PajeObject.h"

extern "C"
{
  #include <rastro.h>
}


class PajeRastroTraceEvent : public PajeTraceEvent {
 private:
  PajeEventDefinition *pajeEventDefinition;
  
  std::vector<char *> v_string_ref;  
  char v_string[RST_MAX_FIELDS_PER_TYPE][RST_MAX_STRLEN];
  u_int32_t v_uint32[RST_MAX_FIELDS_PER_TYPE];
  double v_double[RST_MAX_FIELDS_PER_TYPE];
  int fieldOrder[10];
  int definitionOrder[10];
  std::map <PajeField, int> *paje_field;
  bool useRastroRef;
  
 public:
  PajeRastroTraceEvent ();
  PajeRastroTraceEvent (PajeEventDefinition *def,rst_event_t *event);
  PajeRastroTraceEvent (PajeEventDefinition *def,rst_event_t *event,std::vector<char*> str_refs);
  ~PajeRastroTraceEvent ();
  PajeEventId pajeEventId (void);
  void clear (void);
  bool check (rst_event_t line);
  std::string description (void) const;
  PajeEventDefinition *definition (void);
  void setDefinition (PajeEventDefinition *def);

  char* valueForStringField(PajeField field);
  int valueForIntField(PajeField field);
  double valueForDoubleField(PajeField field);

};

std::ostream &operator<< (std::ostream &output, const PajeRastroTraceEvent &event);

#endif

