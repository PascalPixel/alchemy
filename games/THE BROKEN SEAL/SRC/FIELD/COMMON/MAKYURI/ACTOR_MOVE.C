#include "TYPES.H"
#include "FIELD_EFFECT.H"
#include "IWRAM_CALL.H"

void Field_OffsetPosition(s32 radius, s32 angle, union FieldCoordinate *pos);
s32 Object_CheckMovementCollision(struct FieldActor *actor, union FieldCoordinate *pos);
s32 AnimationObjects_SelectAnimationFar(struct FieldSprite *sprite, s32 animation);
void Makyuri_SpawnMoveEffect(struct FieldActor *actor);

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
extern const s32 Makyuri_StartMoveScript[];
extern const s32 Makyuri_EndMoveScript[];
extern u32 Makyuri_ContinueInput;
extern u32 Makyuri_DirectionState;

struct PushState {
    s32 count;
    s32 level;
    s32 active;
    s32 target_x;
    s32 target_z;
    struct FieldActor *source;
    struct FieldActor *effect;
};

#define MAP_CELLS ((struct MapCell *)0x02010000)

/* FAKEMATCH: the inline boundary rematerializes pos for the collision call. */
static __inline__ s32 CheckMove(struct FieldActor *actor, union FieldCoordinate *pos)
{
    return Object_CheckMovementCollision(actor, pos);
}

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
    angle = (u16)Makyuri_MoveAngles[(Makyuri_DirectionState >> 4) & 15];
    if (Makyuri_MoveAngles[(Makyuri_DirectionState >> 4) & 15] == -1)
        return;
    pos[0].fixed = (actor->x.fixed & -0x100000) + 0x80000;
    pos[1].fixed = *(s32 *)((u8 *)actor + 20);
    pos[2].fixed = (actor->z.fixed & -0x100000) + 0x80000;
    from = &MAP_CELLS[(pos[2].fixed / 0x100000 << 7) + pos[0].fixed / 0x100000];
    Field_OffsetPosition(0x200000, angle, pos);
    to = &MAP_CELLS[(pos[2].fixed / 0x100000 << 7) + pos[0].fixed / 0x100000];
    if (from->level != state->level && to->level == state->level && state->count == 0)
        return;

    Engine_EventBegin();
    if (CheckMove(actor, pos) != 0)
        return;
    obj = state->effect;
    if (obj != NULL) {
        ((union FieldObject *)obj)->effect.spin = 0;
        Engine_ObjectSetScript(obj, Makyuri_EndMoveScript);
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
            Engine_ObjectSetPosition(obj, pos[0].fixed, pos[1].fixed, pos[2].fixed);
            if (sprite != NULL) {
                AnimationObjects_SelectAnimationFar(sprite, 6);
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
    Engine_TaskWait(3);
    Engine_AudioPlayCue(152);
    Engine_ObjectSetAnimation(actor, 7);
    actor->speed = 0x30000;
    actor->acceleration = 0x20000;
    actor->velocity_y = 0x40000;
    *flags &= 0x7e;
    Engine_ActorSetSpriteFlags(actor, 0);
    Engine_ObjectMotionSetPositionAndCommit(0, pos[0].part.pixel, pos[2].part.pixel);
    Engine_ObjectSetAnimation(actor, 6);
    Engine_TaskWait(2);
    if (to->level != state->level)
        Engine_ActorSetSpriteFlags(actor, 1);
    else
        Engine_AudioPlayCue(215);
    Engine_TaskWait(1);
    *flags = saved;
    if (to->level == state->level && state->effect == NULL) {
        Engine_ObjectSetAnimation(actor, 18);
        Engine_AudioPlayCue(241);
        for (i = 0;; i++) {
            if ((i & 15) == 0)
                Makyuri_SpawnMoveEffect(actor);
            if (i > 31 && Makyuri_ContinueInput != 0)
                break;
            Engine_TaskWait(1);
        }
        Engine_AudioPlayCue(0x120);
        Engine_TaskWait(1);
        actor->x.fixed = state->target_x;
        actor->z.fixed = state->target_z;
        Engine_ActorSetSpriteFlags(actor, 1);
    }
    state->active = 0;
    Engine_EventEnd();
    *(s32 *)(work + 0x1b4) += Iwram_MulQ16(*(s32 *)(work + 0x1b0), 0x200000);
}
