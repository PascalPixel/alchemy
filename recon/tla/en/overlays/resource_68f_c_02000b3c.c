// Draft, differ 2 of 75: for the emote call (Func_020030c8, overlay 694's
// Func_0200417c) the native builds the 0x102 half (movs r1, #129) before it
// loads the owner; here the owner is loaded first. Tried: a direct call, Call2,
// prototypes with u32, u16 and s16 parameters, permute 1 minute. The -1
// arguments of Camera_MoveWithinMapBounds need Call4.
#include "../../../../games/tla/src/scene/tundaria_tower/tundaria_tower.h"

void Camera_MoveWithinMapBounds(s32 a, s32 b, s32 c, s32 d);
void Object_SetModeById(u32 id, s32 mode);
void Object_SetActionById(u32 id, s32 action);
void Motion_SetVarCbAndRefresh(u32 id, s32 variant);
void FieldEvent_WaitFrames(s32 frames);
void Func_020030c8(s32 id, s32 emote);

static __inline__ void TundariaTower_Emote(s32 id, s32 emote) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer. */
  Func_020030c8(id, emote);
}
void Func_02003030(s32 x, s32 z, s32 a, s32 b);
void Func_020030e8(s32 a);

void Func_02000b3c(void) {
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  s32 zero;

  Func_02003048();
  Func_02003148(0);
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.currentOwner),
                                     0);
  zero = 0;
  leader->angle = 0x4000;
  FieldEvent_WaitFrames(10);
  Object_SetModeById(gPartyState.currentOwner, 22);
  FieldEvent_WaitFrames(30);
  TundariaTower_Emote(gPartyState.currentOwner, 0x102);
  Motion_SetVarCbAndRefresh(gPartyState.currentOwner, 2);
  FieldEvent_WaitFrames(20);
  leader->angle = 0xc000;
  Object_SetModeById(gPartyState.currentOwner, 5);
  Object_SetActionById(gPartyState.currentOwner, 24);
  FieldEvent_WaitFrames(40);
  Func_02003030(0x1300000, 0x3900000, 0, -12);
  Object_GetById(gPartyState.currentOwner)->bounce = zero;
  Func_020030e8(11);
  Func_02003050();
}
