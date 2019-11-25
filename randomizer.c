#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

struct recipe{
    char name[50];
    char procedure[50];
    char ingredients[50];
    char tags[30];
    int time;
};
typedef struct recipe recipe;

int *randomizer(recipe all_recipes[], char *user_tags, int number_of_recipes);
recipe make_recipe(char *name, char *ingredients, char *procedure, char *tags, int time);
int tags_match (char *user_tags, char *recipe_tags);
int is_duplicate(int i, int k, int *week_plan);

int main(void){
    int *week_plan; 
    int recipe_number, i;
    int number_of_recipes = 9;
    char *user_tags = "!&";
    recipe all_recipes[9];

    srand(time(NULL));
    
    all_recipes[0] = make_recipe("kebabrulle", "lav den", "kebab", "&!&", 20);
    all_recipes[1] = make_recipe("kebabsovs", "just do it", "sovs", "€<&", 21);
    all_recipes[2] = make_recipe("kødsovs", "bland sovs og kød", "sovs", "&<-!+", 15);
    all_recipes[3] = make_recipe("ulle", "bland og bland", "vandmand", "!€<&-+", 44);
    all_recipes[4] = make_recipe("Boller", "lav boller", "", "!<&", 20);
    all_recipes[5] = make_recipe("Pizza", "lav den plz", "tomater", "&-+!<", 20);
    all_recipes[6] = make_recipe("bagekartofler", "smid det vaek", "kartoffel", "!&", 20);
    all_recipes[7] = make_recipe("kebabbolle", "make it good", "kebarulleboi", "!*&", 10);
    all_recipes[8] = make_recipe("vand", "drik", "kun vand", "!*<&", 25);
    
    printf("entered tags = %s\n", user_tags);
    week_plan = randomizer(all_recipes, user_tags, number_of_recipes);

    for (i = 0; i < 7; i++){
        recipe_number = week_plan[i]; 
        printf("name: %s\n"
               "Ingredienser: %s\n"
               "Opskrift: %s\n"
               "tags: %s\n"
               "tid: %d\n\n", all_recipes[recipe_number].name, all_recipes[recipe_number].ingredients,
                              all_recipes[recipe_number].procedure, all_recipes[recipe_number].tags, 
                              all_recipes[recipe_number].time);
    }  
    free(week_plan); 
    return 0;
}


int *randomizer(recipe all_recipes[], char *user_tags, int number_of_recipes){
    int i = 0, k, n = 0;
    int *week_plan = malloc(sizeof(int) * 7);

    for (i = 0 ; i < 7 ; i++){
        n = week_plan[i] = rand() % number_of_recipes;
        for (k = 0 ; k < 7 ; k++){
            if (is_duplicate(i, k, week_plan) || !(tags_match(user_tags, all_recipes[n].tags))){
                i--;
                break;
            }
        }
    }
    return week_plan;
}

int is_duplicate(int i, int k, int *week_plan){
    if ((i != k) && (week_plan[i] == week_plan[k]))
        return 1;
    else
        return 0;
    
    
}

int tags_match (char *user_tags, char *recipe_tags){
    int i, n = 0;
    int tag_len = strlen(user_tags);

    for(i = 0; i < strlen(recipe_tags); i++){
        if(strchr(user_tags, recipe_tags[i]) != NULL)
            n++;
    }
    if (n == tag_len)
        return 1;
    else
        return 0;
    
}

recipe make_recipe(char *name, char *procedure, char *ingredients, char *tag, int time){
    recipe result;
    
    strcpy(result.name, name);
    strcpy(result.ingredients, ingredients);
    strcpy(result.procedure, procedure);
    strcpy(result.tags, tag);
    result.time = time;
    
    return result;
}
