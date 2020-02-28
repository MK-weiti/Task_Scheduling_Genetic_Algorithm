#include <iostream>

using namespace std;
#include <fstream>

#include "dataContainer.h"
#include "population.h"
#include "rand.h"

void setPopulation(Population &population){
    // Uses promiles instead of percentages.

    population.populationSize = 1000;
    population.maxNumberOfGeneration = 20000;
    population.numberOfGenerWithoutImprov = 1000;

    /*
    Below values were precisely selected. 
    It is not advisable to change them.
    */

    population.crossoverProb = 550;

    /*
    This is high because that was the best value deduced through experiments but 
    you can change it and check if you can get better results.
    In theory it should be small like 10% (100 in our case).
    */
    population.mutationProb = 500;  

    /*
    These are boundaries that tell about probability of choosing one of below options for crossover and mutation.
    */
    population.PCl1 = 350;  //only task list crossover boundaries
    population.PCg1 = 550;
    population.PCl2 = 550;  //only processor list crossover boundaries
    population.PCg2 = 750;
    population.PCl12 = 750; //both crosovers boundaries
    population.PCg12 = 950;

    population.PMl1 = 150;  //only task list mutation boundaries
    population.PMg1 = 250;
    population.PMl2 = 250;  //only processor list mutationr boundaries
    population.PMg2 = 400;
    population.PMl12 = 400; //both mutations boundaries
    population.PMg12 = 500;
}

using namespace task_scheduling;

int main(int argc, char** argv)
{

    #ifdef DEBUG
    cout << "Debug version" << endl;
    #endif

    Rand::newSeed();

    task_scheduling::DataContainer container;
    if (container.prepareData(argc, argv)) {
        cout << "Something bad happened while preparing data. Program ends." << endl;
        return 1;
    }

    container.printTasks();


    Population population;

    population.dataContainer = &container;
    setPopulation(population);

    population.run(1);

    cout << "Task chedule: " << endl;

    for (unsigned int i = 0; i < container.taskSchedule.size(); ++i){
        cout << "\t Processor[" << i << "] " << endl;
        for(unsigned int j = 0; j < container.taskSchedule[i].size(); ++j){
            cout << *container.taskSchedule[i][j] << endl;
        }
    }

    cout << "Delay sum: " << container.delaySum() << endl;

    return 0;
}
