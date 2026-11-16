#ifndef ALCHEMY_SPRITE_H
#define ALCHEMY_SPRITE_H

/// @file
/// Animated sprites: their layers, the sprite picture caches and the actor
/// system's work.

#include "edition.h"
#include "fixed_math.h"
#include "types.h"
#include "static_assert.h"
#include "graphics/render_placement.h"

/// SpriteAnimation.param: how Render_DecodePicture colours a layer's pixels.
/// Modes 1 to 6 and 9 to 11 run them through a 256-byte table (the five
/// gResourceSlot_ConversionTables, then the four gSpriteColorRemapTables
/// ones); the solid modes paint every drawn pixel one colour of the basic
/// sprite palette. Mode 8 is a second white and mode 12 reads past the tables.
enum SpriteColorMode {
  SPRITE_COLOR_NORMAL = 0,
  SPRITE_COLOR_GRB = 1,         ///< red and green swapped
  SPRITE_COLOR_RBG = 2,         ///< green and blue swapped
  SPRITE_COLOR_BGR = 3,         ///< red and blue swapped
  SPRITE_COLOR_GBR = 4,         ///< red to blue, green to red, blue to green
  SPRITE_COLOR_BRG = 5,         ///< red to green, green to blue, blue to red
  SPRITE_COLOR_GREY = 6,        ///< greyscale
  SPRITE_COLOR_WHITE = 7,       ///< solid colour 15, white
  SPRITE_COLOR_BLUE_TINT = 9,   ///< blue raised to full
  SPRITE_COLOR_RED_TINT = 10,   ///< red raised to full
  SPRITE_COLOR_GREEN_TINT = 11, ///< green raised to full
  SPRITE_COLOR_FILL = 13,       ///< solid gDecodeFillByte
  SPRITE_COLOR_BLACK = 14,      ///< solid colour 1, black
  SPRITE_COLOR_HIDDEN = 15      ///< solid colour 0, transparent
};

/// Passed to Actor_ConfigureSpriteColor in place of a SpriteColorMode: the
/// actor blinks white every few frames (Actor_UpdateBlink) until it is given a
/// mode again.
#define SPRITE_COLOR_BLINK 0x100

/// SpriteAnimation.picture and pictureBase when no picture is shown.
#define SPRITE_PICTURE_NONE 0xff

/// One animation layer of a sprite: its animation id, pictures, running script
/// and timer.
struct SpriteAnimation {
  s16 animationId; ///< the sprite id whose metadata and pictures it uses
  /// Time left on the current picture in sixteenths of a frame, read signed:
  /// each timed command adds its argument times 16, each frame takes off step.
  u16 timer;
  /// The facing table its pictures follow (from the metadata, or
  /// animation command 240).
  u8 kind;
  u8 colorMode;    ///< SpriteColorMode its pixels are drawn with
  u8 priority;     ///< drawing order 0 to 3 among the layers; above 3 not drawn
  u8 mode;         ///< picture codec: 1 type-01, 3 LZ, anything else raw
  void **pictures; ///< picture pointers, from the metadata or a picture cache
  u8 **scripts;    ///< the metadata's command scripts, one per animation number
  u8 *script;      ///< the running script; null once command 239 ends it
  u8 scriptOffset; ///< byte offset of the next command in script
  u8 step;         ///< taken off timer each frame; 16 is normal speed
  u8 picture; ///< picture shown, pictureBase plus the facing offset; 255 none
  u8 pictureBase; ///< picture the script chose, before facing; 255 blank
};

/// A sprite picture cache: the tag of the animation loaded into it (slot number
/// and animation id) and the buffer holding its pictures.
struct SpritePictureCache {
  s32 tag;        ///< (slot << 12) | animation id
  void *pictures; ///< the decoded pictures, their offsets made pointers
};

