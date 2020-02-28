#ifndef POPULATION_CPP
#define POPULATION_CPP

#include "population.h"

using namespace task_scheduling;

//
// Individual
//

Population::Individual::Individual(const unsigned int &numberOfGenes, Population *pop)
:tl_m(new ChromosomeTL(numberOfGenes)), pl_m(new ChromosomePL(numberOfGenes)), population(pop), delay_m(UINT_MAX)
{
    #ifdef DEBUG
    if(pop == nullptr || numberOfGenes == UINT_MAX) throw std::runtime_error("Failed to initialize Individual. Wrong data.");
    #endif
}

Population::Individual::Individual()
{}

Population::Individual::~Individual(){
    delete tl_m;
    delete pl_m;
}

Population::Individual::Individual(const Individual *other){

    #ifdef DEBUG
    if(other == nullptr) throw std::runtime_error("Failed to copy Individual -> nullptr.");
    #endif
    tl_m = new ChromosomeTL(other->tl_m);
    pl_m = new ChromosomePL(other->pl_m);
    population = other->population;
    delay_m = UINT_MAX;
}

unsigned int Population::Individual::fitness(){
    unsigned int *processorSum = new unsigned int[population->dataContainer->getNumberOfProcessors()];

    if(delay_m != UINT_MAX) return delay_m;

    delay_m = 0;

    for (unsigned int i = 0; i < population->dataContainer->getNumberOfProcessors(); ++i){
        processorSum[i] = 0;
    }

    for (unsigned int i = 0; i < tl_m->size(); ++i){
        #ifdef DEBUG
        if(tl_m->getPointerToGene(i) == nullptr || pl_m->getPointerToGene(tl_m->getPointerToGene(i)->ID) == nullptr)
            throw std::runtime_error("Fitness cannot be calculated -> nullptr.");
        #endif
        unsigned int tmpProc = processorSum[*pl_m->getPointerToGene(tl_m->getPointerToGene(i)->ID)] += tl_m->getPointerToGene(i)->durationTime;

        if(tmpProc > tl_m->getPointerToGene(i)->deadline){
            delay_m += tmpProc - tl_m->getPointerToGene(i)->deadline;
        }
    }
    delete[] processorSum;

    return delay_m;
}

std::pair<Population::Individual*, Population::Individual*> Population::Individual::crossover(const Individual *individual, const unsigned int &probability) const{

    Individual *ret1 = new Individual(), *ret2 = new Individual();
    ret1->delay_m = UINT_MAX;
    ret1->population = individual->population;
    ret2->delay_m = UINT_MAX;
    ret2->population = individual->population;

    std::pair<ChromosomeTL*, ChromosomeTL*> TL;
    std::pair<ChromosomePL*, ChromosomePL*> PL;

    if(population->PCl1 <= probability && probability <= population->PCg1) {

        TL = tl_m->crossover(individual->tl_m);
        ret1->tl_m = TL.first;
        ret2->tl_m = TL.second;

        if(Rand::getClose(0, 2)){   //chance to "swap" processor lists
            ret1->pl_m = new ChromosomePL(this->pl_m);
            ret2->pl_m = new ChromosomePL(individual->pl_m);
        } else{
            ret1->pl_m = new ChromosomePL(individual->pl_m);
            ret2->pl_m = new ChromosomePL(this->pl_m);
        }
    }
    else if(population->PCl2 <= probability && probability <= population->PCg2) {

        PL = pl_m->crossover(individual->pl_m);
        ret1->pl_m = PL.first;
        ret2->pl_m = PL.second;

        if(Rand::getClose(0, 2)){   //chance to "swap" task lists
            ret1->tl_m = new ChromosomeTL(this->tl_m);
            ret2->tl_m = new ChromosomeTL(individual->tl_m);
        } else{
            ret1->tl_m = new ChromosomeTL(individual->tl_m);
            ret2->tl_m = new ChromosomeTL(this->tl_m);
        }

    }
    else if(population->PCl12 <= probability && probability <= population->PCg12){

        TL = tl_m->crossover(individual->tl_m);
        PL = pl_m->crossover(individual->pl_m);

        ret1->pl_m = PL.first;
        ret2->pl_m = PL.second;

        if(Rand::getClose(0, 2)){
            ret1->tl_m = TL.first;
            ret2->tl_m = TL.second;
        } else{
            ret1->tl_m = TL.second;
            ret2->tl_m = TL.first;
        }

    } else{
        ret1->tl_m = nullptr;
        ret2->tl_m = nullptr;
        ret1->pl_m = nullptr;
        ret2->pl_m = nullptr;
        delete ret1;
        delete ret2;
        return std::pair<Individual*, Individual*>(nullptr, nullptr);
    }

    return std::pair<Individual*, Individual*>(ret1, ret2);
}

