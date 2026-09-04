#include "types.h"

#define SetActorPose Func_020012de
#define SetActorFourteenPose Func_02000030
#define FieldScene_RunPrimaryScript Func_02000508

extern s16 Data_02000240[];
extern u8 Value_000000b3;
extern u8 Data_02009690[];
extern u8 Data_020096b0[];
extern u8 Value_000000b4;
extern u8 Data_02009734[];
extern u8 Data_02009974[];
extern u8 Data_0200989c[];
extern u8 *Data_03001ebc;
extern u8 Data_0200a010[];
extern u8 Data_02009eb4[];
extern u8 Data_02009ca4[];
extern u8 Data_02009a94[];
extern u8 Value_00002880;

void Func_020012de(s32 actor, s32 pose);
s32 Func_02001278(s32);
void Func_020015de(s32);
void Func_020015fa(void);
void Func_020016d6(void);
void Func_020015fc(s32);
void Func_020016c6(s32, s32, s32, s32);
void Func_020016d2(void);
void Func_02001618(s32);
void Func_020016a6(s32);
s32 Func_020016be(s32, s32);
void Func_0200162c(s32);
s32 Func_020016ae(s32, s32, s32);
void Func_0200163c(s32);
void Func_0200164a(s32);
void Func_0200165e(void);
void Func_02001652(s32);
void Func_020016f0(s32);
void Func_0200175a(s32);
void Func_020017b0(s32);
s32 Func_020016b6();
void Func_02001590();
void Func_020016fe();
void Func_0200174a();
void Func_02001764();
void Func_02001774();
void Func_02001780();
void Func_020017ac();
void Func_020017ae();
void Func_020017b2();
void Func_020017c0();
void Func_020017c4();
void Func_020017ca();
void Func_020017ce();
void Func_020017d6();
void Func_020017ec();
void Func_020017ee();
void Func_02001804();
void Func_02001810();
void Func_02001812();
void Func_0200181c();
void Func_02001820();
void Func_0200182a();
void Func_0200182e();
void Func_0200183c();
void Func_02001854();
void Func_02001856();
void Func_02001860();
void Func_02001864();
void Func_0200186c();
void Func_02001872();
s32 Func_02001878(s32);
s32 Func_0200187a();
void Func_02001886();
void Func_02001888();
void Func_02001896();
void Func_02001898();
void Func_0200189e();
void Func_020018a0();
void Func_020018ae();
void Func_020018b2();
void Func_020018bc();
void Func_020018be();
void Func_020018ce();
void Func_020018d6();
void Func_020018e8();
void Func_020018f0();
void Func_020018f6();
void Func_02001904();
void Func_02001912();
void Func_02001918();
void Func_02001920();
void Func_02001922();
void Func_0200192a();
void Func_0200192e();
void Func_0200193e();
void Func_0200194e();
void Func_0200195c();
void Func_0200195e();
void Func_02001960();
void Func_0200196a();
void Func_02001978();
void Func_0200197e();
void Func_02001982();
void Func_02001988();
void Func_0200198a();
void Func_0200198c();
void Func_0200199a();
void Func_020019a4();
void Func_020019a8();
void Func_020019b6();
void Func_020019c0();
void Func_020019c4();
void Func_020019c6();
void Func_020019d0();
void Func_020019d2();
void Func_020019e0();
void Func_020019f0();
void Func_020019f6();
void Func_020019fc();
void Func_02001a02();
void Func_02001a18();
void Func_02001a1e();
s32 Func_02001a26(s32, s32);
void Func_02001a2c();
void Func_02001a42();
void Func_02001a48();
void Func_02001a54();
void Func_02001a56();
void Func_02001a64();
void Func_02001a72();
void Func_02001a74();
void Func_02001a7e();
void Func_02001a94();
void Func_02001a9c();
void Func_02001ab0();
void Func_02001abe();
void Func_02001ac4();
void Func_02001ad0();
void Func_02001ad2();
void Func_02001adc();
void Func_02001af0();
void Func_02001af6();
void Func_02001afe();
void Func_02001b0c();
void Func_02001b26();
void Func_02001b2c();
void Func_02001b40();
void Func_02001b46();
void Func_02001b4c();
void Func_02001b58();
void Func_02001b68();
void Func_02001b7c();
void Func_02001b82();
void Func_02001b90();
void Func_02001b96();
void Func_02001b9e();
void Func_02001ba0();
void Func_02001bac();
void Func_02001bba();
void Func_02001bd4();
void Func_02001bd8();
void Func_02001be0();
void Func_02001be4();
void Func_02001bea();
void Func_02001bf2();
void Func_02001bfe();
void Func_02001c00();
void Func_02001c0a();
void Func_02001c12();
void Func_02001c16();
void Func_02001c20();
void Func_02001c2e();
void Func_02001c30();
void Func_02001c3c();
void Func_02001c4a();
void Func_02001c4c();
void Func_02001c58();
void Func_02001c66();
void Func_02001c6a();
void Func_02001c6e();
void Func_02001c78();
void Func_02001c7e();
void Func_02001c84();
void Func_02001c88();
void Func_02001c92();
void Func_02001cac();
void Func_02001cb2();
void Func_02001cb4();
void Func_02001cba();
void Func_02001cce();
void Func_02001cd4();
void Func_02001ce6();
void Func_02001cea();
void Func_02001cf4();
void Func_02001d04();
void Func_02001d06();
void Func_02001d0a();
void Func_02001d20();
void Func_02001d24();
void Func_02001d2c();
void Func_02001d2e();
void Func_02001d3c();
void Func_02001d44();
void Func_02001d46();
void Func_02001d4c();
void Func_02001d62();
void Func_02001d66();
void Func_02001d68();
void Func_02001d6e();
void Func_02001d78();
void Func_02001d80();
void Func_02001d86();
void Func_02001d88();
void Func_02001d94();
void Func_02001da0();
void Func_02001da6();
void Func_02001dac();
void Func_02001db2();
void Func_02001db4();
void Func_02001dba();
void Func_02001dbc();
void Func_02001dc4();
void Func_02001dcc();
void Func_02001dce();
void Func_02001dd6();
void Func_02001de4();
void Func_02001df2();
void Func_02001dfe();
void Func_02001e04();
void Func_02001e12();
void Func_02001e18();
void Func_02001e1c();
void Func_02001e2a();
void Func_02001e38();
void Func_02001e3e();
void Func_02001e40();
void Func_02001e46();
void Func_02001e4a();
void Func_02001e64();
void Func_02001e66();
void Func_02001e70();
void Func_02001e76();
void Func_02001e84();
void Func_02001e92();
void Func_02001e94();
void Func_02001e9e();
void Func_02001ea0();
void Func_02001eae();
void Func_02001eb6();
void Func_02001ebc();
void Func_02001eca();
void Func_02001ee4();
void Func_02001ef6();
void Func_02001efa();
void Func_02001efe();
void Func_02001f04();
void Func_02001f0a();
void Func_02001f10();
void Func_02001f16();
void Func_02001f18();
void Func_02001f26();
void Func_02001f28();
void Func_02001f2c();
void Func_02001f40();
void Func_02001f4c();
void Func_02001f4e();
void Func_02001f52();
void Func_02001f56();
void Func_02001f5c();
void Func_02001f5e();
void Func_02001f60();
void Func_02001f6e();
void Func_02001f72();
void Func_02001f7c();
void Func_02001f86();
void Func_02001f8a();
void Func_02001f8e();
void Func_02001f96();
void Func_02001f98();
void Func_02001fa6();
void Func_02001fb4();
void Func_02001fb8();
void Func_02001fc2();
void Func_02001fca();
void Func_02001fe6();
void Func_02001fec();
void Func_02001ff2();
void Func_02001ffa();
void Func_02002000();
void Func_0200200e();
void Func_02002018();
void Func_0200201c();
void Func_02002038();
void Func_0200204a();
void Func_02002054();
void Func_0200205a();
void Func_0200205e();
void Func_02002062();
u8 *Func_02002064(s32);
void Func_0200206e();
void Func_02002072();
void Func_0200207a();
void Func_02002086();
void Func_0200208e(s32, s32, s32);
u8 *Func_02002094(s32);
void Func_02002096();
void Func_020020a4();
void Func_020020b6();
void Func_020020be(s32, s32, s32);
u8 *Func_020020c4(s32);
void Func_020020c6();
void Func_020020cc();
void Func_020020d4();
void Func_020020e0();
void Func_020020e6();
void Func_020020ee(s32, s32, s32);
void Func_020020f6();
void Func_02002104();
void Func_02002116();
void Func_0200216e();
void Func_020020ec();
void Func_02002164();
void Func_020020fa();
void Func_0200219a();
void Func_02002108();
void Func_0200218a();
void Func_020021c4();
void Func_02002122();
void Func_020021ce();
void Func_02002134();
void Func_020021d4();
void Func_02002142();
void Func_020021aa();
void Func_02002150();
void Func_020021f0();
void Func_0200215e();
void Func_020021d6();
void Func_0200216c();
void Func_0200220c();
void Func_0200217a();
void Func_020021f2();
void Func_02002188();
void Func_02002228();
void Func_02002196();
void Func_0200220e();
void Func_020021a4();
void Func_02002222();
void Func_020021b8();
void Func_02002258();
void Func_020021c6();
void Func_0200222e();
void Func_02002274();
void Func_020021e2();
void Func_0200224a();
void Func_02002290();
void Func_020021fe();
void Func_02002266();
void Func_020022ac();
void Func_0200221a();
void Func_0200229c();
s32 Func_020022bc();

