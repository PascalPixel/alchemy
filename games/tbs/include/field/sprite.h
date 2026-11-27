#ifndef ALCHEMY_SPRITE_H
#define ALCHEMY_SPRITE_H

// Animated sprites: their layers, pictures and colour modes, and the resource slots they draw from.

#include "edition.h"
#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"
#include "graphics/render_placement.h"

// SPR_ANIMATION.colorMode: how fdRenderDecodePicture colours a layer's pixels.
typedef enum SPR_COL_MODE
{
	SPRITE_COLOR_NORMAL     = 0,
	SPRITE_COLOR_GRB        = 1,  // red and green swapped
	SPRITE_COLOR_RBG        = 2,  // green and blue swapped
	SPRITE_COLOR_BGR        = 3,  // red and blue swapped
	SPRITE_COLOR_GBR        = 4,  // red to blue, green to red, blue to green
	SPRITE_COLOR_BRG        = 5,  // red to green, green to blue, blue to red
	SPRITE_COLOR_GREY       = 6,  // greyscale
	SPRITE_COLOR_WHITE      = 7,  // solid colour 15, white
	SPRITE_COLOR_BLUE_TINT  = 9,  // blue raised to full
	SPRITE_COLOR_RED_TINT   = 10, // red raised to full
	SPRITE_COLOR_GREEN_TINT = 11, // green raised to full
	SPRITE_COLOR_FILL       = 13, // solid DecodeFillByte
	SPRITE_COLOR_BLACK      = 14, // solid colour 1, black
	SPRITE_COLOR_HIDDEN     = 15  // solid colour 0, transparent
} SPR_COL_MODE;

// Passed to fdActorConfigureSpriteColor in place of a SPR_COL_MODE: the actor blinks white every
// few ticks (fdActorUpdateBlink) until it is given a mode again.
#define SPRITE_COLOR_BLINK          0x100

// A sprite animation's picture or pictureBase when it shows none: fdSpriteComposePicture skips it
// until an animation is selected.
#define SPRITE_PICTURE_NONE         255

// An animation number's bit 7: keep the place and timer of the script already running.
#define SPRITE_ANIMATION_KEEP_PLACE 0x80
#define SPRITE_ANIMATION_INDEX_MASK 0x7f

// One of a sprite's four sprite animations: its animation id, pictures, running script and timer.
typedef struct SPR_ANIMATION
{
	Sint16  animationId; // the sprite id whose metadata and pictures it uses
	// Time left on the current picture in sixteenths of a tick, read signed: each timed command
	// adds its argument times 16, each tick takes off step.
	Uint16  timer;
	// The facing table its pictures follow (from the metadata, or ANIMCMD_SET_KIND); zero marks a
	// free sprite animation.
	Uint8   kind;
	Uint8   colorMode; // SPR_COL_MODE its pixels are drawn with; 0 when taken
	// drawing order 0 to 3 among the animations; above 3 not drawn
	Uint8   priority;
	Uint8   mode; // an SPR_PICTURE_CODEC; anything else is raw
	// picture pointers, from the metadata or a picture cache
	void  **pictures;
	// the metadata's command scripts, one per animation number
	Uint8 **scripts;
	Uint8  *script;       // the running script; null when stopped (ANIMCMD_END)
	Uint8   scriptOffset; // byte offset of the next command in script
	// taken off timer each tick; ANIM_STEP_NORMAL is normal speed
	Uint8   step;
	// picture shown, pictureBase plus the facing offset; SPRITE_PICTURE_NONE for none
	Uint8   picture;
	// picture the script chose, before facing; SPRITE_PICTURE_NONE for none
	Uint8   pictureBase;
} SPR_ANIMATION;

// SPR_ANIMATION.step at normal speed, as fdActorSetAnimationSpeed sets it: a timed command's ticks
// run as written.
#define ANIM_STEP_NORMAL 16

// A sprite picture cache: the tag of the animation loaded into it (slot number and animation id)
// and the buffer holding its pictures.
typedef struct SPR_PICTURE_CACHE
{
	Sint32 tag;      // SPRITE_PICTURE_TAG(slot, animation id)
	void  *pictures; // the decoded pictures, their offsets made pointers
} SPR_PICTURE_CACHE;

// A picture cache's tag holds its slot above the sprite id's bits.
#define PICTURE_CACHE_TAG_SLOT_SHIFT 12
// A sprite id with the sprite picture cache its pictures load into, as a SPR_PICTURE_CACHE's tag
// holds them.
#define SPRITE_PICTURE_TAG(slot, id) ((id) + ((slot) << PICTURE_CACHE_TAG_SLOT_SHIFT))

