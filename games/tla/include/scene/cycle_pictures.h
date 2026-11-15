#ifndef ALCHEMY_CYCLE_PICTURES_H
#define ALCHEMY_CYCLE_PICTURES_H

/// @file
/// A sprite that steps through a list of pictures.

#include "types.h"

struct SpritePictures {
  u8 unk00[5];
  u8 picture;
};

struct AnimatedSprite {
  u8 unk00[0x12];
  s16 rotation;
  u8 unk14[5];
  u8 picture;
  u8 nextPicture;
  u8 unk1b[0xd];
  struct SpritePictures *pictures;
};

#endif
