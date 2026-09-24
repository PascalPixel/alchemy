#include "TYPES.H"
#include "FIELD_EFFECT.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "ITEM_IDS.H"

enum FlaggedCueMessage {
    MSG_MORE_TOLBIS_SOLDIERS_LAY_DEFEATED = 0x2694,
    MSG_OHH_THEY_TOOK_SHEBA_HEADED = 0x26af,
    MSG_WHERE_GOING_ROBIN_IODEM_ASKED = 0x288e,
    MSG_BABI_TOLD_ME_SHIP_ANCIENTS = 0x28a5,
    MSG_THOUGHT_SHIP_WE_SAW_AT = 0x28b0,
    MSG_WHERE_GOING_ARENT_WE_TAKING = 0x28b7,
    MSG_WAS_TOLD_LET_IN_IF = 0x28b8
};


#define NULL ((void *)0)
#define ObjectMotion_RealignToTrackedObjectAndArmCallback_1(a0, a1, a2, a3) Call4(Func_02001882, a0, a1, a2, a3)
#define ObjectMotion_RealignToTrackedObjectAndArmCallback_1_020007c4(a0, a1, a2, a3) Call4(Func_02001e12, a0, a1, a2, a3)
#define ObjectMotion_RealignToTrackedObjectAndArmCallback_2(a0, a1, a2, a3) Call4(Func_02001e22, a0, a1, a2, a3)
#define ObjectMotion_RealignToTrackedObjectAndArmCallback_3(a0, a1, a2, a3) Call4(Func_02001e30, a0, a1, a2, a3)
#define Scene_GetRecord_1_020007c4(args...) Func_02001ee4(args)
#define Scene_GetRecord_4(a0) Value1(Engine_ActorGet, a0)
#define Scene_GetRecord_5(a0) Value1(Engine_ActorGet, a0)
#define Scene_GetRecord_6(a0) Value1(Engine_ActorGet, a0)
#define SCENE_STEP (*(u16 *)(*(u8 **)0x03001ebc + 0x1d8))
#define REC_S16(rec, off) (*(s16 *)((rec) + (off)))
#define Scene_RunTableTransition Func_02000158
#define SceneActor_SetActor23Params2And6 Func_02000030
#define SceneData_GetTable975c Func_02000040
#define SceneData_ReturnZero Func_02000048
#define SceneData_GetTable98c4 Func_0200004c
#define SceneData_SelectTableByFlag9a7 Func_02000054
#define SceneActor_SetupActorForTable9638 Func_02000078
#define SceneActor_UpdateObjectWithCue28be Func_020000c4
#define SceneActor_SetupActor18Event Func_0200010c
#define SceneState_SetWord1c8To16AndForward16c Func_020001f0
#define FieldScene_RunPrimarySequence Func_02000218
#define FieldScene_RunSecondarySequence Func_020007c4
#define FieldScene_RunScene3c6SequenceA Func_02001090
#define FieldScene_RunSequenceB Func_020011bc
#define SceneData_SelectSecondaryTableByFlag9a7 Func_020011e8

extern u8 Value_000028be;
extern s16 Data_02009ca8[][2];
extern u8 Data_02009cee[];
extern u8 Data_02009cd8[];

void Func_0200164a(s32, s32, s32);
void Func_02001882();
void Func_02001d8c_a(void);
void Func_02001e12();
void Func_02001e22();
void Func_02001e30();
void Func_02001eb6();
void Func_02001ef0();
void Func_020023d2();
void Func_0200240a();
u8 *Func_020016c6(s32);

/* A signed 16-bit field of an actor record returned by one of the record
 * lookups. */

/*
 * Each pseudo symbol above names the per-site call word the overlay image
 * holds, not a runtime address -- one word can serve two sites with different
 * targets -- and the macro names the function the site reaches through the
 * veneers, keeping the site's own calling form. Names without a repository
 * binding are provisional.
 */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    void Actor_FaceDirection();

    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    void Actor_FaceDirection();

    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    void Actor_FaceDirection();

    f(a0, a1, a2);
}

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    void Actor_SetSpeed(s32, s32, s32);

    Actor_SetSpeed(actor, x, y);
}

