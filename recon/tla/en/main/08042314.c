// Draft of RenderOutput_Create: differ 4 of 63 (tla-en, 2026-10-18). Its ☀️
// twin is games/tbs/src/window/battle_command.c's RenderOutput_Create: it
// takes a free window output, writes its OAM attributes at the window's
// position and appends it to the window's list.
//
// Left: instruction order only (a register copy and the y store each sit one
// place apart).

#include "render_input.h"
#include "output.h"
#include "vram_tab.h"
#include "resource.h"

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
  /* FAKEMATCH: ⚓️ finishes the stores before it passes the list */
  asm volatile("");
  RenderOutput_AppendToList((struct RenderOutputList *)input, output);
  return output;
}
