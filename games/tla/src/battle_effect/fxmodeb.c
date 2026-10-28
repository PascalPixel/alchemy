#include "types.h"

void Func_0813cb34(void *object, s32 mode);

/// Abilities: Sleep Star (kind 349).
void BattleEffect_RunSleepStar(void *object) { Func_0813cb34(object, 1); }

/// Abilities: Soothing Star (kind 344).
void BattleEffect_RunSoothingStar(void *object) { Func_0813cb34(object, 0); }

/// Abilities: Slaver (kind 395).
void BattleEffect_RunSlaver(void *object) { Func_0813cb34(object, 2); }
