#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "./utility/utility.h"
#include "./user_prefs/user_prefs.h"
#include "./database/database.h"
#include "./weekplan/weekplan.h"
#include "./shopping_list/shopping_list.h"

void exit_program(void);
void weekplan_menu(void);
void main_menu(void);
void on_invalid_input(void);
void new_weekplan_prompt(void);

int main(void) {
    /* Initialize the database */
    number_of_recipes = count_recipe_files();
    char *file_names[number_of_recipes];

    get_file_names(file_names);
    recipe_database = get_database(file_names);

    if (weekplan_exists())
        load_weekplan();
    else 
        weekplan = calloc(sizeof(int), DAYS_IN_WEEK); /* Memory is allocated here beacause else it would only be allocated in load weekplan() */

    check_setup();
    main_menu();

    /* Program should exit from the main menu function (4) so something is wrong if we get to this! */
    printf("\nEXIT FAILURE");
    return EXIT_FAILURE;
}

void main_menu(void) {
    int menu_selector = 0;
    system("cls");

    while (menu_selector != 4) {
        int scanres = 0;

        printf("1. Settings \n2. Ugeplan \n3. Vis indkoebsliste \n4. Exit\n");
        scanres = scanf("%d", &menu_selector);

        switch(menu_selector) {
            case 1: change_user_preferences(); break;
            case 2: weekplan_menu();           break;
            case 3: print_groceries();         break;
            case 4: exit_program();
            default: on_invalid_input();
        }
        menu_selector = 0;

        if (scanres == 0)
            clear_input_buffer();
    }
}

void weekplan_menu(void) {
    int menu_selector = 0;
    system("cls");

    while(menu_selector != 4) {
        int scanres = 0;

        printf("1. Generer ny madplan \n2. Vis opskrifter \n3. Rediger \n4. Return to main menu \n");
        scanres = scanf("%d", &menu_selector);
        
        switch (menu_selector) {
            case 1:  new_weekplan_prompt();   break;
            case 2:  print_weekplan_recipe(); break;
            case 3:  change_weekplan();       break;
            case 4:  main_menu();             break;
            default: on_invalid_input();
        }
        menu_selector = 0;

        if (scanres == 0)
            clear_input_buffer();
    }
}

void new_weekplan_prompt(void) {
    char choice = 0;
    printf("Hallo\n"); /* TESTING */
    if (weekplan_exists()) {
        printf("Er du sikker paa, at du vil generer en ny madplan og overskrive den nuvaerende? (y/n)\n");
        scanf(" %c", &choice);
        do {
            if (choice == 'y') {
                make_random_weekplan();
                printf("Success - ny madplan.\nFoerste dag skal du have: %s\n", recipe_database[weekplan[0]].name);
                press_any_key_to_continue();
            }
            else if (choice != 'n' && choice != 'y')
                on_invalid_input();
            
            clear_input_buffer();
        } while (choice != 'y' && choice != 'n');
    } 
    else {
        printf("Made new weekplan successfully\n");
        make_random_weekplan();
        printf("YES\n");
        press_any_key_to_continue();
    }

    weekplan_menu();
}

void exit_program(void) {
    free(recipe_database);
    exit(EXIT_SUCCESS);
}