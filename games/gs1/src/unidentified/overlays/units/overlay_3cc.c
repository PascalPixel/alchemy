#include "types.h"

#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

#include "resource_3cc.h"

void Func_02000114(s32);
void Func_02000116(s32, s32, s32);
void Func_0200012c(s32);
void Func_0200012e(s32, s32, s32);
void Func_02000144(s32);
void Func_02000146(s32, s32, s32);
void Func_0200015c(s32);
void Func_0200015e(s32, s32, s32);

/*
 * resource_3cc owner at 0x02000030, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000034 holding 0x20081a8.
 *
 * LEAF RESIDUE. Published at image offset 0xc; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x02000030 reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x20081a8 is image offset
 * 0x1a8 under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

/*
 * resource_3cc owner at 0x02000038, 4 bytes: `movs r0, #0 / bx lr`.
 *
 * LEAF RESIDUE. Published at image offset 0x2c; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 * A leaf never does -- it saves no register and returns with `bx lr`.
 *
 * Complete owner: both instructions. No prologue, no stack frame, no
 * literal pool, no callees, no argument read.
 *
 * One of the 70 rows sharing this exact body across the tree. The body is
 * shared; the identity is not -- this row is bounded by ITS overlay's
 * neighbours and published from ITS overlay's table.
 */

/*
 * resource_3cc owner at 0x0200003c, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000040 holding 0x20081d8.
 *
 * LEAF RESIDUE. Published at image offset 0x14; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x0200003c reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x20081d8 is image offset
 * 0x1d8 under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

/*
 * resource_3cc owner at 0x02000044, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000048 holding 0x20081ec.
 *
 * LEAF RESIDUE. Published at image offset 0x1c; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x02000044 reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x20081ec is image offset
 * 0x1ec under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

/*
 * resource_3cc owner at 0x020000ac, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x20000b0 holding 0x2008264.
 *
 * LEAF RESIDUE. Published at image offset 0x24; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x020000ac reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x2008264 is image offset
 * 0x264 under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

/*
 * resource_3cc owner at 0x020000b4, 4 bytes: `movs r0, #0 / bx lr`.
 *
 * THIS IS THE OVERLAY'S ENTRY DRIVER, not an ordinary leaf. The word at
 * image offset 4 -- the one the opening `ldr r4,[pc,#0] / bx r4` veneer
 * jumps through -- is 0x020080b5, which is this address plus the Thumb bit
 * under the base + 0x8000 spelling. The loader calls it; nothing in the
 * image does.
 *
 * It is the one driver in all 96 that does not open with a `push`, and this
 * is why: there is no body to save registers for. resource_3cc's entry
 * returns 0 and does nothing else. Every other overlay's driver sequences
 * its scene; this one declines to.
 *
 * CORRECTION TO AN EARLIER COUNT. All 96 entry drivers were reported as
 * drafted because `overlay_driver.ts --all --unowned` reported 0, but its
 * `ownerOf` accepts "inventory row" -- merely appearing in
 * the census -- as ownership, so an undrafted driver is not UNOWNED to it.
 * Two drivers are in that state, this one and resource_373's 0x02002a54.
 * The blind spot was in what the tool ACCEPTS, not in what it scans, which
 * is the same fault already identified in sweep B and covered by sweep D's tail.
 *
 * Reached through leaf-residue analysis rather than as a driver: sweep B saw
 * the header word as a publication and discarded the target for opening
 * with no `push`. The two populations meet here.
 *
 * Complete owner: both instructions. No prologue, no stack frame, no
 * literal pool, no callees, no argument read.
 */

u8 *Func_02000030(void)
{
    return (u8 *)0x020081a8;
}

s32 Func_02000038(void)
{
    return 0;
}

u8 *Func_0200003c(void)
{
    return (u8 *)0x020081d8;
}

u8 *Func_02000044(void)
{
    return (u8 *)0x020081ec;
}

void Func_0200004c(void) {
    Func_02000116(8, 0, 0);
    Func_02000114(0x30);
}

void Func_02000064(void) {
    Func_0200012e(9, 1, 0);
    Func_0200012c(0x44);
}

void Func_0200007c(void) {
    Func_02000146(0xA, 2, 0);
    Func_02000144(0x58);
}

void Func_02000094(void) {
    Func_0200015e(0xB, 3, 0);
    Func_0200015c(0x6C);
}

u8 *Func_020000ac(void)
{
    return (u8 *)0x02008264;
}

s32 Func_020000b4(void)
{
    return 0;
}
