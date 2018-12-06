/* warshall.c -- implementation of the Warshall algorithm for computing the
 *               matrix of the transitive closure of a finite binary relation.
 *
 * A binary relation R on a set X is a set of ordered pairs from X, i.e., some
 * subset of X^2:
 * 
 *    R = { (a, b) | a, b in X }
 *
 * A relation is called transitive iff for all a,b,c in X
 * 
 *    [(a, b) in R & (b, c) in R] -> (a, c) in R
 *
 * When R is not transitive, its transitive closure is the smallest
 * relation R* that contains R and is transitive (i.e., any missing pairs are
 * included).
 *
 * Now, if X is a finite set of of n elements, R can be (non-uniquely) represented
 * by an nxn matrix M, whose Mij entries are 1 when i is related to j, and 0 otherwise.
 *
 * The matrix of the transitive closure of R can be found generally by logically OR'ing
 * the boolean powers of M:
 *
 *    MR* = M v M^2 v M^3 v ... v M^n
 *
 * Such an algorithm has O(n^4) asymptotic running time.  The Warshall algorithm, implemented
 * below in the code, finds the matrix of the transitive closure of R in O(n^3).
 *
 * Usage: The program will prompt for the size of the set X and then ask you to build the
 *        matrix of the relation entry-by-entry, starting with all the columns in the first
 *        row, then all the columns in the second row, etc.  It will print the matrix of
 *        of your relation, and then print its transitive closure.
 *
 * javier lombillo
 * 2018-12-01
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSIZE 50  // maximum n for an nxn matrix

void print_matrix(int **, int);
void print_set(char *, int **, int);

int main(void)
{
    int i, j, k, n = 0;
    char row_string[MAXSIZE];
    int **m, **w;   // m is the user's relation matrix, w is its transitive closure

    printf( "We'll build the nxn relation matrix one row at a time. For example,\n"
            "entering the first row as 1010 will result in a 4x4 matrix.\n\n" );

    printf( "Enter row 1: " );
    fgets( row_string, MAXSIZE, stdin );

    // get the matrix size
    n = strlen( row_string ) - 1;

    // allocate storage for the n row pointers
    m = malloc( n * sizeof(m) );
    w = malloc( n * sizeof(w) );

    // storage for each matrix, layed out linearly and addressed at their respective 1st rows
    m[0] = malloc( n * n * sizeof(**m) );
    w[0] = malloc( n * n * sizeof(**w) );

    // associate each logical row with the appropriate address in memory.
    // m[0] holds the base address, and (n * i) is the ith offset address (the compiler knows the size of a column)
    for ( i = 1; i < n; ++i )
        m[i] = m[0] + n * i;

    for ( i = 1; i < n; ++i )
        w[i] = w[0] + n * i;

    // now that we have storage, let's build the rest of the matrix
    int row = 0;

    while ( 1 )
    {
        if ( row == n ) break;

        if ( row > 0 ) // we've already asked for first row
        {
            printf( "Enter row %d: ", row + 1 );
            fgets( row_string, MAXSIZE, stdin );
        }

        // parse the string
        for ( i = 0; i < MAXSIZE; ++i )
        {
            if ( row_string[i] == '\n' || row_string[i] == '\0' )
                break;

            if ( row_string[i] == '1' )
                m[row][i] = 1;
            else
                m[row][i] = 0;
        }

        ++row;
    }
        
    printf( "\nCreated a %dx%d matrix.\n", n, n );

    printf( "\nMatrix of the relation R:\n\n" );
    print_matrix( m, n );
    puts("");
    print_set( "R", m, n );

    // copy the matrix
    for ( i = 0; i < n; ++i )
        for ( j = 0; j < n; ++j )
            w[i][j] = m[i][j];

    // find the transitive closure of m
    // (algorithm from Rosen: _Discrete Mathematics and its Applications_, 7th ed)
    for ( k = 0; k < n; ++k )
        for ( i = 0; i < n; ++i )
            for ( j = 0; j < n; ++j )
                w[i][j] = w[i][j] || (w[i][k] && w[k][j]);

    printf("\nMatrix of its transitive closure R*:\n\n");
    print_matrix( w, n );
    puts("");
    print_set( "R*", w, n );

    return 0;
}

void print_matrix(int **p, int size)
{
    int i, j;

    for ( i = 0; i < size; ++i )
    {
        for ( j = 0; j < size; ++j )
            printf( " %d ", p[i][j] );
        puts("");
    }
}

void print_set(char *name, int **p, int size)
{
    int i, j;
    int offset = 97; // 0th entry in row/col corresponds to ASCII 'a' (and so on)
    int first = 1;   // state variable 

    if ( size > 26 ) return;

    printf( "%s = { ", name );

    for ( i = 0; i < size; ++i )
        for ( j = 0; j < size; ++j )
            if ( p[i][j] == 1 )
            {
                printf( "%s(%c, %c)", first ? "" : ", ", i + offset, j + offset );
                first = 0;
            }

    puts( " }" );
}
