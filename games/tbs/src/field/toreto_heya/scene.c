#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "resource.h"
#include "global_cells.h"
#include "heya.h"
#include "iwram_call.h"
#include "dma.h"
#include "types.h"
#include "field_event.h"
#include "field_service.h"
#include "io_reg.h"
#include "text/msg_ids.h"
#include "heap_state.h"

#define Object_ReplaceResourceEntry ((s32 (*)(struct FieldSprite*,s32))Object_ReplaceResourceEntry)


void Event_SetDestination(s32 scene, s32 entrance);



void Party_SetFields1ceAnd1d0(s32 scene, s32 entrance);
void BattleFx_SetWeightedResult(s32 value, s32 mode);



extern struct BattleEffectBuffers *gPaletteBufferWork;



void Vector_AddPolarOffset();

struct Vec {
    s32 x;
    s32 y;
    s32 z;
};

struct Swirl {
    u8 pad00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 pad14[4];
    s32 scaleX;
    s32 scaleY;
    u8 pad20[24];
    s32 ox;
    s32 oy;
    s32 oz;
    u8 pad44[12];
    u8 *sprite;
    u8 pad54[16];
    s16 timer;
    s16 angle;
};


struct Spark {
    u8 unknown_00[0x64];
    u16 phase;
    u16 angle;
};

struct Vec3 {
    s32 x;
    s32 y;
    s32 z;
};


void Effect_Spawn(s32 x, s32 y, s32 z, s32 dx, s32 dy, s32 dz, s32 lift, void *params);

struct EffectParams {
    s32 count;
    s32 kind;
    s32 spread;
    s32 rise;
    u8 pad10[20];
    s32 script;
};


const struct SceneEntrance *Scene_GetEntrances(void)
{
    return gToretoHeyaEntrances;
}

const struct SceneRegion *Scene_GetRegions(void)
{
    return gToretoHeyaRegions;
}

const u32 *Scene_GetExits(void)
{
    return gToretoHeyaExits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
    return gToretoHeyaPlacements;
}

void ToretoHeya_HandleFloorSwitch(s32 flag, s32 src_x, s32 src_y, s32 entrance)
{
    s32 dest_x;
    s32 dest_y;
    s32 leader;
    struct FieldActor *actor;
    s32 cnt;

    dest_x = (gGameState.x >> 20) + 64;
    dest_y = gGameState.z >> 20;
    leader = gGameState.selected_actor;
    actor = Object_GetById(leader);
    if (entrance == *ToretoHeya_PaletteBuffer)
        return;
    *ToretoHeya_PaletteBuffer = entrance;
    if (Engine_GameFlagIsSet(flag) == 0) {
        Engine_MapCopyCells(src_x, src_y, 1, 1, dest_x, dest_y);
        Engine_GameFlagSet(flag);
        return;
    }
    *ToretoHeya_PaletteBuffer = -1;
    Engine_MapCopyCells(src_x, src_y + 1, 1, 1, dest_x, dest_y);
    Engine_AudioPlayCue(206);
    Engine_EventBegin();
    Event_SetDestination((s32)&SceneId_ToretoHeya, entrance);
    Engine_ActorSetAnimation(leader, 27);
    Engine_ActorSetSpriteFlags(Object_GetById(leader), 0);
    Actor_SetAttachedEffect(leader, 0x101);
    Engine_EventWait(30);
    Camera_MoveTo(-1, -1, -1, 0);
    actor->motion_flags = 2;
    actor->terrain_height = -0xa00000;
    actor->gravity = 0x8000;
    Engine_AudioPlayCue(204);
    Engine_EventWait(3);
    actor->terrain_id = 2;
    Engine_ActorSetSpritePriority(leader, 3);
    for (cnt = 29; cnt >= 0; cnt--) {

        actor->facing += 0x2000;
        Engine_TaskWait(1);
    }
    if (entrance != 50)
        Engine_GameFlagSet(0x122);
}

void FieldScene_RunStep200(void) { ToretoHeya_HandleFloorSwitch(0x200, 64, 35, 21); }

void FieldScene_RunStep201(void) { ToretoHeya_HandleFloorSwitch(0x201, 65, 35, 22); }

void FieldScene_RunStep202(void) { ToretoHeya_HandleFloorSwitch(0x202, 66, 35, 23); }

