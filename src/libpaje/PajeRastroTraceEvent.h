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
#ifndef __PAJETRACEEVENT_H__
#define __PAJETRACEEVENT_H__
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "PajeEventDefinition.h"
#include "PajeObject.h"

//#define RST_MAX_FIELDS_PER_TYPE  10
//#define   RST_MAX_STRLEN 120

extern "C"
{
  #include <rastro.h>

}
union T
{
    int i;
    char *s;
    double d;
};

class PajeRastroTraceEvent : public PajeObject {
 private:
  PajeEventDefinition *pajeEventDefinition;
  
  char v_string[RST_MAX_FIELDS_PER_TYPE][RST_MAX_STRLEN];
  //u_int8_t v_uint8[RST_MAX_FIELDS_PER_TYPE];
  //u_int16_t v_uint16[RST_MAX_FIELDS_PER_TYPE];
  u_int32_t v_uint32[RST_MAX_FIELDS_PER_TYPE];
  //u_int64_t v_uint64[RST_MAX_FIELDS_PER_TYPE];
  //float v_float[RST_MAX_FIELDS_PER_TYPE];
  double v_double[RST_MAX_FIELDS_PER_TYPE];

  std::vector<char*>str_fields;
  std::vector<double> double_fields;
  std::vector<int> int_fields;
  
  int fieldOrder[10];
  int definitionOrder[10];
  std::map <PajeField, int> paje_field;
 public:
  PajeRastroTraceEvent ();
  PajeRastroTraceEvent (PajeEventDefinition *def,rst_event_t *event);
  ~PajeRastroTraceEvent ();
  PajeEventId pajeEventId (void);
  void addField (char *field);
  void addField (double field);
  void addField (int field);
  void clear (void);
  bool check (paje_line *line);
  T valueForField (PajeField field);
  std::string valueForExtraField (std::string fieldName);
  std::string description (void) const;
  PajeEventDefinition *definition (void);
  void setDefinition (PajeEventDefinition *def);

  char* valueForStringField(PajeField field);

  int valueForIntField(PajeField field);

  double valueForDoubleField(PajeField field);

};

std::ostream &operator<< (std::ostream &output, const PajeRastroTraceEvent &event);

#endif

