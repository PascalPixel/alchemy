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

s32 Func_080042c8(s32);

void Func_08011984(void) {
    if ((s8) M2C_FIELD(*(void **)0x03001E70, u8 *, 0xFC) == 0) {
        Func_080042c8(0x0801179D);
    }
}
