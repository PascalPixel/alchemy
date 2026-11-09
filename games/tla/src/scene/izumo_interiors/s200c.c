#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_inline.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Func_0200200c(void) {
  FIELD_AT(Ram_HeapSlots->eventWork, s32, 428) = 0x201;
  if (gMainRoutineWork.entrance != 7)
    Event_ClearFlag(0x350);
  if (GameFlag_Test(0x927) && !GameFlag_Test(0x928) &&
      gMainRoutineWork.entrance == 7) {
    Event_PlaySound(7);
    Actor_SetPosition(22, 0x900000, 0x2380000);
    Actor_SetPosition(21, 0, 0);
    Actor_SetPosition(16, 0, 0);
    Actor_SetPosition(17, 0, 0);
    Actor_SetPosition(18, 0, 0);
    Actor_SetPosition(19, 0, 0);
  }
  if (GameFlag_Test(0x927) && GameFlag_Test(0x928) &&
      gMainRoutineWork.entrance == 7 && GameFlag_Test(0x350)) {
    Event_PlaySound(7);
    Actor_SetPosition(16, 0, 0);
    Actor_SetPosition(17, 0, 0);
    Actor_SetPosition(18, 0, 0);
    Actor_SetPosition(19, 0, 0);
  }
  if (gMainRoutineWork.entrance == 5 && !GameFlag_Test(0x927)) {
    Actor_SetAnimation(17, 5);
    Actor_SetAttachedEffect(17, 0x101);
  }
  if (gMainRoutineWork.entrance == 7 && !GameFlag_Test(0x927)) {
    struct Actor *object;

    Actor_SetAnimation(9, 5);
    Actor_SetAnimation(10, 5);
    Actor_SetAnimation(11, 5);
    Actor_SetAnimation(12, 5);
    object = Actor_GetById(11);
    object->scaleX = -Actor_GetById(11)->scaleX;
  }
  return 0;
}
