typedef signed int s32;

extern void Func_02002b7e(s32);
extern s32 Func_02002a8e(s32);
extern void Func_02000906(s32, s32, s32);
extern void Func_02002ab4(s32);
extern void Func_02002ac2(s32);
extern void Func_02002ac8(s32);
extern void Func_0200092e(s32, s32, s32);
extern void Func_02002ad4(s32);
extern void Func_02002ae4(s32);
extern void Func_02002aea(s32);
extern void Func_02002af0(s32);
extern void Func_02002bde(s32);
extern void Func_02002a34(s32);
extern void Func_02001aa4(void);

void Func_02000768(void)
{
    Func_02002b7e(241);
    if (Func_02002a8e(0x308) != 0) {
        Func_02000906(10, 0, -64);
        Func_02002ab4(0x30b);
        Func_02002ab4(0x30c);
        Func_02002ac2(0x30d);
        Func_02002ac8(0x30e);
    } else {
        Func_0200092e(10, 0, -128);
        Func_02002ad4(0x30b);
        Func_02002ae4(0x30c);
        Func_02002aea(0x30d);
        Func_02002af0(0x30e);
    }
    Func_02002bde(0x121);
    Func_02002a34(2);
    Func_02001aa4();
}
