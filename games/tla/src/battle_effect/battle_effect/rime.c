/// @file
/// Rime and Haze.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "sounds.h"

/// BattleEffect_LoadBlitter's packed argument: the mode in the high nibble.

/// The Djinni's picture, 40 pixels wide, at the end of the work sheet.
#define DJINNI_OFFSET 0x65c0
/// Rime's crystal: a 48x48 picture mirrored from its left half into a
/// 64-pixel-wide square after the rune picture.
#define CRYSTAL_OFFSET 0x480
#define CRYSTAL_ORIGIN (CRYSTAL_OFFSET + 8 * 64 + 8)

/// Rime and Haze: the Djinni rises swaying through the canvas over the
/// party while its sign turns in the air: for Rime a crystal that spins in
/// two mirrored copies and shrinks, for Haze sixteen stars that bloom and
/// fade one after another.
///
/// Setup: canvas layer 0 placed on the far side of the targets by
/// BattleEffect_RunDjinniEntrance; the Djinni's picture with its palette;
/// Rime's rune picture mirrored into a 48x48 crystal, Haze's star sheet; one
/// additive blitter, flipped on the player's side. Frame loop (48 frames).
void BattleEffect_RunDjinniSign(struct BattleEffectParams *args, s32 mode) {
  s32 center[3];
  s32 base[3];
  struct BattleEffectWork *work;
  void *canvas;
  // Unused: blit[1] gives the stack frame its 4 bytes at sp 24.
  DrawRectangle blit[2];
  void *vertices;
  struct ModelTexture texture;
  struct RenderModel *model;
  s32 frame;
  s32 i;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  BattleEffect_StartCanvasLayer(0);
  if (mode == 0) {
    s32 row;
    s32 column;

    BattleEffect_RunDjinniEntrance(args,
                                   DJINNI_ENTRANCE(ELEMENT_MERCURY,
                                                   args->side ^ 1,
                                                   DJINNI_ANCHOR_OVER_TARGET),
                                   center, base);
    Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSheet,
                               work->sheet + DJINNI_OFFSET, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_RuneSheet, work->sheet, 1, 0);

    // Mirror the rune's 24x48 left half into a 48x48 crystal.
    Iwram_ClearWordsEntry(work->sheet + CRYSTAL_OFFSET, 0x4000);
    for (row = 0; row != 48; row++) {
      for (column = 0; column != 24; column++) {
        work->sheet[CRYSTAL_ORIGIN + row * 64 + column] =
            work->sheet[row * 24 + column];
        work->sheet[CRYSTAL_ORIGIN + row * 64 + 47 - column] =
            work->sheet[row * 24 + column];
      }
    }
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  } else {
    s32 haze[3];

    BattleEffect_RunDjinniEntrance(args,
                                   DJINNI_ENTRANCE(ELEMENT_JUPITER,
                                                   args->side ^ 1,
                                                   DJINNI_ANCHOR_OVER_TARGET),
                                   center, haze);
    Resource_LoadAndDecompress((s32)&ResourceId_JupiterDjinnSheet,
                               work->sheet + DJINNI_OFFSET, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_StarCrossSheet, work->sheet, 1,
                               0);
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  }
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  REG_BG2PA = Q_8_8(1);
  REG_BG2X = (64 - center[0]) << 8;
  if (args->side == EFFECT_SIDE_ENEMIES)
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        BLITTER_ADDITIVE);
  else
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                        BLITTER_ADDITIVE);
  blit[0] = (DrawRectangle)Ram_HeapSlots->ramCode;

  for (frame = 0; frame != 48; frame++) {
    // Frame 0: Haze's stars take random places about the centre, one more
    // every two frames.
    if (frame == 0) {
      for (i = 0; i != 32; i++) {
        struct Particle *star = &work->particles[i];

        SET_POSITION(star, px((Random16() & 31) - 16),
                     px((Random16() & 63) - 16), (Random16() & 127) + 128);
        star->velocityY = Random16();
        star->variant = -i * 2;
      }
    }
    if (frame == 38)
      EFFECT_SHOW_DAMAGE(-1);
    if (mode == 0) {
      if (frame == 32)
        Sound_PlayFar(SE_POWER_UP);
    } else {
      if (frame > 7)
        BattleEffect_StepPaletteToResource((s32)&ResourceId_StarCrossSheet);
      if (frame == 32)
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                 TINT_KEEP, 0, 24);
    }

    // The Djinni rises a pixel a frame, swaying, until it leaves the top.
    if (--center[1] > 0) {
      s32 height = center[1];

      if (height > 39)
        height = 40;
      blit[0](canvas, work->sheet + DJINNI_OFFSET,
              ((Trig_Sin(frame << 10) << 5) >> 16) + 44, center[1] - 20, 40,
              height);
    }

    Transform_Reset();
    gCameraSceneParameters[4] = 104;
    vertices = Heap_AllocEwram(32);
    model = Render_AllocateModelList(1);
    texture.logWidth = 6;
    texture.logHeight = 6;
    model->kind = 6;
    model->texture = &texture;
    model->mesh = gBattleEffect_SquareMesh;
    model->vertices = vertices;
    if (mode == 0) {
      // Rime's crystal fades in over the first eight frames and turns in
      // two mirrored copies that come together at frame 32, shrinking
      // from twice its size.
      s32 brightness = frame * 8 - 64;

      if (brightness > 0)
        brightness = 0;
      if (frame > 55)
        brightness = (56 - frame) * 8;
      model->shade = brightness;
      texture.pixels = work->sheet + CRYSTAL_OFFSET;
      for (i = 0; i != 2; i++) {
        s32 scale = Q16_ONE * 2 - frame * 0x800;

        if (frame > 31 && i == 1)
          continue;
        if (scale < Q16_ONE)
          scale = Q16_ONE;
        Transform_Reset();
        Transform_ApplyPitch(-TRIG_TURN / 4);
        if (frame <= 31) {
          if (i == 0)
            Transform_ApplyYaw((32 - frame) << 9);
          else
            Transform_ApplyYaw((frame << 9) - TRIG_TURN / 4);
        }
        SceneTransform_ApplyScale3(scale >> 1, scale >> 1, scale >> 1);
        Render_TransformVertices(gBattleEffect_GroundSquareCorners, vertices,
                                 4);
        Render_DrawModelList(model);
      }
    } else {
      // Each of Haze's stars swells and shrinks over 16 frames, and every
      // fourth one chimes as it opens.
      struct Particle *star;

      model->shade = 0;
      texture.pixels = work->sheet;
      for (i = 0, star = work->particles; i != 16; i++, star++) {
        if (star->variant == 4 && (i & 3) == 0)
          Sound_PlayFar(SE_PSYNERGY_START);
        if (star->variant >= 0 && star->variant < 16) {
          s32 swell = Trig_Sin(star->variant << 11) * star->z;
          s32 size;

          Transform_Reset();
          SceneTransform_ApplyPosition3(star->x, star->y, 0);
          size = swell >> 8;
          Transform_ApplyPitch(TRIG_TURN / 4);
          Transform_ApplyYaw(frame << 9);
          size /= 2;
          SceneTransform_ApplyScale3(size, size, size);
          Render_TransformVertices(gBattleEffect_GroundSquareCorners, vertices,
                                   4);
          Render_DrawModelList(model);
        }
        star->variant++;
      }
    }
    Heap_Release(model);
    Heap_Release(vertices);
    gCameraSceneParameters[3] = 120;
    gCameraSceneParameters[4] = 120;

    EFFECT_END_FRAME(work);
  }

  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