void FieldScene_RunStep203(void) { ToretoHeya_HandleFloorSwitch(0x203, 67, 35, 24); }

void FieldScene_RunStep204(void) { ToretoHeya_HandleFloorSwitch(0x204, 68, 35, 25); }

void FieldScene_RunStep205(void) { ToretoHeya_HandleFloorSwitch(0x205, 69, 35, 26); }

void FieldScene_RunStep206(void) { ToretoHeya_HandleFloorSwitch(0x206, 70, 35, 27); }

void FieldScene_RunStep207(void) { ToretoHeya_HandleFloorSwitch(0x207, 71, 35, 28); }

void FieldScene_RunStep208(void) { ToretoHeya_HandleFloorSwitch(0x208, 72, 35, 29); }

void FieldScene_RunStep209(void) { ToretoHeya_HandleFloorSwitch(0x209, 73, 35, 31); }

void FieldScene_RunStep20a(void) { ToretoHeya_HandleFloorSwitch(0x20a, 74, 35, 32); }

void FieldScene_RunStep20b(void) { ToretoHeya_HandleFloorSwitch(0x20b, 79, 35, 50); }

void FieldScene_RunStep20c(void) { ToretoHeya_HandleFloorSwitch(0x20c, 75, 35, 51); }

void FieldScene_RunStep20d(void) { ToretoHeya_HandleFloorSwitch(0x20d, 76, 35, 52); }

void FieldScene_RunStep20e(void) { ToretoHeya_HandleFloorSwitch(0x20e, 77, 35, 53); }

void FieldScene_RunStep20f(void) { ToretoHeya_HandleFloorSwitch(0x20f, 78, 35, 54); }

void FieldScene_RunStep210(void) { ToretoHeya_HandleFloorSwitch(0x210, 80, 35, 55); }

void FieldScene_RunStep211(void) { ToretoHeya_HandleFloorSwitch(0x211, 81, 35, 56); }

void FieldScene_RunStep212(void) { ToretoHeya_HandleFloorSwitch(0x212, 82, 35, 57); }

void FieldScene_RunStep213(void) { ToretoHeya_HandleFloorSwitch(0x213, 83, 35, 58); }

void FieldScene_RunStep214(void) { ToretoHeya_HandleFloorSwitch(0x214, 84, 35, 59); }

void SceneState_ClearStoryVariantWhenIdle(void)
{
    if (Engine_LeaderCheckAhead() == 0)
        *ToretoHeya_PaletteBuffer = -1;
}

const struct SceneEvent *Scene_GetEvents(void)
{
    return gToretoHeyaEvents;
}

