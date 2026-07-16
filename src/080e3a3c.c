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

s32 Func_08002dd8(s32);
s32 Func_080048b0(s32, s32);
s32 Func_080048f4(s32, s32);
s32 Func_080e3aa0(s32 *);
s32 Func_080e40a4(s32 *);
s32 Func_080e47b8(s32 *);

void Func_080e3a3c(s32 *arg0) {
    register s32 temp_r3 asm("r3");
    register u32 delta asm("r1");

    Func_080048f4(0x29, 0x60E);
    Func_080048b0(0x27, 0x782C);
    Func_080048b0(0x28, 0x4000);
    temp_r3 = *arg0;
    delta = temp_r3;
    delta -= 0x64;
    asm volatile("" : "+r" (delta) : "r" (temp_r3));
    if (delta <= 0x23U) {
        Func_080e47b8(arg0);
    } else if (temp_r3 > 0xC7) {
        Func_080e40a4(arg0);
    } else {
        Func_080e3aa0(arg0);
    }
    Func_08002dd8(0x28);
    Func_08002dd8(0x27);
    Func_08002dd8(0x29);
}
