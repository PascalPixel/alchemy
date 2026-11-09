#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

extern const u8 gData_0200469c[];
extern const u8 gData_020046ac[];
extern const u8 gData_020046d8[];
extern const u8 gData_02004704[];

void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Func_02000038(const u8 *a, const u8 *b, const u8 *c, const u8 *d);
s32 Func_020006d0(struct Actor *object);
void Func_02001724(void);
void Func_020036f0(void);
void Func_02003ffc(void);

/// Schedules Func_020036f0 and gives objects 14 and 15 their flag bytes and
/// Func_020006d0 as their update (object 15 brought in and placed while flag
/// 0xa21 is set and 0x9b2 clear), places or hides object 17 by flags 0x9e7 and
/// 0x9e6, sets flag 0x200 on entrances 1 to 4 unless flag 0x109 is set, and
/// while flag 0x200 is set has the camera follow the leader; it ends with
/// Func_02000038.
void Func_02003720(void) {
  struct Actor *object = Actor_GetById(14);
  s32 zero;

  Task_Add(Func_020036f0, 0x480);
  /* FAKEMATCH(temporary): the second byte's 0 built after the first
   * byte's address; as a plain 0, 21 of 136 instructions differ in tla-en. */
  zero = 0;
  BYTE_AT(object, 98) = 1;
  BYTE_AT(object, 99) = zero;
  *(void **)((u8 *)object + 108) = Func_020006d0;
  if (GameFlag_Test(0xa21) != 0 && GameFlag_Test(0x9b2) == 0) {
    Actor_Stop(15);
    Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(13, 0, 0);
    Func_02001724();
  } else {
    object = Actor_GetById(15);
    BYTE_AT(object, 98) = 0;
    BYTE_AT(object, 99) = 0;
    *(void **)((u8 *)object + 108) = Func_020006d0;
  }
  if (GameFlag_Test(0x9e7) != 0 && GameFlag_Test(0x9e6) == 0) {
    Actor_SetPosition(17, PIXELS(32), PIXELS(708));
  } else {
    Actor_SetPartAttribute(Actor_GetById(17), 15);
    Actor_SetSingleChildFlags(Actor_GetById(17), 0);
  }
  if (GameFlag_Test(0x109) == 0) {
    switch (gMainRoutineWork.entrance) {
    case 1:
    case 2:
    case 3:
    case 4:
      GameFlag_SetBit(0x200);
      break;
    default:
      Field_SetCameraBounds(0, 0x2000000, 0x2000000, 0x4000000);
      break;
    }
  }
  if (GameFlag_Test(0x200) != 0) {
    Field_SetCameraBounds(0, 0x2000000, 0x3700000, 0x4000000);
    Camera_FollowActor(gMainRoutineWork.currentOwner, 0);
    Func_02003ffc();
    wait(1);
  }
  Func_02000038(gData_0200469c, gData_020046ac, gData_020046d8, gData_02004704);
}
