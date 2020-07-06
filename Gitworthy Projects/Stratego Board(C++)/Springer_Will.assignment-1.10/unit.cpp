#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;

#include "board.h"
#include "unit.h"

unit* createMine(int player)
{
  unit* u= new unit();
  u->setValue(0);
  u->setName("Mine");
  u->setPlayer(player);
  u->setMine(true);
  u->setSymbol('B');

  return u;
}

unit* createMarshall(int player)
{
  unit* u= new unit();
  u->setValue(10);
  u->setName("Marshall");
  u->setPlayer(player);
  u->setMarshall(true);
  u->setSymbol('M');

  return u;
}

unit* createGeneral(int player)
{
  unit* u= new unit();
  u->setValue(9);
  u->setName("General");
  u->setPlayer(player);
  u->setSymbol('9');

  return u;
}

unit* createColonol(int player)
{
  unit* u= new unit();
  u->setValue(8);
  u->setName("Colonol");
  u->setPlayer(player);
  u->setSymbol('8');

  return u;
}

unit* createMajor(int player)
{
  unit* u= new unit();
  u->setValue(7);
  u->setName("Major");
  u->setPlayer(player);
  u->setSymbol('7');

  return u;
}

unit* createCaptain(int player)
{
  unit* u= new unit();
  u->setValue(6);
  u->setName("Captain");
  u->setPlayer(player);
  u->setSymbol('6');

  return u;
}

unit* createLieutenant(int player)
{
  unit* u= new unit();
  u->setValue(5);
  u->setName("Lieutenant");
  u->setPlayer(player);
  u->setSymbol('5');

  return u;
}

unit* createSergent(int player)
{
  unit* u= new unit();
  u->setValue(4);
  u->setName("Sergent");
  u->setPlayer(player);
  u->setSymbol('4');

  return u;
}

unit* createMiner(int player)
{
  unit* u= new unit();
  u->setValue(3);
  u->setName("Miner");
  u->setPlayer(player);
  u->setMiner(true);
  u->setSymbol('3');

  return u;
}

unit* createScout(int player)
{
  unit* u= new unit();
  u->setValue(2);
  u->setName("Scout");
  u->setPlayer(player);
  u->setScout(true);
  u->setSymbol('2');

  return u;
}

unit* createSpy(int player)
{
  unit* u= new unit();
  u->setValue(1);
  u->setName("Spy");
  u->setPlayer(player);
  u->setSpy(true);
  u->setSymbol('1');

  return u;
}

unit* createFlag(int player)
{
  unit* u= new unit();
  u->setValue(0);
  u->setName("Flag");
  u->setPlayer(player);
  u->setFlag(true);
  u->setSymbol('F');

  return u;

}

void startGameUnits(board* b)
{
  int i;
  b->player1pieces.push_back(createMarshall(1));
  b->player1pieces.push_back(createGeneral(1));
  b->player2pieces.push_back(createMarshall(2));
  b->player2pieces.push_back(createGeneral(2));
  for(i=0; i<2;i++)
    {
  b->player1pieces.push_back(createColonol(1));
  b->player2pieces.push_back(createColonol(2));
    }
  for(i=0; i<3;i++)
    {
  b->player1pieces.push_back(createMajor(1));
  b->player2pieces.push_back(createMajor(2));
    }
  for(i=0; i<4;i++)
    {
  b->player1pieces.push_back(createCaptain(1));
  b->player2pieces.push_back(createCaptain(2));
  b->player1pieces.push_back(createLieutenant(1));
  b->player2pieces.push_back(createLieutenant(2));
  b->player1pieces.push_back(createSergent(1));
  b->player2pieces.push_back(createSergent(2));
    }
  for(i=0; i<5;i++)
    {
  b->player1pieces.push_back(createMiner(1));
  b->player2pieces.push_back(createMiner(2));
    }
  for(i=0; i<8;i++)
    {
  b->player1pieces.push_back(createScout(1));
  b->player2pieces.push_back(createScout(2));
    }

  b->player1pieces.push_back(createSpy(1));
  b->player2pieces.push_back(createSpy(2));

  for(i=0; i<6;i++)
    {
  b->player1pieces.push_back(createMine(1));
  b->player2pieces.push_back(createMine(2));

    }

  b->player1pieces.push_back(createFlag(1));
  b->player2pieces.push_back(createFlag(2));
   
}