void Population::Individual::mutation(const unsigned int &probability){

    #ifdef DEBUG
    if(probability > 1000) throw std::runtime_error("Failed to mutate. Too high probability.");
    if(population->PMl1 >= population->PMg1 || population->PMl2 >= population->PMg2 || \
    population->PMl12 >= population->PMg12) throw std::runtime_error("Failed to mutate. Wrong percentage.");
    #endif

    if(population->PMl1 <= probability && probability <= population->PMg1){
        tl_m->mutation();
        delay_m = UINT_MAX;
    }else if(population->PMl2 <= probability && probability <= population->PMg2){
        pl_m->mutation(population->dataContainer->getNumberOfProcessors());
        delay_m = UINT_MAX;
    }else if(population->PMl12 <= probability && probability <= population->PMg12){
        tl_m->mutation();
        pl_m->mutation(population->dataContainer->getNumberOfProcessors());
        delay_m = UINT_MAX;
    }
}

void Population::Individual::randFeatures(const std::vector<Task*> &taskVector, const unsigned int &numberOfProcessors, const unsigned int &randLoops){
    // copy pointers and draw processor number for pl_m
    #ifdef DEBUG

    if(taskVector.size() == 0) throw std::runtime_error("Failed to randomize features of individual. Tasks container is empty.");
    if(randLoops < 1) throw std::runtime_error("Failed to randomize features of individual. Too little randLoops");
    #endif

    for(unsigned int i = 0; i < taskVector.size(); ++i){
        tl_m->getPointerToGene(i) = taskVector[i];
        pl_m->getPointerToGene(i) = new unsigned int (Rand::getOpen(0, numberOfProcessors));
    }

    // swap random pointers
    Task *tmpTL;
    unsigned int *tmpPL;
    unsigned int randTL, randPL;

    for(unsigned int k = 0; k < randLoops; ++k){
        for(unsigned int i = 0; i < taskVector.size(); ++i){
            tmpTL = tl_m->getPointerToGene(i);
            tl_m->getPointerToGene(i) = tl_m->getPointerToGene(randTL = Rand::getOpen(0, taskVector.size()));
            tl_m->getPointerToGene(randTL) = tmpTL;

            tmpPL = pl_m->getPointerToGene(i);
            pl_m->getPointerToGene(i) = pl_m->getPointerToGene(randPL = Rand::getOpen(0, taskVector.size()));
            pl_m->getPointerToGene(randPL) = tmpPL;
        }
    }
}

void Population::Individual::setOutputData(std::vector<std::vector<Task*>> &taskSchedule, const unsigned int &numberOfProcessors){
    if(taskSchedule.size() == 0) throw "Output vector not initialized";

    taskSchedule.resize(numberOfProcessors, std::vector<Task*>());

    for(unsigned int i = 0; i < tl_m->size(); ++i){
        unsigned int taskID = tl_m->getPointerToGene(i)->ID;
        taskSchedule[*(pl_m->getPointerToGene(taskID))].push_back(tl_m->getPointerToGene(i));
    }
}
//
// Mating Pool
//

Population::MatingPool::MatingPool(Population *pop, unsigned int matingPoolSize)
:population(pop), pool(nullptr), newResidents(nullptr), matingPoolSize(matingPoolSize)
{}

Population::MatingPool::~MatingPool(){
    if(pool != nullptr) delete[] pool;

    delete[] newResidents;
    population = nullptr;
}

bool Population::MatingPool::checkMatingPoolData() const{
    if(matingPoolSize == UINT_MAX) return true;

    return false;
}

void Population::MatingPool::setMatingPoolSize(const unsigned int &newSize){
    #ifdef DEBUG
    if(newSize == UINT_MAX) throw std::runtime_error("Chromosome invalid size set (UINT_MAX).");
    #endif
    matingPoolSize = newSize;
}

