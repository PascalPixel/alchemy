#ifndef ALCHEMY_OBJECT_EFFECT_H
#define ALCHEMY_OBJECT_EFFECT_H

#include "types.h"
#include "edition.h"

#if GAME_TLA
extern u8 *gActorEffectWork;
#else
#include "animspr.h"

struct FieldActor;

// The fade overlay draws one mark on each side of its tracked actor.
struct FadeOverlayWork {
  struct AnimationSpritePart marks[2];
  struct FieldActor *actor;
};

STATIC_ASSERT(sizeof(struct FadeOverlayWork) == 28);

extern struct FadeOverlayWork *gActorEffectWork;

#define FIELD_WEATHER_PARTICLE_COUNT 32

// One weather particle: OAM attributes followed by its world position.
struct GroundParticle {
  u8 unk00[4]; // Cleared with the attributes; never read by the effects.
  u16 y : 8;
  u16 affine : 2;
  u16 blendMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 attr3[2]; // OAM attribute 3, cleared with the others.
  s32 posX;    // World position, in 16.16 pixels.
  s32 posY;    // The ground's height there.
  s32 posZ;
  s32 delay; // Motes: height risen; sparkles: phase within a burst.
  u16 timer; // Frames left; zero frees the particle.
};

STATIC_ASSERT(offsetof(struct GroundParticle, posX) == 0x0c);
STATIC_ASSERT(sizeof(struct GroundParticle) == 0x20);

// Work slot 29 while ground particles, motes or sparkles run.
struct GroundParticleWork {
  s32 entry;    // Resource entry holding the effect's tiles.
  s32 tileBase; // Their first tile in VRAM.
  struct GroundParticle particles[FIELD_WEATHER_PARTICLE_COUNT];
  s32 unk408;  // Unused by the weather effects.
  s32 stopped; // Prevent new sparkle bursts while set.
};

STATIC_ASSERT(sizeof(struct GroundParticleWork) == 0x410);

extern struct GroundParticleWork *gParticleWork;
#endif

#endif
