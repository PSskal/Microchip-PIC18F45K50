/* 
 * File:   maincode.c
 * Author: HOME
 *
 * Created on 24 de mayo de 2023, 02:34 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "cabecera.h"
#define _XTAL_FREQ 4000000UL

#include <htc.h>
#include <math.h>

#define NUM_FEATURES 2
#define NUM_CLASSES 2

typedef struct {
    float weights[NUM_FEATURES];
} neuron_t;

typedef struct {
    neuron_t neurons[NUM_CLASSES];
} layer_t;

typedef struct {
    layer_t layers[1];
} network_t;

void CONFIG_Init(void){
    OSCCON = 0x52;
    TRISAbits.RA0 = 0;
    ANSELAbits.ANSA0 = 0;
    TRISAbits.RA1 = 0;
    ANSELAbits.ANSA1 = 0;
}

int main(int argc, char** argv) {
    CONFIG_Init();
    float input[NUM_FEATURES];
    float output[NUM_CLASSES];
    network_t network;
    int i, j;

    // Inicializar los pesos de la red neuronal
    for (i = 0; i < NUM_CLASSES; i++) {
        for (j = 0; j < NUM_FEATURES; j++) {
            network.layers[0].neurons[i].weights[j] = (float) rand() / RAND_MAX;
        }
    }

    // Leer entradas y producir una salida
    input[0] = 0.5;
    input[1] = 0.7;

    for (i = 0; i < NUM_CLASSES; i++) {
        output[i] = 0;
        for (j = 0; j < NUM_FEATURES; j++) {
            output[i] += input[j] * network.layers[0].neurons[i].weights[j];
        }
        output[i] = 1.0 / (1.0 + exp(-output[i]));
    }

    // Clasificar la entrada según la salida
    if (output[0] > output[1]) {
        // Entrada clasificada como clase 1
        LATAbits.LA0 = 1;
    } else {
        // Entrada clasificada como clase 2
        LATAbits.LA1 = 1;
    }
    return (EXIT_SUCCESS);
}

