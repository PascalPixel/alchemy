#include "types.h"
#include "scene.h"

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define ACTOR_FLAGS_OFFSET 89
#define ACTOR_FLAGS_OFFSET 90
#define ACTOR_FLAGS_OFFSET2 90
#define ACTOR_DONE_OFFSET 100

struct Object {
    u8 filler00[24];
    s32 x;
    s32 z;
    u8 filler20[68];
    s16 counter;
    s16 mode;
};

/*
 * Prepare the scene service selected by index zero, set its halfword at +6,
 * and run the four follow-up services in the order present in the overlay.
 *
 * This owner starts at the saved-link prologue at 0x02001990 and returns at
 * 0x020019b8.  The next saved-link prologue is at 0x020019bc, so the two zero
 * bytes between them are alignment and are deliberately outside this source.
 */
struct SceneService {
    u16 unknown00[3];
    u16 value06;
};

typedef struct {
    u8 filler0[6];
    u16 unk6;
    u8 filler8[82];
    u8 unk5A;
    u8 filler5B[9];
    u16 unk64;
} T;

/* Layout of the record returned for an actor: only the word at offset
 * 0x64 (100) is touched here. */
struct SceneRecord {
    u8 pad[100];
    u16 field_0x64;
};

typedef struct {
    u8 filler0[8];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8 filler14[80];
    u16 unk64;
    s16 unk66;
} T;

/*
 * resource_399 owner at 0x02001704, 72 bytes.
 *
 * Resets one actor: clears the state byte at +85 and the halfword at +100,
 * drops bit 0 of the flag byte at +35, sets the two-bit mode field of the
 * linked record to 1, and re-centres the two 20.12 offsets on half a unit.
 *
 * The mode write is a bitfield assignment, not mask arithmetic. Spelling it
 * `(f9 & ~12) | 4` lets the compiler narrow the mask to a byte and emit
 * `movs r3,#243`; the reference builds -13 as `movs r3,#13 / negs r3,r3`,
 * which is what the bitfield path produces.
 */
struct Rec_399 {
    u8 pad00[9];
    u8 lo : 2;
    u8 mode : 2;                /* +9, bits 2..3 */
    u8 hi : 4;
};

struct Work_399 {
    u8 pad00[24];
    s32 f24;                    /* +24 */
    s32 f28;                    /* +28 */
    u8 pad20[3];
    u8 f35;                     /* +35 */
    u8 pad24[44];
    struct Rec_399 *f80;        /* +80 */
    u8 pad54[1];
    u8 f85;                     /* +85 */
    u8 pad56[14];
    u16 f100;                   /* +100 */
};

typedef struct {
    u8 filler0[8];
    s32 unk8;
    s32 unkC;
    s32 unk10;
} T2;

typedef struct {
    u8 filler0[23];
    u8 unk17;
} State;

struct SharedData {
    u8 pad_000[500];
    s32 selected_subject;
};

struct Subject {
    u8 pad_000[6];
    u16 heading;
    s32 x;
    s32 y;
    s32 z;
    u8 pad_020[14];
    u8 id;
    u8 pad_035[13];
    s32 state_048;
    s32 state_052;
    u8 pad_056[34];
    u8 flags_090;
    u8 pad_091[9];
    u16 state_100;
    u8 pad_102[6];
    void *callback;
};

extern s16 gOv[];
extern u8 Value_00000033;
extern u8 gOv2[];
extern u8 gOv3[];
extern u8 gOv4[];
extern u8 gOv5[];
extern u8 gOv6[];
extern u8 gOv7[];
extern u8 gOv8[];
extern u8 Value;
extern u8 gWork[];
extern u8 gOv9[];
extern u8 gOv10[];
extern s32 gOv11[];
extern s32 gOv12[];
extern s16 gOv13[];
extern u32 gIw;

struct SceneService *Talk_unk12_4(s32 index);

void *Motion_SetAngleToward_1();
void *Scene_GetRecord_1();
void *Scene_GetRecord_2();

void gOv14();
void gOv15();
void gOv16();

T *Talk_unk13_4(s32);

struct Subject *Talk_unk14_4();

/* The three returned addresses are even, so they name in-image script blocks
 * rather than Thumb entry points. */

/* Talk_unk20 and Talk_unk21 are the same flag query reached from two
 * mutually exclusive arms; each names its own loader-relocated call word. */

