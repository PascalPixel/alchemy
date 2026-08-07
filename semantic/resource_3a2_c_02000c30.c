#include "types.h"

/*
 * resource_3a2 owner at 0x02000c30, 924 bytes.  A scripted cutscene beat,
 * reached as a Thumb callback: the pool word 0x02008c31 sits inside this
 * overlay's script/table block near file offset 0x18xx, and under the proven
 * 0x02008000 link base it is file offset 0xc30 plus the Thumb bit -- odd, so a
 * function entry, per the parity rule.  Nothing in the image `bl`s it, which is
 * consistent: `bun tools/lib/overlay_call_targets.ts resource_3a2` reports the
 * overlay's five internal prologue targets as 0x01ec, 0x0870, 0x0b2c, 0x10b8
 * and 0x1144, and 0x0c30 is not among them.
 *
 * LINK BASE.  Proven at 0x02008000 for this overlay in
 * semantic/overlays/resource_3a2_c_020011b0.c: image offset 0 is an
 * exported-entry veneer table whose six words are all odd and all resolve, at
 * pool_word - 0x8000, to known function starts, four of them already-tracked
 * byte-exact siblings under assets/code/.  In-image address = pool_word -
 * 0x8000; odd = Thumb entry, even = data.
 *
 * Boundary.  Prologue `push {r5, lr}` at 0x02000c30, single interworking return
 * `pop {r5} / pop {r0} / bx r0` at 0x02000faa.  Per the epilogue rule,
 * `pop {r0} ; bx r0` means r0 holds the popped RETURN ADDRESS, so this owner
 * returns nothing: it is `void`.  It takes no arguments -- r0 is set to a
 * constant before its first use.
 *
 * The body is entirely straight-line: a control-flow walk from the prologue
 * meets no branch of any kind, so every halfword from 0x02000c30 to 0x02000faf
 * is code and everything at or after 0x02000fb0 is pool.  That pool fills the
 * row's span exactly (0x02000c30 + 924 = 0x02000fcc, which is the start of the
 * byte-exact sibling assets/code/resource_3a2_c_02000fcc.c):
 *   0x02000fb0 = 0x000019da  dialogue cue id
 *   0x02000fb4 = 0x00003333  16.16 scale x (0.2)
 *   0x02000fb8 = 0x00001999  16.16 scale y (0.1)
 *   0x02000fbc = 0x0000cccc  16.16 scale x (0.8)
 *   0x02000fc0 = 0x00006666  16.16 scale y (0.4)
 *   0x02000fc4 = 0x000008b2  progress-flag id
 *   0x02000fc8 = 0x03001ebc  IWRAM workspace-POINTER cell
 * The 0xcccc/0x6666 pair is the same one the admitted
 * semantic/overlays/resource_3a2_c_02000180.c passes to Func_0808a090, which is
 * independent confirmation of that import's 16.16 argument shape.
 *
 * Call sites resolved with `bun tools/lib/overlay_call_targets.ts resource_3a2 0c30`
 * -- never from overlay_show's printed annotations, since an overlay `bl` stores
 * target_offset - 2.  104 sites, 23 distinct veneer targets, no internal calls:
 *   0x14d8 -> Func_080770c8   0x1540 -> Func_0808a0e0
 *   0x14e8 -> Func_0808a010   0x1548 -> Func_0808a0e8
 *   0x14f0 -> Func_0808a018   0x1558 -> Func_0808a100
 *   0x14f8 -> Func_0808a020   0x1560 -> Func_0808a110
 *   0x1508 -> Func_0808a080   0x1568 -> Func_0808a128
 *   0x1510 -> Func_0808a090   0x1570 -> Func_0808a130
 *   0x1528 -> Func_0808a0a8   0x1578 -> Func_0808a138
 *   0x1530 -> Func_0808a0c8   0x1580 -> Func_0808a170
 *   0x1538 -> Func_0808a0d0   0x1598 -> Func_0808a188
 *   0x15a0 -> Func_0808a190   0x15a8 -> Func_0808a1b8
 *   0x15c0 -> Func_0808a1f0   0x15e0 -> Func_0808a248
 *   0x1618 -> Func_080f9010
 *
 * PER-TARGET MULTISET, C calls placed against the tool's per-target site counts
 * (104 = 104 target by target; the inventory's `calls=100` is a known FLOOR,
 * not an equality test):
 *   Func_0808a010 22=22   Func_0808a100 13=13   Func_0808a0e0 12=12
 *   Func_0808a188 12=12   Func_0808a110  7=7    Func_0808a0e8  5=5
 *   Func_0808a1b8  5=5    Func_080f9010  4=4    Func_0808a080  3=3
 *   Func_0808a090  3=3    Func_0808a128  3=3    Func_0808a0d0  2=2
 *   Func_0808a138  2=2    Func_0808a1f0  2=2
 *   and exactly one each of Func_0808a018, Func_0808a020, Func_0808a0a8,
 *   Func_0808a0c8, Func_0808a130, Func_0808a170, Func_0808a190,
 *   Func_0808a248, Func_080770c8.
 * Because the owner is branch-free the transcription is one C statement per
 * assembly call in source order, so no shared-tail, per-arm-copy or
 * condition-feeding duplication can arise.
 *
 * Behaviour: the long conversation beat between actors 8 and 10 (with 9 as a
 * bystander), in the same vocabulary the admitted
 * semantic/overlays/resource_3a2_c_02000b2c.c establishes for this overlay --
 * Func_0808a018 opens the beat and Func_0808a020 closes it, Func_0808a010(n)
 * waits n frames, Func_0808a188(actor, 0, n) and Func_0808a1b8(actor, heading,
 * n) are the move/turn pair, Func_0808a100/Func_0808a110/Func_0808a130/
 * Func_0808a138 set pose indices, and Func_0808a170 posts the dialogue cue
 * (0x19da here).  Four times the beat runs the identical four-call camera
 * figure -- Func_080f9010(125) then Func_0808a0e0 on actors 8, 9 and 10 with a
 * step of 2 or 4 -- which is written out each time rather than folded into a
 * loop, because folding would deflate the multiset by twelve.  It ends by
 * posting progress flag 0x8b2, writing 513 into the workspace word at +448 (the
 * slot Func_02000180 sets to 32 and Func_020011b0 to 256) and closing with
 * Func_0808a248(6).
 *
 * Headings are all built by shift, as elsewhere in this overlay: 208<<8 =
 * 0xd000, 160<<7 = 0x5000, 176<<8 = 0xb000; the id 129<<1 = 258 is the same one
 * Func_02000b2c passes to Func_0808a1f0.  The `movs r2,#N / lsls r2,r2,#1`
 * pairs feeding Func_0808a0c8/Func_0808a0d0 are values 276, 284 and 296, not
 * displacements.
 *
 * Uncertainties: Func_0808a0c8 and Func_0808a0d0 are given three arguments each
 * because three registers are set at their sites; their meaning is not asserted.
 * The record byte at +90 is cleared for actors 8 and 10 early and set for actor
 * 10 late, matching the +85/+89 record-byte writes of the overlay's other
 * owners, and the record pointer comes from Func_0808a080 exactly as in
 * semantic/overlays/resource_3a2_c_02000180.c.  Old-style declarations keep
 * every import's interface open, as overlay sources require.
 */

