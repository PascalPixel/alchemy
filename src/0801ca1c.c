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
s32 Func_080022fc(s32, s32);
extern u8 Data_02000240[];
extern u8 Data_08036750[];
void Func_0801ca1c(void *arg0) {
    s16 temp_r5; s32 temp_sl; s32 var_r2, var_r6, var_r7;
    temp_r5 = Func_080022fc(Data_02000240[0x205] + 0xC, 0x18) * 4;
    temp_sl = Data_02000240[0x206] - 7;
    var_r6 = Data_08036750[(s16) Func_080022fc(temp_r5, 0x60)];
    var_r7 = Data_08036750[Func_080022fc(temp_r5 + 0x20, 0x60)];
    var_r2 = Data_08036750[Func_080022fc(temp_r5 + 0x40, 0x60)];
    var_r6 += temp_sl; var_r7 += temp_sl; var_r2 += temp_sl;
    if (var_r6 < 0) var_r6 = 0; if (var_r7 < 0) var_r7 = 0; if (var_r2 < 0) var_r2 = 0;
    if (var_r6 > 0x1F) var_r6 = 0x1F; if (var_r7 > 0x1F) var_r7 = 0x1F; if (var_r2 > 0x1F) var_r2 = 0x1F;
    M2C_FIELD(arg0, s16 *, 0x576) = (s16) var_r6;
    M2C_FIELD(arg0, s16 *, 0x578) = (s16) var_r7;
    M2C_FIELD(arg0, s16 *, 0x57A) = (s16) var_r2;
}
