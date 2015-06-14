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

  ReadStrings();  
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
  if (event->ct.n_uint8 > 0) {
    for (int i = 0; i < event->ct.n_uint8; i++) {
      strcpy(event->v_string[i],FindStringParam(event->v_uint8[i]));
    }
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
      //if event is a reference load(either save the events right before they are used or have a separated file with the strings)
      if(rst_event.type == 888)
      {
        //AddToParamList(&rst_event);
      }else{ 
        PajeRastroTraceEvent *event = PajeRastroReader::scanEventLine(&rst_event);
        
        if (event != NULL){
          PajeComponent::outputEntity(event);
          currentEvent++;
        }
        delete event;
      }    
    }
  }else{
    moreData = false;
  }

}

void PajeRastroReader::ReadStrings()
{

  rst_rastro_t rastro_strings;
  bzero(&rastro_strings, sizeof(rst_rastro_t));
  rst_event_t event_strings;
  int i;

  /* open rst file */
    int status = rst_open_file (&rastro_strings, 100000,
                                "strings_reference.rst",
                                (char*)"out.txt");
    if (status == RST_NOK){
      fprintf(stderr,
              "[rastro_read] at %s, "
              "trace file %s could not be opened\n",
              __FUNCTION__, "strings_reference.rst");
      return;
  }
  
  
  /* reading all the files */
  while (rst_decode_event (&rastro_strings, &event_strings)) {
    if(stringList == NULL){
      stringList =(StringParamsList*) malloc(sizeof(StringParamsList));
     // memcpy(stringList->string , &(event_strings.v_string[0]),sizeof(&(event_strings.v_string[0]))); 
      stringList->string = strdup(event_strings.v_string[0]);
      stringList->string_position = event_strings.v_uint8[0];
      stringList->next = NULL;
      continue;
    }  
    StringParamsList *actual = stringList;  
    while(actual != NULL){
        if(actual->next == NULL)
        {
          break;
        }
        actual = actual->next;
    }
    
    StringParamsList *new_param = (StringParamsList *)malloc(sizeof(StringParamsList));
    //memcpy(new_param->string , event_strings.v_string[0],sizeof(event_strings.v_string[0])); 
    new_param->string = strdup(event_strings.v_string[0]);
    new_param->string_position = event_strings.v_uint8[0];
    new_param->next = NULL;
    actual->next = new_param;
  }
  

  /* closing everything */
  rst_close (&rastro_strings);

  /*printf("printings strings \n");
  StringParamsList *actual = stringList;
  while(actual != NULL){
        printf("\n %d  -- %s \n", actual->string_position, actual->string);
        actual = actual->next;
    }*/
}

char* PajeRastroReader::FindStringParam(short position)
{
  struct StringParamsList *actual = stringList;  
  short counter =0;
  while(actual->string_position < position){
      actual = actual->next;
      counter = counter + 1;
  }
  return actual->string;
}
/*
void PajeRastroReader::AddToParamList(rst_event_t *reference);
{

  if(stringList == NULL){
    stringList = (struct StringParamsList *)malloc(sizeof(struct StringParamsList));
    stringList->string = event->v_string[0];
    stringList->string_position = event->v_uint16[0];
    stringList->next = NULL;
    return;
  }

  StringParamsList *actual = stringList;  
  while(actual != NULL){
    if(actual->next == NULL) break;
    actual = actual->next;
  }
  struct StringParamsList *new_param = (struct StringParamsList *)malloc(sizeof(struct StringParamsList));
  new_param->string = event->v_string[0];
  new_param->string_position = event->v_uint16[0];
  new_param->next = NULL;
  actual->next = new_param;
}*/


