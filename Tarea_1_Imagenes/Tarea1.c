//////////////////////////////////////////////////////////////////
// Alvaro Aguilar Sabuco                                        //
// Computacion Paralela, Practica 1, Tarea 1                    //
// Fecha: 02/11/2021                                            //
//////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#define ELEMENTOS 9

/**
 * @brief Lee la imagen en formato raw, que es binario
 * 
 * @param nombre_fichero Nombre del fichero de imagen raw
 * @param tamanyo tamanyo de la matriz
 * @param matriz matriz que guardara la imagen raw
 */
void leerArchivo(char *nombre_fichero,int tamanyo, unsigned char **matriz, int num_filas, int num_columnas){
    FILE *f1;
    f1 = fopen(nombre_fichero,"rb");
    int filas = 0, columnas = 0;
    int lectura_columnas = 0;

    if(f1){
        //Lectura del fichero
        /*
        for(int i = 0; i < tamanyo; i++){
            for(int j = 0; j < tamanyo; j++){
                fread(&matriz[i][j],sizeof(unsigned char), 1, f1);
            }
        }*/
        while(!feof(f1)){

            fread(&matriz[filas][columnas],sizeof(unsigned char), 1, f1);
            columnas++;
            if(matriz[filas][columnas] == '\n'){
                filas++;
                if(lectura_columnas == 0){
                    num_columnas = columnas;
                    lectura_columnas++;
                }
                columnas == 0;
            }
        }
        num_filas = filas;
        fclose(f1);
    }else{
        printf("Error al abrir el fichero");
    }
}

/**
 * @brief Guarda una imagen en formato raw, que es binario
 * 
 * @param matrizSalida matriz resultado del filtrado
 * @param imagen_salida nombre del fichero que guardará la imagen de salida
 * @param tamanyo tamanyo de la mtriz
 */
void guardarImagenSalida(unsigned char **matrizSalida,char *imagen_salida, int tamanyo){
    FILE *f1;
    f1 = fopen(imagen_salida, "wb");
    for(int i = 0;i<tamanyo; i++){
        for(int j = 0;j<tamanyo;j++){
            fwrite(&matrizSalida[i][j],sizeof(unsigned char),1,f1);
        }
    }
    fclose(f1);
}

/**
 * @brief Ordena un vector de menor a mayor
 * 
 * @param elementos vector que contiene los numeros a ordenar
 */
void ordenarVector(unsigned char elementos[]){
    int  elemento_auxiliar;
    //qsort(elementos, tamanyo, sizeof(unsigned char));

    for(int i = 0;i<(ELEMENTOS-1);i++){
       
        for (int j = i+1; j < ELEMENTOS; j++){
           // printf("ordenar %d %d\n", i,j);
            if(elementos[i] > elementos[j]){
            
                elemento_auxiliar = elementos[j];
                elementos[j] = elementos[i];
                elementos[i] = elemento_auxiliar;
            }
        }
        
    }

}

/**
 * @brief Filtra la imagen usando la media de los elementos de alrededor
 * 
 * @param matriz matriz que tenemos que filtrar
 * @param imagen_salida nombre del fichero de salida que guardara la imagen
 * @param tamanyo tamanño de la matriz
 */
void filtradoMedia(unsigned char **matriz, char *imagen_salida,int tamanyo){
    printf("Filtrado por Media");

    unsigned char **matrizSalida, elementoSeleccionado, elementos[9];
    int media = 0;
    matrizSalida = (unsigned char **)malloc(tamanyo*sizeof(unsigned char *));
    for(int i = 0; i<tamanyo;i++){
        matrizSalida[i] = (unsigned char *)malloc(tamanyo*sizeof(unsigned char));
    }

    for(int i = 0;i<tamanyo;i++){
        for(int j = 0;j<tamanyo;j++){
            elementoSeleccionado = matriz[i][j];
            //printf("%c\n",elementoSeleccionado);
            if(i == 0 || j == 0 || i == (tamanyo-1) || j == (tamanyo-1)){
                matrizSalida[i][j] == matriz[i][j];
            }else{
                media = matriz[i-1][j-1] +matriz[i][j-1]+ matriz[i+1][j-1] +matriz[i-1][j] +matriz[i][j] +matriz[i+1][j] +matriz[i-1][j+1] + matriz[i][j+1] +matriz[i+1][j+1] ;
                media = fabs(media/9);
                matrizSalida[i][j] = (unsigned char)media;
            }
        }
        guardarImagenSalida(matrizSalida,imagen_salida,tamanyo);
    }
}

