#include "types.h"

s32 WaitTicks(s32);
void BattleMotion_ApproachTargetFar(s32, s16, s32, s32);
s32 BattleEffect_RunSparkGroups(void *, s32);

struct SparkObject {
  u8 padding00[8];
  s32 target;
  u8 padding0c[0xc];
  s32 phase;
  u8 padding1c[8];
  s16 angle;
};

void BattleEffect_LongApproachThenSpark(struct SparkObject *obj) {
  BattleMotion_ApproachTargetFar(obj->target, obj->angle, 0x18, 0xc3333);
  WaitTicks(26);
  obj->phase = 5;
  BattleEffect_RunSparkGroups(obj, 2);
}
