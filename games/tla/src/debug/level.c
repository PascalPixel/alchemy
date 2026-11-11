/// @file
/// Overlay 0x6b7's scene: its code up to level/finish.c, then its tables, in ROM order.

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "main_routine_work.h"
#include "event_inline.h"
#include "ram_buffer.h"
#include "actor.h"
#include "input.h"
#include "text/msg_ids.h"
#include "item.h"
#include "dma.h"

#include "edition.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern u32 gData_0200082c[];
extern u32 gData_020008a4[];
extern s16 gData_02000a0c[];
extern s16 gData_02000b74[];
extern u32 gData_02000cf4[];

const u8 *Func_0200004c(void) {
  if (gMainRoutineWork.entrance == 99)
    return gData_020008a4;
  if (gMainRoutineWork.entrance == 98)
    return gData_02000a0c;
  if (gMainRoutineWork.entrance == 97)
    return gData_02000b74;
  if (gMainRoutineWork.entrance == 96)
    return gData_02000cf4;
  return gData_0200082c;
}

void Func_020006e4(void);
void Func_020006ec(void);

void Func_02000098(s32 index) {
  Func_020006e4();
  Event_ResolvePendingActions(0);
  if (gMainRoutineWork.entrance == 99)
    Event_SetMessageId(index + (s32)MsgFixingThisWill);
  else if (gMainRoutineWork.entrance == 98)
    Event_SetMessageId(index + (s32)MsgFortuneTeller2);
  else if (gMainRoutineWork.entrance == 97)
    Event_SetMessageId(index + (s32)MsgConfirmGraphic2);
  else
    Event_SetMessageId(index + (s32)MsgPiers2);
  Event_ShowMessageAndWait(index, 0);
  Func_020006ec();
}

void Func_0200068c(s32 index, s32 flags);

void Func_020000fc(s32 index) {
  Func_020006e4();
  Event_ResolvePendingActions(0);
  Event_SetMessageId(index + (s32)MsgIsaac2);
  Func_0200068c(index, 1);
  Event_ShowMessageAndWait(index, 0);
  if (index == 1)
    Event_ShowMessageAndWait(0, 0);
  Func_020006ec();
}

extern s32 gData_02000f10[];
extern s32 gData_02000ebc[];
u8 *Unit_Get(s32 owner);
void Unit_Calculate(s32 owner);
void Func_020006a4(void);
void Func_020006ac(void);

const u8 *Func_02000138(void) {
  if (gMainRoutineWork.entrance > 94)
    return gData_02000f10;
  return gData_02000ebc;
}

void Func_02000160(s32 owner, s32 delta) {
  Unit_LevelUpTo(owner, Unit_Get(owner)[15] + delta);
  Unit_Calculate(owner);
}

void Func_02000160(s32 owner, s32 delta);

void Func_0200017c(s32 delta) {
  u16 owners[16];
  s32 count;
  s32 i;

  count = FieldParty_ListMembers(owners);
  for (i = 0; i < count; i++)
    Func_02000160(owners[i], delta);
}

extern u32 gData_02000744[];

void *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 style);
void Text_DrawMessage(s32 msg, void *window, s32 x, s32 y);
void WindowSprite_RedrawSavedRect(void *window);
void Text_DrawStringAtOffset(const u8 *text, void *window, s32 x, s32 y);
void Text_DrawStringInWindow(const u8 *text, void *window, s32 x, s32 y);
void Text_DrawNumberInWindow(s32 value, s32 digits, void *window, s32 x,
                               s32 y);
void Window_Close(void *window, s32 mode);
void Func_0200017c(s32 delta);

/// The pressed keys under a mask, for the last test of a group: the next
/// test loads gInput's address again.
static inline u32 Input_PressedFenced(u32 mask) {
  u32 keys = gInput.pressed & mask;

  return keys;
}

