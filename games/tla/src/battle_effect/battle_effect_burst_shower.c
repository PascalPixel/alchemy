/// @file
/// The blessings' burst showers.
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
  BURST_SHOWER_MODE_FIRE_DANCE_UNLEASH,
  BURST_SHOWER_MODE_GRAVEL_BLOW,
  BURST_SHOWER_MODE_DARKSOL_GASP,
  BURST_SHOWER_MODE_FURY,
  BURST_SHOWER_MODE_STORM_BREATH_STRONG,
  BURST_SHOWER_MODE_CHAR
};

/// Restarts DMA channel 0 as an H-blank transfer of the effect work's line
/// table into BG2X, one word a line.
void BattleEffect_ArmBg2AffineHBlankDma(void) {
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;
  volatile u16 *channel = REG_DMA0;
  u16 control = channel[5];
  u32 io;

  channel[5] = control & 0xc5ff;
  channel[5] &= 0x7fff;
  io = 0x80;
  (void)channel[5];
  Dma_Set(work->bg2xLines, (void *)((io << 19) + 0x28),
          DMA_HBLANK_WORD_PER_LINE, (volatile u32 *)channel);
}

/// BattleEffect_ArmBg2AffineHBlankDma for BG1HOFS and BG1VOFS: the line table
/// moves layer 1.
void BattleEffect_ArmBg1HBlankDma(void) {
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;
  volatile u16 *channel = REG_DMA0;
  u16 control = channel[5];
  u32 io;

  channel[5] = control & 0xc5ff;
  channel[5] &= 0x7fff;
  io = 0x80;
  (void)channel[5];
  Dma_Set(work->bg2xLines, (void *)((io << 19) + 0x14),
          DMA_HBLANK_WORD_PER_LINE, (volatile u32 *)channel);
}

#define Actor_SetAnimationSpeedFar                                             \
  ((s32 (*)(struct Actor *, s32))Actor_SetAnimationSpeedFar)

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

/// Fire Dance as the Staff unleashes it.
///
/// Abilities: Fire Dance.
void BattleEffect_RunFireDanceUnleash(struct BattleEffectParams *args) {
  BattleEffect_RunBurstShower(args, BURST_SHOWER_MODE_FIRE_DANCE_UNLEASH);
}

/// Abilities: Gravel Blow.
void BattleEffect_RunGravelBlow(struct BattleEffectParams *args) {
  BattleEffect_RunBurstShower(args, BURST_SHOWER_MODE_GRAVEL_BLOW);
}

/// From the user's action pose, the second projectile volley's shots fly at
/// each target.
///
/// Abilities: Wheeze, Aging Gas.
void BattleEffect_RunAgingGas(struct BattleEffectParams *args) {
  struct Actor *object;

  object = (struct Actor *)BattleObject_GetFar(args->user)->object;
  Actor_SelectAnimationFar(object, 2);
  Actor_SetAnimationSpeedFar(object, ANIMATION_STEP_NORMAL * 3);
  BattleEffect_RunBurstShower(args, BURST_SHOWER_MODE_AGING_GAS);
  Actor_SetAnimationSpeedFar(object, ANIMATION_STEP_NORMAL);
}

/// From the user's action pose, the second projectile volley's shots fly at
/// each target.
///
/// Abilities: Gel, Fire Dance.
void BattleEffect_RunFireDance(struct BattleEffectParams *args) {
  struct Actor *object;

  object = (struct Actor *)BattleObject_GetFar(args->user)->object;
  Actor_SelectAnimationFar(object, 2);
  Actor_SetAnimationSpeedFar(object, ANIMATION_STEP_NORMAL * 3);
  BattleEffect_RunBurstShower(args, BURST_SHOWER_MODE_FIRE_DANCE);
  Actor_SetAnimationSpeedFar(object, ANIMATION_STEP_NORMAL);
}

// BattleEffect_RunBurstShower, the body behind the 22 wrappers
// above (23 effect kinds, modes 0-24).

