#include "types.h"

/*
 * Resource 3ca scene-setup step at 0x02000da4 (396 bytes,
 * 0x02000da4-0x02000f2f).
 *
 * Complete owner: `push {r5, r6, r7, lr}` / `sub sp, #4` at 0x02000da4,
 * matching `add sp, #4 / pop {r5, r6, r7} / pop {r1} / bx r1` at 0x02000f24.
 * The popped register is r1, not r0, so r0 survives the return and IS the
 * result — and r0 is set to 0 immediately before the epilogue, so the owner
 * returns `s32` and always returns 0.
 *
 * Two literal pools sit inside the span and are data, not code:
 *   0x02000edc-0x02000eff, branched over by the bare `b.n 0x02000f00` at
 *     0x02000eda (a lone forward hop with no conditional structure), and
 *   0x02000f2c-0x02000f2f, after the epilogue.
 * Pool contents:
 *   0x02000edc 0x00000002   BG3 priority
 *   0x02000ee0 0x00000003   BG2/BG1 priority
 *   0x02000ee4 0x02000240   the cross-overlay scene-flag block
 *   0x02000ee8 0x03001e70   IWRAM pointer table
 *   0x02000eec 0x0400000e   BG3CNT
 *   0x02000ef0 0x0000fffc   priority-field clear mask
 *   0x02000ef4 0x0400000a   BG1CNT
 *   0x02000ef8 0x00002648   BLDCNT value
 *   0x02000efc 0x04000050   BLDCNT
 *   0x02000f2c 0xffa60000   the 16.16 delta -90.0
 *
 * All thirteen call sites resolved with cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --; the
 * disassembler's own `bl` annotations are wrong for overlays by construction.
 * Placed calls per target:
 *   Func_08077028  x1   (0x02000dc0, veneer 0x020013a4)
 *   Func_0808a080 x10   (0x02000dd2, 0x02000dde, 0x02000dea, 0x02000df4,
 *                        0x02000dfe, 0x02000e14, 0x02000e26, 0x02000e44,
 *                        0x02000e62, 0x02000e80; veneer 0x020013d4)
 *   Func_08009128  x1   (0x02000f1a, veneer 0x02001384)
 *   Func_0200007c  x1   (0x02000f1e, an in-image prologue — the byte-exact
 *                        source assets/code/resource_3ca_c_0200007c.c)
 * That is 13 sites against the row's `calls=13`.
 *
 * Func_0808a080 is the scene-record accessor: it takes a slot id and returns
 * that slot's record.  The reading is the project's established one (see the
 * note in HANDOVER §0 about it being the accessor and Func_08009278 the
 * four-argument action, which reads backwards from the argument shapes alone).
 *
 * Two idioms worth naming because they are easy to misread:
 *  - 0x02000dc6 loads 224*2 = 448 as a displacement, stores through it, and
 *    only then does `subs r2, #192` to make the value 256.  The store is at
 *    workspace + 448 with value 256, not at 448-192.
 *  - the IWRAM word at 0x03001e70 + 76 is 0x03001ebc, the same scene-workspace
 *    cell that 0x0200004c dereferences directly.
 *
 * UNCERTAINTIES.  (1) The three `strh` stores to the stack halfword at sp+2 at
 * 0x02000eac/0x02000ebc/0x02000ec8 are never read back and the frame is
 * discarded on return; they are dead and are omitted.  (2) Func_08009128 is
 * reached with r0/r1 still holding the BG2CNT address and value from the block
 * above; nothing sets them deliberately, so it is spelled with no arguments.
 * (3) At 0x02000e94 the mask register r5 is consumed destructively by the last
 * flag update; it is not read again, so nothing depends on it.
 */

extern s16 Data_02000240[];  /* cross-overlay scene-flag block */

/* Main-image imports reached through this overlay's veneer table.  Old-style
 * declarations are mandatory: import arity varies between call sites. */
