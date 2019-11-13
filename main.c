#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum unit { g, kg };
const char *files[] = { "test", "test2" };
const char *dir = "./recipes/";

typedef struct {
    char *name;
    int amount;
    enum unit unit;
} Ingredient;

typedef struct {
    char *name;
    char *procedure;
    Ingredient *ingredients;
} Recipe;

void read_database(void);
char *get_file_directory(const char *fileName);

int main(void) {
    read_database();
    return 0;
}

void get_recipe_data(FILE *fp) {
    /* Temporary static array declarations */
    char title[1024];
    char ingredients[1024];
    char procedure[1024];

    /* Scan through the file until the symbol ; is encountered for each of the three strings */
    fscanf(fp, "%[^;]; %[^;]; %[^;];", title, ingredients, procedure);

    /* Print the result */
    printf("Title:\n- %s\n\nIngredients:\n%s\n\nProcedure:\n%s", title, ingredients, procedure);
}

char *get_file_directory(const char *fileName) {
    /* Concatenate the recipe directory with the name of the file and the .txt extension */ 
    /* Format: ./recipes/fileName.txt where the dot represents the current directory */
    char *directory_name = malloc(sizeof(char) * 80);
    strcpy(directory_name, dir);
    strcat(directory_name, fileName);
    strcat(directory_name, ".txt");

    return directory_name;
}

void read_database(void) {
    /* Get the amount of files to read based on the files array defined at the top */
    int n = sizeof(files) / sizeof(files[0]);
    int i;

    for (i = 0; i < n; i++) {
        /* Initialize file variable and get the file directory as a string */
        FILE *fp;
        char *fileName = get_file_directory(files[i]);

        /* Open the file using the directory name */
        fp = fopen(fileName, "r");

        /* Print the data from the file */
        get_recipe_data(fp);
        
        /* Close the file stream after reading */
        fclose(fp);
    }
}