//gcc -o FileIO FileIO.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main ( void )
{
    static const char filename[] = "test.txt";
    FILE *file = fopen ( filename, "r" );
    unsigned int temp;
    if ( file != NULL )
    {
        char line [ 128 ]; /* or other suitable maximum line size */
        while ( fgets ( line, sizeof line, file ) != NULL ) /* read a line */
        {
            // fputs( line, stdout ); /* write the line */
            // printf("%s",line);
            temp = strtol(line, NULL, 16);
            printf("%08x\n",temp);
           // printf("%u\n",temp);
        }
        fclose ( file );
    }
    else
    {
        perror ( filename ); /* why didn't the file open? */
    }
    return 0;
}