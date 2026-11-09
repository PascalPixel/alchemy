#include "local.h"

void Func_02002834(s32 id);
void Func_020025a8(s32 id);
void BattleEffect_ApplyColorToTargetBuffer(s32 a, s32 b);
void BattleEffect_StartBufferInterpolation(s32 frames);
void Func_02002948(s32 layer);
void Func_02002940(s32 layer);
void Func_02000b10(struct Actor *object);
void Func_02000b58(struct Actor *object);

/// Toggles flag 0xa68 and with it which of two layer pairs is shown: four
/// rounds of a random colour value, then one pair's blend scripts and layer
/// fields set and the other's cleared, while the object blinks.
void Func_02000b68(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);
  s32 i;

  Func_02002998();
  Event_ResolvePendingActions(0);
  object->update = (void (*)(void))Func_02000b10;
  wait(30);
  if (GameFlag_Test(0xa68) == 0)
    Func_02002834(26);
  else
    Func_02002834(0);
  for (i = 3; i >= 0; i--) {
    BattleEffect_ApplyColorToTargetBuffer(Random16Far() & 0x7fff, 0);
    BattleEffect_StartBufferInterpolation(15);
    wait(10);
    BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
    BattleEffect_StartBufferInterpolation(10);
    wait(10);
  }
  BattleEffect_ApplyColorToTargetBuffer(0x7fff, 0);
  BattleEffect_StartBufferInterpolation(20);
  Event_Wait(20);
  if (GameFlag_Test_(0xa68) == 0) {
    GameFlag_SetBit(0xa68);
    Func_02002958_(64, 64, 1, 2, 51, 82);
    Func_02002958_(66, 64, 1, 2, 115, 18);
    Func_02002948(5);
    Func_02002948(6);
    Func_02002940(7);
    Func_02002940(8);
    object->update = (void (*)(void))Func_02000b58;
    Func_020025a8(26);
  } else {
    GameFlag_ClearBit(0xa68);
    Func_02002958_(65, 64, 1, 2, 51, 82);
    Func_02002958_(67, 64, 1, 2, 115, 18);
    Func_02002948(7);
    Func_02002948(8);
    Func_02002940(5);
    Func_02002940(6);
    object->update = (void (*)(void))Func_02000b58;
    Func_020025a8(0);
  }
  Event_Wait(30);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  BattleEffect_StartBufferInterpolation(30);
  Event_Wait(15);
  Func_020029a0();
}