extern u8 *Data_03001ebc;

void Func_02002124();
void Func_020021fe();
void Func_020021b6();
void Func_02002134();
void Func_020021d2();
void Func_020021f4();
void Func_02002226();
void Func_020021d8();
void Func_0200215e();
void Func_02002218();
void Func_02002200();
void Func_02002176();
void Func_02002230();
void Func_0200224c();
void Func_0200220c();
void Func_0200219a();
void Func_020021ee();
void Func_02002202();
void Func_02002284();
void Func_02002290();
void Func_020021d6();
void Func_0200226e();
void Func_020021e4();
void Func_0200229e();
u8 *Func_02002214();
u8 *Func_02002226_b();
void Func_02002240();
void Func_0200224a();
void Func_0200229a();
void Func_020022a2();
void Func_02002238();
void Func_0200236e();
void Func_020022a0();
void Func_020022aa();
void Func_020022b4();
void Func_020022c2();
void Func_02002268();
void Func_020022e0();
void Func_020022e8();
void Func_0200227e();
void Func_020023b4();
void Func_020022e6();
void Func_020022f0();
void Func_020022fa();
void Func_02002308();
void Func_020022ee();
void Func_02002326();
void Func_0200232e();
void Func_020022c4();
void Func_0200234e();
void Func_020022d4();
void Func_0200238e();
void Func_0200235e();
void Func_020022ec();
void Func_020023ac();
void Func_020023ae();
void Func_02002376();
void Func_0200237e();
void Func_02002314();
void Func_0200244a();
void Func_0200237c();
void Func_02002386();
void Func_02002390();
void Func_0200239e();
void Func_02002344();
void Func_020023bc();
void Func_020023c4();
void Func_0200235a();
void Func_02002490();
void Func_020023c2();
void Func_020023cc();
void Func_020023d6();
void Func_020023e4();
void Func_0200238a();
void Func_02002402();
void Func_0200240a();
void Func_02002424();
void Func_020023aa();
void Func_02002476();
void Func_02002470();
void Func_02002440();
void Func_020023ce();
void Func_02002488();
void Func_02002492();
u8 *Func_02002408();
void Func_02002458();
void Func_020024d4();
void Func_0200248c();
void Func_0200249c();
void Func_0200242a();
void Func_020024e4();
void Func_020024b4();
void Func_02002442();
void Func_020024fc();
void Func_020024cc();
void Func_0200245a();
void Func_02002514();
void Func_02002472();
void Func_02002486();
void Func_0200246c();
void Func_02002588();

                     

                     

                     

                     

                     

                     

                     

