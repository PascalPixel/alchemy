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

void SummonEffect_RunCanvasMode0(void *action) {
  SummonEffect_RunCanvasEffect(action, 0);
}

void SummonEffect_RunCanvasMode3(void *action) {
  SummonEffect_RunCanvasEffect(action, 3);
}

void SummonEffect_RunCanvasMode1(void *action) {
  SummonEffect_RunCanvasEffect(action, 1);
}

void SummonEffect_RunCanvasMode2(void *action) {
  SummonEffect_RunCanvasEffect(action, 2);
}
