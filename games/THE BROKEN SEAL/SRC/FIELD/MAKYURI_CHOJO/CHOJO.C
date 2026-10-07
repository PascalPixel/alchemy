#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "GLOBAL_CELLS.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "OVERLAY_OBJECT.H"
#include "CREATE_CONFIGURED_OVERLAY_OBJECT.H"
#include "AERIE.H"
#include "IWRAM_CALL.H"
#include "MAP_SCROLL.H"
#include "CALL.H"
#include "text/MSG_IDS.H"


void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position);
s32 Object_CheckMovementCollision(struct FieldActor *actor, s32 *position);
void Party_SetFields1ceAnd1d0();
void BattleFx_SetWeightedResult();
void Effect_Spawn(s32 x, s32 y, s32 z, s32 dx, s32 dy, s32 dz, s32 lift, void *params);

struct EffectParams {
    s32 count;
    s32 kind;
    s32 spread;
    s32 rise;
    u8 pad10[8];
    u16 tile;
    u16 pad1a;
    u8 pad1c[12];
};

struct ActorTable {
    u8 unknown_00[20];
    struct FieldActor *actors[66];
};

void MakyuriChojo_CollectBandSlots();

const struct SceneEntrance *Scene_GetEntrances(void)
{
    return gMakyuriChojoEntrances;
}

const struct SceneRegion *Scene_GetRegions(void)
{
    return 0;
}

const u32 *Scene_GetExits(void)
{
    return gMakyuriChojoExits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
    if (gGameState.entrance != 1) {
        Engine_GameFlagSet(0x253);
    }
    return gMakyuriChojoPlacements;
}

const struct SceneEvent *Scene_GetEvents(void)
{
    return gMakyuriChojoEvents;
}

s32 Scene_Initialize(void)
{
    u32 i;
    s32 actor;
    struct FieldActor *obj;

    Engine_GameFlagSet(0x111);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    if (gGameState.scene == (s32)&SceneId_MakyuriChojo1) {
    Engine_GameFlagSet(0x144);
    Engine_TaskAddCallback(MakyuriChojo_FlickerActorEight, 3200);
    Engine_ActorSetSpritePriority(0, 1);
    Engine_ActorSetSpritePriority(1, 1);
    Engine_ActorSetSpritePriority(2, 1);
    Engine_ActorSetSpritePriority(3, 1);
    Engine_ActorSetSpritePriority(5, 1);
    Engine_ActorSetSpritePriority(20, 1);
    Engine_ActorSetSpritePriority(21, 1);
    Engine_ActorSetSpritePriority(22, 1);
    Engine_ActorSetSpritePriority(23, 1);
    Engine_ActorSetSpritePriority(24, 1);
    Engine_ActorSetSpritePriority(8, 1);
    Engine_ActorSetSpritePriority(9, 1);
    Engine_ActorSetSpritePriority(10, 1);
    Engine_ActorSetSpritePriority(11, 1);
    Engine_ActorSetSpritePriority(12, 1);
    Engine_ActorSetSpritePriority(13, 1);
    for (i = 14; i < 20; i++) {
        Engine_ActorSetSpritePriority(i, 1);
        Object_GetById(i)->motion_flags = 4;
        Object_GetById(i)->priority_flags |= 2;
        Object_GetById(i)->y.fixed = -0x328000;
    }
    if (Engine_GameFlagIsSet(0x109) && (actor = FindNearestF2Actor()) != 0 && (obj = Object_GetById(actor)) != NULL) {
        obj->motion_flags = 0;
    }
    SetOverlayObjectMode(Object_GetById(9), 0);
    SetOverlayObjectMode(Object_GetById(10), 0);
    SetOverlayObjectMode(Object_GetById(11), 0);
    SetOverlayObjectMode(Object_GetById(12), 0);
    SetOverlayObjectMode(Object_GetById(13), 0);
    Object_GetById(12)->scale_x = -0x10000;
    Object_GetById(13)->scale_x = -0x10000;
    if (gGameState.entrance == 1) {
        if (!Engine_GameFlagIsSet(0x109)) {
            FieldScene_RunFourActorPresentation();
        }
    } else if (gGameState.entrance == 2) {
        if (!Engine_GameFlagIsSet(0x251)) {
            {
                struct MapLayerScroll *lift = &((struct MapScrollWork *)gMapWork[0])->layers[2];

                lift->offset_y = 0x4000000;
            }
            Engine_MapRedraw();
            Engine_TaskWait(1);
            Map_CopyCellsTo(4, 70, 4, 74, 5, 4);
            Engine_ActorSetPosition(9, 0, 0);
            if (!Engine_GameFlagIsSet(0x109)) {
                RunScene59Sequence();
            }
        }
    } else if (gGameState.entrance == 5) {
        Engine_GameFlagSet(0x251);
    }
    }
    return 0;
}

void FieldScene_RunFourActorPresentation(void)
{
    struct FieldActor *leader;

    Engine_EventBegin();
    Owner_RefreshRatiosOnFlag();
    SetOverlayObjectMode(Object_GetById(0), 0);
    SetOverlayObjectMode(Object_GetById(1), 0);
    SetOverlayObjectMode(Object_GetById(2), 0);
    SetOverlayObjectMode(Object_GetById(3), 0);
    Camera_MoveTo(0x1300000, -1, 0x780000, 0);
    Engine_TaskWait(1);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Audio_PlayCue(141);
    Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
    Audio_PlayCue(289);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_MapRenderWaitForValues();
    Engine_EventWait(30);
    Engine_EventGetViewCenter()->motion_flags = 0;
    Camera_SetSpeed(0xcccc, 0x1999);
    Camera_MoveTo(0x2000000, -0x180000, 0xa00000, 1);
    Engine_CameraWaitForMove();
    ColorBuffer_ApplySource(0x10000, 0);
    ColorBuffer_ApplyTarget(0x10005, 0);
    Engine_ColorBufferInterpolate(50);
    Engine_EventWait(50);
    ColorBuffer_ApplyTarget(0x7fff, 0);
    Engine_ColorBufferInterpolate(30);
    Engine_EventWait(30);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x1f80000, 0xa80000);
    Actor_SetPosition(ACTOR_GERALD, 0x2100000, 0x900000);
    Actor_SetPosition(ACTOR_IVAN, 0x1e80000, 0x900000);
    Actor_SetPosition(ACTOR_MIA, 0x2000000, 0x980000);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 19);
    Engine_ActorSetAnimation(ACTOR_GERALD, 19);
    Engine_ActorSetAnimation(ACTOR_IVAN, 19);
    Engine_ActorSetAnimation(ACTOR_MIA, 19);
    Engine_EventWait(10);
    ColorBuffer_ApplyTarget(0x10000, 0);
    Engine_ColorBufferInterpolate(30);
    Engine_EventWait(30);
    Engine_EventWait(80);
    SetOverlayObjectMode(Object_GetById(0), 1);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 4);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_EventWait(60);
    SetOverlayObjectMode(Object_GetById(1), 1);
    Engine_ActorSetAnimation(ACTOR_GERALD, 1);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTHEAST, 0);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTHWEST, 0);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_GERALD, FACING_EAST, 0);
    SetOverlayObjectMode(Object_GetById(2), 1);
    Engine_ActorSetAnimation(ACTOR_IVAN, 1);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTHWEST, 0);
    Engine_EventWait(40);
    SetOverlayObjectMode(Object_GetById(3), 1);
    Engine_ActorSetAnimation(ACTOR_MIA, 1);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_MIA, 3);
    Engine_EventWait(20);
    Actor_SetSpeed(ACTOR_GERALD, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_IVAN, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_MIA, 0xcccc, 0x6666);
    Engine_ActorSetAnimation(ACTOR_GERALD, 2);
    Engine_ActorSetAnimation(ACTOR_IVAN, 2);
    Engine_ActorSetAnimation(ACTOR_MIA, 2);
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if (leader != 0)
        Actor_SetDestination(ACTOR_GERALD, leader->x.part.pixel, leader->z.part.pixel);
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if (leader != 0)
        Actor_SetDestination(ACTOR_IVAN, leader->x.part.pixel, leader->z.part.pixel);
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if (leader != 0)
        Actor_SetDestination(ACTOR_MIA, leader->x.part.pixel, leader->z.part.pixel);
    Engine_ActorWaitForMove(ACTOR_MIA);
    Actor_SetPosition(ACTOR_MIA, 0, 0);
    Engine_ActorWaitForMove(ACTOR_IVAN);
    Actor_SetPosition(ACTOR_IVAN, 0, 0);
    Engine_ActorWaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Engine_EventEnd();
}

