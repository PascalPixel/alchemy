#include "types.h"

/*
 * resource_3c6 cutscene script at 0x020007c4, 2,250 bytes
 * (0x020007c4-0x0200108d).
 *
 * Complete owner: `push {r5, r6, lr}` plus the 8-byte outgoing-argument frame
 * (`sub sp, #8`) at 0x020007c4, through the single interworking epilogue
 * `add sp, #8 / pop {r5, r6} / pop {r0} / bx r0` at 0x02001086.  The epilogue
 * pops the return address into r0, so the owner is void.  Nothing reads
 * r0-r3 before they are first written, so it takes no arguments.
 *
 * Three literal pools sit inside the span.  The pool map was derived by a
 * control-flow walk from the prologue, not from a referenced-words listing;
 * each pool is proved unreachable as code by the branch that steps over it:
 *   0x02000880-0x0200088b  (0x09ba, 0x288e, 0x03001ebc) - the `bne` at
 *                          0x02000850 jumps to 0x0200088c and the `b.n` at
 *                          0x0200087e jumps to 0x020008b8, so nothing falls in;
 *   0x02000c10-0x02000c1b  (0x03001ebc, 0x6666, 0xcccc) - same shape, `bne` at
 *                          0x02000be0 to 0x02000c1c, `b.n` at 0x02000c0e to
 *                          0x02000c48;
 *   0x02000ff0-0x0200100b  (0x03001ebc, 0x09bf, 0x28a5, 0xcccc, 0x6666,
 *                          0x13333, 0x9999) - `beq` at 0x02000fde and `b.n` at
 *                          0x02000fee both to 0x0200100c.
 * No call argument is in flight across any of the three, but the walk is what
 * establishes that.
 *
 * LINK BASE 0x02008000 (witnesses recorded in resource_3c6_c_02000078.c).
 * None of the thirteen pool words falls in 0x0200_8xxx-0x0200_bxxx: they are
 * cue/dialogue ids, 16.16 constants, and three copies of 0x03001ebc, the
 * overlay's IWRAM workspace pointer cell - the same cell the byte-exact
 * resource_3c6_c_020001f0.c reads.
 *
 * Control flow: six conditionals, no loops.  Two of them (0x0200084e and
 * 0x02000bde) are the skip-beat shape: both arms run the same four calls and
 * both bump the u16 skip counter at workspace + 472, differing only in where
 * the bump sits relative to the last call.  They are written as two arms
 * anyway, because each arm's four `bl` sites are distinct sites.
 *
 * Call accounting: 250 `bl` sites, all resolved with
 * `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3c6 07c4` - 32 distinct targets,
 * 249 import veneers plus one intra-overlay prologue call to Func_02001090;
 * no `call_via` site.  The multiset of `Func_...(` calls below equals that
 * multiset per target.  Every site is written exactly once: no site is reached
 * from two control paths, and no `bl` targets this owner's own epilogue.
 * (The inventory's `calls=230` is the usual undercount and is not the check.)
 *
 * UNCERTAINTY 1: the very first argument register is not set for the
 * Func_0808a018 call at 0x020007ce - r0 still holds whatever Func_080770c8
 * left there.  The sibling script Func_02000218 passes an explicit mode to the
 * same import, so this is written argument-less rather than inventing a value.
 * UNCERTAINTY 2: at 0x02000996 r3 is loaded with 2 as the value stored into
 * record + 0x55 and is still live at the `bl` at 0x020009a0.  That is the
 * documented phantom-argument idiom, so Func_0808a1e0 takes two arguments.
 * The same applies to the three Func_0808a0b8 sites, where r3 = 18 is the
 * second `ldrsh` offset.
 * UNCERTAINTY 3: Func_080091c8's fifth and sixth arguments are passed on the
 * stack from r5 and r6, which are set once (4 and 18) and reused by all four
 * sites.  Their meaning is not established here.
 */

