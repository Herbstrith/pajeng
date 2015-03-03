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
#include "PajeRastroReader.h"
#include "PajeException.h"
#include "PajeRastroTraceEvent.h"
#include "PajeDefinitions.h"
#include <stdlib.h>

PajeRastroReader::PajeRastroReader (PajeDefinitions *definitions, char *file_rst)
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

PajeRastroReader::~PajeRastroReader ()
{
  eventDefinitions.clear ();
  /* closing the file */
  rst_close (&rastro);
}


//we'll try saving the fields position here so we dont have to do it for every event scan
void PajeRastroReader::scanDefinitionLine(u_int32_t definitionArray[], u_int32_t size)
{                           
  int n = 1;
                                                //PajeEventId,eventid number,line,defs
  eventBeingDefined = new PajeEventDefinition((PajeEventId)(definitionArray[0]),(int)(definitionArray[0]), currentEvent,defs);
 
  for (; n < size; n=n+2) {
    eventBeingDefined->addField((PajeField)(definitionArray[n]),(PajeFieldType)(definitionArray[n+1]),0);
    
  }
  
  
  
  eventDefinitions[(int)(definitionArray[0])] = eventBeingDefined;
}


PajeRastroTraceEvent *PajeRastroReader::scanEventLine (rst_event_t *event)
{
  int eventId = -1;
  PajeEventDefinition *eventDefinition = NULL;
  eventId = event->type;

  std::map<u_int32_t,PajeEventDefinition*>::iterator it;
  it = eventDefinitions.find(eventId);
  eventDefinition =  it->second;
  

  if (eventDefinition == NULL) { 
    throw PajeDecodeException ("Event with id '"+std::string("%d",eventId)+"' has not been defined");
  }

  return new PajeRastroTraceEvent (eventDefinition,event); 
}

bool PajeRastroReader::hasMoreData()
{
  return moreData;
}




//called by the PajeThreadReader
void PajeRastroReader::readNextChunk ()
{
  /* reading the file */
  if(rst_decode_event(&rastro, &rst_event)){
    int PajeHeaderEventId = 999;
    //header definition
    if(rst_event.type == PajeHeaderEventId ){
      PajeRastroReader::scanDefinitionLine(rst_event.v_uint32,rst_event.ct.n_uint32);   
    }
    //event definition
    else{
      PajeRastroTraceEvent *event = PajeRastroReader::scanEventLine(&rst_event);
      
      if (event != NULL){
        PajeComponent::outputEntity(event);
        currentEvent++;
      }
      delete event;
       //printf("finished read event %d \n",currentEvent);
    }
  }else{
    moreData = false;
  }


}

