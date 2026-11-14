#ifndef ALCHEMY_SPEAKER_H
#define ALCHEMY_SPEAKER_H

/// @file
/// Speaker bits, above the actor id in the low 12 bits of a message command's
/// speaker, as FieldMessage_ShowAtActor reads them.

#define SPEAKER_PORTRAIT_LEFT 0x1000  ///< the portrait left of the speaker
#define SPEAKER_PORTRAIT_RIGHT 0x2000 ///< the portrait right of the speaker
#define SPEAKER_WINDOW_ABOVE 0x4000   ///< the message window above the speaker
#define SPEAKER_WINDOW_BELOW 0x8000   ///< the message window never above it

#endif
