/// @file
/// Windows: message-channel cleanup and the table of message substitutions.

#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "window.h"
#include "edition.h"
#include "ram_buffer.h"

/// Clear the eight-entry table of values substituted into messages.
void UiWork_ClearValueNameTables(void) {
  s32 no;
  struct UiRenderWork *work;

  work = (struct UiRenderWork *)Ram_HeapSlots->windowTiles;
  /* FAKEMATCH: the barrier keeps the work-pointer load before the table
   * offsets. */
  __asm__("");
  no = 0;

  do {
    work->values[no] = 0;
    work->names[no] = 0;
    no++;
  } while (no != 8);
}

/// Insert value of type kind in a free entry.
/// Do nothing if the table is full.
void UiWork_PushValueSlot(u32 value, u32 kind) {
  struct UiRenderWork *work = (struct UiRenderWork *)Ram_HeapSlots->windowTiles;
  u32 no = 0;
  u32 limit = 8;

  do {
    if (work->names[no] == 0) {
      work->values[no] = value;
      work->names[no] = kind;
      break;
    }
    no++;
  } while (no != limit);
}
