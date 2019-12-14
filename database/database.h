#include <dirent.h>

#define MAX_LINE_LENGTH 1024
#define MAX_INGREDIENTS_CHARS 256
#define MAX_PROCEDURE_CHARS 2048

void       get_database(char **file_names);
char       **get_and_count_recipe_files(void);