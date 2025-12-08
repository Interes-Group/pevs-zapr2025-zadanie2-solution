#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_help(){
    printf("Help of program journal\nUsage:\n");
}

int main(int argc, char* argv[]){
    if(argc <= 1) {
        print_help();
        return 0;
    }



    return 0;
}