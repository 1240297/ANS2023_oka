#include <stdio.h>

union data {
    int i; /* int の時のメンバ名 */
    double d; /* double の時のメンバ名 */
};

int main (void) {
    union data d;
    d.i = 12;
    printf ("%d\n", d.i); /* 12 */
    d.d = 34.56; /* d.i は上書きされる */
    printf ("%f\n", d.d); /* 34.56 */
    printf ("%d\n", d.i); /* 変な結果となる */
    
    return 0;
}