/* Reviewed owner resource_399:0x02000f90, 1468 bytes.
 *
 * A long primary-script scene body for overlay resource_399. It arranges
 * actor records 0, 1, 2, 3, 8, 9 and 10 through position, animation, camera
 * and timing calls, waits twice on a per-record halfword at offset 100,
 * plays three sound cues, toggles bit 0 of a record flag byte at offset 90
 * for actors 0, 1 and 2, and stamps the shared scene work record at
 * 0x03001ebc (phase word 0x1c0, related word 0x1c8) at the start and again
 * at the end before setting one game flag.
 *
 * Loader-relocated overlay calls: every Func_02xxxxxx symbol below names the
 * pre-relocation call word the stored image holds at its site, not a function
 * body in this address range. The loader rewrites those BL pairs to the
 * overlay's own veneer island (0x0200a1xx-0x0200a3xx at runtime), which in
 * turn reaches the main image. Because the stored word is site-relative, one
 * runtime veneer is named by several such symbols and one symbol can name two
 * different veneers.
 *
 * NOTE FOR THE INTEGRATOR: eleven of the reference's site names are
 * genuinely ambiguous over this owner - ten cover two veneers each and
 * Talk_unk15_4 covers three - so at 23 sites this draft spells the call
 * with another site's unambiguous name for the same veneer. That keeps the
 * emitted target correct and lets the owner score without a translation-unit
 * binding. On adoption, prefer this overlay's established convention
 * (Func_<site>_a / Func_<site>_b plus explicit absolute_symbols) as in
 * run_scene_399_three_actor_choreography.c. Three veneers have no
 * unambiguous site name at all (runtime 0x0200a218, 0x0200a370, 0x0200a378)
 * and are spelled here as Data_<runtime address> so they resolve by address.
 *
 * The complete substitution list, as reference site -> spelling used here:
 *   Talk_unk16_4  0x02000fa0 -> Talk_unk17_4   0x02001000 -> Talk_unk18_4
 *   Talk_unk19_4  0x02001140 -> Talk_unk20_4   0x02001198 -> Talk_unk21_4
 *   Talk_unk22_4  0x0200115e -> Talk_unk20_4   0x0200131e -> Talk_unk23_4
 *   Talk_unk24_4  0x020012d8 -> Talk_unk25_4   0x02001350 -> Talk_unk26_4
 *   Talk_unk27_4  0x020013ac -> Talk_unk28_4   0x020013cc -> Talk_unk22
 *   Talk_unk15_4  0x020012e8 -> gOv15   0x020012f0 -> Talk_unk29_4
 *                  0x02001338 -> Talk_unk30_4
 *   Talk_unk31_4  0x020012f6 -> gOv16   0x02001366 -> Talk_unk25_4
 *   Talk_unk32_4  0x02001318 -> gOv16   0x02001440 -> Talk_unk18_4
 *   Talk_unk33_4  0x02001494 -> Talk_unk34_4   0x020014c4 -> Talk_unk22
 *   Talk_unk35_4  0x0200140a -> Talk_unk30_4   0x02001502 -> Talk_unk36_4
 *   Talk_unk37_4  0x02001416 -> Talk_unk30_4   0x0200151e -> gOv14
 *
 * Veneer roles. Every row below is the role name the overlay's exact-C
 * sibling resource_399:0x020019e8 already gives the same main-image entry
 * through an unambiguous site symbol, matched by main address:
 *   0x0200a218  GameFlag_Set                              (0x080770c8)
 *   0x0200a228  Battle_WaitMode0              (0x0808a010)
 *   0x0200a230  Battle_Reset                       (0x0808a018)
 *   0x0200a238  Battle_SchedShoulder
 *   0x0200a248  Scene_GetRecord                           (0x0808a080)
 *   0x0200a250  Motion_SetSpeed           (0x0808a090)
 *   0x0200a288  Motion_ResetPosMode2   (0x0808a0c8)
 *   0x0200a290  Motion_SetPosReset          (0x0808a0d0)
 *   0x0200a2b0  Motion_SetHPosTerrain
 *   0x0200a2b8  Object_SetModeById                        (0x0808a100)
 *   0x0200a2c0  Motion_CallWaitAnim
 *   0x0200a2d8  Motion_SetVarCbObj
 *   0x0200a300  SceneWork_SetStepValue                    (0x0808a170)
 *   0x0200a310  Battle_RunThenWaitIfModeZero       (0x0808a188)
 *   0x0200a320  Motion_ArmCb                  (0x0808a1b8)
 *   0x0200a330  BattleFx_SpawnLinked    (0x0808a1e8)
 *   0x0200a338  Battle_WaitMode0              (0x0808a1f0)
 *   0x0200a340  Motion_SetSpeedLim (0x0808a208)
 *   0x0200a348  Motion_CamBounds      (0x0808a210)
 *   0x0200a380  Battle_WaitMode0              (0x0808a360)
 * One further role is evidenced outside that sibling: 0x0200a3e8 reaches
 * main 0x080f9010, which battle/event_runtime/dispatch_queued_events.c calls
 * with a pending sound id, so its three uses here are sound cues.
 * The remaining veneers (0x0200a170, 0x0200a1e0, 0x0200a258, 0x0200a260,
 * 0x0200a268, 0x0200a270, 0x0200a278, 0x0200a2f8, 0x0200a308, 0x0200a350,
 * 0x0200a368, 0x0200a370, 0x0200a378) have no repository name yet and keep
 * their raw spellings.
 *
 * Talk_unk38_4 is the one intra-overlay direct call; it reaches
 * State_StoreTable96adToWork at 0x020016c8.
 *
 * STATUS: drafted, not adopted. 1468 candidate bytes against 1468 reference
 * bytes, 36 differing halfwords, 4 wrong instructions, topology equal,
 * residual class allocation-uncovered. All 142 reference call sites, both
 * `record != 0` guards, both wait loops, every literal constant and every
 * store are represented; the extent is exact and no code was dropped.
 *
 * Residual, now one coupled defect. The literal table address held in r5
 * across each of the three table groups (0x0200a74c, 0x0200a5ec, 0x0200a760)
 * is scheduled one to two calls earlier here than in the reference, which
 * issues each `ldr r5, [pc, ...]` at the group's first use. Because the third
 * hoisted load makes r5 live immediately after the last `|= 1` flag update,
 * that update also computes into r3 here where the reference clobbers the
 * now-dead constant register r5. Fixing the schedule would fix both; nothing
 * structural is missing.
 *
 * Measured source hypotheses, so the next reader does not repeat them:
 *  - Binding the wait flag's address before materialising the zero
 *    (`done = (u16 *)(... + 100); val = 0; *done = val;`) keeps the zero's
 *    live range off the call and restores the reference's scratch register.
 *    This removed the two `strh` mismatches: 40 -> 36 differing halfwords,
 *    12 -> 4 wrong instructions.
 *  - Storing the zero directly (`*(u16 *)(... + 100) = 0;`) makes GCC 2.96
 *    materialise the HImode constant from the literal pool, adding two pool
 *    words: 1476 bytes, 416 differing halfwords. The `val` temporary is
 *    load-bearing, not decoration.
 *  - Block-scoping the three table locals: neutral.
 *  - Splitting the shared table local into three: neutral.
 *  - Inlining the table constants at their call sites: regressed to 255.
 *  - Spelling the first table call directly rather than through a far call:
 *    regressed to 637; the Call/Value wrappers are load-bearing.
 * The triage router reports "do not probe register roles by respelling
 * source", so the remaining schedule residual is reported, not chased. */

/* Veneers with no unambiguous reference site name over this owner. */

/* Byte offset of the flag byte on an actor record; bit 0 is cleared for
 * actors 0, 1 and 2 and set again after they are repositioned. */

/* Halfword at offset 100 of an actor record: the scene waits until it
 * becomes non-zero. */

/* Scripted dialogue step for resource_399.  The imports name loader-relocated
 * call words and stay old-style because their arity varies between sites. */

/* Typed wrappers around the two angle calls.  Their parameters are what force
 * the 0x3000 argument to be built from an immediate and a shift at each call. */

/* Byte offset of a flags field on the actor 8 record touched below; bit 1
 * of that byte is toggled at the end of the third branch. */

/* Byte offset of a flags field on the actor records touched below; bit 0
 * of that byte is cleared and later set back for actors 20 (twice). */

static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)gWork;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* gOv13 is this overlay's heading table.  gCell is a resident
 * table; its word at byte offset 500 selects the subject. */

/* The per-frame callback installed on the subject, named by its address. */

/* Imports named by their relocated call words.  Old-style declarations are
 * mandatory: one of these imports is reached with two arguments here while its
 * other sites pass three or four. */
static __inline__ void AdvanceProbe(s32 heading, s32 *probe)
{
    extern struct SharedData gCell;

    /* Keep this call behind an inline boundary: the third argument's address is
     * then rematerialized before the split 0x100000 constant is completed. */
    Talk_unk39_4((s32)0x100000, heading, probe);
}

s32 OvObj_UpdateWobbleByCounter(struct Object *obj)
{
    switch (obj->counter) {
    case 6:
        obj->x += 0xffffc000;
        obj->z += 0x2000;
        break;
    case 4:
        obj->x += 0x2000;
        obj->z -= 0x1000;
        break;
    case 2:
        obj->x += 0x1000;
        obj->z += 0xfffff800;
        break;
    case 0:
        obj->x += 0x1000;
        obj->z += 0xfffff800;
        if (obj->mode != 0) {
            obj->counter = Talk_Apply(Talk_unk23(), 40) + 40;
        } else {
            obj->counter = Talk_Apply2(Talk_unk24(), 20) + 20;
        }
        break;
    }
    obj->counter--;
    return 1;
}

