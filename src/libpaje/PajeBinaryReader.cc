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
#include "PajeBinaryReader.h"
#include "PajeException.h"
#include "PajeDefinitions.h"
#include <stdlib.h>

PajeBinaryReader::PajeBinaryReader (PajeDefinitions *definitions, char *file_rst)
{

  defStatus = OUT_DEF;
  currentEvent = 0;

  moreData = true;
  defs  = definitions;

  bzero(&rastro, sizeof(rst_rastro_t));
  //open rst_file
  int status = rst_open_file (&rastro, 10000000,
                              file_rst,
                              (char*)"out.txt");
                             
                              
  if (status == RST_NOK){
    //TODO: throw Exception
    printf("error at openning of the rst_file\n");
  }

}

PajeBinaryReader::~PajeBinaryReader ()
{
  eventDefinitions.clear ();
  /* closing the file */
  rst_close (&rastro);
}


paje_line *poti_print_event (rst_event_t *event,PajeEventDefinition *eventDefinition)
{
  int event_id;
  event_id = event->type;
  paje_line *paje_line_string = new paje_line;
  paje_line_string->word_count = 0;		
  char temp[3];
  sprintf(temp , "%d" , event_id);
  paje_line_string->word[0] = temp;
  std::list<PajeField>::const_iterator ff;
  ff = eventDefinition->fields.begin();

	int f;
	int double_mark=0,int_mark=0,float_mark=0,string_mark = 0;
  
  //paje_line string build-up
  std::list<PajeFieldType>::iterator i = eventDefinition->types.begin();
  i++;
  
	for(f = 1; f < eventDefinition->fields.size();f++)
	{

		if((*i  ==  PAJE_string || *i ==  PAJE_color) && f == eventDefinition->fields.size()-1){

      paje_line_string->word[f] = event->v_string[string_mark];

      paje_line_string->word_count++;
			break;


		}
		if((*i== PAJE_double || *i == PAJE_date) && f== eventDefinition->fields.size()-1){
       char temp[50];
       snprintf(temp ,50, "%lf" , event->v_double[double_mark]);
       paje_line_string->word[f] = temp;
   // sprintf(temp , "%d" , event_id);

      paje_line_string->word_count++;       
			break;

		}				
	
		if(*i == PAJE_string || *i == PAJE_color){
      paje_line_string->word[f] = event->v_string[string_mark];
			string_mark = string_mark +1;
		}
		if(*i == PAJE_double || *i == PAJE_date){
      char temp[50];
      sprintf(temp , "%lf" , event->v_double[double_mark]);
      paje_line_string->word[f] = temp;
      double_mark = double_mark +1;
  	}
    paje_line_string->word_count++;
		i++;

	}
  
  paje_line_string->word_count++;
  paje_line_string->lineNumber=0;  
  return paje_line_string;

}

void PajeBinaryReader::scanDefinitionLine(u_int32_t definitionArray[], u_int32_t size)
{         
                        

  int n = 1;
                                                //PajeEventId,eventid number,line,defs
  eventBeingDefined = new PajeEventDefinition((PajeEventId)(definitionArray[0]),(int)(definitionArray[0]), currentEvent,defs);
 
  for (; n < size; n=n+2) {
      eventBeingDefined->addField((PajeField)(definitionArray[n]),(PajeFieldType)(definitionArray[n+1]),0);
		}

  eventDefinitions[(int)(definitionArray[0])] = eventBeingDefined;

}


PajeTraceEvent *PajeBinaryReader::scanEventLine (rst_event_t *event)
{
  int eventId = -1;
  PajeEventDefinition *eventDefinition = NULL;
  eventId = event->type;

  std::map<u_int32_t,PajeEventDefinition*>::iterator it;
  it = eventDefinitions.find(eventId);
  eventDefinition =  it->second;
  
  paje_line *pajeLine = poti_print_event(event, it->second);  
  int i = 0;

  if (eventDefinition == NULL) { 
    throw PajeDecodeException ("Event with id '"+std::string("%d",eventId)+"' has not been defined");
  }

  return new PajeTraceEvent (eventDefinition,pajeLine); 
}

bool PajeBinaryReader::hasMoreData()
{
  return moreData;
}




//called by the PajeThreadReader
void PajeBinaryReader::readNextChunk ()
{


  /* reading the file */
  if(rst_decode_event(&rastro, &rst_event)){

    int PajeHeaderEventId = 999;
    //header definition
    if(rst_event.type == PajeHeaderEventId ){
      PajeBinaryReader::scanDefinitionLine(rst_event.v_uint32,rst_event.ct.n_uint32);
     
    }

    //event
    else{
      PajeTraceEvent *event = PajeBinaryReader::scanEventLine(&rst_event);
      if (event != NULL){
        PajeComponent::outputEntity(event);
        delete event;
        currentEvent++;
      }

    }
  }
  else{
    moreData = false;
  }


}

