#include "airs_rock_inside.h"

void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndCommit(s32 actor, s32 x, s32 z);

/// While the leader is no further than z 653, moves the leader to (376, 668)
/// with bit 0 of its byte 90 cleared for the move.
void Func_02003b08(void) {
  u8 *flags;
  u8 bit;

  if (((s16 *)&Object_GetById(gPartyState.currentOwner)->z)[1] > 653)
    return;
  Func_02005e14();
  Func_02005f2c(0);
  Func_02005f94(186);
  BYTE_AT(Object_GetById(gPartyState.currentOwner), 90) &= ~1;
  ObjectMotion_SetSpeedParameters(gPartyState.currentOwner, 0x40000, 0x20000);
  ObjectMotion_SetPositionAndCommit(gPartyState.currentOwner, 376, 668);
  flags = &BYTE_AT(Object_GetById(gPartyState.currentOwner), 90);
  /* FAKEMATCH: the bit as a u8 local is built in the register it ORs into, as
   * the native does; as |= 1 the byte loads there instead (differ 4, also as an
   * s32 local). */
  bit = 1;
  *flags = bit | *flags;
  Func_02005e1c();
}
