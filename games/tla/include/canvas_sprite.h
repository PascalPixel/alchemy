#ifndef ALCHEMY_CANVAS_SPRITE_H
#define ALCHEMY_CANVAS_SPRITE_H

#include "types.h"
#include "static_assert.h"
#include "effect_sprite.h"

// The canvas sprite of the field Psynergy scenes: a 64x64 sprite whose
// pixels an effect draws into a 4 KB canvas (battle_fx_work[1]).
// Func_080eb824 starts it, Func_080eb6a0 draws the sprite each frame,
// Func_080eb594 copies the canvas into its tiles on command, and
// BattleFx_EndCanvasSprite ends it.

/// The heap slots (byte offsets into Ram_HeapSlots) it takes.
#define HEAP_SLOT_CANVAS_SPRITE 240
#define HEAP_SLOT_CANVAS 96
#define HEAP_SLOT_CANVAS_MAP 180

/// The canvas: 64x64 pixels of 8 bits, in 8x8 tiles.
#define CANVAS_WIDTH 64
#define CANVAS_SIZE 0x1000

// How the sprite is drawn.
#define CANVAS_SPRITE_16_COLORS 1  // rebuilt each frame, in OBJ palette 14
#define CANVAS_SPRITE_256_COLORS 2 // rebuilt each frame, in 256 colours
#define CANVAS_SPRITE_KEPT 3       // drawn as the effect left it

/// What Func_080eb594 does with the canvas in the next frame.
#define CANVAS_UPLOAD_AND_CLEAR 1
#define CANVAS_UPLOAD 2
#define CANVAS_UPLOAD_AND_FADE1 3
#define CANVAS_UPLOAD_AND_FADE2 4
#define CANVAS_UPLOAD_AND_FADE3 5
#define CANVAS_UPLOAD_TO_BG 11

struct CanvasSpriteWork {
  struct EffectSprite sprite;
  u8 unk28[0x78];
  s32 drawnFrames;
  s32 frame;
  s32 x; ///< whole pixels
  s32 y;
  s32 scaleX;
  s32 scaleY;
  s32 angle;
  u16 tile;
  u8 depth;
  u8 priority;
  s8 mode;
  s8 command;
  u8 unkc2[2];
};

STATIC_ASSERT(sizeof(struct CanvasSpriteWork) == 0xc4);
STATIC_ASSERT(offsetof(struct CanvasSpriteWork, frame) == 0xa4);
STATIC_ASSERT(offsetof(struct CanvasSpriteWork, tile) == 0xbc);

void Func_080eb594(void);
void Func_080eb6a0(void);
void Func_080eb824(void);
void BattleFx_EndCanvasSprite(void);

#endif
