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
    number_of_recipes = count_recipe_files();
    char *file_names[number_of_recipes];
    char *user_tags = "!";

    get_file_names(file_names);

    recipe_database = get_database(file_names);
    
    /* Example. Prints the index at which the recipe "Wok" is fine. */
    printf("Val: %d\n", get_recipe_by_file_name("Wok"));

    /* Example. Prints the name of the first recipe in the database. */
    printf("Name of the first recipe in the database is: %s.\n", recipe_database[0].name);

    /* Example on how to save a weekplan and randomizing it */
    save_weekplan(make_random_weekplan(recipe_database, user_tags));

    load_weekplan();

    /*make_random_weekplan(recipe_database, "!", 22);*/

    free(recipe_database);

    return EXIT_SUCCESS;
}