// Draft, differ 116 (tla-en, 2026-11-05). Func_02002148 of overlay 6ab.
// Plain loops and block-scoped counters under -Os; the barrier and one-pass
// are gone. A rise from 112: the step counter and the pointers set per pass
// now sit as natively. Left: the frame address of position takes r8 here and
// the scan counter spills to r10 and a slot, where natively that pseudo gets
// no register and is rebuilt per use (add r1, sp, 4); the frame is 20 against
// 16.
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

        actor->velocityX = work->unk68 << 17;
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
        radius = actor->radius - 2;
        other = Ram_HeapSlots->scriptObjects;
        flags = &other->collisionFlags;
        for (i = 0; i <= 63; i++, flags += 128, other++) {
          if (other->script != 0 && (*flags & 1) && other != actor &&
              Func_0200428c(&other->x, radius, &position, other->radius - 2) >=
                  0) {
            wait(1);
            goto done;
          }
        }
        x = actor->x += work->unk68 << 17;
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
