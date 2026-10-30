#ifndef ALCHEMY_SPEAKER_H
#define ALCHEMY_SPEAKER_H

/// @file
/// The speaker lookup's view of an actor's sprite, the speaker
/// definitions, and the palette fade's heap slot.

#include "module/field/field_sequence.h"
#include "types.h"
#include "static_assert.h"
#include "palette_buffers.h"
#include "sounds.h"

/// An actor's animated sprite (struct Sprite), as far as the
/// pointer to its sprite id.
struct SpeakerSprite {
  u8 beforeEntries[40]; ///< Sprite's fields before its layers
  /// Sprite.entries[0], its first layer, whose first halfword is the
  /// sprite id it shows (SpriteAnimation.animationId); null for none.
  s16 *firstEntry;
};

/// SpeakerDefinition.value of a voice: the first of its four blip sounds, less
/// SE_VOICE_FIRST.
#define SPEAKER_VOICE(sound) ((sound) - SE_VOICE_FIRST)
/// SpeakerDefinition.value of a sprite with no voice.
#define SPEAKER_VOICE_NONE 0xff

/// A speaker definition, a row of gSpeakerDefinitions: a sprite, its
/// voice (SPEAKER_VOICE_NONE for none) and the flags that choose its idle line.
struct SpeakerDefinition {
  u16 id; ///< the sprite id
  /// The voice, SPEAKER_VOICE of the first of its four blip sounds: a message
  /// plays SE_VOICE_FIRST + value as it types (Speaker_GetVoice).
  u8 value;
  u8 flags; ///< choose its line when it has nothing to say
            ///< (Speaker_GetIdleFlags)
};

/// The palette fade, heap slot HEAP_SLOT_PALETTE_FADE under its own name.
extern struct PaletteFade *gPaletteFadeWork;

#endif