bool Population::MatingPool::newMatingPool(){

    if(pool != nullptr) delete[] pool;
    if(newResidents != nullptr) delete[] newResidents;

    pool = new Individual*[matingPoolSize];
    newResidents = new Individual*[population->populationSize];

    #ifdef DEBUG
    for(unsigned int i = 0; i < matingPoolSize; ++i){
        pool[i] = nullptr;
        newResidents[i] = nullptr;
    }
    #endif

    return false;
}

Population::Individual *Population::MatingPool::createNewPopulation(Individual **&oldNewGeneration){

    #ifdef DEBUG
    if(population->populationSize == UINT_MAX) throw std::runtime_error("Population initial size not set (UINT_MAX).");
    for(unsigned int i = 0; i < population->populationSize; ++i){
        if(oldNewGeneration[i] == nullptr) throw std::runtime_error("Residents not set -> nullptr.");
    }
    #endif

    // find best two individuals
    unsigned int ind1 = population->residents[0]->fitness(), ind2 = population->residents[1]->fitness(), tmp;
    unsigned int i1 = 0, i2 = 1;

    if(ind1 > ind2){
        tmp = ind1;
        ind1 = ind2;
        ind2 = tmp;
        i1 = 1;
        i2 = 0;
    }

    for (unsigned int i = 2; i < population->populationSize; ++i){
        tmp = population->residents[i]->fitness();
        if(tmp < ind1) {
            ind2 = ind1;
            i2 = i1;
            ind1 = tmp;
            i1 = i;
        } else if(tmp < ind2){
            ind2 = tmp;
            i2 = i;
        }
    }

    // add them to mating pool and as new residents(as copy because later they are deleted as old residents)
    pool[0] = population->residents[i1];
    pool[1] = population->residents[i2];

    newResidents[0] = new Individual(population->residents[i1]);
    newResidents[1] = new Individual(population->residents[i2]);

    // other algorithms
    tournament(oldNewGeneration);
    #ifdef DEBUG // check pool
    for(unsigned int i = 0; i < matingPoolSize; ++i){
        if(pool[i] == nullptr) throw std::runtime_error("Pool has nullptr. Error");
    }
    #endif
    Individual *ret = crossover();

    // delete old generation and replace with new generation
    // swap pointers
    for (unsigned int i = 0; i < population->populationSize; ++i){
        delete oldNewGeneration[i];
        #ifdef DEBUG
        oldNewGeneration[i] = nullptr;
        #endif
    }
    Individual **tmpPointer = oldNewGeneration;
    oldNewGeneration = newResidents;
    newResidents = tmpPointer;

    return ret;
}

// tournament, crossover and mutation

// fill pool with individuals
void Population::MatingPool::tournament(Individual **oldNewGeneration){
    unsigned int tournamentSize;

    for(unsigned int i = 2; i < matingPoolSize; ++i){
        tournamentSize = Rand::getOpen(1, population->populationSize);
        Individual **eliminations = new Individual*[tournamentSize];

        // choose individuals to compete with each other
        for(unsigned int j = 0; j < tournamentSize; ++j){
            eliminations[j] = population->residents[Rand::getOpen(0, population->populationSize)];
        }

        // find best individual in eliminations
        unsigned int tmp = eliminations[0]->fitness(), pos = 0;
        for(unsigned int j = 1; j < tournamentSize; ++j){
            if(tmp > eliminations[j]->fitness()){
                tmp = eliminations[j]->fitness();
                pos = j;
            }
        }

        // now add the best to mating pool
        pool[i] = eliminations[pos];
        delete[] eliminations;
    }
}

Population::Individual *Population::MatingPool::crossover(){
    unsigned int newGenPos = 2; // the first two are occupied by the best individuals of previous generation
    #ifdef DEBUG
    if(newResidents[0] == nullptr || newResidents[1] == nullptr)
        throw std::runtime_error("There are no best selected individuals in new generation.");
    #endif
    Individual *best = newResidents[0]->fitness() < newResidents[1]->fitness() ? newResidents[0] : newResidents[1];

    while(newGenPos != population->populationSize){

        /*
        Random positions must be outside it because if they weren't, the if statement would be useless.
        In this way we can draw a lot more random numbers before it is decided to crossover.
        */
        unsigned int choose1 = Rand::getOpen(0, matingPoolSize), choose2 = Rand::getOpen(0, matingPoolSize);

        if((unsigned int)Rand::getClose(0, 1000) < population->crossoverProb){

            std::pair<Individual*, Individual*> children(pool[choose1]->crossover(pool[choose2], population->crossoverProb));

            if(children.first == nullptr) continue;

            if(newGenPos + 1 == population->populationSize){

                mutation(children.first);
                if(children.first->fitness() < best->fitness()) best = children.first;
                newResidents[newGenPos++] = children.first;

            } else{

                mutation(children.first);
                if(children.first->fitness() < best->fitness()) best = children.first;
                newResidents[newGenPos++] = children.first;

                mutation(children.second);
                if(children.second->fitness() < best->fitness()) best = children.second;
                newResidents[newGenPos++] = children.second;

            }
        }
    }
    return best;
}

