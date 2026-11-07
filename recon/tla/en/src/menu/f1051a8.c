#include "types.h"
#include "ram_buffer.h"
#include "task.h"

struct MenuIconWork {
  u8 unk000[0x220];
  void *objects[4];
  s16 x[4];
  s16 y[4];
  s32 scale[4];
  s16 frame[4];
  u8 unk258[0x2b2];
  s8 active;
};

extern s32 gMenuIcon_ResourceIds[];
void ResourceObject_ReleaseFar(void *object);
void *ResourceObject_CreateFar(s32 resource);
void Animation_ApplyChildArgumentFar(void *object, s32 argument);
void MenuIcon_UpdateTransforms(void);

void Func_081051a8(void) {
  struct MenuIconWork *menu = Ram_HeapSlots->menuRuntime;
  s8 *object;
  s32 i;

  for (i = 0; i < 4; i++) {
    if (menu->objects[i] != NULL) {
      ResourceObject_ReleaseFar(menu->objects[i]);
      menu->objects[i] = NULL;
    }
  }
  for (i = 0; i < 4; i++) {
    object = ResourceObject_CreateFar(gMenuIcon_ResourceIds[i]);
    if (object != NULL) {
      Animation_ApplyChildArgumentFar(object, 2);
      object[9] &= -13;
    }
    menu->objects[i] = object;
    menu->x[i] = 16;
    menu->y[i] = 200;
    menu->scale[i] = 0x10000;
    menu->frame[i] = 0x4000;
  }
  menu->active = 1;
  Task_Add(MenuIcon_UpdateTransforms, TASK_PASS_VBLANK);
}