/* The import's main-image target is Func_0808a1f0. Tracked scene callers
 * identify its second argument (258) as a pose id. */

/* Contiguous unnamed leaf-owner run for resource_3c7. */

/*
 * resource_3c7 owner at 0x02000044, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000048 holding 0x2009438.
 *
 * LEAF RESIDUE. Published at image offset 0xc; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x02000044 reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x2009438 is image offset
 * 0x1438 under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

/*
 * resource_3c7 owner at 0x0200007c, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000080 holding 0x20096d0.
 *
 * LEAF RESIDUE. Published at image offset 0x14; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x0200007c reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x20096d0 is image offset
 * 0x16d0 under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

/*
 * Resource 3c7 script selector at 0x020004b0.
 *
 * Complete owner: `push {lr}` at 0x020004b0 through `pop {r1} / bx r1` at
 * 0x020004e6.  r0 is not the popped return address, so it survives and is the
 * result (HANDOVER §0).  0x020004ea is an alignment halfword and
 * 0x020004ec-0x02000507 is this owner's literal pool; neither is executed.
 *
 * This is the two-by-two sibling of 0x02000084: the same scene-id test and the
 * same 0x9a7 predicate, but here the predicate is evaluated on BOTH sides of
 * the scene-id test rather than only the matching one.  The two `bl` sites at
 * 0x020004c6 and 0x020004d8 reach the same import veneer with the same
 * argument, and are written as two calls because they are genuinely two sites
 * on disjoint paths (site count 2, matching `overlay_call_targets.ts`).
 *
 * Link base 0x02008000: the four returned pool words 0x0200a010 / 0x02009eb4 /
 * 0x02009ca4 / 0x02009a94 are in-image data at file offsets 0x2010 / 0x1eb4 /
 * 0x1ca4 / 0x1a94, all even and so data rather than Thumb entries.
 *
 * The scene-state block at 0x02000240 is the cross-overlay idiom; index 224
 * (byte offset 448) is the signed scene/progress id.
 */

