from matrix import *
from tetris import *
from enum import Enum

class CTetris(Tetris):
    @classmethod
    def init(cls, setOfBlockArrays):
        Tetris.nBlockTypes = len(setOfBlockArrays)
        Tetris.nBlockDegrees = len(setOfBlockArrays[0])
        Tetris.setOfBlockObjects = [[0] * Tetris.nBlockDegrees for _ in range(Tetris.nBlockTypes)]
        colorblk = [[0] * Tetris.nBlockDegrees for _ in range(Tetris.nBlockTypes)]
        arrayBlk_maxSize = 0
        for i in range(Tetris.nBlockTypes):
            if arrayBlk_maxSize <= len(setOfBlockArrays[i][0]):
                arrayBlk_maxSize = len(setOfBlockArrays[i][0])
        Tetris.iScreenDw = arrayBlk_maxSize     # larget enough to cover the largest block
        for i in range(Tetris.nBlockTypes):
            for j in range(Tetris.nBlockDegrees):
                colorblk[i][j] = setOfBlockArrays[i][j]
                blkSize_x = len(setOfBlockArrays[i][j])
                for y in range(blkSize_x):
                    blkSize_y = len(setOfBlockArrays[i][j][y])
                    for x in range(blkSize_y):
                        if(colorblk[i][j][y][x] == 1):
                            colorblk[i][j][y][x] = i+1
                Tetris.setOfBlockObjects[i][j] = Matrix(colorblk[i][j])
                        
    def accept(self, key):
        self.state = TetrisState.Running

        if key >= '0' and key <= '6':
            if self.justStarted == False:
                self.deleteFullLines()
            self.iScreen = Matrix(self.oScreen)
            self.idxBlockType = int(key)
            self.idxBlockDegree = 0
            self.currBlk = Tetris.setOfBlockObjects[self.idxBlockType][self.idxBlockDegree]
            self.top = 0
            self.left = Tetris.iScreenDw + self.iScreenDx//2 - self.currBlk.get_dx()//2
            self.tempBlk = self.iScreen.clip(self.top, self.left, self.top+self.currBlk.get_dy(), self.left+self.currBlk.get_dx())
            self.undoBlk = self.tempBlk.binary() + self.currBlk.binary() #undo judgement blk
            self.tempBlk = self.tempBlk + self.currBlk
            self.justStarted = False
            print()

            if self.undoBlk.anyGreaterThan(1):
                self.state = TetrisState.Finished
            self.oScreen = Matrix(self.iScreen)
            self.oScreen.paste(self.tempBlk, self.top, self.left)
            return self.state
        elif key == 'q':
            pass
        elif key == 'a': # move left
            self.left -= 1
        elif key == 'd': # move right
            self.left += 1
        elif key == 's': # move down
            self.top += 1
        elif key == 'w': # rotate the block clockwise
            self.idxBlockDegree = (self.idxBlockDegree + 1) % Tetris.nBlockDegrees
            self.currBlk = Tetris.setOfBlockObjects[self.idxBlockType][self.idxBlockDegree]
        elif key == ' ': # drop the block
            while not self.undoBlk.anyGreaterThan(1):
                    self.top += 1
                    self.tempBlk = self.iScreen.clip(self.top, self.left, self.top+self.currBlk.get_dy(), self.left+self.currBlk.get_dx())
                    self.undoBlk = self.tempBlk.binary() + self.currBlk.binary() #undo judgement blk
                    self.tempBlk = self.tempBlk + self.currBlk
        else:
            print('Wrong key!!!')
            
        self.tempBlk = self.iScreen.clip(self.top, self.left, self.top+self.currBlk.get_dy(), self.left+self.currBlk.get_dx())
        self.undoBlk = self.tempBlk.binary() + self.currBlk.binary() #undo judgement blk
        self.tempBlk = self.tempBlk + self.currBlk

        if self.undoBlk.anyGreaterThan(1):   ## 벽 충돌시 undo 수행
            if key == 'a': # undo: move right
                self.left += 1
            elif key == 'd': # undo: move left
                self.left -= 1
            elif key == 's': # undo: move up
                self.top -= 1
                self.state = TetrisState.NewBlock
            elif key == 'w': # undo: rotate the block counter-clockwise
                self.idxBlockDegree = (self.idxBlockDegree - 1) % Tetris.nBlockDegrees
                self.currBlk = Tetris.setOfBlockObjects[self.idxBlockType][self.idxBlockDegree]
            elif key == ' ': # undo: move up
                self.top -= 1
                self.state = TetrisState.NewBlock
            
            self.tempBlk = self.iScreen.clip(self.top, self.left, self.top+self.currBlk.get_dy(), self.left+self.currBlk.get_dx())
            self.undoBlk = self.tempBlk.binary() + self.currBlk.binary()
            self.tempBlk = self.tempBlk + self.currBlk

        self.oScreen = Matrix(self.iScreen)
        self.oScreen.paste(self.tempBlk, self.top, self.left)

        return self.state
    
    def deleteFullLines(self):
        array = self.oScreen.get_array()
        deleteLine = False
        for y in range(self.oScreen.get_dy()-Tetris.iScreenDw):
            deleteLine = True
            for x in range(Tetris.iScreenDw, self.oScreen.get_dx()-Tetris.iScreenDw):
                if array[y][x] == 0:
                    deleteLine = False
                    break;
            if deleteLine == True:
                tempY = y
                for z in range(tempY, 0, -1):
                    for x in range(Tetris.iScreenDw, self.oScreen.get_dx()-Tetris.iScreenDw):
                        if z == 0:
                            array[z][x] = 0
                        else:
                            array[z][x] = array[z-1][x]
        self.oScreen = Matrix(array)
        return
    