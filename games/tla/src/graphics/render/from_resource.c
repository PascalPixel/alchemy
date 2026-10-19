#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "render_input.h"
#include "output.h"
#include "vram_tab.h"
#include "resource.h"

/// Takes a free window output, writes its OAM attributes at the window's
/// position offset by (offsetX, offsetY) pixels and appends it to the window's
/// list, as ☀️'s RenderOutput_Create. Releases the resource slot and returns
/// NULL when no output is free.
struct RenderOutput *RenderOutput_Create(s32 slot, s32 flags,
                                         struct RenderInput *input, s32 offsetX,
                                         s32 offsetY) {
  s32 x;
  struct RenderOutput *output;
  s32 y;

  output = RenderOutput_AcquireFree();
  if (output == NULL) {
    Resource_ResetEntry((u32)slot);
    return NULL;
  }
  x = offsetX + (s16)input->x * 8 + 8;
  y = offsetY + (s16)input->y * 8 + 8;
  x &= 0x1ff;
  y &= 0xff;
  *(u32 *)&output->oam = (x << 16) | y | flags;
  ((u32 *)&output->oam)[1] = gResourceTableEntries[slot].offset >> 5;
  output->sentinel = 254;
  output->x = x;
  output->y = (s16)y;
  output->next = NULL;
  output->index = (s8)slot;
  output->kind = 1;
  output->active = 1;
  RenderOutput_AppendToList((struct RenderOutputList *)input, output);
  /* FAKEMATCH: the barrier after the call keeps the list argument's copy
   * (mov r0, r8) after the last stores; without it the copy comes three
   * stores earlier (differ 2). */
  __asm__ volatile("");
  return output;
}

struct RenderOutput *RenderOutput_CreateFromResource(s32 arg0, s32 arg1,
                                                     struct RenderInput *arg2,
                                                     s32 arg3, s32 arg4) {
  s32 no;

  no = Resource_LoadByMode(arg0, arg1);
  if (no < 0) {
    return NULL;
  }
  return RenderOutput_Create(no, 0x40000000, arg2, arg3, arg4);
}