void Func_02000c30(void)
{
    u8 *workspace;

    Func_02002124();

    Func_020021fe(8, 258);
    Func_020021b6(8, 2);
    Func_02002134(60);
    Func_020021d2(0x19da);
    Func_020021f4(8, 0, 20);

    Func_02002226(10, 258);
    Func_020021d8(10, 4, 0);
    Func_0200215e(60);
    Func_02002218(10, 0, 20);

    Func_02002200(8, 1);
    Func_02002176(20);
    Func_02002230(8, 0, 20);

    Func_0200224c(10, 208 << 8, 20);
    Func_0200220c(10, 3);
    Func_0200219a(20);

    Func_020021ee(8, 178, 138 << 1);
    Func_02002202(10, 172, 142 << 1);
    Func_02002218(8);

    Func_02002284(8, 160 << 7, 0);
    Func_02002290(10, 176 << 8, 0);
    Func_020021d6(20);

    Func_0200226e(8, 2);
    Func_020021e4(20);
    Func_0200229e(8, 0, 20);

    Func_02002214(8)[90] &= 0xfe;
    Func_02002226_b(10)[90] &= 0xfe;
    Func_02002240(8, 0x3333, 0x1999);
    Func_0200224a(10, 0x3333, 0x1999);

    Func_0200229a(8, 5);
    Func_020022a2(10, 6);
    Func_02002238(20);
    Func_0200236e(125);
    Func_020022a0(8, 2, 0);
    Func_020022aa(9, 2, 0);
    Func_020022b4(10, 2, 0);
    Func_020022c2(10);
    Func_02002268(30);

    Func_020022e0(8, 5);
    Func_020022e8(10, 6);
    Func_0200227e(20);
    Func_020023b4(125);
    Func_020022e6(8, 4, 0);
    Func_020022f0(9, 4, 0);
    Func_020022fa(10, 4, 0);
    Func_02002308(10);
    Func_020022ee(9);

    Func_02002326(8, 1);
    Func_0200232e(10, 1);
    Func_020022c4(50);

    Func_0200234e(10, 2, 0);
    Func_020022d4(20);
    Func_0200238e(10, 0, 20);

    Func_0200235e(8, 3);
    Func_020022ec(20);
    Func_020023ac(8, 0);
    Func_020023ae(8, 0, 20);

    Func_02002376(8, 5);
    Func_0200237e(10, 6);
    Func_02002314(20);
    Func_0200244a(125);
    Func_0200237c(8, 2, 0);
    Func_02002386(9, 2, 0);
    Func_02002390(10, 2, 0);
    Func_0200239e(10);
    Func_02002344(30);

    Func_020023bc(8, 5);
    Func_020023c4(10, 6);
    Func_0200235a(20);
    Func_02002490(125);
    Func_020023c2(8, 4, 0);
    Func_020023cc(9, 4, 0);
    Func_020023d6(10, 4, 0);
    Func_020023e4(10);
    Func_0200238a(40);

    Func_02002402(8, 1);
    Func_0200240a(10, 1);
    Func_02002424(10, 2, 0);
    Func_020023aa(20);
    Func_02002476(10, 208 << 8, 20);
    Func_02002470(10, 0, 20);

    Func_02002440(8, 3);
    Func_020023ce(20);
    Func_02002488(8, 0, 30);
    Func_02002492(8, 0, 20);

    Func_02002408(10)[90] |= 1;
    Func_02002424(10, 0xcccc, 0x6666);
    Func_02002458(10, 168, 148 << 1);
    Func_020024d4(10, 208 << 8, 20);
    Func_0200248c(10, 5);

    Func_0200249c(8, 3);
    Func_0200242a(20);
    Func_020024e4(8, 0, 20);

    Func_020024b4(10, 3);
    Func_02002442(20);
    Func_020024fc(10, 0, 20);

    Func_020024cc(8, 3);
    Func_0200245a(20);
    Func_02002514(8, 0, 20);

    Func_020024e4(0, 3);
    Func_02002472(20);

    Func_02002486();
    Func_0200246c(0x8b2);

    *(s32 *)(workspace + 448) = 513;
    workspace = Data_03001ebc;

    Func_02002588(6);
}
