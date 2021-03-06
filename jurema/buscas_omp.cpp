#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <list>
#include <queue>
#include <ctime>

#include "global.h"
#include <omp.h>
#include "buscas_omp.h"
#include "aux.h"

using namespace std;

/*********************************************************/

typedef std::priority_queue<
     int,
     std::vector<int>,
     std::greater<int>
> min_heap_of_ints;

min_heap_of_ints myheap;

/***********************************************************/

/***************************************************************/


class Root{
	//@TODO:\
		sei que que as variaveis nao devem ser public. dps mudarei
	//@TODO: \
		Precisamos criar o destrutor.

  //  private:
    public:
        int flag[MAX+1];
        int nivel;
        int custo;
        int upper_bound;
        int indice;
        int qtd_sol;
        int qtd_nodos;

 //   public:
        Root(int,int);
        void dfs();
        void branch(int);
        //void set_new_ub(int ub);
        //int get_ub();
        //void set_flag(int position);
        //void get_position_value(int position);
};

Root *matriz[MAX+1];





Root::Root(int cost, int index){ /*Construtor*/
    nivel = ::nivel+1;
    custo = cost;
    upper_bound = limiteSuperior;
    indice = index;
    qtd_sol = ZERO;
    qtd_nodos = ZERO;
    memcpy(flag, activeSet, sizeof(activeSet));

    flag[indice] = TRUE;
}

/*
Root::Root(){ verificar a necessidade de um contrutor dessa maneira
              e alocar todos as raizes antes.
}*/
void Root::dfs(){ /*REMOVER DFS() E IR PARA BRANCH?!*/

    for(int i = 2; i<=dimension; ++i){

			//custo+= cost_matrix[indice][i];
            ++qtd_nodos;
			if( (i!=indice && flag[i]!=TRUE)&&(custo+cost(indice,i)<(upper_bound))) {
                custo+=cost(indice,i);
                branch( i );
                custo-= cost(indice,i);
   			 }

	}


}
void Root::branch(int actual_node){

    flag[actual_node] = TRUE;
	nivel++;
	++qtd_nodos;
    if(nivel == dimension){
        if((custo+cost(actual_node,INICIO))<(upper_bound)){
            qtd_sol++;
            upper_bound = custo+cost(actual_node,INICIO);
        }
    }
    else{
        for( int i = 1 ; i <= dimension ; i++ ){

            if ( (flag[i] == FALSE) && (custo + cost(actual_node,i))<(upper_bound)) {
                custo += cost(actual_node,i);
                branch( i );
                custo -= cost(actual_node,i);
            }
        }
	}
	flag[actual_node] = FALSE;
	nivel--;
	return;
}
/*
class Node{
    private:
        int pai;
        int indice;
        int custo;

    public:
        Node();
};*/
/*****************************************************************/
void dfs_paralelo(){

    printf("\n\n\tINCIANDO O DFS PARALELO STATICO\n\n");
    int contador = ZERO;
	
	memset( activeSet , ZERO , sizeof( activeSet ) ) ;
/*    for(int i = 1; i<=dimension; ++i){
        activeSet[i] = ZERO;}*/
	

	activeSet[INICIO]=TRUE;
    nivel = INICIO;
    custo += limiteInferior; /*Busca se dara como em papadimitriou e steinglitz (1998)*/


    /*
        Criando filhos de 1 no nivel 2
    */
    for(int indice = 1; indice <=dimension; ++indice){
        if(activeSet[indice] == TRUE){
            continue;
        }
        else{
		
            matriz[contador] = new Root(custo+cost(INICIO,indice), indice);
            ++contador;
        }
    }



    /*
        Inicio de regiao paralela
    */
   // #pragma omp parallel for schedule(dynamic, (int)(dimension/4)) shared(myheap)
	#pragma omp parallel for shared(myheap)
    for(int raiz = 0; raiz <contador; ++raiz){

        /*DEVE EXISTIR AQUI UMA ATUALIZACAO DE UB*/

        matriz[raiz]->upper_bound = limiteSuperior;


        matriz[raiz]->dfs();

        #pragma omp critical
        {
           // printf("\n\n\tOla, sou a thread %d e achei %d solucoes no TAD %d: ", omp_get_thread_num(),matriz[raiz]->qtd_sol,raiz);
            myheap.push(matriz[raiz]->upper_bound); //da pra melhorar isso
           // printf("\nsol: %d", myheap.top());
            qtdNodosExpandidos +=matriz[raiz]->qtd_nodos;
            limiteSuperior = myheap.top();
        }

    }//fim do for e regiao paralela

    printf("\n\n\nATENCAO, O OTIMO E': %d \n", myheap.top());
}