/* Imports, reached through this overlay's veneer table.  Old-style
 * declarations: their interfaces vary by call site across the overlay.
 * Func_080770c0 is used in a condition, so it needs a real return type. */
void Func_02001cec();
void Func_02001d00();
void Func_02001d9e();
void Func_02001d52();
void Func_02001dcc();
void Func_02001e12();
void Func_02001e22();
void Func_02001e30();
void Func_02001da6();
void Func_02001d4c();
void Func_02001dfc();
void Func_02001d5a();
void Func_02001e1e();
void Func_02001d6c();
s32 Func_02001d94();
void Func_02001d7e();
void Func_02001dfe();
void Func_02001d8c();
void Func_02001e44();
void Func_02001db8();
void Func_02001e38();
void Func_02001dc6();
void Func_02001e8e();
void Func_02001de4();
void Func_02001eb8();
void Func_02001ea8();
void Func_02001e76();
void Func_02001e0c();
void Func_02001e84();
void Func_02001e8c();
void Func_02001e9c();
void Func_02001e2a();
void Func_02001f36();
void Func_02001f3a();
void Func_02001f46();
void Func_02001f5a();
void Func_02001ed2();
void Func_02001eda();
void Func_02001f2e();
void Func_02001f52();
void Func_02001f5e();
void Func_02001eac();
void Func_02001ee6();
u8 *Func_02001ee4();
void Func_02001f8a();
void Func_02001eb6();
void Func_02001fec();
void Func_02001eea();
void Func_02001f60();
void Func_02001efe();
void Func_02002008();
void Func_02001ef0();
void Func_02002026();
void Func_02001f24();
void Func_02001fdc();
void Func_02001f32();
void Func_02001faa();
void Func_02001fb2();
void Func_02001fba();
void Func_02001fca();
void Func_02001f58();
void Func_02002062();
void Func_02001f74();
void Func_02001f7a();
void Func_02001ffa();
void Func_02001f88();
void Func_02002040();
void Func_02001f96();
void Func_02002016();
void Func_02001fa4();
void Func_0200205c();
void Func_02002042();
void Func_02001fc0();
void Func_02002078();
void Func_02001fce();
void Func_0200205e();
void Func_02002094();
void Func_02001fea();
void Func_020020be();
void Func_020020ae();
void Func_02002004();
void Func_020020d8();
void Func_020020c8();
void Func_0200201e();
void Func_0200209e();
void Func_0200202c();
void Func_020020e4();
void Func_0200203a();
void Func_020020dc();
void Func_020020e6();
void Func_02002054();
void Func_02002118();
void Func_02002124();
void Func_02002130();
void Func_0200213c();
void Func_0200208a();
void Func_02002090();
void Func_02002110();
void Func_02002156();
void Func_020020ac();
void Func_02002180();
void Func_02002170();
void Func_020020c6();
void Func_02002146();
void Func_020020d4();
void Func_0200218c();
void Func_020020e2();
void Func_020021a6();
void Func_020020f4();
void Func_020021a4();
s32 Func_02002124_b();
void Func_0200210e();
void Func_0200218e();
void Func_0200211c();
void Func_020021d4();
void Func_02002148();
void Func_020021c8();
void Func_0200221e();
void Func_02002174();
void Func_02002248();
void Func_02002238();
void Func_02002252();
void Func_0200226e();
void Func_0200225e();
void Func_020021b4();
void Func_0200222a();
void Func_0200227c();
void Func_020021d2();
void Func_020022a6();
void Func_02002268();
void Func_02002272();
void Func_020022aa();
void Func_02002200();
void Func_02002280();
void Func_0200220e();
void Func_020022c6();
s32 Func_0200220c();
void Func_020022ca();
void Func_02002230();
void Func_020022c0();
void Func_0200223e();
void Func_020022f6();
void Func_0200224c();
void Func_020022cc();
void Func_0200225a();
void Func_02002312();
void Func_0200233c();
void Func_0200232c();
void Func_02002282();
void Func_02002290();
void Func_02002348();
void Func_0200229e();
void Func_0200231e();
void Func_020022ac();
void Func_02002364();
void Func_020022ba();
void Func_0200233a();
void Func_020022c8();
void Func_02002302();
void Func_02002334();
void Func_020023e6();
void Func_020022ec();
void Func_0200235e();
void Func_02002326();
void Func_02002314();
u8 *Func_02002342();
void Func_02002430();
u8 *Func_0200235e_b();
void Func_02002404();
void Func_02002352();
void Func_02002358();
void Func_0200242c();
void Func_0200241c();
void Func_02002372();
void Func_020023f2();
void Func_02002380();
void Func_02002438();
void Func_0200238e();
void Func_02002406();
void Func_0200240e();
void Func_02002416();
void Func_02002426();
void Func_020023b4();
void Func_020023ba();
void Func_0200243a();
void Func_020023c8();
void Func_020024d4();
void Func_02002498();
void Func_020023d2();
void Func_02002508();
void Func_02002512();
void Func_02002484();
void Func_02002422();
void Func_0200240a();
void Func_02002540();
void Func_0200243e();
void Func_02002502();
void Func_020024e4();
void Func_020024ee();
void Func_02002464();
void Func_0200251c();
void Func_02002472();
void Func_02002480();
void Func_02002538();
void Func_0200248e();
void Func_0200250e();
void Func_0200249c();
void Func_02002554();
void Func_020024aa();
void Func_02002522();
void Func_020024b8();
void Func_02002530();
void Func_02002548();
void Func_020024d6();
void Func_02002510();
void Func_0200251a();
void Func_02002524();
void Func_0200256c();
u8 *Func_0200252a();
void Func_02002598();
void Func_020025aa();
void Func_020025ba();
u8 *Func_02002578();
void Func_020025a2();
void Func_020025c8();
void Func_020025da();
void Func_020025ea();
u8 *Func_020025a8();
void Func_020025d2();
void Func_020025f8();
void Func_0200260a();
void Func_020025a8_b();
void Func_020025bc();
                                /* six-argument scene presentation request */
                                /* query an audio/sequence cue */
                                /* start an audio/sequence cue */
                                /* audio/sequence cue by id */
                                /* wait n frames */
                                /* begin the scripted sequence */
                                /* end the scripted sequence */
                                /* scene resource request */
                                /* scene-state query */
                                /* scene record by slot selector, or 0 */
                                /* per-slot reorientation request */
                                /* move a slot to (x, z) */
                                /* timed per-slot adjustment */
                                /* timed per-slot adjustment */
                                /* per-slot release */
                                /* place an entity at (x, z) */
                                /* per-record scene request */
                                /* scene-presentation request */
                                /* per-slot pairing request with a duration */
                                /* select an entity presentation */
                                /* per-slot pairing request */
                                /* per-slot pairing request */
                                /* show a dialogue line by id */
                                /* dialogue-line variant with a mode word */
                                /* wait for the slot's action to finish */
                                /* face the slot along a heading */
                                /* per-slot presentation select */
                                /* play animation id for n frames */
                                /* signed per-slot adjustment */
                                /* signed per-slot adjustment */
                                /* signed per-slot adjustment */