// Each frame a shot launched each frame from the user toward one target
// after another, every live shot drawn and moved, the targets hit four
// frames apart, and a fade over the last eight frames. The mode chooses
// the picture and how it is prepared (holes punched at random, a level
// remap, a doubled beam, a grey or violet ramp), the palette, the blending,
// how many shots and where they aim; modes 5 and 23 draw one big picture at
// the user instead, mode 10 also draws motes into the user, mode 24 adds
// glints around the targets, and modes 11, 14 and 18 hand over to
// BattleEffect_RunStatReduction at the end.

// A volley of shots flies from the user at the targets, one launched each
// frame toward the next target.

/// Setup: an optional Djinni appears behind the user (the prelude); the
/// mode prepares its picture and palette and moves the launch point. Frame
/// loop: a shot leaves the user each frame for 48 frames (16 in mode 10),
/// aimed near the next target; every live shot is drawn and moved and goes
/// out after 24 frames (32 in modes 9 and 17); from frame 16 the targets
/// are hit four frames apart and recoil; the blend fades out over the last
/// eight frames.
void BattleEffect_RunBurstShower(struct BattleEffectParams *args, u32 mode) {
  void *canvas;
  struct BattleEffectWork *work;
  s32 frame;
  u8 *sheet;
  s32 target;
  s32 i;
  s32 j;
  u8 *widths;
  u8 *heights;
  s32 limit;
  s32 row;
  s32 column;

  u8 *palette;
  s32 origin[3];
  s32 djinniTarget[3];
  struct ScreenPoint userPosition;
  s32 targets[8][3];
  s32 aim[3];
  s32 screen[3];
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
      mode == BURST_SHOWER_MODE_FIRE_DANCE_UNLEASH)
    BattleEffect_StartCanvasLayer(1);
  else
    BattleEffect_StartCanvasLayer(0);

  // The prelude: a Djinni behind the user, Mars for modes 9, 22 and 24 and
  // Venus otherwise.
  if (args->prelude == 1) {
    if (mode == BURST_SHOWER_MODE_FUGUE || mode == BURST_SHOWER_MODE_FURY ||
        mode == BURST_SHOWER_MODE_CHAR) {
      BattleEffect_RunDjinniEntrance(
          args,
          DJINNI_ENTRANCE(ELEMENT_MARS, args->side, DJINNI_ANCHOR_OVER_USER),
          origin, djinniTarget);
      Resource_LoadAndDecompress((s32)&ResourceId_MarsDjinnSmallSheet,
                                 work->sheet + VOLLEY_B_DJINNI, 1, 0);
    } else {
      BattleEffect_RunDjinniEntrance(
          args,
          DJINNI_ENTRANCE(ELEMENT_VENUS, args->side, DJINNI_ANCHOR_OVER_USER),
          origin, djinniTarget);
      Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSmallSheet,
                                 work->sheet + VOLLEY_B_DJINNI, 1, 0);
    }
    REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
                 BGCNT_SCREEN_BASE(7);
  }
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;

  // The blitters: [0] MAX and [1] ADDITIVE, mirrored for the far side.
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
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet, 1, 0);
  if (mode == BURST_SHOWER_MODE_LIGHTNING ||
      mode == BURST_SHOWER_MODE_STORM_BREATH_STRONG) {
    Resource_LoadAndDecompress((s32)&ResourceId_VioletLightningSheetA,
                               work->sheet + VOLLEY_B_PICTURE, 1, 1);
  } else if (mode == BURST_SHOWER_MODE_FUGUE ||
             mode == BURST_SHOWER_MODE_DJINNFEST) {
    // Remap the levels: 0 stays, 1-16 become 1, the rest drop by 16.
    Resource_LoadAndDecompress((s32)&ResourceId_Data0D7, work->sheet, 0, 0);
    for (i = 0; i != 0xc00; i++) {
      u8 *cell = work->sheet + i;
      u32 value = *cell;
      u8 level = value;

      if (level == 0)
        *cell = 0;
      else if (level <= 16)
        *cell = 1;
      else
        *cell = value - 16;
    }
    if (mode == BURST_SHOWER_MODE_DJINNFEST)
      Iwram_CopyWords((void *)BG_PLTT,
                      Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                      SHEET_PALETTE_BYTES);
  } else if (mode == BURST_SHOWER_MODE_GRAVEL_BLOW) {
    Resource_LoadAndDecompress((s32)&ResourceId_BoulderSheet,
                               work->sheet + VOLLEY_B_PICTURE, 1, 1);
  } else if (mode == BURST_SHOWER_MODE_FACES) {
    Resource_LoadAndDecompress((s32)&ResourceId_DemonFaceSheet,
                               work->sheet + VOLLEY_B_PICTURE, 1, 1);
  } else if (mode == BURST_SHOWER_MODE_FURY) {
    Resource_LoadAndDecompress((s32)&ResourceId_DemonFaceSheet,
                               work->sheet + VOLLEY_B_PICTURE, 1, 1);
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                    SHEET_PALETTE_BYTES);
  } else if (mode == BURST_SHOWER_MODE_DARKSOL_GASP) {
    // A violet ramp from color 1, blended 16:4.
    for (i = 1; i != 64; i++) {
      s32 low = 15 - i / 4;
      s32 value = ((31 - i / 2) << 10) | low;

      BG_PLTT[i] = value;
    }
    Resource_LoadAndDecompress((s32)&ResourceId_DemonFaceSheet,
                               work->sheet + VOLLEY_B_PICTURE, 1, 0);
    REG_BLDALPHA = BLEND_ALPHA(16, 4);
  } else if (mode == BURST_SHOWER_MODE_HEAT_KISS) {
    Resource_LoadAndDecompress((s32)&ResourceId_HeartSheet, work->sheet, 1, 1);
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                    SHEET_PALETTE_BYTES);
  } else if (mode == BURST_SHOWER_MODE_FIRE_DANCE_UNLEASH) {
    Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, work->sheet, 1, 0);
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                    SHEET_PALETTE_BYTES);
  } else if (mode == BURST_SHOWER_MODE_CHAR) {
    // Six smoke pictures, each with more holes punched at random.
    Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet,
                               work->sheet + VOLLEY_B_PICTURE, 1, 0);
    for (i = 0; i != 6; i++) {
      for (j = 0; j != i * 256 + 512; j++)
        work->sheet[VOLLEY_B_PICTURE + i * 1152 + Random16() % 1152] = 0;
    }
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                    SHEET_PALETTE_BYTES);
  } else if (mode == BURST_SHOWER_MODE_TOXIC_BREATH ||
             mode == BURST_SHOWER_MODE_TYPHOON_BLOW) {
    // The beam, each 24-pixel row drawn twice side by side.
    Resource_LoadAndDecompress((s32)&ResourceId_BlueBeamSheet, work->sheet, 1,
                               1);
    for (row = 0; row != 24; row++) {
      for (column = 0; column != 24; column++) {
        work->sheet[VOLLEY_B_BEAM + row * 48 + column] =
            work->sheet[row * 24 + column];
        work->sheet[VOLLEY_B_BEAM + row * 48 + 24 + column] =
            work->sheet[row * 24 + column];
      }
    }
    if (mode == BURST_SHOWER_MODE_TOXIC_BREATH)
      Iwram_CopyWords((void *)BG_PLTT,
                      Resource_GetTableEntry((s32)&ResourceId_LimePalette),
                      SHEET_PALETTE_BYTES);
    else
      Iwram_CopyWords((void *)BG_PLTT,
                      Resource_GetTableEntry((s32)&ResourceId_IceBlockSheet),
                      SHEET_PALETTE_BYTES);
  } else {
    Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                               work->sheet + VOLLEY_B_PICTURE, 1, 1);
    if (mode == BURST_SHOWER_MODE_GREY) {
      // A grey ramp, unblended.
      for (i = 0; i != 64; i++) {
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
          for (j = 0; j != i * 512 + 1024; j++)
            work->sheet[VOLLEY_B_PICTURE + (i << 11) + (Random16() & 0x7ff)] =
                0;
        }
        if (mode == BURST_SHOWER_MODE_BLACK_ICE) {
          REG_BLDCNT = 0;
          for (i = 0; i != 64; i++) {
            s32 level = i / 4;

            BG_PLTT[i] = RGB(level, level, level);
          }
          palette = NULL;
        } else if (mode == BURST_SHOWER_MODE_SAND_BREATH) {
          REG_BLDCNT = 0;
          palette = Resource_GetTableEntry((s32)&ResourceId_RockSpireSheet);
        } else {
          palette = Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB);
        }
        break;
      case BURST_SHOWER_MODE_FLASH:
        palette = Resource_GetTableEntry((s32)&ResourceId_FlashBurstSheet);
        break;
      case BURST_SHOWER_MODE_ICE:
        palette = Resource_GetTableEntry((s32)&ResourceId_IceBlockSheet);
        break;
      case BURST_SHOWER_MODE_SPRAY:
      case BURST_SHOWER_MODE_TYPHOON_BLOW:
        palette = Resource_GetTableEntry((s32)&ResourceId_WaterSpraySheet);
        break;
      case BURST_SHOWER_MODE_PINK:
        palette = Resource_GetTableEntry((s32)&ResourceId_PinkPalette);
        break;
      case BURST_SHOWER_MODE_PETRA:
        palette = Resource_GetTableEntry((s32)&ResourceId_BlastSheet);
        break;
      case BURST_SHOWER_MODE_AGING_GAS:
        palette = Resource_GetTableEntry((s32)&ResourceId_EffectPalette01);
        break;
      case BURST_SHOWER_MODE_FIRE_DANCE:
        palette = Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB);
        break;
      default:
        palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
        break;
      }
      if (palette != NULL) {
        __typeof__(Iwram_CopyWordsEntry) routine = Iwram_CopyWordsEntry;
        routine((void *)BG_PLTT, palette, 128);
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
             mode == BURST_SHOWER_MODE_FIRE_DANCE_UNLEASH ||
             mode == BURST_SHOWER_MODE_CHAR) {
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  } else {
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  }
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  EFFECT_FREE_PARTICLES(i, work->particles, 64);
  {
    s32 shots;
    struct ScreenPoint *user;
    s32 k;
    if (mode == BURST_SHOWER_MODE_PETRA ||
        mode == BURST_SHOWER_MODE_FIRE_DANCE ||
        mode == BURST_SHOWER_MODE_DESERT_GASP)
      shots = 32;
    else if (mode == BURST_SHOWER_MODE_GRAVEL_BLOW)
      shots = 64;
    else if (mode == BURST_SHOWER_MODE_HEAT_KISS)
      shots = 48;
    else
      shots = 56;

    // Where the shots leave from, and where the targets are.
    EffectPosition_ProjectSprite(args->user, user = &userPosition);
    for (i = 0; i != args->targetCount; i++)
      EffectPosition_ProjectScaledBattler(args->targets[i],
                                          (struct ScreenPoint *)targets[i]);
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
    if (mode == BURST_SHOWER_MODE_FIRE_DANCE_UNLEASH)
      user->y -= 16;
    if (mode == BURST_SHOWER_MODE_AGING_GAS) {
      user->y -= 24;
      for (i = 0; i != args->targetCount; i++)
        targets[i][1] += 80;
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
      REG_BG2PA = 0x55; // three times as wide
    }

    // Mode 10: 128 motes burst from the middle of the first target.
    if (mode == BURST_SHOWER_MODE_AGING_GAS) {
      struct MotionObject *object;
      s32 half;

      object = BattleObject_GetFar(args->targets[0])->object;
      half = BattleUnit_GetEffectHeightFar(args->targets[0]) / 2;
      for (i = 0; i != 128; i++) {
        struct Particle *mote = &MOTES[i];

        SET_POSITION(mote, object->x, object->y + half, object->z);
        mote->velocityX = ((Random16() & 0xff) - 128) << 10;
        mote->velocityY = ((Random16() & 0xff) - 128) << 10;
        mote->velocityZ = ((Random16() & 0xff) - 128) << 10;
        mote->variant = 0;
      }
    }

    for (frame = 0; frame != shots; frame++) {
      target = frame % args->targetCount;

      // Fade out over the last eight frames.
      if (frame >= shots - 8)
        REG_BLDALPHA = -((frame - shots) * 2) | 0x1000;
      if (mode == BURST_SHOWER_MODE_PETRA && frame > 15)
        BattleEffect_StepPaletteToResource((s32)&ResourceId_CyanPalette);
      if (mode == BURST_SHOWER_MODE_AGING_GAS && frame > 31)
        BattleEffect_StepPaletteToResource((s32)&ResourceId_BlastSheet);
      if (frame == 4) {
        if (mode == BURST_SHOWER_MODE_FUGUE ||
            mode == BURST_SHOWER_MODE_DJINNFEST)
          Sound_PlayFar(MUS_JINGLE_MEDIUM);
        else
          Sound_PlayFar(SE_WHOOSH);
      }
      if (mode == BURST_SHOWER_MODE_PETRA) {
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
        s32 x =
            (Q_16_16_TO_INT(-Trig_Sin(frame << 11) * 4)) + origin[0] / 2 - 10;
        s32 y = (Q_16_16_TO_INT(Trig_Cos(frame << 11) * 2)) + origin[1] - 24;

        if (frame > 56)
          y = y - frame * 2 + 112;
        if (frame <= 3)
          pair[1](canvas, work->sheet + VOLLEY_B_DJINNI, x, y, 20, 40);
        pair[1](canvas, work->sheet + VOLLEY_B_DJINNI, x, y, 20, 40);
      }

      if (mode == BURST_SHOWER_MODE_LIGHTNING ||
          mode == BURST_SHOWER_MODE_STORM_BREATH_STRONG) {
        // One big picture at the user, three frames a cel.
        if (args->side == EFFECT_SIDE_PARTY)
          pair[0](canvas,
                  work->sheet + VOLLEY_B_PICTURE + frame / 3 % 3 * 0x1200,
                  user->x - 2, user->y - 32, 72, 62);
        else
          pair[0](canvas,
                  work->sheet + VOLLEY_B_PICTURE + frame / 3 % 3 * 0x1200,
                  user->x - 70, user->y - 32, 72, 62);
      } else {
        // Launch a shot at a point near this frame's target.
        aim[0] = targets[target][0] + (Random16() & 31) - 16;
        if (mode == BURST_SHOWER_MODE_FUGUE ||
            mode == BURST_SHOWER_MODE_DJINNFEST ||
            mode == BURST_SHOWER_MODE_FIRE_DANCE_UNLEASH)
          aim[1] = targets[target][1] + (Random16() & 127) - 32;
        else if (mode == BURST_SHOWER_MODE_TOXIC_BREATH ||
                 mode == BURST_SHOWER_MODE_BLACK_ICE ||
                 mode == BURST_SHOWER_MODE_HEAT_KISS ||
                 mode == BURST_SHOWER_MODE_GRAVEL_BLOW ||
                 mode == BURST_SHOWER_MODE_DARKSOL_GASP ||
                 mode == BURST_SHOWER_MODE_FURY ||
                 mode == BURST_SHOWER_MODE_TYPHOON_BLOW)
          aim[1] = targets[target][1] + (Random16() & 127) - 64;
        else if (mode == BURST_SHOWER_MODE_CHAR)
          aim[1] = targets[target][1] + Random16() % 96 - 32;
        else if (mode == BURST_SHOWER_MODE_FIRE_DANCE) {
          aim[1] = targets[target][1] + (Random16() & 255) - 192;
          aim[1] = targets[target][1] +
                   (Q_16_16_TO_INT(Trig_Sin(frame << 12) << 8)) - 128;
        } else
          aim[1] = targets[target][1] + (Random16() & 63) - 16;
        limit = 16;
        if (mode != BURST_SHOWER_MODE_AGING_GAS)
          limit = 48;
        if (frame < limit) {
          struct Particle *shot = &work->particles[frame];
          s32 x = user->x << 15;
          s32 y;
          s32 dx;
          s32 dy;

          shot->x = x;
          y = px(user->y);
          shot->y = y;
          dx = aim[0] - user->x;
          shot->velocityX = dx << 11;
          dy = aim[1] - user->y;
          shot->velocityY = dy << 11;
          shot->x = x + (dx << 12);
          shot->y = y + (dy << 12);
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
          if (mode == BURST_SHOWER_MODE_FURY && (i & 1))
            continue;
          if (age > 7)
            pair[args->side](canvas, work->sheet + VOLLEY_B_PICTURE,
                             Q_16_16_UPPER_HALF(shot->x) - 16,
                             Q_16_16_UPPER_HALF(shot->y) - 32, 32, 64);
        } else if (mode == BURST_SHOWER_MODE_FURY) {
          if (i & 1)
            continue;
          if (age > 2)
            pair[0](canvas, work->sheet + VOLLEY_B_PICTURE,
                    Q_16_16_UPPER_HALF(shot->x) - 16,
                    Q_16_16_UPPER_HALF(shot->y) - 32, 32, 64);
        } else if (mode == BURST_SHOWER_MODE_TOXIC_BREATH ||
                   mode == BURST_SHOWER_MODE_TYPHOON_BLOW) {
          if (age > 1)
            pair[0](canvas, work->sheet + VOLLEY_B_BEAM,
                    Q_16_16_UPPER_HALF(shot->x) - 12,
                    Q_16_16_UPPER_HALF(shot->y) - 24, 24, 48);
        } else if (mode == BURST_SHOWER_MODE_FUGUE ||
                   mode == BURST_SHOWER_MODE_DJINNFEST) {
          cel = i / 2 % 3;

          // Every other shot, wobbling up and down.
          if (i & 1) {
            s32 y = shot->y + (Trig_Sin((i * 2 + frame) << 11) << 5);

            pair[1](canvas, work->sheet + cel * 1024,
                    Q_16_16_UPPER_HALF(shot->x) - 16, (Q_16_16_TO_INT(y)) - 16,
                    32, 32);
          }
        } else if (mode == BURST_SHOWER_MODE_GRAVEL_BLOW) {
          s32 cel = i % 5 * 3 + age % 3;
          u32 width = gSpire_ShardWidths[cel];
          s32 x = Q_16_16_UPPER_HALF(shot->x);
          u32 half = width >> 1;
          const u8 *shardHeights = gSpire_ShardHeights;
          u32 height;
          s32 y;
          x -= half;
          height = shardHeights[cel];
          y = Q_16_16_UPPER_HALF(shot->y) - (height >> 1);

          pair[0](canvas,
                  work->sheet + gSpire_ShardPictures[cel] + VOLLEY_B_GLOW, x, y,
                  width, height);
        } else if (mode == BURST_SHOWER_MODE_FIRE_DANCE_UNLEASH) {
          s32 wobble = Trig_Sin((i * 2 + frame) << 11);
          s32 y;
          cel = age >> 2;
          y = Q_16_16_TO_INT(shot->y + (wobble << 5));

          pair[1](canvas, work->sheet + cel * 1152,
                  Q_16_16_UPPER_HALF(shot->x) - 12, y - 24, 24, 48);
        } else if (mode == BURST_SHOWER_MODE_HEAT_KISS) {
          if (age > 1)
            pair[0](canvas, work->sheet, Q_16_16_UPPER_HALF(shot->x) - 11,
                    Q_16_16_UPPER_HALF(shot->y) - 21, 22, 42);
        } else if (mode == BURST_SHOWER_MODE_DJINNI) {
          if (age > 5)
            pair[0](canvas, work->sheet + VOLLEY_B_PICTURE + age / 4 * 2048,
                    Q_16_16_UPPER_HALF(shot->x) - 16,
                    Q_16_16_UPPER_HALF(shot->y) - 32, 32, 64);
        } else if (mode == BURST_SHOWER_MODE_CHAR) {
          if (age > 1)
            pair[1](canvas, work->sheet + VOLLEY_B_PICTURE + age / 4 * 1152,
                    Q_16_16_UPPER_HALF(shot->x) - 12,
                    Q_16_16_UPPER_HALF(shot->y) - 24, 24, 48);
        } else if (mode != BURST_SHOWER_MODE_LIGHTNING) {
          if (age > 1)
            pair[0](canvas, work->sheet + VOLLEY_B_PICTURE + age / 4 * 2048,
                    Q_16_16_UPPER_HALF(shot->x) - 16,
                    Q_16_16_UPPER_HALF(shot->y) - 32, 32, 64);
        }
        shot->x += shot->velocityX;
        shot->y += shot->velocityY;
        if (mode == BURST_SHOWER_MODE_AGING_GAS)
          shot->velocityY -= px(1) / 4;
        shot->variant++;
        if (mode == BURST_SHOWER_MODE_FUGUE ||
            mode == BURST_SHOWER_MODE_DJINNFEST) {
          if (shot->variant == 32)
            shot->variant = -1;
        } else if (shot->variant == 24) {
          shot->variant = -1;
        }
      }

      // Mode 24: three glints a frame around each target being hit.
      if (mode == BURST_SHOWER_MODE_CHAR) {
        for (i = 0; i != args->targetCount; i++) {
          if (frame >= i * 4 + 16 && frame < shots - 8) {
            // Default: set for real below.
            widths = NULL;
            heights = NULL;
            for (k = 0; k != 3; k++) {
              s32 cel = i & 3;
              s32 angle;
              s32 x;
              s32 y;

              // Default: set for real below.
              widths = NULL;
              heights = NULL;
              angle = Random16() & TRIG_ANGLE_MASK;
              x = Q_16_16_TO_INT(Trig_Sin(angle) << 4);
              x += targets[i][0] / 2;
              widths = gBattleEffect_GlintPictureWidths;
              x -= widths[cel] >> 1;
              y = Q_16_16_TO_INT(Trig_Cos(angle) << 4);
              y += targets[i][1];
              heights = gBattleEffect_GlintPictureHeights;
              y -= heights[cel] >> 1;

              EFFECT_RELEASE_BLITTERS();
              BattleEffect_BuildBlitter(
                  188, 7, 7,
                  gProjectileVolleyB_GlintFlips[Random16() & 3] |
                      EFFECT_CLIP_BOUNDS,
                  BLITTER_ADDITIVE);
              ((DrawRectangle)Ram_HeapSlots->ramCodePaired)(
                  canvas, work->sheet + gBattleEffect_GlintPictures[cel], x, y,
                  widths[cel], heights[cel]);
              HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
              BattleEffect_FetchRectangleBlitters(args->side, pair);
            }
          }
        }
      }

      // The hits.
      if (mode == BURST_SHOWER_MODE_FUGUE ||
          mode == BURST_SHOWER_MODE_DJINNFEST) {
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
          if (frame >= i * 4 + 16 && frame < shots - 8 && (frame & 3) == i) {
            work->shakeFrames = 8;
            if (mode == BURST_SHOWER_MODE_GREY)
              BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_BLACK,
                                       POSE_DAMAGE, i, 3);
            else
              BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                       POSE_DAMAGE, i, 3);
            if (mode != BURST_SHOWER_MODE_FIRE_DANCE &&
                mode != BURST_SHOWER_MODE_DESERT_GASP)
              BattleEffect_StartActorMotion(args->targets[i], 1, px(1), px(4),
                                            px(2), frame + 100);
          }
        }
      }

      // Mode 10: from frame 16 the motes are drawn into the user, and go out
      // once past it.
      if (mode == BURST_SHOWER_MODE_AGING_GAS && frame > 15) {
        struct MotionObject *object;
        s32 half;
        s32 *camera;

        object = BattleObject_GetFar(args->user)->object;
        half = BattleUnit_GetEffectHeightFar(args->user) / 2;
        camera = Ram_HeapSlots->cameraWork;
        Transform_Reset();
        Transform_SetLookAt(camera, camera + 3);
        for (i = 0; i != 24; i++) {
          struct Particle *mote = &MOTES[i];
          s32 height = 12;

          if (mote->variant >= 0) {
            EffectPosition_ProjectPoint((s32 *)mote,
                                        (struct ScreenPoint *)screen);
            screen[0] >>= 1;
            pair[0](canvas, sheet + gBattleEffect_MotePictures[5],
                    screen[0] - 3, screen[1] - height / 2, 6, height);
            BattleEffectKernels_IntegrateVector3(mote, 62, 0);
            if (frame > i + 16) {
              s32 dx;
              s32 dy;
              s32 dz;

              dx = object->x - mote->x;
              dy = object->y + half - mote->y;
              dz = object->z - mote->z;
              /* FAKEMATCH(barrier): an empty asm through dy adds the references
               * that rank it above dz and dx. */
              __asm__("" : "=r"(dy) : "0"(dy));
              /* FAKEMATCH(barrier): an empty asm through dz ranks it above dx.
               */
              __asm__("" : "=r"(dz) : "0"(dz));
              mote->velocityX += dx >> 7;
              mote->velocityY += dy >> 8;
              mote->velocityZ += dz >> 8;
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
#undef MOTES
#undef Actor_SetAnimationSpeedFar
