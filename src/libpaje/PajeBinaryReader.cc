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

PajeBinaryReader::PajeBinaryReader (PajeDefinitions *definitions,)
{
  defStatus = OUT_DEF;
  currentEvent = 0;
  defs = definitions;

  bzero(&rastro, sizeof(rst_rastro_t));
  //open rst_file
   int status = rst_open_file (&rastro, 100000,
                                arguments.input[i],
                                arguments.synchronization_file);
    if (status == RST_NOK){
      //TODO: throw Exception
      printf("error at openning of the rst_file");
      return 1;
  }
    
}

PajeBinaryReader::~PajeBinaryReader ()
{
  eventDefinitions.clear ();
  /* closing the file */
  rst_close (&rastro);
}




void PajeBinaryReader::scanDefinitionLine(u_int32_t definitionArray[])
{
  int n = 1;
                                                //PajeEventId,eventid number,line,defs
  eventBeingDefined = new PajeEventDefinition (event->v_uint32[0], event->v_uint32[0], currentEvent, defs);
  eventDefinitions[event->v_uint32[n]] = eventBeingDefined;

  for (; n < event->ct.n_uint32; n=n+2) {
      eventBeingDefined->addField (event->v_uint32[n], event->v_uint32[n+1],n);
		}
  
}


PajeTraceEvent *PajeBinaryReader::scanEventLine (rst_event_t event)
{

  int eventId = -1;
  PajeEventDefinition *eventDefinition = NULL;

  eventID = event->type;

  eventDefinition = eventDefinitions[eventId];
  if (eventDefinition == NULL) {
    throw PajeDecodeException ("Event with id '"+std::string(eventId)+"' has not been defined");
  }

  return new PajeTraceEvent (eventDefinition, line);

  
  
}

//called by the PajeThreadReader
void PajeBinaryReader::readNextChunk (PajeObject *data)
{

  rst_event_t rst_event;


  
  /* reading the file */
  rst_decode_event(&rastro, &rst_event)); 
  

  if(rst_event->type == PajeHeaderEventId )
  {
     
    PajeBinaryReader::scanDefinitionLine(rst_event->v_uint32);
   
    currentEvent++;
  }

  //poti event event
  else
  {
    PajeTraceEvent *event = PajeBinaryReader::scanEventLine(&rst_event);
    if (event != NULL){
      PajeComponent::outputEntity(event);
      delete event;
      currentEvent++;
    }

  }

    
  



}

