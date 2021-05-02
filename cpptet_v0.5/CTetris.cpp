#include "CTetris.h"
CTetris::CTetris(int cy, int cx) : Tetris(cy, cx) {
    arrayScreen = createArrayScreen();
    iCScreen = Matrix(arrayScreen, arrayScreenDy, arrayScreenDx);
    oCScreen = Matrix(iCScreen);
}
int arrayBlks[7][4][4][4] =  {{{{0, 0, 1, 0},{0, 0, 1, 0},{0, 0, 1, 0},{0, 0, 1, 0}},
                               {{0, 0, 0, 0},{0, 0, 0, 0},{1, 1, 1, 1},{0, 0, 0, 0}},
                               {{0, 1, 0, 0},{0, 1, 0, 0},{0, 1, 0, 0},{0, 1, 0, 0}},
                               {{0, 0, 0, 0},{1, 1, 1, 1},{0, 0, 0, 0},{0, 0, 0, 0}}},
                              {{{1, 0, 0, 0},{1, 1, 1, 0},{0, 0, 0, 0},{0, 0, 0, 0}},
                               {{0, 1, 1, 0},{0, 1, 0, 0},{0, 1, 0, 0},{0, 0, 0, 0}},
                               {{0, 0, 0, 0},{1, 1, 1, 0},{0, 0, 1, 0},{0, 0, 0, 0}},
                               {{0, 1, 0, 0},{0, 1, 0, 0},{1, 1, 0, 0},{0, 0, 0, 0}}},
                              {{{0, 0, 1, 0},{1, 1, 1, 0},{0, 0, 0, 0},{0, 0, 0, 0}},
                               {{0, 1, 0, 0},{0, 1, 0, 0},{0, 1, 1, 0},{0, 0, 0, 0}},
                               {{0, 0, 0, 0},{1, 1, 1, 0},{1, 0, 0, 0},{0, 0, 0, 0}},
                               {{1, 1, 0, 0},{0, 1, 0, 0},{0, 1, 0, 0},{0, 0, 0, 0}}},
                              {{{0, 1, 1, 0},{0, 1, 1, 0},{0, 0, 0, 0},{0, 0, 0, 0}},
                               {{0, 1, 1, 0},{0, 1, 1, 0},{0, 0, 0, 0},{0, 0, 0, 0}},
                               {{0, 1, 1, 0},{0, 1, 1, 0},{0, 0, 0, 0},{0, 0, 0, 0}},
                               {{0, 1, 1, 0},{0, 1, 1, 0},{0, 0, 0, 0},{0, 0, 0, 0}}},
                              {{{0, 1, 1, 0},{1, 1, 0, 0},{0, 0, 0, 0},{0, 0, 0, 0}},
                               {{0, 1, 0, 0},{0, 1, 1, 0},{0, 0, 1, 0},{0, 0, 0, 0}},
                               {{0, 0, 0, 0},{0, 1, 1, 0},{1, 1, 0, 0},{0, 0, 0, 0}},
                               {{1, 0, 0, 0},{1, 1, 0, 0},{0, 1, 0, 0},{0, 0, 0, 0}}},
                              {{{0, 1, 0, 0},{1, 1, 1, 0},{0, 0, 0, 0},{0, 0, 0, 0}},
                               {{0, 1, 0, 0},{0, 1, 1, 0},{0, 1, 0, 0},{0, 0, 0, 0}},
                               {{0, 0, 0, 0},{1, 1, 1, 0},{0, 1, 0, 0},{0, 0, 0, 0}},
                               {{0, 1, 0, 0},{1, 1, 0, 0},{0, 1, 0, 0},{0, 0, 0, 0}}},
                              {{{1, 1, 0, 0},{0, 1, 1, 0},{0, 0, 0, 0},{0, 0, 0, 0}},
                               {{0, 0, 1, 0},{0, 1, 1, 0},{0, 1, 0, 0},{0, 0, 0, 0}},
                               {{0, 0, 0, 0},{1, 1, 0, 0},{0, 1, 1, 0},{0, 0, 0, 0}},
                               {{0, 1, 0, 0},{1, 1, 0, 0},{1, 0, 0, 0},{0, 0, 0, 0}}}};
