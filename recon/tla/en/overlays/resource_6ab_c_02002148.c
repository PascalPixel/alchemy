// Draft, differ 6 of 186 (tla-ja, 2026-11-05): the Japanese form only;
// measure with --edition tla-ja. The English C in
// games/tla/src/scene/mars_lighthouse_star_room/s2148.c without its overlap
// scan, and the position read through a union. Twins: 6ad Func_0200297c, 6ac
// Func_020024e4, 6aa Func_0200263c. Left: actor->z loaded before the sum
// natively, and the last store's zero after the address's copy.
#include "edition.h"


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
s32 Func_0200424c(struct Actor *actor);
void Func_0200436c();
s32 Func_0200428c(s32 *from, s32 radius, struct Vector3 *to, s32 radius2);
void wait(s32 frames);
void Animation_ApplyChildValues(struct Actor *actor, s32 a);
void Func_02002120(void);
extern u8 gData_020023c4[];

void Func_02002148(void) {
  struct MainRoutineWork *main = &gMainRoutineWork;
  struct Work020023c4 *work;
  union { struct Vector3 v; s16 h; } positionU;
#define position positionU.v

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
          position.z = actor->z + (work->unk6a << 19);
          if (Func_0200424c(actor) != 0) {
            actor->velocityX = 0;
            actor->velocityZ = 0;
            break;
          }
          wait(1);
        }
        break;
      } else {
        s32 result;
        s32 radius;
        struct Actor *other;
        u8 *flags;
        s32 x, z, off;
        s32 i;

        position.x = actor->x + (work->unk68 << 19);
        position.y = actor->y;
        position.z = actor->z + (work->unk6a << 19);
        result = Func_0200424c(actor);
        if (result > 0)
          break;
        if (result < 0) {
          actor->angle = 0x4000;
          Call3((void (*)())Func_0200436c, gMainRoutineWork.currentOwner, 6,
                -1);
          break;
        }
#if (LANGUAGE != LANGUAGE_JAPANESE)
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
#endif
        /* FAKEMATCH(one-pass): the statement in a once-only block, whose end
         * holds the order around it; as the plain statement, 8 of 236
         * instructions differ in tla-en. */
        do {
          x = actor->x += work->unk68 << 17;
        } while (0);
        z = actor->z + (work->unk6a << 17);
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
  done:
    actor->update = 0;
    Animation_ApplyChildValues(actor, 0);
  }
  work->unk04 = 0;
}

