// Under -mthumb-slow-loads (2026-10-26): differ 4 of 103; left: two stores of
// the position array issue one row later than the native's; a barrier after
// position[0] fixes one.
// Draft, differ 4 of 103: hides the glow: shrinks its second sprite, then its
// first, waits a frame, stops its callback and the shake and frees its
// video slots and work. Left: the native stores position[0] before it
// finishes the next field offset (adds r1, #4) and position[1] before it
// builds the third offset; an empty asm after the first store moves the
// stack setup instead. Permute, 3 minutes: nothing better.
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
    WaitFrames(1);
    time++;
  } while (state != 999);
  Task_RemoveFar(Func_02002de4);
  Func_02003434(0, 0, 0);
  Resource_ResetEntry(FIELD_AT(work, s16, 4188));
  Resource_ResetEntry(FIELD_AT(work, s16, 4192));
  HeapSlot_Release(220);
}
