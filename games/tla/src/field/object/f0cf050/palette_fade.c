/// @file
/// The palette fade: palettes moved toward target colours.
// Part of f0cf050.c.

#include "../f0cf050_internal.h"
#include "ground_mask.h"
#include "module/field/f0cf050.h"
#include "module/field/object.h"
#include "module/system/system.h"

void BattleEffect_InterpolateBuffers(s16 *source, s16 *target, s16 *step,
                                     s32 frames) {
  s32 index;
  s32 first;
  s32 second;
  s32 (*divide)(s32, s32);

  if (frames > 0) {
    divide = Iwram_SignedDivide;
    index = 0x53F;
    do {
      first = *source;
      second = *target;
      *step = divide(second - first, frames);
      index--;
      source++;
      target++;
      step++;
    } while (index >= 0);
  }
}

void BattlePalette_UpdateBlend(void) {
  struct BattleEffectBuffers *work = Ram_HeapSlots->block32;
  u16 *add = work->delta;
  s32 i;

  if (GameFlag_Test(0x152) != 0) {
    return;
  }
  if (work->duration == 0) {
    return;
  }
  if (++work->step < work->duration) {
    u16 *sum = work->current;
    for (i = 0; i < 0x540; i++) {
      *sum++ += *add++;
    }
  } else {
    // The routine's entry in a local loads ahead of the size's shift; as a
    // direct call, 2 of 175 instructions differ in tla-en.
    s32 (*copy)(void *, const void *, s32) = Iwram_CopyWords;

    copy(work->current, work->target, sizeof(work->current));
    work->duration = 0;
  }

  {
    u16 *packed = work->packed[1 ^ work->page];
    s32 blue = 0x7c00;
    u16 *current;

    i = 0x1c0;
    current = work->current;
    for (; i != 0; i--) {
      *packed++ = (current[0] & blue) | (((s16)current[1] >> 5) & 0x3e0) |
                  (((s16)current[2] >> 10) & 0x1f);
      current += 3;
    }
  }

  work->page ^= 1;
  {
    u16 *packed = work->packed[work->page];

    IO_WRITE_QUEUE_PUSH((u32)BG_PLTT, (u32)packed, DMA_COPY_WORDS(112));
    IO_WRITE_QUEUE_PUSH((u32)OBJ_PLTT, (u32)(packed + 224),
                        DMA_COPY_WORDS(112));
  }
}

#define COLOR_MAX 448
#define COLOR_HALF 224

#define gColorBuffer_ChannelCurve Data_080f0246
#define gColorBuffer_ChannelCurveLight Data_080f0286
#define gColorBuffer_ChannelCurveContrast Data_080f02c6
#define PaletteFade_ClampComponent Graphics_ClampRgb555Channel
#define PaletteFade_ClampShiftedComponent Graphics_ClampRgb555Component

