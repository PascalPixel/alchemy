// Near miss, differ 26 of 495: The Broken Seal's PartyStatusDraw on this
// game's work: at most four party members outside battle, the Djinn counts
// drawn as tiles, with the glyphs past 9 copied to VRAM when any count passes
// 9. Left, instruction order only but for one row: the native stores the
// work pointer's stack slot and reads its menu state one row later, sets up
// the member loop's stack slots in another order, copies hp to r0 before the
// call's other arguments rather than last, and builds the zero for the Djinn
// tiles' last argument (movs r7, #0) inside the Djinn block, where here it is
// set before the menu-state test.
#include "types.h"
#include "ram_buffer.h"
#include "window.h"
#include "uiwindow.h"
#include "battle_work.h"
#include "party_state.h"
#include "dma.h"
#include "io_reg.h"

struct PartyStatusWindow {
  struct UiWindow *window;
  u16 x;
  u16 y;
  u16 width;
  u16 height;
  u16 flags;
};

struct StatusUnit {
  u8 name[0x34];
  s16 maxHp;
  s16 maxPp;
  s16 hp;
  s16 pp;
};

#define PARTY_STATUS_KEEP_FLAGS -1
#define PARTY_STATUS_SHOW_PP 1
#define PARTY_STATUS_SHOW_DJINN 2
#define PARTY_STATUS_CLOSED 9
#define PARTY_STATUS_DJINN_WIDTH 5
#define PARTY_STATUS_COLUMN_WIDTH 6

extern const u8 Data_080aa1b8[];

s32 BattleParty_PrepareActiveOwnersFar(u16 *owners);
s32 Party_CountActiveOwnersFar(void);
s32 Func_08118130(s32 a, u8 *counts);
void UiWindow_EraseBorderRect(s32 x, s32 y, u32 width, u32 height);
void RenderOutput_RedrawSavedRect(struct UiWindow *window);
void Func_080426e0(struct UiWindow *window, u32 flags);
void UiWindow_BuildLayoutBounds(s32 flags);
void UiWindow_DrawFrame(s32 x, s32 y, s32 width, s32 height);
struct StatusUnit *Unit_Get(s32 owner);
void Func_08041f70(s32 nibble);
void UiText_DrawPrefixedNumberAtOffset(s32 value, struct UiWindow *window,
                                       s32 x, s32 y, s32 kind);
void UiText_DrawStringAtOffset(u8 *text, struct UiWindow *window, s32 x, s32 y);
s32 Func_08042808(struct UiWindow *window, s32 x, s32 y, s32 value);
void UiWindow_SetTilemapEntry(struct UiWindow *window, s32 value, s32 x, s32 y,
                              s32 flags);

#define DJINN_COUNT_TILE(n) ((n) > 9 ? (n) + 0xf196 : ((n) + '0') | 0xf000)

