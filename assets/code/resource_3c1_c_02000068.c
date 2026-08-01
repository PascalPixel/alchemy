typedef unsigned char u8;
typedef signed int s32;

extern u8 Value_000025b8;
extern s32 Func_02000332();
extern s32 Func_02000342(s32, s32);
extern void Func_02000334(s32);
extern void Func_02000352(s32);
extern void Func_0200035a(s32);
extern void Func_02000372(s32, s32);

void Func_02000068(s32 object)
{
    s32 cue = (s32) &Value_000025b8;
    Func_02000332(cue);
    Func_02000342(object, 0);
    if (Func_02000332(0, 0) == 0) {
        Func_02000334(10);
        Func_02000352(cue + 1);
    } else {
        Func_0200035a(cue + 2);
    }
    Func_02000372(object, 0);
}
