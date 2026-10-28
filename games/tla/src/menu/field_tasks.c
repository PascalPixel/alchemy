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
#include "task.h"

s32 GameFlag_ClearBit(s32 flag_no);
s32 GameFlag_SetBit(s32 flag_no);

s32 Menu_ResumeFieldTasks(void) {
  Task_ResumeOverlayTasks();
  Map_DisableUpdateCallbackFar();
  GameFlag_ClearBit(0x166);
  return GameFlag_ClearBit(0x152);
}

s32 Menu_SuspendFieldTasks(void) {
  GameFlag_SetBit(0x152);
  GameFlag_SetBit(0x166);
  Map_EnableUpdateCallbackFar();
  return Task_SuspendOverlayTasks();
}