/* Four scripts embedded in this overlay's own data. */

/* Import veneer; used in a condition, so the return type is stated and the
 * arity left open. */

/* Audited retained scene body: all 306 calls through the next owner boundary
 * are represented in machine order. The approved compiler changes scheduling
 * and literal placement, so production retains the reviewed assembly. */

/* Loader-relocated ROM calls: each site names the pre-relocation call word the image holds. */

/*
 * Resource 3c7 cutscene script at 0x02000eec.
 *
 * Complete owner: `push {lr}` at 0x02000eec through the interworking return
 * `pop {r0} / bx r0` at 0x0200103a.  r0 receives the popped return address, so
 * the owner returns nothing (HANDOVER §0).  r0's entry value is overwritten
 * before use, so the owner takes no arguments.
 *
 * Straight-line: the body contains no branch of any kind.  The only non-code
 * bytes in the span are the alignment halfword at 0x0200103e and the literal
 * pool 0x02001040-0x0200104b, all past the epilogue; a control-flow walk from
 * the prologue confirms nothing else is skipped.
 *
 * The three pool words are small constants (((s32) &Value_00002880), 0x0105, 0x2002), not
 * in-image addresses, so this owner is not a link-base witness either way.
 *
 * The script is a sequence of "act on channel N, then wait k frames" beats.
 * Func_0808a010 is the wait (its argument is a frame count), and channels 0,
 * 1, 2, 3, 11 and 12 are driven by Func_0808a110 / Func_0808a138 /
 * Func_0808a180.
 *
 * Call accounting, per target, against `overlay_call_targets.ts`
 * (46 sites, 9 distinct); each count was verified against the individual call
 * sites rather than inferred from the total:
 *   Func_0808a010  22   Func_0808a180  9    Func_0808a138  5
 *   Func_0808a110  4    Func_0808a148  2    Func_0808a170  1
 *   Func_0808a178  1    Func_0808a1b8  1    Func_0808a1e8  1
 *
 * Uncertainties: the channel ids and the beat constants are transcribed
 * literally; what each channel drives is not established here, and the meaning
 * of Func_0808a1e8's middle argument 0x105 (0x102 at the corresponding site in
 * Func_02000308) is unknown.
 */

