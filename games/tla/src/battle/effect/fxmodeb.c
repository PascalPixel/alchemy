#include "types.h"

void Func_0813cb34(void *object, s32 mode);

// Abilities: Sleep Star (kind 349).
void BattleFx_RunSleepStar(void *object) { Func_0813cb34(object, 1); }

// Abilities: Soothing Star (kind 344).
void BattleFx_RunSoothingStar(void *object) { Func_0813cb34(object, 0); }

// Abilities: Slaver (kind 395).
void BattleFx_RunSlaver(void *object) { Func_0813cb34(object, 2); }