s32 FixedPoint_Distance(s32 *first_position, s32 *second_position)
{
    s32 delta_x = (*first_position++ - *second_position++) >> 16;
    s32 delta_y = (*first_position++ - *second_position++) >> 16;
    s32 delta_z = (*first_position - *second_position) >> 16;
    s32 delta_x_squared = delta_x * delta_x;
    s32 delta_y_squared = delta_y * delta_y;
    s32 delta_z_squared = delta_z * delta_z;

    return Iwram_Sqrt(delta_x_squared + delta_y_squared + delta_z_squared);
}

struct SpriteNumber {
    u8 unknown_00[40];
    const s16 *number;
};

s32 FindNearestF2Actor(void)
{
    struct EventWork *work = gEventWork;
    struct FieldActor **slot;
    struct FieldActor *origin;
    s32 nearest_actor = 0;
    s32 min_dist;
    u32 actor_id;

    min_dist = 640;
    origin = Object_GetById(ACTOR_PARTY_LEADER);
    actor_id = ACTOR_FIRST_PLACED;
    slot = work->placed_actors;
    do {
        struct FieldActor *actor = *slot++;
        if (actor != 0) {
            if (*((struct SpriteNumber *)actor->sprite)->number == 242) {
                s32 dist = FixedPoint_Distance(&origin->x.fixed, &actor->x.fixed);
                if (dist < min_dist) {
                    min_dist = dist;
                    nearest_actor = actor_id;
                }
            }
        }
        actor_id++;
    } while (actor_id <= 65);
    return nearest_actor;
}

void SceneActor_InitializeMotion(void)
{
    struct FieldActor *actor;
    s32 position[3];
    s32 angle;
    u8 flags;

    actor = Object_GetById(ACTOR_PARTY_LEADER);
    angle = (actor->facing + 0x1000) & 0xe000;
    flags = actor->motion_flags;
    position[0] = (actor->x.fixed & 0xfff00000) + 0x80000;
    position[1] = actor->y.fixed;
    position[2] = (actor->z.fixed & 0xfff00000) + 0x80000;
    Vector_AddPolarOffset(0x200000, angle, position);
    if (Object_CheckMovementCollision(actor, position) == 0) {
        Engine_GameFlagClear(592);
        SceneActor_SetMode55OnSevenRecords();
        Object_SetMode(actor, 6);
        Engine_TaskWait(6);
        Engine_AudioPlayCue(152);
        Object_SetMode(actor, 7);
        actor->speed = 0x30000;
        actor->acceleration = 0x20000;
        actor->velocity_y = 0x40000;
        actor->motion_flags &= 0x7e;
        SetOverlayObjectMode(actor, 0);
        Engine_ActorMoveToAndWait(ACTOR_PARTY_LEADER, position[0] >> 16, position[2] >> 16);
        Object_SetMode(actor, 6);
        SetOverlayObjectMode(actor, 1);
        actor->motion_flags = flags;
    }
}

void MakyuriChojo_StepOntoNearestActor(void)
{
    struct FieldActor *leader;
    struct FieldActor *actor;
    s32 id;

    leader = Object_GetById(ACTOR_PARTY_LEADER);
    Engine_EventBegin();
    id = FindNearestF2Actor();
    MakyuriChojo_NearestActor = id;
    if (id != 0) {
        Engine_GameFlagSet(0x250);
        actor = Object_GetById(MakyuriChojo_NearestActor);
        actor->motion_flags = 0;
        leader->motion_flags &= 0xfe;
        actor->y.fixed -= 0x30000;
        leader->y.fixed -= 0x30000;
        leader->terrain_height -= 0x30000;
        Engine_TaskWait(2);
        actor->y.fixed -= 0x20000;
        leader->y.fixed -= 0x20000;
        leader->terrain_height -= 0x20000;
        Engine_TaskWait(10);
        actor->y.fixed += 0x20000;
        leader->y.fixed += 0x20000;
        leader->terrain_height += 0x20000;
        Engine_TaskWait(4);
        actor->y.fixed += 0x20000;
        leader->y.fixed += 0x20000;
        leader->terrain_height += 0x20000;
        Engine_TaskWait(4);
        actor->y.fixed += 0x10000;
        leader->y.fixed += 0x10000;
        leader->terrain_height += 0x10000;
    }
    Engine_EventEnd();
}

void SceneActor_SetMode55OnSevenRecords(void)
{
    Object_GetById(ACTOR_PARTY_LEADER)->motion_flags = 3;
    Object_GetById(14)->motion_flags = 4;
    Object_GetById(15)->motion_flags = 4;
    Object_GetById(16)->motion_flags = 4;
    Object_GetById(17)->motion_flags = 4;
    Object_GetById(18)->motion_flags = 4;
    Object_GetById(19)->motion_flags = 4;
}

