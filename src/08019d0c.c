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
#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void Func_08019d0c(void) {
    s16 *base = (s16 *)*(void **)0x03001E8C;
    s32 i = 0x976;
    do {
        base[i] = 0x3E7;
        i++;
    } while (i != 0x978);
}
