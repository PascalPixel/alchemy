/// @file
/// The blessings' burst showers.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

/// Where the burst, lightning or face pictures start in the sprite sheet.
#define BURST_SHOWER_PICTURES GLINT_SET_BYTES
/// VioletLightningSheetA's bolts (stored 72x64, drawn 62 tall) and
/// DemonFaceSheet's face.
#define VIOLET_LIGHTNING_WIDTH 72
#define VIOLET_LIGHTNING_HEIGHT 62
#define VIOLET_LIGHTNING_PICTURE_BYTES (VIOLET_LIGHTNING_WIDTH * 64)
#define DEMON_FACE_WIDTH 32
#define DEMON_FACE_HEIGHT 64
#define BURST_SHOWER_FRAMES 64     ///< frames the shower lasts
#define BURST_SHOWER_BURSTS 48     ///< one fired a frame from frame 0
#define BURST_SHOWER_BURST_LIFE 24 ///< frames each burst lives

/// The burst shower's styles, its mode argument.
enum BurstShowerMode {
  BURST_SHOWER_MODE_FLASH,
  BURST_SHOWER_MODE_ICE,       ///< unused: in the ice block palette
  BURST_SHOWER_MODE_SPRAY,     ///< in the water spray palette
  BURST_SHOWER_MODE_PINK,      ///< pink, fired from 16 pixels higher
  BURST_SHOWER_MODE_DJINNI,    ///< Mars Djinni palette, 28 pixels in front
  BURST_SHOWER_MODE_LIGHTNING, ///< no bursts: a violet lightning bolt
  BURST_SHOWER_MODE_GREY,      ///< grey; the targets stay solid black
  BURST_SHOWER_MODE_FACES,     ///< demon faces from 16 pixels in front
};

/// Bends BG2 line by line from the effect's table.
void BattleEffect_ArmBg2AffineHBlankDma(void) {
  struct BattleEffectState *state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  volatile u16 *channel = REG_DMA0;
  Dma_Stop(channel);
  Dma_Set(state->bg2xLines, &REG_BG2X, DMA_HBLANK_WORD_PER_LINE,
          (volatile u32 *)channel);
}

/// The user fires a stream of flash bursts at the targets, one a frame, each
/// towards a random point near a target.
///
/// Abilities: Fire Blessing.
void BattleEffect_RunFireBlessing(struct BattleEffectParams *args) {
  BattleEffect_RunBurstShower(args, BURST_SHOWER_MODE_FLASH);
}
/// The user fires a stream of bursts at the targets in the water-spray palette.
///
/// Abilities: Water Blessing.
void BattleEffect_RunWaterBlessing(struct BattleEffectParams *args) {
  BattleEffect_RunBurstShower(args, BURST_SHOWER_MODE_SPRAY);
}
/// The user fires a stream of grey bursts at the targets, which turn solid
/// black and stay so.
///
/// Abilities: Dark Blessing (the first two; the third plays Acid Blessing's).
void BattleEffect_RunDarkBlessing(struct BattleEffectParams *args) {
  BattleEffect_RunBurstShower(args, BURST_SHOWER_MODE_GREY);
}
/// The user fires a stream of pink bursts at the targets, from 16 pixels
/// higher than usual.
///
/// Abilities: Dark Blessing, Acid Blessing.
void BattleEffect_RunAcidBlessing(struct BattleEffectParams *args) {
  BattleEffect_RunBurstShower(args, BURST_SHOWER_MODE_PINK);
}
/// Violet lightning reaches from the user across to the targets instead of
/// bursts, on a canvas shown three times as wide, flickering through three
/// pictures, and the targets flash in turn without being knocked back.
///
/// Abilities: Storm Blessing.
void BattleEffect_RunStormBlessing(struct BattleEffectParams *args) {
  BattleEffect_RunBurstShower(args, BURST_SHOWER_MODE_LIGHTNING);
}
/// The user fires a stream of demon faces at the targets, from 16 pixels in
/// front of it.
///
/// Abilities: Evil Blessing.
void BattleEffect_RunEvilBlessing(struct BattleEffectParams *args) {
  BattleEffect_RunBurstShower(args, BURST_SHOWER_MODE_FACES);
}
/// The user fires a stream of Mars-coloured bursts at the targets, from 28
/// pixels in front of it.
///
/// Abilities: Deadly Gas.
void BattleEffect_RunDeadlyGas(struct BattleEffectParams *args) {
  BattleEffect_RunBurstShower(args, BURST_SHOWER_MODE_DJINNI);
}

