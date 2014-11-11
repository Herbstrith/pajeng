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
  int int_mark =0,double_mark =0,string_mark =0;
  while (itf != def->fields.end()){ 
	if(*itt == PAJE_string || *itt == PAJE_color){
		definitionOrder[i]= string_mark;
		string_mark++;
	}
	
	if(*itt == PAJE_double || *itt == PAJE_date){
		definitionOrder[i]= double_mark;
		double_mark++;
	}
	
	if(*itt == PAJE_int){
		definitionOrder[i]= int_mark;
		int_mark++;
	}
	  
    fieldOrder[i] = *itf;
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
  str_fields.push_back (field);
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
  T return_value;
  return_value.i = -1;
  return_value.d = -1;
  return_value.s = "-1";
  int index = pajeEventDefinition->indexForField (field);
  if (index == -1){
    return return_value;
  }else{
	int type = definitionOrder[index];
	  
	return_value.d = double_fields.at(index);
	return_value.i = int_fields.at(index);
	return_value.s = str_fields.at(index);
    return return_value;
  }
}






std::string PajeRastroTraceEvent::description (void) const
{
  std::stringstream output;
  unsigned int i;
  output << ", Fields: '" << str_fields.size() + double_fields.size() + int_fields.size();
  output << ", Contents: '";
  
  for (int def_order =0, i = 0; i < str_fields.size() + double_fields.size() + int_fields.size(); i++,def_order++){
	if(definitionOrder[def_order] == PAJE_string ||definitionOrder[def_order] == PAJE_color)
		output << str_fields.at(i);
	if(definitionOrder[def_order] == PAJE_double || definitionOrder[def_order] == PAJE_date)
		output << double_fields.at(i);
	if(definitionOrder[def_order] == PAJE_int )
		output << int_fields.at(i);
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
