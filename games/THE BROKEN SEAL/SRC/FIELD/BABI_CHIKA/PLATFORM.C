#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "BABI.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "CALL.H"
#include "SCENE_IDS.H"

#define FIELD_STAGED_ACTOR_IMPORTS




s32 SceneActor_CopyActor8PositionWhenAtRow10();


void BattleFx_StartFadeOverlay(s32 value);
#define ACTOR_UPDATE_IDLE ((void (*)(union FieldObject *))SceneActor_SetFlagBitByRelativeDepth)
#define ACTOR_UPDATE_PANEL ((void (*)(union FieldObject *))OverlayObject_SetYAboveLinkedActor)

void SceneEffect_SpawnNineRadialEffects(s32 actor)
{
    struct FieldActor *object;
    struct Vec vec;
    struct EffectParams params;
    u32 i;
    s32 v;
    s32 x;
    s32 z;

    object = Object_GetById(actor);
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
        ((void (*)())Effect_Spawn)(object->x.fixed, object->y.fixed, object->z.fixed, x, vec.y, z, 0x01030001, &params);
    }
}

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

const struct SceneRegion *Scene_GetRegions(void)
{
    if (gGameState.scene == (s32)&SceneId_BabiChika2) {
        return gBabiChikaRegions2;
    }
    return 0;
}

u8 *BabiChika_GetExits(void) { return Data_0200b85c; }

u8 *SceneData_SelectAndApplyTableBySceneId(void)
{
    u8 *tbl;

    if (gGameState.scene == (s32)&SceneId_BabiChika1) {
        tbl = Data_0200b8f4;
    } else {
        tbl = Data_0200ba74;
    }
    FieldScene_PrepareActors((struct SceneRegionEntry *)tbl);
    return tbl;
}

void SceneState_RunRect73x38Step(void)
{
    Engine_EventBegin();
    Map_CopyCellAttributes(73, 38, 5, 5, 9, 38);
    StagedActor_AdvancePair();
    FieldScene_PlaceAndPinSlots8And9();
    Engine_EventEnd();
}

void SceneActor_ApplyPointLeftOfActorZero(void)
{
    s32 point[3];
    struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);

    point[0] = actor->x.fixed + 0xFFE00000;
    point[1] = actor->y.fixed;
    point[2] = actor->z.fixed;
    SceneActor_MoveActorZeroToTarget(point);
}

void FieldScene_RunLayoutAt93By30(void)
{
    Engine_EventBegin();
    Map_CopyCellAttributes(93, 30, 6, 5, 29, 30);
    StagedActor_AdvancePair();
    FieldScene_PlaceAndPinSlots10And11();
    Engine_EventEnd();
}

void FieldScene_RunStepWith6(void)
{
    BattleFx_RunRisingObjectSequence(0, 6, 0);
}

void SceneActor_PassPointTwoRightOfActorZero(void)
{
    s32 pos[3];
    struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);

    pos[0] = actor->x.fixed + 0x200000;
    pos[1] = actor->y.fixed;
    pos[2] = actor->z.fixed;
    SceneActor_MoveActorZeroToTarget(pos);
}

void SceneState_ApplyTwoRectsAndRunThree(void)
{
    Engine_EventBegin();
    Map_CopyCellAttributeRect(89, 49, 3, 2, 25, 49);
    Map_CopyCellAttributeRect(89, 51, 8, 5, 25, 51);
    StagedActor_AdvancePair();
    BabiChika_RestorePlatformCells();
    Engine_EventEnd();
}

void SceneActor_CheckTwoUnitsAboveActorZero(void)
{
    struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
    s32 target[3];

    target[0] = actor->x.fixed;
    target[1] = actor->y.fixed;
    target[2] = actor->z.fixed + 0x00200000;
    if (SceneActor_MoveActorZeroToTarget(target)!= 0) {
        SceneState_ApplyTwoRectsAndRunThree();
    }
}

void SceneActor_MirrorFlag201IntoSlot14(void)
{
    u8 *flags;
    u8 value;

    Engine_GameFlagSet(0x200);
    if (Engine_GameFlagIsSet(0x201) != 0) {
        Object_GetById(14)->rise_counter = 0;
        Object_GetById(14)->collision_flags &= (u8)0xf7;
    } else {
        Object_GetById(14)->rise_counter = 1;
        flags = Object_GetById(14);
        flags += 89;
        value = 8;
        value |= *flags;
        *flags = value;
    }
}

