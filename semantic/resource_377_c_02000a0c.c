#include "types.h"

/*
 * resource_377 owner at 0x02000a0c, 1,064 bytes (0x0a0c..0x0dc7), followed by
 * its literal pool at 0x0dc8..0x0e33 and the next owner's 'push {r5,lr}'
 * prologue at 0x0e34 (the byte-exact-adjacent semantic source
 * resource_377_c_02000e34.c).  Pool map derived by walking control flow from
 * the prologue: the body is straight-line apart from the two backward 'bls'
 * loops at 0x02000cc0 and 0x02000ce4, and the epilogue at 0x02000dba is the
 * only exit, so nothing at or after 0x0dc8 is code.
 *
 * Signature: 'add sp,#8 ; pop {r3,r5} ; mov r8,r3 ; mov sl,r5 ;
 * pop {r5,r6,r7} ; pop {r0} ; bx r0' -- r0 holds the popped return address, so
 * the owner returns nothing (void).  Nothing reads r0 on entry: the first thing
 * the body does is set r0 = 10 for Func_0808a080, so the owner takes no
 * argument.  The 8-byte frame carries the two stack arguments of the
 * six-argument Func_08009180.
 *
 * LINK BASE.  The pool words 0x02009cec, 0x02009bb4, 0x02009b78, 0x02009c04,
 * 0x02009c54, 0x02009d38 and 0x02009ca4 are all EVEN and all sit in
 * 0x0200_8xxx..0200_bxxx, so under the project's proven 0x02008000 overlay link
 * base they are in-image DATA at file offsets 0x1cec, 0x1bb4, 0x1b78, 0x1c04,
 * 0x1c54, 0x1d38, 0x1ca4 -- past this overlay's veneer band (which ends around
 * 0x1a6c) and therefore in its data region.  Every one of them is used as the
 * second argument of Func_0808a098 / Func_0808a0b0, which is the established
 * "animation script" shape.  Declared here as 'extern u8 Data_0200XXXX[]'.
 *
 * Data_03001ebc is the family workspace pointer cell (byte-exact sibling
 * assets/code/resource_377_c_020002e0.c).  Two of the documented
 * displacement/value idioms appear:
 *   0x02000ab0  additive:  displacement 448, 'adds r2,#65' -> value 513
 *   0x02000d9a  subtractive: displacement 448, 'subs r3,#192' -> value 256,
 *               then 'adds r3,#200' -> the NEXT displacement 456, value 32
 * so the stores are workspace+448 = 513, then workspace+448 = 256 and
 * workspace+456 = 32.  The word at 0x03001ec8 (sl+12) is the same second
 * pointer cell resource_377_c_020003f8.c uses; its u16 at +0x1f84 is set to 1.
 *
 * 0x04000050 / 0x04000052 are the GBA blend registers (BLDCNT / BLDALPHA);
 * the two loops fade by writing 0x100e..0x1011 and then 0x100f down to 0x1000
 * one frame at a time via Func_080000c0(1).  Each loop holds exactly ONE call
 * site, so it is written as a loop here without changing the multiset.
 *
 * Uncertainties recorded rather than resolved: the first argument of
 * Func_0808a188 at 0x02000c3a is the pooled constant 0x8009, unlike every other
 * site of that import in this overlay, which passes a small id; and
 * 'r6[9] = (r6[9] & ~0x0c) | 4' operates on the pointer read from
 * record[80] of the Func_0808a080(10) record, whose layout is not established
 * by any byte-exact sibling in this overlay.
 *
 * All 86 call sites resolved with
 * 'bun tools/overlay_call_targets.ts resource_377 0a0c --json' (the inventory's
 * calls=81 is the documented undercount).  Per-target counts, verified against
 * 'overlay_show.ts | overlay_call_targets.ts --annotate | sort | uniq -c' --
 * per target, not by total:
 *   Func_0808a0f0 x10  Func_08009180 x9   Func_0808a098 x7  Func_0808a010 x6
 *   Func_0808a1b8 x4   Func_080000c0 x4   Func_080f9010 x3  Func_0808a208 x3
 *   Func_0808a110 x3   Func_0808a100 x3   Func_0808a090 x3  Func_0808a218 x2
 *   Func_0808a210 x2   Func_0808a200 x2   Func_0808a1f0 x2  Func_0808a138 x2
 *   Func_0808a0a0 x2   Func_0808a080 x2   Func_0808a370 x1  Func_0808a368 x1
 *   Func_0808a360 x1   Func_0808a2d8 x1   Func_0808a2c8 x1  Func_0808a248 x1
 *   Func_0808a1e8 x1   Func_0808a188 x1   Func_0808a170 x1  Func_0808a150 x1
 *   Func_0808a130 x1   Func_0808a0d0 x1   Func_0808a0c8 x1  Func_0808a0b0 x1
 *   Func_0808a018 x1   Func_080091e0 x1   Func_080091a0 x1        (total 86)
 * The disassembler's own 'bl' annotations are wrong, as on every overlay.
 */

