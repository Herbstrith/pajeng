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

union T
{
    int i;
    char *s;
    double d;
};

class PajeRastroTraceEvent : public PajeObject {
 private:
  PajeEventDefinition *pajeEventDefinition;
  std::vector<char*>str_fields;
  std::vector<double> double_fields;
  std::vector<int> int_fields;
  int fieldOrder[10];
  int definitionOrder[10];
public:
  PajeRastroTraceEvent ();
  PajeRastroTraceEvent (PajeEventDefinition *def);
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
};

std::ostream &operator<< (std::ostream &output, const PajeRastroTraceEvent &event);

#endif

