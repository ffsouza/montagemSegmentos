/*
	Lucas Porto Maziero
	Rafael Kendy Arakaki
	MO824 - GRASP aplicada ao Problema do Caixeiro Viajante (PCV)
	Outubro/2014
*/

/*
   V4:
   - Corrigida Busca Local.
   - Criação da nova heurística construtiva (Nearest Neighborhood).
   - Removido heurística construtiva antiga (Best Edges).
   - Removido best-improvement (first-improvement se mostrou melhor).
   - Torna Busca Local mais rápida (otimizações)
   - Ideia: Implementar 3-OPT para a melhor solução de toda a execução do programa. #TODO#
*/

#include <stdio.h>
#include <omp.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <float.h>

#define maxCidades 440

//#define DEBUG

#ifdef DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

using namespace std;

void readFile(char* filePath);
vector<int> grasp(void);
vector<int> contrucaoSolucaoInicialNearestNeighbor(double alfa);
vector<int> buscaLocal (vector<int>);
double funcaoObjetivo (vector<int>);

// Dados Globais
int nCidades;
double distM[maxCidades][maxCidades];

// Funções para Heurística Construtiva
int irandom(int a); // Retorna um número entre [0, a]

int main(void)
{
    char instancia[200];
    printf("Digite a instancia:\n");
    scanf(" %s", instancia);

    // Inicializa dados do programa
    readFile(instancia); // ./instancias/pr299.tsp

    vector<int> melhorSolucao;
    melhorSolucao = grasp();

    return 0;
}

vector<int> grasp (void)
{
    vector<int> solucaoInicial;
    vector<int> solucao;
    double custoSolucao;
    vector<int> melhorSolucao;
    double custoMelhorSolucao = INFINITY;

    //std::srand ( unsigned ( std::time(0) ) );
    clock_t begin = clock();
    double elapsed_secs = 0.0;
    int k = 0;

    double custoMedioInicial = 0.0;
    double custoMedioPosBusca = 0.0;
    double ALPHA = 0.01;
    //printf("Digite o alfa:\n");
    //scanf("%lf", &ALPHA);

    //const int MAX_ITERACOES = 100000;
    // Critério de parada: 10 minutos
    int countIteracoes = 0;
    for (countIteracoes = 0; elapsed_secs < 600; countIteracoes++)
    {
        clock_t end = clock();
        elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        solucaoInicial = contrucaoSolucaoInicialNearestNeighbor(ALPHA*irandom(5)); // Alpha definido dentre { 0.00, 0.01, 0.02, 0.03, 0.04, 0.05} randomizadamente.
        //printf("Construcao Inicial NN: %f. Vertices: %d\n", funcaoObjetivo(solucaoInicial), solucaoInicial.size());
        custoMedioInicial += funcaoObjetivo(solucaoInicial);
        solucao = buscaLocal(solucaoInicial);
        //printf("Pos Busca Local: %f. Vertices: %d\n\n", funcaoObjetivo(solucao), solucao.size());
        custoSolucao = funcaoObjetivo(solucao);
        custoMedioPosBusca += custoSolucao;
        if (custoSolucao < custoMelhorSolucao)
        {
            custoMelhorSolucao = custoSolucao;
            melhorSolucao = solucao;
            printf("Uma melhor solução foi encontrada: %f. Vertices: %d\n", funcaoObjetivo(melhorSolucao), melhorSolucao.size());
        }
        //getchar();
    }
    custoMedioInicial /= countIteracoes;
    custoMedioPosBusca /= countIteracoes;
    printf("\n***Custo medio:***\nMedio Inicial: %f\nMedio PosBusca: %f\nQtde Iteracoes: %d\n",custoMedioInicial, custoMedioPosBusca, countIteracoes);
    printf("***Melhor solucao: %f\nAlpha = %f", custoMelhorSolucao, ALPHA);
    printf("Solucao = { ");
    for(int i = 0; i < nCidades; i++){
        printf("%d, ", melhorSolucao[i]);
    }
    printf("}\n");
    return melhorSolucao;
}

// *** INÍCIO das função de Heurística Construtiva NEAREST NEIGHBOUR ***
vector<int> contrucaoSolucaoInicialNearestNeighbor(double alfa){
    vector<int> myCities;
    int visitados[nCidades];

    for (int i = 0; i < nCidades; i ++) visitados[i] = 0;

    int verticeInicial = irandom(nCidades-1);
    int x = verticeInicial;
    int y;
    myCities.push_back(verticeInicial + 1);
    visitados[verticeInicial] = 1;

    // Gera uma solução inicial vizinho mais próximo
    while (myCities.size() < nCidades)
    {
        double menor = INFINITY;
        double maior = -DBL_MAX;
        vector<int> conjuntoCandidatos;
        vector<int> custoCandidatos;
        vector<int> conjuntoRCL;
		
        for (int k = 0; k < nCidades; k ++)
        {
            if ( k != x && visitados[k] == 0){
                conjuntoCandidatos.push_back(k);
                //printf("%d ", k+1);
                if ( distM[x][k] < menor){
                    menor = distM[x][k];
                    //printf("menor = %d (%f)\n", k+1, distM[x][k]);
                }
                if ( distM[x][k] > maior){
                    maior = distM[x][k];
                    //printf("maior = %d (%f)\n", k+1, distM[x][k]);
                }
            }
        }
        //printf("\n");
        
		// Monta o conjunto RCL com base no conjunto de candidatos
        double limiteRCL = menor + alfa*(maior-menor);
        for(int i = 0; i < conjuntoCandidatos.size(); i++){
            if( distM[x][conjuntoCandidatos[i]] <= limiteRCL ){
                conjuntoRCL.push_back(conjuntoCandidatos[i]);
            }
        }
		
        // Escolhe um elemento aleatorio da RCL
        y = irandom(conjuntoRCL.size()-1);
        y = conjuntoRCL[y];
        myCities.push_back(y + 1);
        visitados[y] = 1;
        //printf("Inseri %d na solucao (custo aresta %f)\n\n", y+1, distM[x][y]);
        x = y;
    }
    return myCities;
}
// *** FIM das função de Heurística Construtiva NEAREST NEIGHBOUR ***

