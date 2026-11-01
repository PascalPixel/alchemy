// Draft, differ 10 of 197, skeleton gap 3 (tla-en, 2026-11-01)
// Overlay 6ad's copy of resource_698_c_020005e4.c, with this overlay's symbols,
// shared by thirteen overlays (698 to 6a4): the update of an object that
// carries the leader. While no event runs it animates, plays a sound every 32
// frames, takes the leader's sprite priority and moves along its angle; when
// the leader stands close enough (and flag 260 is clear) it links the leader to
// itself. Off its track it stops, takes its rest script and clears the ride.
// The priority is merged as signed bytes, not through FieldSprite's bitfield:
// only a char access has alias set 0, so the word loads after it wait a cycle
// and the cell lookup's 2 goes first; signed gives the mask as -13. 200 goes
// through a temporary so it is built with movs as an SImode constant (as a
// halfword constant it loads from the pool). The stop path's zero is one local
// so the update store lands just before the call. Left:
// - orrs r2, r3 for orrs r3, r2 in the merge, registers only; twenty spellings
//   tried, each write of the operands the other way round costs 12 more.
// - The second word of the speed read before the multiply's address: the
//   constant has no dependence on the Math_Cosine call, so it wins rank class.
//   Declared as a symbol with the long_call attribute it loads from the pool
//   and the order matches (differ 8, gap 2). That is a question of how fixed
//   addresses are routed, so it is not taken here.
// - The leader's link and angle stores before r1 is copied for the flags. The
//   same order needs a barrier in f0d59dc.c. The copy outranks the stores by
//   one and the load's read-modify-write rule keeps their edge at 1; statement
//   order, a flags pointer and that file's idiom change nothing.
#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_sprite.h"

extern const s32 gData_02007044[];
extern s32 Data_02007064;

struct Actor *Actor_GetById(s32 id);
void Actor_SetAnimationSpeed(struct Actor *object, s32 speed);
void Func_020056c0(s32 sound);
s32 Func_020056a0(s32 x, s32 z, s32 layer);
void Func_02005658(s32 *position);
s32 Func_02001868(s32 *from, s32 *to);
s32 GameFlag_Test(s32 flag);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
void Func_02005490(struct Actor *object, const s32 *script);

void Func_02001894(struct Actor *self) {
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
    Func_020056c0(231);
  {
    s8 *to = self->animation;
    s8 *from = leader->animation;
    to[9] = (from[9] & 12) | (to[9] & ~12);
  }
  if (Func_020056a0(self->x, self->z, 2) != 255) {
    position[0] = self->x;
    position[1] = self->y;
    position[2] = self->z;
    Func_02005658(position);
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
        if (Func_02001868(&leader->x, &self->x) <= 12 && distance < 0xc0000)
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
      self->x += Iwram_MulQ16(Math_Cosine(self->angle), self->speedLimit);
      self->z += Iwram_MulQ16(Math_Sine(self->angle), self->speedLimit);
      return;
    }
  }
stop:
  {
    s32 zero = 0;
    *((u8 *)self + 0x63) = zero;
    self->update = zero;
    Func_02005490(self, gData_02007044);
    Func_020056c0(228);
    Data_02007064 = zero;
  }
}
