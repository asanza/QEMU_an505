#include "logPrint.h"
#include "uart.h"
#include <stddef.h>

extern int sec_sum(int idx, int m);

/* Non Secure main() */
int main(void) {
    int i = 0;

    while(1)
    {
        i = sec_sum(i, 1);
    }

    return 0;
}
