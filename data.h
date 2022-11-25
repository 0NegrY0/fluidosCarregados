/*
Programmer: Carlos Negri (UFRGS - 00333174)
Language: C/C++
Subject: Física(IC)
main.c data.c data.h Makefile
*/

typedef struct str_vector{
    double x;
    double y;
    double z;
}VECTOR;

typedef struct str_particula{
    VECTOR vector;
    VECTOR lennard;
    VECTOR vel;
    VECTOR gaussian;
    double raio;
    int carga;
    int id;
}PARTICULA;

double numeroAleatorio(double min, double max);

int colocaParticula(int pos, PARTICULA particulas[], int colocados);

int imprimeArquivo(PARTICULA particulas[], int colocados);

void calculaLennardJhonson(PARTICULA particulas[]);

void movement(PARTICULA particulas[]);

double gausran();