typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
#define NULL ((void *)0)

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
