/// @file
/// Shows the world map's placed objects that come within the view and whose
/// flags allow them, hides those that leave it, and starts a placement's
/// event when the leader touches it. Overlay 64c keeps the same code.

#include "types.h"
#include "call.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "map.h"
#include "actor.h"
#include "event_work.h"
#include "iwram_call.h"
#include "flag_call.h"

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
struct Actor *Func_02004b30(s32 sprite, s32 x, s32 y, s32 z);
void Func_02004b20(struct Actor *object, void *dispatch);
void Func_02004b18(struct Actor *object, s32 mode);
void Func_02004b38(struct Actor *object);

void Func_020036a8(void) {
  struct WorldMapPlacement *placement = Func_02004e08();
  struct Actor *leader = Actor_GetById(Func_02004e10());
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
  struct Actor **objects = gData_02006c08;

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
  /* FAKEMATCH(barrier): the form kept; as no asm, 7 of 260 instructions
   * differ in tla-en. */
  asm("" : "+g"(next));
  slot = objects;
  /* FAKEMATCH(barrier): the form kept; as no asm, 16 of 260 instructions
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
        shown = Flag_IsClear(flag);
      if (shown) {
        if (object == NULL) {
          struct WorldMapSprite *sprite;

          object = Func_02004b30(placement->sprite, px, 0, pz);
          if (object == NULL)
            goto next;
          Func_02004b20(object, gData_02006bd4);
          object->motionFlags = 0;
          object->collisionFlags = 1;
          object->angle = placement->angle;
          Func_02004b18(object, 1);
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
      Func_02004b38(object);
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
