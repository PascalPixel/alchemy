// Draft, differ 279 of 260, skeleton gap 43 (tla-en, 2026-11-01)
// Overlay 64b's Func_020036a8 (the world map's placed objects, shown and
// hidden as the view moves) under this overlay's names: shows the objects
// that come within the view and whose flags allow them, hides those that
// leave it, and starts a placement's event when the leader touches it.
// Left: the whole gap is the spill. The native spills i and keeps work in r9,
// with leader r10, slot r11; here global.c ranks i (refs 9, length 228) above
// leader and work (6 refs, length 134), so work spills. Reading the dumps,
// the native's i must have 7 refs: its bottom test compares a value that is
// not i. Measured, not kept, because each is a device this round forbids:
// goto loop with `u32 next = i + 1; i = next;` tested at the bottom (a
// `temporary`) flips the spill, differ 211, gap 23; the first-entry copy of
// kind comes from a pre-if `u8 kind = placement->kind;` overwritten by
// `(kind = placement->kind) != 0` in the for test (a `dead` store), which
// makes the native's load and copy (gcse), gap 48 alone. Together they are
// the likely way to skeleton 0. No plain form of i was found: signed i with a
// cast, i <= 127, objects[i] (not reduced at every site, gap 71), and a
// one-pass block (no change in refs) all fail; one-pass and objects[i] worse.
// The head's order (zoom in a local after center, objects after z) took the
// gap from 59 to 43; zoom after z 47, zoom before center 47.
#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "map.h"
#include "actor.h"
#include "event_work.h"
#include "iwram_call.h"

/// One of the scene's placed objects, 20 bytes; a zero kind ends the list.
struct WorldMapPlacement {
  u8 kind;
  u8 unk01; ///< 15 skips it
  s16 sprite;
  s16 x; ///< pixels
  s16 z; ///< pixels
  u16 angle;
  s16 flag; ///< -1 always shown; bit 12 shown while set, else while clear
  u8 unk0c[8];
};

struct WorldMapSprite {
  u8 unk00[0x0c];
  s32 scale;
};

extern struct Actor *gData_02003dc0[];
extern u8 gData_02003da8[];

struct WorldMapPlacement *Func_02002094(void);
s32 Func_0200209c(void);
struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
struct Actor *Func_02001fcc(s32 sprite, s32 x, s32 y, s32 z);
void Func_02001fc4(struct Actor *object, void *dispatch);
void Func_02001fbc(struct Actor *object, s32 mode);
void Func_02001fd4(struct Actor *object);

void Func_02000d64(void) {
  struct WorldMapPlacement *placement = Func_02002094();
  struct Actor *leader = Actor_GetById(Func_0200209c());
  s32 leaderReach =
      ((struct WorldMapSprite *)leader->animation)->scale * leader->radius;
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *center = work->viewCenter;
  s32 zoom = Ram_HeapSlots->mapWork->zoom;
  s32 x = center->x;
  s32 z = center->z;
  struct Actor **objects = gData_02003dc0;
  s32 left, right, top, bottom;
  u32 i;
  struct Actor **slot;
  struct MainRoutineWork *routine = &gMainRoutineWork;

  if (zoom <= 0x10000) {
    left = x - 0xa00000;
    right = x + 0xa00000;
    top = z - 0x12c0000;
    bottom = z + 0xc80000;
  } else {
    left = x - 0xf00000;
    right = x + 0xf00000;
    top = z - 0x1c20000;
    bottom = z + 0x12c0000;
  }
  for (i = 0, slot = objects; i < 128 && placement->kind != 0;
       i++, slot++, placement++) {
    struct Actor *object;
    s32 px, pz;

    if (placement->unk01 == 15)
      continue;
    object = *slot;
    px = placement->x << 16;
    pz = placement->z << 16;
    if (px > left && px < right && pz > top && pz < bottom) {
      s32 shown = 1;
      s32 flag = placement->flag;

      if (flag != -1) {
        if (flag & 0x1000)
          shown = GameFlag_Test(flag);
        else
          shown = !GameFlag_Test(flag);
      }
      if (shown) {
        if (object == NULL) {
          struct WorldMapSprite *sprite;

          object = Func_02001fcc(placement->sprite, px, 0, pz);
          if (object == NULL)
            continue;
          Func_02001fc4(object, gData_02003da8);
          object->motionFlags = 0;
          object->collisionFlags = 1;
          object->angle = placement->angle;
          Func_02001fbc(object, 1);
          sprite = object->animation;
          sprite->scale = Iwram_MulQ16(sprite->scale, 0xc000);
          *slot = object;
        }
        if (routine->ownerOverride == 0) {
          s32 scale = ((struct WorldMapSprite *)object->animation)->scale;
          s32 dx = object->x - leader->x;
          s32 reach, dz;

          if (dx < 0)
            dx = leader->x - object->x;
          reach = leaderReach + object->radius * scale;
          dz = object->z - leader->z;
          if (dz >= 0 ? dx + dz < reach
                      : dx + (leader->z - object->z) < reach) {
            if (GameFlag_Test(0x104) == 0) {
              if (routine->renderMode == 9)
                work->unk166 = 0x208b;
              else
                work->exit = i + 100;
            }
          }
        }
        continue;
      }
    }
    if (object != NULL) {
      Func_02001fd4(object);
      *slot = NULL;
    }
  }
}
