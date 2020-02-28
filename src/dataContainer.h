#ifndef DATACONTAINER_H
#define DATACONTAINER_H

#include <iostream>
#include <vector>
#include <fstream>
#include <stdlib.h>

#include "task.h"


namespace task_scheduling{

    /*
    The order of tasks list and number of processors on input are interchangeable.
    Text file is given as a path to the file in the program arguments.
    */
    class DataContainer{

        // input data, private
        unsigned int m_processorsNumber;
        /*
        Data here is already sorted by ID starting at 0.
        Position in array is equal to the ID of the task.
        It is guaranteed to always get the same task by its ID.
        */
        std::vector<Task*> tasks;

    public:
        // output data, public
        // taskSchedule[processor ID][consecutive tasks]
        std::vector<std::vector<Task*>> taskSchedule;

        DataContainer();
        ~DataContainer();

        unsigned int getNumberOfProcessors() const;

        // important method, set all data
        bool prepareData(int &argc, char** argv);

        // get access to tasks
        Task*& getPointerToTask(const size_t &number);

        unsigned int getNumberOfTasks() const;
        unsigned long int getTaskPosition(const Task *task) const;

        unsigned int delaySum() const;

        void printTasksSchedule(bool raw) const;
        void printTasks() const;

        std::vector<Task*> &getTasksVector();
    };

};



#endif //DATACONTAINER_H
