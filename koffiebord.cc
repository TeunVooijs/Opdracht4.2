#include <ctime>
#include <iostream>
#include <fstream>
#include "stdlib.h"
#include "koffiebord.h"
#include "stapel.h"
using namespace std;

int read_num(int aantal);


bordvakje::bordvakje ( ) {
  geopend = false;
  flag = false;
  marked = false;
  aantal_buren = 0;
  for (int i = 0; i < 8; i++){
    buren[i] = nullptr;
  }

}//bordvakje::bordvakje

koffiebord::koffiebord (int h, int b, int p) {
  srand(time(NULL));
  ingang = nullptr;
  nieuwe_rij = nullptr;
  vorige_rij = nullptr;
  hoogte = h; 
  breedte = b; 
  percentage = p;
  zetten = 0;
  ingang = maak_bord();
  koffies();
  spel_af = false;
}//koffiebord::koffiebord

koffiebord::~koffiebord ( ) {
  // bordvakje* col_fol = ingang;
  bordvakje* deleter;
  bordvakje* hulp_deleter;
  
  // while (ingang -> buren[4]){
  //   ingang = ingang -> buren[4];
  // }
  // while (ingang){
  //   hulp_deleter = col_fol;
  //   col_fol = col_fol -> buren[4];
  //   while (hulp_deleter){
  //     deleter = hulp_deleter;
  //     hulp_deleter = hulp_deleter -> buren[2];
  //     delete deleter;
  //   }
  // }
  while (ingang){
    hulp_deleter = ingang;
    ingang = ingang -> buren[4];
    while (hulp_deleter){
      deleter = hulp_deleter;
      hulp_deleter = hulp_deleter -> buren[2];
      delete deleter;
    }
  }
}//koffiebord::~koffiebord

void koffiebord::drukaf ( ) {
  bordvakje* left_col = ingang;
  bordvakje* printer = left_col;
  int row_num = 0;

  cout << "   ";
  for (int i = 0; i < breedte; i++){
    if (i < 10){
      cout << "  " << i;  
    } else {
      cout << " " << i;
    }
  }
  cout << endl;

  if (left_col){
    while (left_col){
      printer = left_col;
      if (row_num < 10){
        cout << "  " << row_num << "  ";
      } else {
        cout << " " << row_num << "  ";
      }
      while (printer){
        
        if (printer -> flag == true){
          cout << "F  ";
        } else if (printer -> geopend == false){
          cout << "#  ";
        } else if (printer -> geopend == true &&
         printer -> marked == false){
          if (printer -> aantal_buren == 0){
            cout << ".  ";
          } else {
            cout << printer -> aantal_buren << "  ";
          }
        } else if (printer -> geopend == true && printer -> marked == true){
          cout << "X  ";
        }
        printer = printer -> buren[2];
      }
      row_num++;
      cout << endl;        
      left_col = left_col -> buren[4];
    } 
  }
}//koffiebord::drukaf

void koffiebord::legenda(){
  cout << "Aantal zetten tot nu toe: "<< zetten << " Aantal koffies: "
  << aantal_koffies << endl;
}


bordvakje* koffiebord::maak_bord(){
  bordvakje* hulp_ingang;
  for (int i = 0; i < hoogte; i++){
    hulp_ingang = maakrij();
    ritsen();
  }
  return hulp_ingang;
}

bordvakje* koffiebord::copybord(){
  bordvakje* stapel_bord = maak_bord();
  bordvakje* stap_bord_ing = stapel_bord;
  bordvakje* left_col_stapel;
  bordvakje* stap_copier;
  bordvakje* left_col_main;
  bordvakje* main_copier;

  left_col_main = ingang;
  left_col_stapel = stap_bord_ing;
  while (left_col_main){
    main_copier = left_col_main;
    stap_copier = left_col_stapel;
    while (main_copier)
    {
      stap_copier -> aantal_buren = main_copier -> aantal_buren;
      stap_copier -> marked = main_copier -> marked;
      stap_copier -> geopend = main_copier -> geopend;
      stap_copier ->  flag = main_copier -> flag;

      main_copier = main_copier -> buren[2];
      stap_copier = stap_copier -> buren[2];
    }
    left_col_main = left_col_main -> buren[4];
    left_col_stapel = left_col_stapel -> buren[4];
  }
  return stap_bord_ing;
}

