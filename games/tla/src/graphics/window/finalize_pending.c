#include "types.h"
#include "system.h"
#include "window.h"
#include "ram_buffer.h"
#include "input.h"
#include "io_reg.h"

s32 Audio_Check(void);

/// Ends a message wait (clearing the channel's countdown and returning 1) when
/// the window's voice has finished or A, B or a shoulder button is held.
s32 UiWork_CheckCancelByInput(struct UiChannelSlot *ch) {
  struct UiRenderWork *work = (struct UiRenderWork *)Ram_HeapSlots->windowTiles;
  s32 cancel = 0;
  /* FAKEMATCH: the native keeps the address of the held keys and reads them
   * twice; a plain read is shared. */
  volatile u32 *held;

  /* FAKEMATCH: the empty asm holds r2 until the render work is loaded, then
   * starts the busy flag's offset 0x1389; as plain C, 2 instructions differ in
   * tla-en. */
  asm("" : "+l"(work) : : "r2");
  if (work->busy != 0 && Audio_Check() == 0)
    cancel = 1;
  held = &gInput.held;
  if (*held & (KEY_A | KEY_B | KEYS_SHOULDERS))
    cancel = 1;
  if (*held & KEY_B)
    cancel = 1;
  if (cancel) {
    ch->countdown = 0;
    return 1;
  }
  return 0;
}

/// As UiWork_CheckCancelByInput, but on newly pressed A, B or shoulder buttons
/// (the pending presses in a window mode); while B is held the wait ends only
/// at countdown 1, and otherwise the countdown is set to 2.
s32 UiWork_CheckCancelByModeInput(struct UiChannelSlot *ch) {
  /* FAKEMATCH: the work is pinned to the r6 the native keeps it in; with the
   * barrier below and no pin it takes r5 and the channel r6 (differ 14). */
  register struct UiRenderWork *work asm("r6") =
      (struct UiRenderWork *)Ram_HeapSlots->windowTiles;
  s32 cancel = 0;
  u32 keys;

  /* FAKEMATCH: ⚓️ loads the render work before it starts the busy flag's offset
   * 0x1389 in r2; the empty asm holds r2 until the work is loaded. Plainly the
   * offset is started first (differ 2). */
  asm("" : "+l"(work) : : "r2");
  if (work->busy != 0 && Audio_Check() == 0)
    cancel = 1;
  keys = gInput.pressed;
  if (work->mode != 0)
    keys = gInput.pendingPressed;
  if (keys & (KEY_A | KEY_B | KEYS_SHOULDERS))
    cancel = 1;
  if (gInput.held & KEY_B) {
    if (ch->countdown == 1)
      cancel = 1;
  } else {
    ch->countdown = 2;
  }
  if (cancel) {
    ch->countdown = 0;
    return 1;
  }
  return 0;
}

struct UiWindowState {
  u8 unk00[0x14];
  u16 state;
  u16 flags;
  s32 pendingValue;
};

void UiWork_FinalizePendingCore(void) {
  s32 slotIndex;
  struct UiChannelSlot *slot;
  struct UiWindowState *work;

  slot = ((struct UiRenderWork *)Ram_HeapSlots->windowTiles)->channels;
  slotIndex = 0;
  do {
    work = (struct UiWindowState *)slot->work;
    if (work != NULL && work->pendingValue == 0 && work->flags != 0 &&
        work->state != 0) {
      UiWork_Finalize((struct UiWindow *)work, work->flags & 2);
    }
    slotIndex++;
    slot++;
  } while (slotIndex != 3);
  WaitFrames(10);
}
