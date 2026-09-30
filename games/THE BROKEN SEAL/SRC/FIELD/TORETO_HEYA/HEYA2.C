#include "HEYA.H"
#include "CALL.H"
#include "IWRAM_CALL.H"
#include "DMA.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"

/* The room's tables, which the entry veneers publish to the map engine. */
extern const struct SceneEntrance gToretoHeyaEntrances[];
extern const struct SceneRegion gToretoHeyaRegions[];
extern const u32 gToretoHeyaExits[];
extern const struct ScenePlacement gToretoHeyaPlacements[];

void Event_SetPairWork1c0(s32 scene, s32 entrance);

extern const struct SceneEvent gToretoHeyaEvents[];

extern u8 MsgToretoHmHrooom[];

/* The action tables the four party members take at the table. */
extern const u8 ToretoHeya_TableActions0[];
extern const u8 ToretoHeya_TableActions1[];
extern const u8 ToretoHeya_TableActions2[];
extern const u8 ToretoHeya_TableActions3[];
void Party_SetFields1ceAnd1d0(s32 scene, s32 entrance);
void BattleFx_SetWeightedResult(s32 value, s32 mode);

extern u8 MsgToretoDoingNowsNot[];
extern u8 MsgToretoMmmmm[];
extern u8 MsgToretoTurnedPeopleKolima[];

extern struct MapRenderWork *gMapWork;
void ToretoPalette_CaptureBank(void);
void ToretoHeya_ApplyFlaggedMapPatches(void);
void SceneState_ApplyRectsByFlag844(s32 flag);
void ToretoHeya_RunLandingDustScene(void);

extern struct BattleEffectBuffers *Data_03001ed0;

/* FAKEMATCH: reading the event work and the palette work through the
 * engine's table of work pointers keeps one address base for both cells,
 * where their own names would each take a pool word. */
extern u8 *gWork[];

/* Current entry in the RGB tint list; a red of 99 ends the list. */
extern s32 ToretoHeya_TintIndex;
extern s32 ToretoHeya_TintSteps[];

void Vector_AddPolarOffset();
void Resource_ResetEntry();
void Engine_ObjectDispatchRelease();

struct Vec {
    s32 x;
    s32 y;
    s32 z;
};

/* Swirl the object around its origin for 80 frames, shrinking it over the
 * first 40, then release it. */
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

s32 Object_ReplaceResourceEntry(struct FieldSprite *sprite, s32 previous);

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

/* Each patch stores flag, enabled, source x/y and destination x/y. */
extern s16 ToretoHeya_MapPatches[];

void ToretoHeya_AdvanceEffectMotion();
void Engine_EventBegin(void);
void Engine_CameraMoveTo();
void Engine_MapRedraw(void);
void Engine_TaskWait(s32 frames);
void Engine_EventOpenScreen(void);
void Engine_EventWaitForScreen(void);
void Engine_AudioPlayCue(s32 cue);
void Engine_EventWait(s32 frames);
s32 Engine_MathCos(s32 angle);
s32 Engine_MathSin(s32 angle);
void Effect_Spawn(s32 x, s32 y, s32 z, s32 dx, s32 dy, s32 dz, s32 lift, void *params);
void Engine_ActorSetAttachedEffect();
void Engine_ActorSetAnimation(s32 actor, s32 anim);
void Engine_WorkSetValuesIfNonNegative();
void Engine_MapWaitWorkValuesBelow256(void);
void Engine_EventEnd(void);

struct EffectParams {
    s32 count;
    s32 kind;
    s32 spread;
    s32 rise;
    u8 pad10[20];
    s32 script;
};

void ToretoHeya_UpdateSwirlObject(struct Swirl *obj);

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

