#ifndef ALCHEMY_MODULE_FIELD_F0DCB44_H
#define ALCHEMY_MODULE_FIELD_F0DCB44_H

/// @file
/// What games/tla/src/field/event_commands/f0dcb44.c declares for the files
/// that use it, written once.

#include "types.h"
#include "field/field_particle.h"

struct Actor;
struct Vec3;

struct WaveFxSprite {
  u32 unk00[2];
  u8 unk08;
  u32 unk09_0 : 2;
  u32 mode : 2;
  u32 unk09_4 : 4;
  u8 unk0a[8];
  u16 orientation;
  u8 unk14[6];
  u8 palette;
};

struct WaveFxVector {
  s32 x;
  s32 y;
  s32 z;
};

struct WaveFxDescriptor {
  s16 sprite;
};

struct WaveFxMetadata {
  u8 unk00[8];
  s8 boxX;
};

struct ResourceEntry {
  u8 unk00[0x10];
  u8 vramBlock;
};

struct Effect04Scene {
  u16 angle;
  u8 unk02[2];
  s32 x;
  s32 y;
  s32 z;
  struct Actor *mainObject;
  struct Actor *child;
  s16 firstObjectId;
  s16 secondObjectId;
  u8 unk1c[4];
  s8 enabled;
  u8 unk21[0x13];
  s8 childOption;
  s8 childMode;
  u8 unk36[0x1a];
  struct FieldParticle slots[24];
};

struct Effect04Vector {
  s32 x;
  s32 y;
  s32 z;
};

struct OrbitingParticleVector {
  s32 x;
  s32 y;
  s32 z;
};

struct OrbitingParticleChild {
  u8 reserved00[35];
  u8 flags;
};

// The field event work's Reveal state (eventWork + 0xca4).
struct RevealEvent {
  u8 unk000[0xca4];
  s8 active;   // Reveal is showing
  u8 markers;  // the nearby markers are up
  s16 frames;  // how long it lasts
  s16 centreX; // pixels
  s16 centreY;
};

struct BurstParticleVector {
  s32 values[3];
};

struct BurstParticleVisual {
  u8 reserved00[5];
  u8 flagsALow : 5;
  u8 flip : 1;
  u8 flagsAHigh : 2;
  u8 reserved06;
  u8 flagsBLow : 6;
  u8 flagsBHigh : 2;
  union {
    struct {
      u16 shape : 10;
      u16 unused : 6;
    } bits;
    struct {
      u8 lo;
      u8 highLow : 4;
      u8 palette : 4;
    } bytes;
  } attributes;
};

// ⚓️'s sprite keeps its second part 28 bytes in.
struct BurstParticleVisualGroup {
  struct BurstParticleVisual primary;
  u8 reserved0c[16];
  struct BurstParticleVisual child;
};

struct BattleEffectScene {
  u8 reserved00[4];
  s32 x;
  s32 y;
  s32 z;
  void *mainObject;
  void *child;
};

struct EffectChild {
  u8 reserved00[12];
  s32 y;
  u8 reserved10[0x55 - 0x10];
  u8 flag;
  u8 reserved56[0x6c - 0x56];
  void (*callback)(void);
};

struct EffectTarget {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
};

struct RadialBurstEffect {
  u8 unk00[12];
  s32 x;
  s32 z;
  s32 sourceX;
  s32 sourceZ;
  u8 unk1c[4];
  s32 velocityX;
  s32 velocityZ;
  u8 unk28[10];
  u16 scale;
  u8 unk34[12];
  s8 state;
  u8 unk41;
  u8 enabled;
};

struct ImpactVector {
  s32 x;
  s32 y;
  s32 z;
};

struct WaveFxScroll {
  u16 lines[2][162];
  u8 unk288[2];
  u8 page;
  u8 unk28b[9];
  u8 delay;
};

struct ItemBreakFragmentPosition {
  s32 x;
  s32 y;
  s32 z;
};

struct ItemBreakFragmentSource {
  u8 reserved00[6];
  u16 angle; ///< Facing angle (0x10000 is one turn).
  s32 x;
  s32 y;
  s32 z;
  u8 reserved14[70];
  u8 flag5a;
  u8 reserved5b[13];
  struct ItemBreakFragmentSource *target; ///< Facing target.
};

struct ItemBreakFragmentObject {
  u8 reserved00[72];
  s32 field48; ///< Gravity (same offset as gravity).
  u8 reserved4c[9];
  u8 mode55; ///< Same offset as motionFlags.
  u8 reserved56[8];
  u16 field5e;
};

struct Output_08097f80 {
  s32 x;
  s32 y;
  s32 z;
};

struct SwayObject {
  u8 unk00[8];
  s32 y;
  u8 unk0c[44];
  s32 baseY;
  u8 unk3c[40];
  s16 phase;
};

struct OrbitVector {
  s32 x;
  s32 y;
  s32 z;
};

struct SparkObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[4];
  s32 scaleX;
  s32 scaleY;
  u8 unk20[24];
  s32 anchorX;
  u8 unk3c[12];
  s32 gravity;
  u8 unk4c[9];
  u8 motionFlags;
  u8 unk56[8];
  u16 unk5e;
  u8 unk60[4];
  s16 phase;
  u8 unk66[6];
  void (*update)(struct SparkObject *);
};

struct OwnerObject {
  u8 unk00[8];
  s32 x;
  u8 unk0c[4];
  s32 z;
  u8 unk14[14];
  u8 layer;
};

extern const u8 gData_080f0ef8[];

extern const u8 gData_080f0e58[];
extern u8 gData_080f0e9c[];

struct Actor *ScriptObject_FindOverlappingEntryFar(struct Actor *object,
                                                   struct Vec3 *position);

extern const u8 Data_080f0ee4[];

#endif
