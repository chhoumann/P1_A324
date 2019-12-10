void       get_file_names(char **file_names);
int        count_recipe_files(void);
int        count_ingredients_from_file(FILE *fp);
char       *get_file_directory(const char *file_name);
recipe     *get_database(char **file_names, int file_count);
ingredient *get_ingredients(FILE *fp);