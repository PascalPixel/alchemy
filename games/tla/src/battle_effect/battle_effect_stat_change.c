/// @file
/// Stat boosts and reductions (Impact, Guard, Ward, Dull, Weaken ...).
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// The stat boosts' modes (BattleEffect_RunStatBoost), each named for the
/// effect that plays it.
enum StatBoostMode {
  STAT_BOOST_IMPACT,  ///< a dagger, in Mars colours
  STAT_BOOST_GUARD,   ///< a shield, in Mars colours
  STAT_BOOST_WARD,    ///< a shield, in Jupiter colours
  STAT_BOOST_GRANITE, ///< a pane of light, in Venus colours
  STAT_BOOST_ZEPHYR,  ///< a gust of wind, keeping the palette
  STAT_BOOST_FLASH,   ///< a pane of light, in Mars colours
  STAT_BOOST_IRON,
  STAT_BOOST_UNUSED, ///< no ability's effect plays it
  STAT_BOOST_SHADE,
  STAT_BOOST_COAL,
  STAT_BOOST_CRUCIBLE_FIRE, ///< Impact's burst without the gathering lights
  STAT_BOOST_STEAM,
  STAT_BOOST_GUARD_AURA
};

/// Abilities: Impact, High Impact, Demon Spear, Angel Spear; Forge; Kindle.
void BattleEffect_RunImpact(struct BattleEffectParams *args) {
  BattleEffect_RunStatBoost(args, STAT_BOOST_IMPACT);
}

/// Abilities: Guard, Protect, Guardian, Protector; Corona.
void BattleEffect_RunGuard(struct BattleEffectParams *args) {
  BattleEffect_RunStatBoost(args, STAT_BOOST_GUARD);
}

/// Abilities: Iron.
void BattleEffect_RunIron(struct BattleEffectParams *args) {
  BattleEffect_RunStatBoost(args, STAT_BOOST_IRON);
}

/// Plays the stat boost's unused mode; no ability's effect names this one.
void BattleEffect_RunUnusedStatBoost(struct BattleEffectParams *args) {
  BattleEffect_RunStatBoost(args, STAT_BOOST_UNUSED);
}

/// Abilities: Ward, Resist, Magic Shell, Magic Shield; Breeze.
void BattleEffect_RunWard(struct BattleEffectParams *args) {
  BattleEffect_RunStatBoost(args, STAT_BOOST_WARD);
}

/// Abilities: Steam.
void BattleEffect_RunSteam(struct BattleEffectParams *args) {
  BattleEffect_RunStatBoost(args, STAT_BOOST_STEAM);
}

/// Abilities: Granite.
void BattleEffect_RunGranite(struct BattleEffectParams *args) {
  BattleEffect_RunStatBoost(args, STAT_BOOST_GRANITE);
}

/// Abilities: Flash.
void BattleEffect_RunFlash(struct BattleEffectParams *args) {
  BattleEffect_RunStatBoost(args, STAT_BOOST_FLASH);
}

/// Abilities: Shade.
void BattleEffect_RunShade(struct BattleEffectParams *args) {
  BattleEffect_RunStatBoost(args, STAT_BOOST_SHADE);
}

/// Abilities: Guard Aura.
void BattleEffect_RunGuardAura(struct BattleEffectParams *args) {
  BattleEffect_RunStatBoost(args, STAT_BOOST_GUARD_AURA);
}

/// Abilities: Zephyr.
void BattleEffect_RunZephyr(struct BattleEffectParams *args) {
  BattleEffect_RunStatBoost(args, STAT_BOOST_ZEPHYR);
}

/// Abilities: Coal.
void BattleEffect_RunCoal(struct BattleEffectParams *args) {
  BattleEffect_RunStatBoost(args, STAT_BOOST_COAL);
}

/// The stat boost's burst with no gathering lights, with no prelude Djinni and
/// the fade step cleared.
///
/// Abilities: Crucible Fire.
void BattleEffect_RunCrucibleFire(struct BattleEffectParams *args) {
  struct BattleEffectWork *work = Ram_HeapSlots->battleEffect;

  work->fadeStep = 0;
  args->prelude = 0;
  BattleEffect_RunStatBoost(args, STAT_BOOST_CRUCIBLE_FIRE);
}

