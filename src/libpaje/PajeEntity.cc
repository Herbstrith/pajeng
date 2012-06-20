/*
    This file is part of Paje.

    Paje is free software: you can redistribute it and/or modify it
    under the terms of the GNU Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Paje is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Public License for more details.

    You should have received a copy of the GNU Public License
    along with Paje. If not, see <http://www.gnu.org/licenses/>.
*/
#include "PajeEntity.h"

PajeEntity::PajeEntity (PajeContainer *container, PajeType *type, std::string name)
{
  this->entityContainer = container;
  this->entityType = type;
  this->entityName = name;
}

PajeContainer *PajeEntity::container (void) const
{
  return entityContainer;
}

PajeType *PajeEntity::type (void) const
{
  return entityType;
}

std::string PajeEntity::name (void) const
{
  return entityName;
}

void PajeEntity::setName (std::string newname)
{
  this->entityName = newname;
}

bool PajeEntity::isContainedBy (PajeContainer *container)
{
  return this->container() == container;
}

bool PajeEntity::isContainer (void)
{
  return false;
}

std::string PajeEntity::value (void)
{
  return name();
}

void PajeEntity::setValue (std::string newvalue)
{
  throw "should be implemented in subclass";
}

void PajeEntity::setDoubleValue (double value)
{
  throw "should be implemented in subclass";
}

void PajeEntity::addDoubleValue (double value)
{
  setDoubleValue (value);
}

void PajeEntity::subtractDoubleValue (double value)
{
  setDoubleValue (value);
}

double PajeEntity::doubleValue (void)
{
  return 0;
}

PajeContainer *PajeEntity::startContainer (void)
{
  return container();
}

PajeContainer *PajeEntity::endContainer (void)
{
  return container();
}

int PajeEntity::imbricationLevel (void)
{
  return 0;
}

/**************************************************************
 * PajeUserEvent
 */
PajeUserEvent::PajeUserEvent (PajeContainer *container, PajeType *type, std::string name, double time):PajeEntity(container, type, name)
{
  this->t = time;
}

std::string PajeUserEvent::description (void)
{
  std::stringstream description;
  description << "Event, "
              << container()->name() << ", "
              << type()->name << ", "
              << startTime() << ", "
              << name();
  return description.str();
}

double PajeUserEvent::time (void)
{
  return t;
}

double PajeUserEvent::startTime (void)
{
  return t;
}

void PajeUserEvent::setStartTime (double time)
{
  this->t = time;
}

double PajeUserEvent::endTime (void)
{
  return startTime();
}

void PajeUserEvent::setEndTime (double endTime)
{
  setStartTime (endTime);
}

double PajeUserEvent::firstTime (void)
{
  return startTime();
}

double PajeUserEvent::lastTime (void)
{
  return startTime();
}

double PajeUserEvent::duration (void)
{
  return 0;
}

/**************************************************************
 * PajeUserState
 */
PajeUserState::PajeUserState (PajeContainer *container, PajeType *type, std::string value, double startTime):PajeUserEvent(container, type, value, startTime)
{
  this->etime = -1;
  this->imbrication = 0;
}

PajeUserState::PajeUserState (PajeContainer *container, PajeType *type, std::string value, double startTime, int imbric):PajeUserEvent(container, type, value, startTime)
{
  this->etime = -1;
  this->imbrication = imbric;
}

std::string PajeUserState::description (void)
{
  std::stringstream description;
  description << "State, "
              << container()->name() << ", "
              << type()->name << ", "
              << startTime() << ", "
              << endTime() << ", "
              << name();
  return description.str();
}

void PajeUserState::setValue (std::string newvalue)
{
  setName (newvalue);
}

void PajeUserState::setEndTime (double endTime)
{
  this->etime = endTime;
}

double PajeUserState::endTime (void)
{
  return etime;
}

double PajeUserState::lastTime (void)
{
  return etime;
}

double PajeUserState::duration (void)
{
  return endTime() - startTime();
}

int PajeUserState::imbricationLevel (void)
{
  return imbrication;
}

/**************************************************************
 * PajeUserVariable
 */
PajeUserVariable::PajeUserVariable (PajeContainer *container, PajeType *type, double value, double startTime, double endTime):PajeUserState (container, type, "", startTime, endTime)
{
  this->val = value;
}

std::string PajeUserVariable::description (void)
{

  std::stringstream description;
  description << "Variable, "
              << container()->name() << ", "
              << type()->name << ", "
              << startTime() << ", "
              << endTime() << ", "
              << doubleValue();
  return description.str();
}

double PajeUserVariable::doubleValue (void)
{
  return val;
}

void PajeUserVariable::setDoubleValue (double value)
{
  this->val = value;
}

void PajeUserVariable::addDoubleValue (double value)
{
  this->val += value;
}

void PajeUserVariable::subtractDoubleValue (double value)
{
  this->val -= value;
}

/**************************************************************
 * PajeUserLink
 */
PajeUserLink::PajeUserLink (PajeContainer *container, PajeType *type, std::string value, std::string key, PajeContainer *startContainer, double startTime):PajeUserState(container, type, value, startTime, startTime)
{
  this->key = key;
  this->startCont = startContainer;
  this->endCont = NULL;
}

std::string PajeUserLink::description (void)
{
  std::stringstream description;
  description << "Link, "
              << container()->name() << ", "
              << type()->name << ", "
              << startTime() << ", "
              << endTime() << ", "
              << value() << ", "
              << startContainer()->name() << ", "
              << endContainer()->name();
  return description.str();
}

PajeContainer *PajeUserLink::startContainer (void)
{
  return startCont;
}

void PajeUserLink::setStartContainer (PajeContainer *startContainer)
{
  this->startCont = startContainer;
}

void PajeUserLink::setEndContainer (PajeContainer *endContainer)
{
  this->endCont = endContainer;
}

PajeContainer *PajeUserLink::endContainer (void)
{
  return endCont;
}