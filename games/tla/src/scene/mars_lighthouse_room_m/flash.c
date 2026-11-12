#include "types.h"
#include "ram_buffer.h"
#include "frame.h"
#include "actor.h"
#include "event_inline.h"

struct RideEntry {
  s16 a;
  u8 unk02[2];
  s16 b;
  s16 c;
  u8 unk08[4];
  s32 d;
};

struct RideRow {
  u8 unk00[20];
  s16 a;
  u8 unk16[2];
  s16 b;
  s16 c;
  u8 unk1c[4];
  s32 d;
};

struct RideSlot {
  u16 value;
  u8 unk02[2];
};

struct RideWork {
  s32 counter;
  s16 active;
  u8 unk06[8];
  s16 value0e;
  u8 unk10[4];
  struct RideEntry entries[4];
  struct RideSlot slots[4];
  u8 unk64[12];
  void *handle;
};

extern u8 gData_020023c4[];
extern u16 gData_02004cf0[];
void Animation_ApplyChildValues(struct Actor *object, s32 value);
void Func_02003130(void *handle);
void Func_020024e4(void);
s32 Func_02002708(void);
void Func_02002e6c(struct Actor *self);
#include "main_routine_work.h"

void Func_02003118(struct Actor *object, s32 value);
void Func_02003230(s32 a, s32 b);
void Func_02003228(void);
void Func_02001608(void);
void Func_020032a8(void);
void Func_0200166c(void);
void Func_0200055c(void);
void Func_0200170c(void);

void Func_020024bc(struct Actor *object) {
  switch (gFrameCount & 7) {
  case 0:
    Animation_ApplyChildValues(object, 10);
    break;
  case 4:
    Animation_ApplyChildValues(object, 0);
    break;
  }
}

#include "version.h"

#include "call.h"
#include "vector3.h"
#include "fixed_math.h"

struct Work020023c4 {
  u8 unk00[4];
  u16 unk04;
  u8 unk06[0x68 - 6];
  s16 unk68;
  s16 unk6a;
};

void Func_02002368(void);
void Func_020031e0(void);
s32 Func_02003168(s32 terrain, s32 x, s32 z);
s32 Func_02003180(struct Actor *actor, struct Vector3 *position);
void Func_02003200();
s32 Func_020031b0(s32 *from, s32 radius, struct Vector3 *to, s32 radius2);

void Func_020024e4(void) {
  struct MainRoutineWork *main = &gMainRoutineWork;
  struct Work020023c4 *work;
  struct Vector3 position;

  if (main->renderMode == 0) {
    struct Actor *actor;

#ifdef UBFIX
    Func_02002368();
#else
    // UB: Func_02002368 takes no arguments and returns nothing.
    ((s32 (*)())Func_02002368)(-10, 0);
#endif
    actor = Actor_Get(main->currentOwner);
    work = (struct Work020023c4 *)(gData_020023c4 + 0x188);
    Func_020031e0();
    FieldEvent_ResolvePendingActions(0);
    actor->update = Func_020024bc;
    actor->angle = TRIG_TURN / 4;
    Func_02003118(actor, 49);
    for (;;) {
      if (Func_02003168(actor->terrainId, actor->x, actor->z) == 7) {
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
        actor->acceleration = Q16_ONE / 16;
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
          if (Func_02003180(actor, &position) != 0) {
            actor->velocityX = 0;
            actor->velocityZ = 0;
            break;
          }
          System_Wait(1);
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
        result = Func_02003180(actor, &position);
#else
        // A once-only block, as in the loop above, with the call inside so
        // the call reuses position's address.
        do {
          position.z = actor->z + (work->unk6a << 19);
          result = Func_02003180(actor, &position);
        } while (0);
#endif
        if (result > 0)
          break;
        if (result < 0) {
          actor->angle = TRIG_TURN / 4;
          Func_02003200(gMainRoutineWork.currentOwner, 6, -1);
          break;
        }
#if TLA_PATCH_1
        radius = actor->radius - 2;
        other = Ram_HeapSlots->scriptObjects;
        flags = &other->collisionFlags;
        /* FAKEMATCH(wrapper): the overlap test forwarded through Value4; as a
         * direct Func_020031b0 call, 117 of 236 instructions differ in tla-en.
         */
        for (i = 0; i <= 63; i++, flags += 128, other++) {
          if (other->script != 0 && (*flags & 1) && other != actor &&
              Value4((s32 (*)())Func_020031b0, (s32)&other->x, radius,
                     (s32)&position, other->radius - 2) >= 0) {
            System_Wait(1);
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
        /* FAKEMATCH(temporary): oldZ carries the z read out of the block; as z
         * = actor->z + (work->unk6a << 17), 8 of 186 instructions differ in
         * tla-ja. */
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
        System_Wait(1);
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
  // UB: work is set only when renderMode is 0; otherwise the store goes
  // through whatever its register holds.
#ifdef UBFIX
  if (main->renderMode == 0)
#endif
    work->unk04 = 0;
}

void Func_020026f0(void) {
  if (((struct RideWork *)(gData_020023c4 + 0x188))->active)
    Func_020024e4();
}
