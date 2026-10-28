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
#include "scene.h"
#include "system.h"

void Sound_WaitForFanfare(void) {
  while (Sound_GetBgmRestoreDelayFar() != 0) {
    WaitFrames(1);
  }
}
