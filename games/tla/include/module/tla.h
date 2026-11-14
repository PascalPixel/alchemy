#ifndef ALCHEMY_MODULE_TLA_H
#define ALCHEMY_MODULE_TLA_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "battle/battle_unit.h"
#include "field/ground_mask.h"
#include "field/particle.h"
#include "module/scene/scene.h"
#include "system/ram_buffer.h"

struct AnimationEntry;
struct RenderInput;
struct WindowSprite;

struct SparkleSprite {
  u8 unk00[5];
  u8 attr5Low : 5;
  u8 fullColor : 1;
  u8 shape : 2;
  u8 unk06[3];
  u8 tileHigh : 2;
  u8 priority : 2;
  u8 palette : 4;
  u8 unk0a[2];
  s32 unk0c;
  s32 unk10;
  s32 unk14;
  s32 unk18;
  u8 unk1c[2];
  u16 unk1e;
  u8 unk20[8];
};

struct SparkleWork {
  struct SparkleSprite sprites[4];
  s16 unka0[2];
  s16 vramEntry;
  s16 unka6;
  struct Actor *object;
};

struct CounterBits {
  u8 low : 1;
  u8 rest : 7;
};

struct ParticleSprite {
  u8 unk0[8];
  u16 attr;
  u8 unkA[30];
};

struct ParticleWork {
  u8 tiles[0x500];
  struct Particle particles[64];
  struct ParticleSprite sprites[64];
  s16 next;
  u16 tile;
};

struct RockSprite {
  u8 unk00[5];
  u8 unk05_0 : 5;
  u8 unk05_5 : 1;
  u8 unk05_6 : 2;
  u8 unk06[3];
  u8 unk09_0 : 2;
  u8 priority : 2;
  u8 palette : 4;
  u8 unk0a[20];
  u16 unk1e;
  u8 unk20[8];
};

struct SpritePictures {
  u8 unk00[5];
  u8 picture;
};

struct AnimatedSprite {
  u8 unk00[0x12];
  s16 rotation;
  u8 unk14[5];
  u8 picture;
  u8 nextPicture;
  u8 unk1b[0xd];
  struct SpritePictures *pictures;
};

struct EventWork15a {
  u8 unk000[0x15a];
  u16 flag;
};

struct EncounterWork {
  u8 unk000[0x194];
  s32 bias;
  s32 threshold;
};

// The event work as the parts read it.

// Only so the shape store has an alias set of its own; the grid holds bytes.
struct ShapeCell {
  u8 shape;
};

struct EffectWork {
  u8 unknown[20];
  struct Actor *target;
};

struct EventWork1cc {
  u8 unk000[0x1cc];
  struct Actor *object;
};

struct GroundParticleWork {
  s32 entry;
  s32 tileBase;
  struct GroundParticle particles[64];
  s32 *leader;
};

struct DustParticle {
  u32 link;
  u32 attr01;
  u32 attr2;
  s32 posX;
  s32 posY;
  s32 posZ;
  u8 pad18[4];
  u16 timer;
  u8 pad1e[2];
};

struct DustWork {
  s32 vramEntry;
  s32 tileBase;
  struct DustParticle particles[64];
  u32 field808;
  u8 pad80c[8];
};

/// One falling rock: where it is, how it moves and how long it has fallen.
struct Rock {
  s32 position[3];
  s32 velocity[3];
  s32 timer;
};

struct OrbitEffect {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x1c];
  s32 radius;
  u8 unk34[0x1c];
  struct FieldSprite *sprite;
  u8 unk54;
  u8 motionFlags;
  u8 unk56[0x0e];
  u16 angle;
  u16 unk66;
  u8 unk68[4];
  void (*update)(struct OrbitEffect *effect);
};

struct SparkSprite {
  u8 unk00[4];
  u8 y;
  u8 low : 5;
  u8 fullColor : 1;
  u8 shape : 2;
  u16 attr1;
  u8 tile;
  u8 tileHigh : 2;
  u8 priority : 2;
  u8 palette : 4;
  u8 unk0a[0x14];
  u16 depth;
  u8 unk20[8];
};

struct WorldCamera {
  u8 unk000[0x11c];
  u16 unk11c;
  u16 yaw;
};

struct SwingWork {
  u32 unk00;
  s32 arg;
  struct SwingPoint points[16];
  u16 values[4];
};

struct SpriteAttr {
  unsigned y : 8;
  unsigned affine : 2;
  unsigned blendMode : 2;
  unsigned mosaic : 1;
  unsigned fullColor : 1;
  unsigned shape : 2;
  unsigned x : 9;
  unsigned affineIndex : 5;
  unsigned size : 2;
};

struct ShopCursor {
  struct WindowSprite *anchor;
  u16 x;
  u16 y;
  s16 targetX;
  s16 targetY;
  s8 active;
  s8 kind;
  u8 unk00e[2];
};

// The sprite cell at WindowSprite.sprite: a link, then the OAM words.

struct Object {
  u8 filler0[6];
  u16 src6;
  u8 src8;
  u8 filler9[6];
  u8 out15;
  u8 filler16[4];
  u8 out20;
  u8 mode21 : 2;
  u8 rest21 : 6;
  u16 pos22 : 9;
  u16 affine22 : 5;
  u16 rest22 : 2;
};

void Import_DebugMadPlantLeap(s32, s32, s32);

extern char ResourceId_FallingRock;

extern char ResourceId_DustPuffA;

extern char ResourceId_ColouredOrbs;

extern char ResourceId_StarSparkleFade;

s32 Map_GetTerrainHeight(s32 terrain, s32 x, s32 z);

extern u8 gInventorySnapshot[];

extern s8 gData_02001000[];

s32 IwramMulQ16(s32 a, s32 b) __attribute__((long_call));
extern u8 gDebugTextEnabled;
extern u32 Data_02018000[];
extern s16 gSaveSlot;
extern u16 gData_02003860;
extern u8 gSaveBuffer[];
extern u16 gData_02003a74[];

extern s32 gData_020017b0;

extern char SceneId_MarsLighthouseAerie1;

void Func_02002f90(struct FieldEffect *effect, const s32 *script);

struct RenderInput *Portrait_OpenWindowFar(s32 resource, s32 x, s32 y,
                                           s32 mode);

struct UnitBaseStats {
  s16 hp;
  s16 pp;
  s16 hpGauge;
  s16 ppGauge;
  u16 attack;
  u16 defense;
  u16 agility;
  u8 luck;
};

#endif
