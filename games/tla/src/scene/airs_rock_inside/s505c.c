/// @file
/// Scene code of overlay 0x664, in ROM order, from s505c's offset up to the next object the overlay links.

#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "system/field_effect.h"
#include "system/vector3.h"
#include "call.h"
#include "field.h"

struct Actor *Actor_GetById(u32 id);
void Func_02005e14(void);
void Func_02005e1c(void);
void Func_02005f2c(s32 skip);
void Func_02005f94(s32 cue);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_02005d84(void);
void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
u32 Random16Far(void);
void Effect_AdvanceMotion(union FieldObject *object);
void Func_020002f4(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);
void Func_02005ec4(u32 id, s32 mode);
void Func_02005dcc(s32 a, s32 b, s32 c);
void Func_02005dd4(void);

void Func_0200505c(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct EffectOptions options;
  u32 i;

  Func_02005e14();
  Func_02005f2c(0);
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Func_02005d84();
  wait(1);
  leader->y = 0x820000;
  leader->gravity = 0x8000;
  leader->bounce = 0;
  leader->motionFlags = 0;
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_02005f94(204);
  leader->motionFlags = 3;
  FieldEvent_Wait(24);
  options.palette = 7;
  options.update = Effect_AdvanceMotion;
  options.startScaleX = 0xcccc;
  options.startScaleY = 0xcccc;
  for (i = 0; i <= 16; i++) {
    s32 angle;
    s32 velocity[3];

    angle = i << 12;
    velocity[0] = Math_Cosine(angle);
    velocity[1] = 0;
    velocity[2] = Math_Sine(angle);
    velocity[0] += velocity[0] >> 1;
    velocity[0] += ((Random16Far() * 3) << 14) >> 16;
    velocity[0] += -0x6000;
    velocity[2] += (((Random16Far() * 3) << 13) >> 16) + -0x3000;
    Func_020002f4(
        leader->x, leader->y, leader->z, velocity[0], velocity[1], velocity[2],
        EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE | 1,
        &options);
  }
  Func_02005f94(188);
  Func_02005ec4(gMainRoutineWork.currentOwner, 257);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 22);
  Call3((void (*)())Func_02005dcc, 0x50000, 0x50000, 0x10000);
  Call3((void (*)())Func_02005dcc, -1, -1, 0xe666);
  Func_02005dd4();
  Func_02005ec4(gMainRoutineWork.currentOwner, 256);
  leader->gravity = 0x10000;
  leader->bounce = 0x4000;
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x204;
  Func_02005e1c();
}

#include "version.h"

#include "scene_tables.h"
#include "system/io_reg.h"

extern char SceneId_Ov664Room1, SceneId_Ov664Room2, SceneId_Ov664Room3,
    SceneId_Ov664Room4, SceneId_Ov664Room5, SceneId_Ov664Room6,
    SceneId_Ov664Room7, SceneId_Ov664Room8, SceneId_Ov664Room9;
extern const u8 gData_02006050[], gData_02006054[], gData_0200605a[],
    gData_02006064[], gData_02006068[], gData_02006072[], gData_0200607e[],
    gData_0200621c[];
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Func_02002770(void);
s32 Task_Add(void (*func)(void), s32 priority);
void Func_02004f3c(void);
void Func_02001690(void);
void Func_02004eb0(void);
void Func_02005e6c(s32 id, s32 x, s32 z);
void Func_02005f6c(const u8 *pillars);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Func_02001084(struct Actor *object, s32 a, s32 b);
void Func_02001420(struct Actor *object, s32 a, s32 b);
void Func_02005d9c(s32 x, s32 z, s32 width, s32 height, s32 a, s32 b);
void Func_02005dac(s32 srcX, s32 srcZ, s32 width, s32 height, s32 destX,
                   s32 destZ);
void Func_02004cc8(void);
void Func_02005f4c(s32 id);
void Func_02005f54(void);
s32 Func_02005f5c(s32 slot, s32 a, s32 b);
s32 Func_0200062c(s32 layer, s32 x, s32 z, s32 cell);
void Func_02000908(const u8 *table);

