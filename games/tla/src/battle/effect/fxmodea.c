#include "types.h"

void Func_0813c5f4(void *object, s32 mode);

// Abilities: Sonic Wave (kind 300).
void BattleFx_RunSonicWave(void *object) { Func_0813c5f4(object, 0); }

// Abilities: Shriek, Harpy (kind 301).
void BattleFx_RunShriek(void *object) { Func_0813c5f4(object, 1); }

// Abilities: Banshee Howl (kind 302).
void BattleFx_RunBansheeHowl(void *object) { Func_0813c5f4(object, 2); }

// Abilities: War Cry (kind 304).
void BattleFx_RunWarCry(void *object) { Func_0813c5f4(object, 4); }

// Abilities: Wicked Howl (kind 305).
void BattleFx_RunWickedHowl(void *object) { Func_0813c5f4(object, 3); }

// The streak volley in lime (mode 1). UNUSED: no effect table entry
// points here.
void BattleFx_RunLimeStreaksAgain(void *object) { Func_0813c5f4(object, 1); }
