#include "types.h"

void Func_080e1f2c(void);

/// Effect 29 cast on a target: runs the effect. Nothing calls it; the
/// targeted dispatch has no case for animation 29.
void BattleEffect_CallEffect29(void) { Func_080e1f2c(); }
