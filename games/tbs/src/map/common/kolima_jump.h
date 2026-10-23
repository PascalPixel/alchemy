/// @file
/// Kolima Forest (SceneId_KolimaForestEntrance and SceneId_KolimaForestCliff):
/// the hop east onto the scene's third placed actor.
#include "types.h"
#include "engine_call.h"
#include "object_event.h"
#include "field_runtime.h"
#include "game_state.h"
#include "songs.h"

/// Hops the leader to the pixel of the 16.16 position request, unless something
/// blocks the way: the leader crouches, jumps (3 pixels a frame, rising at 4)
/// and lands while that actor plays its animation 7; the leader then dips a
/// pixel twice, its height and its ground height together, and rises back.
/// Returns 1 after a hop.
static s32 StagedActor_RunStepEffect(struct FieldPosition *request) {
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
  u8 *flags = &actor->motionFlags;
  u8 saved = *flags;
  s32 ret = FieldObject_CheckMovementCollision(actor, request);

  if (ret == 0) {
    Engine_EventStart();
    Object_SetMode(actor, 6);
    Engine_TaskWait(6);
    Engine_AudioPlayCue(SE_JUMP);
    Object_SetMode(actor, 7);
    actor->speed = PIXELS(3);
    actor->acceleration = PIXELS(2);
    actor->velocityY = PIXELS(4);
    *flags &= 0x7e;
    Engine_ActorSetSpriteFlags(actor, 0);
    Engine_ActorMoveToAndWait(
        ACTOR_PARTY_LEADER, ((union FieldCoordinate *)&request->x)->part.pixel,
        ((union FieldCoordinate *)&request->z)->part.pixel);
    Object_SetMode(actor, 6);
    Engine_ActorSetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
    *flags = ret;
    Engine_ActorSetAnimation(ACTOR_FIRST_PLACED + 2, 7);
    actor->y.fixed += -PIXELS(1);
    actor->terrainHeight += -PIXELS(1);
    Engine_TaskWait(2);
    actor->y.fixed += -PIXELS(1);
    actor->terrainHeight += -PIXELS(1);
    Engine_TaskWait(10);
    actor->y.fixed += PIXELS(1);
    actor->terrainHeight += PIXELS(1);
    Engine_TaskWait(4);
    actor->y.fixed += PIXELS(1);
    actor->terrainHeight += PIXELS(1);
    *flags = saved;
    Engine_EventEnd();
    return 1;
  }
  return 0;
}

/// Touch trigger 11, walking east: hop 30 pixels east of the leader. The
/// patched editions hop from the centre of the moving character's cell, 40
/// pixels.
static void KolimaForest_HopEast(void) {
  struct FieldPosition target;
#if !TBS_PATCH_1
  target.x = Object_GetById(ACTOR_PARTY_LEADER)->x.fixed + PIXELS(30);
  target.y = Object_GetById(ACTOR_PARTY_LEADER)->y.fixed;
  target.z = Object_GetById(ACTOR_PARTY_LEADER)->z.fixed;
#else
  // gGameState.selectedActor
  s32 off = 500;
  struct FieldActor *p = Object_GetById(*(s32 *)((u8 *)&gGameState + off));
  u32 base = p->x.fixed & 0xfff00000;

  target.x = base + PIXELS(8);
  target.y = p->y.fixed;
  target.z = (p->z.fixed & 0xfff00000) + PIXELS(8);
  target.x = base + PIXELS(40);
#endif
  StagedActor_RunStepEffect(&target);
}
