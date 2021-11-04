/////////////////////////////////////////////////
// Álvaro Aguilar Sabuco                       //
// Computación Paralela, Práctica 0, Tarea 2   //
// Fecha: 20/10/2021                           //
/////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#define N 1200


/**
 * @brief Realiza los pasos de la primera iteración
 * 
 * @param matriz Matriz que usaremos para las iteraciones
 * @param v_uni Vector principal
 * @param v_aux Vector auxiliar
 */
void primeraIteracion(double **matriz, double *v_uni, double *v_aux){

    double a;

    //Multiplicamos la matriz por el vector unitario y almacenamos el resultado en el vector auxiliar
    for(int i = 0;i < N;i++){
        a=0;
        for(int u = 0;u < N;u++){
            a = a + (matriz[i][u]*v_uni[u]);
        }
        v_aux[i] = a;
    }
   
    for(int i = 0;i < N;i++){
        v_uni[i]=v_aux[i];
    }
}


/**
 * @brief Realiza los pasos de las iteraciones impares
 * 
 * @param matriz Matriz que usaremos para las iteraciones
 * @param v_uni Vector principal
 * @param v_aux Vector auxiliar
 * @return double 
 */
double siguienteIteracion(double **matriz, double *v_uni, double *v_aux){
    
    //Multiplicamos la matriz por el vector resultado de la anterior iteracion y almacenamos el resultado en el vector auxiliar
    double a,mayor_absoluto,aux,valor_mayor;
    int mayor_indice;
    for(int i = 0;i < N;i++){ 
        a=0;
        for(int u = 0;u < N;u++){
            a = a + (matriz[i][u]*v_uni[u]);
        }
        v_aux[i] = a;
    }

    //Buscamos el valor mayor absoluto
    mayor_absoluto = 0;
    for(int i = 0;i < N;i++){
        aux = sqrt(v_aux[i]*v_aux[i]);
        if( aux > mayor_absoluto){
            mayor_absoluto = aux;
            mayor_indice = i;
        }
    }
    valor_mayor=v_aux[mayor_indice];

    //dividimos todo el vector entre el mayor valor absoluto
    for(int i = 0;i<N;i++){
        v_aux[i]=v_aux[i]/valor_mayor;
        v_uni[i]=v_aux[i];

        //printf(" %.3lf \n", v_uni[i]);
    } 
    return valor_mayor;
    
}


/**
 * @brief Realiza según el número de iteración que sea una operación u otra
 * 
 * @param num_ite Número de iteraciones
 * @param matriz Matriz que usaremos para las iteraciones
 * @param v_uni Vector principal
 * @param v_aux Vector auxiliar
 * @param v_absoluto Vector que guarda los valores absolutos
 */
void realizarIteraciones(int num_ite, double **matriz,double *v_uni,double *v_aux, double *v_absoluto){
    for(int i = 1;i<=num_ite;i++){
        if(i==1){
            primeraIteracion(matriz, v_uni, v_aux);
        }else{ 
            v_absoluto[i-2] = siguienteIteracion(matriz, v_uni, v_aux);
        }
    }
    
}

/**
 * @brief Guardar en un fichero información sobre los parámetros de entrada, normas calculadas, norma vector final, tiempos de ejecución, guardado y generado de matriz y total
 * 
 * @param nom_fich_resm Nombre del fichero que guarda todos los datos de la ejecución del programa
 * @param num_iteraciones Número de iteraciones que realiza el programa
 * @param t_gen Tiempo que tarda en generarse la matriz
 * @param t_ejec Tiempo que tarda en ejecutarse las iteraciones
 * @param t_global Tiempo global que dura toda la ejecución del programa
 * @param v_absoluto Vector que contiene el valor absoluto de cada iteración
 */
void guardarResumen(char nom_fich_resm[],int num_iteraciones, double t_gen, double t_ejec, double t_global,double *v_absoluto){
    FILE *f1;
    f1 = fopen(nom_fich_resm,"w");
    if(f1==NULL){
        printf("No se puede abrir el fichero");
    }
   
    fprintf(f1,"Numero de iteraciones: %i \n", num_iteraciones);
    fprintf(f1,"Nombre fichero salida: %s \n", nom_fich_resm);


    
    fprintf(f1,"Valores absolutos: \n");
    fprintf(f1,"Iteracion 0: No se aplica \n");
    for(int i = 0;(i<num_iteraciones-1);i++){
        fprintf(f1,"Iteracion %d: %.3lf \n",(i+1), v_absoluto[i]);
    }
    
    fprintf(f1,"Tiempo(milisengundos) de generacion de matriz:  %f \n", t_gen);

    fprintf(f1,"Teimpo(milisengundos) de ejecucion:  %f \n", t_ejec);
    fprintf(f1,"Tiempo(milisengundos) global:  %f \n", t_global);
    
    fclose(f1);
}


/**
 * @brief Imprime información sobre los parámetros de entrada, normas calculadas, norma vector final, tiempos de ejecución, guardado y generado de matriz y total
 * 
 * @param num_iteraciones Número de iteraciones que realiza el programa
 * @param nom_fich_sal Nombre del fichero que guarda todos los datos de la ejecución del programa
 * @param t_gen Tiempo que tarda en generarse la matriz
 * @param t_ejec Tiempo que tarda en ejecutarse las iteraciones
 * @param t_global Tiempo global que dura toda la ejecución del programa
 * @param v_absoluto Vector que contiene el valor absoluto de cada iteración
 */
