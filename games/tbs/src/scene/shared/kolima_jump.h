/// @file
/// Kolima Forest (SceneId_KolimaForestEntrance and SceneId_KolimaForestCliff):
/// the hop east onto the scene's third placed actor.
#include "types.h"
#include "system_call.h"
#include "actor.h"
#include "scene.h"
#include "main_routine_work.h"
#include "sounds.h"

/// Hops the leader to the pixel of the 16.16 position request, unless something
/// blocks the way: the leader crouches, jumps (3 pixels a frame, rising at 4)
/// and lands while that actor plays its animation 7; the leader then
/// dips a pixel twice, its height and its ground height together, and rises
/// back. Returns 1 after a hop.
static s32 PushedActor_RunStepEffect(struct FieldPosition *request) {
  struct Actor *actor = Actor_GetById(ACTOR_PARTY_LEADER);
  u8 *flags = &actor->motionFlags;
  u8 saved = *flags;
  s32 ret = FieldActor_CheckMovementCollision(actor, request);

  if (ret == 0) {
    Event_Start();
    Actor_SelectAnimation(actor, ANIM_LEADER_SPRING);
    wait(6);
    Sound_Play(SE_JUMP);
    Actor_SelectAnimation(actor, ANIM_LEADER_LEAP);
    actor->speedLimit = px(3);
    actor->acceleration = px(2);
    actor->velocityY = px(4);
    *flags &= ACTOR_JUMP_KEEPS_MOTION;
    Actor_SetSpriteFlags(actor, 0);
    Actor_MoveToAndWaitById(ACTOR_PARTY_LEADER,
                            ((union FieldCoordinate *)&request->x)->part.pixel,
                            ((union FieldCoordinate *)&request->z)->part.pixel);
    Actor_SelectAnimation(actor, ANIM_LEADER_SPRING);
    Actor_SetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
    *flags = ret;
    Actor_SetAnimation(ACTOR_FIRST_PLACED + 2, 7);
    actor->y.fixed += -px(1);
    actor->groundHeight += -px(1);
    wait(2);
    actor->y.fixed += -px(1);
    actor->groundHeight += -px(1);
    wait(10);
    actor->y.fixed += px(1);
    actor->groundHeight += px(1);
    wait(4);
    actor->y.fixed += px(1);
    actor->groundHeight += px(1);
    *flags = saved;
    Event_End();
    return 1;
  }
  return 0;
}

/// Step trigger 11, walking east: hop 30 pixels east of the leader. The
/// patched editions hop from the centre of the moving character's cell, 40
/// pixels.
static void KolimaForest_HopEast(void) {
  struct FieldPosition target;
#if !TBS_PATCH_1
  target.x = Actor_GetById(ACTOR_PARTY_LEADER)->x.fixed + px(30);
  target.y = Actor_GetById(ACTOR_PARTY_LEADER)->y.fixed;
  target.z = Actor_GetById(ACTOR_PARTY_LEADER)->z.fixed;
#else
  struct Actor *p = Actor_GetById(gMainRoutineWork.leaderActorId);
  u32 base = p->x.fixed & FIELD_CELL_MASK;

  target.x = base + px(8);
  target.y = p->y.fixed;
  target.z = (p->z.fixed & FIELD_CELL_MASK) + px(8);
  target.x = base + px(40);
#endif
  PushedActor_RunStepEffect(&target);
}