s32 OvObj_UpdateFacingTowardTarget(void *obj)
{
    s32 delta;
    u16 old;
    s32 angle;
    void *target;
    target = FIELD(obj, void *, 0x68);
    if (target != NULL) {
        FIELD(obj, u8, 0x5A) = (u8)(0xFE & FIELD(obj, u8, 0x5A));
        angle = (u16)Talk_Apply3(FIELD(target, s32, 0x10) - FIELD(obj, s32, 0x10), FIELD(target, s32, 8) - FIELD(obj, s32, 8));
        old = FIELD(obj, u16, 6);
        delta = (s16)(angle - old);
        if (delta != 0) {
            if (delta > 0x1000) delta = 0x1000;
            if (delta < -0x1000) delta = -0x1000;
            FIELD(obj, u16, 6) = (u16)(old + delta);
        }
    }
    return 1;
}

/*
 * Scene-script selection for resource_399: read the scenario id from the
 * shared table, branch on it and on story flag 0x881, and return the chosen
 * in-image script block.
 */
s32 SceneData_SelectTableByWord224(void)
{
    extern s16 gCell[];

    if (gCell[224] == (s32)&Value_00000033) {
        return (s32)gOv2;
    }
    return (s32)gOv3;
}

/* The whole four-byte owner. */
s32 SceneData_ReturnZero(void)
{
    extern s16 gCell[];

    return 0;
}

/* The eight-byte owner includes its one pool word. */
u8 *SceneData_GetTableA990(void)
{
    extern s16 gCell[];

    return (u8 *)0x0200a990;
}

/*
 * The overlay image is writable, so the chosen block is patched in place. The
 * coordinates are written as shifts, which is how a 16.16 whole number is
 * built here. The word at +0x4c is set only on this path and never read back,
 * so its meaning is unverified.
 */
u8 *SceneData_SelectScriptByScene33AndFlag881(void)
{
    extern s16 gCell[];

    u8 *script;

    if (gCell[224] == ((s32)&Value_00000033)) {
        script = gOv6;
        Talk_unk40_4(script);
        if (Talk_unk20(0x881) != 0) {
            script[262] = 0;
            *(s32 *)(script + 0x50) = 182 << 16;
            *(s32 *)(script + 0x58) = 564 << 16;
            *(s32 *)(script + 0x4c) = 2;
        }
        return script;
    }

    if (Talk_unk21(0x881) != 0) {
        return gOv5;
    }
    return gOv4;
}

void Scene_Forward(void)
{
    Talk_unk41_4();
}

s32 SceneData_SelectTableByWord224B(void)
{
    extern s16 gCell[];

    if (gCell[224] == (s32)&Value_00000033) {
        return (s32)gOv7;
    }
    return (s32)gOv8;
}

void Dialogue_RunActorEightFlagGatedDialogue(void)
{
    extern s32 gOv17[];

    Talk_unk42_4();
    if (Talk_unk25(3) != 0) {
        Talk_unk11_2(0x1570);
    } else {
        Talk_unk12_2(0x1529);
    }
    {
        s32 val = 0;
        s32 mode = 8;
        Talk_Apply4(mode, val);
    }
    Talk_unk43_4();
}

void Dialogue_ShowLine1571Or152F(void)
{
    Talk_unk44_4();
    if (Talk_unk26(3) != 0) {
        Talk_unk13_2(0x1571);
    } else {
        Talk_unk14_2(0x152f);
    }
    Talk_Apply5(8, 0);
    Talk_unk45_4();
}

void Dialogue_RunActor9Line(void)
{
    Talk_unk27();
    Talk_unk32_3(9, 0, 10);
    Talk_unk28(0x152a);
    Talk_Apply6(9, 0);
    Talk_unk29();
}

/*
 * One scripted section, bracketed by an open and a close call, in which story
 * flag 0x881 picks between two arms on channel 10.  The arms differ only in the
 * message id and one step call, and stay separate so that every call is written
 * once.  258 is a pose id, 0x3000 three sixteenths of a turn.  Talk_unk30's
 * unused s32 return is what fixes that call's argument order.
 */
void Dialogue_RunActorTenFlag881Dialogue(void)
{
    Talk_unk46_4();

    if (Talk_unk31(0x881) != 0) {
        Talk_unk47_4(0x163c);
        Talk_unk48_4(10, 0);
        Talk_unk49_4(10, 258);
        Talk_unk50_4(40);
        Talk_unk51_4(10, 1);
        Talk_unk52_4(20);
        Talk_unk53_4(10, 0, 20);
        Talk_unk30(10, 0);
        Talk_Arm(10, 0x3000, 10);
        Talk_unk54_4(10, 9);
    } else {
        Talk_unk55_4(0x152d);
        Talk_unk56_4(10, 0);
        Talk_unk57_4(10, 258);
        Talk_unk58_4(40);
        Talk_unk59_4(10, 1);
        Talk_unk60_4(20);
        Talk_unk61_4(10, 0, 20);
        Talk_unk62_4(10, 0);
        Talk_Arm2(10, 0x3000, 10);
        Talk_unk63_4(10, 9);
    }

    Talk_unk64_4();
}

/* Picks one of three scripted call sequences depending on two condition
 * checks (codes 2177 and 2091), each acting on actor 9 and/or actor 8. */
