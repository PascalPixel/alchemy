#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"

u8 GameFlag_GetByte(s32 flag) {
  flag = ((u32)flag << 20) >> 23;
  return gGameFlagBytes[flag];
}
