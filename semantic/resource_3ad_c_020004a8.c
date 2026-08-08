#include "types.h"

/*
 * Resource 3ad, cutscene at 0x020004a8 (896 bytes, 95 call sites).
 *
 * Complete owner: `push {r5, lr}` at 0x020004a8 and `pop {r5} / pop {r0} /
 * bx r0` at 0x02000800.  The popped branch register is r0, so it holds the
 * return address and the owner is `void`.  The entry guard reaches that
 * epilogue through a long `b.n 0x2000800` (the `bne` over it exists only
 * because the short branch could not span the body), so it is an early return,
 * not a call.
 *
 * An eight-word literal pool occupies 0x02000808-0x02000827, past the return:
 *   0x941, 0x94d (story flags), 0x00014ccc / 0x0000a666 (a coordinate pair),
 *   0x250d, 0x2512 (dialogue line bases), 0x00006666 / 0x00003333 (a second
 *   coordinate pair).
 * Two of those are the documented trap: 0x00014ccc disassembles as a plausible
 * `ldr r4,[pc,#816]` and 0x0000a666 as `add r6,pc,#408`.  They are data - the
 * control flow never reaches 0x02000810, and both words are loaded by the four
 * `Func_0808a090` call sites in the body.
 *
 * r5 is the dialogue line cursor and is reloaded twice: 0x250d for the first
 * conversation, 0x2512 for the second, then `adds r5, #3` to 0x2515.
 *
 * Call targets resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --`; 94 sites are
 * import veneers and one is this overlay's own prologue at 0x02000828.  The
 * per-target multiset is reproduced exactly by the C, target for target:
 *   Func_0808a1b8 x10, Func_0808a170 x9, Func_0808a010 x8, Func_0808a180 x8,
 *   Func_0808a0e8 x7,  Func_0808a090 x6, Func_0808a0c8 x6, Func_0808a0f0 x5,
 *   Func_0808a080 x4,  Func_0808a130 x4, Func_0808a148 x4, Func_0808a1e8 x4,
 *   Func_0808a100 x3,  Func_0808a110 x2, Func_0808a138 x2, Func_0808a578 x2,
 *   and one each of Func_080770c0, Func_080770c8, Func_0808a018,
 *   Func_0808a020, Func_0808a070, Func_0808a178, Func_0808a208,
 *   Func_0808a210, Func_0808a368, Func_0808a370, Func_02000828.
 * (The inventory row says calls=86, a known lower bound.)
 *
 * The four `Func_0808a080(0)` blocks read the actor record's 16.16 words at +8
 * and +16 directly (`ldr`), not the s16 views at +10/+18 that 0x02001760 and
 * 0x02001448 use.
 *
 * Shift-built constants, all spelled as their values below: 0x90 << 16,
 * 0xc8 << 17, 0xc0 << 9, 0xc0 << 8, 0xc8 << 1, 0xc0 << 6, 0x80 << 8, 0x80 << 5,
 * 0xc0 << 16, 0xd8 << 17, 0xe8 << 1, 0xf4 << 1, 0x81 << 1, 0x80 << 1,
 * 0x80 << 7, 0xd8 << 1.  -1, -16 and -13 are `movs`/`negs` pairs.
 */

/* Old-style declarations: overlay import arities vary per call site. */
s32 Func_02001fb2();
void Func_02001fc6();
void Func_02001fe2();
void Func_02002038();
void Func_0200201e();
void Func_0200203a();
void Func_02002050();
void Func_02002068();
void Func_020020bc();
void Func_02002094();
void Func_0200202a();
void Func_020020fe();
void Func_02002118();
u8 *Func_0200206e();
void Func_020020ac();
void Func_0200208e();
void Func_020020aa();
void Func_020020c0();
void Func_02002124();
u8 *Func_020020aa_b();
void Func_020020e8();
void Func_020020ca();
void Func_020020e6();
void Func_020020fc();
void Func_02002160();
u8 *Func_020020e6_b();
void Func_02002106();
void Func_02002122();
void Func_02002138();
void Func_0200219c();
u8 *Func_02002122_b();
void Func_02002142();
void Func_0200215e();
void Func_02002174();
void Func_020021d8();
void Func_0200213e();
void Func_020021b6();
void Func_020021d6();
void Func_020021ee();
void Func_0200215c();
void Func_020021c4();
void Func_0200216a();
void Func_020021f8();
void Func_02002210();
void Func_02002234();
void Func_02002206();
void Func_02002226();
void Func_02002236();
void Func_02002228();
void Func_02002232();
void Func_020021c0();
s32 Func_020021e0();
void Func_0200225a();
void Func_02002272();
void Func_0200226a();
void Func_02002282();
void Func_020022a6();
void Func_02002286();
void Func_0200229e();
void Func_02002276();
void Func_0200227e();
void Func_0200228e();
void Func_020022d2();
void Func_020022de();
void Func_020022ea();
void Func_020022f6();
void Func_020022b6();
void Func_02002372();
void Func_020022b8();
void Func_020022d0();
void Func_02002306();
void Func_0200231e();
void Func_02002342();
void Func_02002320();
void Func_02002338();
void Func_02002308();
void Func_020022ae();
void Func_020022e0();
void Func_020023cc();
void Func_02002312();
void Func_02002376();
void Func_02002394();
void Func_0200238a();
void Func_02002336();
void Func_02002304();
void Func_02002400();
void Func_0200240c();
void Func_02002326();
void Func_02001026();
                        /* test a story flag (used in a condition) */
                        /* set a story flag */
                        /* wait n frames */

                        /* dialogue prompt; result selects the branch */
                        /* scene entity record by selector */

                     

                        /* scene-presentation request */

                        /* scene request */
                        /* show a dialogue line by id */
                        /* dialogue-line variant with a mode word */
                        /* wait for the slot's action to finish */
                        /* face the slot along a heading */
                        /* play animation id for n frames */

                     

                        /* this overlay */

