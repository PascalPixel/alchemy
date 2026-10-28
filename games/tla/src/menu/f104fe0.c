#include "types.h"
#include "ram_buffer.h"
#include "task.h"

struct MenuEntryWindow {
  u8 unk00[12];
  s16 x;
  s16 y;
};

struct MenuEntryWork {
  u8 unk000[0xf8];
  void *objects[8];
  s16 x[8];
  s16 y[8];
  s32 scale[8];
  u8 unk158[0x509 - 0x158];
  u8 active;
};

void Func_08104ef8(s32 first);
void Menu_UpdateEntryObjectTransforms(void);

void Menu_PlaceOwnerEntryObjects(struct MenuEntryWindow *window, s32 originX,
                                 s32 originY) {
  struct MenuEntryWork *menu = Ram_HeapSlots->menuRuntime;
  s32 i;

  for (i = 7; i >= 0; i--)
    menu->objects[i] = NULL;
  Func_08104ef8(0);
  for (i = 0; i <= 3; i++) {
    if (menu->objects[i] != NULL) {
      menu->x[i] = (i * 3 + (window->x + originX)) * 8;
      menu->y[i] = (window->y + originY) * 8 + 16;
      menu->scale[i] = 0x10000;
    }
  }
  menu->active = 1;
  Task_Add(Menu_UpdateEntryObjectTransforms, TASK_PASS_VBLANK);
}
