#include <fstream>
#include <iostream>
#include<cstdlib>
#include<vector>
#include<iomanip>
#include<queue>
#include <stdexcept>


using namespace std;

int red[] = { -1, 0, 0, 1 };
int kol[] = { 0, -1, 1, 0 };

struct Cvor
{
	int x;
	int y;
	int dist;
};

class Labirint
{
    int brRedova;
    int brKolona;
    vector< vector<int>> M;
public:
    Labirint( int n, int m);
    Labirint();
    void Ispisi();
    void DodajProlaze(int &brojac, vector<int> praznine);
    int BFS( int i, int j, int x, int y);
    int Udaljenost(char slovo, int j, char slovo1, int y);
    int uBroj(char c);
    bool provjeri( vector< vector<bool>> posjeceni, int red, int kol);
};

int Labirint::uBroj(char c)
{
int vrijednosti[26];
vrijednosti[0]=0;
for(int i=1;i<26;i++)
    {
        if(i%2==1)
            vrijednosti[i]=vrijednosti[i-1]+3;
        else
            vrijednosti[i]=vrijednosti[i-1]+1;
    }
return vrijednosti[int(c)-65];
}

int Labirint::BFS( int i, int j, int x, int y)
{
	vector< vector<bool>> posjeceni;
	posjeceni.resize(brRedova);
    for ( int k=0; k<brRedova; k++ )
    {
        posjeceni[k].resize( brKolona );
        for( int l=0; l<brKolona;l++ )
            posjeceni[k][l]=0;
    }
	queue<Cvor> q;
	posjeceni[i][j] = true;
	q.push({i, j, 0});
	int min_dist = INT_MAX;
	while (!q.empty())
	{
		Cvor Cvor = q.front();
		q.pop();

		int i = Cvor.x;
		int j = Cvor.y;
		int dist = Cvor.dist;

		if (i == x && j == y)
		{
			min_dist = dist;
			break;
		}

		for( int k = 0; k < 4; k++ )
		{

			if ( provjeri( posjeceni, i + red[k], j + kol[k]) )
			{
				posjeceni[i + red[k]][j + kol[k]] = true;
				q.push( { i + red[k], j + kol[k], dist + 1 } );
			}
		}
	}

	if (min_dist != INT_MAX)
		return min_dist;
	else
		return 0;
}


int Labirint::Udaljenost(char slovo, int j, char slovo1, int y)
{

    int i = uBroj(slovo);
    int x = uBroj(slovo1);

     if((i==0 or i%2==0)and (x==brRedova-1))
      return BFS(i,j-1,x-1,y-1);

    if(i==0 and x%2==1)
        return BFS(i,j-1,x+1,y-1);

    if(i==0 and x%2==0)
        return BFS(i,j-1,x+1,y-1)+1;

    if(i%2==0 and x%2==1)
        return BFS(i+1,j-1,x-1,y-1)+1;

    if(i%2==1 and x%2==1)
        return BFS(i-1,j-1,x-1,y-1)+1;

    if(i%2==0 and x%2==0)
        return BFS(i+1,j-1,x+1,y-1)+1;

    return BFS(i-1,j-1,x+1,y-1)+1;

}


bool Labirint::provjeri( vector< vector<bool>> posjeceni, int red, int kol)
{
	return (red >= 0) && (red < brRedova) && (kol >= 0) && (kol < brKolona)
		&& !M[red][kol] && !posjeceni[red][kol];
}

Labirint::Labirint( int n, int m)
{
    brRedova = n;
    brKolona = m;
    M.resize(n);
    for( int i=0; i<n; i++ )
        M[i].resize(m);
    for( int i=0; i<n; i++)
        for( int j=0; j<m; j++)
        {
            if(i==0 or i==n-1)
                M[i][j]=j+1;
            else
                M[i][j]=0;
        }
}

 void Labirint::Ispisi()
 {
     for(int i = 0; i < brRedova; i++)
        {
        for(int j = 0; j < brKolona; j++) cout << "+--";
            cout << "+\n";
        for(int j = 0; j < brKolona; j++) cout << "|" << setw(2) << M[i][j];
            cout << "|\n";
        }
    for(int j = 0; j < brKolona; j++) cout << "+--";
    cout << "+\n";
 }



int stringUInt (string s){
    int i = atoi(s.c_str());
    return i;
}

vector<int> procitajLiniju( string linija, int m )
{
    string pom1 = "";
    vector<int> V;
    int i=0;
   while(linija[i])
    {
        if(linija[i]!=',')
            {
                pom1+=linija[i];
            }
        else
            {
            int broj = stringUInt(pom1);
            if(broj > m )
                 throw invalid_argument("Pozicija prolaza je veca od broja kolona!");
            V.push_back(broj);
            pom1="";
            }
            i++;
    }
    if(pom1!="")
        {
            int broj = stringUInt(pom1);
            if(broj > m )
                 throw invalid_argument("Pozicija prolaza je veca od broja kolona!");
            V.push_back(broj);
        }
    return V;
}

void Labirint::DodajProlaze(int &brojac, vector<int> praznine)
{
    if(praznine.size()==1 )
    {
        for( int j=0; j<brKolona; j++)
         if( j+1 != praznine[0])
            {
                M[brojac][j]=j+1;
                M[brojac+1][j]=j+1;
            }
    }
    else{
            int v =0;
    while(v < praznine.size())
        {
         for( int j=0; j<brKolona; j++)
         if( j+1 != praznine[v])
         {
                M[brojac][j]=j+1;
                M[brojac+1][j]=j+1;
         }
         else v++;
        }
        }

brojac++;
}

int main()
{
try{
ifstream infile("labirint.txt");
string linija;
int brojac=0;
int n,m;
getline(infile, linija);

n = 2*stringUInt(linija);
if(n>52)
    throw invalid_argument("Prva linija fajla mora biti manja od broja 26!");
getline(infile, linija);
m = stringUInt(linija);
Labirint L(n,m);
while(getline(infile, linija))
{
        brojac+=3;
        vector<int> praznine;
        praznine = procitajLiniju(linija, m);
        L.DodajProlaze(brojac, praznine);
}
infile.close();

//L.Ispisi();
//cout<<endl;
cout<<endl;
    cout<<"Najkraca udaljenost je: "<<L.Udaljenost('B',3,'C',3)<<endl;
    cout<<endl;
    cout<<"Najkraca udaljenost je: "<<L.Udaljenost('A',1,'G',2)<<endl;
    cout<<endl;
    cout<<"Najkraca udaljenost je: "<<L.Udaljenost('A',1,'J',2)<<endl;
    cout<<endl;
    cout<<"Najkraca udaljenost je: "<<L.Udaljenost('G',3, 'H',7)<<endl;
    cout<<endl;
    cout<<"Najkraca udaljenost je: "<<L.Udaljenost('F',1,'F',7)<<endl;
    cout<<endl;
    cout<<"Najkraca udaljenost je: "<<L.Udaljenost('C',1,'G',1)<<endl;
}
catch ( const exception& e )
    {
        cout<<e.what();
    }
}

