#ifndef ALCHEMY_MODULE_FIELD_RESOURCE_SLOT_H
#define ALCHEMY_MODULE_FIELD_RESOURCE_SLOT_H

/// @file
/// What games/tla/src/field/resource_slot/ declares for the files that use it,
/// written once.

#include "types.h"

struct Actor;
struct MapCell;

typedef void (*ConvertFn)(void *destination, void *source, void *saved);

/// The words of an object's script. As in The Broken Seal's ActorScriptWords,
/// a flat word pointer changes one load's base and index order.
struct MotionScript {
  s32 word[1];
};

struct AnimTrack {
  u8 unk00[0x10];
  u8 *script;
  u8 index;
};

struct AnimObject {
  u8 unk00[0x18];
  u8 frame;
  u8 unk19[0xf];
  struct AnimTrack *track;
};

struct Thing {
  u8 filler0[8];
  s32 field8;
  u8 filler12[4];
  s32 field16;
};

struct MapAnimationWork {
  u8 unk00[6];
  u8 unk06;
  u8 unk07;
  u8 unk08[84];
};

struct PlacedSprite {
  u8 unk00[9];
  u8 low09 : 2;
  u8 priority : 2;
  u8 high09 : 4;
  u8 unk0a[2];
  s32 scale; ///< 16.16 fixed point
  u8 slot;
  u8 retainCacheEntry; ///< bit 0
  u8 unk12[7];
  u8 dirty;
  u8 unk1a[0x0b];
  u8 low25 : 2;
  u8 shadowPriority : 2;
  u8 high25 : 4;
};

struct CameraPoint {
  s32 x;
  s32 y;
  s32 z;
};

struct MapPriorityWork {
  u8 unk000[0x101];
  s8 basePriority;
};

struct FixedCamera {
  struct CameraPoint eye;
  struct CameraPoint target;
  struct CameraPoint *eyeOverride;    ///< when set, used as the eye
  struct CameraPoint *targetOverride; ///< when set, used as the target
};

struct PlacedObject {
  u8 unk00[2];
  u16 active;
  u8 unk04[2];
  u16 angle;
  s32 position[4];
  s32 scale[2];
  u8 unk20[2];
  u8 layer;
  u8 flags;
  u8 unk24[0x2c];
  void *target;
  u8 kind;
  u8 unk55[0x2b];
};

struct PlayerSprite {
  u8 unk00[9];
  u8 low09 : 2;
  u8 priority : 2;
  u8 high09 : 4;
  u8 unk0a[7];
  u8 shape11;
  u16 rotation;
  u8 unk14[5];
  u8 dirty;
  u8 flags;
};

struct MapFlagsWork {
  u8 unk000[0x17];
  u8 groundEffects; ///< bits: 3..2 the wading kind | 1..0 footprints
};

struct LayeredSceneEntry {
  u16 resources[6];
};

struct LayeredSceneLayer {
  u8 x;
  u8 y;
  s8 scaleX;
  s8 scaleY;
  s8 speedX;
  s8 speedY;
  u8 periodX;
  u8 periodY;
};

struct LayeredSceneHeader {
  u8 origin[4];
  u8 priority[3];
  u8 charBase[3];
  u16 unk0a;
  struct LayeredSceneLayer layers[3];
  s32 tiles;
  s32 collision;
  s32 tilemap;
  s32 shapes;
  s32 animation;
  s32 blend;
  s32 script;
};

struct MapLayerScroll {
  s32 x, y;
  s32 offsetX, offsetY;
  s32 scaleX, scaleY;
  s32 speedX, speedY;
  s32 phaseX, phaseY;
  u16 maskX, maskY;
  u16 cellX, cellY;
  struct MapCell *cells;
  u8 *shapes;
};

struct MapScrollWork {
  u8 unk000[0x14];
  u16 flags;
  u8 unk016[0xe4 - 0x16];
  s32 viewX, viewY;
  s32 minX, minY, maxX, maxY;
  u8 unk0fc[5];
  u8 unk101;
  u8 unk102[2];
  u8 enabled[3];
  u8 unk107;
  struct MapLayerScroll layers[3];
};

struct WorldView {
  s32 *origin;
  s32 shakeX, shakeY, shakeDecay;
  u8 *unk010;
  u16 unk014;
  u8 unk016;
  u8 unk017[0xcd];
  s32 viewX;
  s32 viewY;
  u8 unk0ec[0x18];
  u16 windowTop;
  u16 windowBottom;
  u8 unk108[0x14];
  u16 pitch;
  u16 yaw;
  u8 unk120[0x83c];
  s32 farPlane;
  s32 distance;
  u8 unk964[4];
  s32 unk968;
  u16 unk96c;
  u8 unk96e[3];
  s8 flat;
  s8 paused;
  u8 unk973[3];
  s8 fixedEye;
};

struct EyeOffset {
  s32 x, y, z;
};

struct MapTileWindow_08010d48 {
  s32 *position;
  u8 unk004[0x144];
  u16 tiles[32][32];
};

