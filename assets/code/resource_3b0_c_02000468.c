typedef unsigned char u8;
typedef signed int s32;

extern void Func_02001680();
extern void Func_020016dc(s32, s32, s32);
extern void Func_02001704(s32, s32);
extern s32 Func_020016ba(s32);
extern void Func_02001646(s32);
extern void Func_02001672(void);
extern void Func_02001650(s32);
extern void Func_02000950(void);
extern void Func_020016c0(void);

void Func_02000468(void)
{
    Func_02001680();
    Func_020016dc(0, 0xA40000, 0x01410000);
    Func_02001704(0, 15);
    Func_02001680(Func_020016ba(0), 0);
    Func_02001646(1);
    Func_02001672();
    Func_02001650(1);
    Func_02000950();
    Func_020016c0();
}
