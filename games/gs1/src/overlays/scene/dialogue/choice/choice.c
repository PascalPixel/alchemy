#include "types.h"

/* overlays/scene/dialogue/choice/branching_actor_sequence.c */
/* DRAFT for Scene_RunBranchingActorSequence
 * (resource_3b8:0x020017e8, complete owner extent 2092 bytes).
 *
 * A scripted field scene driving four presentation slots (0..3). It opens the
 * scene, installs slot 0's record, seats the three visible slots, brings the
 * layers up, then runs two counted step loops that publish a step index into
 * the scene workspace. From there it plays a long alternating motion/pause
 * script. Three selector points steer it: two identical queries through
 * Func_02005fe0(-1, 0), and a third reuse of the second query's answer held in
 * `pick`. Each taken branch advances the scene script counter in the state
 * record at offset 0x1d8 - by one for the first two branches, by two for the
 * third. The tail spins on the input word at Data_03001ae8, restores the
 * record, re-seats slots 1, 3 and 2 from their live records, and closes the
 * scene.
 *
 * Call names. Overlay call bytes encode the destination only, so the legacy
 * Func_<address> labels are per-site aliases, not distinct routines: the 215
 * calls here reach 26 distinct targets, and this draft declares one name per
 * target with the main-image routine noted beside it.
 *
 * One target has no unambiguous legacy alias, because two call sites in this
 * owner produce the same legacy label for different destinations. It is
 * spelled here as Value_0200c488 - its true runtime address - so the draft
 * compiles and scores standalone. When this owner gets a translation unit,
 * rename it to Func_02005d94_a and declare
 *   "Func_02005d94_a": { "address": "0x0200c488", "kind": "thumb" }
 * in that unit's absolute_symbols; the emitted bytes are the same either way.
 *
 * Call3, Value2 and SetHalf are a modeling hypothesis, not recovered history.
 * The reference materializes each call's argument constants independently - it
 * builds 0x680000 twice for Func_02005c6c(1, ..., ...) - and writes each
 * workspace halfword through its own address and value register. Spelling those
 * sites through small inline helpers reproduces that argument and address
 * materialization; spelling them as direct calls makes the compiler share the
 * constants and does not (measured: every Call3 site written directly moves the
 * residual from 2 to 855 differing halfwords, 154 wrong instructions).
 *
 * Only sites that need that shaping use a helper. Every helper site that
 * remains was measured on its own: rewriting just that one site as a direct
 * call changes the emitted bytes. Sites where a direct call emits identical
 * bytes are spelled directly, so no helper here is decorative.
 *
 * Uncertain: the twenty-six callees, every record and workspace offset, and the
 * resource selectors 0x228c and 0x0100/0x0101/0x0102/0x0103/0x0107/0x0108 are
 * unrecovered and keep raw addresses and raw offsets. The workspace halfwords at
 * 0x534 and 0x536 and the state-record words at 0x1c0/0x1c8 are written but
 * never read here. The residual against the reference is one scheduled pair:
 * the compiler hoists
 * `ldr r1, [r6, #0]` above the `strh` that precedes it at owner offset 0xf4. */

#define Scene_RunBranchingActorSequence Func_020017e8

void Func_02005b7e();    /* main 0x080091e0 */
void Func_02005bae();    /* main 0x080000c0, frame wait */
void Func_02005bcc();    /* main 0x0808a018, scene open */
s32 Func_02005c00();     /* main 0x0808a080, record lookup */
void Func_02005c5a();    /* main 0x0808a100 */
void Func_02005c6c();    /* main 0x0808a0f0 */
void Func_02005c8a();    /* main 0x0808a170 */
void Func_02005c96();    /* main 0x0808a010, script pause */
void Func_02005cec();    /* main 0x0808a1b8 */
void Func_02005d38();    /* main 0x0808a138 */
void Func_02005d5c();    /* main 0x0808a300 */
void Func_02005d82();    /* main 0x0808a360 */
void Func_02005d88();    /* main 0x0808a090 */
void Func_02005d8e();    /* main 0x0808a370 */
void Value_0200c488();   /* main 0x0808a148; see the call-name note above */
void Func_02005dba();    /* main 0x0808a1e8 */
void Func_02005dfa();    /* main 0x0808a180 */
void Func_02005e48();    /* main 0x0808a110 */
void Func_02005ebc();    /* main 0x0808a580 */
void Func_02005ed0();    /* main 0x0808a178 */
void Func_02005ed6();    /* main 0x0808a128 */
s32 Func_02005fe0();     /* main 0x0808a070, branch selector */
void Func_0200629c();    /* main 0x0808a150 */
void Func_020063a2();    /* main 0x0808a0b8 */
void Func_020063d0();    /* main 0x0808a0e8 */
void Func_020063dc();    /* main 0x0808a020, scene close */

/* Argument and store shaping helpers; see the header note. */

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The work-record cells are written as a union, not as bare halfwords: the
 * halfword store and the scene-pointer read that follows are only ordered
 * against each other when they can alias. */
union WorkCell {
    s32 w;
    u16 h;
};

static __inline__ void SetHalf(u16 *at, s32 value)
{
    ((union WorkCell *)at)->h = value;
}

extern u8 *Data_03001ebc[];
extern s32 Data_03001ae8;