/* A floor switch: when the leader steps onto a new entrance tile, copy its
 * cell; the first time, just set its flag, otherwise drop the leader through
 * the floor to map 0x2d at the entrance, spinning as it falls. */
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
    actor = Engine_ActorGet(leader);
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
    Event_SetPairWork1c0((s32)&SceneId_ToretoHeya, entrance);
    Engine_ActorSetAnimation(leader, 27);
    Engine_ActorSetSpriteFlags(Engine_ActorGet(leader), 0);
    Call2(Engine_ActorSetAttachedEffect, leader, 0x101);
    Engine_EventWait(30);
    Call4(Engine_CameraMoveTo, -1, -1, -1, 0);
    actor->motion_flags = 2;
    *(s32 *)((u8 *)actor + 20) = -0xa00000;
    *(s32 *)((u8 *)actor + 72) = 0x8000;
    Engine_AudioPlayCue(204);
    Engine_EventWait(3);
    actor->unknown_22 = 2;
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
    if (Leader_CheckAhead() == 0)
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
    s32 record;
    s32 base3_2000240;

    rec8 = Engine_GameFlagIsSet(3);
    Engine_EventBegin();
    Engine_AudioPlayCue(17);
    Engine_EventSetMessage((s32)MsgToretoHmHrooom);
    Engine_EventShowMessageAndWait(0x8009, 0, 20);
    Engine_AudioPlayCue(29);
    Call3(Engine_ActorSetSpeed, 0, 0x10000, 0x8000);
    Call3(Engine_ActorSetSpeed, 1, 0x10000, 0x8000);
    Call3(Engine_ActorSetSpeed, 2, 0x10000, 0x8000);
    Engine_ActorSetSpeed(3, 0x10000, 0x8000);
    *((u8 *)Engine_ActorGet(3) + 35) &= 254;
    Engine_ActorSetSpritePriority(3, 2);
    *((u8 *)Engine_ActorGet(0) + 35) &= 254;
    Engine_ActorSetSpritePriority(0, 2);
    record = ((s32 (*)())Engine_ActorGet)(0);
    if (record != 0) {
        Engine_ActorSetPosition(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = ((s32 (*)())Engine_ActorGet)(0);
    if (record != 0) {
        Engine_ActorSetPosition(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    if (rec8 != 0) {
        record = ((s32 (*)())Engine_ActorGet)(0);
        if (record != 0) {
            ((void (*)())Engine_ActorSetPosition)(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
        }
        Engine_ActorEnableActionCallback(3, ToretoHeya_TableActions3);
    }
    ((s32 (*)())Engine_ActorEnableActionCallback)(0, (s32)ToretoHeya_TableActions0);
    ((s32 (*)())Engine_ActorEnableActionCallback)(1, (s32)ToretoHeya_TableActions1);
    Object_SetActionCallbackAndRefreshById(2, (s32)ToretoHeya_TableActions2);
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xc000, 40);
    Engine_ActorSetAnimation(8, 11);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(8, 8);
    Engine_EventWait(20);
    ToretoHeya_PlayGesture(8);
    Call2(Engine_EventShowMessage, 0x8008, 0);
    Engine_ActorStartRepeatedMotion(0, 2);
    Engine_ActorStartRepeatedMotion(1, 2);
    Engine_ActorStartRepeatedMotion(3, 2);
    Engine_ActorStartRepeatedMotion(2, 2);
    Call3(Engine_ActorShowEmote, 0, 0x100, 0);
    Call3(Engine_ActorShowEmote, 1, 0x100, 0);
    Call3(Engine_ActorShowEmote, 3, 0x100, 0);
    Engine_ActorShowEmote(2, 0x100, 60);
    ToretoHeya_PlayGesture(11);
    Call3(Engine_EventShowMessageAndWait, 0x8008, 0, 10);
    Engine_ActorStartRepeatedMotion(0, 1);
    Engine_ActorStartRepeatedMotion(1, 1);
    Engine_ActorStartRepeatedMotion(3, 1);
    Engine_ActorRunRepeatedMotion(2, 1);
    Call2(Engine_EventShowMessage, 0x8008, 0);
    Call2(Engine_ActorSetAttachedEffect, 0, 0x102);
    Call2(Engine_ActorSetAttachedEffect, 1, 0x102);
    Call2(Engine_ActorSetAttachedEffect, 3, 0x102);
    Engine_ActorSetAttachedEffect(2, 0x102);
    Engine_EventWait(40);
    ToretoHeya_PlayGesture(11);
    Engine_EventShowMessage(0x8008, 0);
    {
        u8 *work = *(u8 **)&gEventWork;

        *(s32 *)(work + 0x1c0) = 0x200;
        *(s32 *)(work + 0x1c8) = 64;
    }
    base3_2000240 = (s32)&gGameState;
    *(u8 *)((base3_2000240 + 0x22b)) = 3;
    Party_SetFields1ceAnd1d0((s32)&SceneId_ToretoHeya, 19);
    BattleFx_SetWeightedResult(36, 0);
    Engine_EventEnd();
}

void FieldScene_RunFourActorEncounter(void)
{
    u32 i;
    s32 rec;
    s32 record;
    s32 v6;
    s32 v5;
    s32 base5_200962d;
    s32 base5_2009ec8;

    rec = GameFlag_IsSet(3);
    *((u8 *)Engine_ActorGet(3) + 35) &= 254;
    Actor_SetSpritePriority(ACTOR_MIA, 2);
    *((u8 *)Engine_ActorGet(0) + 35) &= 254;
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 2);
    PartyInventory_FindOwner(184);
    Audio_PlayCue(17);
    Event_Begin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_GERALD, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_IVAN, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_MIA, 0xcccc, 0x6666);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0xa60000, 0x500000);
    v6 = 192;
    record = Actor_Get(ACTOR_PARTY_LEADER);
    *(u16 *)(record + 6) = (v6 << 8);
    Actor_SetPosition(ACTOR_GERALD, 0x940000, 0x5a0000);
    record = Actor_Get(ACTOR_GERALD);
    *(u16 *)(record + 6) = (v6 << 8);
    Actor_SetPosition(ACTOR_IVAN, 0xb60000, 0x5a0000);
    record = Engine_ActorGet(ACTOR_IVAN);
    *(u16 *)(record + 6) = (v6 << 8);
    if (rec != 0) {
        Actor_SetPosition(ACTOR_MIA, 0xa60000, 0x680000);
        record = Engine_ActorGet(ACTOR_MIA);
        *(u16 *)(record + 6) = (v6 << 8);
    }
    ToretoHeya_PlayGesture(0);
    Task_Wait(10);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    gEventWork->transition_frames = 48;
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    Camera_SetSpeed(0x13333, 0x2666);
    Camera_MoveTo(0xa80000, -1, 0x980000, 1);
    Camera_WaitForMove();
    Event_Wait(10);
    v5 = 10;
    Audio_PlayCue(123);
    Map_CopyCellAttributes(26, 3, 1, 2, v5, 8);
    Map_CopyCells(26, 38, 1, 1, v5, 43);
    Task_Wait(4);
    Map_CopyCells(26, 37, 1, 2, v5, 42);
    Task_Wait(4);
    Map_CopyCells(26, 36, 1, 3, v5, 41);
    Task_Wait(4);
    Map_CopyCells(26, 35, 1, 4, v5, 40);
    Task_Wait(80);
    Event_SetMessage((s32)MsgToretoMmmmm);
    Event_ShowMessageAndWait(0x8009, 0, 20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_StartRepeatedMotion(ACTOR_MIA, 2);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_Wait(20);
    Camera_MoveTo(0xa80000, -1, 0x5a0000, 1);
    Camera_WaitForMove();
    Event_Wait(40);
    ToretoHeya_PlayGesture(1);
    Event_Wait(60);
    Audio_PlayCue(21);
    ToretoHeya_PlayGesture(4);
    Event_ShowMessageAndWait(0x8009, 0, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 80);
    Event_ShowMessage(0x8009, 0);
    Event_Wait(40);
    Event_ShowMessageAndWait(0x8009, 0, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_MIA, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x8009, 0, 20);
    ToretoHeya_PlayGesture(0);
    Event_Wait(40);
    Event_ShowMessageAndWait(0x8009, 0, 20);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Actor_SetAttachedEffect(ACTOR_MIA, 0x102);
    Actor_SetAttachedEffect(ACTOR_IVAN, 0x102);
    Event_Wait(60);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 10);
    Event_ShowMessageAndWait(0x8001, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 4);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 0);
    Event_ShowMessageAndWait(0x8002, 0, 20);
    ToretoHeya_PlayGesture(0);
    Event_Wait(40);
    Event_ShowMessageAndWait(0x8009, 0, 10);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_StartRepeatedMotion(ACTOR_MIA, 2);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, (v6 << 8), 0);
    ((void (*)())Engine_ActorFaceDirection)(1, (v6 << 8), 0);
    Actor_FaceDirection(ACTOR_IVAN, (v6 << 8), 40);
    ToretoHeya_PlayGesture(4);
    Event_OpenMessage(0x8009, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 0);
    if (Event_ChooseYesNo(0, 0) != 0) {
        Actor_ShowEmote(ACTOR_GERALD, 0x103, 20);
        Actor_SetAnimation(ACTOR_GERALD, 4);
        Event_SetMessage((s32)MsgToretoDoingNowsNot);
        Event_ShowMessage(0x8001, 0);
        Actor_ShowEmote(ACTOR_IVAN, 0x103, 10);
        Actor_SetAnimation(ACTOR_IVAN, 3);
        Event_ShowMessage(0x8002, 0);
    }
    Event_Wait(20);
    ToretoHeya_PlayGesture(4);
    Event_SetMessage((s32)MsgToretoTurnedPeopleKolima);
    Event_ShowMessageAndWait(0x8009, 0, 20);
    Event_ShowMessageAndWait(0x8009, 0, 10);
    ToretoHeya_PlayGesture(0);
    Event_Wait(20);
    ColorBuffer_ApplySource(0x10000, 0);
    ColorBuffer_ApplyTarget(0x406218, 1);
    ColorBuffer_Interpolate(20);
    Task_Wait(40);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_StartRepeatedMotion(ACTOR_MIA, 2);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Actor_FaceDirection(ACTOR_GERALD, (v6 << 8), 0);
    Actor_FaceDirection(ACTOR_IVAN, (v6 << 8), 20);
    Event_Wait(20);
    *(s32 *)ToretoHeya_SparkCounter = 0;
    {
        s32 *bank = (s32 *)ToretoHeya_SparkOrigin;
        bank[0] = 0xa80000;
        bank[1] = 0x200000;
        base5_200962d = (s32)ToretoHeya_SpawnSwirlSparks;
        bank[2] = 0x340000;
    }
    Call2(Engine_TaskAddCallback, base5_200962d, 0xc80);
    Event_Wait(220);
    Engine_TaskRemoveCallback(base5_200962d);
    ColorBuffer_ApplyTarget(0x10000, 1);
    ColorBuffer_Interpolate(20);
    Task_Wait(40);
    ToretoHeya_PlayGesture(4);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x8009, 0, 10);
    ToretoHeya_PlayGesture(0);
    Event_ShowMessage(0x8009, 0);
    Object_SetActionCallbackAndRefreshById(8, (s32)ToretoHeya_ActionTable1);
    Event_Wait(40);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 60);
    Event_ShowMessage(0x8001, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x102, 10);
    Event_ShowMessage(0x8002, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 10);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Event_ShowMessageAndWait(0x8001, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    Event_ShowMessageAndWait(0x8002, 0, 10);
    if (rec != 0) {
        Actor_RunRepeatedMotion(ACTOR_MIA, 1);
        Engine_EventShowMessageAndWait(0x8003, 0, 10);
    }
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_MIA, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    base5_2009ec8 = (s32)ToretoHeya_ActionTable2;
    Actor_EnableActionCallback(ACTOR_GERALD, base5_2009ec8);
    if (rec != 0) {
        Actor_EnableActionCallback(ACTOR_MIA, base5_2009ec8);
    }
    Object_SetActionCallbackAndRefreshById(2, base5_2009ec8);
    Event_Wait(20);
    *((u8 *)Engine_ActorGet(0) + 35) |= 1;
    GameFlag_Set(0x844);
    Engine_TaskAddCallback((s32)ToretoPalette_ApplyTint, 0xc80);
    Event_End();
}

