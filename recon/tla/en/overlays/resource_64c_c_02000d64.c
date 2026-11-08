// Differ 6 of 260 (tla-en, 2026-11-08, -mtune=gba; the same 6 on 2026-11-07; the
// twin's 2026-11-08 round, negation forms and device re-measures, moved nothing): overlay 64b's Func_020036a8 form,
// carried over whole; same three devices to tag under S2 on adoption (work
// pinned to r9, empty asms on next and leader) and the same 6 left, in the
// flag negation's registers. See the twin's header for what each step gave.
// Earlier: differ 63 (2026-11-07), slot ranked above leader and work.
// 2026-11-07: slot needs 2x (12 / 234, between work and i), which only a
// REG_EQUAL note on its first set gives; the join copy from the early objects
// gets none, since the join (a label after the zoom arms) is in no cse block
// with the head. Native head scratch is r5, not r4, so objects is live from
// early in the head: gcse hoisting from the arms (which inserts at the head's
// end) is not the source. kind's copy is merged by combine (next dies at the
// test); it survives behind a used label `top:` before the test with `goto top`
// at the bottom (82: scratch right, bottom unrotated, slot still r10), and a
// bottom `if (next != 0) goto top` threads but drops the label again (65).
// i = 0, kind and slot set in both arms (all 4x) give leader r10, work r9,
// slot r11 and i spilled, but reload's picks in the arms and cross-jumping
// break them (181 to 194, every order). volatile i, 117; types of next and
// kind, and forms of !GameFlag_Test, no change; a 10-minute permuter run on
// the label form, nothing under 82.
// 2026-11-06: the twin's block-local mode = 9 for the renderMode
// test (overlay 64b's Func_020036a8) takes 75 to 63 here too. Left: register picks, leader r9,
// work r11, slot r10 for the native r10, r9, r11 (slot ranks above leader and work).
// Before, differ 75: slot ranks above leader and work.
// Draft notes of 5 November 2026: A plain while on a copy of the
// kind, i++ first at the bottom, the routine work read direct, scale before dx. Left:
// slot ranks above leader and work (2 * 6 / 116 against 2 * 4 / 136), where the native
// holds them in r10 and r9 and slot in r11; the kind's copy ties into r2 where the
// native keeps adds r3, r2, #0. Making the slot objects[i] is worse (208), and a
// local for the flag's result moves the whole rotation (236).
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
// 2026-11-06: under -Os, gcse's code hoisting moves an expression computed in
// both arms of the zoom if into the head and leaves copies (it needs two
// occurrences): `slot = gData_02003dc0; next = placement->kind;` in both arms,
// with no early locals, gives the head's ldrb r2 and ldr r4 and the join's
// copies as the native has them, but the two REG_EQUAL sets put slot at 4x
// (2 * 7 / 476), below i (2 * 5 / 232), so slot spills and i takes r11: differ
// 170; with slot set once at the join instead, 160. `slot = gData_02003dc0`
// once at the join beside the early kind (2x, 2 * 6 / 232) gives leader r10,
// work r9, slot r11 as the native, but loses the early pool load: differ 178.
// i = 0 in both arms as well (4x) keeps slot in r11 but cross-jumps the arms'
// stores: differ 193 to 199.
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

void Func_02000d64(void) {
  struct WorldMapPlacement *placement = Func_02002094();
  struct Actor *leader = Actor_GetById(Func_0200209c());
  s32 leaderReach =
      ((struct WorldMapSprite *)leader->animation)->scale * leader->radius;
  u32 i;
  s32 next;
  s32 left, right, top, bottom;
  struct Actor **slot;
  /* FAKEMATCH(pin): the form kept; as a plain local, 24 of 260 instructions
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
  /* FAKEMATCH(barrier): the form kept; without it, 13 of 260 instructions
   * differ in tla-en. */
  asm("" : "+g"(next));
  slot = objects;
  /* FAKEMATCH(barrier): the form kept; without it, 22 of 260 instructions
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
