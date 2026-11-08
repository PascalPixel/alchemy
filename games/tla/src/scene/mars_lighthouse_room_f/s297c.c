#include "version.h"

#include "types.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "vector3.h"
#include "fixed_math.h"

struct Work020023c4 {
  u8 unk00[4];
  u16 unk04;
  u8 unk06[0x68 - 6];
  s16 unk68;
  s16 unk6a;
};

void Func_02002800(void);
struct Actor *Actor_GetById(s32 id);
void Func_02005560(void);
void Func_02005648(s32 a);
void Func_02005480(struct Actor *actor, s32 a);
s32 Func_020054d8(s32 terrain, s32 x, s32 z);
s32 Func_020054f8(struct Actor *actor);
void Func_020055a8(s32 owner, s32 a, s32 b);
s32 Func_02005538(s32 *from, s32 radius, struct Vector3 *to, s32 radius2);
void wait(s32 frames);
void Animation_ApplyChildValues(struct Actor *actor, s32 a);
void Func_02002954(void);
extern u8 gData_020023c4[];

#if TLA_PATCH_1
void Func_0200297c(void) {
#else
s32 Func_0200297c(void) {
  /* FAKEMATCH(signature): declared returning s32, though it returns nothing,
   * so the return register's clobber and use rank the last store's address
   * copy ahead of its zero; as void, 2 of 186 instructions differ in
   * tla-ja. */
#endif
  struct MainRoutineWork *main = &gMainRoutineWork;
  struct Work020023c4 *work;
#if TLA_PATCH_1
  struct Vector3 position;
#else
  /* FAKEMATCH(temporary): the position held in a union beside a halfword;
   * as a plain struct Vector3, 16 of 186 instructions differ in tla-ja. */
  union {
    struct Vector3 v;
    s16 h;
  } positionU;
#define position positionU.v
#endif

  if (main->renderMode == 0) {
    struct Actor *actor;

    // UB: Func_02002800 takes no arguments and returns nothing; the bytes call
    // it with two through an int-returning type.
#ifdef UBFIX
    Func_02002800();
#else
    ((s32 (*)(s32, s32))Func_02002800)(-10, 0);
#endif
    actor = Actor_GetById(main->currentOwner);
    work = (struct Work020023c4 *)(gData_020023c4 + 0x188);
    Func_02005560();
    Func_02005648(0);
    actor->update = Func_02002954;
    actor->angle = TRIG_TURN / 4;
    Func_02005480(actor, 49);
    for (;;) {
      if (Func_020054d8(actor->terrainId, actor->x, actor->z) == 7) {
        s32 i;

        /* FAKEMATCH(temporary): velocityX stored through a union; as the plain
         * store, 2 of 236 instructions differ in tla-en (2 of 186 in
         * tla-ja). */
        ((union {
           s32 value;
           u16 half;
         } *)&actor->velocityX)
            ->value = work->unk68 << 17;
        actor->velocityZ = work->unk6a << 17;
        actor->acceleration = Q16_ONE / 16;
        for (i = 0; i <= 9; i++) {
          position.x = actor->x + (work->unk68 << 19);
          position.y = actor->y;
          position.z = actor->z + (work->unk6a << 19);
          if (Func_020054f8(actor) != 0) {
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
        position.z = actor->z + (work->unk6a << 19);
        result = Func_020054f8(actor);
        if (result > 0)
          break;
        if (result < 0) {
          actor->angle = TRIG_TURN / 4;
          Func_020055a8(gMainRoutineWork.currentOwner, 6, -1);
          break;
        }
#if TLA_PATCH_1
        radius = actor->radius - 2;
        other = Ram_HeapSlots->scriptObjects;
        flags = &other->collisionFlags;
        for (i = 0; i <= 63; i++, flags += 128, other++) {
          /* FAKEMATCH(wrapper): Func_02005538 forwarded through Value4; as a
           * direct call, 110 of 236 instructions differ in tla-en. */
          if (other->script != 0 && (*flags & 1) && other != actor &&
              Value4((s32 (*)())Func_02005538, (s32)&other->x, radius,
                     (s32)&position, other->radius - 2) >= 0) {
            wait(1);
            goto done;
          }
        }
#endif
        // A once-only block: its end holds the order around it (as the plain
        // statement, 6 of 236 instructions differ in tla-en; tla-ja matches
        // either way).
        do {
          x = actor->x += work->unk68 << 17;
        } while (0);
#if TLA_PATCH_1
        z = actor->z + (work->unk6a << 17);
#else
        // The old z read in a once-only block, whose end keeps the read ahead
        // of the step's (as a plain statement, 2 of 186 instructions differ
        // in tla-ja; as the patch-1 sum, 4).
        do {
          oldZ = actor->z;
        } while (0);
        z = oldZ + (work->unk6a << 17);
#endif
        actor->z = z;
        off = (x & 0xfffff) + 0xfff80000;
        if (off > Q16_ONE)
          off = Q16_ONE;
        if (off < (s32)0xffff0000)
          off = 0xffff0000;
        actor->x = x - off;
        off = (z & 0xfffff) + 0xfff80000;
        if (off > Q16_ONE)
          off = Q16_ONE;
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
  work->unk04 = 0;
}
#if !TLA_PATCH_1
#undef position
#endif
