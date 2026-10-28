#include "types.h"
#include "battle_effect_argument.h"

void BattleEffect_StartCanvasLayer(s32 layer);
void Func_0815585c(struct BattleEffectArgument *effect, s32 flags, s32 *b,
                   s32 *a);
void BattleEffect_EndCanvasLayer(void);

/// Abilities: Kite (kind 217).
void BattleEffect_RunKite(struct BattleEffectArgument *effect) {
  s32 b[3];
  s32 a[3];

  BattleEffect_StartCanvasLayer(0);
  Func_0815585c(effect, ((effect->side ^ 1) << 4) | 7, b, a);
  BattleEffect_EndCanvasLayer();
}
