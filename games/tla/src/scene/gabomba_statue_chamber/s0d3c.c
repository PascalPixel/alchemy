#include "types.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "system/io_reg.h"

void Func_02001da0(void);
void Func_02001da8(void);
void Func_02001ee8(s32 skip);
void Func_02001d60(void);
void Actor_SetSpeedParameters(u32 id, s32 speed, s32 acceleration);
void Actor_OffsetPositionAndResetMotion(u32 id, s32 dx, s32 dz);
void Actor_WaitForMoveById(u32 id);
void Func_02001e70(u32 id, s32 angle);
void Func_02001f10(s32 cue);
void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
struct Actor *Actor_GetById(u32 id);
void GameFlag_SetBit(s32 flag);

void Func_02000d3c(void) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  u8 *lower = map + 0x108;
  u8 *upper = map + 0x178;
  s32 i, step;

  Func_02001da0();
  Func_02001ee8(0);
  Func_02001d60();
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_OffsetPositionAndResetMotion(gMainRoutineWork.currentOwner, 0, -8);
  Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Func_02001e70(gMainRoutineWork.currentOwner, 0x4000);
  Func_02001f10(308);
  step = 1;
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
  for (i = 0; i <= 351; i++) {
    if ((u32)(i - 17) <= 62) {
      s32 blend = ((step + 1) / 4) << 8;

      blend |= 16 - (step + 1) / 4;
      REG_BLDALPHA = blend;
      step++;
    }
    *(s32 *)(lower + 12) += 0x10000;
    *(s32 *)(upper + 12) += 0x10000;
    wait(1);
  }
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  Func_02001f10(298);
  wait(1);
  Func_02001f10(125);
  FieldEvent_Wait(20);
  Actor_GetById(gMainRoutineWork.currentOwner)->terrainId = 1;
  GameFlag_SetBit(0x206);
  GameFlag_SetBit(0x207);
  Func_02001da8();
}
