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

/// Poll the common movement target of a field object for up to 600 frames.
void Script_WaitForEventTimeout(void *fieldObject) {
  s32 waitedFrames;

  waitedFrames = 0;
  while (waitedFrames < 600 && Object_IsTargetUnset(fieldObject) == 0) {
    WaitFrames(1);
    waitedFrames++;
  }
}