void Scene_RunSupplementalSequenceOne(void)
{
    void *Scene_GetRecord_2();

    void *actor9_record;
    void *unused_actor9_record;
    void *actor8_record;

    if (GameFlag_IsSet_1(2177) != 0) {
        Battle_Reset_1();
        unused_actor9_record = Motion_SetAngleToward_1(9, 0, 0);
        Battle_WaitMode0_1(10);
        SceneWork_SetStepValue_1(5700);
        BattleEventRuntime_ProcessAction_1(9, 0);
        Battle_SchedShoulder_1();
    } else {
        if (GameFlag_IsSet_2(2091) != 0) {
            Battle_Reset_2();
            Object_SetModeById_1(9, 7);
            Talk_Place(33599936, 10, 69);
            SceneWork_SetStepValue_2(5484);
            BattleEv_RunWait_1(9, 0);
            Object_SetModeById_2(9, 8);
            Talk_unk2_3((s32)&Value, 10, 69);
            Battle_SchedShoulder_2();
        } else {
            Battle_Reset_3();
            actor9_record = Scene_GetRecord_1(9);
            ((struct SceneRecord *)actor9_record)->field_0x64 = 10;
            Motion_EnableActCb_1(9, 33596660);
            SceneWork_SetStepValue_3(5428);
            BattleEv_RunWait_2(9, 0);
            Motion_EnableReset_1(8);
            BattleFx_SpawnLinked_1(8, 256, 40);
            Motion_ArmCb_1(8, 53248, 10);
            Motion_SetVarCb_1(8, 2);
            Battle_RunThenWaitIfModeZero_1(8, 0, 20);
            Motion_EnableActCb_2(0, 33596772);
            Motion_SetSpeed_1(8, 104857, 52428);
            Motion_MarkActiveAndSetActionCallback_1(8, 33596680);
            Battle_WaitMode0_2(40);
            Motion_Launch_1(8, 2, 0);
            Motion_SetVarCb_2(8, 2);
            Battle_WaitMode0_3(8, 258);
            Battle_WaitMode0_4(60);
            Battle_RunThenWaitIfModeZero_2(8, 0, 10);
            Motion_ArmCb_2(8, 12288, 20);
            Motion_SetVarCb_3(8, 2);
            BattleEv_RunWait_3(8, 0);
            actor8_record = Scene_GetRecord_2(8);
            *(u8 *)((u8 *)(actor8_record) + ACTOR_FLAGS_OFFSET) ^= 0x2;
            GameFlag_Set_1(0x82c);
            Battle_SchedShoulder_3();
        }
    }
}

void Dialogue_RunActor12Line(void)
{
    Talk_unk32();
    Talk_unk33(0x153f);
    Talk_unk34(12, 0);
    Talk_unk35();
}

void Dialogue_RunActor18Line(void)
{
    Talk_unk36();
    Talk_unk37(0x154d);
    Talk_unk38(18, 0);
    Talk_unk39();
}

void Dialogue_RunActor20BranchScene(void)
{
    Talk_unk40();
    if (Talk_unk41(3) != 0) {
        Talk_unk42(0x1574);
        Talk_unk43(20, 0);
    } else {
        Talk_unk44(0x1557);
        Talk_unk45(20, 0);
        Talk_unk46(0x82a);
        Talk_unk47(0x82c);
    }
    Talk_unk48();
}

void Dialogue_RunActor20FlaggedLine(void)
{
    Talk_unk65_4();
    if (Talk_unk49(3) != 0) {
        Talk_unk15_2(0x1575);
    } else {
        Talk_unk16_2(0x155B);
    }
    Talk_Apply7(20, 0);
    Talk_unk66_4();
}

void Scene_RunScene399(void)
{
    u32 i;
    s32 record;

    Talk_unk67_4();
    Talk_Do(0x156d);
    Talk_unk68_4(8, 0);
    Talk_unk3_3(8, 0x3000, 10);
    Talk_unk69_4();
}

void Dialogue_RunActorEightBranchedDialogue(void)
{
    Talk_unk70_4();
    if (Talk_unk50(0x82b) != 0) {
        Talk_unk71_4(0x156f);
    } else if (Talk_unk51(0x82c) != 0) {
        Talk_unk72_4(0x153b);
    } else {
        Talk_unk73_4(0x1533);
    }
    Talk_unk74_4(8, 0);
    Talk_unk75_4();
}

void Scene_RunSingleStep(void)
{
    Talk_unk52();
}

void Dialogue_ShowLine156E(void)
{
    Talk_unk76_4();
    Talk_unk77_3(0x156e);
    Talk_unk78_3(10, 0);
    Talk_unk79_3();
}

void Dialogue_ShowLine1573Or155A(void)
{
    Talk_unk80_3();
    if (Talk_unk53(3) != 0) {
        Talk_unk81_3(0x1573);
    } else {
        Talk_unk82_3(0x155a);
    }
    Talk_unk83_3(19, 0);
    Talk_unk84_3();
}

void Scene_RunScene399(void)
{
    u32 i;
    s32 record;

    record = Talk_unk54(0);
    if ((u32)((*(volatile u16 *)(record + 6) + 0x5fff) << 16) <= 0x3ffe0000) {
        Talk_unk85_3(4, 16);
    } else {
        Talk_unk86_3();
        Talk_unk87_3(16, 0, 10);
        if (Talk_Check(0x881) != 0) {
            Talk_unk2_2(0x1653);
            Talk_unk88_3(16, 0);
        } else {
            Talk_unk3_2(0x154b);
            Talk_unk89_3(16, 0);
        }
        Talk_unk4_3(16, 0x3000, 10);
        Talk_unk90_3();
    }
}

void Scene_RunScene399(void)
{
    s32 record;

    record = Talk_unk55(0);
    if ((u32)((*(volatile u16 *)(record + 6) + 0x5fff) << 16) <= 0x3ffe0000) {
        Talk_unk91_3();
        if (Talk_unk2(0x82d) == 0) {
            Talk_unk4_2(0x1553);
            Talk_unk92_3(19, 0);
            Talk_unk5_2(0x82d);
        }
        Talk_unk93_3();
        Talk_unk94_3(19);
    } else {
        Talk_unk95_3();
        if (Talk_unk3(0x881) != 0) {
            Talk_unk6_2(0x1671);
            Talk_unk96_3(19, 0);
        } else {
            record = Talk_unk4(3);
            if (record != 0) {
                Talk_unk6_2(0x1572);
                Talk_unk96_3(19, 0);
            } else {
                Talk_unk7_2(0x1554);
                (void)Talk_unk5(19, 0);
                Talk_unk5_3(19, 0x3000, 10);
            }
        }
        Talk_unk97_3();
    }
}

