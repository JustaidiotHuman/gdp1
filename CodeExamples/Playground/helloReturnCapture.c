// Call like this:
//
// bin/helloReturnCapture
//
// or
// echo "Peter" | bin/helloReturnCapture
//
// or like this
//
// cat hacked_input | bin/helloReturnCapture

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void pirate(){
	printf("You have been hacked!\n");
}

char* get_name(){
	char buf[15];

	printf("Please insert your surname (15 characters max): ");

	//scanf("%s",buf);
        // We use the function gets(), which is vurlnerable to buffer overflow
	gets(buf);

	return strdup(buf);  // return a copy of the name on the heap
}

int main() {
	char *name=NULL; 

	name = get_name();

	if (name != NULL){
		printf("\nHello %s\n", name);
		free(name);
	}
	return 0;
}

