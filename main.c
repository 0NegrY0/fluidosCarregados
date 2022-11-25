/*
Programmer: Carlos Negri (UFRGS - 00333174)
Language: C/C++
Subject: Física(IC)
main.c data.c data.h Makefile
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "data.h"

//PARTICULAS
const int PARTICULA_MAX_MAIN = 100;

int main(){

    int i, pos;
    int colocados = 0;

    //INICIACAO DAS PARTICULAS

    PARTICULA particulas[PARTICULA_MAX_MAIN];

    for(i = 0; i < PARTICULA_MAX_MAIN; i++){
        particulas[i].raio = 0.05;
        particulas[i].carga = 0;
        particulas[i].vector.x = 0;
        particulas[i].vector.y = 0;
        particulas[i].vector.z = 0;
        particulas[i].vel.x = 0;
        particulas[i].vel.y = 0;
        particulas[i].vel.z = 0;
        particulas[i].lennard.x = 0;
        particulas[i].lennard.y = 0;
        particulas[i].lennard.z = 0;
        particulas[i].id = i;

    }
    
    
    for(i = 0; i < PARTICULA_MAX_MAIN; i++){
        srand(time(NULL));      //seed

        colocados = colocaParticula(pos, particulas, colocados);
    }


//    for(i = 0; i < PARTICULA_MAX_MAIN; i++){                                                                                              //DEBUG
//        printf("MAIN\n particula %d    X: %lf      Y: %lf      Z: %lf      Carga: %d       Raio: %lf\n", 
//        i, particulas[i].vector.x, particulas[i].vector.y, particulas[i].vector.z, particulas[i].carga, particulas[i].raio);
//    }
    //calculaLennardJhonson(particulas);

    imprimeArquivo(particulas, colocados);

    movement(particulas);

    return 0;
}
