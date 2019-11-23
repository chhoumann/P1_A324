#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RECIPES 10
#define MAX_TAGS 8

struct recipe{
    char name[50];
    char procedure[50];
    char ingredients[50];
    char tags[30];
    int time;
};
typedef struct recipe recipe;

void randomizer(recipe all_recipes[], int ugeplan[],char *user_tags);
recipe make_recipe(char *name, char *ingredients, char *procedure, char *tags, int time);
int check_tags_match (char *user_tags, char *recipe_tags);
int is_duplicate(int i, int k, int *ugeplan);

int main(void){
    int ugeplan[7]; 
    int placeholder, i;

    recipe all_recipes[MAX_RECIPES];

    srand(time(NULL));
    
    all_recipes[0] = make_recipe("kebabrulle", "lav den", "kebab", "&!", 20);
    all_recipes[1] = make_recipe("kebabsovs", "just do it", "sovs", "€<", 21);
    all_recipes[2] = make_recipe("kødsovs", "bland sovs og kød", "sovs", "<-!+", 15);
    all_recipes[3] = make_recipe("ulle", "bland og bland", "vandmand", "!€<-+", 44);
    all_recipes[4] = make_recipe("Boller", "lav boller", "", "!<", 20);
    all_recipes[5] = make_recipe("Pizza", "lav den plz", "tomater", "-+!<", 20);
    all_recipes[6] = make_recipe("bagekartofler", "smid det vaek", "kartoffel", "&!", 20);
    all_recipes[7] = make_recipe("kebabbolle", "make it good", "kebarulleboi", "!*&", 10);
    all_recipes[8] = make_recipe("vand", "drik", "kun vand", "!*<", 25);
    
    printf("entered tags = !");
    randomizer(all_recipes, ugeplan, "!");

    for (i = 0; i < 7; i++){
        placeholder = ugeplan[i]; 
        printf("name: %s\n"
               "Ingredienser: %s\n"
               "Opskrift: %s\n"
               "tags: %s\n"
               "tid: %d\n\n", all_recipes[placeholder].name, all_recipes[placeholder].ingredients,
                        all_recipes[placeholder].procedure,
                        all_recipes[placeholder].tags, all_recipes[placeholder].time);
    }   
    return 0;
}


void randomizer(recipe all_recipes[], int ugeplan[], char *user_tags){
    int i = 0, k, n = 0;
    
    
    for (i = 0 ; i < 7 ; i++){
        n = ugeplan[i] = rand() % MAX_RECIPES;
        for (k = 0 ; k < 7 ; k++){
            if (is_duplicate(i, k, ugeplan) || !(check_tags_match(user_tags, all_recipes[n].tags))){
                i--;
                printf("i--\n");
            }
        }
    }
}

int is_duplicate(int i, int k, int *ugeplan){
    if ((i != k) && (ugeplan[i] == ugeplan[k]))
        return 1;
    else
        return 0;
    
    
}

int check_tags_match (char *user_tags, char *recipe_tags){
    int i, n = 0;
    int tag_len = strlen(user_tags);

    printf("last user tag: %c\n", user_tags[tag_len]);
    printf("last recipe tag: %c\n", recipe_tags[strlen(recipe_tags)]);

    for(i = 0; i < strlen(recipe_tags); i++){
        if(strchr(user_tags, recipe_tags[i]) != NULL)
            n++;
    }
    if (n == tag_len){
        printf("------match found------\n");
        return 1;
    }
    else{
        printf("------mismatch found------\n");
        return 0;
    }
   
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

/*
randomizer.c: In function 'make_recipe':
randomizer.c:71:5: warning: 'result.name' is used uninitialized in this function [-Wuninitialized]
     strcpy(result.name, name);
     ^~~~~~~~~~~~~~~~~~~~~~~~~
randomizer.c:72:5: warning: 'result.ingredients' is used uninitialized in this function [-Wuninitialized]
     strcpy(result.ingredients, ingredients);
     ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
randomizer.c:73:5: warning: 'result.procedure' is used uninitialized in this function [-Wuninitialized]
     strcpy(result.procedure, procedure);
     ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
randomizer.c:74:5: warning: 'result.tags' is used uninitialized in this function [-Wuninitialized]
     strcpy(result.tags, tag);
*/
