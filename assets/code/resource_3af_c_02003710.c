#include "types.h"

extern void Func_02006ab6(s32);
extern s32 *Func_02007aac();
extern void Func_02007ab6(s32, s32, s32);
extern void Func_02007ac0(s32, s32, s32);
extern void Func_02007aca(s32, s32, s32);
extern void Func_02006ae4(s32);
extern void Func_02006aea(s32);
extern void Func_02006af0(s32);
extern s32 *Func_02007a90(s32);
extern s32 *Func_02007aa0(s32);
extern s32 *Func_02007ab8(s32);
extern void Func_02006b38(s32);
extern void Func_02006b3e(s32);
extern void Func_02006b44(s32);
extern void Func_02006b4a(s32);
extern s32 *Func_02007adc(s32);
extern s32 *Func_02007ae6(s32);
extern s32 *Func_02007aee(s32);
extern s32 *Func_02007af6(s32);
extern s32 *Func_02007b08(s32);
extern s32 *Func_02007b14(s32);
extern s32 *Func_02007b20(s32);
extern s32 *Func_02007b2c();
extern s32 *Func_02007b38(s32);
extern void Func_02007ba6(s32, s32, s32);
extern void Func_02007a9c(s32);
extern void Func_02007c44(s32, s32);
extern void Func_02007c4c(s32, s32);
extern void Func_02007c54(s32, s32);
extern void Func_02007c5c(s32, s32);
extern void Func_02007c64(s32, s32);
extern void Func_02007c6c(s32, s32);
extern void Func_02007c74(s32, s32);
extern u8 *Func_02007b8a(s32);
extern u8 *Func_02007b96(s32);
extern u8 *Func_02007ba0(s32);
extern u8 *Func_02007baa(s32);
extern u8 *Func_02007bb4(s32);
extern u8 *Func_02007bbe(s32);
extern u8 *Func_02007bc8(s32);
extern u8 *Func_02007bd2(s32);
extern void Func_02007c4a(s32, s32, s32);
extern void Func_02007b40(s32);
extern void Func_02006eda(s32, s32, s32);
extern void Func_02006ee4(s32, s32, s32);
extern void Func_02006eee(s32, s32, s32);
extern void Func_02006ef8(s32, s32, s32);
extern void Func_02006f02(s32, s32, s32);
extern void Func_02006f0c(s32, s32, s32);
extern void Func_02006f16(s32, s32, s32);
extern void Func_02006f20(s32, s32, s32);

extern u16 Data_0200db40[];
extern s32 Data_0200db70[];
extern s32 Data_0200db90[];

void Func_02003710(void)
{
    {
        u16 *p = Data_0200db40;
        u32 i;

        for (i = 0; i < 8; i++) p[i] = 0xC000;
    }
    Func_02006ab6(8);
    ((void (*)())Func_02007aac)(9, 0, 0);
    Func_02007ab6(10, 0, 0);
    Func_02007ac0(11, 0, 0);
    Func_02007aca(12, 0, 0);
    Func_02006ae4(13);
    Func_02006aea(14);
    Func_02006af0(15);
    {
        s32 *t = Data_0200db70;
        s32 z = 0;
        t[0] = z;
        t[1] = z;
        t[2] = z;
        t[3] = z;
        Data_0200db90[0] = Func_02007a90(8)[4];
        Data_0200db90[1] = Func_02007aa0(13)[4];
        Data_0200db90[2] = Func_02007aac(14)[4];
        Data_0200db90[3] = Func_02007ab8(15)[4];
        Func_02006b38(16);
        Func_02006b3e(17);
        Func_02006b44(18);
        Func_02006b4a(19);
        {
            s32 v = 0xFFFF0000;

            Func_02007adc(16)[6] = v;
            Func_02007ae6(17)[6] = v;
            Func_02007aee(18)[6] = v;
            Func_02007af6(19)[6] = v;
        }
        t[4] = z;
        t[5] = z;
        t[6] = z;
        t[7] = z;
        Data_0200db90[4] = Func_02007b08(16)[4];
        Data_0200db90[5] = Func_02007b14(17)[4];
        Data_0200db90[6] = Func_02007b20(18)[4];
        Data_0200db90[7] = Func_02007b2c(19)[4];
    }
    {
        s32 *w = Func_02007b38(0);

        if (w != 0) Func_02007ba6(8, w[2], w[4]);
    }
    Func_02007a9c(1);
    Func_02007c44(13, 8);
    Func_02007c4c(14, 8);
    Func_02007c54(15, 8);
    Func_02007c5c(16, 8);
    Func_02007c64(17, 8);
    Func_02007c6c(18, 8);
    Func_02007c74(19, 8);
    {
        s32 o = 1;

        Func_02007b8a(8)[92] = o;
        Func_02007b96(13)[92] = o;
        Func_02007ba0(14)[92] = o;
        Func_02007baa(15)[92] = o;
        Func_02007bb4(16)[92] = o;
        Func_02007bbe(17)[92] = o;
        Func_02007bc8(18)[92] = o;
        Func_02007bd2(19)[92] = o;
    }
    Func_02007b2c(1);
    Func_02007c4a(8, 0x840000, 0x2780000);
    Func_02007b40(1);
    Func_02006eda(8, 0, 2);
    Func_02006ee4(13, 1, 2);
    Func_02006eee(14, 2, 2);
    Func_02006ef8(15, 3, 2);
    Func_02006f02(16, 4, 3);
    Func_02006f0c(17, 5, 3);
    Func_02006f16(18, 6, 3);
    Func_02006f20(19, 7, 3);
}
