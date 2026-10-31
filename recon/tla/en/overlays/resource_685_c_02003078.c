// Near miss, differ 2 of 103 (tla-en, 2026-10-31): one pair in the scheduler's
// order. The native stores position[1] between the two halves of the rebuilt
// third field offset (movs r1, #128; str; lsls); sched2 ranks that lsls at
// priority 7 and the store at 6, since the store's priority is two above the
// third load's and the lsls's three above it whatever the C. An array, a
// struct, moving state and time, or subtracting separately all measure 2.
// Hides the glow: shrinks its second sprite, then its first, waits a frame,
// stops its callback and the shake and frees its video slots and work.
// Permute, 3 minutes: nothing better.
#include "types.h"
#include "ram_buffer.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void wait(s32 frames);
void Task_RemoveFar(void *func);
void Func_02003434(s32 x, s32 y, s32 z);
s32 Resource_ResetEntry(u32 entry);
void HeapSlot_Release(s32 slot);
void Func_02002de4(void);

void Func_02003078(void) {
  u8 *work = Ram_HeapSlots->menuRuntime;
  s32 position[3];
  s32 state;
  s32 time;

  position[0] = FIELD_AT(work, s32, 4096);
  state = 0;
  position[1] = FIELD_AT(work, s32, 4100) - 0x200000;
  time = 0;
  position[2] = FIELD_AT(work, s32, 4104);
  do {
    switch (state) {
    case 0:
      if ((FIELD_AT(work, s32, 4204) -= 0x1000) < 0) {
        FIELD_AT(work, s32, 4204) = 0;
        time = -1;
        state = 1;
      }
      break;
    case 1:
      if ((FIELD_AT(work, s32, 4196) -= 0x800) < 0) {
        FIELD_AT(work, s32, 4196) = 0;
        state = 2;
        time = -1;
      }
      break;
    case 2:
      if (time == 1)
        state = 999;
      break;
    }
    wait(1);
    time++;
  } while (state != 999);
  Task_RemoveFar(Func_02002de4);
  Func_02003434(0, 0, 0);
  Resource_ResetEntry(FIELD_AT(work, s16, 4188));
  Resource_ResetEntry(FIELD_AT(work, s16, 4192));
  HeapSlot_Release(220);
}