void jurema_paralela(){

	 register int i;
	 int nodoAtual;
	 int branchingNode;
	 register int contador = ZERO;


    /*
        Manipulacao da matriz de ciclo. A priori o ciclo seria a solucao guia.
	 */
     //memcpy(ciclo,solucaoGuia,sizeof(solucaoGuia));

     for(int i = 1; i<=dimension; ++i){
            activeSet[i] = TRUE;
     }
	 printf("\n\n\tINICIANDO O METODO JUREMA PARALELO\n\n");
	 printf("\n\nProcessadores no sistema: %d", omp_get_num_procs());

	 /*Tempo Comeca a ser contado*/

	 custo = limiteSuperior; /*Custo da solucao guia*/

	 custo -= lbound(solucaoGuia[dimension],INICIO);
	 custo -= lbound(solucaoGuia[anterior(dimension)],solucaoGuia[dimension]);
	 activeSet[solucaoGuia[dimension]] = FALSE;

	 for(nivel = (dimension - 2); nivel>=1; --nivel){

            memset( lbOrder , 0 , sizeof( lbOrder ) ) ;

            custo -= lbound(solucaoGuia[nivel],solucaoGuia[proximo(nivel)]);
			nodoAtual = solucaoGuia[nivel];

            contador = 0; /*Para lbOrder[x]*/

            for(int i =1; i<=dimension; ++i){

                if(i == nodoAtual) continue;

                    if(activeSet[i]!=TRUE && (custo + lbound(nodoAtual, i)< limiteSuperior)){
                        lbOrder[contador] = i;
                        ++contador;
                    }//if

            }//for


			qsort( lbOrder, contador, sizeof( int ), lb_cmp2); //VALE A PENA??
	        activeSet[solucaoGuia[proximo(nivel)]] = FALSE;

            /*
                Criando as raizes
            */
            for(int indice = 0; indice <contador; ++indice){
               // printf("\n\nNivel %d da busca",nivel);
               // printf("\n\t lbOrder[%d]=%d.", indice, lbOrder[indice]);
                matriz[indice] = new Root(custo+cost(nodoAtual,lbOrder[indice]), lbOrder[indice]);
                //printf("\n\t Raiz de indice %d, custo %d e nivel %d criada.", (matriz[indice]->indice), matriz[indice]->custo, matriz[indice]->nivel);

            }
    	    /*
		@TODO: Mudar o escalonamento para funcao de contador.
	    *///(int)(dimension/6)

            #pragma omp parallel for schedule(dynamic, (int)(dimension/6)) shared(myheap)

			for(int raiz = 0; raiz <contador; ++raiz){


        		matriz[raiz]->upper_bound = limiteSuperior;

        		matriz[raiz]->dfs();
                /*
                    @TODO: Ajeitar essa parte, regiao critica desnecessaria. Inserir uma flag no TAD.

		*/
        		#pragma omp critical
        		{
            		//printf("\n\n\tOla, sou a thread %d e achei %d solucoes no nivel %d: ", omp_get_thread_num(),matriz[raiz]->qtd_sol,nivel);
            		myheap.push(matriz[raiz]->upper_bound); //da pra melhorar isso
            		qtdNodosExpandidos+=matriz[raiz]->qtd_nodos;
            		limiteSuperior = myheap.top();

        		}

    		}//fim do for e regiao paralela

    }//for

   // printf("\n\n\nATENCAO, O OTIMO E': %d \n", myheap.top());

}//jurema_paralela





