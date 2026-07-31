typedef unsigned char u8;
typedef signed int s32;

extern void Func_0200104c(void);
extern void Func_020010d2(s32);
extern void Func_020010f4(s32, s32, s32);
extern void Func_020010de(s32, s32);
extern void Func_0200104a(u8 *, s32);
extern u8 *Func_02001084(s32);
extern void Func_02001038(s32);
extern void Func_020010f8(s32, s32);
extern void Func_02001064(u8 *, s32);
extern u8 *Func_0200109e(s32);
extern void Func_02001054(s32);
extern void Func_0200111a(s32, s32);
extern void Func_02001086(u8 *, s32);
extern u8 *Func_020010c0(s32);
extern void Func_02001074(s32);
extern void Func_02001134(s32, s32);
extern void Func_020010a0(u8 *, s32);
extern u8 *Func_020010da(s32);
extern void Func_02001090(s32);
extern void Func_0200112e(s32, s32, s32);
extern void Func_020010cc(s32);
extern void Func_020010f0(void);
void Func_020007ac(void)
{
    unsigned int beat;

    Func_0200104c();

    Func_020010d2(0x266d);
    Func_020010f4(10, 0, 10);

    beat = 0;
    do {
        Func_020010de(10, 0);
        Func_0200104a(Func_02001084(10), 1);
        Func_02001038(4);

        Func_020010f8(10, 15);
        Func_02001064(Func_0200109e(10), 0);
        beat++;
        Func_02001054(4);
    } while (beat <= 5);

    beat = 0;
    do {
        Func_0200111a(10, 0);
        Func_02001086(Func_020010c0(10), 1);
        Func_02001074(2);

        Func_02001134(10, 15);
        Func_020010a0(Func_020010da(10), 0);
        beat++;
        Func_02001090(2);
    } while (beat <= 11);

    Func_0200112e(10, 0, 0);

    Func_020010cc(0x897);

    Func_020010f0();
}
