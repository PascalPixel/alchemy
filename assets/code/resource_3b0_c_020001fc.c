typedef unsigned char u8;
typedef signed int s32;

extern s32 Func_020013fe(s32);
extern s32 Func_02001408(s32);
extern void Func_02001428(void);
extern void Func_02001520(s32, s32);
extern s32 Func_02001450(s32, s32);
extern void Func_0200142e(s32);
extern void Func_0200144a(void);
extern u8 Value_00000923;
extern u8 Value_00000922;
extern u8 Value_00000924;

void Func_020001fc(void)
{
    if (Func_020013fe((s32)&Value_00000923) != 0 || Func_02001408((s32)&Value_00000922) != 0) {
        Func_02001428();
        Func_02001520(0xE8, 3);
        Func_02001450(0xE8, 0);
        Func_0200142e((s32)&Value_00000924);
        Func_0200144a();
    }
}
