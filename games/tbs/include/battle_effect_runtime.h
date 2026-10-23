#ifndef ALCHEMY_BATTLE_EFFECT_RUNTIME_H
#define ALCHEMY_BATTLE_EFFECT_RUNTIME_H

/// @file
/// The speaker lookup's views of an object event and its sprite, the speaker
/// definitions, and the colour buffer's work slot.

#include "unit/object/object.h"
#include "object_lookup.h"
#include "types.h"
#include "static_assert.h"
#include "palbuf.h"

/// An object event's animated sprite (struct Sprite), as far as the
/// pointer to its sprite id.
struct SpeakerSprite {
  u8 beforeEntries[0x28]; ///< Sprite's fields before its layers
  /// Sprite.entries[0], its first layer, whose first halfword is the
  /// sprite id it shows (SpriteAnim.animationId); null for none.
  s16 *value28;
};

/// An object event as the speaker lookup reads it: its kind (1 for an animated
/// sprite) and that sprite.
struct SpeakerObject {
  u8 beforeVelocity[0x24]; ///< Object's fields before velocityX
  /// Object's motion, target, bounce, gravity and riseBaseY; not named
  /// through this view.
  ///@{
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 speedLimit;
  s32 acceleration;
  s32 targetX;
  s32 targetY;
  s32 targetZ;
  s32 bounce;
  s32 gravity;
  s32 riseBaseY;
  ///@}
  struct SpeakerSprite *animation; ///< Object.animation
  u8 kind; ///< Object.animationKind: 1 for one animated sprite
};

/// A speaker definition, a row of gSpeakerDefinitions: a sprite, its
/// voice (0xff for none) and the flags that choose its idle line.
struct SpeakerDefinition {
  u16 id; ///< the sprite id
  /// The first of its four voice blip sounds, less 256: a message plays sound
  /// 256 + value as it types (Speaker_GetVoice); 0xff for none.
  u8 value;
  u8 flags; ///< choose its line when it has nothing to say
            ///< (Speaker_GetIdleFlags)
};

/// The colour buffer, heap slot HEAP_SLOT_PALETTE_FADE under its own name.
extern struct PaletteFade *gPaletteFadeWork;

#endif
