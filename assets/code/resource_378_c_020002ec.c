typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

/*
 * Resource 378 owner at 0x020002ec (116 bytes, through 0x0200035f).
 *
 * Its five pool words occupy 0x0200034c--0x0200035f; the next owner begins
 * at 0x02000360.  This is the sibling setup path to 0x0200027c: eleven
 * imported calls, the same signed scene halfword at +450, and three branch
 * sites.
 */

extern s16 Data_02000240[];

extern void Func_020038e0(void);
extern s32 Func_020038c6(s32 flag);
extern void Func_020039a8(s32 value);
extern void Func_020039b0(s32 value);
extern void Func_020039c6(s32 value);
extern void Func_02003954(s32 id);
extern void Func_0200398c(s32 id, s32 enabled);
extern void Func_0200391a(s32 frames);
extern void Func_020039f2(s32 id, s32 arg1);
extern void Func_02003962(s32 id, s32 mode);
extern void Func_0200393e(void);

void Func_020002ec(void)
{
    Func_020038e0();

    if (Func_020038c6(0x855) == 0) {
        Func_020039a8(0x128b);
    } else {
        Func_020039b0(0x1379);
    }

    if (Data_02000240[225] == 11) {
        Func_020039c6(0x1ceb);
    }

    Func_02003954(9);
    Func_0200398c(9, 1);
    Func_0200391a(2);
    Func_020039f2(9, 0);
    Func_02003962(9, 2);
    Func_0200393e();
}
