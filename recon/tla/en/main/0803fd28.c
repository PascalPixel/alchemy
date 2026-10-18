// Draft of Func_0803fd28, the option screen's window (Menu_OpenWorkspaceOptions
// here): differ 42 of 418 (tla-en, 2026-10-18). Its ☀️ twin is
// games/tbs/src/window/window.c's OptionMenu_Open: the labels, the slider
// tiles and palette, the two markers, the cursor and the setting frames. The
// Lost Age's window is a column narrower (27), takes the cursor's tiles from
// Resource_LoadFixedBlockBIntoFreeSlot, and reads the window position signed.
//
// Left: instruction order only. Each output pointer is stored into the work
// just after RenderOutput_Create in the listing and after the window reads
// here (the twin holds those stores in place with its shared cursor union, a
// tagged device); the rest are single loads one place apart.

#include "resource.h"
#include "types.h"
#include "render_input.h"
#include "ram_buffer.h"
#include "dma.h"
#include "io_reg.h"

#define SLIDER_PALETTE 14
#define RESOURCE_SLOTS 96

extern const u8 gResource_FixedBlockBTiles[];
extern const u8 gWorkspaceOptions_SliderTiles[];
extern const u16 gWorkspaceOptions_SliderPalette[];

extern u8 MsgWindowColorLabel;
extern u8 MsgMessageSpeedLabel;
extern s8 gData_080367c9[];
extern s8 gData_080367cc[];
extern s8 gData_080367ce[];
extern u8 MsgSpeechLabel[];
extern u8 MsgAutoSleepLabel[];

struct RenderInput *UiWindow_Create(s32, s32, s32, s32, s32);
void UiWindow_DrawDividerLine(struct RenderInput *, s32, s32, s32, s32);
void UiText_DrawCharacterAtOffset(s32 message, struct RenderInput *win, s32 x,
                                  s32 y);
struct RenderOutput *RenderOutput_Create(s32 no, s32 flags,
                                         struct RenderInput *input, s32 offsetX,
                                         s32 offsetY);
void *RenderResource_CreateFrame(s32 frame, s32 flags,
                                 struct RenderInput *input, s32 offsetX,
                                 s32 offsetY);
s32 Resource_LoadFixedBlockBIntoFreeSlot(void);
s32 __divsi3(s32 numerator, s32 denominator);
void Func_08108048(void *object, s32 x, s32 y);

// The option screen's work (heap slot 0xd0), as The Broken Seal's
// workspace_options.h lays it out; the listing's offsets agree.
struct OptionCursor {
  struct RenderOutput *output;
  u8 unk04[12];
};

struct WorkspaceWork {
  u8 unk000[0x594];
  s8 option[5];
  s8 optionCount[5];
  u8 unk59e[6];
  struct OptionCursor cursor;
  struct OptionCursor marker[2];
  u8 unk5d4[0x18];
  struct RenderOutput *frame[3][3];
};

struct RenderInput *Menu_OpenWorkspaceOptions(void) {
  struct WorkspaceWork *work;
  struct RenderInput *win;
  struct RenderOutput *out;
  s32 x;
  s32 y;
  s32 msg;

  work = (struct WorkspaceWork *)Ram_HeapSlots->unkd0[0];
  win = UiWindow_Create(1, 5, 27, 14, 2);
  UiWindow_DrawDividerLine(win, 0, 2, 26, 2);
  UiWindow_DrawDividerLine(win, 0, 4, 26, 4);
  UiWindow_DrawDividerLine(win, 0, 7, 26, 7);
  UiWindow_DrawDividerLine(win, 0, 10, 26, 10);
  msg = (s32)&MsgWindowColorLabel;
  UiText_DrawCharacterAtOffset(msg, win, 8, 0);
  msg++;
  UiText_DrawCharacterAtOffset(msg, win, 8, 16);
  msg = (s32)&MsgMessageSpeedLabel;
  UiText_DrawCharacterAtOffset(msg, win, 8, 32);
  msg++;
  UiText_DrawCharacterAtOffset(msg, win, 32, 40);
  UiText_DrawCharacterAtOffset((s32)MsgSpeechLabel, win, 8, 64);
  UiText_DrawCharacterAtOffset((s32)MsgAutoSleepLabel, win, 8, 88);

  out = RenderOutput_Create(Resource_LoadFixedBlockBIntoFreeSlot(), 0x40000000,
                            win, 0, 0);
  work->cursor.output = out;
  x = (s16)win->x * 8;
  y = (s16)win->y * 8 + 12;
  Func_08108048(&work->cursor, x, y);

  x = Resource_FindFreeEntry();
  if (x < RESOURCE_SLOTS) {
    Dma_Set(gWorkspaceOptions_SliderPalette,
            (void *)&OBJ_PLTT_COLOR(SLIDER_PALETTE, 0), 0x80000020, REG_DMA3);
    VramCache_Load(x, 256, gWorkspaceOptions_SliderTiles);
    y = 0;
    out = RenderOutput_Create(x, 0x40004000, win, 134, y);
    ((u8 *)&out->oam)[5] = (((u8 *)&out->oam)[5] & 15) | 224;
    out = RenderOutput_Create(x, 0x40004000, win, 166, y);
    out->oam.index += 4;
    ((u8 *)&out->oam)[5] = (((u8 *)&out->oam)[5] & 15) | 224;
    y = 16;
    out = RenderOutput_Create(x, 0x40004000, win, 134, y);
    ((u8 *)&out->oam)[5] |= 240;
    out = RenderOutput_Create(x, 0x40004000, win, 166, y);
    out->oam.index += 4;
    ((u8 *)&out->oam)[5] |= 240;
  }

  x = Resource_FindFreeEntry();
  if (x < RESOURCE_SLOTS) {
    VramCache_Load(x, 256, 0);
    out = RenderOutput_Create(x, 0x40000000, win, 0, 0);
    ((u8 *)&out->oam)[1] |= 32;
    work->marker[0].output = out;
    x = (s16)win->x * 8 + 140;
    x += __divsi3(work->option[0] * 60, work->optionCount[0]);
    y = (s16)win->y * 8 + 4;
    Func_08108048(&work->marker[0], x, y);
  }

  x = Resource_FindFreeEntry();
  if (x < RESOURCE_SLOTS) {
    VramCache_Load(x, 256, 0);
    out = RenderOutput_Create(x, 0x40000000, win, 0, 0);
    ((u8 *)&out->oam)[1] |= 32;
    work->marker[1].output = out;
    x = (s16)win->x * 8 + 140;
    x += __divsi3(work->option[1] * 60, work->optionCount[1]);
    y = (s16)win->y * 8 + 20;
    Func_08108048(&work->marker[1], x, y);
  }

  y = 28;
  work->frame[0][0] =
      RenderResource_CreateFrame(gData_080367c9[0], 0, win, 84, y);
  work->frame[0][1] =
      RenderResource_CreateFrame(gData_080367c9[1], 0, win, 108, y);
  work->frame[0][2] =
      RenderResource_CreateFrame(gData_080367c9[2], 0, win, 132, y);
  y = 52;
  work->frame[1][0] =
      RenderResource_CreateFrame(gData_080367cc[0], 0, win, 100, y);
  work->frame[1][1] =
      RenderResource_CreateFrame(gData_080367cc[1], 0, win, 124, y);
  y = 76;
  work->frame[2][0] =
      RenderResource_CreateFrame(gData_080367ce[0], 0, win, 100, y);
  work->frame[2][1] =
      RenderResource_CreateFrame(gData_080367ce[1], 0, win, 124, y);
  return win;
}
