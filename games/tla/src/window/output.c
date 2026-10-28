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
#include "types.h"
#include "ram_buffer.h"
#include "resource.h"
#include "window.h"
#include "stddef.h"

s32 Portrait_GetForSpeaker(s32 key);
void UiGlyph_LoadEntryWithPalette(u32 icon, s32 unused, s32 *slot, s32 *tile,
                                  s32 palette, s32 reuse);

struct WindowSprite *WindowSprite_CreateWithTransform(s32 arg0,
                                                      struct RenderInput *arg1,
                                                      s32 arg2, s32 arg3) {
  s32 count;
  s32 unused;
  struct WindowSprite *result;

  count = Resource_FindFreeEntry();
  if (count == 0x60) {
    return NULL;
  }
  Icon_BuildCommandIconTiles(arg0, 1, &count, &unused, 1);
  result = WindowSprite_Create(count, OUTPUT_SIZE_16X16, arg1, arg2, arg3);
  result->sentinel = 251;
  return result;
}

/// Loads the character entry for the key into the given side, or into a free
/// one (resource id 999) when the side is 2 or more, and creates its output
/// with that side's palette (14 + side).
struct WindowSprite *CreateSideObject(s32 objectKind, s32 position, s32 side,
                                      s32 arg3, s32 arg4, s32 arg5) {
  struct WindowWork *state =
      (struct WindowWork *)Ram_HeapSlots->windowTiles;
  struct WindowSprite *object = 0;
  s32 first;
  s32 second;
  s32 id;
  s32 slot;

  id = Portrait_GetForSpeaker(objectKind);
  if (id == -1)
    return object;

  if ((u32)side > 1) {
    u32 current;

    current = state->resourceIds[1];
    /* FAKEMATCH: the barrier loads the second side's id before 999 is
       built and the side is set; plain, both come first (one reordered
       row, differ 2). */
    asm("");
    side = 1;
    if (current != 999) {
      side = 0;
      if (state->resourceIds[0] != 999)
        return object;
    }
  }

  slot = 14 + side;
  UiGlyph_LoadEntryWithPalette(id, position, &first, &second, slot, 0);
  object = WindowSprite_Create(first, OUTPUT_SIZE_32X32,
                               (struct RenderInput *)arg3, arg4, arg5);
  if (object != 0) {
    object->oam.palette = slot;
    object->kind = 2;
  }

  state->resourceIds[side] = id;
  state->outputIds[side] = first;
  return object;
}

/// Reloads the portrait picture for the key into a portrait slot with that
/// slot's palette (14 + slot), reusing the slot's VRAM cache entry. A slot of
/// 2 or more means whichever slot already shows the picture, the second
/// first; when neither does, nothing is loaded.
void Portrait_Reload(u32 slot, s32 key, s32 unused) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  s32 id = Portrait_GetForSpeaker(key);
  s32 offset;
  s32 palette;
  s32 cached;
  s32 tile;
  s32 entry;

  if (id == -1)
    return;
  if (slot > 1) {
    if (work->resourceIds[1] == id)
      slot = 1;
    else if (work->resourceIds[0] == id)
      slot = 0;
    else
      return;
  }
  offset = offsetof(struct WindowWork, outputIds);
  palette = slot;
  cached = *(u16 *)((u8 *)work + (slot * 2 + offset));
  /* FAKEMATCH(barrier): the palette's copy of the slot and the offset's
   * shift wait for the cache entry's load; as entry = work->outputIds[slot],
   * 2 of 48 instructions differ in tla-en. */
  __asm__("" : "+l"(palette) : "l"(cached), "l"(offset));
  entry = cached;
  UiGlyph_LoadEntryWithPalette(id, unused, &entry, &tile, palette + 14, 1);
}
