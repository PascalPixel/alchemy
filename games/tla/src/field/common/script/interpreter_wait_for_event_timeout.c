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

/// Waits up to 600 frames for a field object's movement target to be unset.
void Script_WaitForEventTimeout(void *fieldObject) {
  s32 waitedFrames;

  waitedFrames = 0;
  while (waitedFrames < 600 && Object_IsTargetUnset(fieldObject) == 0) {
    WaitTicks(1);
    waitedFrames++;
  }
}
