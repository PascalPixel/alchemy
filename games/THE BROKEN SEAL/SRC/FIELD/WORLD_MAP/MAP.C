#include "GLOBAL_CELLS.H"
#include "STORY.H"
#include "TYPES.H"
#include "FIELD_SERVICE.H"
#include "IWRAM_CALL.H"
#include "SCENE_IDS.H"
#include "CALL.H"

#define FrameCounter gFrameCount
void Vector_AddPolarOffset(s32 radius, s32 angle, union FieldCoordinate *pos);

enum {
    CONTACT_LAST_ACTOR = 65,
    /* An actor's touch trigger is its id plus this base. */
    CONTACT_TRIGGER_BASE = 100,
    FLAG_CONTACT_PAUSED = 0x163,
    FLAG_CONTACT_BLOCKED = 0x104
};

extern u8 gDebugMode[];

void Scene_RunScene371SequenceA(s32 direction);
void Event_SetPairWork1c0(s32 a0, s32 a1);
void PartyInventory_Discard(s32 item);
void Engine_CameraSetSpeed(s32 speed, s32 acceleration);
void Map_SetWindowCellTile(s32 a0, s32 a1, s32 a2, s32 a3);
s32 GameFlag_GetByte(s32 flag);
void WorldMap_ActivateSite138(s32 actor);
void WorldMap_RunBlackOrbScene(void);
void RunEventScript01(void);
void FieldScene_RunActorTransferSequence(void);
void FieldScene_RunScene371_020017fc(void);
void FieldScene_RunScene371_02001888(void);
void FieldScene_RunScene371_02001938(void);
void FieldScene_RunScene371_020019e8(void);
void FieldScene_RunScene371_02001a98(void);
void FieldScene_RunScene371_02001b5c(void);
void FieldScene_RunScene371_02002274(void);
void FieldScene_RunScene371_0200357c(void);
void StoryScene_StartTransition(void);
void FieldScene_RunActorEightApproach(void);
void FieldScene_RunActorPresentationSequence(void);
void MapActor_UpdateContact(void);
void StoryScene_UpdateSelectedActorProgress(void);
extern s32 gWorldMapTriggerActor;

extern u8 MsgWorldMapAfterBringingDjinniIntoYour[];
extern u8 MsgWorldMapNextIllShowHowCan[];
extern u8 MsgWorldMapWeCantStayAnotherMinute[];

/*
 * The progress word is game-state halfword 284 read as a whole word and the level
 * word is the workspace at +428; both offsets are built in one register, so
 * the order in which the locals are declared is what reproduces the
 * reference.
 */
void StoryProgress_TriggerEvent0808(void)
{

    u8 *workspace = (u8 *)gEventWork;
    s16 *state_table = (s16 *)&gGameState;
    s32 *progress = (s32 *)&state_table[284];
    s32 *level = (s32 *)(workspace + 428);

    if (*progress >= Math_Divide(*level * 9, 10)) {
        if ((u32)Engine_RandomNext() < 0x8000) {
            BattleFx_SetPhaseRequest(0x808, 3);
            *(s32 *)(workspace + 424) = 0;
        } else {
            *progress = *level;
        }
    }
}

void StoryProgress_TriggerEvent0809(void)
{

    s16 *state_table = (s16 *)&gGameState;
    s32 *progress = (s32 *)&state_table[284];
    u8 *workspace = (u8 *)gEventWork;
    s32 *level = (s32 *)(workspace + 428);

    if (*progress >= Math_Divide(*level * 9, 10)) {
        BattleFx_SetPhaseRequest(0x809, 42);
        *(s32 *)(workspace + 424) = 0;
    }
}

void StoryProgress_TriggerEvent080A(void)
{

    s16 *state_table = (s16 *)&gGameState;
    s32 *progress = (s32 *)&state_table[284];
    u8 *workspace = (u8 *)gEventWork;
    s32 *level = (s32 *)(workspace + 428);

    if (*progress >= Math_Divide(*level * 9, 10)) {
        BattleFx_SetPhaseRequest(0x80a, 24);
        *(s32 *)(workspace + 424) = 0;
    }
}

void StoryActor_AdvanceTimer(u8 *actor)
{
    u16 *timer = (u16 *)(actor + 0x64);

    /*
     * Arm order decides the branch sense: the fall-through is the increment
     * and the taken branch is the call.  Swapping the arms inverts the test.
     */
    if (*(s16 *)timer <= 0) {
        *timer = (u16)(*timer + 1);
    } else {
        Engine_ObjectDispatchRelease(actor);
    }
}

void StoryActor_ConfigureSpawnedObject(u8 *actor)
{

    s32 fixed_scale;
    u8 *spawned_actor;
    u8 *spawned_record;

    if ((gFrameCount & 4) != 0) {
        fixed_scale = 0x14ccc;
        *(s32 *)(actor + 0x18) = fixed_scale;
        *(s32 *)(actor + 0x1c) = fixed_scale;
    } else {
        fixed_scale = 0x10000;
        *(s32 *)(actor + 0x18) = fixed_scale;
        *(s32 *)(actor + 0x1c) = fixed_scale;
    }

    if ((gFrameCount & 2) == 0) {
        return;
    }

    {
        s32 x = *(s32 *)(actor + 0x08);
        s32 y = *(s32 *)(actor + 0x0c);
        s32 z = *(s32 *)(actor + 0x10);
        spawned_actor = Engine_ObjectCreate(0x11d, x, y, z);
    }
    Engine_AudioPlayCue(0xf6);
    if (spawned_actor == 0) {
        return;
    }

    {
        u8 *spawned_flags = spawned_actor + 0x55;
        s32 zero_value = 0;

        *spawned_flags = zero_value;
        spawned_record = *(u8 **)(spawned_actor + 0x50);
        ((StorySpawnRecord *)spawned_record)->field = 1;
        Engine_ActorSetSpriteFlags(spawned_actor, 0);
        Object_SetMode(spawned_actor, 1);
        *(u16 *)(spawned_actor + 0x64) = zero_value;
        *(s32 *)(spawned_actor + 0x6c) = (s32)StoryActor_AdvanceTimer;
    }
}

/* Blink the actor's palette with the frame counter and, while it is not held,
 * bob it on its sine path and advance the angle. */
void WorldMap_UpdateBobbingMarker(struct FieldActor *actor)
{
    s16 *angle;

    if (FrameCounter & 2)
        Engine_ObjectSetPartPalettes(actor, 10);
    else
        Engine_ObjectSetPartPalettes(actor, 7);
    if ((s16)actor->unknown_66 == 0) {
        angle = (s16 *)&actor->unknown_64;
        actor->x.fixed = 0x15d00000;
        actor->y.fixed = Iwram_MulQ16(Engine_MathSin(*angle << 3), 0x40000) + 0x100000;
        actor->z.fixed = 0x5300000;
        Vector_AddPolarOffset(0x100000, *angle, &actor->x);
        actor->facing = *angle + 0x4000;
        *angle += 0x400;
    }
}

s32 StoryActor_Initialize(u8 *actor)
{
    u8 *actor_flags;
    s32 fixed_scale;

    if (Engine_GameFlagIsSet(0x30) != 0) {
        return 0;
    }
    if (Engine_GameFlagIsSet(0x16E) != 0) {
        return 0;
    }
    *(s32 *)(actor + 0x6C) = (s32)WorldMap_UpdateBobbingMarker;
    actor_flags = actor + 0x55;
    *actor_flags = 0;
    actor_flags += 0xF;
    *(u16 *)actor_flags = 0;
    actor_flags += 2;
    *(u16 *)actor_flags = 0;
    fixed_scale = 0x8000;
    *(s32 *)(actor + 0x18) = fixed_scale;
    *(s32 *)(actor + 0x1C) = fixed_scale;
    return 0;
}

