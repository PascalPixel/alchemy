#ifndef ALCHEMY_SPRITE_H
#define ALCHEMY_SPRITE_H

// Animated sprites: their layers, the sprite picture caches and the actor system's state.

#include "edition.h"
#include "fixed_math.h"
#include "types.h"
#include "static_assert.h"
#include "graphics/render_placement.h"

// SPR_ANIMATION.param: how fdRenderDecodePicture colours a layer's pixels. Modes 1 to 6 and 9 to 11
// run them through a 256-byte table (the five SpriteColorTables, then SpriteColorGreyTable and
// SpriteColorTintTables); the solid modes paint every drawn pixel one colour of the basic sprite
// palette. Mode 8 is a second white, and mode 12 (SPRITE_COLOR_DARK) reads past the tables.
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
    // Drawn dark: the light grey footprints Breath shows in the sand of the Shrine of the Sea God,
    // and the beacon's fire on Mars Lighthouse as it goes out.
    SPRITE_COLOR_DARK       = 12,
    SPRITE_COLOR_FILL       = 13, // solid DecodeFillByte
    SPRITE_COLOR_BLACK      = 14, // solid colour 1, black
    SPRITE_COLOR_HIDDEN     = 15  // solid colour 0, transparent
} SPR_COL_MODE;

// Passed to fdActorConfigureSpriteColor in place of a SPR_COL_MODE: the actor blinks white every
// few ticks (fdActorUpdateBlink) until it is given a mode again.
#define SPRITE_COLOR_BLINK          0x100

// SPR_ANIMATION.picture and pictureBase when no picture is shown.
#define SPRITE_PICTURE_NONE         255

// An animation number's bit 7: keep the place and timer of the script already running.
#define SPRITE_ANIMATION_KEEP_PLACE 0x80
#define SPRITE_ANIMATION_INDEX_MASK 0x7f

// A sprite layer's priority past the four drawn ones (SPR_ANIMATION's priority above 3): the layer
// is kept but not drawn.
#define SPRITE_LAYER_NOT_DRAWN      9

// One animation layer of a sprite: its animation id, pictures, running script and timer.
typedef struct SPR_ANIMATION
{
    Sint16  animationId; // the sprite id whose metadata and pictures it uses
    // Time left on the current picture in sixteenths of a tick, read signed: each timed command
    // adds its argument times 16, each tick takes off step.
    Uint16  timer;
    // The facing table its pictures follow (from the metadata, or animation command 240).
    Uint8   kind;
    Uint8   colorMode; // SPR_COL_MODE its pixels are drawn with
    Uint8   priority;  // drawing order 0 to 3 among the layers; above 3 not drawn
    Uint8   mode;      // picture codec: 1 type-01, 3 LZ, anything else raw
    // picture pointers, from the metadata or a picture cache
    void  **pictures;
    // the metadata's command scripts, one per animation number
    Uint8 **scripts;
    Uint8  *script;       // the running script; null once command 239 ends it
    Uint8   scriptOffset; // byte offset of the next command in script
    Uint8   step;         // taken off timer each tick; ANIM_STEP_NORMAL
    Uint8
        // picture shown, pictureBase plus the facing offset; 255 none
          picture;
    Uint8 pictureBase; // picture the script chose, before facing; 255 blank
} SPR_ANIMATION;

// A sprite picture cache: the tag of the animation loaded into it (slot number and animation id)
// and the buffer holding its pictures.
typedef struct SPR_PICTURE_CACHE
{
    Sint32 tag;      // SPRITE_PICTURE_TAG(slot, animation id)
    void  *pictures; // the decoded pictures, their offsets made pointers
} SPR_PICTURE_CACHE;

// A sprite id with the sprite picture cache its pictures load into, as a SPR_PICTURE_CACHE's tag
// holds them.
#define SPRITE_PICTURE_TAG(slot, id) ((id) + ((slot) << 12))

// The actor system's state: the flag that suspends sprite projection, the outline's edge and fill
// colours, and the sprite picture caches (HEAP_SLOT_ACTOR_SYSTEM).
typedef struct ACTOR_SYSTEM_STATE
{
    Uint16 used;               // sprite pixel bytes composed since the camera reset it
    Uint8  unusedAfterUsed[2]; // no code names it
    Uint16 suspended;          // nonzero while a menu has suspended sprite projection
    Uint8  edge;               // outline edge colour; 15 when the field sets up
    Uint8  fill;               // outline fill colour; 0 when the field sets up
    Uint8  spriteGroups[16];   // the sprite group tables, taken in turn by count
    Sint32 count;              // sprite group tables taken from spriteGroups
    SPR_PICTURE_CACHE
    pictureCaches[8];          // found by tag (fdSpritePictureCacheFindByTag)
} ACTOR_SYSTEM_STATE;

