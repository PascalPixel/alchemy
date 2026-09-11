#include "types.h"
#include "scene.h"

/* overlays/scene/dialogue/choice/branching_actor_sequence.c */
/* DRAFT for Scene_RunBranchingActorSequence
 * (resource_3b8:0x020017e8, complete owner extent 2092 bytes).
 *
 * A scripted field scene driving four presentation slots (0..3). It opens the
 * scene, installs slot 0's record, seats the three visible slots, brings the
 * layers up, then runs two counted step loops that publish a step index into
 * the scene workspace. From there it plays a long alternating motion/pause
 * script. Three selector points steer it: two identical queries through
 * Talk_unk109_3(-1, 0), and a third reuse of the second query's answer held in
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
 * rename it to Talk_unk110_3 and declare
 *   "Talk_unk110_3": { "address": "0x0200c488", "kind": "thumb" }
 * in that unit's absolute_symbols; the emitted bytes are the same either way.
 *
 * Call3, Value2 and SetHalf are a modeling hypothesis, not recovered history.
 * The reference materializes each call's argument constants independently - it
 * builds 0x680000 twice for Talk_unk111_3(1, ..., ...) - and writes each
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

void Talk_unk112_3();    /* main 0x080091e0 */
void Talk_unk113_3();    /* main 0x080000c0, frame wait */
void Talk_unk114_3();    /* main 0x0808a018, scene open */
s32 Talk_unk115_3();     /* main 0x0808a080, record lookup */
void Talk_unk116_3();    /* main 0x0808a100 */
void Talk_unk111_3();    /* main 0x0808a0f0 */
void Talk_unk117_3();    /* main 0x0808a170 */
void Talk_unk118_3();    /* main 0x0808a010, script pause */
void Talk_unk119_3();    /* main 0x0808a1b8 */
void Talk_unk120_3();    /* main 0x0808a138 */
void Talk_unk121_3();    /* main 0x0808a300 */
void Talk_unk122_3();    /* main 0x0808a360 */
void Talk_unk123_3();    /* main 0x0808a090 */
void Talk_unk124_3();    /* main 0x0808a370 */
void Value();   /* main 0x0808a148; see the call-name note above */
void Talk_unk125_3();    /* main 0x0808a1e8 */
void Talk_unk126_3();    /* main 0x0808a180 */
void Talk_unk127_3();    /* main 0x0808a110 */
void Talk_unk128_3();    /* main 0x0808a580 */
void Talk_unk129_3();    /* main 0x0808a178 */
void Talk_unk130_3();    /* main 0x0808a128 */
s32 Talk_unk109_3();     /* main 0x0808a070, branch selector */
void Talk_unk131_3();    /* main 0x0808a150 */
void Talk_unk132_3();    /* main 0x0808a0b8 */
void Talk_unk133_3();    /* main 0x0808a0e8 */
void Talk_unk134_3();    /* main 0x0808a020, scene close */

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

    Talk_unk114_3();
    Talk_unk117_3(0x228c);
    Talk_unk116_3(0, 31);
    record = Talk_unk115_3(0);
    Talk_unk112_3(record, 0);
    Talk_Place(1, 0x680000, 0x680000);
    Talk_Place(3, 0x580000, 0x780000);
    Talk_Place(2, 0x780000, 0x780000);
    Talk_unk2_3(1, 0x4000, 0);
    Talk_unk119_3(3, 0, 0);
    Talk_unk2_3(2, 0x8000, 0);
    state = (s32)gWork[0];
    *(s32 *)(state + 0x1c0) = 0x100;
    *(s32 *)(state + 0x1c8) = 12;
    Talk_unk113_3(1);
    Talk_unk121_3(9);
    work = gWork[4];
    SetHalf((u16 *)(work + 0x52a), 0);
    SetHalf((u16 *)(work + 0x534), 0x1f1f);
    SetHalf((u16 *)(work + 0x536), 1);
    Talk_unk122_3();
    Talk_unk124_3();
    for (step = 1; step <= 5; step++) {
        Talk_unk113_3(3);
        *(u16 *)(work + 0x52a) = step;
    }
    Talk_unk118_3(40);
    Talk_unk120_3(0, 2);
    Talk_unk118_3(30);
    for (step = 5; step <= 31; step++) {
        Talk_unk113_3(3);
        *(u16 *)(work + 0x52a) = step;
    }
    SetHalf((u16 *)(work + 0x536), 31);
    state = (s32)gWork[0];
    *(s32 *)(state + 0x1c0) = 0x209;
    *(s32 *)(state + 0x1c8) = 24;
    Talk_unk118_3(20);
    Talk_unk3_3(1, 0x100, 50);
    Value(1, 0, 40);
    Talk_unk120_3(1, 2);
    Talk_unk118_3(30);
    Talk_unk118_3(10);
    Talk_unk3_3(2, 0x101, 40);
    Talk_unk2_3(2, 0xe000, 0);
    Talk_unk118_3(30);
    Talk_unk126_3(2, 0);
    Talk_unk118_3(10);
    Talk_unk126_3(3, 0);
    Talk_unk119_3(3, 0xe000, 0);
    Talk_unk118_3(30);
    Talk_unk120_3(3, 2);
    Talk_unk118_3(30);
    Talk_unk4_3(3, 0x10000, 0x8000);
    Talk_unk5_3(3, 0, -40);
    Talk_unk128_3(3, 32, 0);
    Talk_unk119_3(3, 0x2000, 0);
    Talk_unk118_3(10);
    Talk_unk126_3(3, 0);
    record = Talk_unk115_3(0);
    *(s32 *)(record + 16) += -0x30000;
    record = Talk_unk115_3(0);
    *(s32 *)(record + 64) += -0x30000;
    Talk_unk116_3(0, 32);
    Talk_unk118_3(40);
    Talk_unk127_3(0, 34);
    Talk_unk118_3(30);
    Talk_unk116_3(0, 33);
    Talk_unk118_3(40);
    Talk_unk127_3(1, 4);
    Talk_unk118_3(20);
    Talk_unk126_3(1, 0);
    Talk_unk118_3(10);
    Talk_unk3_3(0, 0x102, 80);
    Talk_unk3_3(2, 0x100, 50);
    Talk_Run(2, 0);
    if (Talk_unk109_3(-1, 0) == 0) {
        Talk_unk118_3(10);
        Talk_unk127_3(0, 33);
        Talk_unk118_3(20);
        Talk_unk3_3(1, 0x103, 40);
        Talk_unk130_3(1, 4, 13);
        Talk_unk130_3(1, 4, 30);
        Talk_unk126_3(1, 0);
        *(u16 *)(gWork[0] + 0x1d8) += 1;
    } else {
        Talk_unk118_3(10);
        Talk_unk127_3(0, 34);
        Talk_unk118_3(20);
        Talk_unk3_3(1, 0x103, 40);
        Talk_unk130_3(1, 4, 13);
        Talk_unk130_3(1, 4, 30);
        *(u16 *)(gWork[0] + 0x1d8) += 1;
        Talk_unk126_3(1, 0);
    }
    Talk_unk118_3(10);
    Talk_unk119_3(2, 0xa000, 0);
    Talk_unk118_3(30);
    Talk_unk127_3(2, 4);
    Talk_unk118_3(20);
    Talk_unk2_3(1, 0x2000, 0);
    Talk_unk118_3(30);
    Talk_unk120_3(1, 2);
    Talk_unk118_3(40);
    Talk_unk2_3(1, 0xe000, 0);
    Talk_unk119_3(2, 0xe000, 0);
    Talk_unk118_3(30);
    Talk_unk127_3(1, 3);
    Talk_unk118_3(30);
    Talk_unk126_3(1, 0);
    Talk_unk118_3(10);
    Talk_unk119_3(3, 0x6000, 0);
    Talk_unk118_3(50);
    Talk_unk119_3(3, 0x2000, 0);
    Talk_unk118_3(35);
    Talk_unk125_3(3, 0x108, 50);
    Talk_unk118_3(10);
    Talk_unk127_3(3, 3);
    Talk_unk118_3(30);
    Talk_unk126_3(3, 0);
    Talk_unk118_3(10);
    Talk_unk120_3(2, 2);
    Talk_unk118_3(20);
    Talk_Run(2, 0);
    if (Talk_unk109_3(-1, 0) == 0) {
        Talk_unk118_3(10);
        Talk_unk127_3(0, 33);
        Talk_unk118_3(20);
        Talk_unk3_3(1, 0x107, 40);
        Talk_unk126_3(1, 0);
        pick = 0;
        *(u16 *)(gWork[0] + 0x1d8) += 1;
    } else {
        Talk_unk118_3(10);
        Talk_unk127_3(0, 34);
        Talk_unk118_3(20);
        Talk_unk3_3(1, 0x107, 40);
        *(u16 *)(gWork[0] + 0x1d8) += 1;
        pick = 1;
        Talk_unk126_3(1, 0);
    }
    Talk_unk118_3(10);
    Talk_unk119_3(2, 0xa000, 0);
    Talk_unk118_3(30);
    Talk_unk127_3(2, 4);
    Talk_unk118_3(20);
    Talk_unk2_3(1, 0x2000, 0);
    Talk_unk118_3(30);
    Talk_unk120_3(1, 2);
    Talk_unk118_3(40);
    Talk_unk2_3(1, 0xe000, 0);
    Talk_unk119_3(2, 0xe000, 0);
    Talk_unk118_3(30);
    Talk_unk127_3(1, 3);
    Talk_unk118_3(30);
    Talk_unk126_3(1, 0);
    Talk_unk118_3(10);
    Talk_unk119_3(3, 0x6000, 0);
    Talk_unk118_3(50);
    Talk_unk119_3(3, 0x2000, 0);
    Talk_unk118_3(35);
    Talk_unk125_3(3, 0x108, 50);
    Talk_unk118_3(10);
    Talk_unk127_3(3, 3);
    Talk_unk118_3(30);
    Talk_unk126_3(3, 0);
    if (pick == 0) {
        Talk_unk118_3(10);
        Talk_unk127_3(2, 3);
        Talk_unk118_3(30);
        Talk_unk126_3(2, 0);
        Talk_unk118_3(10);
        Talk_unk120_3(1, 2);
        Talk_unk118_3(30);
        Talk_unk126_3(1, 0);
        *(u16 *)(gWork[0] + 0x1d8) += 2;
    } else {
        *(u16 *)(gWork[0] + 0x1d8) += 2;
        Talk_unk118_3(10);
        Talk_unk127_3(2, 4);
        Talk_unk118_3(20);
        Talk_unk126_3(2, 0);
        Talk_unk118_3(10);
        Talk_unk120_3(1, 2);
        Talk_unk118_3(20);
        Talk_unk126_3(1, 0);
    }
    Talk_unk118_3(10);
    Talk_unk125_3(0, 0x102, 60);
    Talk_unk118_3(10);
    Talk_unk127_3(1, 3);
    Talk_unk118_3(30);
    Talk_unk126_3(1, 0);
    Talk_unk118_3(20);
    Talk_unk131_3(2, 3, 40);
    Talk_unk2_3(3, 0x2000, 0);
    Talk_unk119_3(2, 0xe000, 0);
    Talk_unk118_3(30);
    while ((gIw & 240) == 0) {
        Talk_unk113_3(1);
    }
    Talk_unk130_3(0, 6, 0);
    Talk_unk4_3(0, 0x1e666, 0xf333);
    Talk_unk128_3(0, -32, -8);
    record = Talk_unk115_3(0);
    Talk_unk112_3(record, 1);
    Talk_unk118_3(20);
    Talk_unk2_3(0, 0xc000, 0);
    Talk_unk2_3(3, 0x4000, 0);
    Talk_unk119_3(2, 0xc000, 0);
    Talk_unk118_3(20);
    Talk_unk118_3(10);
    Talk_unk120_3(3, 2);
    Talk_unk118_3(20);
    Talk_unk126_3(3, 0);
    Talk_unk118_3(10);
    Talk_unk120_3(1, 2);
    Talk_unk118_3(20);
    Talk_unk126_3(1, 0);
    Talk_unk118_3(20);
    Talk_unk127_3(2, 3);
    Talk_unk118_3(30);
    Talk_unk2_3(0, 0x4000, 0);
    Talk_unk119_3(1, 0x2000, 0);
    Talk_unk118_3(30);
    Talk_unk126_3(2, 0);
    Talk_unk118_3(20);
    Talk_unk116_3(0, 3);
    Talk_unk116_3(1, 3);
    Talk_unk127_3(3, 3);
    Talk_unk118_3(30);
    Talk_unk4_3(1, 0x13333, 0x9999);
    Talk_unk4_3(3, 0x13333, 0x9999);
    Talk_unk4_3(2, 0x13333, 0x9999);
    Talk_unk116_3(1, 2);
    record = Talk_unk115_3(0);
    if (record != 0) {
        Talk_unk132_3(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Talk_unk133_3(1);
    Talk_unk111_3(1, 0, 0);
    Talk_unk116_3(3, 2);
    record = Talk_unk115_3(0);
    if (record != 0) {
        Talk_unk132_3(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Talk_unk133_3(3);
    Talk_unk111_3(3, 0, 0);
    Talk_unk116_3(2, 2);
    record = Talk_unk115_3(0);
    if (record != 0) {
        Talk_unk132_3(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Talk_unk133_3(2);
    Talk_unk111_3(2, 0, 0);
    Talk_unk118_3(10);
    Talk_unk134_3();
}

/* overlays/scene/dialogue/choice/dialogue_layout.c */
extern u8 Value_000022a8;
extern u8 Value_000022a3;

void Dialogue_ShowMessage22a8Branch(s32 a)
{
    s32 k = (s32)&Value_000022a8;

    Talk_unk125(k);
    Talk_Apply(a, 0);
    if (Talk_Apply2(0, 0) == 0)
        Talk_unk126(k + 1);
    else
        Talk_unk127(k + 2);
    Talk_Apply3(a, 0);
}

void Dialogue_ShowMessage22a3Branch(s32 a)
{
    s32 k = (s32)&Value_000022a3;

    Talk_unk128(k);
    Talk_Apply4(a, 0);
    if (Talk_unk135_3(0, 0) == 0) {
        Talk_unk129(k + 1);
        Talk_Apply5(a, 0);
    } else {
        Talk_unk130(k + 2);
        Talk_Apply6(a, 0);
    }
}

/* overlays/scene/dialogue/choice/run_main_cutscene_sequence.c */

u8 *Talk_unk136_3();

u8 *Talk_unk137_3();

s16 *Talk_unk138_3();

s16 *Talk_unk139_3();
s16 *Talk_unk140_3();

s16 *Talk_unk141_3();

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

    carry = Talk_unk29(30);
    Talk_unk142_3(carry);
    carry = Talk_Check(0x22c4);
    carry = Talk_unk30(carry);
    Talk_unk143_3(carry);
    carry = Talk_unk2(0xd80000, -1, 0x2e00000, 1);
    Talk_unk144_3(carry);
    Talk_unk145_3(20);
    Talk_unk146_3(10);
    Talk_unk147_3(20, 0);
    Talk_unk148_3(10);
    Talk_unk6_3(14, 0xcccc, 0x6666);
    Talk_unk149_3(14, 0, 16);
    Talk_unk150_3(10);
    Talk_unk151_2(14, 3);
    Talk_unk152_2(30);
    Talk_unk153_2(14, 0);
    Talk_unk154_2(20);
    Talk_unk155_2(20, 0);
    carry = Talk_unk31(0, 1);
    Talk_unk156_2(carry);
    Talk_unk157_2(40);
    Talk_unk7_3(0, 0x10000, 0x8000);
    Talk_unk8_3(0, 208, 0x2f8);
    Talk_unk158_2(10);
    Talk_unk2_4(0xd80000, -1, 0x2e00000, 1);
    Talk_unk3_4(1, -16, 16, 0xc000);
    Talk_unk4_4(3, 0, 24, 0xc000);
    Talk_unk5_4(2, 16, 16, 0xc000);
    Talk_unk159_2(1);
    Talk_unk160_2(30);
    Talk_unk161_2(10);
    Talk_unk162_2(14, 3);
    Talk_unk163_2(30);
    Talk_unk164_2(14, 0);

    if (Talk_unk3(0x951) != 0) {
        Talk_unk165_2(10);
        Talk_unk166_2(20, 3);
        Talk_unk167_2(20);
        Talk_unk168_2(20, 0);
        Talk_unk169_2(10);
        Talk_unk170_2(14, 0xa000, 0);
        Talk_unk171_2(30);
        Talk_unk172_2(14, 3);
        Talk_unk173_2(20);
        Talk_unk174_2(14, 0);
        Talk_unk175_2(10);
        Talk_unk176_2(20, 3);
        Talk_unk177_2(20);
        Talk_unk178_2(20, 0);
        Talk_unk9_3(20, 0xc000, 0);
        Talk_unk179_2(20);
        Talk_unk10_3(20, 0x10000, 0x8000);
        Talk_unk11_3(20, 0, -16);
        Talk_unk180_2(40);
        Talk_unk181_2(20, 3);
        Talk_unk182_2(40);
        Talk_unk12_3(20, 0x4000, 0);
        Talk_unk183_2(20);
        Talk_unk184_2(20, 0, 32);
        Talk_unk185_2(14, 0x8000, 0);
        Talk_unk186_2(10);
        Talk_unk187_2(20, 0, 0);
        Talk_unk188_2(30);
        Talk_unk189_2(20, 3);
        Talk_unk190_2(30);
        Talk_unk13_3(20, 0x4000, 0);
        Talk_unk191_2(20);
        Talk_unk14_3(20, 0xcccc, 0x6666);
        Talk_unk136_3(20)[90] &= 0xfe;
        Talk_unk15_3(20, 0, -16);
        {
            u8 *record = Talk_unk137_3(20);
            u8 value = *(volatile u8 *)&record[90];

            record[90] = (u8)(value | 1);
        }
        Talk_unk16_3(14, 0x4000, 0);
        Talk_unk192_2(40);
        Talk_unk17_3(14, 0xcccc, 0x6666);
        Talk_unk193_2(14, 0, 16);
        Talk_unk194_2(40);
        Talk_unk195_2(164, 2);
        Talk_unk196_2(-1, 0);
        Talk_unk197_2(164, 3);
        Talk_unk32(164, 0);
        Talk_unk198_2(0, 0xc000, 0);
        Talk_unk199_2(30);
        Talk_unk18_3(14, 0, -16);
        Talk_unk33(14, 0x4000, 0);
        Talk_unk200_2(30);
        Talk_unk201_2(14, 2);
        Talk_unk202_2(20);
        Talk_unk203_2(14, 0);
        bump_step(2);
    } else {
        bump_step(5);
        Talk_unk204_2(10);
        Talk_unk205_2(20, 3);
        Talk_unk206_2(20);
        Talk_unk207_2(20, 0);
        Talk_unk208_2(14, 0xa000, 0);
        Talk_unk209_2(40);
        Talk_unk210_2(14, 3);
        Talk_unk211_2(20);
        Talk_unk212_2(20, 3);
        Talk_unk213_2(30);
        Talk_unk214_2(14, 0x4000, 0);
        Talk_unk215_2(30);
        Talk_unk216_2(14, 2);
        Talk_unk217_2(20);
        Talk_unk218_2(14, 0);
    }

    if (Talk_unk34(0, 0) == 0) {
        Talk_unk219_2(20);
        Talk_unk220_2(20, 3);
        Talk_unk221_2(20);
        Talk_unk222_2(20, 0);
        bump_step(1);
    } else {
        Talk_unk223_2(10);
        Talk_unk224_2(20, 3);
        Talk_unk225_2(20);
        bump_step(1);
        Talk_unk226_2(20, 0);
    }

    Talk_unk227_2(10);
    Talk_unk228_2(14, 0xa000, 0);
    Talk_unk229_2(20);
    Talk_unk230_2(14, 3);
    Talk_unk231_2(30);
    Talk_unk232_2(14, 0);
    Talk_unk233_2(10);
    Talk_unk234_2(20, 3);
    Talk_unk235_2(30);
    Talk_unk236_2(10);
    Talk_unk237_2(1, 4, 13);
    Talk_unk238_2(1, 4, 30);
    Talk_unk239_2(1, 0);
    Talk_unk240_2(10);
    Talk_unk19_3(14, 0x4000, 0);
    Talk_unk241_2(20, 2);
    Talk_unk242_2(20);
    Talk_unk20_3(14, 0x19999, 0xcccc);
    Talk_unk243_2(20, 0, 16);
    Talk_unk244_2(2);
    Talk_unk245_2(20, 0x2000, 0);
    Talk_unk246_2(10);
    Talk_unk247_2(20, 0);
    Talk_unk248_2(10);
    Talk_unk249_2(14, 4);
    Talk_unk250_2(30);
    Talk_unk251_2(14, 20, 30);
    Talk_unk252_2(14, 0);
    Talk_unk253_2(10);
    Talk_unk254_2(20, 2);
    Talk_unk255_2(20);
    Talk_unk256_2(14, 1, 0);
    Talk_unk257_2(40);
    Talk_unk258_2(14, 0);
    Talk_unk259_2(10);
    Talk_unk260_2(1, 2);
    Talk_unk261_2(30);
    Talk_unk262_2(1, 2, 30);
    Talk_unk263_2(10);
    Talk_unk264_2(2, 2);
    Talk_unk265_2(20);
    Talk_unk266_2(2, 1, 30);
    Talk_unk267_2(2, 0);
    Talk_unk268_2(10);
    Talk_unk269_2(1, 0x106, 50);
    Talk_unk270_2(10);
    Talk_unk271_2(14, 3);
    Talk_unk272_2(30);
    Talk_unk273_2(14, 0);
    Talk_unk274_2(10);
    Talk_unk21_3(1, 0xc000, 0);
    Talk_unk22_3(2, 0xc000, 0);
    Talk_unk275_2(30);
    Talk_unk276_2(10);
    Talk_unk277_2(20, 2);
    Talk_unk278_2(20);
    Talk_unk279_2(20, 0);
    Talk_unk280(10);
    Talk_unk281(1, 0, 0);
    Talk_unk282(3, 2, 50);
    Talk_unk23_3(0, 0xc000, 0);
    Talk_unk24_3(1, 0xc000, 0);
    Talk_unk25_3(3, 0xc000, 0);
    Talk_unk283(2, 0xc000, 0);
    Talk_unk284(30);
    Talk_unk285(10);
    Talk_unk286(20, 4);
    Talk_unk287(20);
    Talk_unk288(20, 0);
    Talk_unk289(10);
    Talk_unk290(14, 4);
    Talk_unk291(20);
    Talk_unk292(14, 0);
    Talk_unk293(10);
    Talk_unk26_3(2, 0x101, 40);
    Talk_unk294(2, 0);
    Talk_unk295(10);
    Talk_unk296(14, 2);
    Talk_unk297(20);
    Talk_unk35(14, 0);
    if (Talk_unk36(0, 0) == 0) {
        Talk_unk298(20);
        Talk_unk299(14, 3);
        Talk_unk300(30);
        Talk_unk301(14, 0);
        bump_step(1);
    } else {
        Talk_unk302(10);
        Talk_unk303(14, 4);
        Talk_unk304(20);
        bump_step(1);
        Talk_unk305(14, 0);
    }

    Talk_unk306(10);
    Talk_unk27_3(20, 0x102, 40);
    Talk_unk307(20, 0);
    Talk_unk308(20);
    Talk_unk309(0, 3);
    Talk_unk310(1, 3);
    Talk_unk311(3, 3);
    Talk_unk312(2, 3);
    Talk_unk313(50);
    Talk_unk314(14, 20, 60);
    Talk_unk315(14, 0x4000, 0);
    Talk_unk316(20);
    Talk_unk317(14, 0);
    Talk_unk318(10);
    Talk_unk28_3(20, 0x102, 40);
    Talk_unk319(20, 0);
    Talk_unk320(10);
    Talk_unk321(20, 4);
    Talk_unk322(20);
    Talk_unk323(20, 0);
    Talk_unk324(10);
    Talk_unk29_3(0, 0x102, 40);
    Talk_unk325(10);
    Talk_unk326(14, 4);
    Talk_unk327(20);
    Talk_unk328(14, 0);
    Talk_unk329(10);
    Talk_unk330(1, 0, 50);
    Talk_unk30_3(0, 0xc000, 0);
    Talk_unk31_3(1, 0xc000, 0);
    Talk_unk331(30);
    Talk_unk332(10);
    Talk_unk333(0, 3);
    Talk_unk334(1, 3);
    Talk_unk335(30);
    Talk_unk336(10);
    Talk_unk337(14, 3);
    Talk_unk338(30);
    Talk_unk339(14, 0);
    Talk_unk340(10);
    Talk_unk32_3(3, 0x100, 40);
    Talk_unk341(3, 0);
    Talk_unk342(10);
    Talk_unk33_3(14, 0x102, 50);
    Talk_unk343(20, 14, 50);
    Talk_unk344(10);
    Talk_unk345(14, 2);
    Talk_unk346(20);
    Talk_unk347(14, 0);
    Talk_unk348(20);
    Talk_unk349(1, 0, 0);
    Talk_unk350(3, 2, 50);
    Talk_unk34_3(0, 0xc000, 0);
    Talk_unk35_3(1, 0xc000, 0);
    Talk_unk36_3(3, 0xc000, 0);
    Talk_unk37_3(2, 0xc000, 0);
    Talk_unk351(50);
    Talk_unk352(20, 0x2000, 0);
    Talk_unk353(20);
    Talk_unk354(20, 0);
    Talk_unk355(10);
    Talk_unk356(1, 2);
    Talk_unk357(20);
    Talk_unk358(1, 0);
    Talk_unk359(20);
    Talk_unk360(14, 0x8000, 0);
    Talk_unk361(40);
    Talk_unk38_3(14, 0x105, 60);
    Talk_unk362(14, 0);
    Talk_unk363(10);
    Talk_unk39_3(2, 0x100, 40);
    Talk_unk364(2, 0);
    Talk_unk365(10);
    Talk_unk366(14, 2, 40);
    Talk_unk367(14, 3);
    Talk_unk368(30);
    Talk_unk369(14, 0);
    Talk_unk370(10);
    Talk_unk371(1, 0, 0);
    Talk_unk372(3, 2, 60);
    Talk_unk40_3(0, 0xc000, 0);
    Talk_unk41_3(1, 0xc000, 0);
    Talk_unk42_3(3, 0xc000, 0);
    Talk_unk373(2, 0xc000, 0);
    Talk_unk374(50);
    Talk_unk375(20, 2);
    Talk_unk376(20);
    Talk_unk377(20, 0);
    Talk_unk378(10);
    Talk_unk379(1, 0, 40);
    Talk_unk380(1, 2);
    Talk_unk381(20);
    Talk_unk37(1, 0);
    if (Talk_unk38(0, 0) == 0) {
        Talk_unk382(20);
        Talk_unk383(1, 0);
        bump_step(3);
    } else {
        do {
            Talk_unk39(10);
        } while (0);
        {
            u8 **scene_address = (u8 **)0x03001ebc;

            bump_step_at(scene_address, 1);
            Talk_unk40(1, 0);
            if (Talk_unk41(0, 0) == 0) {
                Talk_unk384(20);
                Talk_unk385(1, 0);
                bump_step_at(scene_address, 1);
            } else {
                bump_step_at(scene_address, 1);
                Talk_unk386(1, 0);
            }
        }
    }

    Talk_unk387(10);
    Talk_unk388(14, 4);
    Talk_unk389(20);
    Talk_unk390(14, 0);
    Talk_unk391(10);
    Talk_unk43_3(0, 0xc000, 0);
    Talk_unk44_3(1, 0xc000, 0);
    Talk_unk392(40);
    Talk_unk393(1, 2);
    Talk_unk394(20);
    Talk_unk395(10);
    Talk_unk396(14, 3);
    Talk_unk397(30);
    Talk_unk398(14, 0);
    Talk_unk399(10);
    Talk_unk45_3(3, 0x101, 40);
    Talk_unk400(3, 0);
    Talk_unk401(10);
    Talk_unk402(14, 2);
    Talk_unk403(20);
    Talk_unk404(14, 0);
    Talk_unk405(10);
    Talk_unk406(2, 2);
    Talk_unk407(20);
    Talk_unk408(2, 0);
    Talk_unk409(10);
    Talk_unk410(20, 14, 40);
    Talk_unk411(20, 2);
    Talk_unk412(20);
    Talk_unk413(20, 0);
    Talk_unk414(20);
    Talk_unk46_3(14, 0x105, 70);
    Talk_unk415(14, 0);
    Talk_unk416(10);
    Talk_unk47_3(0, 0x101, 0);
    Talk_unk48_3(1, 0x101, 0);
    Talk_unk49_3(3, 0x101, 0);
    Talk_unk50_3(2, 0x101, 40);
    Talk_unk417(10);
    Talk_unk418(14, 4);
    Talk_unk419(20);
    Talk_unk420(14, 0);
    Talk_unk421(10);
    Talk_unk422(14, 3);
    Talk_unk423(30);
    Talk_unk424(14, 0);
    Talk_unk425(20);
    Talk_unk426(1, 3);
    Talk_unk427(30);
    Talk_unk428(1, 0);
    Talk_unk429(20);
    Talk_unk430(14, 3);
    Talk_unk431(30);
    Talk_unk432(10);
    Talk_unk51_3(2, 0x101, 40);
    Talk_unk433(2, 0);
    Talk_unk434(10);
    Talk_unk435(14, 3);
    Talk_unk436(40);
    Talk_unk437(0, 2, 0);
    Talk_unk438(1, 2, 0);
    Talk_unk439(3, 2, 0);
    Talk_unk440(20, 2, 0);
    Talk_unk441(50);
    Talk_unk442(10);
    Talk_unk443(2, 2);
    Talk_unk444(20);
    Talk_unk445(2, 0);
    Talk_unk446(10);
    Talk_unk52_3(14, 0x101, 40);
    Talk_unk447(14, 0);
    Talk_unk448(10);
    Talk_unk449(0, 3);
    Talk_unk450(1, 3);
    Talk_unk451(3, 3);
    Talk_unk452(30);
    Talk_unk453(10);
    Talk_unk454(2, 4);
    Talk_unk455(20);
    Talk_unk456(2, 0);
    Talk_unk457(20);
    Talk_unk458(20, 3);
    Talk_unk459(30);
    Talk_unk460(20, 14, 30);
    Talk_unk53_3(0, 0xc000, 0);
    Talk_unk54_3(1, 0xc000, 0);
    Talk_unk55_3(3, 0xc000, 0);
    Talk_unk461(20);
    Talk_unk462(20, 0);
    Talk_unk463(10);
    Talk_unk464(14, 3);
    Talk_unk465(30);
    Talk_unk466(14, 0);
    Talk_unk467(10);
    Talk_unk468(0, 2);
    Talk_unk469(1, 2);
    Talk_unk470(3, 2);
    Talk_unk471(2, 2);
    Talk_unk472(30);
    Talk_unk473(10);
    Talk_unk474(20, 2);
    Talk_unk475(20);
    Talk_unk476(20, 0);
    Talk_unk477(10);
    Talk_unk56_3(14, 0x8000, 0);
    Talk_unk478(20);
    Talk_unk479(14, 4);
    Talk_unk480(20);
    Talk_unk481(14, 0);
    Talk_unk482(10);
    Talk_unk483(1, 0, 0);
    Talk_unk484(3, 2, 50);
    Talk_unk57_3(0, 0xc000, 0);
    Talk_unk58_3(1, 0xc000, 0);
    Talk_unk59_3(3, 0xc000, 0);
    Talk_unk60_3(2, 0xc000, 0);
    Talk_unk485(40);
    Talk_unk61_3(14, 0x4000, 0);
    Talk_unk486(30);
    Talk_unk62_3(14, 0x102, 40);
    Talk_unk487(14, 0);
    Talk_unk488(10);
    Talk_unk63_3(20, 0x102, 40);
    Talk_unk489(20, 0);
    Talk_unk490(20);
    Talk_unk491(14, 20, 40);
    Talk_unk492(14, 2);
    Talk_unk493(20);
    Talk_unk494(14, 0);
    Talk_unk495(10);
    Talk_unk6_4(20, 0x102);
    Talk_unk496(20, 2);
    Talk_unk497(20);
    Talk_unk498(20, 0);
    Talk_unk64_3(20, 0x19999, 0xcccc);
    Talk_unk499(20, 0, 24);
    Talk_unk500(0, 20, 0);
    Talk_unk501(1, 20, 0);
    Talk_unk502(3, 20, 0);
    Talk_unk503(2, 20, 0);
    Talk_unk65_3(14, 0x4000, 0);
    Talk_unk504(14, 0);
    Talk_unk505(10);
    Talk_unk66_3(20, 0x100, 60);
    Talk_unk506(20, 14, 0);
    Talk_unk67_3(0, 0xc000, 0);
    Talk_unk68_3(1, 0xc000, 0);
    Talk_unk69_3(3, 0xc000, 0);
    Talk_unk70_3(2, 0xc000, 0);
    Talk_unk507(10);
    Talk_unk508(20, 2);
    Talk_unk509(20);
    Talk_unk510(20, 0);
    Talk_unk511(30);
    Talk_unk71_3(14, 0x105, 40);
    Talk_unk512(14, 0);
    Talk_unk513(10);
    Talk_unk72_3(20, 0x101, 60);
    Talk_unk73_3(20, 0x13333, 0x9999);
    Talk_unk74_3(20, 0, -24);
    Talk_unk514(20, 0, 0);
    Talk_unk515(20);
    Talk_unk516(20, 0);
    Talk_unk517(10);
    Talk_unk75_3(14, 0x8000, 0);
    Talk_unk518(20);
    Talk_unk519(14, 3);
    Talk_unk520(30);
    Talk_unk521(10);
    Talk_unk522(20, 2);
    Talk_unk523(20);
    Talk_unk524(20, 0);
    Talk_unk525(10);
    Talk_unk526(14, 4);
    Talk_unk527(20);
    Talk_unk528(14, 0);
    Talk_unk529(10);
    Talk_unk76_3(20, 0x102, 40);
    Talk_unk530(20, 0);
    Talk_unk531(10);
    Talk_unk532(14, 3);
    Talk_unk533(30);
    Talk_unk534(14, 0);
    Talk_unk535(10);
    Talk_unk536(1, 2);
    Talk_unk537(20);
    Talk_unk538(1, 0);
    Talk_unk539(10);
    Talk_unk77_2(14, 0x4000, 0);
    Talk_unk540(30);
    Talk_unk541(14, 3);
    Talk_unk542(30);
    Talk_unk543(14, 0);
    Talk_unk544(10);
    Talk_unk78_2(3, 0x102, 40);
    Talk_unk545(3, 0);
    Talk_unk546(10);
    Talk_unk547(14, 3);
    Talk_unk548(30);
    Talk_unk549(14, 0);
    Talk_unk550(10);
    Talk_unk551(2, 3);
    Talk_unk552(30);
    Talk_unk553(2, 0);
    Talk_unk554(10);
    Talk_unk555(14, 3);
    Talk_unk556(30);
    Talk_unk557(14, 0);
    Talk_unk558(10);
    Talk_unk559(1, 0, 0);
    Talk_unk560(3, 2, 50);
    Talk_unk79_2(0, 0xc000, 0);
    Talk_unk80_2(1, 0xc000, 0);
    Talk_unk81_2(3, 0xc000, 0);
    Talk_unk82_2(2, 0xc000, 0);
    Talk_unk561(30);
    Talk_unk562(10);
    Talk_unk563(14, 2);
    Talk_unk564(20);
    Talk_unk565(14, 0);
    Talk_unk566(20);
    Talk_unk83_2(0, 0x101, 0);
    Talk_unk84_2(1, 0x101, 0);
    Talk_unk85_2(3, 0x101, 0);
    Talk_unk86_2(2, 0x101, 50);
    Talk_unk567(10);
    Talk_unk568(14, 3);
    Talk_unk569(30);
    Talk_unk570(14, 0);
    Talk_unk571(10);
    Talk_unk87_2(20, 0x102, 40);
    Talk_unk572(20, 0);
    Talk_unk573(10);
    Talk_unk574(14, 20, 30);
    Talk_unk575(14, 0);
    Talk_unk576(10);
    Talk_unk577(14, 4);
    Talk_unk578(20);
    Talk_unk579(14, 0);
    Talk_unk580(10);
    Talk_unk581(1, 2);
    Talk_unk582(20);
    Talk_unk583(1, 0);
    Talk_unk584(10);
    Talk_unk585(14, 2);
    Talk_unk586(20);
    Talk_unk88_2(14, 0x4000, 0);
    Talk_unk587(20);
    Talk_unk588(14, 0);
    Talk_unk589(10);
    Talk_unk89_2(20, 0x101, 40);
    Talk_unk590(20, 0);
    Talk_unk591(10);
    Talk_unk592(14, 3);
    Talk_unk593(30);
    Talk_unk594(14, 0x8000, 0);
    Talk_unk595(20);
    Talk_unk596(14, 0);
    Talk_unk597(10);
    Talk_unk90_2(2, 0x101, 40);
    Talk_unk598(2, 0);
    Talk_unk599(10);
    Talk_unk91_2(14, 0x4000, 0);
    Talk_unk600(20, 0x2000, 0);
    Talk_unk601(30);
    Talk_unk602(14, 3);
    Talk_unk603(30);
    Talk_unk604(10);
    Talk_unk605(3, 2);
    Talk_unk606(20);
    Talk_unk607(3, 0);
    Talk_unk608(10);
    Talk_unk92_2(14, 0x102, 40);
    Talk_unk609(14, 0);
    Talk_unk610(20);
    Talk_unk611(0, 3);
    Talk_unk612(1, 3);
    Talk_unk613(3, 3);
    Talk_unk614(2, 3);
    Talk_unk615(30);
    Talk_unk616(20);
    Talk_unk93_2(14, 0x105, 60);
    Talk_unk617(14, 0);
    Talk_unk618(10);
    Talk_unk94_2(1, 0x101, 40);
    Talk_unk619(1, 0);
    Talk_unk620(10);
    Talk_unk95_2(14, 0x100, 40);
    Talk_unk621(14, 0);
    Talk_unk42(10);
    Talk_unk96_2(1, 0, 50);
    Talk_unk97_2(0, 0xc000, 0);
    Talk_unk98_2(1, 0xc000, 0);
    Talk_unk622(30);
    Talk_unk623(10);
    Talk_unk624(14, 3);
    Talk_unk43(30);
    Talk_unk625(14, 0);
    Talk_unk626(10);
    Talk_unk99_2(3, 0x101, 40);
    Talk_unk627(3, 0);
    Talk_unk628(10);
    Talk_unk44(14, 4);
    Talk_unk629(20);
    Talk_unk630(14, 0);
    Talk_unk631(10);
    Talk_unk100_2(0, 0x101, 0);
    Talk_unk101_2(1, 0x101, 0);
    Talk_unk102_2(2, 0x101, 0);
    Talk_unk103_2(3, 0x101, 40);
    Talk_unk632(10);
    Talk_unk633(14, 4);
    Talk_unk634(20);
    Talk_unk635(14, 0);
    Talk_unk636(10);
    Talk_unk104_2(2, 0x100, 40);
    Talk_unk45(2, 0);
    if (Talk_unk46(14, 0) == 0) {
        Talk_unk47(10);
        Talk_unk637(2, 0);
        bump_step(1);
    } else {
        Talk_unk638(10);
        bump_step(1);
        Talk_unk639(2, 0);
    }

    Talk_unk640(10);
    Talk_unk641(14, 2);
    Talk_unk642(20);
    Talk_unk643(14, 0);
    Talk_unk644(10);
    Talk_unk645(14, 4);
    Talk_unk646(20);
    Talk_unk647(14, 0);
    Talk_unk648(10);
    Talk_unk649(20, 2);
    Talk_unk48(20);
    Talk_unk650(20, 0, 0);
    Talk_unk651(20);
    Talk_unk652(20, 0);
    Talk_unk653(10);
    Talk_unk654(14, 20, 40);
    Talk_unk655(14, 4);
    Talk_unk656(30);
    Talk_unk657(14, 0);
    Talk_unk658(10);
    Talk_unk659(1, 0, 40);
    Talk_unk660(1, 0);
    Talk_unk105_2(14, 0x4000, 0);
    Talk_unk106_2(20, 0x2000, 0);
    Talk_unk661(30);
    Talk_unk662(14, 0);
    Talk_unk663(10);
    Talk_unk107_2(0, 0xc000, 0);
    Talk_unk108_2(1, 0xc000, 0);
    Talk_unk664(30);
    Talk_unk665(3, 2, 60);
    Talk_unk109_2(3, 0xc000, 0);
    Talk_unk666(2, 0xc000, 0);
    Talk_unk667(30);
    Talk_unk668(3, 0);
    Talk_unk669(10);
    Talk_unk670(2, 4);
    Talk_unk671(20);
    Talk_unk672(2, 0);
    Talk_unk673(10);
    Talk_unk674(20, 14, 60);
    Talk_unk110_2(20, 0x2000, 0);
    Talk_unk111_2(14, 0x4000, 0);
    Talk_unk675(30);
    Talk_unk676(14, 0x4000, 0);
    Talk_unk677(10);
    Talk_unk678(14, 3);
    Talk_unk679(30);
    Talk_unk680(14, 0);
    Talk_unk681(10);
    Talk_unk682(20, 2);
    Talk_unk683(20);
    Talk_unk49(20, 0);
    if (Talk_unk50(0, 0) == 0) {
        Talk_unk684(20);
        Talk_unk112_2(1, 0x102, 40);
        Talk_unk685(1, 0);
        bump_step(1);
    } else {
        Talk_unk686(10);
        bump_step(1);
        Talk_unk687(1, 0);
    }

    Talk_unk688(10);
    Talk_unk689(20, 2);
    Talk_unk690(20);
    Talk_unk691(20, 0);
    Talk_unk692(10);
    Talk_unk113_2(20, 0x100, 40);
    Talk_unk693(20, 0);
    Talk_unk694(20);
    Talk_unk695(0, 3);
    Talk_unk696(1, 3);
    Talk_unk697(3, 3);
    Talk_unk698(2, 3);
    Talk_unk699(30);
    Talk_unk700(10);
    Talk_unk114_2(14, 0x100, 40);
    Talk_unk701(14, 0);
    Talk_unk702(10);
    Talk_unk703(20, 14, 40);
    Talk_unk704(10);
    Talk_unk115_2(20, 0x102, 40);
    Talk_unk705(20, 0);
    Talk_unk706(10);
    Talk_unk707(14, 20, 40);
    Talk_unk708(14, 3);
    Talk_unk709(30);
    Talk_unk710(14, 0);
    Talk_unk711(10);
    Talk_unk712(20, 2);
    Talk_unk713(20);
    Talk_unk714(20, 0);
    Talk_unk715(14, 0x4000, 0);
    Talk_unk716(20);
    Talk_unk717(14, 2);
    Talk_unk718(20);
    Talk_unk719(14, 0);
    Talk_unk720(10);
    Talk_unk116_2(0, 0x101, 80);
    Talk_unk117_2(20, 0x102, 70);
    Talk_unk721(14, 4);
    Talk_unk722(20);
    Talk_unk723(14, 0);
    Talk_unk724(10);
    Talk_unk725(1, 0, 0);
    Talk_unk726(3, 2, 50);
    Talk_unk118_2(0, 0xc000, 0);
    Talk_unk119_2(1, 0xc000, 0);
    Talk_unk120_2(3, 0xc000, 0);
    Talk_unk727(2, 0xc000, 0);
    Talk_unk728(30);
    Talk_unk729(10);
    Talk_unk121_2(20, 0x106, 50);
    Talk_unk730(20, 0);
    Talk_unk731(10);
    Talk_unk732(14, 3);
    Talk_unk733(30);
    Talk_unk122_2(20, 0x2000, 0);
    Talk_unk734(20, 0);
    Talk_unk735(10);
    Talk_unk123_2(14, 0x108, 40);
    Talk_unk51(14, 0);
    if (Talk_unk52(0, 0) == 0) {
        Talk_unk736(20);
        Talk_unk124_2(14, 0x100, 40);
        Talk_unk737(14, 0);
        bump_step(1);
    } else {
        Talk_unk738(10);
        Talk_unk125_2(14, 0x100, 40);
        bump_step(1);
        Talk_unk739(14, 0);
    }

    Talk_unk740(10);
    Talk_unk741(20, 2);
    Talk_unk742(20);
    Talk_unk743(20, 0);
    Talk_unk744(10);
    Talk_unk745(14, 20, 40);
    Talk_unk746(14, 3);
    Talk_unk747(30);
    Talk_unk748(20, 0);
    Talk_unk749(10);
    Talk_unk750(20, 14, 40);
    Talk_unk751(20, 0);
    Talk_unk752(10);
    Talk_unk126_2(14, 0x100, 40);
    Talk_unk127_2(14, 0x8000, 0);
    Talk_unk753(20);
    Talk_unk754(14, 0);
    Talk_unk755(10);
    Talk_unk128_2(20, 0x100, 40);
    Talk_unk756(20, 0);
    Talk_unk757(10);
    Talk_unk758(14, 2);
    Talk_unk759(20);
    Talk_unk760(14, 0);
    Talk_unk761(10);
    Talk_unk762(20, 2);
    Talk_unk763(20);
    Talk_unk764(20, 0);
    Talk_unk765(10);
    Talk_unk766(20, 3);
    Talk_unk767(30);
    Talk_unk768(14, 3);
    Talk_unk769(30);
    Talk_unk770(20, 0x2000, 0);
    Talk_unk771(20);
    Talk_unk772(0, 20);
    Talk_unk773(1, 20);
    Talk_unk774(3, 20);
    Talk_unk775(2, 20);
    Talk_unk129_2(20, 0x10000, 0x8000);
    Talk_unk776(20, 0, 32);
    Talk_unk777(20, 0, 0);
    Talk_unk130_2(14, 0x4000, 0);
    Talk_unk778(20);
    Talk_unk779(20, 0);
    Talk_unk780(10);
    Talk_unk781(20, 3);
    Talk_unk782(30);
    Talk_unk783(1, 16, 0);
    Talk_unk784(20, 0, 80);
    Talk_unk785(40);
    Talk_unk131_2(1, -16, 0);
    Talk_unk132_2(1, 0x4000, 0);
    Talk_unk133_2(3, 0x4000, 0);
    Talk_unk786(20);
    Talk_unk787(80);
    Talk_unk788(20, 0, 0);
    Talk_unk134_2(0, 0xc000, 0);
    Talk_unk135_2(1, 0xc000, 0);
    Talk_unk136_2(3, 0xc000, 0);
    Talk_unk789(2, 0xc000, 0);
    Talk_unk790(30);
    Talk_unk791(0, 14);
    Talk_unk792(1, 14);
    Talk_unk793(3, 14);
    Talk_unk794(2, 14);
    Talk_unk795(30);
    Talk_unk796(14, 3);
    Talk_unk797(30);
    Talk_unk137_2(14, 0xcccc, 0x6666);
    Talk_unk798(14, 0, 24);
    Talk_unk799(14, -80, 0);
    Talk_unk800(10);
    Talk_unk801(14, 0, 0);
    Talk_unk802(20);
    Talk_unk803(14, 0);
    Talk_unk804(20);
    Talk_unk805(14, 0x4000, 0);
    Talk_unk806(20);
    Talk_unk138_2(14, 0, 48);
    Talk_unk807(14, -64, 0);
    Talk_unk808(0);
    Talk_unk809(1);
    Talk_unk810(3);
    Talk_unk811(2);
    Talk_unk812(14, 0, 0);
    Talk_unk813(20);
    Talk_unk814(0, 3, 0);
    Talk_unk815(1, 2, 0);
    Talk_unk816(30);
    Talk_unk817(0, 3);
    Talk_unk818(1, 3);
    Talk_unk819(3, 3);
    Talk_unk139_3(2, 3);
    Talk_unk820(30);
    Talk_unk821(17);
    Talk_unk139_2(1, 0x13333, 0x9999);
    Talk_unk140_2(2, 0x13333, 0x9999);
    Talk_unk141_2(3, 0x13333, 0x9999);
    Talk_unk53(1, 2);
    position = Talk_unk138_3(0);
    if (position != 0)
        Talk_unk822(1, position[5], position[9]);
    Talk_unk823(1);
    Talk_unk824(1, 0, 0);
    Talk_unk54(2, 2);
    position = Talk_unk140_3(0);
    if (position != 0)
        Talk_unk825(2, position[5], position[9]);
    Talk_unk826(2);
    Talk_unk827(2, 0, 0);
    Talk_unk55(3, 2);
    position = Talk_unk141_3(0);
    if (position != 0)
        Talk_unk828(3, position[5], position[9]);
    Talk_unk829(3);
    Talk_unk56(3, 0, 0);
    carry = Talk_unk57(10);
    carry = Talk_unk58(carry);
    Talk_unk830(carry);
}

/* overlays/scene/dialogue/choice/run_supplemental_sequence_one.c */
extern u8 Value_00002233;

/* Supplemental scene sequence of the resource_3b8 overlay. */

void RunSupplementalSequenceOne(void)
{
    s32 p;
    Talk_Do(2412);
    Talk_unk7_4();
    Talk_unk8_4();
    Talk_unk142_2(8, 20480, 0);
    Talk_unk143_2(9, 12288, 0);
    Talk_unk144_2(0, 200, 272);
    Talk_unk145_2(0, 49152, 0);
    Talk_unk2_2(20);
    p = (s32)&Value_00002233;
    Talk_unk831(p);
    Talk_unk4(8, 0);
    if (Talk_unk5(0, 0) == 0) {
        Talk_unk3_2(20);
        Talk_unk832(p + 1);
        Talk_unk9_4(8, 0);
    } else {
        Talk_unk6(20);
        Talk_unk833(p + 2);
        Talk_unk10_4(8, 0);
        Talk_unk4_2(20);
        Talk_unk146_2(8, 9, 60);
        Talk_unk147_2(9, 12288, 0);
        Talk_unk5_2(40);
        Talk_unk11_4(9, 2);
        Talk_unk6_2(30);
        Talk_unk148_2(8, 9, 30);
        Talk_unk12_4(9, 3);
        Talk_unk7_2(30);
        Talk_unk149_2(8, 258, 50);
        Talk_unk150_2(8, 20480, 0);
        Talk_unk151(9, 12288, 0);
        Talk_unk8_2(20);
        Talk_unk13_4(8, 4);
        Talk_unk9_2(20);
        Talk_unk14_4(8, 0);
        Talk_unk10_2(10);
        Talk_unk15_4(8, 2);
        Talk_unk11_2(20);
        Talk_unk16_4(8, 0);
    }
    Talk_unk17_4();
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
    if (Talk_unk59(0x950) != 0)
        return (s32)gOv6;
    if (Talk_unk60(0x962) != 0)
        return (s32)gOv7;
    return (s32)gOv8;
}

/* overlays/scene/dialogue/choice/scene_dialogue.c */
extern u8 gWork[];

void Dialogue_ShowLine2239Or223A(void)
{
    u32 i;
    s32 record;

    if (Talk_unk7(0x96d) == 0) {
        Talk_unk12_2(0x96d);
        Talk_unk13_2(0x2239);
        Talk_unk834(9, 0);
    } else {
        Talk_unk14_2(0x223a);
        Talk_unk835(9, 0);
    }
}

/* overlays/scene/dialogue/choice/scene_effect_sequence_head.c */
extern u8 Value_00001ff8;
extern u8 gOv9[];

u8 *Talk_unk836();

/* Primary effect sequence of the resource_3b8 overlay: two hundred and fifty scene calls with constant arguments. */

void RunOpeningAuxiliarySequence(s32 a)
{
    void Talk_unk837();

    u8 *ret;
    s16 v;
    s32 c;
    s32 t;

    ret = Talk_unk836(0);
    v = (*(u16 *)(ret + 6) + 0x2000) & 0xc000;
    Talk_unk838();
    Talk_unk839();
    if (Talk_unk8(512) == 0) {
        Talk_unk15_2(512);
        Talk_unk840(0x969);
        Talk_unk841(0x1ff7);
        Talk_unk842(a, 0);
        Talk_unk843(10);
        t = v << 16;
        c = 0x4000;
        if (t == (0x4000 << 16)) {
            Talk_unk844(0, 40, 104);
            Talk_unk845(0, 0, 0);
        }
        Talk_unk152(a, 0x10000, 0x8000);
        Talk_unk153(a, 0, -48);
        Talk_unk846(a, 64, 0);
        Talk_unk847(a, c, 0);
    } else {
        Talk_unk16_2(512);
        Talk_unk848(0x969);
        Talk_unk154(a, 0x4000, 0);
        Talk_unk849(0, 120, 96);
        Talk_unk850(0, 0xc000, 0);
        Talk_unk851(20);
        c = (s32)&Value_00001ff8;
        Talk_unk852(c);
        Talk_unk61(a, 0);
        if (Talk_unk62(0, 0) == 0) {
            Talk_unk853(c + 1);
            Talk_unk854(a, 0);
        } else {
            Talk_unk855(c + 2);
            Talk_unk856(a, 0);
        }
        Talk_unk857(10);
        Talk_unk858(a, 3);
        Talk_unk859(20);
        Talk_unk155(a, -64, 0);
        Talk_unk860(a, 0, 48);
    }
    Talk_unk861();
}

void RunMiddleAuxiliarySequence(s32 a)
{
    u8 *obj;
    u8 *q;

    obj = Talk_unk837();
    Talk_unk862();
    q = gOv9;
    Talk_unk863(a, q);
    Talk_unk864(0x2009);
    Talk_unk865(a, 0);
    Talk_unk866(a);
    *(s32 *)(obj + 28) = 0x10000;
    *(s32 *)(obj + 24) = 0x10000;
    Talk_unk867(30);
    Talk_unk868(a, 2);
    Talk_unk869(30);
    Talk_unk870(a, 2);
    Talk_unk871(60);
    Talk_unk872(a, 0);
    Talk_unk873(20);
    Talk_unk874(a, 258, 60);
    Talk_unk875(a, 2);
    Talk_unk876(30);
    Talk_unk877(a, 2);
    Talk_unk878(30);
    Talk_unk879(a, 2);
    Talk_unk880(30);
    Talk_unk881(a, q);
    Talk_unk882(a, 0);
    Talk_unk156(a, 0xe000, 0);
    Talk_unk883(10);
    *(s32 *)(obj + 28) = 0x10000;
    *(s32 *)(obj + 24) = 0x10000;
    Talk_unk884(a, q);
    Talk_unk885();
}

    Talk_unk18_4();
    Talk_unk19_4();
    Talk_unk17_2(8225);
    Talk_unk157(0, 520, 424);
    Talk_unk158(0, 57344, 0);
    Talk_unk20_4(36700160, -1, 24117248, 1);
    Talk_unk21_4();
    Talk_unk159(20, 65536, 32768);
    Talk_unk160(20, 40, 0);
    Talk_unk161(20, 584, 360);
    Talk_unk22_4(21, 0);
    Talk_unk18_2(10);
    Talk_unk162(20, 257, 40);
    Talk_unk163(20, 32768, 0);
    Talk_unk164(21, 131072, 65536);
    Talk_unk165(22, 131072, 65536);
    Talk_unk166(21, 528, 352);
    Talk_unk167(22, 528, 368);
    Talk_unk19_2(3);
    Talk_unk23_4(21, 1);
    Talk_unk20_2(30);
    Talk_unk21_2(10);
    Talk_unk24_4(22, 3);
    Talk_unk22_2(30);
    Talk_unk25_4(22, 0);
    Talk_unk23_2(10);
    Talk_unk26_4(20, 2);
    Talk_unk24_2(20);
    Talk_unk168(20, -16, 0);
    Talk_unk25_2(10);
    Talk_unk27_4(20, 0);
    Talk_unk26_2(10);
    Talk_unk28_4(21, 4);
    Talk_unk29_4(22, 4);
    Talk_unk27_2(20);
    Talk_unk28_2(10);
    Talk_unk169(20, 261, 40);
    Talk_unk30_4(20, 0);
    Talk_unk29_2(10);
    Talk_unk170(21, 258, 40);
    Talk_unk171(21, 65536, 32768);
    Talk_unk172(21, 8, 0);
    Talk_unk30_2(20);
    Talk_unk31_4(21, 0);
    Talk_unk31_2(10);
    Talk_unk173(20, 257, 80);
    Talk_unk32_2(10);
    Talk_unk174(22, 258, 40);
    Talk_unk175(22, 65536, 32768);
    Talk_unk176(22, 8, 0);
    Talk_unk33_2(20);
    Talk_unk32_4(22, 0);
    Talk_unk34_2(10);
    Talk_unk33_4(20, 4);
    Talk_unk35_2(20);
    Talk_unk34_4(20, 0);
    Talk_unk36_2(10);
    Talk_unk35_4(21, 3);
    Talk_unk36_4(22, 3);
    Talk_unk37_2(30);
    Talk_unk38_2(10);
    Talk_unk37_4(21, 2);
    Talk_unk39_2(20);
    Talk_unk38_4(21, 0);
    Talk_unk40_2(10);
    Talk_unk39_4(22, 4);
    Talk_unk41_2(20);
    Talk_unk40_4(22, 0);
    Talk_unk42_2(10);
    Talk_unk177(20, 261, 40);
    Talk_unk41_4(20, 0);
    Talk_unk43_2(10);
    Talk_unk178(21, 257, 40);
    Talk_unk42_4(21, 0);
    Talk_unk44_2(10);
    Talk_unk179(20, 258, 40);
    Talk_unk43_4(20, 0);
    Talk_unk45_2(10);
    Talk_unk44_4(22, 2);
    Talk_unk46_2(20);
    Talk_unk45_4(22, 0);
    Talk_unk47_2(10);
    Talk_unk46_4(20, 3);
    Talk_unk48_2(30);
    Talk_unk49_2(10);
    Talk_unk47_4(21, 2);
    Talk_unk50_2(20);
    Talk_unk48_4(21, 0);
    Talk_unk51_2(10);
    Talk_unk180(20, 258, 40);
    Talk_unk49_4(20, 0);
    Talk_unk52_2(20);
    Talk_unk181(21, 22, 60);
    Talk_unk182(21, 20, 0);
    Talk_unk183(22, 20, 0);
    Talk_unk53_2(20);
    Talk_unk54_2(10);
    Talk_unk50_4(20, 2);
    Talk_unk55_2(20);
    Talk_unk51_4(20, 0);
    Talk_unk56_2(10);
    Talk_unk184(21, 256, 40);
    Talk_unk52_4(21, 0);
    Talk_unk57_2(10);
    Talk_unk53_4(22, 3);
    Talk_unk58_2(30);
    Talk_unk54_4(22, 0);
    Talk_unk59_2(10);
    Talk_unk55_4(20, 258);
    Talk_unk56_4(20, 2);
    Talk_unk60_2(30);
    Talk_unk57_4(20, 0);
    Talk_unk61_2(10);
    Talk_unk185(21, 257, 40);
    Talk_unk58_4(21, 0);
    Talk_unk62_2(10);
    Talk_unk59_4(20, 3);
    Talk_unk63_2(30);
    Talk_unk64_2(10);
    Talk_unk186(22, 257, 40);
    Talk_unk60_4(22, 0);
    Talk_unk65_2(10);
    Talk_unk61_4(20, 4);
    Talk_unk66_2(20);
    Talk_unk62_4(20, 0);
    Talk_unk67_2(20);
    Talk_unk187(21, 22, 60);
    Talk_unk188(21, 20, 0);
    Talk_unk189(22, 20, 0);
    Talk_unk68_2(20);
    Talk_unk69_2(10);
    Talk_unk190(20, 261, 40);
    Talk_unk63_4(20, 0);
    Talk_unk70_2(10);
    Talk_unk64_4(21, 2);
    Talk_unk71_2(20);
    Talk_unk65_4(21, 0);
    Talk_unk72_2(10);
    Talk_unk66_4(22, 3);
    Talk_unk73_2(30);
    Talk_unk67_4(22, 0);
    Talk_unk74_2(10);
    Talk_unk68_4(20, 3);
    Talk_unk75_2(30);
    Talk_unk76_2(10);
    Talk_unk69_4(22, 2);
    Talk_unk77(20);
    Talk_unk70_4(22, 0);
    Talk_unk78(10);
    Talk_unk191(20, 258, 40);
    Talk_unk71_4(20, 0);
    Talk_unk79(10);
    Talk_unk72_4(21, 2);
    Talk_unk80(20);
    Talk_unk73_4(21, 0);
    Talk_unk81(10);
    Talk_unk74_4(20, 4);
    Talk_unk82(20);
    Talk_unk75_4(20, 0);
    Talk_unk83(10);
    Talk_unk192(22, 256, 40);
    Talk_unk76_4(22, 0);
    Talk_unk84(10);
    Talk_unk193(20, 258, 40);
    Talk_unk77_3(20, 0);
    Talk_unk85(10);
    Talk_unk194(21, 257, 40);
    Talk_unk78_3(21, 0);
    Talk_unk86(10);
    Talk_unk79_3(20, 4);
    Talk_unk87(20);
    Talk_unk80_3(20, 0);
    Talk_unk88(20);
    Talk_unk195(21, 22, 60);
    Talk_unk196(21, 20, 0);
    Talk_unk197(22, 20, 0);
    Talk_unk89(20);
    Talk_unk90(10);
    Talk_unk81_3(20, 2);
    Talk_unk91(20);
    Talk_unk82_3(20, 0);
    Talk_unk92(10);
    Talk_unk83_3(21, 258);
    Talk_unk84_3(22, 258);
    Talk_unk85_3(21, 2);
    Talk_unk86_3(22, 2);
    Talk_unk93(30);
    Talk_unk94(10);
    Talk_unk198(20, 0, 0);
    Talk_unk95(20);
    Talk_unk87_3(20, 0);
    Talk_unk96(10);
    Talk_unk88_3(21, 4);
    Talk_unk97(20);
    Talk_unk89_3(21, 0);
    Talk_unk98(10);
    Talk_unk90_3(22, 4);
    Talk_unk99(20);
    Talk_unk91_3(22, 0);
    Talk_unk100(10);
    Talk_unk92_3(20, 2);
    Talk_unk101(20);
    Talk_unk199(20, 32768, 0);
    Talk_unk102(20);
    Talk_unk93_3(20, 0);
    Talk_unk103(10);
    Talk_unk94_3(21, 3);
    Talk_unk104(30);
    Talk_unk95_3(21, 0);
    Talk_unk105(20);
    Talk_unk200(20, 261, 40);
    Talk_unk96_3(20, 0);
    Talk_unk106(10);
    Talk_unk97_3(22, 3);
    Talk_unk107(30);
    Talk_unk98_3(22, 0);
    Talk_unk108(10);
    Talk_unk99_3(20, 3);
    Talk_unk109(40);
    Talk_unk201(21, 22, 60);
    Talk_unk100_3(21, 3);
    Talk_unk110(20);
    Talk_unk101_3(22, 3);
    Talk_unk111(20);
    Talk_unk202(22, 16384, 0);
    Talk_unk112(40);
    Talk_unk203(21, 85196, 42598);
    Talk_unk204(22, 85196, 42598);
    Talk_unk205(21, 0, 120);
    Talk_unk206(22, 0, 120);
    Talk_unk207(21, 0, 0);
    Talk_unk208(22, 0, 0);
    Talk_unk113(10);
    Talk_unk102_3(20, 2);
    Talk_unk114(40);
    Talk_unk209(20, 52428, 26214);
    Talk_unk210(20, -16, 0);
    Talk_unk115(30);
    Talk_unk103_3(20, 0);
    Talk_unk116(10);
    Talk_unk104_3(20, 4);
    Talk_unk117(30);
    Talk_unk211(20, 0, 0);
    Talk_unk118(30);
    Talk_unk212(20, 65536, 32768);
    Talk_unk213(20, 120, 0);
    Talk_unk214(20, 60, 0);
    Talk_unk215(20, 0, 0);
    Talk_unk105_3();
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

    Talk_unk886();
    Talk_unk887((s32)&Value_00002280);
    v5 = 0;
    *(u8 *)(Talk_Apply7(0) + 84) = v5;
    *(u8 *)(Talk_unk63(10) + 84) = v5;
    Talk_unk888(1);
    *(volatile u16 *)0x04000000 = 0x1140;
    Talk_unk106_3(-1, 0);
    *(volatile u16 *)0x04000000 = 0x140;
    v5 = 1;
    *(u8 *)(Talk_unk64(0) + 84) = v5;
    *(u8 *)(Talk_unk65(10) + 84) = v5;
    Talk_unk889(0, 31);
    record = Talk_unk66(0);
    Talk_unk890(record, 0);
    Talk_unk216(1, 0x780000, 0x680000);
    Talk_unk217(3, 0x680000, 0x500000);
    Talk_unk218(2, 0x780000, 0x780000);
    Talk_unk891(1, 0, 0);
    Talk_unk892(3, 0, 0);
    Talk_unk219(2, 0xe000, 0);
    *(s32 *)(*(u8 **)gWork + 0x1c8) = 60;
    Talk_unk893();
    Talk_unk894();
    Talk_unk895(20);
    *(s32 *)(*(u8 **)gWork + 0x1c8) = 24;
    Talk_unk220(3, 0x10000, 0x8000);
    Talk_unk896(3, 16, 0);
    Talk_unk221(3, 0x2000, 0);
    Talk_unk897(20);
    Talk_unk898(3, 2);
    Talk_unk899(30);
    Talk_unk900(3, 0);
    Talk_unk901(10);
    record = Talk_unk67(0);
    *(s32 *)(record + 16) += -0x30000;
    record = Talk_unk9(0);
    *(s32 *)(record + 64) += -0x30000;
    Talk_unk902(0, 32);
    Talk_unk903(40);
    Talk_unk904(0, 34);
    Talk_unk905(30);
    Talk_unk906(0, 33);
    Talk_unk907(50);
    Talk_unk908(1, 2);
    Talk_unk909(30);
    Talk_unk910(1, 0);
    Talk_unk911(10);
    Talk_unk222(0, 0x105, 60);
    Talk_unk912(20);
    Talk_unk223(1, 0x102, 60);
    Talk_unk913(1, 0);
    Talk_unk914(10);
    Talk_unk915(1, 4);
    Talk_unk916(20);
    Talk_unk917(1, 0);
    Talk_unk918(10);
    Talk_unk224(0, 0x102, 80);
    Talk_unk225(2, 0x106, 60);
    Talk_unk226(2, 0xc000, 0);
    Talk_unk919(30);
    Talk_unk920(2, 4);
    Talk_unk921(20);
    Talk_unk922(2, 0);
    Talk_unk923(10);
    Talk_unk227(1, 0x4000, 0);
    Talk_unk924(30);
    Talk_unk925(1, 2);
    Talk_unk926(45);
    Talk_unk927(1, 0, 0);
    Talk_unk228(2, 0xe000, 0);
    Talk_unk928(30);
    Talk_unk10(1, 0);
    if (Talk_unk11(-1, 0) != 0) {
        Talk_unk929(10);
        Talk_unk930(0, 34);
        Talk_unk931(20);
        Talk_unk932(1, 3);
        Talk_unk933(20);
        Talk_unk934(1, 0);
        Talk_unk935(10);
        Talk_unk936(0, 33);
        Talk_unk937(30);
        Talk_unk938(1, 3);
        Talk_unk939(20);
        Talk_unk940(1, 0);
        bump_step(1);
    } else {
        Talk_unk941(10);
        Talk_unk942(0, 33);
        bump_step(2);
        Talk_unk943(30);
        Talk_unk944(1, 3);
        Talk_unk945(20);
        Talk_unk946(1, 0);
    }
    Talk_unk947(10);
    Talk_unk229(1, 0x10000, 0x8000);
    Talk_unk230(1, -16, 0);
    Talk_unk948(1, 0, 0);
    Talk_unk949(35);
    Talk_unk950(0, 6, 0);
    Talk_unk231(0, 0x1e666, 0xf333);
    Talk_unk232(0, -32, 0);
    record = Talk_unk68(0);
    Talk_unk951(record, 1);
    Talk_unk233(3, 0x4000, 0);
    Talk_unk234(2, 0xc000, 0);
    Talk_unk952(40);
    Talk_unk953(0, 3);
    Talk_unk954(30);
    Talk_unk955(2, 3);
    Talk_unk956(1, 3);
    Talk_unk957(3, 3);
    Talk_unk958(30);
    Talk_unk235(1, 0x13333, 0x9999);
    Talk_unk236(3, 0x13333, 0x9999);
    Talk_unk237(2, 0x13333, 0x9999);
    Talk_unk959(1, 2);
    record = Talk_unk12(0);
    if (record != 0) {
        Talk_unk960(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Talk_unk961(1);
    Talk_unk962(1, 0, 0);
    Talk_unk963(3, 2);
    record = Talk_unk13(0);
    if (record != 0) {
        Talk_unk964(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Talk_unk965(3);
    Talk_unk966(3, 0, 0);
    Talk_unk967(2, 2);
    record = Talk_unk14(0);
    if (record != 0) {
        Talk_unk968(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Talk_unk969(2);
    Talk_unk970(2, 0, 0);
    ((void (*)())Talk_unk69)(10);
    Talk_unk971();
}

void Scene_RunScene3b8SequenceA(void)
{
    s32 record;
    s16 dir;
    u16 facing;

    record = Talk_unk70(0);
    dir = (*(u16 *)(record + 6) + 0x2000) & -0x4000;
    Talk_unk972();
    Talk_unk973();
    Talk_unk974((s32)&Value_00001ffb);
    Talk_unk15(-1, 0);
    if (Talk_unk16(0, 0) != 0) {
    } else {
        if (Talk_unk17(0x96a) != 0) {
            Talk_unk975(20);
            Talk_unk976(0);
            goto L_02000fe2;
        }
        Talk_unk977(20);
        facing = dir;
        if (facing == 0) {
            Talk_unk978(0, 128, 120);
        }
        if (facing == 0x8000) {
            Talk_unk979(0, 240, 120);
        }
        Talk_unk980(0, 184, 120);
        Talk_unk981(0, 0, 0);
        Talk_unk982(10);
        Talk_unk983(1, 16, 0, 0x8000);
        Talk_unk984(1);
        Talk_unk985(10);
        Talk_unk986(1, 0);
        Talk_unk987(10);
        Talk_unk988(0, 3);
        Talk_unk989(20);
        Talk_unk238(1, 0x10000, 0x8000);
        Talk_unk990(1, 40, 0);
        Talk_unk239(1, 0, -32);
        Talk_unk240(1, 0x6000, 0);
        Talk_unk991(20);
        Talk_unk992(0, 0x8000, 0);
        Talk_unk993(20);
        Talk_unk107_3(2, -16, 0, 0);
        Talk_unk994(2);
        Talk_unk995(10);
        Talk_unk996(2, 0);
        Talk_unk997(10);
        Talk_unk998(0, 3);
        Talk_unk999(20);
        Talk_unk241(2, 0x10000, 0x8000);
        Talk_unk242(2, -40, 0);
        Talk_unk1000(2, 0, 40);
        Talk_unk243(2, 0xe000, 0);
        Talk_unk1001(20);
        Talk_unk1002(0, 0, 0);
        Talk_unk1003(20);
        Talk_unk1004(3, 16, 0, 0x8000);
        Talk_unk1005(3);
        Talk_unk1006(10);
        Talk_unk1007(3, 0);
        Talk_unk1008(10);
        Talk_unk1009(0, 3);
        Talk_unk1010(20);
        Talk_unk244(3, 0x10000, 0x8000);
        Talk_unk1011(3, 40, 0);
        Talk_unk1012(3, 0, 40);
        Talk_unk245(3, 0xa000, 0);
        Talk_unk1013(20);
        Talk_unk108_3(-1, -1, -1, 0);
        Talk_unk246(0, -56, 0);
        Talk_unk247(0, 0, -32);
        Talk_unk248(0, 0x2000, 0);
        Talk_unk1014(30);
        Talk_unk1015(3, 3);
        Talk_unk1016(10);
        Talk_unk1017(3, 0);
        Talk_unk1018(1, 3);
        ((void (*)())Talk_unk71)(10);
        if (Talk_unk18(0x96a) == 0) {
            Talk_unk1019(1, 0);
        } else {
            bump_step(1);
        }
        Talk_unk1020(2, 3);
        Talk_unk1021(10);
        if (Talk_unk19(0x96a) == 0) {
            Talk_unk1022(2, 0);
        } else {
            bump_step(1);
        }
        Talk_unk1023(20);
        Talk_unk1024(0, 0, 0);
        Talk_unk249(1, 0x8000, 0);
        Talk_unk250(3, 0x8000, 0);
        Talk_unk1025(2, 0, 0);
        Talk_unk1026(0);
        Talk_unk251(0, 0x2000, 0);
        Talk_unk252(1, 0x6000, 0);
        Talk_unk253(3, 0xa000, 0);
        Talk_unk254(2, 0xe000, 0);
        Talk_unk1027(20);
        Talk_unk1028(1, 3);
        Talk_unk1029(10);
        Talk_unk1030(1, 0);
        Talk_unk1031(2, 3);
        ((void (*)())Talk_unk72)(10);
        if (Talk_unk20(0x96a) == 0) {
            Talk_unk1032(2, 0);
        } else {
            bump_step(1);
        }
        Talk_unk1033(3, 3);
        Talk_unk1034(10);
        if (Talk_unk21(0x96a) == 0) {
            Talk_unk1035(3, 0);
        } else {
            bump_step(1);
        }
        Talk_unk1036(1, 0, 32);
        Talk_unk255(1, -112, 0);
        Talk_unk256(3, 0, -40);
        Talk_unk257(3, -112, 0);
        Talk_unk1037(50);
        Talk_unk258(2, 0, -24);
        Talk_unk1038(1);
        Talk_unk259(0, 0x4000, 0);
        Talk_unk260(1, 0, -16);
        Talk_unk1039(3);
        Talk_unk261(3, 0xc000, 0);
        Talk_unk1040(20);
        Talk_unk1041(1, 0);
        Talk_unk1042(10);
        Talk_unk1043(0, 3);
        Talk_unk1044(20);
        Talk_unk262(1, 0x13333, 0x9999);
        Talk_unk263(2, 0x13333, 0x9999);
        Talk_unk264(3, 0x13333, 0x9999);
        Talk_unk1045(1, 2);
        record = Talk_unk22(0);
        if (record != 0) {
            Talk_unk1046(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Talk_unk1047(1);
        Talk_unk1048(1, 0, 0);
        Talk_unk1049(3, 2);
        record = Talk_unk23(0);
        if (record != 0) {
            Talk_unk1050(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Talk_unk1051(3);
        Talk_unk1052(3, 0, 0);
        Talk_unk1053(2, 2);
        record = Talk_unk24(0);
        if (record != 0) {
            Talk_unk1054(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Talk_unk1055(2);
        Talk_unk1056(2, 0, 0);
        Talk_unk119(0x96a);
    }
    Talk_unk1057();
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

    Talk_unk1058();
    Talk_unk1059();
    if (Talk_unk25(0x966) == 0) {
        Talk_unk120(0x966);
        Talk_unk121(0x967);
        Talk_unk265(a0, 0x4000, 0);
        Talk_unk1060(0, 120, 96);
        Talk_unk266(0, 0xc000, 0);
        Talk_unk1061(20);
        base6_2241 = (s32)gVal;
        Talk_unk1062(base6_2241);
        Talk_unk26(a0, 0);
        if (Talk_unk27(0, 0) == 0) {
            Talk_unk1063(10);
            Talk_unk1064((base6_2241 + 1));
        } else {
            Talk_unk1065((base6_2241 + 2));
        }
        Talk_unk1066(a0, 0);
        Talk_unk1067(10);
        Talk_unk1068(a0, 3);
        Talk_unk1069(20);
        Talk_unk267(a0, 0x10000, 0x8000);
        Talk_unk268(a0, -64, 0);
        Talk_unk1070(a0, 0, 48);
    } else {
        Talk_unk122(0x2245);
        Talk_unk1071(a0, 0);
    }
    Talk_unk1072();
}

void Scene_RunScene3b8(s32 unused0, s32 a1)
{
    Talk_unk1073();
    Talk_unk123(0x2052);
    Talk_unk1074(a1, 0);
    if (Talk_unk28(0x968) == 0) {
        Talk_unk124(0x968);
        Talk_unk1075();
        Talk_unk1076(50);
        Talk_unk269(a1, 0x100, 70);
        Talk_unk1077(a1, 0, 40);
        Talk_unk1078(a1, 0);
        Talk_unk1079(30);
        Talk_unk1080(a1, 4);
        Talk_unk1081(20);
        Talk_unk1082(a1, 0);
        Talk_unk270(a1, 0x8000, 0);
    }
    Talk_unk1083();
}

void Dialogue_RunChoiceSequence22ab(s32 no)
{
    s32 msg = (s32)&Value_000022ab;

    Talk_unk131(msg);
    Talk_Apply8(no, 0);
    if (Talk_Apply9(0, 0) == 0)
        Talk_unk132(msg + 1);
    else
        Talk_unk133(msg + 2);
    Talk_Apply10(no, 0);
}

void Dialogue_RunChoiceSequence2352(void)
{
    s32 msg;

    Talk_unk1084();
    Talk_unk1085();
    msg = (s32)&Value_00002352;
    Talk_unk1086(msg);
    Talk_Apply11(-1, 0);
    Talk_unk134(10);
    Talk_unk1087(14, 2);
    Talk_unk135(30);
    Talk_unk275(0, 14, 30);
    Talk_Apply7(14, 0);
    if (Talk_Apply12(0, 0) != 0) {
        Talk_unk136(msg + 2);
        Talk_Apply13(14, 0);
    } else {
        Talk_unk137(20);
        Talk_unk138(msg + 3);
        Talk_Apply14(14, 0);
        Talk_unk139(10);
        Talk_Apply15(0, 3);
        Talk_unk140(30);
        Talk_unk276(0, 0x4000, 0);
        Talk_unk141(30);
        Talk_unk277(16, 0, 0);
        Talk_Apply16(205, 3);
        Talk_unk1088(0, 1);
        Talk_unk1089(205, 0);
        Talk_unk142((s32)&Value_00000f31);
    }
}

void Scene_RunScene3b8(void)
{
    u32 i;
    s32 record;
    u8 *p5;

    p5 = *(volatile s32 *)gWork;
    Talk_unk826();
    Talk_unk1090(158);
    Talk_unk271(0, 0x8000, 0x4000);
    Talk_unk1091(0, 2);
    if (*(s16 *)(((s32)p5 + 0x16c)) == 32) {
        Talk_unk1092(1);
        Talk_unk1093(10);
        Talk_unk272(0, 0, -16);
    } else {
        if (*(s16 *)(((s32)p5 + 0x16c)) == 30) {
            Talk_unk1094(4);
            Talk_unk1095(10);
            Talk_unk273(0, 3, -16);
        } else {
            Talk_unk1096(2);
            Talk_unk1097(10);
            Talk_unk274(0, 3, -16);
        }
    }
    Talk_unk1098(16);
    Talk_unk1099(*(s16 *)(((s32)p5 + 0x16c)));
    Talk_unk1100(1);
    Talk_unk1101(2);
    Talk_unk1102(4);
    Talk_unk1103();
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

    Talk_unk143(k);
    Talk_Apply17(a, 0);
    if (Talk_Apply18(0, 0) == 0)
        Talk_unk144(k + 1);
    else
        Talk_unk145(k + 2);
    Talk_Apply19(a, 0);
}

void Scene_RunBranchedSteps2006(s32 a)
{
    s32 k = (s32)&Value_00002006;

    Talk_unk146(k);
    Talk_Apply20(a, 0);
    if (Talk_Apply21(0, 0) == 0) {
        Talk_unk147(10);
        Talk_unk278(a, 0x102, 0x28);
        Talk_unk148(k + 1);
    } else {
        Talk_unk149(10);
        Talk_unk279(a, (s32)&Value_00000105, 0x28);
        Talk_unk150(k + 2);
    }
    Talk_Apply22(a, 0);
}

void Scene_RunStepWithValue29e0(void)
{
    Talk_unk1104();
    Talk_Apply23(0x947, 1);
    Talk_Apply24(0x29e0, 1);
    Talk_unk1105();
}

s32 SceneData_SelectTableD004ByStateAndFlags(void)
{
    if (gCell[224] == (s32)&Value_0000008b) {
        if (Talk_unk73(0x950) != 0) {
            return (s32)gOv10;
        }
        if (Talk_unk74(0x962) != 0) {
            return (s32)gOv11;
        }
        return (s32)gOv12;
    }
    if (Talk_unk75(0x950) != 0) {
        return (s32)gOv13;
    }
    if (Talk_unk76(0x962) != 0) {
        return (s32)gOv14;
    }
    return (s32)gOv15;
}
