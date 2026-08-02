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

extern s32 Func_080770c0(s32 flag);
extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a098(s32 id, s32 mode);
extern void Func_0808a0a8(s32 id);
extern void Func_0808a100(s32 id, s32 enabled);
extern void Func_0808a170(s32 value);
extern void Func_0808a180(s32 id, s32 arg1);

void Func_020002ec(void)
{
    Func_0808a018();

    if (Func_080770c0(0x855) == 0) {
        Func_0808a170(0x128b);
    } else {
        Func_0808a170(0x1379);
    }

    if (Data_02000240[225] == 11) {
        Func_0808a170(0x1ceb);
    }

    Func_0808a0a8(9);
    Func_0808a100(9, 1);
    Func_0808a010(2);
    Func_0808a180(9, 0);
    Func_0808a098(9, 2);
    Func_0808a020();
}