/**
 * @brief Filtra la imagen usando la mediana de los elementos de alrededor
 * 
 * @param matriz matriz que contiene la imagen raw
 * @param imagen_salida nombre del fichero que contiene la imagen de salida
 * @param tamanyo tamanño de la matriz
 */
void filtradoMediana(unsigned char **matriz, char *imagen_salida, int tamanyo){
    
    unsigned char **matrizSalida, elementoSeleccionado,elementos[9];

    matrizSalida = (unsigned char **)malloc(tamanyo*sizeof(unsigned char *));
    for(int i = 0; i<tamanyo;i++){
        matrizSalida[i] = (unsigned char *)malloc(tamanyo*sizeof(unsigned char));
    }

    for(int i = 0;i<tamanyo;i++){
        for(int j = 0;j<tamanyo;j++){
            elementoSeleccionado = matriz[i][j];

            if(i == 0 || j == 0 || i == (tamanyo-1) || j == (tamanyo-1)){
                matrizSalida[i][j] ==matriz[i][j];
            }else{
                elementos[0] = matriz[i-1][j-1];
                elementos[1] = matriz[i][j-1];
                elementos[2] = matriz[i+1][j-1];
                elementos[3] = matriz[i-1][j] ;
                elementos[4] = matriz[i][j] ;
                elementos[5] = matriz[i+1][j] ;
                elementos[6] = matriz[i-1][j+1];
                elementos[7] = matriz[i][j+1];
                elementos[8] = matriz[i+1][j+1];
                ordenarVector(elementos);
                
                matrizSalida[i][j] = elementos[4];
            }
        }
    }
    printf("Filtrado por Mediana\n");
    guardarImagenSalida(matrizSalida,imagen_salida,tamanyo);

}

/**
 * @brief Filtra la imagen usando la técnica de detección de bordes SOBEL
 * 
 * @param matriz matriz que contiene la imagen raw
 * @param imagen_salida nombre del fichero que contiene la imagen de salida
 * @param tamanyo tamanño de la matriz
 */
void deteccionBordes(unsigned char **matriz, char *imagen_salida, int tamanyo){
    unsigned char **matrizSalida, elementoSeleccionado, elementos[9];
    int c = 0, f = 0;
    matrizSalida = (unsigned char **)malloc(tamanyo*sizeof(unsigned char *));
    for(int i = 0; i<tamanyo;i++){
        matrizSalida[i] = (unsigned char *)malloc(tamanyo*sizeof(unsigned char));
    }

    for(int i = 0;i<tamanyo;i++){
        for(int j = 0;j<tamanyo;j++){
            elementoSeleccionado = matriz[i][j];
            

            if(i == 0 || j == 0 || i == (tamanyo-1) || j == (tamanyo-1)){
                matrizSalida[i][j] ==matriz[i][j];
            }else{
                elementos[0] = matriz[i-1][j-1];
                elementos[1] = matriz[i][j-1];
                elementos[2] = matriz[i+1][j-1];
                elementos[3] = matriz[i-1][j] ;
                elementos[4] = matriz[i][j] ;
                elementos[5] = matriz[i+1][j] ;
                elementos[6] = matriz[i-1][j+1];
                elementos[7] = matriz[i][j+1];
                elementos[8] = matriz[i+1][j+1];

                c = (elementos[0] * -1) + (elementos[1] * 0) + (elementos[2] * 1) + (elementos[3] * -2) + (elementos[4] * 0) + (elementos[5] * 2) + (elementos[6] *-1) + (elementos[7] * 0) + (elementos[8] * 1);
                f = (elementos[0] * -1) + (elementos[1] * -2) + (elementos[2] * -1) + (elementos[3] * 0) + (elementos[4] * 0) + (elementos[5] * 0) + (elementos[6] *1) + (elementos[7] * 2) + (elementos[8] * 1);
                matrizSalida[i][j] = (unsigned char)sqrt(pow(c,2)+pow(f,2));
            }
        }
    }
    printf("Filtrado SOBEL");
    guardarImagenSalida(matrizSalida,imagen_salida,tamanyo);
}

