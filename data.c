/*
Programmer: Carlos Negri (UFRGS - 00333174)
Language: C/C++
Subject: Física(IC)
main.c data.c data.h Makefile
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "data.h"

//TAMANHO DA CAIXA
const double caixaX_MAX = 15.00;
const double caixaX_MIN = -15.00;
const double caixaY_MAX = 15.00;
const double caixaY_MIN = -15.00;
const double caixaZ_MAX = 15.00;
const double caixaZ_MIN = -15.00;

//PARTICULAS
const unsigned int PARTICULA_MAX = 100;
const double PARTICULA_RAIO_MAXIMO = 2.00;

//Fiz separadinha pq tava dando problema, da pra ficar menor *-*
double numeroAleatorio(double min, double max){
    double random = ((double) rand()) / RAND_MAX;    //0 - 1

    double range = (max - min) * random;            //0 - (max - min)

    double number = min + range;                    //min - max

    return number;
}


int colocaParticula(int pos, PARTICULA particulas[], int colocados){
    unsigned int sucess = 0;
    int i;
    double x, y, z;

    do{
        x = numeroAleatorio(caixaX_MIN + PARTICULA_RAIO_MAXIMO, caixaX_MAX - PARTICULA_RAIO_MAXIMO);
        y = numeroAleatorio(caixaY_MIN + PARTICULA_RAIO_MAXIMO, caixaY_MAX - PARTICULA_RAIO_MAXIMO);
        z = numeroAleatorio(caixaZ_MIN + PARTICULA_RAIO_MAXIMO, caixaZ_MAX - PARTICULA_RAIO_MAXIMO);


        if(colocados == 0){
            sucess = 1;
        }

        else{
            for(i = 0; i < colocados; i++){
                if(sqrt(pow(particulas[i].vector.x - x, 2) + pow(particulas[i].vector.y - y ,2) + pow(particulas[i].vector.z - z , 2)) > 2 * PARTICULA_RAIO_MAXIMO){
                    sucess = 1;
//                    printf("sucesso em %d (colocados = %d)\n", i, colocados);                                                 //DEBUG 
                }
                else {
                    sucess = 0;
//                    printf("Falha em %d (colocados = %d)\n", i, colocados);                                                   //DEBUG
                    break;
                }
            }
        }

        if(sucess == 1){
            particulas[colocados].vector.x = x;
            particulas[colocados].vector.y = y;
            particulas[colocados].vector.z = z;
        }
        
    }while (sucess == 0);

    sucess = 0;
    colocados++;

    return colocados;

}


int imprimeArquivo(PARTICULA particulas[], int colocados){
    FILE *arq;
    unsigned int i;

    arq = fopen("saida.txt", "w");
    
    if(arq == NULL){
        return 1;               //erro na abertura (ERRO 1)
    }
    
    else{
        fprintf(arq, "%d", colocados);
        fprintf(arq, "%s", "\n\n");

        for(i = 0; i < round(colocados/2.0); i++){
            fprintf(arq, "%s %lf %lf %lf %s", "Na", particulas[i].vector.x, particulas[i].vector.y, particulas[i].vector.z, "\n");
        }
        for(i = round(colocados/2.0); i<colocados; i++){
            fprintf(arq, "%s %lf %lf %lf %s", "Cl", particulas[i].vector.x, particulas[i].vector.y, particulas[i].vector.z, "\n");
        }
        return 0;
    }
}


void calculaLennardJhonson(PARTICULA particulas[]){                 //TEM ERRO AQUI

    VECTOR d;
    VECTOR f;             
    double cutwca = pow(2.0, 1.0/6.0);
    unsigned int i, j;
    unsigned int set = 0;
    double dist;
    double cfive;
    double Lx = 2 * caixaX_MAX;            //USANDO LX COMO TAMANHO DA CAIXA
    double Ly = 2 * caixaY_MAX;
    double Lz = 2 * caixaZ_MAX;
    
    for(i = 0; i< PARTICULA_MAX; i++){
        particulas[i].lennard.x = 0.0;
        particulas[i].lennard.y = 0.0;
        particulas[i].lennard.z = 0.0;
    }



    for(i = 0; i < PARTICULA_MAX; ++i){
        for(j = i + 1; j < PARTICULA_MAX; ++j){
            d.x = particulas[j].vector.x - particulas[i].vector.x; 
            d.x = d.x - round(d.x/Lx) * Lx;                                   

            d.y = particulas[j].vector.y - particulas[i].vector.y; 
            d.y = d.y - round(d.y/Ly) * Ly;                                   

            d.z = particulas[j].vector.z - particulas[i].vector.z; 
            d.z = d.z - round(d.z/Lz) * Lz;                                   


            dist = sqrt(d.x*d.x + d.y*d.y + d.z*d.z);   //FUNCIONANDO

            //printf("%lf\n", dist);                                          //debug

            //LENNARD
            cfive = 0.0;
            if(dist < cutwca){
                cfive = 5.0 * (-6.0 / pow(dist, 7) + 12.0 / pow(dist, 13));

                if(dist <= 0.8){
                    cfive = 5.0 * (-6.0 / pow(0.8, 7) + 12.0 / pow(0.8, 13));
                }
            }
            
            f.x = -cfive*d.x/dist;
            f.y = -cfive*d.y/dist;
            f.z = -cfive*d.z/dist;


            particulas[i].lennard.x += f.x;            
            particulas[i].lennard.y += f.y;
            particulas[i].lennard.z += f.z;
            
            
            particulas[j].lennard.x += -f.x;       
            particulas[j].lennard.y += -f.y;
            particulas[j].lennard.z += -f.z; 
            
        }
    }


    for(i = 0; i < PARTICULA_MAX; i++ ){
        printf("%d X:%lf y:%lf z:%lf\n", i, particulas[i].lennard.x, particulas[i].lennard.y, particulas[i].lennard.z);
    }
}


// generate gaussian random_numbers
double gausran()
{                                                                                                                                                                           
    int g;
    double ran1,ran2,PI,R1,R2,res;
    ran1 = (double)rand()/RAND_MAX;
    ran2 = (double)rand()/RAND_MAX;
    PI   = 4.0*atan(1.0);
    R1   = -log(1.0-ran1);
    R2   = 2.0*PI*ran2;
    R1   = sqrt(2.0*R1);
    res  = R1*cos(R2);
    return res;
}