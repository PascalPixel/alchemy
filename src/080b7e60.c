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
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_080b6e30();
void *Func_080b7dd0(s32);

s32 Func_080b7e60(s32 arg0) {
    register s32 one asm("r3");
    void *entry;

    Func_080b6e30();
    entry = Func_080b7dd0(arg0);
    one = 1;
    asm volatile("" : "+r" (one));
    M2C_FIELD(entry, s16 *, 0x28) = (s16) one;
    return 0;
}
