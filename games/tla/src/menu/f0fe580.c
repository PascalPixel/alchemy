#include "types.h"
#include "ram_buffer.h"

struct OwnerCursor {
  u8 unk00[5];
  u8 active;
  u8 unk06[6];
  u16 animStep;
};

struct OwnerMenuState {
  u8 unk000[16];
  void *ownerWindow;
  struct OwnerCursor *ownerCursors[2];
  s8 ownerIndex[2];
  u8 unk01e[0x1fe];
  u16 page;
};

void WindowSprite_RedrawSavedRectFar(void *window);
s32 GameFlag_Test(s32 flag);
void Window_DrawDividerLineFar(void *window, u32 x1, u32 y1, u32 x2, u32 y2);
void Func_080f8ab4(s32 x, s32 y);
s32 Func_080feb58(void);
s32 Func_080fe894(void);
void Icon_PrepareActor(struct OwnerCursor *cursor);
void wait(s32 frames);

/// The twin of The Broken Seal's CharacterMenu_SelectOwner.
s32 Func_080fe580(s32 slot) {
  struct OwnerMenuState *menu;
  struct OwnerCursor *cursor;
  s32 result;
  s32 index;

  menu = (struct OwnerMenuState *)Ram_HeapSlots->menuRuntime;
  index = menu->ownerIndex[slot];
  result = 0;
  cursor = menu->ownerCursors[slot];
  cursor->active = 1;
  cursor->animStep = 0;
  WindowSprite_RedrawSavedRectFar(menu->ownerWindow);
  if (GameFlag_Test(0x172))
    Window_DrawDividerLineFar(menu->ownerWindow, 9, 1, 9, 3);
  Window_DrawDividerLineFar(menu->ownerWindow, 0, 3, 12, 3);
  if (index == -1)
    menu->ownerIndex[slot] = 0;
  else
    Func_080f8ab4(index * 24 - 10, 16);
  if (menu->page == 3)
    result = Func_080feb58();
  else
    result = Func_080fe894();
  cursor = menu->ownerCursors[slot];
  Icon_PrepareActor(cursor);
  wait(1);
  return result;
}
