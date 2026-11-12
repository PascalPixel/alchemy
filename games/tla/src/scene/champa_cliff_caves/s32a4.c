#include "types.h"
#include "ram_buffer.h"
#include "event_inline.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Resource_ResetEntry(u32 slot);
void HeapSlot_Release(s32 slot);
void Func_02002de4(void);

void Func_020032a4(void) {
  u8 *work = Ram_HeapSlots->menuRuntime;
  s32 state = 0;
  s32 time = 0;

  do {
    switch (state) {
    case 0:
      if ((FIELD_AT(work, s32, 4204) += 0x1000) > 0x1ffff) {
        FIELD_AT(work, s32, 4204) = 0x20000;
        state = 1;
        time = -1;
      }
      break;
    case 1:
      if ((FIELD_AT(work, s32, 4204) -= 0x1000) <= 0x14ccb) {
        FIELD_AT(work, s32, 4204) = 0x14ccc;
        state = 2;
        time = -1;
      }
      /* fall through */
    case 2:
      if ((FIELD_AT(work, s32, 4204) -= 0x1000) < 0)
        FIELD_AT(work, s32, 4204) = 0;
      if ((FIELD_AT(work, s32, 4196) -= 0x1000) < 0) {
        FIELD_AT(work, s32, 4196) = 0;
        state++;
        time = -1;
      }
      break;
    case 3:
      if (time == 1)
        state = 999;
      break;
    }
    wait(1);
    time++;
  } while (state != 999);
  Task_RemoveFar(Func_02002de4);
  Resource_ResetEntry(FIELD_AT(work, s16, 4188));
  Resource_ResetEntry(FIELD_AT(work, s16, 4192));
  HeapSlot_Release(220);
}

#include "scene_tables.h"

u32 gData_02003634[] = {
    439,
    440,
    441,
};