void BabiChika_ConfigureActorFourteenRise(void)
{
    u8 *p;
    u8 val;

    Engine_GameFlagSet(0x201);
    if (Engine_GameFlagIsSet(0x200) != 0) {
        Object_GetById(14)->rise_counter = 0;
        Object_GetById(14)->collision_flags &= (u8)0xf7;
    } else {
        Object_GetById(14)->rise_counter = 1;
        p = Object_GetById(14);
        p += 89;
        val = 8;
        val |= *p;
        *p = val;
    }
}

void SceneState_ApplyFlag970(void)
{
    Engine_GameFlagSet(0x970);
}

void SceneState_RunUnlessActorZeroAtTile32x50(void)
{
    struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);

    if ((actor->x.fixed >> 20) != 32 || (actor->z.fixed >> 20) != 50) {
        SceneActor_ApplyPointLeftOfActorZero();
    }
}

void SceneState_RunUnlessActorZeroAt30_52(void)
{
    struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);

    if ((actor->x.fixed >> 20) != 30 || (actor->z.fixed >> 20) != 52) {
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
    actor = Object_GetById(18);
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
            rec7 = Random_Next();
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
    Engine_ActorSetChildValue(20, 0);
    Object_SetModeById(20, 2);
    flags = &Object_GetById(20)->priority_flags;
    *flags &= 0xFD;
    Engine_GameFlagSet(0x200);
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
    actor = Object_GetById(19);
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

void BabiChika_InstallActorNineteenFlicker(void)
{
    Engine_ActorEnableActionCallback(19, BabiChika_FlickerScript);
}

void SceneState_SetValue202ThenCall(void)
{
    Engine_GameFlagSet(0x202);
    FieldScene_RunSupplementalSequenceOne();
}

void SceneActor_ConfigureSlot21AndSetFlag201(void)
{
    u8 *flags;

    Object_SetModeById(21, 1);
    Engine_ActorSetChildValue(21, 0);
    Object_SetModeById(21, 2);
    flags = &Object_GetById(21)->priority_flags;
    *flags &= 0xFD;
    Engine_GameFlagSet(0x201);
}

void SceneDialogue_RunFlag982Or983Dialogue(void)
{
    Engine_EventBegin();
    Object_SetModeById(ACTOR_PARTY_LEADER, 1);
    if (Engine_GameFlagIsSet(0x982) != 0 || Engine_GameFlagIsSet(0x983) != 0) {
        Engine_MessageShowCentered(MSGNO(MsgBabiChikaStatueSpeaksAfterAnswer), 1);
    } else {
        Engine_MessageShowCentered(MSGNO(MsgBabiChikaStatueSpeaksRobinSoulYe), 1);
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

    struct FieldActor *owner;

    Engine_ActorEnableActionCallback(8, BabiChika_FlickerScript);
    Engine_GameFlagSet(0x203);
    owner = Object_GetById(9);
    owner->update = (void (*)(union FieldObject *))SceneActor_CopyActor8PositionWhenAtRow10;
}

void SceneActor_SetupSlotNineAndInstallHandler(void)
{
    typedef s32(*Handler_02001a10)(struct Record_02000ec8 *record);

    u8 *desc;
    s32 col;
    s32 row;

    Engine_EventBegin();
    Engine_ActorSetSpritePriority(9, 1);
    Object_SetModeById(9, 1);
    Engine_ActorSetChildValue(9, 0);
    Object_SetModeById(9, 2);

    {
        u8 *flag = &Object_GetById(9)->priority_flags;
        *flag &= (u8)0xfd;
    }

    Engine_GameFlagSet(0x204);

    col = Object_GetById(9)->x.fixed;
    row = Object_GetById(9)->z.fixed >> 20;
    Map_CopyCellAttributeRect(26, 8, 1, 1, col >> 20, row);

    desc = Object_GetById(9);
    *(Handler_02001a10 *)(desc + 108) = SceneActor_SetFlagBitByRelativeDepth;

    desc = Object_GetById(8);
    *(Handler_02001a10 *)(desc + 108) = SceneActor_SetFlagBitByRelativeDepth;

    ((u8* (*)())Engine_EventEnd)(desc);
}

s32 OverlayObject_SetYAboveLinkedActor(u8 *owner)
{
    s16 *id = (s16 *)(owner + 100);
    struct FieldActor *actor = Object_GetById(*id);

    *(s32 *)(owner + 12) = actor->y.fixed + 0x100000;
    return 0;
}

void BabiChika_SettleSteps(s32 wait)
{
    u32 i;

    Actor_SetSpeed(10, 0x8000, 0x4000);
    Actor_SetSpeed(11, 0x8000, 0x4000);
    if (wait != 0) {
        Audio_PlayCue(180);
    }
    Object_SetPosition(Object_GetById(10), Object_GetById(10)->x.fixed,
                             Data_0200b350[(s16)Object_GetById(10)->timer], Object_GetById(10)->z.fixed);
    Object_SetPosition(Object_GetById(11), Object_GetById(11)->x.fixed,
                             Data_0200b350[(s16)Object_GetById(11)->timer], Object_GetById(11)->z.fixed);
    ObjectMotion_CommitCurrentPositionAndActivate(10);
    ObjectMotion_CommitCurrentPositionAndActivate(11);
    Object_GetById(10)->y.fixed = Data_0200b350[(s16)Object_GetById(10)->timer];
    Object_GetById(11)->y.fixed = Data_0200b350[(s16)Object_GetById(11)->timer];
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

void SceneActor_LandOnHighestPlatform(s32 subject)
{
    s32 best = -0x500000;
    u32 i;

    for (i = 0; i <= 4; i++) {
        s32 slot = i + 10;

        if (slot == subject) continue;

        if ((Object_GetById(slot)->x.fixed >> 20) != (Object_GetById(subject)->x.fixed >> 20)) continue;
        if ((Object_GetById(slot)->z.fixed >> 20) != (Object_GetById(subject)->z.fixed >> 20)) continue;

        if (best > Object_GetById(slot)->y.fixed + 0x100000) continue;

        best = Object_GetById(slot)->y.fixed + 0x100000;
        Object_GetById(subject)->timer = (u16)slot;
    }

    ObjectMotion_SetSpeedParameters(subject, 0x40000, 0x20000);

    {
        struct FieldActor *target = Object_GetById(subject);
        struct FieldActor *from = Object_GetById(subject);
        s32 z = Object_GetById(subject)->z.fixed;

        Object_SetPosition(target, from->x.fixed, best, z);
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
        if (Object_GetById(i + 12)->sprite->priority == 3
            && GameFlag_IsSet(i + 0x200) == 0) {
            Actor_Get(i + 12);
            SceneActor_WaitValueBelowLimit();
            Actor_SetPosition(i + 12, 0, 0);
            GameFlag_Set(i + 0x200);
            break;
        }
        if ((Object_GetById(i + 12)->z.fixed >> 20) == 9
            && GameFlag_IsSet(i + 0x200) == 0) {
            Object_GetById(i + 12)->terrain_height = 0;
            Object_GetById(i + 12)->velocity_y = 0;
            Object_GetById(i + 12)->target_y = ACTOR_NO_TARGET;
            Actor_Get(i + 12)->motion_flags = 0;
            Object_GetById(i + 12)->timer = 0;
            found = i;
            for (j = 0; j < i; j++) {
                if (GameFlag_IsSet(0x200 + j) == 0) {
                    saved.x.fixed = Object_GetById(i + 12)->x.fixed;
                    saved.y.fixed = Object_GetById(i + 12)->y.fixed;
                    saved.z.fixed = Object_GetById(i + 12)->z.fixed;
                    Actor_Get(i + 12)->x.fixed =
                        Object_GetById(j + 12)->x.fixed;
                    Object_GetById(i + 12)->y.fixed =
                        Object_GetById(j + 12)->y.fixed;
                    Object_GetById(i + 12)->z.fixed =
                        Object_GetById(j + 12)->z.fixed;
                    Actor_Get(j + 12)->x.fixed = saved.x.fixed;
                    Actor_Get(j + 12)->y.fixed = saved.y.fixed;
                    Actor_Get(j + 12)->z.fixed = saved.z.fixed;
                    found = j;
                    break;
                }
            }
            Object_GetById(found + 12)->terrain_height = 0;
            Actor_Get(found + 12)->velocity_y = 0;
            Object_GetById(found + 12)->target_y = ACTOR_NO_TARGET;
            Actor_Get(found + 12)->motion_flags = 0;
            Object_GetById(found + 12)->timer = 0;
            Engine_CameraSetSpeed(0x30000, 0x6000);
            ((struct FieldActor *)Battle_GetWorkObject1e0())->motion_flags = 0;
            Camera_MoveTo(0xa80000, 0x80000, 0xb80000, 1);
            Engine_CameraWaitForMove();
            SceneActor_LandOnHighestPlatform(found + 12);
            if ((Object_GetById(found + 12)->x.fixed >> 20) == 8) {
                Object_GetById(10)->timer++;
                (*(s16 *)&Object_GetById(11)->timer)--;
            } else {
                (*(s16 *)&Object_GetById(10)->timer)--;
                Object_GetById(11)->timer++;
            }
            Actor_Get(found + 12)->update = (void (*)(union FieldObject *))OverlayObject_SetYAboveLinkedActor;
            BabiChika_SettleSteps(40);
            Actor_Get(found + 12)->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
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

    if ((Object_GetById(17)->x.fixed >> 20) == 45) {
        Engine_GameFlagSet(0x974);
    } else {
        Engine_GameFlagClear(0x974);
    }

    if ((Object_GetById(18)->x.fixed >> 20) == 46) {
        Engine_GameFlagSet(0x975);
    } else {
        Engine_GameFlagClear(0x975);
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

void FieldScene_RunLateSequenceHead(void)
{
    Map_CopyCellAttributes(72, 49, 1, 1, 8, 49);
    Map_CopyCellAttributes(113, 43, 1, 1, 49, 43);
    MapObject_SetPosition(100, 0, 0);
    MapObject_SetPosition(101, 0, 0);
    Actor_SetPosition(15, 8912896, 51904512);
    Actor_SetPosition(16, 51904512, 45613056);
}

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
    struct FieldActor *record;
    s32 v6;
    s32 p5;
    s32 q;

    v6 = 0;
    Engine_EventBegin();
    Map_CopyCellAttributes(83, 45, 11, 8, 19, 45);
    record = Object_GetById(19);
    p5 = record->x.fixed;
    q = Object_GetById(19)->z.fixed;
    q >>= 20;
    p5 >>= 20;
    Map_CopyCellAttributes(20, 56, 1, 1, p5, q);
    record = Object_GetById(20);
    p5 = record->x.fixed;
    q = Object_GetById(20)->z.fixed;
    q >>= 20;
    p5 >>= 20;
    Map_CopyCellAttributes(20, 56, 1, 1, p5, q);
    record = Object_GetById(21);
    p5 = record->x.fixed;
    q = Object_GetById(21)->z.fixed;
    q >>= 20;
    p5 >>= 20;
    Map_CopyCellAttributes(20, 56, 1, 1, p5, q);
    record = Object_GetById(22);
    p5 = record->x.fixed;
    q = Object_GetById(22)->z.fixed;
    q >>= 20;
    p5 >>= 20;
    Map_CopyCellAttributes(20, 56, 1, 1, p5, q);
    record = Object_GetById(23);
    p5 = record->x.fixed;
    q = Object_GetById(23)->z.fixed;
    q >>= 20;
    p5 >>= 20;
    Map_CopyCellAttributes(20, 56, 1, 1, p5, q);
    record = Object_GetById(19);
    if ((record->x.fixed >> 20) == 25) {
        record = Object_GetById(19);
        if ((record->z.fixed >> 20) == 49) {
            v6 = 1;
        }
    }
    record = Object_GetById(20);
    if ((record->x.fixed >> 20) == 23) {
        record = Object_GetById(20);
        if ((record->z.fixed >> 20) == 49) {
            v6 = (v6 + 1);
        }
    }
    record = Object_GetById(21);
    if ((record->x.fixed >> 20) == 25) {
        record = Object_GetById(21);
        if ((record->z.fixed >> 20) == 47) {
            v6 = (v6 + 1);
        }
    }
    record = Object_GetById(22);
    if ((record->x.fixed >> 20) == 23) {
        record = Object_GetById(22);
        if ((record->z.fixed >> 20) == 47) {
            v6 = (v6 + 1);
        }
    }
    record = Object_GetById(23);
    if ((record->x.fixed >> 20) == 24) {
        record = Object_GetById(23);
        if ((record->z.fixed >> 20) == 48) {
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

        Map_CopyCellAttributeRect(83, 45, 11, 8, width, height);
    }
    StagedActor_AdvancePair();
    FieldScene_RunScene3c4SequenceA();
    Engine_EventEnd();
}

void SceneState_SetValue268bInScene(void)
{
    Engine_EventBegin();
    Object_SetModeById(ACTOR_PARTY_LEADER, 1);
    Engine_MessageShowCentered(MSGNO(MsgBabiChikaStatueSeemsSpeakYourSoul), 1);
    Engine_EventEnd();
}

void FieldScene_RunScriptedStep953(void)
{
    Engine_EventBegin();
    Engine_MessageShowCentered(MSGNO(MsgFieldDoorTightlyLocked), 1);
    Engine_EventEnd();
}

s32 BabiChika_SelectEvents(void)
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
        Map_CopyCellAttributeRect(73, 38, 5, 5, p5, p6);
    }
    SceneState_SwapSlotPairByRank(9, 8);

    {
        s32 col = Object_GetById(8)->x.fixed >> 20;
        row = Object_GetById(8)->z.fixed >> 20;
        Map_CopyCellAttributeRect(2, 36, 1, 1, col, row);
    }

    {
        s32 col = Object_GetById(9)->x.fixed >> 20;
        row = Object_GetById(9)->z.fixed >> 20;
        Map_CopyCellAttributeRect(2, 36, 1, 1, col, row);
    }
}

void FieldScene_PlaceAndPinSlots10And11(void)
{
    s32 row;

    {
        s32 k5 = 29, k6 = 30;
        Map_CopyCellAttributeRect(93, 30, 6, 5, k5, k6);
    }
    SceneState_SwapSlotPairByRank(11, 10);

    {
        s32 col20 = Object_GetById(10)->x.fixed >> 20;
        row = Object_GetById(10)->z.fixed >> 20;
        Map_CopyCellAttributeRect(2, 36, 1, 1, col20, row);
    }

    {
        s32 col20 = Object_GetById(11)->x.fixed >> 20;
        row = Object_GetById(11)->z.fixed >> 20;
        Map_CopyCellAttributeRect(2, 36, 1, 1, col20, row);
    }
}

void BabiChika_RestorePlatformCells(void)
{
    struct FieldActor *record;
    s32 p5;

    Map_CopyCellAttributes(89, 49, 3, 2, 25, 49);
    Map_CopyCellAttributes(89, 51, 8, 5, 25, 51);
    Object_GetById(14)->terrain_id = 1;
    record = Object_GetById(12);
    p5 = record->x.fixed;
    record = Object_GetById(12);
    p5 = p5 >> 20;
    Map_CopyCellAttributes(22, 52, 1, 1, p5, (record->z.fixed >> 20));
    record = Object_GetById(13);
    p5 = record->x.fixed;
    record = Object_GetById(13);
    p5 = p5 >> 20;
    Map_CopyCellAttributes(22, 52, 1, 1, p5, (record->z.fixed >> 20));
    record = Object_GetById(14);
    p5 = record->x.fixed;
    record = Object_GetById(14);
    p5 = p5 >> 20;
    Map_CopyCellAttributes(22, 52, 1, 1, p5, (record->z.fixed >> 20));
}

void BabiChika_MarkActorCells(void)
{
    Map_CopyCellAttributes(108, 19, 4, 1, 44, 19);
    StagedActor_FillGridAttributeRectangle(0, Object_GetById(17)->x.fixed >> 20, Object_GetById(17)->z.fixed >> 20, 1, 1, 0xff);
    StagedActor_FillGridAttributeRectangle(0, Object_GetById(18)->x.fixed >> 20, Object_GetById(18)->z.fixed >> 20, 1, 1, 0xff);
}

s32 FieldScene_InitializeActorGroups(void)
{
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
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
                Map_CopyCellsTo(121, 4, 74, 9, 5, 8);
                Map_CopyCellsTo(18, 83, 9, 73, 3, 2);
                Engine_MapCopyCellsTo(18, 81, 9, 75, 3, 2);
                Engine_MapCopyCellsTo(18, 83, 9, 77, 3, 2);
                Engine_MapCopyCellsTo(18, 83, 9, 79, 3, 2);
                Engine_MapCopyCellsTo(18, 83, 11, 78, 3, 2);
                Engine_MapCopyCellsTo(18, 83, 13, 79, 3, 2);
            } else {
                if (!Engine_GameFlagIsSet(0x983))
                    break;
                Map_CopyCellsTo(121, 13, 74, 9, 5, 8);
                Map_CopyCellsTo(18, 85, 11, 74, 3, 2);
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
            Engine_TaskAddCallback(BabiChika_UpdateTrackedActor, 0xc80);
            break;
        case 5:
        case 6:
        case 7:
            if (Engine_GameFlagIsSet(0x982))
                Map_CopyCells(23, 17, 1, 2, 30, 8);
            if (Engine_GameFlagIsSet(0x983))
                Map_CopyCells(23, 17, 1, 2, 32, 10);
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
            Engine_TaskAddCallback(BabiChika_UpdateTrackedActor, 0xc80);
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
                Map_CopyCellsTo(59, 8, 49, 8, 1, 3);
                Map_CopyCellAttributeRect(51, 8, 1, 1, 49, 8);
                Object_GetById(18)->priority_flags |= 2;
                Object_SetModeById(18, 3);
                Map_CopyCellAttributeRect(45, 4, 1, 1, 46, 8);
                Actor_SetPosition(18, 186 << 18, 136 << 16);
                Object_GetById(18)->y.fixed = -0x100000;
                Actor_SetPosition(20, 186 << 18, 136 << 16);
            }
            if (Engine_GameFlagIsSet(0x200)) {
                Engine_ActorSetChildValue(20, 0);
                Object_SetModeById(20, 5);
            }
            if (GameFlag_IsSet(0x202))
                Object_SetModeById(19, 2);
            if (Engine_GameFlagIsSet(0x972)) {
                Map_CopyCellsTo(59, 8, 45, 14, 1, 3);
                Map_CopyCellAttributeRect(51, 8, 1, 1, 45, 14);
                Object_GetById(19)->priority_flags |= 2;
                Object_SetModeById(19, 3);
                Map_CopyCellAttributeRect(45, 4, 1, 1, 48, 14);
                Actor_SetPosition(19, 194 << 18, 232 << 16);
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
            BabiChika_RestorePlatformCells();
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
            Engine_TaskAddCallback(BabiChika_UpdateTrackedActor, 0xc80);
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
                Actor_SetPosition(17, 182 << 18, 156 << 17);
            if (Engine_GameFlagIsSet(0x975))
                Actor_SetPosition(18, 186 << 18, 156 << 17);
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
                Object_GetById(10)->timer = 3;
                Object_GetById(11)->timer = 3;
                Engine_ActorSetSpritePriority(10, 1);
                Engine_ActorSetSpritePriority(11, 1);
                Object_GetById(12)->motion_flags = 0;
                Object_GetById(13)->motion_flags = 0;
                Object_GetById(14)->motion_flags = 0;
                Engine_ActorSetSpriteFlags(Object_GetById(12), 0);
                Engine_ActorSetSpriteFlags(Object_GetById(13), 0);
                Engine_ActorSetSpriteFlags(Object_GetById(14), 0);
                Object_GetById(12)->timer = 0;
                Object_GetById(13)->timer = 0;
                Object_GetById(14)->timer = 0;
                if (gGameState.entrance != 5)
                    break;
                Object_GetById(10)->y.fixed = -0x200000;
                Object_GetById(11)->y.fixed = -0x400000;
                Object_GetById(10)->timer = 2;
                Object_GetById(11)->timer = 4;
                Actor_SetPosition(12, 200 << 16, 152 << 16);
                Object_GetById(12)->timer = 11;
                Object_GetById(12)->update = ACTOR_UPDATE_PANEL;
                Object_GetById(12)->priority_flags |= 2;
                Actor_SetPosition(13, 200 << 16, 152 << 16);
                Object_GetById(13)->timer = 12;
                Object_GetById(13)->update = ACTOR_UPDATE_PANEL;
                Object_GetById(13)->priority_flags |= 2;
                Engine_ActorSetPosition(14, 136 << 16, 152 << 16);
                Object_GetById(14)->timer = 10;
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
                Map_CopyCells(10, 30, 1, 2, 16, 30);
            if (Engine_GameFlagIsSet(0x983))
                Map_CopyCells(10, 30, 1, 2, 22, 30);
            Engine_GameFlagSet(0x973);
            break;
        case 12:
            Map_CopyCellAttributes(8, 49, 1, 1, 8, 113);
            FieldScene_RunLateSequenceHead();
            Engine_TaskAddCallback(BabiChika_UpdateTrackedActor, 0xc80);
            break;
        case 13:
        case 14:
            Battle_WaitMode0(1);
            if (Engine_GameFlagIsSet(0x984)) {
                Map_CopyCells(24, 59, 1, 2, 32, 46);
                Actor_SetPosition(19, 204 << 17, 198 << 18);
                Actor_SetPosition(20, 188 << 17, 198 << 18);
                Actor_SetPosition(21, 204 << 17, 190 << 18);
                Actor_SetPosition(22, 188 << 17, 190 << 18);
                Actor_SetPosition(23, 196 << 17, 194 << 18);
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
            Map_CopyCellAttributes(49, 43, 1, 1, 49, 107);
            FieldScene_RunLateSequenceHead();
            break;
        }
    }
    return 0;
}
