#include "types.h"

/*
 * resource_3a2 owner at 0x02000924, 412 bytes.  Complete owner:
 * `push {r5, lr}` prologue at 0x02000924 and the interworking return
 * `pop {r5} / pop {r0} / bx r0` at 0x02000a9e.  r0 holds the popped return
 * address, so the owner returns nothing; it takes no arguments.
 *
 * Literal pool at 0x02000aa4-0x02000abf, after the epilogue and inside the
 * row's 412-byte span; a control-flow walk from the prologue never reaches it:
 *   0x02000aa4 = 0x0000089a  progress flag id
 *   0x02000aa8 = 0x000018b5  text/cue id
 *   0x02000aac = 0x02008055  ODD -> Thumb entry, file offset 0x0054
 *   0x02000ab0 = 0x0200962c  EVEN -> in-image data, file offset 0x162c
 *   0x02000ab4 = 0x020096b8  EVEN -> in-image data, file offset 0x16b8
 *   0x02000ab8 = 0x00000105
 *   0x02000abc = 0x00000101
 *
 * LINK BASE: 0x02008055 - 0x8000 - 1 = 0x0054 = Func_02000054, a function this
 * overlay already holds byte-exact in assets/code/resource_3a2_c_02000054.c.
 * That is a second independent confirmation of the 0x02008000 base for
 * resource_3a2 (Func_020010b8 gives the other, 0x02009061 = Func_02001060 + 1),
 * and it identifies the word as an installed per-entity callback: it is stored
 * into the record at +0x6c and cleared back to 0 at the end of the beat.
 * The two even words are data blocks, spelled with their raw pool values as
 * symbol names per the project convention.
 *
 * All 41 call sites resolved with `bun tools/overlay_call_targets.ts
 * resource_3a2 0924 --json` (the inventory's `calls=39` is a known lower
 * bound).  Veneer map used below:
 *   0x14d0 -> Func_080770c0   0x1538 -> Func_0808a0d0   0x15a8 -> Func_0808a1b8
 *   0x14d8 -> Func_080770c8   0x1550 -> Func_0808a0f0   0x15b0 -> Func_0808a1e0
 *   0x14e8 -> Func_0808a010   0x1560 -> Func_0808a110   0x15b8 -> Func_0808a1e8
 *   0x14f0 -> Func_0808a018   0x1570 -> Func_0808a130   0x15c0 -> Func_0808a1f0
 *   0x14f8 -> Func_0808a020   0x1578 -> Func_0808a138   0x15c8 -> Func_0808a210
 *   0x1508 -> Func_0808a080   0x1580 -> Func_0808a170   0x15d0 -> Func_0808a218
 *   0x1510 -> Func_0808a090   0x1598 -> Func_0808a188
 *   0x1518 -> Func_0808a098
 *   0x1520 -> Func_0808a0a0
 *
 * Placed multiset (41 = 41): Func_0808a010 x6, Func_0808a188 x4,
 * Func_0808a080 x4, Func_0808a0d0 x2, Func_0808a090 x2, Func_0808a098 x2,
 * Func_0808a0a0 x2, Func_0808a138 x2, Func_0808a1b8 x2, Func_0808a1e8 x2, and
 * one each of Func_080770c0, Func_0808a018, Func_0808a0f0, Func_0808a170,
 * Func_0808a1e0, Func_0808a130, Func_0808a1f0, Func_0808a210, Func_080770c8,
 * Func_0808a218, Func_0808a110, Func_0808a020.
 *
 * Behaviour: a gated cutscene.  Func_080770c0(0x89a) queries progress flag
 * 0x89a; if it is zero the owner returns immediately (`cmp r0,#0 / bne` over a
 * `b.n` straight to the epilogue).  Otherwise the beat runs: place actor 10,
 * post cue 0x18b5, install the Func_02000054 callback on the player record,
 * run the walk/turn/pose script, and finally clear the callback again.
 *
 * The one conditional inside the beat reads the player record's 16.16 word at
 * +16 and shifts it arithmetically right by 20 -- the integer part divided by
 * 16, i.e. the tile index -- and only calls Func_0808a0d0(0, 440, 200) when
 * that tile is 13.  The shift is `asrs`, so it is SIGNED.
 *
 * Every heading/coordinate constant is built by shift: 134<<18 = 0x2180000,
 * 216<<16 = 0xd80000, 128<<10 = 0x20000, 128<<9 = 0x10000, 128<<8 = 0x8000,
 * 160<<7 = 0x5000, 148<<17 = 0x1280000, 172<<17 = 0x1580000, 139<<4 = 0x8b0,
 * 220<<1 = 440, 204<<1 = 408, 129<<1 = 258.
 *
 * Uncertainties: Func_0808a218 at 0x02000a1e is reached with no register set
 * since the preceding call, so it is spelled with no arguments.  The record
 * flag byte at +35 has bit 0 SET here, where Func_020010b8 clears bit 1 of the
 * same byte -- two different bits of one field.  Old-style declarations keep
 * every import's interface open, as required for overlay sources.
 */

      /* installed callback, byte-exact in assets/code */
