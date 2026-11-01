// Draft, differ 251 of 260, skeleton gap 27 (tla-en, 2026-11-01)
// Shows the world map's placed objects that come within the view and whose
// flags allow them, hides those that leave it, and starts a placement's
// event when the leader touches it. Overlay 64c has the same code (its
// Func_02000d64), so what closes one closes the other.
// Left: allocation. The native is placement r8, leader r10, work r9, slot r11,
// i spilled at sp+16; here slot and i outrank leader and work, so work spills
// and nearly every register moves. From the lreg dump, priority is leader 7
// refs over 146, work 6 over 140, slot 11 over 118, i 9 over 228 (doubled by
// its REG_EQUIV zero). The native's order needs leader 9, work 8, a slot
// doubled once (236), and i as it is. Those are exactly the counts if the
// four leader and work uses after routine->ownerOverride sit one loop level
// deeper at flow (12.life), and the listing has no such back edge, so
// something folds it after flow. No plain source for that was found.
// Measured with devices, not kept because none reached differ 0:
// - `check:` before the ownerOverride test, with `again = placement->unk01;
//   if (again & 256) goto check;` after it (s32 again; combine folds it, while
//   again >= 256 and object & 1 stay as code): gives leader 9, work 8, i 10
//   over 240, and the native's four registers at last, without touching i.
// - slot needs its REG_EQUIV: `slot = gData_02006c08` at the loop start, with
//   the pool load kept before the if by a dead `u8 read = placement->kind; u8
//   kind = read + (s32)objects;`, which gcse turns into the native's r2 and r4
//   loads and `mov r11, r4`. Together, as a goto loop: differ 168, gap 13.
// - Still missing then: the `adds r3, r2` of kind before the first test
//   (combine folds the copy into the compare, or global ties the registers;
//   a forward jump over the copy and an extra use of kind both failed) and a
//   reload rotation of one in the spill registers. i = 0 and slot set in both
//   arms give the right ranks too (length times 4) but reload's r3, r5, r1
//   order through the arms shows i = 0 follows the second arm, once.
// Plain forms: object doubling as the view center gives the native's r6
// there (a separate center takes r3). Head order: x, z, objects, zoom 251,
// gap 27; x, z, zoom, objects 259, gap 21; zoom first 279. With the older
// notes: radius * scale 257; the flag read four times 287; indexing
// placements[i] and gData_02006c08[i] 262; slot initialized at its declaration
// 281; a local party pointer 289; a real for loop gives the first test's load,
// not the copy, and `next = i + 1` tested at the bottom (a temporary) drops i
// to 7 refs and spills it but leaves leader, work and slot rotated (203).
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
  u32 i;
  s32 left, right, top, bottom;
  struct Actor **slot;
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *object = work->viewCenter;
  s32 x = object->x;
  s32 z = object->z;
  struct Actor **objects = gData_02006c08;
  s32 zoom = Ram_HeapSlots->mapWork->zoom;
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
