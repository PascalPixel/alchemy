#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "IO_REG.H"
#include "IO_WRITE_QUEUE.H"
#include "TYPES.H"
#include "CALL.H"
#include "ANIMSPR.H"
#include "OBJECT_RUNTIME.H"

extern u8 gMenuCtrlWork[];

/* The competitor's starting position and facing, kept in the overlay's
 * variables while a round runs. */
extern s32 Korosseo_CompetitorStartX;
extern s32 Korosseo_CompetitorStartZ;
extern s32 Korosseo_CompetitorStartAngle;

struct CompetitorState {
    u8 unknown_00[6];
    u8 mode;
    u8 stage;
};

#define QUEUE_IO_WRITE(address, value, delay) \
    do { \
        struct IoWriteQueue *queue = &gIoWriteQueue; \
        u32 saved = REG_IME; \
        s32 count; \
        REG_IME = 0; \
        count = queue->count; \
        if (count < 32) { \
            queue->count = count + 1; \
            queue->entries[count][0] = (value); \
            queue->entries[count][1] = (address); \
            queue->entries[count][2] = (delay); \
        } \
        REG_IME = saved; \
    } while (0)

void Engine_ActorSetAnimation();
void Engine_ActorFaceDirection();
void Engine_EventWait();
void Engine_ActorSetSpriteFlags();
void Object_SetMode();
void Engine_TaskWait();

/* Save the competitor's starting position and fade in its sprite. */
void Korosseo_FadeInCompetitor(s32 id, s32 x, s32 z)
{
    struct CompetitorState *state;
    struct FieldActor *actor;
    struct AnimationObject *sprite;
    s32 i;

    state = *(struct CompetitorState **)gMenuCtrlWork;
    actor = Object_GetById(id);
    state->mode = 1;
    state->stage = 4;
    Korosseo_CompetitorStartX = actor->x.fixed;
    Korosseo_CompetitorStartZ = actor->z.fixed;
    sprite = (struct AnimationObject *)actor->sprite;
    Korosseo_CompetitorStartAngle = actor->facing;
    Engine_ActorSetSpritePriority(id, 2);
    actor->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
    actor->facing = FACING_SOUTH;
    Engine_ActorSetSpriteFlags(actor, 3);
    Object_SetMode(actor, 0);
    Object_SetMode(actor, 1);
    Engine_ActorSetPosition(id, x << 16, z << 16);
    Engine_ActorFaceActor(0, 0x4000, 0);
    QUEUE_IO_WRITE(0x4000050, 0xf00, 0x20000);
    sprite->part[0].object_mode = 1;
    sprite->part[1].object_mode = 1;
    Engine_AudioPlayCue(252);
    for (i = 0; i <= 15; i += 2) {
        actor->scale_x = (i << 12) + 0x1000;
        actor->scale_y = 0x1f000 - (i << 12);
        QUEUE_IO_WRITE(0x4000052, ((15 - i) << 8) | (i + 1), 0x20000);
        Engine_TaskWait(1);
    }
    QUEUE_IO_WRITE(0x4000052, 16, 0x20000);
    actor->scale_x = 0x11000;
    actor->scale_y = 0xf000;
    Engine_EventWait(1);
    actor->scale_x = 0x10000;
    actor->scale_y = 0x10000;
    Engine_EventWait(13);
    sprite->part[0].object_mode = 0;
    sprite->part[1].object_mode = 0;
    Engine_ActorSetAnimationAndWait(id, 3);
    Engine_EventWait(20);
}

/* Colosso: put the competitor back at its stored start position and facing
 * after a round, replaying the fall animation unless the retry flag is set.
 * The same function sits in each of the three Colosso trial overlays. */
void Korosseo_RestoreCompetitor(s32 id)
{
    struct CompetitorState *state;
    struct FieldActor *actor;

    state = *(struct CompetitorState **)gMenuCtrlWork;
    actor = Object_GetById(id);
    if (gGameState.movement_mode == 1) {
        gGameState.movement_mode = 0;
        Engine_ActorSetAnimation(id, 1);
    } else {
        Call3(Engine_ActorFaceDirection, id, FACING_SOUTH, 30);
        Engine_ActorSetAnimation(id, 3);
        Engine_EventWait(30);
    }
    state->stage = 0;
    state->mode = 15;
    actor->x.fixed = Korosseo_CompetitorStartX;
    actor->z.fixed = Korosseo_CompetitorStartZ;
    actor->facing = Korosseo_CompetitorStartAngle;
    actor->target_x = ACTOR_NO_TARGET;
    actor->target_z = ACTOR_NO_TARGET;
    actor->velocity_x = 0;
    actor->velocity_z = 0;
    actor->motion_flags = 3;
    actor->unknown_22 = 0;
    actor->y.fixed = 0;
    ((struct ObjectRuntime *)actor)->terrain_height = 0;
    Engine_ActorSetSpriteFlags(actor, 1);
    Object_SetMode(actor, 0);
    Object_SetMode(actor, 1);
    Engine_TaskWait(1);
}
