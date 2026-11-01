// Draft, differ 49 of 83, skeleton gap 21 (tla-en, 2026-11-01): Func_02004254,
// the start of overlay 6A8's falling rocks; Func_02004114, the update, is at
// differ 6 of 57, skeleton gap 6. Diff each with --symbol (the spawn between
// them, Func_02004198, is adopted as prox/s4198.c). The work is overlay 6B9's
// SparkWork: 0x500 bytes of tiles, 64 rocks of 28 bytes, 64 sprites of 40
// (struct EffectSprite; writing its OAM bits through the header's RenderOam
// rather than a local struct took the start from differ 83 to 49), then next,
// tileBase and resource.
// Start: every remaining difference follows from the count. The native keeps
// it in r8 and the tile in r10; here the count takes r4, saved around the call,
// and the tile r8. The count has 7 weighted references and crosses one call, so
// global.c finds caller-saving r4 profitable (4 * 1 < 7) before it tries the
// high registers; with the count high, reload would build the -1 it shares
// with the timer and rematerialize the zero of next after the loop, as the
// native does. Not settled by: i += -1 at any place in the body, --i in the
// test, a copy for the test, register, a do loop (hoists -13). A zero pseudo
// of higher priority takes r2, not r4. A way to give the count two crossed
// calls or at most 4 references would settle it.
// Update: only the mask -1024 and the sprite's ldrh are swapped (the native
// ands into the halfword, this draft into the constant), and the pool load
// for Func_02004614 moves with them. The pointer, frame, mask and base locals
// put tileBase's load before the mask in r2 with the timer in r3; a tile local,
// a u16 base or swapped operands do not move the pair.
// The native rebuilds every offset and mask inside each loop; for loops hoist
// them (differ 60, 48, 84), so the loops are goto loops here. VramLoad as an
// inline keeps 0x500 from being reused for the rock offset.
#include "types.h"
#include "ram_buffer.h"
#include "effect_sprite.h"

struct Rock {
  s32 position[3];
  s32 velocity[3];
  s32 timer;
};

struct RockWork {
  u8 tiles[0x500];
  struct Rock rocks[64];
  struct EffectSprite sprites[64];
  s16 next;
  u16 tileBase;
  s16 resource;
};

extern char ResourceId_FallingRock;

s32 Random16Far(void);
void Vector_AddPolarOffsetFar(s32 radius, s32 angle, s32 *vector);
void *HeapSlot_AllocFar(s32 slot, s32 size);
void *Resource_GetTableEntry(s32 id);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(s32 entry, s32 size, void *data);
void Task_Add(void (*func)(void), s32 priority);
void Func_02004384(void *resource, void *destination);
void Func_02004604(struct EffectSprite *sprite, s32 a, s32 b, s32 c, s32 tile);
void Func_0200460c(struct EffectSprite *sprite, struct Rock *rock);
void Func_02004614(struct Rock *rock, s32 a, s32 b);

static __inline__ s32 VramLoad(s32 entry, s32 size, void *data) {
  /* FAKEMATCH: as in the scene layer, the inline parameters keep the size its
   * own constant; called directly, the rock offset reuses it across the call.
   */
  return VramCache_Load(entry, size, data);
}

void Func_02004114(void) {
  struct RockWork *work = Ram_HeapSlots->menuRuntime;
  struct Rock *rock = work->rocks;
  struct EffectSprite *sprite = work->sprites;
  s32 i;

  /* FAKEMATCH: the loop is written with goto so that it carries no loop notes:
   * the bytes rebuild the constants on every pass and count in r8, and a for
   * loop hoists them. */
  i = 63;
loop:
  {
    s32 timer = rock->timer;

    if ((u32)timer <= 19) {
      u16 *tileBase = &work->tileBase;
      s32 frame = timer >> 2;
      s32 mask = 7;
      s32 base = *tileBase;

      sprite->oam.index = base + (frame & mask) * 8;
      Func_0200460c(sprite, rock);
      Func_02004614(rock, 63, -0x8000);
      rock->timer++;
    }
    sprite++;
    rock++;
  }
  i--;
  if (i >= 0)
    goto loop;
}

void Func_02004254(void) {
  struct RockWork *work = HeapSlot_AllocFar(220, sizeof(struct RockWork));
  struct Rock *rock;
  struct EffectSprite *sprite;
  s32 entry;
  s32 tile;
  s32 i;

  Func_02004384(Resource_GetTableEntry((s32)&ResourceId_FallingRock), work);
  entry = Resource_FindFreeEntry();
  tile = VramLoad(entry, 0x500, work);
  work->tileBase = tile;
  work->resource = entry;
  rock = work->rocks;
  sprite = work->sprites;
  /* FAKEMATCH: the loop is written with goto so that it carries no loop notes:
   * the bytes rebuild the constants on every pass and count in r8, and a for
   * loop hoists them. */
  i = 63;
loop:
  {
    Func_02004604(sprite, 8, 8, 0x40000000, tile);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 0;
    sprite->depth = 240;
    i += -1;
    rock->timer = -1;
    sprite++;
    rock++;
  }
  if (i >= 0)
    goto loop;
  work->next = 0;
  Task_Add(Func_02004114, 0x480);
}
