/* The nine radial effects. */
#include "BABI.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "CALL.H"
#include "SCENE_IDS.H"

#define FIELD_STAGED_ACTOR_IMPORTS
extern const struct SceneEntrance gBabiChikaEntrances1[];
extern const struct SceneEntrance gBabiChikaEntrances2[];
extern const struct SceneEntrance gBabiChikaEntrancesOther[];
extern const struct SceneRegion gBabiChikaRegions2[];

void FieldScene_PlaceAndPinSlots8And9(void);

void FieldScene_PlaceAndPinSlots10And11(void);

void FieldScene_RunScene3c4_02002480(void);

s32 SceneActor_CopyActor8PositionWhenAtRow10();

/* The floor height of each step, by the actor's step index. */
extern s32 Data_0200b350[];

void BattleFx_StartFadeOverlay(s32 value);
void FieldScene_RunLateSequenceHead(void);
void BabiChika_MarkActorCells(void);
void BabiChika_SettleSteps(s32 wait);
s32 SceneActor_SetFlagBitByRelativeDepth();
s32 OverlayObject_SetYAboveLinkedActor();
void BabiChika_UpdateTrackedActor(void);
#define ACTOR_UPDATE_IDLE ((void (*)(union FieldObject *))SceneActor_SetFlagBitByRelativeDepth)
#define ACTOR_UPDATE_PANEL ((void (*)(union FieldObject *))OverlayObject_SetYAboveLinkedActor)
#define SCENE_TASK BabiChika_UpdateTrackedActor

void SceneEffect_SpawnNineRadialEffects(s32 actor)
{
    struct SceneObject *object;
    struct Vec vec;
    struct EffectParams params;
    u32 i;
    s32 v;
    s32 x;
    s32 z;

    object = (struct SceneObject *)Object_GetById(actor);
    params.unk00 = 1;
    params.mode = 7;
    params.callback = (s32)Effect_AdvanceMotion;
    for (i = 0; i <= 16; i += 2) {
        v = i << 12;
        vec.x = Engine_MathCos(v);
        vec.y = 0;
        z = Engine_MathSin(v);
        x = vec.x;
        vec.z = z;
        x = x + Math_Divide(x, 3);
        vec.x = x;
        ((void (*)())Effect_Spawn)(object->x, object->y, object->z, x, vec.y, z, 0x01030001, &params);
    }
}

/* Where the party appears in each of the tunnel's two areas. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&SceneId_BabiChika1) {
        return gBabiChikaEntrances1;
    }
    if (v == (s32)&SceneId_BabiChika2) {
        return gBabiChikaEntrances2;
    }
    return gBabiChikaEntrancesOther;
}

/* Only the second area has map regions. */
const struct SceneRegion *Scene_GetRegions(void)
{
    if (gGameState.scene == (s32)&SceneId_BabiChika2) {
        return gBabiChikaRegions2;
    }
    return 0;
}

/* Scene tables. */
u8 *SceneData_GetTableB85c(void) { return Data_0200b85c; }

u8 *SceneData_SelectAndApplyTableBySceneId(void)
{
    u8 *tbl;

    if (gGameState.scene == (s32)&SceneId_BabiChika1) {
        tbl = Data_0200b8f4;
    } else {
        tbl = Data_0200ba74;
    }
    Func_0808b868(tbl);
    return tbl;
}

/* Copy the cell attributes at (73, 38) to (9, 38), advance the staged pair
   and place and pin slots 8 and 9. */
void SceneState_RunRect73x38Step(void)
{
    Engine_EventBegin();
    Map_CopyCellAttributes(73, 38, 5, 5, 9, 38);
    StagedActor_AdvancePair();
    FieldScene_PlaceAndPinSlots8And9();
    Engine_EventEnd();
}

/* The point left of actor zero. */
void SceneActor_ApplyPointLeftOfActorZero(void)
{
    s32 point[3];
    struct Actor_02000dc8 *actor = Actor_Get(ACTOR_PARTY_LEADER);

    point[0] = actor->f08 + 0xFFE00000;
    point[1] = actor->f0c;
    point[2] = actor->f10;
    SceneActor_MoveActorZeroToTarget(point);
}

/* Copy the cell attributes at (93, 30) to (29, 30), advance the staged pair
   and place and pin slots 10 and 11. */
void FieldScene_RunLayoutAt93By30(void)
{
    Engine_EventBegin();
    Map_CopyCellAttributes(93, 30, 6, 5, 29, 30);
    StagedActor_AdvancePair();
    FieldScene_PlaceAndPinSlots10And11();
    Engine_EventEnd();
}

/* A step and the point two right of actor zero. */
void FieldScene_RunStepWith6(void)
{
    BattleFx_RunRisingObjectSequence(0, 6, 0);
}

void SceneActor_PassPointTwoRightOfActorZero(void)
{
    s32 pos[3];
    struct Actor_02000dc8 *actor = Actor_Get(ACTOR_PARTY_LEADER);

    pos[0] = actor->f08 + 0x200000;
    pos[1] = actor->f0c;
    pos[2] = actor->f10;
    SceneActor_MoveActorZeroToTarget(pos);
}

/* Copy two cell-attribute rectangles to column 25, advance the staged pair
   and run the scene that follows. */
void SceneState_ApplyTwoRectsAndRunThree(void)
{
    Engine_EventBegin();
    Map_CopyCellAttributes(89, 49, 3, 2, 25, 49);
    Map_CopyCellAttributes(89, 51, 8, 5, 25, 51);
    StagedActor_AdvancePair();
    FieldScene_RunScene3c4_02002480();
    Engine_EventEnd();
}

/* Scene tables, layouts and supplemental sequences. */
void SceneActor_CheckTwoUnitsAboveActorZero(void)
{
    struct Actor02001424 *actor = Actor_Get(ACTOR_PARTY_LEADER);
    s32 target[3];

    target[0] = actor->x;
    target[1] = actor->y;
    target[2] = actor->z + 0x00200000;
    if (SceneActor_MoveActorZeroToTarget(target)!= 0) {
        SceneState_ApplyTwoRectsAndRunThree();
    }
}

/*
 * resource_3c4 @ 0x02001458 (84 bytes: 80 code and one pool word).
 *
 * This is the selector-reversed sibling immediately before 0x020014ac and is
 * written in that owner's proven shape.  It initializes query 0x200, tests
 * flag 0x201, then mirrors the queried state into slot 14's byte at +98 and
 * bit 3 of the byte at +89.  The zero halfword at 0x02001456 is alignment
 * after the preceding owner, not part of this one.
 */
