/// @file
/// The blessings' burst showers.
/// Part of effect.c, the battle effect object, compiled only through it.

#include "../effect_internal.h"

// The burst shower fires one burst a frame from the user at random points
// near the targets, and hits each target every 8 frames. One mode draws a
// lightning bolt instead. Each burst is one particle slot, its variant
// counting the 24 frames it lives (-1 when free).

/// The burst shower's second picture in the sheet, after the tornado.
#define BURST_SHOWER_CELLS 0xC56
#define BURST_SHOWER_BURSTS 48     ///< one fired a frame from frame 0
#define BURST_SHOWER_BURST_LIFE 24 ///< frames each burst lives

/// The burst shower's styles, its mode argument.
enum BurstShowerMode {
  BURST_SHOWER_MODE_FLASH,     ///< flash bursts
  BURST_SHOWER_MODE_ICE,       ///< in the ice block palette
  BURST_SHOWER_MODE_SPRAY,     ///< in the water spray palette
  BURST_SHOWER_MODE_PINK,      ///< pink, fired from 16 pixels lower
  BURST_SHOWER_MODE_DJINNI,    ///< Mars djinni palette, 28 pixels in front
  BURST_SHOWER_MODE_LIGHTNING, ///< no bursts: a violet lightning bolt
  BURST_SHOWER_MODE_GREY,      ///< grey; the targets stay solid black
  BURST_SHOWER_MODE_FACES,     ///< demon faces from 16 pixels in front
};

/// Arms DMA 0 to copy one word of gBattleFxWork's bg2xLines table into BG2X at
/// each horizontal blank, bending BG2 line by line.
void BattleFx_ArmBg2AffineHBlankDma(void) {
  struct BattleEffectWork *work = WORK_SLOT(HEAP_SLOT_BATTLE_EFFECT);
  volatile u16 *channel = REG_DMA0;
  Dma_Stop(channel);
  Dma_Set(work->bg2xLines, (void *)&REG_BG2X, DMA_HBLANK_WORD_PER_LINE,
          (volatile u32 *)channel);
}

/// Abilities: Fire Blessing (kind 400).
///
/// The user fires a stream of flash bursts at the targets, one a frame,
/// each towards a random point near a target.
void BattleFx_RunFireBlessing(struct BattleEffectArgument *effect) {
  BattleFx_RunBurstShower(effect, BURST_SHOWER_MODE_FLASH);
}
/// Abilities: Water Blessing (kind 401).
///
/// The user fires a stream of bursts at the targets in the water-spray
/// palette.
void BattleFx_RunWaterBlessing(struct BattleEffectArgument *effect) {
  BattleFx_RunBurstShower(effect, BURST_SHOWER_MODE_SPRAY);
}
/// Abilities: Dark Blessing (kind 403).
///
/// The user fires a stream of grey bursts at the targets, which turn solid
/// black and stay so.
void BattleFx_RunDarkBlessing(struct BattleEffectArgument *effect) {
  BattleFx_RunBurstShower(effect, BURST_SHOWER_MODE_GREY);
}
/// Abilities: Dark Blessing, Acid Blessing (kind 404).
///
/// Fire pink bursts from 16 pixels below the normal origin.
void BattleFx_RunAcidBlessing(struct BattleEffectArgument *effect) {
  BattleFx_RunBurstShower(effect, BURST_SHOWER_MODE_PINK);
}
/// Abilities: Storm Blessing (kind 405).
///
/// Show violet lightning between the user and targets instead of bursts: a
/// bolt flickers through three pictures in front of the user, and the
/// targets flash in turn without being knocked back.
void BattleFx_RunStormBlessing(struct BattleEffectArgument *effect) {
  BattleFx_RunBurstShower(effect, BURST_SHOWER_MODE_LIGHTNING);
}
/// Abilities: Evil Blessing (kind 406).
///
/// The user fires a stream of demon faces at the targets, from 16 pixels in
/// front of it.
void BattleFx_RunEvilBlessing(struct BattleEffectArgument *effect) {
  BattleFx_RunBurstShower(effect, BURST_SHOWER_MODE_FACES);
}
/// Abilities: Deadly Gas (kind 407).
///
/// The user fires a stream of Mars-coloured bursts at the targets, from 28
/// pixels in front of it.
void BattleFx_RunDeadlyGas(struct BattleEffectArgument *effect) {
  BattleFx_RunBurstShower(effect, BURST_SHOWER_MODE_DJINNI);
}
#define BURST_SHOWER_FRAMES 64

