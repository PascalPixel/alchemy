typedef signed int s32;
typedef unsigned int u32;
typedef signed short s16;
typedef unsigned short u16;
typedef signed char s8;
typedef unsigned char u8;

extern void Func_02006a1c(s32, s32);
extern void Func_02006a2a(s32, s32);
extern void Func_02006a38(s32, s32);
extern void Func_02006a4a(s32, s32);
extern void Func_02006af2(s32, s32, s32);
extern void Func_02006afc(s32, s32, s32);
extern void Func_02006b06(s32, s32, s32);
extern void Func_02006b10(s32, s32, s32);
extern void Func_02006b1a(s32, s32, s32);
extern void Func_02006b24(s32, s32, s32);
extern void Func_02006b2e(s32, s32, s32);
extern void Func_02006b38(s32, s32, s32);

extern u16 Data_0200db30[];

void Func_020034bc(void)
{
    u16 *p;

    Func_02006a1c(8, 0);
    p = Data_0200db30;
    {
        s32 z = 0;

        p[0] = z;
    }
    Func_02006a2a(9, 1);
    p[1] += 0x80;
    Func_02006a38(10, 2);
    p[2] += 0x100;
    Func_02006a4a(11, 3);
    p[3] += 0x200;
    Func_02006af2(12, 0, 0);
    Func_02006afc(13, 1, 0);
    Func_02006b06(14, 2, 0);
    Func_02006b10(15, 3, 0);
    Func_02006b1a(16, 4, 1);
    Func_02006b24(17, 5, 1);
    Func_02006b2e(18, 6, 1);
    Func_02006b38(19, 7, 1);
}
