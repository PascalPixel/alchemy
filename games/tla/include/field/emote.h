#ifndef ALCHEMY_EMOTE_H
#define ALCHEMY_EMOTE_H

/// @file
/// What an actor shows over it: the bubbles of Actor_ShowEmoteById and the
/// effects FieldEffect_SetAttached attaches.

/// Actor_ShowEmoteById's emote flag: the bubble takes sprite priority 1
/// instead of its actor's. The low four bits pick the bubble (enum Emote).
#define EMOTE_IN_FRONT 0x100

/// The bubbles Actor_ShowEmoteById can show, by the animation they pick in the
/// bubble's sprite sheet, as The Broken Seal's.
enum Emote {
  EMOTE_EXCLAMATION = 0, ///< "!"
  EMOTE_QUESTION = 1,    ///< "?"
  EMOTE_WINCE = 2,       ///< a wincing face
  EMOTE_ANGER_VEIN = 3,  ///< the cross-shaped vein of anger
  EMOTE_HEART = 4,       ///< a red heart
  EMOTE_ELLIPSIS = 5,    ///< "..."
  EMOTE_LIGHT_BULB = 6,  ///< a lit bulb
  EMOTE_SCOWL = 7,       ///< a scowling face
  EMOTE_SMILE = 8,       ///< a smiling face
  EMOTE_SWEAT_DROP = 9   ///< a sweat drop
};

/// FieldEffect_SetAttached's flags: a mode in the low two bits, and
/// ATTACHED_EFFECT_IN_FRONT, as The Broken Seal's.
enum AttachedEffectFlag {
  ATTACHED_EFFECT_REMOVE = 0, ///< removes the attached actor
  ATTACHED_EFFECT_ATTACH = 1, ///< attaches one that stays until removed
  ATTACHED_EFFECT_ONCE = 2,   ///< shows a one-off one that ends by itself
  /// The attached actor takes sprite priority 1, not the actor's.
  ATTACHED_EFFECT_IN_FRONT = 0x100
};

#endif
