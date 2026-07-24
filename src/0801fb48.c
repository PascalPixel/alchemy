typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

extern u8 Value_00000014;
extern u8 Value_00000017;

s32 Func_08017658(s32, s32, s32, s32);
s32 Func_08017364(void);
void Func_080030f8(s32);
s32 Func_08028df4(s32, s32, s32, s32);
void Func_08019a54(void);
void Func_080f9010(s32);
s32 Func_0801faa8(void);
void Func_0801776c(s32, s32);

s32 Func_0801fb48(void)
{
    s32 result;

    Func_08017658((s32)&Value_00000014, 8, 12, 2);
    while (Func_08017364() == 0) {
        Func_080030f8(1);
    }
    if (Func_08028df4(1, 0, 0, 1)) {
        Func_08019a54();
    } else {
        Func_08019a54();
        Func_080f9010(85);
        result = Func_0801faa8();
        if (result >= 0) {
            Func_0801776c((s32)&Value_00000017, 1);
        }
    }
    return result;
}
