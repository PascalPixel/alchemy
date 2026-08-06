#include "types.h"

/*
 * Resource 394 board repaint at 0x02000194.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus `mov r7,r8 / push {r7}` at
 * 0x02000194, and the matching `add sp,#8 / pop {r3} / mov r8,r3 /
 * pop {r5, r6, r7} / pop {r0} / bx r0` at 0x02000358.  472-byte row: 464 bytes
 * of code plus the two pool words 0x020092c0 and 0x020092c8 at
 * 0x02000364-0x0200036b.  Control-flow walk: every branch target is
 * 0x02000358 or below and the next owner's prologue is at 0x0200036c.  The
 * return address is popped into r0, so the owner is `void`.
 *
 * Called from the root 0x02000980 and reached again from 0x02000980's own
 * chain; it in turn calls 0x02000b3c twice and 0x02000150 once.
 *
 * 0x020092c0 and 0x020092c8 are pointer CELLS, not values — the body always
 * loads through them twice.  tracked byte-exact assets/code/
 * resource_394_c_02000054.c reads `*(s32 *)0x020092C0` the same way, and
 * assets/code/resource_394_c_020008b0.c declares the neighbouring cell
 * `extern u16 *Data_020092c4`.
 *
 * The layout selector at `**(s16 **)0x020092c8` is re-read four times rather
 * than cached, and each read is spelled out: the first two arms test it
 * against zero and the last against one, so folding them into one local would
 * not be faithful to the polarity changes.
 *
 * The record list is the same twelve-byte-stride, -1-terminated array that
 * 0x02000a90 fills and 0x02000b3c stamps: id at +0, column at +2, row at +4,
 * orientation at +6 and the spawned object pointer at +8.
 *
 * Nineteen call sites, matching the row's advertised count.  Per-target,
 * assembly = C: Func_080091b8 10, Func_080091c0 4, Func_08009080 2,
 * Func_02000b3c 2, Func_02000150 1.
 */

/* Old-style declarations: interfaces vary by call site across this overlay. */
  /* place a fixture, first bank: (x, y, w, h, sx, sy) */
  /* place a fixture, second bank */
  /* set object motion state */




extern void Func_0200122a();
extern void Func_0200123e();
extern void Func_02001254();
extern void Func_02001272();
extern void Func_02001284();
extern void Func_0200129c();
extern void Func_020012a6();
extern void Func_020012b8();
extern void Func_020012d0();
extern void Func_020012b6();
extern void Func_02001324();
extern void Func_02001340();
extern void Func_02001312();
extern void Func_02001378();
extern void Func_0200139e();
extern void Func_02000e70(s16 *records, s32 value);
extern void Func_020013bc();
extern void Func_02000e8e(s16 *records, s32 value);
extern void Func_020004a6(void);
void Func_02000194(void)
{
    s16 *record = *(s16 **)0x020092c0;

    if (**(s16 **)0x020092c8 != 0) {
        Func_0200122a(65, 53, 2, 1, 79, 29);
        Func_0200123e(65, 40, 2, 4, 15, 28);
    } else {
        Func_02001254(65, 50, 2, 5, 79, 25);
    }

    if (**(s16 **)0x020092c8 != 0) {
        Func_02001272(0, 32, 32, 32, 32, 0);
        Func_02001284(32, 32, 32, 32, 64, 0);
        Func_0200129c(0, 32, 32, 32, 0, 0);
    } else {
        Func_020012a6(0, 64, 32, 32, 32, 0);
        Func_020012b8(32, 64, 32, 32, 64, 0);
        Func_020012d0(0, 64, 32, 32, 0, 0);
    }

    if (record[0] != -1) {
        do {
            u8 *piece = *(u8 **)(record + 4);

            record += 6;
            if (**(s16 **)0x020092c8 == 1) {
                Func_020012b6(piece, 4);
                piece[35] = 3;
                piece[85] = 0;
                *(s32 *)(piece + 12) = 0x1a0000;

                if (record[3] != 0) {
                    Func_02001324(68, 40, 1, 4, record[1] + 32, record[2]);
                } else {
                    Func_02001340(70, 40, 4, 1, record[1] + 32, record[2]);
                }
            } else {
                Func_02001312(piece, 1);
                piece[35] = 1;
                piece[85] = 2;
                *(s32 *)(piece + 12) = 0;
            }
        } while (record[0] != -1);
    }

    Func_02001378(70, 42, 1, 1, 10, 50);

    if (**(s16 **)0x020092c8 == 1) {
        Func_0200139e(0, 32, 32, 32, 0, 0);
        Func_02000e70(*(s16 **)0x020092c0, 254);
    } else {
        Func_020013bc(0, 64, 32, 32, 0, 0);
        Func_02000e8e(*(s16 **)0x020092c0, 255);
    }

    Func_020004a6();
}
