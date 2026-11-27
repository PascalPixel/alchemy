#ifndef ALCHEMY_WEATHER_H
#define ALCHEMY_WEATHER_H

// The ground particles and the rain that the weather state holds.

#include "fixed_math.h"
#include "types.h"

// A ground particle: an OAM entry linked by depth, its place (16.16 pixels), its delay and its tick
// count.
typedef struct WEATHER_PARTICLE
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
    FIXED  posX;
    FIXED  posY;
    FIXED  posZ;
    Sint32 delay;
    Uint16 timer;
    Uint8  unusedAfterTimer[2]; // no code names it
} WEATHER_PARTICLE;

// WEATHER_PARTICLE.timer after the decrement that counts a live particle down, when it was already
// free (0): the Uint16 wraps.
#define GROUND_PARTICLE_FREED  65535
// The frames of a ground particle's splash (WeatherRainSplashFrames, or a scene's own
// GroundParticleFrames, five halfwords each), played from the last down to the first.
#define RAIN_SPLASH_FRAMES     16

// WEATHER_STATE.particles.
#define WEATHER_PARTICLE_COUNT 64
// A raindrop's frames (fdWeatherUpdateRain).
#define RAIN_DROP_TICKS        12

// The heap slot weather while the ground particles run: their tiles' VRAM cache entry and first
// tile, and the particles.
typedef struct WEATHER_STATE
{
    Sint32           entry;     // the VRAM cache entry holding the particles' tiles
    long             tileBase;  // their first tile in VRAM
    WEATHER_PARTICLE particles[WEATHER_PARTICLE_COUNT];
    Uint8            unused[8]; // no code reads them
    Sint32           stopped;   // nonzero stops new sparkle bursts
} WEATHER_STATE;

#endif
