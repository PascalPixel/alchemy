#include "types.h"

void Func_0813d098(void *object, s32 mode);

// Abilities: Numbing Sting, Paralytail (kind 313).
void BattleFx_RunNumbingSting(void *object) { Func_0813d098(object, 6); }

// Abilities: Cannibal Fang (kind 316).
void BattleFx_RunCannibalFang(void *object) { Func_0813d098(object, 3); }

// Abilities: Poisonous Bite (kind 318); Poison Fang (kind 323).
void BattleFx_RunPoisonousBite(void *object) { Func_0813d098(object, 1); }

// Abilities: Rabid Fang (kind 336).
void BattleFx_RunRabidFang(void *object) { Func_0813d098(object, 10); }

// Abilities: Electric Bite (kind 324).
void BattleFx_RunElectricBite(void *object) { Func_0813d098(object, 5); }

// Abilities: Poison Tail (kind 325); Poison Sting (kind 440).
void BattleFx_RunPoisonTail(void *object) { Func_0813d098(object, 9); }

// Abilities: Vampiric Fang (kind 327).
void BattleFx_RunVampiricFang(void *object) { Func_0813d098(object, 4); }

// Abilities: Double Fang (kind 332).
void BattleFx_RunDoubleFang(void *object) { Func_0813d098(object, 0); }

// Abilities: Mortal Blow (kind 333).
void BattleFx_RunMortalBlow(void *object) { Func_0813d098(object, 8); }

// Abilities: Twin Beaks, Gryphon (kind 335).
void BattleFx_RunTwinBeaks(void *object) { Func_0813d098(object, 7); }

// Abilities: Acid Bite (kind 337).
void BattleFx_RunAcidBite(void *object) { Func_0813d098(object, 2); }

// Abilities: Drain Fang (kind 386).
void BattleFx_RunDrainFang(void *object) { Func_0813d098(object, 11); }

// Abilities: Angle Spike (kind 399).
void BattleFx_RunAngleSpike(void *object) { Func_0813d098(object, 16); }

// Abilities: Heat Stun (kind 458).
void BattleFx_RunHeatStun(void *object) { Func_0813d098(object, 14); }

// Abilities: Maneater (kind 421).
void BattleFx_RunManeater(void *object) { Func_0813d098(object, 15); }

void Func_0813d034(void *object) { Func_0813d098(object, 17); }

void Func_0813d040(void *object) { Func_0813d098(object, 18); }

// Abilities: Kill Sting (kind 441).
void BattleFx_RunKillSting(void *object) { Func_0813d098(object, 19); }

void Func_0813d058(void *object) { Func_0813d098(object, 20); }

void Func_08157530(s32 x, s32 y, s32 width, s32 height);
void WaitFrames(s32 frames);

// Abilities: Mad Zephyr (kind 117).
void BattleFx_RunMadZephyr(u8 *object) {
  Func_08157530(*(s32 *)(object + 8), *(s16 *)(object + 36), 4, 20);
  WaitFrames(2);
  if (*(s32 *)(object + 24) == 0)
    Func_0813d098(object, 12);
  else
    Func_0813d098(object, 13);
}
