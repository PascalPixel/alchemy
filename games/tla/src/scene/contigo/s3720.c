#include "types.h"
#include "object_runtime.h"
#include "party_state.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

extern const u8 gData_0200469c[];
extern const u8 gData_020046ac[];
extern const u8 gData_020046d8[];
extern const u8 gData_02004704[];

struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void WaitTicks(s32 ticks);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void ObjectMotion_EnableActionAndResetMotion(s32 actor);
void Camera_FollowActor(s32 actor, s32 mode);
void Func_02000038(const u8 *a, const u8 *b, const u8 *c, const u8 *d);
s32 Func_020006d0(struct ObjectRuntime *object);
void Func_02001724(void);
void Func_020036f0(void);
void Func_02003ffc(void);
void Func_020040d4(s32 actor, s32 x, s32 z);
void Func_020041a4(s32 a, s32 x, s32 y, s32 z);

static __inline__ void Func_020040d4_(s32 actor, s32 x, s32 z) {
  /* FAKEMATCH(wrapper): forwarding keeps the call's own constants; called
   * directly, 2 of 136 instructions differ in tla-en. */
  Func_020040d4(actor, x, z);
}

static __inline__ void Func_020041a4_(s32 a, s32 x, s32 y, s32 z) {
  /* FAKEMATCH(wrapper): forwarding keeps each call's own constants; called
   * directly, 27 of 136 instructions differ in tla-en. */
  Func_020041a4(a, x, y, z);
}

/// Schedules Func_020036f0 and gives objects 14 and 15 their flag bytes and
/// Func_020006d0 as their update (object 15 brought in and placed while flag
/// 0xa21 is set and 0x9b2 clear), places or hides object 17 by flags 0x9e7 and
/// 0x9e6, sets flag 0x200 on entrances 1 to 4 unless flag 0x109 is set, and
/// while flag 0x200 is set has the camera follow the leader; it ends with
/// Func_02000038.
void Func_02003720(void) {
  struct ObjectRuntime *object = Object_GetById(14);
  s32 zero;

  Scheduler_AddOrUpdateCallback(Func_020036f0, 0x480);
  /* FAKEMATCH(temporary): the second byte's 0 built after the first
   * byte's address; as a plain 0, 21 of 136 instructions differ in tla-en. */
  zero = 0;
  BYTE_AT(object, 98) = 1;
  BYTE_AT(object, 99) = zero;
  *(void **)((u8 *)object + 108) = Func_020006d0;
  if (GameFlag_Test(0xa21) != 0 && GameFlag_Test(0x9b2) == 0) {
    ObjectMotion_EnableActionAndResetMotion(15);
    Func_020040d4(15, 0, 0);
    Func_020040d4(13, 0, 0);
    Func_02001724();
  } else {
    object = Object_GetById(15);
    BYTE_AT(object, 98) = 0;
    BYTE_AT(object, 99) = 0;
    *(void **)((u8 *)object + 108) = Func_020006d0;
  }
  if (GameFlag_Test(0x9e7) != 0 && GameFlag_Test(0x9e6) == 0) {
    Func_020040d4_(17, 0x200000, 0x2c40000);
  } else {
    Object_SetPartAttribute(Object_GetById(17), 15);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(17), 0);
  }
  if (GameFlag_Test(0x109) == 0) {
    switch (gPartyState.entrance) {
    case 1:
    case 2:
    case 3:
    case 4:
      GameFlag_SetBit(0x200);
      break;
    default:
      Func_020041a4_(0, 0x2000000, 0x2000000, 0x4000000);
      break;
    }
  }
  if (GameFlag_Test(0x200) != 0) {
    Func_020041a4_(0, 0x2000000, 0x3700000, 0x4000000);
    Camera_FollowActor(gPartyState.currentOwner, 0);
    Func_02003ffc();
    WaitTicks(1);
  }
  Func_02000038(gData_0200469c, gData_020046ac, gData_020046d8, gData_02004704);
}