vector<int> buscaLocal (vector<int> currentSolution)
{
    // Solução corrente começa como solução inicial
    double currentSolutionCost = funcaoObjetivo(currentSolution);

    // Realiza 2-OPT até exaustão (mínimo local)
    //bool PARAR_BUSCA;
    bool REPETIR_2OPT = true;
    while (REPETIR_2OPT == true)
    {
        REPETIR_2OPT = false;
        //PARAR_BUSCA = false;
        for(int i = 0; i < nCidades-1; i++)
        {
            for (int j = i + 1; j < nCidades; j ++)
            {
                int aux;
                int x = i;
                int y = j;

                // Retiro duas arestas: [x] com [x-1] (se x==0, é [nCidades-1]); e [y] com [y+1] (se y==nCidades-1, é [0]);
                // Adiciono duas arestas: [y] com x[-1] (se x==0, é [nCidades-1]); e [x] com [y+1] (se y==nCidades-1, é [0]);

                // Definindo limites circular
                int ladoEsquerdoX, ladoDireitoY;
                ladoEsquerdoX = x-1;
                ladoDireitoY = y+1;
                if( ladoEsquerdoX == -1){
                    ladoEsquerdoX = nCidades-1;
                }
                if( ladoDireitoY == nCidades){
                    ladoDireitoY = 0;
                }

                if( y == ladoEsquerdoX || x == ladoDireitoY ) continue; // Não pode adicioanr arestas (x,x)

                // Retira (X, ladoEsquerdoX), (Y, ladoDireitoY). Add (Y, ladoEsquerdoX), (X, ladoDireitoY).
                double diff = (distM[ currentSolution[y]-1 ][ currentSolution[ladoEsquerdoX]-1 ]) + (distM[ currentSolution[x]-1 ][ currentSolution[ladoDireitoY]-1 ]) -(distM[ currentSolution[x]-1 ][ currentSolution[ladoEsquerdoX]-1 ]) - (distM[currentSolution[y]-1 ][ currentSolution[ladoDireitoY]-1] );
                //printf("x=%d, y=%d\n", x, y);
                //printf("aresta n indices: aresta(%d,%d) + aresta(%d,%d) - aresta(%d,%d) - aresta(%d,%d) = %f\n", currentSolution[y], currentSolution[ladoEsquerdoX], currentSolution[x], currentSolution[ladoDireitoY], currentSolution[x], currentSolution[ladoEsquerdoX], currentSolution[y], currentSolution[ladoDireitoY], diff);
                if( diff < 0)
                {
                    //printf("De fato antes = %f\nSol = ", funcaoObjetivo(currentSolution));
                    //for(int i = 0; i < nCidades; i++){
                    //    printf("%d, ", currentSolution[i]);
                   // }
                    //printf("\n");
                    while (x < y)
                    {
                        aux = currentSolution[x];
                        currentSolution[x] = currentSolution[y];
                        currentSolution[y] = aux;
                        x ++;
                        y --;
                    }
                    //PARAR_BUSCA = true;
                    REPETIR_2OPT = true;
                    //printf("Antes = %f, Diff = %f, Soma = %f\n", currentSolutionCost, diff, currentSolutionCost+diff);
                    currentSolutionCost = currentSolutionCost + diff;
                    //printf("De fato = %f\n", funcaoObjetivo(currentSolution));
                    //getchar();
                }
            }
        }
    }
    return currentSolution;
}

// Gerador de número pseudo-aleatório entre [0, a]
int irandom( int a)
{
    if(a <= 0) return 0;
    else return rand() % (a + 1);
}

double funcaoObjetivo (vector<int> solucao)
{
    double custo = 0.0;
    for(int i = 1; i < solucao.size(); i++)
    {
        custo += distM[solucao[i]-1][solucao[i-1]-1];
    }
    custo += distM[solucao[solucao.size()-1]-1][solucao[0]-1];
    return custo;
}

void readFile(char* filePath)
{
    ifstream input(filePath);
    bool nodeSection = false;
    vector<int> cityX, cityY;
    if (input.is_open())
    {
        for( string line; getline( input, line ); )
        {
            string dimension = "DIMENSION :";
            size_t found = line.find(dimension);
            if( found != string::npos)
            {
                size_t after = found + dimension.size();
                nCidades = atoi(line.substr(after).c_str());
                //cout << "Numero de Cidades: " << nCidades << endl;
            }
            found = line.find("NODE_COORD_SECTION");
            if(found != string::npos)
            {
                nodeSection = true;
            }
            else if (nodeSection)
            {
                int id, x, y;
                if(sscanf(line.c_str(), " %d %d %d", &id, &x, &y) == 3)
                {
                    //cout << "Lido " << id << " " << x << " " << y << endl;
                    cityX.push_back(x);
                    cityY.push_back(y);
                    for(int i =0; i < cityX.size(); i++)
                    {
                        double dX = x - cityX[i];
                        double dY = y - cityY[i];
                        double dist = pow(dX, 2) + pow(dY, 2);
                        dist = floor(sqrt(dist) + 0.5);
                        //cout << "Distancia entre " << id << " " << i + 1 << " " << dist << endl;
                        distM[id-1][i] = distM[i][id-1] = dist;
                    }
                }
            }
        }
    }
    else
    {
        cout << "Erro ao abrir o arquivo: " << filePath << endl;
    }
}
