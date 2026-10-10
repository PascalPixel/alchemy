#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "field_event.h"
#include "field_scene.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "types.h"
#include "call.h"
#include "animspr.h"
#include "object_runtime.h"




struct CompetitorState {
    u8 unknown_00[6];
    u8 mode;
    u8 stage;
};

#define QUEUE_IO_WRITE(address, value, delay) \
    do { \
        /* FAKEMATCH: needed to match: retain the measured one-pass pointer/read lifetime; direct IME access loads a separate 520 constant. */ \
        volatile u16 *ime; \
        struct IoWriteQueue *queue = &gIoWriteQueue; \
        u32 saved; \
        s32 count; \
        do { \
            /* FAKEMATCH: needed to match: retain the existing IME pointer/read initialization boundary. */ \
            ime = &REG_IME; \
            saved = *ime; \
        } while (0); \
        *ime = (u16)(u32)ime; \
        count = queue->count; \
        if (count < 32) { \
            u32 *entry = (u32 *)((u8 *)queue + count * (s32)sizeof(queue->entries[0]) \
                + (s32)&((struct IoWriteQueue *)0)->entries); \
            *(u16 *)&queue->count = count + 1; \
            *entry++ = (value); \
            *entry++ = (address); \
            *entry = (delay); \
        } \
        *ime = saved; \
    } while (0)

void Korosseo_FadeInCompetitor(s32 id, s32 x, s32 z)
{
    struct CompetitorState *state;
    struct FieldActor *actor;
    struct AnimationObject *sprite;
    s32 i;

    state = (struct CompetitorState *)gMenuCtrlWork;
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
    /* FAKEMATCH: needed to match: the halfword count view and IME lifetime keep row-address setup before the count writes. */
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

void Korosseo_RestoreCompetitor(s32 id)
{
    struct CompetitorState *state;
    struct FieldActor *actor;
    s32 zero;
    u8 *control;

    state = (struct CompetitorState *)gMenuCtrlWork;
    actor = Object_GetById(id);
    if (gGameState.movement_mode == 1) {
        gGameState.movement_mode = 0;
        Engine_ActorSetAnimation(id, 1);
    } else {
        Actor_FaceDirection(id, FACING_SOUTH, 30);
        Engine_ActorSetAnimation(id, 3);
        Engine_EventWait(30);
    }
    zero = 0;
    /* FAKEMATCH: needed to match: byte accesses keep the stage zero before the start-position load. */
    control = (u8 *)state;
    control[(u32)&((struct CompetitorState *)0)->stage] = zero;
    control[(u32)&((struct CompetitorState *)0)->mode] = 15;
    actor->x.fixed = Korosseo_CompetitorStartX;
    actor->z.fixed = Korosseo_CompetitorStartZ;
    actor->facing = Korosseo_CompetitorStartAngle;
    actor->target_x = ACTOR_NO_TARGET;
    actor->target_z = ACTOR_NO_TARGET;
    actor->velocity_x = zero;
    actor->velocity_z = zero;
    actor->motion_flags = 3;
    actor->terrain_id = 0;
    actor->y.fixed = zero;
    ((struct ObjectRuntime *)actor)->terrain_height = zero;
    Engine_ActorSetSpriteFlags(actor, 1);
    Object_SetMode(actor, 0);
    Object_SetMode(actor, 1);
    Engine_TaskWait(1);
}