void Func_020004a8(void)
{
    u8 *actor;
    int line;

    if (Func_02001fb2(0x941) == 0) {
        return;
    }
    Func_02001fc6(0x94d);

    Func_02001fe2();

    Func_02002038(12, 0x900000, 0x1900000);
    Func_0200201e(12, 0x18000, 0xc000);
    Func_0200203a(12, 184, 400);
    Func_02002050(12);
    Func_02002068(12, 1);
    Func_020020bc(12, 0x3000, 0);

    Func_02002094(0, 1);
    Func_0200202a(30);

    Func_020020fe(0x8000, 0x1000);
    Func_02002118(0xc00000, -1, 0x1b00000, 1);

    /*
     * Four near-identical blocks: place slot N at actor 0's 16.16 position,
     * scale it, position it on screen, commit and face it.
     */
    actor = Func_0200206e(0);
    if (actor != 0) {
        Func_020020ac(13, *(s32 *)(actor + 8), *(s32 *)(actor + 16));
    }
    Func_0200208e(13, 0x14ccc, 0xa666);
    Func_020020aa(13, 168, 464);
    Func_020020c0(13);
    Func_02002124(13, 0xc000, 0);

    actor = Func_020020aa_b(0);
    if (actor != 0) {
        Func_020020e8(2, *(s32 *)(actor + 8), *(s32 *)(actor + 16));
    }
    Func_020020ca(2, 0x14ccc, 0xa666);
    Func_020020e6(2, 152, 488);
    Func_020020fc(2);
    Func_02002160(2, 0xc000, 0);

    actor = Func_020020e6_b(0);
    if (actor != 0) {
        Func_02002124(3, *(s32 *)(actor + 8), *(s32 *)(actor + 16));
    }
    Func_02002106(3, 0x14ccc, 0xa666);
    Func_02002122(3, 168, 488);
    Func_02002138(3);
    Func_0200219c(3, 0xc000, 0);

    actor = Func_02002122_b(0);
    if (actor != 0) {
        Func_02002160(1, *(s32 *)(actor + 8), *(s32 *)(actor + 16));
    }
    Func_02002142(1, 0x14ccc, 0xa666);
    Func_0200215e(1, 184, 488);
    Func_02002174(1);
    Func_020021d8(1, 0xc000, 0);

    Func_0200213e(30);
    Func_020021b6(1, 1);

    line = 0x250d;
    Func_020021d6(line);
    Func_020021ee(1, 0);
    Func_0200215c(30);

    Func_020021c4(3, 3);
    Func_0200216a(10);
    Func_020021f8(line + 1);
    Func_02002210(3, 0);

    Func_02002234(2, 0x102, 70);
    Func_02002206(2, 0, 0);
    Func_02002210(0, 2, 0);
    Func_02002226(line + 2);
    Func_02002236(2, 0);

    Func_02002228(3, 0, 0);
    Func_02002232(1, 0, 0);
    Func_020021c0(30);

    /* Two arms, each with its own line id and its own pair of call sites. */
    if (Func_020021e0(0, 0) == 0) {
        Func_0200225a(line + 3);
        Func_02002272(1, 0);
    } else {
        Func_0200226a(line + 4);
        Func_02002282(1, 0);
    }

    Func_020022a6(13, 0x100, 70);

    line = 0x2512;
    Func_02002286(line);
    Func_0200229e(13, 0);

    Func_02002276(0, 2);
    Func_0200227e(1, 2);
    Func_02002286(2, 2);
    Func_0200228e(3, 2);

    Func_020022d2(0, 0xc000, 0);
    Func_020022de(1, 0xc000, 0);
    Func_020022ea(2, 0xc000, 0);
    Func_020022f6(3, 0xc000, 0);

    Func_020022b6(13, 2);
    Func_02002372(13, 0, -16);
    Func_020022b8(13);
    Func_020022d0(13, 1);
    Func_02002306(line + 1);
    Func_0200231e(13, 0);

    Func_02002342(12, 0x100, 65);
    Func_02002320(line + 2);
    Func_02002338(12, 0);

    Func_02002308(13, 3);
    Func_020022ae(80);

    Func_020022e0(12, 0x6666, 0x3333);
    Func_020023cc(12, -13, 0);
    Func_02002312(12);
    Func_02002376(12, 0x4000, 0);

    Func_02002394(12, 0x102, 70);
    Func_02002372(line);
    line += 3;
    Func_0200238a(12, 0);
    Func_02002336(12, 168, 432);
    Func_02002304(40);

    Func_02002400();
    Func_0200240c();
    Func_02002312(20);
    Func_02002326();

    Func_02001026();
}
