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
#include "motobj.h"

void BattleFx_SpinObject(struct MotionObject *object) {
  object->angle += 0x1000;
}

void BattleFx_CallSpinEffect(void) { Func_080e3074(); }
