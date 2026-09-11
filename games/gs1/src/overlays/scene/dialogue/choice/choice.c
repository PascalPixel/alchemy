#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/dialogue/choice/choice.h"

/* overlays/scene/dialogue/choice/branching_actor_sequence.c */
/* DRAFT for Scene_RunBranchingActorSequence
 * (resource_3b8:0x020017e8, complete owner extent 2092 bytes).
 *
 * A scripted field scene driving four presentation slots (0..3). It opens the
 * scene, installs slot 0's record, seats the three visible slots, brings the
 * layers up, then runs two counted step loops that publish a step index into
 * the scene workspace. From there it plays a long alternating motion/pause
 * script. Three selector points steer it: two identical queries through
 * Talk_Run109(-1, 0), and a third reuse of the second query's answer held in
 * `pick`. Each taken branch advances the scene script counter in the state
 * record at offset 0x1d8 - by one for the first two branches, by two for the
 * third. The tail spins on the input word at gIw, restores the
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
 * spelled here as Value - its true runtime address - so the draft
 * compiles and scores standalone. When this owner gets a translation unit,
 * rename it to Talk_Run110 and declare
 *   "Talk_Run110": { "address": "0x0200c488", "kind": "thumb" }
 * in that unit's absolute_symbols; the emitted bytes are the same either way.
 *
 * Call3, Value2 and SetHalf are a modeling hypothesis, not recovered history.
 * The reference materializes each call's argument constants independently - it
 * builds 0x680000 twice for Talk_Run111(1, ..., ...) - and writes each
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

void Talk_Run112();    /* main 0x080091e0 */
void Talk_Run113();    /* main 0x080000c0, frame wait */
void Talk_Run114();    /* main 0x0808a018, scene open */
s32 Talk_Run115();     /* main 0x0808a080, record lookup */
void Talk_Run116();    /* main 0x0808a100 */
void Talk_Run111();    /* main 0x0808a0f0 */
void Talk_Run117();    /* main 0x0808a170 */
void Talk_Run118();    /* main 0x0808a010, script pause */
void Talk_Run119();    /* main 0x0808a1b8 */
void Talk_Run120();    /* main 0x0808a138 */
void Talk_Run121();    /* main 0x0808a300 */
void Talk_Run122();    /* main 0x0808a360 */
void Talk_Run123();    /* main 0x0808a090 */
void Talk_Run124();    /* main 0x0808a370 */
void Value();   /* main 0x0808a148; see the call-name note above */
void Talk_Run125();    /* main 0x0808a1e8 */
void Talk_Run126();    /* main 0x0808a180 */
void Talk_Run127();    /* main 0x0808a110 */
void Talk_Run128();    /* main 0x0808a580 */
void Talk_Run129();    /* main 0x0808a178 */
void Talk_Run130();    /* main 0x0808a128 */
s32 Talk_Run109();     /* main 0x0808a070, branch selector */
void Talk_Run131();    /* main 0x0808a150 */
void Talk_Run132();    /* main 0x0808a0b8 */
void Talk_Run133();    /* main 0x0808a0e8 */
void Talk_Run134();    /* main 0x0808a020, scene close */

/* Argument and store shaping helpers; see the header note. */

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

extern u8 *gWork[];
extern s32 gIw;

