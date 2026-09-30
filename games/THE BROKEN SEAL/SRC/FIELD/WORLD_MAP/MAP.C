#include "STORY.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "IWRAM_CALL.H"
#include "SCENE_IDS.H"
#include "CALL.H"

#define FrameCounter (*(u32 *)&gFrameCount)
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
        if ((u32)Random_Next() < 0x8000) {
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
        spawned_actor = Object_Create(0x11d, x, y, z);
    }
    Audio_PlayCue(0xf6);
    if (spawned_actor == 0) {
        return;
    }

    {
        u8 *spawned_flags = spawned_actor + 0x55;
        s32 zero_value = 0;

        *spawned_flags = zero_value;
        spawned_record = *(u8 **)(spawned_actor + 0x50);
        ((StorySpawnRecord *)spawned_record)->field = 1;
        Actor_SetSpriteFlags(spawned_actor, 0);
        Object_SetAnimation(spawned_actor, 1);
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

    if (GameFlag_IsSet(0x30) != 0) {
        return 0;
    }
    if (GameFlag_IsSet(0x16E) != 0) {
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
    if (GameFlag_IsSet(0x847) != 0) {
        Object_SetAnimation(actor, 2);
    }
    return 1;
}

/* Turns the actor's sprite to the map's rotation. */
s32 StoryActor_ApplyMapRotation(struct FieldActor *actor)
{
    struct MapRenderWork *work = gMapWork;
    struct FieldSprite *sprite = actor->sprite;

    sprite->rotation = work->rotation;
    sprite->flags = 0;
    return 1;
}

/* Sets the actor's first collision flag and turns its sprite to the map's rotation. */
s32 StoryActor_ApplyMapRotationWithCollision(struct FieldActor *actor)
{
    struct MapRenderWork *work = gMapWork;
    struct FieldSprite *sprite = actor->sprite;

    actor->collision_flags |= 1;
    sprite->rotation = work->rotation;
    return 1;
}

s32 StoryActor_ResetPosition(u8 *actor)
{
    s32 zero;
    Actor_SetSpriteFlags(actor, 0);
    Object_SetPartPalettes(actor, 10);
    {
        u8 *mode_flags = actor + 0x59;
        zero = 0;
        *mode_flags = zero;
    }
    if (GameFlag_IsSet(0x8A0) != 0) {
        GameFlag_Set(0x2f1);
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
u8 *StoryScene_SelectPlacementTable(void)
{
    s16 *scene_table = (s16 *)&gGameState;
    s32 scene_id = scene_table[225];

    switch (scene_id) {
    case 49:
        if (GameFlag_IsSet(0x94f) == 0 && GameFlag_IsSet(0x941) != 0) {
            return gWorldMapPlacements49;
        }
        break;
    case 64:
        if (GameFlag_IsSet(0x85a) == 0) {
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

    GameFlag_Set(0x235);
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

    subject_actor = Actor_Get(actor_object - 0x64);
    scene_table = (s16 *)&gGameState;
    other_actor = Actor_Get(*(s32 *)&scene_table[250]);
    scene_state = *(u8 **)&gEventWork;
    if (other_actor->x < subject_actor->x) {
        *(u16 *)(scene_state + 0x170) = val_lower;
    } else {
        *(u16 *)(scene_state + 0x170) = val_other;
    }
    Audio_PlayCue(0x7B);
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

    subject_actor = Actor_Get(actor_object - 0x64);
    scene_table = (s16 *)&gGameState;
    other_actor = Actor_Get(*(s32 *)&scene_table[250]);
    scene_state = *(u8 **)&gEventWork;
    if (other_actor->z < subject_actor->z) {
        *(u16 *)(scene_state + 0x170) = val_lower;
    } else {
        *(u16 *)(scene_state + 0x170) = val_other;
    }
    Audio_PlayCue(0x7B);
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

u8 *WorldMap_GetEvents(void)
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

    leader = Actor_Get(gGameState.selected_actor);
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

        actor = Actor_Lookup(i);
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
        if (gDebugMode[0] != 0 && GameFlag_IsSet(FLAG_CONTACT_PAUSED) != 0) {
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
            && GameFlag_IsSet(FLAG_CONTACT_BLOCKED) == 0) {
            work->touched_trigger = i + CONTACT_TRIGGER_BASE;
        }
    }
}

void SceneState_ApplyFlag85aBranch(void)
{
    if (GameFlag_IsSet(0x85a) == 0) {
        Event_RequestExit(101);
    } else {
        Audio_PlayCue(123);
        Event_RequestExit(3);
    }
}

void FieldScene_RunStep74(void)
{

    Event_Begin();
    Event_RequestExit(74);
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
                    Engine_ActorGet(gWorldMapTriggerActor)->facing = 0x3000;
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
            Engine_ActorGet(53)->scale_x = 0x14000;
            Engine_ActorGet(53)->scale_y = 0x14000;
            break;
        }
    }
    return 0;
}

void RunEventScript01(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Event_OpenScreen();
    Event_WaitForScreen();
    Battle_SetObjectFlag5bWhenMode3();
    GameFlag_Set(0x94f);
    Actor_SetPosition(11, 0x16e00000, 0x49c0000);
    Actor_SetDestinationOffset(11, 24, 8);
    Actor_WaitForMove(11);
    Event_Wait(60);
    Actor_SetPosition(12, 0x16e00000, 0x49c0000);
    Actor_SetDestinationOffset(12, 12, 24);
    Event_Wait(30);
    Actor_FaceDirection(11, 0x5000, 0);
    Actor_FaceDirection(12, 0xd000, 0);
    Event_Wait(60);
    Actor_SetAnimation(11, 3);
    Actor_SetAnimation(12, 3);
    Event_Wait(120);
    Actor_SetPosition(8, 0x16f80000, 0x4b80000);
    Event_Wait(60);
    Actor_SetAnimation(12, 2);
    record = Engine_ActorGet(8);
    if (record != 0) {
        Actor_SetDestination(12, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(12);
    Actor_SetPosition(12, 0, 0);
    ((void (*)())Engine_EventWait)(60);
    Actor_SetAnimation(11, 2);
    record = Engine_ActorGet(8);
    if (record != 0) {
        Actor_SetDestination(11, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(11);
    Actor_SetPosition(11, 0, 0);
    ((void (*)())Engine_EventWait)(60);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
    record = Engine_ActorGet(8);
    if (record != 0) {
        Actor_SetDestination(ACTOR_PARTY_LEADER, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    ((void (*)())Engine_EventWait)(60);
    Actor_SetSpeed(8, 0x8000, 0x4000);
    Actor_SetDestinationOffset(8, 56, 8);
    Actor_WaitForMove(8);
    Actor_SetDestinationOffset(8, 40, 40);
    Actor_WaitForMove(8);
    Actor_SetDestinationOffset(8, 8, 88);
    Actor_WaitForMove(8);
    Event_CloseScreen();
    Event_RequestExit(108);
    Event_End();
}

void FieldScene_RunActorTransferSequence(void)
{
    u8 *actor;
    u8 *record;
    s32 scale;
    s32 action;

    actor = Engine_ActorGet(15);
    Event_Begin();
    BattleFx_ScheduleRatioTransition(0x14000, 1);
    Task_Wait(4);
    Event_OpenScreen();
    Event_WaitForScreen();
    Battle_SetObjectFlag5bWhenMode3();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x19999, 0xcccc);
    Actor_SetSpeed(ACTOR_GERALD, 0x19999, 0xcccc);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x16fc, 0x628);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Actor_SetPosition(8, 0x16d80000, 0x6280000);
    Task_Wait(1);
    Actor_SetChildValue(8, 15);
    record = Actor_Get(8);
    Actor_SetSpriteFlags(record, 0);
    Actor_SetSpeed(10, 0x19999, 0x6666);
    Actor_SetSpeed(11, 0x19999, 0x6666);
    Actor_SetSpeed(12, 0x19999, 0x6666);
    Actor_SetSpeed(13, 0x19999, 0x6666);
    Audio_PlayCue(141);
    Actor_EnableActionCallback(10, gTransferArrive10);
    Event_Wait(20);
    Actor_EnableActionCallback(11, gTransferArrive11);
    Event_Wait(20);
    Actor_EnableActionCallback(12, gTransferArrive12);
    Event_Wait(20);
    Object_SetActionCallbackAndRefreshById(13, (s32)gTransferArrive13);
    Audio_PlayCue(0x121);
    record = Engine_ActorGet(ACTOR_PARTY_LEADER);
    if (record != 0) {
        Actor_SetPosition(ACTOR_GERALD, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_WalkToAndWait(ACTOR_GERALD, 0x1704, 0x640);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xa000, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 60);
    Actor_SetPosition(8, 0x16d80000, 0x6380000);
    Task_Wait(1);
    Event_SetMessage((s32)MsgWorldMapWeCantStayAnotherMinute);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xa000, 60);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 10);
    Audio_PlayCue(141);
    Actor_EnableActionCallback(10, gTransferDepart10);
    Event_Wait(20);
    Actor_EnableActionCallback(11, gTransferDepart11);
    Event_Wait(20);
    Actor_EnableActionCallback(12, gTransferDepart12);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 10);
    Object_SetActionCallbackAndRefreshById(13, (s32)gTransferDepart13);
    Audio_PlayCue(0x121);
    Event_Wait(20);
    Battle_ClearObjectFlag5bWhenMode3();
    Actor_SetAnimation(10, 1);
    Actor_SetAnimation(11, 1);
    Actor_SetAnimation(12, 1);
    Actor_SetAnimation(13, 1);
    Camera_MoveTo(0x16080000, -1, 0x6f80000, 1);
    Camera_WaitForMove();
    Event_Wait(20);
    Battle_SetObjectFlag5bWhenMode3();
    Actor_SetPosition(9, 0x16080000, 0x6d80000);
    Task_Wait(1);
    Actor_SetSpeed(9, 0x13333, 0x9999);
    Actor_WalkToAndWait(9, 0x1608, 0x6c8);
    Actor_WalkToAndWait(9, 0x15f8, 0x6c8);
    Actor_WalkToAndWait(9, 0x15f8, 0x6f8);
    Event_Wait(20);
    Actor_RunRepeatedMotion(9, 2);
    Event_Wait(20);
    Actor_SetAttachedEffect(9, 0x102);
    Event_Wait(60);
    Actor_FaceDirection(9, 0, 20);
    Actor_StartRepeatedMotion(9, 3);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_SetPosition(8, 0x16180000, 0x6f80000);
    Task_Wait(1);
    Actor_SetChildValue(8, 0);
    record = Actor_Get(8);
    Actor_SetSpriteFlags(record, 1);
    Actor_SetSpeed(8, 0xcccc, 0x6666);
    Actor_WalkToAndWait(8, 0x1608, 0x6f8);
    Event_Wait(20);
    Actor_StartRepeatedMotion(8, 2);
    Event_ShowMessageAndWait(0x2008, 0, 10);
    Actor_FaceDirection(8, 0x3000, 60);
    Actor_FaceDirection(8, 0x8000, 10);
    Actor_SetAttachedEffect(8, 0x102);
    Event_Wait(60);
    Actor_FaceDirection(9, 0x3000, 0);
    Actor_FaceDirection(8, 0x3000, 40);
    Actor_SetAttachedEffect(8, 0x102);
    Event_Wait(60);
    Actor_StartRepeatedMotion(8, 2);
    Event_ShowMessageAndWait(0x2008, 0, 40);
    Actor_RunRepeatedMotion(9, 1);
    Actor_FaceDirection(9, 0, 10);
    Event_ShowMessageAndWait(9, 0, 10);
    Actor_ShowEmote(8, 0x105, 60);
    Event_ShowMessageAndWait(0x2008, 0, 10);
    Actor_RunRepeatedMotion(8, 1);
    Actor_SetAnimationAndWait(8, 3);
    Event_ShowMessageAndWait(0x2008, 0, 10);
    Actor_ShowEmote(9, 0x101, 60);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_SetAnimationAndWait(8, 3);
    Battle_ClearObjectFlag5bWhenMode3();
    Audio_PlayCue(107);
    Camera_SetSpeed(0x40000, 0x40000);
    FieldScene_RunScene371_0200155c();
    Audio_PlayCue(0x121);
    Actor_ShowEmote(8, 0x100, 0);
    Actor_ShowEmote(9, 0x100, 0);
    Actor_FaceDirection(8, 0x8000, 0);
    Actor_FaceDirection(9, 0, 40);
    Actor_FaceDirection(8, 0xb000, 0);
    Actor_FaceDirection(9, 0xb000, 0);
    Camera_SetSpeed(0x10000, 0x2000);
    Camera_MoveTo(0x15e80000, -1, 0x6c80000, 1);
    Camera_WaitForMove();
    Actor_SetPosition(14, 0x15a80000, 0x6a80000);
    Task_Wait(1);
    Actor_SetSpeed(14, 0x4ccc, 0x2666);
    Actor_EnableActionCallback(14, gTransferGuide14);
    Event_Wait(160);
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
    Actor_SetSpriteFlags(actor, 0);
    Object_SetAnimation(actor, 2);
    Task_Wait(1);
    record = Actor_Get(15);
    Actor_SetSpriteFlags(record, 0);
    Engine_TaskAddCallback((s32)FieldScene_RunScene371_020017a4, 0xc80);
    do {
        Task_Wait(1);
    } while (*(s16 *)(actor + 100) == 0);
    record = Actor_Get(15);
    Actor_SetSpriteFlags(record, 0);
    record = Actor_Get(14);
    Actor_SetSpriteFlags(record, 0);
    Event_Wait(10);
    scale = 192;
    record = Actor_Get(9);
    *(s32 *)(record + 40) = (scale << 11);
    record = Engine_ActorGet(8);
    *(s32 *)(record + 40) = (scale << 11);
    Audio_PlayCue(145);
    Camera_SetSpeed(0x40000, 0x40000);
    FieldScene_RunScene371_02001680();
    FieldScene_RunScene371_02001680();
    Event_Wait(60);
    Camera_SetSpeed(0x20000, 0x4000);
    Camera_MoveTo(0x16080000, -1, 0x6f80000, 1);
    Camera_WaitForMove();
    Battle_SetObjectFlag5bWhenMode3();
    Actor_SetAttachedEffect(9, 0x102);
    Actor_SetAttachedEffect(8, 0x102);
    Event_Wait(60);
    Engine_TaskRemoveCallback((s32)FieldScene_RunScene371_020017a4);
    Task_Wait(1);
    Actor_SetPosition(14, 0, 0);
    Actor_SetPosition(15, 0, 0);
    Actor_FaceDirection(8, 0x8000, 10);
    Actor_Jump(8, 4, 40);
    Event_ShowMessageAndWait(0x2008, 0, 10);
    Actor_FaceDirection(9, 0, 10);
    Event_ShowMessageAndWait(9, 0, 20);
    Actor_FaceDirection(8, 0xc000, 40);
    Event_ShowMessageAndWait(0x2008, 0, 20);
    Actor_Jump(9, 4, 20);
    Event_ShowMessageAndWait(9, 0, 10);
    Actor_RunRepeatedMotion(8, 1);
    Actor_FaceDirection(8, 0x8000, 10);
    Event_ShowMessageAndWait(0x2008, 0, 10);
    Actor_SetAttachedEffect(9, 0x102);
    Event_Wait(80);
    Actor_SetAnimationAndWait(8, 3);
    Event_Wait(20);
    Actor_RunRepeatedMotion(9, 1);
    Actor_SetAnimationAndWait(9, 3);
    Actor_WalkToAndWait(8, 0x1618, 0x6f8);
    Actor_SetPosition(8, 0, 0);
    Actor_WalkToAndWait(9, 0x15f8, 0x6c8);
    Actor_WalkToAndWait(9, 0x1608, 0x6c8);
    Actor_WalkToAndWait(9, 0x1608, 0x6d8);
    Actor_SetPosition(9, 0, 0);
    Audio_PlayCue(141);
    Actor_EnableActionCallback(10, (s32)gTransferReturn10);
    Actor_EnableActionCallback(11, gTransferReturn11);
    Event_Wait(40);
    Actor_EnableActionCallback(12, gTransferReturn12);
    Event_Wait(40);
    Object_SetActionCallbackAndRefreshById(13, (s32)gTransferReturn13);
    Battle_ClearObjectFlag5bWhenMode3();
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x170c0000, 0x6280000);
    Actor_SetPosition(ACTOR_GERALD, 0x17140000, 0x6400000);
    Camera_SetSpeed(0x40000, 0x8000);
    Camera_MoveTo(0x16d80000, -1, 0x6480000, 1);
    Camera_WaitForMove();
    action = (s32)gTransferGather;
    Actor_EnableActionCallback(10, action);
    Event_Wait(20);
    Camera_SetSpeed(0x6666, 0xccc);
    Camera_MoveTo(0x16d80000, -1, 0x6080000, 1);
    Actor_EnableActionCallback(11, action);
    Event_Wait(20);
    Actor_EnableActionCallback(12, action);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 0);
    Actor_EnableActionCallback(13, action);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Object_RefreshSelectorById(13);
    Audio_PlayCue(0x121);
    Camera_SetSpeed(0x40000, 0x8000);
    Camera_MoveTo(0x16f80000, -1, 0x6480000, 1);
    Camera_WaitForMove();
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xa000, 80);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Event_Wait(20);
    Actor_SetAnimation(ACTOR_GERALD, 2);
    record = Engine_ActorGet(ACTOR_PARTY_LEADER);
    if (record != 0) {
        Actor_SetDestination(ACTOR_GERALD, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Camera_SetSpeed(0xcccc, 0x1999);
    Camera_MoveTo(0x16d80000, -1, 0x6480000, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x16d8, 0x628);
    Event_CloseScreen();
    Event_WaitForScreen();
    GameFlag_Set(0x85a);
    Event_RequestExit(3);
    Event_End();
}

void FieldScene_RunScene371_0200155c(void)
{

    u32 i;
    s32 record;

    Camera_MoveTo(0x160c0000, -1, 0x6f80000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x16040000, -1, 0x6fc0000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x160c0000, -1, 0x6f40000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x160c0000, -1, 0x6fc0000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x16040000, -1, 0x6f40000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x16080000, -1, 0x6f80000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x160a0000, -1, 0x6f80000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x16060000, -1, 0x6fa0000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x160a0000, -1, 0x6f60000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x160a0000, -1, 0x6fa0000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x16060000, -1, 0x6f60000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x16080000, -1, 0x6f80000, 1);
    Task_Wait(4);
}

void FieldScene_RunScene371_02001680(void)
{

    u32 i;
    s32 record;

    Camera_MoveTo(0x15ec0000, -1, 0x6c80000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x15e40000, -1, 0x6cc0000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x15ec0000, -1, 0x6c40000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x15ec0000, -1, 0x6cc0000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x15e40000, -1, 0x6c40000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x15e80000, -1, 0x6c80000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x15ea0000, -1, 0x6c80000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x15e60000, -1, 0x6ca0000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x15ea0000, -1, 0x6c60000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x15ea0000, -1, 0x6ca0000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x15e60000, -1, 0x6c60000, 1);
    Task_Wait(4);
    Camera_MoveTo(0x15e80000, -1, 0x6c80000, 1);
    Task_Wait(4);
}

void FieldScene_RunScene371_020017a4(void)
{

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Engine_ActorGet(15);
    record = Engine_ActorGet(14);
    *(s32 *)(rec7 + 8) = *(s32 *)(record + 8);
    *(s32 *)(rec7 + 16) = *(s32 *)(record + 16);
    if (*(s32 *)(rec7 + 12) < 0xa0000) {
        *(s32 *)(rec7 + 12) = 0xa0000;
        if (GameFlag_IsSet(0x200) == 0) {
            Audio_PlayCue(145);
            Object_SetAnimation(rec7, 3);
            GameFlag_Set(0x200);
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

    rec7 = Engine_ActorGet(8);
    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    *(s32 *)(rec7 + 28) = 0x14000;
    *(s32 *)(rec7 + 24) = 0x14000;
    Camera_FollowActor(8, 1);
    Event_OpenScreen();
    Actor_SetSpeed(8, 0x6666, 0x3333);
    Actor_WalkToAndWait(8, 0x14a8, 0x918);
    Event_CloseScreen();
    Event_WaitForScreen();
    GameFlag_Set(0x927);
    Event_RequestExit(102);
    Event_End();
}

void FieldScene_RunScene371_02001888(void)
{

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Engine_ActorGet(8);
    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Actor_SetPosition(8, 0x1f080000, 0xc80000);
    *(s32 *)(rec7 + 24) = 0x14000;
    *(s32 *)(rec7 + 28) = 0x14000;
    Task_Wait(1);
    Camera_FollowActor(8, 1);
    Event_OpenScreen();
    Actor_SetSpeed(8, 0x9999, 0x4ccc);
    {
        s32 shown = 0;

        *(u16 *)(rec7 + 100) = shown;
    }
    Engine_ActorEnableActionCallback(8, gTransferLeaderIdle);
    do {
        Task_Wait(1);
    } while (*(s16 *)(rec7 + 100) == 0);
    Event_CloseScreen();
    Event_WaitForScreen();
    GameFlag_Set(0x927);
    Event_RequestExit(103);
    Event_End();
}

void FieldScene_RunScene371_02001938(void)
{

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Engine_ActorGet(8);
    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Actor_SetPosition(8, 0x1f080000, 0xc80000);
    *(s32 *)(rec7 + 24) = 0x14000;
    *(s32 *)(rec7 + 28) = 0x14000;
    Task_Wait(1);
    Camera_FollowActor(8, 1);
    Event_OpenScreen();
    Actor_SetSpeed(8, 0x9999, 0x4ccc);
    {
        s32 shown = 0;

        *(u16 *)(rec7 + 100) = shown;
    }
    Engine_ActorEnableActionCallback(8, gTransferLeaderIdle);
    do {
        Task_Wait(1);
    } while (*(s16 *)(rec7 + 100) == 0);
    Event_CloseScreen();
    Event_WaitForScreen();
    GameFlag_Set(0x927);
    Event_RequestExit(104);
    Event_End();
}

void FieldScene_RunScene371_020019e8(void)
{

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Engine_ActorGet(8);
    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Actor_SetPosition(8, 0x1f080000, 0xc80000);
    *(s32 *)(rec7 + 24) = 0x14000;
    *(s32 *)(rec7 + 28) = 0x14000;
    Task_Wait(1);
    Camera_FollowActor(8, 1);
    Event_OpenScreen();
    Actor_SetSpeed(8, 0x9999, 0x4ccc);
    {
        s32 shown = 0;

        *(u16 *)(rec7 + 100) = shown;
    }
    Engine_ActorEnableActionCallback(8, gTransferLeaderIdle);
    do {
        Task_Wait(1);
    } while (*(s16 *)(rec7 + 100) == 0);
    Event_CloseScreen();
    Event_WaitForScreen();
    GameFlag_Set(0x927);
    Event_RequestExit(105);
    Event_End();
}

void FieldScene_RunScene371_02001a98(void)
{

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Engine_ActorGet(8);
    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Actor_SetPosition(8, 0x1f080000, 0xc80000);
    *(s32 *)(rec7 + 24) = 0x14000;
    *(s32 *)(rec7 + 28) = 0x14000;
    Task_Wait(1);
    Camera_FollowActor(8, 1);
    Event_OpenScreen();
    Actor_SetSpeed(8, 0x9999, 0x4ccc);
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
        Task_Wait(1);
    } while (*(s16 *)(rec7 + 100) == 0);
    Event_CloseScreen();
    Event_WaitForScreen();
    GameFlag_Set(0x927);
    Event_RequestExit(106);
    Event_End();
}

void FieldScene_RunScene371_02001b5c(void)
{

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Engine_ActorGet(8);
    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Task_Wait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Actor_SetPosition(8, 0x13e80000, 0x9180000);
    *(s32 *)(rec7 + 28) = 0x14000;
    *(s32 *)(rec7 + 24) = 0x14000;
    Task_Wait(1);
    Camera_FollowActor(8, 1);
    Event_OpenScreen();
    Actor_SetSpeed(8, 0x6666, 0x3333);
    Actor_WalkToAndWait(8, 0x13c8, 0x918);
    Event_CloseScreen();
    Event_WaitForScreen();
    GameFlag_Set(0x93e);
    GameFlag_Clear(0x927);
    Event_RequestExit(107);
    Event_End();
}

void FieldScene_RunScene371_02001c08(void)
{

    u32 i;
    s32 record;

    Battle_ClearObjectFlag5bWhenMode3();
    BattleFx_ScheduleRatioTransition(0x10000, 6);
    Event_WaitForDisplayField358Clear();
    Battle_SetObjectFlag5bWhenMode3();
    Actor_RunRepeatedMotion(8, 2);
    Event_SetMessage((s32)MsgWorldMapAfterBringingDjinniIntoYour);
    Event_ShowMessage(8, 0);
    Event_Wait(30);
    Audio_PlayCue(111);
    Menu_AnimateSelectionToEntry(0, 2);
    GameFlag_Clear(0x16f);
    GameFlag_Clear(0x171);
    ItemMenu_Open();
    Actor_Jump(8, 4, 30);
    Event_SetMessage((s32)MsgWorldMapNextIllShowHowCan);
    Event_ShowMessage(8, 0);
    GameFlag_Clear(0x16f);
    GameFlag_Set(0x171);
    ItemMenu_Open();
    Event_Wait(30);
    BattleFx_SetWeightedResult(12, 6);
}