extern u8 *Data_03001ebc;

extern u8 Data_02009b78[];
extern u8 Data_02009bb4[];
extern u8 Data_02009c04[];
extern u8 Data_02009c54[];
extern u8 Data_02009ca4[];
extern u8 Data_02009cec[];
extern u8 Data_02009d38[];





































extern u8 * Func_02002346();
extern void Func_0200232e();
extern void Func_020023a0();
extern void Func_020023aa();
extern void Func_020023b4();
extern void Func_020023be();
extern void Func_020023c8();
extern void Func_020023d2();
extern void Func_020023dc();
extern void Func_020023e8();
extern void Func_020023a6();
extern u8 * Func_02002324();
extern void Func_020023e4();
extern void Func_0200234a();
extern void Func_0200235c();
extern void Func_0200236e();
extern void Func_02002380();
extern void Func_02002392();
extern void Func_020023a2();
extern void Func_020023b6();
extern void Func_020023d8();
extern void Func_020023f4();
extern void Func_020025ac();
extern void Func_020025ca();
extern void Func_02002398();
extern void Func_020025a0();
extern void Func_020024ce();
extern void Func_020024dc();
extern void Func_020024ea();
extern void Func_020024fa();
extern void Func_02002502();
extern void Func_02002626();
extern void Func_02002514();
extern void Func_02002682();
extern void Func_020025e6();
extern void Func_02002586();
extern void Func_020025e2();
extern void Func_0200261e();
extern void Func_02002636();
extern void Func_02002592();
extern void Func_02002594();
extern void Func_02002658();
extern void Func_020025f6();
extern void Func_02002618();
extern void Func_02002664();
extern void Func_0200267e();
extern void Func_0200268a();
extern void Func_02002568();
extern void Func_0200265c();
extern void Func_020025f4();
extern void Func_02002604();
extern void Func_0200260c();
extern void Func_0200259c();
extern void Func_020025dc();
extern void Func_020025e4();
extern void Func_0200275a();
extern void Func_020025b8();
extern void Func_020025f8();
extern void Func_020024e0();
extern void Func_02002782();
extern void Func_020024f0();
extern void Func_0200263c();
extern void Func_0200267c();
extern void Func_0200269c();
extern void Func_020026ac();
extern void Func_0200261a();
extern void Func_0200270e();
extern void Func_0200271a();
extern void Func_02002744();
extern void Func_0200274e();
extern void Func_0200264c();
extern void Func_020026c6();
extern void Func_02002708();
extern void Func_020026e0();
extern void Func_020026f0();
extern void Func_02002676();
extern void Func_02002796();
extern void Func_020026c8();
extern void Func_02002812();
extern void Func_0200281e();
extern void Func_020027ec();
void Func_02000a0c(void)
{
    u8 *record;
    u8 *sub;
    u8 *workspace;
    u32 i;

    record = Func_02002346(10);
    sub = *(u8 **)(record + 80);

    Func_0200232e();
    Func_020023a0(11, 0, 0);
    Func_020023aa(12, 0, 0);
    Func_020023b4(13, 0, 0);
    Func_020023be(14, 0, 0);
    Func_020023c8(15, 0, 0);
    Func_020023d2(16, 0, 0);
    Func_020023dc(8, 0x01AF0000, 0x01870000);
    Func_020023e8(10, 0x01CF0000, 0x01940000);

    Func_020023a6(Func_02002324(10), 0);

    record[85] = 0;
    record[35] &= (u8)0xFE;
    sub[9] = (u8)((sub[9] & (u8)~0x0C) | 4);

    Func_020023e4(10, Data_02009cec);

    *(s32 *)(workspace + 448) = 513;
    workspace = Data_03001ebc;

    /* six-argument Func_08009180: two arguments are passed on the stack */
    Func_0200234a(83, 15, 83, 19, 5, 4);
    Func_0200235c(90, 16, 90, 20, 5, 4);
    Func_0200236e(77, 23, 82, 23, 5, 7);
    Func_02002380(83, 33, 85, 33, 2, 2);
    Func_02002392(91, 28, 90, 28, 1, 1);
    Func_020023a2(91, 28, 88, 30, 1, 1);
    Func_020023b6(94, 27, 94, 23, 6, 4);
    Func_020023c8(92, 28, 87, 23, 4, 4);
    Func_020023d8(65, 53, 88, 24, 2, 2);
    Func_020023f4();

    *(u16 *)0x04000052 = 0x100C;
    *(u16 *)0x04000050 = 0x3F42;

    Func_020025ac();
    *(u16 *)(((u8 **)&Data_03001ebc)[3] + 0x1F84) = 1;
    Func_020025ca();
    Func_02002398(30);

    Func_020025a0(8, 1);
    Func_020024ce(8, 0x18000, 0xC000);
    Func_020024dc(0, 0x18000, 0xC000);
    Func_020024ea(9, 0x18000, 0xC000);
    Func_020024fa(0, Data_02009bb4);
    Func_02002502(8, Data_02009b78);
    Func_02002626();
    Func_02002514(8);
    Func_02002682(158);
    Func_020025e6(8, 256, 0);
    Func_02002586(8, 2);
    Func_020025e2(8, 0x4000, 10);
    Func_0200261e(0x40000, 0x8000);
    Func_02002636(0x019E0000, -1, 0x02120000, 1);
    Func_02002592(9, 0x019E0000, 0x02120000);
    Func_02002594(9, 0x1AB, 0x1E3);
    Func_02002658();
    Func_020025f6(0xE5B);
    Func_02002618(0x8009, 0, 10);
    Func_02002664(0x18000, 0x3000);
    Func_0200267e(0x01E00000, -1, 0x01BC0000, 1);
    Func_0200268a();
    Func_02002568(20);
    Func_0200265c(8, 0x8000, 20);
    Func_020025f4(8, 3);
    Func_02002604(0, 3);
    Func_0200260c(9, 3);
    Func_020025f6(9, 0x19F, 0x24D);
    Func_0200259c(10);
    Func_020025dc(8, Data_02009c04);
    Func_020025e4(0, Data_02009c54);
    Func_0200275a(234);
    Func_020025b8(20);
    Func_020025f8(10, Data_02009d38);

    for (i = 0; i <= 3; i++) {
        *(u16 *)0x04000052 = (u16)(0x100E + i);
        Func_020024e0(1);
    }

    Func_02002782(202);
    Func_020024f0(10);

    for (i = 0; i <= 15; i++) {
        Func_02002502(1);
        *(u16 *)0x04000052 = (u16)(0x100F - i);
    }

    Func_0200263c(0);
    Func_0200267c(8, 1);
    Func_0200269c(8, 2);
    Func_020026ac(0, 2);
    Func_0200261a(10);
    Func_0200270e(8, 0xC000, 0);
    Func_0200271a(0, 0xC000, 20);
    Func_02002744(8, 258);
    Func_0200274e(0, 258);
    Func_0200264c(80);
    Func_020026c6(9, 0, 0);
    Func_02002708(8, 0, 20);
    Func_020026e0(8, 3);
    Func_020026f0(0, 3);
    Func_02002676(40);
    Func_02002796(0xCCCC, 0x1999);
    Func_02002796(8, 1);
    Func_020026c8(8, Data_02009ca4);
    Func_020026e0(0, Data_02009ca4);

    workspace = Data_03001ebc;
    *(s32 *)(workspace + 448) = 256;
    *(s32 *)(workspace + 456) = 32;

    Func_02002812();
    Func_0200281e();
    Func_020027ec(21);
}
