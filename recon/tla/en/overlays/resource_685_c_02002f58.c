// Draft, differ 130 of 136: shows the glow at a spot: allocates its work, loads
// its picture and sprites, starts its frame callback and a shake, then grows
// its first sprite, waits ten frames and grows its second in two stages.
#include "types.h"
#include "lib/call.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

typedef void (*TaskFunc)(void);

u8 *HeapSlot_AllocFar(s32 slot, s32 size);
void Func_02002c88(void);
void Func_02002d34(void);
s32 Task_Add(TaskFunc func, s32 order);
void Func_02003434(s32 x, s32 y, s32 z);
void wait(s32 frames);
void Func_02002de4(void);

void Func_02002f58(const s32 *position) {
  u8 *work = HeapSlot_AllocFar(220, 0x1078);
  s32 time;
  s32 state;

  FIELD_AT(work, s32, 4096) = position[0];
  FIELD_AT(work, s32, 4100) = position[1];
  time = 0;
  FIELD_AT(work, s32, 4104) = position[2];
  Func_02002c88();
  Func_02002d34();
  FIELD_AT(work, u16, 4212) = 0;
  Call2((void (*)())Task_Add, Func_02002de4, 0x480);
  Call3((void (*)())Func_02003434, 0x40000, 0x40000, 0x10000);
  state = 0;
  do {
    switch (state) {
    case 0:
      if ((FIELD_AT(work, s32, 4196) += 0x600) > 0x14ccb) {
        FIELD_AT(work, s32, 4196) = 0x14ccc;
        state = 1;
        time = -1;
      }
      break;
    case 1:
      if (time == 10) {
        state = 2;
        time = -1;
      }
      break;
    case 2:
      if ((FIELD_AT(work, s32, 4204) += 0x400) > 0x6665) {
        FIELD_AT(work, s32, 4204) = 0x6666;
        state = 3;
        time = -1;
      }
      break;
    case 3:
      if ((FIELD_AT(work, s32, 4204) += 0x3000) > 0x14ccb) {
        FIELD_AT(work, s32, 4204) = 0x14ccc;
        state = 999;
      }
      break;
    }
    Call1((void (*)())wait, 1);
    time++;
  } while (state != 999);
}
