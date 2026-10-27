#include "types.h"
#include "frame.h"
#include "object_dispatch.h"
#include "ram_buffer.h"

struct EffectSceneWork {
  u8 reserved00[16];
  struct DispatchObject *mainObject;
};

void BattleEffect_SetObjectTickMode(struct DispatchObject *object, s32 mode);

void EffectRuntime_StopCurrentObject(void) {
  struct EffectSceneWork *work = Ram_HeapSlots->effectWork;

  BattleEffect_SetObjectTickMode(work->mainObject, 0);
  WaitTicks(1);
}
