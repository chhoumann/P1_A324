#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "./utility/utility.h"

recipe *make_random_weekplan(recipe *all_recipes, char *user_tags, int number_of_recipes);
recipe *discard_recipes_by_tags(recipe all_recipes[], int *recipe_matches, char* user_tags, int number_of_recipes); 
void randomizer(recipe sorted_recipes[], int recipe_matches, recipe weekly_schedule[]);
int check_tags_match (char *user_tags, char *recipe_tags);
int array_contains_int(int array[], int value, int array_size);

/* Calls the methods that sorts recipes by tags and then randomizes these */
recipe *make_random_weekplan(recipe *all_recipes, char *user_tags, int number_of_recipes) {
    printf("AAAAAAAAAAAA\n");
    int recipe_matches = 0, i;

    recipe *weekly_schedule = calloc(sizeof(recipe), DAYS_IN_WEEK); 
    recipe *sorted_recipes = discard_recipes_by_tags(all_recipes, &recipe_matches, user_tags, number_of_recipes);

    srand(time(NULL));
    randomizer(sorted_recipes, recipe_matches, weekly_schedule);
    free(sorted_recipes);

    for(i = 0; i < 7; i++)
        printf("%s %s \n", weekly_schedule[i].name, weekly_schedule[i].tags);

    return weekly_schedule;
}

/* Sorts out the recipes that match user tags and allocates them to array sort_recipes. Returns the array.  */
/* Counts the number of recipe matches via a pointer to recipes_matches */
recipe *discard_recipes_by_tags(recipe *all_recipes, int *recipe_matches, char *user_tags, int number_of_recipes) {
    int i, k;
  
    recipe *sorted_recipes = malloc(sizeof(recipe) * number_of_recipes);

    if(sorted_recipes == NULL)
        printf("Malloc error\n");
    
    for (i = 0, k = 0; i < number_of_recipes; i++) {
        if (check_tags_match(user_tags, all_recipes[i].tags)) {
            sorted_recipes[k] = all_recipes[i];
            k++;
            *recipe_matches += 1;
            printf("AAAAAAAAAAAA 2: electric boogaloo");
        }
    }

    return sorted_recipes;
}

/* Randomizes the indices in sorted_recipes and allocates 7 recipes into weekly_schedule. Duplicates are accounted for in array_contain_int */
void randomizer(recipe sorted_recipes[], int recipe_matches, recipe *weekly_schedule) {
    int i, random;
    int random_number[DAYS_IN_WEEK];

    for (i = 0; i < 7; i++) {
        random = rand() % recipe_matches;

        while (array_contains_int(random_number, random, recipe_matches))
            random = rand() % recipe_matches;

        random_number[i] = random;        
        weekly_schedule[i] = sorted_recipes[random]; 
    }
}

/* Returns 1 if given randomized index is a duplicate */
int array_contains_int(int array[], int value, int array_size) {
    int i;

    for (i = 0; i < array_size; i++)
        if (array[i] == value)
            return 1;

    return 0;
}

/* Returns 1 if recipe tags match user tags */
int check_tags_match(char *user_tags, char *recipe_tags) {
    int i, tag_matches = 0;
    int num_user_tags = strlen(user_tags);
    int num_recipe_tags = strlen(recipe_tags);

    for (i = 0; i < num_recipe_tags; i++)
        if (strchr(user_tags, recipe_tags[i]) != NULL)
            tag_matches++;

    return (tag_matches == num_user_tags);
}