bordvakje* koffiebord::maakrij(){
  bordvakje* row = nullptr;
  bordvakje* hulp = nullptr;
  vorige_rij = nieuwe_rij;

  for (int i = 0; i < breedte; i++){
    row = new bordvakje;
    row -> buren[2] = hulp;
    if (row -> buren[2]){
      row -> buren[2] -> buren[6] = row;
    }  
    hulp = row;
  }
  
  nieuwe_rij = hulp;
  return nieuwe_rij;
}

void koffiebord::ritsen(){
  bordvakje* connecter = nieuwe_rij;

  while (vorige_rij != nullptr && connecter != nullptr){
    vorige_rij -> buren[0] = connecter;
    vorige_rij -> buren[1] = connecter -> buren[2];
    vorige_rij -> buren[7] = connecter -> buren[6];
    connecter -> buren[3] = vorige_rij -> buren[2];
    connecter -> buren[4] = vorige_rij;
    connecter -> buren[5] = vorige_rij -> buren[6];
    connecter = connecter -> buren[2];
    vorige_rij = vorige_rij -> buren[2];
  }
}

bordvakje* koffiebord::readcoord(int i, int j){
// als er een verschil is verplaatst het eerst het aantal stappen dat
// overlapt schuin 
// verschil=i-j
// wanneer verschil is >0 dan i>j en j is het aantal schuine stappen
// wanneer verschil is <0 dan j>i en i is het aantal schuine stappen
// maak een rest
// verplaats in richting bij i of j is groter  
  int verschil = i - j; // i is de x as, j de y as
  bordvakje* mover = ingang;

  if (verschil > 0){
    for (int g = 0; g < j; g++){
      mover = mover -> buren[3];
    }
    for (int h = 0; h < verschil; h++){
      mover = mover -> buren[2];
    }
  } else if (verschil < 0){
    for (int g = 0; g < i; g++){
      mover = mover -> buren[3];
    }
    for (int h = 0; h < (verschil * -1); h++){
      mover = mover -> buren[4];
    }
  } else if (verschil == 0){
    for (int  g = 0; g < i; g++){
      mover = mover -> buren[3];
    }
  }
  return mover;
}

void koffiebord::koffies(){
  // srand(time(0));
  bordvakje* ober = ingang;
  int x_pos,y_pos;
  int controle = 0;
  aantal_koffies = (breedte * hoogte * percentage) / 100;

  if (aantal_koffies == 0){
    aantal_koffies = 1;
  }
  while (aantal_koffies != controle){
    x_pos = rand() % breedte;
    y_pos = rand() % hoogte;
    ober = readcoord(x_pos,y_pos);
    // cout << x_pos << " " << y_pos << endl;
    if (ober -> marked == false){
      ober -> marked = true;

      for (int i = 0; i < 8; i++){
        if (ober -> buren[i]){
          ober -> buren[i] -> aantal_buren++;
        }
      }
      controle++;
    }
  }
}

void koffiebord::flood_fill(bordvakje* pos){
  pos -> geopend = true;

  for (int i = 0; i < 8; i++){
    if (pos -> buren[i] && pos -> buren[i] -> geopend == false){
      if (pos -> buren[i] -> aantal_buren == 0){
        flood_fill(pos -> buren[i]);
      } else if (pos -> buren[i] -> aantal_buren != 0){
        pos -> buren[i] -> geopend = true;
      }
    }
  }
}

