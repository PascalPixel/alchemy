// Draft, differ 72 of 236 (tla-en, 5 November 2026), plain loops and no device.
// Left: the hoisted address of position takes r8 here; the listing rebuilds it (add rN, sp, 4)
// at each use, the mark of a pseudo left without a register, so radius holds r11 and the frame is 16.
// The first read of stepX also hoists here, where the listing reads it in each arm.
#include "types.h"
#include "call.h"
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
s32 Func_02002800();
void Func_02005480(struct Actor *actor, s32 value);
s32 Func_020054d8(s32 layer, s32 x, s32 z);
s32 Func_020054f8(struct Actor *actor, struct SlideVector *position);
s32 Func_02005538(s32 *a, s32 b, struct SlideVector *c, s32 d);
void Func_02005560(void);
void Func_020055a8();
void Func_02005648(s32 a);
struct Actor *Actor_GetById(s32 id);
void wait(s32 frames);
void Func_02002954(void);
extern u8 gData_020023c4[];

void Func_0200297c(void) {
  struct SlideWork *work;
  struct Actor *actor;
  struct Actor *other;
  struct SlideVector position;
  s16 *stepX;
  s16 *stepZ;
  u8 *flags;
  s32 result;
  s32 i;
  s32 j;

  if (gMainRoutineWork.renderMode == 0) {
    Func_02002800(-10, 0);
    actor = Actor_GetById(gMainRoutineWork.currentOwner);
    work = (struct SlideWork *)(gData_020023c4 + 0x188);
    Func_02005560();
    Func_02005648(0);
    actor->update = Func_02002954;
    actor->angle = 0x4000;
    Func_02005480(actor, 49);
    while (1) {
      result = Func_020054d8(actor->terrainId, actor->x, actor->z);
      stepX = &work->stepX;
      stepZ = &work->stepZ;
      if (result == 7) {
        actor->velocityX = *stepX << 17;
        actor->velocityZ = *stepZ << 17;
        actor->acceleration = 0x1000;
        for (i = 0; i <= 9; i++) {
          position.x = actor->x + (*stepX << 19);
          position.y = actor->y;
          position.z = actor->z + (*stepZ << 19);
          if (Func_020054f8(actor, &position) != 0) {
            actor->velocityX = 0;
            actor->velocityZ = 0;
            break;
          }
          wait(1);
        }
        break;
      }
      position.x = actor->x + (*stepX << 19);
      position.y = actor->y;
      position.z = actor->z + (*stepZ << 19);
      result = Func_020054f8(actor, &position);
      if (result > 0)
        break;
      if (result < 0) {
        actor->angle = 0x4000;
        Call3((void (*)())Func_020055a8, gMainRoutineWork.currentOwner, 6, -1);
        break;
      }
      {
        s32 radius = actor->radius - 2;
        s32 x;
        s32 z;
        s32 snap;

        other = Ram_HeapSlots->scriptObjects;
        flags = &other->collisionFlags;
        for (j = 0; j <= 63; j++, flags += sizeof(struct Actor), other++) {
          if (other->script != NULL && (*flags & 1) != 0 && other != actor) {
            if (Func_02005538(&other->x, radius, &position,
                              other->radius - 2) >= 0) {
              wait(1);
              goto done;
            }
          }
        }
        x = actor->x += *stepX << 17;
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
        work->unk04 = 0;
        wait(1);
      }
    }
  done:
    actor->update = NULL;
    Animation_ApplyChildValues(actor, 0);
  }
  work->unk04 = 0;
}