extern u8 Data_0200962c[];
extern u8 Data_020096b8[];
























extern s32 Func_02001dfa();
extern void Func_02001e24();
extern void Func_02001e92();
extern void Func_02001ec8();
extern void Func_02001eea();
extern void Func_02001ed2();
extern void Func_02001e48();
extern u8 * Func_02001e6e();
extern u8 * Func_02001e78();
extern void Func_02001ebc();
extern void Func_02001ea2();
extern void Func_02001f4a();
extern void Func_02001ede();
extern u8 * Func_02001eb4();
extern void Func_02001ea4();
extern void Func_02001f70();
extern void Func_02001f6a();
extern void Func_02001fa4();
extern void Func_02001f8c();
extern void Func_02001f14();
extern void Func_02001fd6();
extern void Func_02001eee();
extern void Func_02001f3c();
extern void Func_02001ff0();
extern void Func_02001f3e();
extern void Func_02001f4e();
extern void Func_02001f5c();
extern void Func_02001f2a();
extern u8 * Func_02001f50();
extern void Func_02001f3a();
extern void Func_02001fd2();
extern void Func_02001f48();
extern void Func_02002014();
extern void Func_0200202e();
extern void Func_02002038();
extern void Func_02001fe8();
extern void Func_02001f76();
extern void Func_02002030();
extern void Func_02001f94();
void Func_02000924(void)
{
    u8 *record;

    if (Func_02001dfa(0x89a) == 0) return;

    Func_02001e24();
    Func_02001e92(10, 0x2180000, 0xd80000);
    Func_02001ec8(0x18b5);
    Func_02001eea(10, 0, 20);
    Func_02001ed2(0, 2);
    Func_02001e48(20);

    *(void **)(Func_02001e6e(0) + 108) = (void *)0x02008055;

    record = Func_02001e78(0);
    if ((*(s32 *)(record + 16) >> 20) == 13) {
        Func_02001ebc(0, 440, 200);
    }

    Func_02001ea2(10, 0x20000, 0x10000);
    Func_02001f4a(10, 2);
    Func_02001ede(10, 408, 216);

    Func_02001eb4(10)[35] |= 1;

    Func_02001ea4(10);
    Func_02001f70(10, 0x8000, 20);
    Func_02001f6a(10, 0, 20);
    Func_02001f4a(10, 2);
    Func_02001fa4(10, 258);
    Func_02001ed2(60);
    Func_02001f8c(10, 0, 20);
    Func_02001f14(10, Data_0200962c);

    Func_02001fd6(0x1280000, -1, 0x1580000, 1);
    Func_02001eee(0x8b0);
    Func_02001f3c(10);
    Func_02001ff0();

    Func_02001f3e(0, 0x10000, 0x8000);
    Func_02001f4e(0, Data_020096b8);
    Func_02001f5c(0);
    Func_02001f2a(10);

    *(void **)(Func_02001f50(0) + 108) = 0;

    Func_02001f3a(30);
    Func_02001fd2(10, 2);
    Func_02001f48(20);
    Func_02002014(10, 0x5000, 120);
    Func_0200202e(10, 0x105, 60);
    Func_02002038(0, 0x101, 60);
    Func_02001fe8(10, 4);
    Func_02001f76(20);
    Func_02002030(10, 0, 20);
    Func_02001f94();
}
