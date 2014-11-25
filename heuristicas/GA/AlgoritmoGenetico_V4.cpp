#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <ctime>
#include <algorithm>

#define MAX_CITIES 440
#define POP 20

/* V4: Codificação por permutação, Seleção USS com elitismo, Mutação por inversão de intervalo.
   Taxa de mutação ajustada: 25%
   População ajustada: 20
*/

//#define DEBUG

#ifdef DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

using namespace std;

// Funções para Números Pseudo-Aleatórios
double unifRand(); // Retorna um número entre [0, 1]
int irandom(int a); // Retorna um número entre [0, a]

// Funções de Algoritmos Genéticos
double fitness (int solution[MAX_CITIES]);
void selectCrossover (vector<int> &selectedChromosomes, const int quant);
int binarySearchSpecial (double arr[], double ele);
void crossover (int pai1, int pai2, int offspring[2][MAX_CITIES]);
void mutation (int candidate[MAX_CITIES]);
void selectElitist(int elite[2]);

// Outras Funções
void inicializa_populacao();
void readFile(char* filePath);
double objective_function(int solution[MAX_CITIES]);
void evaluateSolution (int candidate[MAX_CITIES], double& objValue);
void copySolution (int dest[MAX_CITIES], int src[MAX_CITIES]);

typedef struct population {
  int data[POP][MAX_CITIES];
  double fitness[POP];
  double obj_value[POP];
} Population;

// Dados Globais
int nCities;
double distM[MAX_CITIES][MAX_CITIES];

Population pop1, pop2;
Population* current_population;

// Melhor solução (saída)
int bestSolution[MAX_CITIES];
double bestCost = INFINITY;

int main(void){
    // Inicializa dados do programa
    if( POP % 2 != 0){
        printf("***ERRO***: Quantidade de populacao precisa ser par.");
    }
    readFile("./instancias/pr439.tsp");
    std::srand ( unsigned ( std::time(0) ) );
    current_population = &pop1;

    // Inicialização de população
    inicializa_populacao();

    /*
    PRINTF("Database inicial:\n");
    for(int i =0; i < POP; i++){
        for(int j =0; j < nCities; j++){
        PRINTF("%d ", current_population->data[i][j]);
        }
        PRINTF("\n");
    }
    PRINTF("some fitness: %f e %f. some costs: %f e %f\n", fitness(current_population->data[0]), fitness(current_population->data[1]), objective_function(current_population->data[0]), objective_function(current_population->data[1]));
    */

    // Execução do Algoritmo Genético
    clock_t begin = clock();
    double elapsed_secs = 0.0;
    int k = 0;
    //while(k < 1000000){
    while(k < 4000000 && elapsed_secs < 600){ // Critério de parada: 4 milhões de gerações OU 10 minutos (o que vier antes).
        // Crossover Selection
        vector<int> crossoverSelected;
        const int nElitista = 2;
        const int quantPais = POP - nElitista;
        selectCrossover(crossoverSelected, quantPais);

        // Crossover Parents Randomization
        random_shuffle ( crossoverSelected.begin(), crossoverSelected.end() );

        // Crossover
        for(int p = 0; p < quantPais; p += 2){
            int pai1 = crossoverSelected[p], pai2 = crossoverSelected[p+1];
            if( current_population == &pop1)
                crossover(pai1, pai2, &pop2.data[p]);
            else
                crossover(pai1, pai2, &pop1.data[p]);
        }

        // Mutação
        for(int p = 0; p < quantPais; p++){
            if( unifRand() <= 0.25){ // 25% de chance de cada cromossomo sofrer mutação.
                if( current_population == &pop1){
                    mutation(pop2.data[p]);
                }
                else{
                    mutation(pop1.data[p]);
                }
            }
        }
        // A 'Taxa de Mutação' esperada portanto será de no máximo 25%, provavelmente menos de 25%/2 = 12,5%

        // Seleção por Elitismo (salva os 2 melhores da população atual)
        int elite[2];
        selectElitist(elite);
        if(current_population == &pop1){
            //printf("Elite encontrada: [%d] e [%d]. Valores: %f e %f", elite[0], elite[1], pop1.obj_value[elite[0]], pop1.obj_value[elite[1]]);
            copySolution(pop2.data[quantPais],   pop1.data[elite[0]]);
            copySolution(pop2.data[quantPais+1], pop1.data[elite[1]]);
        }
        else{
            //printf("*Elite encontrada: [%d] e [%d]. Valores: %f e %f", elite[0], elite[1], pop2.obj_value[elite[0]], pop2.obj_value[elite[1]]);
            copySolution(pop1.data[quantPais],   pop2.data[elite[0]]);
            copySolution(pop1.data[quantPais+1], pop2.data[elite[1]]);
        }

        // Seleção da população (serão os filhos recém-gerados e mais os 2 melhores por elitismo)
        if(current_population == &pop1)
            current_population = &pop2;
        else
            current_population = &pop1;

        // Avalia as soluções da nova geração
        for(int p =0; p < POP; p++){
            evaluateSolution(current_population->data[p], current_population->obj_value[p]);
        }

        clock_t end = clock();
        elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        k++;
    }

    printf("** Algoritmo finalizado com solucao de custo = %f . Numero de Geracoes: %d **\nSol = ", bestCost, k);
    for(int s=0; s < nCities; s++){
        printf("%d ", bestSolution[s]);
    }
    return 0;
}

