#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "input.h"

u16 BattleEffect_GetCycledTableWord(void) {
  struct InputState *input = &gInput;
  u16 *words = gBattleEffect_CyclePatternWords;

  /* FAKEMATCH: ⚓️ loads the table's address before reading the buttons */
  asm volatile("" : "+l"(words));
  return *(u16 *)((u8 *)words + (((input->held >> 4) & 15) << 1));
}

u16 BattleEffect_GetEightWayHeadingWord(void) {
  struct InputState *input = &gInput;
  u16 *words = gBattleEffect_EightWayHeadingWords;

  /* FAKEMATCH: ⚓️ loads the table's address before reading the buttons */
  asm volatile("" : "+l"(words));
  return *(u16 *)((u8 *)words + (((input->held >> 4) & 15) << 1));
}
