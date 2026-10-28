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
#include "edition.h"

// Text-workspace tail: the Japanese edition's table is 0x200 bytes shorter.
#if (LANGUAGE == LANGUAGE_JAPANESE)
#define TEXT_WORK_TAIL 0x1184
#else
#define TEXT_WORK_TAIL 0x1384
#endif

struct Work;

s32 Text_GetMessageSize(s32 no, s32 arg1, s32 arg2, s32 arg3,
                                 s32 arg4);
struct Work *Message_OpenWindow(s32 no, s32 x, s32 y, s32 flags);
s32 EventRuntime_GetControlledOwnerFar(void);
s32 Func_080c82a8(s32 object, s32 *out);
s32 Message_AllTyped(void);
void Window_Close(struct Work *work, s32 release);
s32 Window_IsClosed(struct Work *work);

void FieldMessage_Show(s32 no, s32 flags) {
  u8 *base = Ram_HeapSlots->windowTiles;
  s32 release;
  s32 pos[2] = {0, 0};
  s32 height;
  s32 width;
  s32 y;
  s32 x;
  struct Work *work = NULL;
  s32 zero;
  s32 margin;
  s32 rows;

  y = x = 0;
  release = flags & 1;

  if (flags & 2) {
    *(u8 *)(base + TEXT_WORK_TAIL + 5) = 1;
  }

  Text_GetMessageSize(no, (s32)&x, (s32)&y, (s32)&width, (s32)&height);

  margin = 30 - width;
  /* FAKEMATCH: the one-pass block ends the scheduling block: the native reads
   * the height before it halves the horizontal margin (one reordered row,
   * differ 2 without). */
  do {
    rows = height;
  } while (0);
  x = margin >> 1;
  y = (12 - rows) >> 1;

  if (flags & 8) {
    y = y + 4;
  } else if (flags & 0x40) {
    y = y + 12;
  } else {
    s32 dy;

    Func_080c82a8(EventRuntime_GetControlledOwnerFar(), pos);
    dy = pos[1] >> 3;
    if (dy > 9) {
      y = dy - 5;
    } else {
      y = dy + 4;
    }
  }

  work = Message_OpenWindow(no, x, y, release);

  if (work != NULL) {
    while (Message_AllTyped() == 0) {
      WaitFrames(1);
    }

    if (flags & 0x20) {
      Ram_HeapSlots->windowTiles[6] = 1;
    }

    if (!(flags & 4)) {
      Window_Close(work, release);
      while (Window_IsClosed(work) == 0) {
        WaitFrames(1);
      }
    }
  }

  zero = 0;
  *(u8 *)(base + TEXT_WORK_TAIL + 5) = zero;
  *(u16 *)(base + TEXT_WORK_TAIL) = zero;
  *(u16 *)(base + TEXT_WORK_TAIL + 2) = zero;
  WaitFrames(3);
}

void WindowSystem_Update(void) {
  Window_UpdateZooms();
  Message_UpdateAll();
  WindowSprite_UpdateAll();
}

void WindowSystem_UpdateBattle(void) {
  if (Ram_HeapSlots->unke4[0] != 0) {
    Window_UpdateZooms();
    Message_UpdateAll();
    WindowSprite_UpdateAll();
  } else {
    Message_UpdateAll();
  }
}
