#include "types.h"
#include "field_event.h"
#include "scene_tables.h"
#include "event_inline.h"

void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Func_02002c64(void);

void Func_02002fd4(void) {
  s32 flag;

  if (!Event_GetFlag(0x863)) {
    struct Actor *actor;

    Actor_SetPosition(8, PIXELS(416), PIXELS(680));
    MapGrid_CopyCellAttributes(38, 32, 2, 1, 25, 42);
    actor = Actor_Get(8);
    Actor_SetSingleChildFlags(Actor_Get(8), 0);
    actor->angle = FACING_SOUTH;
    actor->motionFlags = 0;
    actor->y = PIXELS(12);
    actor->collisionFlags = 0;
    Actor_SetAnimation(8, 5);
  }
  if (!Event_GetFlag(0x864)) {
    struct Actor *actor;

    Actor_SetPosition(5, PIXELS(288), PIXELS(744));
    MapGrid_CopyCellAttributes(38, 33, 2, 1, 17, 46);
    actor = Actor_Get(5);
    Actor_SetSingleChildFlags(Actor_Get(5), 0);
    actor->angle = FACING_SOUTH;
    actor->motionFlags = 0;
    actor->y = PIXELS(12);
    actor->collisionFlags = 0;
    Actor_SetAnimation(5, 18);
  }
  flag = Event_GetFlag(0x865);
  if (flag == 0) {
    struct Actor *actor;

    actor = Actor_Get(6);
    Actor_SetPosition(6, PIXELS(240), PIXELS(648));
    MapGrid_CopyCellAttributes(38, 34, 2, 1, 14, 40);
    Actor_SetAnimation(6, 19);
    Actor_SetSingleChildFlags(Actor_Get(6), 0);
    actor->motionFlags = 0;
    actor->y = PIXELS(12);
  }
  MapGrid_CopyCellsTo(72, 42, 75, 40, 1, 1);
  System_Wait(1);
  if (!Event_GetFlag(0x844))
    Func_02002c64();
}
