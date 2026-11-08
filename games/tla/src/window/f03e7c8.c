#include "types.h"
#include "resource.h"
#include "select.h"

extern u8 gData_0805c5c4[];
extern u8 gData_0805c1c4[];

void Func_0803e7c8(struct SelectionScreen *state, s32 index) {
  struct SelectionSprite *entry = &state->records[index].sprite;
  u8 *frames = 0;
  u32 count;

  state->records[index].kind = 0;
  if (index != 0) {
    /* FAKEMATCH(temporary): the tile pointer in a local, loaded early into r5;
     * as gData_0805c5c4 where right is read, 56 of 140 instructions differ in
     * tla-en. */
    u8 *right = gData_0805c5c4;

    count = state->count;
    if (state->top != 0)
      count -= state->top;
    if (count > 5) {
      state->records[index].kind = 1;
      count = 5;
    }
    frames = right;
    state->records[1].x = state->baseX + 16 * (count - 1) + 17;
  } else {
    frames = gData_0805c1c4;
    state->records[0].x = state->baseX - 9;
    if (state->top != 0)
      state->records[0].kind = 1;
  }
  if (state->records[index].y == 0) {
    state->records[index].slot = Resource_FindFreeEntry();
    state->records[index].tile =
        VramCache_Load(state->records[index].slot, 128, frames);
    state->records[index].y = state->baseY;
    state->records[index].base = 0;
    entry->mode = 0;
    entry->mosaic = 0;
    entry->colors = 1;
    entry->affine = 0;
    entry->param = 0;
    entry->size = 0;
    entry->shape = 2;
    entry->priority = 0;
  }
}
