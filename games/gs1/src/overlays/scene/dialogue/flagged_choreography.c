#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/dialogue/flagged_choreography.h"

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

struct SceneService *Talk_Run12(s32 index);

void *Motion_SetAngleToward_1();
void *Scene_GetRecord_1();
void *Scene_GetRecord_2();

void gOv14();
void gOv15();
void gOv16();

T *Talk_Run13(s32);

struct Subject *Talk_Run14();

/* The three returned addresses are even, so they name in-image script blocks
 * rather than Thumb entry points. */

/* Talk_Check20 and Talk_Check21 are the same flag query reached from two
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
 * Talk_Run15 covers three - so at 23 sites this draft spells the call
 * with another site's unambiguous name for the same veneer. That keeps the
 * emitted target correct and lets the owner score without a translation-unit
 * binding. On adoption, prefer this overlay's established convention
 * (Func_<site>_a / Func_<site>_b plus explicit absolute_symbols) as in
 * run_scene_399_three_actor_choreography.c. Three veneers have no
 * unambiguous site name at all (runtime 0x0200a218, 0x0200a370, 0x0200a378)
 * and are spelled here as Data_<runtime address> so they resolve by address.
 *
 * The complete substitution list, as reference site -> spelling used here:
 *   Talk_Run16  0x02000fa0 -> Talk_Run17   0x02001000 -> Talk_Run18
 *   Talk_Run19  0x02001140 -> Talk_Run20   0x02001198 -> Talk_Run21
 *   Talk_Run22  0x0200115e -> Talk_Run20   0x0200131e -> Talk_Run23
 *   Talk_Run24  0x020012d8 -> Talk_Run25   0x02001350 -> Talk_Run26
 *   Talk_Run27  0x020013ac -> Talk_Run28   0x020013cc -> Talk_Check22
 *   Talk_Run15  0x020012e8 -> gOv15   0x020012f0 -> Talk_Run29
 *                  0x02001338 -> Talk_Run30
 *   Talk_Run31  0x020012f6 -> gOv16   0x02001366 -> Talk_Run25
 *   Talk_Run32  0x02001318 -> gOv16   0x02001440 -> Talk_Run18
 *   Talk_Run33  0x02001494 -> Talk_Run34   0x020014c4 -> Talk_Check22
 *   Talk_Run35  0x0200140a -> Talk_Run30   0x02001502 -> Talk_Run36
 *   Talk_Run37  0x02001416 -> Talk_Run30   0x0200151e -> gOv14
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
 * Talk_Run38 is the one intra-overlay direct call; it reaches
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
    Talk_Run39((s32)0x100000, heading, probe);
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
            obj->counter = Talk_Apply(Talk_Check23(), 40) + 40;
        } else {
            obj->counter = Talk_Apply2(Talk_Check24(), 20) + 20;
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
        Talk_Run40(script);
        if (Talk_Check20(0x881) != 0) {
            script[262] = 0;
            *(s32 *)(script + 0x50) = 182 << 16;
            *(s32 *)(script + 0x58) = 564 << 16;
            *(s32 *)(script + 0x4c) = 2;
        }
        return script;
    }

    if (Talk_Check21(0x881) != 0) {
        return gOv5;
    }
    return gOv4;
}

void Scene_Forward(void)
{
    Talk_Run41();
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

    Talk_Run42();
    if (Talk_Check25(3) != 0) {
        Talk_Do11(0x1570);
    } else {
        Talk_Do12(0x1529);
    }
    {
        s32 val = 0;
        s32 mode = 8;
        Talk_Apply4(mode, val);
    }
    Talk_Run43();
}

void Dialogue_ShowLine1571Or152F(void)
{
    Talk_Run44();
    if (Talk_Check26(3) != 0) {
        Talk_Do13(0x1571);
    } else {
        Talk_Do14(0x152f);
    }
    Talk_Apply5(8, 0);
    Talk_Run45();
}

void Dialogue_RunActor9Line(void)
{
    Talk_Check27();
    Talk_Place32(9, 0, 10);
    Talk_Check28(0x152a);
    Talk_Apply6(9, 0);
    Talk_Check29();
}

/*
 * One scripted section, bracketed by an open and a close call, in which story
 * flag 0x881 picks between two arms on channel 10.  The arms differ only in the
 * message id and one step call, and stay separate so that every call is written
 * once.  258 is a pose id, 0x3000 three sixteenths of a turn.  Talk_Check30's
 * unused s32 return is what fixes that call's argument order.
 */
