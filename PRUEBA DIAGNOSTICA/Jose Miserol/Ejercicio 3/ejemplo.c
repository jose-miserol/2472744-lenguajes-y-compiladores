#include <stdio.h>

/* Suma los primeros n numeros. Aqui dice int y while pero es un comentario. */
static int suma_hasta(int n)
{
    int total = 0;
    int i = 1;

    while (i <= n) {
        if (i % 2 == 0) {
            total += i;
        } else {
            continue;
        }
        i++;
    }
    return total;
}

int main(void)
{
    unsigned long contador = 0;
    char letra = 'i';

    for (contador = 0; contador < 3; contador++) {
        printf("for int while: %d\n", suma_hasta(10));
    }

    switch (letra) {
        case 'i':
            break;
        default:
            break;
    }
    return 0;
}
