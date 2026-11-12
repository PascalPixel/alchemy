/// @file
/// Altin Peak's frozen puddle and the main image's Frost (frost.c), each with
/// its own colour-mode routine.
#include "types.h"
#include "field.h"

/// An update routine that makes a frozen thing glint: its colour mode is
/// SPRITE_COLOR_RBG for two frames in every eight.
#define DEFINE_FROST_GLINT(linkage, setColorMode)                              \
  linkage void FieldEffect_UpdateFrostGlint(struct Actor *actor) {             \
    struct Actor *particle = (struct Actor *)actor;                            \
    s32 frame = gFrameCount & 7;                                               \
    if (frame == 0) {                                                          \
      setColorMode(particle, SPRITE_COLOR_RBG);                                \
    } else if (frame == 2) {                                                   \
      setColorMode(particle, SPRITE_COLOR_NORMAL);                             \
    }                                                                          \
  }
