// Draft, differ 155 of 260, skeleton gap 9 (tla-en, 2026-11-01)
// Overlay 64b's Func_020036a8 (the world map's placed objects, shown and
// hidden as the view moves) under this overlay's names: shows the objects
// that come within the view and whose flags allow them, hides those that
// leave it, and starts a placement's event when the leader touches it. Its
// draft's head order, goto loop and arms are carried over; only the bottom's
// order is new here (i++ first: gap 11 to 9, differ 147 to 155; the 64b order
// slot++, placement++, i++ is the lowest differ, 147 at gap 11).
// Now right: placement r8, leader r10, work r9, slot r11, i spilled at sp+16.
// Left, all from one place, the rest being reload rotation that follows it
// (argument order at Func_02001fcc, muls/ldr, the pool load by lsls):
// - `i = 0` and `slot = objects` are set in both arms only because each
//   constant set doubles a live length: at 4x, i (10 refs) and slot (12) fall
//   below leader and work. The native has one set of each, in the join, after
//   its label: forward sched2 would lift `movs r3, #0` up the arm otherwise,
//   so cross-jumping is not the source of that tail. Set once there, i at 2x
//   outranks work (differ 212, gap 20; slot too 227, gap 15).
// - The native's i has 7 refs: its bottom compare reads a value that is not i,
//   as `next = i + 1` does, a `temporary`. slot still outranks work at 1x, so
//   that alone cannot be the whole story: leader and work need 8 refs, or i
//   and slot a cause of doubling not found yet.
// - `adds r3, r2` before the first test: reproduced only by a dead pre-if
//   store of kind (a device). The twin copy `mov r11, r4` looks like the same
//   thing, but objects[i] in a real for is not reduced (differ 272, gap 30).
// Tried: all 25 places of i = 0 across the arms (a tail placed last in both is
// cross-jumped, gap 8, differ 189, but it merges the wrong stores); all 6
// orders at the bottom; reach's operand orders and a sprite local (no change).
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
  u32 i;
  s32 left, right, top, bottom;
  struct Actor **slot;
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *object = work->viewCenter;
  s32 x = object->x;
  s32 zoom = Ram_HeapSlots->mapWork->zoom;
  u8 kind = placement->kind;
  s32 z = object->z;
  struct Actor **objects = gData_02003dc0;
  struct MainRoutineWork *routine = &gMainRoutineWork;

  if (zoom <= 0x10000) {
    left = x - 0xa00000;
    right = x + 0xa00000;
    top = z - 0x12c0000;
    bottom = z + 0xc80000;
    i = 0;
    slot = objects;
  } else {
    i = 0;
    left = x - 0xf00000;
    right = x + 0xf00000;
    top = z - 0x1c20000;
    bottom = z + 0x12c0000;
    slot = objects;
  }
  if (kind == 0)
    return;
top : {
    s32 px, pz;

    if (placement->unk01 == 15)
      goto next;
    object = *slot;
    px = placement->x << 16;
    pz = placement->z << 16;
    if (px > left && px < right && pz > top && pz < bottom) {
      s32 flag = placement->flag;
      s32 shown = 1;

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
        if (routine->ownerOverride == 0) {
          s32 dx = object->x - leader->x;
          s32 scale = ((struct WorldMapSprite *)object->animation)->scale;
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
        goto next;
      }
    }
    if (object != NULL) {
      Func_02001fd4(object);
      *slot = NULL;
    }
  next:
    slot++;
    placement++;
    i++;
    if (i < 128 && placement->kind != 0)
      goto top;
}
}
