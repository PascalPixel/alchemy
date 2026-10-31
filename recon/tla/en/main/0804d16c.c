// Near miss, differ 4 of 133 (tla-en, 2026-10-31): two pairs sched2 orders by
// priority, which no C form tried moves. After the first store the native
// starts resourceBase's address (movs r3, #146) before the store: in the
// sched2 dump that address chain (priority 3) outranks the store's address
// copy (priority 2), and only a dependence out of the store would let them
// tie as the native's order needs. After the selection's increment it shifts
// before it loads the count: the ldrsh (priority 4, its load cost) beats the
// shift (priority 3) once both are ready. A 4-minute permuter run on the
// barrier-free form found nothing under differ 6; nested for (;;) loops in
// place of the goto: differ 161; a pointer local for the selection: 85;
// count <= selection: 8.
// Under -mthumb-slow-loads (2026-10-26): differ 4 of 133 in tla-en, instruction
// order only. No empty asm before any one statement lowers it.
// 2026-10-15: The Broken Seal's IconMenu_Run on this game's menu work, cues
// and gInput, whose pressed and repeat keys input.h declares volatile, as the
// native reloads them at each test. One empty asm barrier after the first
// store (found by a device search; differ 6 without it, 2026-10-31).
// Remaining then: the native starts resourceBase's address between the
// selection's address copy and its store; after the selection's increment it
// shifts before it loads the count. Moving the barrier or wrapping the stores
// in a one-pass block did not help.

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
  /* FAKEMATCH(barrier): the empty asm after the first store; as plain C with
   * no asm, 6 of 133 instructions differ in tla-en. */
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
