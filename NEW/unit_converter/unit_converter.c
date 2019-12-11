#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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

double conversion_calculator(char *unit, double value){
    if (strcmp(unit, "g") || strcmp(unit, "gram"))
        return gram_to_kilogram(value);
    
    if (strcmp(unit, "kg") || strcmp(unit, "kilogram") || strcmp(unit, "kilo"))
        return kilogram_to_gram(value);
    
    if (strcmp(unit, "dl") || strcmp(unit, "deciliter") || strcmp(unit, "dL"))
        return deciliter_to_liter(value);
    
    if (strcmp(unit, "l") || strcmp(unit, "L") || strcmp(unit, "liter") || strcmp(unit, "litre"))
        return liter_to_deciliter(value);
    
    return EXIT_FAILURE;
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