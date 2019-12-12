#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "../utility/utility.h"
#include "../database/database.h"
#include "../weekplan/weekplan.h"

#define MAX_GROCERIES 25
#define MAX_GROCERY_NAME 20
#define MAX_UNIT_NAME 8

int grocery_compare(const void *e1, const void *e2); 
int is_item_on_list(ingredient groceries[], char *item);
int index_item(ingredient groceries[], char *item);

int is_duplicate_grocery(ingredient *groceries, ingredient recipe_ingredient, int number_of_groceries) {
    int i;

    for (i = 0; i < number_of_groceries; i++)
        if (groceries[i].name == NULL)
            return -1;
        else if (strcmp(groceries[i].name, recipe_ingredient.name) == 0)
            if (strcmp(groceries[i].unit, recipe_ingredient.unit) == 0)
                return i;

    return -1;
}

void print_groceries(void) {
    const char *shop_list_dir = "./shopping_list.txt";
    ingredient *groceries, *groceries_buffer;
    int number_of_ingredients_in_weekplan = 0;
    int i, k, shopping_list_size = 0;
    FILE *shop_list_file = fopen(shop_list_dir, "w");

    /* Get size of array to create (number of ingredients in the weekplan) */
    for (i = 0; i < DAYS_IN_WEEK; i++)
        number_of_ingredients_in_weekplan += recipe_database[weekplan[i]].number_of_ingredients;

    /* Create array of groceries */
    groceries_buffer = calloc(sizeof(ingredient), number_of_ingredients_in_weekplan);

    /* Add all ingredients to from the recipes in the weekplan */
    for (i = 0; i < DAYS_IN_WEEK; i++) {
        recipe this_recipe = recipe_database[weekplan[i]];

        for (k = 0; k < this_recipe.number_of_ingredients; k++) {
            int recipe_index = is_duplicate_grocery(groceries_buffer, this_recipe.ingredients[k], number_of_ingredients_in_weekplan);

            groceries_buffer[shopping_list_size].amount = 0;
            ingredient this_ingredient = this_recipe.ingredients[k];

            if (recipe_index != -1)
                groceries_buffer[recipe_index].amount += this_ingredient.amount;
            else 
                groceries_buffer[shopping_list_size++] = this_ingredient;
        }
    }

    /* Create the final grocery array */
    groceries = calloc(sizeof(ingredient), shopping_list_size);

    for (i = 0; i < shopping_list_size; i++)
        groceries[i] = groceries_buffer[i];

    /* Sort groecies by amount and unit */
    qsort(groceries, shopping_list_size, sizeof(ingredient), grocery_compare);
    
    /* Print result and write to file */
    for (i = 0; i < shopping_list_size; i++) {
        ingredient grocery = groceries[i];
        
        if (strcmp(grocery.unit, "tsk") == 0 || strcmp(grocery.unit, "spsk") == 0) {
            printf("- %c%s\n", toupper(grocery.name[0]), grocery.name + 1);
            fprintf(shop_list_file, "- %c%s\n", toupper(grocery.name[0]), grocery.name + 1);
        }
        else {
            if (strcmp(grocery.unit, "stk") == 0)
                grocery.amount = ceil(grocery.amount);

            int floored_amount = (int)floor(grocery.amount);

            if (floored_amount == grocery.amount) {
                printf("- %d %s %s\n", floored_amount, grocery.unit, grocery.name);
                fprintf(shop_list_file, "- %d %s %s\n", floored_amount, grocery.unit, grocery.name);
            }
            else {
                printf("- %.2f %s %s\n", grocery.amount, grocery.unit, grocery.name);
                fprintf(shop_list_file, "- %.2f %s %s\n", grocery.amount, grocery.unit, grocery.name);
            }
        }
    }

    fclose(shop_list_file);
}

int grocery_compare(const void *e1, const void *e2) {
    ingredient E1 = *(ingredient*)e1;
    ingredient E2 = *(ingredient*)e2;    

    if (strcmp(E1.unit, "spsk") == 0 || strcmp(E1.unit, "tsk") == 0)
        return 1;
    else if (strcmp(E2.unit, "spsk") == 0 || strcmp(E2.unit, "tsk") == 0)
        return -1;

    return floor(E2.amount) - floor(E1.amount);
}