void Func_020017e8(void)
{
    s32 record;
    s32 step;
    s32 pick;
    s32 state;
    u8 *work;

    Func_02005bcc();
    Func_02005c8a(0x228c);
    Func_02005c5a(0, 31);
    record = Func_02005c00(0);
    Func_02005b7e(record, 0);
    Call3(Func_02005c6c, 1, 0x680000, 0x680000);
    Call3(Func_02005c6c, 3, 0x580000, 0x780000);
    Call3(Func_02005c6c, 2, 0x780000, 0x780000);
    Call3(Func_02005cec, 1, 0x4000, 0);
    Func_02005cec(3, 0, 0);
    Call3(Func_02005cec, 2, 0x8000, 0);
    state = (s32)Data_03001ebc[0];
    *(s32 *)(state + 0x1c0) = 0x100;
    *(s32 *)(state + 0x1c8) = 12;
    Func_02005bae(1);
    Func_02005d5c(9);
    work = Data_03001ebc[4];
    SetHalf((u16 *)(work + 0x52a), 0);
    SetHalf((u16 *)(work + 0x534), 0x1f1f);
    SetHalf((u16 *)(work + 0x536), 1);
    Func_02005d82();
    Func_02005d8e();
    for (step = 1; step <= 5; step++) {
        Func_02005bae(3);
        *(u16 *)(work + 0x52a) = step;
    }
    Func_02005c96(40);
    Func_02005d38(0, 2);
    Func_02005c96(30);
    for (step = 5; step <= 31; step++) {
        Func_02005bae(3);
        *(u16 *)(work + 0x52a) = step;
    }
    SetHalf((u16 *)(work + 0x536), 31);
    state = (s32)Data_03001ebc[0];
    *(s32 *)(state + 0x1c0) = 0x209;
    *(s32 *)(state + 0x1c8) = 24;
    Func_02005c96(20);
    Call3(Func_02005dba, 1, 0x100, 50);
    Value_0200c488(1, 0, 40);
    Func_02005d38(1, 2);
    Func_02005c96(30);
    Func_02005c96(10);
    Call3(Func_02005dba, 2, 0x101, 40);
    Call3(Func_02005cec, 2, 0xe000, 0);
    Func_02005c96(30);
    Func_02005dfa(2, 0);
    Func_02005c96(10);
    Func_02005dfa(3, 0);
    Func_02005cec(3, 0xe000, 0);
    Func_02005c96(30);
    Func_02005d38(3, 2);
    Func_02005c96(30);
    Call3(Func_02005d88, 3, 0x10000, 0x8000);
    Call3(Func_02005ebc, 3, 0, -40);
    Func_02005ebc(3, 32, 0);
    Func_02005cec(3, 0x2000, 0);
    Func_02005c96(10);
    Func_02005dfa(3, 0);
    record = Func_02005c00(0);
    *(s32 *)(record + 16) += -0x30000;
    record = Func_02005c00(0);
    *(s32 *)(record + 64) += -0x30000;
    Func_02005c5a(0, 32);
    Func_02005c96(40);
    Func_02005e48(0, 34);
    Func_02005c96(30);
    Func_02005c5a(0, 33);
    Func_02005c96(40);
    Func_02005e48(1, 4);
    Func_02005c96(20);
    Func_02005dfa(1, 0);
    Func_02005c96(10);
    Call3(Func_02005dba, 0, 0x102, 80);
    Call3(Func_02005dba, 2, 0x100, 50);
    Value2(Func_02005ed0, 2, 0);
    if (Func_02005fe0(-1, 0) == 0) {
        Func_02005c96(10);
        Func_02005e48(0, 33);
        Func_02005c96(20);
        Call3(Func_02005dba, 1, 0x103, 40);
        Func_02005ed6(1, 4, 13);
        Func_02005ed6(1, 4, 30);
        Func_02005dfa(1, 0);
        *(u16 *)(Data_03001ebc[0] + 0x1d8) += 1;
    } else {
        Func_02005c96(10);
        Func_02005e48(0, 34);
        Func_02005c96(20);
        Call3(Func_02005dba, 1, 0x103, 40);
        Func_02005ed6(1, 4, 13);
        Func_02005ed6(1, 4, 30);
        *(u16 *)(Data_03001ebc[0] + 0x1d8) += 1;
        Func_02005dfa(1, 0);
    }
    Func_02005c96(10);
    Func_02005cec(2, 0xa000, 0);
    Func_02005c96(30);
    Func_02005e48(2, 4);
    Func_02005c96(20);
    Call3(Func_02005cec, 1, 0x2000, 0);
    Func_02005c96(30);
    Func_02005d38(1, 2);
    Func_02005c96(40);
    Call3(Func_02005cec, 1, 0xe000, 0);
    Func_02005cec(2, 0xe000, 0);
    Func_02005c96(30);
    Func_02005e48(1, 3);
    Func_02005c96(30);
    Func_02005dfa(1, 0);
    Func_02005c96(10);
    Func_02005cec(3, 0x6000, 0);
    Func_02005c96(50);
    Func_02005cec(3, 0x2000, 0);
    Func_02005c96(35);
    Func_02005dba(3, 0x108, 50);
    Func_02005c96(10);
    Func_02005e48(3, 3);
    Func_02005c96(30);
    Func_02005dfa(3, 0);
    Func_02005c96(10);
    Func_02005d38(2, 2);
    Func_02005c96(20);
    Value2(Func_02005ed0, 2, 0);
    if (Func_02005fe0(-1, 0) == 0) {
        Func_02005c96(10);
        Func_02005e48(0, 33);
        Func_02005c96(20);
        Call3(Func_02005dba, 1, 0x107, 40);
        Func_02005dfa(1, 0);
        pick = 0;
        *(u16 *)(Data_03001ebc[0] + 0x1d8) += 1;
    } else {
        Func_02005c96(10);
        Func_02005e48(0, 34);
        Func_02005c96(20);
        Call3(Func_02005dba, 1, 0x107, 40);
        *(u16 *)(Data_03001ebc[0] + 0x1d8) += 1;
        pick = 1;
        Func_02005dfa(1, 0);
    }
    Func_02005c96(10);
    Func_02005cec(2, 0xa000, 0);
    Func_02005c96(30);
    Func_02005e48(2, 4);
    Func_02005c96(20);
    Call3(Func_02005cec, 1, 0x2000, 0);
    Func_02005c96(30);
    Func_02005d38(1, 2);
    Func_02005c96(40);
    Call3(Func_02005cec, 1, 0xe000, 0);
    Func_02005cec(2, 0xe000, 0);
    Func_02005c96(30);
    Func_02005e48(1, 3);
    Func_02005c96(30);
    Func_02005dfa(1, 0);
    Func_02005c96(10);
    Func_02005cec(3, 0x6000, 0);
    Func_02005c96(50);
    Func_02005cec(3, 0x2000, 0);
    Func_02005c96(35);
    Func_02005dba(3, 0x108, 50);
    Func_02005c96(10);
    Func_02005e48(3, 3);
    Func_02005c96(30);
    Func_02005dfa(3, 0);
    if (pick == 0) {
        Func_02005c96(10);
        Func_02005e48(2, 3);
        Func_02005c96(30);
        Func_02005dfa(2, 0);
        Func_02005c96(10);
        Func_02005d38(1, 2);
        Func_02005c96(30);
        Func_02005dfa(1, 0);
        *(u16 *)(Data_03001ebc[0] + 0x1d8) += 2;
    } else {
        *(u16 *)(Data_03001ebc[0] + 0x1d8) += 2;
        Func_02005c96(10);
        Func_02005e48(2, 4);
        Func_02005c96(20);
        Func_02005dfa(2, 0);
        Func_02005c96(10);
        Func_02005d38(1, 2);
        Func_02005c96(20);
        Func_02005dfa(1, 0);
    }
    Func_02005c96(10);
    Func_02005dba(0, 0x102, 60);
    Func_02005c96(10);
    Func_02005e48(1, 3);
    Func_02005c96(30);
    Func_02005dfa(1, 0);
    Func_02005c96(20);
    Func_0200629c(2, 3, 40);
    Call3(Func_02005cec, 3, 0x2000, 0);
    Func_02005cec(2, 0xe000, 0);
    Func_02005c96(30);
    while ((Data_03001ae8 & 240) == 0) {
        Func_02005bae(1);
    }
    Func_02005ed6(0, 6, 0);
    Call3(Func_02005d88, 0, 0x1e666, 0xf333);
    Func_02005ebc(0, -32, -8);
    record = Func_02005c00(0);
    Func_02005b7e(record, 1);
    Func_02005c96(20);
    Call3(Func_02005cec, 0, 0xc000, 0);
    Call3(Func_02005cec, 3, 0x4000, 0);
    Func_02005cec(2, 0xc000, 0);
    Func_02005c96(20);
    Func_02005c96(10);
    Func_02005d38(3, 2);
    Func_02005c96(20);
    Func_02005dfa(3, 0);
    Func_02005c96(10);
    Func_02005d38(1, 2);
    Func_02005c96(20);
    Func_02005dfa(1, 0);
    Func_02005c96(20);
    Func_02005e48(2, 3);
    Func_02005c96(30);
    Call3(Func_02005cec, 0, 0x4000, 0);
    Func_02005cec(1, 0x2000, 0);
    Func_02005c96(30);
    Func_02005dfa(2, 0);
    Func_02005c96(20);
    Func_02005c5a(0, 3);
    Func_02005c5a(1, 3);
    Func_02005e48(3, 3);
    Func_02005c96(30);
    Call3(Func_02005d88, 1, 0x13333, 0x9999);
    Call3(Func_02005d88, 3, 0x13333, 0x9999);
    Call3(Func_02005d88, 2, 0x13333, 0x9999);
    Func_02005c5a(1, 2);
    record = Func_02005c00(0);
    if (record != 0) {
        Func_020063a2(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_020063d0(1);
    Func_02005c6c(1, 0, 0);
    Func_02005c5a(3, 2);
    record = Func_02005c00(0);
    if (record != 0) {
        Func_020063a2(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_020063d0(3);
    Func_02005c6c(3, 0, 0);
    Func_02005c5a(2, 2);
    record = Func_02005c00(0);
    if (record != 0) {
        Func_020063a2(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_020063d0(2);
    Func_02005c6c(2, 0, 0);
    Func_02005c96(10);
    Func_020063dc();
}
#undef Scene_RunBranchingActorSequence

/* overlays/scene/dialogue/choice/dialogue_layout.c */
#define Dialogue_ShowMessage22a8Branch Func_02000524
#define Dialogue_ShowMessage22a3Branch Func_02003fc4
extern u8 Value_000022a8;
extern u8 Value_000022a3;

void Func_020049c6(s32);
s32 Func_020049d6(s32, s32);
s32 Func_02004936(s32, s32);
void Func_020049e0(s32);
void Func_020049e8(s32);
s32 Func_02004a00(s32, s32);
void Func_02008466(s32);
s32 Func_02008476(s32, s32);
s32 Func_020083d6(s32, s32);
void Func_02008480(s32);
void Func_02008498(s32, s32);
void Func_02008490(s32);
void Func_020084a8(s32, s32);

void Dialogue_ShowMessage22a8Branch(s32 a)
{
    s32 k = (s32)&Value_000022a8;

    Func_020049c6(k);
    Func_020049d6(a, 0);
    if (Func_02004936(0, 0) == 0)
        Func_020049e0(k + 1);
    else
        Func_020049e8(k + 2);
    Func_02004a00(a, 0);
}

void Dialogue_ShowMessage22a3Branch(s32 a)
{
    s32 k = (s32)&Value_000022a3;

    Func_02008466(k);
    Func_02008476(a, 0);
    if (Func_020083d6(0, 0) == 0) {
        Func_02008480(k + 1);
        Func_02008498(a, 0);
    } else {
        Func_02008490(k + 2);
        Func_020084a8(a, 0);
    }
}
#undef Dialogue_ShowMessage22a8Branch
#undef Dialogue_ShowMessage22a3Branch

/* overlays/scene/dialogue/choice/run_main_cutscene_sequence.c */
void Func_020063fe();
void Func_02006420();
void Func_02006426();
void Func_02006434();
void Func_0200644e();
void Func_0200645c();
void Func_0200646a();
void Func_0200646e();
void Func_02006484();
void Func_020064a4();
s32 Func_020064bc();
void Func_020064c2();
s32 Func_020064d8();
void Func_020064e6();
void Func_020064ee();
void Func_020064f4();
void Func_020064fe();
void Func_020064fe_a();
void Func_02006502();
s32 Func_0200650e();
s32 Func_02006518();
void Func_0200651a();
void Func_0200651c();
void Func_02006524();
void Func_0200652a();
void Func_02006534();
void Func_02006538();
void Func_02006542();
void Func_0200654a();
void Func_02006558();
void Func_02006560();
s32 Func_02006562();
void Func_02006566();
s32 Func_0200656a();
void Func_02006574();
void Func_0200657e();
void Func_0200658c();
void Func_0200658e();
void Func_020065a0();
void Func_020065ae();
void Func_020065ae_a();
void Func_020065b4();
void Func_020065bc();
void Func_020065cc();
void Func_020065ce();
void Func_020065da();
void Func_020065e2();
void Func_020065ea();
void Func_020065fa();
void Func_020065fe();
void Func_02006602();
void Func_02006608();
void Func_0200660e();
void Func_0200661a();
void Func_0200661c();
void Func_0200661c_a();
void Func_0200662a();
void Func_02006630();
void Func_02006634();
void Func_02006646();
void Func_0200664c();
u8 *Func_02006652();
void Func_02006654();
void Func_02006660();
void Func_02006662();
u8 *Func_0200666e();
void Func_0200667c();
void Func_02006692();
void Func_0200669c();
void Func_020066a0();
void Func_020066b0();
s32 Func_020066b6();
void Func_020066bc();
void Func_020066cc();
void Func_020066ce();
void Func_020066dc();
void Func_020066ec();
void Func_02006700();
void Func_0200670c();
void Func_0200671a();
void Func_02006730();
void Func_02006734();
void Func_02006734_a();
void Func_02006742();
void Func_02006750();
void Func_0200675e();
void Func_02006762();
void Func_02006770();
void Func_0200677e();
void Func_02006782();
void Func_0200678a();
void Func_02006798();
void Func_02006798_a();
s32 Func_020067a0();
s32 Func_020067a0_a();
void Func_020067a4();
void Func_020067ac();
void Func_020067be();
void Func_020067cc();
void Func_020067cc_a();
void Func_020067ce();
void Func_020067da();
void Func_020067da_a();
void Func_020067f2();
void Func_020067f8();
void Func_020067f8_a();
void Func_02006806();
void Func_0200680a();
void Func_02006812();
void Func_02006814();
void Func_02006818();
void Func_02006822();
void Func_02006826();
void Func_02006834();
void Func_02006834_a();
void Func_0200683a();
void Func_02006840();
void Func_0200685c();
void Func_02006864();
void Func_02006870();
void Func_02006876();
void Func_020068a2();
void Func_020068a2_a();
void Func_020068b0();
void Func_020068b0_a();
void Func_020068be();
void Func_020068be_a();
void Func_020068c2();
void Func_020068d6();
void Func_020068dc();
void Func_020068dc_a();
void Func_020068e4();
void Func_020068e6();
void Func_020068f0();
void Func_020068f4();
void Func_02006902();
void Func_02006910();
void Func_02006918();
void Func_02006920();
void Func_02006926();
void Func_0200692e();
void Func_02006940();
void Func_02006946();
void Func_02006948();
void Func_02006958();
void Func_02006966();
void Func_02006974();
void Func_02006974_a();
void Func_02006978();
void Func_0200697a();
void Func_02006986();
void Func_02006992();
void Func_02006998();
void Func_020069a0();
void Func_020069a6();
void Func_020069a6_a();
void Func_020069b2();
void Func_020069b4();
void Func_020069ca();
void Func_020069cc();
void Func_020069d0();
void Func_020069e2();
void Func_020069e8();
void Func_020069f0();
void Func_020069fe();
void Func_02006a04();
void Func_02006a10();
void Func_02006a12();
void Func_02006a20();
void Func_02006a2e();
void Func_02006a32();
void Func_02006a3c();
void Func_02006a3e();
void Func_02006a48();
void Func_02006a54();
void Func_02006a58();
void Func_02006a62();
void Func_02006a64();
void Func_02006a76();
void Func_02006a7c();
void Func_02006a7e();
void Func_02006a80();
void Func_02006a8a();
s32 Func_02006a92();
void Func_02006a9c();
void Func_02006aac();
void Func_02006ab8();
void Func_02006ab8_a();
void Func_02006abc();
void Func_02006ac4();
void Func_02006aca();
void Func_02006ad0();
void Func_02006ae8();
void Func_02006aea();
void Func_02006b02();
void Func_02006b04();
void Func_02006b06();
void Func_02006b14();
void Func_02006b1e();
void Func_02006b26();
void Func_02006b28();
s32 Func_02006b32();
void Func_02006b44();
void Func_02006b52();
void Func_02006b54();
void Func_02006b62();
void Func_02006b6c();
void Func_02006b7a();
void Func_02006b88();
void Func_02006b92();
void Func_02006b9a();
void Func_02006b9a_a();
void Func_02006ba2();
void Func_02006ba8();
void Func_02006bb2();
void Func_02006bb2_a();
void Func_02006bb6();
void Func_02006bcc();
void Func_02006bd4();
void Func_02006bde();
void Func_02006be2();
void Func_02006be4();
void Func_02006bfa();
void Func_02006c00();
void Func_02006c04();
void Func_02006c0e();
void Func_02006c16();
void Func_02006c1c();
void Func_02006c1c_a();
void Func_02006c32();
void Func_02006c36();
void Func_02006c36_a();
void Func_02006c3e();
void Func_02006c52();
void Func_02006c52_a();
void Func_02006c60();
void Func_02006c6e();
void Func_02006c74();
void Func_02006c74_a();
void Func_02006c78();
void Func_02006c80();
void Func_02006c84();
void Func_02006c98();
void Func_02006ca4();
void Func_02006cb0();
void Func_02006cb8();
void Func_02006cca();
void Func_02006cd8();
void Func_02006ce6();
void Func_02006ce6_a();
void Func_02006cf4();
void Func_02006cfc();
void Func_02006d00();
void Func_02006d02();
void Func_02006d06();
void Func_02006d08();
void Func_02006d1e();
void Func_02006d22();
void Func_02006d30();
void Func_02006d38();
void Func_02006d38_a();
void Func_02006d3a();
void Func_02006d50();
void Func_02006d5e();
void Func_02006d66();
void Func_02006d72();
void Func_02006d7e();
void Func_02006d88();
void Func_02006d8a();
void Func_02006d9c();
void Func_02006da2();
void Func_02006da8();
void Func_02006db6();
void Func_02006dbe();
void Func_02006dc4();
void Func_02006dd8();
void Func_02006dda();
void Func_02006ddc();
void Func_02006de8();
void Func_02006df0();
void Func_02006df6();
void Func_02006dfa();
void Func_02006e02();
void Func_02006e0a();
s32 Func_02006e0c();
s32 Func_02006e16();
void Func_02006e20();
void Func_02006e28();
void Func_02006e2a();
void Func_02006e40();
s32 Func_02006e56();
s32 Func_02006e56_a();
void Func_02006e56_v();
void Func_02006e58();
void Func_02006e62();
void Func_02006e6e();
void Func_02006e7a();
void Func_02006e7e();
void Func_02006e7e_a();
void Func_02006e86();
void Func_02006e8c();
void Func_02006e8e();
void Func_02006e9a();
s32 Func_02006eac();
void Func_02006eb8();
void Func_02006ec6();
void Func_02006ecc();
void Func_02006ece();
void Func_02006eda();
void Func_02006ee8();
s32 Func_02006ef6();
void Func_02006f00();
void Func_02006f0e();
void Func_02006f16();
void Func_02006f18();
void Func_02006f1c();
void Func_02006f2a();
void Func_02006f38();
void Func_02006f48();
void Func_02006f50();
void Func_02006f5e();
void Func_02006f64();
void Func_02006f64_a();
void Func_02006f68();
void Func_02006f76();
void Func_02006f7e();
void Func_02006f8a();
void Func_02006fa4();
void Func_02006fb0();
void Func_02006fb2();
void Func_02006fb2_a();
void Func_02006fc0();
void Func_02006fca();
void Func_02006fcc();
void Func_02006fce();
void Func_02006fd2();
void Func_02006fdc();
void Func_02006fe6();
void Func_02006fea();
void Func_02006ff2();
void Func_02006ff2_a();
void Func_02006ff8();
void Func_02007002();
void Func_02007006();
void Func_0200700c();
void Func_02007024();
void Func_02007028();
void Func_02007032();
void Func_0200703c();
void Func_02007040();
void Func_02007048();
void Func_02007058();
void Func_02007060();
void Func_02007060_a();
void Func_02007066();
void Func_0200706a();
void Func_02007074();
void Func_02007074_a();
void Func_02007074_b();
void Func_0200707e();
void Func_02007082();
void Func_0200708a();
void Func_02007090();
void Func_0200709a();
void Func_020070a6();
void Func_020070b8();
void Func_020070bc();
void Func_020070be();
void Func_020070c2();
void Func_020070cc();
void Func_020070da();
void Func_020070e8();
void Func_020070ec();
void Func_020070ee();
void Func_020070f6();
void Func_020070f6_a();
void Func_02007100();
void Func_0200710a();
void Func_02007116();
void Func_0200711c();
void Func_0200712a();
void Func_0200712a_a();
void Func_02007132();
void Func_02007138();
void Func_02007142();
void Func_02007146();
void Func_0200714c();
void Func_02007156();
void Func_02007164();
void Func_0200716c();
void Func_0200716c_a();
void Func_02007172();
void Func_02007172_a();
void Func_02007180();
void Func_0200718e();
void Func_020071a0();
void Func_020071a2();
void Func_020071a4();
void Func_020071ae();
void Func_020071bc();
void Func_020071c2();
void Func_020071d6();
void Func_020071e2();
void Func_020071ee();
void Func_020071ee_a();
void Func_020071f4();
void Func_020071f6();
void Func_020071fe();
void Func_02007206();
void Func_0200720e();
void Func_02007210();
void Func_02007218();
void Func_02007222();
void Func_02007232();
void Func_02007238();
void Func_0200724c();
void Func_02007258();
void Func_02007264();
void Func_02007272();
void Func_02007272_a();
void Func_0200727e();
void Func_02007286();
void Func_02007288();
void Func_0200728a();
void Func_020072b4();
void Func_020072c0();
void Func_020072cc();
void Func_020072cc_a();
void Func_020072d8();
void Func_020072ea();
void Func_020072fa();
void Func_020072fc();
void Func_02007304();
void Func_02007306();
void Func_0200730e();
void Func_02007316();
void Func_0200731e();
void Func_0200732c();
void Func_0200733c();
void Func_02007346();
void Func_02007354();
void Func_02007360();
void Func_02007362();
void Func_02007362_a();
void Func_02007364();
void Func_0200736a();
void Func_02007374();
void Func_0200737a();
void Func_0200737e();
void Func_020073aa();
void Func_020073b2();
void Func_020073b2_a();
void Func_020073b8();
void Func_020073be();
void Func_020073c0();
void Func_020073ca();
void Func_020073d8();
void Func_020073de();
void Func_020073e6();
void Func_020073ec();
void Func_020073f4();
void Func_020073f6();
void Func_020073fa();
void Func_020073fe();
void Func_02007400();
void Func_02007408();
void Func_0200740c();
void Func_02007416();
void Func_02007418();
void Func_0200742c();
void Func_02007430();
void Func_0200743e();
void Func_02007444();
void Func_0200744c();
void Func_0200744c_a();
void Func_0200745a();
void Func_02007462();
void Func_02007464();
void Func_02007468();
void Func_0200747a();
void Func_0200747c();
void Func_02007482();
void Func_02007488();
void Func_0200748e();
void Func_02007492();
void Func_02007496();
void Func_0200749c();
void Func_020074b0();
void Func_020074be();
void Func_020074c4();
void Func_020074c8();
void Func_020074cc();
void Func_020074da();
void Func_020074e0();
void Func_020074e8();
void Func_020074f2();
void Func_020074f6();
void Func_020074fa();
void Func_020074fc();
void Func_02007502();
void Func_02007504();
void Func_02007512();
void Func_02007516();
void Func_02007532();
void Func_02007548();
void Func_0200754c();
void Func_0200754e();
void Func_02007554();
void Func_02007560();
void Func_02007562();
void Func_02007564();
void Func_02007570();
void Func_0200757a();
void Func_02007580();
void Func_02007582();
void Func_02007596();
void Func_0200759e();
void Func_020075ac();
void Func_020075b2();
void Func_020075ba();
void Func_020075c6();
void Func_020075ce();
void Func_020075d0();
void Func_020075d4();
void Func_020075ec();
void Func_020075fa();
void Func_020075fc();
void Func_02007604();
void Func_02007608();
void Func_02007608_a();
void Func_02007614();
void Func_02007616();
void Func_02007620();
void Func_02007624();
void Func_02007632();
void Func_02007636();
void Func_0200763a();
void Func_02007644();
void Func_02007652();
void Func_0200765a();
void Func_02007664();
void Func_0200766a();
void Func_0200766e();
void Func_02007678();
void Func_02007678_a();
void Func_02007684();
void Func_02007684_a();
void Func_0200768a();
void Func_02007696();
void Func_02007698();
void Func_0200769e();
void Func_020076a6();
void Func_020076b0();
void Func_020076b6();
void Func_020076b8();
void Func_020076ce();
void Func_020076d2();
void Func_020076d4();
void Func_020076dc();
void Func_020076e2();
void Func_020076ee();
void Func_020076f0();
void Func_020076fe();
void Func_02007702();
void Func_02007716();
void Func_02007718();
void Func_0200771c();
void Func_02007734();
void Func_0200773c();
void Func_0200773e();
void Func_02007744();
void Func_0200775c();
void Func_0200775c_a();
void Func_02007762();
void Func_02007766();
void Func_0200777a();
void Func_02007782();
void Func_02007788();
void Func_02007792();
void Func_02007794();
void Func_020077a0();
s32 Func_020077a2();
void Func_020077a8();
void Func_020077b0();
void Func_020077b8();
void Func_020077c8();
void Func_020077c8_a();
void Func_020077ca();
void Func_020077d0();
s32 Func_020077de();
void Func_020077e2();
void Func_020077ea();
void Func_020077ec();
void Func_02007804();
void Func_02007812();
void Func_02007820();
void Func_02007826();
void Func_0200782e();
void Func_0200784e();
void Func_02007852();
void Func_0200785a();
void Func_0200785c();
void Func_02007864();
void Func_02007868();
void Func_0200786a();
void Func_0200786c();
void Func_02007874();
s32 Func_02007890();
s32 Func_02007890_a();
s32 Func_0200789c();
s32 Func_0200789c_a();
s32 Func_020078a6();
void Func_020078b0();
void Func_020078b6();
void Func_020078ce();
void Func_020078ce_a();
s32 Func_020078d6();
void Func_020078dc();
void Func_020078e6();
void Func_020078ea();
void Func_020078ea_a();
void Func_020078f8();
void Func_02007906();
s32 Func_0200790a();
s32 Func_02007914();
s32 Func_02007914_a();
s32 Func_0200791e();
void Func_02007924();
s32 Func_02007928();
void Func_02007932();
void Func_02007934();
s32 Func_02007946();
void Func_0200794a();
void Func_02007956();
void Func_02007958();
void Func_02007968();
void Func_0200797e();
void Func_02007982();
void Func_02007988();
void Func_02007996();
void Func_02007998();
void Func_020079b4();
void Func_020079b4_a();
void Func_020079b6();
void Func_020079d0();
void Func_020079d4();
void Func_020079dc();
void Func_020079ea();
void Func_020079ec();
void Func_020079f6();
void Func_020079f8();
void Func_020079fc();
void Func_02007a06();
void Func_02007a1a();
void Func_02007a22();
void Func_02007a2e();
void Func_02007a3a();
void Func_02007a40();
void Func_02007a4e();
void Func_02007a4e_a();
void Func_02007a5a();
void Func_02007a5c();
void Func_02007a60();
void Func_02007a6a();
void Func_02007a76();
void Func_02007a7a();
void Func_02007a82();
void Func_02007a84();
void Func_02007a86();
s32 Func_02007a9a();
void Func_02007aa2();
void Func_02007aae();
void Func_02007ab4();
void Func_02007ab8();
void Func_02007ac8();
void Func_02007ad0();
void Func_02007ad6();
void Func_02007ad8();
void Func_02007ae4();
void Func_02007af2();
void Func_02007af4();
void Func_02007b00();
void Func_02007b0c();
void Func_02007b0c_a();
void Func_02007b12();
void Func_02007b26();
void Func_02007b32();
void Func_02007b38();
s32 Func_02007b3a();
void Func_02007b52();
void Func_02007b62();
void Func_02007b68();
void Func_02007b70();
void Func_02007b7c();
void Func_02007b86();
void Func_02007b94();
void Func_02007b9c();
void Func_02007ba0();
void Func_02007ba2();
void Func_02007ba4();
void Func_02007bac();
void Func_02007bb0();
void Func_02007bbc();
void Func_02007bbc_a();
void Func_02007bca();
void Func_02007bd6();
void Func_02007bd8();
void Func_02007bde();
void Func_02007be6();
void Func_02007c0a();
void Func_02007c0c();
void Func_02007c18();
void Func_02007c1c();
void Func_02007c1e();
void Func_02007c24();
void Func_02007c36();
void Func_02007c46();
void Func_02007c4e();
void Func_02007c52();
void Func_02007c62();
void Func_02007c68();
void Func_02007c6c();
void Func_02007c7a();
void Func_02007c82();
void Func_02007c88();
void Func_02007c90();
void Func_02007c94();
void Func_02007c9c();
void Func_02007caa();
void Func_02007cb0();
void Func_02007cd0();
void Func_02007cd0_a();
void Func_02007cda();
void Func_02007cdc();
void Func_02007ce2();
void Func_02007ce4();
s32 Func_02007ce6();
void Func_02007d04();
void Func_02007d10();
void Func_02007d1a();
void Func_02007d1c();
void Func_02007d28();
void Func_02007d2e();
void Func_02007d34();
void Func_02007d3c();
void Func_02007d4a();
void Func_02007d4c();
void Func_02007d54();
void Func_02007d62();
void Func_02007d70();
void Func_02007d74();
void Func_02007d74_a();
s32 Func_02007d86();
void Func_02007d88();
void Func_02007d96();
void Func_02007da6();
void Func_02007db4();
void Func_02007db4_a();
void Func_02007dbc();
void Func_02007dce();
void Func_02007ddc();
void Func_02007dde();
void Func_02007dea();
void Func_02007dec();
void Func_02007df0();
void Func_02007df8();
void Func_02007df8_a();
void Func_02007e04();
void Func_02007e06();
void Func_02007e14();
void Func_02007e14_a();
void Func_02007e22();
void Func_02007e2a();
void Func_02007e34();
void Func_02007e3a();
void Func_02007e52();
void Func_02007e74();
void Func_02007e78();
void Func_02007e7e();
void Func_02007e7e_a();
void Func_02007e88();
void Func_02007e92();
void Func_02007e96();
void Func_02007e98();
void Func_02007e9a();
void Func_02007e9e();
void Func_02007ea0();
void Func_02007ea4();
void Func_02007eac();
void Func_02007eb4();
void Func_02007ebe();
void Func_02007ed0();
void Func_02007eee();
void Func_02007f06();
void Func_02007f2e();
void Func_02007f2e_a();
void Func_02007f4e();
void Func_02007f54();
void Func_02007f5a();
void Func_02007f60();
void Func_02007f60_a();
void Func_02007f62();
void Func_02007f78();
void Func_02007f88();
void Func_02007f98();
void Func_02007f9c();
void Func_02007f9c_a();
void Func_02007fa4();
void Func_02007fa6();
void Func_02007fac();
void Func_02007fae();
void Func_02007fb4();
void Func_02007fb8();
void Func_02007fba();
void Func_02007fc4();
void Func_02007fdc();
void Func_02007fe8();
void Func_02007fec();
void Func_02007ff4();
void Func_02007ff6();
void Func_02008000();
void Func_02008006();
void Func_02008008();
void Func_02008010();
void Func_0200801c();
void Func_02008022();
void Func_02008022_a();
void Func_02008028();
void Func_0200802e();
void Func_02008036();
void Func_0200806a();
void Func_02008070();
void Func_02008070_a();
void Func_02008076();
void Func_02008080();
void Func_0200808a();
void Func_0200808a_a();
s16 *Func_02008090();
void Func_02008096();
void Func_0200809e();
void Func_020080a0();
void Func_020080a6();
void Func_020080a8();
void Func_020080ae();
void Func_020080b0();
void Func_020080b8();
s16 *Func_020080c0();
s16 *Func_020080c0_a();
void Func_020080c2();
void Func_020080ca();
void Func_020080ce();
void Func_020080da();
s32 Func_020080ea();
s16 *Func_020080f0();
void Func_020080f8();
void Func_020080fa();
s32 Func_02008108();
void Func_0200810a();
s32 Func_0200811a();
s32 Func_0200811a_a();
void Func_02008120();
void Func_02008126();
void Func_02008128();
void Func_0200812a();
void Func_0200813a();
s32 Func_0200814a();
void Func_02008170();
s32 Func_02008182();
void Func_020081ac();
s32 Func_0200824c();

/* The scene step counter, advanced on every arm of every choice below. */
static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ __attribute__((always_inline)) void bump_step(s32 amount)
{
    u8 *scene = *(u8 **)0x03001ebc;

    *(u16 *)(scene + 0x1d8) = (u16)(*(u16 *)(scene + 0x1d8) + amount);
}
static __inline__ __attribute__((always_inline)) void bump_step_at(
    u8 **scene_address, s32 amount)
{
    u8 *scene = *scene_address;

    *(u16 *)(scene + 0x1d8) = (u16)(*(u16 *)(scene + 0x1d8) + amount);
}

/* Three identical tails: an optional record whose signed halfwords at +10 and
 * +18 are forwarded to a per-channel import. */
static __inline__ __attribute__((always_inline)) void follow_record(
    s16 *record, s32 channel, void (*apply)())
{
    if (record != 0)
        apply(channel, record[5], record[9]);
}

/* Loader-relocated ROM calls: each site names the pre-relocation call word the image holds. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

void Scene_RunMainCutsceneSequence(void)
{
    s32 carry;
    s16 *position;

    carry = Func_02006562(30);
    Func_020063fe(carry);
    carry = Value1(Func_020064bc, 0x22c4);
    carry = Func_02006518(carry);
    Func_02006524(carry);
    carry = Value4(Func_0200650e, 0xd80000, -1, 0x2e00000, 1);
    Func_0200651a(carry);
    Func_02006420(20);
    Func_02006426(10);
    Func_020064fe(20, 0);
    Func_02006434(10);
    Call3(Func_0200646e, 14, 0xcccc, 0x6666);
    Func_020065a0(14, 0, 16);
    Func_0200644e(10);
    Func_020064e6(14, 3);
    Func_0200645c(30);
    Func_02006534(14, 0);
    Func_0200646a(20);
    Func_02006542(20, 0);
    carry = Func_0200656a(0, 1);
    Func_0200657e(carry);
    Func_02006484(40);
    Call3(Func_020064c2, 0, 0x10000, 0x8000);
    Value3(Func_020064fe_a, 0, 208, 0x2f8);
    Func_020064a4(10);
    Call4(Func_020065ae, 0xd80000, -1, 0x2e00000, 1);
    Call4(Func_0200660e, 1, -16, 16, 0xc000);
    Call4(Func_0200661c, 3, 0, 24, 0xc000);
    Value4(Func_0200662a, 2, 16, 16, 0xc000);
    Func_02006560(1);
    Func_020064ee(30);
    Func_020064f4(10);
    Func_0200658c(14, 3);
    Func_02006502(30);
    Func_020065da(14, 0);

    if (Value1(Func_020064d8, 0x951) != 0) {
        Func_0200651c(10);
        Func_020065b4(20, 3);
        Func_0200652a(20);
        Func_02006602(20, 0);
        Func_02006538(10);
        Func_0200661c_a(14, 0xa000, 0);
        Func_0200654a(30);
        Func_020065e2(14, 3);
        Func_02006558(20);
        Func_02006630(14, 0);
        Func_02006566(10);
        Func_020065fe(20, 3);
        Func_02006574(20);
        Func_0200664c(20, 0);
        Call3(Func_02006660, 20, 0xc000, 0);
        Func_0200658e(20);
        Call3(Func_020065cc, 20, 0x10000, 0x8000);
        Value3(Func_02006700, 20, 0, -16);
        Func_020065ae_a(40);
        Func_02006646(20, 3);
        Func_020065bc(40);
        Value3(Func_020066a0, 20, 0x4000, 0);
        Func_020065ce(20);
        Func_02006730(20, 0, 32);
        Func_020066bc(14, 0x8000, 0);
        Func_020065ea(10);
        Func_020066cc(20, 0, 0);
        Func_020065fa(30);
        Func_02006692(20, 3);
        Func_02006608(30);
        Value3(Func_020066ec, 20, 0x4000, 0);
        Func_0200661a(20);
        Value3(Func_02006654, 20, 0xcccc, 0x6666);
        Func_02006652(20)[90] &= 0xfe;
        Value3(Func_02006798, 20, 0, -16);
        {
            u8 *record = Func_0200666e(20);
            u8 value = *(volatile u8 *)&record[90];

            record[90] = (u8)(value | 1);
        }
        Value3(Func_02006734, 14, 0x4000, 0);
        Func_02006662(40);
        Call3(Func_0200669c, 14, 0xcccc, 0x6666);
        Func_020067ce(14, 0, 16);
        Func_0200667c(40);
        Func_02006634(164, 2);
        Func_0200675e(-1, 0);
        Func_020067be(164, 3);
        Func_020066b6(164, 0);
        Func_02006782(0, 0xc000, 0);
        Func_020066b0(30);
        Call3(Func_02006814, 14, 0, -16);
        Func_020067a0(14, 0x4000, 0);
        Func_020066ce(30);
        Func_0200677e(14, 2);
        Func_020066dc(20);
        Func_020067ac(14, 0);
        bump_step(2);
    } else {
        bump_step(5);
        Func_0200670c(10);
        Func_020067a4(20, 3);
        Func_0200671a(20);
        Func_020067f2(20, 0);
        Func_02006806(14, 0xa000, 0);
        Func_02006734_a(40);
        Func_020067cc(14, 3);
        Func_02006742(20);
        Func_020067da(20, 3);
        Func_02006750(30);
        Func_02006834(14, 0x4000, 0);
        Func_02006762(30);
        Func_02006812(14, 2);
        Func_02006770(20);
        Func_02006840(14, 0);
    }

    if (Func_020067a0_a(0, 0) == 0) {
        Func_0200678a(20);
        Func_02006822(20, 3);
        Func_02006798_a(20);
        Func_02006870(20, 0);
        bump_step(1);
    } else {
        Func_020067cc_a(10);
        Func_02006864(20, 3);
        Func_020067da_a(20);
        bump_step(1);
        Func_020068c2(20, 0);
    }

    Func_020067f8(10);
    Func_020068dc(14, 0xa000, 0);
    Func_0200680a(20);
    Func_020068a2(14, 3);
    Func_02006818(30);
    Func_020068f0(14, 0);
    Func_02006826(10);
    Func_020068be(20, 3);
    Func_02006834_a(30);
    Func_0200683a(10);
    Func_020068dc_a(1, 4, 13);
    Func_020068e6(1, 4, 30);
    Func_02006926(1, 0);
    Func_0200685c(10);
    Call3(Func_02006940, 14, 0x4000, 0);
    Func_02006918(20, 2);
    Func_02006876(20);
    Call3(Func_020068b0, 14, 0x19999, 0xcccc);
    Func_020069e2(20, 0, 16);
    Func_020067f8_a(2);
    Func_02006974(20, 0x2000, 0);
    Func_020068a2_a(10);
    Func_0200697a(20, 0);
    Func_020068b0_a(10);
    Func_02006948(14, 4);
    Func_020068be_a(30);
    Func_02006978(14, 20, 30);
    Func_020069a0(14, 0);
    Func_020068d6(10);
    Func_02006986(20, 2);
    Func_020068e4(20);
    Func_020069a6(14, 1, 0);
    Func_020068f4(40);
    Func_020069cc(14, 0);
    Func_02006902(10);
    Func_020069b2(1, 2);
    Func_02006910(30);
    Func_020069ca(1, 2, 30);
    Func_02006920(10);
    Func_020069d0(2, 2);
    Func_0200692e(20);
    Func_020069e8(2, 1, 30);
    Func_02006a10(2, 0);
    Func_02006946(10);
    Func_02006a32(1, 0x106, 50);
    Func_02006958(10);
    Func_020069f0(14, 3);
    Func_02006966(30);
    Func_02006a3e(14, 0);
    Func_02006974_a(10);
    Call3(Func_02006a58, 1, 0xc000, 0);
    Value3(Func_02006a64, 2, 0xc000, 0);
    Func_02006992(30);
    Func_02006998(10);
    Func_02006a48(20, 2);
    Func_020069a6_a(20);
    Func_02006a7e(20, 0);
    Func_020069b4(10);
    Func_02006a76(1, 0, 0);
    Func_02006a80(3, 2, 50);
    Call3(Func_02006aac, 0, 0xc000, 0);
    Call3(Func_02006ab8, 1, 0xc000, 0);
    Call3(Func_02006ac4, 3, 0xc000, 0);
    Func_02006ad0(2, 0xc000, 0);
    Func_020069fe(30);
    Func_02006a04(10);
    Func_02006a9c(20, 4);
    Func_02006a12(20);
    Func_02006aea(20, 0);
    Func_02006a20(10);
    Func_02006ab8_a(14, 4);
    Func_02006a2e(20);
    Func_02006b06(14, 0);
    Func_02006a3c(10);
    Call3(Func_02006b26, 2, 0x101, 40);
    Func_02006b1e(2, 0);
    Func_02006a54(10);
    Func_02006b04(14, 2);
    Func_02006a62(20);
    Func_02006b32(14, 0);
    if (Func_02006a92(0, 0) == 0) {
        Func_02006a7c(20);
        Func_02006b14(14, 3);
        Func_02006a8a(30);
        Func_02006b62(14, 0);
        bump_step(1);
    } else {
        Func_02006abc(10);
        Func_02006b54(14, 4);
        Func_02006aca(20);
        bump_step(1);
        Func_02006bb2(14, 0);
    }

    Func_02006ae8(10);
    Call3(Func_02006bd4, 20, 0x102, 40);
    Func_02006bcc(20, 0);
    Func_02006b02(20);
    Func_02006b92(0, 3);
    Func_02006b9a(1, 3);
    Func_02006ba2(3, 3);
    Func_02006bb2_a(2, 3);
    Func_02006b28(50);
    Func_02006be2(14, 20, 60);
    Func_02006c16(14, 0x4000, 0);
    Func_02006b44(20);
    Func_02006c1c(14, 0);
    Func_02006b52(10);
    Call3(Func_02006c3e, 20, 0x102, 40);
    Func_02006c36(20, 0);
    Func_02006b6c(10);
    Func_02006c04(20, 4);
    Func_02006b7a(20);
    Func_02006c52(20, 0);
    Func_02006b88(10);
    Value3(Func_02006c74, 0, 0x102, 40);
    Func_02006b9a_a(10);
    Func_02006c32(14, 4);
    Func_02006ba8(20);
    Func_02006c80(14, 0);
    Func_02006bb6(10);
    Func_02006c78(1, 0, 50);
    Call3(Func_02006ca4, 0, 0xc000, 0);
    Value3(Func_02006cb0, 1, 0xc000, 0);
    Func_02006bde(30);
    Func_02006be4(10);
    Func_02006c74_a(0, 3);
    Func_02006c84(1, 3);
    Func_02006bfa(30);
    Func_02006c00(10);
    Func_02006c98(14, 3);
    Func_02006c0e(30);
    Func_02006ce6(14, 0);
    Func_02006c1c_a(10);
    Call3(Func_02006d08, 3, 0x100, 40);
    Func_02006d00(3, 0);
    Func_02006c36_a(10);
    Call3(Func_02006d22, 14, 0x102, 50);
    Func_02006cfc(20, 14, 50);
    Func_02006c52_a(10);
    Func_02006d02(14, 2);
    Func_02006c60(20);
    Func_02006d38(14, 0);
    Func_02006c6e(20);
    Func_02006d30(1, 0, 0);
    Func_02006d3a(3, 2, 50);
    Call3(Func_02006d66, 0, 0xc000, 0);
    Call3(Func_02006d72, 1, 0xc000, 0);
    Call3(Func_02006d7e, 3, 0xc000, 0);
    Value3(Func_02006d8a, 2, 0xc000, 0);
    Func_02006cb8(50);
    Func_02006d9c(20, 0x2000, 0);
    Func_02006cca(20);
    Func_02006da2(20, 0);
    Func_02006cd8(10);
    Func_02006d88(1, 2);
    Func_02006ce6_a(20);
    Func_02006dbe(1, 0);
    Func_02006cf4(20);
    Func_02006dd8(14, 0x8000, 0);
    Func_02006d06(40);
    Call3(Func_02006df0, 14, 0x105, 60);
    Func_02006de8(14, 0);
    Func_02006d1e(10);
    Call3(Func_02006e0a, 2, 0x100, 40);
    Func_02006e02(2, 0);
    Func_02006d38_a(10);
    Func_02006dfa(14, 2, 40);
    Func_02006dda(14, 3);
    Func_02006d50(30);
    Func_02006e28(14, 0);
    Func_02006d5e(10);
    Func_02006e20(1, 0, 0);
    Func_02006e2a(3, 2, 60);
    Call3(Func_02006e56, 0, 0xc000, 0);
    Call3(Func_02006e62, 1, 0xc000, 0);
    Call3(Func_02006e6e, 3, 0xc000, 0);
    Func_02006e7a(2, 0xc000, 0);
    Func_02006da8(50);
    Func_02006e58(20, 2);
    Func_02006db6(20);
    Func_02006e8e(20, 0);
    Func_02006dc4(10);
    Func_02006e86(1, 0, 40);
    Func_02006e7e(1, 2);
    Func_02006ddc(20);
    Func_02006eac(1, 0);
    if (Func_02006e0c(0, 0) == 0) {
        Func_02006df6(20);
        Func_02006ece(1, 0);
        bump_step(3);
    } else {
        do {
            Func_02006e16(10);
        } while (0);
        {
            u8 **scene_address = (u8 **)0x03001ebc;

            bump_step_at(scene_address, 1);
            Func_02006ef6(1, 0);
            if (Func_02006e56_a(0, 0) == 0) {
                Func_02006e40(20);
                Func_02006f18(1, 0);
                bump_step_at(scene_address, 1);
            } else {
                bump_step_at(scene_address, 1);
                Func_02006f48(1, 0);
            }
        }
    }

    Func_02006e7e_a(10);
    Func_02006f16(14, 4);
    Func_02006e8c(20);
    Func_02006f64(14, 0);
    Func_02006e9a(10);
    Call3(Func_02006f7e, 0, 0xc000, 0);
    Value3(Func_02006f8a, 1, 0xc000, 0);
    Func_02006eb8(40);
    Func_02006f68(1, 2);
    Func_02006ec6(20);
    Func_02006ecc(10);
    Func_02006f64_a(14, 3);
    Func_02006eda(30);
    Func_02006fb2(14, 0);
    Func_02006ee8(10);
    Call3(Func_02006fd2, 3, 0x101, 40);
    Func_02006fca(3, 0);
    Func_02006f00(10);
    Func_02006fb0(14, 2);
    Func_02006f0e(20);
    Func_02006fe6(14, 0);
    Func_02006f1c(10);
    Func_02006fcc(2, 2);
    Func_02006f2a(20);
    Func_02007002(2, 0);
    Func_02006f38(10);
    Func_02006ff2(20, 14, 40);
    Func_02006ff2_a(20, 2);
    Func_02006f50(20);
    Func_02007028(20, 0);
    Func_02006f5e(20);
    Call3(Func_02007048, 14, 0x105, 70);
    Func_02007040(14, 0);
    Func_02006f76(10);
    Call3(Func_02007060, 0, 0x101, 0);
    Call3(Func_0200706a, 1, 0x101, 0);
    Call3(Func_02007074, 3, 0x101, 0);
    Value3(Func_0200707e, 2, 0x101, 40);
    Func_02006fa4(10);
    Func_0200703c(14, 4);
    Func_02006fb2_a(20);
    Func_0200708a(14, 0);
    Func_02006fc0(10);
    Func_02007058(14, 3);
    Func_02006fce(30);
    Func_020070a6(14, 0);
    Func_02006fdc(20);
    Func_02007074_a(1, 3);
    Func_02006fea(30);
    Func_020070c2(1, 0);
    Func_02006ff8(20);
    Func_02007090(14, 3);
    Func_02007006(30);
    Func_0200700c(10);
    Call3(Func_020070f6, 2, 0x101, 40);
    Func_020070ee(2, 0);
    Func_02007024(10);
    Func_020070bc(14, 3);
    Func_02007032(40);
    Func_020070ec(0, 2, 0);
    Func_020070f6_a(1, 2, 0);
    Func_02007100(3, 2, 0);
    Func_0200710a(20, 2, 0);
    Func_02007060_a(50);
    Func_02007066(10);
    Func_02007116(2, 2);
    Func_02007074_b(20);
    Func_0200714c(2, 0);
    Func_02007082(10);
    Call3(Func_0200716c, 14, 0x101, 40);
    Func_02007164(14, 0);
    Func_0200709a(10);
    Func_0200712a(0, 3);
    Func_02007132(1, 3);
    Func_02007142(3, 3);
    Func_020070b8(30);
    Func_020070be(10);
    Func_02007156(2, 4);
    Func_020070cc(20);
    Func_020071a4(2, 0);
    Func_020070da(20);
    Func_02007172(20, 3);
    Func_020070e8(30);
    Func_020071a2(20, 14, 30);
    Call3(Func_020071d6, 0, 0xc000, 0);
    Call3(Func_020071e2, 1, 0xc000, 0);
    Value3(Func_020071ee, 3, 0xc000, 0);
    Func_0200711c(20);
    Func_020071f4(20, 0);
    Func_0200712a_a(10);
    Func_020071c2(14, 3);
    Func_02007138(30);
    Func_02007210(14, 0);
    Func_02007146(10);
    Func_020071ee_a(0, 2);
    Func_020071f6(1, 2);
    Func_020071fe(3, 2);
    Func_0200720e(2, 2);
    Func_0200716c_a(30);
    Func_02007172_a(10);
    Func_02007222(20, 2);
    Func_02007180(20);
    Func_02007258(20, 0);
    Func_0200718e(10);
    Value3(Func_02007272, 14, 0x8000, 0);
    Func_020071a0(20);
    Func_02007238(14, 4);
    Func_020071ae(20);
    Func_02007286(14, 0);
    Func_020071bc(10);
    Func_0200727e(1, 0, 0);
    Func_02007288(3, 2, 50);
    Call3(Func_020072b4, 0, 0xc000, 0);
    Call3(Func_020072c0, 1, 0xc000, 0);
    Call3(Func_020072cc, 3, 0xc000, 0);
    Value3(Func_020072d8, 2, 0xc000, 0);
    Func_02007206(40);
    Value3(Func_020072ea, 14, 0x4000, 0);
    Func_02007218(30);
    Call3(Func_02007304, 14, 0x102, 40);
    Func_020072fc(14, 0);
    Func_02007232(10);
    Call3(Func_0200731e, 20, 0x102, 40);
    Func_02007316(20, 0);
    Func_0200724c(20);
    Func_0200730e(14, 20, 40);
    Func_02007306(14, 2);
    Func_02007264(20);
    Func_0200733c(14, 0);
    Func_02007272_a(10);
    Call2(Func_02007364, 20, 0x102);
    Func_0200732c(20, 2);
    Func_0200728a(20);
    Func_02007362(20, 0);
    Call3(Func_020072cc_a, 20, 0x19999, 0xcccc);
    Func_020073fe(20, 0, 24);
    Func_02007360(0, 20, 0);
    Func_0200736a(1, 20, 0);
    Func_02007374(3, 20, 0);
    Func_0200737e(2, 20, 0);
    Call3(Func_020073b2, 14, 0x4000, 0);
    Func_020073b2_a(14, 0);
    Func_020072fa(10);
    Call3(Func_020073e6, 20, 0x100, 60);
    Func_020073c0(20, 14, 0);
    Call3(Func_020073f4, 0, 0xc000, 0);
    Call3(Func_02007400, 1, 0xc000, 0);
    Call3(Func_0200740c, 3, 0xc000, 0);
    Value3(Func_02007418, 2, 0xc000, 0);
    Func_02007346(10);
    Func_020073f6(20, 2);
    Func_02007354(20);
    Func_0200742c(20, 0);
    Func_02007362_a(30);
    Call3(Func_0200744c, 14, 0x105, 40);
    Func_02007444(14, 0);
    Func_0200737a(10);
    Call3(Func_02007464, 20, 0x101, 60);
    Call3(Func_020073be, 20, 0x13333, 0x9999);
    Call3(Func_020074f2, 20, 0, -24);
    Func_0200747c(20, 0, 0);
    Func_020073aa(20);
    Func_02007482(20, 0);
    Func_020073b8(10);
    Value3(Func_0200749c, 14, 0x8000, 0);
    Func_020073ca(20);
    Func_02007462(14, 3);
    Func_020073d8(30);
    Func_020073de(10);
    Func_0200748e(20, 2);
    Func_020073ec(20);
    Func_020074c4(20, 0);
    Func_020073fa(10);
    Func_02007492(14, 4);
    Func_02007408(20);
    Func_020074e0(14, 0);
    Func_02007416(10);
    Call3(Func_02007502, 20, 0x102, 40);
    Func_020074fa(20, 0);
    Func_02007430(10);
    Func_020074c8(14, 3);
    Func_0200743e(30);
    Func_02007516(14, 0);
    Func_0200744c_a(10);
    Func_020074fc(1, 2);
    Func_0200745a(20);
    Func_02007532(1, 0);
    Func_02007468(10);
    Value3(Func_0200754c, 14, 0x4000, 0);
    Func_0200747a(30);
    Func_02007512(14, 3);
    Func_02007488(30);
    Func_02007560(14, 0);
    Func_02007496(10);
    Call3(Func_02007582, 3, 0x102, 40);
    Func_0200757a(3, 0);
    Func_020074b0(10);
    Func_02007548(14, 3);
    Func_020074be(30);
    Func_02007596(14, 0);
    Func_020074cc(10);
    Func_02007564(2, 3);
    Func_020074da(30);
    Func_020075b2(2, 0);
    Func_020074e8(10);
    Func_02007580(14, 3);
    Func_020074f6(30);
    Func_020075ce(14, 0);
    Func_02007504(10);
    Func_020075c6(1, 0, 0);
    Func_020075d0(3, 2, 50);
    Call3(Func_020075fc, 0, 0xc000, 0);
    Call3(Func_02007608, 1, 0xc000, 0);
    Call3(Func_02007614, 3, 0xc000, 0);
    Value3(Func_02007620, 2, 0xc000, 0);
    Func_0200754e(30);
    Func_02007554(10);
    Func_02007604(14, 2);
    Func_02007562(20);
    Func_0200763a(14, 0);
    Func_02007570(20);
    Call3(Func_0200765a, 0, 0x101, 0);
    Call3(Func_02007664, 1, 0x101, 0);
    Call3(Func_0200766e, 3, 0x101, 0);
    Call3(Func_02007678, 2, 0x101, 50);
    Func_0200759e(10);
    Func_02007636(14, 3);
    Func_020075ac(30);
    Func_02007684(14, 0);
    Func_020075ba(10);
    Call3(Func_020076a6, 20, 0x102, 40);
    Func_0200769e(20, 0);
    Func_020075d4(10);
    Func_02007696(14, 20, 30);
    Func_020076b6(14, 0);
    Func_020075ec(10);
    Func_02007684_a(14, 4);
    Func_020075fa(20);
    Func_020076d2(14, 0);
    Func_02007608_a(10);
    Func_020076b8(1, 2);
    Func_02007616(20);
    Func_020076ee(1, 0);
    Func_02007624(10);
    Func_020076d4(14, 2);
    Func_02007632(20);
    Value3(Func_02007716, 14, 0x4000, 0);
    Func_02007644(20);
    Func_0200771c(14, 0);
    Func_02007652(10);
    Call3(Func_0200773c, 20, 0x101, 40);
    Func_02007734(20, 0);
    Func_0200766a(10);
    Func_02007702(14, 3);
    Func_02007678_a(30);
    Func_0200775c(14, 0x8000, 0);
    Func_0200768a(20);
    Func_02007762(14, 0);
    Func_02007698(10);
    Call3(Func_02007782, 2, 0x101, 40);
    Func_0200777a(2, 0);
    Func_020076b0(10);
    Call3(Func_02007794, 14, 0x4000, 0);
    Func_020077a0(20, 0x2000, 0);
    Func_020076ce(30);
    Func_02007766(14, 3);
    Func_020076dc(30);
    Func_020076e2(10);
    Func_02007792(3, 2);
    Func_020076f0(20);
    Func_020077c8(3, 0);
    Func_020076fe(10);
    Call3(Func_020077ea, 14, 0x102, 40);
    Func_020077e2(14, 0);
    Func_02007718(20);
    Func_020077a8(0, 3);
    Func_020077b0(1, 3);
    Func_020077b8(3, 3);
    Func_020077c8_a(2, 3);
    Func_0200773e(30);
    Func_02007744(20);
    Call3(Func_0200782e, 14, 0x105, 60);
    Func_02007826(14, 0);
    Func_0200775c_a(10);
    Call3(Func_0200785a, 1, 0x101, 40);
    Func_02007852(1, 0);
    Func_02007788(10);
    Call3(Func_02007874, 14, 0x100, 40);
    Func_0200786c(14, 0);
    Func_020077a2(10);
    Call3(Func_02007864, 1, 0, 50);
    Call3(Func_02007890, 0, 0xc000, 0);
    Value3(Func_0200789c, 1, 0xc000, 0);
    Func_020077ca(30);
    Func_020077d0(10);
    Func_02007868(14, 3);
    Func_020077de(30);
    Func_020078b6(14, 0);
    Func_020077ec(10);
    Call3(Func_020078d6, 3, 0x101, 40);
    Func_020078ce(3, 0);
    Func_02007804(10);
    Func_0200789c_a(14, 4);
    Func_02007812(20);
    Func_020078ea(14, 0);
    Func_02007820(10);
    Call3(Func_0200790a, 0, 0x101, 0);
    Call3(Func_02007914, 1, 0x101, 0);
    Call3(Func_0200791e, 2, 0x101, 0);
    Value3(Func_02007928, 3, 0x101, 40);
    Func_0200784e(10);
    Func_020078e6(14, 4);
    Func_0200785c(20);
    Func_02007934(14, 0);
    Func_0200786a(10);
    Call3(Func_02007956, 2, 0x100, 40);
    Func_02007946(2, 0);
    if (Func_020078a6(14, 0) == 0) {
        Func_02007890_a(10);
        Func_02007968(2, 0);
        bump_step(1);
    } else {
        Func_020078b0(10);
        bump_step(1);
        Func_02007998(2, 0);
    }

    Func_020078ce_a(10);
    Func_0200797e(14, 2);
    Func_020078dc(20);
    Func_020079b4(14, 0);
    Func_020078ea_a(10);
    Func_02007982(14, 4);
    Func_020078f8(20);
    Func_020079d0(14, 0);
    Func_02007906(10);
    Func_020079b6(20, 2);
    Func_02007914_a(20);
    Func_020079f6(20, 0, 0);
    Func_02007924(20);
    Func_020079fc(20, 0);
    Func_02007932(10);
    Func_020079ec(14, 20, 40);
    Func_020079d4(14, 4);
    Func_0200794a(30);
    Func_02007a22(14, 0);
    Func_02007958(10);
    Func_02007a1a(1, 0, 40);
    Func_02007a3a(1, 0);
    Call3(Func_02007a4e, 14, 0x4000, 0);
    Value3(Func_02007a5a, 20, 0x2000, 0);
    Func_02007988(30);
    Func_02007a60(14, 0);
    Func_02007996(10);
    Call3(Func_02007a7a, 0, 0xc000, 0);
    Value3(Func_02007a86, 1, 0xc000, 0);
    Func_020079b4_a(30);
    Func_02007a76(3, 2, 60);
    Call3(Func_02007aa2, 3, 0xc000, 0);
    Func_02007aae(2, 0xc000, 0);
    Func_020079dc(30);
    Func_02007ab4(3, 0);
    Func_020079ea(10);
    Func_02007a82(2, 4);
    Func_020079f8(20);
    Func_02007ad0(2, 0);
    Func_02007a06(10);
    Func_02007ac8(20, 14, 60);
    Call3(Func_02007af4, 20, 0x2000, 0);
    Value3(Func_02007b00, 14, 0x4000, 0);
    Func_02007a2e(30);
    Func_02007b12(14, 0x4000, 0);
    Func_02007a40(10);
    Func_02007ad8(14, 3);
    Func_02007a4e_a(30);
    Func_02007b26(14, 0);
    Func_02007a5c(10);
    Func_02007b0c(20, 2);
    Func_02007a6a(20);
    Func_02007b3a(20, 0);
    if (Func_02007a9a(0, 0) == 0) {
        Func_02007a84(20);
        Call3(Func_02007b70, 1, 0x102, 40);
        Func_02007b68(1, 0);
        bump_step(1);
    } else {
        Func_02007ab8(10);
        bump_step(1);
        Func_02007ba0(1, 0);
    }

    Func_02007ad6(10);
    Func_02007b86(20, 2);
    Func_02007ae4(20);
    Func_02007bbc(20, 0);
    Func_02007af2(10);
    Call3(Func_02007bde, 20, 0x100, 40);
    Func_02007bd6(20, 0);
    Func_02007b0c_a(20);
    Func_02007b9c(0, 3);
    Func_02007ba4(1, 3);
    Func_02007bac(3, 3);
    Func_02007bbc_a(2, 3);
    Func_02007b32(30);
    Func_02007b38(10);
    Call3(Func_02007c24, 14, 0x100, 40);
    Func_02007c1c(14, 0);
    Func_02007b52(10);
    Func_02007c0c(20, 14, 40);
    Func_02007b62(10);
    Call3(Func_02007c4e, 20, 0x102, 40);
    Func_02007c46(20, 0);
    Func_02007b7c(10);
    Func_02007c36(14, 20, 40);
    Func_02007c1e(14, 3);
    Func_02007b94(30);
    Func_02007c6c(14, 0);
    Func_02007ba2(10);
    Func_02007c52(20, 2);
    Func_02007bb0(20);
    Func_02007c88(20, 0);
    Func_02007c9c(14, 0x4000, 0);
    Func_02007bca(20);
    Func_02007c7a(14, 2);
    Func_02007bd8(20);
    Func_02007cb0(14, 0);
    Func_02007be6(10);
    Call3(Func_02007cd0, 0, 0x101, 80);
    Call3(Func_02007cdc, 20, 0x102, 70);
    Func_02007c94(14, 4);
    Func_02007c0a(20);
    Func_02007ce2(14, 0);
    Func_02007c18(10);
    Func_02007cda(1, 0, 0);
    Func_02007ce4(3, 2, 50);
    Call3(Func_02007d10, 0, 0xc000, 0);
    Call3(Func_02007d1c, 1, 0xc000, 0);
    Call3(Func_02007d28, 3, 0xc000, 0);
    Func_02007d34(2, 0xc000, 0);
    Func_02007c62(30);
    Func_02007c68(10);
    Call3(Func_02007d54, 20, 0x106, 50);
    Func_02007d4c(20, 0);
    Func_02007c82(10);
    Func_02007d1a(14, 3);
    Func_02007c90(30);
    Call3(Func_02007d74, 20, 0x2000, 0);
    Func_02007d74_a(20, 0);
    Func_02007caa(10);
    Call3(Func_02007d96, 14, 0x108, 40);
    Func_02007d86(14, 0);
    if (Func_02007ce6(0, 0) == 0) {
        Func_02007cd0_a(20);
        Call3(Func_02007dbc, 14, 0x100, 40);
        Func_02007db4(14, 0);
        bump_step(1);
    } else {
        Func_02007d04(10);
        Call3(Func_02007df0, 14, 0x100, 40);
        bump_step(1);
        Func_02007df8(14, 0);
    }

    Func_02007d2e(10);
    Func_02007dde(20, 2);
    Func_02007d3c(20);
    Func_02007e14(20, 0);
    Func_02007d4a(10);
    Func_02007e04(14, 20, 40);
    Func_02007dec(14, 3);
    Func_02007d62(30);
    Func_02007e3a(20, 0);
    Func_02007d70(10);
    Func_02007e2a(20, 14, 40);
    Func_02007e52(20, 0);
    Func_02007d88(10);
    Call3(Func_02007e74, 14, 0x100, 40);
    Value3(Func_02007e78, 14, 0x8000, 0);
    Func_02007da6(20);
    Func_02007e7e(14, 0);
    Func_02007db4_a(10);
    Call3(Func_02007ea0, 20, 0x100, 40);
    Func_02007e98(20, 0);
    Func_02007dce(10);
    Func_02007e7e_a(14, 2);
    Func_02007ddc(20);
    Func_02007eb4(14, 0);
    Func_02007dea(10);
    Func_02007e9a(20, 2);
    Func_02007df8_a(20);
    Func_02007ed0(20, 0);
    Func_02007e06(10);
    Func_02007e9e(20, 3);
    Func_02007e14_a(30);
    Func_02007eac(14, 3);
    Func_02007e22(30);
    Func_02007f06(20, 0x2000, 0);
    Func_02007e34(20);
    Func_02007f9c(0, 20);
    Func_02007fa4(1, 20);
    Func_02007fac(3, 20);
    Func_02007fb4(2, 20);
    Call3(Func_02007e92, 20, 0x10000, 0x8000);
    Func_02007fc4(20, 0, 32);
    Func_02007f4e(20, 0, 0);
    Value3(Func_02007f5a, 14, 0x4000, 0);
    Func_02007e88(20);
    Func_02007f60(20, 0);
    Func_02007e96(10);
    Func_02007f2e(20, 3);
    Func_02007ea4(30);
    Func_02008006(1, 16, 0);
    Func_02008008(20, 0, 80);
    Func_02007ebe(40);
    Call3(Func_02008022, 1, -16, 0);
    Call3(Func_02007fae, 1, 0x4000, 0);
    Value3(Func_02007fba, 3, 0x4000, 0);
    Func_02007f60_a(20);
    Func_02007eee(80);
    Func_02007f78(20, 0, 0);
    Call3(Func_02007fdc, 0, 0xc000, 0);
    Call3(Func_02007fe8, 1, 0xc000, 0);
    Call3(Func_02007ff4, 3, 0xc000, 0);
    Func_02008000(2, 0xc000, 0);
    Func_02007f2e_a(30);
    Func_02008096(0, 14);
    Func_0200809e(1, 14);
    Func_020080a6(3, 14);
    Func_020080ae(2, 14);
    Func_02007f54(30);
    Func_02007fec(14, 3);
    Func_02007f62(30);
    Call3(Func_02007f9c_a, 14, 0xcccc, 0x6666);
    Func_020080ce(14, 0, 24);
    Func_020080da(14, -80, 0);
    Func_02007f88(10);
    Func_0200806a(14, 0, 0);
    Func_02007f98(20);
    Func_02008070(14, 0);
    Func_02007fa6(20);
    Func_0200808a(14, 0x4000, 0);
    Func_02007fb8(20);
    Call3(Func_0200811a, 14, 0, 48);
    Func_02008126(14, -64, 0);
    Func_0200801c(0);
    Func_02008022_a(1);
    Func_02008028(3);
    Func_0200802e(2);
    Func_02008070_a(14, 0, 0);
    Func_02007ff6(20);
    Func_020080b8(0, 3, 0);
    Func_020080c2(1, 2, 0);
    Func_02008010(30);
    Func_020080a0(0, 3);
    Func_020080a8(1, 3);
    Func_020080b0(3, 3);
    Func_020080c0(2, 3);
    Func_02008036(30);
    Func_020081ac(17);
    Call3(Func_02008076, 1, 0x13333, 0x9999);
    Call3(Func_02008080, 2, 0x13333, 0x9999);
    Call3(Func_0200808a_a, 3, 0x13333, 0x9999);
    Func_020080ea(1, 2);
    position = Func_02008090(0);
    if (position != 0)
        Func_020080ca(1, position[5], position[9]);
    Func_020080f8(1);
    Func_0200810a(1, 0, 0);
    Func_0200811a_a(2, 2);
    position = Func_020080c0_a(0);
    if (position != 0)
        Func_020080fa(2, position[5], position[9]);
    Func_02008128(2);
    Func_0200813a(2, 0, 0);
    Func_0200814a(3, 2);
    position = Func_020080f0(0);
    if (position != 0)
        Func_0200812a(3, position[5], position[9]);
    Func_02008170(3);
    Func_02008182(3, 0, 0);
    carry = Func_02008108(10);
    carry = Func_0200824c(carry);
    Func_02008120(carry);
}

/* overlays/scene/dialogue/choice/run_supplemental_sequence_one.c */
extern u8 Value_00002233;

/* Supplemental scene sequence of the resource_3b8 overlay. */

void Func_020081ee();
void Func_0200822a();
void Func_0200825c();
void Func_0200827e();
s32 Func_02008294();
s32 Func_02008294_a();
void Func_020082a8();
void Func_020082aa();
void Func_020082c4();
void Func_020082d2();
void Func_020082ea();
void Func_0200830a();
void Func_02008314();
void Func_02008316();
void Func_02008322();
void Func_02008324();
void Func_0200832e();
void Func_02008330();
s32 Func_02008334();
void Func_0200833e();
void Func_02008344();
void Func_02008356();
void Func_0200835a();
void Func_0200835a_a();
void Func_0200835c();
void Func_0200836a();
void Func_02008372();
void Func_02008374();
void Func_02008374_a();
void Func_02008394();
void Func_02008396();
void Func_020083ac();
void Func_020083d6();
void Func_020083da();
void Func_020083e0();
void Func_020083e6();
void Func_020083fa();
void Func_02008416();

static __inline__ void Call0(void (*f)())
{
    f();
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

void RunSupplementalSequenceOne(void)
{
    s32 p;
    Call1(Func_020081ee, 2412);
    Call0(Func_0200822a);
    Call0(Func_02008356);
    Call3(Func_0200830a, 8, 20480, 0);
    Call3(Func_02008316, 9, 12288, 0);
    Call3(Func_020082aa, 0, 200, 272);
    Call3(Func_0200832e, 0, 49152, 0);
    Call1(Func_0200825c, 20);
    p = (s32)&Value_00002233;
    Func_02008324(p);
    Value2(Func_02008334, 8, 0);
    if (Value2(Func_02008294, 0, 0) == 0) {
        Call1(Func_0200827e, 20);
        Func_02008344(p + 1);
        Call2(Func_0200835c, 8, 0);
    } else {
        Value1(Func_02008294_a, 20);
        Func_0200835a(p + 2);
        Call2(Func_02008372, 8, 0);
        Call1(Func_020082a8, 20);
        Call3(Func_0200836a, 8, 9, 60);
        Call3(Func_02008396, 9, 12288, 0);
        Call1(Func_020082c4, 40);
        Call2(Func_02008374, 9, 2);
        Call1(Func_020082d2, 30);
        Call3(Func_02008394, 8, 9, 30);
        Call2(Func_02008374_a, 9, 3);
        Call1(Func_020082ea, 30);
        Call3(Func_020083d6, 8, 258, 50);
        Call3(Func_020083da, 8, 20480, 0);
        Call3(Func_020083e6, 9, 12288, 0);
        Call1(Func_02008314, 20);
        Call2(Func_020083ac, 8, 4);
        Call1(Func_02008322, 20);
        Call2(Func_020083fa, 8, 0);
        Call1(Func_02008330, 10);
        Call2(Func_020083e0, 8, 2);
        Call1(Func_0200833e, 20);
        Call2(Func_02008416, 8, 0);
    }
    Call0(Func_0200835a_a);
}

/* overlays/scene/dialogue/choice/scene_data.c */
#define SceneData_SelectTablec614ByState Func_02000030
#define SceneData_GetTableca7c Func_02000060
#define SceneData_GetTableca8c Func_02000068
#define SceneData_SelectDataBySelectorAndFlags Func_02000070
extern u8 Value_0000008b;
extern u8 Data_0200ca1c[];
extern u8 Data_0200c614[];
extern u8 Data_0200ca7c[];
extern u8 Data_0200ca8c[];
extern u8 Data_0200cb3c[];
extern u8 Data_0200ce6c[];
extern u8 Data_0200cd64[];
extern u8 Data_0200cb84[];

s32 Func_0200442e(s32);
s32 Func_0200443c(s32);

/* Contiguous unnamed leaf-owner run for resource_3b8. */

s32 SceneData_SelectTablec614ByState(void)
{
    extern s16 Data_02000240[];

    if (Data_02000240[224] == (s32)&Value_0000008b) {
        return (s32)Data_0200ca1c;
    }
    return (s32)Data_0200c614;
}

s32 SceneData_GetTableca7c(void)
{
    extern s16 Data_02000240[];

    return (s32)Data_0200ca7c;
}

s32 SceneData_GetTableca8c(void)
{
    extern s16 Data_02000240[];

    return (s32)Data_0200ca8c;
}

s32 SceneData_SelectDataBySelectorAndFlags(void)
{
    extern u8 Data_02000240[];

    s16 *tbl = (s16 *)Data_02000240;

    if (tbl[0xe0] == (s32)&Value_0000008b)
        return (s32)Data_0200cb3c;
    if (Func_0200442e(0x950) != 0)
        return (s32)Data_0200ce6c;
    if (Func_0200443c(0x962) != 0)
        return (s32)Data_0200cd64;
    return (s32)Data_0200cb84;
}
#undef SceneData_SelectTablec614ByState
#undef SceneData_GetTableca7c
#undef SceneData_GetTableca8c
#undef SceneData_SelectDataBySelectorAndFlags

/* overlays/scene/dialogue/choice/scene_dialogue.c */
#define Dialogue_ShowLine2239Or223A Func_02003f84
extern u8 Data_03001ebc[];

s32 Func_0200832a();
void Func_0200833c();
void Func_02008432();
void Func_02008442();
void Func_0200844a();
void Func_0200845a();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void Dialogue_ShowLine2239Or223A(void)
{
    u32 i;
    s32 record;

    if (Value1(Func_0200832a, 0x96d) == 0) {
        Call1(Func_0200833c, 0x96d);
        Call1(Func_02008432, 0x2239);
        Func_0200844a(9, 0);
    } else {
        Call1(Func_02008442, 0x223a);
        Func_0200845a(9, 0);
    }
}
#undef Dialogue_ShowLine2239Or223A

/* overlays/scene/dialogue/choice/scene_effect_sequence_head.c */
#define RunOpeningAuxiliarySequence Func_02000108
#define RunMiddleAuxiliarySequence Func_020003b0
extern u8 Value_00001ff8;
extern u8 Data_0200c570[];

s32 Func_020044d0();
void Func_020044e4();
void Func_020044f2();
void Func_02004504();
u8 *Func_02004510();
void Func_0200453c();
void Func_0200456c();
void Func_0200456e();
void Func_0200459c();
void Func_020045b4();
void Func_020045c4();
void Func_020045e0();
void Func_020045f8();
s32 Func_020045fc();
void Func_02004604();
void Func_02004612();
void Func_02004612_a();
s32 Func_02004614();
void Func_02004626();
void Func_0200462c();
void Func_02004630();
void Func_02004636();
void Func_0200463c();
void Func_0200464a();
void Func_02004664();
void Func_02004680();
void Func_02004684();
void Func_0200468c();
void Func_02004696();
s32 Func_0200469c();
void Func_020046a6();
void Func_020046b6();
s32 Func_020046bc();
void Func_020046be();
void Func_020046c2();
void Func_020046ce();
void Func_020046d0();
void Func_020046d2();
void Func_020046da();
void Func_0200470e();
void Func_02004718();
void Func_02004740();
void Func_0200474c();
void Func_02004756();
void Func_0200475a();
void Func_0200475c();
void Func_0200476c();
void Func_02004774();
void Func_02004776();
void Func_02004780();
void Func_0200478c();
void Func_020047cc();
void Func_02004842();
void Func_0200484c();
void Func_020047a2();
void Func_020047ca();
void Func_020047d8();
void Func_020047de();
void Func_020047e6();
void Func_020047f4();
void Func_02004804();
void Func_0200480e();
void Func_0200481c();
void Func_0200482a();
s32 Func_02004856();
void Func_02004868();
void Func_0200486a();
void Func_0200486c();
void Func_0200486e();
void Func_0200487a();
void Func_02004882();
void Func_02004884();
void Func_02004888();
void Func_02004892();
void Func_020048a2();
void Func_020048b0();
void Func_020048be();
void Func_020048be_a();
void Func_020048c6();
void Func_020048cc();
void Func_020048d4();
void Func_020048e0();
void Func_020048fc();
void Func_0200490a();
void Func_0200491e();
void Func_02004940();
void Func_02004958();
void Func_0200495e();
void Func_02004968();
void Func_0200498e();
void Func_02004990();
void Func_020049ac();
void Func_020049c0();
void Func_020049d4();
void Func_020053a6();
void Func_020053e2();
void Func_02005404();
void Func_02005448();
void Func_02005452();
void Func_02005452_a();
void Func_0200549a();
void Func_0200549c();
void Func_020054a4();
void Func_020054aa();
void Func_020054aa_a();
void Func_020054b0();
void Func_020054be();
void Func_020054cc();
void Func_020054d6();
void Func_020054da();
void Func_020054e0();
void Func_020054ec();
void Func_020054f6();
void Func_020054fa();
void Func_02005508();
void Func_0200550e();
void Func_02005510();
void Func_02005512();
void Func_02005514();
void Func_02005516();
void Func_0200552c();
void Func_0200552e();
void Func_02005532();
void Func_02005536();
void Func_02005548();
void Func_02005558();
void Func_02005566();
void Func_02005576();
void Func_02005578();
void Func_0200557c();
void Func_02005584();
void Func_0200558a();
void Func_02005596();
void Func_0200559a();
void Func_020055a0();
void Func_020055ae();
void Func_020055bc();
void Func_020055c0();
void Func_020055c4();
void Func_020055ca();
void Func_020055e0();
void Func_020055e6();
void Func_020055f4();
void Func_020055f8();
void Func_02005600();
void Func_02005602();
void Func_02005610();
void Func_0200561a();
void Func_0200561e();
void Func_02005630();
void Func_02005636();
void Func_0200563e();
void Func_02005646();
void Func_0200564e();
void Func_02005650();
void Func_0200565a();
void Func_02005662();
void Func_02005668();
void Func_0200566a();
void Func_02005676();
void Func_02005678();
void Func_02005684();
void Func_02005692();
void Func_02005694();
void Func_02005696();
void Func_02005698();
void Func_0200569a();
void Func_020056a6();
void Func_020056aa();
void Func_020056b4();
void Func_020056cc();
void Func_020056ce();
void Func_020056e8();
void Func_020056f2();
void Func_020056f2_a();
void Func_020056f8();
void Func_02005700();
void Func_02005706();
void Func_02005708();
void Func_02005714();
void Func_02005718();
void Func_02005718_a();
void Func_0200571c();
void Func_02005720();
void Func_0200572e();
void Func_02005732();
void Func_0200573a();
void Func_0200573c();
void Func_02005748();
void Func_0200574a();
void Func_0200574e();
void Func_02005762();
void Func_02005770();
void Func_0200577e();
void Func_02005788();
void Func_02005790();
void Func_02005792();
void Func_02005796();
void Func_02005798();
void Func_0200579c();
void Func_0200579c_a();
void Func_020057a0();
void Func_020057a8();
void Func_020057b4();
void Func_020057c2();
void Func_020057c6();
void Func_020057de();
void Func_020057e4();
void Func_020057f8();
void Func_02005800();
void Func_02005804();
void Func_02005808();
void Func_0200580e();
void Func_02005814();
void Func_02005820();
void Func_02005826();
void Func_02005834();
void Func_0200583a();
void Func_0200583c();
void Func_02005842();
void Func_0200584c();
void Func_02005850();
void Func_02005852();
void Func_0200585a();
void Func_0200585e();
void Func_0200586c();
void Func_02005872();
void Func_0200587e();
void Func_02005880();
void Func_02005886();
void Func_0200588e();
void Func_0200589a();
void Func_020058a6();
void Func_020058a8();
void Func_020058a8_a();
void Func_020058b2();
void Func_020058b6();
void Func_020058c4();
void Func_020058d2();
void Func_020058d6();
void Func_020058da();
void Func_020058e0();
void Func_020058f0();
void Func_020058f6();
void Func_020058f8();
void Func_020058fa();
void Func_0200590c();
void Func_02005914();
void Func_02005922();
void Func_02005928();
void Func_0200592c();
void Func_0200593a();
void Func_02005948();
void Func_02005958();
void Func_02005958_a();
void Func_0200595c();
void Func_0200596c();
void Func_02005972();
void Func_02005972_a();
void Func_0200597a();
void Func_02005980();
void Func_0200598e();
void Func_0200598e_a();
void Func_020059aa();
void Func_020059b8();
void Func_020059be();
void Func_020059c4();
void Func_020059c4_a();
void Func_020059cc();
void Func_020059ce();
void Func_020059dc();
void Func_020059de();
void Func_020059e6();
void Func_020059ea();
void Func_020059f6();
void Func_020059f8();
void Func_020059fe();
void Func_02005a06();
void Func_02005a0a();
void Func_02005a0c();
void Func_02005a12();
void Func_02005a14();
void Func_02005a16();
void Func_02005a22();
void Func_02005a22_a();
void Func_02005a34();
void Func_02005a42();
void Func_02005a4a();
void Func_02005a50();
void Func_02005a58();
void Func_02005a5a();
void Func_02005a5e();
void Func_02005a74();
void Func_02005a76();
void Func_02005a80();
void Func_02005a84();
void Func_02005a8a();
void Func_02005a90();
void Func_02005a92();
void Func_02005aa0();
void Func_02005aa0_a();
void Func_02005aa6();
void Func_02005ab8();
void Func_02005ac2();
void Func_02005ac4();
void Func_02005ac6();
void Func_02005ad8();
void Func_02005ada();
void Func_02005ade();
void Func_02005b06();
void Func_02005b0c();
void Func_02005b0e();
void Func_02005b12();
void Func_02005b1a();
void Func_02005b1c();
void Func_02005b28();
void Func_02005b28_a();
void Func_02005b2a();
void Func_02005b40();
void Func_02005b42();
void Func_02005b44();
void Func_02005b48();
void Func_02005b50();
void Func_02005b52();
void Func_02005b5c();
void Func_02005b60();
void Func_02005b62();
void Func_02005b62_a();
void Func_02005b70();
void Func_02005b8a();
void Func_02005b94();
void Func_02005baa();
void Func_02005bae();
void Func_02005bb0();
void Func_02005bca();
void Func_02005bea();
void Func_02005c1c();
void Func_02005c1c_a();
void Func_02005c42();
void Func_02005c46();
void Func_02005c58();
void Func_02005c96();
void Func_02005ce0();
void Func_02005cea();

/* Primary effect sequence of the resource_3b8 overlay: two hundred and fifty scene calls with constant arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    void Func_020047bc();

    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    void Func_020047bc();

    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    void Func_020047bc();

    f(a0, a1, a2);
}

static __inline__ void Call0(void (*f)())
{
    f();
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

void RunOpeningAuxiliarySequence(s32 a)
{
    void Func_020047bc();

    u8 *ret;
    s16 v;
    s32 c;
    s32 t;

    ret = Func_02004510(0);
    v = (*(u16 *)(ret + 6) + 0x2000) & 0xc000;
    Func_02004504();
    Func_02004630();
    if (Value1(Func_020044d0, 512) == 0) {
        Call1(Func_020044e4, 512);
        Func_020044f2(0x969);
        Func_020045e0(0x1ff7);
        Func_020045f8(a, 0);
        Func_0200453c(10);
        t = v << 16;
        c = 0x4000;
        if (t == (0x4000 << 16)) {
            Func_020045b4(0, 40, 104);
            Func_02004636(0, 0, 0);
        }
        Call3(Func_0200459c, a, 0x10000, 0x8000);
        Call3(Func_020046d0, a, 0, -48);
        Func_020046da(a, 64, 0);
        Func_02004664(a, c, 0);
    } else {
        Call1(Func_0200456e, 512);
        Func_0200456c(0x969);
        Call3(Func_02004680, a, 0x4000, 0);
        Func_02004612(0, 120, 96);
        Func_02004696(0, 0xc000, 0);
        Func_020045c4(20);
        c = (s32)&Value_00001ff8;
        Func_0200468c(c);
        Func_0200469c(a, 0);
        if (Func_020045fc(0, 0) == 0) {
            Func_020046a6(c + 1);
            Func_020046be(a, 0);
        } else {
            Func_020046b6(c + 2);
            Func_020046ce(a, 0);
        }
        Func_02004604(10);
        Func_0200469c_a(a, 3);
        Func_02004612_a(20);
        Call3(Func_02004776, a, -64, 0);
        Func_02004780(a, 0, 48);
    }
    Func_0200463c();
}

void RunMiddleAuxiliarySequence(s32 a)
{
    u8 *obj;
    u8 *q;

    obj = Func_020047bc();
    Func_020047a2();
    q = Data_0200c570;
    Func_020047de(a, q);
    Func_0200486c(0x2009);
    Func_02004884(a, 0);
    Func_02004804(a);
    *(s32 *)(obj + 28) = 0x10000;
    *(s32 *)(obj + 24) = 0x10000;
    Func_020047ca(30);
    Func_0200487a(a, 2);
    Func_020047d8(30);
    Func_02004888(a, 2);
    Func_020047e6(60);
    Func_020048be(a, 0);
    Func_020047f4(20);
    Func_020048e0(a, 258, 60);
    Func_020048b0(a, 2);
    Func_0200480e(30);
    Func_020048be_a(a, 2);
    Func_0200481c(30);
    Func_020048cc(a, 2);
    Func_0200482a(30);
    Func_0200486a(a, q);
    Func_0200490a(a, 0);
    Call3(Func_0200491e, a, 0xe000, 0);
    Func_0200484c(10);
    *(s32 *)(obj + 28) = 0x10000;
    *(s32 *)(obj + 24) = 0x10000;
    Func_02004892(a, q);
    Func_0200486e();
}

void Func_02000ff8(void)
{
    Call1(Func_020053a6, 2411);
    Call0(Func_020053e2);
    Call0(Func_0200550e);
    Call1(Func_020054a4, 8225);
    Call3(Func_02005452, 0, 520, 424);
    Call3(Func_020054d6, 0, 57344, 0);
    Call4(Func_02005508, 36700160, -1, 24117248, 1);
    Call0(Func_02005514);
    Call3(Func_02005452_a, 20, 65536, 32768);
    Call3(Func_02005584, 20, 40, 0);
    Call3(Func_0200549a, 20, 584, 360);
    Call2(Func_02005512, 21, 0);
    Call1(Func_02005448, 10);
    Call3(Func_02005532, 20, 257, 40);
    Call3(Func_02005536, 20, 32768, 0);
    Call3(Func_0200549c, 21, 131072, 65536);
    Call3(Func_020054aa, 22, 131072, 65536);
    Call3(Func_020054e0, 21, 528, 352);
    Call3(Func_020054f6, 22, 528, 368);
    Call1(Func_02005404, 3);
    Call2(Func_0200552c, 21, 1);
    Call1(Func_020054aa_a, 30);
    Call1(Func_020054b0, 10);
    Call2(Func_02005548, 22, 3);
    Call1(Func_020054be, 30);
    Call2(Func_02005596, 22, 0);
    Call1(Func_020054cc, 10);
    Call2(Func_0200557c, 20, 2);
    Call1(Func_020054da, 20);
    Call3(Func_0200563e, 20, -16, 0);
    Call1(Func_020054ec, 10);
    Call2(Func_020055c4, 20, 0);
    Call1(Func_020054fa, 10);
    Call2(Func_0200558a, 21, 4);
    Call2(Func_0200559a, 22, 4);
    Call1(Func_02005510, 20);
    Call1(Func_02005516, 10);
    Call3(Func_02005600, 20, 261, 40);
    Call2(Func_020055f8, 20, 0);
    Call1(Func_0200552e, 10);
    Call3(Func_0200561a, 21, 258, 40);
    Call3(Func_02005578, 21, 65536, 32768);
    Call3(Func_020056aa, 21, 8, 0);
    Call1(Func_02005558, 20);
    Call2(Func_02005630, 21, 0);
    Call1(Func_02005566, 10);
    Call3(Func_02005650, 20, 257, 80);
    Call1(Func_02005576, 10);
    Call3(Func_02005662, 22, 258, 40);
    Call3(Func_020055c0, 22, 65536, 32768);
    Call3(Func_020056f2, 22, 8, 0);
    Call1(Func_020055a0, 20);
    Call2(Func_02005678, 22, 0);
    Call1(Func_020055ae, 10);
    Call2(Func_02005646, 20, 4);
    Call1(Func_020055bc, 20);
    Call2(Func_02005694, 20, 0);
    Call1(Func_020055ca, 10);
    Call2(Func_0200565a, 21, 3);
    Call2(Func_0200566a, 22, 3);
    Call1(Func_020055e0, 30);
    Call1(Func_020055e6, 10);
    Call2(Func_02005696, 21, 2);
    Call1(Func_020055f4, 20);
    Call2(Func_020056cc, 21, 0);
    Call1(Func_02005602, 10);
    Call2(Func_0200569a, 22, 4);
    Call1(Func_02005610, 20);
    Call2(Func_020056e8, 22, 0);
    Call1(Func_0200561e, 10);
    Call3(Func_02005708, 20, 261, 40);
    Call2(Func_02005700, 20, 0);
    Call1(Func_02005636, 10);
    Call3(Func_02005720, 21, 257, 40);
    Call2(Func_02005718, 21, 0);
    Call1(Func_0200564e, 10);
    Call3(Func_0200573a, 20, 258, 40);
    Call2(Func_02005732, 20, 0);
    Call1(Func_02005668, 10);
    Call2(Func_02005718_a, 22, 2);
    Call1(Func_02005676, 20);
    Call2(Func_0200574e, 22, 0);
    Call1(Func_02005684, 10);
    Call2(Func_0200571c, 20, 3);
    Call1(Func_02005692, 30);
    Call1(Func_02005698, 10);
    Call2(Func_02005748, 21, 2);
    Call1(Func_020056a6, 20);
    Call2(Func_0200577e, 21, 0);
    Call1(Func_020056b4, 10);
    Call3(Func_020057a0, 20, 258, 40);
    Call2(Func_02005798, 20, 0);
    Call1(Func_020056ce, 20);
    Call3(Func_02005790, 21, 22, 60);
    Call3(Func_02005792, 21, 20, 0);
    Call3(Func_0200579c, 22, 20, 0);
    Call1(Func_020056f2_a, 20);
    Call1(Func_020056f8, 10);
    Call2(Func_020057a8, 20, 2);
    Call1(Func_02005706, 20);
    Call2(Func_020057de, 20, 0);
    Call1(Func_02005714, 10);
    Call3(Func_02005800, 21, 256, 40);
    Call2(Func_020057f8, 21, 0);
    Call1(Func_0200572e, 10);
    Call2(Func_020057c6, 22, 3);
    Call1(Func_0200573c, 30);
    Call2(Func_02005814, 22, 0);
    Call1(Func_0200574a, 10);
    Call2(Func_0200583c, 20, 258);
    Call2(Func_02005804, 20, 2);
    Call1(Func_02005762, 30);
    Call2(Func_0200583a, 20, 0);
    Call1(Func_02005770, 10);
    Call3(Func_0200585a, 21, 257, 40);
    Call2(Func_02005852, 21, 0);
    Call1(Func_02005788, 10);
    Call2(Func_02005820, 20, 3);
    Call1(Func_02005796, 30);
    Call1(Func_0200579c_a, 10);
    Call3(Func_02005886, 22, 257, 40);
    Call2(Func_0200587e, 22, 0);
    Call1(Func_020057b4, 10);
    Call2(Func_0200584c, 20, 4);
    Call1(Func_020057c2, 20);
    Call2(Func_0200589a, 20, 0);
    Call1(Func_020057e4, 20);
    Call3(Func_020058a6, 21, 22, 60);
    Call3(Func_020058a8, 21, 20, 0);
    Call3(Func_020058b2, 22, 20, 0);
    Call1(Func_02005808, 20);
    Call1(Func_0200580e, 10);
    Call3(Func_020058f8, 20, 261, 40);
    Call2(Func_020058f0, 20, 0);
    Call1(Func_02005826, 10);
    Call2(Func_020058d6, 21, 2);
    Call1(Func_02005834, 20);
    Call2(Func_0200590c, 21, 0);
    Call1(Func_02005842, 10);
    Call2(Func_020058da, 22, 3);
    Call1(Func_02005850, 30);
    Call2(Func_02005928, 22, 0);
    Call1(Func_0200585e, 10);
    Call2(Func_020058f6, 20, 3);
    Call1(Func_0200586c, 30);
    Call1(Func_02005872, 10);
    Call2(Func_02005922, 22, 2);
    Call1(Func_02005880, 20);
    Call2(Func_02005958, 22, 0);
    Call1(Func_0200588e, 10);
    Call3(Func_0200597a, 20, 258, 40);
    Call2(Func_02005972, 20, 0);
    Call1(Func_020058a8_a, 10);
    Call2(Func_02005958_a, 21, 2);
    Call1(Func_020058b6, 20);
    Call2(Func_0200598e, 21, 0);
    Call1(Func_020058c4, 10);
    Call2(Func_0200595c, 20, 4);
    Call1(Func_020058d2, 20);
    Call2(Func_020059aa, 20, 0);
    Call1(Func_020058e0, 10);
    Call3(Func_020059cc, 22, 256, 40);
    Call2(Func_020059c4, 22, 0);
    Call1(Func_020058fa, 10);
    Call3(Func_020059e6, 20, 258, 40);
    Call2(Func_020059de, 20, 0);
    Call1(Func_02005914, 10);
    Call3(Func_020059fe, 21, 257, 40);
    Call2(Func_020059f6, 21, 0);
    Call1(Func_0200592c, 10);
    Call2(Func_020059c4_a, 20, 4);
    Call1(Func_0200593a, 20);
    Call2(Func_02005a12, 20, 0);
    Call1(Func_02005948, 20);
    Call3(Func_02005a0a, 21, 22, 60);
    Call3(Func_02005a0c, 21, 20, 0);
    Call3(Func_02005a16, 22, 20, 0);
    Call1(Func_0200596c, 20);
    Call1(Func_02005972_a, 10);
    Call2(Func_02005a22, 20, 2);
    Call1(Func_02005980, 20);
    Call2(Func_02005a58, 20, 0);
    Call1(Func_0200598e_a, 10);
    Call2(Func_02005a80, 21, 258);
    Call2(Func_02005a8a, 22, 258);
    Call2(Func_02005a4a, 21, 2);
    Call2(Func_02005a5a, 22, 2);
    Call1(Func_020059b8, 30);
    Call1(Func_020059be, 10);
    Call3(Func_02005aa0, 20, 0, 0);
    Call1(Func_020059ce, 20);
    Call2(Func_02005aa6, 20, 0);
    Call1(Func_020059dc, 10);
    Call2(Func_02005a74, 21, 4);
    Call1(Func_020059ea, 20);
    Call2(Func_02005ac2, 21, 0);
    Call1(Func_020059f8, 10);
    Call2(Func_02005a90, 22, 4);
    Call1(Func_02005a06, 20);
    Call2(Func_02005ade, 22, 0);
    Call1(Func_02005a14, 10);
    Call2(Func_02005ac4, 20, 2);
    Call1(Func_02005a22_a, 20);
    Call3(Func_02005b06, 20, 32768, 0);
    Call1(Func_02005a34, 20);
    Call2(Func_02005b0c, 20, 0);
    Call1(Func_02005a42, 10);
    Call2(Func_02005ada, 21, 3);
    Call1(Func_02005a50, 30);
    Call2(Func_02005b28, 21, 0);
    Call1(Func_02005a5e, 20);
    Call3(Func_02005b48, 20, 261, 40);
    Call2(Func_02005b40, 20, 0);
    Call1(Func_02005a76, 10);
    Call2(Func_02005b0e, 22, 3);
    Call1(Func_02005a84, 30);
    Call2(Func_02005b5c, 22, 0);
    Call1(Func_02005a92, 10);
    Call2(Func_02005b2a, 20, 3);
    Call1(Func_02005aa0_a, 40);
    Call3(Func_02005b62, 21, 22, 60);
    Call2(Func_02005b42, 21, 3);
    Call1(Func_02005ab8, 20);
    Call2(Func_02005b50, 22, 3);
    Call1(Func_02005ac6, 20);
    Call3(Func_02005baa, 22, 16384, 0);
    Call1(Func_02005ad8, 40);
    Call3(Func_02005b12, 21, 85196, 42598);
    Call3(Func_02005b1c, 22, 85196, 42598);
    Call3(Func_02005c46, 21, 0, 120);
    Call3(Func_02005c58, 22, 0, 120);
    Call3(Func_02005b8a, 21, 0, 0);
    Call3(Func_02005b94, 22, 0, 0);
    Call1(Func_02005b1a, 10);
    Call2(Func_02005bca, 20, 2);
    Call1(Func_02005b28_a, 40);
    Call3(Func_02005b62_a, 20, 52428, 26214);
    Call3(Func_02005c96, 20, -16, 0);
    Call1(Func_02005b44, 30);
    Call2(Func_02005c1c, 20, 0);
    Call1(Func_02005b52, 10);
    Call2(Func_02005bea, 20, 4);
    Call1(Func_02005b60, 30);
    Call3(Func_02005c42, 20, 0, 0);
    Call1(Func_02005b70, 30);
    Call3(Func_02005bae, 20, 65536, 32768);
    Call3(Func_02005ce0, 20, 120, 0);
    Call3(Func_02005cea, 20, 60, 0);
    Call3(Func_02005c1c_a, 20, 0, 0);
    Call0(Func_02005bb0);
}
#undef RunOpeningAuxiliarySequence
#undef RunMiddleAuxiliarySequence

/* overlays/scene/dialogue/choice/scene_effect_sequence_mid.c */
#define Scene_RunScene3b8SequenceB Func_02000674

extern u8 Value_00002280[];
extern u8 Data_03001ebc[];
void Func_020049da();
void Func_02004a58();
void Func_02004a62();
s32 Func_02004a84();
s32 Func_02004a90();
s32 Func_02004ab6();
s32 Func_02004ac2();
s32 Func_02004ad4();
void Func_02004b16();
void Func_02004b26();
void Func_02004b2e();
void Func_02004b50();
void Func_02004b54();
void Func_02004b56();
void Func_02004b5e();
void Func_02004b64();
void Func_02004b6a();
void Func_02004b6c();
void Func_02004b72();
void Func_02004b9a();
s32 Func_02004ba0();
void Func_02004ba8();
s32 Func_02004bae();
void Func_02004bb6();
void Func_02004bc4();
void Func_02004bce();
void Func_02004bd2();
void Func_02004bd8();
void Func_02004be2();
void Func_02004be4();
void Func_02004bfc();
void Func_02004c06();
void Func_02004c0a();
void Func_02004c18();
void Func_02004c1c();
void Func_02004c28();
void Func_02004c32();
void Func_02004c34();
void Func_02004c38();
void Func_02004c3c();
void Func_02004c40();
void Func_02004c42();
void Func_02004c50();
void Func_02004c5e();
void Func_02004c66();
void Func_02004c70();
void Func_02004c7e();
void Func_02004c94();
void Func_02004c9a();
void Func_02004c9c();
void Func_02004c9c_a();
void Func_02004cb6();
void Func_02004cbc();
void Func_02004cc4();
void Func_02004cc6();
s32 Func_02004ccc();
void Func_02004cce();
void Func_02004cd2();
void Func_02004cda();
void Func_02004ce0();
void Func_02004ce2();
void Func_02004cee();
void Func_02004cfc();
void Func_02004d04();
void Func_02004d10();
void Func_02004d14();
void Func_02004d1a();
void Func_02004d20();
void Func_02004d28();
void Func_02004d36();
void Func_02004d42();
void Func_02004d44();
void Func_02004d48();
void Func_02004d4e();
void Func_02004d52();
void Func_02004d5c();
void Func_02004d60();
s32 Func_02004d6a();
void Func_02004d6c();
void Func_02004d78();
void Func_02004d7c();
void Func_02004d86();
void Func_02004d90();
void Func_02004daa();
void Func_02004db2();
void Func_02004dc0();
void Func_02004dc6();
s32 Func_02004dca();
void Func_02004dce();
void Func_02004dd4();
void Func_02004dd4_a();
void Func_02004df2();
void Func_02004e1c();
void Func_02004e1e();
void Func_02004e2c();
void Func_02004e36();
void Func_02004e40();
s32 Func_02004e46();
void Func_02004e4e();
void Func_02004e56();
void Func_02004e64();
void Func_02004e6c();
s32 Func_02004e76();
void Func_02004e7c();
void Func_02004e80();
void Func_02004e8c();
void Func_02004e98();
void Func_02004ea0();
s32 Func_02004ea6();
s32 Func_02004ea6_a();
void Func_02004eae();
void Func_02004eb0();
void Func_02004eba();
void Func_02004ec0();
void Func_02004ec4();
void Func_02004ed0();
void Func_02004ede();
void Func_02004ee0();
void Func_02004ef0();
void Func_02004ef4();
void Func_02004f00();
void Func_02004f0e();
void Func_02004f20();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

#define Scene_RunScene3b8SequenceA Func_02000af8
extern u8 Value_00001ffb[];
s32 Func_02004eda();
void Func_02004ef2();
s32 Func_02004efe();
s32 Func_02004f26();
void Func_02004f2c();
void Func_02004f3a();
void Func_02004f78();
void Func_02004f90();
void Func_02004f9e();
void Func_02004fac();
void Func_02004fac_a();
void Func_02004fb4();
void Func_02004fbe();
s32 Func_02004fc6();
void Func_02004fc8();
void Func_02004fca();
void Func_02004fe0();
void Func_02004fe8();
void Func_02004ff0();
void Func_0200500a();
void Func_02005018();
void Func_0200501e();
void Func_02005026();
void Func_02005036();
void Func_02005044();
void Func_0200504a();
void Func_0200505a();
void Func_02005062();
void Func_02005068();
void Func_0200506a();
void Func_02005082();
void Func_02005090();
void Func_0200509a();
void Func_0200509e();
void Func_020050b0();
void Func_020050b2();
void Func_020050bc();
void Func_020050c2();
void Func_020050cc();
void Func_020050d0();
void Func_020050da();
void Func_020050e2();
s32 Func_020050fe();
void Func_0200510c();
void Func_0200511a();
void Func_0200511a_a();
void Func_02005126();
void Func_02005128();
void Func_0200512c();
s32 Func_02005130();
s32 Func_02005130_a();
void Func_0200513c();
void Func_02005146();
void Func_0200515a();
void Func_02005162();
void Func_0200518c();
void Func_02005196();
void Func_020051a0();
void Func_020051a2();
void Func_020051a4();
void Func_020051ba();
void Func_020051be();
void Func_020051da();
void Func_020051de();
s32 Func_020051e6();
void Func_020051ec();
void Func_020051f2();
void Func_020051f4();
void Func_02005202();
void Func_0200520c();
void Func_02005212();
void Func_02005216();
s32 Func_02005218();
s32 Func_02005218_a();
void Func_02005244();
void Func_02005246();
void Func_0200524a();
void Func_02005252();
void Func_0200526e();
void Func_0200527a();
void Func_02005286();
void Func_0200528c();
void Func_02005290();
void Func_020052a2();
void Func_020052a2_a();
void Func_020052a2_b();
void Func_020052ae();
void Func_020052ba();
void Func_020052c6();
void Func_020052d4();
void Func_020052da();
void Func_020052e4();
void Func_020052f2();
void Func_020052fa();
void Func_02005300();
void Func_02005326();
void Func_0200532c();
void Func_0200532c_a();
void Func_0200533a();
void Func_02005344();
void Func_0200534a();
void Func_0200534e();
s32 Func_02005354();
s32 Func_02005384();
s32 Func_02005384_a();
void Func_0200538a();
void Func_0200538e();
void Func_02005398();
void Func_020053ae();
s32 Func_020053b4();
void Func_020053b6();
void Func_020053bc();
void Func_020053bc_a();
void Func_020053be();
void Func_020053c8();
void Func_020053ce();
void Func_020053d0();
void Func_020053d4();
void Func_020053de();
void Func_020053e8();
void Func_020053ec();
void Func_020053ec_a();
void Func_020053ee();
void Func_020053fe();
void Func_02005406();
void Func_0200540e();
void Func_0200541c();
void Func_02005424();
void Func_0200542e();

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}
static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ __attribute__((always_inline)) void bump_step(s32 amount)
{
    u8 *scene = *(u8 **)Data_03001ebc;

    *(u16 *)(scene + 0x1d8) = (u16)(*(u16 *)(scene + 0x1d8) + amount);
}

void Scene_RunScene3b8SequenceB(void)
{
    u32 i;
    s32 record;
    s32 v5;

    Func_02004a58();
    Func_02004b16((s32)&Value_00002280);
    v5 = 0;
    *(u8 *)(Func_02004a84(0) + 84) = v5;
    *(u8 *)(Func_02004a90(10) + 84) = v5;
    Func_020049da(1);
    *(volatile u16 *)0x04000000 = 0x1140;
    Call2(Func_02004b54, -1, 0);
    *(volatile u16 *)0x04000000 = 0x140;
    v5 = 1;
    *(u8 *)(Func_02004ab6(0) + 84) = v5;
    *(u8 *)(Func_02004ac2(10) + 84) = v5;
    Func_02004b2e(0, 31);
    record = Func_02004ad4(0);
    Func_02004a62(record, 0);
    Call3(Func_02004b50, 1, 0x780000, 0x680000);
    Call3(Func_02004b5e, 3, 0x680000, 0x500000);
    Call3(Func_02004b6c, 2, 0x780000, 0x780000);
    Func_02004bce(1, 0, 0);
    Func_02004bd8(3, 0, 0);
    Call3(Func_02004be4, 2, 0xe000, 0);
    *(s32 *)(*(u8 **)Data_03001ebc + 0x1c8) = 60;
    Func_02004c34();
    Func_02004c40();
    Func_02004b26(20);
    *(s32 *)(*(u8 **)Data_03001ebc + 0x1c8) = 24;
    Call3(Func_02004b6a, 3, 0x10000, 0x8000);
    Func_02004c9c(3, 16, 0);
    Call3(Func_02004c28, 3, 0x2000, 0);
    Func_02004b56(20);
    Func_02004c06(3, 2);
    Func_02004b64(30);
    Func_02004c3c(3, 0);
    Func_02004b72(10);
    record = Func_02004ba0(0);
    *(s32 *)(record + 16) += -0x30000;
    record = Value1(Func_02004bae, 0);
    *(s32 *)(record + 64) += -0x30000;
    Func_02004c1c(0, 32);
    Func_02004b9a(40);
    Func_02004c32(0, 34);
    Func_02004ba8(30);
    Func_02004c38(0, 33);
    Func_02004bb6(50);
    Func_02004c66(1, 2);
    Func_02004bc4(30);
    Func_02004c9c_a(1, 0);
    Func_02004bd2(10);
    Call3(Func_02004cbc, 0, 0x105, 60);
    Func_02004be2(20);
    Call3(Func_02004cce, 1, 0x102, 60);
    Func_02004cc6(1, 0);
    Func_02004bfc(10);
    Func_02004c94(1, 4);
    Func_02004c0a(20);
    Func_02004ce2(1, 0);
    Func_02004c18(10);
    Call3(Func_02004d04, 0, 0x102, 80);
    Call3(Func_02004d10, 2, 0x106, 60);
    Call3(Func_02004d14, 2, 0xc000, 0);
    Func_02004c42(30);
    Func_02004cda(2, 4);
    Func_02004c50(20);
    Func_02004d28(2, 0);
    Func_02004c5e(10);
    Call3(Func_02004d42, 1, 0x4000, 0);
    Func_02004c70(30);
    Func_02004d20(1, 2);
    Func_02004c7e(45);
    Func_02004d60(1, 0, 0);
    Call3(Func_02004d6c, 2, 0xe000, 0);
    Func_02004c9a(30);
    Value2(Func_02004d6a, 1, 0);
    if (Value2(Func_02004ccc, -1, 0) != 0) {
        Func_02004cb6(10);
        Func_02004d4e(0, 34);
        Func_02004cc4(20);
        Func_02004d5c(1, 3);
        Func_02004cd2(20);
        Func_02004daa(1, 0);
        Func_02004ce0(10);
        Func_02004d78(0, 33);
        Func_02004cee(30);
        Func_02004d86(1, 3);
        Func_02004cfc(20);
        Func_02004dd4(1, 0);
        bump_step(1);
    } else {
        Func_02004d1a(10);
        Func_02004db2(0, 33);
        bump_step(2);
        Func_02004d36(30);
        Func_02004dce(1, 3);
        Func_02004d44(20);
        Func_02004e1c(1, 0);
    }
    Func_02004d52(10);
    Call3(Func_02004d90, 1, 0x10000, 0x8000);
    Call3(Func_02004ec4, 1, -16, 0);
    Func_02004e4e(1, 0, 0);
    Func_02004d7c(35);
    Func_02004e1e(0, 6, 0);
    Call3(Func_02004dc0, 0, 0x1e666, 0xf333);
    Call3(Func_02004ef4, 0, -32, 0);
    record = Func_02004dca(0);
    Func_02004d48(record, 1);
    Call3(Func_02004e8c, 3, 0x4000, 0);
    Call3(Func_02004e98, 2, 0xc000, 0);
    Func_02004dc6(40);
    Func_02004e56(0, 3);
    Func_02004dd4_a(30);
    Func_02004e64(2, 3);
    Func_02004e6c(1, 3);
    Func_02004e7c(3, 3);
    Func_02004df2(30);
    Call3(Func_02004e2c, 1, 0x13333, 0x9999);
    Call3(Func_02004e36, 3, 0x13333, 0x9999);
    Call3(Func_02004e40, 2, 0x13333, 0x9999);
    Func_02004ea0(1, 2);
    record = Value1(Func_02004e46, 0);
    if (record != 0) {
        Func_02004e80(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02004eae(1);
    Func_02004ec0(1, 0, 0);
    Func_02004ed0(3, 2);
    record = Value1(Func_02004e76, 0);
    if (record != 0) {
        Func_02004eb0(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02004ede(3);
    Func_02004ef0(3, 0, 0);
    Func_02004f00(2, 2);
    record = Value1(Func_02004ea6, 0);
    if (record != 0) {
        Func_02004ee0(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02004f0e(2);
    Func_02004f20(2, 0, 0);
    ((void (*)())Func_02004ea6_a)(10);
    Func_02004eba();
}

void Scene_RunScene3b8SequenceA(void)
{
    s32 record;
    s16 dir;
    u16 facing;

    record = Func_02004efe(0);
    dir = (*(u16 *)(record + 6) + 0x2000) & -0x4000;
    Func_02004ef2();
    Func_0200501e();
    Func_02004fb4((s32)&Value_00001ffb);
    Value2(Func_02004fc6, -1, 0);
    if (Value2(Func_02004f26, 0, 0) != 0) {
    } else {
        if (Value1(Func_02004eda, 0x96a) != 0) {
            Func_02004f2c(20);
            Func_0200509a(0);
            goto L_02000fe2;
        }
        Func_02004f3a(20);
        facing = dir;
        if (facing == 0) {
            Func_02004fac(0, 128, 120);
        }
        if (facing == 0x8000) {
            Func_02004fbe(0, 240, 120);
        }
        Func_02004fc8(0, 184, 120);
        Func_0200504a(0, 0, 0);
        Func_02004f78(10);
        Func_020050cc(1, 16, 0, 0x8000);
        Func_02004fca(1);
        Func_02004f90(10);
        Func_02005068(1, 0);
        Func_02004f9e(10);
        Func_02005036(0, 3);
        Func_02004fac_a(20);
        Call3(Func_02004fe8, 1, 0x10000, 0x8000);
        Func_0200511a(1, 40, 0);
        Call3(Func_02005126, 1, 0, -32);
        Call3(Func_020050b2, 1, 0x6000, 0);
        Func_02004fe0(20);
        Func_020050c2(0, 0x8000, 0);
        Func_02004ff0(20);
        Call4(Func_02005146, 2, -16, 0, 0);
        Func_02005044(2);
        Func_0200500a(10);
        Func_020050e2(2, 0);
        Func_02005018(10);
        Func_020050b0(0, 3);
        Func_02005026(20);
        Call3(Func_02005062, 2, 0x10000, 0x8000);
        Call3(Func_02005196, 2, -40, 0);
        Func_020051a0(2, 0, 40);
        Call3(Func_0200512c, 2, 0xe000, 0);
        Func_0200505a(20);
        Func_0200513c(0, 0, 0);
        Func_0200506a(20);
        Func_020051be(3, 16, 0, 0x8000);
        Func_020050bc(3);
        Func_02005082(10);
        Func_0200515a(3, 0);
        Func_02005090(10);
        Func_02005128(0, 3);
        Func_0200509e(20);
        Call3(Func_020050da, 3, 0x10000, 0x8000);
        Func_0200520c(3, 40, 0);
        Func_02005216(3, 0, 40);
        Call3(Func_020051a2, 3, 0xa000, 0);
        Func_020050d0(20);
        Call4(Func_020051da, -1, -1, -1, 0);
        Call3(Func_02005246, 0, -56, 0);
        Call3(Func_02005252, 0, 0, -32);
        Call3(Func_020051de, 0, 0x2000, 0);
        Func_0200510c(30);
        Func_020051a4(3, 3);
        Func_0200511a_a(10);
        Func_020051f2(3, 0);
        Func_020051ba(1, 3);
        ((void (*)())Func_02005130)(10);
        if (Value1(Func_020050fe, 0x96a) == 0) {
            Func_02005212(1, 0);
        } else {
            bump_step(1);
        }
        Func_020051ec(2, 3);
        Func_02005162(10);
        if (Value1(Func_02005130_a, 0x96a) == 0) {
            Func_02005244(2, 0);
        } else {
            bump_step(1);
        }
        Func_0200518c(20);
        Func_0200526e(0, 0, 0);
        Call3(Func_0200527a, 1, 0x8000, 0);
        Call3(Func_02005286, 3, 0x8000, 0);
        Func_02005290(2, 0, 0);
        Func_02005326(0);
        Call3(Func_020052a2, 0, 0x2000, 0);
        Call3(Func_020052ae, 1, 0x6000, 0);
        Call3(Func_020052ba, 3, 0xa000, 0);
        Call3(Func_020052c6, 2, 0xe000, 0);
        Func_020051f4(20);
        Func_0200528c(1, 3);
        Func_02005202(10);
        Func_020052da(1, 0);
        Func_020052a2_a(2, 3);
        ((void (*)())Func_02005218)(10);
        if (Value1(Func_020051e6, 0x96a) == 0) {
            Func_020052fa(2, 0);
        } else {
            bump_step(1);
        }
        Func_020052d4(3, 3);
        Func_0200524a(10);
        if (Value1(Func_02005218_a, 0x96a) == 0) {
            Func_0200532c(3, 0);
        } else {
            bump_step(1);
        }
        Func_020053d0(1, 0, 32);
        Call3(Func_020053d4, 1, -112, 0);
        Call3(Func_020053e8, 3, 0, -40);
        Call3(Func_020053ec, 3, -112, 0);
        Func_020052a2_b(50);
        Call3(Func_02005406, 2, 0, -24);
        Func_0200532c_a(1);
        Call3(Func_02005398, 0, 0x4000, 0);
        Call3(Func_02005424, 1, 0, -16);
        Func_0200534a(3);
        Call3(Func_020053b6, 3, 0xc000, 0);
        Func_020052e4(20);
        Func_020053bc(1, 0);
        Func_020052f2(10);
        Func_0200538a(0, 3);
        Func_02005300(20);
        Call3(Func_0200533a, 1, 0x13333, 0x9999);
        Call3(Func_02005344, 2, 0x13333, 0x9999);
        Call3(Func_0200534e, 3, 0x13333, 0x9999);
        Func_020053ae(1, 2);
        record = Value1(Func_02005354, 0);
        if (record != 0) {
            Func_0200538e(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Func_020053bc_a(1);
        Func_020053ce(1, 0, 0);
        Func_020053de(3, 2);
        record = Value1(Func_02005384, 0);
        if (record != 0) {
            Func_020053be(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Func_020053ec_a(3);
        Func_020053fe(3, 0, 0);
        Func_0200540e(2, 2);
        record = Value1(Func_020053b4, 0);
        if (record != 0) {
            Func_020053ee(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Func_0200541c(2);
        Func_0200542e(2, 0, 0);
        Call1(Func_02005384_a, 0x96a);
    }
    Func_020053c8();
    L_02000fe2:;
}
#undef Scene_RunScene3b8SequenceB
#undef Scene_RunScene3b8SequenceA

/* overlays/scene/dialogue/choice/scene_primary_script.c */
#define FieldScene_RunScene3b8_02000264 Func_02000264
#define FieldScene_RunScene3b8_0200049c Func_0200049c
#define Dialogue_RunChoiceSequence22ab Func_02000564
#define Dialogue_RunChoiceSequence2352 Func_020005a4
#define FieldScene_RunScene3b8_02003d40 Func_02003d40
extern u8 Data_00002241[];
extern u8 Data_03001ebc[];
extern u8 Value_000022ab;
extern u8 Value_00002352;
extern u8 Value_00000f31;

s32 Func_02004614();
void Func_02004626();
void Func_0200462c();
void Func_0200464a();
void Func_02004684();
void Func_020046a6_a();
s32 Func_020046bc();
void Func_020046c2();
void Func_020046d0_a();
void Func_020046d2();
void Func_0200470e();
void Func_02004718();
void Func_02004740();
void Func_0200474c();
void Func_02004756();
void Func_0200475a();
s32 Func_0200475c();
void Func_0200476c();
void Func_02004774();
void Func_02004776_a();
void Func_0200478c();
void Func_020047bc_a();
void Func_020047cc();
void Func_02004842();
void Func_0200484c_a();
s32 Func_02004856();
void Func_02004868();
void Func_02004882();
void Func_020048a2();
void Func_020048c6();
void Func_020048d4();
void Func_020048fc();
void Func_02004940();
void Func_02004958();
void Func_0200495e();
void Func_02004968();
void Func_0200498e();
void Func_02004990();
void Func_020049ac();
void Func_020049c0();
void Func_020049d4();
void Func_02004a06(s32);
s32 Func_02004a16(s32, s32);
s32 Func_02004976(s32, s32);
void Func_02004a20(s32);
void Func_02004a28(s32);
s32 Func_02004a40(s32, s32);
void Func_02004988(void);
void Func_02004ab4();
void Func_02004ab4_a();
void Func_02004a4c();
void Func_02004a66(s32, s32);
void Func_0200499c(s32);
void Func_020049aa(s32);
void Func_02004a64(s32, s32, s32);
s32 Func_02004a84(s32, s32);
s32 Func_020049e4(s32, s32);
void Func_02004a8e(s32);
void Func_02004aa6(s32, s32);
void Func_020049de(s32);
void Func_02004aa4(s32);
void Func_02004abc(s32, s32);
void Func_020049f2(s32);
void Func_02004a8a(s32, s32);
void Func_02004a00_a(s32);
void Func_02004ae4(s32, s32, s32);
void Func_02004a12(s32);
void Func_02004a9c(s32, s32, s32);
void Func_02004b4c(s32, s32);
void Func_02004a0a(s32);
void Func_020080e0();
void Func_020080fe();
void Func_02008118();
void Func_02008128();
void Func_0200814c();
void Func_02008152();
void Func_02008156();
void Func_02008158();
void Func_02008164();
void Func_02008174();
void Func_0200818e();
void Func_020081a0();
void Func_020081c4();
void Func_020081d2();
void Func_020081d4();
void Func_020081e8();
void Func_02008202();
void Func_02008296();
void Func_020082b6();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_02003d40(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void FieldScene_RunScene3b8_02000264(s32 a0)
{
    u32 i;
    s32 record;
    s32 base6_2241;

    Func_0200464a();
    Func_02004776_a();
    if (Value1(Func_02004614, 0x966) == 0) {
        Call1(Func_02004626, 0x966);
        Call1(Func_0200462c, 0x967);
        Call3(Func_02004740, a0, 0x4000, 0);
        Func_020046d2(0, 120, 96);
        Call3(Func_02004756, 0, 0xc000, 0);
        Func_02004684(20);
        base6_2241 = (s32)Data_00002241;
        Func_0200474c(base6_2241);
        Value2(Func_0200475c, a0, 0);
        if (Value2(Func_020046bc, 0, 0) == 0) {
            Func_020046a6_a(10);
            Func_0200476c((base6_2241 + 1));
        } else {
            Func_02004774((base6_2241 + 2));
        }
        Func_0200478c(a0, 0);
        Func_020046c2(10);
        Func_0200475a(a0, 3);
        Func_020046d0_a(20);
        Call3(Func_0200470e, a0, 0x10000, 0x8000);
        Call3(Func_02004842, a0, -64, 0);
        Func_0200484c_a(a0, 0, 48);
    } else {
        Call1(Func_020047bc_a, 0x2245);
        Func_020047cc(a0, 0);
    }
    Func_02004718();
}

void FieldScene_RunScene3b8_0200049c(s32 unused0, s32 a1)
{
    Func_02004882();
    Call1(Func_02004940, 0x2052);
    Func_02004958(a1, 0);
    if (Value1(Func_02004856, 0x968) == 0) {
        Call1(Func_02004868, 0x968);
        Func_020049d4();
        Func_020048a2(50);
        Call3(Func_0200498e, a1, 0x100, 70);
        Func_02004968(a1, 0, 40);
        Func_02004990(a1, 0);
        Func_020048c6(30);
        Func_0200495e(a1, 4);
        Func_020048d4(20);
        Func_020049ac(a1, 0);
        Call3(Func_020049c0, a1, 0x8000, 0);
    }
    Func_020048fc();
}

void Dialogue_RunChoiceSequence22ab(s32 no)
{
    s32 msg = (s32)&Value_000022ab;

    Func_02004a06(msg);
    Func_02004a16(no, 0);
    if (Func_02004976(0, 0) == 0)
        Func_02004a20(msg + 1);
    else
        Func_02004a28(msg + 2);
    Func_02004a40(no, 0);
}

void Dialogue_RunChoiceSequence2352(void)
{
    s32 msg;

    Func_02004988();
    Func_02004ab4();
    msg = (s32)&Value_00002352;
    Func_02004a4c(msg);
    Func_02004a66(-1, 0);
    Func_0200499c(10);
    Func_02004a4c_a(14, 2);
    Func_020049aa(30);
    Func_02004a64(0, 14, 30);
    Func_02004a84(14, 0);
    if (Func_020049e4(0, 0) != 0) {
        Func_02004a8e(msg + 2);
        Func_02004aa6(14, 0);
    } else {
        Func_020049de(20);
        Func_02004aa4(msg + 3);
        Func_02004abc(14, 0);
        Func_020049f2(10);
        Func_02004a8a(0, 3);
        Func_02004a00_a(30);
        Func_02004ae4(0, 0x4000, 0);
        Func_02004a12(30);
        Func_02004a9c(16, 0, 0);
        Func_02004b4c(205, 3);
        Func_02004ab4_a(0, 1);
        Func_02004a4c_b(205, 0);
        Func_02004a0a((s32)&Value_00000f31);
    }
}

void FieldScene_RunScene3b8_02003d40(void)
{
    u32 i;
    s32 record;
    u8 *p5;

    p5 = *(volatile s32 *)Data_03001ebc;
    Func_02008128();
    Func_02008296(158);
    Call3_02003d40(Func_02008164, 0, 0x8000, 0x4000);
    Func_020081c4(0, 2);
    if (*(s16 *)(((s32)p5 + 0x16c)) == 32) {
        Func_020080e0(1);
        Func_02008156(10);
        Call3_02003d40(Func_020081d2, 0, 0, -16);
    } else {
        if (*(s16 *)(((s32)p5 + 0x16c)) == 30) {
            Func_020080fe(4);
            Func_02008174(10);
            Call3_02003d40(Func_020081e8, 0, 3, -16);
        } else {
            Func_02008118(2);
            Func_0200818e(10);
            Call3_02003d40(Func_02008202, 0, 3, -16);
        }
    }
    Func_020081a0(16);
    Func_020082b6(*(s16 *)(((s32)p5 + 0x16c)));
    Func_0200814c(1);
    Func_02008152(2);
    Func_02008158(4);
    Func_020081d4();
}
#undef FieldScene_RunScene3b8_02000264
#undef FieldScene_RunScene3b8_0200049c
#undef Dialogue_RunChoiceSequence22ab
#undef Dialogue_RunChoiceSequence2352
#undef FieldScene_RunScene3b8_02003d40

/* overlays/scene/dialogue/choice/scene_setup.c */
#define Scene_RunBranchedSteps1FF1 Func_020000c8
#define Scene_RunBranchedSteps2006 Func_02000348
#define Scene_RunStepWithValue29e0 Func_0200400c
#define SceneData_SelectTableD004ByStateAndFlags Func_02004034
extern u8 Value_00001ff1;
extern u8 Value_00002006;
extern u8 Value_00000105;
extern s16 Data_02000240[];
extern u8 Value_0000008b;
extern u8 Data_0200dad8[];
extern u8 Data_0200da48[];
extern u8 Data_0200d9e8[];
extern u8 Data_0200d688[];
extern u8 Data_0200d394[];
extern u8 Data_0200d004[];

void Func_0200456a(s32);
s32 Func_0200457a(s32, s32);
s32 Func_020044da(s32, s32);
void Func_02004584(s32);
void Func_0200458c(s32);
s32 Func_020045a4(s32, s32);
void Func_020047ea(s32);
s32 Func_020047fa(s32, s32);
s32 Func_0200475a_a(s32, s32);
void Func_02004744(s32);
void Func_02004830(s32, s32, s32);
void Func_02004816(s32);
void Func_0200475e(s32);
void Func_02004848(s32, s32, s32);
void Func_0200482e(s32);
void Func_02004846(s32, s32);
void Func_020083f0(void);
void Func_02008398(s32, s32);
void Func_020083a0(s32, s32);
void Func_0200840c(void);
s32 Func_020083ee(s32);
s32 Func_020083fc(s32);
s32 Func_02008410(s32);
s32 Func_0200841e(s32);

/* Contiguous unnamed leaf-owner run for resource_3b8. */

void Scene_RunBranchedSteps1FF1(s32 a)
{
    s32 k = (s32)&Value_00001ff1;

    Func_0200456a(k);
    Func_0200457a(a, 0);
    if (Func_020044da(0, 0) == 0)
        Func_02004584(k + 1);
    else
        Func_0200458c(k + 2);
    Func_020045a4(a, 0);
}

void Scene_RunBranchedSteps2006(s32 a)
{
    s32 k = (s32)&Value_00002006;

    Func_020047ea(k);
    Func_020047fa(a, 0);
    if (Func_0200475a_a(0, 0) == 0) {
        Func_02004744(10);
        Func_02004830(a, 0x102, 0x28);
        Func_02004816(k + 1);
    } else {
        Func_0200475e(10);
        Func_02004848(a, (s32)&Value_00000105, 0x28);
        Func_0200482e(k + 2);
    }
    Func_02004846(a, 0);
}

void Scene_RunStepWithValue29e0(void)
{
    Func_020083f0();
    Func_02008398(0x947, 1);
    Func_020083a0(0x29e0, 1);
    Func_0200840c();
}

s32 SceneData_SelectTableD004ByStateAndFlags(void)
{
    if (Data_02000240[224] == (s32)&Value_0000008b) {
        if (Func_020083ee(0x950) != 0) {
            return (s32)Data_0200dad8;
        }
        if (Func_020083fc(0x962) != 0) {
            return (s32)Data_0200da48;
        }
        return (s32)Data_0200d9e8;
    }
    if (Func_02008410(0x950) != 0) {
        return (s32)Data_0200d688;
    }
    if (Func_0200841e(0x962) != 0) {
        return (s32)Data_0200d394;
    }
    return (s32)Data_0200d004;
}
#undef Scene_RunBranchedSteps1FF1
#undef Scene_RunBranchedSteps2006
#undef Scene_RunStepWithValue29e0
#undef SceneData_SelectTableD004ByStateAndFlags
