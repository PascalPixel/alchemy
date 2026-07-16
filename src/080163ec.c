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

s32 Func_080030f8(s32);

void Func_080163ec(void *arg0) {
    if (!(2 & M2C_FIELD(arg0, u16 *, 0x16)) && (M2C_FIELD(arg0, s16 *, 0x1A) != 0)) {
        do {
            Func_080030f8(1);
        } while (M2C_FIELD(arg0, s16 *, 0x1A) != 0);
    }
}