/* Intra-overlay call, resolved to the prologue at file offset 0x1090. */

extern u8 *Data_03001ebc;       /* overlay workspace pointer, IWRAM cell */

/* The u16 at workspace + 472 is the skip-beat counter: it is bumped on both
 * arms of every scene-variant test in this overlay. */
#define SKIP_BEATS (*(u16 *)(Data_03001ebc + 472))

void Func_020007c4(void)
{
    u8 *record;
    s16 *entity;

    Func_02001cec(2490);   /* 07ca */
    Func_02001d00();   /* 07ce */
    Func_02001d9e(0x288e);   /* 07d4 */
    Func_02001d52(0, 104, 376);   /* 07e0 */
    Func_02001dcc(0, 0, 0);   /* 07ea */
    Func_02001e12(1, -32, 0, 0);   /* 07f8 */
    Func_02001e22(3, -16, 16, 0xe000);   /* 0808 */
    Func_02001e30(2, 0, 16, 0xc000);   /* 0816 */
    Func_02001da6(1);   /* 081c */
    Func_02001d4c(30);   /* 0822 */
    Func_02001dfc(1, 0);   /* 082a */
    Func_02001d5a(10);   /* 0830 */
    Func_02001e1e(0, 0x8000, 0);   /* 083c */
    Func_02001d6c(10);   /* 0842 */
    if (Func_02001d94(0, 0) == 0) {   /* 084a, tested at 0200084e */
        Func_02001d7e(20);   /* 0854 */
        Func_02001dfe(3, 4);   /* 085c */
        Func_02001d8c(20);   /* 0862 */
        Func_02001e44(3, 0);   /* 086a */
        SKIP_BEATS++;
    } else {
        Func_02001db8(20);   /* 088e */
        Func_02001e38(3, 4);   /* 0896 */
        Func_02001dc6(20);   /* 089c */
        SKIP_BEATS++;
        Func_02001e8e(3, 0);   /* 08b4 */
    }

    Func_02001de4(10);   /* 08ba */
    Func_02001eb8(2, 264, 40);   /* 08c6 */
    Func_02001ea8(2, 0);   /* 08ce */
    Func_02001dfe(10);   /* 08d4 */
    Func_02001e76(0, 3);   /* 08dc */
    Func_02001e0c(40);   /* 08e2 */
    Func_02001e84(1, 3);   /* 08ea */
    Func_02001e8c(2, 3);   /* 08f2 */
    Func_02001e9c(3, 3);   /* 08fa */
    Func_02001e2a(30);   /* 0900 */
    Func_02001f36(0, -16, 0);   /* 090c */
    Func_02001f3a(2, 0, -16);   /* 0918 */
    Func_02001f46(3, 0, -8);   /* 0924 */
    Func_02001f5a(0, 0, -16);   /* 0930 */
    Func_02001ed2(3, 1);   /* 0938 */
    Func_02001eda(2, 1);   /* 0940 */
    Func_02001f2e(0, 0xc000, 0);   /* 094c */
    Func_02001f3a(1, 0xc000, 0);   /* 0958 */
    Func_02001f46(3, 0xc000, 0);   /* 0964 */
    Func_02001f52(2, 0xc000, 0);   /* 0970 */
    Func_02001f5e(22, 0x4000, 0);   /* 097c */
    Func_02001eac(80);   /* 0982 */
    Func_02001ee6(22, 0xcccc, 0x6666);   /* 098c */
    record = Func_02001ee4(22);
    record[0x55] = 2;   /* 0992 */
    Func_02001f8a(22, 2);   /* 09a0 */
    Func_02001eb6(34, 0, 1, 2, 4, 18);   /* 09b4 */
    Func_02001fec(158);   /* 09ba */
    Func_02001eea(20);   /* 09c0 */
    Func_02001f60(22, 0x480000, 0x1380000);   /* 09ce */
    Func_02001efe(20);   /* 09d4 */
    Func_02002008(22, 0, 16);   /* 09de */
    Func_02001ef0(32, 0, 1, 2, 4, 18);   /* 09ee */
    Func_02002026(159);   /* 09f4 */
    Func_02001f24(20);   /* 09fa */
    Func_02001fdc(22, 0);   /* 0a02 */
    Func_02001f32(10);   /* 0a08 */
    Func_02001faa(0, 3);   /* 0a10 */
    Func_02001fb2(1, 3);   /* 0a18 */
    Func_02001fba(2, 3);   /* 0a20 */
    Func_02001fca(3, 3);   /* 0a28 */
    Func_02001f58(40);   /* 0a2e */
    Func_02002062(22, 16, 0);   /* 0a38 */
    Func_02002026(22, 0x4000, 0);   /* 0a44 */
    Func_02001f74(20);   /* 0a4a */
    Func_02001f7a(10);   /* 0a50 */
    Func_02001ffa(22, 3);   /* 0a58 */
    Func_02001f88(30);   /* 0a5e */
    Func_02002040(22, 0);   /* 0a66 */
    Func_02001f96(10);   /* 0a6c */
    Func_02002016(3, 3);   /* 0a74 */
    Func_02001fa4(30);   /* 0a7a */
    Func_0200205c(3, 0);   /* 0a82 */
    Func_02001fb2(10);   /* 0a88 */
    Func_02002042(22, 2);   /* 0a90 */
    Func_02001fc0(20);   /* 0a96 */
    Func_02002078(22, 0);   /* 0a9e */
    Func_02001fce(10);   /* 0aa4 */
    Func_0200205e(1, 2);   /* 0aac */
    Func_02001fdc(20);   /* 0ab2 */
    Func_02002094(1, 0);   /* 0aba */
    Func_02001fea(10);   /* 0ac0 */
    Func_020020be(22, 258, 40);   /* 0acc */
    Func_020020ae(22, 0);   /* 0ad4 */
    Func_02002004(10);   /* 0ada */
    Func_020020d8(2, 256, 40);   /* 0ae6 */
    Func_020020c8(2, 0);   /* 0aee */
    Func_0200201e(10);   /* 0af4 */
    Func_0200209e(22, 3);   /* 0afc */
    Func_0200202c(30);   /* 0b02 */
    Func_020020e4(22, 0);   /* 0b0a */
    Func_0200203a(10);   /* 0b10 */
    Func_020020dc(1, 0, 0);   /* 0b1a */
    Func_020020e6(3, 2, 0);   /* 0b24 */
    Func_02002054(60);   /* 0b2a */
    Func_02002118(0, 0xc000, 0);   /* 0b36 */
    Func_02002124(1, 0xc000, 0);   /* 0b42 */
    Func_02002130(2, 0xc000, 0);   /* 0b4e */
    Func_0200213c(3, 0xc000, 0);   /* 0b5a */
    Func_0200208a(40);   /* 0b60 */
    Func_02002090(10);   /* 0b66 */
    Func_02002110(22, 3);   /* 0b6e */
    Func_0200209e(30);   /* 0b74 */
    Func_02002156(22, 0);   /* 0b7c */
    Func_020020ac(10);   /* 0b82 */
    Func_02002180(3, 258, 40);   /* 0b8e */
    Func_02002170(3, 0);   /* 0b96 */
    Func_020020c6(10);   /* 0b9c */
    Func_02002146(22, 3);   /* 0ba4 */
    Func_020020d4(20);   /* 0baa */
    Func_0200218c(22, 0);   /* 0bb2 */
    Func_020020e2(10);   /* 0bb8 */
    Func_020021a6(1, 0xe000, 0);   /* 0bc4 */
    Func_020020f4(20);   /* 0bca */
    Func_020021a4(1, 0);   /* 0bd2 */
    if (Func_02002124_b(0, 0) == 0) {   /* 0bda, tested at 02000bde */
        Func_0200210e(20);   /* 0be4 */
        Func_0200218e(22, 4);   /* 0bec */
        Func_0200211c(20);   /* 0bf2 */
        Func_020021d4(22, 0);   /* 0bfa */
        SKIP_BEATS++;
    } else {
        Func_02002148(20);   /* 0c1e */
        Func_020021c8(22, 4);   /* 0c26 */
        Func_02002156(20);   /* 0c2c */
        SKIP_BEATS++;
        Func_0200221e(22, 0);   /* 0c44 */
    }

    Func_02002174(10);   /* 0c4a */
    Func_02002248(2, 258, 40);   /* 0c56 */
    Func_02002238(2, 0);   /* 0c5e */
    Func_0200218e(10);   /* 0c64 */
    Func_02002252(1, 0xc000, 0);   /* 0c70 */
    Func_0200226e(22, 262, 50);   /* 0c7c */
    Func_0200225e(22, 0);   /* 0c84 */
    Func_020021b4(20);   /* 0c8a */
    Func_0200222a(25, 0x580000, 0x14c0000);   /* 0c98 */
    Func_0200227c(-1, 0);   /* 0ca2 */
    Func_020021d2(10);   /* 0ca8 */
    Func_020022a6(3, 256, 40);   /* 0cb4 */
    Func_02002268(3, 4, 13);   /* 0cbe */
    Func_02002272(3, 4, 30);   /* 0cc8 */
    Func_020022aa(3, 0);   /* 0cd0 */
    Func_02002200(10);   /* 0cd6 */
    Func_02002280(22, 3);   /* 0cde */
    Func_0200220e(30);   /* 0ce4 */
    Func_020022c6(22, 0);   /* 0cec */
    if (Func_0200220c(0x9bf) != 0) {   /* 0cf2, tested at 02000cf6 */
        Func_02001d8c();   /* 0cfa */
    }

    Func_020022ca(0x28a5);   /* 0d00 */
    Func_02002230(10);   /* 0d06 */
    Func_020022c0(22, 2);   /* 0d0e */
    Func_0200223e(20);   /* 0d14 */
    Func_020022f6(22, 0);   /* 0d1c */
    Func_0200224c(10);   /* 0d22 */
    Func_020022cc(22, 3);   /* 0d2a */
    Func_0200225a(30);   /* 0d30 */
    Func_02002312(22, 0);   /* 0d38 */
    Func_02002268(10);   /* 0d3e */
    Func_0200233c(1, 256, 40);   /* 0d4a */
    Func_0200232c(1, 0);   /* 0d52 */
    Func_02002282(10);   /* 0d58 */
    Func_02002312(22, 2);   /* 0d60 */
    Func_02002290(20);   /* 0d66 */
    Func_02002348(22, 0);   /* 0d6e */
    Func_0200229e(20);   /* 0d74 */
    Func_0200231e(2, 3);   /* 0d7c */
    Func_020022ac(30);   /* 0d82 */
    Func_02002364(2, 0);   /* 0d8a */
    Func_020022ba(10);   /* 0d90 */
    Func_0200233a(22, 3);   /* 0d98 */
    Func_020022c8(30);   /* 0d9e */
    Func_02002302(25, 0xcccc, 0x6666);   /* 0da8 */
    Func_02002334(25, 0, 16);   /* 0db2 */
    Func_020023e6(22, 0, 16);   /* 0dbc */
    Func_020022ec(30);   /* 0dc2 */
    Func_0200235e(25, 0, 0);   /* 0dcc */
    SKIP_BEATS++;
    Func_02002326(242, 0);   /* 0de4 */
    Func_02002314(10);   /* 0dea */
    record = Func_02002342(22);
    record[0x5a] &= (u8)~1;   /* 0df0 */
    Func_02002430(22, 0, -16);   /* 0e06 */
    record = Func_0200235e_b(22);
    record[0x5a] |= 1;   /* 0e0c */
    Func_02002404(22, 0x4100, 0);   /* 0e22 */
    Func_02002352(30);   /* 0e28 */
    Func_02002358(10);   /* 0e2e */
    Func_0200242c(22, 256, 40);   /* 0e3a */
    Func_0200241c(22, 0);   /* 0e42 */
    Func_02002372(10);   /* 0e48 */
    Func_020023f2(22, 3);   /* 0e50 */
    Func_02002380(30);   /* 0e56 */
    Func_02002438(22, 0);   /* 0e5e */
    Func_0200238e(20);   /* 0e64 */
    Func_02002406(0, 3);   /* 0e6c */
    Func_0200240e(1, 3);   /* 0e74 */
    Func_02002416(2, 3);   /* 0e7c */
    Func_02002426(3, 3);   /* 0e84 */
    Func_020023b4(50);   /* 0e8a */
    Func_020023ba(10);   /* 0e90 */
    Func_0200243a(22, 3);   /* 0e98 */
    Func_020023c8(30);   /* 0e9e */
    Func_020024d4(22, -16, 0);   /* 0eaa */
    Func_02002498(22, 0xc000, 0);   /* 0eb6 */
    Func_020023e6(20);   /* 0ebc */
    Func_020023d2(34, 0, 1, 2, 4, 18);   /* 0ed0 */
    Func_02002508(158);   /* 0ed6 */
    Func_02002406(10);   /* 0edc */
    Func_02002512(22, 0, -16);   /* 0ee8 */
    Func_02002484(22, 0, 0);   /* 0ef2 */
    Func_02002422(10);   /* 0ef8 */
    Func_0200240a(32, 0, 1, 2, 4, 18);   /* 0f08 */
    Func_02002540(159);   /* 0f0e */
    Func_0200243e(50);   /* 0f14 */
    Func_02002502(0, 0x4000, 0);   /* 0f20 */
    Func_020024e4(1, 0, 0);   /* 0f2a */
    Func_020024ee(2, 0, 0);   /* 0f34 */
    Func_02002464(20);   /* 0f3a */
    Func_0200251c(1, 0);   /* 0f42 */
    Func_02002472(10);   /* 0f48 */
    Func_02002502(3, 2);   /* 0f50 */
    Func_02002480(20);   /* 0f56 */
    Func_02002538(3, 0);   /* 0f5e */
    Func_0200248e(10);   /* 0f64 */
    Func_0200250e(2, 3);   /* 0f6c */
    Func_0200249c(30);   /* 0f72 */
    Func_02002554(2, 0);   /* 0f7a */
    Func_020024aa(10);   /* 0f80 */
    Func_02002522(0, 3);   /* 0f88 */
    Func_020024b8(40);   /* 0f8e */
    Func_02002530(1, 3);   /* 0f96 */
    Func_02002538(2, 3);   /* 0f9e */
    Func_02002548(3, 3);   /* 0fa6 */
    Func_020024d6(30);   /* 0fac */
    Func_02002510(1, 0x13333, 0x9999);   /* 0fb6 */
    Func_0200251a(3, 0x13333, 0x9999);   /* 0fc0 */
    Func_02002524(2, 0x13333, 0x9999);   /* 0fca */
    Func_0200256c(1, 2);   /* 0fd2 */

    entity = (s16 *)Func_0200252a(0);   /* 0fd8 */
    if (entity != 0) {
        /* entity[5] and entity[9] are the +0x0a and +0x12 halfwords, the
         * integer parts of the 16.16 words at +0x08 and +0x10. */
        Func_02002554(1, entity[5], entity[9]);   /* 0fea */
    }

    Func_02002598(1);   /* 100e */
    Func_020025aa(1, 0, 0);   /* 1018 */
    Func_020025ba(3, 2);   /* 1020 */
    entity = (s16 *)Func_02002578(0);   /* 1026 */
    if (entity != 0) {
        Func_020025a2(3, entity[5], entity[9]);   /* 1038 */
    }

    Func_020025c8(3);   /* 103e */
    Func_020025da(3, 0, 0);   /* 1048 */
    Func_020025ea(2, 2);   /* 1050 */
    entity = (s16 *)Func_020025a8(0);   /* 1056 */
    if (entity != 0) {
        Func_020025d2(2, entity[5], entity[9]);   /* 1068 */
    }

    Func_020025f8(2);   /* 106e */
    Func_0200260a(2, 0, 0);   /* 1078 */
    Func_020025a8_b(10);   /* 107e */
    Func_020025bc();   /* 1082 */
}
