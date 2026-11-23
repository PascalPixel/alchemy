/// @file
/// The burst shower: a stream of shots from the user at the targets, which
/// most monster breaths and a few Djinn play, each with its own picture.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// The burst shower's styles, its mode argument
/// (BattleEffect_RunBurstShower): The Broken Seal's eight, then this
/// game's, each named for the effect that plays it.
enum BurstShowerMode {
  BURST_SHOWER_MODE_FLASH,
  BURST_SHOWER_MODE_ICE,
  BURST_SHOWER_MODE_SPRAY,
  BURST_SHOWER_MODE_PINK,
  BURST_SHOWER_MODE_DJINNI,
  BURST_SHOWER_MODE_LIGHTNING,
  BURST_SHOWER_MODE_GREY,
  BURST_SHOWER_MODE_FACES,
  BURST_SHOWER_MODE_PETRA,
  BURST_SHOWER_MODE_FUGUE,
  BURST_SHOWER_MODE_AGING_GAS,
  BURST_SHOWER_MODE_FIRE_DANCE,
  BURST_SHOWER_MODE_TOXIC_BREATH,
  BURST_SHOWER_MODE_SAND_BREATH,
  BURST_SHOWER_MODE_DESERT_GASP,
  BURST_SHOWER_MODE_BLACK_ICE,
  BURST_SHOWER_MODE_TYPHOON_BLOW,
  BURST_SHOWER_MODE_DJINNFEST,
  BURST_SHOWER_MODE_HEAT_KISS,
  BURST_SHOWER_MODE_FIRE_DANCE_ENEMY,
  BURST_SHOWER_MODE_GRAVEL_BLOW,
  BURST_SHOWER_MODE_DARKSOL_GASP,
  BURST_SHOWER_MODE_FURY,
  BURST_SHOWER_MODE_STORM_BREATH_STRONG,
  BURST_SHOWER_MODE_CHAR
};

/// Where the burst shower keeps its pictures in the effect's sprite sheet: the
/// notes, hearts or smoke of the modes that load them first, at 0.
enum BurstShowerSheet {
  BURST_SHOWER_BUBBLE = 4096,  ///< the bubble, each row written twice
  BURST_SHOWER_PICTURE = 3158, ///< the mode's bursts, faces, bolts or rocks
  /// Gravel Blow's rock shards, as gSpire_ShardPictures counts from them.
  BURST_SHOWER_ROCK_SHARDS = 5266,
  BURST_SHOWER_DJINNI = 26048 ///< the prelude's small Djinni
};

/// VioletLightningSheetA's bolts, one every 72x64 bytes, drawn 62 tall;
/// MusicNoteSheet's three 32x32 notes; BlueBeamSheet's 24x24 bubble, each row
/// written twice so that it draws 48 tall, round at the canvas's double width.
#define VIOLET_LIGHTNING_WIDTH 72
#define VIOLET_LIGHTNING_HEIGHT 62
#define VIOLET_LIGHTNING_BYTES (VIOLET_LIGHTNING_WIDTH * 64)
#define MUSIC_NOTE_WIDTH 32
#define MUSIC_NOTE_HEIGHT 32
#define MUSIC_NOTE_COUNT 3
/// DemonFaceSheet's face.
#define DEMON_FACE_WIDTH 32
#define DEMON_FACE_HEIGHT 64
/// The levels the music notes are darkened by; a drawn pixel keeps level 1.
#define MUSIC_NOTE_DIMMING 16
/// The holes punched at random in each of the six burst or smoke pictures of
/// the sand, ice and Char modes, more in each later one.
#define FLASH_BURST_HOLES(picture) ((picture) * 512 + 1024)
#define SMOKE_PUFF_HOLES(picture) ((picture) * 256 + 512)

#define BURST_SHOWER_BURSTS 48     ///< one fired a frame from frame 0
#define BURST_SHOWER_BURST_LIFE 24 ///< frames each shot lives
#define BURST_SHOWER_NOTE_LIFE 32  ///< frames each music note lives
/// Aging Gas fires AGING_GAS_BURSTS shots, and AGING_GAS_MOTES motes burst
/// from the first target, of which the first AGING_GAS_DRAWN_MOTES are drawn
/// into the user.
#define AGING_GAS_BURSTS 16
#define AGING_GAS_MOTES 128
#define AGING_GAS_DRAWN_MOTES 24

/// Restarts DMA channel 0 as an H-blank transfer of the effect work's line
/// table into BG2X, one word a line.
void BattleEffect_ArmBg2AffineHBlankDma(void) {
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;
  volatile u16 *channel = REG_DMA0;
  u16 control = channel[DMA_CONTROL_HALF];

  channel[DMA_CONTROL_HALF] = control & (u16)~DMA_START_TIMING_DRQ_REPEAT;
  channel[DMA_CONTROL_HALF] &= (u16)~DMA_ENABLE;
  (void)channel[DMA_CONTROL_HALF];
  Dma_Set(work->bg2xLines, (void *)&REG_BG2X, DMA_HBLANK_WORD_PER_LINE,
          (volatile u32 *)channel);
}

/// BattleEffect_ArmBg2AffineHBlankDma for BG1HOFS and BG1VOFS: the line table
/// moves layer 1.
void BattleEffect_ArmBg1HBlankDma(void) {
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;
  volatile u16 *channel = REG_DMA0;
  u16 control = channel[DMA_CONTROL_HALF];

  channel[DMA_CONTROL_HALF] = control & (u16)~DMA_START_TIMING_DRQ_REPEAT;
  channel[DMA_CONTROL_HALF] &= (u16)~DMA_ENABLE;
  (void)channel[DMA_CONTROL_HALF];
  Dma_Set(work->bg2xLines, (void *)&REG_BG1HOFS, DMA_HBLANK_WORD_PER_LINE,
          (volatile u32 *)channel);
}

/// Abilities: Fire Breath, Salamander, Cerberus, Wyvern, Chimera, Macetail,
/// Fire Dragon, Flame Breath.
void BattleEffect_RunFireBreath(struct BattleEffectParams *args) {
  BattleEffect_RunBurstShower(args, BURST_SHOWER_MODE_FLASH);
}