static __inline__ void SetOffset(s32 actor, s32 offset, s32 zero)
{
    void Actor_FaceDirection(s32, s32, s32);

    Actor_WalkByAndWait(actor, offset, zero);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    u8 *Func_02001ee4();

    return f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    u8 *Func_02001ee4();

    f(a0, a1, a2, a3);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    s32 Func_02001ee4();

    f(a0, a1);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    s32 Func_02001ee4();

    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ s32 Scene_Value3(s32 (*fn)(), s32 a, s32 b, s32 c)
{
    return fn(a, b, c);
}

static __inline__ void Scene_Call3(void (*fn)(), s32 a, s32 b, s32 c)
{
    fn(a, b, c);
}

void SceneActor_SetActor23Params2And6(void)
{
    Func_0200164a(0x17, 2, 6);
}

/*
 * Returns the in-image table at 0x0200975c. The eight-byte owner includes its
 * one pool word, which holds that address and is returned without being
 * dereferenced.
 */
u8 *SceneData_GetTable975c(void)
{
    return (u8 *)0x0200975c;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

/*
 * Returns the in-image table at 0x020098c4. The eight-byte owner includes its
 * one pool word, which holds that address and is returned without being
 * dereferenced.
 */
u8 *SceneData_GetTable98c4(void)
{
    return (u8 *)0x020098c4;
}

s32 SceneData_SelectTableByFlag9a7(void)
{
    if (GameFlag_IsSet(0x9A7) != 0) {
        return 0x02009A98;
    }
    return 0x02009900;
}

void SceneActor_SetupActorForTable9638(s32 actor)
{
    void Actor_FaceDirection();

    struct FieldActor *object;

    object = (struct FieldActor *)Actor_Get(actor);
    object->scale_x = 0x10000;
    object = (struct FieldActor *)Value1(Engine_ActorGet, actor);
    object->scale_y = 0x10000;
    Event_SetMessage(MSG_OHH_THEY_TOOK_SHEBA_HEADED);
    Event_ShowMessage(actor, 0);
    Actor_FaceDirection(actor, 0xc000, 0);
    Event_Wait(20);
    Engine_ActorEnableActionCallback(actor, 0x2009638);
}

void SceneActor_UpdateObjectWithCue28be(s32 obj)
{
    s32 cue = (s32)&Value_000028be;
    Event_SetMessage(cue);
    Event_OpenMessage(obj, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(10);
        Event_SetMessage(cue + 1);
    } else {
        Event_SetMessage(cue + 2);
    }
    Event_ShowMessage(obj, 0);
}

void SceneActor_SetupActor18Event(void)
{
    void Actor_FaceDirection(s32, s32, s32);

    GameFlag_Set(2491);
    Event_SetMessage(MSG_WAS_TOLD_LET_IN_IF);
    Event_ShowMessage(18, 0);
    PlaceActor(18, 65536, 32768);
    SetOffset(18, -16, 0);
    Actor_FaceDirection(18, 0, 0);
    Event_Wait(10);
}

void Scene_RunTableTransition(void)
{
    s32 no = gEventWork->touched_trigger;
    s32 x = Data_02009ca8[no][0];
    s32 y = Data_02009ca8[no][1];

    Func_020016c6(0)[85] = 2;
    Audio_PlayCue(158);
    if (no == 6) {
        Scene_Value3(Engine_MapAnimateCells, (s32)Data_02009cee, (u16)x, (u16)y);
        Scene_Call3(Engine_ActorWalkBy, 0, 0, -16);
    } else {
        Scene_Value3(Engine_MapAnimateCells, (s32)Data_02009cd8, (u16)x, (u16)y);
        Scene_Call3(Engine_ActorCenterAndWalk, 0, 2, -16);
    }
    Event_Wait(10);
    gEventWork->transition_frames = 16;
    Event_RequestExit(no);
}

void SceneState_SetWord1c8To16AndForward16c(void)
{
    extern u8 *Data_03001ebc;

    u8 *work = Data_03001ebc;
    s16 *p = (s16 *)(work + 0x16C);
    s32 n = *p;

    *(s32 *)(work + 0x1C8) = 16;
    Event_RequestExit(n);
}

/*
 * Runs the fixed call sequence for this scene: a paced series of setup and
 * per-entity calls -- position, pose and property triples keyed by entity id,
 * interleaved with timed single-argument steps -- ending with a record lookup
 * whose s16 fields at +10 and +18 feed the last positioning call.
 */
void FieldScene_RunPrimarySequence(void)
{
    u8 *Func_02001ee4();

    u32 i;
    u8 *record;

    Event_Begin();
    Event_SetMessage(MSG_MORE_TOLBIS_SOLDIERS_LAY_DEFEATED);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0xf80000, 0x1a80000);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_SetAnimation(8, 0);
    Actor_SetAnimation(9, 0);
    Event_OpenScreen(); /* main:0808a360 */
    Event_WaitForScreen(); /* main:0808a370 */
    Event_Wait(20);
    ObjectMotion_RealignToTrackedObjectAndArmCallback_1(22, 8, -16, 0xc000);
    Actor_WaitForMove(22);
    Event_Wait(20);
    Actor_ShowEmote(22, 0x102, 60);
    Actor_SetSpeed(22, 0x10000, 0x8000);
    Actor_WalkByAndWait(22, 0, -16);
    Event_Wait(10);
    Actor_FaceActor(22, 8, 40);
    Actor_FaceActor(22, 9, 40);
    Actor_FaceActor(22, 8, 40);
    Event_Wait(10);
    Actor_RunRepeatedMotion(22, 2);
    Event_Wait(20);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_FaceDirection(22, 0x4000, 0);
    Event_Wait(30);
    Event_OpenMessage(22, 0); /* main:0808a178 */
    Event_ChooseYesNo(0, 0);
    Event_Wait(30);
    Actor_RunRepeatedMotion(8, 2);
    Event_Wait(20);
    Event_ShowMessage(8, 0);
    Event_Wait(10);
    Actor_FaceActor(22, 8, 30);
    Actor_RunRepeatedMotion(22, 2);
    Event_Wait(20);
    Event_ShowMessage(22, 0);
    Actor_SetAnimation(8, 1);
    Event_Wait(20);
    Actor_RunRepeatedMotion(8, 2);
    Event_Wait(30);
    Event_ShowMessage(8, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(22, 3);
    Event_Wait(20);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(9, 2);
    Event_Wait(20);
    Event_ShowMessage(9, 0);
    Event_Wait(10);
    Actor_FaceActor(22, 9, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 9, 30);
    Actor_SetAnimation(9, 1);
    Event_Wait(10);
    Actor_ShowEmote(22, 0x108, 40);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_Wait(20);
    Event_Wait(10);
    Actor_FaceDirection(22, 0x4000, 0);
    Event_Wait(30);
    Actor_RunRepeatedMotion(22, 2);
    Event_Wait(20);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(9, 2);
    Event_Wait(30);
    Event_ShowMessage(9, 0);
    Event_Wait(10);
    Actor_ShowEmote(22, 0x102, 50);
    Actor_FaceActor(22, 9, 20);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(8, 2);
    Event_Wait(20);
    Event_ShowMessage(8, 0);
    Event_Wait(10);
    Actor_FaceDirection(22, 0x4000, 0);
    Event_Wait(30);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_Wait(20);
    Event_Wait(20);
    Actor_ShowEmote(22, 0x102, 50);
    Actor_SetSpeed(22, 0x1cccc, 0xe666);
    Actor_WalkToAndWait(22, 0x100, 0x168);
    Event_Wait(10);
    Actor_FaceDirection(22, 0, 0);
    Event_Wait(30);
    Actor_FaceDirection(22, 0x8000, 0);
    Event_Wait(30);
    Actor_FaceDirection(22, 0xc000, 0);
    Event_Wait(30);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(9, 2);
    Event_Wait(20);
    Event_ShowMessage(9, 0);
    Event_Wait(10);
    Actor_ShowEmote(22, 0x100, 40);
    Actor_FaceDirection(22, 0x2000, 0);
    Event_Wait(20);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(9, 3);
    Event_Wait(30);
    Event_ShowMessage(9, 0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(22, 2);
    Event_Wait(20);
    Actor_SetSpeed(22, 0x19999, 0xcccc);
    Actor_WalkToAndWait(22, 0x100, 0x180);
    Actor_FaceDirection(22, 0, 0);
    Event_Wait(20);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(8, 2);
    Event_Wait(20);
    Event_ShowMessage(8, 0);
    Event_Wait(10);
    Actor_FaceDirection(22, 0x8000, 0);
    Event_Wait(40);
    Actor_SetAnimationAndWait(22, 3);
    Event_Wait(20);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_SetSpeed(22, 0x13333, 0x9999);
    Actor_WalkByAndWait(22, 0, 16);
    Event_Wait(10);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 80);
    Actor_ShowEmote(22, 0x101, 80);
    Actor_FaceActor(22, 8, 40);
    Actor_FaceActor(22, 9, 40);
    Actor_FaceDirection(22, 0x4000, 0);
    Event_Wait(30);
    Actor_RunRepeatedMotion(22, 2);
    Event_Wait(20);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(9, 2);
    Event_Wait(20);
    Event_ShowMessage(9, 0);
    Event_Wait(10);
    Actor_FaceDirection(22, 0, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 9, 30);
    Event_Wait(10);
    Actor_RunRepeatedMotion(8, 2);
    Event_Wait(20);
    Event_ShowMessage(8, 0);
    Event_Wait(10);
    Actor_FaceDirection(22, 0x8000, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 70);
    Actor_FaceDirection(22, 0x4000, 0);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Event_Wait(40);
    Actor_RunRepeatedMotion(22, 2);
    Event_Wait(20);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(22, 3);
    Event_Wait(30);
    Actor_SetSpeed(22, 0x13333, 0x9999);
    Actor_SetAnimation(22, 2);
    record = Actor_Get(ACTOR_PARTY_LEADER);
    if (record != 0) {
        /* Pass the record's s16 fields at +10 and +18 through to entity 22. */
        Actor_SetDestination(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(22);
    Actor_SetPosition(22, 0, 0);
    Event_Wait(10);
    Event_End();
}

/*
 * Drives actors 0 to 3, 22 and 25 through a long timed sequence of pose, move
 * and sprite-flag calls, gated by two condition checks that each pick one of
 * two call sequences and both bump the shared scene step counter.
 */
void FieldScene_RunSecondarySequence(void)
{
    s32 Func_02001ee4();

    u32 i;
    u8 *record;

    GameFlag_Set(0x9ba);
    Event_Begin();
    Event_SetMessage(MSG_WHERE_GOING_ROBIN_IODEM_ASKED);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 104, 0x178);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    ObjectMotion_RealignToTrackedObjectAndArmCallback_1_020007c4(1, -32, 0, 0);
    ObjectMotion_RealignToTrackedObjectAndArmCallback_2(3, -16, 16, 0xe000);
    ObjectMotion_RealignToTrackedObjectAndArmCallback_3(2, 0, 16, 0xc000);
    Actor_WaitForMove(ACTOR_GERALD);
    Event_Wait(30);
    Event_OpenMessage(ACTOR_GERALD, 0);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    Event_Wait(10);
    /* Either path bumps the step counter once, at a different point in its
     * four calls. */
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Actor_SetAnimationAndWait(ACTOR_MIA, 4);
        Event_Wait(20);
        Event_ShowMessage(ACTOR_MIA, 0);
        SCENE_STEP += 1;
    } else {
        Event_Wait(20);
        Actor_SetAnimationAndWait(ACTOR_MIA, 4);
        Event_Wait(20);
        SCENE_STEP += 1;
        Event_ShowMessage(ACTOR_MIA, 0);
    }
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_IVAN, 0x108, 40);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Event_Wait(10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Event_Wait(40);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(30);
    Actor_WalkByAndWait(ACTOR_PARTY_LEADER, -16, 0);
    Actor_WalkBy(ACTOR_IVAN, 0, -16);
    Actor_WalkBy(ACTOR_MIA, 0, -8);
    Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, -16);
    Actor_SetAnimation(ACTOR_MIA, 1);
    Actor_SetAnimation(ACTOR_IVAN, 1);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    Actor_FaceDirection(22, 0x4000, 0);
    Event_Wait(80);
    Actor_SetSpeed(22, 0xcccc, 0x6666);
    /* Set the byte at offset 85 of actor 22's record to 2. */
    *(u8 *)(Scene_GetRecord_1_020007c4(22) + 85) = 2;
    Actor_SetSpritePriority(22, 2);
    Call6(Func_02001eb6, 34, 0, 1, 2, 4, 18);
    Audio_PlayCue(158);
    Event_Wait(20);
    Actor_SetPosition(22, 0x480000, 0x1380000);
    Event_Wait(20);
    Actor_WalkByAndWait(22, 0, 16);
    Call6(Func_02001ef0, 32, 0, 1, 2, 4, 18);
    Audio_PlayCue(159);
    Event_Wait(20);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(40);
    Actor_WalkByAndWait(22, 16, 0);
    Actor_FaceDirection(22, 0x4000, 0);
    Event_Wait(20);
    Event_Wait(10);
    Actor_SetAnimationAndWait(22, 3);
    Event_Wait(30);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(30);
    Event_ShowMessage(ACTOR_MIA, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(22, 2);
    Event_Wait(20);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Event_Wait(10);
    Actor_ShowEmote(22, 0x102, 40);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_IVAN, 0x100, 40);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(22, 3);
    Event_Wait(30);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_FaceEachOther(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 0);
    Event_Wait(60);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    Event_Wait(40);
    Event_Wait(10);
    Actor_SetAnimationAndWait(22, 3);
    Event_Wait(30);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_MIA, 0x102, 40);
    Event_ShowMessage(ACTOR_MIA, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(22, 3);
    Event_Wait(20);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Event_Wait(20);
    Event_OpenMessage(ACTOR_GERALD, 0);
    /* The same branch-and-bump shape as above. */
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Actor_SetAnimationAndWait(22, 4);
        Event_Wait(20);
        Event_ShowMessage(22, 0);
        SCENE_STEP += 1;
    } else {
        Event_Wait(20);
        Actor_SetAnimationAndWait(22, 4);
        Event_Wait(20);
        SCENE_STEP += 1;
        Event_ShowMessage(22, 0);
    }
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_IVAN, 0x102, 40);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_ShowEmote(22, 0x106, 50);
    Event_ShowMessage(22, 0);
    Event_Wait(20);
    Actor_SetPosition(25, 0x580000, 0x14c0000);
    Event_ShowMessage(-1, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_MIA, 0x100, 40);
    Actor_Jump(ACTOR_MIA, 4, 13);
    Actor_Jump(ACTOR_MIA, 4, 30);
    Event_ShowMessage(ACTOR_MIA, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(22, 3);
    Event_Wait(30);
    Event_ShowMessage(22, 0);
    /* One extra call, run only when GameFlag_IsSet(0x9bf) is non-zero. */
    if (GameFlag_IsSet(0x9bf) != 0) {
        Func_02001d8c_a();
    }
    Event_SetMessage(MSG_BABI_TOLD_ME_SHIP_ANCIENTS);
    Event_Wait(10);
    Actor_RunRepeatedMotion(22, 2);
    Event_Wait(20);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(22, 3);
    Event_Wait(30);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 40);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(22, 2);
    Event_Wait(20);
    Event_ShowMessage(22, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_Wait(30);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(22, 3);
    Event_Wait(30);
    Actor_SetSpeed(25, 0xcccc, 0x6666);
    Actor_SetDestinationOffset(25, 0, 16);
    Actor_WalkByAndWait(22, 0, 16);
    Event_Wait(30);
    Actor_SetPosition(25, 0, 0);
    SCENE_STEP += 1;
    Party_GiveItem(242, 0);
    Event_Wait(10);
    /* Clear bit 0 of the actor flag byte, then set it back through a second
     * record accessor. */
    ((struct FieldActor *)Actor_Get(22))->unknown_5a &= ~1;
    Actor_WalkByAndWait(22, 0, -16);
    ((struct FieldActor *)Actor_Get(22))->unknown_5a |= 1;
    Actor_FaceDirection(22, 0x4100, 0);
    Event_Wait(30);
    Event_Wait(10);
    Actor_ShowEmote(22, 0x100, 40);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(22, 3);
    Event_Wait(30);
    Event_ShowMessage(22, 0);
    Event_Wait(20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(50);
    Event_Wait(10);
    Actor_SetAnimationAndWait(22, 3);
    Event_Wait(30);
    Actor_WalkByAndWait(22, -16, 0);
    Actor_FaceDirection(22, 0xc000, 0);
    Event_Wait(20);
    Call6(Func_020023d2, 34, 0, 1, 2, 4, 18);
    Audio_PlayCue(158);
    Event_Wait(10);
    Actor_WalkByAndWait(22, 0, -16);
    Actor_SetPosition(22, 0, 0);
    Event_Wait(10);
    Call6(Func_0200240a, 32, 0, 1, 2, 4, 18);
    Audio_PlayCue(159);
    Event_Wait(50);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_MIA, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_Wait(30);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Event_Wait(10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Event_Wait(40);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(30);
    Actor_SetSpeed(ACTOR_GERALD, 0x13333, 0x9999);
    Actor_SetSpeed(ACTOR_MIA, 0x13333, 0x9999);
    Actor_SetSpeed(ACTOR_IVAN, 0x13333, 0x9999);
    Actor_SetAnimation(ACTOR_GERALD, 2);
    record = Scene_GetRecord_4(0);
    if (record != 0) {
        Actor_SetDestination(ACTOR_GERALD, REC_S16(record, 10), REC_S16(record, 18));
    }
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Actor_SetAnimation(ACTOR_MIA, 2);
    record = Scene_GetRecord_5(0);
    if (record != 0) {
        Actor_SetDestination(ACTOR_MIA, REC_S16(record, 10), REC_S16(record, 18));
    }
    Actor_WaitForMove(ACTOR_MIA);
    Actor_SetPosition(ACTOR_MIA, 0, 0);
    Actor_SetAnimation(ACTOR_IVAN, 2);
    record = Scene_GetRecord_6(0);
    if (record != 0) {
        Actor_SetDestination(ACTOR_IVAN, REC_S16(record, 10), REC_S16(record, 18));
    }
    Actor_WaitForMove(ACTOR_IVAN);
    Actor_SetPosition(ACTOR_IVAN, 0, 0);
    Event_Wait(10);
    Event_End();
}

/* A fixed, unbranching sequence of overlay calls with constant arguments: no
 * loop, no stored result, no use of the scene work record. */
void FieldScene_RunScene3c6SequenceA(void)
{
    u32 i;
    u8 *record;

    Event_SetMessage(MSG_THOUGHT_SHIP_WE_SAW_AT);
    Event_Wait(20);
    Actor_RunRepeatedMotion(22, 2);
    Event_Wait(20);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(22, 4);
    Event_Wait(20);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x100, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x100, 55);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(22, 4);
    Event_Wait(20);
    Event_ShowMessage(22, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_MIA, 0x102, 40);
    Event_ShowMessage(ACTOR_MIA, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Event_Wait(30);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_IVAN, 0x8000, 0);
    Event_Wait(65);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    Event_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_Wait(30);
    Event_ShowMessage(ACTOR_IVAN, 0);
}

void FieldScene_RunSequenceB(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Event_SetMessage(MSG_WHERE_GOING_ARENT_WE_TAKING);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, -16);
    Event_End();
}

