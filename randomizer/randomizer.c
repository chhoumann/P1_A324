#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "../utility/utility.h"
#include "../user_prefs/user_prefs.h"
#include "../weekplan/weekplan.h"

/* Prototype methods */
int *discard_recipes_by_tags(int *recipe_matches); 
void randomizer(int sorted_recipes[], int recipe_matches);
int check_tags_match(char *recipe_tags);
int array_contains_int(int array[], int value, int array_size);

/* Calls the methods that sorts recipes by tags and then randomizes these */
int make_random_weekplan(void) {
    int recipe_matches = 0, i;

    int *sorted_recipes = discard_recipes_by_tags(&recipe_matches);

    /* Sanity check in case no recipes are found with the given user preferences */
    if (recipe_matches == 0) {
        printf("Fejl - ingen opskrifter fundet med givne praeferencer!\n"
                "Proev at aendre dine praeferencer.\n");
        free(sorted_recipes);
        
        return EXIT_FAILURE;
    }

    srand(time(NULL));
    randomizer(sorted_recipes, recipe_matches);
    save_weekplan();
    
    free(sorted_recipes);
    
    return EXIT_SUCCESS;
}

/* Sorts out the recipes that match user tags and allocates them to array sort_recipes. Returns the array. */
/* Counts the number of recipe matches via a pointer to recipes_matches */
int *discard_recipes_by_tags(int *recipe_matches) {
    int i;
  
    int *sorted_recipes = calloc(sizeof(int), number_of_recipes);

    for (i = 0; i < number_of_recipes; i++) {
        if (check_tags_match(recipe_database[i].tags)) {   
            sorted_recipes[*recipe_matches] = i;     
            (*recipe_matches)++;
        }
    }

    return sorted_recipes;
}

/* Randomizes the indices in sorted_recipes and allocates 7 recipes into weekly_schedule.
   Duplicates are accounted for in array_contains_int */
void randomizer(int sorted_recipes[], int recipe_matches) {
    int i;

    for (i = 0; i < 7; i++) {
        int random = rand() % recipe_matches;

        /* We only need to account for duplicates if there are more than 7 recipes given the user preferences
           Otherwise, duplicates are bound to occur, about which which we notify the user in the printf below */
        if (recipe_matches >= 7)
            while (array_contains_int(weekplan, random, DAYS_IN_WEEK))
                random = rand() % recipe_matches;

        weekplan[i] = sorted_recipes[random];      
    }

    if (recipe_matches < 7)
        printf("Databasen indeholder lige nu ikke nok opskrifter til at give dig syv tilfaeldige opskrifter"
                " med dine nuvaerende praeferencer.\nDer vil derfor vaere nogle opskrifter i ugeplanen, "
                "der gaar igen.\n");
}

/* Returns 1 if given randomized index is a duplicate */
int array_contains_int(int array[], int value, int array_size) {
    int i;

    for (i = 0; i < array_size; i++)
        if (array[i] == value)
            return 1;

    return 0;
}

/* These are tags that, if present within the given recipe, will cause the recipe to be discarded instantly
   i.e. any recipe with gluten will be discarded if the user tags contain the gluten tag
   The name is due to the fact that if this tag is present the recipe is not usable. So if the recipe is not
   in accordance (i.e. contains gluten) with the tag, it will be discarded */
int is_not_tag(char tag) {
    return (tag == GLUTEN_TAG || tag == LACTOSE_TAG || tag == NUT_TAG
    || tag == QUICK_MEALS_TAG || tag == SLOW_MEALS_TAG);
}

/* Returns 1 if recipe tags match user tags */
int check_tags_match(char *recipe_tags) {
    int i, tag_matches = 0;
    int tags_to_match = strlen(user_prefs_tags);
    int num_recipe_tags = strlen(recipe_tags);

    /* Tags to match with the user preferences should not contain allergy tags or quick/slow tags */
    for (i = 0; i < strlen(user_prefs_tags); i++) {
        char tag = user_prefs_tags[i];
        if (is_not_tag(tag))
            tags_to_match--;
    }
    
    for (i = 0; i < num_recipe_tags; i++) {
        char tag = recipe_tags[i];
        if (is_not_tag(tag)) {
            /* Recipe contains i.e. an allergy tag and is discarded instantly */
            if (strchr(user_prefs_tags, tag) != NULL)
                return 0;
        }
        else if (strchr(user_prefs_tags, tag) != NULL)
            tag_matches++;
    }

    /* Tag matches need to match the user tags that aren't allergy tags or quick/slow meals tags 
       i.e. if a recipe contains only the low fat tag and the user has specified only this as a preference
       from the list of tags not present in "is_not_tags", both values will be 1 so the recipe is kept */
    return tag_matches == tags_to_match;
}