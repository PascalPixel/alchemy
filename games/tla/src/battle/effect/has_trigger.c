#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"

struct PendingEvent;

s32 BattleEffect_HasMatchingEvent5(s32 effectId) {
  u16 kind = effectId;
  s32 result = (s32)Func_080ce458(0xf0000005, kind, Func_080ce31c(kind));

  return (u32)((-result) | result) >> 31;
}

u32 BattleEffect_HasTrigger(u16 effectId) {
  s32 result;

  result = Event_FindFacingTrigger(effectId);
  return (u32)((0 - result) | result) >> 0x1F;
}
