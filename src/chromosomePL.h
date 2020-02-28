#ifndef CHROMOSOMEPL_H
#define CHROMOSOMEPL_H

#include "chromosome.h"

namespace task_scheduling{

    /*
    Tasks are sorted by their ID.
    It can be checked in DataContainer.
    */
    class ChromosomePL: public Chromosome<unsigned int>{

    public:
        ChromosomePL(const unsigned int &size);
        ~ChromosomePL();
        ChromosomePL(const ChromosomePL *chrom);

        std::pair<ChromosomePL*, ChromosomePL*> crossover(const ChromosomePL *chromosome) const;

        void setRand(const unsigned int &numberOfProcessors);

        void mutation(const unsigned int &numberOfProcessors);
    };
};


#endif //CHROMOSOMEPL_H
