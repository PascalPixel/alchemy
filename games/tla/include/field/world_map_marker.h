#ifndef ALCHEMY_WORLD_MAP_MARKER_H
#define ALCHEMY_WORLD_MAP_MARKER_H

/// @file
/// The world map's marker sprites.

#include "types.h"

/// A marker sprite's OAM entry, after a word of its own.
struct WorldMapMarker {
  u8 unk00[4];
  u16 y : 8;
  u16 affine : 2;
  u16 objectMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[2];
};

#endif
