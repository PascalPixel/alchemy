// First draft of DjinnMenu_DrawPage: draws four party members' djinn from page
// onward in the menu's list window, one column of 56 pixels each, grouped by
// element; each name is coloured by its state (Func_08101a04: 0 grey, 1
// normal, otherwise set) and drawn with Func_08101a54. With few enough djinn
// per member (at most 7) it also draws message 0x10de and a divider under the
// list. No ☀️ twin. Differ 71 of 146 (draft 147). Left: the native keeps the
// column counter counting up (cmp r3, #3) where loop.c reverses it here,
// and with it register choice: page stays in r6 rather than r8, y in r9
// rather than r10 and the column's entries in r10. Measured: walking
// pointers for the counts, owners and entries give 130; indexing each by
// page + column gives 182.
#include "types.h"
#include "ram_buffer.h"

struct DjinnMenuState {
  u8 unk000[0x34];
  s32 listWindow;
  u8 unk038[0xc0];
  void *owners[4];
};

struct DjinnColumn {
  u16 entries[10];
};

struct DjinnList {
  struct DjinnColumn columns[8];
  s8 counts[8];
};

extern u8 Msg010de[];

void RenderOutput_RedrawSavedRectFar(s32 window);
s32 Party_CountActiveOwnersFar(void);
s32 Party_SumDjinnCountsFar(s32 owner);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void UiWindow_DrawDividerLineFar(s32 window, s32 x, s32 y, s32 width,
                                 s32 height);
void Func_081019a4(struct DjinnList *list);
s32 Func_08101a04(u32 entry);
void Func_08101a54(s32 window, s32 x, s32 y, s32 entry);
s32 UiWork_SetParamNibbleFar(s32 color);
void WaitFrames(s32 frames);

void DjinnMenu_DrawPage(struct DjinnList *list, s32 page) {
  u8 *tiles = Ram_HeapSlots->windowTiles;
  struct DjinnMenuState *menu =
      (struct DjinnMenuState *)Ram_HeapSlots->menuRuntime;
  s32 window = menu->listWindow;
  s32 owners;
  s32 column;
  s32 row;
  s32 element;
  s32 i;
  s32 x;
  s32 y;

  tiles[6] = 1;
  RenderOutput_RedrawSavedRectFar(window);
  owners = Party_CountActiveOwnersFar();
  i = Party_SumDjinnCountsFar(-1);
  if (owners != 0 && (i - 1) / owners <= 6) {
    UiText_DrawCharacterAtOffsetFar((s32)Msg010de, window, 0, 80);
    UiWindow_DrawDividerLineFar(window, 0, 10, 28, 10);
  }
  Func_081019a4(list);
  x = 8;
  for (column = 0, row = page; column <= 3; column++, row++) {
    u16 *entries = list->columns[row].entries;

    y = 16;
    if (menu->owners[column] != NULL) {
      for (element = 0; element <= 3; element++) {
        for (i = 0; i < list->counts[row]; i++) {
          if (element == (entries[i] & 0xe0) >> 5) {
            switch (Func_08101a04(entries[i])) {
            case 0:
              UiWork_SetParamNibbleFar(2);
              break;
            case 1:
              UiWork_SetParamNibbleFar(15);
              break;
            default:
              UiWork_SetParamNibbleFar(4);
              break;
            }
            Func_08101a54(window, x, y, entries[i]);
            y += 8;
          }
        }
      }
    }
    x += 56;
  }
  tiles[3] = 1;
  tiles[6] = 0;
  WaitFrames(3);
}
