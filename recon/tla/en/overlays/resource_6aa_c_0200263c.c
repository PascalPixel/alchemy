// Draft, differ 112: overlay 6aa's copy of the function drafted in
// resource_6ab_c_02002148.c, whose header says what it is and what still differs.
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

void Func_020024c0();
struct Actor *Actor_GetById(s32 id);
void Func_02004ca8(void);
void Func_02004da0(s32 a);
void Func_02004bc0(struct Actor *actor, s32 a);
s32 Func_02004c18(s32 terrain, s32 x, s32 z);
s32 Func_02004c38(struct Actor *actor);
void Func_02004ce8();
s32 Func_02004c78(s32 *from, s32 radius, struct Vector3 *to, s32 radius2);
void wait(s32 frames);
void Animation_ApplyChildValues(struct Actor *actor, s32 a);
void Func_02002614(void);
extern u8 gData_020023c4[];

void Func_0200263c(void) {
  struct MainRoutineWork *main = &gMainRoutineWork;
  struct Work020023c4 *work;
  struct Vector3 position;

  if (main->renderMode == 0) {
    struct Actor *actor;

    ((s32 (*)())Func_020024c0)(-10, 0);
    actor = Actor_GetById(main->currentOwner);
    work = (struct Work020023c4 *)(gData_020023c4 + 0x188);
    /* FAKEMATCH(barrier): the empty asm on work changes its allocation rank; as no
     * asm, 116 of 236 instructions differ in tla-en. */
    asm("" : "=r"(work) : "0"(work));
    Func_02004ca8();
    Func_02004da0(0);
    actor->update = Func_02002614;
    actor->angle = 0x4000;
    Func_02004bc0(actor, 49);
  loop:
    {
      s32 result;

      result = Func_02004c18(actor->terrainId, actor->x, actor->z);

      if (result == 7) {
        s32 i;

        /* FAKEMATCH(one-pass): the statement in a once-only block, whose end holds the
         * order around it; as the plain statement, 114 of 236 instructions differ
         * in tla-en. */
        do {
          actor->velocityX = work->unk68 << 17;
        } while (0);
        actor->velocityZ = work->unk6a << 17;
        actor->acceleration = 0x1000;
        i = 0;
      step:
        position.x = actor->x + (work->unk68 << 19);
        position.y = actor->y;
        position.z = actor->z + (work->unk6a << 19);
        if (Func_02004c38(actor) != 0) {
          actor->velocityX = 0;
          actor->velocityZ = 0;
          goto done;
        }
        wait(1);
        i++;
        if (i >= 10)
          goto done;
        goto step;
      } else {
        s32 result2;

        position.x = actor->x + (work->unk68 << 19);
        position.y = actor->y;
        position.z = actor->z + (work->unk6a << 19);
        result2 = Func_02004c38(actor);
        if (result2 > 0)
          goto done;
        if (result2 < 0) {
          actor->angle = 0x4000;
          Call3((void (*)())Func_02004ce8, gMainRoutineWork.currentOwner, 6,
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
              Func_02004c78(&other->x, radius, &position, other->radius - 2) >=
                  0) {
            wait(1);
            goto done;
          }
          i++;
          flags += 128;
          other++;
          if (i < 64)
            goto scan;
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