CTetris::~CTetris(){
    for(int i = 0; i< nBlockTypes; i++)
        delete[] setOfCBlockObjects[i];
    delete[] setOfCBlockObjects;
}

Matrix** CTetris::setOfCBlockObjects =0;
void CTetris::init(int**** setOfBlockArrays, int max_type, int max_degree){
    setOfBlockArrays = new int***[max_type];
    for(int i=0;i<max_type;i++){
        setOfBlockArrays[i] = new int**[max_degree];
        for(int j=0;j<max_degree;j++){
            setOfBlockArrays[i][j] = new int*[4];
            for(int k=0;k<4;k++){
                setOfBlockArrays[i][j][k] = new int[4];
            }
        }
    }

    for(int i=0;i<max_type;i++){
        for(int j=0;j<max_degree;j++){
            for(int k=0;k<4;k++){
                for(int l=0;l<4;l++)
                setOfBlockArrays[i][j][k][l] = arrayBlks[i][j][k][l];
            }
        }
    }

    Tetris::init(setOfBlockArrays, max_type, max_degree);
    int arrayBlk_maxSize = 4;
    setOfCBlockObjects = new Matrix*[max_type];
    for(int i = 0;i<nBlockTypes;i++)
        setOfCBlockObjects[i] = new Matrix[nBlockDegrees];
    
    for(int i=0;i<nBlockTypes;i++){
        for(int j=0;j<nBlockDegrees;j++){
            int idxBlk[arrayBlk_maxSize * arrayBlk_maxSize];
            for(int k=0;k<arrayBlk_maxSize;k++)
                for(int l=0;l<arrayBlk_maxSize;l++)
                    idxBlk[k * arrayBlk_maxSize + l] = setOfBlockArrays[i][j][k][l];
            setOfCBlockObjects[i][j] = Matrix(idxBlk, arrayBlk_maxSize, arrayBlk_maxSize);
            setOfCBlockObjects[i][j].mulc(i+1);
        }
    }

    for(int i=0;i<max_type;i++){
        for(int j=0;j<max_degree;j++){
            for(int k=0;k<4;k++){
                delete[] setOfBlockArrays[i][j][k];
            }
            delete[] setOfBlockArrays[i][j];
        }
        delete[] setOfBlockArrays[i];
    }
    delete[] setOfBlockArrays;
}

TetrisState CTetris::accept(int key){
    if(key >= '0' && key <= '6'){
        if(justStarted == false)
            deleteFullLines();
        iCScreen = Matrix(oCScreen);
    }
    state = Tetris::accept(key);
    currCBlk = setOfCBlockObjects[idxBlockType][idxBlockDegree];
    tempBlk = iCScreen.clip(top, left, top+currCBlk.get_dy(),left+currCBlk.get_dx());
    tempBlk = tempBlk.add(&currCBlk);

    oCScreen = Matrix(iCScreen);
    oCScreen.paste(&tempBlk, top, left);

    return state;
}
void CTetris::deleteFullLines(){
    bool deleteline = false;
    int* c_array = new int[arrayScreenDy*arrayScreenDx];
    int** carray = oCScreen.get_array();
    for(int y=0;y<arrayScreenDy-iScreenDw;y++){
        deleteline = true;
        for(int x=iScreenDw;x<arrayScreenDx-iScreenDw;x++){
            if(carray[y][x] == 0){
                deleteline = false;
                break;
            }
        }
        if(deleteline == true){
            int tempY = y;
            for(int z=tempY;z>=0;z--){
                for(int x=iScreenDw;x<arrayScreenDx-iScreenDw;x++){
                    if(z == 0)
                        carray[z][x] = 0;
                    else
                        carray[z][x] = carray[z-1][x];
                }
            }

        }
    }
    for(int y =0;y<arrayScreenDy;y++)
        for(int x=0;x<arrayScreenDx;x++)
            c_array[y*arrayScreenDx + x] = carray[y][x];

    oCScreen = Matrix(c_array, arrayScreenDy, arrayScreenDx);

    delete[] c_array;
}