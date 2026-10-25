// Draft, differ 161 of 1091: Air's Rock's map setup, by room: sets the screen
// effect, the leader's priority bit 5 and flags 0x300 and 0x89f, then
// places what each room's flags have moved: the pushed pillars
// (Func_02005e6c, then Func_02005f6c with the room's pillar table), the
// pillars of light the Whirlwind and beam scenes leave (flags 0x899 and
// 0x89a: Func_02001084 or a dark palette), the opened walls and their cells
// (Func_02005d9c, Func_02005dac), the blending callbacks (Func_02004f3c,
// Func_02001690) and the rooms' scenes by entrance (Ov664Room1's entrance
// 11 runs Func_0200505c before flag 265, entrance 16 drops the leader in;
// Ov664Room2's entrance 3 Func_02004cc8). Returns 0.
// Left: the native saves only r5 and r6 and reloads gPartyState from the pool
// for each later entrance test in Ov664Room1, where the draft keeps it in r5
// across the calls (saving r7 and r8 too) and shares the tests' loads; the rest
// is branch offsets.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "call.h"

extern char SceneId_Ov664Room1, SceneId_Ov664Room2, SceneId_Ov664Room3,
    SceneId_Ov664Room4, SceneId_Ov664Room5, SceneId_Ov664Room6,
    SceneId_Ov664Room7, SceneId_Ov664Room8, SceneId_Ov664Room9;
extern const u8 gData_02006050[], gData_02006054[], gData_0200605a[],
    gData_02006064[], gData_02006068[], gData_02006072[], gData_0200607e[],
    gData_0200621c[];
struct ObjectRuntime *Object_GetById(u32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Func_02002770(void);
void WaitFrames(s32 frames);
void Func_02005d84(void);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Func_02004f3c(void);
void Func_02001690(void);
void Func_02004eb0(void);
void Func_02005e6c(s32 id, s32 x, s32 z);
void Func_02005f6c(const u8 *pillars);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
void Func_02001084(struct ObjectRuntime *object, s32 a, s32 b);
void Func_02001420(struct ObjectRuntime *object, s32 a, s32 b);
void Func_02005d9c(s32 x, s32 z, s32 width, s32 height, s32 a, s32 b);
void Func_02005dac(s32 srcX, s32 srcZ, s32 width, s32 height, s32 destX,
                   s32 destZ);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        u32 value);
void Func_0200505c(void);
void Func_02004cc8(void);
void Func_02005f4c(void);
void Func_02005f54(void);
void Func_02005f5c(s32 slot, s32 a, s32 b);
void Func_0200062c(s32 layer, s32 x, s32 z, s32 cell);
void Object_SetActionCallback(struct ObjectRuntime *object, s32 action);
void Func_02000908(const u8 *table);

#define REG_BLDCNT (*(volatile u16 *)0x04000050)
#define REG_BLDALPHA (*(volatile u16 *)0x04000052)

