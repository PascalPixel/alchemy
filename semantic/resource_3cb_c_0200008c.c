typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

/*
 * Resource 3cb link-slot agreement test at 0x0200008c (156 bytes,
 * 0x0200008c-0x02000127).
 *
 * Complete owner: `push {r5, r6, r7, lr}` prologue at 0x0200008c and the
 * matching interworking return `pop {r5, r6, r7} / pop {r1} / bx r1` at
 * 0x02000104.  The popped return address goes to r1, so r0 survives and IS the
 * result; every path sets it to 0 or 1.
 *
 * The 28 bytes at 0x0200010c-0x02000127 are this owner's literal pool
 * (0x03001f64, 0x04000128, 0x00000303, 0x02009928, 0x00000302, 0x02002024,
 * 0x02009940), with two alignment bytes at 0x0200010a.  Execution reaches
 * `bx r1` at 0x02000108 and never falls into them; the `subs`/`ldr`/`lsls`
 * halfwords the disassembler prints there are data.  The routine at
 * 0x02000128 that follows is a separate prologue-less leaf, not part of this
 * owner.
 *
 * All six call sites were resolved with tools/overlay_call_targets.ts
 * (true_target_offset = stored_displacement + 2), matching the inventory's
 * calls=6:
 *   0x020000aa -> veneer 0x0200180c -> Func_080770c0   (test flag 0x303)
 *   0x020000bc -> veneer 0x02001814 -> Func_080770c8   (set flag 0x302)
 *   0x020000c4 -> veneer 0x0200181c -> Func_080770d0   (clear flag 0x302)
 *   0x020000ca -> veneer 0x0200180c -> Func_080770c0   (test flag 0x302)
 *   0x020000f0 -> veneer 0x0200181c -> Func_080770d0   (clear flag 0x303)
 *   0x020000fa -> veneer 0x0200180c -> Func_080770c0   (test flag 0x303)
 * The flag trio is Func_080770c0 = test, Func_080770c8 = set,
 * Func_080770d0 = clear; the polarity is forced by Func_020002d8's one-shot
 * timer elsewhere in this overlay.
 *
 * Link base: resource_3cb is linked at 0x02008000 (the handler table at the
 * tail of assets/code/resource_3cb_overlay.s lists 0x02009051 = Func_02001050
 * + 1, byte-exact in assets/code, plus 0x02008861, 0x020082d9, 0x02008341,
 * 0x02009159 for other owners here).  So 0x02009928 and 0x02009940 are
 * IN-IMAGE data at file offsets 0x1928 and 0x1940, while 0x02002024 and
 * 0x03001f64 are below the image and are ordinary RAM.  The overlay image is
 * writable EWRAM, so the in-image arrays are not const.
 *
 * 0x04000128 is SIOCNT.  The word load takes SIOCNT together with the halfword
 * above it, but only bits 4-5 of SIOCNT survive the `lsls #26 / lsrs #30`
 * pair, and in multiplayer mode those two bits are the local unit's slot id.
 * The `(*(u16 *)0x03001f64 & 3) == 3` guard above it is the overlay's own
 * link-active test.  When the link is not up the slot is left at -1 and the
 * owner reports no agreement.
 *
 * Behaviour: establish the local link slot, keep flag 0x303 consistent with
 * whether a link is up, and then, when a link is up, record in flag 0x302
 * whether this unit is a follower (slot != 0) rather than the host.  Finally
 * compare the entry the caller names, Data_02009928[index], against the value
 * the selected table row holds for that index, and report 1 when they agree.
 *
 * Uncertainties: the 24-byte stride of the Data_02002024 table and the meaning
 * of its two rows are not established beyond "the host row and the follower
 * row"; the row select is written faithfully as `Func_080770c0(0x302) ^ 1`
 * rather than as a boolean, because nothing here proves the flag test returns
 * exactly 0 or 1.  The slot comparison at 0x020000f4 is signed, which is what
 * makes the -1 initialisation an exit.
 */

extern u8 Data_02009940[];            /* in-image: per-index column selector */
extern u32 Data_02009928[];           /* in-image: the value being agreed on */

/* Old-style declarations: arities vary per site across this overlay. */
s32 Func_080770c0();
void Func_080770c8();
s32 Func_080770d0();

/* Used for its return value. */

s32 Func_0200008c(s32 index)
{
    s32 slot = -1;
    s32 row;
    u32 expected;

    if ((*(u16 *)0x03001f64 & 3) == 3) {
        slot = (s32)((*(u32 *)0x04000128 << 26) >> 30);
        Func_080770c8(0x303);
    } else {
        Func_080770c8(0x303);
    }

    if (slot < 0) {
        return 0;
    }
    if (Func_080770d0(0x303) == 0) {
        return 0;
    }

    if (slot != 0) {
        Func_080770c0(0x302);
    } else {
        Func_080770d0(0x302);
    }

    row = Func_080770c0(0x302) ^ 1;
    expected = *(u32 *)((u32)0x02002024 + (u32)(row * 24)
                        + (u32)(Data_02009940[index] << 2));

    if (expected != Data_02009928[index]) {
        return 0;
    }
    return 1;
}
