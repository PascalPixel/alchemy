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
  if (GameFlag_Test(0xa3d) == 0 && GameFlag_Test(64) == 0) {
    u8 *work = Ram_HeapSlots->eventWork;

    Func_02004464();
    Event_ResolvePendingActions(1);
    {
      s32 y = 0x80;
      /* FAKEMATCH: ⚓️ starts 0x4000 before the other arguments and shifts it
       * after them; plain constants build it third (differ 2). */
      asm volatile("" : "+l"(y));
      Actor_SetHorizontalPositionAndAngleWithTerrain(20, PIXELS(408),
                                                     PIXELS(145), y << 7);
    }
    wait(1);
    Actor_Jump(20, 4, 20);
    Actor_SetAngleToward(20, gMainRoutineWork.currentOwner, 0);
    FIELD_AT(work, u16, 362) = 200;
    Func_0200446c();
  }
}