void BattleEffect_BuildBuffer(u32 mode, u16 *src, u16 *dst, s32 half) {
  u32 i;
  u32 colorCount = COLOR_MAX;
  s32 red;
  s32 blue;
  u32 color;
  s32 green;

  if (mode == PALETTE_TINT_FROM_SOURCE)
    mode = *(u16 *)BG_PLTT;
  if (half == 1) {
    colorCount = COLOR_HALF;
  } else if (half == 2) {
    dst += COLOR_HALF * 3;
    src += COLOR_HALF;
    colorCount = COLOR_HALF;
  }

  // One colour: write it once, then let DMA copy it over the rest. The copy
  // reads three halfwords behind where it writes, so the colour repeats.
  if (mode < PALETTE_TINT_FROM_SOURCE) {
    *dst++ = mode & 0x7c00;
    *dst++ = (mode & 0x03e0) << 5;
    *dst++ = (mode & 0x001f) << 10;
    Dma_Set(dst - 3, dst, (((colorCount - 1) * 6) >> 1) | (DMA_ENABLE << 16),
            REG_DMA3);
  } else if (mode < PALETTE_TINT_FLAGS) {
    switch (mode) {
    case PALETTE_TINT_GRAYSCALE: // grey of brightness (2R + 4G + B) / 7
      for (i = 0; i < colorCount; i++) {
        color = *src++;
        red = ((color << 11) & 0xf800);
        green = ((color << 7) & 0x1f000);
        blue = (color & 0x7c00);
        color = Iwram_SignedDivide(red + green + blue, 7);
        dst[i * 3 + 0] = color;
        dst[i * 3 + 1] = color;
        dst[i * 3 + 2] = color;
      }
      break;
    case PALETTE_TINT_DIM_GRAY: // a third of R + G + B, B three steps darker
      for (i = 0; i < colorCount; i++) {
        color = *src++;
        red = color & 31;
        color = Iwram_SignedDivide(
            red + ((color >> 5) & 31) + ((color >> 10) & 31), 3);
        red = color - 1;
        green = red;
        red = PaletteFade_ClampComponent(red);
        green = PaletteFade_ClampComponent(green);
        blue = color - 3;
        blue = PaletteFade_ClampComponent(blue);
        dst[0] = gColorBuffer_ChannelCurveLight[blue];
        dst[1] = gColorBuffer_ChannelCurveLight[green];
        dst[2] = gColorBuffer_ChannelCurveLight[red];
        dst += 3;
      }
      break;
    case PALETTE_TINT_SEPIA: // the brightness, red stronger, within 8 to
                             // 28
      for (i = 0; i < colorCount; i++) {
        color = *src++;
        red = color & 31;
        green = (color >> 5) & 31;
        blue = (color >> 10) & 31;
        color = Iwram_SignedDivide(red + green + blue, 10);
        red = color * 4 + 5;
        blue = color * 3 + 5;
        green = blue;
        if (red < 8)
          red = 8;
        if (green < 8)
          green = 8;
        if (blue < 8)
          blue = 8;
        if (red > 28)
          red = 28;
        if (green > 28)
          green = 28;
        if (blue > 28)
          blue = 28;
        *dst++ = gColorBuffer_ChannelCurveLight[blue];
        *dst++ = gColorBuffer_ChannelCurveLight[green];
        *dst++ = gColorBuffer_ChannelCurveLight[red];
      }
      break;
    case PALETTE_TINT_WARM: // half R + 10, two thirds G + 8, B - 7 (black
                            // stays black)
      for (i = 0; i < colorCount; i++) {
        color = *src++;
        red = color & 31;
        green = (color >> 5) & 31;
        blue = (color >> 10) & 31;
        if ((red | green | blue) != 0) {
          red -= (u32)red >> 1;
          green -= green / 3;
          red += 10;
          green += 8;
          blue -= 7;
          red = PaletteFade_ClampComponent(red);
          green = PaletteFade_ClampComponent(green);
          blue = PaletteFade_ClampComponent(blue);
        }
        *dst++ = gColorBuffer_ChannelCurveContrast[blue];
        *dst++ = gColorBuffer_ChannelCurveLight[green];
        *dst++ = gColorBuffer_ChannelCurve[red];
      }
      break;
    case PALETTE_TINT_MUTED: // narrow the range, G and B + 2, past the first
      for (i = 0; i < colorCount; i++) {
        color = *src++;
        red = color & 31;
        green = (color >> 5) & 31;
        blue = (color >> 10) & 31;
        if (i != 0) {
          if (red < 8)
            red = 8;
          if (green < 14)
            green = 14;
          if (blue < 14)
            blue = 14;
          if (red > 29)
            red = 29;
          if (green > 25)
            green = 25;
          if (blue > 27)
            blue = 27;
          green += 2;
          blue += 2;
          red = PaletteFade_ClampComponent(red);
          green = PaletteFade_ClampComponent(green);
          blue = PaletteFade_ClampComponent(blue);
        }
        *dst++ = gColorBuffer_ChannelCurve[blue];
        *dst++ = gColorBuffer_ChannelCurve[green];
        *dst++ = gColorBuffer_ChannelCurve[red];
      }
      break;
    case PALETTE_TINT_PALE: // half of each, plus the mean
      for (i = 0; i < colorCount; i++) {
        s32 mean;

        color = *src++;
        red = color & 31;
        green = (color >> 5) & 31;
        blue = (color >> 10) & 31;
        mean = PaletteFade_ClampComponent((red + green + blue) / 3);
        red = (red >> 1) + mean;
        green = (green >> 1) + mean;
        blue = (blue >> 1) + mean;
        red = PaletteFade_ClampComponent(red);
        green = PaletteFade_ClampComponent(green);
        blue = PaletteFade_ClampComponent(blue);
        dst[0] = gColorBuffer_ChannelCurveContrast[blue];
        dst[1] = gColorBuffer_ChannelCurveContrast[green];
        dst[2] = gColorBuffer_ChannelCurveContrast[red];
        dst += 3;
      }
      break;
    case PALETTE_TINT_RED: // an eighth of G and B added to R; G and B two
                           // thirds
      for (i = 0; i < colorCount; i++) {
        color = *src++;
        red = color & 31;
        green = (color >> 5) & 31;
        blue = (color >> 10) & 31;
        red += ((green >> 3) + (blue >> 3));
        red = PaletteFade_ClampComponent(red);
        green -= green / 3;
        blue -= blue / 3;
        dst[0] = gColorBuffer_ChannelCurve[blue];
        dst[1] = gColorBuffer_ChannelCurve[green];
        dst[2] = gColorBuffer_ChannelCurveLight[red];
        dst += 3;
      }
      break;
    case PALETTE_TINT_WARM_DARK: // half R + 6, two thirds G + 4, B - 6
      for (i = 0; i < colorCount; i++) {
        color = *src++;
        red = color & 31;
        red -= (u32)red >> 1;
        green = (color >> 5) & 31;
        blue = (color >> 10) & 31;
        green -= green / 3;
        red += 6;
        green += 4;
        blue -= 6;
        red = PaletteFade_ClampComponent(red);
        green = PaletteFade_ClampComponent(green);
        blue = PaletteFade_ClampComponent(blue);
        dst[0] = gColorBuffer_ChannelCurveContrast[blue];
        dst[1] = gColorBuffer_ChannelCurveLight[green];
        dst[2] = gColorBuffer_ChannelCurve[red];
        dst += 3;
      }
      break;
    case PALETTE_TINT_DIM_BRIGHT: // a fifth off each channel of a bright colour
      for (i = 0; i < colorCount; i++) {
        color = *src++;
        red = color & 31;
        green = (color >> 5) & 31;
        blue = (color >> 10) & 31;
        if (red + green + blue > 60) {
          red -= red / 5;
          green -= green / 5;
          blue -= blue / 5;
        }
        red = PaletteFade_ClampComponent(red);
        green = PaletteFade_ClampComponent(green);
        blue = PaletteFade_ClampComponent(blue);
        *dst++ = gColorBuffer_ChannelCurve[blue];
        *dst++ = gColorBuffer_ChannelCurve[green];
        *dst++ = gColorBuffer_ChannelCurve[red];
      }
      break;
    default: // unchanged
      for (i = 0; i < colorCount; i++) {
        color = *src++;
        dst[i * 3 + 0] = color & 0x7c00;
        dst[i * 3 + 1] = (color & 0x03e0) << 5;
        dst[i * 3 + 2] = (color & 0x001f) << 10;
      }
      break;
    }
  } else if (mode & PALETTE_TINT_CUSTOM) {
    // The brightness (2R + 4G + B) times the colour in mode.
    u32 tintRed;
    u32 tintGreen;
    u32 tintBlue;
    s32 red;
    s32 green;

    tintRed = mode;
    tintRed &= 31;
    tintGreen = (mode >> 5) & 31;
    tintBlue = mode >> 10;
    tintBlue &= 31;
    for (i = 0; i < colorCount; i++) {
      color = *src++;
      red = ((color << 11) & 0xf800);
      green = ((color << 7) & 0x1f000);
      blue = (color & 0x7c00);
      color = Iwram_SignedDivide(red + green + blue, 96);
      red = color * tintRed;
      green = color * tintGreen;
      blue = color * tintBlue;
      red = PaletteFade_ClampShiftedComponent(red);
      green = PaletteFade_ClampShiftedComponent(green);
      blue = PaletteFade_ClampShiftedComponent(blue);
      dst[i * 3 + 0] = blue;
      dst[i * 3 + 1] = green;
      dst[i * 3 + 2] = red;
    }
  } else if (mode & PALETTE_TINT_BLEND) {
    // The brightness R + G + B shared out in the proportions of mode's colour.
    u32 tintRed;
    u32 tintGreen;
    u32 tintBlue;
    s32 blendedRed;
    s32 blendedGreen;
    s32 blendedBlue;
    s32 clampedRed;
    s32 clampedGreen;
    s32 clampedBlue;

    tintRed = mode;
    tintRed &= 31;
    tintGreen = (mode >> 5) & 31;
    tintBlue = mode >> 10;
    tintBlue &= 31;
    for (i = 0; i < colorCount; i++) {
      color = *src++;
      red = color & 31;
      color = Iwram_SignedDivide(
          (red + ((color >> 5) & 31) + ((color >> 10) & 31)) << 4,
          tintRed + tintGreen + tintBlue);
      blendedRed = Iwram_MulQ16(((color * tintRed) >> 4) << 16,
                                (s32)(tintRed << 16) >> 4);
      blendedGreen = Iwram_MulQ16(((color * tintGreen) >> 4) << 16,
                                  (s32)(tintGreen << 16) >> 4);
      blendedBlue = Iwram_MulQ16(((color * tintBlue) >> 4) << 16,
                                 (s32)(tintBlue << 16) >> 4);
      clampedRed = PaletteFade_ClampComponent((u32)blendedRed >> 16);
      clampedGreen = PaletteFade_ClampComponent((u32)blendedGreen >> 16);
      clampedBlue = PaletteFade_ClampComponent((u32)blendedBlue >> 16);
      dst[i * 3 + 0] = gColorBuffer_ChannelCurve[clampedBlue];
      dst[i * 3 + 1] = gColorBuffer_ChannelCurve[clampedGreen];
      dst[i * 3 + 2] = gColorBuffer_ChannelCurve[clampedRed];
    }
  } else if (mode & PALETTE_TINT_SPLIT) {
    // The colours unchanged, as the default tint writes them.
    for (i = 0; i < colorCount; i++) {
      color = *src++;
      dst[i * 3 + 0] = color & 0x7c00;
      dst[i * 3 + 1] = (color & 0x03e0) << 5;
      dst[i * 3 + 2] = (color & 0x001f) << 10;
    }
  } else {
    // Copy a ready buffer: mode is its address, and the OBJ half starts 224
    // colours of three halfwords in.
    if (half == 2)
      mode += COLOR_HALF * 3 * sizeof(u16);
    Dma_Set((const void *)mode, dst,
            ((colorCount * 6) >> 2) | (DMA_ENABLE | DMA_32BIT) << 16, REG_DMA3);
  }
}

