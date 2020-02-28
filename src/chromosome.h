#ifndef CHROMOSOME_H
#define CHROMOSOME_H

#include <utility>
#include <vector>
#include "dataContainer.h"
#include "rand.h"

#ifdef DEBUG
#include <exception>
#include <bits/stdc++.h>
#endif

namespace task_scheduling{

    template <typename ArrayType>
    class Chromosome{
    protected:
        unsigned int chSize;
        Rand randGene;
        ArrayType **genes;

    public:
        Chromosome(const unsigned int &size);
        virtual ~Chromosome();
        Chromosome(const Chromosome *other);

        /*
        Get direct access to gene.
        */
        ArrayType*& getPointerToGene(const size_t &pos);
        ArrayType*& getPointerToGene(const size_t &pos) const;

        /*
        Swap two genes in the array.
        These genes are randomly selected.
        */
        void mutation();

        void copy(const unsigned int &startPos, const unsigned int &endPos, const Chromosome<ArrayType> *from);

        unsigned int size() const;

        friend std::ostream& operator<< (std::ostream& os, const Chromosome &chrom){
            for (unsigned int i = 0; i < chrom.size(); ++i){
                os << *chrom.getPointerToGene(i) << std::endl;
            }
            return os;
        }
    };

};


#include "chromosome.cpp"

#endif //CHROMOSOME_H