void RunScene58Sequence(void)
{
    struct FieldActor *actor;

    Engine_EventBegin();
    Actor_Get(17)->motion_flags &= 0xfa;
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x0000cccc, 0x00006666);
    Actor_SetSpeed(ACTOR_GERALD, 0x0000cccc, 0x00006666);
    Actor_SetSpeed(ACTOR_IVAN, 0x0000cccc, 0x00006666);
    Actor_SetSpeed(ACTOR_MIA, 0x0000cccc, 0x00006666);
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    if (actor != 0) {
        Actor_SetPosition(ACTOR_GERALD, actor->x.fixed, actor->z.fixed);
    }
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    if (actor != 0) {
        Actor_SetPosition(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
    }
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    if (actor != 0) {
        Actor_SetPosition(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
    }
    Engine_TaskWait(1);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 344, 232);
    Actor_WalkTo(ACTOR_GERALD, 328, 232);
    Actor_WalkTo(ACTOR_IVAN, 344, 248);
    Actor_WalkTo(ACTOR_MIA, 328, 248);
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
    Engine_ActorWaitForMove(ACTOR_GERALD);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTH, 0);
    Engine_ActorWaitForMove(ACTOR_IVAN);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
    Engine_ActorWaitForMove(ACTOR_MIA);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    Engine_EventWait(50);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Engine_EventWait(20);
    Camera_SetSpeed(0x18000, 0x3000);
    Camera_MoveTo(PIXELS(328), PIXELS(40), PIXELS(176), 1);
    Engine_EventGetViewCenter()->motion_flags = 0;
    Actor_SetSpeed(ACTOR_GERALD, 0x18000, 0xC000);
    Actor_WalkToAndWait(ACTOR_GERALD, 328, 216);
    Engine_CameraWaitForMove();
    Actor_SetSpeed(ACTOR_GERALD, 0x0000cccc, 0x00006666);
    Engine_EventWait(60);
    Camera_MoveTo(PIXELS(344), PIXELS(24), PIXELS(232), 1);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTHEAST, 20);
    Engine_CameraWaitForMove();
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 4);
    Engine_EventSetMessage((s32)MsgMakyuriLighthouseAlreadyLit);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_SetAttachedEffect(ACTOR_MIA, 258);
    Engine_EventWait(60);
    Actor_SetSpeed(ACTOR_MIA, 0x18000, 0xC000);
    Actor_WalkToAndWait(ACTOR_MIA, 328, 232);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_MIA, 4);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 30);
    Engine_ActorFaceEachOther(2, 0, 0);
    Engine_EventWait(60);
    Actor_WalkToAndWait(ACTOR_IVAN, 336, 248);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    Engine_EventWait(30);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Actor_FaceDirection(ACTOR_MIA, FACING_EAST, 30);
    Engine_EventOpenMessage(3, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(20);
        Actor_ShowEmote(ACTOR_MIA, 256, 60);
        Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTH, 10);
        Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3);
        Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
        Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
        Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 4);
        Engine_EventWait(20);
        Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
        gEventWork->message += 2;
    } else {
        gEventWork->message += 2;
        Engine_EventWait(20);
        Actor_ShowEmote(ACTOR_MIA, 257, 60);
        Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTH, 10);
        Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 4);
        Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
        Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
        Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
        Engine_EventWait(20);
        Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    }
    Actor_ShowEmote(ACTOR_MIA, 257, 60);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Engine_ActorFaceEachOther(0, 1, 0);
    Engine_EventWait(60);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTH, 20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_MIA, 2);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_MIA, 4);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_MIA, FACING_EAST, 10);
    Engine_ActorSetAnimation(ACTOR_MIA, 16);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 60);
    Audio_PlayCue(17);
    Actor_SetPosition(5, PIXELS(216), PIXELS(200));
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 20);
    Actor_SetPosition(5, PIXELS(120), PIXELS(160));
    Actor_FaceDirection(5, FACING_EAST, 0);
    Engine_ActorSetAnimation(ACTOR_MIA, 1);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 1);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 10);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTHEAST, 20);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 10);
    Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 5);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTH, 10);
    Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 5);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 5);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_FaceDirection(ACTOR_GERALD, FACING_WEST, 10);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Engine_EventWait(20);
    Camera_SetSpeed(0x30000, 0x6000);
    Camera_MoveTo(PIXELS(120), PIXELS(0xFFE8), PIXELS(168), 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(21, 1);
    Engine_EventWait(20);
    Audio_PlayCue(61);
    Event_ShowMessageAndWait(21, 0, 20);
    Engine_ActorRunRepeatedMotion(23, 1);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(23, 0, 20);
    Actor_FaceDirection(23, FACING_NORTH, 60);
    Actor_FaceDirection(23, FACING_EAST, 20);
    Event_ShowMessageAndWait(23, 0, 20);
    Actor_SetPosition(ACTOR_GERALD, PIXELS(264), PIXELS(288));
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_SetPosition(ACTOR_GERALD, PIXELS(328), PIXELS(216));
    Engine_ActorSetAnimationAndWait(23, 4);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(23, 0, 20);
    Engine_ActorSetAnimationAndWait(23, 3);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(23, 0, 20);
    Actor_FaceDirection(23, FACING_NORTHWEST, 10);
    Actor_FaceDirection(21, FACING_SOUTHEAST, 10);
    Engine_ActorRunRepeatedMotion(21, 1);
    Engine_EventWait(20);
    Actor_FaceDirection(5, FACING_WEST, 0);
    Actor_FaceDirection(20, FACING_WEST, 30);
    Actor_FaceDirection(21, FACING_EAST, 30);
    Engine_ActorRunRepeatedMotion(5, 2);
    Engine_ActorSetAnimationAndWait(21, 4);
    Engine_EventWait(20);
    Actor_FaceDirection(21, FACING_SOUTHEAST, 10);
    Engine_ActorSetAnimationAndWait(21, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(23, 3);
    Actor_SetSpeed(0x15, 0x0000cccc, 0x00006666);
    Actor_WalkToAndWait(21, 104, 168);
    Engine_EventWait(20);
    Actor_FaceDirection(23, FACING_EAST, 20);
    Actor_FaceDirection(20, FACING_EAST, 10);
    Actor_ShowEmote(5, 258, 0);
    Actor_ShowEmote(20, 258, 70);
    Actor_SetPosition(22, PIXELS(264), PIXELS(288));
    Event_ShowMessageAndWait(22, 0, 20);
    Actor_SetPosition(22, PIXELS(296), PIXELS(120));
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    Actor_FaceDirection(5, FACING_EAST, 0);
    Actor_FaceDirection(20, FACING_EAST, 0);
    Camera_SetSpeed(0x18000, 0x3000);
    Camera_MoveTo(PIXELS(232), PIXELS(40), PIXELS(152), 1);
    Actor_SetSpeed(0x16, 0x0000cccc, 0x00006666);
    Actor_WalkToAndWait(22, 272, 128);
    Actor_WalkToAndWait(22, 264, 152);
    Actor_WalkToAndWait(22, 280, 168);
    Engine_CameraWaitForMove();
    Actor_FaceDirection(0x16, 0x5000, 20);
    Engine_EventWait(20);
    Event_ShowMessage(23, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(22, 3);
    Engine_EventWait(20);
    Event_ShowMessage(22, 0);
    Camera_MoveTo(PIXELS(296), PIXELS(40), PIXELS(216), 1);
    Actor_WalkToAndWait(22, 304, 176);
    Actor_FaceDirection(22, FACING_SOUTHEAST, 0);
    Engine_CameraWaitForMove();
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 1);
    Engine_ActorStartRepeatedMotion(ACTOR_IVAN, 1);
    Engine_ActorRunRepeatedMotion(ACTOR_MIA, 1);
    Engine_EventWait(20);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(22, 3);
    Engine_EventWait(20);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 263, 0);
    Actor_ShowEmote(ACTOR_GERALD, 263, 0);
    Actor_ShowEmote(ACTOR_MIA, 263, 0);
    Actor_ShowEmote(ACTOR_IVAN, 263, 70);
    Engine_ActorRunRepeatedMotion(22, 1);
    Engine_EventWait(30);
    Event_ShowMessage(22, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 261, 0);
    Actor_ShowEmote(ACTOR_GERALD, 261, 0);
    Actor_ShowEmote(ACTOR_IVAN, 261, 0);
    Actor_ShowEmote(ACTOR_MIA, 261, 70);
    Engine_ActorSetAnimationAndWait(22, 3);
    Engine_EventWait(20);
    Event_ShowMessage(22, 0);
    Engine_EventWait(20);
    Event_ShowMessage(23, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(0x16, 0x5000, 0);
    Engine_EventWait(40);
    Event_ShowMessage(22, 0);
    Engine_EventWait(30);
    Event_ShowMessage(23, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(0x16, 0x3000, 0);
    Engine_EventWait(30);
    Event_ShowMessage(22, 0);
    Engine_EventWait(20);
    Actor_Get(ACTOR_GERALD)->steering_flags &= ~ACTOR_FACES_MOTION;
    Actor_WalkTo(ACTOR_GERALD, 328, 224);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 344, 224);
    Actor_WalkTo(ACTOR_IVAN, 344, 232);
    Engine_ActorWaitForMove(ACTOR_GERALD);
    Actor_Get(ACTOR_GERALD)->steering_flags |= ACTOR_FACES_MOTION;
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTH, 0);
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
    Engine_ActorWaitForMove(ACTOR_IVAN);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    Engine_EventWait(30);
    Actor_SetPosition(23, PIXELS(168), PIXELS(200));
    Event_ShowMessageAndWait(23, 0, 20);
    Actor_SetPosition(23, PIXELS(104), PIXELS(200));
    Engine_ActorRunRepeatedMotion(22, 1);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(22, 0, 20);
    Camera_SetSpeed(0x30000, 0x6000);
    Camera_MoveTo(PIXELS(120), PIXELS(0xFFE8), PIXELS(168), 1);
    Engine_CameraWaitForMove();
    Engine_ActorSetAnimationAndWait(23, 3);
    Engine_EventWait(10);
    Actor_FaceDirection(0x17, 0xD000, 20);
    Engine_ActorSetAnimationAndWait(23, 3);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(23, 0, 20);
    Actor_FaceDirection(5, 0x5000, 0);
    Actor_FaceDirection(0x14, 0x3000, 70);
    Engine_ActorFaceEachOther(5, 20, 0);
    Engine_EventWait(50);
    Actor_FaceDirection(5, 0x5000, 0);
    Actor_FaceDirection(0x14, 0x3000, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_JASMINE, 4);
    Engine_EventWait(20);
    Actor_ShowEmote(23, 256, 60);
    Event_ShowMessageAndWait(23, 0, 20);
    Actor_ShowEmote(5, 261, 60);
    Actor_FaceDirection(0x15, 0xD000, 20);
    Engine_ActorRunRepeatedMotion(21, 1);
    Engine_ActorSetAnimationAndWait(23, 3);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(23, 0, 20);
    Engine_ActorSetAnimationAndWait(23, 4);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(23, 0, 20);
    Actor_SetAttachedEffect(21, 258);
    Engine_EventWait(60);
    Engine_ActorSetAnimationAndWait(21, 4);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(21, 0, 20);
    Actor_SetAttachedEffect(ACTOR_JASMINE, 258);
    Engine_EventWait(60);
    Event_ShowMessageAndWait(5, 0, 40);
    Engine_ActorSetAnimationAndWait(21, 3);
    Engine_EventWait(60);
    Actor_FaceDirection(5, FACING_NORTHWEST, 10);
    Engine_ActorSetAnimationAndWait(20, 3);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(20, 0, 30);
    Actor_SetSpeed(5, 0x0000b333, 0x00005999);
    Actor_SetSpeed(0x14, 0x0000b333, 0x00005999);
    Actor_WalkTo(5, 128, 144);
    Actor_WalkToAndWait(20, 120, 136);
    Actor_FaceDirection(0x14, 0x5000, 0);
    Engine_ActorWaitForMove(5);
    Actor_FaceDirection(ACTOR_JASMINE, FACING_EAST, 20);
    Actor_FaceDirection(0x15, 0x3000, 20);
    Object_GetById(21)->steering_flags &= ~ACTOR_FACES_MOTION;
    Actor_WalkToAndWait(21, 88, 152);
    Actor_Get(21)->steering_flags |= ACTOR_FACES_MOTION;
    Actor_FaceDirection(0x17, 0xB000, 20);
    Engine_ActorSetAnimation(23, 3);
    Engine_ActorSetAnimationAndWait(21, 3);
    Engine_EventWait(40);
    Actor_SetSpeed(0x17, 0x30000, 0x20000);
    Actor_Get(23)->velocity_y = 0x40000;
    Audio_PlayCue(152);
    Actor_Get(23)->motion_flags &= 0x7e;
    SetOverlayObjectMode(Object_GetById(23), 0);
    Object_GetById(17)->motion_flags = 4;
    Engine_ActorMoveToAndWait(23, 104, 168);
    SetOverlayObjectMode(Object_GetById(23), 1);
    Actor_Get(23)->motion_flags = 3;
    Actor_FaceDirection(23, FACING_EAST, 30);
    Actor_FaceDirection(21, FACING_EAST, 10);
    Event_ShowMessageAndWait(21, 0, 20);
    Engine_ActorRunRepeatedMotion(5, 2);
    Engine_ActorSetAnimationAndWait(5, 4);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(5, 0, 20);
    Event_ShowMessageAndWait(23, 0, 20);
    Engine_ActorSetSpritePriority(17, 0);
    Engine_ActorSetSpritePriority(18, 0);
    MakyuriChojo_LowerCollectedActors();
    Engine_ActorSetSpritePriority(17, 1);
    Engine_ActorSetSpritePriority(18, 1);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHWEST, 0);
    Camera_MoveTo(PIXELS(304), PIXELS(32), PIXELS(216), 1);
    Engine_CameraWaitForMove();
    Actor_SetPosition(20, PIXELS(272), PIXELS(280));
    Event_ShowMessageAndWait(20, 0, 20);
    Actor_SetPosition(20, 0, 0);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 1);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_SetSpeed(ACTOR_GERALD, 0x18000, 0xC000);
    Actor_WalkToAndWait(ACTOR_GERALD, 312, 216);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTHWEST, 10);
    Event_ShowMessageAndWait(22, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    Actor_WalkToAndWait(22, 312, 184);
    Actor_FaceDirection(0x16, 0x3000, 20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_ShowEmote(ACTOR_GERALD, 256, 60);
    Actor_SetSpeed(ACTOR_GERALD, 0x20000, 0x10000);
    Actor_Get(ACTOR_GERALD)->steering_flags &= ~ACTOR_FACES_MOTION;
    Actor_WalkToAndWait(ACTOR_GERALD, 328, 224);
    Engine_EventWait(1);
    Actor_Get(ACTOR_GERALD)->steering_flags |= ACTOR_FACES_MOTION;
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 1);
    Engine_ActorStartRepeatedMotion(ACTOR_IVAN, 1);
    Engine_ActorRunRepeatedMotion(ACTOR_MIA, 1);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(22, 1);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(22, 0, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 4);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_IVAN, 1);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, 3);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Actor_ShowEmote(22, 257, 60);
    Engine_EventOpenMessage(22, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(22, 4);
        Engine_EventWait(20);
        Event_ShowMessageAndWait(22, 0, 20);
        gEventWork->message++;
    } else {
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(22, 4);
        Engine_EventWait(20);
        gEventWork->message++;
        Event_ShowMessageAndWait(22, 0, 20);
    }
    Engine_ActorRunRepeatedMotion(22, 2);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(22, 0, 20);
    Actor_WalkToAndWait(22, 328, 200);
    Engine_ActorRunRepeatedMotion(22, 2);
    Actor_FaceDirection(0x16, 0xB000, 20);
    Actor_Get(22)->steering_flags &= ~ACTOR_FACES_MOTION;
    Actor_WalkToAndWait(22, 336, 208);
    Engine_EventWait(1);
    Object_GetById(22)->steering_flags |= ACTOR_FACES_MOTION;
    Actor_ShowEmote(22, 258, 60);
    Actor_ShowEmote(ACTOR_MIA, 257, 60);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Event_ShowMessageAndWait(22, 0, 20);
    Actor_FaceDirection(0x16, 0x5000, 20);
    Engine_ActorRunRepeatedMotion(22, 2);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(22, 0, 20);
    Actor_WalkToAndWait(22, 336, 216);
    Party_SetFields1ceAnd1d0((s32)&SceneId_MakyuriChojo1, 2);
    /* FAKEMATCH: the do/while loads the game state's base before the 0x22b offset, which fixes their registers and literal-pool order. */
    do {
        gGameState.battle_start = 3;
    } while (0);
    BattleFx_SetWeightedResult(36, 2);
    Engine_EventEnd();
}