//Guarda en un fichero los parámetros de ejecución, fichero de entrada, fichero de salida, filtrado escogido y tiempo de ejecucion
/**
 * @brief Guarda los datos de ejecución del programa en un fichero txt
 * 
 * @param imagen_entrada nombre del fichero que contiene la imagen que pasamos al programa
 * @param imagen_salida nombre del fichero que contiene la imagen filtrada
 * @param fichero_salida nombre del fichero que contiene los parámetros de ejecución
 * @param tipo_proceso nombre del filtrado escogido
 * @param t_ejecucion tiempo que tarda en ejecutarse el programa
 */
void ficheroSalida(char *imagen_entrada , char *imagen_salida, char *fichero_salida, char *tipo_proceso, double t_ejecucion){
    FILE *f1;
	
	f1 = fopen(fichero_salida,"w");
    if(f1==NULL){
        printf("No se puede abrir el fichero");
    }
	
	fprintf(f1,"Fichero de entrada: %s\n", imagen_entrada);
	fprintf(f1,"Fichero de salida: %s\n", imagen_salida);
    fprintf(f1,"Tipo de proceso: %s\n",  tipo_proceso);
	fprintf(f1,"Tiempo de ejecucion: %f\n",  t_ejecucion);

}

/**
 * @brief Función principal
 * 
 * @param argc Número de parametros que pasamos al ejecutar el programa
 * @param argv Parametros que pasamos al ejecutar el programa
 * @return int 
 */
int main(int argc, char *argv[]){
    clock_t t_inicio, t_fin;
    double t_ejecucion;
    t_inicio = clock();
    
    //Indicamos un parámetro de entrada que explique que información hay que pasar como parametro
    if(argc == 2 && strcmp("HELP",argv[1])==0){
        printf("Debes indicar el nombre de la imagen de entrada ,imange de salida, fichero de resultados de salida, sin formato, y el tipo de proceso(MEDIA, MEDIANA O SOBEL). Por ejemplo './Tarea1 fotoEntrada fotoSalida resultadosSalida MEDIA'\n");
        return 0;
    }
        
    if(argc != 5){
        printf("Debes indicar varios paramentros. Para mas ayuda pasa como parametro HELP\n");
        return 0;
    }

    struct stat sb;
    char *imagen_entrada,*imagen_salida, *tipo_proceso, *fichero_salida;
    int tamanyo_matriz;
    char opcion;
    unsigned char **matriz;

    //Como el tamanyo de un caracter unsigned char es de 1 byte leemos directamente 
    //el tamanyo del fichero y una vez que tenemos el número de bytes hacemos la raiz cuadrada
    //para saber la altura y la anchura de la matriz que es la misma cantidad
    imagen_entrada = argv[1];
    imagen_salida = argv[2];
    fichero_salida = argv[3];
    tipo_proceso = argv[4];
    strcat(imagen_entrada,".raw");
    strcat(imagen_salida,".raw"); 
    strcat(fichero_salida,".txt");

    if (stat(imagen_entrada, &sb) == -1) {
        return 0;
    }
    tamanyo_matriz = sb.st_size;

    matriz = (unsigned char **)malloc(tamanyo_matriz*sizeof(unsigned char *));
    for(int i = 0;i < tamanyo_matriz; i++){
        matriz[i] = (unsigned char *)malloc(tamanyo_matriz*sizeof(unsigned char));
    }
    //Lectura de matriz
    leerArchivo(imagen_entrada,tamanyo_matriz,matriz);
    //guardarImagenSalida(matriz,imagen_salida,tamanyo_matriz);
    //return 0;

    if(strcmp("MEDIA",tipo_proceso)==0){
        filtradoMedia(matriz,imagen_salida,tamanyo_matriz);
        
    }else if (strcmp("MEDIANA",tipo_proceso)==0){
        filtradoMediana(matriz,imagen_salida,tamanyo_matriz);
        
    }else if (strcmp("SOBEL",tipo_proceso)==0){
        deteccionBordes(matriz,imagen_salida,tamanyo_matriz);
      
    }else{
        printf("Solo puedes pasar como proceso: MEDIA, MEDIANA O SOBEL %s\n",tipo_proceso);
    }

    t_fin = clock();
    t_ejecucion = ((double)(t_fin-t_inicio)/CLOCKS_PER_SEC)*1000;

    ficheroSalida(imagen_entrada, imagen_salida, fichero_salida, tipo_proceso, t_ejecucion);
    return 0;

}
