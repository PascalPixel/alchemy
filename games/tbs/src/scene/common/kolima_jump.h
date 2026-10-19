/// @file
/// Kolima's forest bridge and river: the hop east onto actor 10 (the third
/// placement).
#include "types.h"
#include "engine_call.h"
#include "field_obj.h"
#include "staged_actor_effect.h"
#include "game_state.h"
#include "songs.h"

/// Hops the leader to the pixel of the 16.16 position request, unless something
/// blocks the way: the leader crouches, jumps (3 pixels a frame, rising at 4)
/// and lands while actor 10 plays its animation 7; the leader is then nudged a
/// pixel north-west twice and back. Returns 1 after a hop.
static s32 StagedActor_RunStepEffect(struct StagedActorEffectRequest *request) {
  struct StagedActorEffect *actor = (void *)Object_GetById(0);
  u8 *flags = &actor->motionFlags;
  u8 saved = *flags;
  s32 ret = FieldObject_CheckMovementCollision((struct FieldActor *)actor,
                                               (struct FieldPosition *)request);

  if (ret == 0) {
    Engine_EventBegin();
    Object_SetMode((struct FieldActor *)actor, 6);
    Engine_TaskWait(6);
    Engine_AudioPlayCue(SE_JUMP);
    Object_SetMode((struct FieldActor *)actor, 7);
    actor->moveRateX = 0x30000;
    actor->moveRateZ = 0x20000;
    actor->elevationRate = 0x40000;
    *flags &= 0x7e;
    Engine_ActorSetSpriteFlags((struct FieldActor *)actor, 0);
    Engine_ActorMoveToAndWait(ACTOR_PARTY_LEADER, request->cellX,
                              request->cellZ);
    Object_SetMode((struct FieldActor *)actor, 6);
    Engine_ActorSetSpriteFlags((struct FieldActor *)actor, 1);
    *flags = ret;
    Engine_ActorSetAnimation(10, 7);
    actor->positionX += 0xffff0000;
    actor->positionZ += 0xffff0000;
    Engine_TaskWait(2);
    actor->positionX += 0xffff0000;
    actor->positionZ += 0xffff0000;
    Engine_TaskWait(10);
    actor->positionX += 0x10000;
    actor->positionZ += 0x10000;
    Engine_TaskWait(4);
    actor->positionX += 0x10000;
    actor->positionZ += 0x10000;
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
  u32 buf[3];
#if !TBS_PATCH_1
  buf[0] = Object_GetById(0)->x.fixed + 0x1e0000;
  buf[1] = Object_GetById(0)->y.fixed;
  buf[2] = Object_GetById(0)->z.fixed;
#else
  // gGameState.selectedActor
  s32 off = 500;
  struct FieldActor *p = Object_GetById(*(s32 *)((u8 *)&gGameState + off));
  u32 base = p->x.fixed & 0xfff00000;

  buf[0] = base + 0x80000;
  buf[1] = p->y.fixed;
  buf[2] = (p->z.fixed & 0xfff00000) + 0x80000;
  buf[0] = base + 0x280000;
#endif
  StagedActor_RunStepEffect((struct StagedActorEffectRequest *)buf);
}
