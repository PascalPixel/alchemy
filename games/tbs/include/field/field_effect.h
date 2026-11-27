#ifndef ALCHEMY_FIELD_EFFECT_H
#define ALCHEMY_FIELD_EFFECT_H

// Field effects, their scripts and the flags that spawn them.

#include "edition.h"
#include "fixed_math.h"
#include "field/actor_script.h"
#include "field/script.h"
#include "system.h"

// A field effect: a small actor on the field that moves at its velocity and grows and spins at its
// rates, run by its script and update function.
typedef struct FIELD_EFFECT
{
    // Actor's script pointer, cursor and facing; not named by this view.
    const Sint32 *script;
    Uint16        scriptCursor;
    Uint16        facing;
    FIXED         x;            // 16.16 fixed-point pixels
    FIXED         y;            // 16.16 fixed-point pixels
    FIXED         z;            // 16.16 fixed-point pixels
    Sint32        groundHeight; // Actor's; not named by this view
    FIXED         scaleX;       // 16.16 fixed point, toFIXED(1) full size
    FIXED         scaleY;       // 16.16 fixed point, toFIXED(1) full size
    Uint16        radius;       // Actor's; not named by this view
    Uint8         layer;        // Actor's; not named by this view
    // ACTOR_PRIORITY_FLAG bits: the spawner clears ACTOR_PRIORITY_AUTOMATIC when
    // EFFECT_USE_PRIORITY sets the sprite's priority.
    Uint8         priorityFlags;
    // Actor's velocityX to velocityZ; effects move by velocity instead.
    VECTOR3       objectVelocity;
    // Added to scaleX each tick by the update functions, in 16.16 fixed point a tick; zeroed by the
    // spawner.
    FIXED         scaleRateX;
    Sint32        scaleRateY;
    // Actor's targetX to targetZ; not named by this view.
    Sint32        targetX;
    Sint32        targetY;
    Sint32        targetZ;
    // Added to x, y and z each tick by the update functions, in 16.16 fixed-point pixels per tick;
    // Actor's bounce, gravity and sceneWord.
    VECTOR3       velocity;
    SPR          *sprite;            // the sprite spin turns
    Uint8         spriteStorageKind; // Actor's; not named by this view
    Uint8         motionFlags;       // ACTOR_MOTION_FLAG bits; zeroed by the spawner
    // Actor's arrival axis, last flag test and snap flag; not named by this view.
    Uint8         arrivalAxis;
    Uint8         conditionResult;
    Uint8         snapToTarget;
    Uint8         collisionFlags; // Actor's collisionFlags; not named by this view
    // Actor's steeringFlags to sceneByte; not named by this view.
    Uint8         steeringFlags;
    Uint8         paused;
    Uint8         retainSpriteCache;
    Uint8         repeatCount;
    Uint16        wait;
    Uint8         refused;
    Uint8         frozen;
    Uint8         sceneCount;
    Uint8         sceneByte;
    // Added to the sprite's rotation each tick by the update functions, in 1/65536 turns per tick;
    // Imil's rising effects keep a drift here instead.
    Uint16        spin;
    Sint16        userValue; // Actor's; not named by this view
    // Ticks until a scene's update function releases the effect: 20 for Altin Peak's spray, 60 for
    // Imil's rising effects.
    Sint32        countdown;
    // Called each tick while the effect is live: the spawner sets
    // scFieldEffectUpdateMotionScaleAndSpin, or options.update.
    void          (*update)(ACTOR *actor);
} FIELD_EFFECT;

STATIC_ASSERT(offsetof(FIELD_EFFECT, scaleRateX) == 48);
STATIC_ASSERT(offsetof(FIELD_EFFECT, velocity) == 68);
STATIC_ASSERT(offsetof(FIELD_EFFECT, sprite) == 80);
STATIC_ASSERT(offsetof(FIELD_EFFECT, spin) == 100);
STATIC_ASSERT(offsetof(FIELD_EFFECT, countdown) == 104);
STATIC_ASSERT(offsetof(FIELD_EFFECT, update) == 108);

// An actor read as an actor and as a field effect at once, as Babi Lighthouse's scInit writes one.
typedef union ACTOR_OR_EFFECT
{
    ACTOR        actor;
    FIELD_EFFECT effect;
} ACTOR_OR_EFFECT;

