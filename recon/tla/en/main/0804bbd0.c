// Draft, differ 19 of 24 (psynergy diff ... 0804bbd0; the stand-in parent's
// 17 instructions count as draft-only): a nested function of Func_0804bc08
// (2,200 lines, not drafted), which saves its static chain in its frame and
// reads the parent's work block through it. It copies message 0xc59 (up to
// 52 characters) and draws it at (0, 4) of the work block's window
// (offset 0x44). Left inside the nested function: only the parent slot's
// offset from the chain, -8 in the native and -4 here, which belongs to the
// stand-in parent's frame, not to this body; it adopts with its parent.
// Tried: a second address-taken local in the stand-in (stays at -4).
#include "types.h"
#include "uiwindow.h"

s32 UiText_CopyMessageString(s32 key, u16 *destination, u32 capacity);
void UiText_RenderWideStringAtOffset(u16 *text, struct UiWindow *window, s32 x,
                                     s32 y);
void *Runtime_AllocateBlock(s32 slot, s32 size);

extern u8 Msg_0c59[];

struct Func_0804bc08Work {
  u8 unk00[0x44];
  struct UiWindow *window;
};

// Stand-in for the parent, Func_0804bc08, whose work block (from
// Runtime_AllocateBlock(228, 272)) the nested function reads.
void Func_0804bc08(void) {
  struct Func_0804bc08Work *work;

  void Func_0804bbd0(void) {
    u16 text[64];

    UiText_CopyMessageString((s32)Msg_0c59, text, 52);
    UiText_RenderWideStringAtOffset(text, work->window, 0, 4);
  }

  work = Runtime_AllocateBlock(228, 272);
  Func_0804bbd0();
}
