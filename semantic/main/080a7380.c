#include "types.h"

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void *Func_080048b0(s32, s32);
void Func_08002dd8(s32);
void Func_080030f8(s32);
void Func_08015278(s32);
void Func_08015408(s32, s32, s32, s32);
s32 Func_08015010(s32, s32, s32, s32, s32);
s32 Func_08077158(void *);
void Func_080a1090(s32);
void Func_080a8034(s32, s32, s32, s32);
s32 Func_080a7440(void);
void Func_080a34c0(void);

s32 Func_080a7380(void)
{
    s16 *control;
    s32 var_r2;
    s16 *var_r3;
    s32 temp_r0;
    s32 temp_r3;
    s32 temp_r5;
    s32 temp_r6;
    u8 *temp_r7;

    temp_r7 = Func_080048b0(55, 0xA70);
    control = *(s16 **)0x03001E68;
    temp_r3 = 1;
    control[2] = temp_r3;
    Func_08015408(0, 0, 30, 20);
    Func_080030f8(1);
    Func_080a1090(0);
    M2C_FIELD(temp_r7, s8 *, 0x219) = Func_08077158(temp_r7 + 0x208);
    Func_080a8034(0, 3, 0, 7);
    temp_r0 = Func_08015010(13, 0, 17, 5, 2);
    var_r3 = (s16 *)(temp_r7 + 0x152);
    M2C_FIELD(temp_r7, s32 *, 0x10C) = temp_r0;
    var_r2 = 7;
    do {
        var_r2--;
        *var_r3 = 30;
        var_r3--;
    } while (var_r2 >= 0);

    temp_r3 = 3;
    M2C_FIELD(temp_r7, s16 *, 0x220) = temp_r3;
    temp_r6 = Func_080a7440();
    temp_r5 = 0;
    Func_08015278(M2C_FIELD(temp_r7, s32 *, 0x24));
    Func_080a34c0();
    M2C_FIELD(*(void **)0x03001E68, s16 *, 4) = temp_r5;
    Func_080030f8(1);
    Func_08002dd8(55);
    return temp_r6;
}