void SceneActor_MirrorFlag201IntoSlot14(void)
{
    u8 *flags;
    u8 value;

    GameFlag_Set(0x200);
    if (GameFlag_IsSet(0x201) != 0) {
        ((u8* (*)())Object_GetById)(14)[98] = 0;
        ((u8* (*)())Object_GetById)(14)[89] &= (u8)0xf7;
    } else {
        ((u8* (*)())Object_GetById)(14)[98] = 1;
        flags = Actor_Get(14);
        flags += 89;
        value = 8;
        value |= *flags;
        *flags = value;
    }
}

void SceneActor_SetActor14Field98ByFlag200(void)
{
    u8 *p;
    u8 val;

    GameFlag_Set(0x201);
    if (GameFlag_IsSet(0x200) != 0) {
        ((u8* (*)())Object_GetById)(14)[98] = 0;
        ((u8* (*)())Object_GetById)(14)[89] &= (u8)0xf7;
    } else {
        ((u8* (*)())Object_GetById)(14)[98] = 1;
        p = Actor_Get(14);
        p += 89;
        val = 8;
        val |= *p;
        *p = val;
    }
}

void SceneState_ApplyFlag970(void)
{
    GameFlag_Set(0x970);
}

void SceneState_RunUnlessActorZeroAtTile32x50(void)
{
    struct Actor_02001510 *actor = Actor_Get(ACTOR_PARTY_LEADER);

    if ((actor->f08 >> 20) != 32 || (actor->f10 >> 20) != 50) {
        SceneActor_ApplyPointLeftOfActorZero();
    }
}

void SceneState_RunUnlessActorZeroAt30_52(void)
{
    struct Actor_02000cc0 *actor = Actor_Get(ACTOR_PARTY_LEADER);

    if ((actor->f08 >> 20) != 30 || (actor->f10 >> 20) != 52) {
        SceneActor_PassActorZeroOffsetPoint();
    }
}

void FieldScene_RunSupplementalSequenceTwo(void)
{
    u32 i;
    s32 rec2;
    s32 rec7;
    struct FieldActor *actor;
    s32 value;
    s32 v5;
    s32 v6;
    u8 slot16[40];
    u8 *frame;

    Engine_EventBegin();
    actor = (struct FieldActor *)Object_GetById(18);
    if ((actor->x.fixed >> 20) == 46) {
        Battle_WaitMode0(30);
        rec2 = OverlayObject_CreateAndInitialize(0x2e80000, 0, 0xb80000, 253);
        frame = slot16;
        *(s32 *)(frame + 8) = 0x9999;
        *(s32 *)(frame + 12) = 0x9999;
        *(s32 *)(frame + 4) = 7;
        Actor_Get(18)->motion_flags = 0;
        Audio_PlayCue(185);
        for (i = 0; i < 16; i++) {
            WaitFrames(3);
            Actor_Get(18)->y.fixed -= 0x10000;
            rec7 = Value0(Engine_RandomNext);
            rec7 = ((((u32)(rec7 << 4) >> 16) << 16) + 0x2e00000);
            value = Engine_RandomNext();
            ((void (*)())Effect_Spawn)(rec7, 0, ((((u32)(((value << 3) + value) << 1) >> 16) << 16) + 0x800000), 0, 0, 0, 0x90000, frame);
        }
        Map_CopyCellAttributes(51, 8, 1, 1, 49, 8);
        Battle_WaitMode0(30);
        Actor_Get(18)->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
        Object_SetModeById(18, 3);
        Engine_ObjectDispatchRelease(rec2);
        Map_CopyCellAttributes(45, 4, 1, 1, 46, 8);
        Actor_SetPosition(20, 0x2e80000, 0x880000);
        v5 = 1;
        v6 = 3;
        Audio_PlayCue(188);
        Map_CopyCellsTo(58, 8, 49, 8, v5, v6);
        Work_SetValuesIfNonNegative(0, 0x50000, 0x10000);
        Work_SetValuesIfNonNegative(-1, -1, 0xe666);
        Battle_WaitMode0(20);
        Audio_PlayCue(188);
        Map_CopyCellsTo(59, 8, 49, 8, v5, v6);
        Work_SetValuesIfNonNegative(0, 0x50000, 0x10000);
        Work_SetValuesIfNonNegative(-1, -1, 0xe666);
        Engine_MapRenderWaitForValues();
        Battle_WaitMode0(10);
        GameFlag_Set(0x971);
    }
    Engine_EventEnd();
}

void FieldScene_RunFourCallSequenceB(void)
{
    typedef s32(*Handler_02001a10)(struct Record_02000ec8 *record);

    Engine_EventBegin();
    StagedActor_AdvancePair();
    Engine_EventEnd();
    FieldScene_RunSupplementalSequenceTwo();
}

void ActorPresentation_ConfigureActorTwentyAndFlag200(void)
{
    u8 *flags;

    Object_SetModeById(20, 1);
    Actor_SetChildValue(20, 0);
    Object_SetModeById(20, 2);
    flags = ((u8* (*)())Object_GetById)(20) + 35;
    *flags &= 0xFD;
    GameFlag_Set(0x200);
}

void FieldScene_RunSupplementalSequenceOne(void)
{
    u32 i;
    s32 rec2;
    s32 rec7;
    struct FieldActor *actor;
    s32 value;
    s32 arg0;
    s32 arg2;
    s32 v5;
    s32 v6;
    u8 slot16[40];
    u8 *slot;

    Engine_EventBegin();
    actor = (struct FieldActor *)Object_GetById(19);
    if ((actor->x.fixed >> 20) == 48) {
        if (GameFlag_IsSet(0x202) != 0) {
            Battle_WaitMode0(30);
            rec2 = OverlayObject_CreateAndInitialize(0x3020000, 0, 0x1120000, 223);
            slot = slot16;
            *(s32 *)(slot + 8) = 0x9999;
            *(s32 *)(slot + 12) = 0x9999;
            *(s32 *)(slot + 4) = 7;
            Actor_Get(19)->motion_flags = 0;
            Audio_PlayCue(185);
            for (i = 0; i < 16; i++) {
                WaitFrames(3);
                Actor_Get(19)->y.fixed -= 0x10000;
                rec7 = Engine_RandomNext();
                arg0 = ((((u32)(rec7 << 4) >> 16) << 16) + 0x3000000);
                value = Engine_RandomNext();
                arg2 = ((((u32)(((value << 3) + value) << 1) >> 16) << 16) + 0xe00000);
                ((void (*)())Effect_Spawn)(arg0, 0, arg2, 0, 0, 0, 0x90000, slot);
            }
            Map_CopyCellAttributes(51, 8, 1, 1, 45, 14);
            Battle_WaitMode0(30);
            Actor_Get(19)->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
            Object_SetModeById(19, 3);
            Engine_ObjectDispatchRelease(rec2);
            Map_CopyCellAttributes(45, 4, 1, 1, 48, 14);
            Actor_SetPosition(21, 0x3080000, 0xe80000);
            v5 = 1;
            v6 = 3;
            Audio_PlayCue(188);
            Map_CopyCellsTo(58, 8, 45, 14, v5, v6);
            Work_SetValuesIfNonNegative(0, 0x50000, 0x10000);
            Work_SetValuesIfNonNegative(-1, -1, 0xe666);
            Battle_WaitMode0(20);
            Audio_PlayCue(188);
            Map_CopyCellsTo(59, 8, 45, 14, v5, v6);
            Work_SetValuesIfNonNegative(0, 0x50000, 0x10000);
            Work_SetValuesIfNonNegative(-1, -1, 0xe666);
            Engine_MapRenderWaitForValues();
            Battle_WaitMode0(10);
            GameFlag_Set(0x972);
        }
    }
    Engine_EventEnd();
}

