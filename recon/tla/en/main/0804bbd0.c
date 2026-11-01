// Draft, differ 15 of 24, skeleton gap 15 (tla-en, 2026-11-01)
// Twin of The Broken Seal's Link_DrawWaitingForOpponent: the link wait's
// message, a nested function of Func_0804bc08 (drafted in 0804bc08.c) that
// copies message 0xc59, "Waiting for opponent's input.", up to 52 characters
// and draws it at (0, 4) of the work block's window (offset 0x44). All 24
// listed instructions compile exactly; the 15 left are the stand-in parent's
// own, draft-only, and go when this adopts with its parent. Taken from the
// twin: the nested function in an inner block after the parent's statements.
// The parent's array at chain - 4 (sp + 104 in the listing) has its address
// taken before the nested function compiles, so its other local lands at
// chain - 8 as listed. Declared in the outer scope the local stays at -4.
// Tried: an unused array before it (stays at -4); a static parent (same
// count); an extern inline parent (drops the nested function too).
#include "types.h"
#include "uiwindow.h"

s32 Message_CopyExpanded(s32 key, u16 *destination, u32 capacity);
void Text_DrawWideStringAtOffset(u16 *text, struct Window *window, s32 x,
                                     s32 y);

extern u8 Msg_0c59[];

struct Func_0804bc08Work {
  u8 unk00[0x44];
  struct Window *window;
};

// Stand-in for the parent, kept as small as the frame allows. Its assignment
// only takes the array's address; the real parent's local is the work block
// from HeapSlot_AllocEwram(228, 272).
void Func_0804bc08(void) {
  u16 signatureHeader[2];
  struct Func_0804bc08Work *work;

  work = (struct Func_0804bc08Work *)signatureHeader;
  {
    void Func_0804bbd0(void) {
      u16 text[64];

      Message_CopyExpanded((s32)Msg_0c59, text, 52);
      Text_DrawWideStringAtOffset(text, work->window, 0, 4);
    }

    Func_0804bbd0();
  }
}
