// This is a simple loop which will print some chars
// It is a useful way to test that the scheduler is working

#include <stdio.h>
#include <limits.h>
#include <unistd.h>

int main(int argc, char** argv) 
{
    char c;

    if(argc < 2) c = '*';
    else c = *argv[1];

    for(int i=0; i<30; i++)
    {
        printf("%c",c);
        fflush(stdout);
        usleep(100000);
    }
}