void jurema_paralelismo_em_nivel(){

	 register int i;
	 int nodoAtual;
	 int branchingNode;
	 register int contador = ZERO;


    /*
        Manipulacao da matriz de ciclo. A priori o ciclo seria a solucao guia.
	 */
     //memcpy(ciclo,solucaoGuia,sizeof(solucaoGuia));

     for(int i = 1; i<=dimension; ++i){
            activeSet[i] = TRUE;
     }
	 printf("\n\n\tINICIANDO O METODO JUREMA PARALELO\n\n");
	 printf("\n\nProcessadores no sistema: %d", omp_get_num_procs());

	 /*Tempo Comeca a ser contado*/

	 custo = limiteSuperior; /*Custo da solucao guia*/

	 custo -= lbound(solucaoGuia[dimension],INICIO);
	 custo -= lbound(solucaoGuia[anterior(dimension)],solucaoGuia[dimension]);
	 activeSet[solucaoGuia[dimension]] = FALSE;

	//set_nested_(1); //private?nivel, custo, lborder, nodoAtual, contador, activeSet,matriz//shared?lbound
	 for(nivel = (dimension - 2); nivel>=1; --nivel){
			
			
            memset( lbOrder , 0 , sizeof( lbOrder ) ) ; //apagar

            custo -= lbound(solucaoGuia[nivel],solucaoGuia[proximo(nivel)]);
			nodoAtual = solucaoGuia[nivel];

            contador = 0; /*Para lbOrder[x]*/

            for(int i =1; i<=dimension; ++i){

                if(i == nodoAtual) continue;

                    if(activeSet[i]!=TRUE && (custo + lbound(nodoAtual, i)< limiteSuperior)){
                        lbOrder[contador] = i;
                        ++contador;
                    }//if

            }//for


			qsort( lbOrder, contador, sizeof( int ), lb_cmp2); //VALE A PENA??//apagar
	        activeSet[solucaoGuia[proximo(nivel)]] = FALSE;

            /*
                Criando as raizes
            */
            for(int indice = 0; indice <contador; ++indice){
               // printf("\n\nNivel %d da busca",nivel);
               // printf("\n\t lbOrder[%d]=%d.", indice, lbOrder[indice]);
                matriz[indice] = new Root(custo+cost(nodoAtual,lbOrder[indice]), lbOrder[indice]);
                //printf("\n\t Raiz de indice %d, custo %d e nivel %d criada.", (matriz[indice]->indice), matriz[indice]->custo, matriz[indice]->nivel);

            }
    	    /*
		@TODO: Mudar o escalonamento para funcao de contador.
	    *///(int)(dimension/6)

            #pragma omp parallel for schedule(dynamic, (int)(dimension/6)) shared(myheap)

			for(int raiz = 0; raiz <contador; ++raiz){


        		matriz[raiz]->upper_bound = limiteSuperior;

        		matriz[raiz]->dfs();
                /*
                    @TODO: Ajeitar essa parte, regiao critica desnecessaria. Inserir uma flag no TAD.

		*/
        		#pragma omp critical
        		{
            		//printf("\n\n\tOla, sou a thread %d e achei %d solucoes no nivel %d: ", omp_get_thread_num(),matriz[raiz]->qtd_sol,nivel);
            		myheap.push(matriz[raiz]->upper_bound); //da pra melhorar isso
            		qtdNodosExpandidos+=matriz[raiz]->qtd_nodos;
            		limiteSuperior = myheap.top();

        		}

    		}//fim do for e regiao paralela

    }//for

   // printf("\n\n\nATENCAO, O OTIMO E': %d \n", myheap.top());

}//jurema_paralela
