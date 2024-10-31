#include "utils.h"
#define NUM_DIVISIONES 100  // Número de divisiones para cada segmento
#define MAX 300
#define MIN 3
typedef struct {
    int x;
    int y;
} Punto;
//dimension de x,y en el plano
double distancia(Punto a, Punto b) {
    return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
    //formula para conseguir la distancia maxima entre puntos
}

int interseccion(Punto a, Punto b, Punto c, Punto d) {
    int abc = (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
    int abd = (d.x - a.x) * (b.y - a.y) - (d.y - a.y) * (b.x - a.x);
    int cda = (a.x - c.x) * (d.y - c.y) - (a.y - c.y) * (d.x - c.x);
    int cdb = (b.x - c.x) * (d.y - c.y) - (b.y - c.y) * (d.x - c.x);
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
    Se solicita al usuario que ingrese las coordenadas X e Y de cada punto.
    Parámetros:
                - cantpuntos: cantidad de puntos (ángulos) que tiene el polígono.
                - puntos[]: arreglo de estructuras Punto donde se almacenarán las coordenadas.
    */
}

void cargarCoordenadasDesdeCSV(const char *nombreArchivo, int *cantpuntos, Punto puntos[]) {
    printf("intentando abrir el archivo: %s\n", nombreArchivo); //debuguear
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo %s\n", nombreArchivo);
        return;
    }
    char linea[256];
    *cantpuntos = 0;
    while (fgets(linea, sizeof(linea), archivo) && *cantpuntos < 30) { // Limitar a 30 puntos
        char *token = strtok(linea, " "); // Cambiado de " " a " " aun que signifique lo mismo es una forma de depurar las coordenadas
        if (token != NULL) {
            puntos[*cantpuntos].x = atoi(token);
            token = strtok(NULL, " "); // Cambiado de " " a " " aun que signifique lo mismo es una forma de depurar las coordenadas
            if (token != NULL) {
                puntos[*cantpuntos].y = atoi(token);
                (*cantpuntos)++;
            }
        }
    }
    fclose(archivo);
}

int encontrarSegmento(Punto punto, int cantpuntos, Punto puntos[]) {
    for (int i = 0; i < cantpuntos; i++) {
        Punto a = puntos[i];
        Punto b = puntos[(i + 1) % cantpuntos];
        // Dividir el segmento en puntos intermedios
        for (int div = 0; div <= NUM_DIVISIONES; div++) {
            Punto intermedio = { 
                a.x + (b.x - a.x) * div / NUM_DIVISIONES,
                a.y + (b.y - a.y) * div / NUM_DIVISIONES
            };
            if (fabs(distancia(intermedio, punto)) < 1e-6) {// Usamos un valor muy pequeño para la igualdad de puntos
                return i;
            }
        }
    }
    return -1;
}

