#include "types.h"
#include "frame.h"
#include "actor_view.h"
#include "ram_buffer.h"

struct EffectSceneWork {
  u8 reserved00[16];
  struct ActorView *mainObject;
};

void BattleEffect_SetObjectUpdateMode(struct ActorView *object, s32 mode);

void BattleEffect_StopMainActor(void) {
  struct EffectSceneWork *work = Ram_HeapSlots->effectWork;

  BattleEffect_SetObjectUpdateMode(work->mainObject, 0);
  wait(1);
}
