// Draft, differ 187 of 252, skeleton gap 29 (tla-en, 2026-11-01): 17 listing
// instructions missing, 12 extra, no device left. Removed this round: attr set
// twice (the inner loop is now labels and goto, which keeps tile + frame in the
// body by itself) and the s16 cast read of the cap's height. Found, each
// measured (gap 51 to 29): in the cap x shifts straight after its subtraction,
// and depth comes before sy.
// Left, two causes, neither reached in plain C.
// 1. PRE (gcse.c, the .07.gcse dump): mover->offset's (mem:HI) at the latch is
// redundant with itself round the back edge on every path that skips the call,
// so PRE moves it after the call and into the preheader and the cap's read
// goes too. That is the ldrh before the loop and at its end, where the native
// rereads with ldrsh at the latch and after the loop, with no store or set of
// mover on those paths. Any plain read has this: only a kill stops it (an asm
// memory clobber does, volatile does not fit). The native keeping it may be a
// question about the pass, not this source; the same listing is in 66b's
// Func_02000dac and 661's Func_02003b80, and that is a matter for Pascal (K3).
// 2. Allocation: height takes r4 by caller-save (15 refs, 2 calls crossed), so
// object r8, lift r10, viewY r11, x r5, z r7. The native has z r5, sy r6, x r7,
// height r8, object r10, entry r9, lift r11, viewY on the stack. Its frame is
// 36 with sp+0 never used, as if a caller-save slot was set up for r4 and the
// pseudo left it during reload; unproved. With height pinned to r8 and barriers
// as a probe (not kept) gap is 25 and the frame 32, with y separate from sy.
// Tried, no better: height, z and y as inner-block variables per region (they
// go low, so the native's are shared; gap 97); y as its own variable in the loop
// (same gap), the cap or both; for instead of goto (gap 56); x as one
// expression; x shifted later in the cap; z before height; entry[n] for cursor.
// Draws the Trial Road movers' columns: for each raised mover (of 16) it
// queues one 16 by 16 sprite per 16 pixels of its offset and a bobbing cap on
// top, animated over 4 frames every 3, projected from the map's view and kept
// to the screen, at most 80 sprites (Func_02005790 queues each three-word
// entry at its depth, as Trial Road's cell sprite does).
#include "../../../../games/tla/src/scene/trial_road/trial_road.h"
#include "iwram_call.h"
#include "frame.h"
#include "map.h"

struct ResourceTableEntry {
  u16 unk0;
  u16 offset;
};

struct TrialRoadMover {
  s16 id;
  s16 phase;
  s16 offset;
  s16 unk06[8];
  s16 bob;
  struct Actor *marker;
};

struct TrialRoadMovers {
  s16 vramEntry;
  s16 shape;
  struct TrialRoadMover movers[16];
  u32 *entries;
};

extern struct ResourceTableEntry gResourceTableEntries[];

s32 Math_Sine(s32 angle);
void Func_02005790(void *entry, s32 priority);

void Func_02003f68(void) {
  struct TrialRoadMovers *base =
      (struct TrialRoadMovers *)(gData_020023c4 + 0x188);
  struct TrialRoadMover *mover = base->movers;
  s32 i;
  s32 tile;
  s32 frame;
  s32 count;
  s32 viewX;
  s32 viewZ;
  s32 viewY;
  s32 rise;
  struct MapWork *map;
  s32 *view;
  struct Actor *object;
  u32 *entry;
  u32 *cursor;
  s32 lift;
  s32 x;
  s32 height;
  s32 z;
  s32 sy;
  s32 depth;
  s32 bob;

  tile = gResourceTableEntries[base->vramEntry].offset >> 5;
  entry = base->entries;
  count = 0;
  rise = -0x20000;
  map = Ram_HeapSlots->mapWork;
  view = &map->viewX;
  viewX = view[0] & 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  viewY = map->origin[1];

  for (i = 0; i < 16; i++, mover++) {
    if (mover->id == 0 || mover->offset == 0)
      continue;
    object = Actor_GetById(mover->id);
    frame = ((gFrameCount % 12) / 3) * 8 + 32;
    lift = 0;
    if (lift < mover->offset) {
    next:
      if (count > 79)
        goto skip;
      x = object->x - viewX;
      height = object->y + (lift << 16) - viewY;
      z = object->z - viewZ - viewY;
      x = (x >> 16) - 8;
      sy = z - height;
      sy = (sy >> 16) - 16;
      depth = ((height + z) >> 16) + 58;
      if ((u32)(x + 15) > 334 || sy <= -16 || sy > 239)
        goto skip;
      x &= 0x1ff;
      sy &= 0xff;
      cursor = entry;
      *cursor++ = 0;
      *cursor++ = sy | (x << 16) | 0x40002000;
      *cursor = (tile + frame) | 0x800;
      Func_02005790(entry, depth);
      count++;
      entry += 3;
    skip:
      lift += 16;
      if (lift < mover->offset)
        goto next;
    }
    if (count > 79)
      continue;
    x = object->x - viewX;
    x = (x >> 16) - 8;
    height = object->y + (mover->offset << 16) - viewY;
    z = object->z - viewZ - viewY;
    sy = z - height;
    bob = Iwram_MulQ16(rise, Math_Sine(mover->bob));
    depth = ((height + z) >> 16) + 58;
    sy = ((sy + bob + rise) >> 16) - 4;
    if ((u32)(x + 15) > 334 || sy <= -16 || sy > 239)
      continue;
    x &= 0x1ff;
    sy &= 0xff;
    cursor = entry;
    *cursor++ = 0;
    *cursor++ = sy | (x << 16) | 0x40002000;
    *cursor = (tile + frame - 32) | 0x800;
    Func_02005790(entry, depth);
    count++;
    entry += 3;
  }
}
