#include "logPrint.h"
#include "uart.h"
#include <stddef.h>

extern int sec_sum(int idx);

/* Non Secure main() */
int main(void) {
    int i = 0;

    while(1)
    {
        sec_sum(i++);
    }

    return 0;
}
