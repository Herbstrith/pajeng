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
#include "PajeRastroTraceEvent.h"

PajeRastroTraceEvent::PajeRastroTraceEvent ()
{
  pajeEventDefinition = NULL;
}

PajeRastroTraceEvent::PajeRastroTraceEvent (PajeEventDefinition *def)
{
  pajeEventDefinition = def;
  int i=0;
  std::list<PajeField>::iterator itf = def->fields.begin();
  std::list<PajeFieldType>::iterator itt = def->types.begin();

  //first field is the event type
  itf++;
  itt++;

  while (itf != def->fields.end()){   
    definitionOrder[i]= def->types(i);
    itf++;
    itt++;
    i++;
  }
  pajeEventDefinition = def;
}

PajeRastroTraceEvent::~PajeRastroTraceEvent ()
{
  str_fields.clear();
  double_fields.clear();
  int_fields.clear();
}

PajeEventId PajeRastroTraceEvent::pajeEventId (void)
{
  return pajeEventDefinition->pajeEventIdentifier;
}

void PajeRastroTraceEvent::addField (char *field)
{
  str_fields.push_back (std::string(field));
}
void PajeRastroTraceEvent::addField (double field)
{
  double_fields.push_back (field);
}
void PajeRastroTraceEvent::addField (int field)
{
  int_fields.push_back (field);
}

void PajeRastroTraceEvent::clear (void)
{
  pajeEventDefinition = NULL;
  str_fields.clear();
  double_fields.clear();
  int_fields.clear();
}

/*
bool PajeRastroTraceEvent::check (paje_line *line)
{
  if (!pajeEventDefinition) return false;
  if ((str_fields.size() + double_fields.size() + int_fields.size()) != pajeEventDefinition->fieldCount()){
    std::stringstream st;
    if (line){
      st << *line;
    }else{
      st << this->line;
    }
    std::string lreport = st.str();
    std::cout << *pajeEventDefinition << std::endl;
    std::cout << "Line field count: " << str_fields.size() + double_fields.size() + int_fields.size() << std::endl;
    std::cout << "Definition field count: " << pajeEventDefinition->fieldCount() << std::endl;
    std::cout << "Field count does not match definition for line "+lreport << std::endl;
    return false;
  }else{
    return true;
  }
}
*/

T PajeRastroTraceEvent::valueForField (PajeField field)
{
  int index = pajeEventDefinition->indexForField (field);
  if (index == -1){
    return 0;
  }else{
    return double_fields.at(index);
  }
}






std::string PajeRastroTraceEvent::description (void) const
{
  std::stringstream output;
  unsigned int i;
  output << ", Fields: '" << str_fields.size() + double_fields.size() + int_fields.size();
  output << ", Contents: '";
  for (i = 0; i < str_fields.size() + double_fields.size() + int_fields.size(); i++){
    output << fields.at(i);
    if (i+1 != str_fields.size() + double_fields.size() + int_fields.size()) output << " ";
  }
  output << "')";
  return output.str();
}

PajeEventDefinition *PajeRastroTraceEvent::definition (void)
{
  return pajeEventDefinition;
}

void PajeRastroTraceEvent::setDefinition (PajeEventDefinition *def)
{
  pajeEventDefinition = def;
}

std::ostream &operator<< (std::ostream &output, const PajeRastroTraceEvent &event)
{
  output << event.description();
  return output;
}
