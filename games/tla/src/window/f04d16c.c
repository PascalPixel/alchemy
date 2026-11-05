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

s32 Menu_RunResourceSelectionLoop(s32 initial) {
  struct ResourceMenuWork *work = Ram_HeapSlots->menuSelectWork;
  s32 resource;

  work->selection = initial;
redraw:
  WindowSprite_PrepareForRedraw(work->window);
  if (work->resourceBase != 0)
    resource = work->resourceBase + work->selection;
  else
    resource = work->resourceIds[work->selection] + (s32)MsgCommandName;
  Text_DrawCharacterAtOffset(resource, work->window, 0, 0);
  for (;;) {
    wait(1);
    if (gInput.pressed & KEY_A) {
      Sound_PlayFar(112);
      return work->selection;
    }
    if (gInput.pressed & KEY_B) {
      Sound_PlayFar(113);
      return -1;
    }
    if (gInput.pressed & KEY_START) {
      Sound_PlayFar(113);
      return -1;
    }
    if ((gInput.repeat & KEY_LEFT) || (gInput.repeat & KEY_UP)) {
      Sound_PlayFar(111);
      work->selection--;
      if (work->selection < 0)
        work->selection = work->count - 1;
    } else if ((gInput.repeat & KEY_RIGHT) || (gInput.repeat & KEY_DOWN)) {
      Sound_PlayFar(111);
      work->selection++;
      if (work->selection >= work->count)
        work->selection = 0;
    } else
      continue;
    goto redraw;
  }
}
