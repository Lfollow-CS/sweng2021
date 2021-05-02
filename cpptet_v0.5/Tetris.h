#pragma once
#include "Matrix.h"
#include<string.h>

enum TetrisState { Running, NewBlock, Finished };

class Tetris {
    public:
        int idxBlockDegree;
        int idxBlockType;
        int arrayScreenDx;
        int arrayScreenDy;
        int top;
        int left;
        int iScreenDx;
        int iScreenDy;
        bool justStarted;
        int* arrayScreen;
        Matrix iScreen;
        Matrix oScreen;
        Matrix currBlk;
        Matrix tempBlk;
        TetrisState state;

        static int nBlockTypes;
        static int nBlockDegrees;
        static int iScreenDw;
        static Matrix** setOfBlockObjects;
        
        Tetris(int cy, int cx);
        virtual ~Tetris();
        static void init(int**** setOfCBlockArrays, int max_blk, int max_degree);
        int* createArrayScreen();
        TetrisState accept(int key);
        void deleteFullLines();
};

