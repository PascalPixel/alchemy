#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"

struct ObjectRuntime *Object_GetById(u32 id);
void Func_02005e14(void);
void Func_02005e1c(void);
void Func_02005f2c(s32 skip);
void Func_02005f94(s32 cue);
void ObjectMotion_SetSpeedParameters(u32 id, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndCommit(u32 id, s32 x, s32 z);

/// When the event work's warp number (0x154) is set, plays cue 186 and moves
/// the leader to that warp's place (warps 50 to 55), inside an event.
void Func_02004ab8(void) {
  s16 *warp;
  s32 x, z;

  x = 0;
  z = 0;
  warp = (s16 *)((u8 *)Ram_HeapSlots->eventWork + 0x154);
  if (*warp == 0)
    return;
  Func_02005e14();
  Func_02005f2c(0);
  Func_02005f94(186);
  Object_GetById(gPartyState.currentOwner)->actionFlags &= ~1;
  ObjectMotion_SetSpeedParameters(gPartyState.currentOwner, 0x40000, 0x20000);
  switch (*warp) {
  case 50:
    // XXX: the leader is looked up twice and neither result is used; likely
    // left from a position read, because the other warps give theirs as
    // constants.
    Object_GetById(gPartyState.currentOwner);
    Object_GetById(gPartyState.currentOwner);
    x = 148;
    z = 632;
    break;
  case 51:
    x = 264;
    z = 652;
    break;
  case 52:
    x = 504;
    z = 508;
    break;
  case 53:
    x = 552;
    z = 508;
    break;
  case 54:
    x = 616;
    z = 188;
    break;
  case 55:
    x = 648;
    z = 188;
    break;
  }
  ObjectMotion_SetPositionAndCommit(gPartyState.currentOwner, x, z);
  Object_GetById(gPartyState.currentOwner)->actionFlags |= 1;
  Func_02005e1c();
}