int irandom( int a){
    if(a <= 0) return 0;
    else return rand() % (a + 1); // Gera um número entre [0, a]
}

// Generate a random number between 0 and 1
// return a uniform number in [0,1].
double unifRand()
{
    return rand() / double(RAND_MAX);
}

// Retorna o fitness, só que ao contrário! (As melhores soluções retornarão os valores mais baixos nessa função)
double fitness (int solution[MAX_CITIES]){
    double custo = 0.0;
    for(int i = 1; i < nCities; i++){
        custo += distM[solution[i]-1][solution[i-1]-1];
    }
    custo += distM[solution[nCities-1]-1][solution[0]-1];
    return 1024/custo; // Evitar degenerência numérica em grandes instâncias !
}

double objective_function(int solution[MAX_CITIES]){
    double custo = 0.0;
    for(int i = 1; i < nCities; i++){
        custo += distM[solution[i]-1][solution[i-1]-1];
    }
    custo += distM[solution[nCities-1]-1][solution[0]-1];
    return custo;
}

void mutation (int candidate[MAX_CITIES]){
    int r1, r2;
    r1 = irandom(nCities-1);
    r2 = irandom(nCities-1);
    PRINTF("Trocando os valores de r1 e r2 (%d e %d)\n", r1, r2);
    PRINTF("antes: ");
    for(int q = 0; q < nCities; q++) PRINTF("%d ", candidate[q]);

    //int aux = candidate[r1];
    //candidate[r1] = candidate[r2];
    //candidate[r2] = aux;

    //Mutação diferente: Inverte todos entre os dois índices r1 e r2
    if(r1 > r2){
        int aux = r2;
        r2 = r1;
        r1 = aux;
    }
    while(r1 < r2){
        int aux = candidate[r1];
        candidate[r1] = candidate[r2];
        candidate[r2] = aux;
        r1++;
        r2--;
    }

    PRINTF("\ndepois: ");
    for(int q = 0; q < nCities; q++) PRINTF("%d ", candidate[q]);
    PRINTF("\n");
    PRINTF("\n");
    return;
}

void crossover (int pai1, int pai2, int offspring[2][MAX_CITIES]){
    // "Mantém" registro de cidades já colocadas na solução filho
    bool cidade_adicionada_off1[MAX_CITIES+1], cidade_adicionada_off2[MAX_CITIES+1];

    for(int i = 0; i < nCities+1; i++){
        cidade_adicionada_off1[i] = cidade_adicionada_off2[i] = false;
    }

    PRINTF("\nFazendo crossover dos pais %d e %d.\n", pai1, pai2);
    PRINTF("pai1: ");
    for(int q = 0; q < nCities; q++) PRINTF("%d ", current_population->data[pai1][q]);
    PRINTF("\npai2: ");
    for(int q = 0; q < nCities; q++) PRINTF("%d ", current_population->data[pai2][q]);

    // Operador de crossover (C1)
    int ponto = irandom(nCities-1);
    PRINTF("irandom: %d\n", ponto);
    for(int q = 0; q <= ponto; q++){
        offspring[0][q] = current_population->data[pai1][q];
        cidade_adicionada_off1[ offspring[0][q] ] = true;
        offspring[1][q] = current_population->data[pai2][q];
        cidade_adicionada_off2[ offspring[1][q] ] = true;
    }

    int q = ponto+1;
    for(int i = 0; i < nCities; i++){
        if( cidade_adicionada_off1[ current_population->data[pai2][i] ] == false){
            offspring[0][q] = current_population->data[pai2][i];
            q++;
        }
    }

    q = ponto+1;
    for(int i = 0; i < nCities; i++){
        if( cidade_adicionada_off2[ current_population->data[pai1][i] ] == false){
            offspring[1][q] = current_population->data[pai1][i];
            q++;
        }
    }

    PRINTF("off1: ");
    for(int q = 0; q < nCities; q++) PRINTF("%d ", offspring[0][q]);
    PRINTF("\noff2: ");
    for(int q = 0; q < nCities; q++) PRINTF("%d ", offspring[1][q]);
}

// Função especial para descobrir qual é o cromossomo correspondente da região da roleta em que um dado pino parou.
// Delimitações das regiões dos cromossomos = arr[], pino = ele.
int binarySearchSpecial (double arr[], double ele){
    int first, last, middle;
    first = 0;
    last = POP - 1;
    middle = (first+last)/2;

    while( first <= last )
    {
        if( ele >= arr[middle] && (middle >= (POP-1) || ele <= arr[middle+1])){
            return middle;
        }
        else if ( (middle <= 0 || ele >= arr[middle-1]) && ele <= arr[middle] ){
            if(middle <= 0) return 0;
            else return middle-1;
        }
        else if(arr[middle] < ele){
            first = middle + 1;
        }
        else
            last = middle - 1;

        middle = (first + last)/2;
    }
    if ( first > last )
        PRINTF("Not found! %d is not present in the list.\n", ele);
}

