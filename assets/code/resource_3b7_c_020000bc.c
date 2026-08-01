typedef unsigned char u8;
typedef signed int s32;

extern u8 Value_00000e19;
extern void Func_02001a2a(s32);
extern s32 Func_02001a3a(s32, s32);
extern s32 Func_02001a0a(s32, s32);
extern void Func_020019f4(s32);
extern void Func_02001a4a(s32);
extern void Func_02001a52(s32);
extern void Func_02001a6a(s32, s32);

void Func_020000bc(s32 object)
{
    s32 cue = (s32) &Value_00000e19;
    Func_02001a2a(cue);
    Func_02001a3a(object, 0);
    if (Func_02001a0a(0, 0) == 0) {
        Func_020019f4(10);
        Func_02001a4a(cue + 1);
    } else {
        Func_02001a52(cue + 2);
    }
    Func_02001a6a(object, 0);
}
