from game import *
from matrix import *

##################################################
### ColorDecorator for Tetris Class
##################################################

class ColorDecorator(Game):
    def initCBlocks(self, setOfBlockObjects):
        ### initialize self.setOfCBlockObjects
        self.setOfCBlockObjects = [[0] * self.game.nBlockDegrees for _ in range(self.game.nBlockTypes)]
        for i in range(self.game.nBlockTypes):
            for j in range(self.game.nBlockDegrees):
                obj = Matrix(setOfBlockObjects[i][j])
                obj.mulc(i+1)
                self.setOfCBlockObjects[i][j] = obj
        return
    
    def __init__(self, game):
        self.game = game
        self.initCBlocks(game.setOfBlockObjects)
        arrayScreen = game.createArrayScreen()
        self.iCScreen = Matrix(arrayScreen)
        self.oCScreen = Matrix(self.iCScreen)
        return

    def accept(self, key):
        if key >= '0' and key <= '6':
            if self.game.justStarted == False:
                self.deleteFullLines()
            self.iCScreen = Matrix(self.oCScreen)
        
        state = self.game.accept(key)
        self.currCBlk = self.setOfCBlockObjects[self.game.idxBlockType][self.game.idxBlockDegree]
        self.tempBlk = self.iCScreen.clip(self.game.top, self.game.left, 
                                     self.game.top + self.currCBlk.get_dy(), 
                                     self.game.left + self.currCBlk.get_dx())
        self.tempBlk = self.tempBlk + self.currCBlk
        
        self.oCScreen = Matrix(self.iCScreen)
        self.oCScreen.paste(self.tempBlk, self.game.top, self.game.left)
        return state

    def getScreen(self):
        return self.oCScreen
    
    def deleteFullLines(self):
        nDeleted = 0
        nScanned = self.currCBlk.get_dy()

        if self.game.top + self.currCBlk.get_dy() - 1 >= self.game.iScreenDy:
            nScanned -= (self.game.top + self.currCBlk.get_dy() - self.game.iScreenDy)

        zero = Matrix([[ 0 for x in range(0, (self.game.iScreenDx - 2*self.game.iScreenDw))]])
        for y in range(nScanned - 1, -1, -1):
            cy = self.game.top + y + nDeleted
            line = self.oCScreen.clip(cy, 0, cy+1, self.oCScreen.get_dx()).binary()
            if line.sum() == self.oCScreen.get_dx():
                temp = self.oCScreen.clip(0, 0, cy, self.oCScreen.get_dx())
                self.oCScreen.paste(temp, 1, 0)
                self.oCScreen.paste(zero, 0, self.game.iScreenDw)
                nDeleted += 1

        return nScanned
    
    

