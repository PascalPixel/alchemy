// Draft, differ 49 of 197: overlay 69e's copy of the function drafted in
// resource_698_c_020005e4.c (overlay 698's Func_020005e4, differ 49 there), whose
// header says what it is, what still differs and what was tried.
#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "object_runtime.h"
#include "party_state.h"
#include "field_sprite.h"

extern const s32 gData_020033e0[];
extern s32 gData_02003400;

struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetAnimationSpeed(struct ObjectRuntime *object, s32 speed);
void Func_02002a40(s32 sound);
s32 Func_02002a20(s32 x, s32 z, s32 layer);
void Func_020029b0(s32 *position);
s32 Func_02001044(s32 *from, s32 *to);
s32 GameFlag_Test(s32 flag);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
void Func_02002820(struct ObjectRuntime *object, const s32 *script);

void Func_02001070(struct ObjectRuntime *self) {
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
    Func_02002a40(231);
  ((struct FieldSprite *)self->animation)->priority =
      ((struct FieldSprite *)leader->animation)->priority;
  if (Func_02002a20(self->x, self->z, 2) != 255) {
    position[0] = self->x;
    position[1] = self->y;
    position[2] = self->z;
    Func_020029b0(position);
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
        if (Func_02001044(&leader->x, &self->x) <= 12 && distance < 0xc0000)
          near = 1;
        if (near && !GameFlag_Test(260)) {
          leader->angle = self->angle;
          leader->linkedObject = self;
          leader->priorityFlags &= ~1;
          *(u16 *)(work + 362) = 200;
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
  *((u8 *)self + 0x63) = 0;
  self->update = 0;
  Func_02002820(self, gData_020033e0);
  Func_02002a40(228);
  gData_02003400 = 0;
}
