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
#ifndef __PAJERASTROREADER_H__
#define __PAJERASTROREADER_H__
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "PajeComponent.h"

extern "C"
{
  #include <rastro.h>
}




#define PAJE_DEFAULT_CHUNK_SIZE 1500000
#define PAJE_DEFAULT_LINE_SIZE 512

class PajeRastroReader : public PajeComponent {
private:
  rst_rastro_t rastro;      //the rastro formated file to be read  
/*std::string filename;
  std::ifstream file;
  std::istream *input;
  std::streamoff chunkSize;
  int currentChunk;
  bool moreData;
  unsigned long long length;
  unsigned long long current;
*/
public:
  PajeRastroReader(std::string f);
  ~PajeRastroReader(void);
  /*void readNextChunk (void);
  bool hasMoreData (void);
  void setUserChunkSize (std::streamoff userChunkSize);
  unsigned long long traceSize (void);
  unsigned long long traceRead (void);*/
};
#endif
