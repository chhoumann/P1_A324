#define DAYS_IN_WEEK 7
#define MAX_RECIPE_NAME_LENGTH 50
#define MAX_TAG_BUFFER_LENGTH 32
#define MAX_RECIPES 512

/* Tags used by user preference */
#define GLUTEN_TAG '#'
#define LACTOSE_TAG '+'
#define NUT_TAG '-'
#define VEGAN_TAG '*'
#define QUICK_MEALS_TAG '>'
#define SLOW_MEALS_TAG '<'
#define HIGH_PROTEIN_TAG '!'
#define LOW_FAT_TAG '&'

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

/* Global prototypes */
void  clear_input_buffer(void);
void  make_random_weekplan(void);
void  on_invalid_input(void);
void  print_recipe(recipe recipe, float serving_size);
void  press_any_key_to_continue(void);
void  clear_screen(void);
int   is_file_empty(FILE *file);
int   get_recipe_by_file_name(char *name);
int   yes_no_prompt(void);
int   prompt_for_index_to_change(int max_index);

float get_serving_size(void);
