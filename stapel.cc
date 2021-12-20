#include "koffiebord.h"
#include "stapel.h"
#include <iostream>
using namespace std;


stapel::~stapel ( ) {
  // int getal;
  // haalvanstapel();
}//stapel::~stapel

void stapel::zetopstapel(koffiebord* player_bord){
  // stapelvakje* new_stap = new stapelvakje();
  // koffiebord* new_bord = new koffiebord(*player_bord);
  
  // new_bord -> ingang = player_bord -> copybord();
  
  // new_stap -> stapel_bord_ing = new_bord;
  
  // new_stap -> prev = last_in;  
  
  // last_in = new_stap;

  stapelvakje* new_stap = new stapelvakje();
  koffiebord* new_bord = new koffiebord(*player_bord);
  
  new_bord -> ingang = player_bord -> copybord();
  new_stap -> stapel_bord_ing = new_bord;
  
  new_stap -> prev = last_in;  
  
  last_in = new_stap;

}//stapel::zetopstapel

void stapel::haalvanstapel(koffiebord* & player_bord){
  // stapelvakje* laatste_vakje = last_in;
  // cout << "Haal van stapel:" << endl;
  // cout << player_bord -> ingang << endl;
  // if (!last_in){
  //   cout << "Stapel is leeg" << endl;
  // } else {
  //   player_bord -> ~koffiebord();
  //   last_in = laatste_vakje -> prev;
  //   // player_bord = laatste_vakje -> stapel_bord_ing;
  //   // cout << player_bord -> ingang << endl;
  //   player_bord = last_in -> stapel_bord_ing;
 
  //   delete laatste_vakje;
  // }
  // stapelvakje* oud_vakje = last_in;

  // cout << "Haal van stapel:" << endl;
  // cout << oud_vakje << endl;
  // player_bord -> ~koffiebord();
  
  // last_in = oud_vakje -> prev;

  // player_bord = oud_vakje -> stapel_bord_ing;
  // delete oud_vakje;
  // cout << player_bord -> ingang << endl;

  stapelvakje* oud_vakje = last_in;

  cout << "Haal van stapel:" << endl;
  cout << oud_vakje << endl;
  player_bord -> ~koffiebord();
  
  last_in = oud_vakje;

  player_bord = oud_vakje -> stapel_bord_ing;
  delete oud_vakje;
  cout << player_bord -> ingang << endl;

  // return oude_bord;
}//stapel::haalvanstapel