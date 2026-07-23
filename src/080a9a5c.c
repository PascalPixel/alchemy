typedef unsigned char u8;
typedef signed int s32;

extern u8 *Data_03001f2c;
extern u8 Value_00000b24;

extern void Func_08077008(s32);
extern void Func_080a9cbc(void);
extern void Func_080a345c(void);
extern void Func_08015080(void *, s32, s32, s32);
extern s32 Func_080a9aec(s32, void *);
extern void Func_080030f8(s32);
extern void Func_080a3e28(void *, s32);
extern s32 Func_080a9c18(void *);

void Func_080a9a5c(s32 arg0, s32 arg1, s32 arg2)
{
    u8 *state = Data_03001f2c;
    u8 *object;

    Func_08077008(arg1);
    Func_080a9cbc();
    Func_080a345c();
    Func_08015080(&Value_00000b24, arg0, 0, 0);
    Func_08015080(&Value_00000b24 + 1, arg0, 0, 32);
    Func_08015080(&Value_00000b24 + 2, arg0, 0, 16);
    Func_08015080(&Value_00000b24 + 3, arg0, 0, 48);
    object = state + 456;
    Func_080a9aec(arg0, object);
    if (arg2 == 0) {
        Func_080030f8(1);
        Func_080a3e28(object, 1);
        Func_080a9c18(object);
    }
}
