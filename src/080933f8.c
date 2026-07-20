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

void *Func_080048f4(s32, s32);
void Func_08009140(void *);
void Func_080030f8(u32);
void Func_08009128(void);
void Func_08009150(void *, s32, s32, s32);

void Func_080933f8(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 spC;
    void *sp8;
    s32 sp4;
    s32 sp0;
    s32 temp_fp;
    s32 temp_r2;
    s32 temp_r9;
    s32 var_r6;
    s32 var_r7;
    s32 var_sl;
    void *temp_r1;
    void *temp_r5;

    var_r6 = arg0;
    var_sl = arg1;
    spC = arg3;
    var_r7 = arg2;
    sp8 = Func_080048f4(0x1B, 0xCCC);
    temp_r5 = M2C_FIELD(sp8, void **, 0x1E0);
    temp_r1 = *(void **)0x03001E70;
    sp4 = M2C_FIELD(temp_r1, s32, 0xEC) + 0x780000;
    temp_r2 = M2C_FIELD(temp_r5, s32, 0xC);
    sp0 = M2C_FIELD(temp_r1, s32, 0xF0) + temp_r2 + 0x600000;
    temp_fp = M2C_FIELD(temp_r1, s32, 0xF4) + 0xFF880000;
    temp_r9 = M2C_FIELD(temp_r1, s32, 0xF8) + temp_r2 + 0xFFC00000;
    M2C_FIELD(temp_r1, void **, 0) = (void *) (temp_r5 + 8);
    Func_08009140(temp_r5);
    if (var_r6 == -1) {
        var_r6 = M2C_FIELD(temp_r5, s32, 8);
    }
    if (var_sl == -1) {
        var_sl = M2C_FIELD(temp_r5, s32, 0xC);
    }
    if (var_r7 == -1) {
        var_r7 = M2C_FIELD(temp_r5, s32, 0x10);
    }
    if (var_r6 < sp4) {
        var_r6 = sp4;
    }
    if (var_r7 < sp0) {
        var_r7 = sp0;
    }
    if (var_r6 > temp_fp) {
        var_r6 = temp_fp;
    }
    if (var_r7 > temp_r9) {
        var_r7 = temp_r9;
    }
    if (spC == 0) {
        M2C_FIELD(temp_r5, s32, 8) = var_r6;
        M2C_FIELD(temp_r5, s32, 0xC) = var_sl;
        M2C_FIELD(temp_r5, s32, 0x10) = var_r7;
        Func_080030f8(1U);
        if (M2C_FIELD(sp8, s16, 0x19E) != 3) {
            Func_08009128();
        }
    } else {
        Func_08009150(temp_r5, var_r6, var_sl, var_r7);
    }
}
