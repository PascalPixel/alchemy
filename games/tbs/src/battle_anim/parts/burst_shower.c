/// @file
/// The blessings' burst showers.
// Part of battle_anim.c.

#include "../battle_anim_internal.h"

/// Where the burst, lightning or face pictures start in the sheet: after the
/// six glints that open the tornado sheet, over its tornado strips.
#define BURST_SHOWER_CELLS 0xC56
#define BURST_SHOWER_BURSTS 48     ///< one fired a frame from frame 0
#define BURST_SHOWER_BURST_LIFE 24 ///< frames each burst lives

/// The burst shower's styles, its mode argument.
enum BurstShowerMode {
  BURST_SHOWER_MODE_FLASH,     ///< flash bursts
  BURST_SHOWER_MODE_ICE,       ///< in the ice block palette
  BURST_SHOWER_MODE_SPRAY,     ///< in the water spray palette
  BURST_SHOWER_MODE_PINK,      ///< pink, fired from 16 pixels higher
  BURST_SHOWER_MODE_DJINNI,    ///< Mars Djinni palette, 28 pixels in front
  BURST_SHOWER_MODE_LIGHTNING, ///< no bursts: a violet lightning bolt
  BURST_SHOWER_MODE_GREY,      ///< grey; the targets stay solid black
  BURST_SHOWER_MODE_FACES,     ///< demon faces from 16 pixels in front
};

/// Arms DMA 0 to copy one word of gBattleAnimWork's bg2xLines table into BG2X
/// at each horizontal blank, bending BG2 line by line.
///
/// Runs each frame as a task added by the target motes, the
/// casting impacts, Condemn, the gathering lights, Torch and Planet Diver,
/// each of which removes it before it returns. Stops DMA 0 and restarts it
/// from the top of the table.
void BattleAnim_ArmBg2AffineHBlankDma(void) {
  struct BattleAnimWork *work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_ANIM);
  volatile u16 *channel = REG_DMA0;
  Dma_Stop(channel);
  Dma_Set(work->bg2xLines, (void *)&REG_BG2X, DMA_HBLANK_WORD_PER_LINE,
          (volatile u32 *)channel);
}

/// The user fires a stream of flash bursts at the targets, one a frame, each
/// towards a random point near a target.
///
/// Abilities: Fire Blessing (BATTLE_ANIM_FIRE_BLESSING).
void BattleAnim_RunFireBlessing(struct BattleAnimArgs *effect) {
  BattleAnim_RunBurstShower(effect, BURST_SHOWER_MODE_FLASH);
}
/// The user fires a stream of bursts at the targets in the water-spray palette.
///
/// Abilities: Water Blessing (BATTLE_ANIM_WATER_BLESSING).
void BattleAnim_RunWaterBlessing(struct BattleAnimArgs *effect) {
  BattleAnim_RunBurstShower(effect, BURST_SHOWER_MODE_SPRAY);
}
/// The user fires a stream of grey bursts at the targets, which turn solid
/// black and stay so.
///
/// Abilities: Dark Blessing (BATTLE_ANIM_DARK_BLESSING).
void BattleAnim_RunDarkBlessing(struct BattleAnimArgs *effect) {
  BattleAnim_RunBurstShower(effect, BURST_SHOWER_MODE_GREY);
}
/// The user fires a stream of pink bursts at the targets, from 16 pixels
/// higher than usual.
///
/// Abilities: Dark Blessing, Acid Blessing (BATTLE_ANIM_ACID_BLESSING).
void BattleAnim_RunAcidBlessing(struct BattleAnimArgs *effect) {
  BattleAnim_RunBurstShower(effect, BURST_SHOWER_MODE_PINK);
}
/// Shows violet lightning between the user and targets instead of bursts: a
/// bolt flickers through three pictures in front of the user, and the targets
/// flash in turn without being knocked back.
///
/// Abilities: Storm Blessing (BATTLE_ANIM_STORM_BLESSING).
void BattleAnim_RunStormBlessing(struct BattleAnimArgs *effect) {
  BattleAnim_RunBurstShower(effect, BURST_SHOWER_MODE_LIGHTNING);
}
/// The user fires a stream of demon faces at the targets, from 16 pixels in
/// front of it.
///
/// Abilities: Evil Blessing (BATTLE_ANIM_EVIL_BLESSING).
void BattleAnim_RunEvilBlessing(struct BattleAnimArgs *effect) {
  BattleAnim_RunBurstShower(effect, BURST_SHOWER_MODE_FACES);
}
/// The user fires a stream of Mars-coloured bursts at the targets, from 28
/// pixels in front of it.
///
/// Abilities: Deadly Gas (BATTLE_ANIM_DEADLY_GAS).
void BattleAnim_RunDeadlyGas(struct BattleAnimArgs *effect) {
  BattleAnim_RunBurstShower(effect, BURST_SHOWER_MODE_DJINNI);
}
#define BURST_SHOWER_FRAMES 64