void FieldScene_RunFourStepSequenceA(void)
{
    typedef s32(*Handler_02001a10)(struct Record_02000ec8 *record);

    Engine_EventBegin();
    StagedActor_AdvancePair();
    Engine_EventEnd();
    FieldScene_RunSupplementalSequenceOne();
}

void FieldScene_SetActor19TableB3B8(void)
{
    Engine_ActorEnableActionCallback(19, (s32)BabiChika_FlickerScript);
}

void SceneState_SetValue202ThenCall(void)
{
    GameFlag_Set(0x202);
    FieldScene_RunSupplementalSequenceOne();
}

void SceneActor_ConfigureSlot21AndSetFlag201(void)
{
    u8 *flags;

    Object_SetModeById(21, 1);
    Actor_SetChildValue(21, 0);
    Object_SetModeById(21, 2);
    flags = ((u8* (*)())Object_GetById)(21) + 35;
    *flags &= 0xFD;
    GameFlag_Set(0x201);
}

void SceneDialogue_RunFlag982Or983Dialogue(void)
{
    Engine_EventBegin();
    Object_SetModeById(ACTOR_PARTY_LEADER, 1);
    if (GameFlag_IsSet(0x982) != 0 || GameFlag_IsSet(0x983) != 0) {
        Engine_MessageShowCentered(MsgBabiChikaStatueSpeaksAfterAnswer, 1);
    } else {
        Engine_MessageShowCentered(MsgBabiChikaStatueSpeaksRobinSoulYe, 1);
    }
    Engine_EventEnd();
}

void FieldScene_RunTwoStepSequence(void)
{
    typedef s32(*Handler_02001a10)(struct Record_02000ec8 *record);

    Engine_EventBegin();
    Engine_EventEnd();
}

void FieldScene_RunFourStepSequenceB(void)
{
    typedef s32(*Handler_02001a10)(struct Record_02000ec8 *record);

    Engine_EventBegin();
    StagedActor_AdvancePair();
    Engine_EventEnd();
    FieldScene_RunTwoStepSequence();
}

void SceneActor_InstallSlotNineHandler(void)
{
    typedef s32(*Handler_02001a10)(struct Record_02000ec8 *record);

    u8 *owner;

    Engine_ActorEnableActionCallback(8, (s32)BabiChika_FlickerScript);
    GameFlag_Set(0x203);
    owner = Actor_Get(9);
    *(s32 *)(owner + 108) = (s32)SceneActor_CopyActor8PositionWhenAtRow10;
}

/*
 * Brings slot 9 up: four state writes, clear bit 1 of the byte at +35,
 * publish selector 0x204, pin an overlay at slot 9's 12.20 grid cell, then
 * install one handler on slots 9 and 8. The 136-byte owner at 0x02001a10
 * includes its alignment halfword and its one pool word; that word is an
 * odd Thumb pointer, so the handler is SceneActor_SetFlagBitByRelativeDepth. The bit-clear folds
 * +35 into the returned pointer through the address local, not into a copy.
 */
void SceneActor_SetupSlotNineAndInstallHandler(void)
{
    typedef s32(*Handler_02001a10)(struct Record_02000ec8 *record);

    u8 *desc;
    s32 col;
    s32 row;

    Engine_EventBegin();
    Engine_ActorSetSpritePriority(9, 1);
    Object_SetModeById(9, 1);
    Actor_SetChildValue(9, 0);
    Object_SetModeById(9, 2);

    {
        u8 *flag = ((u8* (*)())Object_GetById)(9) + 35;
        *flag &= (u8)0xfd;
    }

    GameFlag_Set(0x204);

    col = ((Slot_02001a10* (*)())Object_GetById)(9)->col;
    row = ((Slot_02001a10* (*)())Object_GetById)(9)->row >> 20;
    Map_CopyCellAttributes(26, 8, 1, 1, col >> 20, row);

    desc = Actor_Get(9);
    *(Handler_02001a10 *)(desc + 108) = SceneActor_SetFlagBitByRelativeDepth;

    desc = Actor_Get(8);
    *(Handler_02001a10 *)(desc + 108) = SceneActor_SetFlagBitByRelativeDepth;

    ((u8* (*)())Engine_EventEnd)(desc);
}

s32 OverlayObject_SetYAboveLinkedActor(u8 *owner)
{
    s16 *id = (s16 *)(owner + 100);
    struct Actor *actor = Actor_Get(*id);

    *(s32 *)(owner + 12) = actor->f0c + 0x100000;
    return 0;
}

/* Moves steps 10 and 11 to the floor heights their step indices name, then
 * marks the cell under each of actors 10 to 14 that has sunk below the
 * floor. */
void BabiChika_SettleSteps(s32 wait)
{
    u32 i;

    Call3((void (*)())ObjectMotion_SetSpeedParameters, 10, 0x8000, 0x4000);
    Call3((void (*)())ObjectMotion_SetSpeedParameters, 11, 0x8000, 0x4000);
    if (wait != 0) {
        Audio_PlayCue(180);
    }
    Object_SetPosition(Object_GetById(10), Object_GetById(10)->x.fixed,
                             Data_0200b350[(s16)Object_GetById(10)->unknown_64], Object_GetById(10)->z.fixed);
    Object_SetPosition(Object_GetById(11), Object_GetById(11)->x.fixed,
                             Data_0200b350[(s16)Object_GetById(11)->unknown_64], Object_GetById(11)->z.fixed);
    ObjectMotion_CommitCurrentPositionAndActivate(10);
    ObjectMotion_CommitCurrentPositionAndActivate(11);
    Object_GetById(10)->y.fixed = Data_0200b350[(s16)Object_GetById(10)->unknown_64];
    Object_GetById(11)->y.fixed = Data_0200b350[(s16)Object_GetById(11)->unknown_64];
    if (wait != 0) {
        Audio_PlayCue(0x121);
    }
    for (i = 0; i < 5; i++) {
        if (Object_GetById(i + 10)->y.fixed / 0x10000 < 0 && Object_GetById(i + 10)->y.fixed / 0x10000 > -30) {
            Map_CopyCellAttributeRect(4, 9, 1, 1, Object_GetById(i + 10)->x.fixed >> 20,
                                         Object_GetById(i + 10)->z.fixed >> 20);
        }
    }
    Battle_WaitMode0(wait);
}

