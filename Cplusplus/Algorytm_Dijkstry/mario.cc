//Zadanie polegalo na znalezieniu najkrotszej sciezki od startu
//do mety na prostym torze, ktorego pola mialy rozne dzialania, a
//Mario mogl poruszac sie o jedno pole do przodu lub skoczyc o dwa,
//ale jego skocznosc ulegala zmianie po wejsciu na pole oznaczone '*'.

#include <iostream>
#include <queue>

#define nieskonczonosc 2000000000

using namespace std;

class Wierzcholek {
public:
  int id;
  int odl_od_pocz;
  int skok;
  Wierzcholek(int i, int o, int sk)
  {
    id=i;
    odl_od_pocz=o;
    skok=sk;
  }
  friend bool operator < (const Wierzcholek &p1, const Wierzcholek &p2)
  {
    return p1.odl_od_pocz>p2.odl_od_pocz;
  }
};

int ktora_pot_dwojki(int p, int pot[16])
{
  for(int i=0;i<16;++i){
    if(pot[i]==p) return i;
  }
  return 16;
}

int min(int odl[][17], int n)
{
  int wynik=nieskonczonosc;
  for(int i=0;i<17;++i){
    if(odl[n-1][i]<wynik) wynik=odl[n-1][i];
  }
  return wynik;
}

void oblicz(int n, int odl[][17], char tor[], priority_queue<Wierzcholek>& k, int U, int K)
{
  int pot[16];
  pot[0]=2;
  for(int i=1;i<16;++i) pot[i]=pot[i-1]*2;
  Wierzcholek akt=k.top();
  int akt_id, S, wyk;
  while(!k.empty()){
    akt=k.top();
    akt_id=akt.id;
    S=akt.skok;
    wyk=ktora_pot_dwojki(S, pot);
    k.pop();
    if(odl[akt_id][wyk]==nieskonczonosc){
      if(wyk==16) S=1;
      if(odl[akt_id][wyk]>akt.odl_od_pocz) odl[akt_id][wyk]=akt.odl_od_pocz;
      if(min(odl, n)<nieskonczonosc) break;

      if(tor[akt.id]=='.'){
        if((akt_id+1)<n) k.push(Wierzcholek(akt_id+1, akt.odl_od_pocz+1, S));
        if(S!=1 && (akt_id+S)<n) k.push(Wierzcholek(akt_id+S, akt.odl_od_pocz+1, S));
      }
      else if(tor[akt_id]=='>'){
        k.push(Wierzcholek(akt_id+K, akt.odl_od_pocz, S));
      }
      else if(tor[akt_id]=='<'){
        k.push(Wierzcholek(akt_id-K, akt.odl_od_pocz, S));
      }
      else if(tor[akt_id]=='U'){
        if((akt_id+1)<n) k.push(Wierzcholek(akt_id+1, akt.odl_od_pocz+1+U, S));
        if(S!=1 && (akt_id+S)<n) k.push(Wierzcholek(akt_id+S, akt.odl_od_pocz+1+U, S));
      }
      else if(tor[akt_id]=='*'){
        if(S!=1){
          if((akt_id+1)<n) k.push(Wierzcholek(akt_id+1, akt.odl_od_pocz+1, 2*S));
          if((akt_id+2*S)<n) k.push(Wierzcholek(akt_id+2*S, akt.odl_od_pocz+1, 2*S));
        }
        else{
          if((akt_id+1)<n) k.push(Wierzcholek(akt_id+1, akt.odl_od_pocz+1, 1));
        }
      }
    }
  }
}

int main(void)
{
  int n, K, U;
  cin>>n>>K>>U;
  if(n==1 || n==2){
    cout<<n-1<<endl;
    return 0;
  }
  char tor[n];
  int odl[n][17];
  for(int i=0;i<n;++i){
    cin>>tor[i];
  }
  for(int i=0;i<n;++i){
    for(int j=0;j<17;++j){
      odl[i][j]=nieskonczonosc;
    }
  }
  odl[0][0]=0;
  priority_queue<Wierzcholek> k;
  k.push(Wierzcholek(1, 1, 2));
  k.push(Wierzcholek(2, 1, 2));
  oblicz(n, odl, tor, k, U, K);

  if(min(odl, n)==nieskonczonosc) cout<<"-1"<<endl;
  else cout<<min(odl, n)<<endl;

  return 0;
}