void RunScene59Sequence(void)
{

    Engine_EventBegin();
    Actor_Get(9)->motion_flags = 0;
    Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(344), PIXELS(224));
    Actor_SetPosition(ACTOR_GERALD, PIXELS(328), PIXELS(224));
    Actor_SetPosition(ACTOR_IVAN, PIXELS(344), PIXELS(232));
    Actor_SetPosition(ACTOR_MIA, PIXELS(328), PIXELS(232));
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    Actor_SetPosition(22, PIXELS(336), PIXELS(176));
    Engine_ActorSetAnimation(22, 9);
    SetOverlayObjectMode(Object_GetById(22), 0);
    Camera_MoveTo(PIXELS(336), -1, PIXELS(208), 0);
    Engine_TaskWait(1);
    Engine_MapRedraw();
    Engine_TaskWait(1);

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_GERALD, FACING_EAST, 20);
    Engine_EventSetMessage((s32)MsgMakyuriGot);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(22, 2);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(22, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTH, 20);
    Actor_ShowEmote(ACTOR_IVAN, 257, 60);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Engine_ActorRunRepeatedMotion(22, 2);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(22, 0, 30);
    Actor_SetPosition(24, PIXELS(312), PIXELS(112));
    Audio_PlayCue(288);
    Event_ShowMessageAndWait(24, 0, 20);
    Actor_ShowEmote(ACTOR_MIA, 256, 60);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 30);
    Audio_PlayCue(29);
    Camera_SetSpeed(0x0000cccc, 0x00001999);
    Engine_EventGetViewCenter()->motion_flags = 0;
    Camera_MoveTo(PIXELS(336), -1, PIXELS(168), 1);
    Engine_EventWait(20);
    Actor_SetSpeed(0x18, 0x0000cccc, 0x00006666);
    Actor_WalkToAndWait(24, 344, 136);
    Actor_FaceDirection(0x18, 0x5000, 20);
    Engine_CameraWaitForMove();
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 30);
    Camera_MoveTo(PIXELS(336), -1, PIXELS(184), 1);
    Actor_WalkToAndWait(24, 344, 160);
    Actor_WalkToAndWait(24, 328, 168);
    Actor_WalkToAndWait(24, 312, 176);
    Actor_FaceDirection(0x18, 0x3000, 20);
    Engine_CameraWaitForMove();
    Engine_ActorSetAnimationAndWait(24, 3);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(24, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_MIA, 2);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Engine_ActorSetAnimationAndWait(24, 4);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(24, 0, 20);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTHEAST, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 60);
    Actor_FaceDirection(0x18, 0xB000, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 20);
    Event_ShowMessageAndWait(24, 0, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_MIA, 4);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Engine_ActorStartRepeatedMotion(24, 2);
    Actor_ShowEmote(24, 256, 60);
    Actor_FaceDirection(0x18, 0x3000, 20);
    Event_ShowMessageAndWait(24, 0, 20);
    Actor_ShowEmote(ACTOR_MIA, 257, 60);
    Engine_ActorSetAnimationAndWait(24, 4);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(24, 0, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_MIA, 2);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Engine_ActorSetAnimationAndWait(24, 3);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(24, 0, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_MIA, 1);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_WalkToAndWait(24, 312, 184);
    Actor_FaceDirection(0x18, 0x3000, 20);
    Event_ShowMessageAndWait(24, 0, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_GERALD, FACING_EAST, 20);
    Engine_EventOpenMessage(1, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
        Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
        gEventWork->message++;
    } else {
        gEventWork->message++;
        Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 1);
        Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    }
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTH, 20);
    Engine_ActorSetAnimationAndWait(24, 3);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(24, 0, 20);
    Actor_FaceDirection(0x18, 0xD000, 20);
    Event_ShowMessageAndWait(24, 0, 20);
    Actor_FaceDirection(0x18, 0x3000, 20);
    Engine_ActorSetAnimationAndWait(24, 4);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(24, 0, 20);
    Actor_ShowEmote(ACTOR_MIA, 257, 60);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Engine_ActorSetAnimationAndWait(24, 4);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(24, 0, 20);
    Actor_FaceDirection(0x18, 0xB000, 20);
    Event_ShowMessageAndWait(24, 0, 20);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(24, 2);
    Engine_EventWait(30);
    Event_ShowMessage(24, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Engine_EventWait(30);
    Event_ShowMessage(1, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(24, FACING_SOUTHEAST, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(24, 4);
    Engine_EventWait(20);
    Event_ShowMessage(24, 0);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(24, 2);
    Engine_EventWait(30);
    Event_ShowMessage(24, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_IVAN, 256, 60);
    Event_ShowMessage(2, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(24, 3);
    Engine_EventWait(30);
    Event_ShowMessage(24, 0);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(24, 2);
    Engine_EventWait(30);
    Event_ShowMessage(24, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(0x18, 0x1000, 0);
    Engine_EventWait(30);
    Actor_ShowEmote(24, 264, 60);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(22, 2);
    Engine_EventWait(30);
    Engine_ActorSetAnimation(22, 8);
    Engine_EventWait(45);
    Engine_ActorSetAnimation(22, 1);
    SetOverlayObjectMode(Object_GetById(22), 1);
    Engine_EventWait(40);
    Actor_ShowEmote(ACTOR_IVAN, 258, 60);
    Event_ShowMessage(2, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(22, FACING_SOUTHEAST, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(22, 4);
    Engine_EventWait(30);
    Event_ShowMessage(22, 0);
    Engine_EventWait(20);
    Actor_FaceDirection(24, FACING_SOUTHEAST, 0);
    Engine_EventWait(30);
    Event_ShowMessage(24, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(ACTOR_MIA, 2);
    Engine_EventWait(30);
    Event_ShowMessage(3, 0);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(24, 22, 0);
    Engine_EventWait(35);
    Engine_ActorSetAnimationAndWait(22, 3);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(24, 3);
    Engine_EventWait(20);
    Actor_WalkToAndWait(24, 328, 176);
    Actor_FaceDirection(0x18, 0x3000, 20);
    Engine_ActorSetAnimation(24, 5);
    Engine_ActorSetAnimation(22, 7);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 1);
    Engine_EventWait(20);
    Event_ShowMessage(1, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(24, 257, 60);
    Engine_EventOpenMessage(24, 0);
    Engine_EventWait(30);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 2);
    Engine_ActorStartRepeatedMotion(ACTOR_IVAN, 2);
    Engine_ActorRunRepeatedMotion(ACTOR_MIA, 2);
    Actor_FaceDirection(ACTOR_GERALD, FACING_EAST, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 0);
    Engine_EventWait(20);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(30);
        Event_ShowMessage(24, 0);
        gEventWork->message++;
    } else {
        Engine_EventWait(30);

        gEventWork->message++;
        Event_ShowMessage(24, 0);
    }
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(24, 4);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    Engine_EventWait(20);
    Event_ShowMessage(24, 0);
    Engine_ActorRunRepeatedMotion(22, 1);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(22, 0, 20);
    MakyuriChojo_SinkActorPair();
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 2);
    Engine_ActorStartRepeatedMotion(ACTOR_IVAN, 2);
    Engine_ActorRunRepeatedMotion(ACTOR_MIA, 2);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 5);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTHEAST, 5);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 5);
    Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 5);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 5);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTHWEST, 5);
    Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 5);
    Actor_FaceDirection(ACTOR_MIA, FACING_EAST, 5);
    Engine_EventGetViewCenter()->motion_flags = 0;
    Camera_MoveTo(PIXELS(280), -1, PIXELS(232), 1);
    Engine_CameraWaitForMove();
    Actor_SetChildValue(22, 15);
    Actor_SetChildValue(24, 15);
    Actor_SetPosition(22, PIXELS(240), PIXELS(208));
    Actor_SetPosition(24, PIXELS(232), PIXELS(208));
    Actor_FaceDirection(0x16, 0x5000, 0);
    Actor_FaceDirection(0x18, 0x3000, 0);
    MakyuriChojo_RiseActorPair();
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 10);
    Actor_FaceDirection(ACTOR_GERALD, FACING_WEST, 15);
    Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 20);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 328, 208);
    Actor_WalkTo(ACTOR_IVAN, 336, 224);
    Actor_WalkToAndWait(ACTOR_GERALD, 312, 216);
    Actor_FaceDirection(ACTOR_GERALD, FACING_WEST, 0);
    Engine_ActorWaitForMove(ACTOR_IVAN);
    Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 30);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_MIA, 2);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_FaceDirection(ACTOR_GERALD, 0x5000, 20);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_FaceDirection(ACTOR_GERALD, FACING_WEST, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_IVAN, 1);
    Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 20);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_MIA, 4);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 20);
    Event_ShowMessageAndWait(24, 0, 20);
    Camera_SetSpeed(0x30000, 0x6000);
    {
        struct MapLayerScroll *lift;
        void **cells;
        Camera_MoveTo(PIXELS(152), -1, PIXELS(216), 1);
        lift = &((struct MapScrollWork *)gMapWork[0])->layers[2];
        lift->offset_y = 0x3800000;
        cells = gMapWork;
        Engine_MapRedraw();
        Engine_TaskWait(1);
        Actor_Get(9)->motion_flags = 0;
        Actor_SetPosition(9, PIXELS(104), PIXELS(264));
        Actor_Get(9)->y.fixed = -0x200000;
        Object_GetById(9)->target_y = -0x200000;
        Map_CopyCellsTo(29, 74, 4, 74, 5, 4);
        Engine_ActorSetSpritePriority(17, 0);
        Engine_ActorSetSpritePriority(18, 0);
        MakyuriChojo_RaiseCollectedActors();
        Engine_ActorSetSpritePriority(17, 1);
        Engine_ActorSetSpritePriority(18, 1);
        Engine_CameraWaitForMove();
        Engine_EventWait(30);
        Actor_FaceDirection(24, FACING_WEST, 20);
        Event_ShowMessageAndWait(24, 0, 20);
        Camera_MoveTo(PIXELS(280), -1, PIXELS(216), 1);
        Engine_CameraWaitForMove();
        Actor_FaceDirection(0x18, 0x3000, 20);
        Engine_ActorSetAnimation(24, 5);
        Event_ShowMessageAndWait(24, 0, 20);
        Engine_ActorRunRepeatedMotion(ACTOR_IVAN, 2);
        Engine_EventWait(20);
        Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 20);
        Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
        Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 1);
        Engine_EventWait(20);
        Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
        Actor_ShowEmote(24, 257, 60);
        Engine_EventOpenMessage(24, 0);
        if (Engine_EventChooseYesNo(0, 0) == 0) {
            Engine_EventWait(20);
            Engine_ActorRunRepeatedMotion(22, 2);
            Engine_EventWait(20);
            Event_ShowMessageAndWait(22, 0, 20);
            ((struct EventWork *)cells[19])->message++;
        } else {
            ((struct EventWork *)cells[19])->message++;
            Engine_EventWait(20);
            Event_ShowMessageAndWait(22, 0, 20);
        }
    }
    Engine_ActorRunRepeatedMotion(24, 2);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(24, 0, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 257, 0);
    Actor_ShowEmote(ACTOR_GERALD, 257, 0);
    Actor_ShowEmote(ACTOR_IVAN, 257, 0);
    Actor_ShowEmote(ACTOR_MIA, 257, 60);
    Engine_EventOpenMessage(24, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(20);
        Engine_ActorRunRepeatedMotion(24, 2);
        Engine_EventWait(20);
        Event_ShowMessageAndWait(24, 0, 20);
        gEventWork->message++;
    } else {
        gEventWork->message++;
        Engine_EventWait(20);
        Engine_ActorRunRepeatedMotion(24, 1);
        Engine_EventWait(20);
        Event_ShowMessageAndWait(24, 0, 20);
    }
    Engine_EventWait(20);
    MakyuriChojo_SinkActorPair();
    Engine_EventWait(20);
    Camera_MoveTo(PIXELS(128), -1, PIXELS(200), 1);
    Engine_CameraWaitForMove();
    Engine_ActorSetAnimation(22, 1);
    Engine_ActorSetAnimation(24, 1);
    Actor_SetChildValue(22, 15);
    Actor_SetChildValue(24, 15);
    Actor_SetPosition(22, PIXELS(120), PIXELS(152));
    Actor_SetPosition(24, PIXELS(112), PIXELS(160));
    Actor_FaceDirection(0x16, 0x5000, 0);
    Actor_FaceDirection(0x18, 0x3000, 0);
    MakyuriChojo_RiseActorPair();
    Engine_EventWait(30);
    Actor_FaceDirection(24, FACING_EAST, 20);
    Engine_EventOpenMessage(24, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_ActorSetAnimationAndWait(24, 3);
        Engine_EventWait(20);
        Event_ShowMessageAndWait(24, 0, 20);
        gEventWork->message++;
    } else {
        gEventWork->message++;
        Engine_ActorSetAnimationAndWait(24, 4);
        Engine_EventWait(20);
        Event_ShowMessageAndWait(24, 0, 20);
    }
    Event_ShowMessageAndWait(24, 0, 20);
    Engine_ActorRunRepeatedMotion(22, 1);
    Engine_EventWait(20);
    Actor_FaceDirection(22, FACING_EAST, 20);
    Event_ShowMessageAndWait(22, 0, 20);
    Engine_EventWait(20);
    Actor_FaceDirection(24, FACING_EAST, 20);
    Engine_ActorSetSpritePriority(17, 0);
    Engine_ActorSetSpritePriority(18, 0);
    MakyuriChojo_LowerCollectedActors();
    Engine_ActorSetSpritePriority(17, 1);
    Engine_ActorSetSpritePriority(18, 1);
    Audio_PlayCue(17);
    Camera_MoveTo(PIXELS(336), -1, PIXELS(216), 1);
    Engine_CameraWaitForMove();
    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, 4);
    Engine_EventWait(20);
    Audio_PlayCueFromEventWork();
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x3000, 22);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_ShowEmote(ACTOR_MIA, 261, 60);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 20);
    Engine_ActorSetAnimation(ACTOR_MIA, 16);
    Actor_Get(ACTOR_MIA)->scale_x = -0x10000;
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 40);
    Engine_ActorRunRepeatedMotion(ACTOR_IVAN, 2);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, 4);
    Engine_EventWait(20);
    Engine_EventOpenMessage(2, 0);
    Engine_ActorSetAnimation(ACTOR_MIA, 1);
    Actor_Get(ACTOR_MIA)->scale_x = 0x10000;
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTHEAST, 0);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 20);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_ActorSetAnimationAndWait(ACTOR_IVAN, 3);
        Engine_EventWait(20);
        Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
        gEventWork->message++;
    } else {
        gEventWork->message++;
        Engine_ActorRunRepeatedMotion(ACTOR_IVAN, 2);
        Engine_EventWait(20);
        Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    }
    Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, 3);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 30);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTHEAST, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 1);
    Engine_EventWait(10);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_MIA, 3);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_MIA, 1);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 257, 0);
    Actor_ShowEmote(ACTOR_GERALD, 257, 0);
    Actor_ShowEmote(ACTOR_IVAN, 257, 60);
    Actor_SetSpeed(ACTOR_MIA, 0x0000cccc, 0x00006666);
    Actor_SetSpeed(ACTOR_GERALD, 0x0000cccc, 0x00006666);
    Actor_WalkToAndWait(ACTOR_MIA, 328, 216);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
    Actor_Get(ACTOR_GERALD)->steering_flags &= ~ACTOR_FACES_MOTION;
    Actor_WalkTo(ACTOR_GERALD, 312, 200);
    Actor_WalkTo(ACTOR_MIA, 280, 216);
    Engine_ActorWaitForMove(ACTOR_GERALD);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTHWEST, 0);
    Engine_ActorWaitForMove(ACTOR_MIA);
    Engine_ActorRunRepeatedMotion(ACTOR_MIA, 2);
    Engine_EventWait(30);
    Actor_FaceDirection(ACTOR_MIA, FACING_EAST, 20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 1);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_MIA, 3);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_ActorStartRepeatedMotion(ACTOR_IVAN, 2);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 258);
    Actor_SetAttachedEffect(ACTOR_GERALD, 258);
    Actor_SetAttachedEffect(ACTOR_IVAN, 258);
    Engine_EventWait(60);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_MIA, 4);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_FaceDirection(ACTOR_MIA, FACING_EAST, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_MIA, 4);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 30);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTHEAST, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, 3);
    Engine_EventWait(30);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 40);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_MIA, 3);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 312, 216);
    Actor_WalkTo(ACTOR_GERALD, 312, 216);
    Actor_WalkTo(ACTOR_IVAN, 312, 216);
    Actor_WalkTo(ACTOR_MIA, 312, 216);
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Engine_ActorWaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Engine_ActorWaitForMove(ACTOR_IVAN);
    Actor_SetPosition(ACTOR_IVAN, 0, 0);
    Engine_ActorWaitForMove(ACTOR_MIA);
    Actor_SetPosition(ACTOR_MIA, 0, 0);
    {
        struct MapLayerScroll *lift = &((struct MapScrollWork *)gMapWork[0])->layers[2];

        lift->offset_y = 0x4000000;
    }
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Map_CopyCellsTo(4, 70, 4, 74, 5, 4);
    GameFlag_Set(0x880);
    GameFlag_Set(0x00000881);
    Engine_EventEnd();
}

