// Piotr Sulikowski i Jan Szala

#include <bits/stdc++.h>
using namespace std;
const int SZEROKOSC = 35;
const int WYSOKOSC = 20;
const int ILOSC_RUND = 350;
const int JA = 0;
const int ILOSC_KIERUNKOW = 4;

struct PUNKT{
    int x, y;
    bool operator==(const PUNKT other) const{
        return x == other.x && y == other.y;
    }
};

struct GRACZ{
    PUNKT pozycja;
};


PUNKT strategia_chodzenia_do_wybranych_punktow(GRACZ& gracz, int& do_ktorego_idzemy){
    vector <PUNKT> lista_miejsc = {{1,1},{3,3},{7,4},{14,12},{8,15},{1,19},{1,1},{12,1},{16,1},{12,6}};
    if(do_ktorego_idzemy >= lista_miejsc.size()){
        return {0, 0};
    }
    if (!(lista_miejsc[do_ktorego_idzemy] == gracz.pozycja)){
        return lista_miejsc[ do_ktorego_idzemy ];
    }
    else {
        do_ktorego_idzemy ++;
        return lista_miejsc[ do_ktorego_idzemy ];
    }
}

PUNKT strategia_chodzenia_na_pale(vector< string >& mapa){
    for(int i = 0; i < WYSOKOSC; ++i){
        for(int j = 0; j < SZEROKOSC; ++j){
            if(mapa[ i ][ j ] == '.'){
                return {i, j};
            }
        }
    }
    return {0, 0};
}

vector< PUNKT > KIERUNKI = {
    {-1, 0}, // gora
    {0, 1},  // prawo
    {1, 0},  // dol
    {0, -1}  // lewo
};

vector< PUNKT > KIERUNKI_PRZEKATNE = {
    {-1, -1}, //gora-lewo
    {-1, 1},  //gora-prawo
    {1, 1},   //dol-prawo
    {1, -1}  //dol-lewo
};

bool czy_na_mapie(PUNKT punkt){
    return punkt.x >= 0 && punkt.x < WYSOKOSC && punkt.y >= 0 && punkt.y < SZEROKOSC;
}

PUNKT bfs(PUNKT& pozycja, vector< string >& mapa, vector< vector< bool > >& vis){
    queue< PUNKT > q;
    q.push(pozycja);
    while(!q.empty()){
        PUNKT temp = q.front(); q.pop();
        vis[ temp.x ][ temp.y ] = true;
        for(auto[ dx, dy ] : KIERUNKI){
            int nx = temp.x + dx;
            int ny = temp.y + dy;
            if(czy_na_mapie({nx, ny})){
                if(!vis[ nx ][ ny ]){
                    if(mapa[ nx ][ ny ] == '.'){
                        return {nx, ny};
                    }
                    q.push({nx, ny});
                }
            }
        }
    }
    return {0, 0};
}

PUNKT strategia_chodzenia_do_najbliższego_wolnego_pola(GRACZ& gracz, vector< string >& mapa){
    vector< vector< bool > >vis(WYSOKOSC, vector< bool >(SZEROKOSC, false));
    PUNKT cel = bfs(gracz.pozycja, mapa, vis);
    return cel;
}

int ile_razy_przejde_po_swoich_polach(PUNKT start, PUNKT koniec, vector< string >& mapa){
    if(koniec.x > start.x){
        swap(koniec.x, start.x);
    }
    if(koniec.y > start.y){
        swap(koniec.y, start.y);
    }
    int licznik = 0;
    for(int i = koniec.x; i <= start.x; ++i){
        if(mapa[ i ][ koniec.y ] == '0'){
            licznik++;
        }
    }
    for(int i = koniec.x; i <= start.x; ++i){
        if(mapa[ i ][ start.y ] == '0'){
            licznik++;
        }
    }
    for(int i = koniec.y + 1; i <= start.y; ++i){
        if(mapa[ koniec.x ][ i ] == '0'){
            licznik++;
        }
    }
    for(int i = koniec.y + 1; i <= start.y; ++i){
        if(mapa[ start.x ][ i ] == '0'){
            licznik++;
        }
    }
    return licznik;
}

