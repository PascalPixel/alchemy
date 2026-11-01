// Draft, differ 149 of 236, skeleton gap 43 (tla-en, 2026-11-01)
// Control flow, types and every load agree with the listing; 243 instructions
// against 236. Left: gcse's PRE moves &actor->terrainId, the two pointers into
// the work struct and &position from the loop head onto its entry edge (see
// the .07.gcse dump), which costs a spill and the registers after it. The
// bytes keep all four inside the loop. Neither a goto loop nor a for (;;)
// stops it, and pointers, plain member reads and a jump from the entry test
// into the loop change nothing. PRE only skips them if the back edge lacks
// them, so a path that reaches the back edge past the loop head would do it;
// none is visible in the listing. The renderMode test reaches the last store
// with `work` never set, in JA too: the skip path reads an unset local.
// `if (i < 10) goto step` inverts round a far branch (one b.n too many); a
// do-while fixes that but loop.c then moves the velocity-clearing block out.
// Func_02001fcc's -10 stays negated before its 0: the listing has them the
// other way round; the Call3 forwarding fixed the same order at Func_0200436c.
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

    Func_02001fcc(-10, 0);
    actor = Actor_GetById(main->currentOwner);
    work = (struct Work020023c4 *)(gData_020023c4 + 0x188);
    Func_020042dc();
    Func_0200444c(0);
    actor->update = Func_02002120;
    actor->angle = 0x4000;
    Func_020041cc(actor, 49);
  loop:
    {
      s32 result;
      s16 *value68, *value6a;
      struct Vector3 *pos;

      result = Func_0200422c(actor->terrainId, actor->x, actor->z);
      value68 = &work->unk68;
      value6a = &work->unk6a;
      pos = &position;

      if (result == 7) {
        s32 i;

        actor->velocityX = *value68 << 17;
        actor->velocityZ = *value6a << 17;
        actor->acceleration = 0x1000;
        i = 0;
      step:
        position.x = actor->x + (*value68 << 19);
        position.y = actor->y;
        position.z = actor->z + (*value6a << 19);
        if (Func_0200424c(actor) != 0) {
          actor->velocityX = 0;
          actor->velocityZ = 0;
          goto done;
        }
        wait(1);
        i++;
        if (i < 10)
          goto step;
      } else {
        s32 result2;

        pos->x = actor->x + (*value68 << 19);
        pos->y = actor->y;
        pos->z = actor->z + (*value6a << 19);
        result2 = Func_0200424c(actor);
        if (result2 > 0)
          goto done;
        if (result2 < 0) {
          actor->angle = 0x4000;
          Call3((void (*)())Func_0200436c, gMainRoutineWork.currentOwner, 6,
                -1);
          goto done;
        }
        {
          s32 radius = actor->radius - 2;
          struct Actor *other = Ram_HeapSlots->scriptObjects;
          s32 i;
          s32 x, z, off;

          u8 *flags = &other->collisionFlags;

          i = 0;
        scan:
          if (other->script != 0 && (*flags & 1) && other != actor &&
              Func_0200428c(&other->x, radius, &position, other->radius - 2) >=
                  0) {
            wait(1);
            goto done;
          }
          i++;
          flags += 128;
          other++;
          if (i < 64)
            goto scan;
          x = actor->x += *value68 << 17;
          z = actor->z + (*value6a << 17);
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
          goto loop;
        }
      }
    }
  done:
    actor->update = 0;
    Animation_ApplyChildValues(actor, 0);
  }
  work->unk04 = 0;
}
