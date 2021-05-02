#pragma once
#include "Tetris.h"

class CTetris : public Tetris {
    public:
        Matrix iCScreen;
        Matrix oCScreen;
        Matrix currCBlk;
        Matrix tempBlk;

        static Matrix** setOfCBlockObjects;
        
        CTetris(int cy, int cx);
        ~CTetris();
        static void init(int**** setOfBlockArrays, int max_type, int max_degree);
        TetrisState accept(int key);
        void deleteFullLines();
};


