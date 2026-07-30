typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

s32 Func_08009248(s32, u32);
s32 Func_08097384(void);
s32 Func_080974d8(s32 *);
s32 Func_0809ba7c(s32 *, s32);
s32 Func_0809ba90(s32 *, s32, s32, s32);

void Func_0809b208(void)
{
    s32 *var_r5;
    s32 temp_r0;
    u32 var_r7;
    u32 var_r7_2;
    void *temp_r3;
    void *temp_r5;
    void *temp_r8;
    void *var_r2;
    s32 subroutine_arg0[3];

    temp_r3 = *(void **)0x03001F30;
    temp_r5 = M2C_FIELD(temp_r3, void **, 0x10);
    temp_r8 = temp_r3;
    Func_080916b0();
    Func_080933f8(-1, -1, -1, 0);
    Func_08097384();
    Func_080030f8(0xAU);
    Func_08092adc((s32)M2C_FIELD(temp_r8, s16 *, 0x18), 0x4000, 0);
    Func_080030f8(0x1EU);
    M2C_FIELD(temp_r5, s32 *, 0x6C) = 0x08096B89;
    Func_080f9010(0x83);
    Func_08009080((s32)temp_r5, 0x1C);
    Func_080030f8(0x28U);
    Func_080f9010(0xDC);
    Func_08009240((s32)temp_r5, 0);
    Func_08009080((s32)temp_r5, 3);
    M2C_FIELD(temp_r5, s32 *, 0x6C) = 0x0809B0B1;
    M2C_FIELD(temp_r5, s16 *, 0x64) = 0;
    Func_080030f8(0x46U);
    Func_080091e0(temp_r5, 0);
    M2C_FIELD(temp_r5, s8 *, 0x55) = 0;
    M2C_FIELD(temp_r5, s32 *, 0x6C) = 0x0809B0DD;
    M2C_FIELD(temp_r5, s32 *, 0x38) = 0x80000000;
    subroutine_arg0[0] = M2C_FIELD(temp_r5, s32 *, 8);
    subroutine_arg0[1] = M2C_FIELD(temp_r5, s32 *, 0xC);
    subroutine_arg0[2] = M2C_FIELD(temp_r5, s32 *, 0x10);
    Func_080974d8(subroutine_arg0);
    var_r7 = 0;
    var_r5 = temp_r8 + 0x58;
    do {
        Func_0809ba90(var_r5, 0x11C, subroutine_arg0[0], subroutine_arg0[2]);
        Func_0809ba7c(var_r5, 0x0809B11D);
        Func_0809ba70(var_r5, 7);
        Func_08009248(M2C_FIELD(var_r5, s32 *, 0),
                      (u32)(Func_08004458() * 7) >> 0x10);
        temp_r0 = (Func_08004458() >> 1) + 0x13333;
        M2C_FIELD(var_r5, s32 *, 0x2C) = temp_r0;
        M2C_FIELD(var_r5, s32 *, 0x28) = temp_r0;
        var_r7 += 1;
        Func_080030f8(1U);
        var_r5 += 0x48;
    } while (var_r7 <= 0x17U);
    Func_080030f8(0x46U);
    var_r7_2 = 0;
    var_r2 = temp_r8 + 0x98;
    do {
        if (M2C_FIELD(var_r2, s8 *, 5) != 0) {
            M2C_FIELD(var_r2, s8 *, 0) = 2;
        }
        var_r7_2 += 1;
        var_r2 += 0x48;
    } while (var_r7_2 <= 0x17U);
    Func_080030f8(0x28U);
    Func_0809748c();
    Func_080030f8(0xAU);
}
