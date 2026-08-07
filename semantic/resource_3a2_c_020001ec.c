#include "types.h"

/*
 * resource_3a2 owner at 0x020001ec, 1632 bytes -- the overlay's largest.  It is
 * the room-0x58 cutscene, called from exactly one place: the overlay root
 * Func_020011b0 (see semantic/overlays/resource_3a2_c_020011b0.c) reaches it
 * when the sub-state Data_02000240[225] is 3 and progress flag 0x109 is clear.
 * `bun tools/lib/overlay_call_targets.ts resource_3a2` lists 0x020001ec as one of
 * the overlay's five internal `prologue` targets, and 0x11b0's own breakdown
 * accounts for that single site.
 *
 * LINK BASE.  0x02008000, proven in
 * semantic/overlays/resource_3a2_c_020011b0.c from the offset-0 exported-entry
 * veneer table (six odd words, all resolving at pool_word - 0x8000 to known
 * function starts, four of them already-tracked byte-exact siblings under
 * assets/code/).  In-image address = pool_word - 0x8000; odd = Thumb entry,
 * even = data.  This owner's two pointer pool words 0x02009788 and 0x0200979e
 * are both EVEN and both land past the import band's end at 0x1620, so they are
 * in-image data blocks -- and 0x02009788 is the very block the already-admitted
 * semantic/overlays/resource_3a2_c_02000180.c hands to the same import
 * Func_08009178 at the same placement (67, 6).
 *
 * Boundary.  Prologue `push {r5, lr}` at 0x020001ec, single interworking return
 * `pop {r5} / pop {r0} / bx r0` at 0x02000836.  Per the epilogue rule,
 * `pop {r0} ; bx r0` puts the popped RETURN ADDRESS in r0, so the owner returns
 * nothing: `void`.  It takes no arguments.
 *
 * POOL MAP, derived from a CONTROL-FLOW WALK, not from a scan -- and this owner
 * is why that matters: it has an INTERIOR pool.  The walk finds exactly two
 * branches, `bne.n 0x02000400` at 0x02000378 and `b.n 0x02000464` at
 * 0x020003f2.  Nothing reaches 0x020003f4-0x020003ff (the `b.n` jumps over it
 * and the `bne` lands past it), so those three words are pool sitting INSIDE the
 * row:
 *   0x020003f4 = 0x03001ebc  IWRAM workspace-POINTER cell
 *   0x020003f8 = 0x000019e9  dialogue cue id
 *   0x020003fc = 0x00000101  emote/id passed to Func_0808a1e8
 * The trailing pool is 0x0200083c-0x0200084b, which fills the span exactly
 * (0x020001ec + 1632 = 0x0200084c, the start of the byte-exact sibling
 * assets/code/resource_3a2_c_0200084c.c):
 *   0x0200083c = 0x02009788  in-image data block, file offset 0x1788
 *   0x02000840 = 0x0000cccc  16.16 scale x (0.8)
 *   0x02000844 = 0x00006666  16.16 scale y (0.4)
 *   0x02000848 = 0x0200979e  in-image data block, file offset 0x179e
 * Executable extent is therefore 1632 - 12 - 16 = 1604 bytes in two ranges,
 * 0x020001ec-0x020003f3 and 0x02000400-0x0200083b.
 *
 * Call sites resolved with `bun tools/lib/overlay_call_targets.ts resource_3a2 01ec`
 * -- never from overlay_show's printed annotations, since an overlay `bl` stores
 * target_offset - 2.  173 sites, 25 distinct veneer targets, no internal calls:
 *   0x14a8 -> Func_08009178   0x1558 -> Func_0808a100
 *   0x14e8 -> Func_0808a010   0x1560 -> Func_0808a110
 *   0x14f0 -> Func_0808a018   0x1570 -> Func_0808a130
 *   0x14f8 -> Func_0808a020   0x1578 -> Func_0808a138
 *   0x1500 -> Func_0808a070   0x1580 -> Func_0808a170
 *   0x1510 -> Func_0808a090   0x1588 -> Func_0808a178
 *   0x1530 -> Func_0808a0c8   0x1598 -> Func_0808a188
 *   0x1538 -> Func_0808a0d0   0x15a0 -> Func_0808a190
 *   0x1548 -> Func_0808a0e8   0x15a8 -> Func_0808a1b8
 *   0x1550 -> Func_0808a0f0   0x15b8 -> Func_0808a1e8
 *   0x15c0 -> Func_0808a1f0   0x15e8 -> Func_0808a360
 *   0x15f0 -> Func_0808a370   0x1608 -> Func_0808a5e8
 *   0x1618 -> Func_080f9010
 *
 * PER-TARGET MULTISET, C calls placed against the tool's per-target site counts
 * (173 = 173 target by target; the inventory's `calls=158` is a known FLOOR,
 * not an equality test):
 *   Func_0808a010 35=35   Func_0808a1b8 32=32   Func_0808a188 23=23
 *   Func_0808a110 16=16   Func_0808a138 13=13   Func_0808a0f0  9=9
 *   Func_0808a100  8=8    Func_0808a1e8  6=6    Func_0808a130  5=5
 *   Func_0808a090  4=4    Func_0808a0c8  3=3    Func_0808a0e8  3=3
 *   Func_0808a0d0  2=2    Func_08009178  2=2    Func_080f9010  2=2
 *   and exactly one each of Func_0808a018, Func_0808a020, Func_0808a070,
 *   Func_0808a170, Func_0808a178, Func_0808a190, Func_0808a1f0,
 *   Func_0808a360, Func_0808a370, Func_0808a5e8.
 *
 * NO ARM IS MERGED.  The two arms of the 0x02000378 test end in behaviourally
 * identical eight-instruction sequences and share five of their calls by shape,
 * but they are distinct call SITES and are written out separately; folding them
 * would deflate the multiset, which is counted over sites and not over
 * behaviour.  Nothing in this owner is reached from two arms, so no `goto` is
 * needed and none is used.
 *
 * SKIP-BEAT COUNTER, on BOTH arms.  `movs r3,#236 / lsls r3,r3,#1` = 472 off
 * the 0x03001ebc workspace pointer, with the u16 there bumped by 2 -- the
 * documented cross-overlay idiom, appearing here at 0x020003e4 (taken arm, at
 * the end) and at 0x02000400 (skipped arm, at the start).  Because it is on
 * both sides, "the empty else" is NOT the tell; the constants are.  The
 * distinct positions of the bump relative to the surrounding calls are
 * preserved.
 *
 * GUARD POLARITY read at the site, not assumed: `bl Func_0808a070` then
 * `cmp r0,#0 / bne.n`, so the LONG arm is the one taken when the result is
 * ZERO.  This overlay family uses both polarities, so the comparison is
 * transcribed rather than normalised.
 *
 * Behaviour: the full arrival scene for room 0x58, in the vocabulary the
 * already-admitted semantic/overlays/resource_3a2_c_02000b2c.c establishes for
 * this overlay -- Func_0808a018 opens and Func_0808a020 closes the beat,
 * Func_0808a010(n) waits n frames, Func_0808a188(actor, 0, n) and
 * Func_0808a1b8(actor, heading, n) are the move/turn pair, Func_0808a100/110/
 * 130/138 set pose indices, Func_0808a0f0 places an actor at a 16.16 point and
 * Func_0808a170 posts the dialogue cue (0x19e9).  It opens by placing actors 0,
 * 1, 2, 3 and 8 and publishing 513 into the workspace word at +448 (the slot
 * Func_02000180 sets to 32 and Func_020011b0 to 256), plays the conversation,
 * swaps the map block twice through Func_080f9010(188) + Func_08009178, and
 * ends by scaling and re-placing actors 1, 2 and 3.
 *
 * Headings are all built by shift, as everywhere in this overlay: 224<<8 =
 * 0xe000, 192<<8 = 0xc000, 160<<8 = 0xa000, 176<<8 = 0xb000, 128<<8 = 0x8000,
 * 160<<7 = 0x5000, 192<<7 = 0x6000, 128<<7 = 0x4000, 192<<6 = 0x3000,
 * 128<<6 = 0x2000; the id 129<<1 = 258 is the same one Func_02000b2c passes to
 * Func_0808a1f0/Func_0808a1e8.  The placement pairs are 16.16 (n<<16), except
 * 0x02000236's x which is 224<<15.
 *
 * Uncertainties: Func_0808a0c8 and Func_0808a0d0 are given the three registers
 * set at their sites without asserting what the pair means; Func_0808a178 and
 * Func_0808a190 take the two registers set at theirs.  Func_0808a070(0, 0)
 * returns the guard the scene branches on and its meaning is not named -- only
 * that the result is compared against zero.  Old-style declarations keep every
 * import's interface open, as overlay sources require.
 */

