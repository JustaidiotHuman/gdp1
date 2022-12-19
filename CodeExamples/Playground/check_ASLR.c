// Demonstrate effect of
// # Disable ASLR for the current session
// echo 0 | sudo tee /proc/sys/kernel/randomize_va_space
//
// # Enable ASLR again for the current session
// echo 1 | sudo tee /proc/sys/kernel/randomize_va_space

#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("main starts at %p\n", main);
    return 0;
}

