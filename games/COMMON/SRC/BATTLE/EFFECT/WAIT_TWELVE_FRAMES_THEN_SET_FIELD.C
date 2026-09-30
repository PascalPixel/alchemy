#include "TYPES.H"

s32 WaitFrames(s32);
void BattleMotion_ApproachTargetFar(s32, s16, s32, s32);
s32 BattleFx_RunSparkGroups(void *, s32);

struct Object080eceac {
  u8 padding_00[8];
  s32 field_08;
  u8 padding_0c[0xc];
  s32 field_18;
  u8 padding_1c[8];
  s16 field_24;
};

void Object_WaitTwelveFramesThenSetField18(struct Object080eceac *obj)
{
  BattleMotion_ApproachTargetFar(obj->field_08, obj->field_24, 0x18, 0x73333);
  WaitFrames(0xC);
  obj->field_18 = 3;
  BattleFx_RunSparkGroups(obj, 2);
}
