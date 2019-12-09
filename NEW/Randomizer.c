#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RECIPES 9
#define DAYS_IN_WEEK 7

struct recipe {
    char name[100];
    char procedure[100];
    char ingredients[100];
    char tags[30];
    int time;
};
typedef struct recipe recipe;

recipe *discard_recipes_by_tags(recipe all_recipes[], int *recipe_matches, char* user_tags); 
void randomizer(recipe sorted_recipes[], int recipe_matches, recipe weekly_schedule[]);

recipe make_recipe(char *name, char *ingredients, char *procedure, char *tags, int time);
int check_tags_match (char *user_tags, char *recipe_tags);
int array_contains_int(int array[], int value, int array_size);


int main(void) {
    recipe all_recipes[MAX_RECIPES];
    recipe weekly_schedule[DAYS_IN_WEEK]; 
    recipe *sorted_recipes;
    int recipe_matches = 0;
    int i;
    /* -----------------------------------------------------------Test data------------------------------------------ */
    all_recipes[0] = make_recipe("kebabrulle", "lav den", "kebab", "&!", 20);
    all_recipes[1] = make_recipe("kebabsovs", "just do it", "sovs", "#!<", 21);
    all_recipes[2] = make_recipe("koedsovs", "bland sovs og kød", "sovs", "<-+", 15);
    all_recipes[3] = make_recipe("ulle", "bland og bland", "vandmand", "!#<-+", 44); /* € er ikke en del af ASCII så fucker op så det nye symbol er # */
    all_recipes[4] = make_recipe("Boller", "lav boller", "yes", "!<", 20);
    all_recipes[5] = make_recipe("Pizza", "lav den plz", "tomater", "-+!<", 20);
    all_recipes[6] = make_recipe("bagekartofler", "smid det vaek", "kartoffel", "&!", 20);
    all_recipes[7] = make_recipe("kebabbolle", "make it good", "kebarulleboi", "!*&", 10);
    all_recipes[8] = make_recipe("vand", "drik", "kun vand", "!*<", 25);
    /* ----------------------------------------------------------------------------------------------------------------*/

    srand(time(NULL));

    sorted_recipes = discard_recipes_by_tags(all_recipes, &recipe_matches, "!");
    randomizer(sorted_recipes, recipe_matches, weekly_schedule);
    
    for(i = 0; i < 7; i++)
        printf("%s %s \n", weekly_schedule[i].name, weekly_schedule[i].tags);

    free(sorted_recipes);

    return 0;
}

    /* Sorts out the recipes that match user tags and allocates them to array sort_recipes. Returns the array.  */
    /* Counts the number of recipe matches via a pointer to recipes_matches */
recipe *discard_recipes_by_tags(recipe *all_recipes, int *recipe_matches, char *user_tags) {
    int i, k;
  
    recipe *sorted_recipes = malloc(sizeof(recipe) * MAX_RECIPES);

    if(sorted_recipes == NULL)
        printf("Malloc error\n");
    
    for (i = 0, k = 0; i < MAX_RECIPES; i++) {
        if (check_tags_match(user_tags, all_recipes[i].tags)) {
            sorted_recipes[k] = all_recipes[i];
            k++;
            *recipe_matches += 1;
        }
    }

    return sorted_recipes;
}

/* Randomizes the indices in sorted_recipes and allocates 7 recipes into weekly_schedule. Duplicates are accounted for in array_contain_int */
/* Den vi kalder */
void randomizer(recipe sorted_recipes[], int recipe_matches, recipe *weekly_schedule) {
    int i, random;
    int *random_number;
    random_number = malloc(sizeof(int) * recipe_matches);

    if(random_number == NULL)
        printf("Malloc error in randomizer\n");

    for (i = 0; i < 7; i++) {
        random = rand() % recipe_matches;

        while (array_contains_int(random_number, random, recipe_matches))
            random = rand() % recipe_matches;

        random_number[i] = random;
        
        weekly_schedule[i] = sorted_recipes[random]; 
    }
    free(random_number);
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

    for (i = 0; i < num_recipe_tags; i++) {
        if (strchr(user_tags, recipe_tags[i]) != NULL) {
            tag_matches++;
        }
    }

    return (tag_matches == num_user_tags);
}

/* To make test data */
recipe make_recipe(char *name, char *procedure, char *ingredients, char *tag, int time) {
    recipe result;
    
    strcpy(result.name, name);
    strcpy(result.ingredients, ingredients);
    strcpy(result.procedure, procedure);
    strcpy(result.tags, tag);
    result.time = time;
    
    return result;
}
