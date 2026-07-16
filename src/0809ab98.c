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

s32 Func_0809abb4();

void Func_0809ab98(void) {
    M2C_FIELD(M2C_FIELD(*(void **)0x03001F30, void **, 0x14), s8 *, 0x5B) = 1;
    Func_0809abb4();
}
