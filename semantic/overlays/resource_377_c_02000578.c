typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

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
 * 'bun tools/overlay_call_targets.ts resource_377 0578 --json' (the inventory's
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

u8 *Func_0808a080();
void Func_0808a018();
void Func_0808a0f0();
void Func_080091e0();
void Func_0808a100();
void Func_08009188();
void Func_08009208();
void Func_0808a1d8();
void Func_08015210();
void Func_0808a010();
void Func_080091f0();
void Func_08009210();
void Func_08009128();
void Func_080000c0();
void Func_0808a2c8();
void Func_0808a2d8();
void Func_0808a360();
void Func_0808a370();
void Func_0808a2e0();
void Func_0808a110();
void Func_0808a170();
void Func_0808a188();
void Func_0808a138();
void Func_08009190();
void Func_0808a090();
void Func_0808a0c0();
void Func_0808a1b8();
void Func_0808a128();
void Func_0808a0d0();
void Func_0808a1e0();
void Func_0808a180();
void Func_020017e4();
void Func_0808a130();
void Func_08009228();
void Func_080770c8();
void Func_080f9010();
void Func_0808a158();
void Func_0808a200();
void Func_0808a098();
void Func_0808a0a0();
void Func_0808a178();
s32 Func_0808a070();
void Func_0808a368();
void Func_0808a248();

void Func_02000578(void)
{
    u8 *workspace;
    u8 *cam;
    u8 *slot;
    u8 *record;

    workspace = Data_03001ebc;
    cam = Data_03001e70;
    slot = *(u8 **)(workspace + 480);
    record = *(u8 **)(Func_0808a080(17) + 80);

    Func_0808a018();
    Func_0808a0f0(11, 0, 0);
    Func_0808a0f0(12, 0, 0);
    Func_0808a0f0(13, 0, 0);
    Func_0808a0f0(14, 0, 0);
    Func_0808a0f0(15, 0, 0);
    Func_0808a0f0(16, 0, 0);
    Func_080091e0(Func_0808a080(0), 0);
    Func_0808a100(0, 18);

    *(u16 *)(record + 30) = 0x555;
    Func_0808a080(17)[85] = 0;
    Func_080091e0(Func_0808a080(17), 0);
    Func_0808a0f0(17, 0x02400000, 0x028A0000);
    Func_08009188(7);
    Func_0808a0f0(8, 0x02160000, 0x02B00000);
    Func_08009208();

    /* the interior literal pool sits here, hopped by the b.n at 0x02000638 */

    Func_0808a1d8(8);
    Func_08015210(0xE52, 1, 0);
    Func_0808a010(40);
    Func_080091f0(0x10000, 0x10000, 0x10000);
    Func_0808a1d8(8);
    Func_08015210(0xE52 + 1, 1, 0);
    Func_08009210();
    Func_0808a010(40);

    *(s32 *)(cam + 236) = 0x01480000;
    *(s32 *)(cam + 240) = 0x02580000;
    *(s32 *)(cam + 244) = 0x02700000;
    *(s32 *)(cam + 248) = 0x03300000;
    *(s32 *)(slot + 8) = 0x02340000;
    *(s32 *)(slot + 12) = 0;
    *(s32 *)(slot + 16) = 0x02B30000;

    Func_08009128();
    Func_080000c0(1);

    workspace = Data_03001ebc;
    *(s32 *)(workspace + 448) = 521;
    *(s32 *)(workspace + 456) = 64;

    Func_0808a2c8();
    *(u16 *)(((u8 **)&Data_03001ebc)[3] + 0x1F84) = 1;
    Func_0808a2d8();
    Func_080000c0(30);
    Func_0808a360();
    Func_0808a370();
    Func_0808a2e0();

    Func_0808a110(8, 4);
    Func_0808a170(0xE52 + 2);
    Func_0808a188(0x9008, 0, 60);
    Func_0808a138(0, 2);
    Func_0808a010(40);
    Func_0808a138(8, 1);
    Func_0808a010(40);
    Func_0808a188(0x9008, 0, 20);
    Func_0808a138(0, 2);
    Func_08009190(7);
    Func_0808a010(20);
    Func_08009188(8);
    Func_0808a090(0, 0x10000, 0x8000);
    Func_0808a100(0, 19);
    Func_0808a0c0(0, 0x22D, 0x2A7);
    Func_08009190(8);
    Func_08009188(9);
    Func_0808a0c0(0, 0x22B, 680);
    Func_0808a010(30);
    Func_0808a1b8(8, 0xD000, 0);
    Func_080091e0(Func_0808a080(0), 1);
    Func_0808a128(0, 4, 0);
    Func_0808a0d0(0, 0x21F, 0x2A2);
    Func_0808a1e0(0, 3);
    Func_0808a1b8(0, 0x4000, 40);
    Func_0808a110(8, 4);
    Func_0808a010(20);
    Func_0808a180(0x9008, 0);
    Func_020017e4();
    Func_0808a130(8, 2);
    Func_0808a188(0x9008, 0, 20);

    Func_0808a080(8)[90] &= (u8)0xFE;
    Func_0808a0d0(8, 0x21E, 680);
    Func_0808a010(1);
    Func_0808a080(8)[90] |= 1;
    Func_0808a010(10);
    Func_0808a138(8, 2);
    Func_08009228(Func_0808a080(0), 226);
    Func_080770c8(33);
    Func_080f9010(126);
    Func_0808a158(0, 7);
    Func_0808a010(10);
    Func_0808a158(0, 0);
    Func_0808a010(20);

    Func_0808a080(8)[90] &= (u8)0xFE;
    Func_0808a0d0(8, 0x216, 688);
    Func_0808a010(1);
    Func_0808a080(8)[90] |= 1;
    Func_0808a010(20);
    Func_0808a090(8, 0x18000, 0xC000);
    Func_0808a090(0, 0x18000, 0xC000);
    Func_0808a200(8, 1);
    Func_0808a080(0)[35] |= 1;

    Func_0808a098(8, Data_02009ab4);
    Func_0808a010(20);
    Func_0808a098(0, Data_02009ab4);
    Func_0808a0a0(8);
    Func_0808a0d0(8, 0x1A3, 0x295);
    Func_0808a0d0(8, 408, 0x295);
    Func_0808a100(8, 1);
    Func_0808a100(0, 1);
    Func_0808a1b8(8, 0x4000, 10);
    Func_0808a178(0x8008, 0);

    if (Func_0808a070(0, 0) == 0) {
        *(u16 *)(Data_03001ebc + 472) += 1;
    }

    Func_0808a010(20);
    Func_0808a188(0x8008, 0, 20);
    Func_0808a100(0, 3);
    Func_0808a110(8, 3);
    Func_0808a010(20);
    Func_0808a098(8, Data_02009b04);
    Func_0808a098(0, Data_02009b34);
    Func_0808a010(20);

    workspace = Data_03001ebc;
    *(s32 *)(workspace + 448) = 513;
    *(s32 *)(workspace + 456) = 16;

    Func_0808a368();
    Func_0808a370();
    Func_0808a248(20);
}
