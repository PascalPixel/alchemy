// Draft, differ 4 of 83: a scheduled callback that, unless the leader is in
// movement mode 4, moves it one pixel along z each frame and, every eighth frame
// while it moves, kicks up a puff behind it (the effect spawner with a random
// spread and the leader's sprite priority).
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "frame.h"
#include "field_effect.h"

struct ObjectRuntime *Object_GetById(s32 id);

u32 Random16Far(void);
void Effect_AdvanceMotionAtLeaderPriority(union FieldObject *object);
void Func_0200129c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);

void Func_020014e4(void) {
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  struct EffectOptions options;
  s32 velocity[3];

  if (gPartyState.renderMode == 4)
    return;
  leader->z += 0x10000;
  if ((gFrameCount & 7) != 0)
    return;
  if (leader->velocityZ == 0 && leader->velocityX == 0)
    return;
  options.update = Effect_AdvanceMotionAtLeaderPriority;
  options.startScaleX = 0xcccc;
  options.startScaleY = 0xcccc;
  velocity[1] = 0;
  velocity[2] = 0x10000;
  velocity[0] = 0;
  velocity[0] += ((Random16Far() << 17) >> 16) - 0x10000;
  velocity[2] += ((Random16Far() << 16) >> 16) - 0x8000;
  Func_0200129c(leader->x, leader->y, leader->z, velocity[0], velocity[1],
                velocity[2], 0x1000001, &options);
}
