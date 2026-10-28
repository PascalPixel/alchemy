// Draft, differ 308 of 850: Magma Rock's map setup for each of its rooms 1 to 8:
// event work word 0x1ac to 516; per room it places and readies the room's
// objects (raised blocks at 40 pixels, statues on their action scripts with
// Func_02000800 and the room's flag 768 to 770), loads the cell watch's
// tables (Func_02004ed8, Func_02000880), restores what earlier visits opened
// (flags 2544 to 2553, 768 to 770, 516, 772, 773, 324) and, when the room's
// flag is clear, the blend and the closed cells; room 8 starts the cell
// stampers (Func_0200255c). Returns 0.
#include "../../../../games/tla/src/scene/magma_rock_inside/local.h"
#include "io_reg.h"

extern char SceneId_Ov692Room1;
extern const u8 gData_02005074[];
extern const u8 gData_020050d4[];
extern const u8 gData_02005134[];
extern const u8 gData_0200519c[];
extern const u8 gData_020051fc[];
extern const u8 gData_0200525c[];
extern const u8 gData_020052bc[];
extern const u8 gData_0200531c[];
extern const u8 gData_0200537c[];
extern const u8 gData_020053dc[];
extern const u8 gData_0200543c[];
extern const u8 gData_0200549c[];
extern const u8 gData_020054fc[];
extern u16 gData_02005500[];
extern const u8 gData_02005508[];
extern u16 gData_0200550c[];
extern const u8 gData_02005520[];
extern const u8 gData_02005528[];
extern const u8 gData_0200552e[];

