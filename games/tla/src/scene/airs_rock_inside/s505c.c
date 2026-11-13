/// @file
/// Scene code of overlay 0x664, in ROM order.

#include "../shared/tables.h"
#include "types.h"
#include "system.h"
#include "task.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "system/field_effect.h"
#include "system/vector3.h"
#include "call.h"
#include "field.h"
#include "../airs_rock_inside.h"
#include "event_work.h"
#include "flag_call.h"
#include "module/field/field_effect.h"
#include "module/scene/airs_rock_inside.h"
#include "module/scene/scene.h"

void Sound_Play(s32 cue);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
s32 Math_Sine(s32 angle);
void Func_020002f4(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);
void Map_SetScreenShake(s32 a, s32 b, s32 c);

void Func_0200505c(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct EffectOptions options;
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Map_CenterOnFollowedPoint();
  wait(1);
  leader->y = 0x820000;
  leader->gravity = 0x8000;
  leader->bounce = 0;
  leader->motionFlags = 0;
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Sound_Play(204);
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
  Sound_Play(188);
  Func_02005ec4(gMainRoutineWork.currentOwner, 257);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 22);
  Call3((void (*)())Map_SetScreenShake, 0x50000, 0x50000, 0x10000);
  Call3((void (*)())Map_SetScreenShake, -1, -1, 0xe666);
  Func_02005dd4();
  Func_02005ec4(gMainRoutineWork.currentOwner, 256);
  leader->gravity = 0x10000;
  leader->bounce = 0x4000;
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x204;
  Event_End();
}

#include "version.h"

#include "scene_tables.h"
#include "system/io_reg.h"

extern char SceneId_Ov664Room1, SceneId_Ov664Room2, SceneId_Ov664Room3,
    SceneId_Ov664Room4, SceneId_Ov664Room5, SceneId_Ov664Room6,
    SceneId_Ov664Room7, SceneId_Ov664Room8, SceneId_Ov664Room9;
extern s16 gData_02006050[], gData_02006054[], gData_0200605a[],
    gData_02006064[], gData_02006068[], gData_02006072[], gData_0200607e[],
    gData_0200621c[];
void Func_02001690(void);
void Actor_SetHorizontalPositionWithTerrain(s32 id, s32 x, s32 z);
void Func_02001084(struct Actor *object, s32 a, s32 b);
void Func_02001420(struct Actor *object, s32 a, s32 b);
void Map_CopyCellPictureIndicesRect(s32 x, s32 z, s32 width, s32 height, s32 a, s32 b);
void Map_CopyCellAttributeRect(s32 srcX, s32 srcZ, s32 width, s32 height, s32 destX,
                   s32 destZ);
s32 Func_0200062c(s32 layer, s32 x, s32 z, s32 cell);