/// The actor system's work: the flag that suspends sprite projection, the
/// outline's edge and fill colours, and the sprite picture caches. Heap slot 6.
struct ActorSystemWork {
  u16 used; ///< sprite pixel bytes composed since the camera reset it
  u8 unusedAfterUsed[2]; ///< no code names it
  s16 suspended;       ///< nonzero while a menu has suspended sprite projection
  u8 edge;             ///< outline edge colour; 15 when the field sets up
  u8 fill;             ///< outline fill colour; 0 when the field sets up
  u8 spriteGroups[16]; ///< the sprite group tables, taken in turn by count
  s32 count;           ///< sprite group tables taken from spriteGroups
  struct SpritePictureCache
      pictureCaches[8]; ///< found by tag (SpritePictureCache_FindByTag)
};

STATIC_ASSERT(offsetof(struct ActorSystemWork, suspended) == 4);
STATIC_ASSERT(offsetof(struct ActorSystemWork, count) == 0x18);
STATIC_ASSERT(offsetof(struct ActorSystemWork, pictureCaches) == 0x1c);
STATIC_ASSERT(sizeof(struct ActorSystemWork) == 0x5c);

/// The hardware sprite attributes one part of an animated sprite (its image or
/// its shadow) is drawn with: the link to the next part queued at its depth,
/// then the OAM entry's attributes 0 to 3.
struct AnimationSpritePart {
  u8 unused[4]; ///< no code names it
  u16 y : 8;
  u16 affine : 2;
  u16 objectMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unusedAfterPalette[2]; ///< no code names it
};

/// Sprite.flags.
enum SpriteFlag {
  SPRITE_FLAG_SHADOW = 1 << 0, ///< its shadow is drawn; set when created
  /// Drawn as an outline, in the actor system's edge and fill colours.
  SPRITE_FLAG_OUTLINED = 1 << 1
};

/// The VRAM cache entry of the actors' shared shadow (gActor_ShadowTiles),
/// each sprite's part 1.
#define VRAM_CACHE_ENTRY_ACTOR_SHADOW 93

/// An animated sprite: its image, scale, rotation, size, its shadow and up to
/// four sprite animations.
struct Sprite {
  struct AnimationSpritePart image;
  FIXED scale; ///< 16.16 fixed point, FIXED_ONE full size
  u8 slot;     ///< VRAM cache entry holding its tiles
  /// Keeps the VRAM cache entry when it is released or off screen.
  u8 retainCacheEntry : 1;
  /// Drawn at its own scale, not scaled by distance (fixed camera).
  u8 useOwnScale : 1;
  u8 sunkRows : 6; ///< rows the picture is sunk (flags bit 3) or shaded (bit 2)
  u16 rotation;    ///< 1/65536 turns
  u8 width;        ///< pixels; zero marks a free sprite
  u8 height;       ///< pixels
  s8 offsetX;      ///< pixels
  s8 offsetY;      ///< pixels
  /// The animation number last selected, which selecting again skips.
  u8 lastNo;
  u8 dirty; ///< nonzero redraws its pixels into VRAM when next drawn
  /// bits: 1 outlined in the actor system's edge and fill colours |
  /// 0 shadow drawn (set when created)
  u8 flags;
  u8 count; ///< layers in use, up to the last one present
  /// Its shadow, shown while flags bit 0 is set.
  struct AnimationSpritePart shadow;
  struct SpriteAnimation *entries[4]; ///< its layers; empty ones null
};

STATIC_ASSERT(sizeof(struct SpriteAnimation) == 0x18);
STATIC_ASSERT(sizeof(struct AnimationSpritePart) == 0x0c);
STATIC_ASSERT(offsetof(struct Sprite, scale) == 0x0c);
STATIC_ASSERT(offsetof(struct Sprite, rotation) == 0x12);
STATIC_ASSERT(offsetof(struct Sprite, width) == 0x14);
STATIC_ASSERT(offsetof(struct Sprite, shadow) == 0x1c);
STATIC_ASSERT(sizeof(struct Sprite) == 0x38);
STATIC_ASSERT(offsetof(struct Sprite, entries) == 0x28);

