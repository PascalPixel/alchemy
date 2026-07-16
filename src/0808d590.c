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

s32 Func_080072e4(s32);
extern u8 Data_02008000[];

void Func_0808d590(void) {
    register u8 *base asm("r3") = Data_02008000;

    Func_080072e4(M2C_FIELD(base, s32 *, 4));
}
