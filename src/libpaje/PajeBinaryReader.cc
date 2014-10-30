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
#include <PajeDefinitions.h>
#include <stdlib.h>

PajeBinaryReader::PajeBinaryReader (PajeDefinitions *definitions, char *file_rst)
{
   // printf("start create binary reader\n");
  defStatus = OUT_DEF;
  currentEvent = 0;

  moreData = true;
 defs  = definitions;

	
  bzero(&rastro, sizeof(rst_rastro_t));
  //open rst_file
   int status = rst_open_file (&rastro, 100000,
                                file_rst,
                                (char*)"out.txt");
    if (status == RST_NOK){
      //TODO: throw Exception
    //  printf("error at openning of the rst_file\n");
  }
     //   printf("finished creating binary reader\n");
}

PajeBinaryReader::~PajeBinaryReader ()
{
  eventDefinitions.clear ();
  /* closing the file */
  rst_close (&rastro);
}


paje_line *poti_print_event (rst_event_t *event,PajeEventDefinition *eventDefinition)
{
     // std::list<PajeField> fields;
    //  std::list<PajeFieldType> types;

	  //int i;
		int event_id;

		event_id = event->type;


        paje_line *paje_line_string = new paje_line;

      char temp[3];
      //sprintf(temp,"%d",event_id);

      sprintf(temp , "%d" , event_id);
                //   printf("start  %s \n",temp);

      paje_line_string->word[0] = temp;
 // printf("\n %s \n",paje_line_string->word[0]);
    std::list<PajeField>::const_iterator ff;
                 //  printf("start print_evet %d \n",event_id);
    ff = eventDefinition->fields.begin();
                //   printf("start print_evet %d \n",eventDefinition->uniqueIdentifier);
 /* for(  ff = eventDefinition->fields.begin();
    ff != eventDefinition->fields.end(); ++ff)
  {
    printf("%d %d \n", *ff , *ff);
  }*/

	//	set_event_order(event_id);
		int f;
		int double_mark=0,int_mark=0,float_mark=0,string_mark = 0;
                 //  printf("start print_evet %d \n",eventDefinition->uniqueIdentifier);
    
    //paje_line string build-up
    std::list<PajeFieldType>::iterator i = eventDefinition->types.begin();
                   //printf("start fors size %d\n",eventDefinition->fields.size()); 
  i++;
    
		for(f = 1; f < eventDefinition->fields.size();f++)
		{
                         //printf("loop %d \n",f);
				if((*i  ==  PAJE_string || *i ==  PAJE_color) && f == eventDefinition->fields.size()-1)
				{

          paje_line_string->word[f] = event->v_string[string_mark];
			//		printf("%s", event->v_string[string_mark]);            printf("break last str");
          paje_line_string->word_count++;
					break;


				}
				if((*i== PAJE_double || *i == PAJE_date) && f== eventDefinition->fields.size()-1)
				{
             char temp[50];
             sprintf(temp , "%lf" , event->v_double[double_mark]);
           paje_line_string->word[f] = temp;
       // sprintf(temp , "%d" , event_id);

        //  sprintf(paje_line_string->word[f] , "%lf" , event->v_double[double_mark]);
          //paje_line_string->word[f] = dtoa(event->v_double[double_mark]);
				//	printf("%lf", event->v_double[double_mark]);   printf("break last double");  

      paje_line_string->word_count++;       
					break;
		
				}				

					
				if(*i == PAJE_string || *i == PAJE_color)
				{

          paje_line_string->word[f] = event->v_string[string_mark];

				//	printf("%s ", event->v_string[string_mark]);
      //    printf("continue");
					string_mark = string_mark +1;
				}
				if(*i == PAJE_double || *i == PAJE_date)
				{
           char temp[50];
             sprintf(temp , "%lf" , event->v_double[double_mark]);
           paje_line_string->word[f] = temp;
				//	printf("%lf ", event->v_double[double_mark]);

					double_mark = double_mark +1;
     //     printf("continue");
		  	}
        paje_line_string->word_count++;
				i++;

		}
    int zz;
      for(zz = 0; zz <= paje_line_string->word_count; zz++)
  {
   // printf("\n %s",paje_line_string->word[zz]);
  }    
   // printf("end loop \n");
paje_line_string->word_count++;
    paje_line_string->lineNumber=0;  
    return paje_line_string;
	
}

