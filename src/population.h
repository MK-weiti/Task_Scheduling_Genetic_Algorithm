#ifndef POPULATION_H
#define POPULATION_H

#include <vector>

#ifdef __linux__
#include <bits/stdc++.h>
#endif
#include "chromosomeTL.h"
#include "chromosomePL.h"
#include "dataContainer.h"

/*
As for probability we use unsigned integer [0, 1000] which means
the probability * 1000
*/


namespace task_scheduling{

    class Population{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        class Individual{

            ChromosomeTL *tl_m; //Task List
            ChromosomePL *pl_m; //Processor List
            Population *population;
            unsigned int delay_m;

            Individual();

        public:
            Individual(const unsigned int &numberOfGenes, Population *pop);
            ~Individual();
            Individual(const Individual *other);

            unsigned int fitness();
            
            // can return nullptr based on probability
            std::pair<Individual*, Individual*> crossover(const Individual *individual, const unsigned int &probability) const;
            // can do nothing based on probability
            void mutation(const unsigned int &probability);

            // assing genes and reshuffle them
            void randFeatures(const std::vector<Task*> &taskVector, const unsigned int &numberOfProcessors, const unsigned int &randLoops);

            friend std::ostream& operator<< (std::ostream& os, const Individual &ind){
                std::cout << "TL: \n" << *ind.tl_m << std::endl;
                std::cout << "PL: \n" << *ind.pl_m << std::endl;
                return os;
            }

            void setOutputData(std::vector<std::vector<Task*>> &taskSchedule, const unsigned int &numberOfProcessors);

        };

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        class MatingPool{
            // three main methods
            void tournament(Individual **oldNewGeneration);
            Individual *crossover();    // return the best individual and fill the mating pool
            void mutation(Individual *p);

            Population *population;

            // what is in pool and neResidends after creating new population does not matter
            // so do not delete it
            Individual **pool;  // here are parents
            Individual **newResidents;  // here are children with mutation

            unsigned int matingPoolSize;
        public:
            MatingPool(Population *pop, unsigned int matingPoolSize);
            ~MatingPool();

            // only set the variable. Do not allocate memory
            void setMatingPoolSize(const unsigned int &newSize);

            bool checkMatingPoolData() const;

            // include tournament, crossover and mutation
            // return the best individual
            Individual *createNewPopulation(Individual **&oldNewGeneration);

            // create new mating pool and new residents
            bool newMatingPool();
        };

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        Individual **residents; // here are residents of the population
        unsigned int maxFinishingTime;  // to change function from minimization to maximizing
        MatingPool matingPool;
        bool stillPrepare;  // = true

        void createFirstPopulation(const unsigned int &randLoops);
        void prepareMatingPool();

    public:

        unsigned int populationSize;
        unsigned int maxNumberOfGeneration;
        unsigned int crossoverProb;
        unsigned int mutationProb;
        unsigned int numberOfGenerWithoutImprov;
        DataContainer *dataContainer;

        unsigned int PCl1, PCg1, PCl2, PCg2, PCl12, PCg12;
        unsigned int PMl1, PMg1, PMl2, PMg2, PMl12, PMg12;



        Population();
        ~Population();
        Population(const Population& pop) = delete;

        bool checkData();

        /*
        RandLoops means how many loops will be used to randomize first population.
        At least one should be enough but you can use more.
        */
        void run(const unsigned int &randLoops);
    };

};

#endif //POPULATION_H

