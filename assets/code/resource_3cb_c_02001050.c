typedef signed int s32;
typedef unsigned char u8;

extern u8 Value_00000001;

extern void Func_020027a8(void);
extern void Func_020027b6(s32);
extern s32 Func_020028fe(s32, s32);

s32 Func_02001050(void)
{
    Func_020027a8();
    Func_020027b6(2);
    return Func_020028fe((s32)&Value_00000001, 1);
}