void Dialogue_RunActorTenFlag881Dialogue(void)
{
    Talk_Run46();

    if (Talk_Check31(0x881) != 0) {
        Talk_Run47(0x163c);
        Talk_Run48(10, 0);
        Talk_Run49(10, 258);
        Talk_Run50(40);
        Talk_Run51(10, 1);
        Talk_Run52(20);
        Talk_Run53(10, 0, 20);
        Talk_Check30(10, 0);
        Talk_Arm(10, 0x3000, 10);
        Talk_Run54(10, 9);
    } else {
        Talk_Run55(0x152d);
        Talk_Run56(10, 0);
        Talk_Run57(10, 258);
        Talk_Run58(40);
        Talk_Run59(10, 1);
        Talk_Run60(20);
        Talk_Run61(10, 0, 20);
        Talk_Run62(10, 0);
        Talk_Arm2(10, 0x3000, 10);
        Talk_Run63(10, 9);
    }

    Talk_Run64();
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
            Talk_Place2((s32)&Value, 10, 69);
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
    Talk_Check32();
    Talk_Check33(0x153f);
    Talk_Check34(12, 0);
    Talk_Check35();
}

void Dialogue_RunActor18Line(void)
{
    Talk_Check36();
    Talk_Check37(0x154d);
    Talk_Check38(18, 0);
    Talk_Check39();
}

void Dialogue_RunActor20BranchScene(void)
{
    Talk_Check40();
    if (Talk_Check41(3) != 0) {
        Talk_Check42(0x1574);
        Talk_Check43(20, 0);
    } else {
        Talk_Check44(0x1557);
        Talk_Check45(20, 0);
        Talk_Check46(0x82a);
        Talk_Check47(0x82c);
    }
    Talk_Check48();
}

void Dialogue_RunActor20FlaggedLine(void)
{
    Talk_Run65();
    if (Talk_Check49(3) != 0) {
        Talk_Do15(0x1575);
    } else {
        Talk_Do16(0x155B);
    }
    Talk_Apply7(20, 0);
    Talk_Run66();
}

void Scene_RunScene399(void)
{
    u32 i;
    s32 record;

    Talk_Run67();
    Talk_Do(0x156d);
    Talk_Run68(8, 0);
    Talk_Place3(8, 0x3000, 10);
    Talk_Run69();
}

void Dialogue_RunActorEightBranchedDialogue(void)
{
    Talk_Run70();
    if (Talk_Check50(0x82b) != 0) {
        Talk_Run71(0x156f);
    } else if (Talk_Check51(0x82c) != 0) {
        Talk_Run72(0x153b);
    } else {
        Talk_Run73(0x1533);
    }
    Talk_Run74(8, 0);
    Talk_Run75();
}

void Scene_RunSingleStep(void)
{
    Talk_Check52();
}

void Dialogue_ShowLine156E(void)
{
    Talk_Run76();
    Talk_Run77(0x156e);
    Talk_Run78(10, 0);
    Talk_Run79();
}

void Dialogue_ShowLine1573Or155A(void)
{
    Talk_Run80();
    if (Talk_Check53(3) != 0) {
        Talk_Run81(0x1573);
    } else {
        Talk_Run82(0x155a);
    }
    Talk_Run83(19, 0);
    Talk_Run84();
}

void Scene_RunScene399(void)
{
    u32 i;
    s32 record;

    record = Talk_Check54(0);
    if ((u32)((*(volatile u16 *)(record + 6) + 0x5fff) << 16) <= 0x3ffe0000) {
        Talk_Run85(4, 16);
    } else {
        Talk_Run86();
        Talk_Run87(16, 0, 10);
        if (Talk_Check(0x881) != 0) {
            Talk_Do2(0x1653);
            Talk_Run88(16, 0);
        } else {
            Talk_Do3(0x154b);
            Talk_Run89(16, 0);
        }
        Talk_Place4(16, 0x3000, 10);
        Talk_Run90();
    }
}