extern u8 *Data_03001ebc;
extern u8 Data_02009788[];
extern u8 Data_0200979e[];

void Func_08009178();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
s32 Func_0808a070();
void Func_0808a090();
void Func_0808a0c8();
void Func_0808a0d0();
void Func_0808a0e8();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a110();
void Func_0808a130();
void Func_0808a138();
void Func_0808a170();
void Func_0808a178();
void Func_0808a188();
void Func_0808a190();
void Func_0808a1b8();
void Func_0808a1e8();
void Func_0808a1f0();
void Func_0808a360();
void Func_0808a370();
void Func_0808a5e8();
void Func_080f9010();

void Func_020001ec(void)
{
    u8 *workspace;

    Func_0808a018();

    Func_0808a0f0(8, 136 << 16, 168 << 16);
    Func_0808a1b8(8, 160 << 7, 0);
    Func_0808a0f0(0, 144 << 16, 200 << 16);
    Func_0808a0f0(1, 160 << 16, 192 << 16);
    Func_0808a0f0(2, 128 << 16, 200 << 16);
    Func_0808a0f0(3, 224 << 15, 192 << 16);
    Func_0808a1b8(0, 192 << 8, 0);
    Func_0808a1b8(1, 160 << 8, 0);
    Func_0808a1b8(2, 192 << 8, 0);
    Func_0808a1b8(3, 224 << 8, 0);

    workspace = Data_03001ebc;
    *(s32 *)(workspace + 448) = 513;

    Func_0808a360();
    Func_0808a370();

    Func_0808a010(60);
    Func_0808a110(8, 3);
    Func_0808a010(20);
    Func_0808a170(0x19e9);
    Func_0808a188(8, 0, 20);

    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a100(2, 3);
    Func_0808a110(3, 3);
    Func_0808a010(20);

    Func_0808a110(8, 4);
    Func_0808a010(20);
    Func_0808a188(8, 0, 20);

    Func_0808a110(3, 4);
    Func_0808a010(20);
    Func_0808a188(3, 0, 20);

    Func_0808a138(2, 2);
    Func_0808a010(20);
    Func_0808a188(2, 0, 20);

    Func_0808a138(8, 2);
    Func_0808a1b8(8, 160 << 7, 20);
    Func_0808a188(8, 0, 20);

    Func_0808a1e8(2, 258, 60);
    Func_0808a010(120);

    Func_0808a138(1, 2);
    Func_0808a010(20);
    Func_0808a1b8(1, 192 << 7, 20);
    Func_0808a178(1, 0);

    if (Func_0808a070(0, 0) == 0) {
        Func_0808a010(20);
        Func_0808a1b8(3, 0, 20);
        Func_0808a1e8(3, 0x101, 60);
        Func_0808a188(3, 0, 20);
        Func_0808a1b8(0, 224 << 8, 0);
        Func_0808a1b8(2, 224 << 8, 0);
        Func_0808a130(1, 2);
        Func_0808a1f0(1, 258);
        Func_0808a010(60);
        Func_0808a1b8(1, 128 << 8, 20);
        Func_0808a188(1, 0, 20);

    } else {
        *(u16 *)(Data_03001ebc + 472) += 2;
        *(u16 *)(Data_03001ebc + 472) += 2;

        Func_0808a010(20);
        Func_0808a1b8(3, 0, 20);
        Func_0808a110(3, 3);
        Func_0808a010(20);
        Func_0808a188(3, 0, 20);
        Func_0808a1e8(1, 258, 60);
        Func_0808a1b8(0, 224 << 8, 0);
        Func_0808a1b8(2, 224 << 8, 0);
        Func_0808a188(1, 0, 20);
    }

    Func_0808a138(8, 2);
    Func_0808a010(20);
    Func_0808a1b8(0, 192 << 8, 0);
    Func_0808a1b8(1, 160 << 8, 0);
    Func_0808a1b8(2, 192 << 8, 0);
    Func_0808a1b8(3, 224 << 8, 0);
    Func_0808a010(20);

    Func_0808a1b8(8, 192 << 6, 20);
    Func_0808a188(8, 0, 20);

    Func_0808a138(1, 2);
    Func_0808a010(20);
    Func_0808a110(1, 3);
    Func_0808a010(30);

    Func_0808a110(8, 3);
    Func_0808a010(20);
    Func_0808a188(8, 0, 20);

    Func_0808a138(8, 2);
    Func_0808a010(20);
    Func_0808a188(8, 0, 20);

    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a100(2, 3);
    Func_0808a110(3, 3);
    Func_0808a010(20);

    Func_0808a110(8, 3);
    Func_0808a010(20);
    Func_0808a1b8(8, 192 << 8, 30);

    Func_080f9010(188);
    Func_08009178(Data_02009788, 67, 6);
    Func_0808a090(8, 0xcccc, 0x6666);
    Func_0808a0d0(8, 136, 136);
    Func_0808a0f0(8, 0, 0);
    Func_080f9010(188);
    Func_08009178(Data_0200979e, 67, 6);
    Func_0808a010(60);
    Func_0808a5e8();

    Func_0808a138(1, 2);
    Func_0808a010(20);
    Func_0808a1b8(1, 192 << 7, 20);
    Func_0808a188(1, 0, 20);

    Func_0808a1b8(0, 128 << 8, 20);
    Func_0808a138(2, 2);
    Func_0808a010(20);
    Func_0808a1b8(3, 128 << 6, 20);
    Func_0808a188(3, 0, 20);

    Func_0808a138(1, 1);
    Func_0808a1b8(0, 224 << 8, 20);
    Func_0808a190(1, 0);
    Func_0808a010(20);

    Func_0808a138(2, 2);
    Func_0808a010(20);
    Func_0808a1b8(0, 128 << 8, 20);
    Func_0808a188(2, 0, 20);

    Func_0808a130(0, 1);
    Func_0808a130(1, 1);
    Func_0808a138(3, 1);
    Func_0808a010(20);

    Func_0808a110(2, 4);
    Func_0808a010(20);
    Func_0808a188(2, 0, 20);

    Func_0808a138(3, 2);
    Func_0808a010(20);
    Func_0808a188(3, 0, 20);

    Func_0808a1b8(2, 176 << 8, 20);
    Func_0808a110(2, 3);
    Func_0808a010(20);
    Func_0808a110(2, 3);
    Func_0808a010(20);
    Func_0808a188(2, 0, 20);

    Func_0808a1e8(1, 258, 60);
    Func_0808a188(1, 0, 20);

    Func_0808a1b8(2, 224 << 8, 20);
    Func_0808a110(2, 4);
    Func_0808a010(20);
    Func_0808a188(2, 0, 20);

    Func_0808a1e8(0, 258, 0);
    Func_0808a1e8(3, 258, 60);
    Func_0808a188(3, 0, 20);

    Func_0808a1b8(2, 160 << 8, 20);
    Func_0808a110(2, 3);
    Func_0808a010(20);
    Func_0808a188(2, 0, 20);

    Func_0808a130(0, 1);
    Func_0808a130(1, 1);
    Func_0808a138(3, 1);
    Func_0808a010(20);

    Func_0808a090(2, 128 << 8, 128 << 7);
    Func_0808a0d0(2, 128, 184);
    Func_0808a1b8(2, 128 << 7, 20);
    Func_0808a1b8(0, 192 << 8, 0);
    Func_0808a1b8(1, 160 << 8, 0);
    Func_0808a1b8(3, 224 << 8, 20);

    Func_0808a110(2, 3);
    Func_0808a010(20);
    Func_0808a188(2, 0, 20);

    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a110(3, 3);
    Func_0808a010(20);

    Func_0808a090(1, 128 << 8, 128 << 7);
    Func_0808a090(3, 128 << 8, 128 << 7);
    Func_0808a0c8(1, 144, 200);
    Func_0808a0c8(2, 144, 200);
    Func_0808a0c8(3, 144, 200);

    Func_0808a0e8(1);
    Func_0808a0f0(1, 0, 0);
    Func_0808a0e8(2);
    Func_0808a0f0(2, 0, 0);
    Func_0808a0e8(3);
    Func_0808a0f0(3, 0, 0);

    Func_0808a020();
}
