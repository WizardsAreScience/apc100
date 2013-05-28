#include <termios.h>
#include <fcntl.h>
#include <stdlib.h>

#include <iostream>
#include <cstdio>
using namespace std;

#include "apc100.h"

Apc100::Apc100()
{
  fD = open("/dev/ttyUSB0",O_RDWR|O_NOCTTY);
  if(fD<0) {
    cout << "Could not open port for APC100" << endl;
    exit(1);
  }

  struct termios term;

  term.c_cflag = B9600 | CS8 | CREAD | PARENB;
  term.c_iflag = IGNPAR | ICRNL;
  term.c_oflag = VT1;
  term.c_lflag = 0;
  term.c_cc[VEOL] = '\0';

  tcflush(fD, TCIOFLUSH);
  tcsetattr(fD,TCSANOW,&term);

  char mes[6];
  mes[0] = 0x01;
  mes[1] = 0x50;
  mes[2] = 'S';
  mes[3] = 'E';
  mes[4] = 0x03;
  mes[5] = Cheksum(6,mes);//0x00 - (mes[0]+mes[1]+mes[2]+mes[3]+mes[4]);
  
  int r = write(fD, mes, 6);
  
  char buf[255];
  for(int i=0;i<7;i++) {
    int res = read(fD, buf,1);
    //cout << "-> " << "0x"  << hex << int(buf[0]) << endl;
  }
}

bool Apc100::Write(char *twobits) //miscellanious write function
{
  if(Cheksum(2, twobits) > 2) return false; //not two bits

  char mes[6];
  mes[0] = 0x01;
  mes[1] = 0x50;
  mes[2] = twobits[0];
  mes[3] = twobits[1];
  mes[4] = 0x03;
  mes[5] = Cheksum(6,mes);//0x00 - (mes[0]+mes[1]+mes[2]+mes[3]+mes[4]);
 

  int r = write(fD, mes, 6);
  
  char buf[255];
  for(int i=0;i<5;i++) {
    int res = read(fD, buf,1);}
  return true;
}

bool Apc100::Move(char lbit, bool degrees)
{
  float num = 0;
  char buff[5];
  char mes[11];
  mes[0] = 0x01;
  mes[1] = 0x50;
  mes[2] = 'M';
  mes[3] = lbit;

  cout << "Insert new coordinates: ";
  cin >> num; //cin.flush();
   cin.ignore(5,'\n');

  if(degrees == true) //we need to accept degrees and convert
    {
      num /= .00549; //.00549 deg to resolver
    }
  if((num > 65535) || (num < 0)) return false; //failed
 
  //*buff = ftoa(num); //buffers of course
  sprintf(buff, "%05.0f", num);
  cout << "BUF: " << buff << " " << num << endl;
  for(int i = 0; i < 5; i++) mes[i+4] = buff[i]; //writes to mes
 
  mes[9] = 0x03;
  mes[10] = Cheksum(11, mes);

  for(int i = 0; i < 11; i++) 
    cout << mes[i] << "   --  0x" << hex  << int(mes[i]) << endl;

  int r = write(fD, mes, 11);

  cout << "\n\n";

  char buf[255];
  for(int i=0;i<5;i++) {
    int res = read(fD, buf,1);
    
    cout << "-> " << "0x"  << hex << int(buf[0]) << endl;
  }
  cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;

  //  cout.read(fD, 4);
  return true;
}

