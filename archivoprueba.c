#include "utils.h"
#include <stdlib.h>
#define NUM_DIVISIONES 100 // Número de divisiones para cada segmento
#define MAX 300
#define MIN 3
#define OPCION1 1
#define OPCION2 2
#define LIMITANTE 30
typedef struct {
    int x;
    int y;
} Punto;
//dimension de x,y en el plano
typedef struct {
    Punto inicio;
    Punto fin;
} Segmento;
// segmento de línea entre dos puntos: "inicio" y "fin"
double distancia(Punto a, Punto b) {
    return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
    //formula para conseguir la distancia maxima entre puntos(teorema de Pitágoras)
}
//si dos Segmentoa se intersectan
int interseccion(Segmento seg1, Segmento seg2) {
    /*proponemos el metodo del producto vectorial en donde analizamos
    los signos de cada producto y si son distintos es por que se
    intersectan si no no hay interseccion*/
    int abc = (seg2.inicio.x - seg1.inicio.x) * (seg1.fin.y - seg1.inicio.y) - 
              (seg2.inicio.y - seg1.inicio.y) * (seg1.fin.x - seg1.inicio.x);
    
    int abd = (seg2.fin.x - seg1.inicio.x) * (seg1.fin.y - seg1.inicio.y) - 
              (seg2.fin.y - seg1.inicio.y) * (seg1.fin.x - seg1.inicio.x);
    
    int cda = (seg1.inicio.x - seg2.inicio.x) * (seg2.fin.y - seg2.inicio.y) - 
              (seg1.inicio.y - seg2.inicio.y) * (seg2.fin.x - seg2.inicio.x);
    
    int cdb = (seg1.fin.x - seg2.inicio.x) * (seg2.fin.y - seg2.inicio.y) - 
              (seg1.fin.y - seg2.inicio.y) * (seg2.fin.x - seg2.inicio.x);
    
    return (abc * abd < 0) && (cda * cdb < 0);
    /*aca intente (creo que funciona) comparar los valores
    que determina si dos segmentos de línea se cruzan, Toma cuatro estructuras Punto como parámetros.
    representando los extremos de cada segmento*/
}

void ingresarCoordenadas(int cantpuntos, Punto puntos[]) {
    printf("Ingrese los valores utilizando el espacio para cada vertice ej. (35 45)\n");
    for (int i = 0; i < cantpuntos; i++) {
        printf("Ingrese las coordenadas para el punto %d\n", i + 1);
        printf("X | Y\n");
        scanf("%d %d", &puntos[i].x, &puntos[i].y);
    }
     /* Función para ingresar las coordenadas de los puntos que forman el polígono.
    Se solicita al usuario que ingrese las coordenadas X e Y de cada punto
    Parámetros:
                - cantpuntos: cantidad de puntos (par de puntos) que tiene el polígono.
                - puntos[]: arreglo de estructuras Punto donde se almacenarán las coordenadas.
    */
}

void cargarCoordenadasDesdeCSV(const char *nombreArchivo, int *cantpuntos, Punto puntos[]) {
    printf("intentando abrir el archivo: %s\n", nombreArchivo);//debuguear
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo %s\n", nombreArchivo);
    } else {
        char linea[256];
        *cantpuntos = 0;
        while (fgets(linea, sizeof(linea), archivo) && *cantpuntos < LIMITANTE) { // Limitar a 30 puntos
            char *token = strtok(linea, " "); // separa en subcadena usando " " es una forma de depurar las coordenadas
            if (token != NULL) {
                puntos[*cantpuntos].x = atoi(token);//si es NULL hay valor en X y lo convierte en entero con ATOI
                token = strtok(NULL, " "); // NULL indica que se continua en la linea
                if (token != NULL) {//si no es NULL es por que hay un valor para Y 
                    puntos[*cantpuntos].y = atoi(token);
                    (*cantpuntos)++;
                }
            }
        }
        fclose(archivo);
    }
}
void crearPuntosIntermedios(Punto inicio, Punto fin, Punto puntosIntermedios[NUM_DIVISIONES + 1]) {
    for (int div = 0; div <= NUM_DIVISIONES; div++) {
        // Dividir el segmento en puntos intermedios
        puntosIntermedios[div].x = inicio.x + (fin.x - inicio.x) * div / NUM_DIVISIONES;
        puntosIntermedios[div].y = inicio.y + (fin.y - inicio.y) * div / NUM_DIVISIONES;
        /*aca tomamos como base el principio y el fin del segmento para poder dividirlo
        en NUM_DIVISIONES*/
    }
}
// Verifica si el segmento entre "puntoA" y "puntoB" cruza algún lado del polígono definido
int verificarintersecciones(Punto puntoA, Punto puntoB, int cantpuntos, Punto puntos[]) {
    Segmento segPuntos = {puntoA, puntoB};//segmento de A a B
    int hayInterseccion = 0;
    int k=0;
    while (k < cantpuntos && hayInterseccion==0) {
        Segmento segPoligono = {puntos[k], puntos[(k + 1) % cantpuntos]};
        hayInterseccion = interseccion(segPoligono, segPuntos) ? 1 : hayInterseccion;
        k++;
    }
    /*for (int k = 0; k < cantpuntos; k++){
        Segmento segPoligono = {puntos[k], puntos[(k + 1) % cantpuntos]}; // Define un lado del polígono
       if (interseccion(segPoligono, segPuntos)) { // Verifica si el segmento interseca el lado del polígono
            hayInterseccion = interseccion(segPoligono, segPuntos) ? 1 : hayInterseccion; 
        }
    }*/
    return hayInterseccion;
}

