#ifndef ALCHEMY_SPRITE_H
#define ALCHEMY_SPRITE_H

/// @file
/// Animated sprites: their layers, pictures and colour modes, and the resource
/// slots they draw from.

#include "edition.h"
#include "types.h"
#include "static_assert.h"
#include "render_placement.h"

/// SpriteAnimation.colorMode: how Render_DecodePicture colours a layer's
/// pixels. Modes 1 to 6 and 9 to 11 run them through a 256-byte table (the five
/// gSpriteColorTables, then the four gSpriteColorRemapTables
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

/// Passed to Actor_ConfigureSpriteColor in place of a SpriteColorMode:
/// the actor blinks white every few frames (Actor_UpdateBlink)
/// until it is given a mode again.
#define SPRITE_COLOR_BLINK 0x100

/// A sprite animation's picture or pictureBase when it shows none:
/// Sprite_ComposePicture skips it until an animation is selected.
#define SPRITE_PICTURE_NONE 0xff

/// An animation number's bit 7: keep the place and timer of the script already
/// running. Sprite_StartAnimation masks it off with
/// SPRITE_ANIMATION_INDEX_MASK; SpriteAnimation_Start keeps it in the index.
#define SPRITE_ANIMATION_KEEP_PLACE 0x80
#define SPRITE_ANIMATION_INDEX_MASK 0x7f

