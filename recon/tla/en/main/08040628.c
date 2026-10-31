// Near miss, differ 4 of 161 (tla-en, 2026-10-30): scheduler order. When only
// the settings show, the native stores first (2) to its stack slot before it
// sets rows (1): movs r3,#2; str r3,[sp,#8]; movs r2,#1; mov r10,r2. This
// build interleaves the pair (movs r2,#1; movs r3,#2; mov r10,r2; str). In
// sched2 the four moves and the store all cost 1 with equal priority, so the
// tie goes to RTL order and only a block boundary gives the native order;
// written first = 2 before rows = 1, with a barrier between, the order is
// right but the round-robin reload registers trade r2 and r3. The Broken
// Seal's build also interleaves this pair (movs r0,#2; movs r3,#1; str; mov).
// No single empty asm, one-pass block, wrapping block or swap of neighbouring
// statements brings it closer (device search, 2026-10-26). Also no closer
// (2026-10-30): two ifs on settingsOnly (first then rows if-converts, 83
// differ), a ternary for first, first = 0 before the call, rows -= 2, rows
// from first, `if (first != 0)`, a volatile first, and a 4-minute permuter run.
// The Broken Seal's PauseMenu_Open (window/window/settings_menu.c); this game
// has three rows (one with flag 0x17e), three more in debug mode, no cursor
// object (the fixed block B output at 0x5a4) and its icons at 0x610. The three
// messages (0x1154, 0x1156 and 0x1158 in English) are pool symbols the
// catalogs must name before adoption (MsgTestA to MsgTestC here).

#include "types.h"
#include "ram_buffer.h"
#include "frame.h"
#include "render_input.h"

struct WorkspaceScreen {
  u8 unk000[0x5a4];
  struct WindowSprite *cursor;
  u8 unk5a8[0x68];
  struct WindowSprite *icons[6];
};

extern u8 MsgTestA[];
extern u8 MsgTestB[];
extern u8 MsgTestC[];
extern const s8 gData_0805ea8f[];

s32 GameFlag_Test(s32 flag);
struct RenderInput *Window_Create(s32 x, s32 y, s32 width, s32 height,
                                    s32 style);
void Window_DrawDividerLine(struct RenderInput *window, s32 x0, s32 y0,
                              s32 x1, s32 y1);
void Text_DrawMessage(s32 message, struct RenderInput *window, s32 x, s32 y);
s32 Resource_LoadFixedBlockBIntoFreeSlot(void);
struct WindowSprite *RenderResource_CreateCommandIcon(s32 frame, s32 flag,
                                                struct RenderInput *window,
                                                s32 x, s32 y);

struct RenderInput *Func_08040628(void) {
  struct WorkspaceScreen *work;
  s32 rows;
  s32 first;
  s32 settingsOnly;
  s32 top;
  s32 height;
  struct RenderInput *win;
  s32 y;
  s32 msg;
  s32 i;

  rows = 3;
  work = Ram_HeapSlots->unkd0[0];
  settingsOnly = GameFlag_Test(0x17e);
  first = 0;
  if (settingsOnly != 0) {
    rows = 1;
    first = 2;
  }
  if (gDebugMode != 0)
    rows += 3;
  top = 8 - rows;
  height = rows * 3 + 1;
  if (top + height > 19) {
    top = 1;
    height = 19;
  }
  win = Window_Create(5, top, 20, height, 2);
  for (i = 1; i < rows; i++)
    Window_DrawDividerLine(win, 0, i * 3, 19, i * 3);
  y = 4;
  if (settingsOnly == 0) {
    msg = (s32)MsgTestA;
    Text_DrawMessage(msg, win, 48, 4);
    msg++;
    Text_DrawMessage(msg, win, 48, 28);
    y = 52;
  }
  Text_DrawMessage((s32)MsgTestB, win, 48, y);
  y += 24;
  if (gDebugMode != 0) {
    msg = (s32)MsgTestC;
    Text_DrawMessage(msg, win, 48, y);
    y += 24;
    Text_DrawMessage(msg + 1, win, 48, y);
    y += 24;
    msg += 2;
    Text_DrawMessage(msg, win, 48, y);
  }
  work->cursor = WindowSprite_Create(Resource_LoadFixedBlockBIntoFreeSlot(),
                                     0x40000000, win, 0, 0);
  y = -4;
  for (i = 0; i < rows; i++) {
    work->icons[i] =
        RenderResource_CreateCommandIcon(gData_0805ea8f[first + i], 0, win, 12, y);
    y += 24;
  }
  return win;
}