/* Platform landing and the late sequences. */

/*
 * Scan slots 10 to 14, skipping the subject, keep those whose whole-tile x
 * and z match the subject's, and take the greatest height among them; the
 * winning slot index goes into the subject's tag at +100. The starting best
 * height is -5.0 in 12.20, held in the owner's one pool word, and it is what
 * the move receives when no slot matches -- the tag is then left untouched.
 * Every field read re-fetches its record, which is the shape to keep.
 */
void SceneActor_LandOnHighestPlatform(s32 subject)
{
    s32 best = (s32)0xffb00000;
    u32 i;

    for (i = 0; i <= 4; i++) {
        s32 slot = i + 10;

        if (slot == subject) continue;

        if ((((Slot_02001c2c* (*)())Object_GetById)(slot)->x >> 20) != (((Slot_02001c2c* (*)())Object_GetById)(subject)->x >> 20)) continue;
        if ((((Slot_02001c2c* (*)())Object_GetById)(slot)->z >> 20) != (((Slot_02001c2c* (*)())Object_GetById)(subject)->z >> 20)) continue;

        /*
         * 0x00100000 is one whole unit above the candidate's own height. The
         * comparison is signed, and a tie updates the best.
         */
        if (best > ((Slot_02001c2c* (*)())Object_GetById)(slot)->y + 0x100000) continue;

        best = ((Slot_02001c2c* (*)())Object_GetById)(slot)->y + 0x100000;
        *(u16 *)((u8 *)Actor_Get(subject) + 100) = (u16)slot;
    }

    Actor_SetSpeed(subject, 0x40000, 0x20000);   /* 128 << 11, 128 << 10 */

    /*
     * Three separate lookups of the same record, in this order. The locals
     * fix the sequence, which argument evaluation order would not.
     */
    {
        Slot_02001c2c *target = Actor_Get(subject);
        Slot_02001c2c *from = Actor_Get(subject);
        s32 z = ((Slot_02001c2c* (*)())Object_GetById)(subject)->z;

        Object_SetPosition(target, from->x, best, z);
    }

    ObjectMotion_CommitCurrentPositionAndActivate(subject);
    Audio_PlayCue(188);
    SceneEffect_SpawnNineRadialEffects(subject);
    Battle_WaitMode0(30);
}

void FieldScene_RunMiddleSequence(void)
{
    struct FieldActor saved;
    s32 i;
    s32 j;
    s32 found;

    Engine_EventBegin();
    for (i = 0; i <= 2; i++) {
        if (((struct FieldActor *)Object_GetById(i + 12))->sprite->priority == 3
            && GameFlag_IsSet(i + 0x200) == 0) {
            Actor_Get(i + 12);
            SceneActor_WaitValueBelowLimit();
            Actor_SetPosition(i + 12, 0, 0);
            GameFlag_Set(i + 0x200);
            break;
        }
        if ((((struct FieldActor *)Object_GetById(i + 12))->z.fixed >> 20) == 9
            && GameFlag_IsSet(i + 0x200) == 0) {
            *(s32 *)((u8 *)Object_GetById(i + 12) + 20) = 0;
            ((struct FieldActor *)Object_GetById(i + 12))->velocity_y = 0;
            *(s32 *)(((s32 (*)())Object_GetById)(i + 12) + 60) = -0x80000000;
            ((struct FieldActor *)Actor_Get(i + 12))->motion_flags = 0;
            *(u16 *)(((s32 (*)())Object_GetById)(i + 12) + 100) = 0;
            found = i;
            for (j = 0; j < i; j++) {
                if (GameFlag_IsSet(0x200 + j) == 0) {
                    saved.x.fixed = ((struct FieldActor *)Object_GetById(i + 12))->x.fixed;
                    saved.y.fixed = ((struct FieldActor *)Object_GetById(i + 12))->y.fixed;
                    saved.z.fixed = ((struct FieldActor *)Object_GetById(i + 12))->z.fixed;
                    ((struct FieldActor *)Actor_Get(i + 12))->x.fixed =
                        ((struct FieldActor *)Object_GetById(j + 12))->x.fixed;
                    ((struct FieldActor *)Object_GetById(i + 12))->y.fixed =
                        ((struct FieldActor *)Object_GetById(j + 12))->y.fixed;
                    ((struct FieldActor *)Object_GetById(i + 12))->z.fixed =
                        ((struct FieldActor *)Object_GetById(j + 12))->z.fixed;
                    ((struct FieldActor *)Actor_Get(j + 12))->x.fixed = saved.x.fixed;
                    ((struct FieldActor *)Actor_Get(j + 12))->y.fixed = saved.y.fixed;
                    ((struct FieldActor *)Actor_Get(j + 12))->z.fixed = saved.z.fixed;
                    found = j;
                    break;
                }
            }
            *(s32 *)(((s32 (*)())Object_GetById)(found + 12) + 20) = 0;
            ((struct FieldActor *)Actor_Get(found + 12))->velocity_y = 0;
            *(s32 *)(((s32 (*)())Object_GetById)(found + 12) + 60) = -0x80000000;
            ((struct FieldActor *)Actor_Get(found + 12))->motion_flags = 0;
            *(u16 *)(((s32 (*)())Object_GetById)(found + 12) + 100) = 0;
            Engine_CameraSetSpeed(0x30000, 0x6000);
            ((struct FieldActor *)Battle_GetWorkObject1e0())->motion_flags = 0;
            Camera_MoveTo(0xa80000, 0x80000, 0xb80000, 1);
            Engine_CameraWaitForMove();
            SceneActor_LandOnHighestPlatform(found + 12);
            if ((((struct FieldActor *)Object_GetById(found + 12))->x.fixed >> 20) == 8) {
                (*(s16 *)(((s32 (*)())Object_GetById)(10) + 100))++;
                (*(s16 *)(((s32 (*)())Object_GetById)(11) + 100))--;
            } else {
                (*(s16 *)(((s32 (*)())Object_GetById)(10) + 100))--;
                (*(s16 *)(((s32 (*)())Object_GetById)(11) + 100))++;
            }
            ((struct FieldActor *)Actor_Get(found + 12))->update = (void (*)(union FieldObject *))OverlayObject_SetYAboveLinkedActor;
            BabiChika_SettleSteps(40);
            ((struct FieldActor *)Actor_Get(found + 12))->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
            GameFlag_Set(0x200 + found);
            break;
        }
    }
    Engine_EventEnd();
}