void Func_02000800(s32 id, const u8 *script, s32 flag);
void Func_02004cd8(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02004ce8(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02004e60(s32 value);
void Func_02004ed8(const u8 *table);
void Func_02000880(u16 *table);
void Func_02004ec0(void);
void Func_02004ec8(s32 a, s32 b, s32 c);
void Func_02004f08(s32 id, s32 value);
void Func_02000694(s32 layer, s32 x, s32 z, s32 mask);
void ObjectMotion_SetActionVariant(u32 id, s32 variant);
void Object_SetModeById(u32 id, s32 mode);
void Object_SetActionCallback(struct ObjectRuntime *object, s32 value);
void Func_02004da0(s32 id, s32 x, s32 z);
void Func_020034bc(void);
void Func_020034f0(void);
void Func_02003548(void);
void Func_02003630(void);

#define CLOSED_BLEND()                                                         \
  {                                                                            \
    REG_BLDALPHA = 0x1000;                                                     \
    REG_BLDCNT = 0x3f44;                                                       \
  }

static __inline__ void Raise(s32 id) {
  struct ObjectRuntime *object = Object_GetById(id);

  object->motionFlags = 2;
  object->terrainHeight = object->y = 40 << 16;
  object->priorityFlags |= 128;
}

static __inline__ void Lower(s32 id, s32 height) {
  Object_GetById(id)->motionFlags = 0;
  Object_GetById(id)->terrainHeight = Object_GetById(id)->y = height;
}

s32 Func_02003678(void) {
  s32 scene;
  s32 i;

  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 516;
  scene = gAdventure.scene;
  if (scene == (s32)&SceneId_Ov692Room1) {
    Object_GetById(gAdventure.currentOwner)->priorityFlags |= 64;
    Func_02000800(8, gData_02005074, 768);
    if (!GameFlag_Test(768)) {
      CLOSED_BLEND();
      Func_02004cd8(71, 64, 47, 98, 1, 1);
    }
  } else if (scene == (s32)&SceneId_Ov692Room2) {
    Func_02004e60(0);
    Object_GetById(15)->update = Func_02003630;
    for (i = 0; i != 6; i++)
      Lower(i + 9, 0);
    if (GameFlag_Test(2544))
      Object_GetById(15)->z = 132 << 17;
    Func_02004ed8(gData_020054fc);
    Func_02000880(gData_02005500);
    Object_GetById(gAdventure.currentOwner)->priorityFlags |= 64;
    Raise(16);
    if (GameFlag_Test(768))
      Func_020034bc();
    else
      CLOSED_BLEND();
  } else if (scene == (s32)&SceneId_Ov692Room3) {
    Func_02004e60(0);
    if (GameFlag_Test(2545))
      Object_GetById(11)->z = 166 << 18;
    Func_02004ed8(gData_02005508);
    Func_02004ec0();
    Func_02004ec8(0, 13, 14);
    Func_02000880(gData_02005500);
    Object_GetById(gAdventure.currentOwner)->priorityFlags |= 64;
    Raise(9);
    Func_02000800(10, gData_020050d4, 769);
    Func_02000800(12, gData_02005134, 769);
    if (GameFlag_Test(769))
      Func_020034f0();
    else
      CLOSED_BLEND();
  } else if (scene == (s32)&SceneId_Ov692Room4) {
    Func_02004e60(0);
    Object_GetById(gAdventure.currentOwner)->priorityFlags |= 64;
    Func_02000880(gData_0200550c);
    if (gAdventure.entrance <= 7) {
      Func_02000800(10, gData_0200519c, 769);
      Func_02000800(12, gData_020051fc, 769);
      if (GameFlag_Test(769)) {
        Func_020034f0();
      } else {
        CLOSED_BLEND();
        Func_02004cd8(71, 64, 35, 82, 1, 1);
      }
    } else {
      Object_GetById(23)->priorityFlags |= 32;
      Object_GetById(24)->priorityFlags |= 32;
      Object_GetById(25)->priorityFlags |= 32;
      ObjectMotion_SetActionVariant(23, 3);
      ObjectMotion_SetActionVariant(24, 3);
      ObjectMotion_SetActionVariant(25, 3);
      if (GameFlag_Test(2546))
        Object_GetById(23)->x = 172 << 17;
      if (GameFlag_Test(2547))
        Object_GetById(24)->x = 172 << 17;
      if (GameFlag_Test(2548))
        Object_GetById(25)->x = 172 << 17;
      Func_02004ed8(gData_02005520);
      Lower(26, 0);
      for (i = 15; i <= 22; i++)
        Object_GetById(i)->movementState = 3;
      Func_02000800(15, gData_0200525c, 770);
      Func_02000800(16, gData_0200525c, 770);
      Func_02000800(17, gData_020052bc, 770);
      Func_02000800(18, gData_020052bc, 770);
      Func_02000800(19, gData_0200531c, 770);
      Func_02000800(20, gData_0200531c, 770);
      Func_02000800(21, gData_0200537c, 770);
      Func_02000800(22, gData_0200537c, 770);
      if (GameFlag_Test(770))
        Func_02003548();
      else
        CLOSED_BLEND();
    }
  } else if (scene == (s32)&SceneId_Ov692Room5) {
    Func_02004e60(0);
    Object_GetById(gAdventure.currentOwner)->priorityFlags |= 64;
    Func_02004f08(10, 1);
    Func_02004f08(11, 1);
    Raise(8);
    Raise(9);
    if (GameFlag_Test(770)) {
      Func_02000694(0, 22, 26, 0);
      Func_02000694(0, 25, 26, 0);
      ObjectMotion_SetActionVariant(64, 3);
    } else {
      CLOSED_BLEND();
    }
  } else if (scene == (s32)&SceneId_Ov692Room6) {
    Func_02004e60(0);
    Object_GetById(gAdventure.currentOwner)->priorityFlags |= 64;
    if (GameFlag_Test(2549)) {
      Func_02004ce8(46, 74, 1, 1, 46, 73);
      Object_GetById(9)->x = 190 << 18;
    }
    if (GameFlag_Test(2550))
      Func_02004da0(12, 230 << 18, 154 << 18);
    Func_02004ed8(gData_02005528);
    Func_02000800(8, gData_020053dc, 770);
    Func_02000800(10, gData_0200543c, 770);
    Func_02000800(11, gData_0200549c, 770);
    if (GameFlag_Test(770)) {
      Object_SetModeById(9, 4);
      if (GameFlag_Test(2550))
        Object_SetModeById(12, 4);
      if (GameFlag_Test(2549))
        Func_02000694(2, 46, 8, 0xff00);
      Func_02000694(0, 10, 45, 0);
      Func_02000694(0, 53, 40, 0);
    } else {
      CLOSED_BLEND();
      Func_02004cd8(71, 64, 50, 102, 1, 1);
    }
  } else if (scene == (s32)&SceneId_Ov692Room7) {
    for (i = 0; i != 3; i++) {
      struct ObjectRuntime *object = Object_GetById(i + 8);

      Func_02004c98(object, 4);
      object->motionFlags = 0;
      object->terrainHeight = object->y = -0x200000;
    }
    if (GameFlag_Test(2551)) {
      Object_SetModeById(23, 4);
      Func_02004da0(23, 174 << 18, 162 << 18);
    }
    Func_02004ed8(gData_0200552e);
    Func_02004e60(0);
    if (!GameFlag_Test(771))
      Func_02004da0(21, 0, 0);
    if (!GameFlag_Test(772))
      Func_02004da0(22, 0, 0);
    if (GameFlag_Test(771) && GameFlag_Test(772)) {
      Lower(11, -0x200000);
      Object_SetModeById(11, 4);
    } else {
      Func_02000694(0, 38, 18, 0xff00);
    }
    if (GameFlag_Test(2553))
      Object_SetModeById(12, 0);
    for (i = 0; i != 4; i++)
      Object_SetModeById(i + 13, 2);
    for (i = 0; i != 11; i++)
      ObjectDispatch_SetSingleChildFlags(Object_GetById(i + 12), 0);
    if (!GameFlag_Test(2552)) {
      for (i = 0; i != 9; i++)
        Func_02004da0(i + 12, 0, 0);
    }
  } else if (scene == (s32)&SceneId_Ov692Room8) {
    Func_0200255c();
    GameFlag_SetBit(324);
    for (i = 0; i != 6; i++) {
      if (GameFlag_Test(i + 514)) {
        Func_02004cd8(65, 1, 80, 24 - i * 2, 1, 2);
        Func_02000694(0, 16, 24 - i * 2, 0xff00);
      }
    }
    ObjectDispatch_SetSingleChildFlags(Object_GetById(10), 0);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(11), 0);
    if (GameFlag_Test(773)) {
      Lower(9, -0x200000);
      Object_SetModeById(9, 4);
    } else {
      Func_02000694(0, 7, 21, 0xff00);
      Func_02004da0(11, 0, 0);
    }
    Lower(8, 0x100000);
    Object_SetActionCallback(Object_GetById(8), 9);
  }
  return 0;
}
