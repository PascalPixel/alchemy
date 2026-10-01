/* NONMATCHING: 2026-10-01 brief Wave2 CheckMove plain-source attempt.
 * Removing this one source device changes Makyuri_RunActorMove.
 * Remaining difference: a direct call changes Makyuri_RunActorMove from add r1, sp, #16 to mov r1, fp (403/403 assembly lines).
 * Measured with the existing TBS agscc option set, EN edition; no option changes.
 * Production retains the measured helper with a body-local FAKEMATCH reason.
 */
/* Mercury Lighthouse: the leader hops one cell toward the held direction
 * over the water pillars. The same function sits in the entrance and the
 * rooms overlays. */
#include "TYPES.H"
#include "MAKYURI.H"
#include "FIELD_EFFECT.H"
#include "IWRAM_CALL.H"
#include "RAM_BUFFER.H"

void Vector_AddPolarOffset(s32 radius, s32 angle, union FieldCoordinate *pos);
s32 Object_CheckMovementCollision(struct FieldActor *actor, union FieldCoordinate *pos);
s32 AnimationObjects_SelectAnimation(struct FieldSprite *sprite, s32 animation);
void OverlayObject_SpawnKind24AtObject(struct FieldActor *actor);
void Object_SetPosition(struct FieldActor *object, s32 fixed_x, s32 fixed_y, s32 fixed_z);

/* One cell of the height map: the third byte is the cell's floor level. */
struct MapCell {
    u8 unknown_0[2];
    u8 level;
    u8 unknown_3;
};

struct EventActors {
    u8 unknown_00[20];
    struct FieldActor *actors[1];
};

extern s16 Makyuri_MoveAngles[];
extern const s32 Makyuri_RampScript[];
extern const s32 Makyuri_ScaleCounterScript[];
extern u32 gKeyState;
extern u32 gKeysHeld;

struct PushState {
    s32 count;
    s32 level;
    s32 active;
    s32 target_x;
    s32 target_z;
    struct FieldActor *source;
    struct FieldActor *effect;
};

#define MAP_CELLS ((struct MapCell *)Ram_MapCellBuffer)

/* FAKEMATCH: the inline boundary rematerializes pos for the collision call. */


struct SpawnPoint;

struct EventSpawns {
    u8 unknown_00[20];
    struct SpawnPoint *points[1];
};

struct SceneTimer {
    u8 unknown_00[8];
    s32 count;
};

/* The IWRAM event globals: the event work, and at +0x20 the scene work. */
struct EventGlobals {
    struct EventSpawns *event;
    u8 unknown_04[0x1c];
    struct SceneTimer **scene;
};

extern struct EventGlobals gWork;

void Makyuri_RunActorMove(void)
{
    u8 *work;
    struct PushState *state;
    struct FieldActor *actor;
    struct FieldActor *source;
    struct FieldActor *obj;
    struct FieldSprite *sprite;
    struct MapCell *from;
    struct MapCell *to;
    u8 saved;
    u8 *flags;
    s32 angle;
    s32 n;
    s32 i;
    u8 zero;
    union FieldCoordinate pos[3];

    state = **(struct PushState ***)((u8 *)&gEventWork + 32);
    work = (u8 *)gEventWork;
    actor = ((struct EventActors *)work)->actors[gGameState.selected_actor];
    flags = &actor->motion_flags;
    saved = *flags;
    angle = (u16)Makyuri_MoveAngles[(gKeysHeld >> 4) & 15];
    if (Makyuri_MoveAngles[(gKeysHeld >> 4) & 15] == -1)
        return;
    pos[0].fixed = (actor->x.fixed & -0x100000) + 0x80000;
    pos[1].fixed = *(s32 *)((u8 *)actor + 20);
    pos[2].fixed = (actor->z.fixed & -0x100000) + 0x80000;
    from = &MAP_CELLS[(pos[2].fixed / 0x100000 << 7) + pos[0].fixed / 0x100000];
    Vector_AddPolarOffset(0x200000, angle, pos);
    to = &MAP_CELLS[(pos[2].fixed / 0x100000 << 7) + pos[0].fixed / 0x100000];
    if (from->level != state->level && to->level == state->level && state->count == 0)
        return;

    Engine_EventBegin();
    if (Object_CheckMovementCollision(actor, pos) != 0)
        return;
    obj = state->effect;
    if (obj != NULL) {
        ((union FieldObject *)obj)->effect.spin = 0;
        Engine_ObjectSetScript(obj, Makyuri_ScaleCounterScript);
        Object_SetMode(obj, 7);
        state->effect = NULL;
    }
    if (to->level == state->level && state->count != 0) {
        source = state->source;
        obj = Engine_ObjectCreate(26, source->x.fixed, source->y.fixed, source->z.fixed);
        if (obj != NULL) {
            sprite = obj->sprite;
            *(s32 *)((u8 *)obj + 20) = *(s32 *)((u8 *)source + 20);
            Engine_ObjectSetScript(obj, Makyuri_RampScript);
            obj->motion_flags = 0;
            ((union FieldObject *)obj)->effect.spin = 0;
            obj->priority_flags = 2;
            obj->speed = 0x40000;
            obj->acceleration = 0x20000;
            Object_SetPosition(obj, pos[0].fixed, pos[1].fixed, pos[2].fixed);
            if (sprite != NULL) {
                AnimationObjects_SelectAnimation(sprite, 6);
                zero = 0;
                ((u8 *)sprite)[38] = zero;
            }
            state->effect = obj;
        }
        n = state->count - 1;
        state->count = n;
        if (n == 0) {
            Engine_ObjectDispatchRelease(state->source);
            state->source = NULL;
            Engine_GameFlagClear(0x161);
        } else if (state->source != NULL) {
            Object_SetMode(state->source, 6 - n);
        }
    }
    Object_SetMode(actor, 6);
    WaitFrames(3);
    Audio_PlayCue(152);
    Object_SetMode(actor, 7);
    actor->speed = 0x30000;
    actor->acceleration = 0x20000;
    actor->velocity_y = 0x40000;
    *flags &= 0x7e;
    Engine_ActorSetSpriteFlags(actor, 0);
    Engine_ActorMoveToAndWait(0, pos[0].part.pixel, pos[2].part.pixel);
    Object_SetMode(actor, 6);
    WaitFrames(2);
    if (to->level != state->level)
        Engine_ActorSetSpriteFlags(actor, 1);
    else
        Audio_PlayCue(215);
    WaitFrames(1);
    *flags = saved;
    if (to->level == state->level && state->effect == NULL) {
        Object_SetMode(actor, 18);
        Audio_PlayCue(241);
        for (i = 0;; i++) {
            if ((i & 15) == 0)
                OverlayObject_SpawnKind24AtObject(actor);
            if (i > 31 && gKeyState != 0)
                break;
            WaitFrames(1);
        }
        Audio_PlayCue(0x120);
        WaitFrames(1);
        actor->x.fixed = state->target_x;
        actor->z.fixed = state->target_z;
        Engine_ActorSetSpriteFlags(actor, 1);
    }
    state->active = 0;
    Engine_EventEnd();
    *(s32 *)(work + 0x1b4) += Iwram_MulQ16(*(s32 *)(work + 0x1b0), 0x200000);
}

/* Mercury Lighthouse spawn timer: count the scene timer down, and when it runs out spawn an object at the current spawn point and restart it at 10 to 39 frames. */
void Makyuri_TickSpawnTimer(void)
;