void ToretoHeya_RunTableScene(void)
{
    u32 i;
    s32 rec8;
    struct FieldActor *leader;

    rec8 = Engine_GameFlagIsSet(3);
    Engine_EventBegin();
    Engine_AudioPlayCue(17);
    Engine_EventSetMessage((s32)MsgToretoHmHrooom);
    Engine_EventShowMessageAndWait(0x8009, 0, 20);
    Engine_AudioPlayCue(29);
    Actor_SetSpeed(0, 0x10000, 0x8000);
    Actor_SetSpeed(1, 0x10000, 0x8000);
    Actor_SetSpeed(2, 0x10000, 0x8000);
    Engine_ActorSetSpeed(3, 0x10000, 0x8000);
    Object_GetById(3)->priority_flags &= 254;
    Engine_ActorSetSpritePriority(3, 2);
    Object_GetById(0)->priority_flags &= 254;
    Engine_ActorSetSpritePriority(0, 2);
    leader = Object_GetById(0);
    if (leader != NULL) {
        Engine_ActorSetPosition(1, leader->x.fixed, leader->z.fixed);
    }
    leader = Object_GetById(0);
    if (leader != NULL) {
        Engine_ActorSetPosition(2, leader->x.fixed, leader->z.fixed);
    }
    if (rec8 != 0) {
        leader = Object_GetById(0);
        if (leader != NULL) {
            Engine_ActorSetPosition(3, leader->x.fixed, leader->z.fixed);
        }
        Engine_ActorEnableActionCallback(3, ToretoHeya_TableActions3);
    }
    ((s32 (*)())Engine_ActorEnableActionCallback)(0, (s32)ToretoHeya_TableActions0);
    ((s32 (*)())Engine_ActorEnableActionCallback)(1, (s32)ToretoHeya_TableActions1);
    Object_SetActionCallbackAndRefreshById(2, (s32)ToretoHeya_TableActions2);
    Engine_EventWait(10);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(1, 0xc000, 0);
    Actor_FaceDirection(3, 0xc000, 0);
    Actor_FaceDirection(2, 0xc000, 40);
    Engine_ActorSetAnimation(8, 11);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(8, 8);
    Engine_EventWait(20);
    ToretoHeya_PlayGesture(8);
    Event_ShowMessage(0x8008, 0);
    Engine_ActorStartRepeatedMotion(0, 2);
    Engine_ActorStartRepeatedMotion(1, 2);
    Engine_ActorStartRepeatedMotion(3, 2);
    Engine_ActorStartRepeatedMotion(2, 2);
    Actor_ShowEmote(0, 0x100, 0);
    Actor_ShowEmote(1, 0x100, 0);
    Actor_ShowEmote(3, 0x100, 0);
    Engine_ActorShowEmote(2, 0x100, 60);
    ToretoHeya_PlayGesture(11);
    Event_ShowMessageAndWait(0x8008, 0, 10);
    Engine_ActorStartRepeatedMotion(0, 1);
    Engine_ActorStartRepeatedMotion(1, 1);
    Engine_ActorStartRepeatedMotion(3, 1);
    Engine_ActorRunRepeatedMotion(2, 1);
    Event_ShowMessage(0x8008, 0);
    Actor_SetAttachedEffect(0, 0x102);
    Actor_SetAttachedEffect(1, 0x102);
    Actor_SetAttachedEffect(3, 0x102);
    Engine_ActorSetAttachedEffect(2, 0x102);
    Engine_EventWait(40);
    ToretoHeya_PlayGesture(11);
    Engine_EventShowMessage(0x8008, 0);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    gEventWork->transition_frames = 64;
    Battle_SetStart(3);
    Party_SetFields1ceAnd1d0((s32)&SceneId_ToretoHeya, 19);
    BattleFx_SetWeightedResult(36, 0);
    Engine_EventEnd();
}

