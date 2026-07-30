/*
 * resource_3c4 owner at 0x0200252c, 112 bytes: code 0x0200252c-0x0200259b, no
 * pool and no alignment (0x0200259c is the next prologue).
 *
 * A cutscene beat that places one actor and then stamps a single collision
 * cell under each of slots 17 and 18.
 *
 * Targets resolved with `tools/overlay_call_targets.ts` (overlay `bl` rule
 * `true_target_offset = stored_displacement + 2`): 7 sites, 3 distinct
 * callees.  The disassembler's annotations (0x020055e2, 0x02005658,
 * 0x02005660, 0x020027b0, 0x0200567e, 0x02005686, 0x020027d2) are all
 * artefacts; note that the four Func_0808a080 sites carry bit-identical `bl`
 * halfwords yet print four different targets, and the two Func_02000244 sites
 * likewise.  The real callees are the veneers at 0x0200309c
 * (-> Func_080091c0) and 0x0200310c (-> Func_0808a080, four sites) plus this
 * overlay's 0x02000244, for which assets/code/resource_3c4_c_02000244.c is
 * byte-exact.
 *
 * That byte-exact source settles the second call's meaning: it is
 * (layer, x, z, width, height, value) writing `value` into byte +2 of each
 * 4-byte cell of a width x height block.  Both sites here use 1 x 1 with the
 * value 255, i.e. they mark exactly the tile the slot is standing on.
 *
 * The tile coordinates are the slot's 12.20 words at +8 and +16 reduced with
 * `asrs #20`, so both are signed whole-tile indices.  Each slot is fetched
 * twice rather than once - the record pointer is not kept across the read of
 * +8 - which is what the assembly does.
 *
 * Previously skipped because 0x020027b0 and 0x020027d2 looked like in-image
 * addresses that are not function starts; both were decoding artefacts.
 *
 * r8 is used only to carry the constant 1 across the calls, and r6 only to
 * carry 255; both are saved and restored.
 *
 * The epilogue is `add sp,#8 ; pop {r3} ; mov r8,r3 ; pop {r5,r6} ;
 * pop {r0} ; bx r0`, so the owner is void.
 */
typedef signed int s32;
typedef unsigned char u8;

typedef struct Slot_0200252c {
    u8 pad0[8];
    s32 x;                 /* +8,  12.20 fixed point */
    s32 y;                 /* +12 */
    s32 z;                 /* +16, 12.20 fixed point */
} Slot_0200252c;

/* Four register arguments plus two stack words. */
void Func_080091c0();

/* Returns the actor record for the given slot. */
Slot_0200252c *Func_0808a080();

/* Byte-exact in assets/code: (layer, x, z, width, height, value). */
s32 Func_02000244(unsigned int layer, s32 x, s32 z,
                  unsigned int width, unsigned int height, s32 value);

void Func_0200252c(void)
{
    s32 x;
    s32 z;

    Func_080091c0(108, 19, 4, 1, 44, 19);

    x = Func_0808a080(17)->x >> 20;
    z = Func_0808a080(17)->z >> 20;
    Func_02000244(0, x, z, 1, 1, 255);

    x = Func_0808a080(18)->x >> 20;
    z = Func_0808a080(18)->z >> 20;
    Func_02000244(0, x, z, 1, 1, 255);
}
