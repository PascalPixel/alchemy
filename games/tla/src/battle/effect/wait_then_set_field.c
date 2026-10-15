//--------------------------------------------------------------------------
// wait_then_set_field.c
// バトル演出：対象へ近づいてからのスパーク（型 4）
//--------------------------------------------------------------------------

#include "types.h"
#include "scene.h"
#include "system.h"
#include "battle_effect_work.h"

void BattleMotion_ApproachTargetFar(s32, s16, s32, s32);
s32 BattleFx_RunSparkGroups(void *, s32);

//--------------------------------------------------------------------------
// 行動者を最初の対象へ跳んで近づかせ、29 フレーム後に型 4 のスパークを出す
// effect : 演出の引数
//--------------------------------------------------------------------------
void BattleFx_RunSparkApproachVariant4(struct BattleEffectArgument *effect) {
  // 距離の 3/4 まで、移動量 / 24 の速さ、縦の初速 12.2 でとぶ
  BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 24, 0xc3333);
  WaitFrames(29);
  effect->variant = 4;
  BattleFx_RunSparkGroups(effect, 2); // 2 : 行動者の位置から
}
