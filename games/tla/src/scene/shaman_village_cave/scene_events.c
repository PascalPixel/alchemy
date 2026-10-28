#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "adventure.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "call.h"
#include "text/msg_ids.h"

void Func_02001080();
void Func_02001088();
void Func_02001098();
void Func_020010f0();
void Func_020011e0();
void Func_02001200();
void GameFlag_ClearBit();
void GameFlag_SetBit();

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

void Func_0200039c(void) {
  s32 result0;

  ((void (*)())Func_02001080)();
  Call1((void (*)())Func_020011e0, 0);
  result0 = Value3((s32 (*)())Func_02001098, 22, 0x190, -1);
  if (result0 != 0)
    goto L_020083c4;
  Call1((void (*)())GameFlag_ClearBit, 0xa73);
L_020083c4:
  ((void (*)())Func_02001088)();
  return;
}

void Func_020003cc(void) {
  s32 angle;

  angle = 0x5000;
  Call4((void (*)())Func_020010f0, 19, 0xe70000, 0xc70000, angle);
  Call4((void (*)())Func_020010f0, 20, 0xdc0000, 0xbf0000, angle);
  Call4((void (*)())Func_020010f0, 21, 0xcc0000, 0xbf0000, angle);
  return;
}

void Func_02000404(void) {
  s32 angle;

  Call2((void (*)())Func_02001200, 22, 0x190);
  angle = 0xc000;
  Call4((void (*)())Func_020010f0, 22, 0xd40000, 0xc70000, angle);
  Call1((void (*)())GameFlag_SetBit, 0xa73);
  return;
}
