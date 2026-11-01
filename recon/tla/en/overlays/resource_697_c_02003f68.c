// Draft, differ 209 of 252, skeleton gap 51 (tla-en, 2026-11-01): 27 listing
// instructions missing, 24 extra. What is left is a PRE load and allocation.
// Found this round, each measured (319 to 209): the mover pointer is a real
// variable, declared first with i, tile, frame, count, viewX, viewZ, viewY in
// that order (the sp offsets 32 down to 4); the cap's y is one expression;
// word is gone. The header's old array form cannot give the pointer sp+32.
// With a real pointer GCSE's PRE reads offset once and carries it round the
// inner loop (the ldrh before the loop and at its end). The native rereads it
// at the latch test and after the loop, as in the source. The ((s16 *)mover)[2]
// read of the cap's height stops PRE after the loop. An empty asm with a memory
// clobber at the top of the inner body and just after the loop stops it in
// both places and gives skeleton gap 35 (differ 207), but reached no match, so
// it came back out; nothing plain has been found for the latch read yet.
// Allocation with that barrier: x r7 is right, but sy r5, z r6, height r4 by
// caller-save (sp+0 is its slot, so the frame is 36 as in the native), object
// r8, lift r10, viewY r11; the native has z r5, sy r6, height r8, object r10,
// entry r9, lift r11, viewY on the stack. In the cap the native holds x >> 16
// in r10. Tried and worse: cap height, z or y as their own variables (the
// loop's then go low), a separate sx or dy, masks inside the expression, x
// >>= 16 in place, a named x >> 16, other orders of the cap's statements. attr
// is set twice to keep tile + frame from hoisting (without it 199, gap 59) and
// needs judging against S2 before this adopts.
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
  s32 attr;
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
    for (lift = 0; lift < mover->offset; lift += 16) {
      if (count > 79)
        continue;
      x = object->x - viewX;
      height = object->y + (lift << 16) - viewY;
      z = object->z - viewZ - viewY;
      x = (x >> 16) - 8;
      sy = z - height;
      sy = (sy >> 16) - 16;
      depth = ((height + z) >> 16) + 58;
      if ((u32)(x + 15) > 334 || sy <= -16 || sy > 239)
        continue;
      x &= 0x1ff;
      sy &= 0xff;
      cursor = entry;
      *cursor++ = 0;
      *cursor++ = sy | (x << 16) | 0x40002000;
      attr = tile + frame;
      attr |= 0x800;
      *cursor = attr;
      Func_02005790(entry, depth);
      count++;
      entry += 3;
    }
    if (count > 79)
      continue;
    x = object->x - viewX;
    // Through s16 so the read is not merged with the loop test.
    height = object->y + (((s16 *)mover)[2] << 16) - viewY;
    z = object->z - viewZ - viewY;
    sy = z - height;
    bob = Iwram_MulQ16(rise, Math_Sine(mover->bob));
    x = (x >> 16) - 8;
    sy = ((sy + bob + rise) >> 16) - 4;
    depth = ((height + z) >> 16) + 58;
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
