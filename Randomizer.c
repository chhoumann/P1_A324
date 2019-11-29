/*
***** Generelle kommentarer: *****
    Mellemrum før curly brackets
    Single line skal ikke have curly brackets, fx
    for ...
        if ...
            do();
    
    ^ ingen curly brackets i sådan en opstilling
    
    Lidt inconsistent spacing

    Vi har rettet disse layout-mæssige problematikker, men prøv at følge programmeringsstilen
    til næste gang.

    SE PROGRAMMERINGSSTIL DOKUMENT PÅ DOCS
    https://docs.google.com/document/d/12wPTjQHCf06Bhq661h02eJvBM_kOLz0qYBSiaaM2az0/edit
*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RECIPES 9

struct recipe {
    char name[100];
    char procedure[100];
    char ingredients[100];
    char tags[30];
    int time;
};
typedef struct recipe recipe;

void randomizer(recipe all_recipes[], recipe *weekly_schedule, char *user_tags);
recipe make_recipe(char *name, char *ingredients, char *procedure, char *tags, int time);
int check_tags_match (char *user_tags, char *recipe_tags);
int array_contains_int(int array[], int value, int array_size);

int main(void) {
    recipe weekly_schedule[7]; 
    recipe all_recipes[MAX_RECIPES];

    srand(time(NULL));

    /* Test data */
    all_recipes[0] = make_recipe("kebabrulle", "lav den", "kebab", "&!", 20);
    all_recipes[1] = make_recipe("kebabsovs", "just do it", "sovs", "€!<", 21);
    all_recipes[2] = make_recipe("kødsovs", "bland sovs og kød", "sovs", "<-+", 15);
    all_recipes[3] = make_recipe("ulle", "bland og bland", "vandmand", "!€<-+", 44); /* € er ikke en del af ASCII så fucker op så det nye symbol er # */
    all_recipes[4] = make_recipe("Boller", "lav boller", "yes", "!<", 20);
    all_recipes[5] = make_recipe("Pizza", "lav den plz", "tomater", "-+!<", 20);
    all_recipes[6] = make_recipe("bagekartofler", "smid det vaek", "kartoffel", "&!", 20);
    all_recipes[7] = make_recipe("kebabbolle", "make it good", "kebarulleboi", "!*&", 10);
    all_recipes[8] = make_recipe("vand", "drik", "kun vand", "!*<", 25);
    
    /* Randomizes the recipe for each day and prints to a new txt file */

    /*    
    Randomizer og frasortering af opskrifter burde være to forskellige funktioner:

    recipe *sorted_recipes = discard_recipes_by_tags();
    randomizer(sorted_recipes, weekly_schedule);

    (TOPDOWN PROGRAMMERING OG TRINVIS FORFINELSE -- KURT)    
    */

    randomizer(all_recipes, weekly_schedule, "!");
    
    return 0;
}

/* Associates weekly_schedule[dayofweek] to the index of a recipe */
/* Den vi kalder */
void randomizer(recipe all_recipes[], recipe *weekly_schedule, char *user_tags) {
    int i, match_counter = 0, k;
    int random_number[7];
    int random;
    recipe *sorted_recipes;

    /* Sorterer opskrifter fra først. Tæl matches op først, og lav et array på den størrelse */
    for (i = 0; i < MAX_RECIPES; i++)
        if (check_tags_match(user_tags, all_recipes[i].tags))
            match_counter++;

    /* 
    Forslag til ændring af ovenstående funktion, så vi slipper for det ekstra if statement 
    Send en pointer en til match_counter ind i check_tags_match og tæl dennes værdi op med 1 
    i stedet for at returnere 1 eller 0 og tælle en ny variabel op:
    
    for (i = 0; i < MAX_RECIPES; i++)
        check_tags_match(user_tags, all_recipes[i].tags, &match_counter);
    */

    /* 
    Tjek om det er nødvendigt med typecasting. 
    Det burde stadig virke uden (recipe*)-typecasting 
    
    Hint: Det er ikke nødvendigt */
    sorted_recipes = (recipe*)malloc(sizeof(recipe) * match_counter);
    
    /* 
        Det burde være muligt at kun kalde check_tags_match én gang i for løkken i starten af funktionen.
        I stedet har i valgt to forskellige for løkker, der begge kalder check_tags_match, som også er en for løkke.
        Det er ikke optimalt, da vi taler om O(n^4), som kunne forkortes til O(n^2), hvilket er en markant forskel.
    */
    for (i = 0, k = 0; i < MAX_RECIPES; i++) {
        if (check_tags_match(user_tags, all_recipes[i].tags)) {
            sorted_recipes[k] = all_recipes[i];
            k++;
        }
    }   

    for (i = 0; i < 7; i++) {
        random = rand() % match_counter;

        while (array_contains_int(random_number, random, match_counter))
            random = rand() % match_counter;
        
        random_number[i] = random;
        /* weekly_schedule[i] = random */

        printf("random number[%d]: %d\n", i, random);
    }

    /*
    Der er ikke nogen grund til at have endnu en for-løkke.
    
    I kan bare assigne weekly_schedule[i] = random;
    Der er for mange løkker i spil for én enkel assignment.
    */
    for (i = 0; i < 7; i++) {
        k = random_number[i];
        weekly_schedule[i] = sorted_recipes[k];
    }
    
    for (i = 0; i < 7; i++)
        printf("\nname: %s \ntags: %s\n", weekly_schedule[i].name, weekly_schedule[i].tags);
    
    printf("\n");
}

int array_contains_int(int array[], int value, int array_size) {
    int i;

    for (i = 0; i < array_size; i++)
        if (array[i] == value)
            return 1;

    return 0;
}

/* Checks whether the tags in the randomized recipe mathes the user inserted tags. Return 1 if they match */
int check_tags_match(char *user_tags, char *recipe_tags) {
    int i, n = 0;
    int num_user_tags = strlen(user_tags);

    for (i = 0; i < strlen(recipe_tags); i++)
        if (strchr(user_tags, recipe_tags[i]) != NULL)
            n++;
            
    return n == num_user_tags;
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