// Measured 2026-10-26 under the Makefile's options (-mthumb-slow-loads), differ
// 4 of 84 in tla-en. First difference: this issues `ldr r4, [sp, #0]` earlier
// (row 20) than the native does (row 23). An empty asm after `menu->objects[i]
// = NULL;` measured 2. No single empty asm or one-pass block reaches 0.
// First draft of Func_081051a8: (re)creates the menu's four icon figures
// from gFourObjectMotion_ResourceIds at animation 2 and priority 0, each at
// (16, 200), scale 1.0 and frame 0x4000, then starts
// MenuIcon_UpdateTransforms. Beside MenuIcon_ReleaseObjects and
// MenuIcon_ReplaceObject in games/tla/src/menu/menu_icon_obj.c, whose work
// layout this uses. Near miss, differ 8 of 84, instruction order only: the
// native reloads the spilled counter after the clearing store (here before),
// loads the resource id table's address before the second loop's offsets
// (here after), and builds -13 after the byte load.
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

extern s32 gFourObjectMotion_ResourceIds[];
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
    object = ResourceObject_CreateFar(gFourObjectMotion_ResourceIds[i]);
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
