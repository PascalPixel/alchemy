#ifndef ALCHEMY_SCENE_MARS_LIGHTHOUSE_AERIE_H
#define ALCHEMY_SCENE_MARS_LIGHTHOUSE_AERIE_H

/// @file
/// Mars Lighthouse's aerie: its events and scripts, and the views of a sprite
/// and the heap through which the beacon is lit.

#include "fixed_math.h"
#include "types.h"

struct Actor;

/// A field sprite (struct FieldSprite) read a byte at a time, as the aerie's
/// flames take the beacon fire's priority and the Mars Star's sprite its
/// colours and the star's icon.
struct MarsLighthouseAerieSprite {
  u8 link[4];
  u8 y; ///< attribute 0's y
  /// Attribute 0's high byte.
  ///@{
  u8 affineModeMosaic : 5;
  u8 fullColor : 1;
  u8 shape : 2;
  ///@}
  u8 attr1[2];
  u8 tileLow; ///< attribute 2's low byte
  /// Attribute 2's high byte.
  ///@{
  u8 tileHigh : 2; ///< tile number bits 9..8
  u8 priority : 2;
  u8 palette : 4;
  ///@}
  u8 attr3[2];
  u8 scale[4];
  u8 vramBlock; ///< VRAM cache entry holding its tiles
  u8 cacheFlags;
  u8 rotation[2];
  u8 size[4];       ///< width, height and the x and y offsets
  u8 lastAnimation; ///< the animation number last selected
  u8 dirty;         ///< nonzero redraws its pixels
  u8 flags;         ///< the sprite flags (enum SpriteFlag)
  u8 layerCount;
};

void MarsLighthouseAerie_TalkToKyle(void);

void MarsLighthouseAerie_FindWhoTheDragonWas(void);

void MarsLighthouseAerie_UrgeOnToBeacon(void);

void MarsLighthouseAerie_LightBeacon(void);
void MarsLighthouseAerie_WatchGoldenSunRise(void);
void MarsLighthouseAerie_EndRisingRings(struct Actor *actor);

void Orb_UpdateRising(struct Actor *self);

extern u32 gOrb_RisingScript[];

extern s32 gMarsLighthouseAerie_BeaconSparkScript[];

void Orb_SpawnRising(void);

extern s32 gMarsLighthouseAerie_BeamWidenScript[];
extern u32 gMarsLighthouseAerie_BeamShootUpScript[];
extern u32 gMarsLighthouseAerie_BeamRiseScript[];
extern s32 gMarsLighthouseAerie_WatchGoldenSunRiseScript[];

void Teleport_ArriveWhirling(void);

extern s32 gMarsLighthouseAerie_RisingRingTiles[];

void MarsLighthouseAerie_UpdateRisingRings(struct Actor *actor);
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
