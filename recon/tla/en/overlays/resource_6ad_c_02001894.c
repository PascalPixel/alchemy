// Overlay 6ad's copy of resource_698_c_020005e4.c, with this overlay's symbols:
// differ 16 of 197. That draft's header follows. Draft, differ 16 of 197:
// shared by thirteen overlays (698 to 6a4): the update of an object that
// carries the leader. While no event runs it animates, plays a sound every 32
// frames, takes the leader's sprite priority and moves along its angle; when
// the leader stands close enough (and flag 260 is clear) it links the leader to
// itself. Off its track it stops, takes its rest script and clears the ride.
// 200 goes through a temporary so it is built with movs as an SImode constant
// (as a halfword constant it loads from the pool), and the update is cleared
// before the byte so both stores share one zero register. Left, all instruction
// order: the cell lookup's 2 after its x and z loads (an empty asm after the
// priority store gets x before z: 14), the third word of the position read
// before the buffer pointer is copied to r0, the leader's link stored after its
// angle, and the update store before the rest script's arguments. Permute 15
// minutes found nothing better.
#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "object_runtime.h"
#include "party_state.h"
#include "field_sprite.h"

extern const s32 gData_02007044[];
extern s32 Data_02007064;

struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetAnimationSpeed(struct ObjectRuntime *object, s32 speed);
void Func_020056c0(s32 sound);
s32 Func_020056a0(s32 x, s32 z, s32 layer);
void Func_02005658(s32 *position);
s32 Func_02001868(s32 *from, s32 *to);
s32 GameFlag_Test(s32 flag);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
void Func_02005490(struct ObjectRuntime *object, const s32 *script);

void Func_02001894(struct ObjectRuntime *self) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  s32 position[3];
  s32 distance;
  s32 near;

  if (*(s16 *)(work + 358) != 0 || *(s16 *)(work + 346) != 0 ||
      *(s16 *)(work + 350) != 0 || *(s16 *)(work + 360) != 0) {
    ObjectDispatch_SetAnimationSpeed(self, 0);
    return;
  }
  ObjectDispatch_SetAnimationSpeed(self, 16);
  if ((++*(u16 *)&self->action & 31) == 31)
    Func_020056c0(231);
  ((struct FieldSprite *)self->animation)->priority =
      ((struct FieldSprite *)leader->animation)->priority;
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
          leader->linkedObject = self;
          leader->priorityFlags &= ~1;
          {
            s32 delay = 200;
            *(u16 *)(work + 362) = delay;
          }
          gPartyState.renderMode = 0;
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
  self->update = 0;
  *((u8 *)self + 0x63) = 0;
  Func_02005490(self, gData_02007044);
  Func_020056c0(228);
  Data_02007064 = 0;
}