STATIC_ASSERT(offsetof(ACTOR_SYSTEM_STATE, suspended) == 4);
STATIC_ASSERT(offsetof(ACTOR_SYSTEM_STATE, count) == 24);
STATIC_ASSERT(offsetof(ACTOR_SYSTEM_STATE, pictureCaches) == 28);
STATIC_ASSERT(sizeof(ACTOR_SYSTEM_STATE) == 92);

// The hardware sprite attributes one part of an animated sprite (its image or its shadow) is drawn
// with: the link to the next part queued at its depth, then the OAM entry's attributes 0 to 3.
typedef struct ANIMATION_SPR_PART
{
    Uint8  unused[4]; // no code names it
    Uint16 y           : 8;
    Uint16 affine      : 2;
    Uint16 objectMode  : 2;
    Uint16 mosaic      : 1;
    Uint16 fullColor   : 1;
    Uint16 shape       : 2;
    Uint16 x           : 9;
    Uint16 affineIndex : 5;
    Uint16 size        : 2;
    Uint16 tile        : 10;
    Uint16 priority    : 2;
    Uint16 palette     : 4;
    Uint8  unusedAfterPalette[2]; // no code names it
} ANIMATION_SPR_PART;

// SPR.flags.
typedef enum SPR_FLAG
{
    SPRITE_FLAG_SHADOW       = 1 << 0, // its shadow is drawn; set when created
    // Drawn as an outline, in the actor system's edge and fill colours.
    SPRITE_FLAG_OUTLINED     = 1 << 1,
    // Its bottom sunkRows rows drawn in shade, as the leader wading (fdActorUpdatePlayerControl).
    SPRITE_FLAG_SHADED       = 1 << 2,
    // Drawn sunkRows rows lower, its bottom cut off, as sinking in Taopo Swamp's mud; it takes the
    // place of SPRITE_FLAG_SHADED.
    SPRITE_FLAG_SUNK         = 1 << 3,
    // Keeps the picture last drawn and only turns to mirror it: set once a battle's motion releases
    // the buffer its layer drew from (btMotionReleaseBuffer).
    SPRITE_FLAG_KEEP_PICTURE = 1 << 5
} SPR_FLAG;

// The VRAM cache entry of the actors' shared shadow (ActorShadowTiles), each sprite's part 1.
#define VRAM_CACHE_ENTRY_ACTOR_SHADOW 93

// How many the actor system's pools hold: its sprites (HEAP_SLOT_SPRITES) and its sprite animations
// (HEAP_SLOT_SPRITE_ANIMATIONS).
#define ANIM_OBJECT_COUNT             64

// An animated sprite: its image, scale, rotation, size, its shadow and up to four sprite
// animations.
typedef struct SPR
{
    ANIMATION_SPR_PART image;
    FIXED              scale; // 16.16 fixed point, toFIXED(1) full size
    Uint8              slot;  // VRAM cache entry holding its tiles
    // Keeps the VRAM cache entry when it is released or off screen.
    Uint8              retainCacheEntry : 1;
    // Drawn at its own scale, not scaled by distance (fixed camera).
    Uint8              useOwnScale      : 1;
    // rows SPRITE_FLAG_SUNK sinks or SPRITE_FLAG_SHADED * shades
    Uint8              sunkRows         : 6;
    Uint16             rotation; // 1/65536 turns
    Uint8              width;    // pixels; zero marks a free sprite
    Uint8              height;   // pixels
    Sint8              offsetX;  // pixels
    Sint8              offsetY;  // pixels
    // The animation number last selected, which selecting again skips.
    Uint8              lastNo;
    Uint8              dirty; // nonzero redraws its pixels into VRAM when next drawn
    Uint8              flags; // SPR_FLAG
    Uint8              count; // layers in use, up to the last one present
    // Its shadow, shown while SPRITE_FLAG_SHADOW is set.
    ANIMATION_SPR_PART shadow;
    SPR_ANIMATION     *entries[4]; // its layers; empty ones null
} SPR;

STATIC_ASSERT(sizeof(SPR_ANIMATION) == 24);
STATIC_ASSERT(sizeof(ANIMATION_SPR_PART) == 12);
STATIC_ASSERT(offsetof(SPR, scale) == 12);
STATIC_ASSERT(offsetof(SPR, rotation) == 18);
STATIC_ASSERT(offsetof(SPR, width) == 20);
STATIC_ASSERT(offsetof(SPR, shadow) == 28);
STATIC_ASSERT(sizeof(SPR) == 56);
STATIC_ASSERT(offsetof(SPR, entries) == 40);

