#include "jupiter_lighthouse_room_m.h"

extern s32 gData_02005a3c;

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Func_02004994(s32 id, s32 mode);
void Func_02004984(void);
void Func_0200498c(s32 a, s32 b, s32 c, s32 d);
void Func_0200474c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02004744(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02004844(s32 id, s32 a, s32 b);
void Func_02000c08(void);
void Func_02001e38(void);
void Func_02000cac(s32 value);
void Func_02001c7c(void);
void Func_02001ed0(void);
void Func_02000b48(void);

static __inline__ s32 GameFlag_Test_(s32 flag) {
  /* FAKEMATCH(wrapper): forwarded inline; as plain calls, 58 of 203
   * instructions differ in tla-en. */
  return GameFlag_Test(flag);
}

/// Sets up Jupiter Lighthouse room M: objects 8 to 15 take mode 1, objects 24
/// and 25 stand at heights 0 and 32, the cells flags 514 and 2591 opened are
/// stamped again, and with 2591 set the arrival is chosen by flags 2595, 2593
/// and 2596 and the entrance.
s32 Func_02002c54(void) {
  struct Actor *object;

  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 516;
  Func_02004994(8, 1);
  Func_02004994(9, 1);
  Func_02004994(10, 1);
  Func_02004994(11, 1);
  Func_02004994(12, 1);
  Func_02004994(13, 1);
  Func_02004994(14, 1);
  Func_02004994(15, 1);
  Func_02004984();
  Func_0200498c(0, 512, 21, 22);
  gData_02005a3c = 0;
  object = Actor_GetById(24);
  object->motionFlags = 0;
  object->y = 0;
  object->terrainHeight = 0;
  object = Actor_GetById(25);
  object->motionFlags = 0;
  object->y = 0x200000;
  object->terrainHeight = 0x200000;
  if (GameFlag_Test_(514)) {
    Func_0200474c(49, 106, 4, 3, 49, 76);
    Func_02004744(49, 45, 4, 1, 49, 44);
  }
  if (GameFlag_Test_(2591)) {
    Func_0200474c(49, 106, 4, 3, 49, 76);
    Func_02004744(49, 45, 4, 1, 49, 44);
    Func_02004844(23, 0, 0);
    if (GameFlag_Test_(2595)) {
      if (!GameFlag_Test_(2593)) {
        Func_02000c08();
        Func_02001e38();
      }
    } else if (!GameFlag_Test_(2596)) {
      if (gMainRoutineWork.entrance == 3) {
        Func_02000c08();
        GameFlag_SetBit(2596);
        Func_02000cac(gMainRoutineWork.entrance == 3);
      }
    } else {
      Func_02000c08();
      if (gMainRoutineWork.entrance == 5) {
        Func_02001c7c();
      } else if (gMainRoutineWork.entrance == 77) {
        Func_02001ed0();
        GameFlag_SetBit(2595);
        GameFlag_ClearBit(2596);
      } else {
        Func_02000b48();
      }
    }
  }
  Actor_GetById(23)->motionFlags = 0;
  if (GameFlag_Test_(2573))
    Func_02004844(23, 0, 0);
  if (gMainRoutineWork.entrance == 99)
    Func_02001ed0();
  return 0;
}
