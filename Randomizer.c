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
    printf("1\n");
    recipe *sorted_recipes = discard_recipes_by_tags(&recipe_matches);

    srand(time(NULL));
    printf("1.5\n");
    randomizer(sorted_recipes, recipe_matches);
    printf("2\n");
    save_weekplan();
    printf("3\n");
    free(sorted_recipes);
}

/* Sorts out the recipes that match user tags and allocates them to array sort_recipes. Returns the array.  */
/* Counts the number of recipe matches via a pointer to recipes_matches */
recipe *discard_recipes_by_tags(int *recipe_matches) {
    int i, k;
  
    recipe *sorted_recipes = calloc(sizeof(recipe), number_of_recipes);

    if(sorted_recipes == NULL)
        printf("Kunne ikke allokere hukommelse.\n");
    
    for (i = 0, k = 0; i < number_of_recipes; i++) {
        if (check_tags_match(recipe_database[i].tags)) {
            sorted_recipes[k] = recipe_database[i];
            k++;
            *recipe_matches += 1;
        }
    }

    return sorted_recipes;
}

/* Randomizes the indices in sorted_recipes and allocates 7 recipes into weekly_schedule. Duplicates are accounted for in array_contain_int */
void randomizer(recipe sorted_recipes[], int recipe_matches) {
    int i, random;

    
    for (i = 0; i < 7; i++) {
        random = rand() % recipe_matches;
        while (array_contains_int(weekplan, random, DAYS_IN_WEEK)){
            random = rand() % recipe_matches;
        }    

        weekplan[i] = random;
        printf("weekplan[%d] = %d", i, weekplan[i]);        
    }
    printf("Recipe matches for current settings = %d\n", recipe_matches);
}

/* Returns 1 if given randomized index is a duplicate */
int array_contains_int(int array[], int value, int array_size) {
    int i;
    printf("ARRAY CONTAINS INT\n");
    for (i = 0; i < array_size; i++){
        printf("for loop ACI\n");
        printf("array[%d] = %d", i, array[i]);
        if (array[i] == value)
            return 1;
    }

    return 0;
}

/* Returns 1 if recipe tags match user tags */
int check_tags_match(char *recipe_tags) {
    int i, tag_matches = 0;
    int num_user_tags = strlen(user_prefs_tags);
    int num_recipe_tags = strlen(recipe_tags);

    for (i = 0; i < num_recipe_tags; i++)
        if (strchr(user_prefs_tags, recipe_tags[i]) != NULL)
            tag_matches++;

    return (tag_matches == num_user_tags);
}