void selectCrossover (vector<int> &parentChromosomes, const int quant){
    if( quant % 2 != 0){
        printf("***ERRO***: 'quant' Para crossover deve ser PAR!");
        exit(1);
    }
    double totalFitness = 0.0;
    double arrayFitnesses[MAX_CITIES];
    for (int i=0; i < POP; i++){
            arrayFitnesses[i] = totalFitness;
            totalFitness += fitness(current_population->data[i]);
    }
    PRINTF("Valores na roulette:\n");
    for (int i=0; i < POP; i++){
      PRINTF("%d - %f - %f\n", i, arrayFitnesses[i], objective_function(current_population->data[i]));
    }
    PRINTF("Total fitness: %f (%d solucoes)\n\n", totalFitness, POP);

    // Universal Stochastic Selector
    double random = unifRand() * totalFitness; // Sorteia um ponto
    double distPontos = totalFitness/POP;
    for (int i=0; i < quant; ++i){
        int willReproduce;
        // Coloca o indivíduo do correspondente ponto e "move o ponto na roleta"
        willReproduce = binarySearchSpecial (arrayFitnesses, random);
        PRINTF("Procurando por %f e escolhida posicao %d.\n", random, willReproduce);
        parentChromosomes.push_back(willReproduce);
        if( random+distPontos < totalFitness )
            random = random + distPontos;
        else
            random = (random+distPontos)-totalFitness;
    }
}

void selectElitist(int elite[2]){
    int    bestIdx = 0, bestIdx2 = 1;
    double bestVal = INFINITY, bestVal2 = INFINITY;
    for(int i=0; i < POP; i++){
        if( current_population->obj_value[i] < bestVal2 ){
            bestVal2 = current_population->obj_value[i];
            bestIdx2 = i;
            if( bestVal2 < bestVal){
                int aux;
                double auxVal;
                aux = bestIdx;
                auxVal = bestVal;
                bestIdx = bestIdx2;
                bestVal = bestVal2;
                bestIdx2 = aux;
                bestVal2 = auxVal;
            }
        }
    }
    elite[0] = bestIdx;
    elite[1] = bestIdx2;
    return;
}

void copySolution (int dest[MAX_CITIES], int src[MAX_CITIES]){
    for(int q = 0; q < nCities; q++){
        dest[q] = src[q];
    }
}

void evaluateSolution (int candidate[MAX_CITIES], double& objValue){
    // Testa melhor indivíduo (até agora)
    objValue = objective_function(candidate);
    if( objValue < bestCost){
        bestCost = objValue;
        for(int q = 0; q < nCities; q++){
            bestSolution[q] = candidate[q];
        }
        PRINTF("** Uma solucao melhor foi encontrada com custo = %f **\nSol = ", bestCost);
        for(int s=0; s < nCities; s++){
            PRINTF("%d ", bestSolution[s]);
        }
        PRINTF("\n");
    }
}

void inicializa_populacao(){
    vector<int> myCities;
    for (int o=1; o <= nCities; ++o)
        myCities.push_back(o);

    for(int i =0; i < POP; i++){
        // Permutação de cidades
        random_shuffle ( myCities.begin(), myCities.end() );

        // Transfere indivíduo gerado para a database
        int j = 0;
        for (std::vector<int>::iterator it=myCities.begin(); it!=myCities.end(); ++it){
            current_population->data[i][j] = *it;
            j++;
        }

        // Testa melhor indivíduo (até agora)
        evaluateSolution(current_population->data[i], current_population->obj_value[i]);
    }
}

void readFile(char* filePath){
    ifstream input( filePath );
    bool nodeSection = false;
    vector<int> cityX, cityY;
    if (input.is_open()){
        for( string line; getline( input, line ); ){
            string dimension = "DIMENSION :";
            size_t found = line.find(dimension);
            if( found != string::npos){
                size_t after = found + dimension.size();
                nCities = atoi(line.substr(after).c_str());
                //PRINTF("\n*atoi(%s)*\n", line.substr(after).c_str());
                PRINTF("\nNumero de CIDADES: %d\n", nCities);
            }
            found = line.find("NODE_COORD_SECTION");
            if( found != string::npos){
                nodeSection = true;
            }
            else if (nodeSection){
                int id, x, y;
                if( sscanf(line.c_str(), " %d %d %d", &id, &x, &y) == 3){
                    PRINTF("Lido: %d %d %d\n", id, x, y);
                    cityX.push_back(x);
                    cityY.push_back(y);
                    for(int i =0; i < cityX.size(); i++){
                        double dX = x - cityX[i];
                        double dY = y - cityY[i];
                        double dist = pow(dX, 2) + pow(dY, 2);
                        dist = floor(sqrt(dist) + 0.5);
                        PRINTF("Distancia entre %d e %d = %f\n", id, i+1, dist);
                        distM[id-1][i] = distM[i][id-1] = dist;
                    }
                }
            }
        }
    }
    else{
        cout << "Erro ao abrir o arquivo: " << filePath << endl;
    }
}
