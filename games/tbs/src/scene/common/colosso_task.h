#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "affine.h"
#include "vram_block.h"
#include "callback_scheduler.h"
#include "resource.h"
#include "tbs_edition.h"
#include "mem.h"
#include "field_event.h"
#include "field_service.h"
#include "dma.h"
#include "resource_ids.h"
#include "call.h"
#include "types.h"
#include "io_reg.h"
#include "io_write_queue.h"

/// The Colosso finals' three courses (Colosso Log, River and Wall): the large
/// lettering a race starts with. It reads "Match 1", "3", "2", "1", "Go!", from
/// the pictures of games/tbs/src/graphics/colosso/lettering.tsv, in this order:
/// 0 Lend a Hand, 1 to 3 Match 1 to 3, 4 Won and Lose, 5 the 3 and the 2,
/// 6 the 1 and 7 Go. Each picture is 0x400 bytes of tiles after the palettes.
///
/// The lettering moves by a script of 16-bit words, a command and its values:
enum ColossoLetteringCommand {
  /// x, speed: put the lettering at x pixels from the centre, moving at speed
  /// 256ths of a pixel a frame
  COLOSSO_LETTERING_PLACE = 0x4000,
  /// speed, frames: change the speed over that many frames
  COLOSSO_LETTERING_ACCELERATE = 0x3000,
  /// scale, frames: grow or shrink to scale (256 is its own size)
  COLOSSO_LETTERING_SCALE = 0x1000,
  /// weight, frames: fade to weight sixteenths over the screen
  COLOSSO_LETTERING_FADE = 0x2000,
  /// frames: run the moves for that many frames before the next command
  COLOSSO_LETTERING_WAIT = 0x7fff,
  /// remove the lettering and free its VRAM block
  COLOSSO_LETTERING_END = -1
};

static __inline__ void Dma_Wait(volatile u32 *dma) {
  while (dma[2] & 0x80000000)
    ;
}

struct Sprite {
  u32 words[3];
};

extern s16 gColosso_LetteringTimer;
extern s16 gColosso_ModeMoveTarget;
extern s16 gColosso_ModeMoveDuration;
extern s16 gColosso_LetteringMode;
extern s16 gColosso_LetteringParam;
extern s32 gColosso_LetteringPosition;
extern s16 *gColosso_LetteringScript;

/// Loads lettering picture id into the lettering's VRAM block (taken once) and
/// its palette into sprite palette 15. Picture 8 is picture 4 in another
/// palette.
static void Colosso_LoadLettering(s32 id) {
  u8 *buf;
  s32 off;

  buf = Runtime_BumpAllocateAlternatePool(0x1c00 + COLOSSO_PALETTE_BYTES);
  if (gColosso_LetteringSlot == -1)
    gColosso_LetteringSlot = Resource_FindFreeEntry();
  off = gColosso_LetteringPaletteOffsets[id];
  if (id == 8)
    id = 4;
  FieldResource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_Lettering),
                             buf);
  Dma_Set(buf + off, (void *)&OBJ_PLTT_COLOR(15, 0),
          (DMA_ENABLE | DMA_32BIT) << 16 | 8, REG_DMA3);
  Vram_Load(gColosso_LetteringSlot, 0x400,
            (u8 *)((id << 10) + (s32)buf + COLOSSO_PALETTE_BYTES));
  Dma_Wait(REG_DMA3);
  Runtime_BumpFree(buf);
}

