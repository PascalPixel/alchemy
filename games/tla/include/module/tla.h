#ifndef ALCHEMY_MODULE_TLA_H
#define ALCHEMY_MODULE_TLA_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "battle/battle_unit.h"
#include "field/ground_mask.h"
#include "field/particle.h"
#include "module/scene/scene.h"

struct AnimationEntry;
struct RenderInput;
struct WindowSprite;

struct SparkleSprite {
  u8 unk00[12];
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

struct ResourceTableEntry {
  u16 unk0;
  u16 offset; ///< in bytes; its tile is offset / 32
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
struct EventWorkView {
  u8 unk000[0x15a];
  s16 unk15a;
  s16 unk15c;
  s16 unk15e;
  u8 unk160[4];
  s16 unk164;
  s16 unk166;
  u8 unk168[0x1c];
  s16 blockedSteps;
  u16 pushKind;
  u8 unk188[4];
  u8 settings[8];
  s32 unk194;
  s32 unk198;
};

// Only so the shape store has an alias set of its own; the grid holds bytes.
struct ShapeCell {
  u8 shape;
};

struct ChildObject {
  u8 unk00[2];
  u16 value;
  u8 unk04[8];
  s32 active;
  u8 unk10[5];
  u8 mode;
};

/// The sprite as ResourceObject_Create fills it: the image's three attribute
/// words, its layout, then the shadow's three words.
struct ResourceSprite {
  u32 imageAttributes[3];
  s32 scale;
  u8 slot;
  u8 retainCacheEntry : 1;
  u8 useOwnScale : 1;
  u8 unk11 : 6;
  u16 rotation;
  u8 width;
  u8 height;
  s8 offsetX;
  s8 offsetY;
  u8 lastNo;
  u8 dirty;
  u8 flags;
  u8 count;
  u32 shadowAttributes[3];
  struct AnimationEntry *entries[4];
};

struct OwnerStats {
  u8 unk000[0x10];
  s16 baseHp;
  s16 basePp;
  s16 hpRatio;
  s16 ppRatio;
  u16 baseAttack;
  u16 baseDefense;
  u16 baseAgility;
  u8 baseLuck;
  u8 baseTurns;
  u8 base20;
  u8 base21;
  u8 unk022[2];
  s16 baseElement[4][2];
  s16 maxHp;
  s16 maxPp;
  s16 hp;
  s16 pp;
  s16 atk;
  s16 def;
  s16 agi;
  u8 luck;
  u8 turns;
  u8 stat44;
  u8 stat45;
  u8 unk046[2];
  s16 element[4][2];
  u8 unk058[0x80];
  u16 equipment[15];
  u8 unk0f6[0x12];
  u32 djinn[4];
  u8 unk118[0x10];
  u8 character;
  u8 classId;
  u8 unk12a[2];
  s8 elementLevel[4];
  s8 curse;
  u8 unk131[2];
  s8 attackLevel;
  u8 unk134;
  s8 defenseLevel;
  u8 unk136;
  s8 resistLevel;
  u8 unk138[3];
  u8 unk13b[7];
  u8 bonus142;
  u8 bonus143;
  u8 extraTurn;
  u8 unk145[2];
  s8 agilityLevel;
  u8 unk148[2];
  u16 id;
};

struct WindowTiles {
  u8 unk00[3];
  u8 dirty; ///< bits: one per four tile rows, from bit 1
  u8 unk04[4];
  u16 cells[32][32];
};

struct TitleSprite {
  u32 next; ///< the render slot link
  u8 y;
  u8 attr0High; ///< shape and mode bits of attribute 0
  u16 x : 9;
  u16 attr1High : 7; ///< flip and size bits of attribute 1
  s32 tile;
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

struct BurstSprite {
  s32 object;
  s32 x, z, targetX, targetZ, originX, originZ, speed, speedMax, acceleration;
  s32 scaleX;
  s32 scaleY;
  u16 heading;
  s16 turnStepMax;
  void *callback;
  u16 age;
  s16 callbackDelay;
  s16 orbitRadius;
  s16 orbitAngle;
  s8 state;
  s8 stopAtTarget;
  s8 turnStepLimited;
  s8 updateMotion;
  s8 render;
  s8 active;
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

struct ProjectedPoint {
  s32 x;
  s32 y;
  s32 z;
};

struct TargetWindow {
  u8 unk00[8];
  u16 width;
  u16 height;
  s16 x;
  s16 y;
};

struct PreviewUnit {
  u8 unk000[0x38];
  s16 hp;
  s16 pp;
  u16 atk;
  u16 def;
  u16 agi;
  u8 luck;
  u8 unk043[0x15];
  u16 psynergy[1];
  u8 unk05a[0xcf];
  u8 classId;
  u8 unk12a[0x22];
};

// The sprite cell at WindowSprite.sprite: a link, then the OAM words.
struct ArrowSprite {
  u32 link;
  union {
    u32 word;
    struct {
      unsigned y : 8;
      unsigned flags : 8;
      unsigned x : 9;
      unsigned high : 7;
    } bits;
  } attributes;
  union {
    u32 value;
    struct {
      unsigned index : 10;
      unsigned high : 6;
    } bits;
  } tile;
};

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

extern u8 gData_03001110;

void Func_080ad090(void);

void ObjectSystem_InitializeFar(s32 mode);

s32 Djinn_GetDefinitionHeaderFar(s32 element, s32 index);

extern u32 Data_02018000[];

void Animation_ApplyChildArgumentFar(void *, s32);

u32 Math_UnsignedMulHigh(u32 a, u32 b);

void BattleActor_SpawnObjectsForListFar(s16 *list, s32 mode);

void ReleaseBattleObjectRecordsFar(s32 actor);
void BattleActor_CommitPlacementFar(void);

s32 MapGrid_GetTerrainHeightAboveFar(s32 x, s32 z, s32 layer);

void *Func_02000dec(s32, s32, s32, s32);
void Func_02000f14(s32, s32, s32);

extern char ResourceId_FallingRock;

struct Actor *Func_080200c0(s32 kind, s32 x, s32 y, s32 z);

s32 Inventory_GetQuantityFar(s32 member, s32 slot);

extern char ResourceId_DustPuffA;

extern char ResourceId_ColouredOrbs;

extern u8 gMapShapeGrid[];

extern char ResourceId_StarSparkleFade;

extern s8 gData_02001000[];

s32 Party_FindOwnerWithActionFar(s32 action);

void Func_08038348(s32 first, s32 second);

s32 Map_GetTerrainHeight(s32 terrain, s32 x, s32 z);

extern void (*gData_030011f8)(void);

extern u32 gMapBlocks[];

s32 IwramMulQ16(s32 a, s32 b) __attribute__((long_call));

extern s16 gData_020036d0;
extern u16 gData_02003860;

extern char ResourceId_ShopGemTiles;

/// Item menu workspace (known fields only).
extern char ResourceId_MenuArrowTiles;
extern char ResourceId_MenuSmallDownArrowTiles;

extern char ResourceId_ShopUpArrowTiles;
extern char ResourceId_ShopDownArrowTiles;

extern u8 gData_02000000[];

s32 Func_02000df4(s32 *member, s32 *slot);

extern u8 gData_020023c4[];

extern s32 gData_020017b0;

extern u16 gData_02003a74[];

extern char SceneId_Ov6aeRoom1;

void Func_02002f90(struct FieldEffect *effect, const s32 *script);

struct RenderInput *Func_080380f8(s32 resource, s32 x, s32 y, s32 mode);

extern struct BattleUnit gOwnerStates[];

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
