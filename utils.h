#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Este #include servirá para más adelante…
#include<math.h>

typedef char cadena[50]; // Para tratar a los arrays de chars como 'cadena'

///////////////////////////////////////////////////////////////////////////
////////////////////////////  PROTOTIPOS  /////////////////////////////////
///////////////////////////////////////////////////////////////////////////

int leerEntero(cadena mensaje);
float leerFloat(cadena mensaje);
char leerCaracter(cadena mensaje);
int leerEnteroEntre(int valorMin, int valorMax, cadena mensaje);
float leerFloatEntre(float valorMin, float valorMax, cadena mensaje);
int confirmaUsuario(cadena mensaje);

///////////////////////////////////////////////////////////////////////////
/////////////////////////  IMPLEMENTACIONES  //////////////////////////////
///////////////////////////////////////////////////////////////////////////

int leerEntero(cadena mensaje) {
    /* Muestra el mensaje al usuario, lee un entero y lo retorna */
    int numero;
    printf("%s: ", mensaje);
    fflush(stdin);
    scanf("%d", &numero);
    while (numero<0)
    {
        printf("error, vuelva a ingresar un numero entero...");
        printf("%s: ", mensaje);
        fflush(stdin);
        scanf("%d", &numero);
    }
    return numero;
}

float leerFloat(cadena mensaje) {
    /* Muestra el mensaje al usuario, lee un float y lo retorna */
    float numero;
    printf("%s: ", mensaje);
    fflush(stdin); // No es obligatorio pero por las dudas...
    scanf("%f", &numero);
    return numero;
    
}

char leerCaracter(cadena mensaje) {
    /* Muestra el mensaje al usuario, lee un char y lo retorna */
    char caracter;
    printf("%s: ", mensaje);
    fflush(stdin);
    scanf(" %c", &caracter);
    while (caracter!='f' && caracter!='F' && caracter!='m' && caracter!='M')
    {
        printf("error, no has ingresado un valor valido...");
        printf("%s: ", mensaje);
        fflush(stdin);
        scanf(" %c", &caracter);
    }
    return caracter;
}

int leerEnteroEntre(int valorMin, int valorMax, cadena mensaje) {
    /* Muestra el mensaje al usuario. Luego lee enteros mientras éstos estén
       fuera del rango [valorMin, valorMax] informando al usuario de
       su error. Cuando finalmente lea un entero válido, lo retorna */
    int numero;
    numero = leerEntero(mensaje);
    while (numero < valorMin || numero > valorMax)
    {
        printf("Error, la cantidad de puntos es incorrecta rango[%d;%d]\n",valorMin,valorMax);
        numero = leerEntero(mensaje);
    }
}

float leerFloatEntre(float valorMin, float valorMax, cadena mensaje) {
    /* Muestra el mensaje al usuario. Luego, lee floats mientras éstos estén
       fuera del rango [valorMin, valorMax] informando al usuario de
       su error. Cuando finalmente lea un float válido, lo retorna */
    float numero;
    do {
        numero = leerFloat(mensaje);
    } while (numero < valorMin || numero > valorMax);
    return numero;
}

int confirmaUsuario(cadena mensaje) {
    /* Muestra el mensaje al usuario junto a la leyenda "[S/N]" que
       representa "Si" o "No". Luego, lee chars mientras éstos no sean los
       previstos ('S' o 'N', incluyendo minúsculas) informando al
       usuario de su error. Retorna si el usuario seleccionó que sí. */
    char caracter;
    printf("%s",mensaje);
    scanf(" %c", &caracter);
    while (caracter != 's' && caracter != 'n' && caracter != 'N' && caracter != 'S' )
    {
        printf ("error... ingrese una respuesta valida");
        printf("Llueve?(s/n)");
        scanf(" %c", &caracter);
    }
    return (caracter == 's') ? 1 : 0;
}   