s32 SceneData_SelectSecondaryTableByFlag9a7(void)
{
    if (GameFlag_IsSet(0x9A7) != 0) {
        return 0x02009EE4;
    }
    return 0x02009D04;
}

enum {
    ENTRANCE_PRIMARY_SEQUENCE = 20,
    ENTRANCE_PRIMARY_SEQUENCE_ALTERNATE = 21,
    ENTRANCE_FROM_AERIE = 90,
    ENTRANCE_FROM_AERIE_ALTERNATE = 91
};

enum {
    FLAG_AERIE_EVENTS_DONE = 0x9a7,
    FLAG_PRIMARY_SEQUENCE_SEEN = 0x9b8,
    FLAG_ACTOR_18_MOVED = 0x9bb,
    FLAG_ARRIVED_FROM_AERIE = 0x9bf
};

enum {
    ACTOR_BLACK_ORB = 25
};

enum {
    HEAP_ITEM_ICON = 17,
    ITEM_ICON_BUFFER_SIZE = 0x608,
    ITEM_ICON_TILES = 0x400,
    ITEM_ICON_TILE_BYTES = 128
};

/*
 * Lalivero's scene setup. Arriving from the aerie by entrance 90 or 91 sets
 * flag 0x9a7, and entrance 90 also sets flag 0x9bf. Actors 19 to 21 take
 * animation 3. Actor 25 is placed 10 pixels up and shows the Black Orb's icon.
 * While flag 0x9a7 is set, actor 24's sprite flags and collision are cleared,
 * three map cells are copied and flag 0x9bb moves actor 18. Otherwise actors 8
 * and 9 face north and south, five cells are copied, and arriving by entrance
 * 20 or 21 plays the primary sequence once, recorded by flag 0x9b8.
 */
