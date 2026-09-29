#include "STORY.H"

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

enum {
    CONTACT_LAST_ACTOR = 65,
    /* An actor's touch trigger is its id plus this base. */
    CONTACT_TRIGGER_BASE = 100,
    FLAG_CONTACT_PAUSED = 0x163,
    FLAG_CONTACT_BLOCKED = 0x104
};

extern u8 gDebugMode[];

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