/// Abilities: Toxic Breath.
void BattleEffect_RunToxicBreath(struct BattleEffectParams *args) {
  BattleEffect_RunBurstShower(args, BURST_SHOWER_MODE_TOXIC_BREATH);
}

/// Abilities: Water Breath, Aqua Breath.
void BattleEffect_RunWaterBreath(struct BattleEffectParams *args) {
  BattleEffect_RunBurstShower(args, BURST_SHOWER_MODE_SPRAY);
}

/// Abilities: Dark Breath.
void BattleEffect_RunDarkBreath(struct BattleEffectParams *args) {
  BattleEffect_RunBurstShower(args, BURST_SHOWER_MODE_GREY);
}

/// Abilities: Acid Breath.
void BattleEffect_RunAcidBreath(struct BattleEffectParams *args) {
  BattleEffect_RunBurstShower(args, BURST_SHOWER_MODE_PINK);
}

/// Abilities: Storm Breath.
void BattleEffect_RunStormBreath(struct BattleEffectParams *args) {
  BattleEffect_RunBurstShower(args, BURST_SHOWER_MODE_LIGHTNING);
}

/// Storm Breath as its second, stronger row (power 220 against 190) casts it.
///
/// Abilities: Storm Breath.
void BattleEffect_RunStormBreathStrong(struct BattleEffectParams *args) {
  BattleEffect_RunBurstShower(args, BURST_SHOWER_MODE_STORM_BREATH_STRONG);
}

/// Abilities: Evil Blessing.
void BattleEffect_RunEvilBlessing(struct BattleEffectParams *args) {
  BattleEffect_RunBurstShower(args, BURST_SHOWER_MODE_FACES);
}

/// Abilities: Darksol Gasp.
void BattleEffect_RunDarksolGasp(struct BattleEffectParams *args) {
  BattleEffect_RunBurstShower(args, BURST_SHOWER_MODE_DARKSOL_GASP);
}

/// Abilities: Deadly Gas.
void BattleEffect_RunDeadlyGas(struct BattleEffectParams *args) {
  BattleEffect_RunBurstShower(args, BURST_SHOWER_MODE_DJINNI);
}

/// Abilities: Petra.
void BattleEffect_RunPetra(struct BattleEffectParams *args) {
  BattleEffect_RunBurstShower(args, BURST_SHOWER_MODE_PETRA);
}

/// Abilities: Char.
void BattleEffect_RunChar(struct BattleEffectParams *args) {
  BattleEffect_RunBurstShower(args, BURST_SHOWER_MODE_CHAR);
}

/// Abilities: Fury.
void BattleEffect_RunFury(struct BattleEffectParams *args) {
  BattleEffect_RunBurstShower(args, BURST_SHOWER_MODE_FURY);
}

/// Abilities: Fugue.
void BattleEffect_RunFugue(struct BattleEffectParams *args) {
  BattleEffect_RunBurstShower(args, BURST_SHOWER_MODE_FUGUE);
}

/// Abilities: Sand Breath.
void BattleEffect_RunSandBreath(struct BattleEffectParams *args) {
  BattleEffect_RunBurstShower(args, BURST_SHOWER_MODE_SAND_BREATH);
}

/// Abilities: Desert Gasp.
void BattleEffect_RunDesertGasp(struct BattleEffectParams *args) {
  BattleEffect_RunBurstShower(args, BURST_SHOWER_MODE_DESERT_GASP);
}

/// Abilities: Black Ice.
void BattleEffect_RunBlackIce(struct BattleEffectParams *args) {
  BattleEffect_RunBurstShower(args, BURST_SHOWER_MODE_BLACK_ICE);
}

/// Abilities: Typhoon Blow.
void BattleEffect_RunTyphoonBlow(struct BattleEffectParams *args) {
  BattleEffect_RunBurstShower(args, BURST_SHOWER_MODE_TYPHOON_BLOW);
}

/// Abilities: Djinnfest.
void BattleEffect_RunDjinnfest(struct BattleEffectParams *args) {
  BattleEffect_RunBurstShower(args, BURST_SHOWER_MODE_DJINNFEST);
}

/// Abilities: Heat Kiss.
void BattleEffect_RunHeatKiss(struct BattleEffectParams *args) {
  BattleEffect_RunBurstShower(args, BURST_SHOWER_MODE_HEAT_KISS);
}

/// Fire Dance as the enemies cast it.
///
/// Abilities: Fire Dance.
void BattleEffect_RunFireDanceUnleash(struct BattleEffectParams *args) {
  BattleEffect_RunBurstShower(args, BURST_SHOWER_MODE_FIRE_DANCE_ENEMY);
}

/// Abilities: Gravel Blow.
void BattleEffect_RunGravelBlow(struct BattleEffectParams *args) {
  BattleEffect_RunBurstShower(args, BURST_SHOWER_MODE_GRAVEL_BLOW);
}

/// The user takes its leap pose and animates fast while the burst shower
/// plays.
///
/// Abilities: Wheeze, Aging Gas.
void BattleEffect_RunAgingGas(struct BattleEffectParams *args) {
  struct Actor *actor;

  actor = (struct Actor *)BattleObject_GetFar(args->user)->object;
  Actor_SelectAnimationFar(actor, POSE_LEAP);
  Actor_SetAnimationSpeedFar(actor, ANIM_STEP_NORMAL * 3);
  BattleEffect_RunBurstShower(args, BURST_SHOWER_MODE_AGING_GAS);
  Actor_SetAnimationSpeedFar(actor, ANIM_STEP_NORMAL);
}

/// The user takes its leap pose and animates fast while the burst shower
/// plays.
///
/// Abilities: Gel, Fire Dance (the Salamander Rod's unleash).
void BattleEffect_RunFireDance(struct BattleEffectParams *args) {
  struct Actor *actor;

  actor = (struct Actor *)BattleObject_GetFar(args->user)->object;
  Actor_SelectAnimationFar(actor, POSE_LEAP);
  Actor_SetAnimationSpeedFar(actor, ANIM_STEP_NORMAL * 3);
  BattleEffect_RunBurstShower(args, BURST_SHOWER_MODE_FIRE_DANCE);
  Actor_SetAnimationSpeedFar(actor, ANIM_STEP_NORMAL);
}

