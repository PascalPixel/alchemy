#ifndef ALCHEMY_MODULE_SCENE_SETTLEMENTS_H
#define ALCHEMY_MODULE_SCENE_SETTLEMENTS_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"

extern s16 gSettlements_SlideAnglesByKey[];

void Settlements_StepOntoWall(void);

void Settlements_StopLeader(void);

void Settlements_MeetTinder(void);

void Settlements_CatchApple(void);
void Settlements_CutLeftBush(void);

void Settlements_CutStairsBush(void);

void Settlements_EnterDoor5(void);

void Settlements_EnterDoor3(void);

void Settlements_StepOntoIce(void);

void Settlements_StepOffIce(void);
void Settlements_StepOnIceSlide(void);
void Settlements_LashRope(void);

void Settlements_UnblockFallsObjects(void);

void Settlements_BlockFallsObject(void);

void Settlements_MoveCrate(void);

void Settlements_StepOffWall(void);

void Settlements_UnblockCrate(void);

void Settlements_BlockCrate(void);

void Settlements_LiftBoulder(void);

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern s32 gSettlements_BobScript[];
extern u32 gSettlements_CrateBobScript[];

struct OrbitSprite {
  u8 unk00[5];
  u8 flags05Low : 5;
  u8 flags05Bit5 : 1;
  u8 flags05High : 2;
  u8 unk06[3];
  u8 flags09Low : 2;
  u8 flags09Mode : 2;
  u8 flags09High : 4;
  u8 unk0a[6];
  u8 palette;
  u8 unk11[10];
  u8 state;
};

struct OrbitObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[15];
  u8 flags23;
  u8 unk24[12];
  s32 orbitAngle;
  u8 unk34[4];
  s32 orbitCenterX;
  s32 orbitCenterY;
  s32 orbitCenterZ;
  u8 unk44[12];
  struct OrbitSprite *sprite;
  u8 unk54;
  u8 mode;
  u8 state;
  u8 unk57[5];
  u8 active;
  u8 unk5d[4];
  u8 visible;
  u8 unk62[10];
  u32 callback;
};

struct SettlementsVector {
  s32 x;
  s32 y;
  s32 z;
};

struct SettlementsSpawn {
  s32 unk00;
  s32 unk04;
  s32 scaleX;
  s32 scaleY;
  s32 speed;
  s32 acceleration;
  u16 kind;
  const u8 *script;
  s32 unk20;
  void *update;
};

extern char SceneId_Ov690Room1;
extern char SceneId_Ov690Room2;
extern char SceneId_Ov690Room4;
extern char SceneId_Ov690Room7;
extern char SceneId_Ov690Room8;
extern char SceneId_Ov690Room9;
extern char SceneId_Ov690Room10;

void Func_02002170(void);

extern char SceneId_Ov690Room6;

void Settlements_StepOntoWall(void);

void Func_02002140(s32 a);

void Func_02002188(const void *data);

extern u16 gSettlements_WaterColors[];
extern u16 gSettlements_WaterColorsSaved[];

void Settlements_SpawnSplashRing(s32 objectId);

void Settlements_MovePillar(void);

#endif
