#include <stdio.h>
#include <stdlib.h>

void check_allergy(char *question, int *allergy_value);
void clear_input_buffer(void); 

void save_user_prefs(int gluten_allergy, int lactose_intolerance, int nut_allergy) {
    FILE *user_prefs = fopen("./userprefs.txt", "w");

    fprintf(user_prefs, "gluten_allergy = %d\n", gluten_allergy);
    fprintf(user_prefs, "lactose_intolerance = %d\n", lactose_intolerance);
    fprintf(user_prefs, "nut_allergy = %d", nut_allergy);

    free(user_prefs);
}

void get_user_prefs() {
    int gluten_allergy, lactose_intolerance, nut_allergy;

    printf("Velkommen! Foer vi gaar i gang, skal vi lige vide, om du har nogle allergier.\n");
    check_allergy("Har du glutenallergi? (y/n) ", &gluten_allergy);
    check_allergy("Er du laktose intolerant? (y/n) ", &lactose_intolerance);
    check_allergy("Har du noeddeallergi? (y/n) ", &nut_allergy);

    save_user_prefs(gluten_allergy, lactose_intolerance, nut_allergy);
}

int main(void) {
    get_user_prefs();
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