// Near miss, differ 253 of 260 (tla-en, 2026-10-31): register allocation.
// The control flow, calls, field widths and constants follow the native; the
// native ranks leader (r10) and work (r9) above slot (r11) and spills i, where
// this compiler gives slot r10, i r9 and leader r11 and spills work, which
// moves nearly every register. The native also loads the first placement's
// kind before the view bounds' if and copies it at the loop's first test.
// Shows the world map's placed objects that come within the view and whose
// flags allow them, hides those that leave it, and starts a placement's
// event when the leader touches it. Overlay 64c has the same code (its
// Func_02000d64). Measured: zoom read as MapWork.zoom; the leader's reach as
// scale * radius (radius * scale 257); the flag in an s32 local (as the
// field read four times 287); gMainRoutineWork through a local pointer, so
// its fields take the native's base plus built offset; gData_02006c08 in a
// local before the if copied into slot at the loop start, as the native's
// pool load before the if and copy after it (slot set before the if also
// 253); indexing placements[i] and gData_02006c08[i] 262; a kind local read
// before the if and reused by the first test 257; slot initialized at its
// declaration (281, before this round), a local party pointer (289).
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

extern struct Actor *gData_02006c08[];
extern u8 gData_02006bd4[];

struct WorldMapPlacement *Func_02004e08(void);
s32 Func_02004e10(void);
struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
struct Actor *Func_02004b30(s32 sprite, s32 x, s32 y, s32 z);
void Func_02004b20(struct Actor *object, void *dispatch);
void Func_02004b18(struct Actor *object, s32 mode);
void Func_02004b38(struct Actor *object);

void Func_020036a8(void) {
  struct WorldMapPlacement *placement = Func_02004e08();
  struct Actor *leader = Actor_GetById(Func_02004e10());
  s32 leaderReach =
      ((struct WorldMapSprite *)leader->animation)->scale * leader->radius;
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *center = work->viewCenter;
  s32 x = center->x;
  s32 z = center->z;
  s32 left, right, top, bottom;
  u32 i;
  struct Actor **slot;
  struct Actor **objects = gData_02006c08;
  struct MainRoutineWork *routine = &gMainRoutineWork;

  if (Ram_HeapSlots->mapWork->zoom <= 0x10000) {
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

          object = Func_02004b30(placement->sprite, px, 0, pz);
          if (object == NULL)
            continue;
          Func_02004b20(object, gData_02006bd4);
          object->motionFlags = 0;
          object->collisionFlags = 1;
          object->angle = placement->angle;
          Func_02004b18(object, 1);
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
      Func_02004b38(object);
      *slot = NULL;
    }
  }
}
