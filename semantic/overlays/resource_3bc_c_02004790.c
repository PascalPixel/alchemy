typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

/*
 * resource_3bc owner at 0x02004790, 160 bytes: try to walk the caller's
 * scene record forward along its own heading, one tile-unit then two, and
 * return the first collision probe's nonzero rejection (or the second
 * probe's result if the first passed).
 *
 * Complete owner: `push {r5, r6, r7, lr} / mov r7,r8 / push {r7}` at
 * 0x02004790 through the matching interworking return `pop {r3} / mov
 * r8,r3 / pop {r5, r6, r7} / pop {r1} / bx r1` at 0x200481c-0x02004824.
 * The popped return address goes to r1, so r0 survives -- the owner
 * returns whatever the last Func_20045e0 call (or the early-exit branch)
 * left there. One argument slot but the record id is read straight from
 * Data_02000240, not a parameter, so this owner takes none.
 *
 * Not found by the structural inventory walk (unindexed); two incoming
 * call sites per overlay_unindexed.ts. This owner makes six outgoing calls
 * over three distinct targets, all resolved with the `+2` rule against the
 * raw image (this owner starts outside a recorded row, so
 * overlay_call_targets.ts's own table does not cover it -- resolved with
 * `bun tools/overlay_show.ts resource_3bc 4790 | bun
 * tools/overlay_call_targets.ts resource_3bc 4790 4824 --annotate`):
 *   0x20047a2 -> veneer -> Func_0808a080(id)   scene-record accessor, established
 *   0x20047dc, 0x200480e -> veneer -> Func_08000128(distance, heading, probe)   established, "advance the position block one step along a heading" (semantic/overlays/resource_395_c_0200152c.c); called with 0x100000 then 0x200000, the same two-step walk resource_3bb_c_02003cf8.c documents
 *   0x20047e4, 0x2004816 -> raw prologue -> Func_20045e0(probe, record)   in-overlay, same role as Func_080091d8 in resource_3c4_c_02000cd0.c (reject a probe/target), not itself in this overlay's item-28 list
 *
 * The record layout (.tag at +6 as u16, .x/.y/.z at +8/+12/+16 as s32 12.20
 * fixed point) and the probe-construction idiom (snap the horizontal words
 * to their tile grid with the 0xfff00000 mask, lift by half a tile with
 * +0x80000, leave the vertical word unmasked; heading = (tag + 0x2000) &
 * 0xc000) are BYTE-IDENTICAL to resource_3c4_c_02000cd0.c's Actor_02000cd0 /
 * probe-build code -- that file's struct and comments are the reference for
 * every field used here.
 */

extern u8 Data_02000240[];

u8 *Func_0808a080();           /* scene-record accessor, established */
void Func_08000128();          /* advance a position block along a heading, established */
s32 Func_20045e0();            /* in-overlay probe rejection, undrafted */

s32 Func_02004790(void)
{
    u8 *record;
    s32 heading;
    s32 probe[3];
    s32 result;

    record = Func_0808a080(*(s32 *)&Data_02000240[500]);

    heading = (*(u16 *)(record + 6) + 0x2000) & 0xc000;

    probe[0] = (*(s32 *)(record + 8) & (s32)0xfff00000) + 0x80000;
    probe[1] = *(s32 *)(record + 12);
    probe[2] = (*(s32 *)(record + 16) & (s32)0xfff00000) + 0x80000;

    Func_08000128(0x100000, heading, probe);
    result = Func_20045e0(probe, record);
    if (result != 0) {
        return result;
    }

    probe[0] = (*(s32 *)(record + 8) & (s32)0xfff00000) + 0x80000;
    probe[1] = *(s32 *)(record + 12);
    probe[2] = (*(s32 *)(record + 16) & (s32)0xfff00000) + 0x80000;

    Func_08000128(0x200000, heading, probe);
    return Func_20045e0(probe, record);
}
