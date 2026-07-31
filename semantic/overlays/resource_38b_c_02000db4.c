typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;

/*
 * Resource 38b push/shove interaction at 0x02000db4 (292 bytes, 13 call
 * sites over 11 distinct callees).
 *
 * Complete owner: prologue `push {r5, r6, r7, lr} / mov r7, sl / mov r6, r9 /
 * mov r5, r8 / push {r5, r6, r7} / sub sp, #12` at 0x02000db4, and the
 * matching epilogue at 0x02000eaa restoring sl, r9, r8 before
 * `pop {r5, r6, r7} / pop {r0} / bx r0`.  The popped r0 is the return
 * address, so the owner is `void`.  0x02000eba is alignment and
 * 0x02000ebc-0x02000ed7 is the literal pool; the disassembler decodes part of
 * it as a `vrsra` NEON instruction, which is the usual sign that it is data.
 * The pool is reached only by `ldr rN,[pc,...]`; the control-flow walk from
 * the prologue never enters it.
 *
 * PACKED DIRECTION WORD (HANDOVER section 0 family).  Data_02009d3c is
 * indexed by `heading >> 12` scaled by 4, with the X step in the high half
 * and the Z step in the low half.  Both promotions are present in the
 * canonical form: `asrs #16` / sign-extended low half for the pixel-space
 * add, and `& 0xffff0000` / `<< 16` for the 16.16-space add — never a
 * multiply.  The table lives at file offset 0x1d3c under the proven
 * 0x02008000 base, immediately after the 20-byte table at 0x1d28 that
 * 0x02000cb4 walks (0x1d28 + 20 = 0x1d3c), which confirms both bases.
 *
 * HALFWORD COORDINATE VIEWS.  The s16 at +0x0a and +0x12 are the integer
 * parts of the 16.16 words at +0x08 and +0x10, and this owner uses both views
 * of the same fields — the halfword view for the tile probe and the word view
 * for the actual move.  That is the documented reason not to declare one
 * overlapping struct.
 *
 * COMPLETENESS, per target: Func_0808a080 x1, Func_02000d80 x1,
 * Func_080091d8 x1, Func_08009080 x2, Func_080000c0 x1, Func_080f9010 x1,
 * Func_08009150 x2, Func_08009158 x1, Func_02000cb4 x1, Func_020009cc x1,
 * Func_02000fa0 x1 = 13 sites, matching the inventory's `calls=13`, and each
 * appears exactly that many times below.  Four targets are this overlay's own
 * prologues (0x02000d80, 0x02000cb4, 0x020009cc, 0x02000fa0), all real
 * function starts reached as ordinary calls; the rest are import veneers.
 * Resolved with tools/overlay_call_targets.ts.
 *
 * The two Func_08009150 sites and the two Func_08009080 sites are distinct
 * `bl`s with different arguments and are deliberately not merged.
 */

/* Slot accessor. */
extern u8 *Func_0808a080();

/* Byte-exact sibling: returns the record occupying tile (x, z) over slots
 * 8..65, or NULL.  See assets/code/resource_38b_c_02000d80.c. */
extern u8 *Func_02000d80(s32 tile_x, s32 tile_z);

/* Collision probe; a result > 0 means the destination is blocked. */
extern s32 Func_080091d8();

/* Imports; old-style, arity open. */
extern void Func_08009080();
extern void Func_08009150();
extern void Func_08009158();
extern void Func_080000c0();
extern void Func_080f9010();

/* This overlay's own per-scene follow-ups. */
extern void Func_02000cb4(void);
extern void Func_020009cc(void);
extern void Func_02000fa0(void);

/* 16 packed direction words at file offset 0x1d3c. */
extern s32 Data_02009d3c[];

/* Cross-overlay scene-state halfword array; [224] is the scene id. */
extern s16 Data_02000240[];

void Func_02000db4(void)
{
    u8 *player;
    u8 *pushed;
    s32 heading;
    s32 step;
    s32 target[3];
    s32 tile_x;
    s32 tile_z;

    player = Func_0808a080(0);

    /* Heading is the top nibble of the u16 at +0x06: a 16-entry table. */
    heading = *(u16 *)(player + 6) >> 12;
    step = Data_02009d3c[heading];

    /* Pixel-space probe using the integer halfword views at +0x0a / +0x12,
     * then >> 4 to the 16-pixel tile grid. */
    tile_x = (*(s16 *)(player + 0x0a) + (step >> 16)) >> 4;
    tile_z = (*(s16 *)(player + 0x12) + (s16)step) >> 4;

    pushed = Func_02000d80(tile_x, tile_z);
    if (pushed == 0) {
        return;
    }

    *(u8 *)(pushed + 0x22) = 2;

    /* The same packed word re-read from the table, now promoted to 16.16 by
     * masking the high half and shifting the low half up. */
    step = Data_02009d3c[heading];
    target[0] = *(s32 *)(pushed + 0x08) + (step & (s32)0xffff0000);
    target[1] = *(s32 *)(pushed + 0x0c);
    target[2] = *(s32 *)(pushed + 0x10) + (step << 16);

    if (Func_080091d8(pushed, target) > 0) {
        return;
    }

    Func_08009080(player, 8);
    Func_080000c0(15);
    Func_080f9010(185);

    /* 0x3333 is the move speed written into both records' +0x30 / +0x34. */
    *(s32 *)(pushed + 48) = 0x3333;
    *(s32 *)(pushed + 52) = 0x3333;
    Func_08009150(pushed, target[0], target[1], target[2]);

    *(s32 *)(player + 48) = 0x3333;
    *(s32 *)(player + 52) = 0x3333;
    Func_08009150(player, target[0], target[1], target[2]);

    Func_08009158(pushed);

    *(s32 *)(pushed + 0x08) = target[0];
    *(s32 *)(pushed + 0x10) = target[2];
    *(s32 *)(pushed + 0x24) = 0;
    *(s32 *)(pushed + 0x2c) = 0;

    Func_08009080(player, 1);

    /* Same three scene ids as the dispatcher at 0x02000890 and the byte-exact
     * sibling 0x02000088, but mapped to different follow-ups here. */
    if (Data_02000240[224] == 0x23) {
        Func_02000cb4();
    } else if (Data_02000240[224] == 0x1e) {
        Func_020009cc();
    } else if (Data_02000240[224] == 0x20) {
        Func_02000fa0();
    }
}
