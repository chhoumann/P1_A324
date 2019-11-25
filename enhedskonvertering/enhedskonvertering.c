#include <stdio.h>
#include <stdlib.h>
#define  CONVERSION_RATE_KILOGRAM_LITER  1000


/*
Units to convert between:
kg <-> gram | Done by division or multiplication by 1000.
L  <-> dL   | Done by division or multiplication by 1000.
*/

double gram_to_kilogram(double gram);
double kilogram_to_gram(double kilogram);

double liter_to_deciliter(double liter);
double deciliter_to_liter(double deciliter);

int main(void){

    /* Hello World */

    return 0;
}

double gram_to_kilogram(double gram) {
    return gram / CONVERSION_RATE_KILOGRAM_LITER;
}

double kilogram_to_gram(double kilogram) {
    return kilogram * CONVERSION_RATE_KILOGRAM_LITER;
}

double liter_to_deciliter(double liter) {
    return liter * CONVERSION_RATE_KILOGRAM_LITER;
}

double deciliter_to_liter(double deciliter) {
    return deciliter / CONVERSION_RATE_KILOGRAM_LITER;
}