/// An actor's sprite (struct Sprite) as the code that sets its image and shadow
/// priorities reads it (the actors' freeze and placement, an actor taking the
/// leader's layer): a byte at a time, under struct Sprite's names.
struct PlacedSprite {
  u8 unused[9];    ///< no code names it
  u8 tileHigh : 2; ///< tile number bits 9..8
  u8 priority : 2;
  u8 palette : 4;
  u8 unusedAfterPalette[2]; ///< no code names it
  FIXED scale;              ///< 16.16 fixed point
  u8 slot;
  u8 retainCacheEntry; ///< bit 0
  u16 rotation;
  u8 unusedAfterRotation[5]; ///< no code names it
  u8 dirty;
  u8 flags;
  u8 unusedAfterFlags[10]; ///< no code names it
  u8 shadowTileHigh : 2;
  u8 shadowPriority : 2;
  u8 shadowPalette : 4;
};

/// SpriteAnimation.step at normal speed, as Actor_SetAnimationSpeed sets it:
/// a timed command's frames run as written.
#define ANIMATION_STEP_NORMAL 16

/// Animation numbers of a person's sprite, for Actor_SetAnimation. A sprite
/// that is not a person (a bird, a dragon, a pillar) numbers its own.
enum ActorAnimation {
  /// Standing still: the sprite's first frame held, without the blinks and
  /// small gestures of ANIM_STAND.
  ANIM_STILL = 0,
  ANIM_STAND = 1,
  ANIM_WALK = 2,
  ANIM_NOD = 3,
  ANIM_SHAKE_HEAD = 4,
  /// Running, leaning forward, four frames a step to the walk's six. This and
  /// the leader's other poses below are as the leader's sprite draws them, as
  /// The Broken Seal numbers them.
  ANIM_LEADER_RUN = 5,
  ANIM_LEADER_SPRING = 6, ///< leaning back for a moment before a jump
  ANIM_LEADER_LEAP = 7,   ///< in mid-stride, leaning forward
  /// The leader leaning into what it pushes, as when it walks into a pushable
  /// actor (Actor_UpdatePlayerControl).
  ANIM_PUSH = 8,
  /// ANIM_STAND's idle with more glances to each side, as when the leader's
  /// step is blocked.
  ANIM_LEADER_LOOK_AROUND = 9,
  ANIM_LEADER_CLIMB = 10, ///< climbing a wall, seen from behind
  /// Stepping off the top of a ladder onto the ground above.
  ANIM_LEADER_CLIMB_OVER_TOP = 11,
  ANIM_LEADER_CLING = 12, ///< still on a wall, seen from behind
  /// ANIM_LEADER_CLIMB_OVER_TOP played backwards: stepping from the top of a
  /// ladder onto its rungs.
  ANIM_LEADER_CLIMB_DOWN_FROM_TOP = 13,
  ANIM_LEADER_CLIMB_EAST = 14, ///< reaching along a wall as he climbs east
  ANIM_LEADER_CLIMB_WEST = 15, ///< reaching along a wall as he climbs west
  /// Holding out a glowing hand, as the party's sprites cast Psynergy.
  ANIM_CAST_PSYNERGY = 17,
  /// Lying collapsed on the ground.
  ANIM_COLLAPSE = 19,
  ANIM_LEADER_LEAN_BACK = 22, ///< leaning back, his head thrown back
  /// The arrivals and leavings of the animated exits, each four frames of the
  /// leader turning: an arrival grows from a small figure seen side-on to full
  /// size facing the player, a leaving turns from his back to side-on as he
  /// shrinks and is then hidden.
  ///@{
  ANIM_LEADER_ARRIVE_ALTERNATE = 23,
  ANIM_LEADER_ARRIVE = 24,
  ANIM_LEADER_LEAVE_ALTERNATE = 25,
  ANIM_LEADER_LEAVE = 26,
  ///@}
  /// ANIM_WALK's six pictures in reverse order.
  ANIM_LEADER_WALK_BACKWARDS = 27,
  ANIM_LEADER_ARMS_RAISED = 28, ///< both arms raised over his head
  /// ANIM_LEADER_RUN again, on every party member's sheet.
  ANIM_PARTY_RUN = 30
};

#endif