/// Burst shower: for 48 frames the user fires one burst a frame at a random
/// point near one of the targets, taking them in turn, and each target is
/// hit every 8 frames. Each burst lives 24 frames. A sound plays on frame 4
/// and the battle shows the damage on frame 24 (frame 60 in the grey mode).
///
/// effect: the user and the targets.
/// mode: the burst and its palette: 0 flash bursts, 1 in the ice block
///       palette, 2 in the water spray palette, 3 pink and fired from 16
///       pixels lower, 4 in the Mars djinni palette from 28 pixels in
///       front, 6 grey (the targets turn solid black and stay so), 7 demon
///       faces from 16 pixels in front. Mode 5 fires no bursts: a violet
///       lightning bolt stands between the user and the targets, on a
///       canvas shown at three times its width. No kind in the effect
///       table uses mode 1.
void BattleFx_RunBurstShower(struct BattleEffectArgument *effect, s32 mode) {
  u32 *slots;
  struct BattleEffectWork *work;
  void *canvas;
  u8 *sprites;
  BlitterPair draw;
  struct EffectPosition origin;
  struct EffectPosition targetSpots[8];
  s32 aim[3];
  u16 *color;
  s32 frame;
  s32 i;
  s32 target;
  s32 level;
  s32 paletteId;
  s32 width;
  s32 height;

  slots = (u32 *)&gWorkSlot[HEAP_SLOT_BATTLE_CANVAS];
  canvas = (void *)slots[0];
  work = (struct BattleEffectWork *)CANVAS_SLOT(slots, HEAP_SLOT_BATTLE_EFFECT);
  sprites = (u8 *)CANVAS_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);

  if (mode == BURST_SHOWER_MODE_FACES) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
    draw[0] = (DrawRectangle)CANVAS_SLOT(slots, HEAP_SLOT_BLITTER);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_MAX);
    draw[1] = (DrawRectangle)CANVAS_SLOT(slots, HEAP_SLOT_BLITTER_ALTERNATE);
  } else {
    BattleFx_FetchRectangleBlitters(work->effect->side, draw);
  }

  // The pictures and the palette.
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
    // The grey mode: a ramp of 16 greys, four entries each, with blending
    // off.
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

  if (mode == BURST_SHOWER_MODE_FACES) {
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);
  } else {
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);
  }
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer,
                                EFFECT_CALLBACK_ORDER);

  // Where the bursts start: the user, moved for some modes.
  for (i = 0; i != 64; i++) {
    work->particles[i].variant = -1;
  }

  EffectPosition_ProjectSprite(work->effect->actor, &origin);
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
  while (i != work->effect->count) {
    EffectPosition_ProjectScaledBattler(work->effect->actors[i],
                                        &targetSpots[i]);
    i += 1;
  }

  frame = 0;
  do {
    target = frame % work->effect->count;
    if (frame == 4) {
      AudioCommand_PlayFar(SE_136);
    }
    if (mode != BURST_SHOWER_MODE_GREY) {
      if (frame == 24) {
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      }
    } else {
      if (frame == 60) {
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      }
    }

    // Mode 5 draws the lightning from the user; the others fire one burst
    // a frame for 48 frames at a random point near a target.
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
        // The canvas is half the screen's width, so x is halved. The burst
        // reaches its point in 32 frames.
        work->particles[frame].x = origin.x << 15;
        work->particles[frame].y = origin.y << 16;
        work->particles[frame].velocityX = (aim[0] - origin.x) << 11;
        work->particles[frame].velocityY = (aim[1] - origin.y) << 11;
        work->particles[frame].variant = 0;
      }
    }

    // Move and draw the bursts; each lasts 24 frames.
    i = 0;
    width = 32;
    height = 64;
    do {
      struct EffectStep *spark = &work->particles[i];
      // A burst shows from its third frame, or its seventh for the faces
      // and the djinni.
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

    // Hit each target every 8 frames, once its turn has come.
    if (mode == BURST_SHOWER_MODE_LIGHTNING) {
      i = 0;
      while (i != work->effect->count) {
        // The bolt strikes target i from frame i * 4 + 2 and does not knock
        // it back.
        if ((frame >= (i * 4) + 2) && ((frame & 7) == i)) {
          work->shakeFrames = 8;
          ObjectGroup_UpdateMembers(work->effect->actors[i], SPRITE_COLOR_WHITE,
                                    POSE_DAMAGE, i, 4);
        }
        i += 1;
      }
    } else {
      i = 0;
      while (i != work->effect->count) {
        // The bursts reach target i from frame i * 4 + 16.
        if ((frame >= (i * 4) + 16) && ((frame & 7) == i)) {
          work->shakeFrames = 8;
          if (mode == BURST_SHOWER_MODE_GREY) {
            ObjectGroup_UpdateMembers(work->effect->actors[i],
                                      SPRITE_COLOR_BLACK, POSE_DAMAGE, i, 4);
          } else {
            ObjectGroup_UpdateMembers(work->effect->actors[i],
                                      SPRITE_COLOR_WHITE, POSE_DAMAGE, i, 4);
          }
          BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 4);
        }
        i += 1;
      }
    }

    Camera_ApplyShake(4, 4);
    if (mode != BURST_SHOWER_MODE_GREY) {
      ObjectGroup_TickMemberTimers();
    }
    EFFECT_PRESENT_FRAME(work);
    frame += 1;
  } while (frame != BURST_SHOWER_FRAMES);

  EFFECT_FINISH_CANVAS();
}

#undef BURST_SHOWER_CELLS
#undef BURST_SHOWER_BURSTS
#undef BURST_SHOWER_BURST_LIFE
#undef BURST_SHOWER_FRAMES
