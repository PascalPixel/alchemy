// Under -mthumb-slow-loads (2026-10-26): differ 4 of 133 in tla-en, instruction
// order only. No empty asm before any one statement lowers it.
// Near miss, differ 6 of 133 (three reordered pairs), 2026-10-15: The
// Broken Seal's Menu_RunResourceSelectionLoop on this game's menu work, cues
// and gInput, the keys read through a volatile view of gInput, as the native
// reloads the pressed keys at each test (plain: differ 42). One empty asm
// barrier after the first store (found by a device search, differ 8
// without). Remaining: the native starts resourceBase's address between the
// selection's address copy and its store; it loads gInput's address before
// it adds the count's offset; after the selection's increment it shifts
// before it loads the count. Moving the barrier or wrapping the stores in a
// one-pass block did not help.

#include "types.h"
#include "system.h"
#include "resource.h"
#include "resource_menu.h"
#include "ram_buffer.h"
#include "input.h"
#include "io_reg.h"
#include "text/msg_ids.h"

void Sound_PlayFar(u32);
void WindowSprite_PrepareForRedraw(struct Window *);
void Text_DrawCharacterAtOffset(s32, struct Window *, s32, s32);

#define INPUT ((volatile struct InputState *)&gInput)

s32 Menu_RunResourceSelectionLoop(s32 initial) {
  struct ResourceMenuWork *work = Ram_HeapSlots->menuSelectWork;
  s32 resource;

  work->selection = initial;
  asm("");
redraw:
  WindowSprite_PrepareForRedraw(work->window);
  if (work->resourceBase != 0)
    resource = work->resourceBase + work->selection;
  else
    resource = work->resourceIds[work->selection] + (s32)MsgCommandName;
  Text_DrawCharacterAtOffset(resource, work->window, 0, 0);
  for (;;) {
    wait(1);
    if (INPUT->pressed & KEY_A) {
      Sound_PlayFar(112);
      return work->selection;
    }
    if (INPUT->pressed & KEY_B) {
      Sound_PlayFar(113);
      return -1;
    }
    if (INPUT->pressed & KEY_START) {
      Sound_PlayFar(113);
      return -1;
    }
    if ((INPUT->repeat & KEY_LEFT) || (INPUT->repeat & KEY_UP)) {
      Sound_PlayFar(111);
      work->selection--;
      if (work->selection < 0)
        work->selection = work->count - 1;
    } else if ((INPUT->repeat & KEY_RIGHT) || (INPUT->repeat & KEY_DOWN)) {
      Sound_PlayFar(111);
      work->selection++;
      if (work->selection >= work->count)
        work->selection = 0;
    } else
      continue;
    goto redraw;
  }
}
