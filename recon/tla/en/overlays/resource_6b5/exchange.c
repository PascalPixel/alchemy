// Near miss, differ 25 of 26 (2026-10-12), from ☀️
// SceneData_CopyUpToThreeEntries: the listing's loop is not strength reduced
// (it rebuilds 0x218 inside the loop and reads base + (i + 0x218)); see talk.c
// beside this file.
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "io_reg.h"
#include "main_routine_work.h"
#include "serial_runtime.h"

s32 Func_0200039c(void);
s32 Func_02000578(void);

s32 Func_02000538(u16 *dest) {
  s32 cnt = Party_CountActiveOwners();
  s32 i;

  if (cnt > 3)
    cnt = 3;
  for (i = 0; i < cnt; i++) {
    u8 c = gMainRoutineWork.activeOwners[i];

    if (dest != 0) {
      *dest = c;
      dest++;
    }
  }
  if (dest != 0)
    *dest = 0x00ff;
  return cnt;
}
