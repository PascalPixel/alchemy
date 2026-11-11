#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "input.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "serial.h"
#include "text/msg_ids.h"
#include "event_inline.h"

s32 Func_02000f6c(s32 id);
s32 Func_02000fbc(s32 actor);
void Func_0200191c(void);
void Func_0200197c(void);
void Func_020019e4(s32 value, s32 digits);
void Func_020019ec(void);
void Func_02001a14(void);
void Func_02001a1c(void);
s32 Func_02001a84(s32 scene, s32 entrance);

s32 Func_020013ec(void) {
  Func_020019ec();
  FieldMessage_Show(MsgBattleResultsWere, 1);
}

s32 Func_02001400(void) {
  Func_020019ec();
  FieldMessage_Show(MsgMonsterBattleResults, 1);
}

/// Draws value, at most 999, as three digits from right to left.
void Func_02001414(s32 value) {
  s32 i;

  if (value > 999)
    value = 999;
  i = 0;
  // A goto loop; as ☀️'s for loop, loop.c keeps the stacked 1 in r7 (differ
  // 12).
next:
  Map_CopyCellPictureIndicesRect(27, value % 10, 16 - i, 8, 1, 1);
  value /= 10;
  i++;
  if (i <= 2)
    goto next;
  Func_0200197c();
}
