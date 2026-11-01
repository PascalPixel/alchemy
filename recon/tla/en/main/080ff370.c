// Draft, differ 2 of 261, skeleton gap 1 (tla-en, 2026-11-01)
// Twin of The Broken Seal's StatusScreen_DrawStatusAilments
// (games/tbs/src/menu/menu/status.c): the same calls, rows and mode tests,
// with the status list moved out to Func_081054e0. Draws member ownerId's
// status page. MsgDjinnLabel and MsgPowerLabel are messages 4140 and 4141 in
// English; they need their names in the six catalogs before this links.
// Left: one instruction's order at the top. The native stores both spilled
// parameters before it builds the heap slots' address; this builds it first.
// In sched2 the address's first insn has priority 78 and the two stores 77,
// and every form below keeps that one-step gap.
// Tried without gain (2026-11-01): separate assignment, a slots or void *
// local, menu declared last or after unit, u32 mode or ownerId, a pointer
// window, unit initialized, register menu. The twin's declaration order
// gives differ 46 and a hasDjinn expression or ternary differ 19, so the
// locals keep the native slots (hasDjinn 8, djinnRow 12, element 16). No
// single empty asm or one-pass block reaches 0 (2026-10-26).
#include "types.h"
#include "ram_buffer.h"
#include "battle_unit.h"

struct MenuIcon {
  u8 unk00[5];
  u8 active;
};

struct CharacterMenuState {
  u8 unk000[0x170];
  struct MenuIcon *statusCursor;
  u8 unk174[0xa8];
  u16 page;
};

extern u8 MsgDjinnLabel[];
extern u8 MsgPowerLabel[];
extern const u8 gMenu_LvString[];
extern const u8 gMenu_SlashString[];

s32 Party_SumDjinnCountsFar(s32 owner);
struct BattleUnit *Unit_Get(s32 owner);
void ItemMenu_DrawOwnerStatus(s32 window, s32 owner, s32 flags);
s32 CharacterMenu_BuildAvailability(u8 *ailments, s32 requested, s32 ownerId);
void Window_ClearInteriorTilesFar(s32 window, s32 x, s32 y, s32 width,
                                    s32 height);
void Func_081054e0(s32 window, s32 x, s32 y, u8 *ailments);
void wait(s32 frames);
void Text_SetColorFar(s32 color);
void Window_SetTilemapEntryFar(s32 window, s32 tile, s32 x, s32 y,
                                 s32 palette);
void Text_DrawCharacterAtOffsetFar(u8 *message, s32 window, s32 x, s32 y);
void Text_DrawStringAtOffsetFar(const void *text, s32 window, s32 x, s32 y);
void Text_DrawStringInWindowFar(const void *text, s32 window, s32 x, s32 y);
void Text_DrawNumberInWindowFar(s32 value, s32 digits, s32 window, s32 x,
                                  s32 y);
s32 Unit_GetElementLevelFar(s32 ownerId, s32 element);

void Func_080ff370(s32 window, s32 ownerId, s32 mode) {
  struct CharacterMenuState *menu = Ram_HeapSlots->menuRuntime;
  struct BattleUnit *unit;
  s32 element;
  s32 djinnRow;
  s32 keepDrawn;
  s32 hasDjinn;
  u8 ailments[8];

  if (Party_SumDjinnCountsFar(-1) != 0)
    hasDjinn = 1;
  else
    hasDjinn = 0;
  unit = Unit_Get(ownerId);
  if ((mode & 0xff) == 1)
    djinnRow = 7;
  else
    djinnRow = 10;
  menu->statusCursor->active = 1;
  ItemMenu_DrawOwnerStatus(window, ownerId, mode);
  CharacterMenu_BuildAvailability(ailments, 1, ownerId);
  keepDrawn = mode & 0x100;
  if (keepDrawn == 0)
    Window_ClearInteriorTilesFar(window, 0, 40, 96, 96);
  Func_081054e0(window, 0, 40, ailments);
  if (menu->page == 3)
    return;
  if (keepDrawn == 0) {
    wait(1);
    Window_ClearInteriorTilesFar(window, 64, 56, 224, 96);
  }
  Text_SetColorFar(15);
  if (mode == 1 || hasDjinn == 1) {
    Window_SetTilemapEntryFar(window, 1, 15, djinnRow, 4);
    Window_SetTilemapEntryFar(window, 2, 19, djinnRow, 4);
    Window_SetTilemapEntryFar(window, 3, 23, djinnRow, 4);
    Window_SetTilemapEntryFar(window, 4, 27, djinnRow, 4);
  }
  if (hasDjinn)
    Text_DrawCharacterAtOffsetFar(MsgDjinnLabel, window, 64,
                                    djinnRow * 8 + 8);
  if (mode == 1) {
    if (!hasDjinn)
      djinnRow--;
    Text_DrawStringAtOffsetFar(gMenu_LvString, window, 64, djinnRow * 8 + 16);
    Text_DrawCharacterAtOffsetFar(MsgPowerLabel, window, 64,
                                    djinnRow * 8 + 24);
    Text_DrawCharacterAtOffsetFar(MsgPowerLabel + 1, window, 64,
                                    djinnRow * 8 + 32);
  }
  for (element = 0; element <= 3; element++) {
    if (hasDjinn)
      Text_DrawNumberInWindowFar(unit->djinnOwnedCounts[element], 1, window,
                                   element * 32 + 120, djinnRow * 8 + 8);
    if ((mode & 0xff) == 1) {
      if (hasDjinn) {
        Text_DrawNumberInWindowFar(unit->djinnActiveCounts[element], 1,
                                     window, element * 32 + 104,
                                     djinnRow * 8 + 8);
        Text_DrawStringInWindowFar(gMenu_SlashString, window,
                                     element * 32 + 112, djinnRow * 8 + 8);
      }
      Text_DrawNumberInWindowFar(Unit_GetElementLevelFar(ownerId, element), 2,
                                   window, element * 32 + 112,
                                   djinnRow * 8 + 16);
      Text_DrawNumberInWindowFar(unit->element[element].power, 3, window,
                                   element * 32 + 104, djinnRow * 8 + 24);
      Text_DrawNumberInWindowFar(unit->element[element].resistance, 3, window,
                                   element * 32 + 104, djinnRow * 8 + 32);
    }
  }
}