void MakyuriChojo_BringLiftUp(void)
{
    struct MapLayerScroll *lift;

    lift = &((struct MapScrollWork *)gMapWork[0])->layers[2];
    Engine_EventBegin();
    lift->offset_y = 0x3800000;
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Object_GetById(9)->motion_flags = 0;
    Engine_ActorSetPosition(9, PIXELS(104), PIXELS(264));
    Object_GetById(9)->y.fixed = -0x200000;
    Object_GetById(9)->target_y = -0x200000;
    Engine_EventGetViewCenter()->motion_flags = 0;
    Engine_CameraSetSpeed(0xcccc, 0x1999);
    Engine_CameraMoveTo(PIXELS(128), -1, PIXELS(184), 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(30);
    Map_CopyCellsTo(29, 74, 4, 74, 5, 4);
    Engine_ActorSetSpritePriority(17, 0);
    Engine_ActorSetSpritePriority(18, 0);
    MakyuriChojo_RaiseCollectedActors();
    Engine_ActorSetSpritePriority(17, 1);
    Engine_ActorSetSpritePriority(18, 1);
    Engine_EventWait(20);
    Engine_GameFlagSet(0x251);
    Engine_EventEnd();
}

void MakyuriChojo_TakeLiftDown(void)
{
    Engine_EventBegin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 104, 152);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 60);
    Engine_ActorSetSpritePriority(17, 0);
    Engine_ActorSetSpritePriority(18, 0);
    MakyuriChojo_LowerCollectedActors();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_EventRequestExit(1);
    Engine_EventEnd();
}

