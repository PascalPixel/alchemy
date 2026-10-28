// Measured 2026-10-26 under the Makefile's options (-mthumb-slow-loads), differ
// 4 of 91 in tla-en. The native spills its arguments to the stack before it
// builds the heap slots' address; this builds the address first (row 8). No
// single empty asm or one-pass block reaches 0.
// First draft of Func_08104e38: replaces the menu's two figures: releases
// all eight figure objects of the menu work (clearing their ids to -1), then
// creates a figure for first and for second (each unless -1) from the
// resource Func_080c82b8 gives for it, at animation 1 and priority 0. Near
// miss, differ 6 of 91, instruction order only: the native spills first to
// the stack before it builds the work address (here after its first two
// instructions), and copies the cleared word's zero from r9 before reloading
// the spilled id address (here after).
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