void BattleEffect_InitializeBuffers(void) {
  volatile s32 zero;
  struct BattleEffectBuffers *buffer;

  buffer = HeapSlot_AllocEwram(128, sizeof(*buffer));
  zero = 0;
  Dma_Set(&zero, buffer,
          (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 |
              (sizeof(*buffer) / 4),
          REG_DMA3);
  Dma_Set((void *)BG_PLTT, buffer, DMA_COPY_WORDS(112), REG_DMA3);
  Dma_Set((void *)OBJ_PLTT, (u8 *)buffer->palette + 0x1c0, DMA_COPY_WORDS(112),
          REG_DMA3);
  BattleEffect_BuildBuffer(0x10000, buffer, buffer->target, 0);
  Task_Add((s32)BattlePalette_UpdateBlend, TASK_PASS_VBLANK + 15);
}

void BattlePalette_StopBlendAndReleaseBuffer(void) {
  Task_Remove((s32)&BattlePalette_UpdateBlend);
  HeapSlot_Release(128);
}

void BattleEffect_ApplyColorToTargetBuffer(s32 value, s32 mode) {
  struct BattleEffectBuffers *buffers = Ram_HeapSlots->block32;

  if (buffers != NULL) {
    BattleEffect_BuildBuffer(value, buffers, buffers->target, mode);
  }
}

void BattleEffect_ApplyColorToSourceBuffer(s32 value, s32 mode) {
  struct BattleEffectBuffers *buffers = Ram_HeapSlots->block32;

  if (buffers != NULL) {
    BattleEffect_BuildBuffer(value, buffers, buffers->current, mode);
  }
}

void BattleEffect_SetPrimaryBufferValue(u32 value) {
  struct BattleEffectBuffers *buffers = Ram_HeapSlots->block32;

  if (buffers != NULL) {
    buffers->palette[0] = value;
  }
}

// Packs the effect's target channels into its palette.

/// Packs the 448 target colours, three channels each, back into the effect's
/// palette as RGB555.
void Func_080d1760(void) {
  struct BattleEffectBuffers *work = Ram_HeapSlots->block32;
  u16 *packed = work->palette;
  s32 blue = 0x7c00;
  u16 *current;
  s32 i;

  i = 0x1c0;
  current = work->target;
  for (; i != 0; i--) {
    *packed++ = (current[0] & blue) | ((current[1] >> 5) & 0x3e0) |
                ((current[2] >> 10) & 0x1f);
    current += 3;
  }
  /* FAKEMATCH(dead): two updates of work that cancel, after the loop, keep it
   * live apart from packed; without them, 7 of 34 instructions differ in
   * tla-en. */
  work++;
  work--;
}

void BattleEffect_StartBufferInterpolation(s32 mode) {
  struct BattleEffectBuffers *buffers = Ram_HeapSlots->block32;

  if (buffers != NULL) {
    buffers->duration = mode;
    buffers->step = 0;
    BattleEffect_InterpolateBuffers((s16 *)buffers->current,
                                    (s16 *)buffers->target,
                                    (s16 *)buffers->delta, mode);
  }
}

void BattleEffect_LoadBufferPalette(const u16 *colours) {
  struct BattleEffectBuffers *buffers = Ram_HeapSlots->block32;

  if (buffers != NULL) {
    // The routine's entry in a local loads after the size; as a direct call,
    // 2 of 23 instructions differ in tla-en.
    s32 (*copy)(void *destination, const void *source, s32 size) =
        Iwram_CopyWords;

    copy(buffers->palette, colours, 0x1c0);
    BattleEffect_BuildBuffer(0x10000, buffers, buffers->target, 1);
  }
}

// Palette: clamp RGB555 components.

/// Clamps a five-bit component to 0-31.
s32 Graphics_ClampRgb555Channel(s32 val) {
  if (val > 31)
    return 31;
  if (val < 0)
    val = 0;
  return val;
}

/// Clamps to an upper bound of 0x7c00 (RGB555 maximum blue), with no lower
/// bound.
s32 Graphics_ClampRgb555Component(s32 val) {
  if (val > 0x7c00)
    val = 0x7c00;
  return val;
}

// ☀️'s FadeOverlay_DrawMarks: each frame, on the two cells beside the actor
// where the ground one or two cells south rises above the actor's floor,
// draws a 16-pixel-wide mark at the height of that ground.
void Func_080d1840(void) {
  struct FadeOverlayWork *work = Ram_HeapSlots->unk84[3];
  s16 *camera = ((struct FadeView *)Ram_HeapSlots->mapWork)->camera;
  s32 cameraX = camera[1];
  s32 cameraZ = camera[3];
  struct AnimationSpritePart *mark = work->marks;
  struct FadeActor *actor = work->actor;
  s32 x;
  s32 z;
  s32 terrainFloor;
  u8 terrainLayer;
  u32 tile;
  s32 height;
  s32 secondHeight;
  s32 screenRow;
  s16 unused[2];

  if (actor == 0)
    return;
  z = actor->z;
  x = actor->x;
  terrainFloor = actor->floor;
  terrainLayer = actor->terrainId;
  tile = gResourceTableEntries[94].offset >> 5;
  x -= 0x80000;
  height = MapGrid_GetTerrainHeightFar(terrainLayer, x, z + 0x100000) >> 16;
  secondHeight =
      (MapGrid_GetTerrainHeightFar(terrainLayer, x, z + 0x200000) >> 16) - 16;
  if (secondHeight > height)
    height = secondHeight;
  if (height > 0 && height > terrainFloor) {
    ((u32 *)mark)[1] = 0x40000800;
    ((u32 *)mark)[2] = 0x400;
    mark->priority = 0;
    mark->tile = tile;
    mark->objectMode = 1;
    mark->x = ((u32)(x >> 16) & 0xfff0) - cameraX;
    screenRow = (z >> 16) & 0xf0;
    mark->y = screenRow - cameraZ - height + 16;
    Func_080140d8(mark, 0);
  }
  x += 0x100000;
  height = MapGrid_GetTerrainHeightFar(terrainLayer, x, z + 0x100000) >> 16;
  mark = &work->marks[1];
  secondHeight =
      (MapGrid_GetTerrainHeightFar(terrainLayer, x, z + 0x200000) >> 16) - 16;
  if (secondHeight > height)
    height = secondHeight;
  if (height > 0 && height > terrainFloor) {
    ((u32 *)mark)[1] = 0x40000800;
    ((u32 *)mark)[2] = 0;
    mark->priority = 0;
    mark->tile = tile;
    mark->objectMode = 1;
    mark->x = ((u32)(x >> 16) & 0xfff0) - cameraX;
    screenRow = (z >> 16) & 0xf0;
    mark->y = screenRow - cameraZ - height + 16;
    Func_080140d8(mark, 0);
  }
}

/// Clears the overlay tiles to colour 1, sets the blend registers and starts
/// Func_080d1840 at priority 0x480 on a 28-byte work block whose last word is
/// the object (the selected owner's when none is given).
void BattleEffect_StartFadeOverlay(void *object) {
  u32 *work;
  u8 *tiles;
  volatile u32 fill;

  work = HeapSlot_AllocEwram(144, 28);
  tiles = HeapSlot_Alloc(56, 0x400);
  Dma_Fill32Cell(fill, 0x11111111, tiles, 0x200);
  VramCache_Load(94, 0x200, tiles);
  HeapSlot_Release(56);
  Task_Add(Func_080d1840, TASK_PASS_VBLANK);
  Io_Write16(BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 |
                 BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_LIGHTEN | BLDCNT_TGT2_ALL,
             &REG_BLDCNT);
  Io_Write16(16, &REG_BLDALPHA);
  Io_Write16(31, &REG_BLDY);
  Dma_Fill32Cell(fill, 0, work, 28);
  if (object == NULL)
    object = ObjectTable_Get(gMainRoutineWork.currentOwner);
  work[6] = (u32)object;
}
