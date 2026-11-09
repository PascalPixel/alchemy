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
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "call.h"
#include "text/msg_ids.h"
#include "event_inline.h"

void Func_02001080();
void Func_02001088();
void Func_02001098();

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

void Func_0200039c(void) {
  s32 result0;

  Func_02001080();
  FieldEvent_ResolvePendingActions(0);
  result0 = Value3((s32 (*)())Func_02001098, 22, 0x190, -1);
  if (result0 != 0)
    goto L_020083c4;
  Event_ClearFlag(0xa73);
L_020083c4:
  Func_02001088();
  return;
}

void Func_020003cc(void) {
  s32 angle;

  angle = 0x5000;
  Actor_SetPositionAndFacing(19, PIXELS(231), PIXELS(199), angle);
  Actor_SetPositionAndFacing(20, PIXELS(220), PIXELS(191), angle);
  Actor_SetPositionAndFacing(21, PIXELS(204), PIXELS(191), angle);
  return;
}

void Func_02000404(void) {
  s32 angle;

  Actor_ShowItemIconById(22, 0x190);
  angle = 0xc000;
  Actor_SetPositionAndFacing(22, PIXELS(212), PIXELS(199), angle);
  Event_SetFlag(0xa73);
  return;
}
