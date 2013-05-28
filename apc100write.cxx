#include <unistd.h>
#include <iostream> 
using namespace std;

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <termios.h>
#include <fcntl.h>

#include "apc100.h"

int main(char opt, float deg)
{
  Apc100 *apc = new Apc100();
  if((opt != 'A') || (opt != 'E')) return 1; //Invalid move command 
  apc->Move(opt, deg);
  return 0;
}
