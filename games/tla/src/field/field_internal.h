#ifndef ALCHEMY_FIELD_INTERNAL_H
#define ALCHEMY_FIELD_INTERNAL_H

/// @file
/// What every part of the field object shares: its includes and the names two
/// or more parts use. field.c includes it before the parts, and each part
/// includes it too, so that a part read on its own still resolves. It places
/// no bytes.

#include "types.h"
#include "system/scene.h"
#include "field/sprite.h"
#include "field/perspective.h"
#include "system/ram_buffer.h"
#include "system/io_reg.h"
#include "system/dma.h"
#include "system/heap_release.h"
#include "system/iwram_call.h"
#include "graphics/resource.h"
#include <system/affine.h>
#include "system/sprite_metadata.h"
#include "actor.h"
#include "actor_view.h"
#include "module/field/field.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/ram_code.h"

struct Actor;
struct FieldPosition;

/// The collision at an actor's probe point (tile_animation.c): 2 for a cell
/// with no tile, 1 a rise of more than 8 pixels, -1 a drop of more than 12,
/// else 0.
s32 MapGrid_CheckCollision(struct Actor *actor, struct FieldPosition *position);
s32 MapGrid_CheckCollisionAbove(struct Actor *actor,
                                struct FieldPosition *position);

typedef void (*ConvertFn)(void *destination, void *source, void *saved);

/// The words of an object's script. As in The Broken Seal's ActorScriptWords,
/// a flat word pointer changes one load's base and index order.
struct MotionScript {
  s32 word[1];
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

struct PlayerSprite {
  u8 unk00[5];
  u8 low05 : 2;
  u8 objectMode : 2;
  u8 high05 : 4;
  u8 unk06[3];
  u8 low09 : 2;
  u8 priority : 2;
  u8 high09 : 4;
  u8 unk0a[7];
  u8 shape11;
  u16 rotation;
  u8 unk14[5];
  u8 dirty;
  u8 flags;
  u8 unk1b[0x11];
  u8 *entry;
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

struct EyeOffset {
  s32 x, y, z;
};

struct Unk_WakeActor {
  u8 unk00[0x63];
  u8 unk63;
};

struct Unk_ClimbEventWork {
  u8 unk000[0xd4a];
  /* FAKEMATCH(signature): read as the low byte of a halfword. */
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

struct SpriteAngle {
  u8 unk00[18];
  u16 angle;
};

struct Child {
  u8 unk00[9];
  u8 low9 : 2;
  u8 layer : 2;
  u8 high9 : 4;
  u8 unk0a[0x10];
  u8 value1a;
};

/// The actor as SC_SYNC clears it when it gives up waiting: its target, arrival
/// axis and blocked count.
struct WaitingActor {
  u8 unk00[0x38];
  s32 targetX;
  s32 targetY;
  s32 targetZ;
  u8 unk44[0x12];
  u8 arrivalAxis;
  u8 unk57[9];
  u8 refused;
};

enum { RESOURCE_SLOT_COUNT = 8 };

struct ResourceSlotNumber {
  u16 resource;
  s16 number;
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

extern char ResourceId_Data199;

extern u8 Transform_UpdateVerticesCodeSize[];

extern u8 Render_ConvertMapCode[];

extern u8 Render_ConvertMapCodeSize[];

extern u8 gData_0202e002[];

extern struct ResourceSlotNumber gResourceSlot_NumberTable[];

extern const u8 gActor_ShadowTiles[];

extern u8 Render_CopyStridedCode[];

extern u8 Render_CopyStridedCodeSize[];

struct IconOwner {
  u8 unk00[80];
  struct IconSprite__resource_slot *sprite;
  u8 unk54[8];
  u8 unk5c;
  u8 unk5d[5];
  u8 iconId;
};

#endif // ALCHEMY_FIELD_INTERNAL_H