// A sprite's shadow, 16 by 8 pixels: its half width and half height, both doubled when the sprite
// is drawn double-size.
#define SHADOW_HALF_WIDTH  8
#define SHADOW_HALF_HEIGHT 4

// An actor's sprite (SPR) as the code that sets its image and shadow priorities reads it (the
// actors' freeze and placement, an actor taking the leader's layer): a byte at a time, under SPR's
// names.
typedef struct PLACED_SPR
{
    Uint8  unused[9];              // no code names it
    Uint8  tileHigh : 2;           // tile number bits 9..8
    Uint8  priority : 2;
    Uint8  palette  : 4;
    Uint8  unusedAfterPalette[2];  // no code names it
    FIXED  scale;                  // 16.16 fixed point
    Uint8  slot;
    Uint8  retainCacheEntry;       // bit 0
    Uint16 rotation;
    Uint8  unusedAfterRotation[5]; // no code names it
    Uint8  dirty;
    Uint8  flags;
    Uint8  unusedAfterFlags[10];   // no code names it
    Uint8  shadowTileHigh : 2;
    Uint8  shadowPriority : 2;
    Uint8  shadowPalette  : 4;
} PLACED_SPR;

// SPR_ANIMATION.step at normal speed, as fdActorSetAnimationSpeed sets it: a timed command's ticks
// run as written.
#define ANIM_STEP_NORMAL 16

// Animation script commands.
typedef enum ANIMATION_COMMAND
{
    ANIMCMD_END          = 239, // blank the layer and stop its script
    ANIMCMD_SET_KIND     = 240, // use facing table `arg`
    ANIMCMD_HOLD         = 241, // stay on this command and tick for good
    ANIMCMD_OFFSET_Y     = 242, // set the sprite's offsetY to `arg`
    ANIMCMD_OFFSET_X     = 243, // set the sprite's offsetX to `arg`
    // The commands from here to ANIMCMD_UNUSED_LAST no script uses, skipped (ANIMCMD_WAIT lies
    // among them); 251 is not one: it shows as a picture.
    ANIMCMD_UNUSED_FIRST = ANIMCMD_OFFSET_X + 1,
    ANIMCMD_WAIT         = 245, // keep the current tick `arg` ticks longer
    ANIMCMD_UNUSED_LAST  = 250,
    ANIMCMD_SOUND        = 252, // play sound `arg`
    ANIMCMD_JUMP         = 253, // continue at script position `arg`
    ANIMCMD_PLAY         = 254, // switch to animation `arg`
    ANIMCMD_BLANK        = 255  // show nothing for `arg`
} ANIMATION_COMMAND;

