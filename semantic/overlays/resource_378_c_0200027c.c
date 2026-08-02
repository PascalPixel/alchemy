typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

/*
 * Resource 378 owner at 0x0200027c (112 bytes, through 0x020002eb).
 *
 * The five literal words at 0x020002d8--0x020002eb are this owner's pool;
 * the next push, at 0x020002ec, starts the following owner.  The body has
 * ten imported calls and three conditional/unconditional branch sites.
 * The scene selector is the signed halfword at Data_02000240 + 450, built
 * as 225 * 2 by the original code.  The final two calls are deliberately
 * kept after that selector test: the second call's result is discarded by
 * the epilogue, which pops the return address into r0.
 */

extern s16 Data_02000240[];

extern s32 Func_080770c0(s32 flag);
extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a100(s32 id, s32 enabled);
extern void Func_0808a150(s32 id, s32 x, s32 y);
extern void Func_0808a170(s32 value);
extern void Func_0808a180(s32 id, s32 arg1);

void Func_0200027c(void)
{
    Func_0808a018();

    if (Func_080770c0(0x855) != 0) {
        Func_0808a170(0x1377);
    } else {
        Func_0808a170(0x1289);
    }

    if (Data_02000240[225] == 11) {
        Func_0808a170(0x1ce9);
    }

    Func_0808a100(9, 1);
    Func_0808a150(9, 0, 0);
    Func_0808a010(2);
    Func_0808a180(9, 0);
    Func_0808a020();
}
