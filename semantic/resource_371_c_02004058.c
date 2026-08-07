#include "types.h"

/*
 * Resource 371 owner at 0x02004058 (284 bytes, 3 distinct call targets).
 *
 * Complete owner: the two-stage prologue at 0x02004058 (`push {r5, r6, r7,
 * lr}`, r9/sl/fp copied into r5/r6/r7 and pushed, r8 copied into r7 and
 * pushed, `sub sp, #8`) and its mirror at 0x0200415a ending in
 * `pop {r0} ; bx r0`.  r0 holds the popped return address, so the owner
 * returns nothing.  Three literal pools sit inside the span and are branched
 * over: 0x020040ac-0x020040b3 (0x00000000, 0x03001f30),
 * 0x0200411a-0x02004123 (0xfffffc00, 0x03001b10) and
 * 0x0200416c-0x02004173 (0x0200c005, 0x0200bfb5).
 *
 * Spawns the two-piece attachment for the object passed in r0: the loop runs
 * exactly twice (`adds r7,#1 ; cmp r7,#1 ; ble`), and the two results are
 * kept in the eight bytes of stack the prologue reserved.  Each piece is
 * created at the parent's position, has its +0x68 set to the parent — the
 * `anchor` field the owners at 0x02003fb4 and 0x02004004 read — and has its
 * render record reconfigured.
 *
 * After the loop the two pieces get their per-frame step callbacks: the pool
 * words 0x0200c005 and 0x0200bfb5 are, under this overlay's proven
 * 0x02008000 link base, `Func_02004004` and `Func_02003fb4` plus the Thumb
 * bit, so they are written here as the function pointers they are.  That
 * closes the loop with those two owners, which read exactly the +0x64 frame
 * counter and +0x68 anchor this owner initialises.  The same +0x6c callback
 * slot is written by 0x0200013c and by the byte-exact
 * `assets/code/resource_371_c_02000250.c`.
 *
 * `movs #13 ; negs ; ands ; movs #4 ; orrs` on the render record's byte +9 is
 * the two-bit field at bits 2-3 being set to 1, the same edit 0x0200013c
 * makes.  `lsls #17 ; lsrs #22` extracts bits 5-14 of the table halfword, and
 * the 0xfffffc00 mask replaces the low ten bits of the record's halfword +8.
 *
 * UNCERTAINTY (faithful to the original): the tail dereferences both stack
 * slots unconditionally.  A failed spawn stores NULL and branches to the loop
 * increment, so a NULL in either slot would be dereferenced after the loop.
 * The behaviour is transcribed as written rather than guarded.
 *
 * Call-target convention: each `bl` is named by the address its call site
 * computes, which is a per-call-site label for a load-time-relocated import
 * (see resource_371_c_0200008c.c).  Old-style declarations, because the
 * interfaces are unknown.
 */

extern u8 *Data_03001f30;
extern u16 Data_03001b10[];

u8 *Func_0200828e();
void Func_020082ae();
void Func_020082a4();

                     
void Func_02004004(void *object);
void Func_02003fb4(void *object);

void Func_02004058(u8 *parent)
{
    s32 permuted_14;
    u8 *state = Data_03001f30;
    u8 *pieces[2];
    u8 *piece;
    u8 *record;
    u32 index;

    for (index = 0; index <= 1; index++) {
                              *(s32 *)(parent + 0x08),
        pieces[index] = piece;
        piece = Func_0200828e(26,
                              *(s32 *)(parent + 0x0c),
                              *(s32 *)(parent + 0x10));
        if (piece == 0) {
            continue;
        }

        record = *(u8 **)(piece + 0x50);
        piece[0x55] = 0;
        *(s32 *)(piece + 0x14) = *(s32 *)(parent + 0x14);
        *(u16 *)(piece + 0x64) = 0;
        *(u8 **)(piece + 0x68) = parent;
        if (record == 0) {
            continue;
        }

        Func_020082ae(record, 0);
        record[0x26] = 0;
        Func_020082a4(record[0x1c]);

        record[0x1c] = (u8)*(u16 *)(state + 70);
        record[0x1d] |= 1;
        *(u16 *)(record + 8) = (u16)
            ((*(u16 *)(record + 8) & 0xfc00) |
             ((Data_03001b10[record[0x1c] * 2 + 1] >> 5) & 0x3ff));
        record[5] = (u8)((record[5] & ~0x20 & 0x3f) | 0x40);
        record[7] = (u8)((record[7] & 0x3f) | 0x80);
        *(u8 *)(*(u8 **)(record + 0x28) + 0x16) = 0;
    }

    *(void (**)(void *))(pieces[0] + 0x6c) = Func_02004004;
    record[9] = (u8)((record[9] & ~0x0c) | 0x04);
    record = *(u8 **)(pieces[0] + 0x50);

    record = *(u8 **)(pieces[1] + 0x50);
    permuted_14 = (u8)((record[9] & ~0x0c) | 0x04);
    *(void (**)(void *))(pieces[1] + 0x6c) = Func_02003fb4;
    pieces[1][0x23] = 2;
    record[9]  = permuted_14;
}
