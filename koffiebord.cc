#include <ctime>
#include <iostream>
#include <fstream>
#include "stdlib.h"
#include "koffiebord.h"
#include "stapel.h"
using namespace std;

int read_num(int aantal);

bordvakje::bordvakje(){
// dit is de constructor van bordvakje
  geopend = false;
  flag = false;
  marked = false;
  aantal_buren = 0;
  for (int i = 0; i < 8; i++){
    buren[i] = nullptr;
  }
}

koffiebord::koffiebord(int h, int b, int p){
// dit is de contructor van koffiebord
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
}

koffiebord::~koffiebord(){
// dit is de destructor
  bordvakje* deleter;
  bordvakje* hulp_deleter;
  while (ingang){
    hulp_deleter = ingang;
    ingang = ingang -> buren[4];
    while (hulp_deleter){
      deleter = hulp_deleter;
      hulp_deleter = hulp_deleter -> buren[2];
      delete deleter;
    }
  }
}

void koffiebord::drukaf(){
// De drukaf functie print het bord
// er wordt gekeken naar of het een flag, koffie of getal is
// aan het eind wordt er ook gecontroleerd of de speler gewonnen heeft
  bordvakje* left_col = ingang; // volgt de linker kolom
  bordvakje* printer = left_col; // loopt langs de rijen
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
  cout << endl;
  controle_win();
}//koffiebord::drukaf

void koffiebord::legenda(){
// legenda print het aantal zetten die er zijn gezet en het aantal
// koffies die er zijn op het bord
  cout << "Aantal zetten tot nu toe: "<< zetten << " Aantal koffies: "
  << aantal_koffies << endl;
}


bordvakje* koffiebord::maak_bord(){
// de maak_bord functie maakt het bord
// dit wordt per rij gedaan
// die vervolgens aan elkaar geritst worden
  bordvakje* hulp_ingang;
  for (int i = 0; i < hoogte; i++){
    hulp_ingang = maakrij(); // maakt de rij
    ritsen(); // ritst beide rijen aan elkaar
  }
  return hulp_ingang; // Hier wordt de ingang gereturned
}

bordvakje* koffiebord::maakrij(){
// Maakrij maakt de rijen vakje voor vakje
  bordvakje* row = nullptr;
  bordvakje* hulp = nullptr;
  vorige_rij = nieuwe_rij;

  for (int i = 0; i < breedte; i++){
    row = new bordvakje; // nieuw vakje
    row -> buren[2] = hulp; // vakje verbindt met vorige vakje
    if (row -> buren[2]){ // als er een volgend vakje is wordt deze
      row -> buren[2] -> buren[6] = row; // ook weer terug verbonden
    }  // zodat er een heen en terug verbinding ontstaat
    hulp = row;
  }
  nieuwe_rij = hulp;
  return nieuwe_rij; // de ingang wordt weer gereturned
}

