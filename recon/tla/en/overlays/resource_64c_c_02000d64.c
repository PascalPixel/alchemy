// Matches (tla-en, 2026-11-08, -mtune=gba): overlay 64b's Func_020036a8 form,
// carried over whole, with the same three devices to keep their S2 tags on
// adoption (work pinned to r9, empty asms on next and leader) and the same
// close: the flag test as one else-if chain with shown set in each arm, the
// cleared-flag arm read through an inline GameFlag_IsClear. See the twin's
// header for why and for each device's re-measure.
// Earlier: differ 6 of 260 (2026-11-07 and 2026-11-08), 63 (2026-11-07), 75
// (2026-11-06), 155 (2026-11-05).
// Overlay 64b's Func_020036a8 (the world map's placed objects, shown and
// hidden as the view moves) under this overlay's names: shows the objects
// that come within the view and whose flags allow them, hides those that
// leave it, and starts a placement's event when the leader touches it.
#include "types.h"
#include "call.h"
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

// Whether the flag is clear: an inline helper, which the match needs (see the
// header).
static inline s32 GameFlag_IsClear(s32 flag) { return !GameFlag_Test(flag); }

void Func_02000d64(void) {
  struct WorldMapPlacement *placement = Func_02002094();
  struct Actor *leader = Actor_GetById(Func_0200209c());
  s32 leaderReach =
      ((struct WorldMapSprite *)leader->animation)->scale * leader->radius;
  u32 i;
  s32 next;
  s32 left, right, top, bottom;
  struct Actor **slot;
  /* FAKEMATCH(pin): the form kept; as a plain local, 18 of 260 instructions
   * differ in tla-en. */
  register struct EventWork *work asm("r9") = Ram_HeapSlots->eventWork;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct Actor *object = work->viewCenter;
  s32 zoom = map->zoom;
  s32 x = object->x;
  u8 kind = placement->kind;
  s32 z = object->z;
  struct Actor **objects = gData_02003dc0;

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
  i = 0;
  next = kind;
  /* FAKEMATCH(barrier): the form kept; without it, 7 of 260 instructions
   * differ in tla-en. */
  asm("" : "+g"(next));
  slot = objects;
  /* FAKEMATCH(barrier): the form kept; without it, 16 of 260 instructions
   * differ in tla-en. */
  asm("" : "+g"(leader));
  while (next != 0) {
    s32 px, pz;

    if (placement->unk01 == 15)
      goto next;
    object = *slot;
    px = placement->x << 16;
    pz = placement->z << 16;
    if (px > left && px < right && pz > top && pz < bottom) {
      s32 flag = placement->flag;
      s32 shown;

      if (flag == -1)
        shown = 1;
      else if (flag & 0x1000)
        shown = GameFlag_Test(flag);
      else
        shown = GameFlag_IsClear(flag);
      if (shown) {
        if (object == NULL) {
          struct WorldMapSprite *sprite;

          object = Func_02001fcc(placement->sprite, px, 0, pz);
          if (object == NULL)
            goto next;
          Func_02001fc4(object, gData_02003da8);
          object->motionFlags = 0;
          object->collisionFlags = 1;
          object->angle = placement->angle;
          Func_02001fbc(object, 1);
          sprite = object->animation;
          sprite->scale = Iwram_MulQ16(sprite->scale, 0xc000);
          *slot = object;
        }
        if (gMainRoutineWork.ownerOverride == 0) {
          s32 scale = ((struct WorldMapSprite *)object->animation)->scale;
          s32 dx;
          s32 reach, dz;

          dx = object->x - leader->x;
          if (dx < 0)
            dx = leader->x - object->x;
          reach = leaderReach + object->radius * scale;
          dz = object->z - leader->z;
          if (dz >= 0 ? dx + dz < reach
                      : dx + (leader->z - object->z) < reach) {
            if (GameFlag_Test(0x104) == 0) {
              if (gMainRoutineWork.renderMode == 9)
                work->unk166 = 0x208b;
              else
                work->exit = i + 100;
            }
          }
        }
        goto next;
      }
    }
    if (object != NULL) {
      Func_02001fd4(object);
      *slot = NULL;
    }
  next:
    i++;
    slot++;
    placement++;
    if (i > 127)
      break;
    next = placement->kind;
  }
}
