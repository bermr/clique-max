//entrada grafos dimacs

#include <chrono>
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstdio>
#include <algorithm>
#include <time.h>

using namespace std;
using namespace std::chrono;

/*dado.h*/
struct Dados{
	int n;
	int m;
};

/*lista.h*/
struct Lista{
    int v;
    int grau;
    vector<int> adj;
};


/*Arquivo.h*/
int abreArquivo(FILE **, char*);
int carregaArquivo(FILE **, vector<Lista>&);
int carregaTamanho(FILE **, Dados*);

/*cliqueMax.h*/
int cliqueMaxPossivel(vector<Lista>&, int);
int cliqueMaximo(vector<Lista>, int);
void acharMax(vector<int>, vector<Lista> , int , int&);
bool formaClique(vector<int>, vector<Lista>, int);

void contarGraus(vector<Lista>&);
void ordenarAdj(vector<Lista>&);

int main(int argc, char *argv[]){
    duration<double> time_span2;
    FILE* entrada;
    Dados dados;
    char nome[16];

    sprintf(nome, "Instancias/%s", argv[1]);
    if (!abreArquivo(&entrada, nome)){
        cout << "Erro na abertura do arquivo!" << endl;
        return 0;
    }
    carregaTamanho(&entrada, &dados);
    vector<Lista> listaAdj(dados.m);

    carregaArquivo(&entrada, listaAdj);

    cout<<"Quantidade de vertices:"<<dados.m-1<<" Arestas:"<<dados.n-1<<endl;

    contarGraus(listaAdj);

    int cMax;
    int cEncontrado;

    cMax = cliqueMaxPossivel(listaAdj, dados.m);

    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    cEncontrado = cliqueMaximo(listaAdj, cMax);

    cout<<"Clique maximo encontrado: "<<cEncontrado<<endl;

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    cout<<"Tempo total de programa: "<<time_span.count()<<endl;

    return 0;
}
/*Arquivo.cpp*/
int abreArquivo(FILE **entrada, char* nome){
	if ((*entrada) = fopen(nome, "r")){
			cout<<"Arquivo aberto"<<endl;
			return 1;
	}
	return 0;
}

int carregaArquivo(FILE **entrada, vector<Lista> &listaAdj){
	int v1 = 0, v2 = 0;
	char e, enter;
	while(fscanf(*entrada, "%c %d %d %[\n]", &e,  &v1, &v2, &enter) != EOF){
		listaAdj[v1].v = v1;
		listaAdj[v1].adj.push_back(v2);
		listaAdj[v2].v = v2;
		listaAdj[v2].adj.push_back(v1);
	}

	listaAdj[0].adj.clear();
	fclose(*entrada);
}

int carregaTamanho(FILE **entrada, Dados* dados){
	char edge[8];
	fscanf(*entrada, "%[A-Z a-z] %d %d", edge, &dados->m, &dados->n);
	dados-> m++;
	dados-> n++;
}

/*cliqueMax.cpp*/
int cliqueMaxPossivel(vector<Lista>& listaAdj, int n){
    vector<int> b[n];
    int index = 0;
    for (int i=0; i<n; i++){
       int bi = listaAdj[i].grau;
       b[bi].push_back(listaAdj[i].grau);//lista com o grau dos vértices
    }

   int cMax = 0;
   int acumulador = 0;
   for (int i=n-1; i>=0; i--){//Faz o calculo de qual o clique máximo possivel
	acumulador += b[i].size();
    if (acumulador >= i-1 && i > cMax)//Se tiver pelo menos i vértices de grau i-1 o clique máximo possivel é atualizado
        cMax = i+1;
   }

	cout<<endl<<"Clique maximo possivel:" << cMax <<endl;
	return cMax;
}


int cliqueMaximo(vector<Lista> listaAdj, int cMax){
    vector<int> clique;
    int ale;
    int cMaxEncontrado = 1;
    int novoVertice;
    int cont=0;
    int maxFinal = 1;
    srand((unsigned)time(NULL));
    clock_t inicio,fim;
    inicio = clock();

    for(int i=1;i<(listaAdj.size()/2);i++){ //faz n/2 vezes ou até achar o máximo possivel ou até o tempo determinado
        ale = (rand()%(listaAdj.size()-1)+1); //escolhendo um vértice aleatório para tentar formar o clique
        acharMax(clique, listaAdj, ale, cMaxEncontrado);
        if (cMaxEncontrado > maxFinal){
            maxFinal = cMaxEncontrado;
            cout<<cMaxEncontrado<<endl;
        }
        if (maxFinal == cMax) return cMax;
        fim = clock() - inicio;
        if (((int)fim)/CLOCKS_PER_SEC >= listaAdj.size()/2) return maxFinal;
    }
    return maxFinal;
}

void  acharMax(vector<int> clique, vector<Lista> listaAdj, int vertice, int& cMaxEncontrado){
    int novoVertice;
    clock_t inicio, fim;
    inicio = clock();

    clique.push_back(vertice);
    for (int i=0;i<listaAdj[vertice].adj.size();i++){
        novoVertice = listaAdj[vertice].adj[i];//para cada vértice adjacente ao vértice aleatório
        clique.push_back(novoVertice);//adiciona o vértice no clique
        for (int j=1;j<listaAdj[vertice].adj.size();j++){
            novoVertice = listaAdj[vertice].adj[j];//para cada vértice adjacente ao vértice aleatório
            if (formaClique(clique, listaAdj, novoVertice)) clique.push_back(novoVertice);//Verifica se ele forma um clique, se formar, coloca ele na lista do clique
            if (clique.size() > cMaxEncontrado) cMaxEncontrado = clique.size();
        }
        clique.clear();
        clique.push_back(vertice);
        rotate(listaAdj[vertice].adj.begin(),listaAdj[vertice].adj.begin()+1,listaAdj[vertice].adj.end());
        fim = clock() - inicio;
        if (((int)fim)/CLOCKS_PER_SEC >= 320) return;
    }
}

bool formaClique(vector<int> clique, vector<Lista> listaAdj, int vertice){
    int cont = 0;
    for (int k1 = 0; k1< clique.size(); k1++){
        for (int k2 = 0; k2 < listaAdj[vertice].adj.size(); k2++){
            if (listaAdj[vertice].adj[k2] == clique[k1]) cont++;//Fixa um vértice da clique, e verifica se
        }                                                       //esse vértice está na lista de adjacencia
    }
    if (cont == clique.size()) return true;//Se todos os vértices da click foram encontrados na lista de ajacencia
    else return false;
}

/*lista.cpp*/
void contarGraus(vector<Lista>& listaAdj){
    for (int i=0;i<listaAdj.size();i++)
        listaAdj[i].grau = listaAdj[i].adj.size();

}
