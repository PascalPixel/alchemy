#include "types.h"

/*
 * resource_377 owner at 0x02000578, 1,172 bytes (0x0578..0x09c7), followed by
 * its trailing literal pool at 0x09c8..0x0a0b and the next owner's prologue at
 * 0x0a0c (semantic/overlays/resource_377_c_02000a0c.c).
 *
 * POOL MAP -- from a control-flow walk, not from "run to the next label".  This
 * owner has an INTERIOR pool at 0x063a..0x064f, in the middle of its own body:
 * the instruction at 0x02000638 is a bare unconditional 'b.n 0x2000650' with no
 * conditional structure around it, hopping the six pool words 0x00000000 (twice
 * as alignment), 0x03001ebc, 0x00000555, 0x028a0000 and 0x02160000.  Stopping
 * the body at that pool, or running it to the next branch target, would both be
 * wrong.  The single epilogue at 0x020009ba closes the owner, so the second
 * pool run 0x09c8..0x0a0b is likewise data.
 *
 * Signature: 'pop {r3,r5,r6} ; mov r8/r9/sl ; pop {r5,r6,r7} ; pop {r0} ;
 * bx r0' -- r0 holds the popped return address, so the owner returns nothing
 * (void).  Nothing reads r0 on entry (the body sets r0 = 17 before its first
 * call), so it takes no argument.
 *
 * Live registers held across the body: r9 = the address 0x03001ebc (the family
 * workspace pointer CELL, re-dereferenced at each use, as in the byte-exact
 * assets/code/resource_377_c_020002e0.c), r7 = the pointer read from the
 * neighbouring IWRAM cell 0x03001e70 (spelled Data_03001e70, as in
 * assets/code/resource_373_c_02000244.c), r6 = the pointer at workspace + 480,
 * r8 = the pointer at record17[80], and sl = 0.
 *
 * LINK BASE.  Pool words 0x02009ab4, 0x02009b04 and 0x02009b34 are EVEN and lie
 * in 0x0200_8xxx..0200_bxxx, so under the proven 0x02008000 overlay link base
 * they are in-image DATA at file offsets 0x1ab4, 0x1b04 and 0x1b34, past this
 * overlay's veneer band.  All three are the second argument of Func_0808a098,
 * the established animation-script shape.  Declared 'extern u8 Data_...[]'.
 *
 * Workspace stores, both in the documented displacement/value forms:
 *   0x020006ce  displacement 448, 'adds r3,#73'  -> value 521, then
 *               'subs r3,#65' -> next displacement 456, value 64
 *   0x0200099a  displacement 448, 'adds r3,#65'  -> value 513, then
 *               'subs r3,#57' -> next displacement 456, value 16
 * The u16 at workspace + 472 is the family skip-beat counter; note the guard
 * here is 'Func_0808a070(0, 0) == 0', the opposite polarity to the two sites in
 * resource_377_c_02000f90.c.
 *
 * Uncertainties recorded rather than resolved: the pooled first arguments
 * 0x9008 and 0x8008 of Func_0808a188/Func_0808a180/Func_0808a178 are unlike the
 * small ids every other site of those imports takes in this overlay; and the
 * four consecutive stores at r7+236..r7+248 and the three at r6+8..r6+16 are
 * written through pointers whose record layout no byte-exact sibling in this
 * overlay establishes.
 *
 * All 111 call sites resolved with
 * 'cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_377 0578 --json' (the inventory's
 * calls=106 is the documented undercount).  Per-target counts, verified target
 * by target against
 * 'overlay_show.ts | overlay_call_targets.ts --annotate | sort | uniq -c',
 * never by total:
 *   Func_0808a010 x17  Func_0808a080 x11  Func_0808a0f0 x8  Func_0808a100 x5
 *   Func_0808a0d0 x5   Func_0808a188 x4   Func_0808a138 x4  Func_0808a098 x4
 *   Func_0808a1b8 x3   Func_0808a110 x3   Func_0808a090 x3  Func_080091e0 x3
 *   Func_08009188 x3   Func_0808a370 x2   Func_0808a1d8 x2  Func_0808a158 x2
 *   Func_0808a0c0 x2   Func_08015210 x2   Func_08009190 x2  Func_080000c0 x2
 *   Func_020017e4 x1   Func_080f9010 x1   Func_0808a368 x1  Func_0808a360 x1
 *   Func_0808a2e0 x1   Func_0808a2d8 x1   Func_0808a2c8 x1  Func_0808a248 x1
 *   Func_0808a200 x1   Func_0808a1e0 x1   Func_0808a180 x1  Func_0808a178 x1
 *   Func_0808a170 x1   Func_0808a130 x1   Func_0808a128 x1  Func_0808a0a0 x1
 *   Func_0808a070 x1   Func_0808a018 x1   Func_080770c8 x1  Func_08009228 x1
 *   Func_08009210 x1   Func_08009208 x1   Func_080091f0 x1  Func_08009128 x1
 *                                                            (total 111)
 * The disassembler's own 'bl' annotations are wrong, as on every overlay.
 */

