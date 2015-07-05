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

PajeRastroTraceEvent::PajeRastroTraceEvent (PajeEventDefinition *def,rst_event_t *event)
{  
  useRastroRef = false;
  pajeEventDefinition = def;
  paje_field = &def->paje_field;
  memcpy(definitionOrder, def->definitionOrder, sizeof(def->definitionOrder));
  pajeEventDefinition = def;
  
  if(def->int_mark > 0){
    memcpy(v_uint32 , event->v_uint32,sizeof(event->v_uint32));
  }
  if(def->string_mark > 0){
    memcpy(v_string , event->v_string,sizeof(event->v_string)); 
  }
  if(def->double_mark > 0){
    memcpy(v_double , event->v_double,sizeof(event->v_double));  
  }
}

PajeRastroTraceEvent::PajeRastroTraceEvent (PajeEventDefinition *def,rst_event_t *event,std::vector<char*> str_refs)
{
  useRastroRef = true;
  v_string_ref = str_refs;  
  pajeEventDefinition = def;
  paje_field = &def->paje_field;
  memcpy(definitionOrder, def->definitionOrder, sizeof(def->definitionOrder));
  pajeEventDefinition = def;

  if(def->int_mark > 0){
    memcpy(v_uint32 , event->v_uint32,sizeof(event->v_uint32));
  }

  if(def->double_mark > 0){
    memcpy(v_double , event->v_double,sizeof(event->v_double));  
  }
}

PajeRastroTraceEvent::~PajeRastroTraceEvent ()
{
  v_string_ref.clear();
}

PajeEventId PajeRastroTraceEvent::pajeEventId (void)
{
  return pajeEventDefinition->pajeEventIdentifier;
}


void PajeRastroTraceEvent::clear (void)
{
  pajeEventDefinition = NULL;
  free(v_uint32);
  free(v_string);
  free(v_double);
  v_string_ref.clear();
}

/*TODO: recheck this whole part
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
/*
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
*/
char* PajeRastroTraceEvent::valueForStringField(PajeField field)
{
  char* value = NULL;  
  if(useRastroRef)
    value = v_string_ref[(*paje_field)[field]];
  else 
    value = v_string[(*paje_field)[field]];
  return value;
}

int PajeRastroTraceEvent::valueForIntField(PajeField field)
{
  int value;
  value = v_uint32[(*paje_field)[field]];
  return value;
}
double PajeRastroTraceEvent::valueForDoubleField(PajeField field)
{
  double value;
  value = v_double[(*paje_field)[field]];
  return value;
}

std::string PajeRastroTraceEvent::valueForExtraField (std::string fieldName)
{
  return NULL;
}

std::string PajeRastroTraceEvent::description (void) const
{
  std::stringstream output;
  unsigned int i;
  int field_count = sizeof(v_string)/sizeof(*v_string) + sizeof(v_double)/sizeof(*v_double) + sizeof(v_uint32)/sizeof(*v_uint32);
  output << ", Fields: '" << field_count;
  output << ", Contents: '";

  int int_mark =0,double_mark =0,string_mark =0;
  for (int def_order =0, i = 0; i < field_count ; i++,def_order++){
	if(definitionOrder[def_order] == PAJE_string ||definitionOrder[def_order] == PAJE_color)
  {
		output << v_string[string_mark];
    string_mark++;
  }	
  if(definitionOrder[def_order] == PAJE_double || definitionOrder[def_order] == PAJE_date)
  {	
	  output << v_double[double_mark];
    double_mark++;
  }	
  if(definitionOrder[def_order] == PAJE_int )
  {		
    output << v_uint32[int_mark];
    int_mark++;
  }    
  
  if (i+1 != field_count) output << " ";
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
