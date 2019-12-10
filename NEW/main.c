#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "./utility/utility.h"
#include "setup.h"
#include "./database/database.h"
#include "./weekplan/weekplan.h"

int main(void) {
    /* Initialize the database */
    int file_count = count_recipe_files();
    char *file_names[file_count];

    get_file_names(file_names);

    recipe *recipe_database = get_database(file_names, file_count);

    /* Example. Prints the index at which the recipe "Wok" is fine. */
    /*printf("Val: %d", get_recipe_by_name("Wok", recipe_database, file_count));*/

    /* Example. Prints the name of the first recipe in the database. */
    printf("Name of the first recipe in the database is: %s.\n", recipe_database[0].name);

    /*make_random_weekplan(recipe_database, "!", 22);*/

    free(recipe_database);
    return EXIT_SUCCESS;
}