void MakyuriChojo_SinkActorPair(void)
{
    struct EffectParams params;
    struct EffectParams *p;
    struct FieldActor *left;
    struct FieldActor *right;
    u32 i;
    s32 x;
    s32 y;
    s32 r;

    left = Object_GetById(22);
    right = Object_GetById(24);
    Engine_AudioPlayCue(190);
    Actor_SetChildValue(22, 256);
    Engine_ActorSetChildValue(24, 256);
    SetOverlayObjectMode(Object_GetById(22), 0);
    SetOverlayObjectMode(Object_GetById(24), 0);
    p = &params;
    p->count = 1;
    p->kind = 5;
    p->tile = 284;
    p->spread = 0x6666;
    p->rise = 0x30000;
    for (i = 0; i <= 31; i++) {
        Engine_EventWait(1);
        if (i & 1) {
            r = ((((u32)Engine_RandomNext() * 24) >> 16) << 16);
            x = left->x.fixed;
            x += r;
            x += -0xc0000;
            Effect_Spawn(x, left->y.fixed + ((((u32)Engine_RandomNext() << 5) >> 16) << 16) + -0x100000, left->z.fixed, 0, 0x40000, 0, 0x1b0000, p);
        } else {
            r = ((((u32)Engine_RandomNext() * 24) >> 16) << 16);
            x = right->x.fixed;
            x += r;
            x += -0xc0000;
            Effect_Spawn(x, right->y.fixed + ((((u32)Engine_RandomNext() << 5) >> 16) << 16) + -0x100000, right->z.fixed, 0, 0x40000, 0, 0x1b0000, p);
        }
    }
    Engine_ActorSetPosition(22, 0, 0);
    Engine_ActorSetPosition(24, 0, 0);
}