void FieldScene_RunThreeStepSequence(void)
{
    typedef s32(*Handler_02001a10)(struct Record_02000ec8 *record);

    Engine_EventBegin();
    StagedActor_AdvancePair();
    Engine_EventEnd();
}

void SceneState_SetSlot17And18Selectors(void)
{
    Engine_EventBegin();

    if ((((Slot_02001f70* (*)())Object_GetById)(17)->w8 >> 20) == 45) {
        GameFlag_Set(0x974);
    } else {
        GameFlag_Clear(0x974);
    }

    if ((((Slot_02001f70* (*)())Object_GetById)(18)->w8 >> 20) == 46) {
        GameFlag_Set(0x975);
    } else {
        GameFlag_Clear(0x975);
    }

    BabiChika_MarkActorCells();
    Engine_EventEnd();
}

void FieldScene_RunFourCallSequence(void)
{
    typedef s32(*Handler_02001a10)(struct Record_02000ec8 *record);

    Engine_EventBegin();
    StagedActor_AdvancePair();
    SceneState_SetSlot17And18Selectors();
    Engine_EventEnd();
}

/* Runs six queued setup calls for this scene: two paired 6-argument calls
 * whose last two args repeat the first two (72,49 / 113,43), two 3-argument
 * id calls (100, 101) with zeroed remaining args, and two more 3-argument
 * calls (id 15, 16) passing a start/end pair where the second call's start
 * value (198<<18) equals the first call's end value. */
void FieldScene_RunLateSequenceHead(void)
{
    Map_CopyCellAttributes(72, 49, 1, 1, 8, 49); /* main:080091c0 */
    Map_CopyCellAttributes(113, 43, 1, 1, 49, 43); /* main:080091c0 */
    MapObject_SetPosition(100, 0, 0);
    MapObject_SetPosition(101, 0, 0);
    Actor_SetPosition(15, 8912896, 51904512); /* 136<<16, 198<<18 */
    Actor_SetPosition(16, 51904512, 45613056); /* 198<<18, 174<<18 */
}

/* Two six-argument calls whose first and fifth arguments repeat the same id
 * (8 and 49 respectively), followed by four three-argument calls each keyed
 * by an id with a trailing pair of values (-1, -1 or 0, 0). */
void FieldScene_RunLateSequenceSecond(void)
{
    Map_CopyCellAttributes(8, 113, 1, 1, 8, 49);
    Map_CopyCellAttributes(49, 107, 1, 1, 49, 43);
    MapObject_SetPosition(100, -1, -1);
    MapObject_SetPosition(101, -1, -1);
    Actor_SetPosition(15, 0, 0);
    Actor_SetPosition(16, 0, 0);
}

void FieldScene_RunScene3c4SequenceA(void)
{
    s32 record;
    s32 v6;
    s32 p5;
    s32 q;

    v6 = 0;
    Engine_EventBegin();
    Map_CopyCellAttributes(83, 45, 11, 8, 19, 45);
    record = Object_GetById(19);
    p5 = *(s32 *)(record + 8);
    q = Object_GetById(19)->z.fixed;
    q >>= 20;
    p5 >>= 20;
    Map_CopyCellAttributes(20, 56, 1, 1, p5, q);
    record = Object_GetById(20);
    p5 = *(s32 *)(record + 8);
    q = Object_GetById(20)->z.fixed;
    q >>= 20;
    p5 >>= 20;
    Map_CopyCellAttributes(20, 56, 1, 1, p5, q);
    record = Object_GetById(21);
    p5 = *(s32 *)(record + 8);
    q = Object_GetById(21)->z.fixed;
    q >>= 20;
    p5 >>= 20;
    Map_CopyCellAttributes(20, 56, 1, 1, p5, q);
    record = Object_GetById(22);
    p5 = *(s32 *)(record + 8);
    q = Object_GetById(22)->z.fixed;
    q >>= 20;
    p5 >>= 20;
    Map_CopyCellAttributes(20, 56, 1, 1, p5, q);
    record = Object_GetById(23);
    p5 = *(s32 *)(record + 8);
    q = Object_GetById(23)->z.fixed;
    q >>= 20;
    p5 >>= 20;
    Map_CopyCellAttributes(20, 56, 1, 1, p5, q);
    record = Object_GetById(19);
    if ((*(s32 *)(record + 8) >> 20) == 25) {
        record = Object_GetById(19);
        if ((*(s32 *)(record + 16) >> 20) == 49) {
            v6 = 1;
        }
    }
    record = Object_GetById(20);
    if ((*(s32 *)(record + 8) >> 20) == 23) {
        record = Object_GetById(20);
        if ((*(s32 *)(record + 16) >> 20) == 49) {
            v6 = (v6 + 1);
        }
    }
    record = Object_GetById(21);
    if ((*(s32 *)(record + 8) >> 20) == 25) {
        record = Object_GetById(21);
        if ((*(s32 *)(record + 16) >> 20) == 47) {
            v6 = (v6 + 1);
        }
    }
    record = Object_GetById(22);
    if ((*(s32 *)(record + 8) >> 20) == 23) {
        record = Object_GetById(22);
        if ((*(s32 *)(record + 16) >> 20) == 47) {
            v6 = (v6 + 1);
        }
    }
    record = Object_GetById(23);
    if ((*(s32 *)(record + 8) >> 20) == 24) {
        record = Object_GetById(23);
        if ((*(s32 *)(record + 16) >> 20) == 48) {
            v6 = (v6 + 1);
        }
    }
    if (v6 == 5) {
        if (GameFlag_IsSet(0x984) != 0) {
            Engine_EventEnd();
            goto done;
        }
        Battle_WaitMode0(20);
        Camera_SetSpeed(0xcccc, 0x1999);
        Camera_MoveTo(0x1d80000, -1, 0x30c0000, 1);
        Engine_CameraWaitForMove();
        Battle_WaitMode0(30);
        GameFlag_Set(0x984);
        Audio_PlayCue(158);
        Map_AnimateCells(Data_0200b3ec, 32, 46);
        Map_CopyCellAttributes(24, 60, 1, 1, 32, 47);
        Battle_WaitMode0(40);
    } else {
        if (GameFlag_IsSet(0x984) != 0) {
            Battle_WaitMode0(20);
            Camera_SetSpeed(0xcccc, 0x1999);
            Camera_MoveTo(0x1d80000, -1, 0x30c0000, 1);
            Engine_CameraWaitForMove();
            Battle_WaitMode0(30);
            GameFlag_Clear(0x984);
            Audio_PlayCue(159);
            Map_AnimateCells(Data_0200b40c, 32, 46);
            Map_CopyCellAttributes(31, 47, 1, 1, 32, 47);
            Battle_WaitMode0(40);
        }
    }
    Engine_EventEnd();
    done:;
}

