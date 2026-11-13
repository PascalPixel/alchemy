#ifndef ALCHEMY_MODULE_SCENE_VENUS_LIGHTHOUSE_H
#define ALCHEMY_MODULE_SCENE_VENUS_LIGHTHOUSE_H

/// @file
/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"

struct VenusActor {
  u8 unk00[6];
  u16 facing;
  s16 xLow;
  s16 xPixel;
  s32 y;
  s16 zLow;
  s16 zPixel;
  s32 terrainHeight;
  u8 unk18[0xa];
  u8 terrainId;
  u8 unk23;
  s32 velocityX;
  s32 speed;
  s32 velocityZ;
  s32 moveRateX;
  s32 moveRateZ;
  s32 targetX;
  s32 targetY;
  s32 targetZ;
  u8 unk44[0x11];
  u8 motionFlags;
  u8 unk56[3];
  u8 collisionFlags;
  u8 selectFlags;
  u8 unk5b[7];
  u8 busy;
  u8 unk63[9];
  void *update;
};

enum { ENTRANCE_OPENING = 99 };

struct SetupWork {
  u8 unk00[0x24];
  s32 (*idle)(void);
};

extern s16 gData_02001d8c[];

void Func_02000b2c(void);

void Func_02001cd4(s32 actor);

void Func_020006ac(void);

void VenusLighthouse_StepOnFloorSwitch(void);

extern u32 gData_02001ce4[];
extern u32 gData_02001d1c[];
extern s32 gData_0200215c[];
extern s32 gData_02002180[];

#endif
