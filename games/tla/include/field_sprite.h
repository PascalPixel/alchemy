#ifndef TLA_FIELD_SPRITE_H
#define TLA_FIELD_SPRITE_H

#include "types.h"

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
    u8 unk0a[6];
    u8 vramBlock;
    u8 unk11;
    u16 rotation;
    u8 unk14[4];
    u8 frame;
    u8 unk19;
    u8 nextFrame;
    u8 unk1b[9];
    u16 secondTile : 10;
    u16 partPriority : 2;
    u16 secondPalette : 4;
};

#endif
