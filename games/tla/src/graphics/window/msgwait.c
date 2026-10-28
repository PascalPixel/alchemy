#include "types.h"
#include "window.h"
#include "ram_buffer.h"
#include "uiflow.h"

/// The message window's state in heap slot unk84[4].
struct UiMessageSlot {
  struct Window *window;
  s32 result; ///< what the last BattleMessage_Continue returned
  s32 cursor; ///< where the next message starts
};

s32 Message_Decode(s32 message, s32 mode);
struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height,
                                 s32 style);
void Func_0803a2b0(s32 x, s32 y, s32 width, s32 height, s32 mode);
s32 BattleMessage_Continue(struct Window *window, s32 entry, s32 cursor);

/// Decodes message and starts it in the message window along the bottom of
/// the screen (tiles 0, 15, 30 by 6), creating the window first if it is not
/// open; a message that decodes to nothing shows nothing.
void Message_Show(s32 message) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  struct UiMessageSlot *slot = Ram_HeapSlots->unk84[4];
  struct Window *window;
  /* FAKEMATCH(pin): the entry held in r8, copied from r0 only after the
   * result is cleared; as a plain local, 2 of 69 instructions differ in
   * tla-en. */
  register s32 entry asm("r8");
  s32 result;

  // The menu state reads 2 while the message decodes.
  work->menuState = 2;
  {
    s32 decoded = Message_Decode(message, 1);
    u16 present;

    work->menuState = 1;
    /* FAKEMATCH(barrier): an empty asm, so the menu state is stored before
     * the entry is addressed; as plain statements, 4 of 69 instructions
     * differ in tla-en. */
    __asm__ volatile("");
    present = work->entries[decoded];
    result = 0;
    entry = decoded;
    if (present == 0)
      return;
  }
  if (slot->window == NULL) {
    window = Window_Create(0, 15, 30, 6, 10);
    slot->window = window;
    Func_0803a2b0(0, 15, 30, 6, 1);
    slot->cursor = result;
  } else {
    window = slot->window;
  }
  if (window != NULL) {
    result = BattleMessage_Continue(window, entry, slot->cursor);
    slot->result = result;
    slot->cursor = 0;
    if (result == 0)
      Window_Close(window, 1);
  }
}

void WaitFrames(s32 frames);
s32 Message_AllTyped(void);
s32 Message_Start(struct Window *window, s32 entry, s32 x, s32 y,
                              u16 *bounds, s32 mode);

void Message_ShowAndWaitCore(s32 message) {
  while (Message_AllTyped() == 0)
    WaitFrames(1);
  Message_Show(message);
  while (Message_AllTyped() == 0)
    WaitFrames(1);
}

/// Builds the render entries of a message and queues them in the window;
/// returns 0 when the message is empty or there is no window.
s32 Message_OpenDecoded(s32 window, s32 argument) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  s32 entry;
  s32 result = 0;
  /* FAKEMATCH: never read: the native stack frame keeps 8 bytes above the two
   * outgoing arguments, exactly these four slots; without them the stack frame is 8
   * bytes, not 16 (differ 4). */
  u16 entries[4];

  /* FAKEMATCH: the empty asm holds r2 until the render work is loaded, as
   * ⚓️ does before it starts the result offset 0x1384; plainly, 2
   * instructions differ in tla-en. */
  asm("" : "+l"(work) : : "r2");
  work->speakerVoiceSound = 0;
  work->speakerVoiceCountdown = 0;
  entry = Message_Decode(argument, 1);
  if (work->entries[entry] == 0)
    return 0;
  if (window == 0)
    return 0;
  result = (s32)Message_Start((struct Window *)window, entry, 0,
                                          0, 0, 1);
  if (result == 0)
    return 0;
  return result;
}
