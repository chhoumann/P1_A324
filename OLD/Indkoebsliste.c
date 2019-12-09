#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RECIPES 9
#define DAYS_IN_WEEK 7
#define MAX_INGREDIENTS 10
#define MAX_GROCERIES 25

struct ingredient {
    char item[20];
    int  amount;
    char unit[5];
};
typedef struct ingredient ingredient;

struct recipe {
    char name[50];
    char procedure[100];
    ingredient ingredients[MAX_INGREDIENTS];
    char tags[10];
    int time;
};
typedef struct recipe recipe;

struct grocery {
    int amount;
    char unit[7];
    char item[15];
};
typedef struct grocery grocery;

/* ------------------------------------------------------------------------------------------------------------*/

recipe make_recipe(char *name, char *ingredients[], int num_ingredients, char *procedure, char *tags, int time);
ingredient make_ingredient(char *ingredients);
void get_groceries(grocery groceries[], recipe weekly_schedule[]);
void put_ingredients_on_list(grocery groceries[], recipe weekly_schedule[], int i);
int is_item_on_list(grocery groceries[], char *item);
int index_item(grocery groceries[], char *item);
int ingredient_exists(ingredient *weekly_schedule);


int main(void) {   

    /* --------------------------------KOMMENTAR------------------------------ */

        /* Precondition for at programmet virker: */
            /* Ingredienser er et af array af struct ingredient i weekly_schedule */
            /* Enhederne skal sikkert tjekkes. Exempelvis giver det ikke mening, at en person skal købe 39 spsk pebber */

        /* Forbedringer*/
            /* Kunne sikkert forbedre hvordan der allokeres plads til ingrediens arrayet i struct recipe */
                /* Da man skal initialisere alle pladserne, ellers står der bare noget mærkeligt */
    

        
    recipe weekly_schedule[DAYS_IN_WEEK];
    grocery groceries[MAX_GROCERIES];
    int i;
   

    /* --------------------------------------Til test data, ikke en del af program ---------------------------------------------------------*/
    char *ingredient0[] = {"- 400 g kebab", "- 4 spsk pebber"}; 
    char *ingredient1[] = {"- 500 g kebab", "- 5 spsk pebber"}; 
    char *ingredient2[] = {"- 600 g kebabby", "- 6 spsk pebber"}; 
    char *ingredient3[] = {"- 700 g kebab", "- 7 spsk pebber"}; 
    char *ingredient4[] = {"- 800 g kebab", "- 8 spsk pebber"}; 
    char *ingredient5[] = {"- 900 g kebab", "- 9 hele fastelavnsboller"}; 
    char *ingredient6[] = {"- 200 g kebab"}; 

    weekly_schedule[0] = make_recipe("kebabrulle", ingredient0, 2, "lav den", "&!", 20);
    weekly_schedule[1] = make_recipe("kebabsovs", ingredient1, 2, "- just do it\n", "#!<", 21);
    weekly_schedule[2] = make_recipe("koedsovs", ingredient2, 2, "bland sovs og kød","<-+", 15);
    weekly_schedule[3] = make_recipe("ulle", ingredient3, 2, "bland og bland", "!#<-+", 44);  /*€ er ikke en del af ASCII så fucker op så det nye symbol er # */
    weekly_schedule[4] = make_recipe("Boller", ingredient4, 2, "lav boller", "!<", 20);
    weekly_schedule[5] = make_recipe("Pizza", ingredient5, 2, "lav den plz", "-+!<", 20);
    weekly_schedule[6] = make_recipe("bagekartofler", ingredient6, 1, "smid det vaek", "&!", 20);
    /* ----------------------------------------------------------------------------------------------------------------------------------------*/

    /* Test data complete */

    /* --------------Initializing groceries ----------------*/
    for(i = 0; i < MAX_GROCERIES; i++) {
        groceries[i].amount = 0;
        strcpy(groceries[i].unit, "");
        strcpy(groceries[i].item, "");
    }

    /* Input: weekly schedule. Output: groceries.*/
    get_groceries(groceries, weekly_schedule);

    /* Printing */
    for(i = 0; i < MAX_GROCERIES; i++)
        if(groceries[i].amount != 0)
            printf("%d %s %s\n", groceries[i].amount, groceries[i].unit, groceries[i].item);

    return 0;
}

/* To make test data */
recipe make_recipe(char *name, char *ingredients[], int num_ingredients, char *procedure,  char *tag, int time) {
    recipe result;
    int i = 0;

    /* Initializing ingredients array in struct recipe */
    for(i = 0; i < MAX_INGREDIENTS; i++) { 
        result.ingredients[i].amount = 0;
        strcpy(result.ingredients[i].unit, ""); 
        strcpy(result.ingredients[i].item, ""); 
    }

    strcpy(result.name, name);
    strcpy(result.procedure, procedure);
    strcpy(result.tags, tag);
    result.time = time;
    
    for(i = 0; i < num_ingredients; i++)
        result.ingredients[i] = make_ingredient(ingredients[i]);
  
    return result;
}
/* To make ingredients for test data */
ingredient make_ingredient(char *ingredients) {
    ingredient result;

    sscanf(ingredients, "%*[-] %d %s %s", &result.amount, result.unit, result.item);

    return result;
}

/* groceries is output paramenter and weekly schedule is input
 * Goes through the weekly schedule array in for loop using variable i
 * Adds each ingredient to the grocery list. If the ingredient is a duplicate, it adds the amount of the ingredient to the total on the list
 */

void get_groceries(grocery groceries[], recipe weekly_schedule[]) {
    int i;

    for(i = 0; i < DAYS_IN_WEEK; i++)
        put_ingredients_on_list(groceries, weekly_schedule, i);
}

/* For each place in weekly schedule, goes through each ingredient in while loop using variable k
 * If ingredient exists (more than 0 of), checks if it is already on the grocery list in is_item_on_list
 * If true, adds the amount of that item to the total amount in grocery arrray, if false, adds item to the grocery list with index "index"
 */ 
void put_ingredients_on_list(grocery groceries[], recipe weekly_schedule[], int index_weekly_schedule) {
    int i = index_weekly_schedule;
    int k = 0;
    static int index = 0;

    while(ingredient_exists(&weekly_schedule[i].ingredients[k])) {
            if(is_item_on_list(groceries, weekly_schedule[i].ingredients[k].item)) {
                groceries[index_item(groceries, weekly_schedule[i].ingredients[k].item)].amount += weekly_schedule[i].ingredients[k].amount;
                k++;
            }
            else {
                strcpy(groceries[index].item, weekly_schedule[i].ingredients[k].item);
                strcpy(groceries[index].unit, weekly_schedule[i].ingredients[k].unit);
                groceries[index].amount = weekly_schedule[i].ingredients[k].amount;
                index++, k++;
            }
    }
}

/* Returns true if if item is already on the grocery list */
int is_item_on_list(grocery groceries[], char *item) {
    int result, i = 0;

    do {
        result = (strcmp(groceries[i].item, item) == 0);
        i++;
    } while (result != 1 && i < MAX_GROCERIES);

    return result;
}

/* Returns index of item */
int index_item(grocery groceries[], char *item) {
    int index = 0, done;

    done = (strcmp(item, groceries[index].item) == 0);

    while(!done){
        index++;
        done = (strcmp(item, groceries[index].item) == 0);
    }

    return index;
}
/* Returns true if there is more than 0 of the ingredient in the recipe weekly_schedule */
int ingredient_exists(ingredient *ingredients) {
    return ingredients->amount != 0;
}
