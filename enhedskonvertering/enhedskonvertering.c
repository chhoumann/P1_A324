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

double convert(char *input_string);
double conversion_calculator(char *unit, double value);

int main(void){

    /* For testing */
    char ingredients[50] = "-100 g yoghurt, naturel\n";

    printf("Converted to %.2lf", convert(ingredients));

    return 0;
}

double convert(char *input_string){
    int value;
    char unit[2];

    /* Scans string for -|VALUE| |UNIT| */
    sscanf(input_string, "-%d %s", &value, &unit);

    /* Prints arrays */
    printf("Unit: %s\nValue: %d\n", unit, value);
    
    return conversion_calculator(unit, value);
}

double conversion_calculator(char *unit, double value){
    char temp;
    sscanf(unit, "%c", &temp);
    if (temp == 'g')
        return gram_to_kilogram(value);
    
    if (temp == 'k')
        return kilogram_to_gram(value);
    
    if (temp == 'l')
        return deciliter_to_liter(value);
    
    if (temp == 'l')
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