u8 *WorldMap_GetEntrances(void)
{
    return gWorldMapEntrances;
}

s32 WorldMap_GetRegions(void)
{
    return 0;
}

u8 *WorldMap_GetExits(void)
{
    return gWorldMapExits;
}

s32 StoryActor_ApplyFlaggedMode(u8 *actor)
{
    StoryActor_ApplyMapRotation();
    if (Engine_GameFlagIsSet(0x847) != 0) {
        Object_SetMode(actor, 2);
    }
    return 1;
}

/* Turns the actor's sprite to the map's rotation. */
s32 StoryActor_ApplyMapRotation(struct FieldActor *actor)
{
    struct MapRenderWork *work = gMapWork[0];
    struct FieldSprite *sprite = actor->sprite;

    sprite->rotation = work->rotation;
    sprite->flags = 0;
    return 1;
}

/* Sets the actor's first collision flag and turns its sprite to the map's rotation. */
s32 StoryActor_ApplyMapRotationWithCollision(struct FieldActor *actor)
{
    struct MapRenderWork *work = gMapWork[0];
    struct FieldSprite *sprite = actor->sprite;

    actor->collision_flags |= 1;
    sprite->rotation = work->rotation;
    return 1;
}

s32 StoryActor_ResetPosition(u8 *actor)
{
    s32 zero;
    Engine_ActorSetSpriteFlags(actor, 0);
    Engine_ObjectSetPartPalettes(actor, 10);
    {
        u8 *mode_flags = actor + 0x59;
        zero = 0;
        *mode_flags = zero;
    }
    if (Engine_GameFlagIsSet(0x8A0) != 0) {
        Engine_GameFlagSet(0x2f1);
        *(s32 *)(actor + 8) = zero;
        *(s32 *)(actor + 12) = zero;
    }
    return 0;
}

/*
 * The popped register is r1, so r0 survives the return and is the result.
 * The owner includes its alignment halfword and its one pool word, the
 * address of gFrameCount -- a live status word, not overlay image data.
 * The exclusive or against the bit just tested clears bit 0 only.  value is
 * a byte, so value >> 8 is zero and both branches do the same clear and
 * store; that shape is deliberate and decides the register allocation.
 */
s32 StoryActor_ClearActiveFlag(u8 *actor)
{

    u8 *active_flags = actor + 0x54;
    u8 value = *active_flags;
    u32 active_bit = 1 + (value >> 8);

    if ((active_bit & value) != 0 && (gFrameCount & active_bit) != 0) {
        u8 cleared = 1;

        if (active_flags) {
            cleared ^= value;
            *active_flags = (u8)cleared;
        } else {
            cleared ^= value;
            *active_flags = (u8)cleared;
        }
    }
    return 1;
}

/*
 * Selects the placement and spawn table for the current scene. The selector is
 * the signed game-state halfword 225; only 49 through 80 are covered and
 * everything else takes the default arm, which calls Engine_GameFlagSet before
 * returning. Cases 49 and 64 are conditional and fall through to the default
 * when their test fails. The case arms are in the order the reference uses,
 * not ascending, and that order is what reproduces it.
 */
struct ScenePlacement *StoryScene_SelectPlacementTable(void)
{
    s16 *scene_table = (s16 *)&gGameState;
    s32 scene_id = scene_table[225];

    switch (scene_id) {
    case 49:
        if (Engine_GameFlagIsSet(0x94f) == 0 && Engine_GameFlagIsSet(0x941) != 0) {
            return gWorldMapPlacements49;
        }
        break;
    case 64:
        if (Engine_GameFlagIsSet(0x85a) == 0) {
            return gWorldMapPlacements64;
        }
        break;
    case 65:
    case 70:
        return gWorldMapPlacements65;
    case 71:
        return gWorldMapPlacements71;
    case 72:
        return gWorldMapPlacements72;
    case 73:
        return gWorldMapPlacements73;
    case 66:
    case 67:
    case 68:
    case 69:
    case 75:
        return gWorldMapPlacements66;
    case 80:
        return gWorldMapPlacements80;
    default:
        break;
    }

    Engine_GameFlagSet(0x235);
    return gWorldMapPlacements;
}

/* Publishes one of two branch values at +0x170 of the scene state, chosen by
 * comparing the other actor's x against the subject's. */
void StoryScene_SetBranchValueFromX(
    u8 *actor_object, s32 val_lower,
    s32 val_other)
{

    u8 *scene_state;
    s16 *scene_table;
    struct Object *subject_actor;
    struct Object *other_actor;

    subject_actor = Object_GetById(actor_object - 0x64);
    scene_table = (s16 *)&gGameState;
    other_actor = Object_GetById(*(s32 *)&scene_table[250]);
    scene_state = *(u8 **)&gEventWork;
    if (other_actor->x < subject_actor->x) {
        *(u16 *)(scene_state + 0x170) = val_lower;
    } else {
        *(u16 *)(scene_state + 0x170) = val_other;
    }
    Engine_AudioPlayCue(0x7B);
}

/* The same branch value, chosen on z instead of x. */
void StoryScene_SetBranchValueFromZ(
    u8 *actor_object, s32 val_lower,
    s32 val_other)
{

    u8 *scene_state;
    s16 *scene_table;
    struct Object *subject_actor;
    struct Object *other_actor;

    subject_actor = Object_GetById(actor_object - 0x64);
    scene_table = (s16 *)&gGameState;
    other_actor = Object_GetById(*(s32 *)&scene_table[250]);
    scene_state = *(u8 **)&gEventWork;
    if (other_actor->z < subject_actor->z) {
        *(u16 *)(scene_state + 0x170) = val_lower;
    } else {
        *(u16 *)(scene_state + 0x170) = val_other;
    }
    Engine_AudioPlayCue(0x7B);
}

void SceneState_SetValues130_6_47(void)
{
    StoryScene_SetBranchValueFromX(0x82, 6, 0x2F);
}

void SceneState_ApplyValues150And46And11(void)
{
    StoryScene_SetBranchValueFromZ(0x96, 0x2E, 0x0B);
}

void SceneState_ApplyValues116And56And21(void)
{
    StoryScene_SetBranchValueFromZ(0x74, 0x38, 0x15);
}

void SceneState_ApplyValues151And25And54(void)
{
    StoryScene_SetBranchValueFromZ(0x97, 0x19, 0x36);
}

void FieldScene_RunStep7D3B1E(void)
{

    StoryScene_SetBranchValueFromZ(0x7D, 0x3B, 0x1E);
}

struct SceneEvent *WorldMap_GetEvents(void)
{
    return gWorldMapEvents;
}

/*
 * Marks each placed actor active while it stands inside a window around the
 * view centre: 160 pixels to either side, 300 pixels toward lower depth and
 * 200 toward higher depth. When an active actor comes within reach of the
 * selected actor, measured as the sum of the axis distances against both
 * scaled radii, its trigger is recorded as touched unless flag 0x104 is set.
 */
