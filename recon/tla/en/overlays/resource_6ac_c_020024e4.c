// Differ 20 of 186 (tla-ja, 2026-11-07).
// Func_020024e4 of overlay 6ac: the Japanese release's own function, an older
// form of mars_lighthouse_room_m's (patch 1) without its check against the
// other script objects. Left: scheduling of each position.z sum around the
// position.y store, and the clamp's literal loads around the x store.
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
s32 Func_02003180(struct Actor *actor, struct Vector3 *position);
void Func_02003200(s32 id, s32 a, s32 b);
void wait(s32 frames);
void Animation_ApplyChildValues(struct Actor *actor, s32 a);
void Func_020024bc(void);
extern u8 gData_020023c4[];

void Func_020024e4(void) {
  struct MainRoutineWork *main = &gMainRoutineWork;
  struct Work020023c4 *work;
  struct Vector3 position;

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

        actor->velocityX = work->unk68 << 17;
        actor->velocityZ = work->unk6a << 17;
        actor->acceleration = 0x1000;
        for (i = 0; i <= 9; i++) {
          position.x = actor->x + (work->unk68 << 19);
          position.y = actor->y;
          position.z = (work->unk6a << 19) + actor->z;
          if (Func_02003180(actor, &position) != 0) {
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
        position.z = (work->unk6a << 19) + actor->z;
        result = Func_02003180(actor, &position);
        if (result > 0)
          break;
        if (result < 0) {
          actor->angle = 0x4000;
          Func_02003200(gMainRoutineWork.currentOwner, 6, -1);
          break;
        }
        // Once-only block: its end holds the x store before the z sum (4 more
        // of 186 differ as the plain statement).
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
