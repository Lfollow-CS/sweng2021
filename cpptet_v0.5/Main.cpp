#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include "CTetris.h"

using namespace std;


/**************************************************************/
/**************** Linux System Functions **********************/
/**************************************************************/

char saved_key = 0;
int tty_cbreak(int fd);	/* put terminal into cbreak mode */
int tty_reset(int fd);	/* restore terminal's mode */
  
/* Read 1 character - echo defines echo mode */
char getch() {
  char ch;
  int n;
  while (1) {
    tty_cbreak(0);
    n = read(0, &ch, 1);
    tty_reset(0);
    if (n > 0)
      break;
    else if (n < 0) {
      if (errno == EINTR) {
	if (saved_key != 0) {
	  ch = saved_key;
	  saved_key = 0;
	  break;
	}
      }
    }
  }
  return ch;
}

void sigint_handler(int signo) {
  // cout << "SIGINT received!" << endl;
  // do nothing;
}

void sigalrm_handler(int signo) {
  alarm(1);
  saved_key = 's';
}

void unregisterAlarm() {
	alarm(0);
}

void registerAlarm() {
  struct sigaction act, oact;
  act.sa_handler = sigalrm_handler;
  sigemptyset(&act.sa_mask);
#ifdef SA_INTERRUPT
  act.sa_flags = SA_INTERRUPT;
#else
  act.sa_flags = 0;
#endif
  if (sigaction(SIGALRM, &act, &oact) < 0) {
    cerr << "sigaction error" << endl;
    exit(1);
  }
  alarm(1);
}

/**************************************************************/
/**************** Tetris Blocks Definitions *******************/
/**************************************************************/
#define MAX_BLK_TYPES 7
#define MAX_BLK_DEGREES 4

int**** setOfCBlockArrays;

void drawScreen(CTetris *board)
{
  int dy = board->oCScreen.get_dy();
  int dx = board->oCScreen.get_dx();
  int dw = board->iScreenDw;
  int **array = board->oCScreen.get_array();
  system("clear");
  for (int y = 0; y < dy - dw + 1; y++) {
    for (int x = dw - 1; x < dx - dw + 1; x++) {
      if (array[y][x] == 0)
        cout << "\033[37m" << "□" << "\033[0m";
      else if (array[y][x] == 1 && y != dy - dw && x != dw - 1 && x != dx- dw)
        cout << "\033[31m" << "■" << "\033[0m";
      else if (array[y][x] == 2)
        cout << "\033[32m" << "■" << "\033[0m";
      else if (array[y][x] == 3)
        cout << "\033[33m" << "■" << "\033[0m";
      else if (array[y][x] == 4)
        cout << "\033[34m" << "■" << "\033[0m";
      else if (array[y][x] == 5)
        cout << "\033[35m" << "■" << "\033[0m";
      else if (array[y][x] == 6)
        cout << "\033[36m" << "■" << "\033[0m";
      else if (array[y][x] == 7)
        cout << "\033[95m" << "■" << "\033[0m";
      else 
        cout << "\033[37m" << "■" << "\033[0m";
    }
    cout << endl;
  }
}

int main(int argc, char* argv[]) {
  int dy, dx;
  char key = 0;
  if (argc != 3) {
    cout << "usage: " << argv[0] << " dy dx" << endl;
    exit(1);
  }
  if ((dy = atoi(argv[1])) <= 0 || (dx = atoi(argv[2])) <= 0) {
    cout << "dy and dx should be greater than 0" << endl;
    exit(1);
  }
#if 1
  CTetris::init(setOfCBlockArrays, MAX_BLK_TYPES, MAX_BLK_DEGREES);
  CTetris *board = new CTetris(dy, dx);
  TetrisState state;

  srand((unsigned int)time(NULL));
  key = (char)('0' + rand() % MAX_BLK_TYPES);
 #endif

  registerAlarm();
  while (key != 'q') {
#if 1
    state = board->accept(key);
    if (state == NewBlock) {
      key = (char)('0' + rand() % MAX_BLK_TYPES);
      state = board->accept(key);
      if (state == Finished) {
        drawScreen(board);
        cout << endl;
        break;
      }
    }
    drawScreen(board);
    cout << endl;
#endif
    key = getch();
    cout << key << endl;
  }


#if 1
  delete board;
#endif
  

  cout << "Program terminated!" << endl;
  return 0;
}