void FieldScene_RunLayoutAt83By45(void)
{
    Engine_EventBegin();
    {
        s32 width = 19;
        s32 height = 45;

        Map_CopyCellAttributes(83, 45, 11, 8, width, height);
    }
    StagedActor_AdvancePair();
    FieldScene_RunScene3c4SequenceA();
    Engine_EventEnd();
}

void SceneState_SetValue268bInScene(void)
{
    Engine_EventBegin();
    Object_SetModeById(ACTOR_PARTY_LEADER, 1);
    Engine_MessageShowCentered(MsgBabiChikaStatueSeemsSpeakYourSoul, 1);
    Engine_EventEnd();
}

void FieldScene_RunScriptedStep953(void)
{
    Engine_EventBegin();
    Engine_MessageShowCentered(MsgFieldDoorTightlyLocked, 1);
    Engine_EventEnd();
}

s32 SceneData_SelectTableByWord224(void)
{
    if (gGameState.scene == (s32)&SceneId_BabiChika1) {
        return (s32)Data_0200bc0c;
    }
    return (s32)Data_0200bef4;
}

void FieldScene_PlaceAndPinSlots8And9(void)
{
    typedef s32(*Handler_02001a10)(struct Record_02000ec8 *record);

    s32 row;

    {
        s32 p5 = 9, p6 = 38;
        Map_CopyCellAttributes(73, 38, 5, 5, p5, p6);
    }
    SceneState_SwapSlotPairByRank(9, 8);

    {
        s32 col = ((Slot_020023a0* (*)())Object_GetById)(8)->column >> 20;
        row = ((Slot_020023a0* (*)())Object_GetById)(8)->row >> 20;
        Map_CopyCellAttributes(2, 36, 1, 1, col, row);
    }

    {
        s32 col = ((Slot_020023a0* (*)())Object_GetById)(9)->column >> 20;
        row = ((Slot_020023a0* (*)())Object_GetById)(9)->row >> 20;
        Map_CopyCellAttributes(2, 36, 1, 1, col, row);
    }
}

void FieldScene_PlaceAndPinSlots10And11(void)
{
    s32 row;

    {
        s32 k5 = 29, k6 = 30;
        Map_CopyCellAttributes(93, 30, 6, 5, k5, k6);
    }
    SceneState_SwapSlotPairByRank(11, 10);

    {
        s32 col20 = ((Slot_02002410* (*)())Object_GetById)(10)->column >> 20;
        row = ((Slot_02002410* (*)())Object_GetById)(10)->row >> 20;
        Map_CopyCellAttributes(2, 36, 1, 1, col20, row);
    }

    {
        s32 col20 = ((Slot_02002410* (*)())Object_GetById)(11)->column >> 20;
        row = ((Slot_02002410* (*)())Object_GetById)(11)->row >> 20;
        Map_CopyCellAttributes(2, 36, 1, 1, col20, row);
    }
}

void FieldScene_RunScene3c4_02002480(void)
{
    s32 record;
    s32 p5;

    Map_CopyCellAttributes(89, 49, 3, 2, 25, 49);
    Map_CopyCellAttributes(89, 51, 8, 5, 25, 51);
    *(u8 *)(((s32 (*)())Object_GetById)(14) + 34) = 1;
    record = Object_GetById(12);
    p5 = *(s32 *)(record + 8);
    record = Object_GetById(12);
    p5 = p5 >> 20;
    Map_CopyCellAttributes(22, 52, 1, 1, p5, (*(s32 *)(record + 16) >> 20));
    record = Object_GetById(13);
    p5 = *(s32 *)(record + 8);
    record = Object_GetById(13);
    p5 = p5 >> 20;
    Map_CopyCellAttributes(22, 52, 1, 1, p5, (*(s32 *)(record + 16) >> 20));
    record = Object_GetById(14);
    p5 = *(s32 *)(record + 8);
    record = Object_GetById(14);
    p5 = p5 >> 20;
    Map_CopyCellAttributes(22, 52, 1, 1, p5, (*(s32 *)(record + 16) >> 20));
}

/* Copies the cell attributes back and marks the grid cells under actors 17 and 18. */
void BabiChika_MarkActorCells(void)
{
    Map_CopyCellAttributes(108, 19, 4, 1, 44, 19);
    StagedActor_FillGridAttributeRectangle(0, Object_GetById(17)->x.fixed >> 20, Object_GetById(17)->z.fixed >> 20, 1, 1, 0xff);
    StagedActor_FillGridAttributeRectangle(0, Object_GetById(18)->x.fixed >> 20, Object_GetById(18)->z.fixed >> 20, 1, 1, 0xff);
}

