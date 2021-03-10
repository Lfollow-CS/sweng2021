from matrix import *
import random

def draw_matrix(m):
    array = m.get_array()
    for y in range(m.get_dy()):
        for x in range(m.get_dx()):
            if array[y][x] == 0:
                print("□", end='')
            elif array[y][x] == 1:
                print("■", end='')
            else:
                print("XX", end='')
        print()
    
def rotate_right(m): 
    array = m.get_array()
    N = len(array) 
    ret = [[0] * N for _ in range(N)] 
    for r in range(N): 
        for c in range(N): 
            ret[c][N-1-r] = array[r][c] 
    return ret

def rotate_left(m): 
    array = m.get_array()
    N = len(array) 
    ret = [[0] * N for _ in range(N)] 
    for r in range(N): 
        for c in range(N): 
            ret[N-1-c][r] = array[r][c] 
    return ret

###
### initialize variables
###     
I = [ [ 0, 0, 1, 0 ], [ 0, 0, 1, 0 ], [ 0, 0, 1, 0 ], [ 0, 0, 1, 0 ] ]
J = [ [ 0, 1, 0 ], [ 0, 1, 0 ], [ 1, 1, 0] ]
L = [ [ 0, 1, 0 ], [ 0, 1, 0 ], [ 0, 1, 1,] ]
O = [ [ 1, 1 ], [ 1, 1 ] ]
S = [ [ 0, 1, 1 ], [ 1, 1, 0 ], [ 0, 0, 0 ] ]
T = [ [ 1, 1, 1 ], [ 0, 1, 0 ], [ 0, 0, 0 ] ]
Z = [ [ 1, 1, 0 ], [ 0, 1, 1 ], [ 0, 0, 0 ] ]
### integer variables: must always be integer!
iScreenDy = 15
iScreenDx = 10
iScreenDw = 4
top = 0
left = iScreenDw + iScreenDx//2 - 2

newBlockNeeded = False

arrayScreen = [
    [ 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 ],
    [ 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 ],
    [ 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 ],
    [ 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 ],
    [ 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 ],
    [ 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 ],
    [ 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 ],
    [ 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 ],
    [ 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 ],
    [ 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 ],
    [ 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 ],
    [ 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 ],
    [ 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 ],
    [ 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 ],
    [ 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 ],
    [ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ],
    [ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ],
    [ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ],
    [ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ] ]

###
### prepare the initial screen output
###  
randNum = random.randrange(0,6)
if randNum == 0:
    arrayBlk = I
elif randNum == 1:
    arrayBlk = J
    left += 1
elif randNum == 2:
    arrayBlk = L
elif randNum == 3:
    arrayBlk = O
    left += 1
elif randNum == 4:
    arrayBlk = S
elif randNum == 5:
    arrayBlk = T
elif randNum == 6:
    arrayBlk = Z
currBlk = Matrix(arrayBlk)
iScreen = Matrix(arrayScreen)
oScreen = Matrix(iScreen)
tempBlk = iScreen.clip(top, left, top+currBlk.get_dy(), left+currBlk.get_dx())
tempBlk = tempBlk + currBlk
oScreen.paste(tempBlk, top, left)
draw_matrix(oScreen); print()

###
### execute the loop
###

while True:
    key = input('Enter a key from [ q (quit), a (left), d (right), s (down), w (rotate), \' \' (drop) ] : ')
    if key == 'q':
        print('Game terminated...')
        break
    elif key == 'a': # move left
        left -= 1
    elif key == 'd': # move right
        left += 1
    elif key == 's': # move down
        top += 1
    elif key == 'w': # rotate the block clockwise
        currBlk = Matrix(rotate_right(currBlk))
    elif key == ' ': # drop the block
        pass
    else:
        print('Wrong key!!!')
        continue

    tempBlk = iScreen.clip(top, left, top+currBlk.get_dy(), left+currBlk.get_dx())
    tempBlk = tempBlk + currBlk
    
    if key == ' ': # drop the block
        while True:
            top += 1
            tempBlk = iScreen.clip(top, left, top+currBlk.get_dy(), left+currBlk.get_dx())
            tempBlk = tempBlk + currBlk
            if tempBlk.anyGreaterThan(1):
                top -= 1
                tempBlk = iScreen.clip(top, left, top+currBlk.get_dy(), left+currBlk.get_dx())
                tempBlk = tempBlk + currBlk
                newBlockNeeded = True
                break
                
    if tempBlk.anyGreaterThan(1):
        if key == 'a': # undo: move right
            left += 1
        elif key == 'd': # undo: move left
            left -= 1
        elif key == 's': # undo: move up
            top -= 1
            newBlockNeeded = True
        elif key == 'w': # undo: rotate the block counter-clockwise
            currBlk = Matrix(rotate_left(currBlk))

        tempBlk = iScreen.clip(top, left, top+currBlk.get_dy(), left+currBlk.get_dx())
        tempBlk = tempBlk + currBlk

    oScreen = Matrix(iScreen)
    oScreen.paste(tempBlk, top, left)
    draw_matrix(oScreen); print()

    if newBlockNeeded:
        iScreen = Matrix(oScreen)
        top = 0
        left = iScreenDw + iScreenDx//2 - 2
        newBlockNeeded = False
        randNum = random.randrange(0,6)
        if randNum == 0:
            arrayBlk = I
        elif randNum == 1:
            arrayBlk = J
            left += 1
        elif randNum == 2:
            arrayBlk = L
        elif randNum == 3:
            arrayBlk = O
            left += 1
        elif randNum == 4:
            arrayBlk = S
        elif randNum == 5:
            arrayBlk = T
        elif randNum == 6:
            arrayBlk = Z
        currBlk = Matrix(arrayBlk)
        tempBlk = iScreen.clip(top, left, top+currBlk.get_dy(), left+currBlk.get_dx())
        tempBlk = tempBlk + currBlk
        if tempBlk.anyGreaterThan(1):
            print('Game Over!!!')
            break
        
        oScreen = Matrix(iScreen)
        oScreen.paste(tempBlk, top, left)
        draw_matrix(oScreen); print()
        
###
### end of the loop
###
