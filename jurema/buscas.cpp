#include "global.h"
#include <stdio.h>
#include <string.h>
#include <ctime>
#include "buscas.h"

/*
    Expansao DFS
*/
void branch( int k ){

	int register i ;

	activeSet[k] = TRUE;
	nivel++;
    ciclo[nivel] = k;
	if(nivel != dimension){
           for( i = ONE ; i <= dimension ; i++ ){

		   		if (activeSet[i] == FALSE) {

		      		if( custo + lbound(k,i)< limiteSuperior ) {

						 custo += lbound(k,i);
						 ++qtdNodosExpandidos;
			             branch( i );
			             custo -= lbound(k,i);
                    }

		        }
           }
	} else if (k != INICIO && (custo + lbound(k,INICIO)) < limiteSuperior) {
            ++qtdFolhas;
            ++qtdNodosExpandidos;
            printf("\nFolha de numero %d encontrada: de %d para %d, em %d ms.\n", qtdFolhas, limiteSuperior, custo + lbound(k, INICIO),(int)((clock()-tempoInicial))/a);
           // printf("\n\nFolha de numero %d encontrada: de %d para %d em %.3f.\n", qtdFolhas, limiteSuperior, custo + lbound(k, INICIO), (double)(time(NULL)-tempoInicial));
            limiteSuperior = custo + lbound(k,INICIO);
            /*Ciclo formado*/
			ciclo[nivel] = k;
			ciclo[proximo(nivel)] = INICIO;
            printf("TOUR: ");
			for(int j = 1; j<=nivel+1; ++j){
                printf(" %d ", ciclo[j]);
            }

        }
	activeSet[k] = FALSE;
	nivel--;
	return;
}//dfs

/*
    Expansao DFS para jurema de juremas
*/
void branch_jj( int k ){

	int register i ;

	activeSet[k] = TRUE;
	nivel++;
    ciclo[nivel] = k;
	if(nivel != dimension){ //modifiquei aqui
           for( i = ONE ; i <= dimension ; i++ ){

		   		if (activeSet[i] == FALSE && (k != i)) {

		      		if( custo + lbound(k,i)< limiteSuperior ) {

						 custo += lbound(k,i);
						 ++qtdNodosExpandidos;
			             branch( i );
			             custo -= lbound(k,i);
                    }

		        }
           }
	} else if (k != INICIO && (custo + lbound(k,INICIO)) < limiteSuperior) {
            ++qtdFolhas;
          // printf("\nFolha de numero %d encontrada: de %d para %d, em %.3f'.\n", qtdFolhas, limiteSuperior, custo + lbound(k, INICIO),((clock()/a)-tempoInicial)/MINUTO);
           // printf("\n\nFolha de numero %d encontrada: de %d para %d em %.3f.\n", qtdFolhas, limiteSuperior, custo + lbound(k, INICIO), (double)(time(NULL)-tempoInicial));
            limiteSuperior = custo + lbound(k,INICIO);
            /*Ciclo formado*/
			ciclo[nivel] = k;
			ciclo[proximo(nivel)] = INICIO;
            printf("TOUR: ");
			for(int j = 1; j<=nivel+1; ++j){
                printf(" %d ", ciclo[j]);
            }

        }
	activeSet[k] = FALSE;
	nivel--;
	return;
}


/*
    Mergulha diretamente para uma solucao e retorna ao encontra-la.
*/

void dive( int k ){

	int register i ;

	diving_flag = false;

	activeSet[k] = TRUE;
	nivel++;
    ciclo[nivel] = k;

   // printf("\n\nDIVING from %d!", k);

	if(nivel != dimension){
           for( i = ONE ; i <= dimension ; i++ ){

                if( diving_flag== true){
                    activeSet[k] = FALSE;
                    nivel--;
                    return;
                }
		   		if (activeSet[i] == FALSE && (k != i)) {

		      		if( custo+lbound(k,i) < limiteSuperior ) {
						 custo += lbound(k,i);
						 ++qtdNodosExpandidos;
			             dive( i );
			             custo -= lbound(k,i);
                    }
		        }
           }
	} else if (k != INICIO && (custo + lbound(k,INICIO)) < limiteSuperior){

            diving_flag = true;
            ++qtdFolhas;

            printf("\n\nFolha de numero %d encontrada via DIVING: de %d para %d em %.3f.\n", qtdFolhas, limiteSuperior, custo + lbound(k, INICIO), (double)(time(NULL)-tempoInicial));

            limiteSuperior = custo + lbound(k,INICIO); //PROBLEMA

            /*Ciclo formado*/
			ciclo[nivel] = k;
			ciclo[proximo(nivel)] = INICIO;

			memcpy(solucaoGuia, ciclo, sizeof(solucaoGuia));

            printf("TOUR: ");
			for(int j = 1; j<=nivel+1; ++j){
                printf(" %d ", ciclo[j]);
            }
        }
	activeSet[k] = FALSE;
	nivel--;
	return;
}
