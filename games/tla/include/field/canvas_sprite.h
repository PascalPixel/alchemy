#ifndef ALCHEMY_CANVAS_SPRITE_H
#define ALCHEMY_CANVAS_SPRITE_H

/// @file
/// The canvas sprite of a field Psynergy: a 64 by 64 sprite whose pixels an
/// effect draws into a 4 KB canvas. BattleEffect_StartCanvasSprite starts it,
/// BattleEffect_DrawCanvasSprite draws the sprite each frame,
/// BattleEffect_UploadCanvas copies the canvas into its tiles on command, and
/// BattleEffect_EndCanvasSprite ends it.

#include "types.h"
#include "static_assert.h"
#include "field/effect_sprite.h"

/// The canvas: 64x64 pixels of 8 bits, in 8x8 tiles.
#define CANVAS_WIDTH 64
#define CANVAS_SPRITE_BYTES 4096

/// How the sprite is drawn.
#define CANVAS_SPRITE_16_COLORS 1  ///< rebuilt each frame, in OBJ palette 14
#define CANVAS_SPRITE_256_COLORS 2 ///< rebuilt each frame, in 256 colours
#define CANVAS_SPRITE_KEPT 3       ///< drawn as the effect left it

/// The OBJ palette a 16-colour canvas is drawn in. The effects that draw in
/// 256 colours keep their own colours in it and the next, colours 224 to 255.
#define CANVAS_PALETTE 14

/// What BattleEffect_UploadCanvas does with the canvas in the next frame.
#define CANVAS_UPLOAD_AND_CLEAR 1
#define CANVAS_UPLOAD 2
#define CANVAS_UPLOAD_AND_DARKEN_ONE_LEVEL 3
#define CANVAS_UPLOAD_AND_DARKEN_TWO_LEVELS 4
#define CANVAS_UPLOAD_AND_DARKEN_FOUR_LEVELS 5
#define CANVAS_UPLOAD_TO_BG 11

/// A field Psynergy's canvas sprite: its effect sprite, the frame it is on and
/// how long it has drawn, and where, how large, at what angle and depth it is
/// drawn.
struct CanvasSpriteState {
  struct EffectSprite sprite;
  u8 unusedAfterSprite[120]; ///< no code names it
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
  u8 unusedAfterCommand[2]; ///< no code names it
};

STATIC_ASSERT(sizeof(struct CanvasSpriteState) == 196);
STATIC_ASSERT(offsetof(struct CanvasSpriteState, frame) == 164);
STATIC_ASSERT(offsetof(struct CanvasSpriteState, tile) == 188);

void BattleEffect_UploadCanvas(void);
void BattleEffect_DrawCanvasSprite(void);
void BattleEffect_StartCanvasSprite(void);
void BattleEffect_EndCanvasSprite(void);

#endif
