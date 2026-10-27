#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "call.h"

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
struct ObjectRuntime *Object_GetById(s32 id);
void ObjectMotion_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 id);
void FieldEvent_WaitTicks(s32 ticks);
s32 Func_02004dec(s32 a, s32 x, s32 z);
void Func_02004dfc(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                   s32 destY);
void Func_02004e5c(void);
void Func_02004e64(void);
void Func_02004f34(s32 id, s32 emote);
void Func_02004f6c(s32 value);
void Func_02004fcc(s32 value);
void Func_02004fdc(void);
void Func_02004ff4(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);

/// Unless Func_02004dec(2, ...) holds, copies map cell (36, 23) to (38, 25),
/// moves a leader standing on that cell aside, and sets flag 0x30a, or 0x9bf
/// once flag 0x927 is set.
void Func_02000b6c(void) {
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);

  Call7(Func_02004ff4, 37, 26, 0, 3, 3, 61, 0);
  if (Value3(Func_02004dec, 2, 152 << 18, 200 << 17) == 0) {
    Call6(Func_02004dfc, 36, 23, 1, 1, 38, 25);
    Func_02004e5c();
    Func_02004fcc(0);
    if (leader->x >> 20 == 38 && leader->z >> 20 == 25) {
      Call2(Func_02004f34, gPartyState.currentOwner, 258);
      ObjectMotion_OffsetPositionAndResetMotion(gPartyState.currentOwner, 16,
                                                0);
      leader->velocityY = 0x60000;
      ObjectMotion_CommitCurrentPositionAndActivate(gPartyState.currentOwner);
    }
    if (!GameFlag_Test(0x927)) {
      GameFlag_SetBit(0x30a);
      FieldEvent_WaitTicks(30);
      Func_02004fdc();
      Func_02004f6c(20);
    } else {
      GameFlag_SetBit(0x9bf);
      Func_02004e64();
    }
  }
}