/// Abilities: Gasp (kind 242).
DEFINE_EFFECT_KIND(BattleEffect_RunGasp, BattleEffect_RunGaspFamily, 0)

/// Abilities: Reflux (kind 248).
DEFINE_EFFECT_KIND(BattleEffect_RunReflux, BattleEffect_RunGaspFamily, 2)

/// Abilities: Djinn Counter (kind 249).
DEFINE_EFFECT_KIND(BattleEffect_RunDjinnCounter, BattleEffect_RunGaspFamily, 3)

/// Abilities: Counter-rush (kind 396).
DEFINE_EFFECT_KIND(BattleEffect_RunCounterRush, BattleEffect_RunGaspFamily, 7)

/// Abilities: Mold (kind 250).
DEFINE_EFFECT_KIND(BattleEffect_RunMold, BattleEffect_RunGaspFamily, 4)

/// Abilities: Meld (kind 251).
DEFINE_EFFECT_KIND(BattleEffect_RunMeld, BattleEffect_RunGaspFamily, 4)

/// Abilities: Shine (kind 252).
DEFINE_EFFECT_KIND(BattleEffect_RunShine, BattleEffect_RunGaspFamily, 5)

/// Abilities: Lull (kind 241).
DEFINE_EFFECT_KIND(BattleEffect_RunLull, BattleEffect_RunGaspFamily, 6)

/// Abilities: Demon Eye (kind 445).
DEFINE_EFFECT_KIND(BattleEffect_RunDemonEye, BattleEffect_RunGaspFamily, 8)

/// Abilities: Emu, Call Demon, Claw Slash, Mega Slash, Vital Moon (kind 446).
DEFINE_EFFECT_KIND(BattleEffect_RunEmu, BattleEffect_RunGaspFamily, 9)

/// Abilities: Call Dullahan, Formina Sage (kind 448).
void BattleEffect_RunCallDullahan(struct BattleEffectParams *args) {
  BattleEffect_RunGaspFamily(args, 10);
}

/// Abilities: Beat Dance (kind 455).
DEFINE_EFFECT_KIND(BattleEffect_RunBeatDance, BattleEffect_RunGaspFamily, 11)

/// Abilities: Clarion Cry (kind 467).
DEFINE_EFFECT_KIND(BattleEffect_RunClarionCry, BattleEffect_RunGaspFamily, 14)

/// Abilities: Element Swap (kind 464).
DEFINE_EFFECT_KIND(BattleEffect_RunElementSwap, BattleEffect_RunGaspFamily, 12)

/// Abilities: Psy Boost (kind 468).
DEFINE_EFFECT_KIND(BattleEffect_RunPsyBoost, BattleEffect_RunGaspFamily, 15)

/// Abilities: Cage (kind 435).
DEFINE_EFFECT_KIND(BattleEffect_RunCage, BattleEffect_RunGaspFamily, 13)
#undef BLITTER
#undef DJINNI_OFFSET
#undef CRYSTAL_OFFSET
#undef CRYSTAL_ORIGIN
