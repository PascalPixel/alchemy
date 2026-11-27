#ifndef ALCHEMY_FIELD_EFFECT_H
#define ALCHEMY_FIELD_EFFECT_H

// Field effects, the small objects on the field that move, grow and spin by their scripts, and the
// flags of a scene's effect spawner.

#include "edition.h"
#include "field/field_script.h"
#include "fixed_math.h"
#include "vector3.h"
#include "types.h"

// A field effect: a small object on the field that moves at its velocity and grows and spins at its
// rates, run by its script and update hook. It lies over ACTOR; a field marked "Actor's" is the
// actor's field of that place and name, which no code reads through this struct.
typedef struct FIELD_EFFECT
{
	// Actor's script pointer, cursor and facing.
	const Sint32 *script;
	Sint16        scriptCursor;
	Uint16        facing;
	FIXED         x;            // 16.16 fixed-point pixels
	FIXED         y;            // 16.16 fixed-point pixels
	FIXED         z;            // 16.16 fixed-point pixels
	Sint32        groundHeight; // Actor's
	FIXED         scaleX;       // 16.16 fixed point, toFIXED(1) full size
	FIXED         scaleY;       // 16.16 fixed point, toFIXED(1) full size
	Uint16        radius;       // Actor's
	Uint8         layer;        // Actor's
	// The spawner clears bit 0 when EFFECT_USE_PRIORITY sets the sprite's priority.
	Uint8         priorityFlags;
	// Actor's velocityX to velocityZ; effects move by velocity instead.
	VECTOR3       objectVelocity;
	// Added to scaleX each tick by the update hooks, in 16.16 fixed point a tick; zeroed by the
	// spawner.
	FIXED         scaleRateX;
	Sint32        scaleRateY; // as scaleRateX, for scaleY
	// Actor's targetX to targetZ.
	Sint32        targetX;
	Sint32        targetY;
	Sint32        targetZ;
	// Added to x, y and z each tick by the update hooks, in 16.16 fixed-point pixels per tick.
	VECTOR3       velocity;
	FIELD_SPR    *sprite;            // the sprite spin turns
	Uint8         spriteStorageKind; // Actor's
	Uint8         motionFlags;       // zeroed by the spawner
	// Actor's arrival axis, last flag test and snap flag.
	Uint8         arrivalAxis;
	Uint8         conditionResult;
	Uint8         snapToTarget;
	Uint8         collisionFlags; // Actor's
	// Actor's steeringFlags to sceneByte.
	Uint8         steeringFlags;
	Uint8         paused;
	Uint8         retainSpriteCache;
	Uint8         repeatCount;
	Sint16        wait;
	Uint8         refused;
	Uint8         frozen;
	Uint8         sceneCount;
	Uint8         sceneByte;
	// Added to the sprite's rotation each tick by the update hooks, in 1/65536 turns per tick;
	// zeroed by the spawner.
	Uint16        spin;
	Sint16        userValue; // Actor's
	Sint32        countdown; // no code names it
	// Called each tick while the effect is live: the spawner sets fdEffectUpdateMotionScaleAndSpin,
	// or options.update.
	void          (*update)(union FIELD_OBJECT *object);
} FIELD_EFFECT;

STATIC_ASSERT(offsetof(FIELD_EFFECT, scaleRateX) == 48);
STATIC_ASSERT(offsetof(FIELD_EFFECT, velocity) == 68);
STATIC_ASSERT(offsetof(FIELD_EFFECT, sprite) == 80);
STATIC_ASSERT(offsetof(FIELD_EFFECT, spin) == 100);
STATIC_ASSERT(offsetof(FIELD_EFFECT, countdown) == 104);
STATIC_ASSERT(offsetof(FIELD_EFFECT, update) == 108);

// An object on the field, read as an actor or as an effect.
typedef union FIELD_OBJECT
{
	ACTOR        actor;
	FIELD_EFFECT effect;
} FIELD_OBJECT;

// The flags of a scene's effect spawner (scFieldEffectSpawn): a script number in the low bits and
// which of the spawn options' fields to use.
typedef enum EFFECT_SPAWN_FLAGS
{
	// Picks one of the spawner's effect scripts; the object's mode is this plus 1.
	EFFECT_SCRIPT_MASK     = 0x0f,
	EFFECT_USE_PALETTE     = 0x10000, // options.palette
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

// The spawn flags that pick an option, above the script number.
#define EFFECT_OPTION_MASK  0xffff0000
// The sprite an effect takes unless EFFECT_USE_TYPE gives another (sprite_ids.h).
#define EFFECT_DEFAULT_TYPE SPRITE_DUST_CLOUD

// The scripts EFFECT_SCRIPT_MASK picks from FieldEffectDurationScripts: each holds the effect for a
// while, then releases it.
typedef enum EFFECT_DURATION
{
	EFFECT_SCRIPT_SHORT  = 0, // held 24 ticks
	EFFECT_SCRIPT_MEDIUM = 1, // held 46 ticks
	EFFECT_SCRIPT_LONG   = 2  // held 128 ticks
} EFFECT_DURATION;

// The word of a duration script that holds its repeat count, the ticks EFFECT_SCALE_TO_TARGET grows
// over.
#define EFFECT_SCRIPT_REPEAT_COUNT 3

// The names field/field_effect_spawn.c compiles a scene's copy of the effect spawner under, the
// same in every scene that includes it: the spawner, the import that creates its actor, and the
// duration scripts EFFECT_SCRIPT_MASK picks from. The scene names EFFECT_SET_MODE and
// EFFECT_SET_SCRIPT itself.
#define EFFECT_SPAWN               scFieldEffectSpawn
#define EFFECT_CREATE              syImportActorCreate
#define EFFECT_SCRIPTS             FieldEffectDurationScripts

// A drifting effect loses a FIELD_EFFECT_DRAG_X-th of its x speed and a FIELD_EFFECT_DRAG_Z-th of
// its z speed each tick.
#define FIELD_EFFECT_DRAG_X        18
#define FIELD_EFFECT_DRAG_Z        16

// Sixteen puffs of dust in a ring round the leader as he lands from above, the first thrown twice.
#define DUST_RING_PUFFS            16

// The options of a spawned field effect, each used when its EFFECT_USE_ flag (or
// EFFECT_SCALE_TO_TARGET) is set.
typedef struct EFFECT_OPTIONS
{
	Sint32        priority;     // the sprite's priority
	Sint32        palette;      // passed to fdActorSetSpriteColor
	FIXED         startScaleX;  // 16.16 fixed point, toFIXED(1) full size
	FIXED         startScaleY;  // 16.16 fixed point, toFIXED(1) full size
	FIXED         targetScaleX; // 16.16 fixed point, toFIXED(1) full size
	FIXED         targetScaleY; // 16.16 fixed point, toFIXED(1) full size
	Sint16        type;         // the object type to create
	const Sint32 *script;       // the object script, run in mode 1
	Uint16        rotation;     // 1/65536 turns
	Uint16        spin;         // 1/65536 turns per tick
	void         *update;       // replaces the update hook
} EFFECT_OPTIONS;

STATIC_ASSERT(offsetof(EFFECT_OPTIONS, type) == 24);
STATIC_ASSERT(offsetof(EFFECT_OPTIONS, update) == 36);

void fdEffectUpdateMotionScaleAndSpin(FIELD_OBJECT *object);

void fdEffectSpawnResident(Sint32                x,
                           Sint32                y,
                           Sint32                z,
                           Sint32                velocityX,
                           Sint32                velocityY,
                           Sint32                velocityZ,
                           Uint32                flags,
                           const EFFECT_OPTIONS *options);

#endif
