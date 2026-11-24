#ifndef ALCHEMY_SQUASH_AND_STRETCH_H
#define ALCHEMY_SQUASH_AND_STRETCH_H

/// @file
/// The squash and stretch that people standing about play now and then
/// (Scene_SquashAndStretch).

/// A random pause before an actor's next squash and stretch: 60 to 149 frames.
#define SQUASH_AND_STRETCH_PAUSE()                                             \
  (Math_RemainderUnsigned(Random_Next(), 90) + 60)

#endif
