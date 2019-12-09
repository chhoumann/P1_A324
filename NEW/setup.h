typedef struct {
    int gluten_allergy;
    int lactose_intolerance;
    int nut_allergy;
} user_prefs;

void check_setup(void);
void do_setup(void);
void save_user_prefs(user_prefs user_prefs);
void check_allergy(char *question, int *allergy_value);
user_prefs read_user_prefs_from_file(FILE *file);
void clear_input_buffer(void);