// The actor system's state: the flag that suspends sprite projection, the outline's edge and fill
// colours, and the sprite picture caches (HEAP_SLOT_ACTOR_SYSTEM).
typedef struct ACTOR_SYSTEM_STATE
{
	Uint16      used;      // sprite pixel bytes composed since the camera reset it
	Uint8       unused[2];
	Sint16      suspended; // nonzero while a menu has suspended sprite projection
	Uint8       edge;      // outline edge colour; 15 when the field engine sets up
	Uint8       fill;      // outline fill colour; 0 when the field engine sets up
	// The sprite group tables, taken in turn by count.
	struct SPR *spriteGroups[4];
	Sint32      count; // sprite group tables taken from spriteGroups
	SPR_PICTURE_CACHE
	pictureCaches[8];  // found by tag (fdSpritePictureCacheFindByTag)
} ACTOR_SYSTEM_STATE;

STATIC_ASSERT(offsetof(ACTOR_SYSTEM_STATE, suspended) == 4);
STATIC_ASSERT(offsetof(ACTOR_SYSTEM_STATE, count) == 24);
STATIC_ASSERT(offsetof(ACTOR_SYSTEM_STATE, pictureCaches) == 28);
STATIC_ASSERT(sizeof(ACTOR_SYSTEM_STATE) == 92);

// The hardware sprite attributes one part of an animated sprite (its image or its shadow) is drawn
// with.
typedef struct ANIMATION_SPR_PART
{
	// The link to the next part queued in the same depth slot this tick (syOAMAddSpriteAtDepth).
	struct SPR *link;
	// The OAM entry's attributes 0 to 2.
	Uint16      y           : 8;  // screen pixels
	Uint16      affine      : 2;  // 0 plain, 1 affine, 3 affine at double size
	Uint16      objectMode  : 2;  // an SPR_MODE
	Uint16      mosaic      : 1;
	Uint16      fullColor   : 1;  // 256 colours; set for both parts when created
	Uint16      shape       : 2;  // 0 square, 1 wide, 2 tall
	Uint16      x           : 9;  // screen pixels
	Uint16      affineIndex : 5;  // affine matrix, or 8 for a mirrored plain sprite
	Uint16      size        : 2;  // with shape, the sprite's size
	Uint16      tile        : 10; // first sprite tile in VRAM
	Uint16      priority    : 2;  // 2 when created
	Uint16      palette     : 4;  // 16-colour palette; unused at 256 colours
	// the OAM entry's fourth halfword, an affine parameter's place; zero when created, not named
	Uint8       attr3[2];
} ANIMATION_SPR_PART;

// An animated sprite: its image and shadow, scale, rotation, size and up to four sprite animations.
typedef struct SPR
{
	ANIMATION_SPR_PART image;
	// Its shadow, shown while flags has SPRITE_FLAG_SHADOW.
	ANIMATION_SPR_PART shadow;
	FIXED              scale; // 16.16 fixed point, toFIXED(1) full size
	Uint8              slot;  // VRAM cache entry holding its tiles
	// Keeps the VRAM cache entry when it is released or off screen.
	Uint8              retainCacheEntry : 1;
	// Drawn at its own scale, not scaled by distance (fixed camera).
	Uint8              useOwnScale      : 1;
	Uint8              unusedFlags      : 6; // no code names them
	Uint16             rotation;             // 1/65536 turns
	Uint8              width;                // pixels; zero marks a free sprite
	Uint8              height;               // pixels
	Sint8              offsetX;              // pixels
	Sint8              offsetY;              // pixels
	// The animation number last selected, which selecting again skips.
	Uint8              lastNo;
	Uint8              dirty; // nonzero redraws its pixels into VRAM when next drawn
	// SPR_FLAG bits: SPRITE_FLAG_OUTLINED and SPRITE_FLAG_SHADOW (set when created).
	Uint8              flags;
	Uint8              count;      // layers in use, up to the last one present
	SPR_ANIMATION     *entries[4]; // its layers; empty ones null
} SPR;

STATIC_ASSERT(sizeof(SPR_ANIMATION) == 24);
STATIC_ASSERT(sizeof(ANIMATION_SPR_PART) == 12);
STATIC_ASSERT(offsetof(SPR, scale) == 24);
STATIC_ASSERT(offsetof(SPR, rotation) == 30);
STATIC_ASSERT(offsetof(SPR, width) == 32);
STATIC_ASSERT(sizeof(SPR) == 56);
STATIC_ASSERT(offsetof(SPR, entries) == 40);