s32 Func_020051f8(void) {
  struct Actor *object;
  s32 i, z, one;

  /* FAKEMATCH(wrapper): 48 calls forwarded through call.h's Call3, Call4 and
   * Call6 (Call4 through a cast to void (*)()); as direct calls, 122 of 1091
   * instructions differ in tla-en. */

  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 516;
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  GameFlag_SetBit(0x300);
  GameFlag_SetBit(0x89f);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room1) {
    Func_02002770();
    // The Japanese release arrives here whatever flag 265 says.
    if (gMainRoutineWork.entrance == 16 &&
        (!TLA_PATCH_1 || !GameFlag_Test(265))) {
      Actor_GetById(gMainRoutineWork.currentOwner)->terrainId = 1;
      wait(8);
      Func_02005d84();
      wait(1);
    }
    if (gMainRoutineWork.entrance <= 11 || gMainRoutineWork.entrance == 16) {
      Task_Add(Func_02004f3c, 0x480);
      REG_BLDALPHA = 0x1008;
      REG_BLDCNT = 0x3f10;
      Func_02004eb0();
    }
    if (gMainRoutineWork.entrance > 13) {
      if (GameFlag_Test(0xa8e))
        Call3(Func_02005e6c, 12, PIXELS(824), PIXELS(328));
      Func_02005f6c(gData_02006050);
    }
    Actor_GetById(10)->motionFlags = 0;
    Actor_GetById(10)->y = 0;
    Actor_GetById(10)->priorityFlags = 2;
    Actor_SetSpritePriority(10, 3);
    if (!GameFlag_Test(0x89a)) {
      Actor_SetPartAttribute(Actor_GetById(10), 6);
    } else {
      Func_02001084(Actor_GetById(10), 8, 0);
      Actor_SetPartAttribute(Actor_GetById(11), 6);
      Call6(Func_02005d9c, 64, 78, 18, 105, 3, 5);
    }
    if (!GameFlag_Test(0x899)) {
      Actor_SetPartAttribute(Actor_GetById(8), 6);
      Actor_SetPartAttribute(Actor_GetById(9), 6);
      Actor_SetPartAttribute(Actor_GetById(12), 6);
    } else {
      Func_02001084(Actor_GetById(8), 180, 0);
      Func_02001084(Actor_GetById(9), 60, 0);
      Func_02001420(Actor_GetById(12), 60, 0);
    }
    Actor_SetSingleChildFlags(Actor_GetById(8), 0);
    Actor_SetSingleChildFlags(Actor_GetById(9), 0);
    Actor_SetSingleChildFlags(Actor_GetById(12), 0);
    if (gMainRoutineWork.entrance == 11 && !GameFlag_Test(265))
      Func_0200505c();
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room2) {
    REG_BLDALPHA = 0x1008;
    REG_BLDCNT = 0x3f10;
    Func_02002770();
    if (GameFlag_Test(0x897))
      Call3(Func_02005e6c, 9, PIXELS(328), PIXELS(392));
    if (GameFlag_Test(0x898))
      Call3(Func_02005e6c, 11, PIXELS(104), PIXELS(392));
    Func_02005f6c(gData_02006054);
    if (!GameFlag_Test(0x899)) {
      Actor_SetPartAttribute(Actor_GetById(8), 6);
    } else {
      Func_02001084(Actor_GetById(8), 60, 0);
      Actor_SetPartAttribute(Actor_GetById(10), 6);
      Call6(Func_02005d9c, 68, 84, 12, 77, 3, 6);
    }
    Actor_SetSingleChildFlags(Actor_GetById(8), 0);
    if (gMainRoutineWork.entrance == 3 && !GameFlag_Test(265))
      Func_02004cc8();
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room3) {
    Func_02002770();
    Func_02005f4c(245);
    REG_BLDALPHA = 0xc08;
    REG_BLDCNT = 0x3f10;
    Task_Add(Func_02001690, 0x480);
    if (GameFlag_Test(0x89b))
      Call3(Func_02005e6c, 8, PIXELS(264), PIXELS(632));
    if (GameFlag_Test(0x89c))
      Call3(Func_02005e6c, 11, PIXELS(616), PIXELS(568));
    if (GameFlag_Test(0x89d))
      Call3(Func_02005e6c, 12, PIXELS(472), PIXELS(520));
    Func_02005f6c(gData_0200605a);
    if (!GameFlag_Test(0x899)) {
      Actor_SetPartAttribute(Actor_GetById(9), 6);
      Actor_SetPartAttribute(Actor_GetById(10), 6);
      Actor_SetPartAttribute(Actor_GetById(13), 6);
    } else {
      Func_02001084(Actor_GetById(9), 60, 0);
      Func_02001084(Actor_GetById(10), 180, 0);
      Func_02001084(Actor_GetById(13), 200, 0);
    }
    Actor_GetById(9)->actionFlags = 0;
    Actor_GetById(10)->actionFlags = 0;
    Actor_GetById(13)->actionFlags = 0;
    Actor_SetSingleChildFlags(Actor_GetById(9), 0);
    Actor_SetSingleChildFlags(Actor_GetById(10), 0);
    Actor_SetSingleChildFlags(Actor_GetById(13), 0);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room4) {
    Func_02005f54();
    Func_02005f5c(0, 11, 12);
    Func_02005f5c(1, 13, 14);
    Func_02005f5c(2, 15, 0x10);
    Func_02005f5c(3, 17, 18);
    Func_02005f5c(4, 19, 20);
    ((u8 *)Actor_GetById(18))[98] = 1;
    if (GameFlag_Test(769)) {
      Call6(Func_02005d9c, 69, 64, 29, 85, 5, 7);
      Call6(Func_02005dac, 46, 20, 5, 5, 29, 21);
      Call6(Func_02005dac, 32, 89, 1, 1, 31, 89);
      Call6(Func_02005dac, 50, 86, 1, 1, 33, 87);
    } else {
      Call6(Func_02005d9c, 64, 64, 29, 85, 5, 7);
      Call6(Func_02005dac, 52, 20, 5, 5, 29, 21);
      Call6(Func_02005dac, 33, 88, 1, 1, 33, 87);
      Call6(Func_02005dac, 48, 88, 1, 1, 31, 89);
    }
    if (GameFlag_Test(0x300)) {
      Call6(Func_02005d9c, 90, 25, 86, 25, 4, 3);
      Call6(Func_02005d9c, 21, 88, 24, 89, 1, 2);
      Call6(Func_02005dac, 23, 92, 2, 1, 23, 89);
      Call6(Func_02005dac, 18, 33, 2, 1, 23, 25);
    }
    if (GameFlag_Test(0x89e))
      Call3(Func_02005e6c, 9, PIXELS(648), PIXELS(296));
    Func_02005f6c(gData_02006064);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room5) {
    GameFlag_SetBit(324);
    Func_02002770();
    i = 0;
  loop:
    if (GameFlag_Test(514 + i)) {
      z = 24 - (i << 1);
      Call6(Func_02005d9c, 65, 1, 80, z, 1, 2);
      Call4((void (*)())Func_0200062c, 0, 16, z, 0xff00);
    }
    i++;
    if (i != 6)
      goto loop;
    Actor_GetById(8)->motionFlags = 0;
    object = Actor_GetById(8);
    Actor_GetById(8)->terrainHeight = PIXELS(16);
    object->y = PIXELS(16);
    Actor_SetActionCallback(Actor_GetById(8), 9);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room6) {
    Func_02002770();
    Func_02005f54();
    Func_02005f5c(0, 11, 12);
    Func_02005f5c(1, 13, 14);
    Func_02005f5c(2, 15, 0x10);
    if (!GameFlag_Test(770)) {
      Call6(Func_02005d9c, 69, 64, 32, 79, 5, 7);
      Call6(Func_02005dac, 49, 15, 5, 5, 32, 15);
      Call6(Func_02005dac, 33, 82, 1, 1, 34, 82);
      Call6(Func_02005dac, 52, 81, 1, 1, 36, 81);
    } else {
      Call6(Func_02005d9c, 64, 64, 32, 79, 5, 7);
      Call6(Func_02005dac, 49, 21, 5, 5, 32, 15);
      Call6(Func_02005dac, 35, 81, 1, 1, 36, 81);
      Call6(Func_02005dac, 50, 82, 1, 1, 34, 82);
    }
    if (GameFlag_Test(771))
      Call3(Func_02005e6c, 8, PIXELS(520), PIXELS(296));
    if (GameFlag_Test(0x89f)) {
      Call3(Func_02005e6c, 8, PIXELS(312), PIXELS(296));
      Actor_SetSpritePriority(8, 2);
      Actor_GetById(8)->motionFlags = 2;
      Actor_GetById(8)->terrainHeight = 0xffe00000;
      Call6(Func_02005dac, 19, 16, 1, 1, 19, 18);
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room7) {
    Func_02002770();
    // one holds the constant 1 for the stores and calls below; written as
    // 1 at each site, 54 of 1091 differ.
    one = 1;
    ((u8 *)Actor_GetById(12))[98] = one;
    ((u8 *)Actor_GetById(13))[98] = one;
    ((u8 *)Actor_GetById(14))[98] = one;
    ((u8 *)Actor_GetById(15))[98] = one;
    if (!GameFlag_Test(265)) {
      GameFlag_SetBit(513);
      GameFlag_SetBit(514);
    }
    Func_02000908(gData_0200621c);
    if (GameFlag_Test(0x8d0))
      Call3(Func_02005e6c, 8, PIXELS(344), PIXELS(696));
    Func_02005f6c(gData_02006068);
    if (GameFlag_Test(772)) {
      Call6(Func_02005d9c, 39, 82, 38, 82, one, 3);
      Call6(Func_02005dac, 39, 82, 1, 1, 38, 82);
      Call6(Func_02005dac, 38, 19, 1, 1, 38, 18);
    }
    if (GameFlag_Test(773)) {
      Call6(Func_02005d9c, 39, 82, 38, 99, one, 3);
      Call6(Func_02005dac, 38, 98, 1, 1, 38, 99);
      Call6(Func_02005dac, 38, 35, 1, 1, 38, 37);
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room8) {
    Func_02005f4c(245);
    REG_BLDALPHA = 0xc08;
    REG_BLDCNT = 0x3f10;
    Task_Add(Func_02001690, 0x480);
    if (GameFlag_Test(0x8d1))
      Call3(Func_02005e6c, 8, PIXELS(280), PIXELS(472));
    if (GameFlag_Test(0x8d2))
      Call3(Func_02005e6c, 9, PIXELS(360), PIXELS(840));
    if (GameFlag_Test(0xa8f)) {
      Call3(Func_02005e6c, 12, PIXELS(184), PIXELS(616));
      GameFlag_SetBit(0x200);
    }
    Actor_SetSpritePriority(9, 2);
    Func_02005f6c(gData_02006072);
    Func_0200062c(2, Actor_GetById(8)->x >> 20, Actor_GetById(8)->z >> 20, 0);
    Func_0200062c(2, Actor_GetById(9)->x >> 20, Actor_GetById(9)->z >> 20, 0);
    if (!GameFlag_Test(0x899))
      Actor_SetPartAttribute(Actor_GetById(11), 6);
    else
      Func_02001084(Actor_GetById(11), 8, 0);
    Actor_SetSingleChildFlags(Actor_GetById(11), 0);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room9) {
    Func_02002770();
    if (GameFlag_Test(0x8d3))
      Call3(Func_02005e6c, 9, PIXELS(360), PIXELS(264));
    Func_02005f6c(gData_0200607e);
    Func_0200062c(2, Actor_GetById(10)->x >> 20, Actor_GetById(10)->z >> 20, 0);
    Actor_GetById(10)->actionFlags = 0;
    if (!GameFlag_Test(0x899))
      Actor_SetPartAttribute(Actor_GetById(10), 6);
    else
      Func_02001084(Actor_GetById(10), 60, 0);
    Actor_SetSingleChildFlags(Actor_GetById(10), 0);
    if (GameFlag_Test(774)) {
      Call6(Func_02005d9c, 15, 80, 15, 77, 2, 3);
      Call6(Func_02005dac, 15, 80, 1, 1, 15, 79);
      Call6(Func_02005dac, 17, 15, 2, 1, 15, 15);
    }
  }
  return 0;
}
