// Differ 6 of 186 (tla-ja, 2026-11-08, -mtune=gba; the same on 2026-11-07); the two unions and
// the once-only block still pay (8, 22, 10 without each).
// Func_020024e4 of overlay 6ac: the Japanese release's own function, written
// as overlay 6aa's Japanese draft of Func_0200263c with this overlay's symbols
// (from 20: the position and velocityX through unions, actor->z first in the
// sums; both unions are still devices to replace). Left: actor->z loaded
// before the sum natively, and the last store's zero after the address's
// copy; scheduling only.
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

void Func_02002368();
struct Actor *Actor_GetById(s32 id);
void Func_020031e0(void);
void Func_02003280(s32 a);
void Func_02003118(struct Actor *actor, s32 a);
s32 Func_02003168(s32 terrain, s32 x, s32 z);
s32 Func_02003180(struct Actor *actor);
void Func_02003200();
void wait(s32 frames);
void Animation_ApplyChildValues(struct Actor *actor, s32 a);
void Func_020024bc(void);
extern u8 gData_020023c4[];

void Func_020024e4(void) {
  struct MainRoutineWork *main = &gMainRoutineWork;
  struct Work020023c4 *work;
  /* FAKEMATCH(temporary): the position held in a union beside a halfword;
   * as a plain struct Vector3, 22 of 186 instructions differ in tla-ja. */
  union {
    struct Vector3 v;
    s16 h;
  } positionU;
#define position positionU.v

  if (main->renderMode == 0) {
    struct Actor *actor;

    ((s32 (*)())Func_02002368)(-10, 0);
    actor = Actor_GetById(main->currentOwner);
    work = (struct Work020023c4 *)(gData_020023c4 + 0x188);
    Func_020031e0();
    Func_02003280(0);
    actor->update = Func_020024bc;
    actor->angle = 0x4000;
    Func_02003118(actor, 49);
    for (;;) {
      if (Func_02003168(actor->terrainId, actor->x, actor->z) == 7) {
        s32 i;

        /* FAKEMATCH(temporary): velocityX stored through a union; as the plain
         * store, 8 of 186 instructions differ in tla-ja. */
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
          if (Func_02003180(actor) != 0) {
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
        result = Func_02003180(actor);
        if (result > 0)
          break;
        if (result < 0) {
          actor->angle = 0x4000;
          Call3((void (*)())Func_02003200, gMainRoutineWork.currentOwner, 6,
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