/// The user fires one burst a frame at a point near one of the targets, taking
/// them in turn, and each target is hit in turn.
void BattleEffect_RunBurstShower(struct BattleEffectParams *args, s32 mode) {
  struct BattleEffectState *state;
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

  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  sprites = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  state->args = args;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;

  if (mode == BURST_SHOWER_MODE_FACES) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                        COMPOSITE_LIGHTEN);
    draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        COMPOSITE_LIGHTEN);
    draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
  } else {
    BattleEffect_FetchRectangleBlitters(state->args->side, draw);
  }

  Resource_LoadAndDecompress(RESOURCE_TORNADO_SHEET, state->sheet, 1, 0);
  if (mode == BURST_SHOWER_MODE_LIGHTNING) {
    Resource_LoadAndDecompress(RESOURCE_VIOLET_LIGHTNING_SHEET_A,
                               (state->sheet + BURST_SHOWER_PICTURES), 1, 1);
  } else if (mode == BURST_SHOWER_MODE_FACES) {
    Resource_LoadAndDecompress(RESOURCE_DEMON_FACE_SHEET,
                               (state->sheet + BURST_SHOWER_PICTURES), 1, 1);
  } else {
    Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET,
                               (state->sheet + BURST_SHOWER_PICTURES), 1, 1);
    Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sprites, 0, 0);
    // The grey mode: a ramp of greys, with blending off.
    if (mode == BURST_SHOWER_MODE_GREY) {
      for (i = 0, color = (u16 *)BG_PLTT; i != SHEET_PALETTE_COLORS; i++) {
        level = i / 4;
        *color = RGB(level, level, level);
        color++;
      }
      REG_BLDCNT = 0;
    } else {
      switch (mode) {
      case BURST_SHOWER_MODE_FLASH:
        paletteId = RESOURCE_FLASH_BURST_SHEET;
        break;
      case BURST_SHOWER_MODE_ICE:
        paletteId = RESOURCE_ICE_BLOCK_SHEET;
        break;
      case BURST_SHOWER_MODE_SPRAY:
        paletteId = RESOURCE_WATER_SPRAY_SHEET;
        break;
      case BURST_SHOWER_MODE_PINK:
        paletteId = RESOURCE_PINK_PALETTE;
        break;
      case BURST_SHOWER_MODE_DJINNI:
      default:
        paletteId = RESOURCE_MARS_DJINN_SHEET;
        break;
      }
      Iwram_CopyWords(BG_PLTT, Resource_GetTableEntry(paletteId),
                      SHEET_PALETTE_BYTES);
    }
  }

  if (mode == BURST_SHOWER_MODE_FACES) {
    EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  } else {
    EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE,
                        CANVAS_FADE_THREE_QUARTERS);
  }
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);

  for (i = 0; i != ARRAY_COUNT(state->particles); i++) {
    state->particles[i].variant = -1;
  }

  Projection_Sprite(state->args->user, &origin);
  if (mode == BURST_SHOWER_MODE_PINK) {
    origin.y -= 16;
  }
  if (mode == BURST_SHOWER_MODE_DJINNI) {
    if (state->args->side == EFFECT_SIDE_PARTY) {
      origin.x += 28;
    } else {
      origin.x -= 28;
    }
  }
  if (mode == BURST_SHOWER_MODE_FACES) {
    if (state->args->side == EFFECT_SIDE_PARTY) {
      origin.x += 16;
    } else {
      origin.x -= 16;
    }
  }
  if (mode == BURST_SHOWER_MODE_LIGHTNING) {
    origin.x = origin.x / 3;
    REG_BG2PA = Q_8_8(1.0 / 3); // three times the width
  }

  for (i = 0; i != state->args->targetCount; i++) {
    Projection_ScaledUnit(state->args->targets[i], &targetSpots[i]);
  }

  for (frame = 0; frame != BURST_SHOWER_FRAMES; frame++) {
    target = frame % state->args->targetCount;
    if (frame == 4) {
      Sound_PlayFar(SE_WHOOSH);
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

    // The lightning mode flickers its bolt from the user towards the targets;
    // the others fire bursts at them.
    if (mode == BURST_SHOWER_MODE_LIGHTNING) {
      if (state->args->side == EFFECT_SIDE_PARTY) {
        draw[0](canvas,
                (state->sheet + BURST_SHOWER_PICTURES) +
                    frame / 3 % 3 * VIOLET_LIGHTNING_PICTURE_BYTES,
                origin.x - 2, origin.y - 32, VIOLET_LIGHTNING_WIDTH,
                VIOLET_LIGHTNING_HEIGHT);
      } else {
        draw[0](canvas,
                (state->sheet + BURST_SHOWER_PICTURES) +
                    frame / 3 % 3 * VIOLET_LIGHTNING_PICTURE_BYTES,
                origin.x - (VIOLET_LIGHTNING_WIDTH - 2), origin.y - 32,
                VIOLET_LIGHTNING_WIDTH, VIOLET_LIGHTNING_HEIGHT);
      }
    } else {
      aim[0] = (targetSpots[target].x + (Random16() & 31)) - 16;
      aim[1] = (targetSpots[target].y + (Random16() & 63)) - 16;
      if (frame < BURST_SHOWER_BURSTS) {
        // A burst moves a 32nd of the way to its aim a frame for
        // BURST_SHOWER_BURST_LIFE frames: three quarters of the way down, and
        // across the half-width canvas half again as far, past the targets.
        state->particles[frame].x = origin.x * (toFIXED(1) / 2);
        state->particles[frame].y = INTtoFIXED(origin.y);
        state->particles[frame].velocityX =
            (aim[0] - origin.x) * (toFIXED(1) / 32);
        state->particles[frame].velocityY =
            (aim[1] - origin.y) * (toFIXED(1) / 32);
        state->particles[frame].variant = 0;
      }
    }

    // The bursts fly.
    for (i = 0; i != ARRAY_COUNT(state->particles); i++) {
      struct Particle *burst = &state->particles[i];
      if (burst->variant >= 0) {
        if (mode == BURST_SHOWER_MODE_FACES) {
          if (burst->variant > 5) {
            draw[state->args->side](
                canvas, (state->sheet + BURST_SHOWER_PICTURES),
                FIXEDtoINT(burst->x) - DEMON_FACE_WIDTH / 2,
                FIXEDtoINT(burst->y) - DEMON_FACE_HEIGHT / 2, DEMON_FACE_WIDTH,
                DEMON_FACE_HEIGHT);
          }
        } else if (mode == BURST_SHOWER_MODE_DJINNI) {
          if (burst->variant > 5) {
            draw[0](canvas,
                    (state->sheet + BURST_SHOWER_PICTURES) +
                        (burst->variant / 4) * PICTURE_BYTES(FLASH_BURST),
                    FIXEDtoINT(burst->x) - FLASH_BURST_WIDTH / 2,
                    FIXEDtoINT(burst->y) - FLASH_BURST_HEIGHT / 2,
                    FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
          }
        } else if (mode != BURST_SHOWER_MODE_LIGHTNING) {
          if (burst->variant > 1) {
            draw[0](canvas,
                    (state->sheet + BURST_SHOWER_PICTURES) +
                        (burst->variant / 4) * PICTURE_BYTES(FLASH_BURST),
                    FIXEDtoINT(burst->x) - FLASH_BURST_WIDTH / 2,
                    FIXEDtoINT(burst->y) - FLASH_BURST_HEIGHT / 2,
                    FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
          }
        }
        burst->x += burst->velocityX;
        burst->y += burst->velocityY;
        burst->variant++;
        if (burst->variant == BURST_SHOWER_BURST_LIFE) {
          burst->variant = -1;
        }
      }
    }

    // Each target is hit in turn.
    if (mode == BURST_SHOWER_MODE_LIGHTNING) {
      for (i = 0; i != state->args->targetCount; i++) {
        // The bolt strikes target i without knocking it back.
        if ((frame >= (i * 4) + 2) && ((frame & 7) == i)) {
          state->shakeFrames = 8;
          BattleEffect_TintBattler(state->args->targets[i], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, i, 4);
        }
      }
    } else {
      for (i = 0; i != state->args->targetCount; i++) {
        if ((frame >= (i * 4) + 16) && ((frame & 7) == i)) {
          state->shakeFrames = 8;
          if (mode == BURST_SHOWER_MODE_GREY) {
            BattleEffect_TintBattler(state->args->targets[i],
                                     SPRITE_COLOR_BLACK, POSE_DAMAGE, i, 4);
          } else {
            BattleEffect_TintBattler(state->args->targets[i],
                                     SPRITE_COLOR_WHITE, POSE_DAMAGE, i, 4);
          }
          BattleMotion_ApplyKnockbackFar(state->args->targets[i],
                                         KNOCKBACK_HURL);
        }
      }
    }

    BattleCamera_Shake(4, 4);
    // In the grey mode the targets stay black.
    if (mode != BURST_SHOWER_MODE_GREY) {
      BattleEffect_CountDownBattlerTints();
    }
    EFFECT_PRESENT_CANVAS(state);
  }

  EFFECT_FINISH_CANVAS();
}

#undef BURST_SHOWER_PICTURES
#undef VIOLET_LIGHTNING_PICTURE_BYTES
#undef BURST_SHOWER_BURSTS
#undef BURST_SHOWER_BURST_LIFE
#undef BURST_SHOWER_FRAMES
