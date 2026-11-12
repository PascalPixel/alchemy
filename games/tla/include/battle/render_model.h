#ifndef ALCHEMY_RENDER_MODEL_H
#define ALCHEMY_RENDER_MODEL_H

#include "types.h"

/// @file
/// The model records the battle effects hand to the ARM model renderer.

/// A texture for the ARM model renderer: its size as powers of two and the
/// address of its pixels.
struct ModelTexture {
  u32 logWidth : 8;
  u32 logHeight : 8;
  u32 unk02 : 16;
  const void *pixels;
};

/// A model for the ARM renderer (Render_DrawModelList).
struct RenderModel {
  s32 kind;
  s32 unk04;
  const void *mesh;
  void *vertices;
  struct ModelTexture *texture;
  s32 shade;
  u8 scrollU;
  u8 scrollV;
};

#endif
