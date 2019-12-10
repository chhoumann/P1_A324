#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "./utility/utility.h"
#include "setup.h"
#include "./database/database.h"

const char *dir_name = "./recipes/";


int main(void) {
    /* Initialize the database */
    int file_count = count_recipe_files(), i = 0;
    char *file_names[file_count];

    get_file_names(file_names);

    for (i = 0; i < file_count; i++) 
        printf("%s\n", file_names[i]);

    recipe *recipe_database = get_database(file_names, file_count);

    /* Example. Prints the name of the first recipe in the database. */
    printf("Name of the first recipe in the database is: %s.\n", recipe_database[0].name);

    /*make_random_weekplan(recipe_database, "!", 22);*/

    free(recipe_database);
    return EXIT_SUCCESS;
}
