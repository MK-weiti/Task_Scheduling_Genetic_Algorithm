#ifndef RAND_CPP
#define RAND_CPP

#include "rand.h"

Rand::Rand(const int beginRange, const int endRange)
:begin(beginRange), end(endRange)
{
    if(endRange - beginRange <= 0) throw "Bad range:  Rand::Rand()";
    delta = endRange - beginRange;
}

Rand::~Rand()
{}

void Rand::newSeed(){
    srand(time(0));
}

void Rand::newSeed(const int seed){
    srand(seed);
}

int Rand::getClose() const{
    return (rand() % delta) + begin + 1;
}

const int Rand::getCloseConst(){
    return getClose();
}

int Rand::getOpen() const{
    return (rand() % delta) + begin;
}

const int Rand::getOpenConst(){
    return getOpen();
}

int Rand::getClose(const int beginRange, const int endRange){
    #ifdef DEBUG
    if(endRange - beginRange <= 0) throw "Bad range: Rand::get()";
    #endif
    return (rand() % (endRange - beginRange)) + beginRange + 1;
}

int Rand::getOpen(const int beginRange, const int endRange){
    #ifdef DEBUG
    if(endRange - beginRange <= 0) throw "Bad range: Rand::get()";
    #endif
    return (rand() % (endRange - beginRange)) + beginRange;
}


#endif //RAND_CPP
