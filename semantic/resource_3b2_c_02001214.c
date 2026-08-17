/* Scene beat for slot 8: cue the companion in, or clear the flag byte. */
#include "types.h"

extern s16 Data_02000240[];             /* the overlay's scene table */

/* The two id words; only their difference is used. */
extern u8 Data_0000007e[];
extern u8 Data_000008d2[];

/* Old-style declarations: overlay imports vary in arity between call sites.
   One import name per call site: bl displacements are per-site. */
u8 *Func_020042a4();                    /* scene entity by selector */
u8 *Func_0200428e();                    /* companion entity by selector, or 0 */
void Func_02004300();                   /* cue a slot to (x, z) in 16.16 */
u8 *Func_020042da();                    /* scene entity by selector */
void Func_02004290();                   /* set presentation mode */
void Func_02004330();                   /* slot state notification */
void Func_020042b0();                   /* collision repaint, six arguments */
u8 *Func_02004318();                    /* scene entity by selector */

void Func_02001214(void)
{
    u8 *entity;
    u8 *companion;

    entity = Func_020042a4(8);
    companion = Func_0200428e(Data_02000240[224] + (Data_000008d2 - Data_0000007e));

    if (companion == 0) {
        goto absent;
    }

    Func_02004300(8, 0x028a0000, 0x00a80000);   /* (650.0, 168.0) */
    *(s32 *)(entity + 12) = (s32)0xffe00000;    /* y = -32.0 in 16.16 */

    Func_02004290(Func_020042da(8), 0);
    Func_02004330(8, 3);

    entity[85] = 0;
    {
        s32 mark = 2;
        mark |= entity[35];
        entity[35] = mark;
    }

    {
        s32 e = 10;
        s32 d = 40;
        Func_020042b0(42, 10, 1, 1, d, e);
    }
    return;

absent:
    /* Slot absent: clear the entity's flag byte and stop. */
    Func_02004318(8)[85] = 0;
}
