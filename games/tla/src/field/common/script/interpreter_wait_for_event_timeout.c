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

void Script_WaitForEventTimeout(s32 arg0) {
  s32 cnt;

  cnt = 0;
  while (cnt <= 0x257 && Object_IsTargetUnset(arg0) == 0) {
    WaitFrames(1);
    cnt++;
  }
}
