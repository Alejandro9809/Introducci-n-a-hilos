#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
/*  Candados
    - Se declara el candado global
    - El candado se inicializa en el main
    - El candado se abre y se cierra donde cada hilo utiliza la variable compartida
        para que se sincronicen los resultados 
*/
long long sumaTotal=0;
pthread_mutex_t candado;


void suma(void *idPtr);
struct indice{
    int inicio;
    int desp;
};

int main(int cantidadargu, char *argumento[]){
    pthread_mutex_init(&candado,NULL);
    int tama=atoi(argumento[1]);
    struct indice posicion[tama];
    int arreglo[tama]; // se pasa el argumento a numero entero
    for(int i=0;i< tama ;i++){
        arreglo[i]=i+1;
    }
    printf("\n\n");
    int cant=atoi(argumento[2]);
    pthread_t hilos[cant];
    // asignar diferentes la parte proporcionada del arreglo para que cada hilo pueda hacer la suma 
    int division=tama / cant;
    int arreglo2[tama];
    if(tama%cant==0){
        for(int i=0;i<cant;i++){
            printf("\n%d\n", division);
            arreglo2[i]=division;
            posicion[i].desp=arreglo2[i];
        }
    }
    else{
        for(int i=0;i<cant;i++){
            arreglo2[i]=division;
            posicion[i].desp=arreglo2[i];
        }
        for(int i=0;i<tama%cant;i++){
            arreglo2[i]=arreglo2[i]+1;
            posicion[i].desp=arreglo2[i];
        }        
        
    }
   for (int i=0; i<cant;i++){
        if(i==0){
            posicion[i].inicio=i+1;
        }
        else{
            posicion[i].inicio=posicion[i-1].desp+posicion[i-1].inicio;
        }
    }

    for(int i=0;i<cant;i++ ){
        pthread_create(&hilos[i], NULL, (void *) suma, &posicion[i]);
    }
    for(int i=0;i<cant;i++){
        pthread_join(hilos[i], NULL);
    }

    for(int i=0;i<cant;i++){
         printf("\nEstructura %d -->inicio=%d   desplazamiento=%d \n",i,posicion[i].inicio,posicion[i].desp);
    }

    printf("\n\nEl resultado es %lld\n\n", sumaTotal);

    return 0;
}


void suma (void * idPtr) {
  struct indice * intPtr = (struct indice*) idPtr; // Convertir a apuntador a entero.
  struct indice apuntaes= *intPtr;     // Obtener valor de la variable a donde apunta.
  for(int i=1; i<=apuntaes.desp;i++){
   pthread_mutex_lock(&candado);
    sumaTotal=sumaTotal+apuntaes.inicio;
    apuntaes.inicio=apuntaes.inicio+1;
    pthread_mutex_unlock(&candado);
  }
}