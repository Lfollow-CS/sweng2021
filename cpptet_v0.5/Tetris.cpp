#include "Tetris.h"

Tetris::Tetris(int cy, int cx){
    iScreenDx = cx;
    iScreenDy = cy;
    idxBlockDegree = 0;
    arrayScreen = createArrayScreen();
    iScreen = Matrix(arrayScreen, arrayScreenDy, arrayScreenDx);
    oScreen = Matrix(iScreen);
    justStarted = true;
}
Tetris::~Tetris(){
    for(int i = 0; i< nBlockTypes; i++)
        delete[] setOfBlockObjects[i];
    delete[] setOfBlockObjects;

    delete[] arrayScreen;
}

int Tetris::nBlockTypes = 0;
int Tetris::nBlockDegrees = 0;
Matrix** Tetris::setOfBlockObjects = 0;
int Tetris::iScreenDw = 0;

void Tetris::init(int**** setOfBlockArrays, int max_blk, int max_degree){
    nBlockTypes = max_blk;
    nBlockDegrees = max_degree;
    int arrayBlk_maxSize = 4;
    
    Tetris::setOfBlockObjects = new Matrix*[nBlockTypes];
    for(int i = 0;i<nBlockTypes;i++)
        setOfBlockObjects[i] = new Matrix[nBlockDegrees];
    
    iScreenDw = arrayBlk_maxSize;

    for(int i = 0;i<nBlockTypes;i++){
        for(int j=0;j<nBlockDegrees;j++){
            int idxBlk[arrayBlk_maxSize * arrayBlk_maxSize];
            for(int k=0;k<arrayBlk_maxSize;k++)
                for(int l=0;l<arrayBlk_maxSize;l++)
                    idxBlk[k * arrayBlk_maxSize + l] = setOfBlockArrays[i][j][k][l];
            setOfBlockObjects[i][j] = Matrix(idxBlk, arrayBlk_maxSize, arrayBlk_maxSize);
        } 
    }
}

int* Tetris::createArrayScreen(){
    arrayScreenDx = (iScreenDw*2) + iScreenDx;
    arrayScreenDy = iScreenDy + iScreenDw;
    arrayScreen = new int[arrayScreenDy * arrayScreenDx];

    for(int y=0;y<iScreenDy;y++){
        for(int x=0;x<iScreenDw;x++){
            arrayScreen[y * arrayScreenDx + x] = 1;
        }
        for(int x=0;x<iScreenDx;x++){
            arrayScreen[y * arrayScreenDx + iScreenDw + x] = 0;
        }
        for(int x=0;x<iScreenDw;x++){
            arrayScreen[y * arrayScreenDx + iScreenDw + iScreenDx + x] = 1;
        }
    }

    for(int y=0;y<iScreenDw;y++){
        for(int x=0;x<arrayScreenDx;x++){
            arrayScreen[(iScreenDy+y) * arrayScreenDx + x] = 1;
        }
    }
    return arrayScreen;
}

TetrisState Tetris::accept(int key){
    state = Running;

    if(key >= '0' && key <= '6'){
        if(justStarted == false)
            deleteFullLines();
        iScreen = Matrix(oScreen);
        idxBlockType = int(key-'0');
        idxBlockDegree = 0;
        currBlk = setOfBlockObjects[idxBlockType][idxBlockDegree];
        
        top = 0;
        left = iScreenDw + (iScreenDx/2) - (currBlk.get_dx()/2);
        
        tempBlk = iScreen.clip(top, left, top+currBlk.get_dy(), left+currBlk.get_dx());
        tempBlk = tempBlk.add(&currBlk);
        justStarted = false;

        cout << endl;

        if(tempBlk.anyGreaterThan(1))
            state = Finished;
        oScreen = Matrix(iScreen);
        oScreen.paste(&tempBlk, top, left);

        return state;
    }
    else if(key == 'q'){}
    else if(key == 'a'){
        left -= 1;
    }
    else if(key == 'd'){
        left += 1;
    }
    else if(key == 's'){
        top += 1;
    }
    else if(key == 'w'){
        idxBlockDegree = (idxBlockDegree + 1) % nBlockDegrees;
        currBlk = setOfBlockObjects[idxBlockType][idxBlockDegree];
    }
    else if(key == ' '){
        while(!(tempBlk.anyGreaterThan(1))){
            top += 1;
            tempBlk = iScreen.clip(top, left, top+currBlk.get_dy(), left+currBlk.get_dx());
            tempBlk = tempBlk.add(&currBlk);
        }
    }
    else{
        cout << "wrong key!!" << endl;
    }
    tempBlk = iScreen.clip(top, left, top+currBlk.get_dy(), left+currBlk.get_dx());
    tempBlk = tempBlk.add(&currBlk);
    if(tempBlk.anyGreaterThan(1)){
        if(key == 'a')
            left += 1;
        else if(key == 'd')
            left -= 1;
        else if(key == 's'){
            top -= 1;
            state = NewBlock;
        }
        else if(key == 'w'){
            idxBlockDegree = (idxBlockDegree + 3) % nBlockDegrees;
            currBlk = setOfBlockObjects[idxBlockType][idxBlockDegree];
        }
        else if(key == ' '){
            top -= 1;
            state = NewBlock;
        }
        tempBlk = iScreen.clip(top, left, top+currBlk.get_dy(), left+currBlk.get_dx());
        tempBlk = tempBlk.add(&currBlk);
    }
    
    oScreen = Matrix(iScreen);
    oScreen.paste(&tempBlk, top, left);

    return state;
}
void Tetris::deleteFullLines(){
    bool deleteline = false;
    int* r_array = new int[arrayScreenDy*arrayScreenDx];
    int** array = oScreen.get_array();
    for(int y=0;y<arrayScreenDy-iScreenDw;y++){
        deleteline = true;
        for(int x=iScreenDw;x<arrayScreenDx-iScreenDw;x++){
            if(array[y][x] == 0){
                deleteline = false;
                break;
            }
        }
        if(deleteline == true){
            int tempY = y;
            for(int z=tempY;z>=0;z--){
                for(int x=iScreenDw;x<arrayScreenDx-iScreenDw;x++){
                    if(z == 0)
                        array[z][x] = 0;
                    else
                        array[z][x] = array[z-1][x];
                }
            }

        }
    }
    for(int y =0;y<arrayScreenDy;y++)
        for(int x=0;x<arrayScreenDx;x++)
            r_array[y*arrayScreenDx + x] = array[y][x];


    oScreen = Matrix(r_array, arrayScreenDy, arrayScreenDx); 

    delete[] r_array;
}