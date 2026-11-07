// Differ 6 of 260 (tla-en, 2026-11-07), with three devices (S2 tags needed on
// adoption): work pinned to r9 (pin), and empty asms that read and write next
// (barrier: keeps kind's join copy) and leader (barrier: one more reference,
// so leader outranks slot). Plain moves that took it 41 to 6: the map work read
// through a `map` local declared between work and object, with zoom right after
// object (26), and dx declared bare and set after the locals (6); the old
// `mode = 9` temporary is no longer needed (same 6 without it, dropped). Left:
// `!GameFlag_Test(flag)` computes in r2 then 1 in r3 where the native has the
// test in r3 and 1 in r2 (== 0, 1 - (x != 0), ternary, u32 shown: no change).
// Without the next barrier, 13; without the leader barrier, 22; without the
// pin, 24; arms forms with asm("") against cross-jumping, 175 to 185.
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
// Pins measured 2026-11-06 (not kept): work pinned to r9 alone gives 57, with
// leader in r11 and slot in r10 (the native r10, r11) and the head's kind copy
// still missing; adding leader to r10 (142) or slot to r11 (107) breaks the
// tie to the call's r0 and the objects copy. A slot set at the join from
// gData_02006c08 (no objects) with work pinned, 168; slot set in both arms, 166.
// A 20-minute permuter run (2026-11-06) reached 57 only through noise (an identity
// inline around the flag test, pz as << 3 << 13, x << 16 reread), not kept.
// 2026-11-06: a block-local mode = 9 for the renderMode test (permuter) took
// 75 to 63; what is left is register picks: leader r9, work r11, slot r10 for
// the native r10, r9, r11.
// 2026-11-06, from the greg dump: priorities are placement 3 * 11 / 148, slot
// 2 * 6 / 116, leader 2 * 5 / 142, work 2 * 4 / 136, i 2 * 5 / 234 (its i = 0
// doubles it), so slot takes r10 and leader and work r9 and r11. slot's copy
// at the join gets no REG_EQUAL note (cse sees the pool load only in the head's
// block), so it is not doubled; at 2x (2 * 6 / 232) it would sit between work
// and i as the native's r11 does. `struct Actor **slot = gData_02006c08` set in
// the head gives exactly the native's leader r10, work r9, slot r11, but with no
// objects pseudo in r4 through the head every scratch choice moves (r4 for r5):
// differ 152; set there by position in the declarations, 152 to 160. Indexing
// objects[i] gives the same three registers with an offset giv (208); slot =
// &objects[i] in the loop, 87 to 124; i = 0, next and slot hoisted from both
// arms in all six orders, 193 to 203; a goto loop in place of the while, 77.
// Draft, differ under 64c's form (tla-en, 5 November 2026): see the twin overlay 64c's Func_02000d64 for what is left.
// Under -Os, 5 November: differ 138 of 260 (tla-en); older figures below predate it.
// Draft, differ 147 of 260, skeleton gap 11 (tla-en, 2026-11-01)
// Shows the world map's placed objects that come within the view and whose
// flags allow them, hides those that leave it, and starts a placement's
// event when the leader touches it. Overlay 64c has the same code (its
// Func_02000d64), so what closes one closes the other: its draft still has
// the old shape, and this one's head order, goto loop and arms should carry.
// Now right: the native's four registers (placement r8, leader r10, work r9,
// slot r11, i spilled at sp+16), the loop shape and the head's loads. The
// goto loop with its test at the bottom, kind read before the if, and slot
// and i set in the arms did that; a real for or while took 207 to 215 here.
// Why the arms: each constant set doubles a pseudo's live length, so slot
// (11 refs) and i (9 refs) at 4x fall below leader and work.
// Left, 10 removed and 11 added in the skeleton, all around one place:
// - `i = 0` is in the wrong place, and this form is known not to be the
//   source's. The native has one `movs r3, #0; str r3, [sp, #16]` after the
//   second arm: reload's picks run r3, r5, r1, r3 through the first arm and
//   r5, r3, r5, r1, then r3 for i, so the first arm reloads nothing for i.
//   Set only there (`i = 0` before the test), i at 2x outranks work: differ
//   158, gap 14, i in r10 and slot spilled. i wants 7 refs or a third
//   doubling; `next = i + 1` gives 7 but is a `temporary`. ++i in the test,
//   i = i + 1 and u32 i = 0 at its declaration (185) did not change it.
// - `adds r3, r2` before the first test is missing, so kind stays in r1 here
//   and one fewer pick rotates the loop's scratch registers (r5, r1, r0 in the
//   native). It survives combine only if it is in another block from the
//   test. `kind = first` in both arms is propagated away by cprop; a second
//   local k set in the join or at the bottom folds the same (158), and a
//   while or do over kind itself takes 207. Unfound: a plain form that keeps
//   a copy. A dead `(kind = placement->kind)` load keeps it but is a device.
// - The head's constant is r2 for the native's r3, and the x, kind pair is
//   swapped; every order of center, x, z, zoom, kind, objects and routine was
//   measured (1680): this one has the lowest gap, 146 is the lowest differ
//   (center, x, zoom, kind, z, objects, routine) at gap 14.
// i's place in the arms is steered: first arm at its end, second at its start,
// slot last in each (other places 151 to 202). `kind` read once before the
// if is the listing's own load.
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
