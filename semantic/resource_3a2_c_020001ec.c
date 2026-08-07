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

void Func_020016e0();
void Func_0200174e();
void Func_020017b2();
void Func_02001768();
void Func_02001776();
void Func_02001784();
void Func_02001792();
void Func_020017f6();
void Func_02001802();
void Func_0200180e();
void Func_0200181a();
void Func_0200186c();
void Func_02001878();
void Func_02001822();
void Func_02001844();
void Func_0200180c();
void Func_02001814();
void Func_0200181c();
void Func_0200182c();
void Func_020017ba();
void Func_0200183a();
void Func_020017c8();
void Func_02001882();
void Func_02001852();
void Func_020017e0();
void Func_0200189a();
void Func_020017f8();
void Func_020018b2();
void Func_020018d6();
void Func_020018d0();
void Func_020018fc();
void Func_02001832();
void Func_020018ca();
void Func_02001840();
void Func_0200190c();
void Func_020018f4();
s32 Func_02001874();
void Func_02001866();
void Func_02001930();
void Func_0200194a();
void Func_02001934();
void Func_02001950();
void Func_0200195c();
void Func_0200192c();
void Func_02001986();
void Func_020018b4();
void Func_02001980();
void Func_0200197a();
void Func_020018fa();
void Func_020019c4();
void Func_02001984();
void Func_02001912();
void Func_020019cc();
void Func_020019f8();
void Func_020019f4();
void Func_02001a00();
void Func_020019fa();
void Func_020019e2();
void Func_02001958();
void Func_02001a24();
void Func_02001a30();
void Func_02001a3c();
void Func_02001a48();
void Func_0200198e();
void Func_02001a5a();
void Func_02001a54();
void Func_020019b2();
void Func_02001a32();
void Func_020019c0();
void Func_02001a40();
void Func_020019ce();
void Func_02001a88();
void Func_02001a70();
void Func_020019e6();
void Func_02001aa0();
void Func_02001a68();
void Func_02001a78();
void Func_02001a16();
void Func_02001a96();
void Func_02001af0();
void Func_02001b66();
void Func_02001a72();
void Func_02001aa4();
void Func_02001ac6();
void Func_02001b94();
void Func_02001a2e();
void Func_02001a74();
void Func_02001b98();
void Func_02001b10();
void Func_02001a86();
void Func_02001b52();
void Func_02001b4c();
void Func_02001b68();
void Func_02001b40();
void Func_02001ab6();
void Func_02001b82();
void Func_02001b7c();
void Func_02001b64();
void Func_02001ba0();
void Func_02001aee();
void Func_02001b86();
void Func_02001afc();
void Func_02001bc8();
void Func_02001bc2();
void Func_02001ba2();
void Func_02001baa();
void Func_02001bba();
void Func_02001b30();
void Func_02001bb0();
void Func_02001b3e();
void Func_02001bf8();
void Func_02001be0();
void Func_02001b56();
void Func_02001c10();
void Func_02001c2c();
void Func_02001bec();
void Func_02001b7a();
void Func_02001bfa();
void Func_02001b88();
void Func_02001c42();
void Func_02001c6e();
void Func_02001c58();
void Func_02001c74();
void Func_02001c34();
void Func_02001c7c();
void Func_02001ca8();
void Func_02001cb4();
void Func_02001c9e();
void Func_02001cba();
void Func_02001c7a();
void Func_02001c08();
void Func_02001cc2();
void Func_02001ca2();
void Func_02001caa();
void Func_02001c30();
void Func_02001c66();
void Func_02001c98();
void Func_02001d14();
void Func_02001d20();
void Func_02001d2c();
void Func_02001d38();
void Func_02001cf8();
void Func_02001c86();
void Func_02001d40();
void Func_02001d08();
void Func_02001d10();
void Func_02001cae();
void Func_02001ce4();
void Func_02001cf2();
void Func_02001d1c();
void Func_02001d26();
void Func_02001d30();
void Func_02001d4e();
void Func_02001d60();
void Func_02001d5e();
void Func_02001d70();
void Func_02001d6e();
void Func_02001d80();

                     

                     

                     

                     

                     

                     

                     

                     

