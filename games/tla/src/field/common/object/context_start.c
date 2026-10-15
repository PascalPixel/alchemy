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

void GameFlag_SetBit(s32 flag);

void ObjectEffect_BeginContextEffect26(void) {
  ObjectEffect_PrepareContextEffect(0x1A);
  GameFlag_SetBit(0x120);
}

void ObjectEffect_BeginContextEffect25(void) {
  ObjectEffect_PrepareContextEffect(0x19);
  GameFlag_SetBit(0x121);
}