/// The user fires one shot a frame at a point near one of the targets,
/// taking them in turn, and each target is hit in turn; the blend fades over
/// the last eight frames. The mode picks the picture (flash bursts, music
/// notes, hearts, smoke, bubbles, demon faces, rock shards) and how it is
/// prepared, the colours, the blending and where the shots leave from. Storm
/// Breath shows a violet lightning bolt at the user instead; Aging Gas also
/// draws motes into the user; Char sparkles glints around the targets; and
/// Fire Dance, Desert Gasp and Heat Kiss hand over to Dull or Impair at the
/// end. A Djinni's prelude sets it swaying behind the user.
void BattleEffect_RunBurstShower(struct BattleEffectParams *args, u32 mode) {
  void *canvas;
  struct BattleEffectWork *work;
  s32 frame;
  u8 *sheet;
  s32 target;
  s32 i;
  s32 j;
  s32 launchFrames;
  s32 row;
  s32 column;

  u8 *palette;
  s32 origin[3];
  s32 djinniTarget[3];
  struct ScreenPoint userPosition;
  struct ScreenPoint targets[8];
  struct ScreenPoint aim;
  struct ScreenPoint screen;
  DrawRectangle pair[2];

  canvas = Ram_HeapSlots->battleCanvas;
  work = Ram_HeapSlots->battleEffect;
  sheet = Ram_HeapSlots->battleSheet;
  if (mode == BURST_SHOWER_MODE_AGING_GAS ||
      mode == BURST_SHOWER_MODE_FIRE_DANCE ||
      mode == BURST_SHOWER_MODE_SAND_BREATH ||
      mode == BURST_SHOWER_MODE_DESERT_GASP ||
      mode == BURST_SHOWER_MODE_BLACK_ICE ||
      mode == BURST_SHOWER_MODE_DJINNFEST ||
      mode == BURST_SHOWER_MODE_HEAT_KISS ||
      mode == BURST_SHOWER_MODE_FIRE_DANCE_ENEMY)
    BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  else
    BattleEffect_StartCanvasLayer(0);

  // The prelude: a Djinni behind the user, Mars for Fugue, Fury and Char and
  // Venus otherwise.
  if (args->prelude == 1) {
    if (mode == BURST_SHOWER_MODE_FUGUE || mode == BURST_SHOWER_MODE_FURY ||
        mode == BURST_SHOWER_MODE_CHAR) {
      BattleEffect_RunDjinniEntrance(
          args,
          DJINNI_ENTRANCE(ELEMENT_MARS, args->side, DJINNI_ANCHOR_OVER_USER),
          origin, djinniTarget);
      Resource_LoadAndDecompress(RESOURCE_MARS_DJINN_SMALL_SHEET,
                                 work->sheet + BURST_SHOWER_DJINNI, 1, 0);
    } else {
      BattleEffect_RunDjinniEntrance(
          args,
          DJINNI_ENTRANCE(ELEMENT_VENUS, args->side, DJINNI_ANCHOR_OVER_USER),
          origin, djinniTarget);
      Resource_LoadAndDecompress(RESOURCE_VENUS_DJINN_SMALL_SHEET,
                                 work->sheet + BURST_SHOWER_DJINNI, 1, 0);
    }
    REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
                 BGCNT_SCREEN_BASE(7);
  }
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sheet, 0, 0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;

  // The blitters: the first keeps the brighter pixel, the second adds, both
  // mirrored for the far side.
  if (args->prelude == 1) {
    if (args->side == EFFECT_SIDE_ENEMIES) {
      BattleEffect_FetchRectangleBlitters(0, pair);
    } else {
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
      pair[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                          BLITTER_ADDITIVE);

      pair[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;
    }
  } else if (mode == BURST_SHOWER_MODE_GRAVEL_BLOW) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                        BLITTER_TRANSPARENT);
    pair[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        BLITTER_TRANSPARENT);

    pair[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;
    REG_BLDALPHA = BLEND_ALPHA(16, 3);
  } else if (mode == BURST_SHOWER_MODE_FACES ||
             mode == BURST_SHOWER_MODE_DARKSOL_GASP) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
    pair[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);

    pair[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;
  } else {
    BattleEffect_FetchRectangleBlitters(args->side, pair);
  }

  // The pictures and the colors.
  Resource_LoadAndDecompress(RESOURCE_TORNADO_SHEET, work->sheet, 1, 0);
  if (mode == BURST_SHOWER_MODE_LIGHTNING ||
      mode == BURST_SHOWER_MODE_STORM_BREATH_STRONG) {
    Resource_LoadAndDecompress(RESOURCE_VIOLET_LIGHTNING_SHEET_A,
                               work->sheet + BURST_SHOWER_PICTURE, 1, 1);
  } else if (mode == BURST_SHOWER_MODE_FUGUE ||
             mode == BURST_SHOWER_MODE_DJINNFEST) {
    // The notes darkened: a drawn pixel keeps level 1.
    Resource_LoadAndDecompress(RESOURCE_MUSIC_NOTE_SHEET, work->sheet, 0, 0);
    for (i = 0; i != MUSIC_NOTE_COUNT * PICTURE_BYTES(MUSIC_NOTE); i++) {
      u8 *cell = work->sheet + i;
      u32 value = *cell;
      u8 level = value;

      if (level == 0)
        *cell = 0;
      else if (level <= MUSIC_NOTE_DIMMING)
        *cell = 1;
      else
        *cell = value - MUSIC_NOTE_DIMMING;
    }
    if (mode == BURST_SHOWER_MODE_DJINNFEST)
      Iwram_CopyWords((void *)BG_PLTT,
                      Resource_GetTableEntry(RESOURCE_ORANGE_PALETTE_B),
                      SHEET_PALETTE_BYTES);
  } else if (mode == BURST_SHOWER_MODE_GRAVEL_BLOW) {
    Resource_LoadAndDecompress(RESOURCE_BOULDER_SHEET,
                               work->sheet + BURST_SHOWER_PICTURE, 1, 1);
  } else if (mode == BURST_SHOWER_MODE_FACES) {
    Resource_LoadAndDecompress(RESOURCE_DEMON_FACE_SHEET,
                               work->sheet + BURST_SHOWER_PICTURE, 1, 1);
  } else if (mode == BURST_SHOWER_MODE_FURY) {
    Resource_LoadAndDecompress(RESOURCE_DEMON_FACE_SHEET,
                               work->sheet + BURST_SHOWER_PICTURE, 1, 1);
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry(RESOURCE_ORANGE_PALETTE_B),
                    SHEET_PALETTE_BYTES);
  } else if (mode == BURST_SHOWER_MODE_DARKSOL_GASP) {
    // A violet ramp from colour 1, blended 16:4.
    for (i = 1; i != CANVAS_LEVEL_BRIGHTEST + 1; i++) {
      s32 red = RGB_LEVEL_MAX / 2 - i / 4;
      s32 color = RGB(red, 0, RGB_LEVEL_MAX - i / 2);

      BG_PLTT[i] = color;
    }
    Resource_LoadAndDecompress(RESOURCE_DEMON_FACE_SHEET,
                               work->sheet + BURST_SHOWER_PICTURE, 1, 0);
    REG_BLDALPHA = BLEND_ALPHA(16, 4);
  } else if (mode == BURST_SHOWER_MODE_HEAT_KISS) {
    Resource_LoadAndDecompress(RESOURCE_HEART_SHEET, work->sheet, 1, 1);
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry(RESOURCE_ORANGE_PALETTE_B),
                    SHEET_PALETTE_BYTES);
  } else if (mode == BURST_SHOWER_MODE_FIRE_DANCE_ENEMY) {
    Resource_LoadAndDecompress(RESOURCE_SMOKE_SHEET, work->sheet, 1, 0);
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry(RESOURCE_ORANGE_PALETTE_B),
                    SHEET_PALETTE_BYTES);
  } else if (mode == BURST_SHOWER_MODE_CHAR) {
    // Six smoke pictures, each with more holes punched at random.
    Resource_LoadAndDecompress(RESOURCE_SMOKE_SHEET,
                               work->sheet + BURST_SHOWER_PICTURE, 1, 0);
    for (i = 0; i != 6; i++) {
      for (j = 0; j != SMOKE_PUFF_HOLES(i); j++)
        work->sheet[BURST_SHOWER_PICTURE + i * PICTURE_BYTES(SMOKE_PUFF) +
                    Random16() % PICTURE_BYTES(SMOKE_PUFF)] = 0;
    }
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry(RESOURCE_ORANGE_PALETTE_B),
                    SHEET_PALETTE_BYTES);
  } else if (mode == BURST_SHOWER_MODE_TOXIC_BREATH ||
             mode == BURST_SHOWER_MODE_TYPHOON_BLOW) {
    // The bubble, each row written twice.
    Resource_LoadAndDecompress(RESOURCE_BLUE_BEAM_SHEET, work->sheet, 1, 1);
    for (row = 0; row != BLUE_BUBBLE_HEIGHT; row++) {
      for (column = 0; column != BLUE_BUBBLE_WIDTH; column++) {
        work->sheet[BURST_SHOWER_BUBBLE + row * 2 * BLUE_BUBBLE_WIDTH +
                    column] = work->sheet[row * BLUE_BUBBLE_WIDTH + column];
        work->sheet[BURST_SHOWER_BUBBLE + row * 2 * BLUE_BUBBLE_WIDTH +
                    BLUE_BUBBLE_WIDTH + column] =
            work->sheet[row * BLUE_BUBBLE_WIDTH + column];
      }
    }
    if (mode == BURST_SHOWER_MODE_TOXIC_BREATH)
      Iwram_CopyWords((void *)BG_PLTT,
                      Resource_GetTableEntry(RESOURCE_LIME_PALETTE),
                      SHEET_PALETTE_BYTES);
    else
      Iwram_CopyWords((void *)BG_PLTT,
                      Resource_GetTableEntry(RESOURCE_ICE_BLOCK_SHEET),
                      SHEET_PALETTE_BYTES);
  } else {
    Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET,
                               work->sheet + BURST_SHOWER_PICTURE, 1, 1);
    if (mode == BURST_SHOWER_MODE_GREY) {
      // A grey ramp, unblended.
      for (i = 0; i != CANVAS_LEVEL_BRIGHTEST + 1; i++) {
        s32 level = i / 4;

        BG_PLTT[i] = RGB(level, level, level);
      }
      REG_BLDCNT = 0;
    } else {
      switch (mode) {
      case BURST_SHOWER_MODE_SAND_BREATH:
      case BURST_SHOWER_MODE_DESERT_GASP:
      case BURST_SHOWER_MODE_BLACK_ICE:
        // Six flash pictures, each with more holes punched at random.
        for (i = 0; i != 6; i++) {
          for (j = 0; j != FLASH_BURST_HOLES(i); j++)
            work->sheet[BURST_SHOWER_PICTURE + i * PICTURE_BYTES(FLASH_BURST) +
                        (Random16() & (PICTURE_BYTES(FLASH_BURST) - 1))] = 0;
        }
        if (mode == BURST_SHOWER_MODE_BLACK_ICE) {
          REG_BLDCNT = 0;
          for (i = 0; i != CANVAS_LEVEL_BRIGHTEST + 1; i++) {
            s32 level = i / 4;

            BG_PLTT[i] = RGB(level, level, level);
          }
          palette = NULL;
        } else if (mode == BURST_SHOWER_MODE_SAND_BREATH) {
          REG_BLDCNT = 0;
          palette = Resource_GetTableEntry(RESOURCE_ROCK_SPIRE_SHEET);
        } else {
          palette = Resource_GetTableEntry(RESOURCE_ORANGE_PALETTE_B);
        }
        break;
      case BURST_SHOWER_MODE_FLASH:
        palette = Resource_GetTableEntry(RESOURCE_FLASH_BURST_SHEET);
        break;
      case BURST_SHOWER_MODE_ICE:
        palette = Resource_GetTableEntry(RESOURCE_ICE_BLOCK_SHEET);
        break;
      case BURST_SHOWER_MODE_SPRAY:
      case BURST_SHOWER_MODE_TYPHOON_BLOW:
        palette = Resource_GetTableEntry(RESOURCE_WATER_SPRAY_SHEET);
        break;
      case BURST_SHOWER_MODE_PINK:
        palette = Resource_GetTableEntry(RESOURCE_PINK_PALETTE);
        break;
      case BURST_SHOWER_MODE_PETRA:
        palette = Resource_GetTableEntry(RESOURCE_BLAST_SHEET);
        break;
      case BURST_SHOWER_MODE_AGING_GAS:
        palette = Resource_GetTableEntry(RESOURCE_GREY_PALETTE_A);
        break;
      case BURST_SHOWER_MODE_FIRE_DANCE:
        palette = Resource_GetTableEntry(RESOURCE_ORANGE_PALETTE_B);
        break;
      default:
        palette = Resource_GetTableEntry(RESOURCE_MARS_DJINN_SHEET);
        break;
      }
      if (palette != NULL) {
        __typeof__(Iwram_CopyWordsEntry) routine = Iwram_CopyWordsEntry;
        routine((void *)BG_PLTT, palette, SHEET_PALETTE_BYTES);
      }
    }
  }

  // The trail: none for the rock modes, else halved or to 3/4.
  if (mode == BURST_SHOWER_MODE_SAND_BREATH ||
      mode == BURST_SHOWER_MODE_BLACK_ICE ||
      mode == BURST_SHOWER_MODE_GRAVEL_BLOW) {
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  } else if (mode == BURST_SHOWER_MODE_FACES ||
             mode == BURST_SHOWER_MODE_FUGUE ||
             mode == BURST_SHOWER_MODE_DJINNFEST ||
             mode == BURST_SHOWER_MODE_HEAT_KISS ||
             mode == BURST_SHOWER_MODE_FIRE_DANCE_ENEMY ||
             mode == BURST_SHOWER_MODE_CHAR) {
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  } else {
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  }
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
  EFFECT_FREE_PARTICLES(i, work->particles, 64);
  {
    s32 showerFrames;
    struct ScreenPoint *user;
    s32 k;
    if (mode == BURST_SHOWER_MODE_PETRA ||
        mode == BURST_SHOWER_MODE_FIRE_DANCE ||
        mode == BURST_SHOWER_MODE_DESERT_GASP)
      showerFrames = 32;
    else if (mode == BURST_SHOWER_MODE_GRAVEL_BLOW)
      showerFrames = 64;
    else if (mode == BURST_SHOWER_MODE_HEAT_KISS)
      showerFrames = 48;
    else
      showerFrames = 56;

    // Where the shots leave from, and where the targets are.
    EffectPosition_ProjectSprite(args->user, user = &userPosition);
    for (i = 0; i != args->targetCount; i++)
      EffectPosition_ProjectScaledBattler(args->targets[i], &targets[i]);
    if (mode == BURST_SHOWER_MODE_TOXIC_BREATH ||
        mode == BURST_SHOWER_MODE_BLACK_ICE) {
      if (args->side == EFFECT_SIDE_ENEMIES)
        user->x -= 24;
      else
        user->x += 24;
    }
    if (mode == BURST_SHOWER_MODE_GRAVEL_BLOW) {
      if (args->side == EFFECT_SIDE_ENEMIES)
        user->x -= 48;
      else
        user->x += 48;
    }
    if (mode == BURST_SHOWER_MODE_DARKSOL_GASP) {
      if (args->side == EFFECT_SIDE_ENEMIES)
        user->x -= 16;
      else
        user->x += 16;
      user->y -= 40;
    }
    if (mode == BURST_SHOWER_MODE_TYPHOON_BLOW) {
      if (args->side == EFFECT_SIDE_ENEMIES)
        user->x -= 32;
      else
        user->x += 32;
    }
    if (mode == BURST_SHOWER_MODE_PINK)
      user->y -= 16;
    if (mode == BURST_SHOWER_MODE_PETRA)
      user->y -= 40;
    if (mode == BURST_SHOWER_MODE_CHAR)
      user->y -= 48;
    if (mode == BURST_SHOWER_MODE_FURY)
      user->y -= 40;
    if (mode == BURST_SHOWER_MODE_FUGUE)
      user->y -= 40;
    if (mode == BURST_SHOWER_MODE_DJINNFEST)
      user->y -= 16;
    if (mode == BURST_SHOWER_MODE_FIRE_DANCE_ENEMY)
      user->y -= 16;
    if (mode == BURST_SHOWER_MODE_AGING_GAS) {
      user->y -= 24;
      for (i = 0; i != args->targetCount; i++)
        targets[i].y += 80;
    }
    if (mode == BURST_SHOWER_MODE_DJINNI) {
      if (args->side == EFFECT_SIDE_PARTY)
        user->x += 28;
      else
        user->x -= 28;
    }
    if (mode == BURST_SHOWER_MODE_FACES) {
      if (args->side == EFFECT_SIDE_PARTY)
        user->x += 16;
      else
        user->x -= 16;
    }
    if (mode == BURST_SHOWER_MODE_LIGHTNING ||
        mode == BURST_SHOWER_MODE_STORM_BREATH_STRONG) {
      user->x /= 3;
      if (mode == BURST_SHOWER_MODE_STORM_BREATH_STRONG)
        user->y -= 16;
      REG_BG2PA = Q_8_8(1) / 3; // three times as wide
    }

    // Aging Gas: motes burst from the middle of the first target.
    if (mode == BURST_SHOWER_MODE_AGING_GAS) {
      struct MotionObject *object;
      s32 half;

      object = BattleObject_GetFar(args->targets[0])->object;
      half = BattleUnit_GetEffectHeightFar(args->targets[0]) / 2;
      for (i = 0; i != AGING_GAS_MOTES; i++) {
        struct Particle *mote = &PARTICLES[i];

        SET_POSITION(mote, object->x, object->y + half, object->z);
        mote->velocityX = ((Random16() & 0xff) - 128) << 10;
        mote->velocityY = ((Random16() & 0xff) - 128) << 10;
        mote->velocityZ = ((Random16() & 0xff) - 128) << 10;
        mote->variant = 0;
      }
    }

    for (frame = 0; frame != showerFrames; frame++) {
      target = frame % args->targetCount;

      // Fade out over the last eight frames.
      if (frame >= showerFrames - 8)
        REG_BLDALPHA = EFFECT_BLEND_CANVAS(-((frame - showerFrames) * 2));
      if (mode == BURST_SHOWER_MODE_PETRA && frame > 15)
        BattleEffect_StepPaletteToResource(RESOURCE_CYAN_PALETTE);
      if (mode == BURST_SHOWER_MODE_AGING_GAS && frame > 31)
        BattleEffect_StepPaletteToResource(RESOURCE_BLAST_SHEET);
      if (frame == 4) {
        if (mode == BURST_SHOWER_MODE_FUGUE ||
            mode == BURST_SHOWER_MODE_DJINNFEST)
          Sound_PlayFar(MUS_VERY_LUCKY);
        else
          Sound_PlayFar(SE_WHOOSH);
      }
      if (mode == BURST_SHOWER_MODE_PETRA) {
        // Petra shows no damage during the shower.
      } else if (mode == BURST_SHOWER_MODE_GREY) {
        if (frame == 60)
          EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      } else if (mode == BURST_SHOWER_MODE_CHAR) {
        if (frame == 48)
          EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      } else if (frame == 24) {
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      }

      // The Djinni sways, and after frame 56 rises two pixels a frame.
      if (args->prelude == 1) {
        s32 x = (FIXEDtoINT(-Trig_Sin(frame * (TRIG_TURN / 32)) * 4)) +
                origin[0] / 2 - SMALL_DJINNI_WIDTH / 2;
        s32 y = (FIXEDtoINT(Trig_Cos(frame * (TRIG_TURN / 32)) * 2)) +
                origin[1] - 24;

        if (frame > 56)
          y = y - frame * 2 + 112;
        if (frame <= 3)
          pair[1](canvas, work->sheet + BURST_SHOWER_DJINNI, x, y,
                  SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
        pair[1](canvas, work->sheet + BURST_SHOWER_DJINNI, x, y,
                SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
      }

      if (mode == BURST_SHOWER_MODE_LIGHTNING ||
          mode == BURST_SHOWER_MODE_STORM_BREATH_STRONG) {
        // One big picture at the user, three frames a cel.
        if (args->side == EFFECT_SIDE_PARTY)
          pair[0](canvas,
                  work->sheet + BURST_SHOWER_PICTURE +
                      frame / 3 % 3 * VIOLET_LIGHTNING_BYTES,
                  user->x - 2, user->y - 32, VIOLET_LIGHTNING_WIDTH,
                  VIOLET_LIGHTNING_HEIGHT);
        else
          pair[0](canvas,
                  work->sheet + BURST_SHOWER_PICTURE +
                      frame / 3 % 3 * VIOLET_LIGHTNING_BYTES,
                  user->x - 70, user->y - 32, VIOLET_LIGHTNING_WIDTH,
                  VIOLET_LIGHTNING_HEIGHT);
      } else {
        // Launch a shot at a point near this frame's target.
        aim.x = targets[target].x + (Random16() & 31) - 16;
        if (mode == BURST_SHOWER_MODE_FUGUE ||
            mode == BURST_SHOWER_MODE_DJINNFEST ||
            mode == BURST_SHOWER_MODE_FIRE_DANCE_ENEMY)
          aim.y = targets[target].y + (Random16() & 127) - 32;
        else if (mode == BURST_SHOWER_MODE_TOXIC_BREATH ||
                 mode == BURST_SHOWER_MODE_BLACK_ICE ||
                 mode == BURST_SHOWER_MODE_HEAT_KISS ||
                 mode == BURST_SHOWER_MODE_GRAVEL_BLOW ||
                 mode == BURST_SHOWER_MODE_DARKSOL_GASP ||
                 mode == BURST_SHOWER_MODE_FURY ||
                 mode == BURST_SHOWER_MODE_TYPHOON_BLOW)
          aim.y = targets[target].y + (Random16() & 127) - 64;
        else if (mode == BURST_SHOWER_MODE_CHAR)
          aim.y = targets[target].y + Random16() % 96 - 32;
        else if (mode == BURST_SHOWER_MODE_FIRE_DANCE) {
          // The random height is thrown away; Fire Dance's shots weave.
          aim.y = targets[target].y + (Random16() & 255) - 192;
          aim.y = targets[target].y +
                  (FIXEDtoINT(Trig_Sin(frame * (TRIG_TURN / 16)) << 8)) - 128;
        } else
          aim.y = targets[target].y + (Random16() & 63) - 16;
        launchFrames = AGING_GAS_BURSTS;
        if (mode != BURST_SHOWER_MODE_AGING_GAS)
          launchFrames = BURST_SHOWER_BURSTS;
        if (frame < launchFrames) {
          struct Particle *shot = &work->particles[frame];
          s32 x = user->x * (toFIXED(1) / 2); // the canvas is half as wide
          s32 y;
          s32 dx;
          s32 dy;

          shot->x = x;
          y = INTtoFIXED(user->y);
          shot->y = y;
          dx = aim.x - user->x;
          shot->velocityX = dx * (toFIXED(1) / 32);
          dy = aim.y - user->y;
          shot->velocityY = dy * (toFIXED(1) / 32);
          shot->x = x + dx * (toFIXED(1) / 16);
          shot->y = y + dy * (toFIXED(1) / 16);
          shot->variant = 0;
        }
      }

      // Draw and move the shots in flight.
      for (i = 0; i != ARRAY_COUNT(work->particles); i++) {
        struct Particle *shot = &work->particles[i];
        s32 age = shot->variant;
        s32 cel;

        if (age < 0)
          continue;
        if (mode == BURST_SHOWER_MODE_FACES ||
            mode == BURST_SHOWER_MODE_DARKSOL_GASP) {
          // Never true here: Fury takes the branch below.
          if (mode == BURST_SHOWER_MODE_FURY && (i & 1))
            continue;
          if (age > 7)
            pair[args->side](canvas, work->sheet + BURST_SHOWER_PICTURE,
                             FIXED_INTEGER_PART(shot->x) - DEMON_FACE_WIDTH / 2,
                             FIXED_INTEGER_PART(shot->y) -
                                 DEMON_FACE_HEIGHT / 2,
                             DEMON_FACE_WIDTH, DEMON_FACE_HEIGHT);
        } else if (mode == BURST_SHOWER_MODE_FURY) {
          if (i & 1)
            continue;
          if (age > 2)
            pair[0](canvas, work->sheet + BURST_SHOWER_PICTURE,
                    FIXED_INTEGER_PART(shot->x) - DEMON_FACE_WIDTH / 2,
                    FIXED_INTEGER_PART(shot->y) - DEMON_FACE_HEIGHT / 2,
                    DEMON_FACE_WIDTH, DEMON_FACE_HEIGHT);
        } else if (mode == BURST_SHOWER_MODE_TOXIC_BREATH ||
                   mode == BURST_SHOWER_MODE_TYPHOON_BLOW) {
          if (age > 1)
            pair[0](canvas, work->sheet + BURST_SHOWER_BUBBLE,
                    FIXED_INTEGER_PART(shot->x) - BLUE_BUBBLE_WIDTH / 2,
                    FIXED_INTEGER_PART(shot->y) - BLUE_BUBBLE_HEIGHT,
                    BLUE_BUBBLE_WIDTH, BLUE_BUBBLE_HEIGHT * 2);
        } else if (mode == BURST_SHOWER_MODE_FUGUE ||
                   mode == BURST_SHOWER_MODE_DJINNFEST) {
          cel = i / 2 % 3;

          // Every other shot, wobbling up and down.
          if (i & 1) {
            s32 y =
                shot->y + (Trig_Sin((i * 2 + frame) * (TRIG_TURN / 32)) << 5);

            pair[1](canvas, work->sheet + cel * PICTURE_BYTES(MUSIC_NOTE),
                    FIXED_INTEGER_PART(shot->x) - MUSIC_NOTE_WIDTH / 2,
                    FIXEDtoINT(y) - MUSIC_NOTE_HEIGHT / 2, MUSIC_NOTE_WIDTH,
                    MUSIC_NOTE_HEIGHT);
          }
        } else if (mode == BURST_SHOWER_MODE_GRAVEL_BLOW) {
          s32 cel = i % 5 * 3 + age % 3;
          u32 width = gSpire_ShardWidths[cel];
          s32 x = FIXED_INTEGER_PART(shot->x);
          u32 half = width >> 1;
          const u8 *shardHeights = gSpire_ShardHeights;
          u32 height;
          s32 y;
          x -= half;
          height = shardHeights[cel];
          y = FIXED_INTEGER_PART(shot->y) - (height >> 1);

          pair[0](canvas,
                  work->sheet + gSpire_ShardPictures[cel] +
                      BURST_SHOWER_ROCK_SHARDS,
                  x, y, width, height);
        } else if (mode == BURST_SHOWER_MODE_FIRE_DANCE_ENEMY) {
          FIXED wobble = Trig_Sin((i * 2 + frame) * (TRIG_TURN / 32));
          s32 y;
          cel = age >> 2;
          y = FIXEDtoINT(shot->y + (wobble << 5));

          pair[1](canvas, work->sheet + cel * PICTURE_BYTES(SMOKE_PUFF),
                  FIXED_INTEGER_PART(shot->x) - SMOKE_PUFF_WIDTH / 2,
                  y - SMOKE_PUFF_HEIGHT / 2, SMOKE_PUFF_WIDTH,
                  SMOKE_PUFF_HEIGHT);
        } else if (mode == BURST_SHOWER_MODE_HEAT_KISS) {
          if (age > 1)
            pair[0](canvas, work->sheet,
                    FIXED_INTEGER_PART(shot->x) - HEART_WIDTH / 2,
                    FIXED_INTEGER_PART(shot->y) - HEART_HEIGHT / 2, HEART_WIDTH,
                    HEART_HEIGHT);
        } else if (mode == BURST_SHOWER_MODE_DJINNI) {
          if (age > 5)
            pair[0](canvas,
                    work->sheet + BURST_SHOWER_PICTURE +
                        age / 4 * PICTURE_BYTES(FLASH_BURST),
                    FIXED_INTEGER_PART(shot->x) - FLASH_BURST_WIDTH / 2,
                    FIXED_INTEGER_PART(shot->y) - FLASH_BURST_HEIGHT / 2,
                    FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
        } else if (mode == BURST_SHOWER_MODE_CHAR) {
          if (age > 1)
            pair[1](canvas,
                    work->sheet + BURST_SHOWER_PICTURE +
                        age / 4 * PICTURE_BYTES(SMOKE_PUFF),
                    FIXED_INTEGER_PART(shot->x) - SMOKE_PUFF_WIDTH / 2,
                    FIXED_INTEGER_PART(shot->y) - SMOKE_PUFF_HEIGHT / 2,
                    SMOKE_PUFF_WIDTH, SMOKE_PUFF_HEIGHT);
        } else if (mode != BURST_SHOWER_MODE_LIGHTNING) {
          if (age > 1)
            pair[0](canvas,
                    work->sheet + BURST_SHOWER_PICTURE +
                        age / 4 * PICTURE_BYTES(FLASH_BURST),
                    FIXED_INTEGER_PART(shot->x) - FLASH_BURST_WIDTH / 2,
                    FIXED_INTEGER_PART(shot->y) - FLASH_BURST_HEIGHT / 2,
                    FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
        }
        shot->x += shot->velocityX;
        shot->y += shot->velocityY;
        if (mode == BURST_SHOWER_MODE_AGING_GAS)
          shot->velocityY -= toFIXED(1) / 4;
        shot->variant++;
        if (mode == BURST_SHOWER_MODE_FUGUE ||
            mode == BURST_SHOWER_MODE_DJINNFEST) {
          if (shot->variant == BURST_SHOWER_NOTE_LIFE)
            shot->variant = -1;
        } else if (shot->variant == BURST_SHOWER_BURST_LIFE) {
          shot->variant = -1;
        }
      }

      // Char: three glints a frame around each target being hit.
      if (mode == BURST_SHOWER_MODE_CHAR) {
        for (i = 0; i != args->targetCount; i++) {
          if (frame >= i * 4 + 16 && frame < showerFrames - 8) {
            for (k = 0; k != 3; k++) {
              s32 cel = i & 3;
              s32 angle;
              s32 x;
              s32 y;
              u32 flip;

              angle = Random16() & TRIG_ANGLE_MASK;
              x = FIXEDtoINT(Trig_Sin(angle) << 4);
              x += targets[i].x / 2;
              x -= gBattleEffect_GlintPictureWidths[cel] >> 1;
              y = FIXEDtoINT(Trig_Cos(angle) << 4);
              y += targets[i].y;
              y -= gBattleEffect_GlintPictureHeights[cel] >> 1;

              EFFECT_RELEASE_BLITTERS();
              flip = Random16();
              EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                                  gProjectileVolleyB_GlintFlips[flip & 3] |
                                      EFFECT_CLIP_BOUNDS,
                                  BLITTER_ADDITIVE);
              ((DrawRectangle)Ram_HeapSlots->ramCodePaired)(
                  canvas, work->sheet + gBattleEffect_GlintPictures[cel], x, y,
                  gBattleEffect_GlintPictureWidths[cel],
                  gBattleEffect_GlintPictureHeights[cel]);
              HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
              BattleEffect_FetchRectangleBlitters(args->side, pair);
            }
          }
        }
      }

      // The hits.
      if (mode == BURST_SHOWER_MODE_FUGUE ||
          mode == BURST_SHOWER_MODE_DJINNFEST) {
        // The notes hit no one during the shower.
      } else if (mode == BURST_SHOWER_MODE_LIGHTNING ||
                 mode == BURST_SHOWER_MODE_STORM_BREATH_STRONG) {
        for (i = 0; i != args->targetCount; i++) {
          if (frame >= i * 4 + 2 && (frame & 7) == i) {
            work->shakeFrames = 8;
            BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                     POSE_DAMAGE, i, 4);
          }
        }
      } else if (mode == BURST_SHOWER_MODE_AGING_GAS) {
        if (frame == 18) {
          work->shakeFrames = 4;
          BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, 0, 4);
        }
        if (frame == 46)
          BattleEffect_TintBattler(args->user, SPRITE_COLOR_WHITE, TINT_KEEP,
                                   TINT_KEEP, 0);
        if (frame == 54)
          BattleEffect_TintBattler(args->user, SPRITE_COLOR_NORMAL, TINT_KEEP,
                                   TINT_KEEP, 0);
      } else if (mode == BURST_SHOWER_MODE_PETRA) {
        if (frame == 20) {
          for (i = 0; i != args->targetCount; i++)
            BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_GREY,
                                     POSE_DAMAGE, i, 60);
        }
      } else {
        for (i = 0; i != args->targetCount; i++) {
          if (frame >= i * 4 + 16 && frame < showerFrames - 8 &&
              (frame & 3) == i) {
            work->shakeFrames = 8;
            if (mode == BURST_SHOWER_MODE_GREY)
              BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_BLACK,
                                       POSE_DAMAGE, i, 3);
            else
              BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                       POSE_DAMAGE, i, 3);
            if (mode != BURST_SHOWER_MODE_FIRE_DANCE &&
                mode != BURST_SHOWER_MODE_DESERT_GASP)
              BattleEffect_StartActorMotion(args->targets[i], 1, toFIXED(1),
                                            toFIXED(4), toFIXED(2),
                                            frame + 100);
          }
        }
      }

      // Aging Gas: from frame 16 the motes are drawn into the user, and go
      // out once past it.
      if (mode == BURST_SHOWER_MODE_AGING_GAS && frame > 15) {
        struct MotionObject *object;
        s32 half;
        struct BattleCamera *camera;

        object = BattleObject_GetFar(args->user)->object;
        half = BattleUnit_GetEffectHeightFar(args->user) / 2;
        camera = Ram_HeapSlots->cameraWork;
        Matrix_Initialize();
        Matrix_SetLookAt(camera->eye, camera->focus);
        for (i = 0; i != AGING_GAS_DRAWN_MOTES; i++) {
          struct Particle *mote = &PARTICLES[i];
          s32 height = 12;

          if (mote->variant >= 0) {
            EffectPosition_ProjectPoint((s32 *)mote, &screen);
            screen.x >>= 1;
            pair[0](canvas, sheet + gBattleEffect_MotePictures[5], screen.x - 3,
                    screen.y - height / 2, 6, height);
            Particle_AdvanceWithGravity3D(mote, 62, 0);
            if (frame > i + 16) {
              s32 dx = (object->x - mote->x) >> 7;
              s32 dy = (object->y + half - mote->y) >> 8;
              s32 dz = (object->z - mote->z) >> 8;

              mote->velocityX += dx;
              mote->velocityY += dy;
              mote->velocityZ += dz;
              if ((object->x < 0 ? -object->x : object->x) <
                  (mote->x < 0 ? -mote->x : mote->x))
                mote->variant = -1;
            }
          }
        }
      }
      BattleCamera_Shake(4, 4);
      if (mode != BURST_SHOWER_MODE_GREY)
        BattleEffect_CountDownBattlerTints();
      EFFECT_PRESENT_CANVAS(work);
    }

    Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
    EFFECT_RELEASE_BLITTERS();
    if (mode == BURST_SHOWER_MODE_FIRE_DANCE ||
        mode == BURST_SHOWER_MODE_DESERT_GASP ||
        mode == BURST_SHOWER_MODE_HEAT_KISS) {
      // Hand over to the canvas effect: clear both canvases first.
      Iwram_ClearWords((void *)BG_CHAR_BLOCK(1), CANVAS_VIEW_BYTES);
      Iwram_ClearWords(canvas, CANVAS_VIEW_BYTES);
      args->prelude = 0;
      Task_Remove(Palette_StepFadeTransfer);
      if (mode == BURST_SHOWER_MODE_DESERT_GASP)
        BattleEffect_RunImpair(args);
      else
        BattleEffect_RunDull(args);
    } else {
      BattleEffect_EndCanvasLayer();
    }
  }
}
#undef VIOLET_LIGHTNING_WIDTH
#undef VIOLET_LIGHTNING_HEIGHT
#undef VIOLET_LIGHTNING_BYTES
#undef MUSIC_NOTE_WIDTH
#undef MUSIC_NOTE_HEIGHT
#undef MUSIC_NOTE_COUNT
#undef DEMON_FACE_WIDTH
#undef DEMON_FACE_HEIGHT
#undef MUSIC_NOTE_DIMMING
#undef FLASH_BURST_HOLES
#undef SMOKE_PUFF_HOLES
#undef BURST_SHOWER_BURSTS
#undef BURST_SHOWER_BURST_LIFE
#undef BURST_SHOWER_NOTE_LIFE
#undef AGING_GAS_BURSTS
#undef AGING_GAS_MOTES
#undef AGING_GAS_DRAWN_MOTES
