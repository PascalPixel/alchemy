#include "types.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "system/io_reg.h"
#include "event_inline.h"

void Func_02001da0(void);
void Func_02001da8(void);
void Func_02001d60(void);
void Func_02001d40(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);

void Func_02000be8(void) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  u8 *lower = map + 0x108;
  u8 *upper = map + 0x178;
  s32 i, step;

  Func_02001da0();
  Event_ResolvePendingActions(0);
  Func_02001d60();
  step = 1;
  for (i = 0; i < 352; i++) {
    if (i >= 17 && i < 80) {
      s32 blend = ((step + 1) / 4) << 8;

      blend |= 16 - (step + 1) / 4;
      REG_BLDALPHA = blend;
      step++;
    }
    *(s32 *)(lower + 12) += 0x10000;
    *(s32 *)(upper + 12) += 0x10000;
  }
  Func_02001d40();
  wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 328, 184);
  Event_Wait(20);
  Sound_Play(308);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
  for (; i != 0; i--) {
    if (i >= 33 && i < 96) {
      s32 blend = ((step + 1) / 4) << 8;

      blend |= 16 - (step + 1) / 4;
      REG_BLDALPHA = blend;
      step--;
    }
    *(s32 *)(lower + 12) += -0x10000;
    *(s32 *)(upper + 12) += -0x10000;
    wait(1);
  }
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  Sound_Play(298);
  wait(1);
  Sound_Play(125);
  Event_Wait(20);
  GameFlag_SetBit(0x206);
  Func_02001da8();
}
