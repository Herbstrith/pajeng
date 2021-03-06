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
#include <sys/time.h>
#include "PajeUnity.h"
#include "PajeException.h"
#include "PajeFileReader.h"
#include "PajeFlexReader.h"
#include "PajeRastroReader.h"
#include "PajeProbabilisticSimulator.h"

static double gettime (void)
{
  struct timeval tr;
  gettimeofday(&tr, NULL);
  return (double)tr.tv_sec+(double)tr.tv_usec/1000000;
}

PajeUnity::PajeUnity (bool flexReader, bool strictHeader, std::string tracefilename, double stopat, int ignoreIncompleteLinks, char *probabilistic)
{
  //basic configuration
  this->flexReader = flexReader;
	this->rastroReader = rastroReader;
  //the global PajeDefinitions object
  definitions = new PajeDefinitions (strictHeader);
 
  try {
    #ifndef RASTRO_READER
      //alloc reader
      if (flexReader){
        if (tracefilename.empty()) {
          reader = new PajeFlexReader(definitions);
        } else {
          reader = new PajeFlexReader(tracefilename, definitions);
        }
      } else {
        if (tracefilename.empty()){
          reader = new PajeFileReader();
        } else {
          reader = new PajeFileReader (tracefilename);
        }
      }

      //alloc decoder and simulator
      if (!flexReader){
        decoder = new PajeEventDecoder(definitions);
      }
    #else
      char *cstr = &tracefilename[0u];
      reader = new PajeRastroReader(definitions, cstr);
      //reader = (rastroReader == 1) ? new PajeRastroReader(definitions, cstr) :  new PajeRastroReader(definitions, cstr, true);
    #endif 
   
    if (probabilistic){
      simulator = new PajeProbabilisticSimulator (probabilistic);
    } else {
      simulator = new PajeSimulator (stopat, ignoreIncompleteLinks);
    }


    //connect components
    #ifndef RASTRO_READER
    if (flexReader){
      reader->setOutputComponent (simulator);
      simulator->setInputComponent (reader);  
    } else {
      reader->setOutputComponent (decoder);
      decoder->setInputComponent (reader);
      decoder->setOutputComponent (simulator);
      simulator->setInputComponent (decoder);
    }
    #else
      //call the constructor for the Rastro PajeSimulator
      simulator = new PajeSimulator ();
			reader->setOutputComponent (simulator);
      simulator->setInputComponent (reader);
    #endif
    simulator->setOutputComponent (this);
    this->setInputComponent (simulator);
  }catch (PajeException& e){
    e.reportAndExit ();
  }

  //read and simulate
  t1 = gettime();
  try {
    reader->startReading ();
    while (reader->hasMoreData() && simulator->keepSimulating()){
      reader->readNextChunk ();
    }
    reader->finishedReading ();
  }catch (PajeException& e){
    e.reportAndExit();
  }
  t2 = gettime();
}

PajeUnity::~PajeUnity ()
{
  delete reader;
  #ifndef RASTRO_READER
  if (!flexReader) {
    delete decoder;
  }
  #endif
  delete simulator;
  delete definitions;
}

double PajeUnity::getTime ()
{
  return t2-t1;
}

void PajeUnity::report ()
{
  simulator->report();
}

void PajeUnity::reportContainer ()
{
  simulator->reportContainer();
}