// funcion queactualiza la distancia máxima si la distancia entre A y B es mayor que la actual
void actualizarDistanciaMaxima(Punto puntoA, Punto puntoB, int j, int p,
                               double *max_distancia, Punto *punto1, Punto *punto2,
                               int *segmento1, int *segmento2) {
    double dist = distancia(puntoA, puntoB); // calcula la distancia
    if (dist > *max_distancia) { // s la distancia es mayor que la máxima registrada
        *max_distancia = dist; // actualiza la distancia máxima
        *punto1 = puntoA; // guarda "puntoA" como uno de los puntos de distancia máxima
        *punto2 = puntoB; // guarda "puntoB" como el otro punto de distancia máxima
        *segmento1 = j; // almacena el número (valor) del primer segmento
        *segmento2 = p; // almacena el número (valor) del segundo segmento
    }
}
//esta funcion verifica si no intersecta antes de actualizar datos
void procesarPuntoB(Punto puntoA, int j, int p, int cantpuntos, Punto puntos[],
                    double *max_distancia, Punto *punto1, Punto *punto2,
                    int *segmento1, int *segmento2) {
    Punto puntosIntermedios[NUM_DIVISIONES + 1];
    crearPuntosIntermedios(puntos[p], puntos[(p + 1) % cantpuntos], puntosIntermedios);
    for (int div2 = 0; div2 <= NUM_DIVISIONES; div2++) {
        Punto puntoB = puntosIntermedios[div2];
        if (!verificarintersecciones(puntoA, puntoB, cantpuntos, puntos)) {// Si no hay intersección.
            actualizarDistanciaMaxima(puntoA, puntoB, j, p, max_distancia, punto1, punto2, segmento1, segmento2);
        }
    }
}
/*esta funcion procesa el punto "puntoA" y luego llama a  la funcion procesarPuntoB
para verificar cada posible distancia máxima*/
void procesarPuntoA(int j, int p, int cantpuntos, Punto puntos[],
                    double *max_distancia, Punto *punto1, Punto *punto2,
                    int *segmento1, int *segmento2) {
    Punto puntosIntermedios[NUM_DIVISIONES + 1];
    crearPuntosIntermedios(puntos[j], puntos[(j + 1) % cantpuntos], puntosIntermedios);//crea A
    for (int div1 = 0; div1 <= NUM_DIVISIONES; div1++) {
        Punto puntoA = puntosIntermedios[div1];
        procesarPuntoB(puntoA, j, p, cantpuntos, puntos, max_distancia, punto1, punto2, segmento1, segmento2);
    }
}

// funcion para procesar todos los pares de puntos del poligono y encontrar la distancia máxima posible
void procesarParDePuntos(int cantpuntos, Punto puntos[],
                        double *max_distancia, Punto *punto1, Punto *punto2,
                        int *segmento1, int *segmento2) {
    int j = 0;
    while (j < cantpuntos) {
        int p = j + 1; // Inicializa el índice para el segundo punto
        while (p < cantpuntos) {
            /* Llamada a la función procesarPuntoA para verificar todos los puntos intermedios
            entre los puntos j (primer punto) y p (segundo punto), también calculamos la distancia máxima
            y actualiza los valores a mostrar */
            procesarPuntoA(j, p, cantpuntos, puntos, max_distancia, punto1, punto2, segmento1, segmento2);
            p++; // Incrementa el índice del segundo bucle
        }
    j++; // Incrementa el índice del primer bucle
    }   
    /*for (int j = 0; j < cantpuntos; j++) {
        for (int p = j + 1; p < cantpuntos; p++) {
            Llamada a la función procesarPuntoA0 para verificar todos los puntos intermedios
            entre los puntos j(primer unto) y p(segundo punto), también calculamos la distancia máxima
            y actualiza las valores a mostrar
            procesarPuntoA(j, p, cantpuntos, puntos, max_distancia, punto1, punto2, segmento1, segmento2);
        }
    }*/
}