// The flags of a scene's effect spawner (EFFECT_SPAWN in scene/shared/field_effect_spawn.c): a
// script number in the low bits and which of EFFECT_OPTIONS' fields to use.
typedef enum EFFECT_SPAWN_FLAGS
{
    // An EFFECT_DURATION, picking one of FieldEffectDurationScripts; the actor's mode is this
    // plus 1.
    EFFECT_SCRIPT_MASK     = 0x0f,
    EFFECT_USE_PALETTE     = 0x10000, // options.palette, as its group child value
    EFFECT_USE_PRIORITY    = 0x20000, // options.priority, as the sprite's priority
                                   // Grows from the start scale, or full size, to the target scale
                                   // over the script's repeat count.
    EFFECT_SCALE_TO_TARGET = 0x40000,
    EFFECT_USE_START_SCALE = 0x80000,  // options.startScaleX and startScaleY
    EFFECT_USE_TYPE        = 0x100000, // options.type, else EFFECT_DEFAULT_TYPE
    EFFECT_USE_SCRIPT      = 0x200000, // options.script, run in mode 1
    EFFECT_USE_ROTATION    = 0x400000, // options.rotation, as the sprite's
    EFFECT_USE_SPIN        = 0x800000, // options.spin
    EFFECT_USE_UPDATE      = 0x1000000 // options.update
} EFFECT_SPAWN_FLAGS;

// The spawn flags that pick one of EFFECT_OPTIONS' fields (EFFECT_USE_PALETTE and up), above the
// script number.
#define EFFECT_OPTION_MASK  0xffff0000
// The sprite an effect takes unless EFFECT_USE_TYPE gives another (sprite_ids.h).
#define EFFECT_DEFAULT_TYPE SPRITE_DUST_PUFF

// The scripts EFFECT_SCRIPT_MASK picks from FieldEffectDurationScripts, in a scene's spawner and
// the resident one alike: each holds the effect for a while, then releases it.
typedef enum EFFECT_DURATION
{
    // 22 one-tick repeats in a scene, 24 ticks resident
    EFFECT_SCRIPT_SHORT  = 0,
    EFFECT_SCRIPT_MEDIUM = 1, // 44 repeats in a scene, 46 ticks resident
    EFFECT_SCRIPT_LONG   = 2  // 126 repeats in a scene, 128 ticks resident
} EFFECT_DURATION;

// The word of a duration script that holds its repeat count, the ticks EFFECT_SCALE_TO_TARGET grows
// over.
#define EFFECT_SCRIPT_REPEAT_COUNT 3

// The resident scripts, by EFFECT_DURATION (field_mode_tables.c).
extern const Sint32 FieldEffectShortEffectScript[];
extern const Sint32 FieldEffectMediumEffectScript[];
extern const Sint32 FieldEffectLongEffectScript[];

// The three scripts a scene's own effect spawner picks from by EFFECT_SCRIPT_MASK, wrapped so that
// the spawner can copy them into its tick whole.
typedef struct EFFECT_DURATION_SCRIPTS
{
    const Sint32 *script[3];
} EFFECT_DURATION_SCRIPTS;

// The options of a spawned field effect, each used when its EFFECT_USE_ flag (or
// EFFECT_SCALE_TO_TARGET) is set.
typedef struct EFFECT_OPTIONS
{
    Sint32        priority;                       // the sprite's priority
    Sint32        palette;                        // passed to fdActorSetSpriteColor
    FIXED         startScaleX;                    // 16.16 fixed point, toFIXED(1) full size
    FIXED         startScaleY;                    // 16.16 fixed point, toFIXED(1) full size
    FIXED         targetScaleX;                   // 16.16 fixed point, toFIXED(1) full size
    FIXED         targetScaleY;                   // 16.16 fixed point, toFIXED(1) full size
    Sint16        type;                           // the sprite, SPRITE_DUST_PUFF without the flag
    const Sint32 *script;                         // the actor script, run in mode 1
    Uint16        rotation;                       // 1/65536 turns
    Uint16        spin;                           // 1/65536 turns per tick
    void          (*update)(struct ACTOR *actor); // replaces the update function
} EFFECT_OPTIONS;

STATIC_ASSERT(offsetof(EFFECT_OPTIONS, type) == 24);
STATIC_ASSERT(offsetof(EFFECT_OPTIONS, update) == 36);

void fdEffectSpawnResident(Sint32                x,
                           Sint32                y,
                           Sint32                z,
                           Sint32                velocityX,
                           Sint32                velocityY,
                           Sint32                velocityZ,
                           Uint32                flags,
                           const EFFECT_OPTIONS *options);

#endif
