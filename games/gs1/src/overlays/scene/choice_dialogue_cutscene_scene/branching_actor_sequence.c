#include "types.h"

/* DRAFT for FieldScene_RunBranchingActorSequence
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

#define FieldScene_RunBranchingActorSequence Func_020017e8

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
