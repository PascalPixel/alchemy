#ifndef ALCHEMY_EMOTE_H
#define ALCHEMY_EMOTE_H

/// @file
/// The bubbles an actor shows over its head (Actor_ShowEmoteById).

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

#endif
