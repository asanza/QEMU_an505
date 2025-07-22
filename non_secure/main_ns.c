#include "logPrint.h"
#include "uart.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

extern int sec_sum(int idx, int m);

/* Non Secure main() */
int main(void) {
    int i = 0;

    i = sec_sum(i, 1);
    printf("Hello from non-secure world: %d\n", i);
    exit(0);
}