s32 Scene_Initialize(void)
{
    union FieldObject *object;
    struct FieldSprite *sprite;
    u8 *icon;

    if (gGameState.entrance == ENTRANCE_FROM_AERIE) {
        GameFlag_Set(FLAG_AERIE_EVENTS_DONE);
        GameFlag_Set(FLAG_ARRIVED_FROM_AERIE);
    }
    if (gGameState.entrance == ENTRANCE_FROM_AERIE_ALTERNATE) {
        GameFlag_Set(FLAG_AERIE_EVENTS_DONE);
    }
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    gEventWork->transition_frames = 24;

    Actor_SetAnimation(19, 3);
    Actor_Get(19)->collision_flags = 0;
    Actor_SetSpriteFlags(Actor_Get(19), 0);
    Actor_SetAnimation(20, 3);
    Actor_Get(20)->collision_flags = 0;
    Actor_SetSpriteFlags(Actor_Get(20), 0);
    Actor_SetAnimation(21, 3);
    Actor_Get(21)->collision_flags = 0;
    Actor_SetSpriteFlags(Actor_Get(21), 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_BLACK_ORB), 0);

    object = (union FieldObject *)Actor_Get(ACTOR_BLACK_ORB);
    object->actor.unknown_5c = 1;
    object->actor.motion_flags = 0;
    sprite = object->actor.sprite;
    object->actor.y.fixed = PIXELS(10);
    sprite->part_count = 0;
    sprite->full_color = 0;
    sprite->palette = 0;
    icon = Heap_Allocate(HEAP_ITEM_ICON, ITEM_ICON_BUFFER_SIZE);
    Item_LoadIcon(ITEM_BLACK_ORB);
    Vram_Load(sprite->vram_block, ITEM_ICON_TILE_BYTES, &icon[ITEM_ICON_TILES]);
    Heap_Release(HEAP_ITEM_ICON);

    if (GameFlag_IsSet(FLAG_AERIE_EVENTS_DONE)) {
        Actor_SetSpriteFlags(Actor_Get(24), 0);
        Actor_Get(24)->collision_flags = 0;
        Map_CopyCellAttributes(20, 23, 1, 1, 14, 4);
        Map_CopyCellAttributes(20, 23, 1, 1, 15, 4);
        Map_CopyCellAttributes(20, 23, 1, 1, 16, 4);
        if (GameFlag_IsSet(FLAG_ACTOR_18_MOVED)) {
            Actor_SetPosition(18, PIXELS(56), PIXELS(184));
        }
    } else {
        object = (union FieldObject *)Actor_Get(8);
        object->actor.collision_flags = 0;
        object->actor.priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
        object->actor.sprite->flags = 0;
        object->actor.sprite->rotation = FACING_NORTH;
        object = (union FieldObject *)Actor_Get(9);
        object->actor.collision_flags = 0;
        object->actor.priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
        object->actor.sprite->flags = 0;
        object->actor.sprite->rotation = FACING_SOUTH;
        Map_CopyCellAttributes(20, 23, 1, 1, 13, 23);
        Map_CopyCellAttributes(20, 23, 1, 1, 14, 23);
        Map_CopyCellAttributes(20, 23, 1, 1, 78, 23);
        Map_CopyCellAttributes(20, 23, 1, 1, 17, 23);
        Map_CopyCellAttributes(20, 23, 1, 1, 18, 23);
        if ((gGameState.entrance == ENTRANCE_PRIMARY_SEQUENCE
                || gGameState.entrance == ENTRANCE_PRIMARY_SEQUENCE_ALTERNATE)
            && !GameFlag_IsSet(FLAG_PRIMARY_SEQUENCE_SEEN)) {
            GameFlag_Set(FLAG_PRIMARY_SEQUENCE_SEEN);
            object = (union FieldObject *)Actor_Get(11);
            object->actor.unknown_5b = 1;
            object = (union FieldObject *)Actor_Get(17);
            object->actor.unknown_5b = 1;
            Scene_RunPrimarySequence();
            object = (union FieldObject *)Actor_Get(11);
            object->actor.unknown_5b = 0;
            object = (union FieldObject *)Actor_Get(17);
            object->actor.unknown_5b = 0;
        }
    }
    return 0;
}
