#include "types.h"

u8 *Item_Get(s32 item);
s32 Func_08100d58(s32 kind);

s32 Func_08100d40(s32 item) {
  return Func_08100d58(*(u16 *)(Item_Get(item) + 40) & 0x3fff);
}

u8 *BattleAction_Get(s32 actionId);
s32 Sound_PlayReturnOne(s32 cue);

s32 Func_08100d58(s32 kind) {
  u8 *action = BattleAction_Get(kind);

  switch (action[1] & 15) {
  case 1:
    Sound_PlayReturnOne(126);
  case 11:
    Sound_PlayReturnOne(126);
    break;
  default:
    switch (action[3]) {
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
