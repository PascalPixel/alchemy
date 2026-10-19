#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"

#define BattleLayout_HighlightPartyPanels                                      \
  ((void (*)(u16 *))BattleLayout_HighlightPartyPanels)
void Battle_DrawPartyPanelsWithEmptyList(void) {
  /* FAKEMATCH: stands in for the static-chain spill of a function nested in
   * Func_0804bc08, which top-level C omits; it goes when the parent is adopted
   * with both nested functions. */
  volatile u32 frame;
  u16 data[2];
  /* FAKEMATCH: capturing the incoming static chain preserves the r9 save before
   * either call. */
  register u32 chain asm("r9");

  frame = chain;
  data[0] = 255;
  BattleLayout_HighlightPartyPanels(data);
  BattlePres_SetActorModesFar(data, 1);
}