/* Import veneers.  Old-style declarations: one import name can take different
 * argument counts at different sites in this overlay, so no arity is asserted
 * and none of these results is used. */

static __inline__ void ConfigureActorPose(s32 actor, s32 pose)
{
    SetActorPose(actor, pose);
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    void Func_02001708();

    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    void Func_02001708();

    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    void Func_02001708();

    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    void Func_02001708();

    f(a0, a1, a2, a3);
}

static __inline__ s32 Value0(s32 (*f)())
{
    void Func_02001708();

    return f();
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    void Func_02001708();

    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    void Func_02001708();

    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    void Func_02001708();

    return f(a0, a1, a2);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    void Func_02001708();

    return f(a0, a1, a2, a3);
}

s32 SetActorFourteenPose(void)
{
    ConfigureActorPose(14, 258);
    return 0;
}

u8 *Func_02000044(void)
{
    return (u8 *)0x02009438;
}

s32 Func_0200004c(void) {
    if (Data_02000240[224] == (s32)&Value_000000b3) {
        return (s32)Data_02009690;
    }
    return (s32)Data_020096b0;
}

u8 *Func_0200007c(void)
{
    return (u8 *)0x020096d0;
}

u8 *Func_02000084(void)
{
    if (Data_02000240[224] == (s32) &Value_000000b4) {
        if (Func_02001278(0x9A7) != 0) {
            return Data_02009974;
        }
        return Data_0200989c;
    }
    return Data_02009734;
}

void Func_020003f4(void)
{
    s32 Func_020016a4(s32, s32);

    Func_020015de(0x9BC);
    Func_020015fa();
    Func_020016d6();
    Func_020015fc(0xA);
    Func_020016c6(0x780000, -1, 0x600000, 1);
    Func_020016d2();
    Func_02001618(0x1E);
    Func_020016a6(0x288B);
    Func_020016be(0xC, 0);
    Func_0200162c(0xA);
    Func_020016ae(0, 0xC, 0);
    Func_0200163c(0x1E);
    Func_020016a4(0, 3);
    Func_0200164a(0x1E);
    Func_0200165e();
}

void Func_02000468(void)
{
    void Func_02001708(s32, s32);

    Func_02001652(0x9BC);
    Func_020016f0(0x288B);
    Func_02001708(0xC, 0);
}

void Func_0200048c(void)
{
    u8 *state = Data_03001ebc;
    s16 *counter = (s16 *)(state + 0x16C);

    Func_0200175a(*counter);
    Func_020017b0(0x7B);
}

u8 *Func_020004b0(void)
{
    s32 Func_020016a4();

    if (Data_02000240[224] == (s32)&Value_000000b4) {
        if (Func_020016a4(0x9a7) != 0) {
            return Data_0200a010;
        }
        return Data_02009eb4;
    }
    if (Func_020016b6(0x9a7) != 0) {
        return Data_02009ca4;
    }
    return Data_02009a94;
}

