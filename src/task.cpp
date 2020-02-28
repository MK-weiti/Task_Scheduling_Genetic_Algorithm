#ifndef TASK_CPP
#define TASK_CPP

#include "task.h"

using namespace task_scheduling;

Task::Task(unsigned int durationTime, unsigned int deadline, const unsigned long int id)
:ID(id), durationTime(durationTime), deadline(deadline)
{}

bool Task::operator == (const Task &task){
    if(ID == task.ID) return true;
    return false;
}

bool Task::operator != (const Task &task){
    if(ID != task.ID) return true;
    return false;
}

#endif //TASK_CPP