bool Apc100::Move(char lbit, float num) //really just a repeat of the previous one, except that it takes diff parameters.
{
  char buff[5];
  char mes[11];
  mes[0] = 0x01;
  mes[1] = 0x50;
  mes[2] = 'M';
  mes[3] = lbit;

  num  /= .00549; //.00549 deg to resolver
    
  if((num > 65535) || (num < 0)) return false; //failed
 
  //*buff = ftoa(num); //buffers of course
  sprintf(buff, "%05.0f", num);
  cout << "BUF: " << buff << " " << num << endl;
  for(int i = 0; i < 5; i++) mes[i+4] = buff[i]; //writes to mes
 
  mes[9] = 0x03;
  mes[10] = Cheksum(11, mes);

  for(int i = 0; i < 11; i++) 
    cout << mes[i] << "   --  0x" << hex  << int(mes[i]) << endl;

  int r = write(fD, mes, 11);

  cout << "\n\n";

  char buf[255];
  for(int i=0;i<5;i++) {
    int res = read(fD, buf,1);
    
    cout << "-> " << "0x"  << hex << int(buf[0]) << endl;
  }
  cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;

  //  cout.read(fD, 4);
  return true;
}

bool Apc100::StatusErrorRequest()
{
  cout << ">>>>> STATUS ERROR REQUEST " << endl;

  char mes[6];
  mes[0] = 0x01;
  mes[1] = 0x50;
  mes[2] = 'S';
  mes[3] = 'E';
  mes[4] = 0x03;
  mes[5] = Cheksum(6,mes);//0x00 - (mes[0]+mes[1]+mes[2]+mes[3]+mes[4]);
  
  int r = write(fD, mes, 6);
  
  char buf[255];
  for(int i=0;i<7;i++) {
    int res = read(fD, buf,1);
    
    cout << "-> " << "0x"  << hex << int(buf[0]) << endl;
  }
  cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;
  return true;

}
bool Apc100::QueryAngleResolver()
{
  cout << ">>>>> ANGLE (RESOLV) REQUEST " << endl;
  char mes[6];
  mes[0] = 0x01;
  mes[1] = 0x50;
  mes[2] = 'Q';
  mes[3] = 'R';
  mes[4] = 0x03;
  mes[5] = Cheksum(6,mes);//0x00 - (mes[0]+mes[1]+mes[2]+mes[3]+mes[4]);
  
  int r = write(fD, mes, 6);

  char buf[1];
  for(int i=0;i<3;i++) {
    int res = read(fD, &buf,1);
    cout << "-> " << "0x"  << hex << int(buf[0]) << endl;
  }

  for(int i=3;i<23;i++) {
    int res = read(fD, &buf,1);
    cout << buf[0];
  }
  cout << endl;

  for(int i=23;i<25;i++) {
    int res = read(fD, &buf,1);
    cout << "-> " << "0x"  << hex << int(buf[0]) << " " << buf[0] << endl;
  }
  cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;

  return true;  
 
}


bool Apc100::QueryAngleDegree()
{
  cout << ">>>>> ANGLE (DEGREE) REQUEST " << endl;
  char mes[6];
  mes[0] = 0x01;
  mes[1] = 0x50;
  mes[2] = 'Q';
  mes[3] = 'A';
  mes[4] = 0x03;
  mes[5] = Cheksum(6,mes);//0x00 - (mes[0]+mes[1]+mes[2]+mes[3]+mes[4]);
  
  int r = write(fD, mes, 6);
  
  char buf[1];
  for(int i=0;i<3;i++) {
    int res = read(fD, &buf,1);
    cout << "-> " << "0x"  << hex << int(buf[0]) << endl;
  }

  
  for(int i=3;i<25;i++) {
    int res = read(fD, &buf,1);
    cout << buf[0];
  }
  cout << endl;

  for(int i=25;i<28;i++) {
    int res = read(fD, &buf,1);
    cout << "-> " << "0x"  << hex << int(buf[0]) << endl;
  }
  cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;

  return true;

}

char Apc100::Cheksum(int n, char* m)
{
  int mm=0;
  for(int i=0;i<(n-1); i++) {
    mm -= int(m[i]);
  }
  m[n-1] = char(mm);
  
  cout << hex << mm << " " << m[n-1] << endl;

  return(m[n-1]);  

  m[n-1] = 0;
  char tmp = 0x00;
  for(int i=0;i<(n-1); i++) {
    m[n-1] -= m[i];
  }
  return(m[n-1]);
  
}
