#include "types.h"
#include "battle_effect_argument.h"

void BattleEffect_StartCanvasLayer(s32 layer);
void Func_0815585c(struct BattleEffectArgument *effect, u32 packed, s32 *screen, s32 *world);
void BattleEffect_EndCanvasLayer(void);

/// Abilities: Kite (kind 217).
void BattleEffect_RunKite(struct BattleEffectArgument *effect) {
  s32 b[3];
  s32 a[3];

  BattleEffect_StartCanvasLayer(0);
  Func_0815585c(effect, ((effect->side ^ 1) << 4) | 7, b, a);
  BattleEffect_EndCanvasLayer();
}