void Scene_RunBranchingActorSequence(void)
{
    s32 record;
    s32 step;
    s32 pick;
    s32 state;
    u8 *work;

    Talk_Run114();
    Talk_Run117(0x228c);
    Talk_Run116(0, 31);
    record = Talk_Run115(0);
    Talk_Run112(record, 0);
    Talk_Place(1, 0x680000, 0x680000);
    Talk_Place(3, 0x580000, 0x780000);
    Talk_Place(2, 0x780000, 0x780000);
    Talk_Place2(1, 0x4000, 0);
    Talk_Run119(3, 0, 0);
    Talk_Place2(2, 0x8000, 0);
    state = (s32)gWork[0];
    *(s32 *)(state + 0x1c0) = 0x100;
    *(s32 *)(state + 0x1c8) = 12;
    Talk_Run113(1);
    Talk_Run121(9);
    work = gWork[4];
    SetHalf((u16 *)(work + 0x52a), 0);
    SetHalf((u16 *)(work + 0x534), 0x1f1f);
    SetHalf((u16 *)(work + 0x536), 1);
    Talk_Run122();
    Talk_Run124();
    for (step = 1; step <= 5; step++) {
        Talk_Run113(3);
        *(u16 *)(work + 0x52a) = step;
    }
    Talk_Run118(40);
    Talk_Run120(0, 2);
    Talk_Run118(30);
    for (step = 5; step <= 31; step++) {
        Talk_Run113(3);
        *(u16 *)(work + 0x52a) = step;
    }
    SetHalf((u16 *)(work + 0x536), 31);
    state = (s32)gWork[0];
    *(s32 *)(state + 0x1c0) = 0x209;
    *(s32 *)(state + 0x1c8) = 24;
    Talk_Run118(20);
    Talk_Place3(1, 0x100, 50);
    Value(1, 0, 40);
    Talk_Run120(1, 2);
    Talk_Run118(30);
    Talk_Run118(10);
    Talk_Place3(2, 0x101, 40);
    Talk_Place2(2, 0xe000, 0);
    Talk_Run118(30);
    Talk_Run126(2, 0);
    Talk_Run118(10);
    Talk_Run126(3, 0);
    Talk_Run119(3, 0xe000, 0);
    Talk_Run118(30);
    Talk_Run120(3, 2);
    Talk_Run118(30);
    Talk_Place4(3, 0x10000, 0x8000);
    Talk_Place5(3, 0, -40);
    Talk_Run128(3, 32, 0);
    Talk_Run119(3, 0x2000, 0);
    Talk_Run118(10);
    Talk_Run126(3, 0);
    record = Talk_Run115(0);
    *(s32 *)(record + 16) += -0x30000;
    record = Talk_Run115(0);
    *(s32 *)(record + 64) += -0x30000;
    Talk_Run116(0, 32);
    Talk_Run118(40);
    Talk_Run127(0, 34);
    Talk_Run118(30);
    Talk_Run116(0, 33);
    Talk_Run118(40);
    Talk_Run127(1, 4);
    Talk_Run118(20);
    Talk_Run126(1, 0);
    Talk_Run118(10);
    Talk_Place3(0, 0x102, 80);
    Talk_Place3(2, 0x100, 50);
    Talk_Run(2, 0);
    if (Talk_Run109(-1, 0) == 0) {
        Talk_Run118(10);
        Talk_Run127(0, 33);
        Talk_Run118(20);
        Talk_Place3(1, 0x103, 40);
        Talk_Run130(1, 4, 13);
        Talk_Run130(1, 4, 30);
        Talk_Run126(1, 0);
        *(u16 *)(gWork[0] + 0x1d8) += 1;
    } else {
        Talk_Run118(10);
        Talk_Run127(0, 34);
        Talk_Run118(20);
        Talk_Place3(1, 0x103, 40);
        Talk_Run130(1, 4, 13);
        Talk_Run130(1, 4, 30);
        *(u16 *)(gWork[0] + 0x1d8) += 1;
        Talk_Run126(1, 0);
    }
    Talk_Run118(10);
    Talk_Run119(2, 0xa000, 0);
    Talk_Run118(30);
    Talk_Run127(2, 4);
    Talk_Run118(20);
    Talk_Place2(1, 0x2000, 0);
    Talk_Run118(30);
    Talk_Run120(1, 2);
    Talk_Run118(40);
    Talk_Place2(1, 0xe000, 0);
    Talk_Run119(2, 0xe000, 0);
    Talk_Run118(30);
    Talk_Run127(1, 3);
    Talk_Run118(30);
    Talk_Run126(1, 0);
    Talk_Run118(10);
    Talk_Run119(3, 0x6000, 0);
    Talk_Run118(50);
    Talk_Run119(3, 0x2000, 0);
    Talk_Run118(35);
    Talk_Run125(3, 0x108, 50);
    Talk_Run118(10);
    Talk_Run127(3, 3);
    Talk_Run118(30);
    Talk_Run126(3, 0);
    Talk_Run118(10);
    Talk_Run120(2, 2);
    Talk_Run118(20);
    Talk_Run(2, 0);
    if (Talk_Run109(-1, 0) == 0) {
        Talk_Run118(10);
        Talk_Run127(0, 33);
        Talk_Run118(20);
        Talk_Place3(1, 0x107, 40);
        Talk_Run126(1, 0);
        pick = 0;
        *(u16 *)(gWork[0] + 0x1d8) += 1;
    } else {
        Talk_Run118(10);
        Talk_Run127(0, 34);
        Talk_Run118(20);
        Talk_Place3(1, 0x107, 40);
        *(u16 *)(gWork[0] + 0x1d8) += 1;
        pick = 1;
        Talk_Run126(1, 0);
    }
    Talk_Run118(10);
    Talk_Run119(2, 0xa000, 0);
    Talk_Run118(30);
    Talk_Run127(2, 4);
    Talk_Run118(20);
    Talk_Place2(1, 0x2000, 0);
    Talk_Run118(30);
    Talk_Run120(1, 2);
    Talk_Run118(40);
    Talk_Place2(1, 0xe000, 0);
    Talk_Run119(2, 0xe000, 0);
    Talk_Run118(30);
    Talk_Run127(1, 3);
    Talk_Run118(30);
    Talk_Run126(1, 0);
    Talk_Run118(10);
    Talk_Run119(3, 0x6000, 0);
    Talk_Run118(50);
    Talk_Run119(3, 0x2000, 0);
    Talk_Run118(35);
    Talk_Run125(3, 0x108, 50);
    Talk_Run118(10);
    Talk_Run127(3, 3);
    Talk_Run118(30);
    Talk_Run126(3, 0);
    if (pick == 0) {
        Talk_Run118(10);
        Talk_Run127(2, 3);
        Talk_Run118(30);
        Talk_Run126(2, 0);
        Talk_Run118(10);
        Talk_Run120(1, 2);
        Talk_Run118(30);
        Talk_Run126(1, 0);
        *(u16 *)(gWork[0] + 0x1d8) += 2;
    } else {
        *(u16 *)(gWork[0] + 0x1d8) += 2;
        Talk_Run118(10);
        Talk_Run127(2, 4);
        Talk_Run118(20);
        Talk_Run126(2, 0);
        Talk_Run118(10);
        Talk_Run120(1, 2);
        Talk_Run118(20);
        Talk_Run126(1, 0);
    }
    Talk_Run118(10);
    Talk_Run125(0, 0x102, 60);
    Talk_Run118(10);
    Talk_Run127(1, 3);
    Talk_Run118(30);
    Talk_Run126(1, 0);
    Talk_Run118(20);
    Talk_Run131(2, 3, 40);
    Talk_Place2(3, 0x2000, 0);
    Talk_Run119(2, 0xe000, 0);
    Talk_Run118(30);
    while ((gIw & 240) == 0) {
        Talk_Run113(1);
    }
    Talk_Run130(0, 6, 0);
    Talk_Place4(0, 0x1e666, 0xf333);
    Talk_Run128(0, -32, -8);
    record = Talk_Run115(0);
    Talk_Run112(record, 1);
    Talk_Run118(20);
    Talk_Place2(0, 0xc000, 0);
    Talk_Place2(3, 0x4000, 0);
    Talk_Run119(2, 0xc000, 0);
    Talk_Run118(20);
    Talk_Run118(10);
    Talk_Run120(3, 2);
    Talk_Run118(20);
    Talk_Run126(3, 0);
    Talk_Run118(10);
    Talk_Run120(1, 2);
    Talk_Run118(20);
    Talk_Run126(1, 0);
    Talk_Run118(20);
    Talk_Run127(2, 3);
    Talk_Run118(30);
    Talk_Place2(0, 0x4000, 0);
    Talk_Run119(1, 0x2000, 0);
    Talk_Run118(30);
    Talk_Run126(2, 0);
    Talk_Run118(20);
    Talk_Run116(0, 3);
    Talk_Run116(1, 3);
    Talk_Run127(3, 3);
    Talk_Run118(30);
    Talk_Place4(1, 0x13333, 0x9999);
    Talk_Place4(3, 0x13333, 0x9999);
    Talk_Place4(2, 0x13333, 0x9999);
    Talk_Run116(1, 2);
    record = Talk_Run115(0);
    if (record != 0) {
        Talk_Run132(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Talk_Run133(1);
    Talk_Run111(1, 0, 0);
    Talk_Run116(3, 2);
    record = Talk_Run115(0);
    if (record != 0) {
        Talk_Run132(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Talk_Run133(3);
    Talk_Run111(3, 0, 0);
    Talk_Run116(2, 2);
    record = Talk_Run115(0);
    if (record != 0) {
        Talk_Run132(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Talk_Run133(2);
    Talk_Run111(2, 0, 0);
    Talk_Run118(10);
    Talk_Run134();
}

/* overlays/scene/dialogue/choice/dialogue_layout.c */
extern u8 Value_000022a8;
extern u8 Value_000022a3;

void Dialogue_ShowMessage22a8Branch(s32 a)
{
    s32 k = (s32)&Value_000022a8;

    Talk_Do125(k);
    Talk_Apply(a, 0);
    if (Talk_Apply2(0, 0) == 0)
        Talk_Do126(k + 1);
    else
        Talk_Do127(k + 2);
    Talk_Apply3(a, 0);
}

void Dialogue_ShowMessage22a3Branch(s32 a)
{
    s32 k = (s32)&Value_000022a3;

    Talk_Do128(k);
    Talk_Apply4(a, 0);
    if (Talk_Run135(0, 0) == 0) {
        Talk_Do129(k + 1);
        Talk_Apply5(a, 0);
    } else {
        Talk_Do130(k + 2);
        Talk_Apply6(a, 0);
    }
}

/* overlays/scene/dialogue/choice/run_main_cutscene_sequence.c */

u8 *Talk_Run136();

u8 *Talk_Run137();

s16 *Talk_Run138();

s16 *Talk_Run139();
s16 *Talk_Run140();

s16 *Talk_Run141();

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

void Scene_RunMainCutsceneSequence(void)
{
    s32 carry;
    s16 *position;

    carry = Talk_Check29(30);
    Talk_Run142(carry);
    carry = Talk_Check(0x22c4);
    carry = Talk_Check30(carry);
    Talk_Run143(carry);
    carry = Talk_Check2(0xd80000, -1, 0x2e00000, 1);
    Talk_Run144(carry);
    Talk_Run145(20);
    Talk_Run146(10);
    Talk_Run147(20, 0);
    Talk_Run148(10);
    Talk_Place6(14, 0xcccc, 0x6666);
    Talk_Run149(14, 0, 16);
    Talk_Run150(10);
    Talk_Run151(14, 3);
    Talk_Run152(30);
    Talk_Run153(14, 0);
    Talk_Run154(20);
    Talk_Run155(20, 0);
    carry = Talk_Check31(0, 1);
    Talk_Run156(carry);
    Talk_Run157(40);
    Talk_Place7(0, 0x10000, 0x8000);
    Talk_Place8(0, 208, 0x2f8);
    Talk_Run158(10);
    Talk_Run2(0xd80000, -1, 0x2e00000, 1);
    Talk_Run3(1, -16, 16, 0xc000);
    Talk_Run4(3, 0, 24, 0xc000);
    Talk_Run5(2, 16, 16, 0xc000);
    Talk_Run159(1);
    Talk_Run160(30);
    Talk_Run161(10);
    Talk_Run162(14, 3);
    Talk_Run163(30);
    Talk_Run164(14, 0);

    if (Talk_Check3(0x951) != 0) {
        Talk_Run165(10);
        Talk_Run166(20, 3);
        Talk_Run167(20);
        Talk_Run168(20, 0);
        Talk_Run169(10);
        Talk_Run170(14, 0xa000, 0);
        Talk_Run171(30);
        Talk_Run172(14, 3);
        Talk_Run173(20);
        Talk_Run174(14, 0);
        Talk_Run175(10);
        Talk_Run176(20, 3);
        Talk_Run177(20);
        Talk_Run178(20, 0);
        Talk_Place9(20, 0xc000, 0);
        Talk_Run179(20);
        Talk_Place10(20, 0x10000, 0x8000);
        Talk_Place11(20, 0, -16);
        Talk_Run180(40);
        Talk_Run181(20, 3);
        Talk_Run182(40);
        Talk_Place12(20, 0x4000, 0);
        Talk_Run183(20);
        Talk_Run184(20, 0, 32);
        Talk_Run185(14, 0x8000, 0);
        Talk_Run186(10);
        Talk_Run187(20, 0, 0);
        Talk_Run188(30);
        Talk_Run189(20, 3);
        Talk_Run190(30);
        Talk_Place13(20, 0x4000, 0);
        Talk_Run191(20);
        Talk_Place14(20, 0xcccc, 0x6666);
        Talk_Run136(20)[90] &= 0xfe;
        Talk_Place15(20, 0, -16);
        {
            u8 *record = Talk_Run137(20);
            u8 value = *(volatile u8 *)&record[90];

            record[90] = (u8)(value | 1);
        }
        Talk_Place16(14, 0x4000, 0);
        Talk_Run192(40);
        Talk_Place17(14, 0xcccc, 0x6666);
        Talk_Run193(14, 0, 16);
        Talk_Run194(40);
        Talk_Run195(164, 2);
        Talk_Run196(-1, 0);
        Talk_Run197(164, 3);
        Talk_Check32(164, 0);
        Talk_Run198(0, 0xc000, 0);
        Talk_Run199(30);
        Talk_Place18(14, 0, -16);
        Talk_Check33(14, 0x4000, 0);
        Talk_Run200(30);
        Talk_Run201(14, 2);
        Talk_Run202(20);
        Talk_Run203(14, 0);
        bump_step(2);
    } else {
        bump_step(5);
        Talk_Run204(10);
        Talk_Run205(20, 3);
        Talk_Run206(20);
        Talk_Run207(20, 0);
        Talk_Run208(14, 0xa000, 0);
        Talk_Run209(40);
        Talk_Run210(14, 3);
        Talk_Run211(20);
        Talk_Run212(20, 3);
        Talk_Run213(30);
        Talk_Run214(14, 0x4000, 0);
        Talk_Run215(30);
        Talk_Run216(14, 2);
        Talk_Run217(20);
        Talk_Run218(14, 0);
    }

    if (Talk_Check34(0, 0) == 0) {
        Talk_Run219(20);
        Talk_Run220(20, 3);
        Talk_Run221(20);
        Talk_Run222(20, 0);
        bump_step(1);
    } else {
        Talk_Run223(10);
        Talk_Run224(20, 3);
        Talk_Run225(20);
        bump_step(1);
        Talk_Run226(20, 0);
    }

    Talk_Run227(10);
    Talk_Run228(14, 0xa000, 0);
    Talk_Run229(20);
    Talk_Run230(14, 3);
    Talk_Run231(30);
    Talk_Run232(14, 0);
    Talk_Run233(10);
    Talk_Run234(20, 3);
    Talk_Run235(30);
    Talk_Run236(10);
    Talk_Run237(1, 4, 13);
    Talk_Run238(1, 4, 30);
    Talk_Run239(1, 0);
    Talk_Run240(10);
    Talk_Place19(14, 0x4000, 0);
    Talk_Run241(20, 2);
    Talk_Run242(20);
    Talk_Place20(14, 0x19999, 0xcccc);
    Talk_Run243(20, 0, 16);
    Talk_Run244(2);
    Talk_Run245(20, 0x2000, 0);
    Talk_Run246(10);
    Talk_Run247(20, 0);
    Talk_Run248(10);
    Talk_Run249(14, 4);
    Talk_Run250(30);
    Talk_Run251(14, 20, 30);
    Talk_Run252(14, 0);
    Talk_Run253(10);
    Talk_Run254(20, 2);
    Talk_Run255(20);
    Talk_Run256(14, 1, 0);
    Talk_Run257(40);
    Talk_Run258(14, 0);
    Talk_Run259(10);
    Talk_Run260(1, 2);
    Talk_Run261(30);
    Talk_Run262(1, 2, 30);
    Talk_Run263(10);
    Talk_Run264(2, 2);
    Talk_Run265(20);
    Talk_Run266(2, 1, 30);
    Talk_Run267(2, 0);
    Talk_Run268(10);
    Talk_Run269(1, 0x106, 50);
    Talk_Run270(10);
    Talk_Run271(14, 3);
    Talk_Run272(30);
    Talk_Run273(14, 0);
    Talk_Run274(10);
    Talk_Place21(1, 0xc000, 0);
    Talk_Place22(2, 0xc000, 0);
    Talk_Run275(30);
    Talk_Run276(10);
    Talk_Run277(20, 2);
    Talk_Run278(20);
    Talk_Run279(20, 0);
    Talk_Run280(10);
    Talk_Run281(1, 0, 0);
    Talk_Run282(3, 2, 50);
    Talk_Place23(0, 0xc000, 0);
    Talk_Place24(1, 0xc000, 0);
    Talk_Place25(3, 0xc000, 0);
    Talk_Run283(2, 0xc000, 0);
    Talk_Run284(30);
    Talk_Run285(10);
    Talk_Run286(20, 4);
    Talk_Run287(20);
    Talk_Run288(20, 0);
    Talk_Run289(10);
    Talk_Run290(14, 4);
    Talk_Run291(20);
    Talk_Run292(14, 0);
    Talk_Run293(10);
    Talk_Place26(2, 0x101, 40);
    Talk_Run294(2, 0);
    Talk_Run295(10);
    Talk_Run296(14, 2);
    Talk_Run297(20);
    Talk_Check35(14, 0);
    if (Talk_Check36(0, 0) == 0) {
        Talk_Run298(20);
        Talk_Run299(14, 3);
        Talk_Run300(30);
        Talk_Run301(14, 0);
        bump_step(1);
    } else {
        Talk_Run302(10);
        Talk_Run303(14, 4);
        Talk_Run304(20);
        bump_step(1);
        Talk_Run305(14, 0);
    }

    Talk_Run306(10);
    Talk_Place27(20, 0x102, 40);
    Talk_Run307(20, 0);
    Talk_Run308(20);
    Talk_Run309(0, 3);
    Talk_Run310(1, 3);
    Talk_Run311(3, 3);
    Talk_Run312(2, 3);
    Talk_Run313(50);
    Talk_Run314(14, 20, 60);
    Talk_Run315(14, 0x4000, 0);
    Talk_Run316(20);
    Talk_Run317(14, 0);
    Talk_Run318(10);
    Talk_Place28(20, 0x102, 40);
    Talk_Run319(20, 0);
    Talk_Run320(10);
    Talk_Run321(20, 4);
    Talk_Run322(20);
    Talk_Run323(20, 0);
    Talk_Run324(10);
    Talk_Place29(0, 0x102, 40);
    Talk_Run325(10);
    Talk_Run326(14, 4);
    Talk_Run327(20);
    Talk_Run328(14, 0);
    Talk_Run329(10);
    Talk_Run330(1, 0, 50);
    Talk_Place30(0, 0xc000, 0);
    Talk_Place31(1, 0xc000, 0);
    Talk_Run331(30);
    Talk_Run332(10);
    Talk_Run333(0, 3);
    Talk_Run334(1, 3);
    Talk_Run335(30);
    Talk_Run336(10);
    Talk_Run337(14, 3);
    Talk_Run338(30);
    Talk_Run339(14, 0);
    Talk_Run340(10);
    Talk_Place32(3, 0x100, 40);
    Talk_Run341(3, 0);
    Talk_Run342(10);
    Talk_Place33(14, 0x102, 50);
    Talk_Run343(20, 14, 50);
    Talk_Run344(10);
    Talk_Run345(14, 2);
    Talk_Run346(20);
    Talk_Run347(14, 0);
    Talk_Run348(20);
    Talk_Run349(1, 0, 0);
    Talk_Run350(3, 2, 50);
    Talk_Place34(0, 0xc000, 0);
    Talk_Place35(1, 0xc000, 0);
    Talk_Place36(3, 0xc000, 0);
    Talk_Place37(2, 0xc000, 0);
    Talk_Run351(50);
    Talk_Run352(20, 0x2000, 0);
    Talk_Run353(20);
    Talk_Run354(20, 0);
    Talk_Run355(10);
    Talk_Run356(1, 2);
    Talk_Run357(20);
    Talk_Run358(1, 0);
    Talk_Run359(20);
    Talk_Run360(14, 0x8000, 0);
    Talk_Run361(40);
    Talk_Place38(14, 0x105, 60);
    Talk_Run362(14, 0);
    Talk_Run363(10);
    Talk_Place39(2, 0x100, 40);
    Talk_Run364(2, 0);
    Talk_Run365(10);
    Talk_Run366(14, 2, 40);
    Talk_Run367(14, 3);
    Talk_Run368(30);
    Talk_Run369(14, 0);
    Talk_Run370(10);
    Talk_Run371(1, 0, 0);
    Talk_Run372(3, 2, 60);
    Talk_Place40(0, 0xc000, 0);
    Talk_Place41(1, 0xc000, 0);
    Talk_Place42(3, 0xc000, 0);
    Talk_Run373(2, 0xc000, 0);
    Talk_Run374(50);
    Talk_Run375(20, 2);
    Talk_Run376(20);
    Talk_Run377(20, 0);
    Talk_Run378(10);
    Talk_Run379(1, 0, 40);
    Talk_Run380(1, 2);
    Talk_Run381(20);
    Talk_Check37(1, 0);
    if (Talk_Check38(0, 0) == 0) {
        Talk_Run382(20);
        Talk_Run383(1, 0);
        bump_step(3);
    } else {
        do {
            Talk_Check39(10);
        } while (0);
        {
            u8 **scene_address = (u8 **)0x03001ebc;

            bump_step_at(scene_address, 1);
            Talk_Check40(1, 0);
            if (Talk_Check41(0, 0) == 0) {
                Talk_Run384(20);
                Talk_Run385(1, 0);
                bump_step_at(scene_address, 1);
            } else {
                bump_step_at(scene_address, 1);
                Talk_Run386(1, 0);
            }
        }
    }

    Talk_Run387(10);
    Talk_Run388(14, 4);
    Talk_Run389(20);
    Talk_Run390(14, 0);
    Talk_Run391(10);
    Talk_Place43(0, 0xc000, 0);
    Talk_Place44(1, 0xc000, 0);
    Talk_Run392(40);
    Talk_Run393(1, 2);
    Talk_Run394(20);
    Talk_Run395(10);
    Talk_Run396(14, 3);
    Talk_Run397(30);
    Talk_Run398(14, 0);
    Talk_Run399(10);
    Talk_Place45(3, 0x101, 40);
    Talk_Run400(3, 0);
    Talk_Run401(10);
    Talk_Run402(14, 2);
    Talk_Run403(20);
    Talk_Run404(14, 0);
    Talk_Run405(10);
    Talk_Run406(2, 2);
    Talk_Run407(20);
    Talk_Run408(2, 0);
    Talk_Run409(10);
    Talk_Run410(20, 14, 40);
    Talk_Run411(20, 2);
    Talk_Run412(20);
    Talk_Run413(20, 0);
    Talk_Run414(20);
    Talk_Place46(14, 0x105, 70);
    Talk_Run415(14, 0);
    Talk_Run416(10);
    Talk_Place47(0, 0x101, 0);
    Talk_Place48(1, 0x101, 0);
    Talk_Place49(3, 0x101, 0);
    Talk_Place50(2, 0x101, 40);
    Talk_Run417(10);
    Talk_Run418(14, 4);
    Talk_Run419(20);
    Talk_Run420(14, 0);
    Talk_Run421(10);
    Talk_Run422(14, 3);
    Talk_Run423(30);
    Talk_Run424(14, 0);
    Talk_Run425(20);
    Talk_Run426(1, 3);
    Talk_Run427(30);
    Talk_Run428(1, 0);
    Talk_Run429(20);
    Talk_Run430(14, 3);
    Talk_Run431(30);
    Talk_Run432(10);
    Talk_Place51(2, 0x101, 40);
    Talk_Run433(2, 0);
    Talk_Run434(10);
    Talk_Run435(14, 3);
    Talk_Run436(40);
    Talk_Run437(0, 2, 0);
    Talk_Run438(1, 2, 0);
    Talk_Run439(3, 2, 0);
    Talk_Run440(20, 2, 0);
    Talk_Run441(50);
    Talk_Run442(10);
    Talk_Run443(2, 2);
    Talk_Run444(20);
    Talk_Run445(2, 0);
    Talk_Run446(10);
    Talk_Place52(14, 0x101, 40);
    Talk_Run447(14, 0);
    Talk_Run448(10);
    Talk_Run449(0, 3);
    Talk_Run450(1, 3);
    Talk_Run451(3, 3);
    Talk_Run452(30);
    Talk_Run453(10);
    Talk_Run454(2, 4);
    Talk_Run455(20);
    Talk_Run456(2, 0);
    Talk_Run457(20);
    Talk_Run458(20, 3);
    Talk_Run459(30);
    Talk_Run460(20, 14, 30);
    Talk_Place53(0, 0xc000, 0);
    Talk_Place54(1, 0xc000, 0);
    Talk_Place55(3, 0xc000, 0);
    Talk_Run461(20);
    Talk_Run462(20, 0);
    Talk_Run463(10);
    Talk_Run464(14, 3);
    Talk_Run465(30);
    Talk_Run466(14, 0);
    Talk_Run467(10);
    Talk_Run468(0, 2);
    Talk_Run469(1, 2);
    Talk_Run470(3, 2);
    Talk_Run471(2, 2);
    Talk_Run472(30);
    Talk_Run473(10);
    Talk_Run474(20, 2);
    Talk_Run475(20);
    Talk_Run476(20, 0);
    Talk_Run477(10);
    Talk_Place56(14, 0x8000, 0);
    Talk_Run478(20);
    Talk_Run479(14, 4);
    Talk_Run480(20);
    Talk_Run481(14, 0);
    Talk_Run482(10);
    Talk_Run483(1, 0, 0);
    Talk_Run484(3, 2, 50);
    Talk_Place57(0, 0xc000, 0);
    Talk_Place58(1, 0xc000, 0);
    Talk_Place59(3, 0xc000, 0);
    Talk_Place60(2, 0xc000, 0);
    Talk_Run485(40);
    Talk_Place61(14, 0x4000, 0);
    Talk_Run486(30);
    Talk_Place62(14, 0x102, 40);
    Talk_Run487(14, 0);
    Talk_Run488(10);
    Talk_Place63(20, 0x102, 40);
    Talk_Run489(20, 0);
    Talk_Run490(20);
    Talk_Run491(14, 20, 40);
    Talk_Run492(14, 2);
    Talk_Run493(20);
    Talk_Run494(14, 0);
    Talk_Run495(10);
    Talk_Run6(20, 0x102);
    Talk_Run496(20, 2);
    Talk_Run497(20);
    Talk_Run498(20, 0);
    Talk_Place64(20, 0x19999, 0xcccc);
    Talk_Run499(20, 0, 24);
    Talk_Run500(0, 20, 0);
    Talk_Run501(1, 20, 0);
    Talk_Run502(3, 20, 0);
    Talk_Run503(2, 20, 0);
    Talk_Place65(14, 0x4000, 0);
    Talk_Run504(14, 0);
    Talk_Run505(10);
    Talk_Place66(20, 0x100, 60);
    Talk_Run506(20, 14, 0);
    Talk_Place67(0, 0xc000, 0);
    Talk_Place68(1, 0xc000, 0);
    Talk_Place69(3, 0xc000, 0);
    Talk_Place70(2, 0xc000, 0);
    Talk_Run507(10);
    Talk_Run508(20, 2);
    Talk_Run509(20);
    Talk_Run510(20, 0);
    Talk_Run511(30);
    Talk_Place71(14, 0x105, 40);
    Talk_Run512(14, 0);
    Talk_Run513(10);
    Talk_Place72(20, 0x101, 60);
    Talk_Place73(20, 0x13333, 0x9999);
    Talk_Place74(20, 0, -24);
    Talk_Run514(20, 0, 0);
    Talk_Run515(20);
    Talk_Run516(20, 0);
    Talk_Run517(10);
    Talk_Place75(14, 0x8000, 0);
    Talk_Run518(20);
    Talk_Run519(14, 3);
    Talk_Run520(30);
    Talk_Run521(10);
    Talk_Run522(20, 2);
    Talk_Run523(20);
    Talk_Run524(20, 0);
    Talk_Run525(10);
    Talk_Run526(14, 4);
    Talk_Run527(20);
    Talk_Run528(14, 0);
    Talk_Run529(10);
    Talk_Place76(20, 0x102, 40);
    Talk_Run530(20, 0);
    Talk_Run531(10);
    Talk_Run532(14, 3);
    Talk_Run533(30);
    Talk_Run534(14, 0);
    Talk_Run535(10);
    Talk_Run536(1, 2);
    Talk_Run537(20);
    Talk_Run538(1, 0);
    Talk_Run539(10);
    Talk_Place77(14, 0x4000, 0);
    Talk_Run540(30);
    Talk_Run541(14, 3);
    Talk_Run542(30);
    Talk_Run543(14, 0);
    Talk_Run544(10);
    Talk_Place78(3, 0x102, 40);
    Talk_Run545(3, 0);
    Talk_Run546(10);
    Talk_Run547(14, 3);
    Talk_Run548(30);
    Talk_Run549(14, 0);
    Talk_Run550(10);
    Talk_Run551(2, 3);
    Talk_Run552(30);
    Talk_Run553(2, 0);
    Talk_Run554(10);
    Talk_Run555(14, 3);
    Talk_Run556(30);
    Talk_Run557(14, 0);
    Talk_Run558(10);
    Talk_Run559(1, 0, 0);
    Talk_Run560(3, 2, 50);
    Talk_Place79(0, 0xc000, 0);
    Talk_Place80(1, 0xc000, 0);
    Talk_Place81(3, 0xc000, 0);
    Talk_Place82(2, 0xc000, 0);
    Talk_Run561(30);
    Talk_Run562(10);
    Talk_Run563(14, 2);
    Talk_Run564(20);
    Talk_Run565(14, 0);
    Talk_Run566(20);
    Talk_Place83(0, 0x101, 0);
    Talk_Place84(1, 0x101, 0);
    Talk_Place85(3, 0x101, 0);
    Talk_Place86(2, 0x101, 50);
    Talk_Run567(10);
    Talk_Run568(14, 3);
    Talk_Run569(30);
    Talk_Run570(14, 0);
    Talk_Run571(10);
    Talk_Place87(20, 0x102, 40);
    Talk_Run572(20, 0);
    Talk_Run573(10);
    Talk_Run574(14, 20, 30);
    Talk_Run575(14, 0);
    Talk_Run576(10);
    Talk_Run577(14, 4);
    Talk_Run578(20);
    Talk_Run579(14, 0);
    Talk_Run580(10);
    Talk_Run581(1, 2);
    Talk_Run582(20);
    Talk_Run583(1, 0);
    Talk_Run584(10);
    Talk_Run585(14, 2);
    Talk_Run586(20);
    Talk_Place88(14, 0x4000, 0);
    Talk_Run587(20);
    Talk_Run588(14, 0);
    Talk_Run589(10);
    Talk_Place89(20, 0x101, 40);
    Talk_Run590(20, 0);
    Talk_Run591(10);
    Talk_Run592(14, 3);
    Talk_Run593(30);
    Talk_Run594(14, 0x8000, 0);
    Talk_Run595(20);
    Talk_Run596(14, 0);
    Talk_Run597(10);
    Talk_Place90(2, 0x101, 40);
    Talk_Run598(2, 0);
    Talk_Run599(10);
    Talk_Place91(14, 0x4000, 0);
    Talk_Run600(20, 0x2000, 0);
    Talk_Run601(30);
    Talk_Run602(14, 3);
    Talk_Run603(30);
    Talk_Run604(10);
    Talk_Run605(3, 2);
    Talk_Run606(20);
    Talk_Run607(3, 0);
    Talk_Run608(10);
    Talk_Place92(14, 0x102, 40);
    Talk_Run609(14, 0);
    Talk_Run610(20);
    Talk_Run611(0, 3);
    Talk_Run612(1, 3);
    Talk_Run613(3, 3);
    Talk_Run614(2, 3);
    Talk_Run615(30);
    Talk_Run616(20);
    Talk_Place93(14, 0x105, 60);
    Talk_Run617(14, 0);
    Talk_Run618(10);
    Talk_Place94(1, 0x101, 40);
    Talk_Run619(1, 0);
    Talk_Run620(10);
    Talk_Place95(14, 0x100, 40);
    Talk_Run621(14, 0);
    Talk_Check42(10);
    Talk_Place96(1, 0, 50);
    Talk_Place97(0, 0xc000, 0);
    Talk_Place98(1, 0xc000, 0);
    Talk_Run622(30);
    Talk_Run623(10);
    Talk_Run624(14, 3);
    Talk_Check43(30);
    Talk_Run625(14, 0);
    Talk_Run626(10);
    Talk_Place99(3, 0x101, 40);
    Talk_Run627(3, 0);
    Talk_Run628(10);
    Talk_Check44(14, 4);
    Talk_Run629(20);
    Talk_Run630(14, 0);
    Talk_Run631(10);
    Talk_Place100(0, 0x101, 0);
    Talk_Place101(1, 0x101, 0);
    Talk_Place102(2, 0x101, 0);
    Talk_Place103(3, 0x101, 40);
    Talk_Run632(10);
    Talk_Run633(14, 4);
    Talk_Run634(20);
    Talk_Run635(14, 0);
    Talk_Run636(10);
    Talk_Place104(2, 0x100, 40);
    Talk_Check45(2, 0);
    if (Talk_Check46(14, 0) == 0) {
        Talk_Check47(10);
        Talk_Run637(2, 0);
        bump_step(1);
    } else {
        Talk_Run638(10);
        bump_step(1);
        Talk_Run639(2, 0);
    }

    Talk_Run640(10);
    Talk_Run641(14, 2);
    Talk_Run642(20);
    Talk_Run643(14, 0);
    Talk_Run644(10);
    Talk_Run645(14, 4);
    Talk_Run646(20);
    Talk_Run647(14, 0);
    Talk_Run648(10);
    Talk_Run649(20, 2);
    Talk_Check48(20);
    Talk_Run650(20, 0, 0);
    Talk_Run651(20);
    Talk_Run652(20, 0);
    Talk_Run653(10);
    Talk_Run654(14, 20, 40);
    Talk_Run655(14, 4);
    Talk_Run656(30);
    Talk_Run657(14, 0);
    Talk_Run658(10);
    Talk_Run659(1, 0, 40);
    Talk_Run660(1, 0);
    Talk_Place105(14, 0x4000, 0);
    Talk_Place106(20, 0x2000, 0);
    Talk_Run661(30);
    Talk_Run662(14, 0);
    Talk_Run663(10);
    Talk_Place107(0, 0xc000, 0);
    Talk_Place108(1, 0xc000, 0);
    Talk_Run664(30);
    Talk_Run665(3, 2, 60);
    Talk_Place109(3, 0xc000, 0);
    Talk_Run666(2, 0xc000, 0);
    Talk_Run667(30);
    Talk_Run668(3, 0);
    Talk_Run669(10);
    Talk_Run670(2, 4);
    Talk_Run671(20);
    Talk_Run672(2, 0);
    Talk_Run673(10);
    Talk_Run674(20, 14, 60);
    Talk_Place110(20, 0x2000, 0);
    Talk_Place111(14, 0x4000, 0);
    Talk_Run675(30);
    Talk_Run676(14, 0x4000, 0);
    Talk_Run677(10);
    Talk_Run678(14, 3);
    Talk_Run679(30);
    Talk_Run680(14, 0);
    Talk_Run681(10);
    Talk_Run682(20, 2);
    Talk_Run683(20);
    Talk_Check49(20, 0);
    if (Talk_Check50(0, 0) == 0) {
        Talk_Run684(20);
        Talk_Place112(1, 0x102, 40);
        Talk_Run685(1, 0);
        bump_step(1);
    } else {
        Talk_Run686(10);
        bump_step(1);
        Talk_Run687(1, 0);
    }

    Talk_Run688(10);
    Talk_Run689(20, 2);
    Talk_Run690(20);
    Talk_Run691(20, 0);
    Talk_Run692(10);
    Talk_Place113(20, 0x100, 40);
    Talk_Run693(20, 0);
    Talk_Run694(20);
    Talk_Run695(0, 3);
    Talk_Run696(1, 3);
    Talk_Run697(3, 3);
    Talk_Run698(2, 3);
    Talk_Run699(30);
    Talk_Run700(10);
    Talk_Place114(14, 0x100, 40);
    Talk_Run701(14, 0);
    Talk_Run702(10);
    Talk_Run703(20, 14, 40);
    Talk_Run704(10);
    Talk_Place115(20, 0x102, 40);
    Talk_Run705(20, 0);
    Talk_Run706(10);
    Talk_Run707(14, 20, 40);
    Talk_Run708(14, 3);
    Talk_Run709(30);
    Talk_Run710(14, 0);
    Talk_Run711(10);
    Talk_Run712(20, 2);
    Talk_Run713(20);
    Talk_Run714(20, 0);
    Talk_Run715(14, 0x4000, 0);
    Talk_Run716(20);
    Talk_Run717(14, 2);
    Talk_Run718(20);
    Talk_Run719(14, 0);
    Talk_Run720(10);
    Talk_Place116(0, 0x101, 80);
    Talk_Place117(20, 0x102, 70);
    Talk_Run721(14, 4);
    Talk_Run722(20);
    Talk_Run723(14, 0);
    Talk_Run724(10);
    Talk_Run725(1, 0, 0);
    Talk_Run726(3, 2, 50);
    Talk_Place118(0, 0xc000, 0);
    Talk_Place119(1, 0xc000, 0);
    Talk_Place120(3, 0xc000, 0);
    Talk_Run727(2, 0xc000, 0);
    Talk_Run728(30);
    Talk_Run729(10);
    Talk_Place121(20, 0x106, 50);
    Talk_Run730(20, 0);
    Talk_Run731(10);
    Talk_Run732(14, 3);
    Talk_Run733(30);
    Talk_Place122(20, 0x2000, 0);
    Talk_Run734(20, 0);
    Talk_Run735(10);
    Talk_Place123(14, 0x108, 40);
    Talk_Check51(14, 0);
    if (Talk_Check52(0, 0) == 0) {
        Talk_Run736(20);
        Talk_Place124(14, 0x100, 40);
        Talk_Run737(14, 0);
        bump_step(1);
    } else {
        Talk_Run738(10);
        Talk_Place125(14, 0x100, 40);
        bump_step(1);
        Talk_Run739(14, 0);
    }

    Talk_Run740(10);
    Talk_Run741(20, 2);
    Talk_Run742(20);
    Talk_Run743(20, 0);
    Talk_Run744(10);
    Talk_Run745(14, 20, 40);
    Talk_Run746(14, 3);
    Talk_Run747(30);
    Talk_Run748(20, 0);
    Talk_Run749(10);
    Talk_Run750(20, 14, 40);
    Talk_Run751(20, 0);
    Talk_Run752(10);
    Talk_Place126(14, 0x100, 40);
    Talk_Place127(14, 0x8000, 0);
    Talk_Run753(20);
    Talk_Run754(14, 0);
    Talk_Run755(10);
    Talk_Place128(20, 0x100, 40);
    Talk_Run756(20, 0);
    Talk_Run757(10);
    Talk_Run758(14, 2);
    Talk_Run759(20);
    Talk_Run760(14, 0);
    Talk_Run761(10);
    Talk_Run762(20, 2);
    Talk_Run763(20);
    Talk_Run764(20, 0);
    Talk_Run765(10);
    Talk_Run766(20, 3);
    Talk_Run767(30);
    Talk_Run768(14, 3);
    Talk_Run769(30);
    Talk_Run770(20, 0x2000, 0);
    Talk_Run771(20);
    Talk_Run772(0, 20);
    Talk_Run773(1, 20);
    Talk_Run774(3, 20);
    Talk_Run775(2, 20);
    Talk_Place129(20, 0x10000, 0x8000);
    Talk_Run776(20, 0, 32);
    Talk_Run777(20, 0, 0);
    Talk_Place130(14, 0x4000, 0);
    Talk_Run778(20);
    Talk_Run779(20, 0);
    Talk_Run780(10);
    Talk_Run781(20, 3);
    Talk_Run782(30);
    Talk_Run783(1, 16, 0);
    Talk_Run784(20, 0, 80);
    Talk_Run785(40);
    Talk_Place131(1, -16, 0);
    Talk_Place132(1, 0x4000, 0);
    Talk_Place133(3, 0x4000, 0);
    Talk_Run786(20);
    Talk_Run787(80);
    Talk_Run788(20, 0, 0);
    Talk_Place134(0, 0xc000, 0);
    Talk_Place135(1, 0xc000, 0);
    Talk_Place136(3, 0xc000, 0);
    Talk_Run789(2, 0xc000, 0);
    Talk_Run790(30);
    Talk_Run791(0, 14);
    Talk_Run792(1, 14);
    Talk_Run793(3, 14);
    Talk_Run794(2, 14);
    Talk_Run795(30);
    Talk_Run796(14, 3);
    Talk_Run797(30);
    Talk_Place137(14, 0xcccc, 0x6666);
    Talk_Run798(14, 0, 24);
    Talk_Run799(14, -80, 0);
    Talk_Run800(10);
    Talk_Run801(14, 0, 0);
    Talk_Run802(20);
    Talk_Run803(14, 0);
    Talk_Run804(20);
    Talk_Run805(14, 0x4000, 0);
    Talk_Run806(20);
    Talk_Place138(14, 0, 48);
    Talk_Run807(14, -64, 0);
    Talk_Run808(0);
    Talk_Run809(1);
    Talk_Run810(3);
    Talk_Run811(2);
    Talk_Run812(14, 0, 0);
    Talk_Run813(20);
    Talk_Run814(0, 3, 0);
    Talk_Run815(1, 2, 0);
    Talk_Run816(30);
    Talk_Run817(0, 3);
    Talk_Run818(1, 3);
    Talk_Run819(3, 3);
    Talk_Run139(2, 3);
    Talk_Run820(30);
    Talk_Run821(17);
    Talk_Place139(1, 0x13333, 0x9999);
    Talk_Place140(2, 0x13333, 0x9999);
    Talk_Place141(3, 0x13333, 0x9999);
    Talk_Check53(1, 2);
    position = Talk_Run138(0);
    if (position != 0)
        Talk_Run822(1, position[5], position[9]);
    Talk_Run823(1);
    Talk_Run824(1, 0, 0);
    Talk_Check54(2, 2);
    position = Talk_Run140(0);
    if (position != 0)
        Talk_Run825(2, position[5], position[9]);
    Talk_Run826(2);
    Talk_Run827(2, 0, 0);
    Talk_Check55(3, 2);
    position = Talk_Run141(0);
    if (position != 0)
        Talk_Run828(3, position[5], position[9]);
    Talk_Run829(3);
    Talk_Check56(3, 0, 0);
    carry = Talk_Check57(10);
    carry = Talk_Check58(carry);
    Talk_Run830(carry);
}

/* overlays/scene/dialogue/choice/run_supplemental_sequence_one.c */
extern u8 Value_00002233;

/* Supplemental scene sequence of the resource_3b8 overlay. */

void RunSupplementalSequenceOne(void)
{
    s32 p;
    Talk_Do(2412);
    Talk_Run7();
    Talk_Run8();
    Talk_Place142(8, 20480, 0);
    Talk_Place143(9, 12288, 0);
    Talk_Place144(0, 200, 272);
    Talk_Place145(0, 49152, 0);
    Talk_Do2(20);
    p = (s32)&Value_00002233;
    Talk_Run831(p);
    Talk_Check4(8, 0);
    if (Talk_Check5(0, 0) == 0) {
        Talk_Do3(20);
        Talk_Run832(p + 1);
        Talk_Run9(8, 0);
    } else {
        Talk_Check6(20);
        Talk_Run833(p + 2);
        Talk_Run10(8, 0);
        Talk_Do4(20);
        Talk_Place146(8, 9, 60);
        Talk_Place147(9, 12288, 0);
        Talk_Do5(40);
        Talk_Run11(9, 2);
        Talk_Do6(30);
        Talk_Place148(8, 9, 30);
        Talk_Run12(9, 3);
        Talk_Do7(30);
        Talk_Place149(8, 258, 50);
        Talk_Place150(8, 20480, 0);
        Talk_Place151(9, 12288, 0);
        Talk_Do8(20);
        Talk_Run13(8, 4);
        Talk_Do9(20);
        Talk_Run14(8, 0);
        Talk_Do10(10);
        Talk_Run15(8, 2);
        Talk_Do11(20);
        Talk_Run16(8, 0);
    }
    Talk_Run17();
}

/* overlays/scene/dialogue/choice/scene_data.c */
extern u8 Value_0000008b;
extern u8 gOv[];
extern u8 gOv2[];
extern u8 gOv3[];
extern u8 gOv4[];
extern u8 gOv5[];
extern u8 gOv6[];
extern u8 gOv7[];
extern u8 gOv8[];

/* Contiguous unnamed leaf-owner run for resource_3b8. */

s32 SceneData_SelectTablec614ByState(void)
{
    extern s16 gCell[];

    if (gCell[224] == (s32)&Value_0000008b) {
        return (s32)gOv;
    }
    return (s32)gOv2;
}

s32 SceneData_GetTableca7c(void)
{
    extern s16 gCell[];

    return (s32)gOv3;
}

s32 SceneData_GetTableca8c(void)
{
    extern s16 gCell[];

    return (s32)gOv4;
}

s32 SceneData_SelectDataBySelectorAndFlags(void)
{
    extern u8 gCell[];

    s16 *tbl = (s16 *)gCell;

    if (tbl[0xe0] == (s32)&Value_0000008b)
        return (s32)gOv5;
    if (Talk_Check59(0x950) != 0)
        return (s32)gOv6;
    if (Talk_Check60(0x962) != 0)
        return (s32)gOv7;
    return (s32)gOv8;
}

/* overlays/scene/dialogue/choice/scene_dialogue.c */
extern u8 gWork[];

void Dialogue_ShowLine2239Or223A(void)
{
    u32 i;
    s32 record;

    if (Talk_Check7(0x96d) == 0) {
        Talk_Do12(0x96d);
        Talk_Do13(0x2239);
        Talk_Run834(9, 0);
    } else {
        Talk_Do14(0x223a);
        Talk_Run835(9, 0);
    }
}

/* overlays/scene/dialogue/choice/scene_effect_sequence_head.c */
extern u8 Value_00001ff8;
extern u8 gOv9[];

u8 *Talk_Run836();

/* Primary effect sequence of the resource_3b8 overlay: two hundred and fifty scene calls with constant arguments. */

void RunOpeningAuxiliarySequence(s32 a)
{
    void Talk_Run837();

    u8 *ret;
    s16 v;
    s32 c;
    s32 t;

    ret = Talk_Run836(0);
    v = (*(u16 *)(ret + 6) + 0x2000) & 0xc000;
    Talk_Run838();
    Talk_Run839();
    if (Talk_Check8(512) == 0) {
        Talk_Do15(512);
        Talk_Run840(0x969);
        Talk_Run841(0x1ff7);
        Talk_Run842(a, 0);
        Talk_Run843(10);
        t = v << 16;
        c = 0x4000;
        if (t == (0x4000 << 16)) {
            Talk_Run844(0, 40, 104);
            Talk_Run845(0, 0, 0);
        }
        Talk_Place152(a, 0x10000, 0x8000);
        Talk_Place153(a, 0, -48);
        Talk_Run846(a, 64, 0);
        Talk_Run847(a, c, 0);
    } else {
        Talk_Do16(512);
        Talk_Run848(0x969);
        Talk_Place154(a, 0x4000, 0);
        Talk_Run849(0, 120, 96);
        Talk_Run850(0, 0xc000, 0);
        Talk_Run851(20);
        c = (s32)&Value_00001ff8;
        Talk_Run852(c);
        Talk_Check61(a, 0);
        if (Talk_Check62(0, 0) == 0) {
            Talk_Run853(c + 1);
            Talk_Run854(a, 0);
        } else {
            Talk_Run855(c + 2);
            Talk_Run856(a, 0);
        }
        Talk_Run857(10);
        Talk_Run858(a, 3);
        Talk_Run859(20);
        Talk_Place155(a, -64, 0);
        Talk_Run860(a, 0, 48);
    }
    Talk_Run861();
}

void RunMiddleAuxiliarySequence(s32 a)
{
    u8 *obj;
    u8 *q;

    obj = Talk_Run837();
    Talk_Run862();
    q = gOv9;
    Talk_Run863(a, q);
    Talk_Run864(0x2009);
    Talk_Run865(a, 0);
    Talk_Run866(a);
    *(s32 *)(obj + 28) = 0x10000;
    *(s32 *)(obj + 24) = 0x10000;
    Talk_Run867(30);
    Talk_Run868(a, 2);
    Talk_Run869(30);
    Talk_Run870(a, 2);
    Talk_Run871(60);
    Talk_Run872(a, 0);
    Talk_Run873(20);
    Talk_Run874(a, 258, 60);
    Talk_Run875(a, 2);
    Talk_Run876(30);
    Talk_Run877(a, 2);
    Talk_Run878(30);
    Talk_Run879(a, 2);
    Talk_Run880(30);
    Talk_Run881(a, q);
    Talk_Run882(a, 0);
    Talk_Place156(a, 0xe000, 0);
    Talk_Run883(10);
    *(s32 *)(obj + 28) = 0x10000;
    *(s32 *)(obj + 24) = 0x10000;
    Talk_Run884(a, q);
    Talk_Run885();
}

    Talk_Run18();
    Talk_Run19();
    Talk_Do17(8225);
    Talk_Place157(0, 520, 424);
    Talk_Place158(0, 57344, 0);
    Talk_Run20(36700160, -1, 24117248, 1);
    Talk_Run21();
    Talk_Place159(20, 65536, 32768);
    Talk_Place160(20, 40, 0);
    Talk_Place161(20, 584, 360);
    Talk_Run22(21, 0);
    Talk_Do18(10);
    Talk_Place162(20, 257, 40);
    Talk_Place163(20, 32768, 0);
    Talk_Place164(21, 131072, 65536);
    Talk_Place165(22, 131072, 65536);
    Talk_Place166(21, 528, 352);
    Talk_Place167(22, 528, 368);
    Talk_Do19(3);
    Talk_Run23(21, 1);
    Talk_Do20(30);
    Talk_Do21(10);
    Talk_Run24(22, 3);
    Talk_Do22(30);
    Talk_Run25(22, 0);
    Talk_Do23(10);
    Talk_Run26(20, 2);
    Talk_Do24(20);
    Talk_Place168(20, -16, 0);
    Talk_Do25(10);
    Talk_Run27(20, 0);
    Talk_Do26(10);
    Talk_Run28(21, 4);
    Talk_Run29(22, 4);
    Talk_Do27(20);
    Talk_Do28(10);
    Talk_Place169(20, 261, 40);
    Talk_Run30(20, 0);
    Talk_Do29(10);
    Talk_Place170(21, 258, 40);
    Talk_Place171(21, 65536, 32768);
    Talk_Place172(21, 8, 0);
    Talk_Do30(20);
    Talk_Run31(21, 0);
    Talk_Do31(10);
    Talk_Place173(20, 257, 80);
    Talk_Do32(10);
    Talk_Place174(22, 258, 40);
    Talk_Place175(22, 65536, 32768);
    Talk_Place176(22, 8, 0);
    Talk_Do33(20);
    Talk_Run32(22, 0);
    Talk_Do34(10);
    Talk_Run33(20, 4);
    Talk_Do35(20);
    Talk_Run34(20, 0);
    Talk_Do36(10);
    Talk_Run35(21, 3);
    Talk_Run36(22, 3);
    Talk_Do37(30);
    Talk_Do38(10);
    Talk_Run37(21, 2);
    Talk_Do39(20);
    Talk_Run38(21, 0);
    Talk_Do40(10);
    Talk_Run39(22, 4);
    Talk_Do41(20);
    Talk_Run40(22, 0);
    Talk_Do42(10);
    Talk_Place177(20, 261, 40);
    Talk_Run41(20, 0);
    Talk_Do43(10);
    Talk_Place178(21, 257, 40);
    Talk_Run42(21, 0);
    Talk_Do44(10);
    Talk_Place179(20, 258, 40);
    Talk_Run43(20, 0);
    Talk_Do45(10);
    Talk_Run44(22, 2);
    Talk_Do46(20);
    Talk_Run45(22, 0);
    Talk_Do47(10);
    Talk_Run46(20, 3);
    Talk_Do48(30);
    Talk_Do49(10);
    Talk_Run47(21, 2);
    Talk_Do50(20);
    Talk_Run48(21, 0);
    Talk_Do51(10);
    Talk_Place180(20, 258, 40);
    Talk_Run49(20, 0);
    Talk_Do52(20);
    Talk_Place181(21, 22, 60);
    Talk_Place182(21, 20, 0);
    Talk_Place183(22, 20, 0);
    Talk_Do53(20);
    Talk_Do54(10);
    Talk_Run50(20, 2);
    Talk_Do55(20);
    Talk_Run51(20, 0);
    Talk_Do56(10);
    Talk_Place184(21, 256, 40);
    Talk_Run52(21, 0);
    Talk_Do57(10);
    Talk_Run53(22, 3);
    Talk_Do58(30);
    Talk_Run54(22, 0);
    Talk_Do59(10);
    Talk_Run55(20, 258);
    Talk_Run56(20, 2);
    Talk_Do60(30);
    Talk_Run57(20, 0);
    Talk_Do61(10);
    Talk_Place185(21, 257, 40);
    Talk_Run58(21, 0);
    Talk_Do62(10);
    Talk_Run59(20, 3);
    Talk_Do63(30);
    Talk_Do64(10);
    Talk_Place186(22, 257, 40);
    Talk_Run60(22, 0);
    Talk_Do65(10);
    Talk_Run61(20, 4);
    Talk_Do66(20);
    Talk_Run62(20, 0);
    Talk_Do67(20);
    Talk_Place187(21, 22, 60);
    Talk_Place188(21, 20, 0);
    Talk_Place189(22, 20, 0);
    Talk_Do68(20);
    Talk_Do69(10);
    Talk_Place190(20, 261, 40);
    Talk_Run63(20, 0);
    Talk_Do70(10);
    Talk_Run64(21, 2);
    Talk_Do71(20);
    Talk_Run65(21, 0);
    Talk_Do72(10);
    Talk_Run66(22, 3);
    Talk_Do73(30);
    Talk_Run67(22, 0);
    Talk_Do74(10);
    Talk_Run68(20, 3);
    Talk_Do75(30);
    Talk_Do76(10);
    Talk_Run69(22, 2);
    Talk_Do77(20);
    Talk_Run70(22, 0);
    Talk_Do78(10);
    Talk_Place191(20, 258, 40);
    Talk_Run71(20, 0);
    Talk_Do79(10);
    Talk_Run72(21, 2);
    Talk_Do80(20);
    Talk_Run73(21, 0);
    Talk_Do81(10);
    Talk_Run74(20, 4);
    Talk_Do82(20);
    Talk_Run75(20, 0);
    Talk_Do83(10);
    Talk_Place192(22, 256, 40);
    Talk_Run76(22, 0);
    Talk_Do84(10);
    Talk_Place193(20, 258, 40);
    Talk_Run77(20, 0);
    Talk_Do85(10);
    Talk_Place194(21, 257, 40);
    Talk_Run78(21, 0);
    Talk_Do86(10);
    Talk_Run79(20, 4);
    Talk_Do87(20);
    Talk_Run80(20, 0);
    Talk_Do88(20);
    Talk_Place195(21, 22, 60);
    Talk_Place196(21, 20, 0);
    Talk_Place197(22, 20, 0);
    Talk_Do89(20);
    Talk_Do90(10);
    Talk_Run81(20, 2);
    Talk_Do91(20);
    Talk_Run82(20, 0);
    Talk_Do92(10);
    Talk_Run83(21, 258);
    Talk_Run84(22, 258);
    Talk_Run85(21, 2);
    Talk_Run86(22, 2);
    Talk_Do93(30);
    Talk_Do94(10);
    Talk_Place198(20, 0, 0);
    Talk_Do95(20);
    Talk_Run87(20, 0);
    Talk_Do96(10);
    Talk_Run88(21, 4);
    Talk_Do97(20);
    Talk_Run89(21, 0);
    Talk_Do98(10);
    Talk_Run90(22, 4);
    Talk_Do99(20);
    Talk_Run91(22, 0);
    Talk_Do100(10);
    Talk_Run92(20, 2);
    Talk_Do101(20);
    Talk_Place199(20, 32768, 0);
    Talk_Do102(20);
    Talk_Run93(20, 0);
    Talk_Do103(10);
    Talk_Run94(21, 3);
    Talk_Do104(30);
    Talk_Run95(21, 0);
    Talk_Do105(20);
    Talk_Place200(20, 261, 40);
    Talk_Run96(20, 0);
    Talk_Do106(10);
    Talk_Run97(22, 3);
    Talk_Do107(30);
    Talk_Run98(22, 0);
    Talk_Do108(10);
    Talk_Run99(20, 3);
    Talk_Do109(40);
    Talk_Place201(21, 22, 60);
    Talk_Run100(21, 3);
    Talk_Do110(20);
    Talk_Run101(22, 3);
    Talk_Do111(20);
    Talk_Place202(22, 16384, 0);
    Talk_Do112(40);
    Talk_Place203(21, 85196, 42598);
    Talk_Place204(22, 85196, 42598);
    Talk_Place205(21, 0, 120);
    Talk_Place206(22, 0, 120);
    Talk_Place207(21, 0, 0);
    Talk_Place208(22, 0, 0);
    Talk_Do113(10);
    Talk_Run102(20, 2);
    Talk_Do114(40);
    Talk_Place209(20, 52428, 26214);
    Talk_Place210(20, -16, 0);
    Talk_Do115(30);
    Talk_Run103(20, 0);
    Talk_Do116(10);
    Talk_Run104(20, 4);
    Talk_Do117(30);
    Talk_Place211(20, 0, 0);
    Talk_Do118(30);
    Talk_Place212(20, 65536, 32768);
    Talk_Place213(20, 120, 0);
    Talk_Place214(20, 60, 0);
    Talk_Place215(20, 0, 0);
    Talk_Run105();
}

/* overlays/scene/dialogue/choice/scene_effect_sequence_mid.c */

extern u8 Value_00002280[];
extern u8 gWork[];

extern u8 Value_00001ffb[];

static __inline__ __attribute__((always_inline)) void bump_step(s32 amount)
{
    u8 *scene = *(u8 **)gWork;

    *(u16 *)(scene + 0x1d8) = (u16)(*(u16 *)(scene + 0x1d8) + amount);
}

void Scene_RunScene3b8SequenceB(void)
{
    u32 i;
    s32 record;
    s32 v5;

    Talk_Run886();
    Talk_Run887((s32)&Value_00002280);
    v5 = 0;
    *(u8 *)(Talk_Apply7(0) + 84) = v5;
    *(u8 *)(Talk_Check63(10) + 84) = v5;
    Talk_Run888(1);
    *(volatile u16 *)0x04000000 = 0x1140;
    Talk_Run106(-1, 0);
    *(volatile u16 *)0x04000000 = 0x140;
    v5 = 1;
    *(u8 *)(Talk_Check64(0) + 84) = v5;
    *(u8 *)(Talk_Check65(10) + 84) = v5;
    Talk_Run889(0, 31);
    record = Talk_Check66(0);
    Talk_Run890(record, 0);
    Talk_Place216(1, 0x780000, 0x680000);
    Talk_Place217(3, 0x680000, 0x500000);
    Talk_Place218(2, 0x780000, 0x780000);
    Talk_Run891(1, 0, 0);
    Talk_Run892(3, 0, 0);
    Talk_Place219(2, 0xe000, 0);
    *(s32 *)(*(u8 **)gWork + 0x1c8) = 60;
    Talk_Run893();
    Talk_Run894();
    Talk_Run895(20);
    *(s32 *)(*(u8 **)gWork + 0x1c8) = 24;
    Talk_Place220(3, 0x10000, 0x8000);
    Talk_Run896(3, 16, 0);
    Talk_Place221(3, 0x2000, 0);
    Talk_Run897(20);
    Talk_Run898(3, 2);
    Talk_Run899(30);
    Talk_Run900(3, 0);
    Talk_Run901(10);
    record = Talk_Check67(0);
    *(s32 *)(record + 16) += -0x30000;
    record = Talk_Check9(0);
    *(s32 *)(record + 64) += -0x30000;
    Talk_Run902(0, 32);
    Talk_Run903(40);
    Talk_Run904(0, 34);
    Talk_Run905(30);
    Talk_Run906(0, 33);
    Talk_Run907(50);
    Talk_Run908(1, 2);
    Talk_Run909(30);
    Talk_Run910(1, 0);
    Talk_Run911(10);
    Talk_Place222(0, 0x105, 60);
    Talk_Run912(20);
    Talk_Place223(1, 0x102, 60);
    Talk_Run913(1, 0);
    Talk_Run914(10);
    Talk_Run915(1, 4);
    Talk_Run916(20);
    Talk_Run917(1, 0);
    Talk_Run918(10);
    Talk_Place224(0, 0x102, 80);
    Talk_Place225(2, 0x106, 60);
    Talk_Place226(2, 0xc000, 0);
    Talk_Run919(30);
    Talk_Run920(2, 4);
    Talk_Run921(20);
    Talk_Run922(2, 0);
    Talk_Run923(10);
    Talk_Place227(1, 0x4000, 0);
    Talk_Run924(30);
    Talk_Run925(1, 2);
    Talk_Run926(45);
    Talk_Run927(1, 0, 0);
    Talk_Place228(2, 0xe000, 0);
    Talk_Run928(30);
    Talk_Check10(1, 0);
    if (Talk_Check11(-1, 0) != 0) {
        Talk_Run929(10);
        Talk_Run930(0, 34);
        Talk_Run931(20);
        Talk_Run932(1, 3);
        Talk_Run933(20);
        Talk_Run934(1, 0);
        Talk_Run935(10);
        Talk_Run936(0, 33);
        Talk_Run937(30);
        Talk_Run938(1, 3);
        Talk_Run939(20);
        Talk_Run940(1, 0);
        bump_step(1);
    } else {
        Talk_Run941(10);
        Talk_Run942(0, 33);
        bump_step(2);
        Talk_Run943(30);
        Talk_Run944(1, 3);
        Talk_Run945(20);
        Talk_Run946(1, 0);
    }
    Talk_Run947(10);
    Talk_Place229(1, 0x10000, 0x8000);
    Talk_Place230(1, -16, 0);
    Talk_Run948(1, 0, 0);
    Talk_Run949(35);
    Talk_Run950(0, 6, 0);
    Talk_Place231(0, 0x1e666, 0xf333);
    Talk_Place232(0, -32, 0);
    record = Talk_Check68(0);
    Talk_Run951(record, 1);
    Talk_Place233(3, 0x4000, 0);
    Talk_Place234(2, 0xc000, 0);
    Talk_Run952(40);
    Talk_Run953(0, 3);
    Talk_Run954(30);
    Talk_Run955(2, 3);
    Talk_Run956(1, 3);
    Talk_Run957(3, 3);
    Talk_Run958(30);
    Talk_Place235(1, 0x13333, 0x9999);
    Talk_Place236(3, 0x13333, 0x9999);
    Talk_Place237(2, 0x13333, 0x9999);
    Talk_Run959(1, 2);
    record = Talk_Check12(0);
    if (record != 0) {
        Talk_Run960(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Talk_Run961(1);
    Talk_Run962(1, 0, 0);
    Talk_Run963(3, 2);
    record = Talk_Check13(0);
    if (record != 0) {
        Talk_Run964(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Talk_Run965(3);
    Talk_Run966(3, 0, 0);
    Talk_Run967(2, 2);
    record = Talk_Check14(0);
    if (record != 0) {
        Talk_Run968(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Talk_Run969(2);
    Talk_Run970(2, 0, 0);
    ((void (*)())Talk_Check69)(10);
    Talk_Run971();
}

void Scene_RunScene3b8SequenceA(void)
{
    s32 record;
    s16 dir;
    u16 facing;

    record = Talk_Check70(0);
    dir = (*(u16 *)(record + 6) + 0x2000) & -0x4000;
    Talk_Run972();
    Talk_Run973();
    Talk_Run974((s32)&Value_00001ffb);
    Talk_Check15(-1, 0);
    if (Talk_Check16(0, 0) != 0) {
    } else {
        if (Talk_Check17(0x96a) != 0) {
            Talk_Run975(20);
            Talk_Run976(0);
            goto L_02000fe2;
        }
        Talk_Run977(20);
        facing = dir;
        if (facing == 0) {
            Talk_Run978(0, 128, 120);
        }
        if (facing == 0x8000) {
            Talk_Run979(0, 240, 120);
        }
        Talk_Run980(0, 184, 120);
        Talk_Run981(0, 0, 0);
        Talk_Run982(10);
        Talk_Run983(1, 16, 0, 0x8000);
        Talk_Run984(1);
        Talk_Run985(10);
        Talk_Run986(1, 0);
        Talk_Run987(10);
        Talk_Run988(0, 3);
        Talk_Run989(20);
        Talk_Place238(1, 0x10000, 0x8000);
        Talk_Run990(1, 40, 0);
        Talk_Place239(1, 0, -32);
        Talk_Place240(1, 0x6000, 0);
        Talk_Run991(20);
        Talk_Run992(0, 0x8000, 0);
        Talk_Run993(20);
        Talk_Run107(2, -16, 0, 0);
        Talk_Run994(2);
        Talk_Run995(10);
        Talk_Run996(2, 0);
        Talk_Run997(10);
        Talk_Run998(0, 3);
        Talk_Run999(20);
        Talk_Place241(2, 0x10000, 0x8000);
        Talk_Place242(2, -40, 0);
        Talk_Run1000(2, 0, 40);
        Talk_Place243(2, 0xe000, 0);
        Talk_Run1001(20);
        Talk_Run1002(0, 0, 0);
        Talk_Run1003(20);
        Talk_Run1004(3, 16, 0, 0x8000);
        Talk_Run1005(3);
        Talk_Run1006(10);
        Talk_Run1007(3, 0);
        Talk_Run1008(10);
        Talk_Run1009(0, 3);
        Talk_Run1010(20);
        Talk_Place244(3, 0x10000, 0x8000);
        Talk_Run1011(3, 40, 0);
        Talk_Run1012(3, 0, 40);
        Talk_Place245(3, 0xa000, 0);
        Talk_Run1013(20);
        Talk_Run108(-1, -1, -1, 0);
        Talk_Place246(0, -56, 0);
        Talk_Place247(0, 0, -32);
        Talk_Place248(0, 0x2000, 0);
        Talk_Run1014(30);
        Talk_Run1015(3, 3);
        Talk_Run1016(10);
        Talk_Run1017(3, 0);
        Talk_Run1018(1, 3);
        ((void (*)())Talk_Check71)(10);
        if (Talk_Check18(0x96a) == 0) {
            Talk_Run1019(1, 0);
        } else {
            bump_step(1);
        }
        Talk_Run1020(2, 3);
        Talk_Run1021(10);
        if (Talk_Check19(0x96a) == 0) {
            Talk_Run1022(2, 0);
        } else {
            bump_step(1);
        }
        Talk_Run1023(20);
        Talk_Run1024(0, 0, 0);
        Talk_Place249(1, 0x8000, 0);
        Talk_Place250(3, 0x8000, 0);
        Talk_Run1025(2, 0, 0);
        Talk_Run1026(0);
        Talk_Place251(0, 0x2000, 0);
        Talk_Place252(1, 0x6000, 0);
        Talk_Place253(3, 0xa000, 0);
        Talk_Place254(2, 0xe000, 0);
        Talk_Run1027(20);
        Talk_Run1028(1, 3);
        Talk_Run1029(10);
        Talk_Run1030(1, 0);
        Talk_Run1031(2, 3);
        ((void (*)())Talk_Check72)(10);
        if (Talk_Check20(0x96a) == 0) {
            Talk_Run1032(2, 0);
        } else {
            bump_step(1);
        }
        Talk_Run1033(3, 3);
        Talk_Run1034(10);
        if (Talk_Check21(0x96a) == 0) {
            Talk_Run1035(3, 0);
        } else {
            bump_step(1);
        }
        Talk_Run1036(1, 0, 32);
        Talk_Place255(1, -112, 0);
        Talk_Place256(3, 0, -40);
        Talk_Place257(3, -112, 0);
        Talk_Run1037(50);
        Talk_Place258(2, 0, -24);
        Talk_Run1038(1);
        Talk_Place259(0, 0x4000, 0);
        Talk_Place260(1, 0, -16);
        Talk_Run1039(3);
        Talk_Place261(3, 0xc000, 0);
        Talk_Run1040(20);
        Talk_Run1041(1, 0);
        Talk_Run1042(10);
        Talk_Run1043(0, 3);
        Talk_Run1044(20);
        Talk_Place262(1, 0x13333, 0x9999);
        Talk_Place263(2, 0x13333, 0x9999);
        Talk_Place264(3, 0x13333, 0x9999);
        Talk_Run1045(1, 2);
        record = Talk_Check22(0);
        if (record != 0) {
            Talk_Run1046(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Talk_Run1047(1);
        Talk_Run1048(1, 0, 0);
        Talk_Run1049(3, 2);
        record = Talk_Check23(0);
        if (record != 0) {
            Talk_Run1050(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Talk_Run1051(3);
        Talk_Run1052(3, 0, 0);
        Talk_Run1053(2, 2);
        record = Talk_Check24(0);
        if (record != 0) {
            Talk_Run1054(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Talk_Run1055(2);
        Talk_Run1056(2, 0, 0);
        Talk_Do119(0x96a);
    }
    Talk_Run1057();
    L_02000fe2:;
}

/* overlays/scene/dialogue/choice/scene_primary_script.c */
extern u8 gVal[];
extern u8 gWork[];
extern u8 Value_000022ab;
extern u8 Value_00002352;
extern u8 Value_00000f31;

void Scene_RunScene3b8(s32 a0)
{
    u32 i;
    s32 record;
    s32 base6_2241;

    Talk_Run1058();
    Talk_Run1059();
    if (Talk_Check25(0x966) == 0) {
        Talk_Do120(0x966);
        Talk_Do121(0x967);
        Talk_Place265(a0, 0x4000, 0);
        Talk_Run1060(0, 120, 96);
        Talk_Place266(0, 0xc000, 0);
        Talk_Run1061(20);
        base6_2241 = (s32)gVal;
        Talk_Run1062(base6_2241);
        Talk_Check26(a0, 0);
        if (Talk_Check27(0, 0) == 0) {
            Talk_Run1063(10);
            Talk_Run1064((base6_2241 + 1));
        } else {
            Talk_Run1065((base6_2241 + 2));
        }
        Talk_Run1066(a0, 0);
        Talk_Run1067(10);
        Talk_Run1068(a0, 3);
        Talk_Run1069(20);
        Talk_Place267(a0, 0x10000, 0x8000);
        Talk_Place268(a0, -64, 0);
        Talk_Run1070(a0, 0, 48);
    } else {
        Talk_Do122(0x2245);
        Talk_Run1071(a0, 0);
    }
    Talk_Run1072();
}

void Scene_RunScene3b8(s32 unused0, s32 a1)
{
    Talk_Run1073();
    Talk_Do123(0x2052);
    Talk_Run1074(a1, 0);
    if (Talk_Check28(0x968) == 0) {
        Talk_Do124(0x968);
        Talk_Run1075();
        Talk_Run1076(50);
        Talk_Place269(a1, 0x100, 70);
        Talk_Run1077(a1, 0, 40);
        Talk_Run1078(a1, 0);
        Talk_Run1079(30);
        Talk_Run1080(a1, 4);
        Talk_Run1081(20);
        Talk_Run1082(a1, 0);
        Talk_Place270(a1, 0x8000, 0);
    }
    Talk_Run1083();
}

void Dialogue_RunChoiceSequence22ab(s32 no)
{
    s32 msg = (s32)&Value_000022ab;

    Talk_Do131(msg);
    Talk_Apply8(no, 0);
    if (Talk_Apply9(0, 0) == 0)
        Talk_Do132(msg + 1);
    else
        Talk_Do133(msg + 2);
    Talk_Apply10(no, 0);
}

void Dialogue_RunChoiceSequence2352(void)
{
    s32 msg;

    Talk_Run1084();
    Talk_Run1085();
    msg = (s32)&Value_00002352;
    Talk_Run1086(msg);
    Talk_Apply11(-1, 0);
    Talk_Do134(10);
    Talk_Run1087(14, 2);
    Talk_Do135(30);
    Talk_Place275(0, 14, 30);
    Talk_Apply7(14, 0);
    if (Talk_Apply12(0, 0) != 0) {
        Talk_Do136(msg + 2);
        Talk_Apply13(14, 0);
    } else {
        Talk_Do137(20);
        Talk_Do138(msg + 3);
        Talk_Apply14(14, 0);
        Talk_Do139(10);
        Talk_Apply15(0, 3);
        Talk_Do140(30);
        Talk_Place276(0, 0x4000, 0);
        Talk_Do141(30);
        Talk_Place277(16, 0, 0);
        Talk_Apply16(205, 3);
        Talk_Run1088(0, 1);
        Talk_Run1089(205, 0);
        Talk_Do142((s32)&Value_00000f31);
    }
}

void Scene_RunScene3b8(void)
{
    u32 i;
    s32 record;
    u8 *p5;

    p5 = *(volatile s32 *)gWork;
    Talk_Run826();
    Talk_Run1090(158);
    Talk_Place271(0, 0x8000, 0x4000);
    Talk_Run1091(0, 2);
    if (*(s16 *)(((s32)p5 + 0x16c)) == 32) {
        Talk_Run1092(1);
        Talk_Run1093(10);
        Talk_Place272(0, 0, -16);
    } else {
        if (*(s16 *)(((s32)p5 + 0x16c)) == 30) {
            Talk_Run1094(4);
            Talk_Run1095(10);
            Talk_Place273(0, 3, -16);
        } else {
            Talk_Run1096(2);
            Talk_Run1097(10);
            Talk_Place274(0, 3, -16);
        }
    }
    Talk_Run1098(16);
    Talk_Run1099(*(s16 *)(((s32)p5 + 0x16c)));
    Talk_Run1100(1);
    Talk_Run1101(2);
    Talk_Run1102(4);
    Talk_Run1103();
}

/* overlays/scene/dialogue/choice/scene_setup.c */
extern u8 Value_00001ff1;
extern u8 Value_00002006;
extern u8 Value_00000105;
extern s16 gCell[];
extern u8 Value_0000008b;
extern u8 gOv10[];
extern u8 gOv11[];
extern u8 gOv12[];
extern u8 gOv13[];
extern u8 gOv14[];
extern u8 gOv15[];

/* Contiguous unnamed leaf-owner run for resource_3b8. */

void Scene_RunBranchedSteps1FF1(s32 a)
{
    s32 k = (s32)&Value_00001ff1;

    Talk_Do143(k);
    Talk_Apply17(a, 0);
    if (Talk_Apply18(0, 0) == 0)
        Talk_Do144(k + 1);
    else
        Talk_Do145(k + 2);
    Talk_Apply19(a, 0);
}

void Scene_RunBranchedSteps2006(s32 a)
{
    s32 k = (s32)&Value_00002006;

    Talk_Do146(k);
    Talk_Apply20(a, 0);
    if (Talk_Apply21(0, 0) == 0) {
        Talk_Do147(10);
        Talk_Place278(a, 0x102, 0x28);
        Talk_Do148(k + 1);
    } else {
        Talk_Do149(10);
        Talk_Place279(a, (s32)&Value_00000105, 0x28);
        Talk_Do150(k + 2);
    }
    Talk_Apply22(a, 0);
}

void Scene_RunStepWithValue29e0(void)
{
    Talk_Run1104();
    Talk_Apply23(0x947, 1);
    Talk_Apply24(0x29e0, 1);
    Talk_Run1105();
}

s32 SceneData_SelectTableD004ByStateAndFlags(void)
{
    if (gCell[224] == (s32)&Value_0000008b) {
        if (Talk_Check73(0x950) != 0) {
            return (s32)gOv10;
        }
        if (Talk_Check74(0x962) != 0) {
            return (s32)gOv11;
        }
        return (s32)gOv12;
    }
    if (Talk_Check75(0x950) != 0) {
        return (s32)gOv13;
    }
    if (Talk_Check76(0x962) != 0) {
        return (s32)gOv14;
    }
    return (s32)gOv15;
}