/// The debug room's level raiser: Start or Select raises the party's levels
/// by 5 and A by 1; B closes the window and recalculates the four units.
void Func_020001a8(void) {
  u8 *record;
  void *win;
  s32 flag;
  s32 msg;

  record = Unit_Get(gMainRoutineWork.currentOwner);
  win = Window_Create(0, 0, 30, 9, 2);

  msg = (s32)MsgRaiseEveryoneS;
  Text_DrawMessage(msg, win, 0, 0);
  Text_DrawMessage(msg + 1, win, 0, 16);
  msg += 2;
  flag = 1;
  Text_DrawMessage(msg, win, 0, 32);

  for (;;) {
    if (flag != 0) {
      WindowSprite_RedrawSavedRect(win);
      Text_DrawStringAtOffset(record, win, 0, 48);
      Text_DrawStringInWindow(gData_02000744, win, 48, 48);
      flag = 0;
      Text_DrawNumberInWindow(record[15], 0, win, 72, 48);
    }

    if ((gInput.pressed & 8) != 0 || Input_PressedFenced(4) != 0) {
      Func_0200017c(5);
      Sound_Play(93);
      flag = 1;
    }

    if ((gInput.pressed & 1) != 0) {
      Func_0200017c(1);
      Sound_Play(91);
      flag = 1;
    }

    if ((gInput.pressed & 2) != 0) {
      Sound_Play(113);
      WindowSprite_RedrawSavedRect(win);
      wait(1);
      Window_Close(win, 1);

      Unit_Calculate(0);
      Unit_Calculate(1);
      Unit_Calculate(3);
      Unit_Calculate(2);
      return;
    }

    wait(1);
  }
}

#define ITEM_COUNT 512

extern u32 gData_02000748[];
extern u32 gData_02000754[];
extern u32 gData_0200076c[];

void WindowSprite_ClearList(void *window);
void Text_DrawNumberAtOffset(s32 value, s32 format, void *window, s32 x, s32 y);
void Text_DrawCharacterAtOffset(s32 character, void *window, s32 x, s32 y);
s32 PartyInventory_HasSpace(void);
s32 PartyInventory_Add(s32 item);
void ItemMenu_DrawItemDetails(void *window, s32 item);
struct ItemDefinition *Func_020006bc(s32 item);

void Func_020002cc(void) {
  void *window;
  void *details;
  s32 item;
  s32 redraw;
  s32 index;
  s32 count;
  s8 step;

  count = ITEM_COUNT;
  step = 0;
  Sound_Play(112);
  item = 1;
  redraw = 1;
  window = Window_Create(0, 0, 30, 7, 2);
#if LANGUAGE < LANGUAGE_GERMAN
  details = Window_Create(0, 8, 13, 10, 2);
#else
  details = Window_Create(0, 9, 18, 10, 2);
#endif
  Dma_Set((const void *)0x05000200, (void *)0x050001c0, 0x80000010,
          (volatile u32 *)0x040000d4);
  Dma_Set((const void *)0x050001e8, (void *)0x050001dc, 0x80000001,
          (volatile u32 *)0x040000d4);
  wait(1);
  while (1) {
    if (redraw) {
      redraw = 0;
      item = (item + ITEM_COUNT) % count;
      WindowSprite_RedrawSavedRect(window);
      WindowSprite_ClearList(window);
      Text_DrawStringInWindow(gData_02000748, window, 0, 0);
      Text_DrawNumberAtOffset(item, 0, window, 80, 0);
      if (PartyInventory_HasSpace()) {
        Text_DrawStringInWindow(gData_02000754, window, 0, 32);
        index = item & 0x1ff;
        Func_020006bc(index);
        Text_DrawCharacterAtOffset(index + (s32)MsgItemName, window, 120, 0);
        Text_DrawMessage(index + (s32)MsgItemPlainName, window, 0, 16);
        WindowSprite_RedrawSavedRect(details);
        ItemMenu_DrawItemDetails(details, item);
      } else {
        Text_DrawStringInWindow(gData_0200076c, window, 0, 32);
      }
    }
    if (Input_PressedFenced(1)) {
      if (PartyInventory_Add(item) == -1) {
        Sound_Play(113);
        break;
      }
      Sound_Play(175);
    }
    if (gInput.pressed & 2) {
      Sound_Play(113);
      break;
    }
    if (gInput.repeat & 64) {
      step = -1;
      item--;
      redraw = 1;
      Sound_Play(111);
    }
    if (gInput.repeat & 128) {
      step = 1;
      item++;
      redraw = 1;
      Sound_Play(111);
    }
    if (gInput.repeat & 16) {
      step = 1;
      item += 10;
      redraw = 1;
      Sound_Play(111);
    }
    if (gInput.repeat & 32) {
      step = -1;
      item -= 10;
      redraw = 1;
      Sound_Play(111);
    }
    if (gInput.repeat & 256) {
      step = 1;
      item += 30;
      redraw = 1;
      Sound_Play(111);
    }
    if (gInput.repeat & 512) {
      step = -1;
      item -= 30;
      redraw = 1;
      Sound_Play(111);
    }
    if (step == -1) {
      item = (item + ITEM_COUNT) % count;
      goto check_back;
    back:
      item = (item + ITEM_COUNT - 1) % count;
    check_back:
      if (Func_020006bc(item & 0x1ff)->icon == 0)
        goto back;
    }
    if (step == 1) {
      item = (item + ITEM_COUNT) % count;
      goto check_ahead;
    ahead:
      item = (item + ITEM_COUNT + 1) % count;
    check_ahead:
      if (Func_020006bc(item & 0x1ff)->icon == 0)
        goto ahead;
    }
    step = 0;
    wait(1);
  }
  WindowSprite_RedrawSavedRect(window);
  wait(1);
  Window_Close(window, 1);
  Window_Close(details, 1);
}