void Func_02000508(void)
{
    void Func_02001708();

    Value0(Func_02001708);
    Value1(Func_020016fe, 1);
    Call3(Func_02001764, 0, 6291456, 12058624);
    Call3(Func_020017c0, 0, 49152, 0);
    Call3(Func_020017ca, 11, 0, 0);
    Value3(Func_020017d6, 12, 32768, 0);
    Call1(Func_020017c4, 10320);
    Value0(Func_02001810);
    Value0(Func_0200181c);
    Value1(Func_0200174a, 10);
    Call3(Func_02001780, 0, 65536, 32768);
    Call3(Func_02001854, 0, 0, -16);
    Value3(Func_020017ae, 0, 104, 136);
    Value1(Func_02001774, 10);
    Call4(Func_0200186c, 1, -16, 16, 49152);
    Call4(Func_0200187a, 3, 0, 24, 49152);
    Value4(Func_02001888, 2, 16, 16, 49152);
    Value1(Func_020017ee, 1);
    Value1(Func_020017ac, 20);
    Value1(Func_020017b2, 10);
    Value2(Func_0200182a, 2, 2);
    Value1(Func_020017c0, 20);
    Value2(Func_02001860, 2, 0);
    Value1(Func_020017ce, 10);
    Call3(Func_0200187a, 11, 16384, 0);
    Value3(Func_02001886, 12, 16384, 0);
    Value1(Func_020017ec, 30);
    Call3(Func_0200189e, 11, 261, 70);
    Value2(Func_02001896, 11, 0);
    Value1(Func_02001804, 10);
    Value2(Func_0200186c, 11, 4);
    Value1(Func_02001812, 20);
    Value2(Func_020018b2, 11, 0);
    Value1(Func_02001820, 20);
    Value2(Func_02001898, 11, 2);
    Value1(Func_0200182e, 20);
    Value2(Func_020018ce, 11, 0);
    Value1(Func_0200183c, 20);
    Call3(Func_020018f0, 12, 258, 40);
    Value2(Func_020018e8, 12, 0);
    Value1(Func_02001856, 10);
    Value2(Func_020018be, 12, 4);
    Value1(Func_02001864, 20);
    Value2(Func_02001904, 12, 0);
    Value1(Func_02001872, 20);
    Call1(Func_02001878, 10);
    Call3(Func_0200192a, 11, 261, 50);
    Value2(Func_02001922, 11, 0);
    if (Value1(Func_02001878, 2495) == 0) {
        Value0(Func_02001590);
    } else {
    Call1(Func_020018a0, 20);
    Value2(Func_02001918, 11, 2);
    Value1(Func_020018ae, 20);
    Value2(Func_0200194e, 11, 0);
    Value1(Func_020018bc, 10);
    Call3(Func_0200193e, 1, 0, 50);
    Call3(Func_02001978, 0, 261, 60);
    Value1(Func_020018d6, 10);
    Value3(Func_02001982, 1, 49152, 0);
    Value1(Func_020018e8, 20);
    Value2(Func_02001988, 1, 0);
    Value1(Func_020018f6, 10);
    Value2(Func_0200195e, 3, 4);
    Value1(Func_02001904, 20);
    Value2(Func_020019a4, 3, 0);
    Value1(Func_02001912, 10);
    Value2(Func_0200198a, 2, 2);
    Value1(Func_02001920, 20);
    Value2(Func_020019c0, 2, 0);
    Value1(Func_0200192e, 20);
    Call3(Func_020019e0, 12, 257, 40);
    Value2(Func_020019d0, 12, 0);
    Value2(Func_02001960, 0, 0);
    Value1(Func_0200194e, 20);
    Value2(Func_020019c6, 12, 2);
    Value1(Func_0200195c, 20);
    Value2(Func_020019fc, 12, 0);
    Value1(Func_0200196a, 20);
    Value2(Func_020019d2, 0, 3);
    Value1(Func_02001978, 30);
    Value1(Func_0200197e, 10);
    Value2(Func_020019f6, 1, 2);
    Value1(Func_0200198c, 20);
    Value2(Func_02001a2c, 1, 0);
    Value1(Func_0200199a, 10);
    Value2(Func_02001a02, 3, 4);
    Value1(Func_020019a8, 20);
    Value2(Func_02001a48, 3, 0);
    Value1(Func_020019b6, 10);
    Value2(Func_02001a1e, 2, 3);
    Value1(Func_020019c4, 30);
    Value2(Func_02001a64, 2, 0);
    Value1(Func_020019d2, 10);
    Call3(Func_02001a54, 2, 0, 30);
    Call2(Func_02001a74, 8194, 0);
    }
    Call1(Func_02001a72, 10337);
    Value1(Func_020019f0, 10);
    Call3(Func_02001a9c, 0, 16384, 0);
    Call3(Func_02001a7e, 1, 0, 20);
    if (Value2(Func_02001a26, 0, 0) == 0) {
    Value1(Func_02001a18, 20);
    Call3(Func_02001ac4, 0, 49152, 0);
    Call3(Func_02001ad0, 1, 49152, 0);
    Value3(Func_02001adc, 2, 49152, 0);
    Value1(Func_02001a42, 30);
    Value1(Func_02001a48, 10);
    Value2(Func_02001ab0, 11, 3);
    Value1(Func_02001a56, 30);
    Value2(Func_02001af6, 11, 0);
    (*(u16 *)(*(u8 **)0x03001ebc + 0x1d8))++;
    } else {
    Call1(Func_02001a94, 20);
    Call3(Func_02001b40, 0, 49152, 0);
    Call3(Func_02001b4c, 1, 49152, 0);
    Value3(Func_02001b58, 2, 49152, 0);
    Value1(Func_02001abe, 30);
    Value1(Func_02001ac4, 10);
    Value2(Func_02001b2c, 11, 4);
    Value1(Func_02001ad2, 20);
    (*(u16 *)(*(u8 **)0x03001ebc + 0x1d8))++;
    Call2(Func_02001b82, 11, 0);
    }
    Value1(Func_02001af0, 10);
    Value2(Func_02001b68, 11, 2);
    Value1(Func_02001afe, 20);
    Value2(Func_02001b9e, 11, 0);
    Value1(Func_02001b0c, 10);
    Call3(Func_02001b96, 1, 0, 0);
    Value3(Func_02001ba0, 3, 2, 0);
    Value1(Func_02001b26, 40);
    Value1(Func_02001b2c, 10);
    Call3(Func_02001be0, 11, 258, 40);
    Value2(Func_02001bd8, 11, 0);
    Value1(Func_02001b46, 10);
    Call3(Func_02001bf2, 0, 49152, 0);
    Call3(Func_02001bfe, 1, 49152, 0);
    Call3(Func_02001c0a, 2, 49152, 0);
    Value3(Func_02001c16, 3, 49152, 0);
    Value1(Func_02001b7c, 50);
    Value1(Func_02001b82, 10);
    Value2(Func_02001bea, 11, 3);
    Value1(Func_02001b90, 30);
    Value2(Func_02001c30, 11, 0);
    Value1(Func_02001b9e, 10);
    Value2(Func_02001c16, 11, 2);
    Value1(Func_02001bac, 20);
    Value2(Func_02001c4c, 11, 0);
    Value1(Func_02001bba, 10);
    Call3(Func_02001c6e, 3, 256, 40);
    Value2(Func_02001c66, 3, 0);
    Value1(Func_02001bd4, 20);
    Value3(Func_02001c7e, 11, 0, 0);
    Value1(Func_02001be4, 20);
    Value2(Func_02001c84, 11, 0);
    Value1(Func_02001bf2, 10);
    Value2(Func_02001c6a, 12, 2);
    Value1(Func_02001c00, 20);
    Value3(Func_02001cac, 12, 32768, 0);
    Value1(Func_02001c12, 20);
    Value2(Func_02001cb2, 12, 0);
    Value1(Func_02001c20, 10);
    Value2(Func_02001c88, 11, 4);
    Value1(Func_02001c2e, 20);
    Value2(Func_02001cce, 11, 0);
    Value1(Func_02001c3c, 10);
    Value2(Func_02001cb4, 2, 2);
    Value1(Func_02001c4a, 20);
    Value2(Func_02001cea, 2, 0);
    Value1(Func_02001c58, 10);
    Value3(Func_02001d04, 11, 16384, 0);
    Value1(Func_02001c6a, 30);
    Value2(Func_02001d0a, 11, 0);
    Value1(Func_02001c78, 10);
    Call3(Func_02001d2c, 12, 258, 50);
    Value2(Func_02001d24, 12, 0);
    Value1(Func_02001c92, 20);
    Call3(Func_02001d44, 11, 257, 65);
    Value3(Func_02001d46, 11, 0, 0);
    Value1(Func_02001cac, 20);
    Value2(Func_02001d4c, 11, 0);
    Value1(Func_02001cba, 20);
    Call3(Func_02001d6e, 12, 256, 40);
    Value2(Func_02001d66, 12, 0);
    Value1(Func_02001cd4, 20);
    Value3(Func_02001d80, 12, 16384, 0);
    Call1(Func_02001ce6, 40);
    Value2(Func_02001d86, 12, 0);
    Value1(Func_02001cf4, 10);
    Value3(Func_02001da0, 11, 16384, 0);
    Value1(Func_02001d06, 50);
    Call3(Func_02001dba, 11, 262, 60);
    Value2(Func_02001db2, 11, 0);
    Value1(Func_02001d20, 10);
    Value2(Func_02001d88, 11, 3);
    Value1(Func_02001d2e, 30);
    Value2(Func_02001dce, 11, 0);
    Value1(Func_02001d3c, 10);
    Call2(Func_02001dac, 0, 2);
    Call2(Func_02001db4, 1, 2);
    Call2(Func_02001dbc, 3, 2);
    Value2(Func_02001dcc, 2, 2);
    Value1(Func_02001d62, 30);
    Value1(Func_02001d68, 10);
    Value3(Func_02001e12, 11, 0, 0);
    Value1(Func_02001d78, 20);
    Value2(Func_02001e18, 11, 0);
    Value1(Func_02001d86, 10);
    Value2(Func_02001dfe, 12, 2);
    Value1(Func_02001d94, 30);
    Value3(Func_02001e40, 12, 32768, 0);
    Value1(Func_02001da6, 20);
    Value2(Func_02001e46, 12, 0);
    Value1(Func_02001db4, 10);
    Call3(Func_02001e66, 11, 257, 0);
    Value1(Func_02001dc4, 10);
    Value3(Func_02001e70, 1, 57344, 0);
    Value1(Func_02001dd6, 30);
    Value2(Func_02001e3e, 1, 4);
    Value1(Func_02001de4, 20);
    Value2(Func_02001e84, 1, 0);
    Value1(Func_02001df2, 10);
    Value3(Func_02001e9e, 12, 16384, 0);
    Value1(Func_02001e04, 30);
    Call3(Func_02001eb6, 12, 261, 60);
    Value2(Func_02001eae, 12, 0);
    Value1(Func_02001e1c, 10);
    Value2(Func_02001e94, 1, 2);
    Value1(Func_02001e2a, 20);
    Value2(Func_02001eca, 1, 0);
    Value1(Func_02001e38, 10);
    Value3(Func_02001ee4, 11, 16384, 0);
    Value1(Func_02001e4a, 40);
    Call3(Func_02001efe, 11, 258, 40);
    Value2(Func_02001ef6, 11, 0);
    Value1(Func_02001e64, 10);
    Value3(Func_02001f10, 2, 32768, 0);
    Value1(Func_02001e76, 30);
    Call3(Func_02001f28, 2, 263, 60);
    Value3(Func_02001f2c, 2, 49152, 0);
    Value1(Func_02001e92, 30);
    Value2(Func_02001efa, 2, 4);
    Value1(Func_02001ea0, 20);
    Value2(Func_02001f40, 2, 0);
    Value1(Func_02001eae, 10);
    Value2(Func_02001f16, 11, 4);
    Value1(Func_02001ebc, 20);
    Value2(Func_02001f5c, 11, 0);
    Value1(Func_02001eca, 10);
    Call2(Func_02001f4e, 0, 2);
    Call2(Func_02001f56, 1, 2);
    Call2(Func_02001f5e, 3, 2);
    Value2(Func_02001f6e, 2, 2);
    Value1(Func_02001f04, 30);
    Value1(Func_02001f0a, 10);
    Value2(Func_02001f72, 11, 3);
    Value1(Func_02001f18, 30);
    Value2(Func_02001fb8, 11, 0);
    Value1(Func_02001f26, 20);
    Call2(Func_02001f86, 0, 3);
    Call2(Func_02001f8e, 1, 3);
    Call2(Func_02001f96, 3, 3);
    Value2(Func_02001fa6, 2, 3);
    Value1(Func_02001f4c, 30);
    Value1(Func_02001f52, 10);
    Value2(Func_02001fca, 11, 2);
    Value1(Func_02001f60, 20);
    Value2(Func_02002000, 11, 0);
    Value1(Func_02001f6e, 10);
    Value2(Func_02001fe6, 1, 2);
    Value1(Func_02001f7c, 20);
    Value2(Func_0200201c, 1, 0);
    Value1(Func_02001f8a, 10);
    Value2(Func_02001ff2, 11, 3);
    Value1(Func_02001f98, 30);
    Value2(Func_02002038, 11, 0);
    Value1(Func_02001fa6, 20);
    Value2(Func_0200200e, 3, 3);
    Value1(Func_02001fb4, 30);
    Value2(Func_02002054, 3, 0);
    Value1(Func_02001fc2, 20);
    Call3(Func_0200206e, 0, 16384, 0);
    Call3(Func_0200207a, 1, 57344, 0);
    Value3(Func_02002086, 2, 40960, 0);
    Value1(Func_02001fec, 30);
    Value2(Func_02002054, 0, 3);
    Value1(Func_02001ffa, 10);
    Call2(Func_0200205a, 1, 3);
    Call2(Func_02002062, 2, 3);
    Value2(Func_02002072, 3, 3);
    Value1(Func_02002018, 30);
    Call3(Func_0200204a, 1, 78643, 39321);
    Call3(Func_02002054, 3, 78643, 39321);
    Call3(Func_0200205e, 2, 78643, 39321);
    Value2(Func_02002096, 1, 2);
    {
        u8 *entry = Value1(Func_02002064, 0);
        if (entry != 0) {
            s16 y = *(s16 *)(entry + 10);
            s16 x = *(s16 *)(entry + 18);
            Value3(Func_0200208e, 1, y, x);
        }
    }
    Value1(Func_020020a4, 1);
    Call3(Func_020020b6, 1, 0, 0);
    Value2(Func_020020c6, 3, 2);
    {
        u8 *entry = Value1(Func_02002094, 0);
        if (entry != 0) {
            s16 y = *(s16 *)(entry + 10);
            s16 x = *(s16 *)(entry + 18);
            Value3(Func_020020be, 3, y, x);
        }
    }
    Value1(Func_020020d4, 3);
    Call3(Func_020020e6, 3, 0, 0);
    Value2(Func_020020f6, 2, 2);
    {
        u8 *entry = Value1(Func_020020c4, 0);
        if (entry != 0) {
            s16 y = *(s16 *)(entry + 10);
            s16 x = *(s16 *)(entry + 18);
            Value3(Func_020020ee, 2, y, x);
        }
    }
    Value1(Func_02002104, 2);
    Value3(Func_02002116, 2, 0, 0);
    Value1(Func_020020cc, 10);
    Value0(Func_020020e0);
}

