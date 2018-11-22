// This is a simple loop which will print some chars
// It is a useful way to test that the scheduler is working

#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    char c;
    int j = 0;

    if(argc < 3) {
        c = '*';
        j = 30;
    } else {
        c = *argv[1];
        j = atoi(argv[2]);
    }

    for(int i=0; i<j; i++)
    {
        printf("%c",c);
        fflush(stdout);
        usleep(10000);
    }
}