/// For 48 frames the user fires one burst a frame at a random point near one of
/// the targets, taking them in turn, and each target is hit every 8 frames.
/// Each burst lives 24 frames. A sound plays on frame 4 and the battle shows
/// the damage on frame 24 (frame 60 in the grey mode).
///
/// Called by the seven wrappers above, six blessings and Deadly Gas. Each
/// burst is one of work->particles, its variant counting the frames it has
/// lived (-1 when free). Writes REG_BLDALPHA, the first 64 BG colours (all but
/// the lightning and the faces), REG_BLDCNT in the grey mode and REG_BG2PA in
/// the lightning mode; sets work->shakeFrames on each hit.
///
/// effect: the user and the targets.
/// mode: the burst and its palette (enum BurstShowerMode). The grey mode turns
///       the targets solid black and keeps them so. BURST_SHOWER_MODE_LIGHTNING
///       fires no bursts: a violet lightning bolt stands between the user and
///       the targets, on a canvas shown at three times its width. No animation
///       kind in the animation table uses BURST_SHOWER_MODE_ICE.
void BattleAnim_RunBurstShower(struct BattleAnimArgs *effect, s32 mode) {
  u32 *slots;
  struct BattleAnimWork *work;
  void *canvas;
  u8 *sprites;
  BlitterPair draw;
  struct ScreenPoint origin;
  struct ScreenPoint targetSpots[8];
  s32 aim[3];
  u16 *color;
  s32 frame;
  s32 i;
  s32 target;
  s32 level;
  s32 paletteId;
  s32 width;
  s32 height;

  // The slots are read from the canvas's, not the work block's.
  slots = (u32 *)&gHeapSlots[HEAP_SLOT_BATTLE_CANVAS];
  canvas = (void *)slots[0];
  work = (struct BattleAnimWork *)CANVAS_SLOT(slots, HEAP_SLOT_BATTLE_ANIM);
  sprites = (u8 *)CANVAS_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  BattleAnim_StartCanvasLayer(0);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);

  // The faces are drawn facing each way by side; the others take the usual
  // pair for the side.
  if (mode == BURST_SHOWER_MODE_FACES) {
    ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
    draw[0] = (DrawRectangle)CANVAS_SLOT(slots, HEAP_SLOT_RAM_CODE);
    ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, ANIM_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                      COMPOSITE_LIGHTEN);
    draw[1] = (DrawRectangle)CANVAS_SLOT(slots, HEAP_SLOT_RAM_CODE_2);
  } else {
    BattleAnim_FetchRectangleBlitters(work->effect->side, draw);
  }

  // The pictures and the palette. The tornado sheet is loaded first in every
  // mode, but nothing here draws from it: the burst pictures are loaded over
  // its tornado strips, after the six glints that open it.
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet, 1, 0);
  if (mode == BURST_SHOWER_MODE_LIGHTNING) {
    Resource_LoadAndDecompress((s32)&ResourceId_VioletLightningSheetA,
                               (work->sheet + BURST_SHOWER_CELLS), 1, 1);
  } else if (mode == BURST_SHOWER_MODE_FACES) {
    Resource_LoadAndDecompress((s32)&ResourceId_DemonFaceSheet,
                               (work->sheet + BURST_SHOWER_CELLS), 1, 1);
  } else {
    Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                               (work->sheet + BURST_SHOWER_CELLS), 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0,
                               0);
    // The grey mode: a ramp of 16 greys, four entries each, up to level 15
    // of 31, about half brightness, with blending off.
    if (mode == BURST_SHOWER_MODE_GREY) {
      for (i = 0, color = (u16 *)BG_PLTT; i != 64; i++) {
        level = i / 4;
        *color = ((level << 10) | (level << 5)) | level;
        color++;
      }
      REG_BLDCNT = 0;
    } else {
      switch (mode) {
      case BURST_SHOWER_MODE_FLASH:
        paletteId = (s32)&ResourceId_FlashBurstSheet;
        break;
      case BURST_SHOWER_MODE_ICE:
        paletteId = (s32)&ResourceId_IceBlockSheet;
        break;
      case BURST_SHOWER_MODE_SPRAY:
        paletteId = (s32)&ResourceId_WaterSpraySheet;
        break;
      case BURST_SHOWER_MODE_PINK:
        paletteId = (s32)&ResourceId_PinkPalette;
        break;
      case BURST_SHOWER_MODE_DJINNI:
      default:
        paletteId = (s32)&ResourceId_MarsDjinnSheet;
        break;
      }
      Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(paletteId), 128);
    }
  }

  // The faces leave shorter trails than the bursts.
  if (mode == BURST_SHOWER_MODE_FACES) {
    ANIM_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);
  } else {
    ANIM_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);
  }
  Task_Add(BattleAnim_ProcessPendingGraphicsTransfer, ANIM_CALLBACK_ORDER);

  for (i = 0; i != 64; i++) {
    work->particles[i].variant = -1;
  }

  // Where the bursts start: the user, moved for some modes. The lightning's
  // canvas is shown at three times its width, so its x is a third.
  Projection_Sprite(work->effect->user, &origin);
  if (mode == BURST_SHOWER_MODE_PINK) {
    origin.y -= 16;
  }
  if (mode == BURST_SHOWER_MODE_DJINNI) {
    if (work->effect->side == 1) {
      origin.x += 28;
    } else {
      origin.x -= 28;
    }
  }
  if (mode == BURST_SHOWER_MODE_FACES) {
    if (work->effect->side == 1) {
      origin.x += 16;
    } else {
      origin.x -= 16;
    }
  }
  if (mode == BURST_SHOWER_MODE_LIGHTNING) {
    origin.x = origin.x / 3;
    REG_BG2PA = Q_8_8(1.0 / 3);
  }

  // Where each target stands.
  i = 0;
  while (i != work->effect->targetCount) {
    Projection_ScaledUnit(work->effect->targets[i], &targetSpots[i]);
    i += 1;
  }

  frame = 0;
  do {
    // The bursts aim at the targets in turn, one a frame.
    target = frame % work->effect->targetCount;
    if (frame == 4) {
      Sound_PlaySongFar(SE_WHOOSH);
    }
    if (mode != BURST_SHOWER_MODE_GREY) {
      if (frame == 24) {
        ANIM_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      }
    } else {
      if (frame == 60) {
        ANIM_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      }
    }

    // The lightning mode draws the lightning from the user, stepping through
    // its three 72x62 pictures every 3 frames (0x1200 bytes apart); the others
    // fire one burst a frame for 48 frames at a random point near a target.
    if (mode == BURST_SHOWER_MODE_LIGHTNING) {
      if (work->effect->side == 1) {
        draw[0](canvas,
                (work->sheet + BURST_SHOWER_CELLS) + ((frame / 3 % 3 * 9) << 9),
                origin.x - 2, origin.y - 32, 72, 62);
      } else {
        draw[0](canvas,
                (work->sheet + BURST_SHOWER_CELLS) + ((frame / 3 % 3 * 9) << 9),
                origin.x - 70, origin.y - 32, 72, 62);
      }
    } else {
      // The point is picked every frame, fired or not, as it draws two
      // random numbers.
      aim[0] = (targetSpots[target].x + (Random16() & 31)) - 16;
      aim[1] = (targetSpots[target].y + (Random16() & 63)) - 16;
      if (frame <= BURST_SHOWER_BURSTS - 1) {
        // The canvas is half the screen's width, so x is halved, but the
        // velocity is not: each frame the burst moves 1/32 of the way in
        // screen pixels, so it covers its x distance on the canvas in 16
        // frames, when the hits start, and only three quarters of its y
        // distance in its 24 frames.
        work->particles[frame].x = origin.x << 15;
        work->particles[frame].y = origin.y << 16;
        work->particles[frame].velocityX = (aim[0] - origin.x) << 11;
        work->particles[frame].velocityY = (aim[1] - origin.y) << 11;
        work->particles[frame].variant = 0;
      }
    }

    // Move and draw the bursts; each lasts 24 frames and steps through the
    // 32x64 pictures (0x800 bytes apart) every 4. The faces have one picture,
    // mirrored on side 1.
    i = 0;
    width = 32;
    height = 64;
    do {
      struct Particle *spark = &work->particles[i];
      // A burst shows from its third frame, or its seventh for the faces
      // and the Mars Djinni's bursts.
      if (spark->variant >= 0) {
        if (mode == BURST_SHOWER_MODE_FACES) {
          if (spark->variant > 5) {
            draw[work->effect->side](canvas, (work->sheet + BURST_SHOWER_CELLS),
                                     Q_16_16_TO_INT(spark->x) - 16,
                                     Q_16_16_TO_INT(spark->y) - 32, width,
                                     height);
          }
        } else if (mode == BURST_SHOWER_MODE_DJINNI) {
          if (spark->variant > 5) {
            draw[0](canvas,
                    (work->sheet + BURST_SHOWER_CELLS) +
                        ((spark->variant / 4) << 11),
                    Q_16_16_TO_INT(spark->x) - 16,
                    Q_16_16_TO_INT(spark->y) - 32, width, height);
          }
        } else if (mode != BURST_SHOWER_MODE_LIGHTNING) {
          if (spark->variant > 1) {
            draw[0](canvas,
                    (work->sheet + BURST_SHOWER_CELLS) +
                        ((spark->variant / 4) << 11),
                    Q_16_16_TO_INT(spark->x) - 16,
                    Q_16_16_TO_INT(spark->y) - 32, width, height);
          }
        }
        spark->x += spark->velocityX;
        spark->y += spark->velocityY;
        spark->variant += 1;
        if (spark->variant == BURST_SHOWER_BURST_LIFE) {
          spark->variant = -1;
        }
      }
      i += 1;
    } while (i != 64);

    // Hit each target every 8 frames, once its turn has come: target i on
    // frames whose low three bits are i.
    if (mode == BURST_SHOWER_MODE_LIGHTNING) {
      i = 0;
      while (i != work->effect->targetCount) {
        // The bolt strikes target i from frame i * 4 + 2 and does not knock
        // it back.
        if ((frame >= (i * 4) + 2) && ((frame & 7) == i)) {
          work->shakeFrames = 8;
          BattleAnim_TintBattler(work->effect->targets[i], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, i, 4);
        }
        i += 1;
      }
    } else {
      i = 0;
      while (i != work->effect->targetCount) {
        // The bursts reach target i from frame i * 4 + 16.
        if ((frame >= (i * 4) + 16) && ((frame & 7) == i)) {
          work->shakeFrames = 8;
          if (mode == BURST_SHOWER_MODE_GREY) {
            BattleAnim_TintBattler(work->effect->targets[i], SPRITE_COLOR_BLACK,
                                   POSE_DAMAGE, i, 4);
          } else {
            BattleAnim_TintBattler(work->effect->targets[i], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, i, 4);
          }
          BattleMotion_ApplyKnockbackFar(work->effect->targets[i],
                                         KNOCKBACK_HURL);
        }
        i += 1;
      }
    }

    BattleCamera_Shake(4, 4);
    // In the grey mode the flash timers never run down, so the targets stay
    // black.
    if (mode != BURST_SHOWER_MODE_GREY) {
      BattleAnim_TickBattlerTints();
    }
    ANIM_PRESENT_FRAME(work);
    frame += 1;
  } while (frame != BURST_SHOWER_FRAMES);

  ANIM_FINISH_CANVAS();
}

#undef BURST_SHOWER_CELLS
#undef BURST_SHOWER_BURSTS
#undef BURST_SHOWER_BURST_LIFE
#undef BURST_SHOWER_FRAMES