extern u8 *Data_03001ebc;
extern u8 *Data_03001e70;

extern u8 Data_02009ab4[];
extern u8 Data_02009b04[];
extern u8 Data_02009b34[];

u8 *Func_02001ec4();
void Func_02001eac();
void Func_02001f1e();
void Func_02001f28();
void Func_02001f32();
void Func_02001f3c();
void Func_02001f46();
void Func_02001f50();
u8 *Func_02001f0e();
void Func_02001e8c();
void Func_02001f6c();
u8 *Func_02001f2c();
u8 *Func_02001f38();
void Func_02001eb6();
void Func_02001f92();
void Func_02001ea8();
void Func_02001fa4();
void Func_02001ee8();
void Func_02002046();
void Func_02001f3a();
void Func_02001f68();
void Func_02001f20();
void Func_0200206e();
void Func_02001f60();
void Func_02001f44();
void Func_02001f2e();
void Func_02001edc();
void Func_0200211e();
void Func_0200213e();
void Func_02001f0c();
void Func_0200215a();
void Func_0200216e();
void Func_02002092();
void Func_020020d8();
void Func_020020fa();
void Func_020020c2();
void Func_02002030();
void Func_020020d0();
void Func_0200203e();
void Func_02002120();
void Func_020020e8();
void Func_02001fde();
void Func_0200205c();
void Func_02001fe2();
void Func_020020a0();
void Func_020020f0();
void Func_020020da();
void Func_02002010();
void Func_0200200e();
void Func_020020f2();
void Func_02002194();
u8 *Func_020020da_b();
void Func_02002058();
void Func_0200214a();
void Func_02002134();
void Func_020021cc();
void Func_020021c8();
void Func_02002168();
void Func_020020ee();
void Func_020021c6();
void Func_02001fda();
void Func_02002192();
void Func_020021e4();
u8 *Func_0200213a();
void Func_02002130();
u8 *Func_0200215e();
void Func_02002146();
void Func_020021e6();
u8 *Func_0200217c();
void Func_0200211a();
void Func_02002150();
void Func_0200230e();
void Func_0200221e();
void Func_02002174();
void Func_0200222c();
void Func_02002182();
u8 *Func_020021b0();
void Func_02002204();
void Func_020021a2();
u8 *Func_020021d0();
void Func_020021b6();
void Func_020021f4();
void Func_02002202();
void Func_020022ea();
u8 *Func_02002208();
void Func_0200222a();
void Func_020021f8();
void Func_02002238();
void Func_02002246();
void Func_02002278();
void Func_02002284();
void Func_0200229c();
void Func_020022a4();
void Func_02002320();
void Func_02002308();
s32 Func_02002268();
void Func_02002262();
void Func_02002344();
void Func_020022ec();
void Func_020022fc();
void Func_02002282();
void Func_020022c2();
void Func_020022ca();
void Func_02002298();
void Func_02002412();
void Func_0200241e();
void Func_020023ec();

                     

                     

                     

                     

                     

                     

                     

                     

                     

                     

                     

                     

                     

                    

