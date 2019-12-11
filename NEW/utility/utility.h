#define DAYS_IN_WEEK 7
#define TXT_EXTENSION_LENGTH 4
#define MAX_RECIPE_NAME_LENGTH 50

/* Recipe struct and ingredient which contains all the info for each recipe */
typedef struct {
    char *name;
    char *unit;
    float amount;
} ingredient;

typedef struct {
    char *file_name;
    char *name;
    char *procedure;
    char *tags;
    ingredient *ingredients;
    int time;
} recipe;

/* The database which will be easily accessible from everywhere in the program */
recipe *recipe_database;
int number_of_recipes;

/* Utility methods */
int is_file_empty(FILE *file);
void clear_input_buffer(void);
int *make_random_weekplan(recipe *all_recipes, char *user_tags);
int get_recipe_by_file_name(char *name);
