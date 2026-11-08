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
};

void ResourceObject_ReleaseFar(void *object);
void MenuIcon_UpdateTransforms(void);

void MenuIcon_ReleaseObjects(void) {
  struct MenuIconWork *menu = Ram_HeapSlots->menuRuntime;
  s32 i;

  for (i = 0; i < 4; i++) {
    if (menu->objects[i] != NULL) {
      ResourceObject_ReleaseFar(menu->objects[i]);
      menu->objects[i] = NULL;
    }
  }
  Task_Remove(MenuIcon_UpdateTransforms);
}

extern s32 gMenuIcon_ResourceIds[];
void ResourceObject_ReleaseFar(void *object);
void *ResourceObject_CreateFar(s32 resource);
void Animation_ApplyChildArgumentFar(void *object, s32 argument);

void MenuIcon_ReplaceObject(s32 index, s32 kind, s32 animation) {
  struct MenuIconWork *menu = Ram_HeapSlots->menuRuntime;
  s8 *object;

  if (menu->objects[index] != NULL) {
    ResourceObject_ReleaseFar(menu->objects[index]);
    menu->objects[index] = NULL;
  }
  object = ResourceObject_CreateFar(gMenuIcon_ResourceIds[kind]);
  if (object != NULL) {
    Animation_ApplyChildArgumentFar(object, animation);
    object[9] &= -13;
  }
  menu->objects[index] = object;
}

void MenuIcon_SetPosition(s32 index, s32 x, s32 y) {
  struct MenuIconWork *menu = Ram_HeapSlots->menuRuntime;

  /* FAKEMATCH: ⚓️ loads the menu work before it builds the field offset */
  __asm__ volatile("" : "+l"(menu));

  if (menu->objects[index] != NULL) {
    menu->x[index] = x;
    menu->y[index] = y;
  }
}

void MenuIcon_SetPicture(s32 index, s32 frame) {
  struct MenuIconWork *menu = Ram_HeapSlots->menuRuntime;

  /* FAKEMATCH: ⚓️ loads the menu work before it builds the field offset */
  __asm__ volatile("" : "+l"(menu));

  if (menu->objects[index] != NULL)
    menu->frame[index] = frame;
}

void Func_08105350(s32 index, s32 scale) {
  struct MenuIconWork *menu = Ram_HeapSlots->menuRuntime;

  /* FAKEMATCH: the ARM9 scheduling would scale the index before loading the
   * menu work. */
  __asm__("");
  if (menu->objects[index] != NULL)
    menu->scale[index] = scale;
}