/// Build a blitter in its heap slot and fetch its entry.
#define LOAD_BLITTER(out, flags, mode)                                         \
  BattleEffect_BuildBlitter(HEAP_SLOT_RAM_CODE, 7, 7, flags, mode);            \
  out = (DrawRectangle)Ram_HeapSlots->ramCode

#define FLARES ((struct Particle *)Ram_MapCellBuffer)
#define FLARES_PER_MEMBER 64

/// The IWRAM word copy, fetched as a routine.
static inline CopyWordsFn CopyWordsRoutine(void) {
  /* FAKEMATCH(wrapper): the routine fetched through this inline. */
  return Iwram_CopyWordsEntry;
}

// Lights gather around each target in turn, eight frames apart, turning as
// they close in; at the center they burst, and the mode's picture plays
// there (a dagger, a shield, a streak or a wisp).

/// Flares of light swirl in to the centre of each target in turn and burst, and
/// the mode's picture plays over it.
void BattleEffect_RunStatBoost(struct BattleEffectParams *args, u32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw[2];
  s32 member;
  s32 shift;
  s32 flags;
  s32 start;
  // Holds BLITTER_ADDITIVE for the whole function: the lights' second build
  // then reloads its 0 in place of a fresh constant.
  s32 additive = BLITTER_ADDITIVE;
  // Holds 0 for the whole function: the center's coordinates are stored from
  // it.
  s32 zero = 0;
  s32 flipX = COMPOSITE_FLIP_X;
  s32 flipX2 = COMPOSITE_FLIP_X;
  s32 frame;
  s32 i;
  s32 origin[3];
  s32 target[3];
  struct ScreenPoint base;
  s32 center[3];
  struct ScreenPoint screen;
  s32 point[3];

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  start = 0;
  BattleEffect_StartCanvasLayer(0);

  // The prelude: the Djinni appears on the far side; origin receives where.
  if (args->prelude == 1) {
    if (mode == STAT_BOOST_GRANITE || mode == STAT_BOOST_IRON)
      BattleEffect_RunDjinniEntrance(args,
                                     DJINNI_ENTRANCE(ELEMENT_VENUS,
                                                     args->side ^ 1,
                                                     DJINNI_ANCHOR_TARGET_X),
                                     origin, target);
    else if (mode == STAT_BOOST_UNUSED || mode == STAT_BOOST_SHADE ||
             mode == STAT_BOOST_STEAM)
      BattleEffect_RunDjinniEntrance(args,
                                     DJINNI_ENTRANCE(ELEMENT_MERCURY,
                                                     args->side ^ 1,
                                                     DJINNI_ANCHOR_TARGET_X),
                                     origin, target);
    else if (mode == STAT_BOOST_WARD || mode == STAT_BOOST_ZEPHYR)
      BattleEffect_RunDjinniEntrance(args,
                                     DJINNI_ENTRANCE(ELEMENT_JUPITER,
                                                     args->side ^ 1,
                                                     DJINNI_ANCHOR_TARGET_X),
                                     origin, target);
    else if (mode != STAT_BOOST_CRUCIBLE_FIRE)
      BattleEffect_RunDjinniEntrance(
          args,
          DJINNI_ENTRANCE(ELEMENT_MARS, args->side ^ 1, DJINNI_ANCHOR_TARGET_X),
          origin, target);
    origin[0] = origin[0] * 4 / 5;
  }

  // Crucible Fire is Impact joined at frame 53, with a fade.
  if (mode == STAT_BOOST_CRUCIBLE_FIRE) {
    work->fadeFrames = 24;
    mode = STAT_BOOST_IMPACT;
    start = 53;
  }
  REG_BG2PA = 0xcc; // 1.25 times as wide

  // The pictures and the colors.
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD,
                             work->sheet + SERIES_B_FLARES, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_GlowOrbSheet,
                             work->sheet + SERIES_B_BURST, 1, 1);
  if (mode == STAT_BOOST_GRANITE || mode == STAT_BOOST_FLASH ||
      mode == STAT_BOOST_SHADE || mode == STAT_BOOST_GUARD_AURA) {
    Resource_LoadAndDecompress((s32)&ResourceId_VioletStreakSheet,
                               work->sheet + SERIES_B_PICTURE, 1, 1);
    if (mode == STAT_BOOST_GRANITE)
      CopyWordsRoutine()(
          (void *)BG_PLTT,
          Resource_GetTableEntry((s32)&ResourceId_VenusDjinnSheet), 128);
    else if (mode == STAT_BOOST_FLASH)
      CopyWordsRoutine()(
          (void *)BG_PLTT,
          Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet), 128);
    else
      CopyWordsRoutine()(
          (void *)BG_PLTT,
          Resource_GetTableEntry((s32)&ResourceId_MercuryDjinnSheet), 128);
  } else if (mode == STAT_BOOST_ZEPHYR) {
    Resource_LoadAndDecompress((s32)&ResourceId_WispSheet,
                               work->sheet + SERIES_B_PICTURE, 1, 1);
  } else if (mode == STAT_BOOST_COAL) {
    Resource_LoadAndDecompress((s32)&ResourceId_WispSheet,
                               work->sheet + SERIES_B_PICTURE, 1, 0);
    CopyWordsRoutine()((void *)BG_PLTT,
                       Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet),
                       128);
  } else {
    void *palette;

    if (mode == STAT_BOOST_IMPACT)
      Resource_LoadAndDecompress((s32)&ResourceId_DaggerSheet,
                                 work->sheet + SERIES_B_PICTURE, 1, 0);
    else
      Resource_LoadAndDecompress((s32)&ResourceId_ShieldSheet,
                                 work->sheet + SERIES_B_PICTURE, 1, 0);
    if (mode == STAT_BOOST_IMPACT)
      palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
    else if (mode == STAT_BOOST_WARD || mode == STAT_BOOST_ZEPHYR)
      palette = Resource_GetTableEntry((s32)&ResourceId_JupiterDjinnSheet);
    else if (mode == STAT_BOOST_IRON)
      palette = Resource_GetTableEntry((s32)&ResourceId_VenusDjinnSheet);
    else if (mode == STAT_BOOST_UNUSED || mode == STAT_BOOST_STEAM)
      palette = Resource_GetTableEntry((s32)&ResourceId_MercuryDjinnSheet);
    else if (mode == STAT_BOOST_GUARD)
      palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
    else
      palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
    CopyWordsRoutine()((void *)BG_PLTT, palette, 128);
  }
  if (mode == STAT_BOOST_GRANITE || mode == STAT_BOOST_IRON)
    Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSheet,
                               work->sheet + SERIES_B_DJINNI, 1, 0);
  else if (mode == STAT_BOOST_GUARD_AURA)
    Graphics_LoadGreyRamp();
  else if (mode == STAT_BOOST_WARD || mode == STAT_BOOST_ZEPHYR)
    Resource_LoadAndDecompress((s32)&ResourceId_JupiterDjinnSheet,
                               work->sheet + SERIES_B_DJINNI, 1, 0);
  else if (mode == STAT_BOOST_UNUSED || mode == STAT_BOOST_SHADE ||
           mode == STAT_BOOST_STEAM)
    Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSheet,
                               work->sheet + SERIES_B_DJINNI, 1, 0);
  else
    Resource_LoadAndDecompress((s32)&ResourceId_MarsDjinnSheet,
                               work->sheet + SERIES_B_DJINNI, 1, 0);

  // The lights: 25 pixels across and deep, 50 up and down.
  for (i = 0; i != 512; i++) {
    struct Particle *flare = &FLARES[i];

    SET_POSITION(flare, (Random16() % 200 - 100) << 14,
                 (Random16() % 200 - 100) << 15,
                 (Random16() % 200 - 100) << 14);
    flare->variant = 0;
  }

  // A lone target is moved to the canvas's middle (64).
  if (args->targetCount == 1) {
    EffectPosition_ProjectScaledBattler(args->targets[0], &base);
    shift = -base.x * 4 / 5 + 64;
  } else {
    if (args->side == EFFECT_SIDE_PARTY)
      shift = -64;
    else
      shift = 0;
  }
  REG_BG2X = shift << 8;
  work->transferMode =
      CANVAS_COPY_AND_FADE; // each transfer fades the canvas: a trail
  work->transferValue = 50; // to 1/2
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  if (args->side == EFFECT_SIDE_PARTY)
    flags = COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X;
  else
    flags = COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y;

  for (frame = start; frame != args->targetCount * 8 + 108; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;
    s32 prelude;

    if (frame == 0)
      Sound_PlayFar(SE_SWIRL);
    if (mode == STAT_BOOST_IRON && (frame & 3) == 0)
      BattleEffect_StepPaletteToResource((s32)&ResourceId_EffectPalette01);
    if (mode == STAT_BOOST_STEAM && (frame & 3) == 0)
      BattleEffect_StepPaletteToResource((s32)&ResourceId_JupiterDjinnSheet);
    if (frame == 80)
      EFFECT_SHOW_DAMAGE(0);

    // The Djinni sways, and after frame 32 rises two pixels a frame; for
    // the first four frames it is drawn ADDITIVE as well.
    prelude = args->prelude;
    if (prelude == 1) {
      s32 x =
          (Q_16_16_TO_INT(Trig_Sin(frame << 11) * 20)) + origin[0] + shift - 20;
      s32 y = (Q_16_16_TO_INT(Trig_Cos(frame << 11) * 4)) + origin[1] - 24;

      // The mode is the prelude, 1 here: BLITTER_MAX.
      LOAD_BLITTER(draw[0], flags ^ flipX, prelude);
      BattleEffect_BuildBlitter(HEAP_SLOT_RAM_CODE_PAIRED, 7, 7, flags ^ flipX2,
                                additive);
      // Read through the slot's address, which folds to one constant.
      draw[1] = *(DrawRectangle *)&Ram_HeapSlots->ramCodePaired;

      if (frame > 32)
        y = y - frame * 2 + 64;
      draw[0](canvas, work->sheet + SERIES_B_DJINNI, x, y, 40, 40);
      if (frame <= 3)
        draw[1](canvas, work->sheet + SERIES_B_DJINNI, x, y, 40, 40);
      EFFECT_RELEASE_BLITTERS();
    }

    for (member = 0; member != args->targetCount; member++) {
      struct MotionObject *object =
          BattleObject_GetFar(args->targets[member])->object;
      s32 height = BattleUnit_GetEffectHeightFar(args->targets[member]) * 2 / 3;

      if (frame == member * 8 + 80)
        Sound_PlayFar(SE_POWER_UP);
      Transform_Reset();
      Transform_SetLookAt(camera->eye, camera->focus);
      // The lights turn about two thirds of the target's height.
      point[0] = object->x;
      point[1] = height;
      point[2] = object->z;
      Transform_ApplyPosition(point);
      if (start != 0) {
        if (frame == member * 8 + 53)
          BattleEffect_TintBattler(args->targets[member], SPRITE_COLOR_WHITE,
                                   TINT_KEEP, member, 16);
      } else {
        if (frame == member * 8 + 48)
          BattleEffect_TintBattler(args->targets[member], SPRITE_COLOR_WHITE,
                                   TINT_KEEP, member, 16);

        // The lights turn and close in two pixels a frame.
        if (frame > member * 8) {
          /* FAKEMATCH(barrier): an empty asm through mode. */
          __asm__("" : "+h"(mode));
          LOAD_BLITTER(draw[0], flags, BLITTER_MAX);
          BattleEffect_BuildBlitter(HEAP_SLOT_RAM_CODE_PAIRED, 7, 7, flags,
                                    additive);
          {
            void **slot = &Ram_HeapSlots->ramCodePaired;

            /* FAKEMATCH(barrier): an empty asm through slot hides that it holds
             * the heap slot's address. */
            __asm__("" : "=r"(slot) : "0"(slot));
            draw[1] = (DrawRectangle)*slot;
          }
          if (mode == STAT_BOOST_IMPACT) {
            Transform_ApplyPitch(-frame << 10);
          } else if (mode == STAT_BOOST_GUARD) {
          } else if (mode == STAT_BOOST_WARD) {
            Transform_ApplyYaw(frame << 10);
          } else {
            Transform_ApplyYaw(frame << 10);
            Transform_ApplyRoll(frame << 10);
          }
          for (i = 0; i != 32; i++) {
            struct Particle *flare = &FLARES[member * FLARES_PER_MEMBER + i];

            if (frame > member * 8 + i) {
              s32 xx = (flare->x >> 8) * (flare->x >> 8);
              s32 yy = (flare->y >> 8) * (flare->y >> 8);
              s32 zz = (flare->z >> 8) * (flare->z >> 8);
              s32 distance = Iwram_Sqrt(xx + yy + zz) >> 9;

              if (distance != 0) {
                s32 size;

                // Half the width: depth 314-634 gives 6 down to 1.
                EffectPosition_ProjectPoint((s32 *)flare, &screen);
                screen.x = screen.x * 4 / 5 + shift;
                if (screen.depth < 314)
                  screen.depth = 314;
                if (screen.depth > 634)
                  screen.depth = 634;
                size = 6 - (screen.depth - 314) / 64;
                draw[1](canvas,
                        work->sheet + gBattleEffect_FlarePictures[size - 1],
                        screen.x - size, screen.y - size, size * 2, size * 2);
                flare->x -= flare->x / distance;
                flare->y -= flare->y / distance;
                flare->z -= flare->z / distance;
              }
            }
          }
          EFFECT_RELEASE_BLITTERS();
        }
      }

      // Where the center is on the screen.
      center[0] = zero;
      center[1] = zero;
      center[2] = zero;
      EffectPosition_ProjectPoint(center, &screen);
      screen.x = screen.x * 4 / 5 + shift;

      // The burst: from frame 52, six pictures four frames apart.
      if (frame >= member * 8 + 52 && frame < member * 8 + 76) {
        s32 cel;

        /* FAKEMATCH(dead): a once-only block. */
        do {
        } while (0);
        cel = (frame - member * 8 - 52) / 4 % 6;

        LOAD_BLITTER(draw[0], flags, BLITTER_MAX);
        draw[0](canvas, work->sheet + cel * 1600 + SERIES_B_BURST,
                screen.x - 20, screen.y - 20, 40, 40);
        HeapSlot_Release(HEAP_SLOT_RAM_CODE);
      }

      // From frame 80, the mode's picture.
      if (mode == STAT_BOOST_IMPACT) {
        // Seven 24x40 pictures, four frames apart.
        if (frame >= member * 8 + 80 && frame < member * 8 + 108) {
          s32 cel = (frame - member * 8 - 80) / 4 % 7;

          LOAD_BLITTER(draw[0], flags, BLITTER_MAX);
          draw[0](canvas, work->sheet + cel * 960 + SERIES_B_PICTURE,
                  screen.x - 12, screen.y - 20, 24, 40);
          HeapSlot_Release(HEAP_SLOT_RAM_CODE);
        }
      } else if (mode == STAT_BOOST_GRANITE || mode == STAT_BOOST_FLASH ||
                 mode == STAT_BOOST_SHADE || mode == STAT_BOOST_GUARD_AURA) {
        // Six 32x64 pictures, four frames apart.
        if (frame >= member * 8 + 80 && frame < member * 8 + 104) {
          s32 cel = (frame - member * 8 - 80) / 4 % 6;

          LOAD_BLITTER(draw[0], flags, BLITTER_MAX);
          draw[0](canvas, work->sheet + cel * 2048 + SERIES_B_PICTURE,
                  screen.x - 16, screen.y - 32, 32, 64);
          HeapSlot_Release(HEAP_SLOT_RAM_CODE);
        }
      } else if (mode == STAT_BOOST_ZEPHYR || mode == STAT_BOOST_COAL) {
        // Six 64x32 pictures two frames apart, mirrored below into 64x64.
        if (frame >= member * 8 + 80 && frame < member * 8 + 104) {
          s32 cel = (frame - member * 8 - 80) / 2 % 6;
          u8 *picture;    // the cel, drawn as its upper and mirrored lower half
          s32 lowerFlags; // the lower half drawn flipped

          BattleEffect_BuildBlitter(HEAP_SLOT_RAM_CODE, 7, 7, flags,
                                    BLITTER_MAX);
          lowerFlags = flags | COMPOSITE_FLIP_Y;
          draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
          BattleEffect_BuildBlitter(HEAP_SLOT_RAM_CODE_PAIRED, 7, 7, lowerFlags,
                                    BLITTER_MAX);
          picture = work->sheet + cel * 2048 + SERIES_B_PICTURE;
          draw[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;
          draw[0](canvas, picture, screen.x - 32, screen.y - 24, 64, 32);
          draw[1](canvas, picture, screen.x - 32, screen.y + 8, 64, 32);
          EFFECT_RELEASE_BLITTERS();
        }
      } else {
        // Six 40x40 pictures four frames apart, ADDITIVE.
        if (frame >= member * 8 + 80 && frame < member * 8 + 104) {
          s32 cel = (frame - member * 8 - 80) / 4 % 6;

          LOAD_BLITTER(draw[0], flags, BLITTER_ADDITIVE);
          draw[0](canvas, work->sheet + cel * 1600 + SERIES_B_PICTURE,
                  screen.x - 20, screen.y - 20, 40, 40);
          HeapSlot_Release(HEAP_SLOT_RAM_CODE);
        }
      }
    }
    EFFECT_END_FRAME(work);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

/// The stat reductions' modes (BattleEffect_RunStatReduction).
enum StatReductionMode {
  STAT_REDUCTION_UNUSED, ///< a dagger; no effect table entry reaches it
  STAT_REDUCTION_IMPAIR, ///< a shield
  STAT_REDUCTION_WEAKEN, ///< a shield
  STAT_REDUCTION_DULL    ///< a dagger
};

/// Breaks the dagger picture into fading shards as Dull does; no effect table
/// entry reaches it.
UNUSED void BattleEffect_RunUnusedStatReduction(void *action) {
  BattleEffect_RunStatReduction(action, STAT_REDUCTION_UNUSED);
}

/// Abilities: Dull, Blunt.
void BattleEffect_RunDull(struct BattleEffectParams *args) {
  BattleEffect_RunStatReduction(args, STAT_REDUCTION_DULL);
}

/// Abilities: Impair, Debilitate.
void BattleEffect_RunImpair(struct BattleEffectParams *args) {
  BattleEffect_RunStatReduction(args, STAT_REDUCTION_IMPAIR);
}

/// Abilities: Weaken, Enfeeble.
void BattleEffect_RunWeaken(struct BattleEffectParams *args) {
  BattleEffect_RunStatReduction(args, STAT_REDUCTION_WEAKEN);
}

#define RAMP_IMAGE 0x2580 ///< the shard picture, then seven darker copies
#define RAMP_SIZE 696     ///< one copy

/// A dagger (STAT_REDUCTION_DULL and _UNUSED) or a shield (the others) appears
/// over each target in turn, four frames apart, and breaks into shards that fly
/// off and darken.
///
/// Setup: seven darker copies of the shard picture are built by clamping
/// its levels; 24 shards a target start in a grid across the target and
/// scatter with random headings toward the user's side. Frame loop: a
/// target's picture plays for 24 frames; at frame 24 a chime sounds and the
/// shards fly for 36 frames, one shade darker every four frames.
void BattleEffect_RunStatReduction(struct BattleEffectParams *args, s32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 j;
  s32 xOffset;
  DrawRectangle *draw;
  struct BattleCamera *camera;
  s32 origin[3];
  struct ScreenPoint screen;
  s32 point[3];
  DrawRectangle callbacks[2];
  s32 i;
  s32 palette;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  EFFECT_START_NARROW_CANVAS(0);
  if (mode == STAT_REDUCTION_UNUSED || mode == STAT_REDUCTION_DULL)
    Resource_LoadAndDecompress((s32)&ResourceId_DaggerSheet, work->sheet, 1, 1);
  else
    Resource_LoadAndDecompress((s32)&ResourceId_ShieldSheet, work->sheet, 1, 1);
  if (mode == STAT_REDUCTION_UNUSED)
    palette = (s32)&ResourceId_PinkBurstSheet;
  else if (mode == STAT_REDUCTION_DULL) {
    palette = (s32)&ResourceId_GlowOrbSheet;
    /* FAKEMATCH(barrier): an empty asm. */
    asm("");
  } else if (mode == STAT_REDUCTION_IMPAIR)
    palette = (s32)&ResourceId_GlowOrbSheet;
  else
    palette = (s32)&ResourceId_PinkBurstSheet;
  Iwram_CopyWordsEntry((void *)BG_PLTT, Resource_GetTableEntry(palette), 128);
  Resource_LoadAndDecompress((s32)&ResourceId_PaletteRampImage,
                             work->sheet + RAMP_IMAGE, 0, 0);

  // Copy i holds every level clamped to 64 - 7i.
  for (i = 1; i != 8; i++) {
    for (j = 0; j != RAMP_SIZE; j++) {
      s32 level = work->sheet[RAMP_IMAGE + j];

      if (level > 64 - i * 7)
        level = 64 - i * 7;
      if (level < 0)
        level = 0;
      work->sheet[RAMP_IMAGE + i * RAMP_SIZE + j] = level;
    }
  }

  if (args->side == EFFECT_SIDE_PARTY) {
    REG_BG2X = -0x7000;
    xOffset = -112;
  } else {
    REG_BG2X = 0;
    xOffset = 0;
  }

  // The shards: rows six pixels apart, columns two (dagger) or eight
  // (shield) pixels deep, thrown toward the user's side.
  for (i = 0; i != 512; i++) {
    struct Particle *shard = &SHARDS[i];
    s32 radius = 192;
    s32 angle;

    angle = Random16() & 0xffff;
    shard->x = 0;
    if (mode == STAT_REDUCTION_UNUSED || mode == STAT_REDUCTION_DULL) {
      shard->y = px((i & 31) / 4 * 6 - 10);
      shard->z = px(i % 4 * 2 - 2);
    } else {
      shard->y = px((i & 31) / 4 * 6 - 10);
      shard->z = px(i % 4 * 8 - 16);
    }
    if (args->side == EFFECT_SIDE_PARTY)
      shard->velocityX = Q16_ONE * 2;
    else
      shard->velocityX = -Q16_ONE * 2;
    shard->velocityY = ((Trig_Cos(angle) * radius) >> 6) + Q16_ONE;
    shard->velocityZ = (Trig_Sin(angle) * radius) >> 6;
    shard->variant = Random16() & 0xff;
  }

  BattleEffect_FetchRectangleBlitters(args->side, draw = callbacks);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  for (frame = 0; frame != args->targetCount * 4 + 64; frame++) {
    camera = Ram_HeapSlots->cameraWork;
    if (frame == 72)
      EFFECT_SHOW_DAMAGE(0);
    for (j = 0; j != args->targetCount; j++) {
      struct MotionObject *object;
      s32 time;
      s32 x;
      s32 y;

      time = frame - j * 4;
      object = BattleObject_GetFar(args->targets[j])->object;
      if (time > 0) {
        // The picture stands 40 pixels above the target.
        Transform_Reset();
        Transform_SetLookAt(camera->eye, camera->focus);
        point[0] = object->x;
        point[1] = 160 << 13;
        point[2] = object->z;
        Transform_Reset();
        Transform_SetLookAt(camera->eye, camera->focus);
        Transform_ApplyPosition(point);
        origin[0] = 0;
        origin[1] = 0;
        origin[2] = 0;
        EffectPosition_ProjectPoint(origin, &screen);
        x = screen.x + xOffset;
        y = screen.y;
        if (mode == STAT_REDUCTION_UNUSED || mode == STAT_REDUCTION_DULL) {
          if (time <= 26)
            callbacks[0](canvas, work->sheet + time / 4 % 7 * 960, x - 12,
                         y - 20, 24, 40);
        } else {
          if (time <= 23)
            draw[1](canvas, work->sheet + time / 4 % 6 * 1600, x - 20, y - 20,
                    40, 40);
        }
        // frame - j * 4 is time; this spelling matches here and below.
        if (frame - j * 4 == 24)
          Sound_PlayFar(SE_CRACK);

        // The shards fly for 36 frames, a shade darker every four.
        if (time >= 24 && time <= 60) {
          s32 level;

          level = 0;
          if (frame - j * 4 > 28) {
            level = (time - 24) / 4;
            if (level > 7)
              level = 7;
          }
          for (i = 0; i != 24; i++) {
            struct Particle *shard = &SHARDS[j * 32 + i];
            s32 cell;

            cell = i % 4 * 3 + (shard->variant + time) / 8 % 3;
            EffectPosition_ProjectPoint((s32 *)shard, &screen);
            x = screen.x + xOffset;
            y = screen.y;
            callbacks[0](
                canvas,
                work->sheet +
                    (level * RAMP_SIZE + gStatChange_ShardPictures[cell]) +
                    RAMP_IMAGE,
                x, y, gStatChange_ShardWidths[cell],
                gStatChange_ShardHeights[cell]);
            BattleEffectKernels_IntegrateVector3(shard, 60, 0);
          }
        }
      }
    }
    EFFECT_PRESENT_CANVAS(work);
  }

  EFFECT_FINISH_CANVAS();
}
#undef SHARDS
