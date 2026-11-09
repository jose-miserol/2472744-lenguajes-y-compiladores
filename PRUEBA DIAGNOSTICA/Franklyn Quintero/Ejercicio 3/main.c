#include <stdio.h>

int main() {
    int x = 10;
    float y = 5.5;
    const char letra = 'A';

    if (x > 5) {
        for (int i = 0; i < 3; i++) {
            x += i;
        }
        return 0;
    } else {
        while (x < 5) {
            x++;
        }
        return 1;
    }
}