void encontrarDistanciaMaxima(int cantpuntos, Punto puntos[], double *max_distancia,
                            Punto *punto1, Punto *punto2, int *segmento1, int *segmento2) {
    *max_distancia = 0;
    /*llamamos a la función procesarParDePuntos, para que pase por todos los pares de puntos
    y calcule la distancia máxima entre ellos.*/
    procesarParDePuntos(cantpuntos, puntos, max_distancia, punto1, punto2, segmento1, segmento2);
    
    printf("la distancia maxima encontrada es: %f\n", *max_distancia);
    printf("se encuentra entre los puntos (%d;%d) y (%d;%d)\n", punto1->x, punto1->y, punto2->x, punto2->y);
    
    if (*segmento1 != -1 && *segmento2 != -1) {
        printf("el punto (%d;%d) esta en el segmento %d\n", punto1->x, punto1->y, *segmento1 + 1);
        printf("el punto (%d;%d) esta en el segmento %d\n", punto2->x, punto2->y, *segmento2 + 1);
    }
}

void guardarDatosEnArchivo(int cantpuntos, Punto puntos[], double max_distancia,
                          Punto punto1, Punto punto2, int segmento1, int segmento2) {
    FILE *archivo = fopen("../resultados_poligono.txt", "w");
    // verifica si se pudo abrir el archivo
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo para escritura.\n");
    }else {
        // Escribe los datos del polígono y los resultados en el archivo
        fprintf(archivo, "Datos del polígono:\n");
        fprintf(archivo, "Número de puntos: %d\n\n", cantpuntos);
        fprintf(archivo, "Coordenadas de los puntos:\n");
        
        for (int i = 0; i < cantpuntos; i++) {
            fprintf(archivo, "Punto %d: (%d, %d)\n", i+1, puntos[i].x, puntos[i].y);
        }
        
        fprintf(archivo, "\nResultados:\n");
        fprintf(archivo, "La distancia máxima encontrada es: %f\n", max_distancia);
        fprintf(archivo, "Se encuentra entre los puntos (%d,%d) y (%d,%d)\n", punto1.x, punto1.y, punto2.x, punto2.y);
        
        if (segmento1 <0 && segmento2 <0) {
            fprintf(archivo, "El punto (%d,%d) está en el segmento %d\n", punto1.x, punto1.y, segmento1+1);
            fprintf(archivo, "El punto (%d,%d) está en el segmento %d\n", punto2.x, punto2.y, segmento2+1);
            fprintf(archivo, "Muchas gracias por darme esta oportunidad de explotar mi rendimiento y de poder hacerme volar con la programación\n");
        
        }
        
        fclose(archivo);
        printf("\nLos resultados se han guardado en el archivo 'resultados_poligono.txt'\n");
    }
}

void verificacionnumerodevertice() {
    // Solicita al usuario el número de vértices del polígono
    int opcion;
    int cantpuntos = 0;
    Punto puntos[MAX];
    printf("seleccione una opcion:\n");
    printf("1. ingresar coordenadas manualmente\n");
    printf("2. cargar coordenadas desde un archivo CSV\n");
    opcion = leerEnteroEntre(OPCION1, OPCION2, "ingrese su opcion ");
    //desde acá comparamos la eleccion del usuario para saber como realizar la carga de las coordenadas
    if (opcion == OPCION1) {
        cantpuntos = leerEnteroEntre(3, 30,
                    "ingrese la cantidad de puntos (vertices) que tiene el poligono");
        ingresarCoordenadas(cantpuntos, puntos);
        /*necesitamos como minimo 3 aungulos o pares de cooordenadas
        usando mi libreria*/
    } else if (opcion == OPCION2) {
        cargarCoordenadasDesdeCSV("coordenadas.csv", &cantpuntos, puntos);
    } else {
        printf("opcion no valida\n");
    }
    
    if (cantpuntos < MIN && cantpuntos > MAX) {
        printf("se requieren al menos 3 puntos para formar un poligono\n");
    }
    // Calcula la distancia máxima y guarda los resultados
    double max_distancia;
    Punto punto1, punto2;
    int segmento1 = -1, segmento2 = -1;
    
    encontrarDistanciaMaxima(cantpuntos, puntos, &max_distancia,
                            &punto1, &punto2, &segmento1, &segmento2);
    guardarDatosEnArchivo(cantpuntos, puntos, max_distancia,
                         punto1, punto2, segmento1, segmento2);
}

int main() {
    verificacionnumerodevertice();
}