void SceneState_ApplyRectsByFlag844(s32 flag)
{
    if (flag != 0 && GameFlag_IsSet(0x109) == 0)
        FieldScene_RunFourActorEncounter();

    Task_Wait(1);
    if (GameFlag_IsSet(0x844) != 0) {
        s32 w1 = 10;
        Map_CopyCells(121, 34, 3, 1, 93, w1);
        {
            s32 w2 = 30;
            Map_CopyCells(46, 38, 1, 1, w2, 43);
            Map_CopyCellAttributes(0, 0, 1, 2, w2, 9);
        }
        Map_CopyCellAttributes(26, 3, 1, 2, w1, 8);
        Map_CopyCells(26, 35, 1, 4, w1, 40);
    } else {
        s32 w1 = 10;
        s32 w2 = 8;
        Map_CopyCellAttributes(11, 8, 1, 2, w1, w2);
    }
}

/* FAKEMATCH: the map work and the event work are read as cells of one
 * pointer array based at gMapWork (the event work is its twentieth), which
 * keeps one pool address for both. */

/* Enter the room: hide the lamps, drift the camera, set the room flags, and
 * finish the fall from the floor above when entered through entrance 20 to
 * 50 (the leader lands on the entrance ten below it). */
s32 ToretoHeya_EnterRoom(void)
{
    struct FieldActor *lamp;
    struct FieldActor *actor;
    u8 **globals;
    u8 *work;
    s32 *camera;
    s32 entrance;
    s32 leader;

    lamp = Engine_ActorGet(8);
    ToretoHeya_PaletteBuffer = (s16 *)gSceneState;
    ToretoPalette_CaptureBank();
    lamp->motion_flags = 0;
    lamp->y.fixed = -0xa0000;
    {
        struct FieldActor *other = Engine_ActorGet(9);

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
    if (Value1(Engine_GameFlagIsSet, 0x109))
        ToretoHeya_ApplyFlaggedMapPatches();
    globals = (u8 **)&gMapWork;
    work = globals[0];
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
    *(s32 *)(globals[19] + 0x1c0) = 0x202;
    entrance = gGameState.entrance;
    leader = gGameState.selected_actor;
    actor = Engine_ActorGet(leader);
    if (entrance == 50 || entrance == 40 || entrance == 30 || entrance == 20) {
        Engine_EventOpenScreen();
        Engine_ActorSetAnimation(leader, 27);
        Engine_ActorSetSpriteFlags(Engine_ActorGet(leader), 0);
        Call2(Engine_ActorSetAttachedEffect, leader, 0x101);
        Call4(Engine_CameraMoveTo, -1, -1, -1, 0);
        actor->motion_flags = 2;
        actor->y.fixed = 0x640000;
        *(s32 *)((u8 *)actor + 20) = -0xa00000;
        *(s32 *)((u8 *)actor + 72) = 0x8000;
        Engine_AudioPlayCue(204);
        Event_SetPairWork1c0((s32)&SceneId_ToretoHeya, entrance - 10);
        Engine_EventWait(20);
        actor->unknown_22 = 2;
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

/*
 * Toreto house: upload the room's palette bank from the source pointed at by
 * the loader's palette pointer. The DMA runs from the VRAM pattern bank into
 * the palette RAM and waits on the DMA status register.
 */
void ToretoPalette_CaptureBank(void)
{
    Dma_Set((const void *)0x05000000, *(void **)&Data_03001ed0, 0x84000070,
            (volatile u32 *)0x040000d4);
}

/* Every 32 frames, tint the background palette, weaker for later colours. */
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

    event = gWork[0];
    src = (u16 *)gWork[5];
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

/* Play one of actor 8's numbered gestures, then wait twelve frames. */
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
    wave = Engine_MathDivide(frame, 10);
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
                Engine_ObjectSetAnimation(spark, 1);
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
        /* FAKEMATCH: the counter pointer is taken again here so it is dead
         * across the spark loop, where the loop index takes its register. */
        counter = &ToretoHeya_SparkCounter;
        break;
    }
    if (++*counter > 120) {
        *counter = 0;
    }
}

/* Copy the one-cell patch of each enabled entry whose flag is set. */
void ToretoHeya_ApplyFlaggedMapPatches(void)
{
    s32 i;
    /* FAKEMATCH: halfword storage delays the size producer until after the
     * sentinel and offset, matching the loop preheader. */
    struct Half { u16 v; } size;

    for (i = 0; ToretoHeya_MapPatches[i] != -1; i += 6) {
        size.v = 1;
        if (Engine_GameFlagIsSet(ToretoHeya_MapPatches[i]) && ToretoHeya_MapPatches[i + 1] != 0)
            Map_CopyCellsTo(ToretoHeya_MapPatches[i + 2], ToretoHeya_MapPatches[i + 3],
                ToretoHeya_MapPatches[i + 4], ToretoHeya_MapPatches[i + 5], size.v, size.v);
    }
}

void SceneEffect_RegisterPaletteFadeCallback(void)
{
    Engine_TaskRemoveCallback(ToretoPalette_ApplyTint);
}

/*
 * The decay of the Z velocity stays a signed divide by sixteen: that shape is
 * what reproduces the negative bias and arithmetic shift in the reference.
 */
void ToretoHeya_AdvanceEffectMotion(struct Effect *effect)
{
    s32 velocity_z;
    struct Sprite *sprite;
    s32 velocity_x;

    /* This block orders the Z load after the Y store; do not flatten it. */
    do {
        velocity_x = effect->velocity_x;
        effect->position[0] += velocity_x;
        effect->position[1] += effect->velocity_y;
    } while (0);
    velocity_z = effect->velocity_z;
    effect->position[2] += velocity_z;

    effect->velocity_x = velocity_x - Math_Divide(velocity_x, 18);
    effect->velocity_z = velocity_z - velocity_z / 16;

    effect->accum18 += effect->rate30;
    effect->accum1c += effect->rate34;

    sprite = effect->sprite;
    sprite->angle += effect->step64;
}

/* The leader lands in the room in a ring of dust, then the camera settles. */
void ToretoHeya_RunLandingDustScene(void)
{
    struct EffectParams params;
    struct Vec dir;
    struct EffectParams *p;
    struct Vec *v;
    u8 *leader;
    u32 i;
    s32 x;
    s32 z;

    Engine_EventBegin();
    Call4(Engine_CameraMoveTo, -1, -1, -1, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    *(s32 *)((u8 *)Engine_ActorGet(0) + 12) = 0x820000;
    *(s32 *)((u8 *)Engine_ActorGet(0) + 72) = 0x8000;
    *(s32 *)((u8 *)Engine_ActorGet(0) + 68) = 0;
    ((u8 *)Engine_ActorGet(0))[85] = 0;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_AudioPlayCue(204);
    ((u8 *)Engine_ActorGet(0))[85] = 3;
    Engine_EventWait(24);
    leader = (u8 *)Engine_ActorGet(0);
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
        Effect_Spawn(*(s32 *)(leader + 8), *(s32 *)(leader + 12), *(s32 *)(leader + 16), x, v->y, z, 0x1090001, p);
    }
    Engine_AudioPlayCue(188);
    Call2(Engine_ActorSetAttachedEffect, 0, 0x101);
    Engine_ActorSetAnimation(0, 22);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x50000, 0x50000, 0x10000);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Engine_MapWaitWorkValuesBelow256();
    Engine_ActorSetAttachedEffect(0, 0x100);
    *(s32 *)((u8 *)Engine_ActorGet(0) + 72) = 0x10000;
    *(s32 *)((u8 *)Engine_ActorGet(0) + 68) = 0x4000;
    Engine_EventEnd();
}
