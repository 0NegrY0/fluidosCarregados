#include "data.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
const double FRIC = 0.1;
const double DT = 0.005;
const double TF = 53.0;

const unsigned int PARTICULA_MAX_MOV = 100;
const double LX = 30.0;                                 //USANDO TAMANHO DA CAIXA ECONOMICA FEDERAL
const double LY = 30.0;
const double LZ = 30.0;
const int CHECKPOINT = 10;


void movement(PARTICULA particulas[]){

    double Xold, Yold, Zold;
    double Con, Co, Cone, Ctwo;
    double desv_r;
    double desv_v;
    double Cvv, CvvTwo;
    
    double DT2 = pow(DT, 2);

    unsigned int i;

    VECTOR old;
    VECTOR Fold[PARTICULA_MAX_MOV];             //NAO USADO
    
    FILE *arq;

    char nome[25];

    int numArq = 0;
    int iteracao = 0;

	
    //QUERO NEM OLHAR PRA ESSA BAGUNÇA
    Con = FRIC * DT;
    Co = exp(-(Con));
    Cone = ((1 - Co) / Con);
    Ctwo = ((1 - Cone) / Con);
    desv_r = DT*sqrt((1/Con) * (2 - ((1/Con) * (3 - (4*Co) + (Co*Co)))));

    desv_v = sqrt(1 - pow(Co, 2));
    Cvv = (DT / Con / desv_v / desv_r) * (1 - Co) * (1 - Co);
    CvvTwo = sqrt(1 - pow(Cvv, 2));

    /*for(i = 0; i < PARTICULA_MAX_MOV; i++ ){
        printf("%d X:%lf y:%lf z:%lf\n", i, particulas[i].vector.x, particulas[i].vector.y, particulas[i].vector.z);
    }*/                 //ESTA CHEGANDO ATE AKI


    for(double t = 0; t <= TF; t += DT){

        calculaLennardJhonson(particulas);

        /*for(i = 0; i < PARTICULA_MAX_MOV; i++ ){
            printf("PRIMEIRA %d X:%lf y:%lf z:%lf\n", i, particulas[i].lennard.x, particulas[i].lennard.y, particulas[i].lennard.z);
        }*/             

        //PRIMEIRO LOOPZIN ESPACO
        for(i = 0; i < PARTICULA_MAX_MOV; ++i){
            old.x = particulas[i].vector.x;
            old.y = particulas[i].vector.y;
            old.z = particulas[i].vector.z;

            particulas[i].gaussian.x = gausran();             
            particulas[i].gaussian.y = gausran();
            particulas[i].gaussian.z = gausran();

            //PROBLEMA NO GAUSSRAN

            particulas[i].vector.x += Cone * DT * particulas[i].vel.x + Ctwo * DT2 * particulas[i].lennard.x + particulas[i].gaussian.x * desv_r;              //INICIAR F    era Fold no lugar de lennard
            particulas[i].vector.y += Cone * DT * particulas[i].vel.y + Ctwo * DT2 * particulas[i].lennard.y + particulas[i].gaussian.y * desv_r;              //INICIAR F
            particulas[i].vector.z += Cone * DT * particulas[i].vel.z + Ctwo * DT2 * particulas[i].lennard.z + particulas[i].gaussian.z * desv_r;              //INICIAR F


            /*if(iteracao == 0){
                int j = 0;
                for(j = 0; j < PARTICULA_MAX_MOV; j++ ){
                    printf("SEGUNDA %d X:%lf y:%lf z:%lf\n", i, particulas[i].vector.x, particulas[i].vector.y, particulas[i].vector.z);
                }              
            }*/                 //RETORNA UM VALOR MUITO ALTO


            if(fabs(particulas[i].vector.x) > LX/2.0){
                particulas[i].vector.x = particulas[i].vector.x - LX * (fabs(particulas[i].vector.x) / particulas[i].vector.x);
            }       
            if(fabs(particulas[i].vector.y) > LY/2.0){
                particulas[i].vector.y = particulas[i].vector.y - LY * (fabs(particulas[i].vector.y) / particulas[i].vector.y);
            }
            if(fabs(particulas[i].vector.z) > LZ/2.0){
                particulas[i].vector.z = particulas[i].vector.z - LZ * (fabs(particulas[i].vector.z) / particulas[i].vector.z);
            }
        }


        calculaLennardJhonson(particulas);

        /*for(i = 0; i < PARTICULA_MAX_MOV; i++ ){
            printf("SEGUNDA %d X:%lf y:%lf z:%lf\n", i, particulas[i].lennard.x, particulas[i].lennard.y, particulas[i].lennard.z);
        }*/              
        

        for(i = 0; i < PARTICULA_MAX_MOV; ++i){
            particulas[i].vel.x = Co * particulas[i].vel.x + (Cone - Ctwo) * DT * particulas[i].lennard.x + Ctwo * DT * particulas[i].lennard.x + desv_v * (Cvv * particulas[i].gaussian.x + CvvTwo * gausran());                 //era Fold no lugar do primeiro lennard
            particulas[i].vel.y = Co * particulas[i].vel.y + (Cone - Ctwo) * DT * particulas[i].lennard.y + Ctwo * DT * particulas[i].lennard.y + desv_v * (Cvv * particulas[i].gaussian.y + CvvTwo * gausran());
            particulas[i].vel.z = Co * particulas[i].vel.z + (Cone - Ctwo) * DT * particulas[i].lennard.z + Ctwo * DT * particulas[i].lennard.z + desv_v * (Cvv * particulas[i].gaussian.z + CvvTwo * gausran());

        //    Fold[i].x = particulas[i].lennard.x;
        //    Fold[i].y = particulas[i].lennard.y;
        //    Fold[i].z = particulas[i].lennard.z;

        }
    

        //BINARIO
        /*if(iteracao == 100){
            sprintf(nome, "mov%d.bin",numArq);
            arq = fopen(nome, "wb");
            fwrite(particulas, sizeof(PARTICULA), PARTICULA_MAX_MOV, arq);
            fclose(arq);
            numArq++;
            iteracao = 0;
            
            for(i = 0; i < PARTICULA_MAX_MOV; i++){                                                                                              //DEBUG
            printf("particula %d    X: %lf      Y: %lf      Z: %lf      Carga: %d       Raio: %lf\n", 
                i, particulas[i].vector.x, particulas[i].vector.y, particulas[i].vector.z, particulas[i].carga, particulas[i].raio);
            }
        }

        else {
            iteracao++;
        }*/

        //ANIMACAO

        if(iteracao >= 10000){
            sprintf(nome, "mov_anim%d.txt",numArq);
            arq = fopen(nome, "w");
            fprintf(arq, "%d\n\n", PARTICULA_MAX_MOV);

            for(i = 0; i < PARTICULA_MAX_MOV; i++){
                if(i < PARTICULA_MAX_MOV/2){
                    fprintf(arq, "Na ");
                }

                else{
                    fprintf(arq, "Cl ");
                }

                fprintf(arq, "%lf %lf %lf\n", particulas[i].vector.x, particulas[i].vector.y, particulas[i].vector.z);
            }

            fprintf(arq, "\n");
            fclose(arq);
            numArq++;
            iteracao = 9990;
        }
        else{
            iteracao++;
        }
    }
}