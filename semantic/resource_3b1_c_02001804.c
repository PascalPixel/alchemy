#include "types.h"

/*
 * Resource 3b1 unindexed helper at 0x02001804 (144 bytes incl. one-word
 * pool, 14 calls).
 *
 * Derived span: no inventory row (item 28's unindexed population, found
 * via overlay_unindexed.ts's reference resolution). `push {r5,r6,r7,lr}`
 * at 0x02001804, epilogue `pop {r5,r6,r7} / pop {r0} / bx r0` at
 * 0x0200188a-0x0200188e. The trailing pool word at 0x02001890
 * (0x03001ebc, the workspace pointer used elsewhere in this overlay, see
 * assets/code/resource_3b1_c_02000b84.c / 02000cc8.c) is included per the
 * usual rule (last instruction's own pool load), immediately followed by
 * the next owner's `push {lr}` at 0x02001894.
 *
 * Raw callee naming. All three arguments pass straight through unmodified
 * to Func_02007cd6.
 */

extern u8 *Data_03001ebc;














extern void Func_02007cd6();
extern void Func_02007dbc();
extern void Func_02007dcc();
extern s32 Func_02007d04();
extern void Func_0200609a();
extern void Func_02007d86();
extern u8 * Func_02007d24();
extern u8 * Func_02007d66();
extern void Func_02007d94();
extern void Func_02007da6();
extern void Func_02007d0e();
extern void Func_02007d14();
extern void Func_020060f0();
extern void Func_02007d58(void);
void Func_02001804(s32 a0, s32 a1, s32 a2)
{
    Func_02007cd6(a0, a1, a2);
    Func_02007dbc(a1);
    Func_02007dcc(a0, 0);

    if (Func_02007d04(0, 0) != 0) {
        u16 *slot = (u16 *)(Data_03001ebc + 472);
        (*slot)++;
        Func_0200609a(a0);
    } else {
        u8 *obj;

        Func_02007d86(a0);
        Func_02007d24(a0, 2);
        obj = Func_02007d66(0);
        if (obj != 0) {
            s16 v1 = *(s16 *)(obj + 10);
            s16 v2 = *(s16 *)(obj + 18);
            Func_02007d94(a0, v1, v2);
        }
        Func_02007da6(a0);
        Func_02007d0e(a0, 0, 0);
        Func_02007d14(0x300);
        Func_020060f0(a2);
    }

    Func_02007d58();
}
