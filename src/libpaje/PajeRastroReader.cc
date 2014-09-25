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
#include <sys/time.h>

PajeRastroReader::PajeRastroReader (std::string f)
{

  char* sync_file = "syncfile";
  int status = rst_open_file (&rastro, 100000,(char*)f.c_str() ,sync_file);
    if (status == RST_NOK){
      fprintf(stderr,
              "trace file  could not be opened\n"
               );
  }
     


  /*chunkSize = PAJE_DEFAULT_CHUNK_SIZE;
  moreData = true;
  currentChunk = 0;
  filename = f;
  file.open (filename.c_str());
  if (file.fail()){
    throw PajeFileReadException (f);
  }
  file.seekg (0, std::ios::end);
  length = file.tellg ();
  file.seekg (0, std::ios::beg);

  input = &file;*/
}


PajeRastroReader::~PajeRastroReader(void)
{
    rst_close (&rastro);
}

/*
void PajeRastroReader::readNextChunk (void)
{
  if (input->eof()) moreData = false;
  if (!moreData) return;

  //read a chunk
  PajeData *buffer = new PajeData(chunkSize);
  input->read (buffer->bytes, chunkSize);
  current = input->tellg();
  std::streamoff length = buffer->length = input->gcount ();

  //read until next \n
  while (!input->eof()){
    char c;
    input->get (c);
    if (buffer->length + 1 > buffer->capacity){
      buffer->increaseCapacityOf (100);
    }
    buffer->bytes[buffer->length] = c;
    length = buffer->length++;
    if (c == '\n') break;
  }

  if (length > 0){
    PajeComponent::outputEntity (buffer);
  }
  delete buffer;
}

bool PajeRastroReader::hasMoreData (void)
{
  return moreData;
}

void PajeRastroReader::setUserChunkSize (std::streamoff userChunkSize)
{
  chunkSize = userChunkSize;
}

unsigned long long PajeRastroReader::traceSize (void)
{
  return length;
}

unsigned long long PajeRastroReader::traceRead (void)
{
  if (!input->eof())
    return current;
  else
    return length;
}*/
