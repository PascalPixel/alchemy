#ifndef ALCHEMY_GROUND_MASK_H
#define ALCHEMY_GROUND_MASK_H

// The ground mask, which hides the parts of an actor that raised ground in front of it should
// cover, and the state of the weather's ground particles, motes of light and sparkles.

#include "edition.h"
#include "fixed_math.h"
#include "types.h"
#include "field/sprite.h"
#include "system.h"

struct ACTOR;

// The ground mask: where the ground just south of an actor rises above its floor (a ledge, the rim
// of a sunken floor), a 16-pixel square on each side of it is drawn over what of it the ground
// should stand in front of: beside a ledge in the Kalay Tunnel's treasure room, Isaac shows from
// the waist up.
typedef struct GROUND_MASK_STATE
{
    // The left and right marks: 16-pixel-wide semi-transparent sprites drawn where the ground
    // beside the actor rises above its floor.
    ANIMATION_SPR_PART marks[2];
    struct ACTOR      *actor; // the actor tracked; null draws nothing
} GROUND_MASK_STATE;

STATIC_ASSERT(sizeof(GROUND_MASK_STATE) == 28);

// HEAP_SLOT_GROUND_MASK: the ground mask's state from fdEffectStartGroundMask until the next scene
// change.
#define GroundMaskStateWork    (*(GROUND_MASK_STATE **)&HeapSlots[HEAP_SLOT_GROUND_MASK])

#define WEATHER_PARTICLE_COUNT 32 // WEATHER_STATE.particles

// One weather particle: OAM attributes followed by its world position.
typedef struct WEATHER_PARTICLE
{
    // The depth-slot queue's link, as ANIMATION_SPR_PART.link; cleared with the attributes and
    // never read by the effects.
    Uint8  link[4];
    // The OAM entry's attributes 0 to 2: the start functions write them whole as semi-transparent,
    // size 1, priority 1 and palette 13.
    Uint16 y           : 8;  // Screen pixels.
    Uint16 affine      : 2;  // Not named by the effects.
    Uint16 blendMode   : 2;  // 1, semi-transparent; not named by the effects.
    Uint16 mosaic      : 1;  // Not named by the effects.
    Uint16 fullColor   : 1;  // Not named by the effects.
    Uint16 shape       : 2;  // From the ground particle frame; motes, sparkles: 0.
    Uint16 x           : 9;  // Screen pixels.
    Uint16 affineIndex : 5;  // Sparkles: 8 on alternate ticks, else 0.
    Uint16 size        : 2;  // From the ground particle frame; motes, sparkles: 1.
    Uint16 tile        : 10; // The state's tileBase plus the tick's tile.
    Uint16 priority    : 2;  // 1.
    Uint16 palette     : 4;  // 13; not named by the effects.
    // The OAM entry's fourth halfword, cleared with the others.
    Uint8  attr3[2];
    FIXED  posX;  // World position, in 16.16 pixels.
    Sint32 posY;  // The ground's height there.
    FIXED  posZ;  // World position, in 16.16 pixels.
    Sint32 delay; // Motes: height risen; sparkles: phase within a burst.
    Uint16 timer; // Ticks left; zero frees the particle.
} WEATHER_PARTICLE;

// WEATHER_PARTICLE.timer after the decrement that counts a live particle down, when it was already
// free (0): the Uint16 wraps.
#define GROUND_PARTICLE_FREED 65535

STATIC_ASSERT(offsetof(WEATHER_PARTICLE, posX) == 12);
STATIC_ASSERT(sizeof(WEATHER_PARTICLE) == 32);

// HEAP_SLOT_WEATHER while ground particles, motes or sparkles run.
typedef struct WEATHER_STATE
{
    Sint32           entry;    // Resource entry holding the effect's tiles.
    Sint32           tileBase; // Their first tile in VRAM.
    WEATHER_PARTICLE particles[WEATHER_PARTICLE_COUNT];
    Sint32           unused;   // Unused by the weather effects.
    Sint32           stopped;  // Nonzero stops new sparkle bursts.
} WEATHER_STATE;

STATIC_ASSERT(sizeof(WEATHER_STATE) == 1040);

// HEAP_SLOT_WEATHER: the running ground particles, motes or sparkles.
#define WeatherStateWork (*(WEATHER_STATE **)&HeapSlots[HEAP_SLOT_WEATHER])

#endif
