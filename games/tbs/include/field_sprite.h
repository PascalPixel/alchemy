#ifndef ALCHEMY_FIELD_SPRITE_H
#define ALCHEMY_FIELD_SPRITE_H

#include "types.h"
#include "layout_guard.h"

struct FieldSprite {
  u8 unk00[4];
  u16 y : 8;
  u16 affine : 2;
  u16 blendMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 flipX : 1;
  u16 flipY : 1;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[0x0a];
  u16 secondTile : 10;
  u16 secondPriority : 2;
  u16 secondPalette : 4;
  u8 unk16[2];
  s32 scale;
  u8 vramBlock;
  u8 unk1d0 : 1;
  u8 unk1d1 : 1;
  u8 unk1d2 : 6;
  u16 rotation;
  u8 unk20[6];
  u8 flags;
  u8 partCount;
};

LAYOUT_OFFSET_GUARD(FieldSprite_Scale, struct FieldSprite, scale, 0x18);
LAYOUT_OFFSET_GUARD(FieldSprite_VramBlock, struct FieldSprite, vramBlock, 0x1c);
LAYOUT_OFFSET_GUARD(FieldSprite_Rotation, struct FieldSprite, rotation, 0x1e);
LAYOUT_OFFSET_GUARD(FieldSprite_PartCount, struct FieldSprite, partCount, 0x27);

struct FieldSpritePriority {
  u8 unk00[9];
  u8 tileHigh : 2;
  u8 priority : 2;
  u8 palette : 4;
};

#endif
