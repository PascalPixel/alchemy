#include "types.h"
#include "ram_buffer.h"
#include "sprite.h"

struct FigureMenuState {
  u8 unk000[0xf8];
  struct AnimationObject *figures[8];
  u8 unk118[0x40];
  s16 ids[8];
};

s32 Func_080c82b8(s32 id);
struct AnimationObject *ResourceObject_CreateFar(s32 resource);
void ResourceObject_ReleaseFar(struct AnimationObject *object);
void Animation_ApplyChildArgumentFar(void *object, s32 argument);

void Func_08104e38(s32 first, s32 second) {
  struct FigureMenuState *menu =
      (struct FigureMenuState *)Ram_HeapSlots->menuRuntime;
  struct AnimationObject *figure;
  s32 id;
  s32 i;

  for (i = 0; i < 8; i++) {
    figure = menu->figures[i];
    if (figure != NULL) {
      ResourceObject_ReleaseFar(figure);
      menu->figures[i] = NULL;
      menu->ids[i] = -1;
    }
  }
  for (i = 0; i < 2; i++) {
    id = first;
    if (i != 0)
      id = second;
    if (id != -1) {
      menu->ids[i] = id;
      figure = ResourceObject_CreateFar(Func_080c82b8(id));
      if (figure != NULL) {
        Animation_ApplyChildArgumentFar(figure, 1);
        figure->image.priority = 0;
        ((u8 *)figure)[26] = 0;
      }
      menu->figures[i] = figure;
    }
  }
}
