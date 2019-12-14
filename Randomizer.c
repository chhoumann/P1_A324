#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "./utility/utility.h"
#include "./user_prefs/user_prefs.h"
#include "./weekplan/weekplan.h"

recipe *discard_recipes_by_tags(int *recipe_matches); 
void randomizer(recipe sorted_recipes[], int recipe_matches);
int check_tags_match(char *recipe_tags);
int array_contains_int(int array[], int value, int array_size);

/* Calls the methods that sorts recipes by tags and then randomizes these */
void make_random_weekplan(void) {
    int recipe_matches = 0, i;

    recipe *sorted_recipes = discard_recipes_by_tags(&recipe_matches);

    if (recipe_matches == 0) {
        printf("Fejl - ingen opskrifter fundet med givne praeferencer!\n"
                "Proev at aendre dine praeferencer.\n");
        free(sorted_recipes);
        return;
    }

    srand(time(NULL));
    randomizer(sorted_recipes, recipe_matches);
    save_weekplan();
    
    free(sorted_recipes);
}

/* Sorts out the recipes that match user tags and allocates them to array sort_recipes. Returns the array.  */
/* Counts the number of recipe matches via a pointer to recipes_matches */
recipe *discard_recipes_by_tags(int *recipe_matches) {
    int i;
  
    recipe *sorted_recipes = calloc(sizeof(recipe), number_of_recipes);

    for (i = 0; i < number_of_recipes; i++) {
        if (check_tags_match(recipe_database[i].tags)) {         
            sorted_recipes[*recipe_matches] = recipe_database[i];     
            (*recipe_matches)++;
        }
    }

    return sorted_recipes;
}

/* Randomizes the indices in sorted_recipes and allocates 7 recipes into weekly_schedule. Duplicates are accounted for in array_contain_int */
void randomizer(recipe sorted_recipes[], int recipe_matches) {
    int i, random;

    for (i = 0; i < 7; i++) {
        random = rand() % recipe_matches;

        if (recipe_matches >= 7)
            while (array_contains_int(weekplan, random, DAYS_IN_WEEK))
                random = rand() % recipe_matches;

        weekplan[i] = random;      
    }

    if (recipe_matches < 7)
        printf("Databasen indeholder lige nu ikke nok opskrifter til at give dig syv tilfaeldige opskrifter"
                " med dine nuvaerende praeferencer.\nDer vil derfor vaere nogle opskrifter i ugeplanen, "
                "der gaar igen.\n");
    else 
        printf("Success - der er blevet genereret en ny madplan.\n");
    /*printf("Recipe matches for current settings = %d\n", recipe_matches);*/
}

/* Returns 1 if given randomized index is a duplicate */
int array_contains_int(int array[], int value, int array_size) {
    int i;

    for (i = 0; i < array_size; i++)
        if (array[i] == value)
            return 1;

    return 0;
}

int is_not_tag(char tag) {
    return (tag == GLUTEN_TAG || tag == LACTOSE_TAG || tag == NUT_TAG
    || tag == QUICK_MEALS_TAG || tag == SLOW_MEALS_TAG);
}

/* Returns 1 if recipe tags match user tags */
int check_tags_match(char *recipe_tags) {
    int i, tag_matches = 0;
    int tags_to_match = strlen(user_prefs_tags);
    int num_recipe_tags = strlen(recipe_tags);

    for (i = 0; i < strlen(user_prefs_tags); i++) {
        char tag = user_prefs_tags[i];
        if (is_not_tag(tag))
            tags_to_match--;
    }
    
    for (i = 0; i < num_recipe_tags; i++) {
        char tag = recipe_tags[i];
        if (is_not_tag(tag)) {
            if (strchr(user_prefs_tags, tag) != NULL)
                return 0;
        }
        else if (strchr(user_prefs_tags, tag) != NULL)
            tag_matches++;
    }

    return tag_matches == tags_to_match;
}