void koffiebord::closed_counter(){
  bordvakje* left_col = ingang;
  bordvakje* counter;
  closed_cells = 0;
  
  while (left_col){
    counter  = left_col;
    while (counter){
      if (counter -> geopend == false && counter -> flag == false){
        closed_cells++;
      }
      counter = counter -> buren[2];
    }
    left_col = left_col -> buren[4];
  }
  cout << "klaar" << endl;
  cout << closed_cells << endl;
}



void koffiebord::player_zet(){
  bordvakje* pos;
  int x_zet;
  int y_zet;
  cout << "Geef de kolom: ";
  x_zet = read_num(2);
  cout << "Geef de rij: ";
  y_zet = read_num(2);
  pos = readcoord(x_zet,y_zet);
  doe_zet(pos);
}

void koffiebord::rand_zet(){
  closed_counter();
  bordvakje* left_col = ingang;
  bordvakje* pos_controle;
  int pos_count = 0;
  int controle = -1;

  pos_count = rand() % (closed_cells - 1);
  cout << pos_count << endl;
  while (controle != pos_count){
    pos_controle = left_col;

    while (pos_controle){
      if (pos_controle -> geopend == false && pos_controle -> flag == false){
        controle++;
      }
      if (controle == pos_count){
        break;  
      }
      pos_controle = pos_controle -> buren[2];
    }
    left_col = left_col -> buren[4];
  }
  cout << "olla" << endl;
  doe_zet(pos_controle);
}



void koffiebord::doe_zet(bordvakje* pos){
  if (zetten == 0 && pos -> marked == true){
    cout << "fuck me" << endl;
    koffie_zet();
    pos -> marked = false;
    for (int i = 0; i < 8; i++){
      if (pos -> buren[i]){
        pos -> buren[i] -> aantal_buren--;
      }
    }
    pos -> geopend = true;
  } else if (pos ->marked == true){
    zetten++;
    speler_af();
  } else if (pos -> flag == true){
    cout << "Deze positie is gemarkeerd!" << endl;
    zetten--;
  } else if (pos -> geopend == true){
    cout << "Deze positie is al open" << endl;
    zetten--;
  } else {
    pos -> geopend = true;
  }
  if (pos -> aantal_buren == 0){
    flood_fill(pos);
  }
  zetten++;
}

void koffiebord::koffie_zet(){
  bordvakje* left_col = ingang;
  bordvakje* ober;
  int koffie_plaats;
  int controle = 0;
  int rij_tel = 0;
  int controle_mark = 0;
  int aantal_vakjes = 0;
  koffie_plaats = rand() % ((breedte*hoogte) - aantal_koffies);
  if (koffie_plaats == 0){
    koffie_plaats = 1;
  }
  cout << koffie_plaats << endl;
  cout << aantal_koffies << endl;
  while (left_col /* && koffie_pos != controle*/){
    // cout << rij_tel << endl;
    ober = left_col;
    while (ober /* && koffie_pos != controle*/){
      if (ober -> marked == true){
        controle_mark++;
        // cout << controle_mark << " Marks" << endl;
      }
      

      if (ober -> marked == false){
        controle++;
        // cout << controle << "  " << koffie_plaats << endl;
      }
      // aantal_vakjes++;
      // cout << aantal_vakjes << endl;
      if (controle == koffie_plaats){
        break;
      }
      
      ober = ober -> buren[2];
    }
    if (controle == koffie_plaats){
      break;
    }
    rij_tel++;
    left_col = left_col -> buren[4];
    
  }
  
  for (int i = 0; i < 8; i++){
    if (ober -> buren[i]){
      ober -> buren[i] -> aantal_buren++;
    }
  }
  cout << "Test " << endl;
  ober -> marked = true;
  
  
}

