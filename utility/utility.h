#define DAYS_IN_WEEK 7
#define TXT_EXTENSION_LENGTH 4
#define MAX_RECIPE_NAME_LENGTH 50
#define MAX_TAGS 8

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
    int number_of_ingredients;
} recipe;

/* The database which will be easily accessible from everywhere in the program */
recipe *recipe_database;
int number_of_recipes;

/* Utility methods */
int is_file_empty(FILE *file);
void clear_input_buffer(void);
void make_random_weekplan(void);
int get_recipe_by_file_name(char *name);
int yes_no_prompt(void);
void on_invalid_input(void);
void print_recipe(recipe recipe);
void press_any_key_to_continue(void);