// Animation numbers of a person's sprite, for fdActorSetAnimation. A sprite that is not a person (a
// bird, a dragon, a pillar) numbers its own.
typedef enum ACTOR_ANIMATION
{
    // Standing still: the sprite's first frame held, without the blinks and small gestures of
    // ANIM_STAND.
    ANIM_STILL                      = 0,
    ANIM_STAND                      = 1,
    ANIM_WALK                       = 2,
    ANIM_NOD                        = 3,
    ANIM_SHAKE_HEAD                 = 4,
    // Running, leaning forward, four ticks a step to the walk's six. This and the leader's other
    // poses below are as the leader's sprite draws them.
    ANIM_LEADER_RUN                 = 5,
    ANIM_LEADER_SPRING              = 6, // leaning back for a moment before a jump
    ANIM_LEADER_LEAP                = 7, // in mid-stride, leaning forward
                          // The leader leaning into what it pushes, as when it walks into a
                          // pushable actor (fdActorUpdatePlayerControl).
    ANIM_PUSH                       = 8,
    // ANIM_STAND's idle with more glances to each side, as when the leader's step is blocked.
    ANIM_LEADER_LOOK_AROUND         = 9,
    ANIM_LEADER_CLIMB               = 10, // climbing a wall, seen from behind
                                          // Stepping off the top of a ladder onto the ground above.
    ANIM_LEADER_CLIMB_OVER_TOP      = 11,
    ANIM_LEADER_CLING               = 12, // still on a wall, seen from behind
                            // ANIM_LEADER_CLIMB_OVER_TOP played backwards: stepping from the top of
                            // a ladder onto its rungs.
    ANIM_LEADER_CLIMB_DOWN_FROM_TOP = 13,
    ANIM_LEADER_CLIMB_EAST          = 14, // reaching along a wall as he climbs east
    ANIM_LEADER_CLIMB_WEST          = 15, // reaching along a wall as he climbs west
                                 // Leaping from a hanging vine, stretched out level in the air.
    ANIM_LEADER_SWING               = 16,
    // Holding out a glowing hand, as the party's sprites cast Psynergy.
    ANIM_CAST_PSYNERGY              = 17,
    // Tumbling head over heels in a burst of spray, as a current sweeps the leader along.
    ANIM_LEADER_TUMBLE_IN_WATER     = 18,
    // Lying collapsed on the ground.
    ANIM_COLLAPSE                   = 19,
    // ANIM_CAST_PSYNERGY's first two pictures, held on the second, and its last two, held on the
    // last with the hand held out: the caster's start and end of a field Psynergy.
    ANIM_CAST_PSYNERGY_START        = 20,
    ANIM_CAST_PSYNERGY_END          = 21,
    ANIM_LEADER_LEAN_BACK           = 22, // leaning back, his head thrown back
                                // The arrivals and leavings of the animated exits, each four ticks
                                // of the leader turning: an arrival grows from a small figure seen
                                // side-on to full size facing the player, a leaving turns from his
                                // back to side-on as he shrinks and is then hidden.
    ANIM_LEADER_ARRIVE_ALTERNATE    = 23,
    ANIM_LEADER_ARRIVE              = 24,
    ANIM_LEADER_LEAVE_ALTERNATE     = 25,
    ANIM_LEADER_LEAVE               = 26,
    // ANIM_WALK's six pictures in reverse order.
    ANIM_LEADER_WALK_BACKWARDS      = 27,
    ANIM_LEADER_ARMS_RAISED         = 28, // both arms raised over his head
                                          // Hanging from a rope by his hands as he crosses it.
    ANIM_LEADER_CROSS_ROPE          = 29,
    // ANIM_LEADER_RUN again, on every party member's sprite sheet.
    ANIM_PARTY_RUN                  = 30,
    // Hanging from a vine, seen from behind with an arm raised.
    ANIM_LEADER_HANG                = 32,
    // Slumping and falling forward onto his face, then lying there: the leader as the whole party
    // falls.
    ANIM_LEADER_FALL_ON_FACE        = 36,
    // ANIM_STILL's picture held again on the leader's sprite sheet: the leader standing as he lands
    // on a statue that rose under him.
    ANIM_LEADER_STILL               = 38,
    // Swinging an arm out to throw something.
    ANIM_LEADER_TOSS                = 39,
    // Seen from behind, arms flung up and waving: the leader carried over a waterfall.
    ANIM_LEADER_FLAIL               = 40,
    // Lying on his back, seen from his feet: the leader washed up or put to bed.
    ANIM_LEADER_LIE_ON_BACK         = 41,
    ANIM_LEADER_SIT_UP              = 42, // sitting up, legs out in front of him
                             // Swinging each arm and kicking out each leg, as Felix checks himself
                             // for injuries on Idejima's beach.
    ANIM_LEADER_SWING_RIGHT_ARM     = 45,
    ANIM_LEADER_SWING_LEFT_ARM      = 46,
    ANIM_LEADER_KICK_RIGHT_LEG      = 47,
    ANIM_LEADER_KICK_LEFT_LEG       = 48,
    // Sitting on the floor, legs out, arms flung up and then lowered: the leader knocked down.
    ANIM_LEADER_SIT                 = 49
} ACTOR_ANIMATION;

// A Djinni's lively animation on the four Djinn sprite sheets, facing the player: Venus bounces,
// Mars, Jupiter and Mercury wave their arms. It plays as the Djinni shrinks away on joining.
#define DJINNI_ANIMATION_EXCITED 3

// SPRITE_PARTICLE's animations.
typedef enum PARTICLE_ANIMATION
{
    ANIM_PARTICLE_FADE        = 2, // its glow shrinking to a speck and gone
    // Its full glow and the next size down in turn, 3 ticks each.
    ANIM_PARTICLE_TWINKLE     = 7,
    // Its swells, from a speck to its full glow, held 32 ticks, and back: slowly, 12 ticks a step,
    // or quickly, 6. Jupiter Lighthouse's sparks and the motes of light inside Air's Rock take
    // turns at them.
    ANIM_PARTICLE_SLOW_SWELL  = 9,
    ANIM_PARTICLE_QUICK_SWELL = 10
} PARTICLE_ANIMATION;

#endif