void Func_02000578(void)
{
    u8 *workspace;
    u8 *cam;
    u8 *slot;
    u8 *record;

    slot = *(u8 **)(workspace + 480);
    workspace = Data_03001ebc;
    cam = Data_03001e70;
    record = *(u8 **)(Func_02001ec4(17) + 80);

    Func_02001eac();
    Func_02001f1e(11, 0, 0);
    Func_02001f28(12, 0, 0);
    Func_02001f32(13, 0, 0);
    Func_02001f3c(14, 0, 0);
    Func_02001f46(15, 0, 0);
    Func_02001f50(16, 0, 0);
    Func_02001e8c(Func_02001f0e(0), 0);
    Func_02001f6c(0, 18);

    Func_02001f2c(17)[85] = 0;
    *(u16 *)(record + 30) = 0x555;
    Func_02001eb6(Func_02001f38(17), 0);
    Func_02001f92(17, 0x02400000, 0x028A0000);
    Func_02001ea8(7);
    Func_02001fa4(8, 0x02160000, 0x02B00000);
    Func_02001ee8();

    /* the interior literal pool sits here, hopped by the b.n at 0x02000638 */

    Func_02002046(8);
    Func_02001f3a(0xE52, 1, 0);
    Func_02001f68(40);
    Func_02001f20(0x10000, 0x10000, 0x10000);
    Func_0200206e(8);
    Func_02001f60(0xE52 + 1, 1, 0);
    Func_02001f44();
    Func_02001f92(40);

    *(s32 *)(cam + 236) = 0x01480000;
    *(s32 *)(cam + 240) = 0x02580000;
    *(s32 *)(cam + 244) = 0x02700000;
    *(s32 *)(cam + 248) = 0x03300000;
    *(s32 *)(slot + 12) = 0;
    *(s32 *)(slot + 16) = 0x02B30000;
    *(s32 *)(slot + 8) = 0x02340000;

    Func_02001f2e();
    Func_02001edc(1);

    workspace = Data_03001ebc;
    *(s32 *)(workspace + 456) = 64;
    *(s32 *)(workspace + 448) = 521;

    Func_0200211e();
    *(u16 *)(((u8 **)&Data_03001ebc)[3] + 0x1F84) = 1;
    Func_0200213e();
    Func_02001f0c(30);
    Func_0200215a();
    Func_0200216e();
    Func_0200215a();

    Func_02002092(8, 4);
    Func_020020d8(0xE52 + 2);
    Func_020020fa(0x9008, 0, 60);
    Func_020020c2(0, 2);
    Func_02002030(40);
    Func_020020d0(8, 1);
    Func_0200203e(40);
    Func_02002120(0x9008, 0, 20);
    Func_020020e8(0, 2);
    Func_02001fde(7);
    Func_0200205c(20);
    Func_02001fe2(8);
    Func_020020a0(0, 0x10000, 0x8000);
    Func_020020f0(0, 19);
    Func_020020da(0, 0x22D, 0x2A7);
    Func_02002010(8);
    Func_0200200e(9);
    Func_020020f2(0, 0x22B, 680);
    Func_020020a0(30);
    Func_02002194(8, 0xD000, 0);
    Func_02002058(Func_020020da_b(0), 1);
    Func_0200214a(0, 4, 0);
    Func_02002134(0, 0x21F, 0x2A2);
    Func_020021cc(0, 3);
    Func_020021c8(0, 0x4000, 40);
    Func_02002168(8, 4);
    Func_020020ee(20);
    Func_020021c6(0x9008, 0);
    Func_02001fda();
    Func_02002192(8, 2);
    Func_020021e4(0x9008, 0, 20);

    Func_0200213a(8)[90] &= (u8)0xFE;
    Func_02002192(8, 0x21E, 680);
    Func_02002130(1);
    Func_0200215e(8)[90] |= 1;
    Func_02002146(10);
    Func_020021e6(8, 2);
    Func_0200211a(Func_0200217c(0), 226);
    Func_02002150(33);
    Func_0200230e(126);
    Func_0200221e(0, 7);
    Func_02002174(10);
    Func_0200222c(0, 0);
    Func_02002182(20);

    Func_020021b0(8)[90] &= (u8)0xFE;
    Func_02002204(8, 0x216, 688);
    Func_020021a2(1);
    Func_020021d0(8)[90] |= 1;
    Func_020021b6(20);
    Func_020021f4(8, 0x18000, 0xC000);
    Func_02002202(0, 0x18000, 0xC000);
    Func_020022ea(8, 1);
    Func_02002208(0)[35] |= 1;

    Func_0200222a(8, Data_02009ab4);
    Func_020021f8(20);
    Func_02002238(0, Data_02009ab4);
    Func_02002246(8);
    Func_02002278(8, 0x1A3, 0x295);
    Func_02002284(8, 408, 0x295);
    Func_0200229c(8, 1);
    Func_020022a4(0, 1);
    Func_02002320(8, 0x4000, 10);
    Func_02002308(0x8008, 0);

    if (Func_02002268(0, 0) == 0) {
        *(u16 *)(Data_03001ebc + 472) += 1;
    }

    Func_02002262(20);
    Func_02002344(0x8008, 0, 20);
    Func_020022ec(0, 3);
    Func_020022fc(8, 3);
    Func_02002282(20);
    Func_020022c2(8, Data_02009b04);
    Func_020022ca(0, Data_02009b34);
    Func_02002298(20);

    workspace = Data_03001ebc;
    *(s32 *)(workspace + 448) = 513;
    *(s32 *)(workspace + 456) = 16;

    Func_02002412();
    Func_0200241e();
    Func_020023ec(20);
}
