#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "event_inline.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02004464(void);
void Func_0200446c(void);

void Func_02001770(void) {
  if (Event_GetFlag(0xa3d) == 0 && Event_GetFlag(64) == 0) {
    u8 *work = Ram_HeapSlots->eventWork;

    Func_02004464();
    FieldEvent_ResolvePendingActions(1);
    {
      s32 y = 0x80;
      Actor_SetPositionAndFacing(20, PIXELS(408),
                                                     PIXELS(145), y << 7);
    }
    System_Wait(1);
    FieldActor_Jump(20, 4, 20);
    Actor_FaceActor(20, gMainRoutineWork.currentOwner, 0);
    FIELD_AT(work, u16, 362) = 200;
    Func_0200446c();
  }
}
