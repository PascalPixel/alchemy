#include "types.h"

/*
 * Resource 3ae, owner at 0x020000a0 (164 bytes including its 13-word pool at
 * 0x02000110..0x02000142).
 *
 * Exported entry: the overlay's veneer table at 0x02000000 holds the pool word
 * 0x020080a1 == Func_020000a0 + 1 (fourth `ldr r4,[pc,#0] / bx r4` pair), so
 * this is a public entry point of the overlay rather than a table callback.
 *
 * Prologue `push {lr}` at 0x020000a0, return `pop {r1} / bx r1` at 0x0200010a.
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
 * Link base: the returned pool words 0x020098ec..0x02009dcc are in-image data
 * under the proven 0x02008000 base (file offsets 0x18ec..0x1dcc); the sibling
 * at 0x02000040 spells the identical band as `Data_020098xx`/`Data_02009xxx`
 * symbols, which is the base witness.
 *
 * Call targets resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --` (an overlay `bl`
 * stores target offset - 2; the disassembler's annotation is wrong).  All four
 * sites go through the veneer at 0x02001550 to `Func_080770c0`, the event-flag
 * test also used by `semantic/overlays/resource_3ae_c_020002dc.c`.  sites=4
 * equals the inventory's calls=4, so the call set is complete.
 *
 * Uncertainties: 0x93e and 0x950 are read as event-flag ids purely from their
 * use as the sole argument of Func_080770c0.  The comparison is `cmp r2,r3`
 * against a full word loaded from the pool while r2 came from `ldrsh`, so the
 * compare is signed 32-bit; the sibling at 0x02000040 has the same shape.
 */

/* Raw per-call-site import labels.  The overlay's relocated BL encoding names
 * the same veneer at a different arithmetic address from each call site. */
s32 Func_02001608();
s32 Func_02001622();
s32 Func_0200163c();
s32 Func_0200164a();

extern s16 Data_02000240[];
extern u8 Value_0000006b;
extern u8 Value_00000070;
extern u8 Value_0000006c;

extern u8 Data_020098d4[];
extern u8 Data_020098ec[];
extern u8 Data_020099c4[];
extern u8 Data_02009acc[];
extern u8 Data_02009ba4[];
extern u8 Data_02009c7c[];
extern u8 Data_02009d24[];
extern u8 Data_02009dcc[];

s32 Func_020000a0(void)
{
    s16 room = Data_02000240[224];

    if (room == (s32)&Value_0000006b) {
        if (Func_02001608(0x93e) != 0) {
            return (s32)Data_02009ba4;
        }
        return (s32)Data_02009acc;
    }

    if (room == (s32)&Value_00000070) {
        /* movs r0,#149 / lsls r0,r0,#4 == 0x950 */
        if (Func_02001622(0x950) != 0) {
            return (s32)Data_020099c4;
        }
        return (s32)Data_020098ec;
    }

    if (room == (s32)&Value_0000006c) {
        if (Func_0200163c(0x950) != 0) {
            return (s32)Data_02009dcc;
        }
        if (Func_0200164a(0x93e) != 0) {
            return (s32)Data_02009d24;
        }
        return (s32)Data_02009c7c;
    }

    return (s32)Data_020098d4;
}
