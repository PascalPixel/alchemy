#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_sprite.h"

extern const s32 gData_02005a1c[];
extern s32 gData_02005a3c;

struct Actor *Actor_GetById(s32 id);
void Actor_SetAnimationSpeed(struct Actor *object, s32 speed);
void Func_020049c4(s32 sound);
s32 Func_020049a4(s32 x, s32 z, s32 layer);
void Func_02004954(s32 *position);
s32 Func_02003288(s32 *from, s32 *to);
s32 GameFlag_Test(s32 flag);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
void Func_02004704(struct Actor *object, const s32 *script);

void Func_020032b4(struct Actor *self) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 position[3];
  s32 distance;
  s32 near;

  if (*(s16 *)(work + 358) != 0 || *(s16 *)(work + 346) != 0 ||
      *(s16 *)(work + 350) != 0 || *(s16 *)(work + 360) != 0) {
    Actor_SetAnimationSpeed(self, 0);
    return;
  }
  Actor_SetAnimationSpeed(self, 16);
  if ((++*(u16 *)&self->action & 31) == 31)
    Func_020049c4(231);
  {
    s8 *to = self->animation;
    s8 *from = leader->animation;
    u32 a = from[9] & 12;
    to[9] = (s8)((to[9] & ~12) | a);
  }
  if (Func_020049a4(self->x, self->z, 2) != 255) {
    position[0] = self->x;
    position[1] = self->y;
    position[2] = self->z;
    Func_02004954(position);
    distance = position[0];
    if (distance > 0x1100000)
      goto stop;
    if (distance < -0x200000)
      goto stop;
    {
      if (*((u8 *)self + 0x62) == 0) {
        distance = leader->y - self->y;
        if (distance < 0)
          distance = self->y - leader->y;
        near = 0;
        if (Func_02003288(&leader->x, &self->x) <= 12 && distance < 0xc0000)
          near = 1;
        if (near && !GameFlag_Test(260)) {
          leader->angle = self->angle;
          leader->watchedObject = self;
          leader->priorityFlags &= ~1;
          {
            s32 delay = 200;
            *(u16 *)(work + 362) = delay;
          }
          gMainRoutineWork.renderMode = 0;
          *((u8 *)self + 0x62) = 1;
          *((u8 *)self + 0x63) = 1;
        }
      }
      {
        s32 (*multiply)(s32, s32);
        s32 cosine = Math_Cosine(self->angle);
        s32 speed = self->speedLimit;
        /* FAKEMATCH(one-pass): the routine address taken inside a once-only
         * block, so the speed loads first; as a plain call, 2 of 197
         * instructions differ in tla-en. */
        do {
          multiply = Iwram_MulQ16;
        } while (0);
        self->x += multiply(cosine, speed);
      }
      self->z += Iwram_MulQ16(Math_Sine(self->angle), self->speedLimit);
      return;
    }
  }
stop:
  {
    s32 zero = 0;
    *((u8 *)self + 0x63) = zero;
    self->update = zero;
    Func_02004704(self, gData_02005a1c);
    Func_020049c4(228);
    gData_02005a3c = zero;
  }
}
