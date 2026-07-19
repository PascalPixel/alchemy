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

void Func_08016418(s32, s32);
s32 Func_08017394(s32);
void Func_080030f8(s32);
void Func_08003f3c(u16);
void Func_08002dd8(s32);

void Func_0801c954(void) {
    u8 *r5;
    u8 *r6;
    u16 *p;

    r5 = *(u8 **)0x03001E9C;
    Func_08016418(M2C_FIELD(r5, s32 *, 0xFF4), 0);
    while (Func_08017394(M2C_FIELD(r5, s32 *, 0xFF4)) == 0) {
        Func_080030f8(1);
    }
    if (M2C_FIELD(r5, u16 *, 0x46) != 0) {
        Func_08003f3c(M2C_FIELD(r5, u16 *, 0x48));
    }
    p = (u16 *)(r5 + 0x352);
    if (*p != 0) {
        p = (u16 *)((u8 *)p + 2);
        Func_08003f3c(*p);
    }
    Func_08002dd8(0x13);
}