void Scene_RunPrimaryScriptChoreography(void)
{
    extern u8 gOv17[];

    s32 record;
    s32 tbl;
    s32 val;
    u16 *done;
    u8 *work;

    Talk_unk98_3();
    Talk_unk6_3(3, 0xb60000, 0x960000);
    Talk_Run(0x8d0000, -1, 0xdd0000, 0);
    Talk_unk23_4(1);
    Talk_unk2_4(0x4ccc, 0x999);
    Talk_unk3_4(0x8c0000, -1, 0xa40000, 1);
    /* Enter the scene phase. */
    work = *(u8 *volatile *)gWork;
    *(volatile s32 *)(work + 0x1c0) = 0x100;
    *(volatile s32 *)(work + 0x1c8) = 40;
    Talk_unk99_3();
    Talk_unk7_3(0, 0x6666, 0x3333);
    Talk_unk7_3(1, 0x6666, 0x3333);
    Talk_unk8_3(2, 0x6666, 0x3333);
    Talk_unk26_4(0, 142, 221);
    Talk_unk9_3(0, 0xd000, 0);
    record = Talk_unk6(0);
    if (record != 0) {
        Talk_unk17_4(1, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    record = Talk_unk7(0);
    if (record != 0) {
        Talk_unk100_3(2, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Talk_unk101_3(1, 150, 234);
    Talk_unk102_3(2, 134, 234);
    Talk_unk103_3(1, 1);
    tbl = (s32)gOv9;
    Talk_unk10_3(0, 0x10003, tbl);
    Talk_unk11_3(1, 0x10003, tbl);
    Talk_unk12_3(2, 0x10003, tbl);
    Talk_unk104_3();
    tbl = (s32)gOv17;
    Talk_unk105_3(9, tbl);
    Talk_unk36_4(40);
    Talk_unk4_4(3, 0x102);
    Talk_unk106_3(40);
    Talk_unk21_4(3, 1);
    Talk_unk8_2(0x155c);
    Talk_unk107_3(3, 0, 20);
    Talk_unk8(9, tbl);
    Talk_unk108_3(9, 0, 20);
    Talk_unk13_3(3, 0x8000, 20);
    Talk_unk109_3(8, 0, 10);
    Talk_unk110_3(8, 4);
    Talk_unk111_3(8, 0, 40);
    Talk_unk112_3(3, 3);
    Talk_unk113_3(10);
    Talk_unk14_3(3, 0x4000, 0);
    Talk_unk15_3(8, 0x3000, 20);
    Talk_unk114_3(3, 0, 10);
    Talk_unk9(9, tbl);
    /* State_StoreTable96adToWork at 0x020016c8. */
    Talk_unk38_4();
    Talk_unk16_3(3, 0x101, 60);
    Talk_unk115_3(3, 0, 40);
    Talk_unk116_3(9, 0, 20);
    Talk_unk16_3(8, 0x105, 60);
    Talk_unk117_3(9, 7);
    Talk_unk17_3(0x200b1c0, 10, 69);
    Talk_unk118_3(10);
    Talk_unk119_3(3, 2);
    Talk_unk120_3(3, 4);
    Talk_unk121_3(3, 0, 20);
    Talk_unk21_4(9, 1);
    Talk_unk122_3(40);
    Talk_unk123_3(9, 8);
    Talk_unk18_3(0x200b1d6, 10, 69);
    Talk_unk124_3(40);
    Talk_unk125_3(3, 3);
    Talk_unk126_3(20);
    Talk_unk127_3(8, 0, 20);
    Talk_unk128_3(8, 3);
    Talk_unk129_3(8, 0, 10);
    Talk_unk16_3(3, 0x101, 30);
    Talk_unk19_3(3, 0x8000, 10);
    Talk_unk130_3(3, 4);
    Talk_unk131_3(3, 0, 10);
    Talk_unk132_3(8, 3);
    Talk_unk133_3(20);
    Talk_unk134_3(3, 3);
    Talk_unk135_3(40);
    Talk_unk20_3(3, 0x10000, 0x8000);
    done = (u16 *)(Talk_unk56(3) + ACTOR_DONE_OFFSET);
    val = 0;
    *done = val;
    Talk_unk5_4(3, 0x200a670);
    while (*(s16 *)(Talk_unk57(3) + ACTOR_DONE_OFFSET) == 0) {
        Talk_unk136_3(1);
    }
    Talk_unk6_4(0x8c0000, -1, 0xc60000, 1);
    Talk_unk137_3(3);
    Talk_unk21_3(3, 0x101, 80);
    Talk_unk138_3(3, 0, 40);
    Talk_unk139_3(3, 1);
    Talk_unk140_3(10);
    Talk_unk25_4(3, 0);
    Talk_unk141_3(131);
    Talk_unk7_4(0x10000, 0);
    Talk_unk8_4(0x207e9f, 0);
    gOv16(10);
    Talk_unk142_3(1);
    Talk_unk143_3(220);
    Talk_unk144_3(40);
    Talk_unk8_4(0x10000, 0);
    gOv16(60);
    Talk_unk23_4(60);
    Talk_unk9_4(3, 0x102);
    Talk_unk145_3(20);
    Talk_unk30_4(3, 0, 10);
    Talk_unk22_3(3, 0x20000, 0x10000);
    Talk_unk26_4(3, 202, 198);
    Talk_unk146_3(40);
    Talk_unk147_3(3, 2);
    Talk_unk25_4(3, 0);
    Talk_unk148_3(3, 4);
    Talk_unk149_3(3, 0, 20);
    Talk_unk10_4(3, 0x102);
    Talk_unk150_3(40);
    Talk_unk151_2(3, 0, 40);
    Talk_unk23_3(3, 0x100, 40);
    Talk_unk25_4(3, 0);
    Talk_unk9_2(0);
    Talk_unk28_4(1);
    Talk_unk152_2(2);
    Talk_unk24_3(3, 0x30000, 0x18000);
    done = (u16 *)(Talk_unk22(3) + ACTOR_DONE_OFFSET);
    val = 0;
    *done = val;
    Talk_unk11_4(3, 0x200a6e0);
    while (*(s16 *)(Talk_unk58(3) + ACTOR_DONE_OFFSET) == 0) {
        Talk_unk153_2(1);
    }
    Talk_unk25_3(0, 0x4000, 0);
    Talk_unk9_3(1, 0x4000, 0);
    Talk_unk9_3(2, 0x4000, 10);
    Talk_unk26_3(0, 0x40000, 0x20000);
    Talk_unk27_3(1, 0x40000, 0x20000);
    Talk_unk7_3(2, 0x40000, 0x20000);
    Talk_unk154_2(152);
    *(u8 *)(Talk_unk59(0) + ACTOR_FLAGS_OFFSET2) &= 254;
    *(u8 *)(Talk_unk60(1) + ACTOR_FLAGS_OFFSET2) &= 254;
    *(u8 *)(Talk_unk61(2) + ACTOR_FLAGS_OFFSET2) &= 254;
    Talk_unk34_4(0, 132, 206);
    Talk_unk155_2(1, 136, 221);
    Talk_unk28_3(2, 122, 238);
    Talk_unk156_2(3);
    Talk_unk157_2(80);
    *(u8 *)(Talk_unk62(0) + ACTOR_FLAGS_OFFSET2) |= 1;
    *(u8 *)(Talk_unk63(1) + ACTOR_FLAGS_OFFSET2) |= 1;
    {
        u8 *flags = (u8 *)(Talk_unk22(2) + ACTOR_FLAGS_OFFSET2);
        u8 raised = (u8)(*flags | 1);

        *flags = raised;
    }
    Talk_unk29_3(0, 0xcccc, 0x6666);
    Talk_unk30_3(1, 0xcccc, 0x6666);
    Talk_unk31_3(2, 0xcccc, 0x6666);
    tbl = (s32)gOv10;
    Talk_unk158_2(1, tbl);
    Talk_unk159_2(2, tbl);
    Talk_unk36_4(20);
    /* Advance the scene phase word to its next value. */
    work = *(u8 *volatile *)gWork;
    *(volatile s32 *)(work + 0x1c0) = 0x209;
    *(volatile s32 *)(work + 0x1c8) = 24;
    Talk_unk10_2(0x82b);
    Talk_unk160_2();
}

void Actor_UpdateCountdownArcPosition(T *o)
{
    s32 buf[3];
    s32 *b;
    s32 n;
    s32 t;

    if (o != 0) {
        n = o->unk64 - 1;
        o->unk64 = n;
        t = (s16)n;
        if (t != 0) {
            b = buf;
            b[0] = gOv11[0];
            b[1] = gOv11[1] + 0x80000;
            b[2] = gOv11[2];
            Talk_unk33_3(t << 16, (t << 11) + o->unk66, b);
            o->unk8 = b[0];
            o->unkC = b[1];
            o->unk10 = b[2];
        } else {
            Talk_unk17_2(o);
        }
    }
}

void ActorDraw_ApplyTableA5ecToActorNine(void)
{
    extern s32 gOv17[];

    Talk_Apply8(9, (s32)gOv17);
    Talk_Apply9(9, 0);
}

void State_StoreTable96adToWork(void)
{
    u8 *work;

    Talk_Apply10(93, 1);
    work = *(u8 **)0x03001f30;
    Talk_Apply11(3, 9);
    *(s32 *)(work + 36) = (s32)gOv12;
    Talk_unk161_2();
    Talk_unk18_2(1);
    Talk_unk162_2();
    Talk_unk163_2();
}

void Actor_ResetActorAndCenterOffsets(struct Work_399 *work)
{
    struct Rec_399 *rec;

    work->f85 = 0;
    work->f100 = 0;
    work->f35 &= ~1;

    rec = work->f80;
    rec->mode = 1;

    Talk_unk164_2(work, 9);
    Talk_unk165_2(work, 0);

    work->f24 = 0x8000;
    work->f28 = 0x8000;
}

void Scene_RunSupplementalSequenceTwo(s32 a0)
{
    s32 remaining;

    *(volatile s32 *)(a0 + 8) += (*(s16 *)(a0 + 100) << 8);
    *(volatile s32 *)(a0 + 12) += 0x8000;
    *(volatile s32 *)(a0 + 24) += 0x7ae;
    *(volatile s32 *)(a0 + 28) += 0x7ae;
    *(volatile u16 *)(a0 + 100) += 2;
    remaining = *(volatile s32 *)(a0 + 104) - 1;
    *(volatile s32 *)(a0 + 104) = remaining;
    if (remaining == 0) {
        Talk_unk166_2();
    }
}

void Scene_RunScene399SequenceA(void)
{
    u32 i;
    s32 rec7;
    s32 record;
    s32 base6_3001e40;

    base6_3001e40 = 0x3001e40;
    if (Talk_unk10(*(volatile s32 *)base6_3001e40, 60) == 0) {
        rec7 = Talk_unk11(222, 0x1cf0000, 0, 0x1240000);
        if (rec7 != 0) {
            Talk_unk167_2();
            *(volatile s32 *)(rec7 + 104) = 60;
            *(volatile s32 *)(rec7 + 108) = 0x200974d;
            Talk_unk168_2(rec7, 5);
        }
    }
    if (Talk_unk12((*(volatile s32 *)base6_3001e40 + 30), 60) == 0) {
        rec7 = Talk_unk13(222, 0x1400000, 0x200000, 0x1640000);
        if (rec7 != 0) {
            Talk_unk169_2();
            *(volatile s32 *)(rec7 + 104) = 60;
            *(volatile s32 *)(rec7 + 108) = 0x200974d;
            Talk_unk170_2(rec7, 5);
        }
    }
    if (Talk_unk14((*(volatile s32 *)base6_3001e40 + 10), 60) == 0) {
        rec7 = Talk_unk15(222, 0x760000, 0, 0x460000);
        if (rec7 != 0) {
            Talk_unk171_2();
            *(volatile s32 *)(rec7 + 104) = 60;
            *(volatile s32 *)(rec7 + 108) = 0x200974d;
            Talk_unk172_2(rec7, 5);
        }
    }
    if (Talk_unk16((*(volatile s32 *)base6_3001e40 + 50), 60) == 0) {
        rec7 = Talk_unk17(222, 0x1560000, 0, 0x7c0000);
        if (rec7 != 0) {
            Talk_unk173_2();
            *(volatile s32 *)(rec7 + 104) = 60;
            *(volatile s32 *)(rec7 + 108) = 0x200974d;
            Talk_unk174_2(rec7, 5);
        }
    }
    if (Talk_unk18((*(volatile s32 *)base6_3001e40 + 80), 60) == 0) {
        rec7 = Talk_unk19(222, 0x1af0000, 0, 0xab0000);
        if (rec7 != 0) {
            Talk_unk175_2();
            *(volatile s32 *)(rec7 + 104) = 60;
            *(volatile s32 *)(rec7 + 108) = 0x200974d;
            Talk_unk176_2(rec7, 5);
        }
    }
}

void State_UpdateZoneFlagsFromActorZero(void)
{
    T2 *obj;
    s32 x;
    s32 cx;
    s32 y;
    s32 r;
    s32 g;
    s32 h;
    State *st;

    obj = Talk_unk13_4(0);
    x = obj->unk8;
    cx = x >> 19;
    g = 0x200;
    h = 0x201;
    if ((u32)(cx - 24) > 7) {
        y = obj->unk10;
        if ((u32)((y >> 19) - 36) > 9 || (u32)(cx - 22) > 9)
            goto rest;
    }
    r = Talk_unk64(g);
    if (r != 0)
        return;
    (*(State **)0x03001e70)->unk17 = r;
    Talk_unk19_2(g);
    Talk_unk20_2(h);
    return;

rest:
    if (x > 0xE80000 && obj->unkC > 0x1E0000 && y > 0xD40000) {
        st = *(State **)0x03001e70;
        st->unk17 = 0;
        Talk_unk19_2(g);
        Talk_unk20_2(h);
        return;
    }
    r = Talk_unk65(h);
    if (r != 0)
        return;
    st = *(State **)0x03001e70;
    st->unk17 = 1;
    Talk_unk21_2(h);
    Talk_unk22_2(g);
    return;
}

void State_UpdateActor11WithFlag203(void)
{
    s32 a;
    s32 b;

    Talk_unk23_2(0x203);
    Talk_Apply12(11, 3);
    a = 15;
    b = 7;
    Talk_SetRect(15, 6, 1, 1, a, b);
}

void Actor_SetActorZeroFacingC000AndRun(void)
{
    struct SceneService *work;

    Talk_unk177_2();
    work = Talk_unk12_4(0);
    work->value06 = 0xc000;
    Talk_unk24_2(123);
    Talk_unk178_2();
    Talk_unk179_2();
    Talk_unk25_2(8);
}

/* Runs a short fixed sequence of two calls, one 3-argument call passing a
 * fixed-point-looking pair of constants, one 3-argument call passing
 * (0, 232, 204), and a final call, in that order. */
void RunEventScript02(void)
{
    u32 i;
    u8 *record;

    BattleRuntime_Reset_1();
    BattleRuntime_WaitIfModeZero_1(); /* main:0808a360 */
    ObjectMotion_SetSpeedParameters_1(0, 0x20000, 0x1999);
    Motion_SetPositionAndCommit_1(0, 232, 204);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}

/* Runs a long scripted sequence of position, animation, and timing calls
 * against actor records 0, 3, 19, and 20, with a scene phase word at
 * offset 0x1c0 of the shared scene work record set at the start and near
 * the end. */
void Scene_RunThreeActorChoreography(void)
{
    s32 Scene_GetRecord_2();

    u32 i;
    s32 record;
    u8 *work;

    BattleRuntime_Reset_1();
    ObjectMotion_ArmCallback_1(3, 0xa000, 0);
    ObjectMotion_SetSpeedParameters_1(0, 0x9999, 0x4ccc);
    Motion_ResetPosMode2_1(0, 0x2b2, 200);
    /* Clear the byte at offset 85 of the returned record. */
    *(u8 *)(RuntimeBlock_GetOffset1e0Pointer_1() + 85) = 0;
    Motion_SetSpeedLim_1(0xcccc, 0x1999);
    Motion_CamBounds_1(0x2b20000, 0, 0xa40000, 1);
    /* Set the scene phase word (offset 0x1c0) and a related word at
     * offset 0x1c8 of the shared scene work record. */
    work = *(u8 *volatile *)gWork;
    *(volatile s32 *)((work + 0x1c0)) = 0x100;
    *(volatile s32 *)((work + 0x1c8)) = 48;
    BattleRuntime_WaitIfModeZero_1();
    Motion_CommitPos_1(0);
    Object_SetModeById_1(0, 1);
    Motion_SetSpeed_2(3, 0x9999, 0x4ccc);
    record = Scene_GetRecord_1(0);
    if (record != 0) {
        Motion_SetHPosTerrain_1(3, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Motion_SetPosReset_1(3, 0x2a1, 183);
    ObjectMotion_ArmCallback_2(3, 0xc000, 0);
    ObjectMotion_SetVariantCallback_1(19, 2);
    Motion_SetVarCbObj_1(20, 2);
    BattleRuntime_WaitIfModeZero_2(40);
    SceneWork_SetStepValue_1(0x165b);
    BattleRuntime_RunThenWaitIfModeZero_1(19, 0, 10);
    Motion_ArmCb_3(3, 0xe000, 40);
    Motion_CallWaitAnim_1(3, 3);
    BattleRuntime_WaitIfModeZero_3(20, 0x102);
    BattleRuntime_WaitIfModeZero_4(20);
    BattleRuntime_RunThenWaitIfModeZero_2(0x4014, 0, 10);
    Motion_ArmCb_4(3, 0xa000, 40);
    Motion_CallWaitAnim_2(3, 4);
    Battle_RunThenWaitIfModeZero_3(0x2003, 0, 10);
    Motion_CallWaitAnim_3(20, 3);
    Battle_WaitMode0_5(20);
    Battle_WaitMode0_6(19, 0x102);
    Battle_WaitMode0_7(20);
    Battle_RunThenWaitIfModeZero_4(19, 0, 10);
    Motion_ArmCb_5(0, 0xa000, 0);
    Motion_ArmCb_6(3, 0xf000, 10);
    Motion_ArmCb_7(3, 0x2000, 40);
    Motion_ArmCb_8(0, 0xc000, 0);
    Motion_ArmCb_9(3, 0xc000, 40);
    Motion_SetVarCbObj_2(20, 2);
    Battle_WaitMode0_8(20);
    Battle_RunThenWaitIfModeZero_5(0x4014, 0, 20);
    Motion_ArmCb_10(3, 0xa000, 20);
    Motion_CallWaitAnim_4(3, 3);
    Battle_WaitMode0_9(60);
    BattleEffect_SpawnLinkedResourceObject_1(3, 0x105, 60);
    BattleFx_SpawnLinked_2(19, 0x101, 0);
    BattleFx_SpawnLinked_3(20, 0x101, 60);
    Motion_SetVarCbObj_3(19, 1);
    Battle_WaitMode0_10(20);
    Battle_RunThenWaitIfModeZero_6(19, 0, 10);
    Motion_ArmCb_11(3, 0xe000, 40);
    Motion_ArmCb_12(3, 0xa000, 40);
    Motion_ArmCb_13(3, 0xe000, 20);
    Motion_ArmCb_14(3, 0x6000, 80);
    Battle_RunThenWaitIfModeZero_7(0x2003, 0, 20);
    Motion_ArmCb_15(20, 0xf000, 0);
    Motion_ArmCb_16(19, 0x7000, 40);
    Motion_ArmCb_17(19, 0x5000, 0);
    Motion_ArmCb_18(20, 0x3000, 20);
    Motion_SetSpeed_3(20, 0x10000, 0x8000);
    *(u8 *)(Scene_GetRecord_2(20) + ACTOR_FLAGS_OFFSET) &= 254;
    Motion_SetPosReset_2(20, 0x290, 166);
    Battle_WaitMode0_11(1);
    *(u8 *)(Scene_GetRecord_3(20) + ACTOR_FLAGS_OFFSET) |= 1;
    Battle_WaitMode0_12(20);
    Battle_RunThenWaitIfModeZero_8(0x4014, 0, 10);
    Motion_SetVarCbObj_4(3, 2);
    Battle_WaitMode0_13(40);
    Motion_ArmCb_19(3, 0xa000, 10);
    Battle_RunThenWaitIfModeZero_9(0x2003, 0, 40);
    Motion_ArmCb_20(3, 0x2000, 20);
    Battle_RunThenWaitIfModeZero_10(0x4003, 0, 10);
    Battle_WaitMode0_14(19, 0x102);
    Battle_WaitMode0_15(20, 0x102);
    Battle_WaitMode0_16(40);
    Motion_ArmCb_21(3, 0xc000, 20);
    Object_SetModeById_2(3, 4);
    Battle_RunThenWaitIfModeZero_11(0x2003, 0, 20);
    Motion_SetVarCbObj_5(19, 1);
    Battle_RunThenWaitIfModeZero_12(19, 0, 10);
    Motion_ArmCb_22(3, 0x2000, 40);
    Motion_ArmCb_23(3, 0xc000, 20);
    Motion_CallWaitAnim_5(3, 3);
    Battle_WaitMode0_17(20);
    Motion_SetVarCbObj_6(20, 1);
    Battle_RunThenWaitIfModeZero_13(0x4014, 0, 20);
    Motion_SetVarCbObj_7(3, 1);
    Battle_WaitMode0_18(20);
    Motion_ArmCb_24(3, 0xa000, 20);
    Motion_CallWaitAnim_6(3, 3);
    Battle_RunThenWaitIfModeZero_14(0x2003, 0, 80);
    BattleFx_SpawnLinked_4(19, 0x105, 0);
    BattleFx_SpawnLinked_5(20, 0x105, 60);
    Motion_CallWaitAnim_7(19, 4);
    Battle_RunThenWaitIfModeZero_15(19, 0, 10);
    Object_SetModeById_3(20, 4);
    Battle_RunThenWaitIfModeZero_16(0x4014, 0, 20);
    BattleFx_SpawnLinked_6(3, 0x102, 60);
    Battle_RunThenWaitIfModeZero_17(0x2003, 0, 40);
    Motion_CallWaitAnim_8(19, 3);
    Battle_RunThenWaitIfModeZero_18(19, 0, 10);
    Motion_ArmCb_25(3, 0xe000, 20);
    Motion_CallWaitAnim_9(20, 3);
    Battle_RunThenWaitIfModeZero_19(0x4014, 0, 10);
    Motion_ArmCb_26(3, 0xa000, 60);
    Motion_ArmCb_27(3, 0xe000, 20);
    Motion_ArmCb_28(3, 0xa000, 20);
    Motion_ArmCb_29(3, 0xc000, 40);
    Motion_CallWaitAnim_10(3, 3);
    Battle_RunThenWaitIfModeZero_20(0x2003, 0, 10);
    Object_SetModeById_4(19, 3);
    Motion_CallWaitAnim_11(20, 3);
    Battle_WaitMode0_19(40);
    Motion_ArmCb_30(3, 0x2000, 20);
    Battle_RunThenWaitIfModeZero_21(0x4003, 0, 20);
    Motion_ArmCb_31(0, 0xa000, 20);
    Motion_CallWaitAnim_12(0, 3);
    Battle_WaitMode0_20(20);
    Motion_SetPosReset_3(3, 0x2b0, 200);
    Motion_SetHPosTerrain_2(3, 0, 0);
    *(u8 *)(Scene_GetRecord_4(20) + ACTOR_FLAGS_OFFSET) &= 254;
    Motion_SetPosReset_4(20, 0x284, 166);
    Battle_WaitMode0_21(1);
    *(u8 *)(Scene_GetRecord_5(20) + ACTOR_FLAGS_OFFSET) |= 1;
    /* Advance the scene phase word to its next value. */
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x209;
    GameFlag_Set_1(0x82e);
    GameFlag_Clear_1(0x82d);
    BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
}

void Actor_TurnTowardTableAngle(s32 z)
{
    T *o;
    s32 t;
    s32 d;
    u16 prev;
    s32 n;

    o = (T *)z;
    n = o->unk64;
    z = 0;
    t = ((s16 *)&o->unk64)[z];
    if (t != 0) {
        o->unk64 = n - 1;
        return;
    }
    o->unk5A = t;
    z = 1;
    d = gOv[(*(u32 *)0x03001ae8 >> 4) & 0xF];
    z = -z;
    if (d == z) {
        Talk_Apply13(o, 9);
        return;
    }
    prev = o->unk6;
    d = (s16)(d - prev);
    if (d > 0x1000)
        d = 0x1000;
    if (d < -0x1000)
        d = -0x1000;
    o->unk6 = prev + d;
    Talk_Apply14(o, 2);
    Talk_Apply15(o, 0x30);
}

/*
 * The overlay's pathing step for resource_399.  The six pool words after the
 * return belong to this owner.
 *
 * Frame map: the goal marker, then the heading, then the three-word probe
 * position that the stepping imports are handed by address.  The order of the x
 * and z assignments, together with the inline stepping wrapper, is what leaves
 * a single high-register copy before the arithmetic that follows it.
 */
void StagedActor_RunHeadingProbeStep(void)
{
    extern struct SharedData gCell;

    struct Subject *subject;
    s32 probe[3];
    s32 heading;
    s32 goal;
    s32 marker;
    s32 z;
    s32 x;
    u8 *p;

    subject = Talk_unk14_4(gCell.selected_subject);

    for (;;) {
        heading = gOv13[(gIw >> 4) & 15];
        /* The test is on heading << 16 against 0xffff0000, that is on the
         * signed halfword -1, which means "no heading". */
        if ((heading << 16) == (s32)0xffff0000) {
            return;
        }
        /* Nothing is placed in an argument register before this call. */
        Talk_unk180_2();

        /* The 0x80000 bias is built from an immediate and a shift. */
        probe[0] = (subject->x & (s32)0xfff00000) + 0x80000;
        probe[1] = subject->y;
        probe[2] = (subject->z & (s32)0xfff00000) + 0x80000;
        z = probe[2];
        x = probe[0];
        p = (u8 *)subject;
        p += 34;
        goal = Talk_unk66((s32)*p, x, z);
        /* 0x100000 is built from an immediate and a shift.  The probe block is
         * passed by address and is advanced by the callee. */
        Talk_unk181_2((s32)0x100000, heading, probe);

        marker = Talk_unk67((s32)*p, probe[0], probe[2]);
        if (marker == 255
                || Talk_unk68((s32)*p, probe[0], probe[2])
                    - subject->y > 0x80000) {
            subject->heading = (u16)heading;
            goto tail;
        }

        /* Rewind the probe to the position it held before the step above. */
        probe[0] = x;
        probe[2] = z;
        subject->state_048 = 0x20000;
        subject->state_052 = 0x1999;
        subject->state_100 = 0;
        Talk_unk182_2(subject, x, subject->y, z);
        /* Same import as the probe above, two arguments here. */
        Talk_unk183_2(subject, 2);
        Talk_unk184_2(subject, 48);
        Talk_unk185_2(subject);
        subject->callback = (void *)Talk_unk186_2;

        goto advanceProbe;
continueProbe:
        if (Talk_unk69((s32)*p, probe[0], probe[2])
                - subject->y > 0x80000) {
            goto finishProbe;
        }
        x = probe[0];
        z = probe[2];
        subject->state_048 = 0x20000;
        subject->state_052 = 0x1999;
        Talk_unk187_2(subject, probe[0], probe[1], probe[2]);
        Talk_unk188_2(subject);
        if (marker != goal) {
            goto blocked;
        }

advanceProbe:
        AdvanceProbe(heading, probe);
        marker = Talk_unk70((s32)*p, probe[0], probe[2]);
        if (marker != 255) {
            goto continueProbe;
        }

finishProbe:
        subject->state_048 = 0x20000;
        subject->state_052 = 0x10000;
        Talk_unk189_2(subject, x, subject->y, z);
        Talk_unk190_2(subject);
        Talk_unk191_2(2);
        /* The back edge re-reads the heading table and starts again. */
    }

blocked:
    subject->callback = NULL;
    subject->flags_090 |= 1;
    /* 0x4000 is built from an immediate and a shift. */
    subject->state_052 = 0x4000;

tail:
    Talk_unk192_2(10);
    Talk_unk193_2();
}
