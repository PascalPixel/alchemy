/// @file
/// Stat boosts and reductions (Impact, Guard, Ward, Dull, Weaken ...).
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "sounds.h"

/// Abilities: Impact, High Impact, Demon Spear, Angel Spear (kind 26); Forge
/// (kind 210); Kindle (kind 223); kind 46 is used by no ability.
DEFINE_EFFECT_KIND(BattleEffect_RunImpact, BattleEffect_RunStatBoost, 0)

/// Abilities: Guard, Protect, Guardian, Protector (kind 28); Corona (kind 211).
DEFINE_EFFECT_KIND(BattleEffect_RunGuard, BattleEffect_RunStatBoost, 1)

/// Abilities: Iron (kind 220).
DEFINE_EFFECT_KIND(BattleEffect_RunIron, BattleEffect_RunStatBoost, 6)

DEFINE_EFFECT_KIND(BattleEffect_RunStatBoostMode7, BattleEffect_RunStatBoost, 7)

/// Abilities: Ward, Resist, Magic Shell, Magic Shield (kind 30); Breeze (kind
/// 215).
DEFINE_EFFECT_KIND(BattleEffect_RunWard, BattleEffect_RunStatBoost, 2)

/// Abilities: Steam (kind 221).
DEFINE_EFFECT_KIND(BattleEffect_RunSteam, BattleEffect_RunStatBoost, 11)

/// Abilities: Granite (kind 200).
DEFINE_EFFECT_KIND(BattleEffect_RunGranite, BattleEffect_RunStatBoost, 3)

/// Abilities: Flash (kind 213).
DEFINE_EFFECT_KIND(BattleEffect_RunFlash, BattleEffect_RunStatBoost, 5)

/// Abilities: Shade (kind 222).
DEFINE_EFFECT_KIND(BattleEffect_RunShade, BattleEffect_RunStatBoost, 8)

/// Abilities: Guard Aura (kind 460).
DEFINE_EFFECT_KIND(BattleEffect_RunGuardAura, BattleEffect_RunStatBoost, 12)

/// Abilities: Zephyr (kind 216).
DEFINE_EFFECT_KIND(BattleEffect_RunZephyr, BattleEffect_RunStatBoost, 4)

/// Abilities: Coal (kind 224).
DEFINE_EFFECT_KIND(BattleEffect_RunCoal, BattleEffect_RunStatBoost, 9)

/// Effect kind 182: the stat boost's mode 10 (a burst with no gathering
/// lights), with no prelude Djinni and the fade step cleared.
void BattleEffect_RunStatBoostMode10(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];

  work->fadeStep = 0;
  effect->prelude = 0;
  BattleEffect_RunStatBoost(effect, 10);
}

// The two blitters' heap slots, byte offsets into Ram_HeapSlots (The Broken
// Seal's HEAP_SLOT_BLITTER and HEAP_SLOT_BLITTER_ALTERNATE).
#define BLITTER_SLOT 104           // Ram_HeapSlots->unk64[1]
#define BLITTER_SLOT_ALTERNATE 188 // Ram_HeapSlots->unkb8[1]

// Build a blitter in its heap slot and fetch its entry.
#define LOAD_BLITTER(out, flags, mode)                                         \
  BattleEffect_BuildBlitter(BLITTER_SLOT, 7, 7, flags, mode);                  \
  out = (DrawRectangle)Ram_HeapSlots->unk64[1]

#define FLARES ((struct EffectStep *)Ram_MapCellBuffer)
#define FLARES_PER_MEMBER 64

static inline CopyWordsFn CopyWordsRoutine(void) {
  /* FAKEMATCH(wrapper): the routine fetched through this inline; as direct
   * calls to Iwram_CopyWords, 4 of 1146 instructions of
   * BattleEffect_RunStatBoost differ in tla-en. */
  return Iwram_CopyWordsEntry;
}

// Lights gather around each target in turn, eight frames apart, turning as
// they close in; at the center they burst, and the mode's picture plays
// there (a dagger, a shield, a streak or a wisp).

