typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
#define M2C_FIELD(addr, type, offset) (*(type)((char *)(addr) + (offset)))

s32 Func_08004080(void);
void Func_080216e8(s32, s32, s32);
void *Func_0801eadc(s32, s32, s32, s32, s32);

void *Func_08021750(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 temp_r0;
    s32 var_r0;

    temp_r0 = Func_08004080();
    var_r0 = 0;
    if (temp_r0 != 0x60) {
        Func_080216e8(arg0, temp_r0, arg1);
        var_r0 = (s32) Func_0801eadc(temp_r0, 0x80000000, arg2, arg3, arg4);
        M2C_FIELD(var_r0, u8 *, 0x15) = (u8) (M2C_FIELD(var_r0, u8 *, 0x15) | 0x20);
        M2C_FIELD(var_r0, u8 *, 0xF) = 0xFB;
    }
    return (void *) var_r0;
}
