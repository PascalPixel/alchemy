#include "types.h"

s32 wait(s32);
void BattleMotion_ApproachTargetFar(s32, s16, s32, s32);
s32 BattleEffect_RunSparkGroups(void *, s32);

struct Object080eceac {
  u8 padding00[8];
  s32 field08;
  u8 padding0c[0xc];
  s32 field18;
  u8 padding1c[8];
  s16 field24;
};

void Actor_WaitTwelveFramesThenSetField18(struct Object080eceac *obj) {
  BattleMotion_ApproachTargetFar(obj->field08, obj->field24, 0x18, 0x73333);
  wait(0xC);
  obj->field18 = 3;
  BattleEffect_RunSparkGroups(obj, 2);
}