// Setup: an optional Djinni appears first (the prelude); the canvas is
// 1.25 times as wide, centered on a lone target or set by the side. Frame
// loop: the Djinni sways and rises; each target's 32 lights come out one a
// frame and close in two pixels a frame; from frame 52 of a target its
// burst plays, six pictures four frames apart, and from frame 80 the
// mode's picture.
void BattleEffect_RunStatBoost(struct BattleEffectArgument *effect, u32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw[2];
  s32 member;
  s32 shift;
  s32 flags;
  s32 start;
  // Holds BLIT_ADDITIVE for the whole function: the lights' second build
  // then reloads its 0 in place of a fresh constant.
  s32 additive = BLIT_ADDITIVE;
  // Holds 0 for the whole function: the center's coordinates are stored from
  // it.
  s32 zero = 0;
  // Each holds COMPOSITE_FLIP_X for the whole function, one for each of the
  // Djinni's builds: reload rebuilds the 4 at each xor, and the two xors stay
  // apart.
  s32 flipX = COMPOSITE_FLIP_X;
  s32 flipX2 = COMPOSITE_FLIP_X;
  s32 frame;
  s32 i;
  s32 origin[3];
  s32 target[3];
  struct EffectPosition base;
  s32 center[3];
  struct EffectPosition screen;
  s32 point[3];

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  start = 0;
  BattleEffect_StartCanvasLayer(0);

  // The prelude: the Djinni appears on the far side; origin receives where.
  if (effect->prelude == 1) {
    if (mode == 3 || mode == 6)
      BattleEffect_RunDjinniEntrance(
          effect,
          DJINNI_VENUS | DJINNI_SIDE(effect->side ^ 1) | DJINNI_ANCHORED,
          origin, target);
    else if (mode == 7 || mode == 8 || mode == 11)
      BattleEffect_RunDjinniEntrance(
          effect,
          DJINNI_MERCURY | DJINNI_SIDE(effect->side ^ 1) | DJINNI_ANCHORED,
          origin, target);
    else if (mode == 2 || mode == 4)
      BattleEffect_RunDjinniEntrance(
          effect,
          DJINNI_JUPITER | DJINNI_SIDE(effect->side ^ 1) | DJINNI_ANCHORED,
          origin, target);
    else if (mode != 10)
      BattleEffect_RunDjinniEntrance(
          effect, DJINNI_MARS | DJINNI_SIDE(effect->side ^ 1) | DJINNI_ANCHORED,
          origin, target);
    origin[0] = origin[0] * 4 / 5;
  }

  // Mode 10 is mode 0 joined at frame 53, with a fade.
  if (mode == 10) {
    work->fadeFrames = 24;
    mode = 0;
    start = 53;
  }
  REG_BG2PA = 0xcc; // 1.25 times as wide

  // The pictures and the colors.
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD,
                             work->sheet + SERIES_B_FLARES, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_GlowOrbSheet,
                             work->sheet + SERIES_B_BURST, 1, 1);
  if (mode == 3 || mode == 5 || mode == 8 || mode == 12) {
    Resource_LoadAndDecompress((s32)&ResourceId_VioletStreakSheet,
                               work->sheet + SERIES_B_PICTURE, 1, 1);
    if (mode == 3)
      CopyWordsRoutine()(
          (void *)BG_PLTT,
          Resource_GetTableEntry((s32)&ResourceId_VenusDjinnSheet), 128);
    else if (mode == 5)
      CopyWordsRoutine()(
          (void *)BG_PLTT,
          Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet), 128);
    else
      CopyWordsRoutine()(
          (void *)BG_PLTT,
          Resource_GetTableEntry((s32)&ResourceId_MercuryDjinnSheet), 128);
  } else if (mode == 4) {
    Resource_LoadAndDecompress((s32)&ResourceId_WispSheet,
                               work->sheet + SERIES_B_PICTURE, 1, 1);
  } else if (mode == 9) {
    Resource_LoadAndDecompress((s32)&ResourceId_WispSheet,
                               work->sheet + SERIES_B_PICTURE, 1, 0);
    CopyWordsRoutine()((void *)BG_PLTT,
                       Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet),
                       128);
  } else {
    void *palette;

    if (mode == 0)
      Resource_LoadAndDecompress((s32)&ResourceId_DaggerSheet,
                                 work->sheet + SERIES_B_PICTURE, 1, 0);
    else
      Resource_LoadAndDecompress((s32)&ResourceId_ShieldSheet,
                                 work->sheet + SERIES_B_PICTURE, 1, 0);
    if (mode == 0)
      palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
    else if (mode == 2 || mode == 4)
      palette = Resource_GetTableEntry((s32)&ResourceId_JupiterDjinnSheet);
    else if (mode == 6)
      palette = Resource_GetTableEntry((s32)&ResourceId_VenusDjinnSheet);
    else if (mode == 7 || mode == 11)
      palette = Resource_GetTableEntry((s32)&ResourceId_MercuryDjinnSheet);
    else if (mode == 1)
      palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
    else
      palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
    CopyWordsRoutine()((void *)BG_PLTT, palette, 128);
  }
  if (mode == 3 || mode == 6)
    Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSheet,
                               work->sheet + SERIES_B_DJINNI, 1, 0);
  else if (mode == 12)
    Graphics_LoadGreyRamp();
  else if (mode == 2 || mode == 4)
    Resource_LoadAndDecompress((s32)&ResourceId_JupiterDjinnSheet,
                               work->sheet + SERIES_B_DJINNI, 1, 0);
  else if (mode == 7 || mode == 8 || mode == 11)
    Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSheet,
                               work->sheet + SERIES_B_DJINNI, 1, 0);
  else
    Resource_LoadAndDecompress((s32)&ResourceId_MarsDjinnSheet,
                               work->sheet + SERIES_B_DJINNI, 1, 0);

  // The lights: 25 pixels across and deep, 50 up and down.
  for (i = 0; i != 512; i++) {
    struct EffectStep *flare = &FLARES[i];

    flare->x = (Random16() % 200 - 100) << 14;
    flare->y = (Random16() % 200 - 100) << 15;
    flare->z = (Random16() % 200 - 100) << 14;
    flare->variant = 0;
  }

  // A lone target is moved to the canvas's middle (64).
  if (effect->count == 1) {
    EffectPosition_ProjectScaledBattler(effect->actors[0], &base);
    shift = -base.x * 4 / 5 + 64;
  } else {
    if (effect->side == 1)
      shift = -64;
    else
      shift = 0;
  }
  REG_BG2X = shift << 8;
  work->transferMode = 2;   // each transfer fades the canvas: a trail
  work->transferValue = 50; // to 1/2
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  if (effect->side == 1)
    flags = COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X;
  else
    flags = COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y;

  for (frame = start; frame != effect->count * 8 + 108; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;
    s32 prelude;

    if (frame == 0)
      Sound_PlayFar(SE_SWIRL);
    if (mode == 6 && (frame & 3) == 0)
      BattleEffect_StepPaletteToResource((s32)&ResourceId_EffectPalette01);
    if (mode == 11 && (frame & 3) == 0)
      BattleEffect_StepPaletteToResource((s32)&ResourceId_JupiterDjinnSheet);
    if (frame == 80)
      BattlePlayback_StartFar(0);

    // The Djinni sways, and after frame 32 rises two pixels a frame; for
    // the first four frames it is drawn ADDITIVE as well.
    prelude = effect->prelude;
    if (prelude == 1) {
      s32 x = ((Trig_Sin(frame << 11) * 20) >> 16) + origin[0] + shift - 20;
      s32 y = ((Trig_Cos(frame << 11) * 4) >> 16) + origin[1] - 24;

      // The mode is the prelude, 1 here: BLIT_MAX.
      LOAD_BLITTER(draw[0], flags ^ flipX, prelude);
      BattleEffect_BuildBlitter(BLITTER_SLOT_ALTERNATE, 7, 7, flags ^ flipX2,
                                additive);
      // Read through the slot's address, which folds to one constant.
      draw[1] = *(DrawRectangle *)&Ram_HeapSlots->unkb8[1];

      if (frame > 32)
        y = y - frame * 2 + 64;
      draw[0](canvas, work->sheet + SERIES_B_DJINNI, x, y, 40, 40);
      if (frame <= 3)
        draw[1](canvas, work->sheet + SERIES_B_DJINNI, x, y, 40, 40);
      HeapSlot_Release(BLITTER_SLOT_ALTERNATE);
      HeapSlot_Release(BLITTER_SLOT);
    }

    for (member = 0; member != effect->count; member++) {
      struct MotionObject *object =
          BattleObject_GetFar(effect->actors[member])->object;
      s32 height =
          BattleUnit_GetEffectHeightFar(effect->actors[member]) * 2 / 3;

      if (frame == member * 8 + 80)
        Sound_PlayFar(SE_POWER_UP);
      Transform_Reset();
      Transform_SetLookAt(camera->eye, camera->pos);
      // The lights turn about two thirds of the target's height.
      point[0] = object->x;
      point[1] = height;
      point[2] = object->z;
      SceneTransform_ApplyPosition(point);
      if (start != 0) {
        if (frame == member * 8 + 53)
          BattleEffect_TintBattler(effect->actors[member], 7, -1, member, 16);
      } else {
        if (frame == member * 8 + 48)
          BattleEffect_TintBattler(effect->actors[member], 7, -1, member, 16);

        // The lights turn and close in two pixels a frame.
        if (frame > member * 8) {
          /* FAKEMATCH(barrier): an empty asm through mode keeps it in a high
           * register; without it, 2 of 1146 instructions differ in tla-en. */
          __asm__("" : "+h"(mode));
          LOAD_BLITTER(draw[0], flags, BLIT_MAX);
          BattleEffect_BuildBlitter(BLITTER_SLOT_ALTERNATE, 7, 7, flags,
                                    additive);
          {
            void **slot = &Ram_HeapSlots->unkb8[1];

            /* FAKEMATCH(barrier): an empty asm through slot hides that it holds
             * the heap slot's address; as draw[1] = Ram_HeapSlots->unkb8[1], 32
             * of 1146 instructions differ in tla-en. */
            __asm__("" : "=r"(slot) : "0"(slot));
            draw[1] = (DrawRectangle)*slot;
          }
          if (mode == 0) {
            Transform_ApplyPitch(-frame << 10);
          } else if (mode == 1) {
          } else if (mode == 2) {
            Transform_ApplyYaw(frame << 10);
          } else {
            Transform_ApplyYaw(frame << 10);
            Transform_ApplyRoll(frame << 10);
          }
          for (i = 0; i != 32; i++) {
            struct EffectStep *flare = &FLARES[member * FLARES_PER_MEMBER + i];

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
          HeapSlot_Release(BLITTER_SLOT_ALTERNATE);
          HeapSlot_Release(BLITTER_SLOT);
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

        // An empty once-only block: its loop notes end CSE's block here, so the
        // burst reads screen from the stack, not through the pointer passed to
        // EffectPosition_ProjectPoint.
        do {
        } while (0);
        cel = (frame - member * 8 - 52) / 4 % 6;

        LOAD_BLITTER(draw[0], flags, BLIT_MAX);
        draw[0](canvas, work->sheet + cel * 1600 + SERIES_B_BURST,
                screen.x - 20, screen.y - 20, 40, 40);
        HeapSlot_Release(BLITTER_SLOT);
      }

      // From frame 80, the mode's picture.
      if (mode == 0) {
        // Seven 24x40 pictures, four frames apart.
        if (frame >= member * 8 + 80 && frame < member * 8 + 108) {
          s32 cel = (frame - member * 8 - 80) / 4 % 7;

          LOAD_BLITTER(draw[0], flags, BLIT_MAX);
          draw[0](canvas, work->sheet + cel * 960 + SERIES_B_PICTURE,
                  screen.x - 12, screen.y - 20, 24, 40);
          HeapSlot_Release(BLITTER_SLOT);
        }
      } else if (mode == 3 || mode == 5 || mode == 8 || mode == 12) {
        // Six 32x64 pictures, four frames apart.
        if (frame >= member * 8 + 80 && frame < member * 8 + 104) {
          s32 cel = (frame - member * 8 - 80) / 4 % 6;

          LOAD_BLITTER(draw[0], flags, BLIT_MAX);
          draw[0](canvas, work->sheet + cel * 2048 + SERIES_B_PICTURE,
                  screen.x - 16, screen.y - 32, 32, 64);
          HeapSlot_Release(BLITTER_SLOT);
        }
      } else if (mode == 4 || mode == 9) {
        // Six 64x32 pictures two frames apart, mirrored below into 64x64.
        if (frame >= member * 8 + 80 && frame < member * 8 + 104) {
          s32 cel = (frame - member * 8 - 80) / 2 % 6;
          u8 *picture;    // the cel, drawn as its upper and mirrored lower half
          s32 lowerFlags; // the lower half drawn flipped

          // The lower half's flags are taken before the first entry is
          // fetched, which leaves the first entry r8 and the second sl.
          BattleEffect_BuildBlitter(BLITTER_SLOT, 7, 7, flags, BLIT_MAX);
          lowerFlags = flags | COMPOSITE_FLIP_Y;
          draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
          BattleEffect_BuildBlitter(BLITTER_SLOT_ALTERNATE, 7, 7, lowerFlags,
                                    BLIT_MAX);
          picture = work->sheet + cel * 2048 + SERIES_B_PICTURE;
          draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
          draw[0](canvas, picture, screen.x - 32, screen.y - 24, 64, 32);
          draw[1](canvas, picture, screen.x - 32, screen.y + 8, 64, 32);
          HeapSlot_Release(BLITTER_SLOT_ALTERNATE);
          HeapSlot_Release(BLITTER_SLOT);
        }
      } else {
        // Six 40x40 pictures four frames apart, ADDITIVE.
        if (frame >= member * 8 + 80 && frame < member * 8 + 104) {
          s32 cel = (frame - member * 8 - 80) / 4 % 6;

          LOAD_BLITTER(draw[0], flags, BLIT_ADDITIVE);
          draw[0](canvas, work->sheet + cel * 1600 + SERIES_B_PICTURE,
                  screen.x - 20, screen.y - 20, 40, 40);
          HeapSlot_Release(BLITTER_SLOT);
        }
      }
    }
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

void SummonEffect_RunCanvasMode0(void *action) {
  SummonEffect_RunCanvasEffect(action, 0);
}

void SummonEffect_RunCanvasMode3(void *action) {
  SummonEffect_RunCanvasEffect(action, 3);
}

void SummonEffect_RunCanvasMode1(void *action) {
  SummonEffect_RunCanvasEffect(action, 1);
}

void SummonEffect_RunCanvasMode2(void *action) {
  SummonEffect_RunCanvasEffect(action, 2);
}

// The dagger and shield canvas effect, behind the four wrappers above;
// the Broken Seal's twin is
// BattleEffect_RunStatReduction, which has no mode 3.

#define RAMP_IMAGE 0x2580 // the shard picture, then seven darker copies
#define RAMP_SIZE 696     // one copy

/// A dagger (modes 0 and 3) or a shield (1 and 2) appears over each target
/// in turn, four frames apart, and breaks into shards that fly off and
/// darken.
///
/// Setup: seven darker copies of the shard picture are built by clamping
/// its levels; 24 shards a target start in a grid across the target and
/// scatter with random headings toward the user's side. Frame loop: a
/// target's picture plays for 24 frames; at frame 24 a chime sounds and the
/// shards fly for 36 frames, one shade darker every four frames.
void SummonEffect_RunCanvasEffect(struct BattleEffectArgument *effect,
                                  s32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 j;
  s32 xOffset;
  DrawRectangle *draw;
  struct BattleCamera *camera;
  s32 origin[3];
  struct EffectPosition screen;
  s32 point[3];
  DrawRectangle callbacks[2];
  s32 i;
  s32 palette;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(0);
  REG_BG2PA = 0x100;
  if (mode == 0 || mode == 3)
    Resource_LoadAndDecompress((s32)&ResourceId_DaggerSheet, work->sheet, 1, 1);
  else
    Resource_LoadAndDecompress((s32)&ResourceId_ShieldSheet, work->sheet, 1, 1);
  if (mode == 0)
    palette = (s32)&ResourceId_PinkBurstSheet;
  else if (mode == 3) {
    palette = (s32)&ResourceId_GlowOrbSheet;
    /* FAKEMATCH(barrier): an empty asm keeps the mode-3 arm apart from the
     * identical mode-1 arm; without it, 8 of 553 instructions differ in
     * tla-en. */
    asm("");
  } else if (mode == 1)
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

  if (effect->side == 1) {
    REG_BG2X = -0x7000;
    xOffset = -112;
  } else {
    REG_BG2X = 0;
    xOffset = 0;
  }

  // The shards: rows six pixels apart, columns two (dagger) or eight
  // (shield) pixels deep, thrown toward the user's side.
  for (i = 0; i != 512; i++) {
    struct EffectStep *shard = &SHARDS[i];
    s32 radius = 192;
    s32 angle;

    angle = Random16() & 0xffff;
    shard->x = 0;
    if (mode == 0 || mode == 3) {
      shard->y = ((i & 31) / 4 * 6 - 10) << 16;
      shard->z = (i % 4 * 2 - 2) << 16;
    } else {
      shard->y = ((i & 31) / 4 * 6 - 10) << 16;
      shard->z = (i % 4 * 8 - 16) << 16;
    }
    if (effect->side == 1)
      shard->velocityX = Q16_ONE * 2;
    else
      shard->velocityX = -Q16_ONE * 2;
    shard->velocityY = ((Trig_Cos(angle) * radius) >> 6) + Q16_ONE;
    shard->velocityZ = (Trig_Sin(angle) * radius) >> 6;
    shard->variant = Random16() & 0xff;
  }

  BattleEffect_FetchRectangleBlitters(effect->side, draw = callbacks);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  for (frame = 0; frame != effect->count * 4 + 64; frame++) {
    camera = Ram_HeapSlots->cameraWork;
    if (frame == 72)
      BattlePlayback_StartFar(0);
    for (j = 0; j != effect->count; j++) {
      struct MotionObject *object;
      s32 time;
      s32 x;
      s32 y;

      time = frame - j * 4;
      object = BattleObject_GetFar(effect->actors[j])->object;
      if (time > 0) {
        // The picture stands 40 pixels above the target.
        Transform_Reset();
        Transform_SetLookAt(camera->eye, camera->pos);
        point[0] = object->x;
        point[1] = 160 << 13;
        point[2] = object->z;
        Transform_Reset();
        Transform_SetLookAt(camera->eye, camera->pos);
        SceneTransform_ApplyPosition(point);
        origin[0] = 0;
        origin[1] = 0;
        origin[2] = 0;
        EffectPosition_ProjectPoint(origin, &screen);
        x = screen.x + xOffset;
        y = screen.y;
        if (mode == 0 || mode == 3) {
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
            struct EffectStep *shard = &SHARDS[j * 32 + i];
            s32 cell;

            cell = i % 4 * 3 + (shard->variant + time) / 8 % 3;
            EffectPosition_ProjectPoint((s32 *)shard, &screen);
            x = screen.x + xOffset;
            y = screen.y;
            callbacks[0](
                canvas,
                work->sheet +
                    (level * RAMP_SIZE + gStatChange_ShardCells[cell]) +
                    RAMP_IMAGE,
                x, y, gStatChange_ShardWidths[cell],
                gStatChange_ShardHeights[cell]);
            BattleEffectKernels_IntegrateVector3(shard, 60, 0);
          }
        }
      }
    }
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}
