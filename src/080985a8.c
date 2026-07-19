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

void Func_08098698(void);
void Func_08009080(s32, s32);
void Func_080091e0(void *, s32);
void Func_080030f8(s32);
void Func_080f9010(s32);
void Func_0809748c(void);

void Func_080985a8(void) {
    s32 temp_r5;
    u8 *ptr23;
    u8 val;

    temp_r5 = M2C_FIELD(*(void **)0x03001F30, s32 *, 0x14);
    if (temp_r5 != 0) {
        Func_08098698();
        Func_08009080(temp_r5, 2);
        M2C_FIELD(temp_r5, s8 *, 0x59) = 0;
        Func_080091e0((void *) temp_r5, 0);
        ptr23 = (u8 *) (temp_r5 + 0x23);
        val = 2;
        val |= *ptr23;
        *ptr23 = val;
        Func_080030f8(0xAU);
        Func_080f9010(0x7E);
        Func_080030f8(0x28U);
        Func_0809748c();
    }
}
