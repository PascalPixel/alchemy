#ifndef ALCHEMY_SPEAKER_H
#define ALCHEMY_SPEAKER_H

/// @file
/// The speaker lookup's view of an object event's sprite, the speaker
/// definitions, and the palette fade's heap slot.

#include "unit/overworld/overworld.h"
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

/// The palette fade, heap slot HEAP_SLOT_PALETTE_FADE under its own name.
extern struct PaletteFade *gPaletteFadeWork;

#endif
