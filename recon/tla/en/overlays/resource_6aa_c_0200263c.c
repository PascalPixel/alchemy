// Differ 6 of 186 (tla-ja, 2026-11-07).
// The Japanese release's own Func_0200263c of overlay 6aa: the patch-1 C in
// games/tla/src/scene/mars_lighthouse_entrance/s263c.c without its overlap
// scan, as overlay 6ab's Japanese draft of Func_02002148 (also 6 of 186).
// The position read through a union (plain: 22 differ) and velocityX stored
// through a union (plain: 8 differ) are still devices to replace. Left:
// actor->z loaded before the sum natively, and the last store's zero after
// the address's copy; scheduling only.
#include "edition.h"
#include "types.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "vector3.h"

struct Work020023c4 {
  u8 unk00[4];
  u16 unk04;
  u8 unk06[0x68 - 6];
  s16 unk68;
  s16 unk6a;
};

void Func_020024c0();
struct Actor *Actor_GetById(s32 id);
void Func_02004ca8(void);
void Func_02004da0(s32 a);
void Func_02004bc0(struct Actor *actor, s32 a);
s32 Func_02004c18(s32 terrain, s32 x, s32 z);
s32 Func_02004c38(struct Actor *actor);
void Func_02004ce8();
void wait(s32 frames);
void Animation_ApplyChildValues(struct Actor *actor, s32 a);
void Func_02002614(void);
extern u8 gData_020023c4[];

void Func_0200263c(void) {
  struct MainRoutineWork *main = &gMainRoutineWork;
  struct Work020023c4 *work;
  union {
    struct Vector3 v;
    s16 h;
  } positionU;
#define position positionU.v

  if (main->renderMode == 0) {
    struct Actor *actor;

    ((s32 (*)())Func_020024c0)(-10, 0);
    actor = Actor_GetById(main->currentOwner);
    work = (struct Work020023c4 *)(gData_020023c4 + 0x188);
    Func_02004ca8();
    Func_02004da0(0);
    actor->update = Func_02002614;
    actor->angle = 0x4000;
    Func_02004bc0(actor, 49);
    for (;;) {
      if (Func_02004c18(actor->terrainId, actor->x, actor->z) == 7) {
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
          if (Func_02004c38(actor) != 0) {
            actor->velocityX = 0;
            actor->velocityZ = 0;
            break;
          }
          wait(1);
        }
        break;
      } else {
        s32 result;
        s32 x, z, off;

        position.x = actor->x + (work->unk68 << 19);
        position.y = actor->y;
        position.z = actor->z + (work->unk6a << 19);
        result = Func_02004c38(actor);
        if (result > 0)
          break;
        if (result < 0) {
          actor->angle = 0x4000;
          Call3((void (*)())Func_02004ce8, gMainRoutineWork.currentOwner, 6,
                -1);
          break;
        }
        // A once-only block: its end holds the order around it (as the plain
        // statement, 10 of 186 instructions differ in tla-ja).
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
    actor->update = 0;
    Animation_ApplyChildValues(actor, 0);
  }
  work->unk04 = 0;
}
