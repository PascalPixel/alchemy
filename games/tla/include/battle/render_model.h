#ifndef ALCHEMY_RENDER_MODEL_H
#define ALCHEMY_RENDER_MODEL_H

#include "types.h"

/// @file
/// The model records the battle effects hand to the ARM model renderer.

/// A texture for the ARM model renderer: its size as powers of two and the
/// address of its pixels.
struct ModelTexture {
  u32 logWidth : 8;  ///< the width is 1 << logWidth pixels
  u32 logHeight : 8; ///< the height is 1 << logHeight pixels
  u32 unk02 : 16;
  const u8 *pixels;
};

/// One entry of the list Render_DrawModelList draws: which rasterizer draws
/// it, its faces and their transformed corners, its texture and its shade.
/// Render_AllocateModelList ends the list with a cleared entry.
struct RenderModel {
  s32 kind;        ///< the rasterizer kernel that draws it; 0 ends the list
  s32 flags;       ///< MODEL_TEXTURE_IN_PLACE keeps the texture where it is
  const u16 *mesh; ///< the faces, as the effect tables write them
  s16 *vertices;   ///< the corners Render_TransformVertices wrote
  struct ModelTexture *texture;
  long shade; ///< a textured model's brightness
  u8 scrollU; ///< how far the texture is scrolled across
  u8 scrollV; ///< how far the texture is scrolled down
};

/// Draws a list of models, ended by a cleared entry: each model's rasterizer is
/// copied into IWRAM for a run of its kind, and its texture with it where there
/// is room.
void Render_DrawModelList(struct RenderModel *model);

#endif