void FieldScene_RunFourActorEncounter(void)
{
    u32 i;
    s32 rec;
    struct FieldActor *record;
    s32 v6;
    s32 v5;
    s32 base5_200962d;
    s32 base5_2009ec8;

    rec = GameFlag_IsSet(3);
    Object_GetById(3)->priority_flags &= 254;
    Engine_ActorSetSpritePriority(MEARI, 2);
    Object_GetById(0)->priority_flags &= 254;
    Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
    PartyInventory_FindOwner(184);
    Event_PlayCue(17);
    Engine_EventBegin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_SetSpeed(GERALD, 0xcccc, 0x6666);
    Actor_SetSpeed(IWAN, 0xcccc, 0x6666);
    Actor_SetSpeed(MEARI, 0xcccc, 0x6666);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0xa60000, 0x500000);
    v6 = 192;
    record = Actor_Get(ACTOR_PARTY_LEADER);
    record->facing = v6 << 8;
    Actor_SetPosition(GERALD, 0x940000, 0x5a0000);
    record = Actor_Get(GERALD);
    record->facing = v6 << 8;
    Actor_SetPosition(IWAN, 0xb60000, 0x5a0000);
    record = Object_GetById(IWAN);
    record->facing = v6 << 8;
    if (rec != 0) {
        Actor_SetPosition(MEARI, 0xa60000, 0x680000);
        record = Object_GetById(MEARI);
        record->facing = v6 << 8;
    }
    ToretoHeya_PlayGesture(0);
    Engine_TaskWait(10);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    gEventWork->transition_frames = 48;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    Camera_SetSpeed(0x13333, 0x2666);
    Camera_MoveTo(0xa80000, -1, 0x980000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(10);
    v5 = 10;
    Event_PlayCue(123);
    Map_CopyCellAttributes(26, 3, 1, 2, v5, 8);
    Map_CopyCells(26, 38, 1, 1, v5, 43);
    Engine_TaskWait(4);
    Map_CopyCells(26, 37, 1, 2, v5, 42);
    Engine_TaskWait(4);
    Map_CopyCells(26, 36, 1, 3, v5, 41);
    Engine_TaskWait(4);
    Map_CopyCells(26, 35, 1, 4, v5, 40);
    Engine_TaskWait(80);
    Engine_EventSetMessage((s32)MsgToretoMmmmm);
    Event_ShowMessageAndWait(0x8009, 0, 20);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_ActorStartRepeatedMotion(GERALD, 2);
    Engine_ActorStartRepeatedMotion(MEARI, 2);
    Engine_ActorRunRepeatedMotion(IWAN, 2);
    Engine_EventWait(20);
    Camera_MoveTo(0xa80000, -1, 0x5a0000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(40);
    ToretoHeya_PlayGesture(1);
    Engine_EventWait(60);
    Event_PlayCue(21);
    ToretoHeya_PlayGesture(4);
    Event_ShowMessageAndWait(0x8009, 0, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(GERALD, 0x101, 0);
    Actor_ShowEmote(MEARI, 0x101, 0);
    Actor_ShowEmote(IWAN, 0x101, 80);
    Event_ShowMessage(0x8009, 0);
    Engine_EventWait(40);
    Event_ShowMessageAndWait(0x8009, 0, 20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(GERALD, 3);
    Engine_ActorSetAnimation(MEARI, 3);
    Engine_ActorSetAnimationAndWait(IWAN, 3);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(0x8009, 0, 20);
    ToretoHeya_PlayGesture(0);
    Engine_EventWait(40);
    Event_ShowMessageAndWait(0x8009, 0, 20);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(GERALD, 0x102);
    Actor_SetAttachedEffect(MEARI, 0x102);
    Actor_SetAttachedEffect(IWAN, 0x102);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(GERALD, 2);
    Actor_FaceDirection(GERALD, 0xe000, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 10);
    Event_ShowMessageAndWait(0x8001, 0, 10);
    Engine_ActorSetAnimationAndWait(IWAN, 4);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
    Actor_FaceDirection(IWAN, 0xa000, 0);
    Event_ShowMessageAndWait(0x8002, 0, 20);
    ToretoHeya_PlayGesture(0);
    Engine_EventWait(40);
    Event_ShowMessageAndWait(0x8009, 0, 10);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_ActorStartRepeatedMotion(GERALD, 2);
    Engine_ActorStartRepeatedMotion(MEARI, 2);
    Engine_ActorRunRepeatedMotion(IWAN, 2);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, (v6 << 8), 0);
    Engine_ActorFaceDirection(1, (v6 << 8), 0);
    Actor_FaceDirection(IWAN, (v6 << 8), 40);
    ToretoHeya_PlayGesture(4);
    Event_OpenMessage(0x8009, 0);
    Actor_FaceDirection(GERALD, 0xe000, 0);
    Actor_FaceDirection(IWAN, 0xa000, 0);
    if (Engine_EventChooseYesNo(0, 0) != 0) {
        Actor_ShowEmote(GERALD, 0x103, 20);
        Engine_ActorSetAnimation(GERALD, 4);
        Engine_EventSetMessage((s32)MsgToretoDoingNowsNot);
        Event_ShowMessage(0x8001, 0);
        Actor_ShowEmote(IWAN, 0x103, 10);
        Engine_ActorSetAnimation(IWAN, 3);
        Event_ShowMessage(0x8002, 0);
    }
    Engine_EventWait(20);
    ToretoHeya_PlayGesture(4);
    Engine_EventSetMessage((s32)MsgToretoTurnedPeopleKolima);
    Event_ShowMessageAndWait(0x8009, 0, 20);
    Event_ShowMessageAndWait(0x8009, 0, 10);
    ToretoHeya_PlayGesture(0);
    Engine_EventWait(20);
    ColorBuffer_ApplySource(0x10000, 0);
    ColorBuffer_ApplyTarget(0x406218, 1);
    Engine_ColorBufferInterpolate(20);
    Engine_TaskWait(40);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_ActorStartRepeatedMotion(GERALD, 2);
    Engine_ActorStartRepeatedMotion(MEARI, 2);
    Engine_ActorRunRepeatedMotion(IWAN, 2);
    Actor_FaceDirection(GERALD, (v6 << 8), 0);
    Actor_FaceDirection(IWAN, (v6 << 8), 20);
    Engine_EventWait(20);
    *(s32 *)ToretoHeya_SparkCounter = 0;
    {
        s32 *bank = (s32 *)ToretoHeya_SparkOrigin;
        bank[0] = 0xa80000;
        bank[1] = 0x200000;
        base5_200962d = (s32)ToretoHeya_SpawnSwirlSparks;
        bank[2] = 0x340000;
    }
    Task_AddCallback((void (*)(void))base5_200962d, 0xc80);
    Engine_EventWait(220);
    Engine_TaskRemoveCallback(base5_200962d);
    ColorBuffer_ApplyTarget(0x10000, 1);
    Engine_ColorBufferInterpolate(20);
    Engine_TaskWait(40);
    ToretoHeya_PlayGesture(4);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(0x8009, 0, 10);
    ToretoHeya_PlayGesture(0);
    Event_ShowMessage(0x8009, 0);
    Object_SetActionCallbackAndRefreshById(8, (s32)ToretoHeya_ActionTable1);
    Engine_EventWait(40);
    Actor_ShowEmote(GERALD, 0x102, 60);
    Event_ShowMessage(0x8001, 0);
    Actor_ShowEmote(IWAN, 0x102, 10);
    Event_ShowMessage(0x8002, 0);
    Actor_FaceDirection(GERALD, 0xe000, 0);
    Actor_FaceDirection(IWAN, 0xa000, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 10);
    Engine_ActorRunRepeatedMotion(GERALD, 1);
    Event_ShowMessageAndWait(0x8001, 0, 10);
    Engine_ActorRunRepeatedMotion(IWAN, 1);
    Event_ShowMessageAndWait(0x8002, 0, 10);
    if (rec != 0) {
        Engine_ActorRunRepeatedMotion(MEARI, 1);
        Engine_EventShowMessageAndWait(0x8003, 0, 10);
    }
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(GERALD, 3);
    Engine_ActorSetAnimation(MEARI, 3);
    Engine_ActorSetAnimationAndWait(IWAN, 3);
    base5_2009ec8 = (s32)ToretoHeya_ActionTable2;
    Engine_ActorEnableActionCallback(GERALD, base5_2009ec8);
    if (rec != 0) {
        Engine_ActorEnableActionCallback(MEARI, base5_2009ec8);
    }
    Object_SetActionCallbackAndRefreshById(2, base5_2009ec8);
    Engine_EventWait(20);
    Object_GetById(0)->priority_flags |= 1;
    GameFlag_Set(0x844);
    Engine_TaskAddCallback((s32)ToretoPalette_ApplyTint, 0xc80);
    Engine_EventEnd();
}

void SceneState_ApplyRectsByFlag844(s32 flag)
{
    if (flag != 0 && Engine_GameFlagIsSet(0x109) == 0)
        FieldScene_RunFourActorEncounter();

    Engine_TaskWait(1);
    if (Engine_GameFlagIsSet(0x844) != 0) {
        s32 w1 = 10;
        Engine_MapCopyCells(121, 34, 3, 1, 93, w1);
        {
            s32 w2 = 30;
            Engine_MapCopyCells(46, 38, 1, 1, w2, 43);
            Engine_MapCopyCellAttributes(0, 0, 1, 2, w2, 9);
        }
        Engine_MapCopyCellAttributes(26, 3, 1, 2, w1, 8);
        Engine_MapCopyCells(26, 35, 1, 4, w1, 40);
    } else {
        s32 w1 = 10;
        s32 w2 = 8;
        Engine_MapCopyCellAttributes(11, 8, 1, 2, w1, w2);
    }
}

s32 ToretoHeya_EnterRoom(void)
{
    struct FieldActor *lamp;
    struct FieldActor *actor;
    u8 *work;
    s32 *camera;
    s32 entrance;
    s32 leader;

    lamp = Object_GetById(8);
    ToretoHeya_PaletteBuffer = (s16 *)gSceneState;
    ToretoPalette_CaptureBank();
    lamp->motion_flags = 0;
    lamp->y.fixed = -0xa0000;
    {
        struct FieldActor *other = Object_GetById(9);

        other->motion_flags = 0;
        other->y.fixed = -0xa0000;
    }
    Engine_ActorSetChildValue(9, 15);
    ToretoHeya_PlayGesture(0);
    if (gGameState.entrance != 19)
        ((void (*)())Engine_TaskAddCallback)((s32)ToretoPalette_ApplyTint, 0xc80);
    if (Engine_GameFlagIsSet(0x844)) {
        Engine_ActorSetPosition(9, 0, 0);
        Engine_ActorSetPosition(8, 0, 0);
    }
    if (GameFlag_IsSet(0x109))
        ToretoHeya_ApplyFlaggedMapPatches();
    work = WORK_SLOT(8);
    camera = (s32 *)(work + 260);
    camera[2] += Iwram_MulQ16(*(s32 *)(work + 236) + 0xa00000, 0x1999);
    camera[3] += Iwram_MulQ16(*(s32 *)(work + 240) + 0x880000, 0x1999);
    camera[4] = 0xe666;
    camera[5] = 0xe666;
    Engine_GameFlagSet(0x201);
    Engine_GameFlagSet(0x20d);
    Engine_GameFlagSet(0x20f);
    Engine_GameFlagSet(0x213);
    Engine_TaskWait(1);
    SceneState_ApplyRectsByFlag844(0);
    ((struct EventWork *)WORK_SLOT(HEAP_SLOT_EVENT))->start_transition = 0x202;
    entrance = gGameState.entrance;
    leader = gGameState.selected_actor;
    actor = Object_GetById(leader);
    if (entrance == 50 || entrance == 40 || entrance == 30 || entrance == 20) {
        Engine_EventOpenScreen();
        Engine_ActorSetAnimation(leader, 27);
        Engine_ActorSetSpriteFlags(Object_GetById(leader), 0);
        Actor_SetAttachedEffect(leader, 0x101);
        Camera_MoveTo(-1, -1, -1, 0);
        actor->motion_flags = 2;
        actor->y.fixed = 0x640000;
        actor->terrain_height = -0xa00000;
        actor->gravity = 0x8000;
        Engine_AudioPlayCue(204);
        Event_SetDestination((s32)&SceneId_ToretoHeya, entrance - 10);
        Engine_EventWait(20);
        actor->terrain_id = 2;
        Engine_ActorSetSpritePriority(leader, 3);
        Engine_EventWait(2);
        Engine_ActorSetAttachedEffect(leader, 0x100);
        Engine_EventWait(8);
    } else if (entrance == 10) {
        if (!Engine_GameFlagIsSet(0x109))
            ToretoHeya_RunLandingDustScene();
    } else if (entrance == 19) {
        SceneState_ApplyRectsByFlag844(1);
    }
    return 0;
}

void ToretoPalette_CaptureBank(void)
{
    Dma_Set((const void *)0x05000000, *(void **)&gPaletteBufferWork, 0x84000070,
            REG_DMA3);
}

void ToretoPalette_ApplyTint(void)
{
    u16 *src;
    u16 *dst;
    u32 i;
    s32 r;
    s32 g;
    s32 b;
    s32 red;
    s32 green;
    s32 blue;
    u32 color;
    u8 *event;

    event = WORK_SLOT(HEAP_SLOT_EVENT);
    src = WORK_SLOT(32);
    if (*(s16 *)(event + 0x17e) != 0)
        return;
    if ((gFrameCount & 31) != 0)
        return;
    src += 16;
    dst = (u16 *)0x05000020;
    i = 0;
    for (; i <= 62; i++, src++) {
        r = ToretoHeya_TintSteps[ToretoHeya_TintIndex];
        g = ToretoHeya_TintSteps[ToretoHeya_TintIndex + 1];
        b = ToretoHeya_TintSteps[ToretoHeya_TintIndex + 2];
        if (i > 47) {
            r -= r / 2 + r / 3;
            g -= g / 2 + g / 3;
            b -= b / 2 + b / 3;
        } else if (i > 31) {
            r -= r / 3 + r / 4;
            g -= g / 3 + g / 4;
            b -= b / 3 + b / 4;
        } else if (i > 15) {
            r -= r / 4 + r / 5;
            g -= g / 4 + g / 5;
            b -= b / 4 + b / 5;
        }
        color = *src;
        red = color & 31;
        green = (color >> 5) & 31;
        blue = (color >> 10) & 31;
        red += r;
        green += g;
        blue += b;
        if (red > 31)
            red = 31;
        if (green > 31)
            green = 31;
        if (blue > 31)
            blue = 31;
        if (red < 0)
            red = 0;
        if (green < 0)
            green = 0;
        if (blue < 0)
            blue = 0;
        *dst++ = (blue << 10) | (green << 5) | red;
    }
    ToretoHeya_TintIndex += (Engine_RandomNext() & 7) * 3;
    if (ToretoHeya_TintSteps[ToretoHeya_TintIndex] == 99)
        ToretoHeya_TintIndex = 0;
}

void ToretoHeya_PlayGesture(s32 gesture)
{
    switch (gesture) {
    case 0:
        Engine_ActorSetAnimation(8, 1);
        Engine_TaskWait(6);
        Engine_ActorSetAnimation(8, 3);
        break;
    case 1:
        Engine_ActorSetAnimation(8, 1);
        break;
    case 2:
        Engine_ActorSetAnimation(8, 1);
        Engine_TaskWait(6);
        Engine_ActorSetAnimation(8, 5);
        break;
    case 3:
        Engine_ActorSetAnimation(8, 1);
        Engine_TaskWait(6);
        Engine_ActorSetAnimation(8, 4);
        break;
    case 4:
        Engine_ActorSetAnimation(8, 1);
        Engine_TaskWait(6);
        Engine_ActorSetAnimation(8, 3);
        Engine_TaskWait(6);
        Engine_ActorSetAnimation(8, 1);
        Engine_TaskWait(6);
        Engine_ActorSetAnimation(8, 3);
        Engine_TaskWait(6);
        Engine_ActorSetAnimation(8, 1);
        break;
    case 5:
        Engine_ActorSetAnimation(8, 1);
        Engine_TaskWait(6);
        Engine_ActorSetAnimation(8, 2);
        break;
    case 7:
        Engine_ActorSetAnimation(8, 6);
        Engine_TaskWait(6);
        Engine_ActorSetAnimation(8, 8);
        break;
    case 8:
        Engine_ActorSetAnimation(8, 6);
        break;
    case 9:
        Engine_ActorSetAnimation(8, 6);
        Engine_TaskWait(6);
        Engine_ActorSetAnimation(8, 9);
        break;
    case 10:
        Engine_ActorSetAnimation(8, 6);
        Engine_TaskWait(6);
        Engine_ActorSetAnimation(8, 10);
        break;
    case 11:
        Engine_ActorSetAnimation(8, 6);
        Engine_TaskWait(6);
        Engine_ActorSetAnimation(8, 8);
        Engine_TaskWait(6);
        Engine_ActorSetAnimation(8, 6);
        Engine_TaskWait(6);
        Engine_ActorSetAnimation(8, 8);
        Engine_TaskWait(6);
        Engine_ActorSetAnimation(8, 6);
        break;
    case 12:
        Engine_ActorSetAnimation(8, 6);
        break;
    }
    Engine_TaskWait(12);
}

void ToretoHeya_UpdateSwirlObject(struct Swirl *obj)
{
    struct Vec pos;
    s32 t;

    t = obj->timer;
    if (t <= 79) {
        pos.x = obj->ox;
        pos.y = obj->oy;
        pos.z = obj->oz;
        {
            s32 a = obj->angle;

            Vector_AddPolarOffset(t << 16, ((t * 3) << 8) + a, &pos);
        }
        obj->x = pos.x;
        obj->y = pos.y;
        obj->z = pos.z;
        if (obj->timer <= 39) {
            obj->scaleX += -0x51e;
            obj->scaleY += -0x51e;
        }
        obj->timer++;
    } else {
        Resource_ResetEntry(obj->sprite[28]);
        Engine_ObjectDispatchRelease(obj);
    }
}

void ToretoHeya_SpawnSwirlSparks(void)
{
    struct FieldActor *spark;
    u32 frame;
    s32 previous;
    s32 wave;
    u32 i;
    s32 *counter;

    counter = &ToretoHeya_SparkCounter;
    frame = *counter;
    previous = 0;
    wave = __divsi3(frame, 10);
    switch (frame) {
    case 0:
    case 10:
    case 20:
    case 30:
    case 40:
        Engine_AudioPlayCue(220);
        for (i = 0; i < 6 - wave; i++) {
            spark = Engine_ObjectCreate(0x11d, (*(struct Vec3 *)ToretoHeya_SparkOrigin).x, (*(struct Vec3 *)ToretoHeya_SparkOrigin).y, (*(struct Vec3 *)ToretoHeya_SparkOrigin).z);
            if (spark != 0) {
                previous = Object_ReplaceResourceEntry(spark->sprite, previous);
                spark->motion_flags = 0;
                spark->sprite->priority = 0;
                Engine_ActorSetSpriteFlags(spark, 0);
                Object_SetMode(spark, 1);
                ((struct Spark *)spark)->phase = 0;
                ((struct Spark *)spark)->angle = ((360 / (u32)(6 - wave) * i) << 16) / 360;
                spark->target_x = (*(struct Vec3 *)ToretoHeya_SparkOrigin).x;
                spark->target_y = (*(struct Vec3 *)ToretoHeya_SparkOrigin).y;
                spark->target_z = (*(struct Vec3 *)ToretoHeya_SparkOrigin).z;
                spark->speed = 0x19999;
                spark->update = ToretoHeya_UpdateSwirlObject;
            }
        }
    case 44:
        Engine_AudioPlayCue(0x121);
        /* FAKEMATCH: the counter pointer is taken again here so it is dead across the spark loop, where the loop index takes its register. */
        counter = &ToretoHeya_SparkCounter;
        break;
    }
    if (++*counter > 120) {
        *counter = 0;
    }
}

void ToretoHeya_ApplyFlaggedMapPatches(void)
{
    s32 i;

    for (i = 0; ToretoHeya_MapPatches[i] != -1; i += 6) {
        if (Engine_GameFlagIsSet(ToretoHeya_MapPatches[i]) && ToretoHeya_MapPatches[i + 1] != 0)
            Map_CopyCellsTo(ToretoHeya_MapPatches[i + 2], ToretoHeya_MapPatches[i + 3],
                ToretoHeya_MapPatches[i + 4], ToretoHeya_MapPatches[i + 5], 1, 1);
    }
}

void SceneEffect_RegisterPaletteFadeCallback(void)
{
    Engine_TaskRemoveCallback(ToretoPalette_ApplyTint);
}

void ToretoHeya_AdvanceEffectMotion(struct Effect *effect)
{
    struct Sprite *sprite;

    effect->position[0] += effect->velocity_x;
    effect->position[1] += effect->velocity_y;
    effect->position[2] += effect->velocity_z;

    effect->velocity_x -= effect->velocity_x / 18;
    effect->velocity_z -= effect->velocity_z / 16;

    effect->accum18 += effect->rate30;
    effect->accum1c += effect->rate34;

    sprite = effect->sprite;
    sprite->angle += effect->step64;
}

void ToretoHeya_RunLandingDustScene(void)
{
    struct EffectParams params;
    struct Vec dir;
    struct EffectParams *p;
    struct Vec *v;
    struct FieldActor *leader;
    u32 i;
    s32 x;
    s32 z;

    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Object_GetById(0)->y.fixed = 0x820000;
    Object_GetById(0)->gravity = 0x8000;
    Object_GetById(0)->bounce = 0;
    Object_GetById(0)->motion_flags = 0;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_AudioPlayCue(204);
    Object_GetById(0)->motion_flags = 3;
    Engine_EventWait(24);
    leader = Object_GetById(0);
    p = &params;
    p->kind = 7;
    p->script = (s32)ToretoHeya_AdvanceEffectMotion;
    p->spread = 0xcccc;
    p->rise = 0xcccc;
    for (i = 0; i <= 16; i++) {
        v = &dir;
        v->x = Engine_MathCos(i << 12);
        v->y = 0;
        z = Engine_MathSin(i << 12);
        x = v->x;
        v->z = z;
        x += x / 2;
        v->x = x;
        Effect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed, x, v->y, z, 0x1090001, p);
    }
    Engine_AudioPlayCue(188);
    Actor_SetAttachedEffect(0, 0x101);
    Engine_ActorSetAnimation(0, 22);
    Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Engine_MapWaitWorkValuesBelow256();
    Engine_ActorSetAttachedEffect(0, 0x100);
    Object_GetById(0)->gravity = 0x10000;
    Object_GetById(0)->bounce = 0x4000;
    Engine_EventEnd();
}
