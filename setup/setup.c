#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 32

typedef struct {
    int gluten_allergy;
    int lactose_intolerance;
    int nut_allergy;
} user_prefs;
 
/* Prototype methods */
void do_setup(void);
void save_user_prefs(user_prefs user_prefs);
void check_allergy(char *question, int *allergy_value);
user_prefs read_user_prefs_from_file(void);
int file_exists(void);
void clear_input_buffer(void);

/* The path of the user preferences .txt file */
const char *user_prefs_file_name = "./userprefs.txt";

/* Runs the initial setup if the setup file is empty or does not exist */
int main(void) {
    if (!file_exists())
        do_setup();
    else
         read_user_prefs_from_file();
}

/* Given the user preferences file already exists, we read the preferences from there */
user_prefs read_user_prefs_from_file() {
    FILE *user_prefs_file = fopen(user_prefs_file_name, "r");
    user_prefs user_prefs;

    fscanf(user_prefs_file, "%*s %*c %d", &user_prefs.gluten_allergy);
    fscanf(user_prefs_file, "%*s %*c %d", &user_prefs.lactose_intolerance);
    fscanf(user_prefs_file, "%*s %*c %d", &user_prefs.nut_allergy);

    printf("Gluten allergy = %d\n", user_prefs.gluten_allergy);
    printf("Lactose intolerance = %d\n", user_prefs.lactose_intolerance);
    printf("Nut allergy = %d\n", user_prefs.nut_allergy);

    return user_prefs;
}

/* Prompts the user for allergies and calls the save user preferences method */
void do_setup() {
    user_prefs user_prefs;

    printf("Velkommen! Foer vi gaar i gang, skal vi lige vide, om du har nogle allergier.\n");
    check_allergy("Har du glutenallergi? (y/n) ", &user_prefs.gluten_allergy);
    check_allergy("Er du laktose intolerant? (y/n) ", &user_prefs.lactose_intolerance);
    check_allergy("Har du noeddeallergi? (y/n) ", &user_prefs.nut_allergy);

    save_user_prefs(user_prefs);
}

/* Saves user preferences to the .txt file using the input parameters */
void save_user_prefs(user_prefs user_prefs) {
    FILE *user_prefs_file = fopen(user_prefs_file_name, "w");

    fprintf(user_prefs_file, "gluten_allergy = %d\n", user_prefs.gluten_allergy);
    fprintf(user_prefs_file, "lactose_intolerance = %d\n", user_prefs.lactose_intolerance);
    fprintf(user_prefs_file, "nut_allergy = %d", user_prefs.nut_allergy);

    free(user_prefs_file);
}

/* Scans user's yes/no input in terminal and sets the allergy value to true or false */
void check_allergy(char *question, int *allergy_value) {
    char input;

    printf("%s", question);
    scanf("%c", &input);

    *allergy_value = input == 'y';

    clear_input_buffer();
}

/* Checks whether the setup file exists or not (and if it exists, checks if it's empty) */
int file_exists() {
    FILE *user_prefs_file = fopen(user_prefs_file_name, "r");

    if(user_prefs_file != NULL) {
        /* Check if file is empty */
        int file_size;
        
        fseek(user_prefs_file, 0, SEEK_END);
        file_size = ftell(user_prefs_file);

        if (file_size == 0) {
            /* File exists, but is empty */
            return 0;
        }

        /* File is not empty */
        return 1;
    }
    return 0;
}

/* Clear input buffer (we should move this to a utility header) */
void clear_input_buffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}