s32 Func_020051f8(void) {
  struct ObjectRuntime *object;
  s32 i, z;

  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 516;
  Object_GetById(gPartyState.currentOwner)->priorityFlags |= 32;
  GameFlag_SetBit(0x300);
  GameFlag_SetBit(0x89f);
  if (gPartyState.scene == (s32)&SceneId_Ov664Room1) {
    Func_02002770();
    if (gPartyState.entrance == 16 && !GameFlag_Test(265)) {
      Object_GetById(gPartyState.currentOwner)->terrainId = 1;
      WaitFrames(8);
      Func_02005d84();
      WaitFrames(1);
    }
    if (gPartyState.entrance <= 11 || gPartyState.entrance == 16) {
      Scheduler_AddOrUpdateCallback(Func_02004f3c, 0x480);
      REG_BLDALPHA = 0x1008;
      REG_BLDCNT = 0x3f10;
      Func_02004eb0();
    }
    if (gPartyState.entrance > 13) {
      if (GameFlag_Test(0xa8e))
        Call3(Func_02005e6c, 12, 0x3380000, 0x1480000);
      Func_02005f6c(gData_02006050);
    }
    Object_GetById(10)->motionFlags = 0;
    Object_GetById(10)->y = 0;
    Object_GetById(10)->priorityFlags = 2;
    ObjectMotion_SetActionVariant(10, 3);
    if (!GameFlag_Test(0x89a)) {
      Object_SetPartAttribute(Object_GetById(10), 6);
    } else {
      Func_02001084(Object_GetById(10), 8, 0);
      Object_SetPartAttribute(Object_GetById(11), 6);
      Call6(Func_02005d9c, 0x40, 78, 18, 105, 3, 5);
    }
    if (!GameFlag_Test(0x899)) {
      Object_SetPartAttribute(Object_GetById(8), 6);
      Object_SetPartAttribute(Object_GetById(9), 6);
      Object_SetPartAttribute(Object_GetById(12), 6);
    } else {
      Func_02001084(Object_GetById(8), 180, 0);
      Func_02001084(Object_GetById(9), 60, 0);
      Func_02001420(Object_GetById(12), 60, 0);
    }
    ObjectDispatch_SetSingleChildFlags(Object_GetById(8), 0);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(9), 0);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(12), 0);
    if (gPartyState.entrance == 11 && !GameFlag_Test(265))
      Func_0200505c();
  } else if (gPartyState.scene == (s32)&SceneId_Ov664Room2) {
    REG_BLDALPHA = 0x1008;
    REG_BLDCNT = 0x3f10;
    Func_02002770();
    if (GameFlag_Test(0x897))
      Call3(Func_02005e6c, 9, 0x1480000, 0x1880000);
    if (GameFlag_Test(0x898))
      Call3(Func_02005e6c, 11, 0x680000, 0x1880000);
    Func_02005f6c(gData_02006054);
    if (!GameFlag_Test(0x899)) {
      Object_SetPartAttribute(Object_GetById(8), 6);
    } else {
      Func_02001084(Object_GetById(8), 60, 0);
      Object_SetPartAttribute(Object_GetById(10), 6);
      Call6(Func_02005d9c, 68, 84, 12, 77, 3, 6);
    }
    ObjectDispatch_SetSingleChildFlags(Object_GetById(8), 0);
    if (gPartyState.entrance == 3 && !GameFlag_Test(265))
      Func_02004cc8();
  } else if (gPartyState.scene == (s32)&SceneId_Ov664Room3) {
    Func_02002770();
    Func_02005f4c();
    REG_BLDALPHA = 0xc08;
    REG_BLDCNT = 0x3f10;
    Scheduler_AddOrUpdateCallback(Func_02001690, 0x480);
    if (GameFlag_Test(0x89b))
      Call3(Func_02005e6c, 8, 0x1080000, 0x2780000);
    if (GameFlag_Test(0x89c))
      Call3(Func_02005e6c, 11, 0x2680000, 0x2380000);
    if (GameFlag_Test(0x89d))
      Call3(Func_02005e6c, 12, 0x1d80000, 0x2080000);
    Func_02005f6c(gData_0200605a);
    if (!GameFlag_Test(0x899)) {
      Object_SetPartAttribute(Object_GetById(9), 6);
      Object_SetPartAttribute(Object_GetById(10), 6);
      Object_SetPartAttribute(Object_GetById(13), 6);
    } else {
      Func_02001084(Object_GetById(9), 60, 0);
      Func_02001084(Object_GetById(10), 180, 0);
      Func_02001084(Object_GetById(13), 200, 0);
    }
    Object_GetById(9)->actionFlags = 0;
    Object_GetById(10)->actionFlags = 0;
    Object_GetById(13)->actionFlags = 0;
    ObjectDispatch_SetSingleChildFlags(Object_GetById(9), 0);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(10), 0);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(13), 0);
  } else if (gPartyState.scene == (s32)&SceneId_Ov664Room4) {
    Func_02005f54();
    Call3(Func_02005f5c, 0, 11, 12);
    Call3(Func_02005f5c, 1, 13, 14);
    Call3(Func_02005f5c, 2, 15, 0x10);
    Call3(Func_02005f5c, 3, 17, 18);
    Call3(Func_02005f5c, 4, 19, 20);
    ((u8 *)Object_GetById(18))[98] = 1;
    if (GameFlag_Test(769)) {
      Call6(Func_02005d9c, 69, 0x40, 29, 85, 5, 7);
      Call6(Func_02005dac, 46, 20, 5, 5, 29, 21);
      Call6(Func_02005dac, 0x20, 89, 1, 1, 31, 89);
      Call6(Func_02005dac, 50, 86, 1, 1, 33, 87);
    } else {
      Call6(Func_02005d9c, 0x40, 0x40, 29, 85, 5, 7);
      Call6(Func_02005dac, 52, 20, 5, 5, 29, 21);
      Call6(Func_02005dac, 33, 88, 1, 1, 33, 87);
      Call6(Func_02005dac, 0x30, 88, 1, 1, 31, 89);
    }
    if (GameFlag_Test(0x300)) {
      Call6(Func_02005d9c, 90, 25, 86, 25, 4, 3);
      Call6(Func_02005d9c, 21, 88, 24, 89, 1, 2);
      Call6(Func_02005dac, 23, 92, 2, 1, 23, 89);
      Call6(Func_02005dac, 18, 33, 2, 1, 23, 25);
    }
    if (GameFlag_Test(0x89e))
      Call3(Func_02005e6c, 9, 0x2880000, 0x1280000);
    Func_02005f6c(gData_02006064);
  } else if (gPartyState.scene == (s32)&SceneId_Ov664Room5) {
    GameFlag_SetBit(324);
    Func_02002770();
    for (i = 0; i != 6; i++) {
      if (GameFlag_Test(514 + i)) {
        z = 24 - (i << 1);
        Call6(Func_02005d9c, 65, 1, 80, z, 1, 2);
        Func_0200062c(0, 16, z, 0xff00);
      }
    }
    Object_GetById(8)->motionFlags = 0;
    object = Object_GetById(8);
    Object_GetById(8)->terrainHeight = 0x100000;
    object->y = 0x100000;
    Object_SetActionCallback(Object_GetById(8), 9);
  } else if (gPartyState.scene == (s32)&SceneId_Ov664Room6) {
    Func_02002770();
    Func_02005f54();
    Call3(Func_02005f5c, 0, 11, 12);
    Call3(Func_02005f5c, 1, 13, 14);
    Call3(Func_02005f5c, 2, 15, 0x10);
    if (!GameFlag_Test(770)) {
      Call6(Func_02005d9c, 69, 0x40, 0x20, 79, 5, 7);
      Call6(Func_02005dac, 49, 15, 5, 5, 0x20, 15);
      Call6(Func_02005dac, 33, 82, 1, 1, 34, 82);
      Call6(Func_02005dac, 52, 81, 1, 1, 36, 81);
    } else {
      Call6(Func_02005d9c, 0x40, 0x40, 0x20, 79, 5, 7);
      Call6(Func_02005dac, 49, 21, 5, 5, 0x20, 15);
      Call6(Func_02005dac, 35, 81, 1, 1, 36, 81);
      Call6(Func_02005dac, 50, 82, 1, 1, 34, 82);
    }
    if (GameFlag_Test(771))
      Call3(Func_02005e6c, 8, 0x2080000, 0x1280000);
    if (GameFlag_Test(0x89f)) {
      Call3(Func_02005e6c, 8, 0x1380000, 0x1280000);
      ObjectMotion_SetActionVariant(8, 2);
      Object_GetById(8)->motionFlags = 2;
      Object_GetById(8)->terrainHeight = 0xffe00000;
      Call6(Func_02005dac, 19, 0x10, 1, 1, 19, 18);
    }
  } else if (gPartyState.scene == (s32)&SceneId_Ov664Room7) {
    Func_02002770();
    ((u8 *)Object_GetById(12))[98] = 1;
    ((u8 *)Object_GetById(13))[98] = 1;
    ((u8 *)Object_GetById(14))[98] = 1;
    ((u8 *)Object_GetById(15))[98] = 1;
    if (!GameFlag_Test(265)) {
      GameFlag_SetBit(513);
      GameFlag_SetBit(514);
    }
    Func_02000908(gData_0200621c);
    if (GameFlag_Test(0x8d0))
      Call3(Func_02005e6c, 8, 0x1580000, 0x2b80000);
    Func_02005f6c(gData_02006068);
    if (GameFlag_Test(772)) {
      Call6(Func_02005d9c, 39, 82, 38, 82, 1, 3);
      Call6(Func_02005dac, 39, 82, 1, 1, 38, 82);
      Call6(Func_02005dac, 38, 19, 1, 1, 38, 18);
    }
    if (GameFlag_Test(773)) {
      Call6(Func_02005d9c, 39, 82, 38, 99, 1, 3);
      Call6(Func_02005dac, 38, 98, 1, 1, 38, 99);
      Call6(Func_02005dac, 38, 35, 1, 1, 38, 37);
    }
  } else if (gPartyState.scene == (s32)&SceneId_Ov664Room8) {
    Func_02005f4c();
    REG_BLDALPHA = 0xc08;
    REG_BLDCNT = 0x3f10;
    Scheduler_AddOrUpdateCallback(Func_02001690, 0x480);
    if (GameFlag_Test(0x8d1))
      Call3(Func_02005e6c, 8, 0x1180000, 0x1d80000);
    if (GameFlag_Test(0x8d2))
      Call3(Func_02005e6c, 9, 0x1680000, 0x3480000);
    if (GameFlag_Test(0xa8f)) {
      Call3(Func_02005e6c, 12, 0xb80000, 0x2680000);
      GameFlag_SetBit(0x200);
    }
    ObjectMotion_SetActionVariant(9, 2);
    Func_02005f6c(gData_02006072);
    Func_0200062c(2, Object_GetById(8)->x >> 20, Object_GetById(8)->z >> 20, 0);
    Func_0200062c(2, Object_GetById(9)->x >> 20, Object_GetById(9)->z >> 20, 0);
    if (!GameFlag_Test(0x899))
      Object_SetPartAttribute(Object_GetById(11), 6);
    else
      Func_02001084(Object_GetById(11), 8, 0);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(11), 0);
  } else if (gPartyState.scene == (s32)&SceneId_Ov664Room9) {
    Func_02002770();
    if (GameFlag_Test(0x8d3))
      Call3(Func_02005e6c, 9, 0x1680000, 0x1080000);
    Func_02005f6c(gData_0200607e);
    Func_0200062c(2, Object_GetById(10)->x >> 20, Object_GetById(10)->z >> 20,
                  0);
    Object_GetById(10)->actionFlags = 0;
    if (!GameFlag_Test(0x899))
      Object_SetPartAttribute(Object_GetById(10), 6);
    else
      Func_02001084(Object_GetById(10), 60, 0);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(10), 0);
    if (GameFlag_Test(774)) {
      Call6(Func_02005d9c, 15, 0x50, 15, 77, 2, 3);
      Call6(Func_02005dac, 15, 0x50, 1, 1, 15, 79);
      Call6(Func_02005dac, 17, 15, 2, 1, 15, 15);
    }
  }
  return 0;
}
