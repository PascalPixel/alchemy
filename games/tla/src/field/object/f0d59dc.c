#include "types.h"
#include "field_sprite.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"

struct Work9cItemPosition {
  u8 unk00[4];
  s32 x;
  s32 y;
  s32 z;
};

struct Work9cItemPosition *Func_080d5930(s32 *position);
void Sound_PlayFar(s32 cue);

struct GrabSave {
  u8 unk00[98];
  u8 priority;
};

struct EventWorkGrab {
  u8 unk000[388];
  s16 grabState;
};

s32 Func_080d59dc(void) {
  struct MainRoutineWork *work = &gMainRoutineWork;
  struct Actor *leader = ObjectTable_Get(work->currentOwner);
  struct FieldSprite *sprite = leader->animation;
  struct Work9cItemPosition *item = Func_080d5930(&leader->x);
  u8 *flags;

  ((struct EventWorkGrab *)Ram_HeapSlots->eventWork)->grabState = 0;
  if (item == NULL)
    return 0;
  Sound_PlayFar(152);
  leader->acceleration = 0x10000;
  leader->speedLimit = 0x20000;
  leader->velocityY = 0x40000;
  leader->watchedObject = (struct Actor *)item;
  Actor_SetMode(leader, 2);
  Actor_SetTargetPosition(leader, item->x, item->y, item->z);
  Actor_CommitPosition(leader);
  leader->action = leader->motionFlags;
  leader->unk66 = sprite->nextFrame;
  ((struct GrabSave *)leader)->priority = sprite->priority;
  flags = &leader->priorityFlags;
  leader->priorityFlags = *flags & ~1;
  sprite->nextFrame = 0;
  sprite->priority = 1;
  work->renderMode = 4;
  return 1;
}
