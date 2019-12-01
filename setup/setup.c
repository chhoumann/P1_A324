#include <stdio.h>
#include <stdlib.h>

void check_allergy(char *question, int *allergy_value);
void clear_input_buffer(void); 

int main(void) {
    int gluten_allergy, lactose_intolerance, nut_allergy;

    printf("Velkommen! Før vi går i gang, skal vi lige vide, om du har nogle allergier.");
    check_allergy("Har du glutenallergi? (y/n)", &gluten_allergy);
    check_allergy("Er du laktose intolerant? (y/n)", &lactose_intolerance);
    check_allergy("Har du nøddeallergi? (y/n)", &nut_allergy);
}

void check_allergy(char *question, int *allergy_value) {
    char input;

    printf("%s", question);
    scanf("%c", &input);

    *allergy_value = input == 'y';

    clear_input_buffer();
}

void clear_input_buffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}