void koffiebord::ritsen(){
// ritsen zorgt ervoor dat de nieuwe en vorige rij verbonden worden
// mbv pointers
  bordvakje* connecter = nieuwe_rij;

  while (vorige_rij != nullptr && connecter != nullptr){
    // vorige rijen
    vorige_rij -> buren[0] = connecter; 
    // vakje wordt direct naar boven verbonden
    vorige_rij -> buren[1] = connecter -> buren[2];
    // vakje wordt rechts naar boven verbonden
    vorige_rij -> buren[7] = connecter -> buren[6];
    // vakje wordt links naar boven verbonden
    
    // nieuwe rij
    connecter -> buren[3] = vorige_rij -> buren[2];
    // vakje wordt rechts naar beneden verbonden
    connecter -> buren[4] = vorige_rij;
    // vakje wordt naar beneden verbonden
    connecter -> buren[5] = vorige_rij -> buren[6];
    // vakje wordt links naar beneden verbonden
    
    // beide hulp variabelen worden 1 naar rechts geschoven
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

  if (verschil > 0){ // eerst schuin en dan de overgebleven stappen
    for (int g = 0; g < j; g++){ // naar rechts
      mover = mover -> buren[3];
    }
    for (int h = 0; h < verschil; h++){
      mover = mover -> buren[2];
    }
  } else if (verschil < 0){ // eerst schuin en dan de overgebleven stappen
    for (int g = 0; g < i; g++){ // naar beneden
      mover = mover -> buren[3];
    }
    for (int h = 0; h < (verschil * -1); h++){
      mover = mover -> buren[4];
    }
  } else if (verschil == 0){ // wanneer i en j gelijk zijn en je alleen
    for (int  g = 0; g < i; g++){ // schuin hoeft te bewegen
      mover = mover -> buren[3];
    }
  }
  return mover; // Geeft de adres terug
}

void koffiebord::koffies(){
// De funtie koffies plaatst de koffies
// Eerst neemt hij de hoogte breedte en het percentage, om het aantal
// koffies te bereken. Vervolgens is er een loop die zolang het aantal
// koffies niet gelijk is aan de controle 
// er nieuwe koffies worden geplaatst
  bordvakje* ober = ingang;
  int x_pos,y_pos;
  int controle = 0;
  aantal_koffies = (breedte * hoogte * percentage) / 100;

  if (aantal_koffies == 0){ // zodat er iig 1 koffie is
    aantal_koffies = 1;
  }

  // zolang het aantal koffies niet gelijk is aan de controle
  // blijven er nieuwe posities gegenereerd voor de koffies
  while (aantal_koffies != controle){  
    x_pos = rand() % breedte; // random x en y voor de koffies worden
    y_pos = rand() % hoogte; // hier gemaakt
    ober = readcoord(x_pos,y_pos);
    
    if (ober -> marked == false){ // als er op de random gemaakte pos.
      ober -> marked = true; // geen koffie is, wordt het neergezet
      for (int i = 0; i < 8; i++){ // en worden hier alle buren +1 gemaakt
        if (ober -> buren[i]){
          ober -> buren[i] -> aantal_buren++;
        }
      }
      controle++; // dit is voor het aantal koffies te controleren
    }
  }
}

void koffiebord::flood_fill(bordvakje* pos){
// flood_fill opent alle aanliggende vakjes 
// wanneer een 0 vakje wordt geopend
  pos -> geopend = true;

  for (int i = 0; i < 8; i++){
    if (pos -> buren[i] && pos -> buren[i] -> geopend == false){
      if (pos -> buren[i] -> aantal_buren == 0){
        flood_fill(pos -> buren[i]); 
        // als het geopend vakje 0 is wordt de positie in de functie
        // terug gezet, zodat het door kan gaan tot alle aanliggende
        // nullen open zijn gemaakt
      } else if (pos -> buren[i] -> aantal_buren != 0){
        pos -> buren[i] -> geopend = true; 
        // wanneer het niet een 0 is word het vakje alleen geopend
      }
    }
  }
}

void koffiebord::closed_counter(){
// deze functie telt alle niet geopende vakjes
// hij loopt door het bord en doet +1 wanneer het een gesloten 
// of geflagged vakje tegenkomt
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
}

void koffiebord::player_zet(){
// vraagt de coordinaten op en geeft ze door aan de readcoord functie
// die de positie teruggeeft
// die positie word vervolgens aan de doe_zet functie gegeven
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
// Deze funtie genereerd een getal tussen de 0 
// en het aantal gesloten vakjes
// en laat 2 pointers lopen totdat de controle gelijk is aan dit getal
  closed_counter();
  bordvakje* left_col = ingang;
  bordvakje* pos_controle;
  int pos_count = 0;
  int controle = -1;
  pos_count = rand() % (closed_cells - 1);
  // blijft lopen tot de controle gelijk is aan de positie int
  while (controle != pos_count){
    pos_controle = left_col;

    while (pos_controle){
      if (pos_controle -> geopend == false && pos_controle -> flag == false){
        controle++; // doet plus 1 wanneer het vakje dicht is
      } // en er geen flag op staat
      if (controle == pos_count){
        break;  
      }
      pos_controle = pos_controle -> buren[2];
    }
    left_col = left_col -> buren[4];
  }
  doe_zet(pos_controle); // geeft het adres door aan de doe_zet funtie
}

void koffiebord::doe_zet(bordvakje* pos){
  if (zetten == 0 && pos -> marked == true){
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
  // controle_win();
}

void koffiebord::controle_win(){
  bordvakje* col_fol = ingang;
  bordvakje* checker;
  int gesloten_vakjes = 0;

  while (col_fol){
    checker = col_fol;
    while (checker){
      if (checker -> geopend == false || checker -> flag == true){
        gesloten_vakjes++;
      }
      
      checker = checker -> buren[2];
    }
    col_fol = col_fol -> buren[4];
  }
  if (gesloten_vakjes == aantal_koffies){
    player_win(); 
  }
}

void koffiebord::player_win(){
  spel_af = true;

  cout << "Je hebt gewonnen!!" << endl
  << "Je hebt alle koffies ontweken!" << endl;
}


void koffiebord::koffie_zet(){
  bordvakje* left_col = ingang;
  bordvakje* ober;
  int koffie_plaats;
  int controle = 0;
  koffie_plaats = rand() % ((breedte*hoogte) - aantal_koffies);

  if (koffie_plaats == 0){
    koffie_plaats = 1;
  }
  while (left_col){
    ober = left_col;
    while (ober){
      if (ober -> marked == false){
        controle++;
      }
      if (controle == koffie_plaats){
        break;
      } 
      ober = ober -> buren[2];
    }
    if (controle == koffie_plaats){
      break;
    }
    left_col = left_col -> buren[4];
  }
  for (int i = 0; i < 8; i++){
    if (ober -> buren[i]){
      ober -> buren[i] -> aantal_buren++;
    }
  }
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

// Dit programma runt wannneer je de computer een x aantal keren een bord
// wil laten spelen
void koffiebord::comp(){
  int aantal_runs = 0;
  int a[100]{0} ,wins = 0;
  cout << "Hoe vaak wil je het programma laten gaan?: ";
  aantal_runs = read_num(6); // Opvragen aantal runs

  for (int i = 0; i < aantal_runs; i++){
    while (comp_af == false && comp_win == false){
      comp_zet();
    }
    if (comp_af == true){
      a[comp_zetten]++;  
    } else if (comp_win == true){
      wins++;
    }
    reset_bord();
    koffies();
    comp_zetten = 0;
    comp_win = false;
    comp_af = false;
  }
  for (int i = 1; i < 21; i++){
    cout << "Aantal zetten: " << i << "  Aantal keer: " << a[i] << endl;
  }
  cout << "Aantal keer gewonnen: " << wins << endl;
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
  comp_doe_zet(pos_controle);
}


void koffiebord::comp_doe_zet(bordvakje* pos){
  if (pos -> marked == true){
    if (comp_zetten == 0){
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
      comp_af = true;
    }
  } else if (pos -> geopend == true){
    comp_zet();
  } else {
    pos -> geopend = true;
  }
 
  if (pos -> aantal_buren == 0){
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
}