void encontrarDistanciaMaxima(int cantpuntos, Punto puntos[], double *max_distancia, Punto *punto1, Punto *punto2, int *segmento1, int *segmento2) {
    *max_distancia = 0;
    // Bucle para recorrer todos los pares de puntos
    for (int j = 0; j < cantpuntos; j++) {
        for (int p = j + 1; p < cantpuntos; p++) {
            // Dividir los lados en puntos intermedios
            for (int div1 = 0; div1 <= NUM_DIVISIONES; div1++) {
                for (int div2 = 0; div2 <= NUM_DIVISIONES; div2++) {
                    /* creamos puntos intermedios en los segmentos del polígono 
                    para verificar si hay una distancia mazima*/
                    Punto puntoA = { 
                        puntos[j].x + (puntos[(j + 1) % cantpuntos].x - puntos[j].x) * div1 / NUM_DIVISIONES,
                        puntos[j].y + (puntos[(j + 1) % cantpuntos].y - puntos[j].y) * div1 / NUM_DIVISIONES
                        //generamos puntos extra en el segmento7 de los puntos (j;j+1)
                    };
                    Punto puntoB = { 
                        puntos[p].x + (puntos[(p + 1) % cantpuntos].x - puntos[p].x) * div2 / NUM_DIVISIONES,
                        puntos[p].y + (puntos[(p + 1) % cantpuntos].y - puntos[p].y) * div2 / NUM_DIVISIONES
                        //generamos puntos extra en el segmento de los puntos (p;p+1)
                    };
                    int corta_linea = 0;
                    /*estas estructura nos ayua a poder organizar y capturar
                    mayor cantidades de puntos entre las rectas que forman cada vertices
                    podemos decir que nos estructura una tabla de valores mas legibles 
                    yn nos da mas puntos para comparar*/
                    for (int k = 0; k < cantpuntos; k++) {
                        if (interseccion(puntoA, puntoB, puntos[k], puntos[(k + 1) % cantpuntos])) {
                            corta_linea = 1; //hay intersepcion
                        }
                    }
                    if (!corta_linea) {
                        double dist = distancia(puntoA, puntoB);
                        if (dist > *max_distancia) {
                            *max_distancia = dist;
                            *punto1 = puntoA;
                            *punto2 = puntoB;
                            *segmento1 = encontrarSegmento(puntoA, cantpuntos, puntos);
                            *segmento2 = encontrarSegmento(puntoB, cantpuntos, puntos);
                        }
                    }
                    /* hacemos una validacion (si no hay una intercepcion), calculamos
                    las distancias entre los puntos. coparamos la distanxia maxima y la distancia
                    es mayor a la ya registrada... actualizamos los datos
                    y lo mismo con los puntos y segmentos*/
                }
            }
        }
    }

    printf("La distancia maxima encontrada es: %f\n", *max_distancia);
    printf("Se encuentra entre los puntos (%d;%d) y (%d;%d)\n", punto1->x, punto1->y, punto2->x, punto2->y);
    if (*segmento1 != -1 && *segmento2 != -1) {
        printf("el punto (%d;%d) esta en el segmento %d\n", punto1->x, punto1->y, *segmento1+1);
        printf("el punto (%d;%d) esta en el segmento %d\n", punto2->x, punto2->y, *segmento2+1);
    }
}

void guardarDatosEnArchivo(int cantpuntos, Punto puntos[], double max_distancia, Punto punto1, Punto punto2, int segmento1, int segmento2) {
    FILE *archivo;
    archivo = fopen("../resultados_poligono.txt", "w");
    // verifica si se pudo abrir el archivo
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo para escritura.\n");
        return;
    }
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
    
    if (segmento1 != -1 && segmento2 != -1) {
        fprintf(archivo, "El punto (%d,%d) está en el segmento %d\n", punto1.x, punto1.y, segmento1+1);
        fprintf(archivo, "El punto (%d,%d) está en el segmento %d\n", punto2.x, punto2.y, segmento2+1);
    }

    fclose(archivo);
    printf("\nLos resultados se han guardado en el archivo 'resultados_poligono.txt'\n");
}
 // Escribe los datos del polígono y los resultados en el archivo
void verificacionnumerodevertice() {
    // Solicita al usuario el número de vértices del polígono
    int opcion;
    int cantpuntos= 0;
    Punto puntos[MAX];
    printf("seleccione una opcion:\n");
    printf("1. ingresar coordenadas manualmente\n");
    printf("2. cargar coordenadas desde un archivo CSV\n");
    printf("ingrese su opcion: ");
    scanf("%d", &opcion);
    //desde acá comparamos la eleccion del usuario para saber como realizar la carga de las coordenadas
    if (opcion == 1) {
        cantpuntos = leerEnteroEntre(3, 30, "ingrese la cantidad de puntos (vertices) que tiene el poligono");
        ingresarCoordenadas(cantpuntos, puntos);
        /*necesitamos como minimo 3 aungulos o pares de cooordenadas
        usando mi libreria*/
    } else if (opcion == 2) {
        cargarCoordenadasDesdeCSV("coordenadas.csv", &cantpuntos, puntos);
    } else {
        printf("opcion no valida\n");
        return;
    }
    if (cantpuntos < MIN) {
        printf("se requieren al menos 3 puntos para formar un poligono\n");
        return;
    }
    // Calcula la distancia máxima y guarda los resultados
    double max_distancia;
    Punto punto1, punto2;
    int segmento1 = -1, segmento2 = -1;
    encontrarDistanciaMaxima(cantpuntos, puntos, &max_distancia, &punto1, &punto2, &segmento1, &segmento2);
    guardarDatosEnArchivo(cantpuntos, puntos, max_distancia, punto1, punto2, segmento1, segmento2);
}

int main() {
    verificacionnumerodevertice();
    return 0;
}