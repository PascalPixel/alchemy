#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "EDITION.H"
#include "INVENTORY.H"

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "CALL.H"
#include "text/MSG_IDS.H"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

struct EffectRecord {
    u8 pad[9];
    u8 flags_lo : 2;
    u8 mode : 2;
    u8 flags_hi : 4;
};

union MotionWork {
  struct {
    u32 unk_00[2];
    s32 x, y, z;
    u32 unk_14;
    s32 accum_x, accum_y;
    u32 unk_20[4];
    s32 rate_x, rate_y;
    u32 unk_38[3];
    s32 velocity_x, velocity_y, velocity_z;
    u16 *record;
    u8 unk_54[16];
    u16 angle_step;
  } fields;
  u8 bytes[102];
};

struct SceneActor {
    u8 unk_00[6];
    u16 facing;
    u8 unk_08[92];
    u16 state_flags;
};

struct SceneActor_02000350 {
    u8 unk_00[6];
    u16 facing;
    s32 x, y, z;
    u8 unk_14[71];
    u8 active;
};


s32 SceneActor_GetPositionDistance(s32 *, s32 *);
s32 ArcTan2(s32, s32);
void ActorPresentation_MoveActorToPositionAndWait();
void BattleFx_RunPageEffectForSlot(s32 actor, s32 mode, s32 value);

void ActorPresentation_SetupActorZeroForSceneTwelve(void)
{
    struct SceneActor_02000c1c *actor = Object_GetById(ACTOR_PARTY_LEADER);
    struct Presentation_02000c1c *presentation = actor->presentation;
    u8 flags;

    Engine_AudioPlayCue(158);
    Engine_MapAnimateCells(KuupuappuMuraSai_Scene12Cells, 35, 9);
    {
        s32 cell = 4;
        s32 row = 10;

        Engine_MapCopyCellAttributes(33, 20, 1, 3, cell, row);
    }
    actor->state_23 &= ~1;
    flags = presentation->flags;
    flags |= 12;
    presentation->flags = flags;
    SceneActor_PlaceAndSetSceneDelay(72, 160, 12);
}

void SceneState_SetFlag200AndConfigureRegion55_26(void)
{
    Engine_GameFlagSet(0x200);
    {
        s32 a = 23;
        s32 b = 26;
        Engine_MapCopyCellAttributes(55, 26, 4, 2, a, b);
    }
}

void ActorPresentation_SetFlag200AndSceneCell23(void)
{
    Engine_GameFlagClear(0x200);
    {
        s32 first_value = 23;
        s32 second_value = 26;
        Engine_MapCopyCellAttributes(23, 23, 4, 2, first_value, second_value);
    }
}

void FieldScene_SetActor21Values0And4(void)
{
    BattleFx_RunPageEffectForSlot(21, 0, 4);
}

void FieldScene_RunActor18MotionSequence(void)
{
    u32 i;
    s32 record;

    PartyInventory_Discard(231);
    Engine_EventBegin();
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(18, 2);
    Actor_SetSpeed(18, 0xcccc, 0x6666);
    Actor_WalkToAndWait(18, 216, 0x198);
    Engine_EventWait(10);
    Actor_FaceDirection(18, 0x4000, 20);
    Engine_ActorJump(18, 6, 0);
    Engine_EventWait(30);
    Engine_ActorJump(18, 6, 0);
    Engine_EventWait(30);
    Engine_ActorJump(18, 6, 0);
    Engine_EventWait(30);
    Actor_WalkToAndWait(18, 216, 0x188);
    Engine_EventWait(10);
    Actor_FaceDirection(18, 0x4000, 20);
    GameFlag_Set(0x858);
    Engine_EventEnd();
}

void ActorPresentation_SetPairedSceneCells(void)
{
    s32 v1 = 13;
    s32 v2 = 25;

    Engine_MapCopyCellAttributes(41, 43, 1, 1, v1, v2);
    Engine_MapCopyCells(40, 42, 12, 22, 3, 3);
}

void ActorPresentation_SetAlternatePairedSceneCells(void)
{
    s32 v1 = 13;
    s32 v2 = 25;

    Engine_MapCopyCellAttributes(37, 43, 1, 1, v1, v2);
    Engine_MapCopyCells(36, 42, 12, 22, 3, 3);
}

void FieldScene_RunActorEighteenDialogue(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgKuupuappuRuffRrruff2);
    Engine_ActorSetAnimation(18, 0);
    Engine_ActorFaceEachOther(18, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(2);
    Engine_EventShowMessage(18, 0);
    Engine_ActorSetAnimation(18, 1);

    if (PartyInventory_FindOwner(231) != -1 && Engine_GameFlagIsSet(0x858) == 0) {
        ((struct SceneWork_02000e90 *)gEventWork)->actor18_marker = 1;
    }

    Engine_EventEnd();
}

void KuupuappuMuraSai_InspectWell(void)
{
    Engine_EventBegin();
    Engine_MessageShowCentered((s32)MsgFieldPeeredWell, 1);
    Engine_MessageShowCentered((s32)MsgKuupuappuCanHearWaterRumblingDown, 1);
    Engine_EventEnd();
}

const u8 *SceneData_GetExtraTable(void)
{
    return KuupuappuMuraSai_Extras;
}

s32 SceneSetup_InitializeActorsAndFlags(void)
{

    struct FieldActor *actor;
    s16 *scene;
    s32 mode;

    if (Engine_GameFlagIsSet(0x200))
        Map_CopyCellAttributes(55, 26, 4, 2, 23, 26);
    OverlayObject_CreateConfiguredObjectB(0x800000, 0, 0x1a40000, 223);
    Engine_MapCopyCells(45, 41, 8, 45, 3, 3);
    Engine_TaskWait(1);
    actor = Object_GetById(14);
    actor->update = (void (*)(union FieldObject *))UpdateActorProximity;
    {
        struct FieldActor *actor = Object_GetById(14);
        s32 mode = 1;
        actor->timer = mode;
    }
    mode = 0;
    actor = Object_GetById(15);
    actor->update = (void (*)(union FieldObject *))UpdateActorProximity;
    Object_GetById(15)->timer = mode;
    if (Engine_GameFlagIsSet(0x858))
        Actor_SetPosition(18, 0xd80000, 0x1880000);
    if (gGameState.entrance <= 2 && !Engine_GameFlagIsSet(52)
#if EDITION_INTERNATIONAL
        && !Engine_GameFlagIsSet(0x109)
#endif
       )
        Engine_GameFlagClear(0x867);
    if (Engine_GameFlagIsSet(0x867) && !Engine_GameFlagIsSet(52))
        Actor_SetPosition(21, 0x1980000, 0x780000);
    scene = (s16 *)&gGameState;
    if (scene[225] == 11)
        Engine_GameFlagClear(FLAG_ARRIVAL_EVENT_PENDING);
    if (scene[225] == 13)
        Engine_GameFlagClear(0x120);
    return 0;
}
