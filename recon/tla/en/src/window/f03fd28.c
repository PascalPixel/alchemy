#include "resource.h"
#include "call.h"
#include "types.h"
#include "render_input.h"
#include "ram_buffer.h"
#include "dma.h"
#include "io_reg.h"

#define SLIDER_PALETTE 14
#define RESOURCE_SLOTS 96

extern const u8 gResource_FixedBlockBTiles[];
extern const u8 Data_0804e584[];
extern const u16 Data_080aa0e8[];

extern u8 MsgWindowColorLabel;
extern u8 MsgMessageSpeedLabel;
extern s8 gOptionMenu_SpeedIcons[];
extern s8 gOptionMenu_SpeechIcons[];
extern s8 gOptionMenu_SleepIcons[];
extern u8 MsgSpeechLabel[];
extern u8 MsgAutoSleepLabel[];

struct RenderInput *Window_Create(s32, s32, s32, s32, s32);
void Window_DrawDividerLine(struct RenderInput *, s32, s32, s32, s32);
void Text_DrawCharacterAtOffset(s32 message, struct RenderInput *win, s32 x,
                                s32 y);
struct WindowSprite *WindowSprite_Create(s32 no, s32 flags,
                                         struct RenderInput *input, s32 offsetX,
                                         s32 offsetY);
void *RenderResource_CreateCommandIcon(s32 frame, s32 flags,
                                       struct RenderInput *input, s32 offsetX,
                                       s32 offsetY);
s32 Resource_LoadFixedBlockBIntoFreeSlot(void);
s32 __divsi3(s32 numerator, s32 denominator);
void Func_08108048(void *object, s32 x, s32 y);

// The option screen's work (heap slot 0xd0), as The Broken Seal's
// workspace_options.h lays it out; the listing's offsets agree.
struct OptionCursorMove {
  struct WindowSprite *anchor;
  u16 x;
  u16 y;
  s16 targetX;
  s16 targetY;
  s8 active;
  s8 kind;
};

union OptionCursor {
  struct WindowSprite *output;
  struct OptionCursorMove cursor;
};

struct WorkspaceWork {
  u8 unk000[0x594];
  s8 option[5];
  s8 optionCount[5];
  u8 unk59e[6];
  union OptionCursor cursor;
  union OptionCursor marker[2];
  u8 unk5d4[0x18];
  struct WindowSprite *frame[3][3];
};

struct RenderInput *Menu_OpenWorkspaceOptions(void) {
  struct WorkspaceWork *work;
  struct RenderInput *win;
  struct WindowSprite *out;
  s32 x;
  s32 y;
  s32 msg;

  work = (struct WorkspaceWork *)Ram_HeapSlots->unkd0[0];
  win = Window_Create(1, 5, 27, 14, 2);
  Window_DrawDividerLine(win, 0, 2, 26, 2);
  Window_DrawDividerLine(win, 0, 4, 26, 4);
  Window_DrawDividerLine(win, 0, 7, 26, 7);
  Window_DrawDividerLine(win, 0, 10, 26, 10);
  msg = (s32)&MsgWindowColorLabel;
  Text_DrawCharacterAtOffset(msg, win, 8, 0);
  msg++;
  Text_DrawCharacterAtOffset(msg, win, 8, 16);
  msg = (s32)&MsgMessageSpeedLabel;
  Text_DrawCharacterAtOffset(msg, win, 8, 32);
  msg++;
  Text_DrawCharacterAtOffset(msg, win, 32, 40);
  Text_DrawCharacterAtOffset((s32)MsgSpeechLabel, win, 8, 64);
  Text_DrawCharacterAtOffset((s32)MsgAutoSleepLabel, win, 8, 88);

  out = WindowSprite_Create(Resource_LoadFixedBlockBIntoFreeSlot(), 0x40000000,
                            win, 0, 0);
  work->cursor.output = out;
  x = (s16)win->x * 8;
  y = (s16)win->y * 8 + 12;
  Func_08108048(&work->cursor, x, y);

  x = Resource_FindFreeEntry();
  if (x < RESOURCE_SLOTS) {
    Dma_Set(Data_080aa0e8, (void *)&OBJ_PLTT_COLOR(SLIDER_PALETTE, 0),
            0x80000020, SPLIT_CONSTANT(REG_DMA3));
    VramCache_Load(x, 256, Data_0804e584);
    y = 0;
    out = WindowSprite_Create(x, 0x40004000, win, 134, y);
    out->sprite.oam.palette = SLIDER_PALETTE;
    out = WindowSprite_Create(x, 0x40004000, win, 166, y);
    out->sprite.oam.index += 4;
    out->sprite.oam.palette = SLIDER_PALETTE;
    y = 16;
    out = WindowSprite_Create(x, 0x40004000, win, 134, y);
    out->sprite.oam.palette = 15;
    out = WindowSprite_Create(x, 0x40004000, win, 166, y);
    out->sprite.oam.index += 4;
    out->sprite.oam.palette = 15;
  }

  x = Resource_FindFreeEntry();
  if (x < RESOURCE_SLOTS) {
    VramCache_Load(x, 256, 0);
    out = WindowSprite_Create(x, 0x40000000, win, 0, 0);
    out->sprite.oam.palette256 = 1;
    work->marker[0].output = out;
    x = (s16)win->x * 8 + 140;
    x += __divsi3(work->option[0] * 60, work->optionCount[0]);
    y = (s16)win->y * 8 + 4;
    Func_08108048(&work->marker[0], x, y);
  }

  x = Resource_FindFreeEntry();
  if (x < RESOURCE_SLOTS) {
    VramCache_Load(x, 256, 0);
    out = WindowSprite_Create(x, 0x40000000, win, 0, 0);
    out->sprite.oam.palette256 = 1;
    work->marker[1].output = out;
    x = (s16)win->x * 8 + 140;
    x += __divsi3(work->option[1] * 60, work->optionCount[1]);
    y = (s16)win->y * 8 + 20;
    Func_08108048(&work->marker[1], x, y);
  }

  y = 28;
  work->frame[0][0] = RenderResource_CreateCommandIcon(
      gOptionMenu_SpeedIcons[0], 0, win, 84, y);
  work->frame[0][1] = RenderResource_CreateCommandIcon(
      gOptionMenu_SpeedIcons[1], 0, win, 108, y);
  work->frame[0][2] = RenderResource_CreateCommandIcon(
      gOptionMenu_SpeedIcons[2], 0, win, 132, y);
  y = 52;
  work->frame[1][0] = RenderResource_CreateCommandIcon(
      gOptionMenu_SpeechIcons[0], 0, win, 100, y);
  work->frame[1][1] = RenderResource_CreateCommandIcon(
      gOptionMenu_SpeechIcons[1], 0, win, 124, y);
  y = 76;
  work->frame[2][0] = RenderResource_CreateCommandIcon(
      gOptionMenu_SleepIcons[0], 0, win, 100, y);
  work->frame[2][1] = RenderResource_CreateCommandIcon(
      gOptionMenu_SleepIcons[1], 0, win, 124, y);
  return win;
}
