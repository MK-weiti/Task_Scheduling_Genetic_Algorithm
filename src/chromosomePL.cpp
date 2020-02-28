#ifndef CHROMOSOMEPL_CPP
#define CHROMOSOMEPL_CPP

#include "chromosomePL.h"

using namespace task_scheduling;

// create specialization
template <>
Chromosome<unsigned int>::Chromosome(const Chromosome *other)
:chSize(other->chSize), randGene(0, other->chSize)
{
    genes = new unsigned int*[chSize];

    // copy integers
    for (unsigned int i = 0; i < chSize; ++i){
        genes[i] = new unsigned int (*(other->genes[i]));
    }
}

template <>
Chromosome<unsigned int>::~Chromosome(){
    // delete integers
    for (unsigned int i = 0; i < chSize; ++i){
        delete genes[i];
    }
    delete[] genes;
}

template <>
void Chromosome<unsigned int>::copy(const unsigned int &startPos, const unsigned int &endPos, const Chromosome<unsigned int> *from){
    #ifdef DEBUG
    if(startPos >= endPos || from == nullptr) throw std::runtime_error("Copy of chromosome failed.");
    #endif

    for (unsigned int i = startPos; i < endPos; ++i){
        #ifdef DEBUG
        if(genes[i] != nullptr) throw std::runtime_error("Can copy only when points to nullptr.");
        #endif
        genes[i] = new unsigned int (*(from->genes[i]));
        #ifdef DEBUG
        if(*genes[i] != *(from->genes[i])) throw std::runtime_error("Copy of part of the chromosome failed.");
        #endif
    }

}

/////////////////////////////////////////

ChromosomePL::ChromosomePL(const unsigned int &size)
:Chromosome(size)
{}

ChromosomePL::~ChromosomePL()
{}

ChromosomePL::ChromosomePL(const ChromosomePL *chrom)
:Chromosome(chrom)
{}

std::pair<ChromosomePL*, ChromosomePL*> ChromosomePL::crossover(const ChromosomePL *chromosome) const{
    ChromosomePL *one = new ChromosomePL(chSize), *two = new ChromosomePL(chSize);

    unsigned int randPoint = randGene.getOpen();

    one->copy(0, randPoint, this);
    one->copy(randPoint, chSize, chromosome);

    two->copy(0, randPoint, chromosome);
    two->copy(randPoint, chSize, this);

    return std::pair<ChromosomePL*, ChromosomePL*>(one, two);
}

void ChromosomePL::setRand(const unsigned int &numberOfProcessors){

    for (unsigned int i = 0; i < chSize; ++i){
        *genes[i] = Rand::getOpen(0, numberOfProcessors);
    }
}

void ChromosomePL::mutation(const unsigned int &numberOfProcessors){

    //Chose between swapping genes or to change the value
    if(Rand::getOpen(0, 2)) Chromosome<unsigned int>::mutation();
    else {
        *(this->genes[randGene.getOpen()]) = Rand::getOpen(0, numberOfProcessors);
    }
}


#endif //CHROMOSOMEPL_CPP
