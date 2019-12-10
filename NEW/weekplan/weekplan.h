const char *weekplan_directory = "./saved_weekplan.txt";

int weekplan_exists(void);
recipe *load_weekplan(void);
void save_weekplan(recipe *weekplan);