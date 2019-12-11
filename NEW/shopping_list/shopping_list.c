#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utility/utility.h"
#include "../database/database.h"

#define DAYS_IN_WEEK 7
#define MAX_GROCERIES 25
#define MAX_GROCERY_NAME 20
#define MAX_UNIT_NAME 8

int is_item_on_list(ingredient groceries[], char *item);
int index_item(ingredient groceries[], char *item);


/* --------------------------------KOMMENTAR--------------------------------- */

        /* Precondition for at programmet virker: */
            /* Ingredienser er et af array af struct ingredient i weekplan */
            /* Enhederne skal sikkert tjekkes. Exempelvis giver det ikke mening, at en person skal købe 39 spsk pebber */

        /* Forbedringer*/
            /* Kunne sikkert forbedre hvordan der allokeres plads til ingrediens arrayet i struct recipe */
                /* Da man skal initialisere alle pladserne, ellers står der bare noget mærkeligt */
/* ----------------------------------------------------------------------------*/


ingredient *get_groceries(int *weekplan) {   

    ingredient *groceries = calloc(sizeof(ingredient), MAX_GROCERIES);
    int i, k, index = 0;
   
    /* --------------Initializing groceries ----------------*/
    for (i = 0; i < MAX_GROCERIES; i++) {
        groceries[i].amount = 0;
        groceries[i].unit = calloc(sizeof(char), MAX_UNIT_NAME);
        groceries[i].name = calloc(sizeof(char), MAX_GROCERY_NAME);
    }
    
    for (i = 0; i < DAYS_IN_WEEK; i++) {
        recipe this_recipe = recipe_database[weekplan[i]];

        for (k = 0; k < this_recipe.number_of_ingredients; k++) {
            ingredient grocery = this_recipe.ingredients[k];
            
            if(is_item_on_list(groceries, grocery.name)) {
                groceries[index_item(groceries, grocery.name)].amount += grocery.amount;
                k++;
            }
            else {
                strcpy(groceries[index].name, grocery.name);
                strcpy(groceries[index].unit, grocery.unit);
                groceries[index].amount = grocery.amount;
                index++, k++;
            }
        }
    }

    /* Printing */
    for (i = 0; i < MAX_GROCERIES; i++)
        if (groceries[i].amount != 0)
            printf("%d %s %s\n", groceries[i].amount, groceries[i].unit, groceries[i].name);

    return groceries;
}

/* Returns true if if item is already on the grocery list */
int is_item_on_list(ingredient *groceries, char *item) {
    return groceries[index_item(groceries, item)].amount > 0;
}

/* Returns index of item */
int index_item(ingredient *groceries, char *item) {
    int index = 0, done;

    done = (strcmp(item, groceries[index].name) == 0);

    while (!done) {
        index++;
        done = (strcmp(item, groceries[index].name) == 0);
    }

    return index;
}
