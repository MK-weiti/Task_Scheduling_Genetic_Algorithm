#ifndef CHROMOSOME_CPP
#define CHROMOSOME_CPP

#include "chromosome.h"

using namespace task_scheduling;

template <typename ArrayType>
Chromosome<ArrayType>::Chromosome(const unsigned int &size)
:chSize(size), randGene(0, size)
{
    #ifdef DEBUG
    if(size == UINT_MAX) throw std::runtime_error("Chromosome initial size not set (UINT_MAX).");
    #endif
    genes = new ArrayType*[size];

    #ifdef DEBUG
    for(unsigned int i = 0; i < size; ++i){
        genes[i] = nullptr;
    }
    #endif
}

template <typename ArrayType>
Chromosome<ArrayType>::~Chromosome(){
    // delete only array
    delete[] genes;
}

// copy only pointers
template <typename ArrayType>
Chromosome<ArrayType>::Chromosome(const Chromosome *other)
:chSize(other->chSize), randGene(0, other->chSize)
{
    #ifdef DEBUG
    if(chSize == UINT_MAX) throw std::runtime_error("Chromosome not initialized. chSize not set.");
    #endif
    genes = new ArrayType*[chSize];

    for (unsigned int i = 0; i < chSize; ++i){
        genes[i] = other->genes[i];
    }

    #ifdef DEBUG
    for (unsigned int i = 0; i < chSize; ++i){
        if(*(genes[i]) != *(other->genes[i])) throw std::runtime_error("Copy of chromosome failed.");
    }
    #endif
}

template <typename ArrayType>
ArrayType*& Chromosome<ArrayType>::getPointerToGene(const size_t &pos){
    #ifdef DEBUG
    if(pos >= chSize) throw std::runtime_error("Out of range.");
    #endif
    return genes[pos];
}

template <typename ArrayType>
ArrayType*& Chromosome<ArrayType>::getPointerToGene(const size_t &pos) const{
    #ifdef DEBUG
    if(pos >= chSize) throw std::runtime_error("Out of range.");
    if(genes[pos] == nullptr) throw std::runtime_error("Gene points to nullptr.");
    #endif
    return genes[pos];
}

template <typename ArrayType>
void Chromosome<ArrayType>::mutation(){

    #ifdef DEBUG
    ArrayType *tmpPointer1, *tmpPointer2;
    if(chSize == UINT_MAX || genes[chSize-1] == nullptr) throw std::runtime_error("Mutation of chromosome failed. Chromosome not initialized");
    #endif

    int rand1 = randGene.getOpen(), rand2 = randGene.getOpen();
    ArrayType *tmp;

    if(rand1 == rand2) rand2 = (rand2 + 1) % chSize;

    #ifdef DEBUG
    tmpPointer1 = this->genes[rand1];
    tmpPointer2 = this->genes[rand2];
    #endif

    tmp = this->genes[rand2];
    this->genes[rand2] = this->genes[rand1];
    this->genes[rand1] = tmp;

    #ifdef DEBUG
    if(tmpPointer1 != this->genes[rand2] || tmpPointer2 != this->genes[rand1]) throw std::runtime_error("Mutation of chromosome failed");
    #endif
}

template <typename ArrayType>
unsigned int Chromosome<ArrayType>::size() const{
    #ifdef DEBUG
    if(chSize == UINT_MAX) throw std::runtime_error("Chromosome not initialized.");
    #endif
    return chSize;
}

// copy only pointers
template <typename ArrayType>
void Chromosome<ArrayType>::copy(const unsigned int &startPos, const unsigned int &endPos, const Chromosome<ArrayType> *from){
    #ifdef DEBUG
    std::cout << "Chromosome<ArrayType>::copy position " << startPos << " " << endPos << std::endl;
    if(startPos > endPos || from == nullptr) throw std::runtime_error("Copy of chromosome failed.");

    for (unsigned int i = 0; i < chSize; ++i){
        if(from->genes[i] == nullptr) throw std::runtime_error("Copy of chromosome failed. Cannot copy nullptr.");
    }
    #endif

    for (unsigned int i = startPos; i < endPos; ++i){
        genes[i] = from->genes[i];
    }

    #ifdef DEBUG
    for (unsigned int i = startPos; i < endPos; ++i){
        std::cout << "Chromosome<ArrayType>::copy genes " << *(genes[i]) << " " << *(from->genes[i]) << std::endl;
        if(*(genes[i]) != *(from->genes[i])) throw std::runtime_error("Copy of chromosome failed.");
    }
    #endif
}


#endif //CHROMOSOME_CPP