void PajeBinaryReader::scanDefinitionLine(u_int32_t definitionArray[], u_int32_t size)
{         
     // PajeDefinitions *definitions;
      //initialize (definitions);                         


                  // printf("start scandefline  \n");
  int n = 1;
                                                //PajeEventId,eventid number,line,defs
  eventBeingDefined = new PajeEventDefinition((PajeEventId)(definitionArray[0]),(int)(definitionArray[0]), currentEvent,defs);
 // printf("event id %d",eventBeingDefined->uniqueIdentifier);
                      // printf("event being defined  \n");
  //eventDefinitions[definitionArray[n]] = eventBeingDefined;
                   //  printf("start loop \n");
  for (; n < size; n=n+2) {

      eventBeingDefined->addField((PajeField)(definitionArray[n]),(PajeFieldType)(definitionArray[n+1]),0);
		}

  eventDefinitions[(int)(definitionArray[0])] = eventBeingDefined;

}


PajeTraceEvent *PajeBinaryReader::scanEventLine (rst_event_t *event)
{
                 //  printf("start eventdefine  \n");
  int eventId = -1;
  PajeEventDefinition *eventDefinition = NULL;

  eventId = event->type;

  std::map<u_int32_t,PajeEventDefinition*>::iterator it;
    it = eventDefinitions.find(eventId);
  eventDefinition =  it->second;
  


  //printf("poti_print call event id %d \n",eventId);
  paje_line *pajeLine = poti_print_event(event, it->second);  
  int i = 0;
  //printf(" fim do print_event");

  /*for(i = 0; i < 3; i++)
  {
    printf("\n %s",pajeLine->word[i]);
  } */               
  if (eventDefinition == NULL) { // printf("event DEFINITION NULL");
    throw PajeDecodeException ("Event with id '"+std::string("%d",eventId)+"' has not been defined");
  }
                   //  printf("return  eventdefine  \n");
  return new PajeTraceEvent (eventDefinition,pajeLine);

  
  
}

bool PajeBinaryReader::hasMoreData()
{
  return moreData;
}




//called by the PajeThreadReader
void PajeBinaryReader::readNextChunk ()
{
   // printf("Read chunk \n");




  /* reading the file */
  if(rst_decode_event(&rastro, &rst_event)) 
  {
    //  printf("decoding rst event \n");
    int PajeHeaderEventId = 999;
    if(rst_event.type == PajeHeaderEventId )
    {
           //  printf("its a header \n");
      PajeBinaryReader::scanDefinitionLine(rst_event.v_uint32,rst_event.ct.n_uint32);
     
     // currentEvent++;
             //      printf("finished header \n");
    }

    //poti event event
    else
    {
   /*   for(std::map<u_int32_t,PajeEventDefinition*>::const_iterator it = eventDefinitions.begin();
    it != eventDefinitions.end(); ++it)
  {
    printf("%d %d \n", it->first , it->second->uniqueIdentifier);
  }*/
               //  printf("its a event \n");
      PajeTraceEvent *event = PajeBinaryReader::scanEventLine(&rst_event);
                  //     printf("created the event \n");
      if (event != NULL){
        //printf("event id %d",event->pajeEventDefinition);
        PajeComponent::outputEntity(event);
        delete event;
        currentEvent++;
                   //    printf("finished event \n");
      }

    }
  }
  else
  {
	  printf("\n %d \n",currentEvent);
        //  printf("no more data \n");
    moreData = false;
  }

      
     // printf("finished Read chunk \n");



}

