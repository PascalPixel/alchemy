#ifndef ALCHEMY_MODULE_SCENE_MARS_LIGHTHOUSE_AERIE_H
#define ALCHEMY_MODULE_SCENE_MARS_LIGHTHOUSE_AERIE_H

/// @file
/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"

struct Actor;
struct SparkleSprite;
struct SparkleSprite__s53e4;

/// A halfword whose store may alias any read after it.
union StoredHalfword {
  u16 value;
  s32 word;
};

struct OverlayActor {
  u8 unk0[8];
  s32 x, y, z, terrainHeight, scaleX, scaleY;
  u8 unk20[3];
  u8 priorityFlags;
  u8 unk24[12];
  s32 speedLimit, acceleration;
  u8 unk38[16];
  s32 gravity;
  s32 unk4c;
  s32 animation;
  u8 unk54;
  u8 motionFlags;
  u8 unk56[6];
  u8 unk5c;
  u8 unk5d[15];
  s32 update;
};

struct OverlaySprite {
  u8 unk0[5];
  u8 unk5_0 : 5;
  u8 unk5_5 : 1;
  u8 unk5_6 : 2;
  u8 unk6[3];
  u8 unk9_0 : 2;
  u8 unk9_2 : 2;
  u8 unk9_4 : 4;
  u8 unkA[6];
  u8 vramBlock;
  u8 unk11[10];
  u8 unk1b;
};

/// The heap slots read as words.
struct HeapWords {
  u8 unk00[108];
  s32 eventWord;
};

void MarsLighthouseAerie_TalkToCoughingParent(void);

void MarsLighthouseAerie_FindWhoTheDragonWas(void);

void MarsLighthouseAerie_UrgeOnToBeacon(void);

void MarsLighthouseAerie_LightBeacon(void);
void MarsLighthouseAerie_WatchGoldenSunRise(void);
void Func_020054e4(struct Actor *object);

void Func_02003210(struct Actor *self);

extern u32 gData_02006afc[];

extern u32 Data_02006ccc;
extern u32 Data_02006b68;

extern s32 gData_02006180[];

s32 Func_0200557c();
void Func_02005794();

void Func_0200326c(void);

extern s32 gData_020060b4[];
extern u32 gData_02006120[];
extern u32 gData_02006150[];
extern s32 gData_020061d4[];

extern u16 *Data_02006ce4;
extern u16 *Data_02006ce8;
extern u16 *Data_02006cd4;

void Func_020051e0(void);

void Func_020057ac(struct SparkleSprite *sprite);
s32 Func_020057bc(struct Actor *object);

extern s32 gData_020057e4[];

void Func_020057a4(struct SparkleSprite__s53e4 *sprite, s32 width, s32 height,
                   u32 attributes, s32 tile);

void Func_02005268(struct Actor *object);
void MarsLighthouseAerie_HoldBackFromParents(void);
void MarsLighthouseAerie_HoldBackFromParentsAgain(void);
void MarsLighthouseAerie_OfferToCastMarsStar(void);
void Func_02004d14(void);
void MarsLighthouseAerie_TalkToConsoler(void);
void MarsLighthouseAerie_TalkToBlindedParent(void);
void MarsLighthouseAerie_TalkToGladParent(void);
void Func_02004ecc(void);
void Func_02004ee4(void);

#endif