/// The lettering's scene callback: run its script, move, scale and fade it, and
/// draw it as double-size affine sprites blended over the screen, in the shape
/// of gColosso_LetteringMode (gColosso_LetteringParam picks the second half of
/// a picture): 1: four 32x16 pieces in a row (Match 1 to 3, Lend a Hand); 2:
/// one 32x32 piece (3, 2, 1); 3: two 32x16 pieces (Won or Lose); 4: Go!, one
/// 64x32 piece, or two 32x16 in French.
static void Colosso_UpdateLettering(void) {
  u32 *write = gColosso_LetteringSprites;
  struct Sprite *sprite = (struct Sprite *)write;
  s32 tile = gVramBlockCache[gColosso_LetteringSlot].offset >> 5;
  u32 flags;
  s32 scale, blend, pos;
  s32 matrix, i, x, y, left;
  struct AffineTransform work;

commands:
  if (gColosso_LetteringTimer == 0) {
    {
      switch (*gColosso_LetteringScript++) {
      case COLOSSO_LETTERING_PLACE:
        gColosso_LetteringPosition = *gColosso_LetteringScript++ << 8;
        gColosso_ModeMoveTarget = *gColosso_LetteringScript++;
        gColosso_ModeMoveDuration = 0;
        break;
      case COLOSSO_LETTERING_ACCELERATE:
        gColosso_ModeMoveStart = gColosso_ModeMoveTarget;
        gColosso_ModeMoveTarget = *gColosso_LetteringScript++;
        gColosso_ModeMoveDuration = *gColosso_LetteringScript++;
        gColosso_ModeMoveStep = 0;
        break;
      case COLOSSO_LETTERING_SCALE:
        gColosso_ModeScaleStart = gColosso_ModeScaleTarget;
        gColosso_ModeScaleTarget = *gColosso_LetteringScript++;
        gColosso_ModeScaleDuration = *gColosso_LetteringScript++;
        gColosso_ModeScaleStep = 0;
        break;
      case COLOSSO_LETTERING_FADE:
        gColosso_ModeBlendStart = gColosso_ModeBlendTarget;
        gColosso_ModeBlendTarget = *gColosso_LetteringScript++;
        gColosso_ModeBlendDuration = *gColosso_LetteringScript++;
        gColosso_ModeBlendStep = 0;
        break;
      case COLOSSO_LETTERING_WAIT:
        gColosso_LetteringTimer = *gColosso_LetteringScript++;
        break;
      case COLOSSO_LETTERING_END:
        Engine_TaskRemoveCallback(Colosso_UpdateLettering);
        Resource_ResetEntry(gColosso_LetteringSlot);
        return;
      }
    }
    goto commands;
  }
  gColosso_LetteringTimer--;
  if (gColosso_ModeScaleDuration == 0) {
    scale = gColosso_ModeScaleTarget;
  } else {
    s32 duration, start, progress, target;
    duration = gColosso_ModeScaleDuration;
    start = gColosso_ModeScaleStart;
    target = gColosso_ModeScaleTarget;
    progress = ++gColosso_ModeScaleStep;
    scale = start + (target - start) * progress / duration;
    if (progress >= duration)
      gColosso_ModeScaleDuration = 0;
  }
  if (gColosso_ModeBlendDuration == 0) {
    blend = gColosso_ModeBlendTarget;
  } else {
    s32 duration, start, progress, target;
    duration = gColosso_ModeBlendDuration;
    start = gColosso_ModeBlendStart;
    target = gColosso_ModeBlendTarget;
    progress = ++gColosso_ModeBlendStep;
    blend = start + (target - start) * progress / duration;
    if (progress >= duration)
      gColosso_ModeBlendDuration = 0;
  }
  if (gColosso_ModeMoveDuration == 0) {
    pos = gColosso_ModeMoveTarget;
  } else {
    s32 duration, start, progress, target;
    duration = gColosso_ModeMoveDuration;
    start = gColosso_ModeMoveStart;
    target = gColosso_ModeMoveTarget;
    progress = ++gColosso_ModeMoveStep;
    pos = start + (target - start) * progress / duration;
    if (progress >= duration)
      gColosso_ModeMoveDuration = 0;
  }
  work.angle = 0;
  work.scaleX = scale;
  work.scaleY = scale;
  matrix = (s16)AffineMatrix_BuildForEffect(&work);
  gColosso_LetteringPosition += pos;
  pos = gColosso_LetteringPosition / 256;
  switch (gColosso_LetteringMode) {
  case 1: {
    u32 attr;
    i = 0;
    y = 56;
    flags = 0x80004000;
    attr = matrix << 25;
    for (; i < 4; i++) {
      x = pos + scale * (i * 32 - 48) / 256;
      left = x + 88;
      if ((u32)(x + 152) < 304) {
        x = left & 511;
        *write++ = 0;
        *write++ = (x << 16) | y | flags | attr | 0x700;
        *write++ = 0xf400 | tile;
        Runtime_PushSlotEntry(sprite++, 236);
      }
      tile += 8;
    }
    break;
  }
  case 3: {
    u32 attr;
    i = 0;
    y = 48;
    flags = 0x80004000;
    attr = matrix << 25;
    for (; i < 2; i++) {
      x = pos + scale * (i * 32 - 16) / 256;
      left = x + 88;
      if ((u32)(x + 152) < 304) {
        x = left & 511;
        *write++ = 0;
        *write++ = (x << 16) | y | flags | attr | 0x700;
        *write++ = 0xf400 | (tile + gColosso_LetteringParam);
        Runtime_PushSlotEntry(sprite++, 236);
      }
      tile += 8;
    }
    break;
  }
#if LANGUAGE == LANGUAGE_FRENCH
  case 4: {
    u32 attr;
    i = 0;
    y = 64;
    flags = 0x80004000;
    attr = matrix << 25;
    for (; i < 2; i++) {
      x = pos + scale * (i * 32 - 16) / 256;
      left = x + 88;
      if ((u32)(x + 152) < 304) {
        x = left & 511;
        *write++ = 0;
        *write++ = (x << 16) | y | flags | attr | 0x700;
        *write++ = 0xf400 | (tile + gColosso_LetteringParam);
        Runtime_PushSlotEntry(sprite++, 236);
      }
      tile += 8;
    }
    break;
  }
#else
  case 4:
    y = 48;
    flags = 0xc0004000;
    left = pos + 56;
    if ((u32)(pos + 120) < 304) {
      x = left & 511;
      *write++ = 0;
      *write++ = (x << 16) | y | flags | (matrix << 25) | 0x700;
      *write++ = 0xf400 | (tile + gColosso_LetteringParam);
      Runtime_PushSlotEntry(sprite, 236);
    }
    break;
#endif
  case 2:
    y = 48;
    flags = 0x80000000;
    left = pos + 88;
    if ((u32)(pos + 152) < 304) {
      x = left & 511;
      *write++ = 0;
      *write++ = (x << 16) | y | flags | (matrix << 25) | 0x700;
      *write++ = 0xf400 | (tile + gColosso_LetteringParam);
      Runtime_PushSlotEntry(sprite, 236);
    }
    break;
  }
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT, 0x3f00, 0x20000);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, ((16 - blend) << 8) | blend, 0x20000);
}