void Scene_RunScene399(void)
{
    s32 record;

    record = Talk_Check55(0);
    if ((u32)((*(volatile u16 *)(record + 6) + 0x5fff) << 16) <= 0x3ffe0000) {
        Talk_Run91();
        if (Talk_Check2(0x82d) == 0) {
            Talk_Do4(0x1553);
            Talk_Run92(19, 0);
            Talk_Do5(0x82d);
        }
        Talk_Run93();
        Talk_Run94(19);
    } else {
        Talk_Run95();
        if (Talk_Check3(0x881) != 0) {
            Talk_Do6(0x1671);
            Talk_Run96(19, 0);
        } else {
            record = Talk_Check4(3);
            if (record != 0) {
                Talk_Do6(0x1572);
                Talk_Run96(19, 0);
            } else {
                Talk_Do7(0x1554);
                (void)Talk_Check5(19, 0);
                Talk_Place5(19, 0x3000, 10);
            }
        }
        Talk_Run97();
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

    Talk_Run98();
    Talk_Place6(3, 0xb60000, 0x960000);
    Talk_Run(0x8d0000, -1, 0xdd0000, 0);
    Talk_Run23(1);
    Talk_Run2(0x4ccc, 0x999);
    Talk_Run3(0x8c0000, -1, 0xa40000, 1);
    /* Enter the scene phase. */
    work = *(u8 *volatile *)gWork;
    *(volatile s32 *)(work + 0x1c0) = 0x100;
    *(volatile s32 *)(work + 0x1c8) = 40;
    Talk_Run99();
    Talk_Place7(0, 0x6666, 0x3333);
    Talk_Place7(1, 0x6666, 0x3333);
    Talk_Place8(2, 0x6666, 0x3333);
    Talk_Run26(0, 142, 221);
    Talk_Place9(0, 0xd000, 0);
    record = Talk_Check6(0);
    if (record != 0) {
        Talk_Run17(1, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    record = Talk_Check7(0);
    if (record != 0) {
        Talk_Run100(2, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Talk_Run101(1, 150, 234);
    Talk_Run102(2, 134, 234);
    Talk_Run103(1, 1);
    tbl = (s32)gOv9;
    Talk_Place10(0, 0x10003, tbl);
    Talk_Place11(1, 0x10003, tbl);
    Talk_Place12(2, 0x10003, tbl);
    Talk_Run104();
    tbl = (s32)gOv17;
    Talk_Run105(9, tbl);
    Talk_Run36(40);
    Talk_Run4(3, 0x102);
    Talk_Run106(40);
    Talk_Run21(3, 1);
    Talk_Do8(0x155c);
    Talk_Run107(3, 0, 20);
    Talk_Check8(9, tbl);
    Talk_Run108(9, 0, 20);
    Talk_Place13(3, 0x8000, 20);
    Talk_Run109(8, 0, 10);
    Talk_Run110(8, 4);
    Talk_Run111(8, 0, 40);
    Talk_Run112(3, 3);
    Talk_Run113(10);
    Talk_Place14(3, 0x4000, 0);
    Talk_Place15(8, 0x3000, 20);
    Talk_Run114(3, 0, 10);
    Talk_Check9(9, tbl);
    /* State_StoreTable96adToWork at 0x020016c8. */
    Talk_Run38();
    Talk_Place16(3, 0x101, 60);
    Talk_Run115(3, 0, 40);
    Talk_Run116(9, 0, 20);
    Talk_Place16(8, 0x105, 60);
    Talk_Run117(9, 7);
    Talk_Place17(0x200b1c0, 10, 69);
    Talk_Run118(10);
    Talk_Run119(3, 2);
    Talk_Run120(3, 4);
    Talk_Run121(3, 0, 20);
    Talk_Run21(9, 1);
    Talk_Run122(40);
    Talk_Run123(9, 8);
    Talk_Place18(0x200b1d6, 10, 69);
    Talk_Run124(40);
    Talk_Run125(3, 3);
    Talk_Run126(20);
    Talk_Run127(8, 0, 20);
    Talk_Run128(8, 3);
    Talk_Run129(8, 0, 10);
    Talk_Place16(3, 0x101, 30);
    Talk_Place19(3, 0x8000, 10);
    Talk_Run130(3, 4);
    Talk_Run131(3, 0, 10);
    Talk_Run132(8, 3);
    Talk_Run133(20);
    Talk_Run134(3, 3);
    Talk_Run135(40);
    Talk_Place20(3, 0x10000, 0x8000);
    done = (u16 *)(Talk_Check56(3) + ACTOR_DONE_OFFSET);
    val = 0;
    *done = val;
    Talk_Run5(3, 0x200a670);
    while (*(s16 *)(Talk_Check57(3) + ACTOR_DONE_OFFSET) == 0) {
        Talk_Run136(1);
    }
    Talk_Run6(0x8c0000, -1, 0xc60000, 1);
    Talk_Run137(3);
    Talk_Place21(3, 0x101, 80);
    Talk_Run138(3, 0, 40);
    Talk_Run139(3, 1);
    Talk_Run140(10);
    Talk_Run25(3, 0);
    Talk_Run141(131);
    Talk_Run7(0x10000, 0);
    Talk_Run8(0x207e9f, 0);
    gOv16(10);
    Talk_Run142(1);
    Talk_Run143(220);
    Talk_Run144(40);
    Talk_Run8(0x10000, 0);
    gOv16(60);
    Talk_Run23(60);
    Talk_Run9(3, 0x102);
    Talk_Run145(20);
    Talk_Run30(3, 0, 10);
    Talk_Place22(3, 0x20000, 0x10000);
    Talk_Run26(3, 202, 198);
    Talk_Run146(40);
    Talk_Run147(3, 2);
    Talk_Run25(3, 0);
    Talk_Run148(3, 4);
    Talk_Run149(3, 0, 20);
    Talk_Run10(3, 0x102);
    Talk_Run150(40);
    Talk_Run151(3, 0, 40);
    Talk_Place23(3, 0x100, 40);
    Talk_Run25(3, 0);
    Talk_Do9(0);
    Talk_Run28(1);
    Talk_Run152(2);
    Talk_Place24(3, 0x30000, 0x18000);
    done = (u16 *)(Talk_Check22(3) + ACTOR_DONE_OFFSET);
    val = 0;
    *done = val;
    Talk_Run11(3, 0x200a6e0);
    while (*(s16 *)(Talk_Check58(3) + ACTOR_DONE_OFFSET) == 0) {
        Talk_Run153(1);
    }
    Talk_Place25(0, 0x4000, 0);
    Talk_Place9(1, 0x4000, 0);
    Talk_Place9(2, 0x4000, 10);
    Talk_Place26(0, 0x40000, 0x20000);
    Talk_Place27(1, 0x40000, 0x20000);
    Talk_Place7(2, 0x40000, 0x20000);
    Talk_Run154(152);
    *(u8 *)(Talk_Check59(0) + ACTOR_FLAGS_OFFSET2) &= 254;
    *(u8 *)(Talk_Check60(1) + ACTOR_FLAGS_OFFSET2) &= 254;
    *(u8 *)(Talk_Check61(2) + ACTOR_FLAGS_OFFSET2) &= 254;
    Talk_Run34(0, 132, 206);
    Talk_Run155(1, 136, 221);
    Talk_Place28(2, 122, 238);
    Talk_Run156(3);
    Talk_Run157(80);
    *(u8 *)(Talk_Check62(0) + ACTOR_FLAGS_OFFSET2) |= 1;
    *(u8 *)(Talk_Check63(1) + ACTOR_FLAGS_OFFSET2) |= 1;
    {
        u8 *flags = (u8 *)(Talk_Check22(2) + ACTOR_FLAGS_OFFSET2);
        u8 raised = (u8)(*flags | 1);

        *flags = raised;
    }
    Talk_Place29(0, 0xcccc, 0x6666);
    Talk_Place30(1, 0xcccc, 0x6666);
    Talk_Place31(2, 0xcccc, 0x6666);
    tbl = (s32)gOv10;
    Talk_Run158(1, tbl);
    Talk_Run159(2, tbl);
    Talk_Run36(20);
    /* Advance the scene phase word to its next value. */
    work = *(u8 *volatile *)gWork;
    *(volatile s32 *)(work + 0x1c0) = 0x209;
    *(volatile s32 *)(work + 0x1c8) = 24;
    Talk_Do10(0x82b);
    Talk_Run160();
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
            Talk_Place33(t << 16, (t << 11) + o->unk66, b);
            o->unk8 = b[0];
            o->unkC = b[1];
            o->unk10 = b[2];
        } else {
            Talk_Do17(o);
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
    Talk_Run161();
    Talk_Do18(1);
    Talk_Run162();
    Talk_Run163();
}

void Actor_ResetActorAndCenterOffsets(struct Work_399 *work)
{
    struct Rec_399 *rec;

    work->f85 = 0;
    work->f100 = 0;
    work->f35 &= ~1;

    rec = work->f80;
    rec->mode = 1;

    Talk_Run164(work, 9);
    Talk_Run165(work, 0);

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
        Talk_Run166();
    }
}

void Scene_RunScene399SequenceA(void)
{
    u32 i;
    s32 rec7;
    s32 record;
    s32 base6_3001e40;

    base6_3001e40 = 0x3001e40;
    if (Talk_Check10(*(volatile s32 *)base6_3001e40, 60) == 0) {
        rec7 = Talk_Check11(222, 0x1cf0000, 0, 0x1240000);
        if (rec7 != 0) {
            Talk_Run167();
            *(volatile s32 *)(rec7 + 104) = 60;
            *(volatile s32 *)(rec7 + 108) = 0x200974d;
            Talk_Run168(rec7, 5);
        }
    }
    if (Talk_Check12((*(volatile s32 *)base6_3001e40 + 30), 60) == 0) {
        rec7 = Talk_Check13(222, 0x1400000, 0x200000, 0x1640000);
        if (rec7 != 0) {
            Talk_Run169();
            *(volatile s32 *)(rec7 + 104) = 60;
            *(volatile s32 *)(rec7 + 108) = 0x200974d;
            Talk_Run170(rec7, 5);
        }
    }
    if (Talk_Check14((*(volatile s32 *)base6_3001e40 + 10), 60) == 0) {
        rec7 = Talk_Check15(222, 0x760000, 0, 0x460000);
        if (rec7 != 0) {
            Talk_Run171();
            *(volatile s32 *)(rec7 + 104) = 60;
            *(volatile s32 *)(rec7 + 108) = 0x200974d;
            Talk_Run172(rec7, 5);
        }
    }
    if (Talk_Check16((*(volatile s32 *)base6_3001e40 + 50), 60) == 0) {
        rec7 = Talk_Check17(222, 0x1560000, 0, 0x7c0000);
        if (rec7 != 0) {
            Talk_Run173();
            *(volatile s32 *)(rec7 + 104) = 60;
            *(volatile s32 *)(rec7 + 108) = 0x200974d;
            Talk_Run174(rec7, 5);
        }
    }
    if (Talk_Check18((*(volatile s32 *)base6_3001e40 + 80), 60) == 0) {
        rec7 = Talk_Check19(222, 0x1af0000, 0, 0xab0000);
        if (rec7 != 0) {
            Talk_Run175();
            *(volatile s32 *)(rec7 + 104) = 60;
            *(volatile s32 *)(rec7 + 108) = 0x200974d;
            Talk_Run176(rec7, 5);
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

    obj = Talk_Run13(0);
    x = obj->unk8;
    cx = x >> 19;
    g = 0x200;
    h = 0x201;
    if ((u32)(cx - 24) > 7) {
        y = obj->unk10;
        if ((u32)((y >> 19) - 36) > 9 || (u32)(cx - 22) > 9)
            goto rest;
    }
    r = Talk_Check64(g);
    if (r != 0)
        return;
    (*(State **)0x03001e70)->unk17 = r;
    Talk_Do19(g);
    Talk_Do20(h);
    return;

rest:
    if (x > 0xE80000 && obj->unkC > 0x1E0000 && y > 0xD40000) {
        st = *(State **)0x03001e70;
        st->unk17 = 0;
        Talk_Do19(g);
        Talk_Do20(h);
        return;
    }
    r = Talk_Check65(h);
    if (r != 0)
        return;
    st = *(State **)0x03001e70;
    st->unk17 = 1;
    Talk_Do21(h);
    Talk_Do22(g);
    return;
}

void State_UpdateActor11WithFlag203(void)
{
    s32 a;
    s32 b;

    Talk_Do23(0x203);
    Talk_Apply12(11, 3);
    a = 15;
    b = 7;
    Talk_SetRect(15, 6, 1, 1, a, b);
}

void Actor_SetActorZeroFacingC000AndRun(void)
{
    struct SceneService *work;

    Talk_Run177();
    work = Talk_Run12(0);
    work->value06 = 0xc000;
    Talk_Do24(123);
    Talk_Run178();
    Talk_Run179();
    Talk_Do25(8);
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

    subject = Talk_Run14(gCell.selected_subject);

    for (;;) {
        heading = gOv13[(gIw >> 4) & 15];
        /* The test is on heading << 16 against 0xffff0000, that is on the
         * signed halfword -1, which means "no heading". */
        if ((heading << 16) == (s32)0xffff0000) {
            return;
        }
        /* Nothing is placed in an argument register before this call. */
        Talk_Run180();

        /* The 0x80000 bias is built from an immediate and a shift. */
        probe[0] = (subject->x & (s32)0xfff00000) + 0x80000;
        probe[1] = subject->y;
        probe[2] = (subject->z & (s32)0xfff00000) + 0x80000;
        z = probe[2];
        x = probe[0];
        p = (u8 *)subject;
        p += 34;
        goal = Talk_Check66((s32)*p, x, z);
        /* 0x100000 is built from an immediate and a shift.  The probe block is
         * passed by address and is advanced by the callee. */
        Talk_Run181((s32)0x100000, heading, probe);

        marker = Talk_Check67((s32)*p, probe[0], probe[2]);
        if (marker == 255
                || Talk_Check68((s32)*p, probe[0], probe[2])
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
        Talk_Run182(subject, x, subject->y, z);
        /* Same import as the probe above, two arguments here. */
        Talk_Run183(subject, 2);
        Talk_Run184(subject, 48);
        Talk_Run185(subject);
        subject->callback = (void *)Talk_Run186;

        goto advanceProbe;
continueProbe:
        if (Talk_Check69((s32)*p, probe[0], probe[2])
                - subject->y > 0x80000) {
            goto finishProbe;
        }
        x = probe[0];
        z = probe[2];
        subject->state_048 = 0x20000;
        subject->state_052 = 0x1999;
        Talk_Run187(subject, probe[0], probe[1], probe[2]);
        Talk_Run188(subject);
        if (marker != goal) {
            goto blocked;
        }

advanceProbe:
        AdvanceProbe(heading, probe);
        marker = Talk_Check70((s32)*p, probe[0], probe[2]);
        if (marker != 255) {
            goto continueProbe;
        }

finishProbe:
        subject->state_048 = 0x20000;
        subject->state_052 = 0x10000;
        Talk_Run189(subject, x, subject->y, z);
        Talk_Run190(subject);
        Talk_Run191(2);
        /* The back edge re-reads the heading table and starts again. */
    }

blocked:
    subject->callback = NULL;
    subject->flags_090 |= 1;
    /* 0x4000 is built from an immediate and a shift. */
    subject->state_052 = 0x4000;

tail:
    Talk_Run192(10);
    Talk_Run193();
}