void MakyuriChojo_RiseActorPair(void)
{
    struct EffectParams params;
    struct EffectParams *p;
    struct FieldActor *left;
    struct FieldActor *right;
    u32 i;
    s32 x;
    s32 y;
    s32 r;

    left = Object_GetById(22);
    right = Object_GetById(24);
    Engine_AudioPlayCue(190);
    SetOverlayObjectMode(Object_GetById(22), 0);
    SetOverlayObjectMode(Object_GetById(24), 0);
    p = &params;
    p->count = 1;
    p->kind = 5;
    p->tile = 284;
    p->spread = 0x6666;
    p->rise = 0x30000;
    for (i = 0; i <= 31; i++) {
        Engine_EventWait(1);
        if (i & 1) {
            r = ((((u32)Engine_RandomNext() * 24) >> 16) << 16);
            x = left->x.fixed;
            x += r;
            x += -0xc0000;
            Effect_Spawn(x, left->y.fixed + ((((u32)Engine_RandomNext() << 5) >> 16) << 16) + 0x200000, left->z.fixed, 0, -0x40000, 0, 0x1b0000, p);
        } else {
            r = ((((u32)Engine_RandomNext() * 24) >> 16) << 16);
            x = right->x.fixed;
            x += r;
            x += -0xc0000;
            Effect_Spawn(x, right->y.fixed + ((((u32)Engine_RandomNext() << 5) >> 16) << 16) + 0x200000, right->z.fixed, 0, -0x40000, 0, 0x1b0000, p);
        }
        if (i == 20) {
            Actor_SetChildValue(22, 256);
            Engine_ActorSetChildValue(24, 256);
        }
    }
    Engine_ActorSetChildValue(22, 0);
    Engine_ActorSetChildValue(24, 0);
    SetOverlayObjectMode(Object_GetById(22), 1);
    SetOverlayObjectMode(Object_GetById(24), 1);
}

