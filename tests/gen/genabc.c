#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("commands must contains at least one and at most one arguments\n");
        exit(-1);
    }
    int m = strlen(argv[1]);
    unsigned int seed = 0;

    for(int i=0;i<m;i++) seed += argv[1][i], seed <<= 7, seed ^= argv[1][i];

    srand(seed);
    int n = 500;

    for(int i=0;i<n;i++) printf("%c", 97+rand()%26);
}