#include "jupiter_lighthouse_room_7.h"
#include "scene_tables.h"
#include "event_inline.h"

void Scene_SetArrivalFlags(s32 flags);
void Func_02001224(s32 mode);
s32 Func_02000290(void);
void Func_020002f0(void);
s32 Func_02002b5c(s32 x, s32 z, s32 layer);
void Func_02002b54(s32 x, s32 z, s32 layer, s32 value);
void Func_02002c4c(s32 x, s32 z, s32 layer, s32 value);

s32 Func_02000fc0(void) {
  struct Actor *object;
  s32 value;

  Scene_SetArrivalFlags(0);
  Func_02001224(1);
  if (gMainRoutineWork.entrance == 12) {
    /* FAKEMATCH(wrapper): the placements forwarded through the call layer; as
     * direct calls, they share 0xa80000 and the z values in saved registers
     * and 46 instructions differ. */
    Actor_SetPosition(10, PIXELS(168), PIXELS(264));
    Actor_SetPosition(11, PIXELS(200), PIXELS(264));
    Actor_SetPosition(12, PIXELS(232), PIXELS(264));
    Actor_SetPosition(13, PIXELS(168), PIXELS(296));
    Actor_SetPosition(14, PIXELS(136), PIXELS(328));
    Actor_SetPosition(15, PIXELS(168), PIXELS(328));
  }
  if (!Event_GetFlag(0x109) && Func_02000290() != 2) {
    Event_ClearFlag(0xa10);
    Event_ClearFlag(0xa12);
    Event_ClearFlag(0xa14);
    Event_ClearFlag(0xa11);
    Event_ClearFlag(0xa13);
    Event_ClearFlag(0xa15);
  }
  if (Func_02000290() == 2)
    Event_ClearFlag(0xa16);
  else
    Event_SetFlag(0xa16);
  if (Event_GetFlag(0xa17))
    Event_ClearFlag(0xa1a);
  else
    Event_SetFlag(0xa1a);
  Actor_Get(18)->motionFlags = 0;
  Actor_Get(19)->motionFlags = 0;
  Actor_Get(20)->motionFlags = 0;
  Func_020002f0();
  if (!Event_GetFlag(0xa1a)) {
    object = Actor_Get(20);
    value = Func_02002b5c(object->x, object->z, 0);
    Func_02002b54(object->x, object->z, 0, value + 2);
    Func_02002c4c(object->x, object->z, 0, 0x80);
  }
  Func_020005e8(0);
  return 0;
}
