#include "types.h"
#include "object_runtime.h"
#include "party_state.h"

extern s32 gData_02005af0;
struct ObjectRuntime *Object_GetById(s32 id);
struct ObjectRuntime *Func_02000624(struct ObjectRuntime *origin, s32 id);
void Func_020006ac(struct ObjectRuntime *object, s32 dx, s32 dz, s32 velocity);
s32 Func_0200584c(void);
void Func_020056b4(struct ObjectRuntime *object);
void Func_020056bc(void);
void Func_0200580c(s32 value);
void Func_020057a4(s32 speed, s32 acceleration);
void Func_02005714(s32 id, s32 x, s32 z);
void Func_0200578c(s32 id, s32 a, s32 b);
void Func_02005854(s32 sound);
void Battle_WaitMode0(s32 frames);
void GameFlag_SetBit(s32 flag);
void Motion_CamBounds(s32 x, s32 y, s32 z, s32 mode);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndReset(s32 id, s32 x, s32 z);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 frames);
void ObjectMotion_SetAngleToward(s32 actor, s32 target, s32 frames);
void Object_LinkObjectAndSetCallback(s32 actor, s32 target);
void Object_AttachWorkTargetToObject(s32 actor, s32 value);

void Func_02000c38(void) {
  s32 *speed = &gData_02005af0;

  if (*speed != 0) {
    struct ObjectRuntime *actor = Object_GetById(Func_0200584c());
    struct ObjectRuntime *found = Func_02000624(actor, 359);
    s32 dx = *speed << 20;

    if (found != NULL) {
      u16 facing = *(u16 *)((u8 *)found + 6);

      /* FAKEMATCH: ⚓️ reads the facing before it builds the half turn */
      asm volatile("");
      if (facing == 0x8000)
        dx = -dx;
      facing = *(u16 *)((u8 *)actor + 6);
      if ((facing == 0 && dx > 0) || (facing == 0x8000 && dx < 0))
        Func_020006ac(actor, dx, 0, 0x40000);
    }
  }
}

void Func_02000c8c(void) {
  s32 *speed = &gData_02005af0;

  if (*speed != 0) {
    struct ObjectRuntime *actor = Object_GetById(Func_0200584c());
    struct ObjectRuntime *found = Func_02000624(actor, 360);
    s32 dz = *speed << 20;
    u16 facing;

    if (found == NULL) {
      found = Func_02000624(actor, 361);
      dz = -dz;
      if (found == NULL)
        return;
    }
    facing = *(u16 *)((u8 *)actor + 6);
    /* FAKEMATCH: ⚓️ reads the facing before it builds the quarter turn */
    asm volatile("");
    if ((facing == 0x4000 && dz > 0) || (facing == 0xc000 && dz < 0))
      Func_020006ac(actor, 0, dz, 0x40000);
  }
}

static __inline__ void Actor_SetSpeed(s32 id, s32 speed, s32 acceleration) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  ObjectMotion_SetSpeedParameters(id, speed, acceleration);
}

static __inline__ void Actor_Walk(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  ObjectMotion_SetPositionAndReset(id, x, z);
}

static __inline__ void Actor_Turn(s32 id, s32 angle, s32 frames) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  ObjectMotion_ArmCallback(id, angle, frames);
}

static __inline__ void Camera_SetSpeed(s32 speed, s32 acceleration) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_020057a4(speed, acceleration);
}

static __inline__ void Camera_SetBounds(s32 x, s32 y, s32 z, s32 mode) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Motion_CamBounds(x, y, z, mode);
}

void Func_02000cf0(struct ObjectRuntime *object) {
  Func_020056b4(object);
  Func_0200580c(0);
  Camera_SetSpeed(0x10000, 0x2000);
  Func_02005714(39, 164 << 17, 148 << 17);
  Battle_WaitMode0(30);
  Actor_Turn(gPartyState.currentOwner, 0x2000, 0);
  Battle_WaitMode0(15);
  Object_LinkObjectAndSetCallback(gPartyState.currentOwner, 39);
  Func_0200578c(gPartyState.currentOwner, 257, 30);
  Object_AttachWorkTargetToObject(39, 1);
  Battle_WaitMode0(30);
  Object_GetById(39)->priorityFlags |= 0x20;
  Object_SetModeById(39, 1);
  Actor_Walk(39, 320, 240);
  Actor_Walk(39, 304, 216);
  Actor_Walk(39, 280, 216);
  Actor_Walk(39, 280, 184);
  Battle_WaitMode0(20);
  ObjectMotion_SetAngleToward(39, gPartyState.currentOwner, 0);
  Battle_WaitMode0(10);
  Func_02005854(262);
  {
    s32 a = 256, b = 30;

    /* FAKEMATCH: the native sets the id last; called plainly or through an
     * inline wrapper the id is set before the 30 (one reordered pair, differ
     * 2). */
    asm volatile("" : "+l"(a), "+l"(b));
    Func_0200578c(39, a, b);
  }
  Actor_Walk(39, 344, 184);
  Actor_Walk(39, 344, 200);
  Battle_WaitMode0(2);
  Actor_Turn(39, 0x8000, 0);
  Battle_WaitMode0(15);
  Func_02005854(152);
  Object_GetById(39)->velocityY = 0x40000;
  Actor_SetSpeed(39, 0x40000, 0x20000);
  Actor_Walk(39, 312, 200);
  Func_02005854(152);
  Object_GetById(39)->velocityY = 0x60000;
  Actor_SetSpeed(39, 0x40000, 0x20000);
  Actor_Walk(39, 312, 168);
  Actor_SetSpeed(39, 0x20000, 0x10000);
  Actor_Walk(39, 328, 152);
  Camera_SetBounds(-1, -1, -1, 0);
  Battle_WaitMode0(15);
  Func_02005854(152);
  Object_GetById(39)->velocityY = 0x60000;
  Actor_SetSpeed(39, 0x40000, 0x20000);
  Actor_Walk(39, 360, 152);
  Battle_WaitMode0(15);
  Func_02005854(152);
  Object_GetById(39)->velocityY = 0x60000;
  Actor_SetSpeed(39, 0x20000, 0x10000);
  Actor_Walk(39, 360, 184);
  Actor_SetSpeed(39, 0x20000, 0x10000);
  Actor_Walk(39, 376, 184);
  Actor_Walk(39, 392, 200);
  Actor_Walk(39, 408, 184);
  Battle_WaitMode0(15);
  Func_02005854(152);
  Object_GetById(39)->velocityY = 0x60000;
  Actor_SetSpeed(39, 0x40000, 0x20000);
  Actor_Walk(39, 440, 184);
  Func_02005714(39, 0, 0);
  GameFlag_SetBit(0xa6c);
  Func_020056bc();
}