void PartyStatusDraw(s32 flags) {
  struct PartyStatusWindow *layout = Ram_HeapSlots->windowLayout;
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  struct UiRenderWork *work = (struct UiRenderWork *)Ram_HeapSlots->windowTiles;
  struct UiWindow *window = layout->window;
  s32 x = 0;
  u32 count;
  s32 y = 0;
  u32 i;
  u16 members[5];
  u8 standbyDjinn[4];

  if (work->menuState != 0) {
    count = BattleParty_PrepareActiveOwnersFar(0);
    y = -1;
    for (i = 0; i < count; i++) {
      members[i] = battle->partyUnits[i];
      if (members[i] == 255)
        break;
    }
  } else {
    count = Party_CountActiveOwnersFar();
    if (count > 4)
      count = 4;
    for (i = 0; i < count; i++)
      members[i] = gPartyState.activeOwners[i];
    members[i] = 255;
  }
  count = i;

  if (flags == PARTY_STATUS_KEEP_FLAGS)
    flags = layout->flags;
  if (!(flags & PARTY_STATUS_SHOW_PP))
    flags &= ~PARTY_STATUS_SHOW_DJINN;
  if (work->menuState == 0 || Func_08118130(0, 0) == 0)
    flags &= ~PARTY_STATUS_SHOW_DJINN;
  if (flags == PARTY_STATUS_CLOSED) {
    UiWindow_EraseBorderRect(layout->x, layout->y, layout->width,
                             layout->height);
    return;
  }

  work->menuBusy = 1;
  if (layout->flags == flags) {
    RenderOutput_RedrawSavedRect(window);
    Func_080426e0(window, flags);
  } else {
    UiWindow_EraseBorderRect(layout->x, layout->y, layout->width,
                             layout->height);
    UiWindow_BuildLayoutBounds(flags);
    window->width = layout->width;
    window->height = layout->height;
    window->x = layout->x;
    UiWindow_DrawFrame(layout->x, layout->y, layout->width, layout->height);
    Func_080426e0(window, flags);
  }

  if (flags & PARTY_STATUS_SHOW_DJINN)
    x = PARTY_STATUS_DJINN_WIDTH;
  for (i = 0; i != count; i++) {
    struct StatusUnit *unit = Unit_Get(members[i]);
    s32 hp = unit->hp;
    s32 maxHp = unit->maxHp;
    s32 barLength;
    s32 current;

    if (hp == 0)
      Func_08041f70(2);
    else if (hp <= maxHp / 4)
      Func_08041f70(4);
    else
      Func_08041f70(15);
    work->level = 14;
    if (work->menuState != 0)
      work->level = 5;
    UiText_DrawPrefixedNumberAtOffset(
        hp, window, (x + i * PARTY_STATUS_COLUMN_WIDTH) * 8, y * 8 + 8, 0);
    work->level = 15;
    UiText_DrawStringAtOffset(unit->name, window,
                              (x + i * PARTY_STATUS_COLUMN_WIDTH) * 8, y * 8);
    Func_08041f70(15);
    if (unit->maxHp != 0) {
      current = unit->hp;
      barLength = (current * 40) / unit->maxHp;
      if (barLength == 0 && current != 0)
        barLength = 1;
      Func_08042808(window, x + i * PARTY_STATUS_COLUMN_WIDTH + 1, y + 2,
                    barLength);
    }
    if (flags & PARTY_STATUS_SHOW_PP) {
      work->level = 14;
      if (work->menuState != 0)
        work->level = 5;
      UiText_DrawPrefixedNumberAtOffset(unit->pp, window,
                                        (x + i * PARTY_STATUS_COLUMN_WIDTH) * 8,
                                        y * 8 + 16, 1);
      if (unit->maxPp != 0) {
        current = unit->pp;
        barLength = (current * 40) / unit->maxPp;
        if (barLength == 0 && current != 0)
          barLength = 1;
        Func_08042808(window, x + i * PARTY_STATUS_COLUMN_WIDTH + 1, y + 3,
                      barLength);
      }
    }
  }
  work->level = 15;

  if (work->menuState != 0 && (flags & PARTY_STATUS_SHOW_DJINN)) {
    s32 row = y;

    if (flags & PARTY_STATUS_SHOW_PP)
      row++;
    Func_08118130(0, standbyDjinn);
    UiWindow_SetTilemapEntry(window, 0x5001, 0, row, 0);
    UiWindow_SetTilemapEntry(window, 0x5002, 2, row, 0);
    UiWindow_SetTilemapEntry(window, 0x5003, 0, row + 1, 0);
    UiWindow_SetTilemapEntry(window, 0x5004, 2, row + 1, 0);
    if (standbyDjinn[0] > 9 || standbyDjinn[1] > 9 || standbyDjinn[2] > 9 ||
        standbyDjinn[3] > 9)
      Dma_Set(Data_080aa1b8, (void *)0x06003400, 0x84000048, REG_DMA3);
    UiWindow_SetTilemapEntry(window, DJINN_COUNT_TILE(standbyDjinn[0]), 1, row,
                             0);
    UiWindow_SetTilemapEntry(window, DJINN_COUNT_TILE(standbyDjinn[1]), 3, row,
                             0);
    UiWindow_SetTilemapEntry(window, DJINN_COUNT_TILE(standbyDjinn[2]), 1,
                             row + 1, 0);
    UiWindow_SetTilemapEntry(window, DJINN_COUNT_TILE(standbyDjinn[3]), 3,
                             row + 1, 0);
  }
  work->menuBusy = 0;
}