void MapActor_UpdateContact(void)
{
    struct FieldActor *leader;
    struct EventWork *work;
    struct FieldActor *actor;
    s32 left;
    s32 right;
    s32 top;
    s32 bottom;
    s32 leader_reach;
    u32 i;

    leader = Object_GetById(gGameState.selected_actor);
    leader_reach = leader->sprite->scale * leader->radius;
    work = gEventWork;
    actor = work->view_center;
    left = actor->x.fixed - PIXELS(160);
    right = actor->x.fixed + PIXELS(160);
    top = actor->z.fixed - PIXELS(300);
    bottom = actor->z.fixed + PIXELS(200);

    for (i = ACTOR_FIRST_PLACED; i <= CONTACT_LAST_ACTOR; i++) {
        s32 x;
        s32 z;
        s32 dx;
        s32 reach;
        s32 scale;

        actor = Engine_ActorLookup(i);
        if (actor == NULL) {
            continue;
        }
        x = actor->x.fixed;
        z = actor->z.fixed;
        if (x < left || x > right || z < top || z > bottom) {
            actor->active = 0;
            continue;
        }
        actor->active = 1;
        if (gDebugMode[0] != 0 && Engine_GameFlagIsSet(FLAG_CONTACT_PAUSED) != 0) {
            continue;
        }
        scale = actor->sprite->scale;
        dx = actor->x.fixed - leader->x.fixed;
        if (dx < 0) {
            dx = leader->x.fixed - actor->x.fixed;
        }
        reach = leader_reach + actor->radius * scale;
        if (dx + (actor->z.fixed - leader->z.fixed < 0 ? leader->z.fixed - actor->z.fixed
                                                       : actor->z.fixed - leader->z.fixed)
                < reach
            && Engine_GameFlagIsSet(FLAG_CONTACT_BLOCKED) == 0) {
            work->touched_trigger = i + CONTACT_TRIGGER_BASE;
        }
    }
}

void SceneState_ApplyFlag85aBranch(void)
{
    if (Engine_GameFlagIsSet(0x85a) == 0) {
        Engine_EventRequestExit(101);
    } else {
        Engine_AudioPlayCue(123);
        Engine_EventRequestExit(3);
    }
}

void FieldScene_RunStep74(void)
{

    Engine_EventBegin();
    Engine_EventRequestExit(74);
}

/* World map entry: record the arrival and start the map's camera and tasks, then run the scene the entrance or the story flags call for. */
s32 WorldMap_EnterScene(void)
{
    s16 *entrance;
    u8 *state;

    state = (u8 *)&gGameState;
    entrance = &((struct GameState *)state)->entrance;
    if (*entrance == 99) {
        Engine_GameFlagSet(0x160);
        Engine_GameFlagSet(0x161);
        Engine_GameFlagSet(0x163);
    }
    if (*entrance == 90) {
        Scene_RunScene371SequenceA(0);
        Event_SetPairWork1c0((s32)&SceneId_MakyuriChojo1, 1);
    } else if (*entrance == 91) {
        Scene_RunScene371SequenceA(1);
        Event_SetPairWork1c0((s32)&SceneId_VinasuChojo, 93);
    } else if (*entrance == 78) {
        Engine_EventBegin();
        PartyInventory_Discard(242);
        Engine_EventRequestExit(112);
    } else {
        Engine_GameFlagSet(0x144);
        gEventWork->start_transition = 0x400;
        gEventWork->transition_frames = 16;
        Engine_TaskWait(1);
        Engine_CameraSetSpeed(0x80000, 0x10000);
        Engine_GameFlagClear(0x12f);
        Engine_TaskAddCallback(MapActor_UpdateContact, 0xc80);
        if (Engine_GameFlagIsSet(0x90a) == 0) {
            Call4(Map_SetWindowCellTile, 128, 256, 176, 56);
        }
        switch (*entrance) {
        case 1:
            if (Value1(Engine_GameFlagIsSet, 0x815) == 0) {
                Engine_GameFlagSet(0x815);
                Engine_GameFlagSet(0x85c);
            }
            break;
        case 33:
            if (Engine_GameFlagIsSet(0x109) != 0) {
                if (Engine_GameFlagIsSet(0x85d) == 0 && Engine_GameFlagIsSet(0x234) != 0) {
                    gWorldMapTriggerActor = 55;
                    Call3(Engine_ActorSetPosition, 55, 0x17940000, 0xd480000);
                    Object_GetById(gWorldMapTriggerActor)->facing = 0x3000;
                    WorldMap_ActivateSite138(gWorldMapTriggerActor);
                }
            } else if (Engine_GameFlagIsSet(0x85d) == 0 && Engine_GameFlagIsSet(0x9b8) == 0) {
                WorldMap_RunBlackOrbScene();
            }
            break;
        case 49:
            if (Engine_GameFlagIsSet(0x94f) == 0 && Engine_GameFlagIsSet(0x941) != 0) {
                RunEventScript01();
            }
            break;
        case 64:
            if (Engine_GameFlagIsSet(0x85a) == 0) {
                FieldScene_RunActorTransferSequence();
            }
            break;
        case 65:
            FieldScene_RunScene371_020017fc();
            break;
        case 66:
            FieldScene_RunScene371_02001888();
            break;
        case 67:
            FieldScene_RunScene371_02001938();
            break;
        case 68:
            FieldScene_RunScene371_020019e8();
            break;
        case 69:
            FieldScene_RunScene371_02001a98();
            break;
        case 70:
            FieldScene_RunScene371_02001b5c();
            break;
        case 71:
            FieldScene_RunScene371_02002274();
            break;
        case 72:
            FieldScene_RunScene371_0200357c();
            break;
        case 73:
            StoryScene_StartTransition();
            break;
        case 74:
        case 76:
        case 77:
            Engine_GameFlagSet(0x11c);
            if (GameFlag_GetByte(0x2f8) != 0) {
                state = (u8 *)&gGameState;
                state[0x1f2] = 2;
                Engine_TaskAddCallback(StoryScene_UpdateSelectedActorProgress, 0xc80);
            }
            break;
        case 75:
            FieldScene_RunActorEightApproach();
            break;
        case 80:
            FieldScene_RunActorPresentationSequence();
            break;
        default:
            Object_GetById(53)->scale_x = 0x14000;
            Object_GetById(53)->scale_y = 0x14000;
            break;
        }
    }
    return 0;
}

