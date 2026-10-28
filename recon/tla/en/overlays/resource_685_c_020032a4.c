// Draft, differ 81 of 116: the glow flares out: its second sprite swells to
// double size, falls back, then both shrink to nothing; it then stops the
// callback and frees its video slots and work.
#include "types.h"
#include "ram_buffer.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void WaitFrames(s32 frames);
void Task_RemoveFar(void *func);
s32 Resource_ResetEntry(u32 entry);
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
        time = -1;
        state = 1;
      }
      break;
    case 1:
      if ((FIELD_AT(work, s32, 4204) -= 0x1000) <= 0x14ccb) {
        FIELD_AT(work, s32, 4204) = 0x14ccc;
        time = -1;
        state = 2;
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
    WaitFrames(1);
    time++;
  } while (state != 999);
  Task_RemoveFar(Func_02002de4);
  Resource_ResetEntry(FIELD_AT(work, s16, 4188));
  Resource_ResetEntry(FIELD_AT(work, s16, 4192));
  HeapSlot_Release(220);
}
