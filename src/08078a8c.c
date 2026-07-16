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

void *Func_08078414();
u8 *Func_08078b9c(u16);

u8 Func_08078a8c(void) {
    return *Func_08078b9c(M2C_FIELD(Func_08078414(), u16 *, 0x28));
}