void RunEventScript01(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Battle_SetObjectFlag5bWhenMode3();
    Engine_GameFlagSet(0x94f);
    Engine_ActorSetPosition(11, 0x16e00000, 0x49c0000);
    Engine_ActorSetDestinationOffset(11, 24, 8);
    Engine_ActorWaitForMove(11);
    Engine_EventWait(60);
    Engine_ActorSetPosition(12, 0x16e00000, 0x49c0000);
    Engine_ActorSetDestinationOffset(12, 12, 24);
    Engine_EventWait(30);
    Engine_ActorFaceDirection(11, 0x5000, 0);
    Engine_ActorFaceDirection(12, 0xd000, 0);
    Engine_EventWait(60);
    Engine_ActorSetAnimation(11, 3);
    Engine_ActorSetAnimation(12, 3);
    Engine_EventWait(120);
    Engine_ActorSetPosition(8, 0x16f80000, 0x4b80000);
    Engine_EventWait(60);
    Engine_ActorSetAnimation(12, 2);
    record = Object_GetById(8);
    if (record != 0) {
        Engine_ActorSetDestination(12, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(12);
    Engine_ActorSetPosition(12, 0, 0);
    Engine_EventWait(60);
    Engine_ActorSetAnimation(11, 2);
    record = Object_GetById(8);
    if (record != 0) {
        Engine_ActorSetDestination(11, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(11);
    Engine_ActorSetPosition(11, 0, 0);
    Engine_EventWait(60);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
    record = Object_GetById(8);
    if (record != 0) {
        Engine_ActorSetDestination(ACTOR_PARTY_LEADER, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Engine_ActorSetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Engine_EventWait(60);
    Engine_ActorSetSpeed(8, 0x8000, 0x4000);
    Engine_ActorSetDestinationOffset(8, 56, 8);
    Engine_ActorWaitForMove(8);
    Engine_ActorSetDestinationOffset(8, 40, 40);
    Engine_ActorWaitForMove(8);
    Engine_ActorSetDestinationOffset(8, 8, 88);
    Engine_ActorWaitForMove(8);
    Engine_EventCloseScreen();
    Engine_EventRequestExit(108);
    Engine_EventEnd();
}

void FieldScene_RunActorTransferSequence(void)
{
    u8 *actor;
    u8 *record;
    s32 scale;
    s32 action;

    actor = Object_GetById(15);
    Engine_EventBegin();
    BattleFx_ScheduleRatioTransition(0x14000, 1);
    Engine_TaskWait(4);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Battle_SetObjectFlag5bWhenMode3();
    Engine_CameraMoveTo(-1, -1, -1, 0);
    Engine_TaskWait(1);
    Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, 0x19999, 0xcccc);
    Engine_ActorSetSpeed(ACTOR_GERALD, 0x19999, 0xcccc);
    Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 0x16fc, 0x628);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Engine_ActorSetPosition(8, 0x16d80000, 0x6280000);
    Engine_TaskWait(1);
    Engine_ActorSetChildValue(8, 15);
    record = Object_GetById(8);
    Engine_ActorSetSpriteFlags(record, 0);
    Engine_ActorSetSpeed(10, 0x19999, 0x6666);
    Engine_ActorSetSpeed(11, 0x19999, 0x6666);
    Engine_ActorSetSpeed(12, 0x19999, 0x6666);
    Engine_ActorSetSpeed(13, 0x19999, 0x6666);
    Engine_AudioPlayCue(141);
    Engine_ActorEnableActionCallback(10, gTransferArrive10);
    Engine_EventWait(20);
    Engine_ActorEnableActionCallback(11, gTransferArrive11);
    Engine_EventWait(20);
    Engine_ActorEnableActionCallback(12, gTransferArrive12);
    Engine_EventWait(20);
    Object_SetActionCallbackAndRefreshById(13, (s32)gTransferArrive13);
    Engine_AudioPlayCue(0x121);
    record = Object_GetById(ACTOR_PARTY_LEADER);
    if (record != 0) {
        Engine_ActorSetPosition(ACTOR_GERALD, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Engine_ActorWalkToAndWait(ACTOR_GERALD, 0x1704, 0x640);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
    Engine_ActorFaceDirection(ACTOR_GERALD, 0xa000, 20);
    Engine_ActorShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Engine_ActorShowEmote(ACTOR_GERALD, 0x101, 60);
    Engine_ActorSetPosition(8, 0x16d80000, 0x6380000);
    Engine_TaskWait(1);
    Engine_EventSetMessage((s32)MsgWorldMapWeCantStayAnotherMinute);
    Engine_EventShowMessageAndWait(8, 0, 10);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Engine_ActorFaceDirection(ACTOR_GERALD, 0x6000, 40);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
    Engine_ActorFaceDirection(ACTOR_GERALD, 0xa000, 60);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Engine_ActorFaceDirection(ACTOR_GERALD, 0x6000, 10);
    Engine_AudioPlayCue(141);
    Engine_ActorEnableActionCallback(10, gTransferDepart10);
    Engine_EventWait(20);
    Engine_ActorEnableActionCallback(11, gTransferDepart11);
    Engine_EventWait(20);
    Engine_ActorEnableActionCallback(12, gTransferDepart12);
    Engine_EventWait(10);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Engine_ActorFaceDirection(ACTOR_GERALD, 0x4000, 10);
    Object_SetActionCallbackAndRefreshById(13, (s32)gTransferDepart13);
    Engine_AudioPlayCue(0x121);
    Engine_EventWait(20);
    Battle_ClearObjectFlag5bWhenMode3();
    Engine_ActorSetAnimation(10, 1);
    Engine_ActorSetAnimation(11, 1);
    Engine_ActorSetAnimation(12, 1);
    Engine_ActorSetAnimation(13, 1);
    Engine_CameraMoveTo(0x16080000, -1, 0x6f80000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(20);
    Battle_SetObjectFlag5bWhenMode3();
    Engine_ActorSetPosition(9, 0x16080000, 0x6d80000);
    Engine_TaskWait(1);
    Engine_ActorSetSpeed(9, 0x13333, 0x9999);
    Engine_ActorWalkToAndWait(9, 0x1608, 0x6c8);
    Engine_ActorWalkToAndWait(9, 0x15f8, 0x6c8);
    Engine_ActorWalkToAndWait(9, 0x15f8, 0x6f8);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(9, 2);
    Engine_EventWait(20);
    Engine_ActorSetAttachedEffect(9, 0x102);
    Engine_EventWait(60);
    Engine_ActorFaceDirection(9, 0, 20);
    Engine_ActorStartRepeatedMotion(9, 3);
    Engine_EventShowMessageAndWait(9, 0, 20);
    Engine_ActorSetPosition(8, 0x16180000, 0x6f80000);
    Engine_TaskWait(1);
    Engine_ActorSetChildValue(8, 0);
    record = Object_GetById(8);
    Engine_ActorSetSpriteFlags(record, 1);
    Engine_ActorSetSpeed(8, 0xcccc, 0x6666);
    Engine_ActorWalkToAndWait(8, 0x1608, 0x6f8);
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(8, 2);
    Engine_EventShowMessageAndWait(0x2008, 0, 10);
    Engine_ActorFaceDirection(8, 0x3000, 60);
    Engine_ActorFaceDirection(8, 0x8000, 10);
    Engine_ActorSetAttachedEffect(8, 0x102);
    Engine_EventWait(60);
    Engine_ActorFaceDirection(9, 0x3000, 0);
    Engine_ActorFaceDirection(8, 0x3000, 40);
    Engine_ActorSetAttachedEffect(8, 0x102);
    Engine_EventWait(60);
    Engine_ActorStartRepeatedMotion(8, 2);
    Engine_EventShowMessageAndWait(0x2008, 0, 40);
    Engine_ActorRunRepeatedMotion(9, 1);
    Engine_ActorFaceDirection(9, 0, 10);
    Engine_EventShowMessageAndWait(9, 0, 10);
    Engine_ActorShowEmote(8, 0x105, 60);
    Engine_EventShowMessageAndWait(0x2008, 0, 10);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventShowMessageAndWait(0x2008, 0, 10);
    Engine_ActorShowEmote(9, 0x101, 60);
    Engine_EventShowMessageAndWait(9, 0, 20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Battle_ClearObjectFlag5bWhenMode3();
    Engine_AudioPlayCue(107);
    Engine_CameraSetSpeed(0x40000, 0x40000);
    FieldScene_RunScene371_0200155c();
    Engine_AudioPlayCue(0x121);
    Engine_ActorShowEmote(8, 0x100, 0);
    Engine_ActorShowEmote(9, 0x100, 0);
    Engine_ActorFaceDirection(8, 0x8000, 0);
    Engine_ActorFaceDirection(9, 0, 40);
    Engine_ActorFaceDirection(8, 0xb000, 0);
    Engine_ActorFaceDirection(9, 0xb000, 0);
    Engine_CameraSetSpeed(0x10000, 0x2000);
    Engine_CameraMoveTo(0x15e80000, -1, 0x6c80000, 1);
    Engine_CameraWaitForMove();
    Engine_ActorSetPosition(14, 0x15a80000, 0x6a80000);
    Engine_TaskWait(1);
    Engine_ActorSetSpeed(14, 0x4ccc, 0x2666);
    Engine_ActorEnableActionCallback(14, gTransferGuide14);
    Engine_EventWait(160);
    *(s32 *)(actor + 72) = 0x1999;
    *(s32 *)(actor + 68) = 0x1999;
    *(s32 *)(actor + 24) = 0x18000;
    *(s32 *)(actor + 28) = 0x18000;
    {
        s32 shown = 0;

        *(u16 *)(actor + 100) = shown;
    }
    *(s32 *)(actor + 12) = 0x400000;
    {
        u8 *target = *(u8 **)(actor + 80);
        s32 shown = 0xf000;

        *(u16 *)(target + 30) = shown;
    }
    Engine_ActorSetSpriteFlags(actor, 0);
    Object_SetMode(actor, 2);
    Engine_TaskWait(1);
    record = Object_GetById(15);
    Engine_ActorSetSpriteFlags(record, 0);
    Engine_TaskAddCallback((s32)FieldScene_RunScene371_020017a4, 0xc80);
    do {
        Engine_TaskWait(1);
    } while (*(s16 *)(actor + 100) == 0);
    record = Object_GetById(15);
    Engine_ActorSetSpriteFlags(record, 0);
    record = Object_GetById(14);
    Engine_ActorSetSpriteFlags(record, 0);
    Engine_EventWait(10);
    scale = 192;
    record = Object_GetById(9);
    *(s32 *)(record + 40) = (scale << 11);
    record = Object_GetById(8);
    *(s32 *)(record + 40) = (scale << 11);
    Engine_AudioPlayCue(145);
    Engine_CameraSetSpeed(0x40000, 0x40000);
    FieldScene_RunScene371_02001680();
    FieldScene_RunScene371_02001680();
    Engine_EventWait(60);
    Engine_CameraSetSpeed(0x20000, 0x4000);
    Engine_CameraMoveTo(0x16080000, -1, 0x6f80000, 1);
    Engine_CameraWaitForMove();
    Battle_SetObjectFlag5bWhenMode3();
    Engine_ActorSetAttachedEffect(9, 0x102);
    Engine_ActorSetAttachedEffect(8, 0x102);
    Engine_EventWait(60);
    Engine_TaskRemoveCallback((s32)FieldScene_RunScene371_020017a4);
    Engine_TaskWait(1);
    Engine_ActorSetPosition(14, 0, 0);
    Engine_ActorSetPosition(15, 0, 0);
    Engine_ActorFaceDirection(8, 0x8000, 10);
    Engine_ActorJump(8, 4, 40);
    Engine_EventShowMessageAndWait(0x2008, 0, 10);
    Engine_ActorFaceDirection(9, 0, 10);
    Engine_EventShowMessageAndWait(9, 0, 20);
    Engine_ActorFaceDirection(8, 0xc000, 40);
    Engine_EventShowMessageAndWait(0x2008, 0, 20);
    Engine_ActorJump(9, 4, 20);
    Engine_EventShowMessageAndWait(9, 0, 10);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_ActorFaceDirection(8, 0x8000, 10);
    Engine_EventShowMessageAndWait(0x2008, 0, 10);
    Engine_ActorSetAttachedEffect(9, 0x102);
    Engine_EventWait(80);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(9, 1);
    Engine_ActorSetAnimationAndWait(9, 3);
    Engine_ActorWalkToAndWait(8, 0x1618, 0x6f8);
    Engine_ActorSetPosition(8, 0, 0);
    Engine_ActorWalkToAndWait(9, 0x15f8, 0x6c8);
    Engine_ActorWalkToAndWait(9, 0x1608, 0x6c8);
    Engine_ActorWalkToAndWait(9, 0x1608, 0x6d8);
    Engine_ActorSetPosition(9, 0, 0);
    Engine_AudioPlayCue(141);
    Engine_ActorEnableActionCallback(10, (s32)gTransferReturn10);
    Engine_ActorEnableActionCallback(11, gTransferReturn11);
    Engine_EventWait(40);
    Engine_ActorEnableActionCallback(12, gTransferReturn12);
    Engine_EventWait(40);
    Object_SetActionCallbackAndRefreshById(13, (s32)gTransferReturn13);
    Battle_ClearObjectFlag5bWhenMode3();
    Engine_ActorSetPosition(ACTOR_PARTY_LEADER, 0x170c0000, 0x6280000);
    Engine_ActorSetPosition(ACTOR_GERALD, 0x17140000, 0x6400000);
    Engine_CameraSetSpeed(0x40000, 0x8000);
    Engine_CameraMoveTo(0x16d80000, -1, 0x6480000, 1);
    Engine_CameraWaitForMove();
    action = (s32)gTransferGather;
    Engine_ActorEnableActionCallback(10, action);
    Engine_EventWait(20);
    Engine_CameraSetSpeed(0x6666, 0xccc);
    Engine_CameraMoveTo(0x16d80000, -1, 0x6080000, 1);
    Engine_ActorEnableActionCallback(11, action);
    Engine_EventWait(20);
    Engine_ActorEnableActionCallback(12, action);
    Engine_EventWait(20);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    Engine_ActorFaceDirection(ACTOR_GERALD, 0x8000, 0);
    Engine_ActorEnableActionCallback(13, action);
    Engine_EventWait(40);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Engine_ActorFaceDirection(ACTOR_GERALD, 0xc000, 0);
    Object_RefreshSelectorById(13);
    Engine_AudioPlayCue(0x121);
    Engine_CameraSetSpeed(0x40000, 0x8000);
    Engine_CameraMoveTo(0x16f80000, -1, 0x6480000, 1);
    Engine_CameraWaitForMove();
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
    Engine_ActorFaceDirection(ACTOR_GERALD, 0xa000, 80);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(ACTOR_GERALD, 2);
    record = Object_GetById(ACTOR_PARTY_LEADER);
    if (record != 0) {
        Engine_ActorSetDestination(ACTOR_GERALD, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(ACTOR_GERALD);
    Engine_ActorSetPosition(ACTOR_GERALD, 0, 0);
    Engine_CameraSetSpeed(0xcccc, 0x1999);
    Engine_CameraMoveTo(0x16d80000, -1, 0x6480000, 1);
    Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 0x16d8, 0x628);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_GameFlagSet(0x85a);
    Engine_EventRequestExit(3);
    Engine_EventEnd();
}

void FieldScene_RunScene371_0200155c(void)
{

    u32 i;
    s32 record;

    Engine_CameraMoveTo(0x160c0000, -1, 0x6f80000, 1);
    Engine_TaskWait(4);
    Engine_CameraMoveTo(0x16040000, -1, 0x6fc0000, 1);
    Engine_TaskWait(4);
    Engine_CameraMoveTo(0x160c0000, -1, 0x6f40000, 1);
    Engine_TaskWait(4);
    Engine_CameraMoveTo(0x160c0000, -1, 0x6fc0000, 1);
    Engine_TaskWait(4);
    Engine_CameraMoveTo(0x16040000, -1, 0x6f40000, 1);
    Engine_TaskWait(4);
    Engine_CameraMoveTo(0x16080000, -1, 0x6f80000, 1);
    Engine_TaskWait(4);
    Engine_CameraMoveTo(0x160a0000, -1, 0x6f80000, 1);
    Engine_TaskWait(4);
    Engine_CameraMoveTo(0x16060000, -1, 0x6fa0000, 1);
    Engine_TaskWait(4);
    Engine_CameraMoveTo(0x160a0000, -1, 0x6f60000, 1);
    Engine_TaskWait(4);
    Engine_CameraMoveTo(0x160a0000, -1, 0x6fa0000, 1);
    Engine_TaskWait(4);
    Engine_CameraMoveTo(0x16060000, -1, 0x6f60000, 1);
    Engine_TaskWait(4);
    Engine_CameraMoveTo(0x16080000, -1, 0x6f80000, 1);
    Engine_TaskWait(4);
}

void FieldScene_RunScene371_02001680(void)
{

    u32 i;
    s32 record;

    Engine_CameraMoveTo(0x15ec0000, -1, 0x6c80000, 1);
    Engine_TaskWait(4);
    Engine_CameraMoveTo(0x15e40000, -1, 0x6cc0000, 1);
    Engine_TaskWait(4);
    Engine_CameraMoveTo(0x15ec0000, -1, 0x6c40000, 1);
    Engine_TaskWait(4);
    Engine_CameraMoveTo(0x15ec0000, -1, 0x6cc0000, 1);
    Engine_TaskWait(4);
    Engine_CameraMoveTo(0x15e40000, -1, 0x6c40000, 1);
    Engine_TaskWait(4);
    Engine_CameraMoveTo(0x15e80000, -1, 0x6c80000, 1);
    Engine_TaskWait(4);
    Engine_CameraMoveTo(0x15ea0000, -1, 0x6c80000, 1);
    Engine_TaskWait(4);
    Engine_CameraMoveTo(0x15e60000, -1, 0x6ca0000, 1);
    Engine_TaskWait(4);
    Engine_CameraMoveTo(0x15ea0000, -1, 0x6c60000, 1);
    Engine_TaskWait(4);
    Engine_CameraMoveTo(0x15ea0000, -1, 0x6ca0000, 1);
    Engine_TaskWait(4);
    Engine_CameraMoveTo(0x15e60000, -1, 0x6c60000, 1);
    Engine_TaskWait(4);
    Engine_CameraMoveTo(0x15e80000, -1, 0x6c80000, 1);
    Engine_TaskWait(4);
}

void FieldScene_RunScene371_020017a4(void)
{

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Object_GetById(15);
    record = Object_GetById(14);
    *(s32 *)(rec7 + 8) = *(s32 *)(record + 8);
    *(s32 *)(rec7 + 16) = *(s32 *)(record + 16);
    if (*(s32 *)(rec7 + 12) < 0xa0000) {
        *(s32 *)(rec7 + 12) = 0xa0000;
        if (Engine_GameFlagIsSet(0x200) == 0) {
            Engine_AudioPlayCue(145);
            Object_SetMode(rec7, 3);
            Engine_GameFlagSet(0x200);
            {
                u16 *target = (u16 *)(rec7 + 100);
                s32 shown = 1;

                *target = shown;
            }
        }
    }
}

void FieldScene_RunScene371_020017fc(void)
{

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Object_GetById(8);
    Engine_EventBegin();
    Engine_CameraMoveTo(-1, -1, -1, 0);
    Engine_TaskWait(1);
    Engine_ActorSetPosition(ACTOR_PARTY_LEADER, 0, 0);
    *(s32 *)(rec7 + 28) = 0x14000;
    *(s32 *)(rec7 + 24) = 0x14000;
    Engine_CameraFollowActor(8, 1);
    Engine_EventOpenScreen();
    Engine_ActorSetSpeed(8, 0x6666, 0x3333);
    Engine_ActorWalkToAndWait(8, 0x14a8, 0x918);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_GameFlagSet(0x927);
    Engine_EventRequestExit(102);
    Engine_EventEnd();
}

void FieldScene_RunScene371_02001888(void)
{

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Object_GetById(8);
    Engine_EventBegin();
    Engine_CameraMoveTo(-1, -1, -1, 0);
    Engine_TaskWait(1);
    Engine_ActorSetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Engine_ActorSetPosition(8, 0x1f080000, 0xc80000);
    *(s32 *)(rec7 + 24) = 0x14000;
    *(s32 *)(rec7 + 28) = 0x14000;
    Engine_TaskWait(1);
    Engine_CameraFollowActor(8, 1);
    Engine_EventOpenScreen();
    Engine_ActorSetSpeed(8, 0x9999, 0x4ccc);
    {
        s32 shown = 0;

        *(u16 *)(rec7 + 100) = shown;
    }
    Engine_ActorEnableActionCallback(8, gTransferLeaderIdle);
    do {
        Engine_TaskWait(1);
    } while (*(s16 *)(rec7 + 100) == 0);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_GameFlagSet(0x927);
    Engine_EventRequestExit(103);
    Engine_EventEnd();
}

void FieldScene_RunScene371_02001938(void)
{

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Object_GetById(8);
    Engine_EventBegin();
    Engine_CameraMoveTo(-1, -1, -1, 0);
    Engine_TaskWait(1);
    Engine_ActorSetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Engine_ActorSetPosition(8, 0x1f080000, 0xc80000);
    *(s32 *)(rec7 + 24) = 0x14000;
    *(s32 *)(rec7 + 28) = 0x14000;
    Engine_TaskWait(1);
    Engine_CameraFollowActor(8, 1);
    Engine_EventOpenScreen();
    Engine_ActorSetSpeed(8, 0x9999, 0x4ccc);
    {
        s32 shown = 0;

        *(u16 *)(rec7 + 100) = shown;
    }
    Engine_ActorEnableActionCallback(8, gTransferLeaderIdle);
    do {
        Engine_TaskWait(1);
    } while (*(s16 *)(rec7 + 100) == 0);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_GameFlagSet(0x927);
    Engine_EventRequestExit(104);
    Engine_EventEnd();
}

void FieldScene_RunScene371_020019e8(void)
{

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Object_GetById(8);
    Engine_EventBegin();
    Engine_CameraMoveTo(-1, -1, -1, 0);
    Engine_TaskWait(1);
    Engine_ActorSetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Engine_ActorSetPosition(8, 0x1f080000, 0xc80000);
    *(s32 *)(rec7 + 24) = 0x14000;
    *(s32 *)(rec7 + 28) = 0x14000;
    Engine_TaskWait(1);
    Engine_CameraFollowActor(8, 1);
    Engine_EventOpenScreen();
    Engine_ActorSetSpeed(8, 0x9999, 0x4ccc);
    {
        s32 shown = 0;

        *(u16 *)(rec7 + 100) = shown;
    }
    Engine_ActorEnableActionCallback(8, gTransferLeaderIdle);
    do {
        Engine_TaskWait(1);
    } while (*(s16 *)(rec7 + 100) == 0);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_GameFlagSet(0x927);
    Engine_EventRequestExit(105);
    Engine_EventEnd();
}

void FieldScene_RunScene371_02001a98(void)
{

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Object_GetById(8);
    Engine_EventBegin();
    Engine_CameraMoveTo(-1, -1, -1, 0);
    Engine_TaskWait(1);
    Engine_ActorSetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Engine_ActorSetPosition(8, 0x1f080000, 0xc80000);
    *(s32 *)(rec7 + 24) = 0x14000;
    *(s32 *)(rec7 + 28) = 0x14000;
    Engine_TaskWait(1);
    Engine_CameraFollowActor(8, 1);
    Engine_EventOpenScreen();
    Engine_ActorSetSpeed(8, 0x9999, 0x4ccc);
    {
        s32 shown = 0;

        *(u16 *)(rec7 + 100) = shown;
    }
    if (StoryScene_ComputeOpposingSlotDelta() == 11) {
        Engine_ActorEnableActionCallback(8, gTransferLeaderTurn);
    } else {
        Engine_ActorEnableActionCallback(8, gTransferLeaderIdle);
    }
    do {
        Engine_TaskWait(1);
    } while (*(s16 *)(rec7 + 100) == 0);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_GameFlagSet(0x927);
    Engine_EventRequestExit(106);
    Engine_EventEnd();
}

void FieldScene_RunScene371_02001b5c(void)
{

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Object_GetById(8);
    Engine_EventBegin();
    Engine_CameraMoveTo(-1, -1, -1, 0);
    Engine_TaskWait(1);
    Engine_ActorSetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Engine_ActorSetPosition(8, 0x13e80000, 0x9180000);
    *(s32 *)(rec7 + 28) = 0x14000;
    *(s32 *)(rec7 + 24) = 0x14000;
    Engine_TaskWait(1);
    Engine_CameraFollowActor(8, 1);
    Engine_EventOpenScreen();
    Engine_ActorSetSpeed(8, 0x6666, 0x3333);
    Engine_ActorWalkToAndWait(8, 0x13c8, 0x918);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_GameFlagSet(0x93e);
    Engine_GameFlagClear(0x927);
    Engine_EventRequestExit(107);
    Engine_EventEnd();
}

void FieldScene_RunScene371_02001c08(void)
{

    u32 i;
    s32 record;

    Battle_ClearObjectFlag5bWhenMode3();
    BattleFx_ScheduleRatioTransition(0x10000, 6);
    Event_WaitForDisplayField358Clear();
    Battle_SetObjectFlag5bWhenMode3();
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventSetMessage((s32)MsgWorldMapAfterBringingDjinniIntoYour);
    Engine_EventShowMessage(8, 0);
    Engine_EventWait(30);
    Engine_AudioPlayCue(111);
    Menu_AnimateSelectionToEntry(0, 2);
    Engine_GameFlagClear(0x16f);
    Engine_GameFlagClear(0x171);
    ItemMenu_Open();
    Engine_ActorJump(8, 4, 30);
    Engine_EventSetMessage((s32)MsgWorldMapNextIllShowHowCan);
    Engine_EventShowMessage(8, 0);
    Engine_GameFlagClear(0x16f);
    Engine_GameFlagSet(0x171);
    ItemMenu_Open();
    Engine_EventWait(30);
    BattleFx_SetWeightedResult(12, 6);
}
extern u8 MsgWorldMapOh[];
extern u8 MsgWorldMapComePromiseWont[];
extern u8 MsgWorldMapMeanieDontCare[];
extern u8 MsgWorldMapSeeWontRegret[];
extern u8 MsgWorldMapAbilityVenusDjinni[];
extern u8 MsgWorldMapSeeDjinnUseful[];
extern u8 MsgWorldMapGoSetStandby[];
extern u8 MsgWorldMapHmmmmExplainAgain[];
extern u8 MsgWorldMapYeahWantLearn[];

void Owner_RefreshActiveRatios(s32 owner);
s32 Djinn_AddToOwner();
s32 Trade_AddOffer();
void BattleEffect_CleanupSceneObjects(void);
void UiWork_PushValueSlot(s32 value, s32 slot);
void BattleFx_RunPageEffectForSlot(s32 actor, s32 a1, s32 a2);

#define DJINNI 8
#define FLAG_DJINNI_MET 0x16e

/* The world map's Venus Djinni: on the first meeting it joins the leader, grows
 * from a speck and explains itself, pleading until the party agrees to listen;
 * later it offers to explain Djinn again. The pleas are counted apart from the
 * loops, which is what lets the growing loop count down from its own start. */
void WorldMap_MeetVenusDjinni(void)
{
    struct FieldActor *djinni;
    struct FieldActor *leader;
    s32 x;
    s32 z;
    s32 count;
    s32 i;

    count = 0;
    djinni = Object_GetById(DJINNI);
    leader = Object_GetById(0);
    x = (leader->x.fixed + (s32)0xea300000) / 2 + 0x15d00000;
    z = (leader->z.fixed + (s32)0xfad00000) / 2 + 0x5300000;
    if (!Engine_GameFlagIsSet(FLAG_DJINNI_MET)) {
        Owner_RefreshActiveRatios(1);
        Engine_GameFlagSet(FLAG_DJINNI_MET);
        Engine_EventBegin();
        leader = Object_GetById(0);
        if (leader != 0)
            Engine_ActorSetPosition(DJINNI, leader->x.fixed, leader->z.fixed);
        Djinn_AddToOwner(0, 0, 0);
        Trade_AddOffer(0, 0, 0);
        Battle_SetObjectFlag5bWhenMode3();
        Engine_ActorFaceActor(0, DJINNI, 0);
        Engine_EventWait(10);
        Engine_ActorShowEmote(0, 0x101, 60);
        djinni->unknown_66 = 1;
        Engine_ActorFaceActor(DJINNI, 0, 0);
        Engine_TaskWait(16);
        Engine_EventSetMessage((s32)MsgWorldMapOh);
        Engine_EventShowMessage(DJINNI, 0);
        Battle_ClearObjectFlag5bWhenMode3();
        BattleFx_ScheduleRatioTransition(0x13333, 6);
        Event_WaitForDisplayField358Clear();
        Battle_SetObjectFlag5bWhenMode3();
        djinni->motion_flags = 2;
        *(s32 *)&djinni->unknown_44[4] = 0x4000;
        djinni->speed = 0x10000;
        djinni->acceleration = 0x10000;
        djinni->velocity_y = 0;
        *(s32 *)djinni->unknown_14 = 0;
        Engine_ObjectSetPosition(djinni, 0x15d00000, 0, 0x5300000);
        for (i = 0; i < 16; i++) {
            djinni->scale_x += 0x800;
            djinni->scale_y += 0x800;
            Engine_TaskWait(1);
        }
        Engine_ActorFaceActor(DJINNI, 0, 0);
        Engine_ActorFaceActor(0, DJINNI, 0);
        Engine_TaskWait(16);
        djinni->update = 0;
        Engine_ObjectSetPartPalettes(djinni, 0);
        *(s32 *)&djinni->unknown_44[4] = 0x10000;
        Engine_EventShowMessage(DJINNI, 0);
        Engine_AudioPlayCue(131);
        Engine_PsynergyBegin(140, 0);
        for (i = 59; i >= 0; i--) {
            if (gFrameCount & 2)
                Engine_ObjectSetPartPalettes(djinni, 7);
            else
                Engine_ObjectSetPartPalettes(djinni, 0);
            if ((gFrameCount & 15) == 0)
                WorldMap_CreateLinkedEffects(djinni);
            Engine_TaskWait(1);
        }
        BattleEffect_CleanupSceneObjects();
        Engine_ObjectSetPartPalettes(djinni, 0);
        Engine_ActorRunRepeatedMotion(DJINNI, 2);
        Engine_EventShowMessage(DJINNI, 0);
        Engine_ActorShowEmote(0, 0x102, 30);
        Engine_EventShowMessage(DJINNI, 0);
        Engine_ActorShowEmote(0, 0x101, 30);
        Engine_ActorWalkToAndWait(DJINNI, x >> 16, z >> 16);
        Engine_ActorSetAnimation(0, 22);
        Engine_EventShowMessage(DJINNI, 0);
        Engine_ActorShowEmote(0, 0x101, 40);
        Engine_ActorJump(DJINNI, 4, 30);
        UiWork_PushValueSlot(300, 4);
        Engine_EventShowMessage(DJINNI, 0);
        Engine_ActorShowEmote(0, 0x100, 30);
        Engine_EventShowMessage(DJINNI, 0);
        Engine_ActorRunRepeatedMotion(0, 2);
        Engine_EventShowMessage(DJINNI, 0);
        Engine_ActorJump(DJINNI, 2, 30);
        Engine_EventShowMessage(DJINNI, 0);
        i = 0;
        djinni->motion_flags = 0;
        Engine_ObjectSetPosition(djinni, x, 0x100000, z);
        for (; i < 16; i++) {
            djinni->facing += 0x1000;
            Engine_TaskWait(1);
        }
        Engine_ActorSetAnimation(0, 1);
        Engine_EventShowMessage(DJINNI, 0);
        djinni->motion_flags = 2;
        djinni->velocity_y = 0;
        *(s32 *)djinni->unknown_14 = 0;
        for (i = 7; i >= 0; i--) {
            djinni->facing += 0x1000;
            Engine_TaskWait(1);
        }
        Engine_ActorSetAnimation(0, 22);
        Engine_EventShowMessage(DJINNI, 0);
        Engine_ActorShowEmote(DJINNI, 0x102, 30);
        Engine_ActorFaceActor(DJINNI, 0, 0);
        Engine_ActorRunRepeatedMotion(DJINNI, 2);
        Engine_EventShowMessage(DJINNI, 0);
        Engine_ActorJump(DJINNI, 2, 30);
        Engine_EventOpenMessage(DJINNI, 0);
        count = 0;
    plead:
        if (Engine_EventChooseYesNo(0, 0) == 1) {
            Engine_ActorJump(DJINNI, 2, 20);
            Engine_ActorJump(DJINNI, 2, 20);
            if (count == 6) {
                Engine_EventSetMessage((s32)MsgWorldMapMeanieDontCare);
                Engine_EventShowMessage(DJINNI, 0);
                goto listened;
            }
            Engine_EventSetMessage(count + (s32)MsgWorldMapComePromiseWont);
            Engine_EventOpenMessage(DJINNI, 0);
            count++;
            goto plead;
        }
        Engine_ActorSetAnimation(0, 22);
        Engine_ActorJump(DJINNI, 2, 20);
        Engine_ActorJump(DJINNI, 4, 20);
        Engine_EventSetMessage((s32)MsgWorldMapSeeWontRegret);
        Engine_EventShowMessage(DJINNI, 0);
listened:
        UiWork_PushValueSlot(300, 4);
        Engine_AudioPlayCue(81);
        count = (s32)MsgWorldMapAbilityVenusDjinni;
        Engine_MessageShowCentered(count++, 3);
        Engine_EventSetMessage(count);
        {
            register s32 frames asm("r2") = 20; /* FAKEMATCH: pins the frames to r2 */
            register s32 flags asm("r1"); /* FAKEMATCH: pins the flags to r1 */

            asm volatile("" : : "r"(frames)); /* FAKEMATCH: sets the jump's frames first */
            Engine_ActorJump(DJINNI, 2, frames);
            flags = 0;
            asm volatile("" : : "r"(flags)); /* FAKEMATCH: sets the message flags first */
            Engine_EventShowMessage(DJINNI, flags);
        }
        Engine_AudioPlayCue(9);
        goto finish;
    }
    Engine_EventBegin();
    leader = Object_GetById(0);
    if (leader != 0)
        Engine_ActorSetPosition(DJINNI, leader->x.fixed, leader->z.fixed);
    djinni->velocity_y = 0xa0000;
    Engine_ObjectSetPosition(djinni, x, 0, z);
    Engine_EventWait(30);
    Battle_SetObjectFlag5bWhenMode3();
    Engine_ActorFaceActor(DJINNI, 0, 0);
    Engine_ActorFaceActor(0, DJINNI, 0);
    Engine_ActorSetAnimation(0, 22);
    Engine_EventSetMessage((s32)MsgWorldMapSeeDjinnUseful);
    Engine_ActorJump(DJINNI, 2, 20);
    Engine_ActorJump(DJINNI, 2, 20);
    Engine_EventShowMessage(DJINNI, 0);
    Engine_ActorRunRepeatedMotion(DJINNI, 2);
    Engine_EventShowMessage(DJINNI, 0);
    Engine_AudioPlayCue(111);
    Menu_AnimateSelectionToEntry(0, 2);
    Engine_GameFlagSet(0x16f);
    Engine_GameFlagClear(0x171);
    ItemMenu_Open();
    Engine_EventSetMessage((s32)MsgWorldMapGoSetStandby);
    Engine_ObjectSetPosition(djinni, 0x15d00000, 0, 0x5300000);
    Engine_EventWait(30);
    Engine_EventShowMessage(DJINNI, 0);
    Engine_ActorFaceActor(DJINNI, 0, 0);
    Engine_EventShowMessage(DJINNI, 0);
    Engine_EventOpenMessage(DJINNI, 0);
    if (Engine_EventChooseYesNo(0, 0) != 1)
        goto learn;
    Engine_ActorSetAnimation(0, 22);
    Engine_ActorRunRepeatedMotion(DJINNI, 2);
    Engine_EventSetMessage((s32)MsgWorldMapHmmmmExplainAgain);
    Engine_EventOpenMessage(DJINNI, 0);
    if (Engine_EventChooseYesNo(0, 0) == 1)
        goto learn;
    Engine_EventShowMessage(DJINNI, 0);
    {
        register s32 far asm("r3") = z; /* FAKEMATCH: pins z to r3 */
        register s32 near asm("r2") = x; /* FAKEMATCH: pins x to r2 */
        register s32 tile_x asm("r1"); /* FAKEMATCH: pins the tile x to r1 */
        register s32 tile_z asm("r2"); /* FAKEMATCH: reuses r2 for the tile z */

        asm volatile("" : : "r"(far), "r"(near)); /* FAKEMATCH: copies z before x */
        tile_x = near >> 16;
        asm("asr %0, %1, #16" : "=l"(tile_z) : "l"(far), "l"(tile_x)); /* FAKEMATCH: shifts z into r2 after the tile x */
        Engine_ActorWalkToAndWait(DJINNI, tile_x, tile_z);
    }
finish:
    FieldScene_RunScene371_02001c08();
    Battle_ClearObjectFlag5bWhenMode3();
    return;
learn:
    Engine_ActorSetAnimation(0, 22);
    Engine_EventSetMessage((s32)MsgWorldMapYeahWantLearn);
    Engine_ActorJump(DJINNI, 2, 20);
    Engine_ActorJump(DJINNI, 2, 20);
    Engine_ActorSetAnimationAndWait(0, 3);
    Engine_ActorShowEmote(DJINNI, 0x100, 30);
    Engine_EventShowMessage(DJINNI, 0);
    Engine_GameFlagSet(0x16f);
    Engine_GameFlagSet(0x171);
    ItemMenu_Open();
    Engine_ActorJump(DJINNI, 2, 20);
    Engine_EventShowMessage(DJINNI, 0);
    Battle_ClearObjectFlag5bWhenMode3();
    BattleFx_RunPageEffectForSlot(DJINNI, 0, 0);
    Engine_AudioPlayCue(42);
    Engine_EventEnd();
    Engine_GameFlagClear(FLAG_DJINNI_MET);
    Engine_GameFlagClear(0x16f);
    Engine_GameFlagClear(0x171);
}
