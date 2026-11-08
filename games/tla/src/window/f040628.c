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

extern u8 MsgSaveYourQuest[];
extern u8 MsgChangeSettings[];
extern u8 MsgRoughMenu[];
extern const s8 gMenu_WorkspaceIcons[];

s32 GameFlag_Test(s32 flag);
struct RenderInput *Window_Create(s32 x, s32 y, s32 width, s32 height,
                                  s32 style);
void Window_DrawDividerLine(struct RenderInput *window, s32 x0, s32 y0, s32 x1,
                            s32 y1);
void Text_DrawMessage(s32 message, struct RenderInput *window, s32 x, s32 y);
s32 Resource_LoadFixedBlockBIntoFreeSlot(void);
struct WindowSprite *
RenderResource_CreateCommandIcon(s32 frame, s32 flag,
                                 struct RenderInput *window, s32 x, s32 y);

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
    msg = (s32)MsgSaveYourQuest;
    Text_DrawMessage(msg, win, 48, 4);
    msg++;
    Text_DrawMessage(msg, win, 48, 28);
    y = 52;
  }
  Text_DrawMessage((s32)MsgChangeSettings, win, 48, y);
  y += 24;
  if (gDebugMode != 0) {
    msg = (s32)MsgRoughMenu;
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
    work->icons[i] = RenderResource_CreateCommandIcon(
        gMenu_WorkspaceIcons[first + i], 0, win, 12, y);
    y += 24;
  }
  return win;
}