/// One of a sprite's four sprite animations: its animation id, pictures,
/// running script and timer.
struct SpriteAnimation {
  s16 animationId; ///< the sprite id whose metadata and pictures it uses
  /// Time left on the current picture in sixteenths of a frame, read signed:
  /// each timed command adds its argument times 16, each frame takes off step.
  u16 timer;
  /// The facing table its pictures follow (from the metadata, or
  /// ANIMCMD_SET_KIND); zero marks a free sprite animation.
  u8 kind;
  u8 colorMode; ///< SpriteColorMode its pixels are drawn with; 0 when taken
  u8 priority; ///< drawing order 0 to 3 among the animations; above 3 not drawn
  u8 mode;     ///< picture codec: 1 tagged LZ, 3 LZ, anything else raw
  void **pictures; ///< picture pointers, from the metadata or a picture cache
  u8 **scripts;    ///< the metadata's command scripts, one per animation number
  u8 *script;      ///< the running script; null when stopped (ANIMCMD_END)
  u8 scriptOffset; ///< byte offset of the next command in script
  u8 step;         ///< taken off timer each frame; 16 is normal speed
  u8 picture;      ///< picture shown, pictureBase plus the facing offset;
                   ///< SPRITE_PICTURE_NONE for none
  u8 pictureBase;  ///< picture the script chose, before facing;
                   ///< SPRITE_PICTURE_NONE for none
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
  u16 used;      ///< sprite pixel bytes composed since the camera reset it
  u8 unused[2];  ///< no code names it
  s16 suspended; ///< nonzero while a menu has suspended sprite projection
  u8 edge;       ///< outline edge colour; 15 when the field sets up
  u8 fill;       ///< outline fill colour; 0 when the field sets up
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
/// its shadow) is drawn with.
struct AnimationSpritePart {
  /// The link to the next part queued in the same depth slot this frame
  /// (Oam_AddSpriteAtDepth).
  u8 link[4];
  /// The OAM entry's attributes 0 to 2.
  ///@{
  u16 y : 8;          ///< screen pixels
  u16 affine : 2;     ///< 0 plain, 1 affine, 3 affine at double size
  u16 objectMode : 2; ///< an enum ObjMode
  u16 mosaic : 1;
  u16 fullColor : 1;   ///< 256 colours; set for both parts when created
  u16 shape : 2;       ///< 0 square, 1 wide, 2 tall
  u16 x : 9;           ///< screen pixels
  u16 affineIndex : 5; ///< affine matrix, or 8 for a mirrored plain sprite
  u16 size : 2;        ///< with shape, the sprite's size
  u16 tile : 10;       ///< first sprite tile in VRAM
  u16 priority : 2;    ///< 2 when created
  u16 palette : 4;     ///< 16-colour palette; unused at 256 colours
  ///@}
  u8 attr3[2]; ///< OAM attribute 3; zero when created, not named by TBS code
};

/// An animated sprite: its image and shadow, scale, rotation, size and up to
/// four sprite animations.
struct Sprite {
  struct AnimationSpritePart image;
  /// Its shadow, shown while flags bit 0 is set.
  struct AnimationSpritePart shadow;
  s32 scale; ///< 16.16 fixed point, Q16_ONE full size
  u8 slot;   ///< VRAM cache entry holding its tiles
  /// Keeps the VRAM cache entry when it is released or off screen.
  u8 retainCacheEntry : 1;
  /// Drawn at its own scale, not scaled by distance (fixed camera).
  u8 useOwnScale : 1;
  u8 unusedFlags : 6; ///< no code names them
  u16 rotation;       ///< 1/65536 turns
  u8 width;           ///< pixels; zero marks a free sprite
  u8 height;          ///< pixels
  s8 offsetX;         ///< pixels
  s8 offsetY;         ///< pixels
  /// The animation number last selected, which selecting again skips.
  u8 lastNo;
  u8 dirty; ///< nonzero redraws its pixels into VRAM when next drawn
  /// bits: 1 outlined in ActorSystemWork's edge and fill colours |
  /// 0 shadow drawn (set when created)
  u8 flags;
  u8 count; ///< layers in use, up to the last one present
  struct SpriteAnimation *entries[4]; ///< its layers; empty ones null
};

STATIC_ASSERT(sizeof(struct SpriteAnimation) == 0x18);
STATIC_ASSERT(sizeof(struct AnimationSpritePart) == 0x0c);
STATIC_ASSERT(offsetof(struct Sprite, scale) == 0x18);
STATIC_ASSERT(offsetof(struct Sprite, rotation) == 0x1e);
STATIC_ASSERT(offsetof(struct Sprite, width) == 0x20);
STATIC_ASSERT(sizeof(struct Sprite) == 0x38);
STATIC_ASSERT(offsetof(struct Sprite, entries) == 0x28);

/// Animation numbers of an actor's sprite, for Import_ActorSetAnimation
/// and Sprite_StartAnimation. The menus' and shops' party and Djinn
/// figures are the same field sprites and play the same numbers.
enum ActorAnimation {
  /// Standing still: nearly every sprite's first frame held, without the
  /// blinks and small gestures of ANIM_STAND.
  ANIM_STILL = 0,
  ANIM_STAND = 1,
  ANIM_WALK = 2,
  ANIM_NOD = 3,
  ANIM_SHAKE_HEAD = 4,
  /// Running, leaning forward, four frames a step to the walk's six. This and
  /// the leader's other poses below are as Isaac's sprite (spr_000.png) draws
  /// them.
  ANIM_LEADER_RUN = 5,
  ANIM_LEADER_SPRING = 6, ///< leaning back for a moment before a jump
  ANIM_LEADER_LEAP = 7,   ///< in mid-stride, leaning forward
  /// The leader leaning into what it pushes, as scenes play it while a statue
  /// or stone pillar slides ahead of it.
  ANIM_PUSH = 8,
  /// ANIM_STAND's idle with more glances to each side, as when a whirlwind
  /// catches the leader.
  ANIM_LEADER_LOOK_AROUND = 9,
  ANIM_LEADER_CLIMB = 10, ///< climbing a wall, seen from behind
  /// Stepping off the top of a ladder, as onto the roof beside Isaac's house.
  ANIM_LEADER_CLIMB_OVER_TOP = 11,
  ANIM_LEADER_CLING = 12, ///< still on a wall, seen from behind
  /// ANIM_LEADER_CLIMB_OVER_TOP played backwards: stepping from the top of a
  /// ladder onto its rungs.
  ANIM_LEADER_CLIMB_DOWN_FROM_TOP = 13,
  ANIM_LEADER_CLIMB_EAST = 14, ///< reaching along a wall as he climbs east
  ANIM_LEADER_CLIMB_WEST = 15, ///< reaching along a wall as he climbs west
  /// One leg forward, braced, held briefly: the frame ANIM_LEADER_LEAP holds
  /// and the run starts on, as when he lunges into a fight or pulls someone
  /// from a river.
  ANIM_LEADER_STRIDE = 16,
  /// Holding out a glowing hand, as the party's sprites cast Psynergy
  /// (spr_000.png, spr_001.png).
  ANIM_CAST_PSYNERGY = 17,
  /// Sitting waist-deep in water and splashing, as in a Lamakan Desert oasis.
  ANIM_LEADER_BATHE = 18,
  /// Lying collapsed on the ground; every party member's sheet maps this
  /// number to such a pose (spr_000.png to spr_003.png).
  ANIM_COLLAPSE = 19,
  ANIM_LEADER_LEAN_BACK = 22, ///< leaning back, his head thrown back
  /// The arrivals and leavings of the animated exits (enum ExitStyle), each
  /// four frames of the leader turning: an arrival grows from a small figure
  /// seen side-on to full size facing the player, a leaving turns from his
  /// back to side-on as he shrinks and is then hidden. The alternate pair has
  /// its own drawings of the same turns.
  ///@{
  ANIM_LEADER_ARRIVE_ALTERNATE = 23,
  ANIM_LEADER_ARRIVE = 24,
  ANIM_LEADER_LEAVE_ALTERNATE = 25,
  ANIM_LEADER_LEAVE = 26,
  ///@}
  /// Spinning round once, then drawn blank (ANIMCMD_BLANK).
  ANIM_LEADER_SPIN_AWAY = 27,
  ANIM_LEADER_ARMS_RAISED = 28, ///< both arms raised over his head
  /// Swinging an arm out to throw, as he tosses a coin or medal at Tolbi's
  /// spring.
  ANIM_LEADER_TOSS = 29,
  /// ANIM_LEADER_RUN again, on every party member's sheet: the shops run the
  /// chosen party member in place with it.
  ANIM_PARTY_RUN = 30,
  /// Held in ANIM_LEADER_SPRING's crouch, knees bent, as after a lost
  /// match.
  ANIM_LEADER_CROUCH = 35
};

#endif