void MakyuriChojo_CollectBandSlots(s32 *out, s32 y)
{
    struct ActorTable *table;
    u32 slot;

    table = (struct ActorTable *)gEventWork;
    y = 64 - (y >> 20);
    for (slot = 0; slot <= 65; slot++) {
        struct FieldActor *actor = table->actors[slot];

        if (actor != 0) {
            s32 cell = actor->x.fixed >> 20;
            s32 row = actor->z.fixed >> 20;

            if (cell >= 4 && cell <= 8 && y + 8 <= row && row < y + 11)
                *out++ = slot;
        }
    }
}

void MakyuriChojo_LowerCollectedActors(void)
{
    s32 ids[5];
    struct MapLayerScroll *lift;
    u32 i;
    u32 j;
    u32 n;
    s32 speed;
    struct FieldActor *actor;

    lift = &((struct MapScrollWork *)gMapWork[0])->layers[2];
    speed = 0x1999;
    n = 0;
    for (i = 0; i <= 4; i++)
        ids[i] = 66;
    ((s32 (*)())MakyuriChojo_CollectBandSlots)(ids, lift->offset_y, lift);
    for (i = 0; i <= 4; i++) {
        if (ids[i] == 66)
            break;
        Object_GetById(ids[i])->motion_flags = 0;
        n++;
    }
    Engine_AudioPlayCue(223);
    for (i = 0; i <= 227; i++) {
        lift->offset_y -= speed;
        for (j = 0; j < n; j++) {
            Object_GetById(ids[j])->z.fixed += speed;
            actor = Object_GetById(ids[j]);
            actor->target_z = Object_GetById(ids[j])->z.fixed;
        }
        if ((i & 3) == 3)
            speed += 0x1999;
        if (speed > 0x17fff)
            speed = 0x18000;
        Engine_TaskWait(1);
    }
    for (i = 0; i < n; i++)
        Object_GetById(ids[i])->motion_flags = 0;
}

void MakyuriChojo_RaiseCollectedActors(void)
{
    s32 ids[5];
    struct MapLayerScroll *lift;
    u32 i;
    u32 j;
    u32 n;
    s32 speed;
    struct FieldActor *actor;

    lift = &((struct MapScrollWork *)gMapWork[0])->layers[2];
    speed = 0x18000;
    n = 0;
    for (i = 0; i <= 4; i++)
        ids[i] = 66;
    MakyuriChojo_CollectBandSlots(ids, lift->offset_y);
    for (i = 0; i <= 4; i++) {
        if (ids[i] == 66)
            break;
        Object_GetById(ids[i])->motion_flags = 0;
        n++;
    }
    Engine_AudioPlayCue(223);
    for (i = 0; i <= 85; i++) {
        lift->offset_y += speed;
        for (j = 0; j < n; j++) {
            Object_GetById(ids[j])->z.fixed -= speed;
            actor = Object_GetById(ids[j]);
            actor->target_z = Object_GetById(ids[j])->z.fixed;
        }
        if ((i & 3) == 3 && i > 75)
            speed += -0x3333;
        if (speed < 3276)
            speed = 3276;
        Engine_TaskWait(1);
    }
    lift->offset_y = 0x4000000;
    Engine_MapRedraw();
    Engine_TaskWait(2);
}

void FieldScene_ConfigureValue93Scene(void)
{
    Engine_PsynergyBegin(93, 1);
    Engine_PsynergySetTarget(24, 9);
    Engine_PsynergyRaiseHands();
    Engine_PsynergyPlayEffect(1);
    Engine_PsynergyLowerHands();
    BattleEffect_CleanupSceneObjects();
}

void SceneEffect_UpdateArcPosition(struct OverlayObject *object)
{
    struct OverlayObject *parent;
    u16 angle;

    parent = object->linked_object;
    angle = object->angle_64;
    object->coordinate_08 = parent->coordinate_08 + Engine_MathCos(angle) * (object->field_30 + 28);
    object->coordinate_10 = (Engine_MathSin(angle) << 4) + 0x900000;
    object->coordinate_38 = object->coordinate_08;
    object->coordinate_40 = object->coordinate_10;
    object->angle_64 -= 512;
}

struct LampSpark {
    u8 unknown_00[48];
    s32 swing;
    u8 unknown_34[28];
    struct FieldSprite *sprite;
    u8 unknown_54;
    u8 motion_flags;
    u8 unknown_56[14];
    u16 angle;
    u16 turn;
    struct FieldActor *source;
    void (*update)();
};


void MakyuriChojo_FlickerActorEight(void)
{
    struct FieldActor *lamp;
    struct LampSpark *spark;
    struct FieldSprite *sprite;
    struct MapScrollWork *map;
    s32 height;
    s32 scale;
    s32 tick;
    u32 angle;

    lamp = Object_GetById(8);
    map = gMapWork[0];
    height = ((u32)(Engine_RandomNext() * 48) >> 16) << 16;
    if (((union FieldCoordinate *)&map->view_y)->part.pixel <= 129) {
        if (gFrameCount & 1) {
            Engine_ActorSetPosition(8, 0x1300000, 0x900000);
            scale = 0x10000;
            Object_GetById(8)->scale_x = scale;
        } else {
            Engine_ActorSetPosition(8, 0x1300000, 0x970000);
            scale = 0x14ccc;
            Object_GetById(8)->scale_x = scale;
        }
        Object_GetById(8)->scale_y = scale;
    } else {
        Actor_SetPosition(8, 0x80000, 0x80000);
    }
    if (lamp == NULL) {
        return;
    }
    tick = gFrameCount & 15;
    if (tick != 0) {
        return;
    }
    spark = (struct LampSpark *)Object_Create(0x11c, lamp->x.fixed + 0x80000,
                                              lamp->y.fixed + height + 0x80000, lamp->z.fixed);
    height = height / 0x60000;
    height <<= 16;
    if (spark == NULL) {
        return;
    }
    sprite = spark->sprite;
    Engine_ObjectSetScript((struct FieldActor *)spark, Data_02003c54);
    ObjectGroup_SetChildValue((struct FieldActor *)spark, 3);
    spark->motion_flags = tick;
    angle = Engine_RandomNext() & 0x0ffff000;
    spark->angle = angle;
    spark->turn = tick;
    spark->source = lamp;
    spark->update = SceneEffect_UpdateArcPosition;
    spark->swing = (Engine_MathSin((height & 0xfffff) >> 4) * 24) >> 16;
    /* FAKEMATCH: the byte view keeps this store ahead of the lamp's reload; the field store lets the reload schedule first (score 210). */
    *(u8 *)&sprite->flags = 0;
    sprite->priority = lamp->sprite->priority;
}
