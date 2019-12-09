#define DAYS_IN_WEEK 7

/* Recipe struct and ingredient which contains all the info for each recipe */
typedef struct {
    char *name;
    char *unit;
    float amount;
} ingredient;

typedef struct {
    char *name;
    char *procedure;
    char *tags;
    ingredient *ingredients;
    int time;
} recipe;

/* Utility methods */
int is_file_empty(FILE *file);
void clear_input_buffer(void);
recipe *make_random_weekplan(recipe *all_recipes, char *user_tags, int number_of_recipes);