void koffiebord::flag_zet(){
  bordvakje* flag_pos;
  int x_flag, y_flag;
  cout << "Geef kolom voor flag: ";
  x_flag = read_num(2);
  cout << "Geef rij voor flag: ";
  y_flag = read_num(2);

  flag_pos = readcoord(x_flag,y_flag);

  if (flag_pos -> geopend == true){
    cout << "Deze plek is al open" << endl;
  } else if (flag_pos -> flag == true){
    flag_pos -> flag = false;
  } else if (flag_pos -> flag == false){
    flag_pos -> flag = true;
  }
}

void koffiebord::speler_af(){
  bordvakje* left_col = ingang;
  bordvakje* opener;

  spel_af = true;
  
  while (left_col){
    opener = left_col;
    while (opener){
      if (opener -> flag == true)
        opener -> flag = false;
      opener -> geopend = true;
      opener = opener -> buren[2];
    }
    left_col = left_col -> buren[4];
  }
  cout << "Je hebt de koffie omgestoten :(" << endl <<
  "Volgende keer beter" << endl;
}

void koffiebord::comp(){
  int aantal_runs = 0;
  int a[100]{0} ,b[100]{0};
  cout << "Hoe vaak wil je het programma laten gaan?: ";
  aantal_runs = read_num(7);
  int test = 0;

  for (int i = 0; i < aantal_runs; i++){
    test++;
    while (comp_af == false && comp_win == false){
      comp_zet();
    }
    
    if (comp_af == true){
      a[comp_zetten]++;  
    } else if (comp_win == true){
      b[comp_zetten]++;
    }
    
    reset_bord();
    koffies();
    comp_zetten = 0;
    comp_win = false;
    comp_af = false;
  }

  for (int i = 1; i < 30; i++){
    cout << "Aantal zetten: " << i << "  Aantal keer: " << a[i] << 
    "     Aantal zetten: " << i <<  "  Aantal keer: " << b[i] << endl;
  }
}

void koffiebord::tester(){
  for (int i = 0; i < 10; i++){
    reset_bord();
  }
}

void koffiebord::comp_zet(){
  closed_counter();
  bordvakje* left_col = ingang;
  bordvakje* pos_controle;
  int pos_count = 0;
  int controle = -1;

  if (closed_cells == aantal_koffies){
    comp_win = true;
    return;
  }
  

  pos_count = rand() % (closed_cells - 1);
  // cout << pos_count << endl;
  while (controle != pos_count){
    pos_controle = left_col;

    while (pos_controle){
      if (pos_controle -> geopend == false && pos_controle -> flag == false){
        controle++;
        // cout << controle << endl;
      }
      if (controle == pos_count){
        break;  
      }
      pos_controle = pos_controle -> buren[2];
    }
    left_col = left_col -> buren[4];
  }
  // cout << "olla" << endl;
  comp_doe_zet(pos_controle);
}


void koffiebord::comp_doe_zet(bordvakje* pos){
  if (pos -> marked == true){
    if (comp_zetten == 0){
      // cout << "crash 0" << endl;
      comp_zetten--;
      koffie_zet();
      pos -> marked = false;
      for (int i = 0; i < 8; i++){
        if (pos -> buren[i]){
          pos -> buren[i] -> aantal_buren--;
        }
      }
      pos -> geopend = true;
    } else {
      // cout << "crash 1" << endl;
      comp_af = true;
    }
  } else if (pos -> geopend == true){
    // cout << "crash 3";
    comp_zet();
  } else {
    // cout << "crash 4" << endl;
    pos -> geopend = true;
  }
  
  if (pos -> aantal_buren == 0){
    // cout << "crash 2" << endl;
    flood_fill(pos);
  }
  comp_zetten++;

}



void koffiebord::reset_bord(){
  bordvakje* left_col = ingang;
  bordvakje* reseter;
  
  while (left_col){
    reseter = left_col;
    while (reseter){
      reseter -> aantal_buren = 0;
      reseter -> marked = false;
      reseter -> flag = false;
      reseter -> geopend = false;
      reseter -> aantal_buren = 0;
      reseter = reseter -> buren[2];
    }
    left_col = left_col -> buren[4];
  }
  // koffies();

}