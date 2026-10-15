#include "types.h"

s32 WaitFrames(s32);
void BattleMotion_ApproachTargetFar(s32, s16, s32, s32);
s32 BattleFx_RunSparkGroups(void *, s32);

struct Object080eceac {
  u8 padding00[8];
  s32 field08;
  u8 padding0c[0xc];
  s32 field18;
  u8 padding1c[8];
  s16 field24;
};

void Object_WaitTwelveFramesThenSetField18(struct Object080eceac *obj)
{
  BattleMotion_ApproachTargetFar(obj->field08, obj->field24, 0x18, 0x73333);
  WaitFrames(0xC);
  obj->field18 = 3;
  BattleFx_RunSparkGroups(obj, 2);
}
