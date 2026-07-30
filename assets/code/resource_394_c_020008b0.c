typedef signed int s32;
typedef unsigned short u16;

extern u16 *Data_020092c4;

extern void Func_02001952(void);
extern void Func_0200199a(s32, s32);
extern void Func_02001958(s32);
extern void Func_020019de(s32);
extern void Func_0200198a(s32, s32, s32);
extern void Func_020019ba();
extern void Func_020019a4();
extern void Func_02001982(s32);
extern void Func_020019d2(s32, s32);
extern void Func_020019b4(s32, s32, s32);
extern void Func_020019ce(s32, s32, s32);
extern void Func_020019f4(s32, s32);
extern void Func_020019ea(s32);
extern void Func_02001a02(s32, s32);
extern void Func_02001a42(s32);
extern void Func_02001a48(s32);
extern void Func_020019cc(s32, s32, s32, s32, s32, s32);
extern void Func_02001a08(void);

void Func_020008b0(void)
{
    Func_02001952();
    Func_0200199a(0, 8);
    Func_02001958(6);
    Func_020019de(0xef);
    Func_0200198a(8, 0x8000, 0x3333);
    Func_020019ba(8, 2);
    Func_020019a4(8, 0x68, 0xb0);
    Func_02001982(6);
    Func_020019d2(0, 2);
    Func_020019b4(0, 0x4ccc, 0x3333);
    Func_020019ce(0, 8, 0);
    Func_020019a4(24);
    Func_020019f4(0, 1);
    Func_020019ea(8);
    Func_02001a02(8, 1);
    Func_02001a42(288);
    Func_02001a48(213);
    Func_020019ba(5, 9, 1, 4, 4, 9);
    Func_020019cc(0, 0, 1, 4, 6, 9);
    *Data_020092c4 = 0;
    Func_02001a08();
}
