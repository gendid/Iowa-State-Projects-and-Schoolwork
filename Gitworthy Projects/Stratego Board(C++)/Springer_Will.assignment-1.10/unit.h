#ifndef UNIT_H
#define UNIT_H 
#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;

#include "board.h"

class board;

class unit;

class unit
{
 private:
  int value;
  std::string name;
  int player;
  char symbol;
  bool isScout;
  bool isMiner;
  bool isSpy;
  bool isMarshall;
  bool isMine;
  bool isFlag;
 public:
 unit(): value(), name("default"), player(), symbol('#'), isScout(false),
    isMiner(false), isSpy(false), isMarshall(false),
    isMine(false), isFlag(false) {}
  int returnValue(){return this->value;};
  std::string returnName(){return this->name;};
  int returnPlayer(){return this->player;};
  char returnSymbol(){return this->symbol;};
  bool returnScout(){return this->isScout;};
  bool returnMiner(){return this->isMiner;};
  bool returnSpy(){return this->isSpy;};
  bool returnMarshall(){return this->isMarshall;};
  bool returnMine(){return this->isMine;};
  bool returnFlag(){return this->isFlag;};
  void setValue(int value){this->value=value;};
  void setName(std::string name){this->name=name;};
  void setPlayer(int player){this->player=player;};
  void setSymbol(char symbol){this->symbol=symbol;};
  void setScout(bool scout){this->isScout=scout;};
  void setMiner(bool miner){this->isMiner=miner;};
  void setSpy(bool spy){this->isSpy=spy;};
  void setMarshall(bool marshall){this->isMarshall=marshall;};
  void setMine(bool mine){this->isMine=mine;};
  void setFlag(bool flag){this->isFlag=flag;};
};

unit* createMine(int player);
unit* createMarshall(int player);
unit* createGeneral(int player);
unit* createColonol(int player);
unit* createMajor(int player);
unit* createCaptain(int player);
unit* createLieutenant(int player);
unit* createSergent(int player);
unit* createMiner(int player);
unit* createScout(int player);
unit* createSpy(int player);
unit* createFlag(int player);
void startGameUnits(board* b);

#endif
