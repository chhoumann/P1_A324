#define MAX_NAME_LENGTH 50
#define MAX_LINE_LENGTH 1024
#define MAX_INGREDIENTS_CHARS 256
#define MAX_PROCEDURE_CHARS 2048
#define MAX_TAGS 10

void       get_file_names(char **file_names);
int        count_recipe_files(void);
int        count_ingredients_from_file(FILE *fp);
char       *get_file_directory(const char *file_name);
recipe     *get_database(char **file_names, int file_count);
ingredient *get_ingredients(FILE *fp);