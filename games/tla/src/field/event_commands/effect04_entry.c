#include "types.h"

struct BattleEffectArgument;

void Func_080de21c(struct BattleEffectArgument *effect);

/// Runs field Psynergy animation 4; the target is unused.
void FieldEffect_RunForceOnTarget(struct BattleEffectArgument *effect) {
  Func_080de21c(effect);
}