s32 Scene_Initialize(void) {
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
      Map_CenterOnFollowedPoint();
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
        Call3(Actor_SetHorizontalPositionWithTerrain, 12, PIXELS(824), PIXELS(328));
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
      Call6(Map_CopyCellPictureIndicesRect, 64, 78, 18, 105, 3, 5);
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
      Call3(Actor_SetHorizontalPositionWithTerrain, 9, PIXELS(328), PIXELS(392));
    if (GameFlag_Test(0x898))
      Call3(Actor_SetHorizontalPositionWithTerrain, 11, PIXELS(104), PIXELS(392));
    Func_02005f6c(gData_02006054);
    if (!GameFlag_Test(0x899)) {
      Actor_SetPartAttribute(Actor_GetById(8), 6);
    } else {
      Func_02001084(Actor_GetById(8), 60, 0);
      Actor_SetPartAttribute(Actor_GetById(10), 6);
      Call6(Map_CopyCellPictureIndicesRect, 68, 84, 12, 77, 3, 6);
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
      Call3(Actor_SetHorizontalPositionWithTerrain, 8, PIXELS(264), PIXELS(632));
    if (GameFlag_Test(0x89c))
      Call3(Actor_SetHorizontalPositionWithTerrain, 11, PIXELS(616), PIXELS(568));
    if (GameFlag_Test(0x89d))
      Call3(Actor_SetHorizontalPositionWithTerrain, 12, PIXELS(472), PIXELS(520));
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
      Call6(Map_CopyCellPictureIndicesRect, 69, 64, 29, 85, 5, 7);
      Call6(Map_CopyCellAttributeRect, 46, 20, 5, 5, 29, 21);
      Call6(Map_CopyCellAttributeRect, 32, 89, 1, 1, 31, 89);
      Call6(Map_CopyCellAttributeRect, 50, 86, 1, 1, 33, 87);
    } else {
      Call6(Map_CopyCellPictureIndicesRect, 64, 64, 29, 85, 5, 7);
      Call6(Map_CopyCellAttributeRect, 52, 20, 5, 5, 29, 21);
      Call6(Map_CopyCellAttributeRect, 33, 88, 1, 1, 33, 87);
      Call6(Map_CopyCellAttributeRect, 48, 88, 1, 1, 31, 89);
    }
    if (GameFlag_Test(0x300)) {
      Call6(Map_CopyCellPictureIndicesRect, 90, 25, 86, 25, 4, 3);
      Call6(Map_CopyCellPictureIndicesRect, 21, 88, 24, 89, 1, 2);
      Call6(Map_CopyCellAttributeRect, 23, 92, 2, 1, 23, 89);
      Call6(Map_CopyCellAttributeRect, 18, 33, 2, 1, 23, 25);
    }
    if (GameFlag_Test(0x89e))
      Call3(Actor_SetHorizontalPositionWithTerrain, 9, PIXELS(648), PIXELS(296));
    Func_02005f6c(gData_02006064);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room5) {
    GameFlag_SetBit(324);
    Func_02002770();
    i = 0;
  loop:
    if (GameFlag_Test(514 + i)) {
      z = 24 - (i << 1);
      Call6(Map_CopyCellPictureIndicesRect, 65, 1, 80, z, 1, 2);
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
      Call6(Map_CopyCellPictureIndicesRect, 69, 64, 32, 79, 5, 7);
      Call6(Map_CopyCellAttributeRect, 49, 15, 5, 5, 32, 15);
      Call6(Map_CopyCellAttributeRect, 33, 82, 1, 1, 34, 82);
      Call6(Map_CopyCellAttributeRect, 52, 81, 1, 1, 36, 81);
    } else {
      Call6(Map_CopyCellPictureIndicesRect, 64, 64, 32, 79, 5, 7);
      Call6(Map_CopyCellAttributeRect, 49, 21, 5, 5, 32, 15);
      Call6(Map_CopyCellAttributeRect, 35, 81, 1, 1, 36, 81);
      Call6(Map_CopyCellAttributeRect, 50, 82, 1, 1, 34, 82);
    }
    if (GameFlag_Test(771))
      Call3(Actor_SetHorizontalPositionWithTerrain, 8, PIXELS(520), PIXELS(296));
    if (GameFlag_Test(0x89f)) {
      Call3(Actor_SetHorizontalPositionWithTerrain, 8, PIXELS(312), PIXELS(296));
      Actor_SetSpritePriority(8, 2);
      Actor_GetById(8)->motionFlags = 2;
      Actor_GetById(8)->terrainHeight = 0xffe00000;
      Call6(Map_CopyCellAttributeRect, 19, 16, 1, 1, 19, 18);
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
      Call3(Actor_SetHorizontalPositionWithTerrain, 8, PIXELS(344), PIXELS(696));
    Func_02005f6c(gData_02006068);
    if (GameFlag_Test(772)) {
      Call6(Map_CopyCellPictureIndicesRect, 39, 82, 38, 82, one, 3);
      Call6(Map_CopyCellAttributeRect, 39, 82, 1, 1, 38, 82);
      Call6(Map_CopyCellAttributeRect, 38, 19, 1, 1, 38, 18);
    }
    if (GameFlag_Test(773)) {
      Call6(Map_CopyCellPictureIndicesRect, 39, 82, 38, 99, one, 3);
      Call6(Map_CopyCellAttributeRect, 38, 98, 1, 1, 38, 99);
      Call6(Map_CopyCellAttributeRect, 38, 35, 1, 1, 38, 37);
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room8) {
    Func_02005f4c(245);
    REG_BLDALPHA = 0xc08;
    REG_BLDCNT = 0x3f10;
    Task_Add(Func_02001690, 0x480);
    if (GameFlag_Test(0x8d1))
      Call3(Actor_SetHorizontalPositionWithTerrain, 8, PIXELS(280), PIXELS(472));
    if (GameFlag_Test(0x8d2))
      Call3(Actor_SetHorizontalPositionWithTerrain, 9, PIXELS(360), PIXELS(840));
    if (GameFlag_Test(0xa8f)) {
      Call3(Actor_SetHorizontalPositionWithTerrain, 12, PIXELS(184), PIXELS(616));
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
      Call3(Actor_SetHorizontalPositionWithTerrain, 9, PIXELS(360), PIXELS(264));
    Func_02005f6c(gData_0200607e);
    Func_0200062c(2, Actor_GetById(10)->x >> 20, Actor_GetById(10)->z >> 20, 0);
    Actor_GetById(10)->actionFlags = 0;
    if (!GameFlag_Test(0x899))
      Actor_SetPartAttribute(Actor_GetById(10), 6);
    else
      Func_02001084(Actor_GetById(10), 60, 0);
    Actor_SetSingleChildFlags(Actor_GetById(10), 0);
    if (GameFlag_Test(774)) {
      Call6(Map_CopyCellPictureIndicesRect, 15, 80, 15, 77, 2, 3);
      Call6(Map_CopyCellAttributeRect, 15, 80, 1, 1, 15, 79);
      Call6(Map_CopyCellAttributeRect, 17, 15, 2, 1, 15, 15);
    }
  }
  return 0;
}

#include "../airs_rock_inside.h"

s32 Scene_PrepareMap(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room1)
    Func_02004f3c();
  return 0;
}

void Func_02000610(void);
void Func_02003ed4(void);
void Func_02003f94(void);
void Func_02004038(void);
void Func_02004084(void);

extern u32 gData_02005f9c[];
extern u32 gData_02006014[];

// clang-format off
u32 gData_02005f9c[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02005fd8[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02006014[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

s16 gData_02006050[] = {
    12, -1,
};

s16 gData_02006054[] = {
    9, 11, -1,
};

s16 gData_0200605a[] = {
    8, 11, 12, 10, -1,
};

s16 gData_02006064[] = {
    9, -1,
};

s16 gData_02006068[] = {
    8, 9, 10, 11, -1,
};

s16 gData_02006072[] = {
    12, 8, 9, 13, 14, -1,
};

s16 gData_0200607e[] = {
    9, 10, -1,
};

u32 gData_02006084[] = {
    0, 6, 38, 17,
};

u32 gData_02006094[] = {
    5, 0, 0xa00000, 0, 1, 38,
    17,
};

DEFINE_G_DATA_02003584(gData_020060b0, Func_02003ed4, 38, 17)

DEFINE_G_DATA_02004014(gData_020060c0, Func_02003f94, Func_02004038,
    Func_02004084)

u32 gData_020060e0[] = {
    5, 0, 0x180000, 0, 1, 22,
    26, 4, 17,
};

u32 gData_02006104[] = {
    5, 0, 0x100000, 0, 1, 22,
    26, 4, 17,
};

s32 gData_02006128[] = {
    22, 26, 0, 5, 0, -0x180000,
    0, 1, 22, 26, 0, 17,
};

s32 gData_02006158[] = {
    22, 26, 0, 5, 0, -0x100000,
    0, 1, 22, 26, 3, 17,
};

s32 gData_02006188[] = {
    (s32)gData_02005f9c,
    (s32)gData_02005fd8,
    (s32)gData_02006014,
};

u32 gData_02006194[] = {
    39, 5, 0, 12, 38, 17,
};

u32 gData_020061ac[] = {
    0, 60, 39, 5, 0, 12,
    38, 17,
};

u32 gData_020061cc[] = {
    39, 5, 0, 12, 2, 0,
    0, 0, 0, 1, 17,
};

u32 gData_020061f8[] = {
    22, 10, 0x18000, 22, 31, 0,
    39, 2, 17,
};

s16 gData_0200621c[] = {
    12, 1, 512, 13, 1, 513, 14, 1,
    514, 15, 1, 515, -1, 0,
};

s32 gData_02006238[] = {
    -0x10000, 320, 0x40000140, -0x10000, -1, 0xffff,
    0xffff, 0, 0, 0, 0, 0,
};

s16 gData_02006268[] = {
    1, -1, 232, 0, 120, 0x4000, 0, 64,
    16, 560, 1008, 0, 2, -1, 152, 0,
    136, 0x4000, 0, 64, 16, 560, 1008, 0,
    3, -1, 152, 0, 232, 0x4000, 0, 64,
    16, 560, 1008, 0, 4, -1, 152, 320,
    648, 0x4000, 0, 64, 16, 560, 1008, 0,
    5, -1, 152, 128, 648, 0x4000, 0, 64,
    16, 560, 1008, 0, 6, -1, 232, 0,
    632, 0x4000, 0, 64, 16, 560, 1008, 0,
    7, -1, 312, 0, 616, 0x4000, 0, 64,
    16, 560, 1008, 0, 8, -1, 472, 0,
    136, 0x4000, 0, 64, 16, 560, 1008, 0,
    9, -1, 472, 0, 232, 0x4000, 0, 64,
    16, 560, 1008, 0, 10, -1, 408, 0,
    952, 0xc000, 0, 64, 16, 560, 1008, 0,
    11, -1, 312, 0, 840, 0x4000, 0, 64,
    16, 560, 1008, 0, 12, -1, 808, 0,
    136, 0xc000, 0, 608, 16, 864, 224, 0,
    13, -1, 680, 0, 168, 0x4000, 0, 608,
    16, 864, 224, 0, 14, -1, 872, 0,
    344, 0x4000, 0, 592, 256, 976, 1008, 0,
    15, -1, 904, 0, 904, 0x4000, 0, 592,
    256, 976, 1008, 0, 16, -1, 312, 0,
    808, 0x4000, 0, 64, 16, 560, 1008, 0,
    -1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(85),
    SCENE_EXIT_IF(1, 86, 2, CONDITION_ALWAYS),
    SCENE_EXIT_IF(2, 87, 1, CONDITION_ALWAYS),
    SCENE_EXIT_IF(3, 88, 4, CONDITION_ALWAYS),
    SCENE_EXIT_IF(4, 90, 2, CONDITION_ALWAYS),
    SCENE_EXIT_IF(5, 85, 12, CONDITION_ALWAYS),
    SCENE_EXIT_IF(6, 94, 1, CONDITION_ALWAYS),
    SCENE_EXIT_IF(7, 94, 2, CONDITION_ALWAYS),
    SCENE_EXIT_IF(8, 91, 1, CONDITION_ALWAYS),
    SCENE_EXIT_IF(9, 92, 1, CONDITION_ALWAYS),
    SCENE_EXIT_IF(10, 83, 4, CONDITION_ALWAYS),
    SCENE_EXIT_IF(11, 85, 14, CONDITION_ALWAYS),
    SCENE_EXIT_IF(12, 85, 5, CONDITION_ALWAYS),
    SCENE_EXIT_IF(13, 85, 13, CONDITION_ALWAYS),
    SCENE_EXIT_IF(14, 93, 2, CONDITION_ALWAYS),
    SCENE_EXIT_IF(15, 89, 1, CONDITION_ALWAYS),
    SCENE_EXIT_IF(16, 86, 3, CONDITION_ALWAYS),
    SCENE_EXITS(86),
    SCENE_EXIT_IF(1, 84, 14, CONDITION_ALWAYS),
    SCENE_EXIT_IF(2, 85, 1, CONDITION_ALWAYS),
    SCENE_EXIT_IF(3, 85, 11, CONDITION_ALWAYS),
    SCENE_EXITS(87),
    SCENE_EXIT_IF(1, 85, 2, CONDITION_ALWAYS),
    SCENE_EXIT_IF(2, 88, 1, CONDITION_ALWAYS),
    SCENE_EXITS(88),
    SCENE_EXIT_IF(1, 87, 2, CONDITION_ALWAYS),
    SCENE_EXIT_IF(2, 90, 1, CONDITION_ALWAYS),
    SCENE_EXIT_IF(3, 93, 1, CONDITION_ALWAYS),
    SCENE_EXIT_IF(4, 85, 3, CONDITION_ALWAYS),
    SCENE_EXITS(89),
    SCENE_EXIT_IF(1, 85, 16, CONDITION_ALWAYS),
    SCENE_EXITS(90),
    SCENE_EXIT_IF(1, 88, 2, CONDITION_ALWAYS),
    SCENE_EXIT_IF(2, 85, 4, CONDITION_ALWAYS),
    SCENE_EXITS(91),
    SCENE_EXIT_IF(1, 85, 8, CONDITION_ALWAYS),
    SCENE_EXIT_IF(2, 92, 2, CONDITION_ALWAYS),
    SCENE_EXITS(92),
    SCENE_EXIT_IF(1, 85, 9, CONDITION_ALWAYS),
    SCENE_EXIT_IF(2, 91, 2, CONDITION_ALWAYS),
    SCENE_EXITS(93),
    SCENE_EXIT_IF(1, 88, 3, CONDITION_ALWAYS),
    SCENE_EXIT_IF(2, 85, 15, CONDITION_ALWAYS),
    SCENE_EXITS(94),
    SCENE_EXIT_IF(1, 85, 6, CONDITION_ALWAYS),
    SCENE_EXIT_IF(2, 85, 7, CONDITION_ALWAYS),
    SCENE_EXITS_END,
};

u32 gData_0200654c[] = {
    0xffff, 0, 0, 0, 0, 0,
};

u32 gData_02006564[] = {
    0xffff0179, 1, 1, 0, 0, 0x2020000,
    0xffff0179, 1, 2, 0, 0, 0x2028000,
    0xffff0179, 7, 3, 0, 0, 0x202c000,
    0xffff017e, 7, 0x1380000, 0, 0x3280000, 0x24000,
    0xffff0179, 7, 5, 0, 0, 0x2024000,
    0xffff0118, 7, 0x80000, 0, 0x80000, 0x24000,
    0xffff0118, 7, 0x80000, 0, 0x80000, 0x24000,
    0xffff0118, 7, 0x80000, 0, 0x80000, 0x24000,
    0xffff, 0, 0, 0, 0, 0,
};

u32 gData_0200663c[] = {
    0xffff0179, 1, 1, 0, 0, 0x2024000,
    0xffff0110, 1, 2, 0, 0, 0x2024000,
    0xffff017e, 1, 3, 0, 0, 0x2024000,
    0xffff0110, 1, 4, 0, 0, 0x2024000,
    0xffff, 0, 0, 0, 0, 0,
};

u32 gData_020066b4[] = {
    0xffff0110, 1, 1, 0, 0, 0x2024000,
    0xffff0179, 1, 2, 0, 0, 0x2020000,
    0xffff0179, 1, 3, 0, 0, 0x2028000,
    0xffff0110, 1, 4, 0, 0, 0x2024000,
    0xffff0110, 1, 5, 0, 0, 0x2024000,
    0xffff0179, 1, 6, 0, 0, 0x2028000,
    0xffff, 0, 0, 0, 0, 0,
};

s32 gData_0200675c[] = {
    (s32)0xffff0145,
    (s32)gData_020061f8,
    1,
    0,
    0,
    0x2024000,
    (s32)0xffff0110,
    1,
    2,
    0,
    0,
    0x2024000,
    (s32)0xffff0145,
    (s32)gData_020061f8,
    3,
    0,
    0,
    0x2024000,
    (s32)0xffff0197,
    1,
    4,
    0,
    0,
    0x2024000,
    (s32)0xffff0197,
    1,
    5,
    0,
    0,
    0x3024000,
    (s32)0xffff0197,
    1,
    6,
    0,
    0,
    0x3024000,
    (s32)0xffff0197,
    1,
    7,
    0,
    0,
    0x3024000,
    (s32)0xffff0197,
    1,
    8,
    0,
    0,
    0x3024000,
    (s32)0xffff0197,
    1,
    9,
    0,
    0,
    0x3024000,
    (s32)0xffff0197,
    1,
    10,
    0,
    0,
    0x3024000,
    (s32)0xffff0197,
    1,
    11,
    0,
    0,
    0x3024000,
    (s32)0xffff0197,
    1,
    12,
    0,
    0,
    0x3024000,
    (s32)0xffff0197,
    1,
    13,
    0,
    0,
    0x3024000,
    0xffff,
    0,
    0,
    0,
    0,
    0,
};

u32 gData_020068ac[] = {
    0xffff0143, 7, 1, 0, 0, 0x2024000,
    0xffff0006, 1, 0x80000, 0, 0x80000, 0x28000,
    0xffff, 0, 0, 0, 0, 0,
};
// clang-format on

s32 gData_020068f4[] = {
    (s32)0xffff0110,
    1,
    1,
    0,
    0,
    0x2024000,
    (s32)0xffff0145,
    (s32)gData_020061f8,
    2,
    0,
    0,
    0x2024000,
    (s32)0xffff0145,
    (s32)gData_020061f8,
    3,
    0,
    0,
    0x2024000,
    (s32)0xffff0197,
    1,
    4,
    0,
    0,
    0x2024000,
    (s32)0xffff0197,
    1,
    5,
    0,
    0,
    0x3024000,
    (s32)0xffff0197,
    1,
    6,
    0,
    0,
    0x3024000,
    (s32)0xffff0197,
    1,
    7,
    0,
    0,
    0x3024000,
    (s32)0xffff0197,
    1,
    8,
    0,
    0,
    0x3024000,
    (s32)0xffff0197,
    1,
    9,
    0,
    0,
    0x3024000,
    0xffff,
    0,
    0,
    0,
    0,
    0,
};

s32 gData_020069e4[] = {
    (s32)0xffff0110,
    1,
    1,
    0,
    0,
    0x2024000,
    (s32)0xffff0110,
    1,
    2,
    0,
    0,
    0x2024000,
    (s32)0xffff0110,
    1,
    3,
    0,
    0,
    0x2024000,
    (s32)0xffff0110,
    1,
    4,
    0,
    0,
    0x2024000,
    (s32)0xffff0196,
    1,
    5,
    0,
    0,
    0x2024000,
    (s32)0xffff0196,
    1,
    6,
    0,
    0,
    0x2024000,
    (s32)0xffff0196,
    1,
    7,
    0,
    0,
    0x2024000,
    (s32)0xffff0196,
    1,
    8,
    0,
    0,
    0x2024000,
    (s32)0xffff0145,
    (s32)gData_020061f8,
    9,
    0,
    0,
    0x2024000,
    (s32)0xffff0145,
    (s32)gData_020061f8,
    10,
    0,
    0,
    0x2024000,
    0xffff,
    0,
    0,
    0,
    0,
    0,
};

s32 gData_02006aec[] = {
    (s32)0xffff0110,
    1,
    1,
    0,
    0,
    0x2024000,
    (s32)0xffff0110,
    1,
    2,
    0,
    0,
    0x2024000,
    (s32)0xffff0145,
    (s32)gData_020061f8,
    3,
    0,
    0,
    0x2024000,
    (s32)0xffff0179,
    1,
    4,
    0,
    0,
    0x2028000,
    (s32)0xffff0110,
    1,
    5,
    0,
    0,
    0x2024000,
    (s32)0xffff0110,
    1,
    6,
    0,
    0,
    0x2024000,
    (s32)0xffff0110,
    1,
    7,
    0,
    0,
    0x2024000,
    0xffff,
    0,
    0,
    0,
    0,
    0,
};

DEFINE_G_DATA_02003C88(gData_02006bac, 0xffff0145, gData_020061f8, 1, 0x2024000,
    1, 2, 0xffff0179, 3, 0x2020000)

s32 gData_02006c0c = -1;

s32 gData_02006c10[] = {
    33,
    (s32)0xffff0001,
    1,
    1,
    (s32)0xffff0002,
    2,
    1,
    (s32)0xffff0003,
    3,
    1,
    (s32)0xffff0004,
    4,
    1,
    (s32)0xffff0005,
    5,
    49,
    (s32)0xffff0006,
    6,
    49,
    (s32)0xffff0007,
    7,
    1,
    (s32)0xffff0008,
    8,
    1,
    (s32)0xffff0009,
    9,
    1,
    (s32)0xffff000a,
    10,
    33,
    (s32)0xffff000b,
    11,
    1,
    (s32)0xffff000c,
    12,
    49,
    (s32)0xffff000d,
    13,
    33,
    (s32)0xffff000e,
    14,
    0xc602,
    (s32)0xffff001e,
    (s32)Func_02003170,
    2,
    (s32)0xffff001e,
    (s32)Func_02002898,
    2,
    (s32)0xffff0050,
    (s32)Func_02002790,
    2,
    (s32)0xffff0051,
    (s32)Func_02002898,
    2,
    (s32)0xffff0052,
    (s32)Func_02002980,
    2,
    (s32)0xffff0053,
    (s32)Func_02002a14,
    (s32)0x80004e15,
    0x89a000b,
    (s32)Func_02000610,
    0x10004e15,
    0x89a000b,
    (s32)Func_0200061c,
    0x4e15,
    0x89a000b,
    (s32)Func_02002cf0,
    8,
    -0x10000,
    (s32)Func_02003254,
    9,
    -0x10000,
    (s32)Func_0200332c,
    0x10008c15,
    (s32)0xffff000c,
    (s32)Func_02003254,
    0x8c15,
    (s32)0xffff000c,
    (s32)Func_0200332c,
    6,
    (s32)0xffff00c8,
    (s32)Func_02000988,
    -1,
    0,
    0,
};

s32 gData_02006d6c[] = {
    33,
    (s32)0xffff0001,
    1,
    49,
    (s32)0xffff0002,
    2,
    0x4602,
    (s32)0xffff0014,
    (s32)Func_02003ac4,
    2,
    (s32)0xffff0050,
    (s32)Func_02002790,
    2,
    (s32)0xffff0051,
    (s32)Func_02002898,
    8,
    -0x10000,
    (s32)Func_02003254,
    9,
    -0x10000,
    (s32)Func_0200332c,
    0x10008c15,
    0x8970009,
    (s32)Func_02003254,
    0x8c15,
    0x8970009,
    (s32)Func_0200332c,
    0x10008c15,
    0x898000b,
    (s32)Func_02003254,
    0x8c15,
    0x898000b,
    (s32)Func_0200332c,
    (s32)0x80004e15,
    0x899000a,
    (s32)Func_02000610,
    0x10004e15,
    0x899000a,
    (s32)Func_0200061c,
    0x4e15,
    0x899000a,
    (s32)Func_02003654,
    6,
    (s32)0xffff00c8,
    (s32)Func_02000988,
    -1,
    0,
    0,
};

s32 gData_02006e2c[] = {
    1,
    (s32)0xffff0001,
    1,
    49,
    (s32)0xffff0002,
    2,
    2,
    (s32)0xffff0050,
    (s32)Func_02002790,
    8,
    -0x10000,
    (s32)Func_02003254,
    9,
    -0x10000,
    (s32)Func_0200332c,
    0x10008c15,
    0x89b0008,
    (s32)Func_02003254,
    0x8c15,
    0x89b0008,
    (s32)Func_0200332c,
    0x10008c15,
    0x89c000b,
    (s32)Func_02003254,
    0x8c15,
    0x89c000b,
    (s32)Func_0200332c,
    0x10008c15,
    0x89d000c,
    (s32)Func_02003254,
    0x8c15,
    0x89d000c,
    (s32)Func_0200332c,
    0x10008c15,
    (s32)0xffff000a,
    (s32)Func_02003254,
    0x8c15,
    (s32)0xffff000a,
    (s32)Func_0200332c,
    6,
    (s32)0xffff00c8,
    (s32)Func_02000988,
    6,
    (s32)0xffff00c9,
    (s32)Func_02003b08,
    -1,
    0,
    0,
};

s32 gData_02006eec[] = {
    33,
    (s32)0xffff0001,
    1,
    49,
    (s32)0xffff0002,
    2,
    49,
    (s32)0xffff0003,
    3,
    1,
    (s32)0xffff0004,
    4,
    8,
    -0x10000,
    (s32)Func_02003254,
    9,
    -0x10000,
    (s32)Func_0200332c,
    0x10008c15,
    0x89e0009,
    (s32)Func_02003254,
    0x8c15,
    0x89e0009,
    (s32)Func_0200332c,
    (s32)0x80004e15,
    (s32)0xffff0008,
    (s32)Func_02000610,
    0x10004e15,
    (s32)0xffff0008,
    (s32)Func_0200061c,
    0x4e15,
    (s32)0xffff0008,
    (s32)Func_02001828,
    (s32)0x80004e15,
    (s32)0xffff000a,
    (s32)Func_02000610,
    0x10004e15,
    (s32)0xffff000a,
    (s32)Func_0200061c,
    0x4e15,
    (s32)0xffff000a,
    (s32)Func_02001828,
    -1,
    0,
    0,
};

s32 gData_02006fa0[] = {
    1,
    (s32)0xffff0001,
    1,
    2,
    (s32)0xffff0050,
    (s32)Func_02002790,
    2,
    (s32)0xffff0051,
    (s32)Func_02002898,
    2,
    (s32)0xffff005a,
    (s32)Func_02003d40,
    0x8602,
    (s32)0xffff000c,
    (s32)Func_02004880,
    0xc403,
    (s32)0xffff001e,
    (s32)Func_02004534,
    0x4403,
    (s32)0xffff001e,
    (s32)Func_02004740,
    0x8403,
    (s32)0xffff001e,
    (s32)Func_020047b0,
    0x403,
    (s32)0xffff001e,
    (s32)Func_02004818,
    0x50009085,
    -0x10000,
    (s32)Func_02003e28,
    0x40009085,
    -0x10000,
    (s32)Func_02003dd4,
    6,
    (s32)0xffff00ca,
    (s32)Func_02003cac,
    6,
    (s32)0xffff00cb,
    (s32)Func_02003cac,
    6,
    (s32)0xffff00cc,
    (s32)Func_02003cac,
    6,
    (s32)0xffff00cd,
    (s32)Func_02003cac,
    6,
    (s32)0xffff00ce,
    (s32)Func_02003cac,
    -1,
    0,
    0,
};

s32 gData_0200706c[] = {
    33,
    (s32)0xffff0001,
    1,
    1,
    (s32)0xffff0002,
    2,
    2,
    (s32)0xffff0050,
    (s32)Func_02002790,
    2,
    (s32)0xffff0051,
    (s32)Func_02002898,
    0x8c15,
    0x89f0008,
    (s32)Func_02004884,
    (s32)0x80004e15,
    (s32)0xffff0009,
    (s32)Func_02000610,
    0x10004e15,
    (s32)0xffff0009,
    (s32)Func_0200061c,
    0x4e15,
    (s32)0xffff0009,
    (s32)Func_02001828,
    (s32)0x80004e15,
    (s32)0xffff000a,
    (s32)Func_02000610,
    0x10004e15,
    (s32)0xffff000a,
    (s32)Func_0200061c,
    0x4e15,
    (s32)0xffff000a,
    (s32)Func_02001828,
    -1,
    0,
    0,
};

s32 gData_020070fc[] = {
    1,
    (s32)0xffff0001,
    1,
    49,
    (s32)0xffff0002,
    2,
    2,
    (s32)0xffff0050,
    (s32)Func_02002790,
    2,
    (s32)0xffff00d4,
    (s32)Func_020048c4,
    8,
    -0x10000,
    (s32)Func_02003254,
    9,
    -0x10000,
    (s32)Func_0200332c,
    0x10008c15,
    0x8d00008,
    (s32)Func_02003254,
    0x8c15,
    0x8d00008,
    (s32)Func_0200332c,
    0x10008c15,
    (s32)0xffff0009,
    (s32)Func_02003254,
    0x8c15,
    (s32)0xffff0009,
    (s32)Func_0200332c,
    0x10008c15,
    (s32)0xffff000a,
    (s32)Func_02003254,
    0x8c15,
    (s32)0xffff000a,
    (s32)Func_0200332c,
    0x10008c15,
    (s32)0xffff000b,
    (s32)Func_02003254,
    0x8c15,
    (s32)0xffff000b,
    (s32)Func_0200332c,
    0x50008615,
    0x200000c,
    (s32)Func_020048bc,
    0x50008615,
    0x201000d,
    (s32)Func_020048bc,
    0x50008615,
    0x202000e,
    (s32)Func_020048bc,
    0x50008615,
    0x203000f,
    (s32)Func_020048bc,
    (s32)0x80004e15,
    (s32)0xffff0010,
    (s32)Func_02000610,
    0x10004e15,
    (s32)0xffff0010,
    (s32)Func_0200061c,
    0x4e15,
    (s32)0xffff0010,
    (s32)Func_02001828,
    (s32)0x80004e15,
    (s32)0xffff0011,
    (s32)Func_02000610,
    0x10004e15,
    (s32)0xffff0011,
    (s32)Func_0200061c,
    0x4e15,
    (s32)0xffff0011,
    (s32)Func_02001828,
    -1,
    0,
    0,
};

s32 gData_02007228[] = {
    1,
    (s32)0xffff0001,
    1,
    33,
    (s32)0xffff0002,
    2,
    6,
    (s32)0xffff00c9,
    (s32)Func_02004ab8,
    6,
    (s32)0xffff00c8,
    (s32)Func_02000988,
    8,
    -0x10000,
    (s32)Func_02003254,
    9,
    -0x10000,
    (s32)Func_0200332c,
    0x10008c15,
    0xa8f000c,
    (s32)Func_02003254,
    0x8c15,
    0xa8f000c,
    (s32)Func_0200332c,
    0x10008c15,
    0x8d10008,
    (s32)Func_02003254,
    0x8c15,
    0x8d10008,
    (s32)Func_0200332c,
    0x10008c15,
    0x8d20009,
    (s32)Func_02003254,
    0x8c15,
    0x8d20009,
    (s32)Func_0200332c,
    0x10008c15,
    (s32)0xffff000d,
    (s32)Func_02003254,
    0x8c15,
    (s32)0xffff000d,
    (s32)Func_0200332c,
    0x10008c15,
    (s32)0xffff000e,
    (s32)Func_02003254,
    0x8c15,
    (s32)0xffff000e,
    (s32)Func_0200332c,
    (s32)0x80004e15,
    (s32)0xffff000a,
    (s32)Func_02000610,
    0x10004e15,
    (s32)0xffff000a,
    (s32)Func_0200061c,
    0x4e15,
    (s32)0xffff000a,
    (s32)Func_02001828,
    -1,
    0,
    0,
};

s32 gData_02007318[] = {
    33,
    (s32)0xffff0001,
    1,
    49,
    (s32)0xffff0002,
    2,
    2,
    (s32)0xffff0050,
    (s32)Func_02002790,
    6,
    (s32)0xffff00c8,
    (s32)Func_02000988,
    8,
    -0x10000,
    (s32)Func_02003254,
    9,
    -0x10000,
    (s32)Func_0200332c,
    0x10008c15,
    0x8d30009,
    (s32)Func_02003254,
    0x8c15,
    0x8d30009,
    (s32)Func_0200332c,
    (s32)0x80004e15,
    (s32)0xffff0008,
    (s32)Func_02000610,
    0x10004e15,
    (s32)0xffff0008,
    (s32)Func_0200061c,
    0x4e15,
    (s32)0xffff0008,
    (s32)Func_02001828,
    -1,
    0,
    0,
};

s32 gData_020073a8[] = {
    33,
    (s32)0xffff0001,
    1,
    33,
    (s32)0xffff0002,
    2,
    3,
    (s32)0xffff001e,
    (s32)Func_02004bb4,
    -1,
    0,
    0,
};

s32 gData_020073d8[] = {
    -1,
};
