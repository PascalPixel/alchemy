#include "types.h"

struct EpilogueWindow {
  u8 unk00[8];
  u16 unk08;
  u16 unk0a;
  u8 unk0c[2];
  u16 unk0e;
};

extern s32 gData_02000a28;
extern s8 gData_02000a34;
extern s16 gData_02000a38;
extern s16 gData_02000a2c;

s32 BattleEffect_GetResourceId(s32 message);
struct EpilogueWindow *Message_OpenWindow(s32 id, s32 x, s32 y,
                                                s32 resource);
struct EpilogueWindow *Func_02000804(s32 message, s32 a, s32 left, s32 line);
s32 Message_AllTyped(void);
void Window_Close(struct EpilogueWindow *window, s32 mode);
void wait(s32 frames);

/// Opens the epilogue's message window at x and y and, unless bit 15 of the
/// message is set, a name window at left and line that Func_0200005c follows;
/// in mode 2 it restarts the window's text. Then it waits for the text, closes
/// both windows and waits the given frames.
void Func_02000158(s32 message, s32 x, s32 y, s32 left, s32 line, s32 frames,
                   s32 mode) {
  s32 wide;
  struct EpilogueWindow *window;
  struct EpilogueWindow *name;

  name = 0;
  wide = 0x8000 & message;
  message &= 0x7fff;
  window = Message_OpenWindow(
      gData_02000a28, x, y, (BattleEffect_GetResourceId(message) << 16) | 4);
  if (!wide) {
    /* FAKEMATCH(temporary): the name window held in a temporary until after
     * the stores; as name = Func_02000804(...), 2 of 100 instructions differ
     * in tla-en. */
    struct EpilogueWindow *opened = Func_02000804(message, 0, left, line);

    gData_02000a34 = 1;
    gData_02000a38 = left << 3;
    gData_02000a2c = line << 3;
    name = opened;
  }
  if (mode == 2) {
    window->unk0e++;
    window->unk08 = 0;
    window->unk0a = 0;
  }
  while (!Message_AllTyped())
    wait(1);
  wait(1);
  Window_Close(window, 2);
  if (!wide) {
    Window_Close(name, 2);
    gData_02000a34 = wide;
  }
  if (frames > 0) {
    s32 i = frames;

    do {
      wait(1);
    } while (--i != 0);
  }
  gData_02000a28++;
}
