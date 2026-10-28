#include "tundaria_tower.h"

void Camera_MoveWithinMapBounds(s32 a, s32 b, s32 c, s32 d);
void Actor_SetModeById(u32 id, s32 mode);
void Actor_SetActionById(u32 id, s32 action);
void Motion_SetVarCbAndRefresh(u32 id, s32 variant);
void FieldEvent_Wait(s32 frames);
void Func_020030c8(s32 id, s32 emote);
void Func_02003030(s32 x, s32 z, s32 a, s32 b);
void Func_020030e8(s32 a);

/// The leader turns to angle 0x4000 and takes mode 22, shows emote 0x102, then
/// turns to 0xc000 and takes mode 5 and action 24.
void Func_02000b3c(void) {
  struct Actor *leader = Actor_GetById(gAdventure.currentOwner);

  Func_02003048();
  Func_02003148(0);
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Actor_SetSingleChildFlags(Actor_GetById(gAdventure.currentOwner),
                                     0);
  leader->angle = 0x4000;
  FieldEvent_Wait(10);
  Actor_SetModeById(gAdventure.currentOwner, 22);
  FieldEvent_Wait(30);
  Func_020030c8(gAdventure.currentOwner, 0x102);
  Motion_SetVarCbAndRefresh(gAdventure.currentOwner, 2);
  FieldEvent_Wait(20);
  {
    s32 mode = 5;

    leader->angle = 0xc000;
    /* FAKEMATCH(barrier): the barrier builds the mode after the angle store and
     * before the owner's load; as Actor_SetModeById(owner, 5), 2 of 75
     * instructions differ in tla-en. */
    asm("" : "+r"(mode) : : "memory");
    Actor_SetModeById(gAdventure.currentOwner, mode);
  }
  Actor_SetActionById(gAdventure.currentOwner, 24);
  FieldEvent_Wait(40);
  Func_02003030(0x1300000, 0x3900000, 0, -12);
  Actor_GetById(gAdventure.currentOwner)->bounce = 0;
  Func_020030e8(11);
  Func_02003050();
}