void Func_02000eec(void)
{
    Func_0200216e(((s32) &Value_00002880));
    Func_020020ec(20);

    Func_02002164(11, 2);
    Func_020020fa(20);
    Func_0200219a(11, 0);
    Func_02002108(10);

    Func_0200218a(1, 0, 50);
    Func_020021c4(0, 0x105, 60);
    Func_02002122(10);
    Func_020021ce(1, 0xc000, 0);
    Func_02002134(20);
    Func_020021d4(1, 0);
    Func_02002142(10);

    Func_020021aa(3, 4);
    Func_02002150(20);
    Func_020021f0(3, 0);
    Func_0200215e(10);

    Func_020021d6(2, 2);
    Func_0200216c(20);
    Func_0200220c(2, 0);
    Func_0200217a(10);

    Func_020021f2(12, 2);
    Func_02002188(20);
    Func_02002228(12, 0);
    Func_02002196(20);

    Func_0200220e(0, 2);
    Func_020021a4(20);
    Func_020021aa(25);

    Func_02002222(1, 2);
    Func_020021b8(30);
    Func_02002258(1, 0);
    Func_020021c6(30);

    Func_0200222e(1, 4);
    Func_020021d4(20);
    Func_02002274(1, 0);
    Func_020021e2(10);

    Func_0200224a(3, 4);
    Func_020021f0(20);
    Func_02002290(3, 0);
    Func_020021fe(10);

    Func_02002266(2, 3);
    Func_0200220c(30);
    Func_020022ac(2, 0);
    Func_0200221a(10);

    Func_0200229c(2, 0, 30);
    Func_020022bc(0x2002, 0);
}
