/* Global variable for weekplan - makes it easier to read and write from every part of the program */
int *weekplan;

/* Global prototypes */
int  weekplan_exists(void);
void load_weekplan(void);
void save_weekplan(void);
void print_weekplan_recipe(void);
void change_weekplan(void);
void new_weekplan_prompt(void);