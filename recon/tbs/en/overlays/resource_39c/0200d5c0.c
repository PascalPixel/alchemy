/* Draft of Makyuri_RunActorMove, resource_39c at 0x0200d5c0, for
 * FIELD/MAKYURI_HEYA (it compiles with that directory's PROBE.H), from the
 * unlinked FIELD/COMMON/MAKYURI/ACTOR_MOVE.C. Linking it needs the rows at
 * 0x0200de44 labelled Makyuri_MoveAngles and 0x0200de20 Makyuri_GrowScript;
 * the key words are the main image's gKeysHeld and gKeyState.
 * Remaining difference: 148 halfwords from one allocation. The game keeps
 * the facing in r6 and reloads the cell buffer's address from the pool for
 * each cell lookup; here the address is shared across the polar-offset call
 * in sl, which shifts the registers after it. Looking the first cell up
 * through the inline helper (as here) is what puts the facing in r6; written
 * directly, the facing lands in ip instead and the address in r6 (267
 * halfwords), and a u16 facing loads the table once (64 halfwords, but one
 * sign-extending load where the game has two). */
#include "PROBE.H"
#include "IWRAM_CALL.H"

extern u8 gMapCellBuffer[];
extern u32 gKeysHeld;
extern u32 gKeyState;

s32 Object_CheckMovementCollision(struct FieldActor *actor, union FieldCoordinate *pos);
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

/* The facing each d-pad combination moves the actor in, or -1. */
extern s16 Makyuri_MoveAngles[];
extern const s32 Makyuri_StartMoveScript[];
extern const s32 Makyuri_GrowScript[];

struct PushState {
    s32 count;
    s32 level;
    s32 active;
    s32 target_x;
    s32 target_z;
    struct FieldActor *source;
    struct FieldActor *effect;
};

#define MAP_CELLS ((struct MapCell *)gMapCellBuffer)

static __inline__ struct MapCell *MapCell_At(struct MapCell *cells, union FieldCoordinate *pos)
{
    return &cells[(pos[2].fixed / 0x100000 << 7) + pos[0].fixed / 0x100000];
}

/* FAKEMATCH: the inline boundary rematerializes pos for the collision call. */
static __inline__ s32 CheckMove(struct FieldActor *actor, union FieldCoordinate *pos)
{
    return Object_CheckMovementCollision(actor, pos);
}

/* Hop the selected actor one cell the way the d-pad points. Stepping onto
 * the state's floor level spends one of its charges on a growing light at
 * the target, and when the last charge is spent the actor waits there for a
 * key, then lands on the state's target. */
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
    from = MapCell_At(MAP_CELLS, pos);
    Vector_AddPolarOffset(0x200000, angle, (s32 *)pos);
    to = MapCell_At(MAP_CELLS, pos);
    if (from->level != state->level && to->level == state->level && state->count == 0)
        return;

    Engine_EventBegin();
    if (CheckMove(actor, pos) != 0)
        return;
    obj = state->effect;
    if (obj != NULL) {
        ((union FieldObject *)obj)->effect.spin = 0;
        Engine_ObjectSetScript(obj, Makyuri_GrowScript);
        Engine_ObjectSetAnimation(obj, 7);
        state->effect = NULL;
    }
    if (to->level == state->level && state->count != 0) {
        source = state->source;
        obj = Engine_ObjectCreate(26, source->x.fixed, source->y.fixed, source->z.fixed);
        if (obj != NULL) {
            sprite = obj->sprite;
            *(s32 *)((u8 *)obj + 20) = *(s32 *)((u8 *)source + 20);
            Engine_ObjectSetScript(obj, Makyuri_StartMoveScript);
            obj->motion_flags = 0;
            ((union FieldObject *)obj)->effect.spin = 0;
            obj->priority_flags = 2;
            obj->speed = 0x40000;
            obj->acceleration = 0x20000;
            Object_SetPosition(obj, pos[0].fixed, pos[1].fixed, pos[2].fixed);
            if (sprite != NULL) {
                AnimationObjects_SelectAnimation((u8 *)sprite, 6);
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
            Engine_ObjectSetAnimation(state->source, 6 - n);
        }
    }
    Engine_ObjectSetAnimation(actor, 6);
    Task_Wait(3);
    Audio_PlayCue(152);
    Engine_ObjectSetAnimation(actor, 7);
    actor->speed = 0x30000;
    actor->acceleration = 0x20000;
    actor->velocity_y = 0x40000;
    *flags &= 0x7e;
    Engine_ActorSetSpriteFlags(actor, 0);
    Engine_ActorMoveToAndWait(0, pos[0].part.pixel, pos[2].part.pixel);
    Engine_ObjectSetAnimation(actor, 6);
    Task_Wait(2);
    if (to->level != state->level)
        Engine_ActorSetSpriteFlags(actor, 1);
    else
        Audio_PlayCue(215);
    Task_Wait(1);
    *flags = saved;
    if (to->level == state->level && state->effect == NULL) {
        Engine_ObjectSetAnimation(actor, 18);
        Audio_PlayCue(241);
        for (i = 0;; i++) {
            if ((i & 15) == 0)
                OverlayObject_SpawnKind24AtObject((u8 *)actor);
            if (i > 31 && gKeyState != 0)
                break;
            Task_Wait(1);
        }
        Audio_PlayCue(0x120);
        Task_Wait(1);
        actor->x.fixed = state->target_x;
        actor->z.fixed = state->target_z;
        Engine_ActorSetSpriteFlags(actor, 1);
    }
    state->active = 0;
    Engine_EventEnd();
    *(s32 *)(work + 0x1b4) += Iwram_MulQ16(*(s32 *)(work + 0x1b0), 0x200000);
}
