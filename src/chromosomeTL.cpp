#ifndef CHROMOSOMETL_CPP
#define CHROMOSOMETL_CPP

#include "chromosomeTL.h"


using namespace task_scheduling;

ChromosomeTL::ChromosomeTL(const unsigned int &size)
:Chromosome(size)
{}


ChromosomeTL::~ChromosomeTL()
{}

ChromosomeTL::ChromosomeTL(const ChromosomeTL* chrom)
:Chromosome(chrom)
{}

std::pair<ChromosomeTL*, ChromosomeTL*> ChromosomeTL::crossover(const ChromosomeTL *chromosome) const{
    ChromosomeTL *one = new ChromosomeTL(chSize), *two = new ChromosomeTL(chSize);

    unsigned int randPoint = Rand::getOpen(1, chSize-1);
    unsigned int copyOne = randPoint, copyTwo = randPoint;
    bool foundOne = false, foundTwo = false;

    // copy without randPoint
    #ifdef DEBUG
    std::cout << "Crossover copy randPoint: " << randPoint << std::endl;
    #endif
    one->copy(0, randPoint, this);
    two->copy(0, randPoint, chromosome);

    #ifdef DEBUG
    for(unsigned int i = randPoint; i < chSize; ++i){
        one->genes[i] = nullptr;
        two->genes[i] = nullptr;
    }
    #endif

    // change second half
    for (unsigned int loop = 0; loop < chSize; ++loop){    // check if this gene is repeated somewhere else
        foundOne = false;
        foundTwo = false;

        for(unsigned int i = 0; i < copyOne; ++i){     // compare these genes for first chromosome
            if(chromosome->genes[loop] == one->genes[i]) {
                foundOne= true;
                break;
            }
        }

        if(!foundOne){
            one->genes[copyOne] = chromosome->genes[loop];
            ++copyOne;
        }

        // mirror

        for(unsigned int i = 0; i < copyTwo; ++i){      // compare these genes for second chromosome
            if(this->genes[loop] == two->genes[i]) {
                foundTwo= true;
                break;
            }
        }

        if(!foundTwo){
            two->genes[copyTwo] = this->genes[loop];
            ++copyTwo;
        }
    }

    #ifdef DEBUG
    if(copyOne != chSize || copyTwo != chSize) throw std::runtime_error("Crossover TL failed");
    for(unsigned int i = 0; i < chSize; ++i){
        if(one->genes[i] == nullptr || two->genes[i] == nullptr) throw std::runtime_error("Crossover TL failed");
    }
    #endif

    return std::pair<ChromosomeTL*, ChromosomeTL*>(one, two);
    }


#endif //CHROMOSOMETL_CPP