void Func_020001ec(void)
{
    u8 *workspace;

    Func_020016e0();

    Func_0200174e(8, 136 << 16, 168 << 16);
    Func_020017b2(8, 160 << 7, 0);
    Func_02001768(0, 144 << 16, 200 << 16);
    Func_02001776(1, 160 << 16, 192 << 16);
    Func_02001784(2, 128 << 16, 200 << 16);
    Func_02001792(3, 224 << 15, 192 << 16);
    Func_020017f6(0, 192 << 8, 0);
    Func_02001802(1, 160 << 8, 0);
    Func_0200180e(2, 192 << 8, 0);
    Func_0200181a(3, 224 << 8, 0);

    workspace = Data_03001ebc;
    *(s32 *)(workspace + 448) = 513;

    Func_0200186c();
    Func_02001878();

    Func_02001776(60);
    Func_020017f6(8, 3);
    Func_02001784(20);
    Func_02001822(0x19e9);
    Func_02001844(8, 0, 20);

    Func_0200180c(0, 3);
    Func_02001814(1, 3);
    Func_0200181c(2, 3);
    Func_0200182c(3, 3);
    Func_020017ba(20);

    Func_0200183a(8, 4);
    Func_020017c8(20);
    Func_02001882(8, 0, 20);

    Func_02001852(3, 4);
    Func_020017e0(20);
    Func_0200189a(3, 0, 20);

    Func_02001882(2, 2);
    Func_020017f8(20);
    Func_020018b2(2, 0, 20);

    Func_0200189a(8, 2);
    Func_020018d6(8, 160 << 7, 20);
    Func_020018d0(8, 0, 20);

    Func_020018fc(2, 258, 60);
    Func_02001832(120);

    Func_020018ca(1, 2);
    Func_02001840(20);
    Func_0200190c(1, 192 << 7, 20);
    Func_020018f4(1, 0);

    if (Func_02001874(0, 0) == 0) {
        Func_02001866(20);
        Func_02001930(3, 0, 20);
        Func_0200194a(3, 0x101, 60);
        Func_02001934(3, 0, 20);
        Func_02001950(0, 224 << 8, 0);
        Func_0200195c(2, 224 << 8, 0);
        Func_0200192c(1, 2);
        Func_02001986(1, 258);
        Func_020018b4(60);
        Func_02001980(1, 128 << 8, 20);
        Func_0200197a(1, 0, 20);

    } else {
        *(u16 *)(Data_03001ebc + 472) += 2;
        *(u16 *)(Data_03001ebc + 472) += 2;

        Func_020018fa(20);
        Func_020019c4(3, 0, 20);
        Func_02001984(3, 3);
        Func_02001912(20);
        Func_020019cc(3, 0, 20);
        Func_020019f8(1, 258, 60);
        Func_020019f4(0, 224 << 8, 0);
        Func_02001a00(2, 224 << 8, 0);
        Func_020019fa(1, 0, 20);
    }

    Func_020019e2(8, 2);
    Func_02001958(20);
    Func_02001a24(0, 192 << 8, 0);
    Func_02001a30(1, 160 << 8, 0);
    Func_02001a3c(2, 192 << 8, 0);
    Func_02001a48(3, 224 << 8, 0);
    Func_0200198e(20);

    Func_02001a5a(8, 192 << 6, 20);
    Func_02001a54(8, 0, 20);

    Func_02001a3c(1, 2);
    Func_020019b2(20);
    Func_02001a32(1, 3);
    Func_020019c0(30);

    Func_02001a40(8, 3);
    Func_020019ce(20);
    Func_02001a88(8, 0, 20);

    Func_02001a70(8, 2);
    Func_020019e6(20);
    Func_02001aa0(8, 0, 20);

    Func_02001a68(0, 3);
    Func_02001a70(1, 3);
    Func_02001a78(2, 3);
    Func_02001a88(3, 3);
    Func_02001a16(20);

    Func_02001a96(8, 3);
    Func_02001a24(20);
    Func_02001af0(8, 192 << 8, 30);

    Func_02001b66(188);
    Func_02001a00(Data_02009788, 67, 6);
    Func_02001a72(8, 0xcccc, 0x6666);
    Func_02001aa4(8, 136, 136);
    Func_02001ac6(8, 0, 0);
    Func_02001b94(188);
    Func_02001a2e(Data_0200979e, 67, 6);
    Func_02001a74(60);
    Func_02001b98();

    Func_02001b10(1, 2);
    Func_02001a86(20);
    Func_02001b52(1, 192 << 7, 20);
    Func_02001b4c(1, 0, 20);

    Func_02001b68(0, 128 << 8, 20);
    Func_02001b40(2, 2);
    Func_02001ab6(20);
    Func_02001b82(3, 128 << 6, 20);
    Func_02001b7c(3, 0, 20);

    Func_02001b64(1, 1);
    Func_02001ba0(0, 224 << 8, 20);
    Func_02001ba0(1, 0);
    Func_02001aee(20);

    Func_02001b86(2, 2);
    Func_02001afc(20);
    Func_02001bc8(0, 128 << 8, 20);
    Func_02001bc2(2, 0, 20);

    Func_02001ba2(0, 1);
    Func_02001baa(1, 1);
    Func_02001bba(3, 1);
    Func_02001b30(20);

    Func_02001bb0(2, 4);
    Func_02001b3e(20);
    Func_02001bf8(2, 0, 20);

    Func_02001be0(3, 2);
    Func_02001b56(20);
    Func_02001c10(3, 0, 20);

    Func_02001c2c(2, 176 << 8, 20);
    Func_02001bec(2, 3);
    Func_02001b7a(20);
    Func_02001bfa(2, 3);
    Func_02001b88(20);
    Func_02001c42(2, 0, 20);

    Func_02001c6e(1, 258, 60);
    Func_02001c58(1, 0, 20);

    Func_02001c74(2, 224 << 8, 20);
    Func_02001c34(2, 4);
    Func_02001bc2(20);
    Func_02001c7c(2, 0, 20);

    Func_02001ca8(0, 258, 0);
    Func_02001cb4(3, 258, 60);
    Func_02001c9e(3, 0, 20);

    Func_02001cba(2, 160 << 8, 20);
    Func_02001c7a(2, 3);
    Func_02001c08(20);
    Func_02001cc2(2, 0, 20);

    Func_02001ca2(0, 1);
    Func_02001caa(1, 1);
    Func_02001cba(3, 1);
    Func_02001c30(20);

    Func_02001c66(2, 128 << 8, 128 << 7);
    Func_02001c98(2, 128, 184);
    Func_02001d14(2, 128 << 7, 20);
    Func_02001d20(0, 192 << 8, 0);
    Func_02001d2c(1, 160 << 8, 0);
    Func_02001d38(3, 224 << 8, 20);

    Func_02001cf8(2, 3);
    Func_02001c86(20);
    Func_02001d40(2, 0, 20);

    Func_02001d08(0, 3);
    Func_02001d10(1, 3);
    Func_02001d20(3, 3);
    Func_02001cae(20);

    Func_02001ce4(1, 128 << 8, 128 << 7);
    Func_02001cf2(3, 128 << 8, 128 << 7);
    Func_02001d1c(1, 144, 200);
    Func_02001d26(2, 144, 200);
    Func_02001d30(3, 144, 200);

    Func_02001d4e(1);
    Func_02001d60(1, 0, 0);
    Func_02001d5e(2);
    Func_02001d70(2, 0, 0);
    Func_02001d6e(3);
    Func_02001d80(3, 0, 0);

    Func_02001d2c();
}
