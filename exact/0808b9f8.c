#include "types.h"

extern u8 *Data_03001ebc;

void Func_0808b9f8(void) {
    s32 *current;
    s32 offset;
    s32 zero;
    s32 count;
    offset = 0x8C;
    zero = 0;
    count = 0x41;
    current = (s32 *)(Data_03001ebc + offset * 2);
    do {
        count--;
        *current = zero;
        current--;
    } while (count >= 0);
}
