#ifndef DATACONTAINER_CPP
#define DATACONTAINER_CPP

#include "dataContainer.h"

using namespace task_scheduling;

DataContainer::DataContainer()
:m_processorsNumber(0) {}

DataContainer::~DataContainer(){
    for(auto &it : tasks){
        delete it;
    }
}

unsigned int DataContainer::getNumberOfTasks() const{
    return tasks.size();
}

unsigned int DataContainer::getNumberOfProcessors() const{
    return m_processorsNumber;
}

Task*& DataContainer::getPointerToTask(const size_t &number){
    #ifdef DEBUG
    if(number >= tasks.size()) throw std::runtime_error("Out of range.");
    if(tasks[number] == nullptr) throw std::runtime_error("Pointer not initialized.");
    #endif

    return tasks[number];
}

bool DataContainer::prepareData(int &argc, char** argv){
    if(argc != 3) return true;

    unsigned char flags = 0, txt;

    // interpret the program`s arguments
    for (int i = 1; i < argc; ++i){
        int tmp = (int) argv[i][0];

        if(tmp < 48 || tmp > 57){   // is not a number
            char c = argv[i][0];
            int charSize = 0;

            // get to the end of string
            while(c != '\0'){
                ++charSize;
                c = argv[i][charSize];

            }

            if(argv[i][charSize-1] == 't' && argv[i][charSize-2] == 'x' && argv[i][charSize-3] == 't' && argv[i][charSize-4] == '.'){
                txt = i;
                flags = flags | 1;
            }
            else return true;
        }
        else{   // is a number
            m_processorsNumber = atoi(argv[i]);
            flags = flags | 2;
        }
    }


    if(flags != 3) return true;

    std::ifstream fileHandler;
    fileHandler.open(argv[txt], fileHandler.out);

    if(!fileHandler.is_open()) return true;

    unsigned long int tasksNumber = 0;
    while(1){
        unsigned int durationTime, deadline;
        fileHandler >> durationTime;
        fileHandler >> deadline;

        if(!fileHandler.eof()) tasks.push_back(new Task(durationTime, deadline, tasksNumber++));
        else break;
    }

    fileHandler.close();

    taskSchedule.resize(m_processorsNumber, std::vector<Task*>());

    return false;
}

unsigned int DataContainer::delaySum() const{
    unsigned int sum = 0;
    for(unsigned int it = 0; it < taskSchedule.size(); ++it){
        unsigned int start = 0, delayOnProcessor = 0;

        for(unsigned int i = 0; i < taskSchedule[it].size(); ++i){
            start += taskSchedule[it][i]->durationTime;
            if(start > taskSchedule[it][i]->deadline) delayOnProcessor += start - taskSchedule[it][i]->deadline;
        }
        sum += delayOnProcessor;
    }
    return sum;
}

void DataContainer::printTasksSchedule(bool raw) const{
    if(raw){
        for (unsigned int i = 0; i < taskSchedule.size(); ++i){
            for (unsigned int j = 0; j < taskSchedule[i].size(); ++j){
                std::cout << taskSchedule[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
    else{
        std::cout << "Task scheduler" << std::endl;
        for (unsigned int i = 0; i < taskSchedule.size(); ++i){
            std::cout << i+1 << ": ";
            for (unsigned int j = 0; j < taskSchedule[i].size(); ++j){
                std::cout << taskSchedule[i][j] << "\t";
            }
            std::cout << std::endl;
        }
    }
}

void DataContainer::printTasks() const{
    for (unsigned int i = 0; i < tasks.size(); ++i){
        std::cout << "dt: " << tasks[i]->durationTime << "\t\td: " << tasks[i]->deadline << std::endl;
    }
}

std::vector<Task*> &DataContainer::getTasksVector(){
    return tasks;
}

unsigned long int DataContainer::getTaskPosition(const Task *task) const{
    #ifdef DEBUG
    if(task == nullptr) throw std::runtime_error("Given task is nullptr.");
    #endif
    return task->ID;
}

#endif //DATACONTAINER_CPP
