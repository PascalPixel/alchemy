// Draft, differ 16 of 197: overlay 69b's copy of the function drafted in
// resource_698_c_020005e4.c (overlay 698's Func_020005e4, differ 16 there), whose
// header says what it is, what still differs and what was tried.
#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "object_runtime.h"
#include "adventure.h"
#include "field_sprite.h"

extern const s32 gData_02002b3c[];
extern s32 Data_02002b94;

struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetAnimationSpeed(struct ObjectRuntime *object, s32 speed);
void Func_0200232c(s32 sound);
s32 Func_020022fc(s32 x, s32 z, s32 layer);
void Func_020022e4(s32 *position);
s32 Func_02000d68(s32 *from, s32 *to);
s32 GameFlag_Test(s32 flag);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
void Func_020021c4(struct ObjectRuntime *object, const s32 *script);

void Func_02000d94(struct ObjectRuntime *self) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct ObjectRuntime *leader = Object_GetById(gAdventure.currentOwner);
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
    Func_0200232c(231);
  ((struct FieldSprite *)self->animation)->priority =
      ((struct FieldSprite *)leader->animation)->priority;
  if (Func_020022fc(self->x, self->z, 2) != 255) {
    position[0] = self->x;
    position[1] = self->y;
    position[2] = self->z;
    Func_020022e4(position);
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
        if (Func_02000d68(&leader->x, &self->x) <= 12 && distance < 0xc0000)
          near = 1;
        if (near && !GameFlag_Test(260)) {
          leader->angle = self->angle;
          leader->linkedObject = self;
          leader->priorityFlags &= ~1;
          {
            s32 delay = 200;
            *(u16 *)(work + 362) = delay;
          }
          gAdventure.renderMode = 0;
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
  Func_020021c4(self, gData_02002b3c);
  Func_0200232c(228);
  Data_02002b94 = 0;
}
