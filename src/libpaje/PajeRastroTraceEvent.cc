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
  pajeEventDefinition = def;

  std::list<PajeField>::iterator itFields = def->fields.begin();
  std::list<PajeFieldType>::iterator itTypes = def->types.begin();
  int i=0;
  //first field is the event type
  itFields++;
  itTypes++;
  int int_mark =0,double_mark =0,string_mark =0;
  
   //   printf("\n list size %d %d %d\n ", def->fields.size(),*itFields,*itTypes);


  
  while (itFields != def->fields.end()){ 
  
    
    if(*itTypes == PAJE_string || *itTypes == PAJE_color){
      definitionOrder[i] = PAJE_string;    
      paje_field[*itFields] = string_mark;
      string_mark++;
    }
    
    if(*itTypes == PAJE_double || *itTypes == PAJE_date){
      definitionOrder[i] = PAJE_double;
      paje_field[*itFields] = double_mark;		
      double_mark++;

    }
    
    if(*itTypes == PAJE_int){
      definitionOrder[i] = PAJE_int;
      paje_field[*itFields] = int_mark;		
      int_mark++;

    }  
      itFields++;
      itTypes++;
      i++;
  }

  pajeEventDefinition = def;

  memcpy(v_uint32 , event->v_uint32,sizeof(event->v_uint32));
  memcpy(v_string , event->v_string,sizeof(event->v_string)); 
  memcpy(v_double , event->v_double,sizeof(event->v_double));  
  
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
  free(v_uint32);
  free(v_string);
  free(v_double);
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
  char* value = "-1";  
  value = v_string[paje_field[field]];
  return value;
}

int PajeRastroTraceEvent::valueForIntField(PajeField field)
{
  int value;
  value = v_uint32[paje_field[field]];
  return value;
}
double PajeRastroTraceEvent::valueForDoubleField(PajeField field)
{
  double value;
  value = v_double[paje_field[field]];
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
