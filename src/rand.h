#ifndef RAND_H
#define RAND_H

#include <time.h>
#include <stdlib.h>

class Rand{
    int begin, end;
    unsigned int delta;

public:
    Rand(const int beginRange, const int endRange);
    ~Rand();

    int getClose() const;
    const int getCloseConst();
    int getOpen() const;
    const int getOpenConst();
    static int getClose(const int beginRange, const int endRange);
    static int getOpen(const int beginRange, const int endRange);


    static void newSeed();
    static void newSeed(const int seed);
};


#endif //RAND_H
