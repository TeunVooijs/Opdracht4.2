const int MAX = 100;

class stapelvakje
{
public:
  stapelvakje* prev;
  koffiebord* stapel_bord_ing;
};

class stapel { // voor maximaal MAX integers
  private:
    stapelvakje* last_in;
  
  public:
    stapel() { 
      last_in=nullptr;
      teller = 0;
    } // constructor
    ~stapel();
    void zetopstapel (koffiebord* player_bord);
    void haalvanstapel (koffiebord* & player_bord);
    int teller; 

};//stapel
