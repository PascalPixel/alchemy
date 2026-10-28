#include "types.h"
#include "adventure.h"
#include "actor.h"
#include "flag_call.h"

struct Actor *Actor_GetById(s32 id);
s32 WaitFrames(s32 frames);
s32 Map_GetTerrainHeight(s32 terrain, s32 x, s32 z);
void Camera_FollowActor(s32 id, s32 a);

void Func_020024c8(void) {
  struct Actor *object = Actor_GetById(gAdventure.currentOwner);
  /* FAKEMATCH: one zero for the velocity store and, through a 64-bit copy, the
   * call's argument clears r1 after the owner's load (one reordered pair;
   * alchemy permute). */
  u32 zero = 0;

  if (Flag_IsSet(0x109)) {
    s32 height;

    WaitFrames(1);
    height = Map_GetTerrainHeight(object->terrainId, object->x, object->z);
    object->velocityY = zero;
    object->terrainHeight = height;
    object->y = height;
    object->targetY = 0x80000000;
    {
      s64 flags = zero;

      Camera_FollowActor(gAdventure.currentOwner, flags);
    }
  }
}
