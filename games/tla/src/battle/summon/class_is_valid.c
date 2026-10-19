
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
#include "ram_buffer.h"
#include "motobj.h"

s32 BattleMotion_GetSlotPalette(s32 id) { return BattleObjGet(id)->palette; }

struct Layout {
  u8 pad[4];
  s16 field[6];
};

s32 EnemyCheck(s32 arg0) {
  s32 retval;
  s32 i;
  struct Layout *ptr;

  retval = BattleClass_IsSmall(arg0);
  ptr = (struct Layout *)Ram_HeapSlots->battleWork;
  i = 0;
  for (; i < 6; i++) {
    if (ptr->field[i] != 0)
      continue;
    if (retval != 0)
      break;
    if (i <= 4 && ptr->field[i + 1] == 0)
      break;
  }
  return i != 6;
}
