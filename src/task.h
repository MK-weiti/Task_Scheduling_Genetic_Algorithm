#ifndef TASK_H
#define TASK_H

#include <fstream>

namespace task_scheduling{

    class Task{
    public:
        const unsigned long int ID;
        const unsigned int durationTime;
        const unsigned int deadline;

        Task(unsigned int durationTime, unsigned int deadline, const unsigned long int id);

        bool operator == (const Task &task);
        bool operator != (const Task &task);
        friend std::ostream& operator << (std::ostream& os, const Task& task){
            os << "dt: " << task.durationTime << '\t' << "d: " << task.deadline;
            return os;
        }
    };

};



#endif //TASK_H
