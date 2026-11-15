#ifndef ALCHEMY_SCENE_MARS_LIGHTHOUSE_AERIE_H
#define ALCHEMY_SCENE_MARS_LIGHTHOUSE_AERIE_H

/// @file
/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"

struct Actor;

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

void MarsLighthouseAerie_TalkToKyle(void);

void MarsLighthouseAerie_FindWhoTheDragonWas(void);

void MarsLighthouseAerie_UrgeOnToBeacon(void);

void MarsLighthouseAerie_LightBeacon(void);
void MarsLighthouseAerie_WatchGoldenSunRise(void);
void MarsLighthouseAerie_EndRisingRings(struct Actor *object);

void Orbs_UpdateRising(struct Actor *self);

extern u32 gOrbs_RisingScript[];

extern s32 gMarsLighthouseAerie_BeaconSparkScript[];

void Orbs_SpawnRising(void);

extern s32 gMarsLighthouseAerie_BeamWidenScript[];
extern u32 gMarsLighthouseAerie_BeamShootUpScript[];
extern u32 gMarsLighthouseAerie_BeamRiseScript[];
extern s32 gMarsLighthouseAerie_WatchGoldenSunRiseScript[];

void Teleport_ArriveWhirling(void);

extern s32 gMarsLighthouseAerie_RisingRingTiles[];

void MarsLighthouseAerie_UpdateRisingRings(struct Actor *object);
void MarsLighthouseAerie_HoldBackFromParents(void);
void MarsLighthouseAerie_HoldBackFromParentsAgain(void);
void MarsLighthouseAerie_OfferToCastMarsStar(void);
void MarsLighthouseAerie_ClearCastOffer(void);
void MarsLighthouseAerie_TalkToIvan(void);
void MarsLighthouseAerie_TalkToJennasMother(void);
void MarsLighthouseAerie_TalkToJennasFather(void);
void MarsLighthouseAerie_HaltJennasMother(void);
void MarsLighthouseAerie_HaltJennasFather(void);

#endif
