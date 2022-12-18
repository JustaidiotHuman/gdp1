// Demonstrate effect of
// # Disable ASLR for the current session as user root
// echo 0 | tee /proc/sys/kernel/randomize_va_space
//
// # Enable ASLR again for the current session as user root
// echo 1 | tee /proc/sys/kernel/randomize_va_space

#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("main starts at %p\n", main);
    return 0;
}

