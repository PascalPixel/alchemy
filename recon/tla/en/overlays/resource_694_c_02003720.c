// Draft, differ 23 of 136: everything matches but the first two flag stores.
// The native forms each address from the object (adds r1, r5; movs r3, #1; adds
// r1, #98; strb r3, [r1]; adds r3, r5; adds r3, #99; movs r2, #0; strb r2,
// [r3]) where this form steps the first address (adds r2, #1) as the native
// itself does for object 15 further down, and it loads the callback before it
// starts flag 0xa21. A one-pass block or an empty asm between the two stores
// measured 21 to 23.
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
void WaitFrames(s32 frames);
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
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_020040d4(actor, x, z);
}

static __inline__ void Func_020041a4_(s32 a, s32 x, s32 y, s32 z) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_020041a4(a, x, y, z);
}

void Func_02003720(void) {
  struct ObjectRuntime *object = Object_GetById(14);

  Scheduler_AddOrUpdateCallback(Func_020036f0, 0x480);
  BYTE_AT(object, 98) = 1;
  BYTE_AT(object, 99) = 0;
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
    WaitFrames(1);
  }
  Func_02000038(gData_0200469c, gData_020046ac, gData_020046d8, gData_02004704);
}
