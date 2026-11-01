// Draft, differ 136 of 236, skeleton gap 18/24 (tla-en, 2026-11-01)
// Overlay 6ac's Func_020024e4. Control flow and block order are the listing's;
// all three loops are goto loops, since real loops move the exit blocks to the
// end of the function (loop.c). Func_02002368 and Func_02003200 stay
// unprototyped: that gives -10's argument order. Left:
// - gcse's PRE lifts work + 104, work + 106 and the frame address out of the
//   frame loop; the listing rebuilds them each pass, with work spilled to
//   sp+0. The base/actor and work/data copies at the top of the loop stop it
//   for actor + 34 and move the rest, but cost a register, so i is spilled
//   instead of work, and work is rematerialized from the pool (it has one set,
//   so it is REG_EQUIV) where the listing stores it to the stack.
// - The slide loop's test threads to bgt/b, one extra branch, and a do-while
//   instead moves its exit block out of line.
// Tried without gain: position through a pointer (several placements), no
// base or no data (gap 19/31 and worse), a per-pass copy after the call.
#include "types.h"
#include "actor.h"
#include "system/ram_buffer.h"
#include "system/main_routine_work.h"

struct SlideVector {
  s32 x;
  s32 y;
  s32 z;
};

struct SlideWork {
  u8 unk00[4];
  u16 unk04;
  u8 unk06[0x62];
  s16 stepX;
  s16 stepZ;
};

void Animation_ApplyChildValues(struct Actor *actor, s32 value);
s32 Func_02002368();
void Func_02003118(struct Actor *actor, s32 value);
s32 Func_02003168(s32 layer, s32 x, s32 z);
s32 Func_02003180(struct Actor *actor, struct SlideVector *position);
s32 Func_020031b0(s32 *a, s32 b, struct SlideVector *c, s32 d);
void Func_020031e0(void);
s32 Func_02003200();
void Func_02003280(s32 a);
struct Actor *Actor_GetById(s32 id);
void wait(s32 frames);
void Func_020024bc(void);
extern u8 gData_020023c4[];

void Func_020024e4(void) {
  struct SlideWork *work;
  struct SlideWork *data;
  struct Actor *actor;
  struct Actor *other;
  struct Actor *base;
  struct SlideVector position;
  s16 *stepX;
  s16 *stepZ;
  u8 *flags;
  s32 result;
  s32 i;

  if (gMainRoutineWork.renderMode == 0) {
    Func_02002368(-10, 0);
    base = Actor_GetById(gMainRoutineWork.currentOwner);
    work = (struct SlideWork *)(gData_020023c4 + 0x188);
    Func_020031e0();
    Func_02003280(0);
    base->update = Func_020024bc;
    base->angle = 0x4000;
    Func_02003118(base, 49);
  top:
    actor = base;
    result = Func_02003168(actor->terrainId, actor->x, actor->z);
    data = work;
    stepX = &data->stepX;
    stepZ = &data->stepZ;
    if (result == 7) {
      actor->velocityX = *stepX << 17;
      actor->velocityZ = *stepZ << 17;
      actor->acceleration = 0x1000;
      i = 0;
    sliding:
        position.x = actor->x + (*stepX << 19);
        position.y = actor->y;
        position.z = actor->z + (*stepZ << 19);
        if (Func_02003180(actor, &position) != 0) {
          actor->velocityX = 0;
          actor->velocityZ = 0;
          goto done;
        }
        i++;
        wait(1);
        if (i <= 9)
          goto sliding;
      goto done;
    }
    {
      position.x = actor->x + (*stepX << 19);
      position.y = actor->y;
      position.z = actor->z + (*stepZ << 19);
      result = Func_02003180(actor, &position);
      if (result <= 0) {
        if (result < 0) {
          actor->angle = 0x4000;
          Func_02003200(gMainRoutineWork.currentOwner, 6, -1);
        } else {
          s32 radius = actor->radius - 2;
          s32 x;
          s32 z;
          s32 snap;

          other = Ram_HeapSlots->scriptObjects;
          flags = &other->collisionFlags;
          i = 0;
        scan:
            if (other->script != NULL && (*flags & 1) != 0 &&
                other != actor) {
              if (Func_020031b0(&other->x, radius, &position,
                                other->radius - 2) >= 0) {
                wait(1);
                goto done;
              }
            }
          i++;
          flags += sizeof(struct Actor);
          other++;
          if (i <= 63)
            goto scan;
          x = actor->x + (*stepX << 17);
          actor->x = x;
          z = actor->z + (*stepZ << 17);
          actor->z = z;
          snap = (x & 0xfffff) - 0x80000;
          if (snap > 0x10000)
            snap = 0x10000;
          if (snap < -0x10000)
            snap = -0x10000;
          actor->x = x - snap;
          snap = (z & 0xfffff) - 0x80000;
          if (snap > 0x10000)
            snap = 0x10000;
          if (snap < -0x10000)
            snap = -0x10000;
          actor->z = z - snap;
          data->unk04 = 0;
          wait(1);
          goto top;
        }
      }
    }
  done:
    actor->update = NULL;
    Animation_ApplyChildValues(actor, 0);
  }
  data->unk04 = 0;
}
