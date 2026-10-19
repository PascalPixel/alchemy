#ifndef TLA_FIELD_SPRITE_H
#define TLA_FIELD_SPRITE_H

#include "types.h"

/// An animated sprite on the field: its image's sprite attributes, VRAM
/// cache entry, rotation, its shadow's attribute 2 and its first layer.
struct FieldSprite {
  u8 unk00[4]; ///< not named by TLA code
  /// The image's OAM attributes 0 to 2.
  ///@{
  u16 y : 8; ///< screen pixels
  u16 affine : 2;
  u16 blendMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1; ///< 256 colours
  u16 shape : 2;
  u16 x : 9; ///< screen pixels
  u16 affineIndex : 5;
  u16 flipX : 1;
  u16 flipY : 1;
  u16 tile : 10; ///< first sprite tile in VRAM
  u16 priority : 2;
  u16 palette : 4;
  ///@}
  /// Bytes 2 to 5 hold the sprite's 16.16 scale in the projection draft's
  /// layout (recon/tla LayeredSprite); not named here.
  u8 unk0a[6];
  u8 vramBlock; ///< VRAM cache entry holding its tiles
  /// The keep-cache-entry and own-scale bits in the projection draft's layout.
  u8 unk11;
  u16 rotation; ///< 1/65536 turns
  /// Width, height and the signed x and y offsets in that layout; scene code
  /// moves the offsets (bytes 2 and 3).
  u8 unk14[4];
  /// The animation number last selected, saved with the object's snapshot.
  u8 frame;
  u8 unk19; ///< nonzero redraws its pixels (dirty in that layout)
  /// The sprite flags, saved with the object's snapshot; zeroed for sprites
  /// shown without them.
  u8 nextFrame;
  /// Byte 0 is the layer count, cleared with the first layer; then the
  /// shadow's link and attributes 0 and 1.
  u8 unk1b[9];
  /// The shadow's OAM attribute 2.
  ///@{
  u16 secondTile : 10;
  u16 partPriority : 2;
  u16 secondPalette : 4;
  ///@}
  u8 unk26[2];  ///< not named by TLA code
  void *record; ///< released by ResourceMetadata_ClearRecordFar
};

#endif
