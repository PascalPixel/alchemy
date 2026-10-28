#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "sequence_parent_work.h"

struct JupiterLighthouseRoom8EventWork {
  u8 unk000[0x1ac];
  s32 screenEffect;
};

struct Actor *Actor_GetById(s32 id);
void Actor_SetSpritePriority(u32 id, s32 variant);
void Func_02002248(s32 id);
void Func_02000af0(s32 id);
void Func_020018f4(s32 a, s32 b, s32 c, s32 d, s32 e);
void Func_02002010(s32 x, s32 z, s32 a);

s32 Func_02000110(void) {
  ((struct JupiterLighthouseRoom8EventWork *)Ram_HeapSlots->eventWork)->screenEffect =
      0x204;
  Actor_GetById(8)->scaleX = 0xc000;
  Actor_GetById(8)->scaleY = 0xc000;
  Actor_SetSpritePriority(12, 3);
  Actor_SetSpritePriority(14, 3);
  Func_02002248(11);
  Func_02002248(12);
  Func_02002248(13);
  Func_02002248(14);
  Func_02002248(15);
  Func_02000af0(11);
  Func_02000af0(12);
  Func_02000af0(13);
  Func_02000af0(14);
  Func_02000af0(15);
  Func_020018f4(33, 0x280, 0x380, 0, 1);
  if (gSequenceParentWork.entrance == 13)
    Func_02002010(19, 0, 0x204);
  else if (gSequenceParentWork.entrance == 14)
    Func_02002010(-20, -22, 0x204);
  return 0;
}
