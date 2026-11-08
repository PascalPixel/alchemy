// Matches: differ 0 (tla-en, 2026-11-08)
// All six editions at 0 (tla-en, tla-ja, tla-de, tla-es, tla-fr, tla-it). The
// patch-1 side is games/tla/src/scene/mars_lighthouse_star_room/s2148.c with
// the far routine's position passed (it matches either way); the #else side
// is the Japanese release's own form (no overlap scan), still on its scaffold
// in tla-ja. To adopt: this file replaces s2148.c and tla-ja's scaffold
// section for Func_02002148 in resource_6ab_overlay.s goes.
// Func_0200424c is the veneer to Func_08020210, which takes (object,
// position) (games/tla/src/field/event_commands/f0df1fc.c), and r1 holds
// &position at both calls, so the call passes it. Found 8 November, as in the
// twins (6ad Func_0200297c, 6ac Func_020024e4, 6aa Func_0200263c): with the
// position passed and position.z's store in a once-only block (8 of 186
// without each), the position union of the earlier rounds goes (it was a
// tagged temporary; 16 without, before); the old z read in its own
// once-only block puts sched2's barrier on the work's s16 read, which
// settles the clamp constant 0x10000 (as a plain read 2, as the patch-1 sum
// 4); the x step's block is no longer needed in Japanese. Two tagged devices
// remain: the velocityX union (both sides, 2 without: the work's s16 read must
// wait on that s32 store with i's start filling the gap; i = 0 moved ahead
// of either store, or once-only blocks around either store, 2 to 16) and,
// in Japanese only, an empty asm reading work before the last store (2
// without: sched2 sets the zero ahead of reload's copy of work by RTL order).
// Milder forms tried for the last store without gain: a block-local copy of
// work, an inline helper, a long-lived pointer reused for it (combine folds
// the copy), the asm after the store ("r" 2, "l" 2), "l" before it (17), a
// taken address (70). Returning work also matches tla-ja, but is the worse
// signature device and breaks patch 1's form (44).
#include "version.h"

#include "types.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "vector3.h"

struct Work020023c4 {
  u8 unk00[4];
  u16 unk04;
  u8 unk06[0x68 - 6];
  s16 unk68;
  s16 unk6a;
};

void Func_02001fcc();
struct Actor *Actor_GetById(s32 id);
void Func_020042dc(void);
void Func_0200444c(s32 a);
void Func_020041cc(struct Actor *actor, s32 a);
s32 Func_0200422c(s32 terrain, s32 x, s32 z);
s32 Func_0200424c(struct Actor *actor, struct Vector3 *position);
void Func_0200436c();
s32 Func_0200428c(s32 *from, s32 radius, struct Vector3 *to, s32 radius2);
void wait(s32 frames);
void Animation_ApplyChildValues(struct Actor *actor, s32 a);
void Func_02002120(void);
extern u8 gData_020023c4[];

void Func_02002148(void) {
  struct MainRoutineWork *main = &gMainRoutineWork;
  struct Work020023c4 *work;
  struct Vector3 position;

  if (main->renderMode == 0) {
    struct Actor *actor;

    ((s32 (*)())Func_02001fcc)(-10, 0);
    actor = Actor_GetById(main->currentOwner);
    work = (struct Work020023c4 *)(gData_020023c4 + 0x188);
    Func_020042dc();
    Func_0200444c(0);
    actor->update = Func_02002120;
    actor->angle = 0x4000;
    Func_020041cc(actor, 49);
    for (;;) {
      if (Func_0200422c(actor->terrainId, actor->x, actor->z) == 7) {
        s32 i;

        /* FAKEMATCH(temporary): velocityX stored through a union, so the
         * store aliases the work's s16 read after it; as the plain store, 2
         * of 236 instructions differ in tla-en (2 of 186 in tla-ja). */
        ((union {
           s32 value;
           u16 half;
         } *)&actor->velocityX)
            ->value = work->unk68 << 17;
        actor->velocityZ = work->unk6a << 17;
        actor->acceleration = 0x1000;
        for (i = 0; i <= 9; i++) {
          position.x = actor->x + (work->unk68 << 19);
          position.y = actor->y;
#if TLA_PATCH_1
          position.z = actor->z + (work->unk6a << 19);
#else
          // A once-only block: its start holds position's first two stores
          // ahead of the work read.
          do {
            position.z = actor->z + (work->unk6a << 19);
          } while (0);
#endif
          if (Func_0200424c(actor, &position) != 0) {
            actor->velocityX = 0;
            actor->velocityZ = 0;
            break;
          }
          wait(1);
        }
        break;
      } else {
        s32 result;
#if TLA_PATCH_1
        s32 radius;
        struct Actor *other;
        u8 *flags;
        s32 x, z, off;
        s32 i;
#else
        s32 x, z, off, oldZ;
#endif

        position.x = actor->x + (work->unk68 << 19);
        position.y = actor->y;
#if TLA_PATCH_1
        position.z = actor->z + (work->unk6a << 19);
        result = Func_0200424c(actor, &position);
#else
        // A once-only block, as in the loop above, with the call inside so
        // the call reuses position's address.
        do {
          position.z = actor->z + (work->unk6a << 19);
          result = Func_0200424c(actor, &position);
        } while (0);
#endif
        if (result > 0)
          break;
        if (result < 0) {
          actor->angle = 0x4000;
          Call3((void (*)())Func_0200436c, gMainRoutineWork.currentOwner, 6,
                -1);
          break;
        }
#if TLA_PATCH_1
        radius = actor->radius - 2;
        other = Ram_HeapSlots->scriptObjects;
        flags = &other->collisionFlags;
        for (i = 0; i <= 63; i++, flags += 128, other++) {
          if (other->script != 0 && (*flags & 1) && other != actor &&
              Value4((s32 (*)())Func_0200428c, (s32)&other->x, radius,
                     (s32)&position, other->radius - 2) >= 0) {
            wait(1);
            goto done;
          }
        }
        // A once-only block: its end holds the order around it (as the plain
        // statement, 6 of 236 instructions differ in tla-en).
        do {
          x = actor->x += work->unk68 << 17;
        } while (0);
        z = actor->z + (work->unk6a << 17);
#else
        x = actor->x += work->unk68 << 17;
        // The old z read in a once-only block, whose end keeps the read
        // between x's store and the work read.
        do {
          oldZ = actor->z;
        } while (0);
        z = oldZ + (work->unk6a << 17);
#endif
        actor->z = z;
        off = (x & 0xfffff) + 0xfff80000;
        if (off > 0x10000)
          off = 0x10000;
        if (off < (s32)0xffff0000)
          off = 0xffff0000;
        actor->x = x - off;
        off = (z & 0xfffff) + 0xfff80000;
        if (off > 0x10000)
          off = 0x10000;
        if (off < (s32)0xffff0000)
          off = 0xffff0000;
        actor->z = z - off;
        work->unk04 = 0;
        wait(1);
      }
    }
#if TLA_PATCH_1
  done:
#endif
    actor->update = 0;
    Animation_ApplyChildValues(actor, 0);
  }
#if !TLA_PATCH_1
  /* FAKEMATCH(barrier): an empty asm reading work before the last store; as
   * the plain store, 2 of 186 instructions differ in tla-ja. */
  asm("" : : "r"(work));
#endif
  work->unk04 = 0;
}