void Func_02002166();
u8 *Func_020021a8();
u8 *Func_020021b4();
u8 *Func_020021c0();
u8 *Func_020021ca();
u8 *Func_020021d4();
u8 *Func_020021ea();
u8 *Func_020021fc();
u8 *Func_0200221a();
u8 *Func_02002238();
u8 *Func_02002256();
void Func_020022a0();
void Func_02000f9c(void);
                     
                       /* scene-record accessor: slot id -> record */

/* In-image callee, already byte-exact as resource_3ca_c_0200007c.c. */

/*
 * record + 80 points at a display sub-record; +9 and +21 are flag bytes whose
 * bits 2-3 this owner rewrites to the value 1.  Spelled as a macro rather than
 * a helper function so that the file keeps one C call expression per assembly
 * call site.
 */
#define SET_MODE_FIELD(display, byte_offset) \
    ((display)[byte_offset] = (u8)(((display)[byte_offset] & 0xf3) | 4))

s32 Func_02000da4(void)
{
    u8 *workspace;
    u8 *record;
    u8 *display;

    /*
     * The signed halfword at byte offset 450 of the 0x02000240 block.  The
     * cross-overlay idiom documented for this block is the halfword at 448;
     * this is its neighbour, read the same way.
     */
    if (Data_02000240[225] == 99) Func_02002166(0, 242);

    /* 0x03001e70 + 76 is the scene-workspace pointer cell. */
    workspace = *(u8 **)0x03001EBC;
    *(s32 *)(workspace + 448) = 256;

    /* Slots 8 and 9 get two byte fields set directly. */
    Func_020021a8(8)[89] = 0;
    Func_020021b4(8)[35] = 2;
    Func_020021c0(9)[89] = 0;
    Func_020021ca(9)[35] = 2;

    /* The two overlay slots have only their +9 field rewritten... */
    display = *(u8 **)(Func_020021d4(8) + 80);
    SET_MODE_FIELD(display, 9);
    display = *(u8 **)(Func_020021ea(9) + 80);
    SET_MODE_FIELD(display, 9);

    /* ...while party slots 0-3 get both +9 and +21.  Each pair is one accessor
     * call whose record is re-dereferenced for the second field, exactly as
     * the assembly does (`ldr r1, [r0, #80]` twice off one call). */
    record = Func_020021fc(0);
    SET_MODE_FIELD(*(u8 **)(record + 80), 9);
    SET_MODE_FIELD(*(u8 **)(record + 80), 21);
    record = Func_0200221a(1);
    SET_MODE_FIELD(*(u8 **)(record + 80), 9);
    SET_MODE_FIELD(*(u8 **)(record + 80), 21);
    record = Func_02002238(2);
    SET_MODE_FIELD(*(u8 **)(record + 80), 9);
    SET_MODE_FIELD(*(u8 **)(record + 80), 21);
    record = Func_02002256(3);
    SET_MODE_FIELD(*(u8 **)(record + 80), 9);
    SET_MODE_FIELD(*(u8 **)(record + 80), 21);

    /* Background priorities: BG3 to 2, BG2 and BG1 to 3. */
    *(volatile u16 *)0x0400000E = (u16)((*(volatile u16 *)0x0400000E & 0xfffc) | 2);
    *(volatile u16 *)0x0400000C = (u16)((*(volatile u16 *)0x0400000C & 0xfffc) | 3);
    *(volatile u16 *)0x0400000A = (u16)((*(volatile u16 *)0x0400000A & 0xfffc) | 3);

    /* Alpha blend: BLDCNT then BLDALPHA (129 << 4 = 0x810). */
    *(volatile u16 *)0x04000050 = 0x2648;
    *(volatile u16 *)0x04000052 = 0x0810;

    /*
     * Two 16.16 coordinates in the record block at 0x03001e70 are shifted by
     * -90.0.  154*2 = 308 and 178*2 = 356 are the record bases; the field is
     * at +12 of each.
     */
    record = *(u8 **)0x03001E70;
    *(s32 *)(record + 308 + 12) += (s32)0xffa60000;
    *(s32 *)(record + 356 + 12) += (s32)0xffa60000;

    Func_020022a0();
    Func_02000f9c();
    return 0;
}