bool czy_jest_przeciwnik_w_srodku(PUNKT start, PUNKT koniec, vector< string >& mapa){
    if(koniec.x > start.x){
        swap(koniec.x, start.x);
    }
    if(koniec.y > start.y){
        swap(koniec.y, start.y);
    }
    for(int i = koniec.x; i <= start.x; ++i){
        for(int j = koniec.y; j <= koniec.y; ++j){
            if(mapa[ i ][ j ] == '1' || mapa[ i ][ j ] == '2' || mapa[ i ][ j ] == '3'){
                return true;
            }
        }
    }
    return false;
}

PUNKT szukaj_kwadratu(PUNKT& punkt, vector< string >& mapa){
    const int BOK = 8;
    const int ILOSC_DOZWOLONYCH_POWTORZEN = 5;
    for(auto[ dx, dy ] : KIERUNKI_PRZEKATNE){
        PUNKT nowy_punkt = {punkt.x + (dx * BOK), punkt.y + (dy * BOK)};
        if(czy_na_mapie(nowy_punkt)){
            if(mapa[ nowy_punkt.x ][ nowy_punkt.y ] == '.' && ile_razy_przejde_po_swoich_polach(punkt, nowy_punkt, mapa) <= ILOSC_DOZWOLONYCH_POWTORZEN && !czy_jest_przeciwnik_w_srodku(punkt, nowy_punkt, mapa)){
                return nowy_punkt;
            }
        }
    }
    return {-1, -1};
}

bool czy_jestem_na_krewedzi(PUNKT& pozycja, vector< string >& mapa){
    for(auto [dx, dy] : KIERUNKI){
        PUNKT nowy_punkt = {pozycja.x + dx, pozycja.y + dy};
        if(czy_na_mapie(nowy_punkt)){
            if(mapa[ nowy_punkt.x ][ nowy_punkt.y ] == '.'){
                return true;
            }
        }
    }
    return false;
}

PUNKT idz_do_najbliższej_krawędzi(PUNKT pozycja, vector< string >& mapa){
    int minimalna_ilosc = 1 << 20;
    PUNKT cel = {0, 0};
    for(auto[ dx, dy ] : KIERUNKI){
        PUNKT nowa_pozycja = {pozycja.x + dx, pozycja.y + dy};
        int licznik = 1;
        while(czy_na_mapie(nowa_pozycja)){
            if(mapa[ nowa_pozycja.x ][ nowa_pozycja.y ] == '.'){
                break;
            }
            licznik++;
            nowa_pozycja = {nowa_pozycja.x + dx, nowa_pozycja.y + dy};
        }
        if(minimalna_ilosc > licznik && czy_na_mapie(nowa_pozycja)){
            minimalna_ilosc = licznik;
            cel = nowa_pozycja;
        }
    }
    return cel;
}

bool brak_zaczetego_kwadratu(PUNKT& start, PUNKT& koniec){
    return koniec == (PUNKT){-1, -1} && start == (PUNKT){-1, -1};
}

PUNKT strategia_robienia_kwadrata_na_pale(GRACZ gracz, vector< string >& mapa, PUNKT& start, PUNKT& koniec){
    if(!czy_jestem_na_krewedzi(gracz.pozycja, mapa) && brak_zaczetego_kwadratu(start, koniec)){
        return idz_do_najbliższej_krawędzi(gracz.pozycja, mapa);
    }
    if(brak_zaczetego_kwadratu(start, koniec)){
        start = {gracz.pozycja.x, gracz.pozycja.y};
        koniec = szukaj_kwadratu(gracz.pozycja, mapa);
        if(koniec == (PUNKT){-1, -1}){
            start = {-1, -1};
            return strategia_chodzenia_do_najbliższego_wolnego_pola(gracz, mapa);
        }
        return koniec;
    }
    if(!(gracz.pozycja == koniec) && !(koniec == (PUNKT){-1, -1})){
        return koniec;
    }
    else if(gracz.pozycja == koniec){
        koniec = {-1, -1};
        return start;
    }
    else if(!(gracz.pozycja == start)){
        return start;
    }
    else if(gracz.pozycja == start){
        start = {-1, -1};
        return strategia_robienia_kwadrata_na_pale(gracz, mapa, start, koniec);
    }
    else{
        return strategia_chodzenia_do_najbliższego_wolnego_pola(gracz, mapa);
    }
}

