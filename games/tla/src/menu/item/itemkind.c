#include "types.h"

u8 *Item_Get(s32 item);
s32 Ability_PlayUseAnimation(s32 action);

s32 Func_08100d40(s32 item) {
  return Ability_PlayUseAnimation(*(u16 *)(Item_Get(item) + 40) & 0x3fff);
}

u8 *BattleAction_Get(s32 actionId);
s32 Sound_PlayReturnOne(s32 cue);

/// Plays the sound of action used from the menu, and returns at once: no
/// animation is drawn and nothing waits. Formulas 1 and 11 play sound 126,
/// formula 1 twice; otherwise effects 1, 2, 31 and 32 play nothing, effect 5
/// sound 82, effect 3 sound 84, and every other effect sound 91.
s32 Ability_PlayUseAnimation(s32 action) {
  u8 *definition = BattleAction_Get(action);

  switch (definition[1] & 15) {
  case 1:
    Sound_PlayReturnOne(126);
  case 11:
    Sound_PlayReturnOne(126);
    break;
  default:
    switch (definition[3]) {
    case 1:
    case 2:
    case 31:
    case 32:
      break;
    case 5:
      Sound_PlayReturnOne(82);
      break;
    case 3:
      Sound_PlayReturnOne(84);
      break;
    default:
      Sound_PlayReturnOne(91);
      break;
    }
    break;
  }
}