/* Entry setup for the underground passage: by area and entrance, restores the lift cells, pins and parks the paired actors and re-applies each flagged block. */
s32 FieldScene_InitializeActorGroups(void)
{
    gEventWork->start_transition = 0x204;
    if (gGameState.scene == (s32)&SceneId_BabiChika1 || gGameState.scene == (s32)&SceneId_BabiChika2) {
        BattleFx_StartFadeOverlay(0);
        gGameState.retreat_entrance = 1;
        gGameState.retreat_scene = (s32)&SceneId_BabiChika1;
    }
    if (gGameState.scene == (s32)&SceneId_BabiChika1) {
        switch (gGameState.entrance) {
        case 1:
        case 2:
            if (Engine_GameFlagIsSet(0x982)) {
                Call6((void (*)())Engine_MapCopyCellsTo, 121, 4, 74, 9, 5, 8);
                Call6((void (*)())Engine_MapCopyCellsTo, 18, 83, 9, 73, 3, 2);
                Engine_MapCopyCellsTo(18, 81, 9, 75, 3, 2);
                Engine_MapCopyCellsTo(18, 83, 9, 77, 3, 2);
                Engine_MapCopyCellsTo(18, 83, 9, 79, 3, 2);
                Engine_MapCopyCellsTo(18, 83, 11, 78, 3, 2);
                Engine_MapCopyCellsTo(18, 83, 13, 79, 3, 2);
            } else {
                if (!Engine_GameFlagIsSet(0x983))
                    break;
                Call6((void (*)())Engine_MapCopyCellsTo, 121, 13, 74, 9, 5, 8);
                Call6((void (*)())Engine_MapCopyCellsTo, 18, 85, 11, 74, 3, 2);
                Engine_MapCopyCellsTo(18, 83, 13, 75, 3, 2);
                Engine_MapCopyCellsTo(18, 85, 11, 76, 3, 2);
                Engine_MapCopyCellsTo(18, 83, 11, 78, 3, 2);
                Engine_MapCopyCellsTo(18, 83, 13, 79, 3, 2);
            }
            break;
        case 3:
        case 4:
            FieldScene_PlaceAndPinSlots8And9();
            Object_GetById(8)->motion_flags = 0;
            Object_GetById(9)->motion_flags = 0;
            Engine_ActorSetSpriteFlags(Object_GetById(8), 0);
            Engine_ActorSetSpriteFlags(Object_GetById(9), 0);
            Object_GetById(8)->update = ACTOR_UPDATE_IDLE;
            Object_GetById(9)->update = ACTOR_UPDATE_IDLE;
            Engine_TaskAddCallback(SCENE_TASK, 0xc80);
            break;
        case 5:
        case 6:
        case 7:
            if (Engine_GameFlagIsSet(0x982))
                Call6((void (*)())Engine_MapCopyCells, 23, 17, 1, 2, 30, 8);
            if (Engine_GameFlagIsSet(0x983))
                Call6((void (*)())Engine_MapCopyCells, 23, 17, 1, 2, 32, 10);
            break;
        case 8:
        case 9:
            FieldScene_PlaceAndPinSlots10And11();
            Object_GetById(10)->motion_flags = 0;
            Object_GetById(11)->motion_flags = 0;
            Engine_ActorSetSpriteFlags(Object_GetById(10), 0);
            Engine_ActorSetSpriteFlags(Object_GetById(11), 0);
            Object_GetById(10)->update = ACTOR_UPDATE_IDLE;
            Object_GetById(11)->update = ACTOR_UPDATE_IDLE;
            Engine_TaskAddCallback(SCENE_TASK, 0xc80);
            break;
        case 10:
        case 11:
            Engine_ActorSetSpriteFlags(Object_GetById(18), 0);
            Engine_ActorSetSpriteFlags(Object_GetById(19), 0);
            Object_SetModeById(18, 2);
            Engine_ActorSetSpriteFlags(Object_GetById(20), 0);
            Engine_ActorSetSpriteFlags(Object_GetById(21), 0);
            Engine_ActorSetChildValue(20, 15);
            Engine_ActorSetChildValue(21, 15);
            if (Engine_GameFlagIsSet(0x971)) {
                Call6((void (*)())Engine_MapCopyCellsTo, 59, 8, 49, 8, 1, 3);
                Map_CopyCellAttributeRect(51, 8, 1, 1, 49, 8);
                Object_GetById(18)->priority_flags |= 2;
                Object_SetModeById(18, 3);
                Map_CopyCellAttributeRect(45, 4, 1, 1, 46, 8);
                Call3((void (*)())Engine_ActorSetPosition, 18, 186 << 18, 136 << 16);
                Object_GetById(18)->y.fixed = -0x100000;
                Call3((void (*)())Engine_ActorSetPosition, 20, 186 << 18, 136 << 16);
            }
            if (Engine_GameFlagIsSet(0x200)) {
                Engine_ActorSetChildValue(20, 0);
                Object_SetModeById(20, 5);
            }
            if (Value1(Engine_GameFlagIsSet, 0x202))
                Object_SetModeById(19, 2);
            if (Engine_GameFlagIsSet(0x972)) {
                Call6((void (*)())Engine_MapCopyCellsTo, 59, 8, 45, 14, 1, 3);
                Map_CopyCellAttributeRect(51, 8, 1, 1, 45, 14);
                Object_GetById(19)->priority_flags |= 2;
                Object_SetModeById(19, 3);
                Map_CopyCellAttributeRect(45, 4, 1, 1, 48, 14);
                Call3((void (*)())Engine_ActorSetPosition, 19, 194 << 18, 232 << 16);
                Object_GetById(19)->y.fixed = -0x100000;
                Engine_ActorSetPosition(21, 194 << 18, 232 << 16);
                Engine_GameFlagSet(0x202);
            }
            if (Engine_GameFlagIsSet(0x201)) {
                Engine_ActorSetChildValue(21, 0);
                Object_SetModeById(21, 5);
            }
            break;
        case 12:
        case 13:
            FieldScene_RunScene3c4_02002480();
            Object_GetById(12)->motion_flags = 0;
            Object_GetById(13)->motion_flags = 0;
            Engine_ActorSetSpriteFlags(Object_GetById(15), 0);
            Engine_ActorSetSpriteFlags(Object_GetById(16), 0);
            Engine_ActorSetSpriteFlags(Object_GetById(17), 0);
            Engine_ActorSetSpriteFlags(Object_GetById(12), 0);
            Engine_ActorSetSpriteFlags(Object_GetById(13), 0);
            Engine_ActorSetSpriteFlags(Object_GetById(14), 0);
            Object_GetById(12)->update = ACTOR_UPDATE_IDLE;
            Object_GetById(13)->update = ACTOR_UPDATE_IDLE;
            Object_GetById(14)->update = ACTOR_UPDATE_IDLE;
            Engine_TaskAddCallback(SCENE_TASK, 0xc80);
            break;
        }
    } else {
        switch (gGameState.entrance) {
        case 0:
            break;
        case 1:
        case 2:
        case 3:
            gGameState.retreat_entrance = 1;
            gGameState.retreat_scene = (s32)&SceneId_BabiIriguchi3;
            Engine_GameFlagClear(0x12f);
            Engine_ActorSetChildValue(17, 6);
            Engine_ActorSetChildValue(18, 6);
            if (Engine_GameFlagIsSet(0x974))
                Call3((void (*)())Engine_ActorSetPosition, 17, 182 << 18, 156 << 17);
            if (Engine_GameFlagIsSet(0x975))
                Call3((void (*)())Engine_ActorSetPosition, 18, 186 << 18, 156 << 17);
            BabiChika_MarkActorCells();
            break;
        case 6:
        case 7:
            Engine_ActorSetSpritePriority(8, 1);
            Object_GetById(8)->motion_flags = 0;
            Engine_ActorSetSpriteFlags(Object_GetById(8), 0);
            Engine_ActorSetSpritePriority(9, 1);
            Engine_ActorSetChildValue(9, 15);
            Engine_ActorSetSpriteFlags(Object_GetById(9), 0);
            Object_GetById(9)->motion_flags = 0;
            if (!Engine_GameFlagIsSet(0x204))
                break;
            Engine_ActorSetChildValue(9, 0);
            Object_SetModeById(9, 5);
            Map_CopyCellAttributeRect(26, 8, 1, 1, Object_GetById(9)->x.fixed >> 20, Object_GetById(9)->z.fixed >> 20);
            Object_GetById(9)->update = ACTOR_UPDATE_IDLE;
            Object_GetById(8)->update = ACTOR_UPDATE_IDLE;
            break;
        case 4:
        case 5:
            if (!Engine_GameFlagIsSet(0x109)) {
                Object_GetById(10)->motion_flags = 0;
                Object_GetById(11)->motion_flags = 0;
                Object_GetById(10)->y.fixed = -0x300000;
                Object_GetById(11)->y.fixed = -0x300000;
                Object_GetById(10)->priority_flags |= 2;
                Object_GetById(11)->priority_flags |= 2;
                Object_GetById(10)->collision_flags &= 0xfe;
                Object_GetById(11)->collision_flags &= 0xfe;
                Object_GetById(10)->unknown_64 = 3;
                Object_GetById(11)->unknown_64 = 3;
                Engine_ActorSetSpritePriority(10, 1);
                Engine_ActorSetSpritePriority(11, 1);
                Object_GetById(12)->motion_flags = 0;
                Object_GetById(13)->motion_flags = 0;
                Object_GetById(14)->motion_flags = 0;
                Engine_ActorSetSpriteFlags(Object_GetById(12), 0);
                Engine_ActorSetSpriteFlags(Object_GetById(13), 0);
                Engine_ActorSetSpriteFlags(Object_GetById(14), 0);
                Object_GetById(12)->unknown_64 = 0;
                Object_GetById(13)->unknown_64 = 0;
                Object_GetById(14)->unknown_64 = 0;
                if (gGameState.entrance != 5)
                    break;
                Object_GetById(10)->y.fixed = -0x200000;
                Object_GetById(11)->y.fixed = -0x400000;
                Object_GetById(10)->unknown_64 = 2;
                Object_GetById(11)->unknown_64 = 4;
                Call3((void (*)())Engine_ActorSetPosition, 12, 200 << 16, 152 << 16);
                Object_GetById(12)->unknown_64 = 11;
                Object_GetById(12)->update = ACTOR_UPDATE_PANEL;
                Object_GetById(12)->priority_flags |= 2;
                Call3((void (*)())Engine_ActorSetPosition, 13, 200 << 16, 152 << 16);
                Object_GetById(13)->unknown_64 = 12;
                Object_GetById(13)->update = ACTOR_UPDATE_PANEL;
                Object_GetById(13)->priority_flags |= 2;
                Engine_ActorSetPosition(14, 136 << 16, 152 << 16);
                Object_GetById(14)->unknown_64 = 10;
                Object_GetById(14)->update = ACTOR_UPDATE_PANEL;
                Object_GetById(14)->priority_flags |= 2;
                Battle_WaitMode0(2);
                Engine_GameFlagSet(0x200);
                Engine_GameFlagSet(0x201);
                Engine_GameFlagSet(0x202);
            }
            BabiChika_SettleSteps(0);
            break;
        case 8:
        case 9:
        case 10:
        case 11:
            if (Engine_GameFlagIsSet(0x982))
                Call6((void (*)())Engine_MapCopyCells, 10, 30, 1, 2, 16, 30);
            if (Engine_GameFlagIsSet(0x983))
                Call6((void (*)())Engine_MapCopyCells, 10, 30, 1, 2, 22, 30);
            Engine_GameFlagSet(0x973);
            break;
        case 12:
            Call6((void (*)())Map_CopyCellAttributeRect, 8, 49, 1, 1, 8, 113);
            FieldScene_RunLateSequenceHead();
            Engine_TaskAddCallback(SCENE_TASK, 0xc80);
            break;
        case 13:
        case 14:
            Battle_WaitMode0(1);
            if (Engine_GameFlagIsSet(0x984)) {
                Call6((void (*)())Engine_MapCopyCells, 24, 59, 1, 2, 32, 46);
                Call3((void (*)())Engine_ActorSetPosition, 19, 204 << 17, 198 << 18);
                Call3((void (*)())Engine_ActorSetPosition, 20, 188 << 17, 198 << 18);
                Call3((void (*)())Engine_ActorSetPosition, 21, 204 << 17, 190 << 18);
                Call3((void (*)())Engine_ActorSetPosition, 22, 188 << 17, 190 << 18);
                Call3((void (*)())Engine_ActorSetPosition, 23, 196 << 17, 194 << 18);
            }
            Object_GetById(19)->motion_flags &= 0xfe;
            Object_GetById(20)->motion_flags &= 0xfe;
            Object_GetById(21)->motion_flags &= 0xfe;
            Object_GetById(22)->motion_flags &= 0xfe;
            Object_GetById(23)->motion_flags &= 0xfe;
            Engine_ActorSetChildValue(19, 4);
            Engine_ActorSetChildValue(20, 1);
            Engine_ActorSetChildValue(21, 4);
            Engine_ActorSetChildValue(22, 10);
            Engine_ActorSetChildValue(23, 0);
            Object_SetModeById(19, 2);
            Object_SetModeById(23, 2);
            Map_CopyCellAttributeRect(20, 56, 1, 1, Object_GetById(19)->x.fixed >> 20, Object_GetById(19)->z.fixed >> 20);
            Map_CopyCellAttributeRect(20, 56, 1, 1, Object_GetById(20)->x.fixed >> 20, Object_GetById(20)->z.fixed >> 20);
            Map_CopyCellAttributeRect(20, 56, 1, 1, Object_GetById(21)->x.fixed >> 20, Object_GetById(21)->z.fixed >> 20);
            Map_CopyCellAttributeRect(20, 56, 1, 1, Object_GetById(22)->x.fixed >> 20, Object_GetById(22)->z.fixed >> 20);
            Map_CopyCellAttributeRect(20, 56, 1, 1, Object_GetById(23)->x.fixed >> 20, Object_GetById(23)->z.fixed >> 20);
            break;
        case 17:
            Call6((void (*)())Map_CopyCellAttributeRect, 49, 43, 1, 1, 49, 107);
            FieldScene_RunLateSequenceHead();
            break;
        }
    }
    return 0;
}
