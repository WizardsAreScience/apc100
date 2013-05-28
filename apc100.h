#ifndef APC100_H

class Apc100 {
 public:
  Apc100();
  ~Apc100();
  
  bool StatusErrorRequest();
  bool QueryAngleDegree();
  bool QueryAngleResolver();
  bool Move(char, bool);
  bool Move(char, float);
  bool Write(char*);
 private:
  char Cheksum(int n, char* m);
 private:
  int fD;
};
#endif
