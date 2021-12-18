// file koffiebord.h

int leesgetal (int bovengrens);

class bordvakje {
  public:
    bool geopend;
    int aantal_buren;     //     7 0 1
    bordvakje* buren[8];  //     6   2
    bordvakje ( );        //     5 4 3
    bool marked;
    bool flag;
    // TODO
};//bordvakje

class koffiebord {
  private:
    bordvakje* nieuwe_rij;
    bordvakje* vorige_rij;
    void koffies();
    bordvakje* readcoord(int i, int j);
    bordvakje* maak_bord();
    bordvakje* maakrij();
    void ritsen();
    void flood_fill(bordvakje* pos);
    void doe_zet(bordvakje* pos);
    void koffie_zet();
    void closed_counter();
    void speler_af();
    int closed_cells;
    int hoogte, breedte, percentage;
    int aantal_koffies;
    int zetten;

    // void doe_zet(bordvakje* pos);
  public:
    bordvakje* copybord();
    bordvakje* ingang;
    bool spel_af;
    void legenda();
    void drukaf ( );
    void rand_zet();
    void player_zet();
    void flag_zet();
    koffiebord (int h, int b, int p);
    ~koffiebord ();
    // TODO
};//koffiebord
