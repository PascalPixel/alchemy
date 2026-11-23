/// @file
/// Rime and Haze, whose Djinni rises while its sign turns over the target,
/// then the entry points of the abilities BattleEffect_RunGaspFamily plays.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// The Djinni's picture, after the sign's pictures in the work sprite sheet.
#define DJINNI_SIGN_DJINNI_PICTURE 26048
/// How far the Djinni sways either side as it rises, in pixels.
#define DJINNI_SIGN_DJINNI_SWAY 32
#define DJINNI_SIGN_FRAMES 48

/// Rime's seal: the 64x64 texture after RuneSheet's half, which the rune's
/// half and its mirror fill (RUNE_TEXTURE_RUNE); the stretch of the sprite
/// sheet cleared for it is four times the texture.
#define RIME_SEAL_TEXTURE PICTURE_BYTES(RUNE_HALF)
#define RIME_SEAL_CLEARED_BYTES 16384

/// Haze's stars: thirty-two placed, of which the first sixteen bloom, each
/// over sixteen frames.
#define HAZE_PLACED_STARS 32
#define HAZE_STARS 16
#define HAZE_STAR_FRAMES 16

/// Rime and Haze: the Djinni rises swaying out of sight over the targets while
/// its sign turns in the air: for Rime a seal that spins in two mirrored copies
/// and shrinks onto the target, for Haze sixteen stars that bloom and fade one
/// after another.
void BattleEffect_RunDjinniSign(struct BattleEffectParams *args, s32 mode) {
  s32 center[3];
  s32 base[3];
  struct BattleEffectWork *work;
  void *canvas;
  /* LEFTOVER(frame): blit[1], the pair's idle second blitter, which the ROM's
   * frame reserves and never touches; as one DrawRectangle. */
  DrawRectangle blit[2];
  void *vertices;
  struct ModelTexture texture;
  struct RenderModel *model;
  s32 frame;
  s32 i;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  BattleEffect_StartCanvasLayer(0);
  if (mode == DJINNI_SIGN_RIME) {
    s32 row;
    s32 column;

    BattleEffect_RunDjinniEntrance(args,
                                   DJINNI_ENTRANCE(ELEMENT_MERCURY,
                                                   args->side ^ 1,
                                                   DJINNI_ANCHOR_OVER_TARGET),
                                   center, base);
    Resource_LoadAndDecompress(RESOURCE_MERCURY_DJINN_SHEET,
                               work->sheet + DJINNI_SIGN_DJINNI_PICTURE, 1, 1);
    Resource_LoadAndDecompress(RESOURCE_RUNE_SHEET, work->sheet, 1, 0);

    // The rune's half and its mirror make the seal.
    Iwram_ClearWordsEntry(work->sheet + RIME_SEAL_TEXTURE,
                          RIME_SEAL_CLEARED_BYTES);
    for (row = 0; row != RUNE_HALF_HEIGHT; row++) {
      for (column = 0; column != RUNE_HALF_WIDTH; column++) {
        work->sheet[RIME_SEAL_TEXTURE + RUNE_TEXTURE_RUNE +
                    row * RUNE_TEXTURE_SIZE + column] =
            work->sheet[row * RUNE_HALF_WIDTH + column];
        work->sheet[RIME_SEAL_TEXTURE + RUNE_TEXTURE_RUNE +
                    row * RUNE_TEXTURE_SIZE + 2 * RUNE_HALF_WIDTH - 1 -
                    column] = work->sheet[row * RUNE_HALF_WIDTH + column];
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
    Resource_LoadAndDecompress(RESOURCE_JUPITER_DJINN_SHEET,
                               work->sheet + DJINNI_SIGN_DJINNI_PICTURE, 1, 1);
    Resource_LoadAndDecompress(RESOURCE_STAR_CROSS_SHEET, work->sheet, 1, 0);
    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  }
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
  REG_BG2PA = Q_8_8(1);
  REG_BG2X = (CANVAS_SIZE / 2 - center[0]) << 8;
  if (args->side == EFFECT_SIDE_ENEMIES)
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        BLITTER_ADDITIVE);
  else
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                        BLITTER_ADDITIVE);
  blit[0] = (DrawRectangle)Ram_HeapSlots->ramCode;

  for (frame = 0; frame != DJINNI_SIGN_FRAMES; frame++) {
    // Frame 0: Haze's stars take random places about the centre, one more
    // blooming every two frames.
    if (frame == 0) {
      for (i = 0; i != HAZE_PLACED_STARS; i++) {
        struct Particle *star = &work->particles[i];

        SET_POSITION(star, INTtoFIXED((Random16() & 31) - 16),
                     INTtoFIXED((Random16() & 63) - 16),
                     (Random16() & 127) + 128);
        star->velocityY = Random16();
        star->variant = -i * 2;
      }
    }
    if (frame == 38)
      EFFECT_SHOW_DAMAGE(-1);
    if (mode == DJINNI_SIGN_RIME) {
      if (frame == 32)
        Sound_PlayFar(SE_POWER_UP);
    } else {
      if (frame > 7)
        BattleEffect_StepPaletteToResource(RESOURCE_STAR_CROSS_SHEET);
      if (frame == 32)
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                 TINT_KEEP, 0, 24);
    }

    // The Djinni rises a pixel a frame, swaying, until it leaves the top.
    if (--center[1] > 0) {
      s32 height = center[1];

      if (height > DJINNI_HEIGHT - 1)
        height = DJINNI_HEIGHT;
      blit[0](canvas, work->sheet + DJINNI_SIGN_DJINNI_PICTURE,
              FIXEDtoINT(Trig_Sin(frame * (TRIG_TURN / 64)) *
                         DJINNI_SIGN_DJINNI_SWAY) +
                  CANVAS_SIZE / 2 - DJINNI_WIDTH / 2,
              center[1] - DJINNI_HEIGHT / 2, DJINNI_WIDTH, height);
    }

    Matrix_Initialize();
    gProjection.centerY = 104;
    vertices = Heap_AllocEwram(32);
    model = Render_AllocateModelList(1);
    // 64x64: Rime's seal, or Haze's star.
    texture.logWidth = 6;
    texture.logHeight = 6;
    model->kind = MODEL_SHADED_MAX;
    model->texture = &texture;
    model->mesh = gBattleEffect_SquareMesh;
    model->vertices = vertices;
    if (mode == DJINNI_SIGN_RIME) {
      // Rime's seal fades in over the first eight frames and turns in two
      // mirrored copies that come together at frame 32, shrinking from twice
      // its size.
      s32 brightness = frame * 8 - 64;

      if (brightness > 0)
        brightness = 0;
      // DEAD: the effect ends after frame 47, so this fade-out never runs;
      // the ROM keeps its test and store.
      if (frame > 55)
        brightness = (56 - frame) * 8;
      model->shade = brightness;
      texture.pixels = work->sheet + RIME_SEAL_TEXTURE;
      for (i = 0; i != 2; i++) {
        FIXED scale = FIXED_ONE * 2 - frame * (FIXED_ONE / 32);

        if (frame > 31 && i == 1)
          continue;
        if (scale < FIXED_ONE)
          scale = FIXED_ONE;
        Matrix_Initialize();
        Matrix_RotateX(-DEGtoANG(90));
        if (frame <= 31) {
          if (i == 0)
            Matrix_RotateY((32 - frame) << 9);
          else
            Matrix_RotateY((frame << 9) - DEGtoANG(90));
        }
        Matrix_Scale(scale >> 1, scale >> 1, scale >> 1);
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
      for (i = 0, star = work->particles; i != HAZE_STARS; i++, star++) {
        if (star->variant == 4 && (i & 3) == 0)
          Sound_PlayFar(SE_PSYNERGY_START);
        if (star->variant >= 0 && star->variant < HAZE_STAR_FRAMES) {
          FIXED swell = Trig_Sin(star->variant * (TRIG_TURN / 32)) * star->z;
          s32 size;

          Matrix_Initialize();
          Matrix_Translate(star->x, star->y, 0);
          size = swell >> 8;
          Matrix_RotateX(DEGtoANG(90));
          Matrix_RotateY(frame << 9);
          size /= 2;
          Matrix_Scale(size, size, size);
          Render_TransformVertices(gBattleEffect_GroundSquareCorners, vertices,
                                   4);
          Render_DrawModelList(model);
        }
        star->variant++;
      }
    }
    Heap_Release(model);
    Heap_Release(vertices);
    gProjection.centerX = BATTLE_PROJECTION_CENTER_X;
    gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;

    EFFECT_END_FRAME(work);
  }

  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

/// Abilities: Gasp.
void BattleEffect_RunGasp(struct BattleEffectParams *args) {
  BattleEffect_RunGaspFamily(args, GASP_MODE_GASP);
}

/// Abilities: Reflux.
void BattleEffect_RunReflux(struct BattleEffectParams *args) {
  BattleEffect_RunGaspFamily(args, GASP_MODE_REFLUX);
}

/// Abilities: Djinn Counter.
void BattleEffect_RunDjinnCounter(struct BattleEffectParams *args) {
  BattleEffect_RunGaspFamily(args, GASP_MODE_DJINN_COUNTER);
}

/// Abilities: Counter-rush.
void BattleEffect_RunCounterRush(struct BattleEffectParams *args) {
  BattleEffect_RunGaspFamily(args, GASP_MODE_COUNTER_RUSH);
}

/// Abilities: Mold.
void BattleEffect_RunMold(struct BattleEffectParams *args) {
  BattleEffect_RunGaspFamily(args, GASP_MODE_MOLD);
}

/// Abilities: Meld.
void BattleEffect_RunMeld(struct BattleEffectParams *args) {
  BattleEffect_RunGaspFamily(args, GASP_MODE_MOLD);
}

/// Abilities: Shine.
void BattleEffect_RunShine(struct BattleEffectParams *args) {
  BattleEffect_RunGaspFamily(args, GASP_MODE_SHINE);
}

/// Abilities: Lull.
void BattleEffect_RunLull(struct BattleEffectParams *args) {
  BattleEffect_RunGaspFamily(args, GASP_MODE_LULL);
}

/// Abilities: Demon Eye.
void BattleEffect_RunDemonEye(struct BattleEffectParams *args) {
  BattleEffect_RunGaspFamily(args, GASP_MODE_DEMON_EYE);
}

/// Abilities: Emu, Call Demon, Claw Slash, Mega Slash, Vital Moon.
void BattleEffect_RunEmu(struct BattleEffectParams *args) {
  BattleEffect_RunGaspFamily(args, GASP_MODE_EMU);
}

/// Abilities: Call Dullahan, Formina Sage.
void BattleEffect_RunCallDullahan(struct BattleEffectParams *args) {
  BattleEffect_RunGaspFamily(args, GASP_MODE_CALL_DULLAHAN);
}

/// Abilities: Beat Dance.
void BattleEffect_RunBeatDance(struct BattleEffectParams *args) {
  BattleEffect_RunGaspFamily(args, GASP_MODE_BEAT_DANCE);
}

/// Abilities: Clarion Cry.
void BattleEffect_RunClarionCry(struct BattleEffectParams *args) {
  BattleEffect_RunGaspFamily(args, GASP_MODE_CLARION_CRY);
}

/// Abilities: Element Swap.
void BattleEffect_RunElementSwap(struct BattleEffectParams *args) {
  BattleEffect_RunGaspFamily(args, GASP_MODE_ELEMENT_SWAP);
}

/// Abilities: Psy Boost.
void BattleEffect_RunPsyBoost(struct BattleEffectParams *args) {
  BattleEffect_RunGaspFamily(args, GASP_MODE_PSY_BOOST);
}

/// Abilities: Cage.
void BattleEffect_RunCage(struct BattleEffectParams *args) {
  BattleEffect_RunGaspFamily(args, GASP_MODE_CAGE);
}

#undef DJINNI_SIGN_DJINNI_PICTURE
#undef DJINNI_SIGN_DJINNI_SWAY
#undef RIME_SEAL_TEXTURE
#undef RIME_SEAL_CLEARED_BYTES
#undef HAZE_PLACED_STARS
#undef HAZE_STARS
#undef HAZE_STAR_FRAMES