void Population::MatingPool::mutation(Individual *p){
    p->mutation(population->mutationProb);
}

//
// Population
//

Population::Population()
:residents(nullptr), maxFinishingTime(0), matingPool(this, UINT_MAX), stillPrepare(true), \
populationSize(UINT_MAX), maxNumberOfGeneration(UINT_MAX), crossoverProb(UINT_MAX), \
mutationProb(UINT_MAX), numberOfGenerWithoutImprov(UINT_MAX), dataContainer(nullptr), \
PCl1(UINT_MAX), PCg1(UINT_MAX), PCl2(UINT_MAX), PCg2(UINT_MAX), PCl12(UINT_MAX), PCg12(UINT_MAX), \
PMl1(UINT_MAX), PMg1(UINT_MAX), PMl2(UINT_MAX), PMg2(UINT_MAX), PMl12(UINT_MAX), PMg12(UINT_MAX)
{}

Population::~Population(){
    if(populationSize != UINT_MAX && !stillPrepare){
        for (unsigned int i = 0; i < populationSize; ++i){
            delete residents[i];

        }
    }

    delete[] residents;
}

bool Population::checkData(){
    if(populationSize == UINT_MAX || maxNumberOfGeneration == UINT_MAX || crossoverProb == UINT_MAX || \
    mutationProb == UINT_MAX || numberOfGenerWithoutImprov == UINT_MAX || dataContainer == nullptr) return true;

    if(populationSize < 4 || crossoverProb > 1000 || mutationProb > 1000 || \
    maxNumberOfGeneration < numberOfGenerWithoutImprov + 1) return true;

    if(PCl1 == UINT_MAX || PCg1 == UINT_MAX || PCl2 == UINT_MAX || PCg2 == UINT_MAX || PCl12 == UINT_MAX || PCg12 == UINT_MAX || \
    PMl1 == UINT_MAX || PMg1 == UINT_MAX || PMl2 == UINT_MAX || PMg2 == UINT_MAX || PMl12 == UINT_MAX || PMg12 == UINT_MAX) return true;

    return matingPool.checkMatingPoolData();
}

void Population::createFirstPopulation(const unsigned int &randLoops){
    // randomize their features
    #ifdef DEBUG
    if(populationSize == UINT_MAX) throw std::runtime_error("Failed to create first population. populationSize not initialized.");
    if(residents != nullptr) throw std::runtime_error("Residents are initialized but they should not be.");
    #endif

    residents = new Individual*[populationSize];
    for (unsigned int i = 0; i < populationSize; ++i){
        residents[i] = new Individual(dataContainer->getNumberOfTasks(), this);
        residents[i]->randFeatures(dataContainer->getTasksVector(), dataContainer->getNumberOfProcessors(), randLoops);
    }
}

void Population::prepareMatingPool(){
    matingPool.newMatingPool();
}

void Population::run(const unsigned int &randLoops){
    matingPool.setMatingPoolSize(populationSize / 2);
    if(checkData()) throw "Something went wrong. Cannot run algorithm";

    prepareMatingPool();
    createFirstPopulation(randLoops);

    stillPrepare = false;

    Individual *best = nullptr;
    unsigned int minDelay = UINT_MAX, withoutImprovement = 0;

    for(unsigned int i = 0; i < maxNumberOfGeneration; ++i){

        best = matingPool.createNewPopulation(residents);

        if(best->fitness() == 0) break;

        if(best->fitness() < minDelay) {
            minDelay = best->fitness();
            withoutImprovement = 0;
        } else ++withoutImprovement;

        if(withoutImprovement == numberOfGenerWithoutImprov) break;
    }

    best->setOutputData(dataContainer->taskSchedule, dataContainer->getNumberOfProcessors());

    // results
}

#endif //POPULATION_CPP
