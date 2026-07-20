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

void *Func_080b7dd0(s32);
void Func_08009140(void *);
void Func_08009150(void *, s32, s32, s32);
s32 Func_080044d0(s32, s32);

void Func_080b8000(s32 arg0) {
    s32 var_r0; s32 zero; void *temp_r0; void *temp_r5;
    temp_r0 = Func_080b7dd0(arg0);
    temp_r5 = M2C_FIELD(temp_r0, void *, 0);
    zero = 0;
    M2C_FIELD(temp_r5, s32, 0x34) = 0x20000;
    M2C_FIELD(temp_r5, s32, 0x30) = 0x80000;
    M2C_FIELD(temp_r5, s32, 0x48) = 0xAB85;
    M2C_FIELD(temp_r5, s32, 0x28) = zero;
    M2C_FIELD(temp_r5, s32, 0x44) = zero;
    M2C_FIELD(temp_r5, s8, 0x5A) = zero;
    M2C_FIELD(temp_r5, s8, 0x58) = 1;
    Func_08009140(temp_r5);
    Func_08009150(temp_r5, M2C_FIELD(temp_r0, s32, 0xC), 0, M2C_FIELD(temp_r0, s32, 0x10));
    var_r0 = M2C_FIELD(temp_r0, s32, 0x10);
    if (var_r0 < 0) { var_r0 += 7; }
    M2C_FIELD(temp_r5, s16, 6) = (s16)(Func_080044d0(var_r0 >> 3, M2C_FIELD(temp_r0, s32, 0xC)) - 0x8000);
}
