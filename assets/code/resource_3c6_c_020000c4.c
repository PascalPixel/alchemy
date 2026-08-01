typedef unsigned char u8;
typedef signed int s32;

extern u8 Value_000028be;
extern void Func_02001696(s32);
extern s32 Func_020016a6(s32, s32);
extern s32 Func_02001626(s32, s32);
extern void Func_02001610(s32);
extern void Func_020016b6(s32);
extern void Func_020016be(s32);
extern void Func_020016d6(s32, s32);

void Func_020000c4(s32 object)
{
    s32 cue = (s32) &Value_000028be;
    Func_02001696(cue);
    Func_020016a6(object, 0);
    if (Func_02001626(0, 0) == 0) {
        Func_02001610(10);
        Func_020016b6(cue + 1);
    } else {
        Func_020016be(cue + 2);
    }
    Func_020016d6(object, 0);
}
