// Near miss, differ 8 of 146 (tla-en, 2026-10-31; 69 before): scheduler order.
// Before the loop, sched2 issues the column's entries address (page * 20 +
// list: lsl, add, lsl, add fp) as soon as x's store frees r3, because its
// chain ranks first (priority 5, against 3 for page + 160 and 2 for the rest);
// the native interleaves it with column = 0, page + 160 and menu + 248 and
// adds the list after the three stores. Every register already matches, so the
// dependences, and with them the priorities, are fixed: no C reshaping moves
// it (permuter 5 minutes: nothing; a hoisted column or entries pointer: 59).
// DjinnMenu_DrawPage draws four party members' djinn from page onward in the
// menu's list window, one column of 56 pixels each, grouped by element; each
// name is coloured by its state (Func_08101a04: 0 grey, 1 normal, otherwise
// set) and drawn with Func_08101a54. With few enough djinn per member (at most
// 7) it also draws message 0x10de and a divider under the list. No ☀️ twin.
// The lists indexed by page + column (the native keeps the count's offset
// apart from the list and counts the column up), the djinn total in its own
// local, x stepped in the for and the menu read before the tiles give the
// native's shape and registers.
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

void WindowSprite_RedrawSavedRectFar(s32 window);
s32 Party_CountActiveOwnersFar(void);
s32 Party_SumDjinnCountsFar(s32 owner);
void Text_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void Window_DrawDividerLineFar(s32 window, s32 x, s32 y, s32 width,
                                 s32 height);
void Func_081019a4(struct DjinnList *list);
s32 Func_08101a04(u32 entry);
void Func_08101a54(s32 window, s32 x, s32 y, s32 entry);
s32 Text_SetColorFar(s32 color);
void wait(s32 frames);

void DjinnMenu_DrawPage(struct DjinnList *list, s32 page) {
  struct DjinnMenuState *menu =
      (struct DjinnMenuState *)Ram_HeapSlots->menuRuntime;
  u8 *tiles = Ram_HeapSlots->windowTiles;
  s32 window = menu->listWindow;
  s32 owners;
  s32 column;
  s32 element;
  s32 i;
  s32 x;
  s32 y;
  s32 djinn;

  tiles[6] = 1;
  WindowSprite_RedrawSavedRectFar(window);
  owners = Party_CountActiveOwnersFar();
  djinn = Party_SumDjinnCountsFar(-1);
  if (owners != 0 && (djinn - 1) / owners <= 6) {
    Text_DrawCharacterAtOffsetFar((s32)Msg010de, window, 0, 80);
    Window_DrawDividerLineFar(window, 0, 10, 28, 10);
  }
  Func_081019a4(list);
  for (column = 0, x = 8; column <= 3; column++, x += 56) {
    u16 *entries = list->columns[page + column].entries;

    y = 16;
    if (menu->owners[column] != NULL) {
      for (element = 0; element <= 3; element++) {
        for (i = 0; i < list->counts[page + column]; i++) {
          if (element == (entries[i] & 0xe0) >> 5) {
            switch (Func_08101a04(entries[i])) {
            case 0:
              Text_SetColorFar(2);
              break;
            case 1:
              Text_SetColorFar(15);
              break;
            default:
              Text_SetColorFar(4);
              break;
            }
            Func_08101a54(window, x, y, entries[i]);
            y += 8;
          }
        }
      }
    }
  }
  tiles[3] = 1;
  tiles[6] = 0;
  wait(3);
}