void imprimirResumen(int num_iteraciones, char *nom_fich_sal, double t_gen, double t_ejec, double t_global, double *v_absoluto){

    printf("Numero de iteraciones: %i \n", num_iteraciones);
    printf("Nombre fichero salida: %s \n", nom_fich_sal);


    printf("Valores absolutos: \n");
    printf("Iteracion 0: No se aplica \n");
    for(int i = 0;i<(num_iteraciones-1);i++){
        printf("Iteracion %d: %.3lf \n",(i+1), v_absoluto[i]);
    }
    printf("Tiempo(milisengundos) de generacion de matriz:  %f  \n", t_gen);

    printf("Tiempo(milisengundos) de ejecucion:  %f \n", t_ejec);
    printf("Tiempo(milisengundos) global:  %f \n", t_global);
}



/**
 * @brief Genera una matriz de tamaño NxN
 * 
 * @param matriz matriz donde se guardará la matriz que generamos
 */
void generarMatriz(double **matriz){
    double aux,a=0;
    srand(time(NULL));
    
    for(int x =0; x<N;x++){
        for(int y =0; y<N;y++){
            
            if(x == y){ //Elementos de la diagonal
                matriz[x][y] = 1;
            }else if(x < y){ //Elementos triangular inferior
                aux = rand( ) % 50001;
                matriz[x][y] = (double)50*(x+1)*(y+1)/(N+N);
              
            }else{//Elementos triangular superior valores negativos
                aux = -1*(rand( ) % 50001);
                matriz[x][y] = (double)-50*(x+1)*(y+1)/(N+N); 
            }
            //printf("%.1lf ", matriz[x][y]);
        }
       // printf("\n");
    }
}



/**
 * @brief Función principal
 * 
 * @param argc Número de parametros que pasamos al ejecutar el programa
 * @param argv Parametros que pasamos al ejecutar el programa
 * @return int 
 */
int main(int argc, char *argv[]){

    //Declaramos las variables de tiempo e iniciamos el contador del tiempo total
    clock_t t_ini, t_fin, t_init, t_fint;
    t_init = clock();

    //Indicamos un parámetro de entrada que explique que información hay que pasar como parametro
    if(argc == 2 && strcmp("HELP",argv[1])==0){
        printf("Tienes que indicar el numero de iteraciones y el fichero de salida para los datos como argumentos, respectivamente\n");
        return 0;
    }

    //Si no recibimos los dos parametros la ejecución se para
    if(argc != 3){
        printf("Debes indicar varios paramentros. Para mas ayuda pasa como parametro HELP\n");
        return 0;
    }

    
    double t_gen, t_ejec, t_global, a;
    int diagonal = 0;
    int num_ite = atoi(argv[1]);
    FILE *f1, *f2;
    

    //Fichero de salida
    char *nom_fich_sal =argv[2];
    strcat(nom_fich_sal,".txt");
    
    
    //Declaramos y reservamos memoria del vector unitario
    double *v_inicial;
    v_inicial = (double *)malloc(N*sizeof(double));
    for(int i=0;i<N;i++){
        v_inicial[i]=1; 
    }

    //Declaramos y reservamos memoria del vector aux
    double *v_auxiliar;
    v_auxiliar = (double *)malloc(N*sizeof(double));
    for(int i=0;i<N;i++){
        v_auxiliar[i]=0;
    }

    //Declaramos y reservamos memoria de la matriz
    double **matriz;
    matriz = (double **)malloc(N*sizeof(double *));
    for(int i =0; i<N;i++){
        matriz[i] = (double *)malloc(N*sizeof(double));
    }

     //Declaramos y reservamos memoria del vector con valores maximos absolutos
    double *v_absoluto;
    v_absoluto = (double *)malloc((num_ite-1)*sizeof(double));
    
   
    t_ini = clock();
    generarMatriz(matriz);
    t_fin= clock();
    //Tiempo de generación de matriz
    t_gen = ((double)(t_fin-t_ini)/CLOCKS_PER_SEC)*1000; 

    t_ini = clock();
    
    //Realizamos las iteraciones
    realizarIteraciones(num_ite, matriz, v_inicial, v_auxiliar,v_absoluto);

    t_fin= clock();
    //Tiempo que tarda en realizar las iteraciones y sus respectivas operaciones
    t_ejec = ((double)(t_fin-t_ini)/CLOCKS_PER_SEC)*1000;


    t_fint = clock();
    //Tiempo global de todo el programa
    t_global = ((double)(t_fint-t_init)/CLOCKS_PER_SEC)*1000;
    
    //Guardamos los datos obtenidos en in fichero 
    guardarResumen(nom_fich_sal, num_ite, t_gen,  t_ejec, t_global,v_absoluto);
    
    //Imprimimos por pantalla los datos obtenidos
    imprimirResumen(num_ite, nom_fich_sal, t_gen,  t_ejec, t_global,v_absoluto);

    return 0;
}