#include "types.h"

/*
 * Resource 3ae, owner at 0x0200019c (164 bytes including its 13-word pool at
 * 0x0200020c..0x0200023e).
 *
 * Exported entry: the overlay's veneer table at 0x02000000 holds the pool word
 * 0x0200819d == Func_0200019c + 1 (fifth `ldr r4,[pc,#0] / bx r4` pair), so
 * this is a public entry point of the overlay rather than a table callback.
 *
 * Prologue `push {lr}` at 0x0200019c, return `pop {r1} / bx r1` at 0x02000206.
 * The popped register is r1, not r0, so r0 survives and IS the result: every
 * arm loads a pool address into r0 and branches to the common epilogue.
 *
 * Selector: `ldr r3,[pc] (= 0x02000240) / movs r1,#224 / lsls r1,r1,#1 /
 * adds r3,r3,r1 / movs r1,#0 / ldrsh r2,[r3,r1]` — the signed halfword at byte
 * offset 448 of the cross-overlay workspace, i.e. `Data_02000240[224]`.  The
 * byte-exact sibling `assets/code/resource_3ae_c_02000040.c` selects on exactly
 * the same halfword against the same three constants (0x6b, 0x70, 0x6c) and
 * returns in-image data pointers, so this owner is the same accessor family for
 * a different asset set.
 *
 * Link base: the returned pool words 0x02009e74..0x0200a390 are in-image data
 * under the proven 0x02008000 base (file offsets 0x1e74..0x2390); the sibling
 * at 0x02000040 spells the identical band as `Data_020098xx`/`Data_02009xxx`
 * symbols, which is the base witness.
 *
 * Call targets resolved with `tools/lib/overlay_call_targets.ts` (an overlay `bl`
 * stores target offset - 2; the disassembler's annotation is wrong).  All four
 * sites go through the veneer at 0x02001550 to `Func_080770c0`, the event-flag
 * test also used by `semantic/overlays/resource_3ae_c_020002dc.c`.  sites=4
 * equals the inventory's calls=4, so the call set is complete.
 *
 * Byte-identical twin of Func_020000a0: the 164-byte instruction streams differ
 * in nothing but the eight returned data-pointer pool words (verified by
 * diffing `overlay_show.ts` output for both rows), so this is the same accessor
 * over a second asset set.
 *
 * Uncertainties: 0x93e and 0x950 are read as event-flag ids purely from their
 * use as the sole argument of Func_080770c0.  The comparison is `cmp r2,r3`
 * against a full word loaded from the pool while r2 came from `ldrsh`, so the
 * compare is signed 32-bit; the sibling at 0x02000040 has the same shape.
 */

/* Raw per-call-site labels for the relocated import. */
s32 Func_02001704();
s32 Func_0200171e();
s32 Func_02001738();
s32 Func_02001746();

extern s16 Data_02000240[];
extern u8 Value_0000006b;
extern u8 Value_00000070;
extern u8 Value_0000006c;

extern u8 Data_02009e74[];
extern u8 Data_0200a018[];
extern u8 Data_0200a120[];
extern u8 Data_02009e80[];
extern u8 Data_02009fa0[];
extern u8 Data_0200a24c[];
extern u8 Data_0200a30c[];
extern u8 Data_0200a390[];

s32 Func_0200019c(void)
{
    s16 room = Data_02000240[224];

    if (room == (s32)&Value_0000006b) {
        if (Func_02001704(0x93e) != 0) {
            return (s32)Data_02009fa0;
        }
        return (s32)Data_02009e80;
    }

    if (room == (s32)&Value_00000070) {
        /* movs r0,#149 / lsls r0,r0,#4 == 0x950 */
        if (Func_0200171e(0x950) != 0) {
            return (s32)Data_0200a120;
        }
        return (s32)Data_0200a018;
    }

    if (room == (s32)&Value_0000006c) {
        if (Func_02001738(0x950) != 0) {
            return (s32)Data_0200a390;
        }
        if (Func_02001746(0x93e) != 0) {
            return (s32)Data_0200a30c;
        }
        return (s32)Data_0200a24c;
    }

    return (s32)Data_02009e74;
}
