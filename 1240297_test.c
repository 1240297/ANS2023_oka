#include <stdio.h>

union IntChar {
    int i; /* int時のメンバ名 */
    char bytes[4]; /* char時のメンバ名 */
};

int main(void) {
    union IntChar data;
    data.i = 0x12345678; /* unionのint時に 0x12345678 を代入 */

    //printf("%x : %x : %x : %x\n", data.bytes[0], data.bytes[1], data.bytes[2], data.bytes[3]);
    if(data.bytes[0] == 0x12) {
        printf("Big Endian.\n");
    } else if(data.bytes[0] == 0x78) {
        printf("Little Endian.\n");
    }

    return 0;
}