struct Unk_WakeActor {
  u8 unk00[0x63];
  u8 unk63;
};

struct Unk_ClimbEventWork {
  u8 unk000[0xd4a];
  /// Read as the low byte of a halfword: as a u8 or s16 field the read waits
  /// for the sprite store (8 of 965 instructions differ in tla-en).
  u16 unkd4a : 8;
  u16 unkd4b : 8;
};

struct Unk0802a124Actor {
  u8 unk00[0x62];
  u8 unk62;
  u8 unk63;
};

/// A map layer as its renderers read it: its scroll, then the window over
/// the map's cells (masks and origins that wrap the source cells).
struct MapLayerWindow {
  s32 x, y;
  s32 offsetX, offsetY;
  s32 scaleX, scaleY;
  s32 speedX, speedY;
  s32 phaseX, phaseY;
  u16 maskX, maskY;
  u16 originX, originY;
  struct MapCell *cells;
  u8 *terrainIds;
};

struct TilePos {
  s32 x, y;
};

struct MapCopyCommand {
  u16 source;
  s16 y, width, height, delay;
} __attribute__((packed, aligned(2)));

struct MapHeader {
  u8 unk00[44];
  s32 cells;
  s32 shapes;
};

struct PerspectiveTiles {
  u8 unk000[0x114];
  u32 *tiles;
  u8 unk118[0x830];
  u16 quarterTiles[8];
};

struct SpriteAngle {
  u8 unk00[18];
  u16 angle;
};

struct LeaderUnit {
  u8 unk00[0x38];
  s16 hp;
};

/// The requests the configured keys raise.
// Named as in ☀️'s FieldWork.
struct EventPrompt {
  u8 unk000[0x15a];
  u16 menu;
  u16 examine;
  u16 start;
  u8 unk160[6];
  u16 choice[2];
  u16 result;
  u8 unk16c[30];
  s8 mode;
  u8 unk18b[0xb21];
  s8 pendingSecond;
};

struct Child {
  u8 unk00[9];
  u8 low9 : 2;
  u8 layer : 2;
  u8 high9 : 4;
  u8 unk0a[0x10];
  u8 value1a;
};

struct WorldEventWork {
  u8 unk000[0xd39];
  s8 unkd39;
  u8 terrainKind;
};

/// The actor as SC_SYNC reads it: the frames a solid object has blocked it
/// are a byte of the actor itself, which struct Actor leaves unnamed.
struct WaitingActor {
  u8 unk00[0x38];
  s32 targetX;
  s32 targetY;
  s32 targetZ;
  u8 unk44[0x12];
  u8 unk56;
  u8 unk57[9];
  u8 delay;
};

struct ScriptObject {
  u8 unk00[0x6c];
  s32 value;
};

struct PartyKeys {
  u8 unk000[0x23c];
  u16 dashKeys;
};

struct UnitHp {
  u8 unk00[0x38];
  s16 hp;
};

enum { RESOURCE_SLOT_COUNT = 8 };

struct ResourceSlotNumber {
  u16 resource;
  s16 number;
};

struct MetadataSlotState {
  u8 unk00[0x0c];
  s32 scale;
  u8 unk10[4];
  u8 width;
  u8 height;
  s8 offsetX;
  s8 offsetY;
  u8 unk18[3];
  u8 count;
  u8 unk1c[0x0c];
  s32 slots[4];
};

struct ResourceObjectWork {
  u8 unk00[16];
  u8 resource; ///< VRAM cache index.
  u8 flags;    ///< Bit 0: do not return the VRAM cache entry.
  u8 unk12[22];
  void *children[4];
};

struct MapScanline {
  u8 unk00[20];
};

struct MapAffinePair {
  u32 words[8];
};

struct MapAffineWork {
  struct MapScanline scanlines[160];
  struct MapAffinePair pages[2][160];
};

struct PerspectiveWindow {
  u8 unk000[0x104];
  u16 windowTop;
  u16 windowBottom;
  u16 shownTop;
  u16 shownBottom;
  u16 windowPending;
};

s32 Func_080ad318(void);

extern char ResourceId_Map000;

extern char ResourceId_Data199;

extern const u8 Data_0802146c[];

extern u8 Data_0802146cSize[];

s32 Func_080c8770(s32 x, s32 z, s32 kind);

extern u8 Render_ConvertMapCode[];
extern u8 Render_ConvertMapCodeSize[];

extern const u16 gData_0802f380[][6];

extern u8 gData_0202e002[];

s32 Func_080c8710(struct Actor *object);

s32 Func_080c8700(struct Actor *object);

s32 Func_080c8828(s32 arg);

extern struct ResourceSlotNumber gResourceSlot_NumberTable[];
extern const u8 gData_0802e89c[];

extern u8 Render_CopyStridedCode[];
extern u8 Render_CopyStridedCodeSize[];

struct IconSprite__resource_slot;

struct IconOwner {
  u8 unk00[80];
  struct IconSprite__resource_slot *sprite;
  u8 unk54[8];
  u8 unk5c;
  u8 unk5d[5];
  u8 iconId;
};

#endif
