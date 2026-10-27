/// @file
/// Kolima Forest (SceneId_KolimaForestEntrance and SceneId_KolimaForestCliff):
/// the hop east onto the map's third placed actor.
#include "types.h"
#include "engine_call.h"
#include "actor.h"
#include "scene_header.h"
#include "save_block.h"
#include "songs.h"

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
    Actor_SelectAnimation(actor, 6);
    WaitFrames(6);
    Sound_PlaySong(SE_JUMP);
    Actor_SelectAnimation(actor, 7);
    actor->speedLimit = PIXELS(3);
    actor->acceleration = PIXELS(2);
    actor->velocityY = PIXELS(4);
    *flags &= 0x7e;
    Actor_SetSpriteFlags(actor, 0);
    Actor_SetPositionAndCommit(
        ACTOR_PARTY_LEADER, ((union FieldCoordinate *)&request->x)->part.pixel,
        ((union FieldCoordinate *)&request->z)->part.pixel);
    Actor_SelectAnimation(actor, 6);
    Actor_SetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
    *flags = ret;
    Import_ActorSetAnimation(ACTOR_FIRST_PLACED + 2, 7);
    actor->y.fixed += -PIXELS(1);
    actor->groundHeight += -PIXELS(1);
    WaitFrames(2);
    actor->y.fixed += -PIXELS(1);
    actor->groundHeight += -PIXELS(1);
    WaitFrames(10);
    actor->y.fixed += PIXELS(1);
    actor->groundHeight += PIXELS(1);
    WaitFrames(4);
    actor->y.fixed += PIXELS(1);
    actor->groundHeight += PIXELS(1);
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
  target.x = Actor_GetById(ACTOR_PARTY_LEADER)->x.fixed + PIXELS(30);
  target.y = Actor_GetById(ACTOR_PARTY_LEADER)->y.fixed;
  target.z = Actor_GetById(ACTOR_PARTY_LEADER)->z.fixed;
#else
  // gSaveBlock.leaderActorId
  s32 off = 500;
  struct Actor *p = Actor_GetById(*(s32 *)((u8 *)&gSaveBlock + off));
  u32 base = p->x.fixed & 0xfff00000;

  target.x = base + PIXELS(8);
  target.y = p->y.fixed;
  target.z = (p->z.fixed & 0xfff00000) + PIXELS(8);
  target.x = base + PIXELS(40);
#endif
  PushedActor_RunStepEffect(&target);
}