// Animation numbers of an actor's sprite, for syImportActorSetAnimation and fdSpriteStartAnimation.
typedef enum ACTOR_ANIMATION
{
	// Standing still: nearly every sprite's first frame held, without the blinks and small gestures
	// of ANIM_STAND.
	ANIM_STILL                      = 0,
	ANIM_STAND                      = 1,
	ANIM_WALK                       = 2,
	ANIM_NOD                        = 3,
	ANIM_SHAKE_HEAD                 = 4,
	// Running, leaning forward, four ticks a step to the walk's six. This and the leader's other
	// poses below are as Isaac's sprite (isaac.png) draws them.
	ANIM_LEADER_RUN                 = 5,
	ANIM_LEADER_SPRING              = 6, // leaning back for a moment before a jump
	ANIM_LEADER_LEAP                = 7, // in mid-stride, leaning forward
	                      // The leader leaning into what it pushes, as scenes play it while a
	                      // statue or stone pillar slides ahead of it.
	ANIM_PUSH                       = 8,
	// ANIM_STAND's idle with more glances to each side, as when a whirlwind catches the leader.
	ANIM_LEADER_LOOK_AROUND         = 9,
	ANIM_LEADER_CLIMB               = 10, // climbing a wall, seen from behind
	                        // Stepping off the top of a ladder, as onto the roof beside Isaac's
	                        // house.
	ANIM_LEADER_CLIMB_OVER_TOP      = 11,
	ANIM_LEADER_CLING               = 12, // still on a wall, seen from behind
	                        // ANIM_LEADER_CLIMB_OVER_TOP played backwards: stepping from the top of
	                        // a ladder onto its rungs.
	ANIM_LEADER_CLIMB_DOWN_FROM_TOP = 13,
	ANIM_LEADER_CLIMB_EAST          = 14, // reaching along a wall as he climbs east
	ANIM_LEADER_CLIMB_WEST          = 15, // reaching along a wall as he climbs west
	                             // One leg forward, braced, held briefly: the tick ANIM_LEADER_LEAP
	                             // holds and the run starts on, as when he lunges into a fight or
	                             // pulls someone from a river.
	ANIM_LEADER_STRIDE              = 16,
	// Holding out a glowing hand, as the party's sprites cast Psynergy (isaac.png, garet.png).
	ANIM_CAST_PSYNERGY              = 17,
	// Sitting waist-deep in water and splashing, as in a Lamakan Desert oasis.
	ANIM_LEADER_BATHE               = 18,
	// Lying collapsed on the ground; every party member's sprite sheet maps this number to such a
	// pose (isaac.png to mia.png).
	ANIM_COLLAPSE                   = 19,
	ANIM_LEADER_LEAN_BACK =
		22, // leaning back, his head thrown back
	        // The arrivals and leavings of the animated exits (EXIT_STYLE), each four
	        // ticks of the leader turning: an arrival grows from a small figure seen
	        // side-on to full size facing the player, a leaving turns from his back to
	        // side-on as he shrinks and is then hidden.
	ANIM_LEADER_ARRIVE_ALTERNATE = 23,
	ANIM_LEADER_ARRIVE           = 24,
	ANIM_LEADER_LEAVE_ALTERNATE  = 25,
	ANIM_LEADER_LEAVE            = 26,
	// ANIM_WALK's six pictures in reverse order, three ticks each, played as a log carries the
	// leader across water or a torn leaf drops him. XXX (no evidence): what the player sees of it;
	// render script reaches neither a torn leaf's fall (the second touch of one leaf) nor a ride on
	// Crossbone Isle's floating logs.
	ANIM_LEADER_WALK_BACKWARDS   = 27,
	ANIM_LEADER_ARMS_RAISED      = 28, // both arms raised over his head
	                              // Swinging an arm out to throw, as he tosses a coin or medal at
	                              // Tolbi's spring.
	ANIM_LEADER_TOSS             = 29,
	// ANIM_LEADER_RUN again, on every party member's sprite sheet: the shops run the chosen party
	// member in place with it.
	ANIM_PARTY_RUN               = 30,
	// Held in ANIM_LEADER_SPRING's crouch, knees bent, as after a lost match.
	ANIM_LEADER_CROUCH           = 35,
	// Staggering, reeling back, then lying flat (isaac.png), as he collapses at the end of a
	// Colosso match.
	ANIM_LEADER_FALL             = 36
} ACTOR_ANIMATION;

#endif