vector <vector<int>> bfs_2(PUNKT& pozycja, vector< string >& mapa, vector< vector< bool > >& vis){
    vector< vector< int > >odleg(WYSOKOSC, vector< int >(SZEROKOSC, 0));
    queue< PUNKT > q;
    q.push(pozycja);
    while(!q.empty()){
        PUNKT temp = q.front(); q.pop();
        vis[ temp.x ][ temp.y ] = true;
        for(auto[ dx, dy ] : KIERUNKI){
            int nx = temp.x + dx;
            int ny = temp.y + dy;
            if(czy_na_mapie({nx, ny})){
                if(!vis[ nx ][ ny ]){
                    odleg[nx][ny] = odleg[temp.x][temp.y]+1; 
                    vis[nx][ny] = true;
                    q.push({nx, ny});
                }
            }
        }
    }
    return odleg;
}

vector< vector< int > > tworzenie_mapy_kto_ma_blizej ( vector< GRACZ >& gracze, vector< string >& mapa){
    vector< vector< bool > >vis(WYSOKOSC, vector< bool >(SZEROKOSC, false));
    vector<vector<int>> dystans_do_mnie = bfs_2(gracze[JA].pozycja, mapa, vis);
    vector< vector< bool > >vis_opo(WYSOKOSC, vector< bool >(SZEROKOSC, false));
    vector<vector<int>> dystans_do_oponenta = bfs_2(gracze[1].pozycja, mapa, vis_opo);
    vector< vector< int > >mapa_odl(WYSOKOSC, vector< int >(SZEROKOSC, 0));
    for (int i = 0;i < WYSOKOSC;i++){
        for (int j = 0;j < SZEROKOSC;j++){
            int ile = dystans_do_oponenta[i][j] - dystans_do_mnie[i][j];
            mapa_odl[i][j] = ((ile <= 0)? -1 : 1) * dystans_do_mnie[i][j];
            //wypisywanie
            //string wypisz = to_string(mapa_odl[i][j]);
            //wypisz += ((mapa_odl[i][j] < 10 && mapa_odl[i][j] > -10) ? " " : "");
            //wypisz = ((mapa_odl[i][j] >= 0) ? " " : "") + wypisz;
            //cerr << wypisz << " ";
        }
        cerr << endl;
    }
    return mapa_odl;
}
// faza alfa
PUNKT decyzaj_gdzie_isc( vector< GRACZ >& gracze, vector< string >& mapa,vector< vector< int > >& kto_ma_blizej){
    return {0,0};
}
 
PUNKT strategia_laczenia_bfs_ow ( vector< GRACZ >& gracze, vector< string >& mapa){
    vector< vector< int > > kto_ma_blizej = tworzenie_mapy_kto_ma_blizej (gracze, mapa);
    return decyzaj_gdzie_isc(gracze, mapa, kto_ma_blizej);
    return {0,0};
}

int main(){

    int ILOSC_PRZECIWNIKOW;
    int AKTUALNA_RUNDA;
    cin >> ILOSC_PRZECIWNIKOW;
    vector< GRACZ > gracze(ILOSC_PRZECIWNIKOW + 1);
    vector< string > mapa(WYSOKOSC);
    PUNKT nastepny_kwadrat = {-1, -1};
    PUNKT pozycja_poczatkowa = {-1, -1};
    int do_ktorego_idzemy = 0;
    
    while (1) {
        cin >> AKTUALNA_RUNDA;
        int x, y, _;
        cin >> x >> y >> _;
        int nx = y;
        int ny = x;
        gracze[ JA ].pozycja = {nx, ny};
        for (int i = 0; i < ILOSC_PRZECIWNIKOW; ++i) {
            cin >> x >> y >> _;
            int nx = y;
            int ny = x;
            gracze[ i + 1 ].pozycja = {nx, ny};
        }
        for (int i = 0; i < 20; i++) {
            cin >> mapa[ i ];
        }
        PUNKT wyjscie;
        //wyjscie = strategia_chodzenia_do_wybranych_punktow(gracze[ JA ], do_ktorego_idzemy);
        //wyjscie = strategia_chodzenia_na_pale(mapa);
        //wyjscie = strategia_chodzenia_do_najbliższego_wolnego_pola(gracze[ JA ], mapa);
        //wyjscie = strategia_laczenia_bfs_ow (gracze, mapa);
        wyjscie = strategia_robienia_kwadrata_na_pale(gracze[ JA ], mapa, nastepny_kwadrat, pozycja_poczatkowa);
        cout << wyjscie.y << " " << wyjscie.x << '\n';
    }
}
