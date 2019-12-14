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
int is_spoonful(char *unit_name);
int is_unit(const char source[], const char compare[]);
void format_ingredient(ingredient *ingredient);
void convert_unit(ingredient *ingredient, float conversion_rate, const char new_unit[]);

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
            ingredient this_ingredient = this_recipe.ingredients[k];
            format_ingredient(&this_ingredient);

            if (strcmp(this_ingredient.name, "vand") != 0) {            
                int recipe_index = is_duplicate_grocery(groceries_buffer, this_ingredient, number_of_ingredients_in_weekplan);

                groceries_buffer[shopping_list_size].amount = 0;

                if (recipe_index != -1)
                    groceries_buffer[recipe_index].amount += this_ingredient.amount;
                else 
                    groceries_buffer[shopping_list_size++] = this_ingredient;
            }
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
        
        if (is_spoonful(grocery.unit)) {
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

    press_any_key_to_continue();
    system("cls");
}

int is_spoonful(char *unit_name) {
    return (strcmp(unit_name, "tsk") == 0);
}

int grocery_compare(const void *e1, const void *e2) {
    ingredient E1 = *(ingredient*)e1;
    ingredient E2 = *(ingredient*)e2;    

    if (is_spoonful(E1.unit))
        return 1;
    else if (is_spoonful(E2.unit))
        return -1;

    return floor(E2.amount) - floor(E1.amount);
}

void format_ingredient(ingredient *ingredient) {
    /* Convert 1 kg to 1000 g */
    if (is_unit(ingredient->unit, "kg"))
        convert_unit(ingredient, 1000, "g");   

    /* Convert 1 l to 10 dl */
    else if (is_unit(ingredient->unit, "l"))
        convert_unit(ingredient, 10, "dl");

    /* Convert 1 spoonful to 3 teaspoons */
    else if (is_unit(ingredient->unit, "spsk"))
        convert_unit(ingredient, 3, "tsk");

    /* Convert 1 bowlful to 3 dl */
    else if (is_unit(ingredient->unit, "skaal"))
        convert_unit(ingredient, 3, "dl");
}

/* Converts ingredient.unit to new_unit and multiplies its amounbt by conversion_rate */
void convert_unit(ingredient *ingredient, float conversion_rate, const char new_unit[]) {
    ingredient->amount *= conversion_rate;
    ingredient->unit = calloc(sizeof(char), strlen(new_unit));
    
    strcpy(ingredient->unit, new_unit);
}

/* Returns true if source is the same string as compare */
int is_unit(const char source[], const char compare[]) {
    return (strcmp(source, compare) == 0);
}