#include "exposition.h"

/* Function to remove trailing newline characters from a string */
void chomp(char *s) {
    if (s == NULL) return;
    size_t len = strlen(s);
    while (len > 0 && (s[len-1] == '\n' || s[len-1] == '\r')) {
        s[--len] = '\0';
    }
}