void Func_020001a8(void);
void Func_020002cc(void);
void Func_020005d8(void);
void Func_02000610(void);
void Func_02000624(void);
void Func_0200062c(void);

u32 gData_02000744[] = {
    0x764c,
};

u32 gData_02000748[] = {
    0x6d657449,
    0x3a6f4e20,
    0,
};

// clang-format off
u32 gData_02000754[] = {
    0x65473a41, 0x74492074, 0x20206d65, 0x65523a42, 0x6e727574, 0,
};

u32 gData_0200076c[] = {
    0x4d455449, 0x4c554620, 0x2e2e2e4c, 0x2e2e2e2e, 46,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 80, 0, 72, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {10, CONDITION_ALWAYS, 80, 0, 272, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {96, CONDITION_ALWAYS, 192, 0, 240, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {97, CONDITION_ALWAYS, 192, 0, 240, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {98, CONDITION_ALWAYS, 176, 0, 240, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {99, CONDITION_ALWAYS, 176, 0, 240, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS_END,
};

u32 gData_0200082c[] = {
    0xffff0150, 1, 0x300000, 0, 0x1100000, 0x2000,
    0xffff00fc, 1, 0x300000, 0, 0x480000, 0x2000,
    0xffff00fd, 1, 0x300000, 0, 0x880000, 0x2000,
    0xffff00d8, 1, 0x680000, 0, 0x1100000, 0x2000,
    0xffff, 0, 0, 0, 0, 0,
};

u32 gData_020008a4[] = {
    0xffff0015, 1, 0xa00000, 0, 0xa00000, 0x22000,
    0xffff0012, 1, 0xa00000, 0, 0xc00000, 0x22000,
    0xffff0011, 1, 0xa00000, 0, 0xe00000, 0x22000,
    0xffff0014, 1, 0xa00000, 0, 0x1000000, 0x22000,
    0xffff0013, 1, 0xa00000, 0, 0x1200000, 0x22000,
    0xffff0018, 1, 0xa00000, 0, 0x1400000, 0x22000,
    0xffff0019, 1, 0xa00000, 0, 0x1600000, 0x22000,
    0xffff001a, 1, 0xd00000, 0, 0xa00000, 0x22000,
    0xffff0029, 1, 0xd00000, 0, 0xc00000, 0x22000,
    0xffff002a, 1, 0xd00000, 0, 0xe00000, 0x22000,
    0xffff0010, 1, 0xd00000, 0, 0x1000000, 0x22000,
    0xffff0025, 1, 0xd00000, 0, 0x1200000, 0x22000,
    0xffff0026, 1, 0xd00000, 0, 0x1400000, 0x22000,
    0xffff0024, 1, 0xd00000, 0, 0x1600000, 0x22000,
    0xffff, 0, 0, 0, 0, 0,
};

s16 gData_02000a0c[] = {
    29, -1, 1, 0, 0, 256, 0, 0,
    0, 160, 0x2000, 2, 30, -1, 1, 0,
    0, 256, 0, 0, 0, 192, 0x2000, 2,
    32, -1, 1, 0, 0, 256, 0, 0,
    0, 224, 0x2000, 2, 31, -1, 1, 0,
    0, 256, 0, 0, 0, 256, 0x2000, 2,
    34, -1, 1, 0, 0, 256, 0, 0,
    0, 288, 0x2000, 2, 23, -1, 1, 0,
    0, 256, 0, 0, 0, 320, 0x2000, 2,
    33, -1, 1, 0, 0, 256, 0, 0,
    0, 352, 0x2000, 2, 60, -1, 1, 0,
    0, 208, 0, 0, 0, 160, 0x2000, 2,
    61, -1, 1, 0, 0, 208, 0, 0,
    0, 192, 0x2000, 2, 62, -1, 1, 0,
    0, 208, 0, 0, 0, 224, 0x2000, 2,
    43, -1, 1, 0, 0, 208, 0, 0,
    0, 256, 0x2000, 2, 44, -1, 1, 0,
    0, 208, 0, 0, 0, 288, 0x2000, 2,
    46, -1, 1, 0, 0, 208, 0, 0,
    0, 320, 0x2000, 2, 45, -1, 1, 0,
    0, 208, 0, 0, 0, 352, 0x2000, 2,
    -1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0,
};

s16 gData_02000b74[] = {
    0, -1, 1, 0, 0, 160, 0, 0,
    0, 160, 0x2000, 2, 1, -1, 1, 0,
    0, 160, 0, 0, 0, 192, 0x2000, 2,
    2, -1, 1, 0, 0, 160, 0, 0,
    0, 224, 0x2000, 2, 3, -1, 1, 0,
    0, 160, 0, 0, 0, 256, 0x2000, 2,
    5, -1, 1, 0, 0, 160, 0, 0,
    0, 288, 0x2000, 2, 6, -1, 1, 0,
    0, 160, 0, 0, 0, 320, 0x2000, 2,
    7, -1, 1, 0, 0, 160, 0, 0,
    0, 352, 0x2000, 2, 57, -1, 1, 0,
    0, 208, 0, 0, 0, 160, 0x2000, 2,
    56, -1, 1, 0, 0, 208, 0, 0,
    0, 192, 0x2000, 2, 59, -1, 1, 0,
    0, 208, 0, 0, 0, 224, 0x2000, 2,
    201, -1, 1, 0, 0, 208, 0, 0,
    0, 256, 0x2000, 2, 202, -1, 1, 0,
    0, 208, 0, 0, 0, 288, 0x2000, 2,
    64, -1, 1, 0, 0, 208, 0, 0,
    0, 336, 0x2000, 2, 65, -1, 1, 0,
    0, 208, 0, 0, 0, 368, 0x2000, 2,
    63, -1, 1, 0, 0, 208, 0, 0,
    0, 400, 0x2000, 2, -1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

u32 gData_02000cf4[] = {
    0xffff00cd, 1, 0x800000, 0, 0xa00000, 0x22000,
    0xffff00ce, 1, 0x800000, 0, 0xc00000, 0x22000,
    0xffff00c6, 1, 0x800000, 0, 0xe00000, 0x22000,
    0xffff0079, 1, 0x800000, 0, 0x1000000, 0x22000,
    0xffff0077, 1, 0x800000, 0, 0x1200000, 0x22000,
    0xffff007b, 1, 0x800000, 0, 0x1400000, 0x22000,
    0xffff0068, 1, 0xb00000, 0, 0xa00000, 0x22000,
    0xffff0069, 1, 0xb00000, 0, 0xc00000, 0x22000,
    0xffff0081, 1, 0xb00000, 0, 0xe00000, 0x22000,
    0xffff0082, 1, 0xb00000, 0, 0x1000000, 0x22000,
    0xffff00c7, 1, 0xb00000, 0, 0x1200000, 0x22000,
    0xffff00d1, 1, 0xb00000, 0, 0x1400000, 0x22000,
    0xffff006c, 1, 0xe00000, 0, 0xa00000, 0x22000,
    0xffff006d, 1, 0xe00000, 0, 0xc00000, 0x22000,
    0xffff006e, 1, 0xe00000, 0, 0xe00000, 0x22000,
    0xffff006f, 1, 0xe00000, 0, 0x1000000, 0x22000,
    0xffff0086, 1, 0xe00000, 0, 0x1200000, 0x22000,
    0xffff0087, 1, 0xe00000, 0, 0x1400000, 0x22000,
    0xffff, 0, 0, 0, 0, 0,
};
// clang-format on

s32 gData_02000ebc[] = {
    0,
    (s32)0xffff0008,
    (s32)Func_020002cc,
    0,
    (s32)0xffff0009,
    (s32)Func_02000624,
    0,
    (s32)0xffff000a,
    (s32)Func_0200062c,
    0,
    (s32)0xffff000b,
    (s32)Func_020001a8,
    0x8e15,
    (s32)0xffff000a,
    (s32)Func_020005d8,
    0x10008e15,
    (s32)0xffff000a,
    (s32)Func_02000610,
    -1,
    0,
    0,
};

s32 gData_02000f10[] = {
    0,
    -0x10000,
    (s32)Func_020000fc,
    0,
    (s32)0xffff0001,
    (s32)Func_020000fc,
    0,
    (s32)0xffff0002,
    (s32)Func_020000fc,
    0,
    (s32)0xffff0003,
    (s32)Func_020000fc,
    0,
    (s32)0xffff0005,
    (s32)Func_020000fc,
    0,
    (s32)0xffff0006,
    (s32)Func_020000fc,
    0,
    (s32)0xffff0007,
    (s32)Func_020000fc,
    0,
    (s32)0xffff0008,
    (s32)Func_02000098,
    0,
    (s32)0xffff0009,
    (s32)Func_02000098,
    0,
    (s32)0xffff000a,
    (s32)Func_02000098,
    0,
    (s32)0xffff000b,
    (s32)Func_02000098,
    0,
    (s32)0xffff000c,
    (s32)Func_02000098,
    0,
    (s32)0xffff000d,
    (s32)Func_02000098,
    0,
    (s32)0xffff000e,
    (s32)Func_02000098,
    0,
    (s32)0xffff000f,
    (s32)Func_02000098,
    0,
    (s32)0xffff0010,
    (s32)Func_02000098,
    0,
    (s32)0xffff0011,
    (s32)Func_02000098,
    0,
    (s32)0xffff0012,
    (s32)Func_02000098,
    0,
    (s32)0xffff0013,
    (s32)Func_02000098,
    0,
    (s32)0xffff0014,
    (s32)Func_02000098,
    0,
    (s32)0xffff0015,
    (s32)Func_02000098,
    0,
    (s32)0xffff0016,
    (s32)Func_02000098,
    0,
    (s32)0xffff0017,
    (s32)Func_02000098,
    0,
    (s32)0xffff0018,
    (s32)Func_02000098,
    0,
    (s32)0xffff0019,
    (s32)Func_02000098,
    0,
    (s32)0xffff001a,
    (s32)Func_02000098,
    0,
    (s32)0xffff001b,
    (s32)Func_02000098,
    0,
    (s32)0xffff001c,
    (s32)Func_02000098,
    0,
    (s32)0xffff001d,
    (s32)Func_02000098,
    0,
    (s32)0xffff001e,
    (s32)Func_02000098,
    0,
    (s32)0xffff001f,
    (s32)Func_02000098,
    0,
    (s32)0xffff0020,
    (s32)Func_02000098,
    0x8e15,
    (s32)0xffff000a,
    (s32)Func_020005d8,
    0x10008e15,
    (s32)0xffff000a,
    (s32)Func_02000610,
    -1,
    0,
    0,
};
