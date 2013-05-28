#include <unistd.h>
#include <iostream> 
#include <iomanip> 
using namespace std;

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <termios.h>
#include <fcntl.h>

#include "apc100.h"

int main() 
{
   Apc100 *apc = new Apc100();


  char choice;
  char command;
 
  cout << "(A)bort\n(D)ownload Position\nDownload (S)etup\n (M)ove\n(Q)uery\n(R)eset\n";
  cin >> command;
  //cin.flush();
  cin.ignore(5,'\n');

  switch(command) //The first character specifies the command. Cool innit?
  {
  case 'Q':
   {
     apc->StatusErrorRequest();
     apc->QueryAngleDegree();
     apc->QueryAngleResolver();
   }
   break;
  case 'A':
     apc->Write("AB");
     break;
  case 'R':
     apc->Write("RT");
     break;
 
  case 'M':
   {
     cout << "\n\n(A)zimuth, (E)levation, (P)olarization: ";
     cin >> command; //cin.flush();
     cin.ignore(5,'\n');
     cout << "\n\n(d)egrees or resolver units?";
     cin >> choice; //cin.flush();
     cin.ignore(5,'\n');

     if (choice == 'd') apc->Move(command, true); //using degrees
     else apc->Move(command, false); //using resolver units
   }
   break;

  default:
    return (1);
  }
  return 0;
}
