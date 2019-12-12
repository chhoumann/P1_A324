typedef struct {
    int gluten_allergy;
    int lactose_intolerance;
    int nut_allergy;
    int vegan;
    int quick_meals;    /* user prefers meals that take <= 20 minutes to cook */
    int slow_meals;
    int high_protein;
    int low_fat;
} user_prefs;

user_prefs saved_user_prefs;
char *user_prefs_tags;

void check_setup(void);
void save_user_prefs(void);
void change_user_preferences();