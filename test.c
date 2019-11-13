#include <stdio.h>

enum unit { g, kg };

typedef struct {
    char *name;
    int amount;
    enum unit unit;
} Recipe;

void read_database(void);

int main(void) {
    return 0;
}