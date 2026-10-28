#include "types.h"
#include "ram_buffer.h"
#include "battle_unit.h"
#include "edition.h"

struct ItemMenuCursor {
  u8 unk00[5];
  u8 state;
};

struct ItemMenuState {
  u8 unk000[0x170];
  struct ItemMenuCursor *cursor;
};

extern u8 MsgExpLabel[];
extern u8 MsgClassName[];
extern const u8 gMenu_LvString[];
extern const u8 gMenu_SlashString[];
extern const u8 gMenu_HpString[];
extern const u8 gMenu_PpString[];

void Window_ClearInteriorTilesFar(s32 window, s32 x, s32 y, s32 width,
                                    s32 height);
void Text_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void Text_DrawStringAtOffsetFar(const void *text, s32 window, s32 x, s32 y);
void Text_DrawStringInWindowFar(const void *text, s32 window, s32 x, s32 y);
void Text_DrawNumberInWindowFar(s32 value, s32 digits, s32 window, s32 x,
                                  s32 y);
void Text_SetColorFar(s32 color);
struct BattleUnit *Unit_Get(s32 owner);
void WaitFrames(s32 frames);

// The Japanese edition draws the Lv, HP and PP labels through
// Text_DrawStringInWindowFar, and the four stats a digit wider, starting a
// tile further left.
#if LANGUAGE == LANGUAGE_JAPANESE
#define DRAW_STAT_LABEL Text_DrawStringInWindowFar
#define STAT_DIGITS 4
#define STAT_X 192
#else
#define DRAW_STAT_LABEL Text_DrawStringAtOffsetFar
#define STAT_DIGITS 3
#define STAT_X 200
#endif

void ItemMenu_DrawOwnerStatus(s32 window, s32 owner, s32 flags) {
  struct ItemMenuState *menu;
  struct BattleUnit *unit;

  menu = Ram_HeapSlots->menuRuntime;
  unit = Unit_Get(owner);
  menu->cursor->state = 1;
  flags &= 0x100;
  if (flags == 0)
    Window_ClearInteriorTilesFar(window, 0, 0, 128, 40);
  Text_DrawStringAtOffsetFar(unit->name, window, 40, 0);
  Text_DrawCharacterAtOffsetFar(unit->classIndex + (s32)MsgClassName, window,
                                  0, 32);
  DRAW_STAT_LABEL(gMenu_LvString, window, 104, 0);
  Text_SetColorFar(15);
  Text_DrawNumberInWindowFar(unit->level, 2, window, 128, 0);
  DRAW_STAT_LABEL(gMenu_HpString, window, 40, 16);
  Text_DrawNumberInWindowFar(unit->hp, 4, window, 72, 16);
  Text_DrawNumberInWindowFar(unit->maxHp, 4, window, 112, 16);
  Text_DrawStringInWindowFar(gMenu_SlashString, window, 104, 16);
  DRAW_STAT_LABEL(gMenu_PpString, window, 40, 24);
  Text_DrawNumberInWindowFar(unit->pp, 4, window, 72, 24);
  Text_DrawNumberInWindowFar(unit->maxPp, 4, window, 112, 24);
  Text_DrawStringInWindowFar(gMenu_SlashString, window, 104, 24);
  Text_DrawCharacterAtOffsetFar((s32)MsgExpLabel, window, 40, 8);
  Text_DrawNumberInWindowFar(unit->experience, 8, window, 80, 8);
  if (flags == 0) {
    WaitFrames(1);
    Window_ClearInteriorTilesFar(window, 144, 0, 224, 40);
  }
  Text_DrawCharacterAtOffsetFar((s32)MsgExpLabel - 23, window, 152, 0);
  Text_DrawCharacterAtOffsetFar((s32)MsgExpLabel - 22, window, 152, 8);
  Text_DrawCharacterAtOffsetFar((s32)MsgExpLabel - 21, window, 152, 16);
  Text_DrawCharacterAtOffsetFar((s32)MsgExpLabel - 20, window, 152, 24);
  Text_DrawNumberInWindowFar(unit->atk, STAT_DIGITS, window, STAT_X, 0);
  Text_DrawNumberInWindowFar(unit->def, STAT_DIGITS, window, STAT_X, 8);
  Text_DrawNumberInWindowFar(unit->agi, STAT_DIGITS, window, STAT_X, 16);
  Text_DrawNumberInWindowFar(unit->luck, STAT_DIGITS, window, STAT_X, 24);
}
