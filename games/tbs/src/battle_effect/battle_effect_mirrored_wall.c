/// @file
/// The walls of Berserk, Spasm and Recovery.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

#define MIRRORED_WALL_FRAMES 48 ///< frames of the wall

/// The mirrored wall's modes (BattleEffect_RunMirroredWall), each named for the
/// effect that plays it.
enum MirroredWallMode {
  MIRRORED_WALL_SPASM,   ///< actual size, a gentle shake for 32 frames
  MIRRORED_WALL_BERSERK, ///< 1.25 times as wide, a hard shake for 8 frames
  MIRRORED_WALL_RECOVERY ///< 1.25 times as wide, no shake, in the Fireball
                         ///< colours
};

/// Builds the two usual canvas blitters, the lightening one and the additive
/// one, both drawing mirrored when flip is nonzero.
void BattleEffect_FetchRectangleBlitters(s32 flip, BlitterPair out) {
  if (flip == 0) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                        COMPOSITE_LIGHTEN);
    out[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                        COMPOSITE_ADDITIVE);
    out[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        COMPOSITE_LIGHTEN);
    out[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        COMPOSITE_ADDITIVE);
    out[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
  }
}

/// The wall rises wide and shakes the screen hard.
///
/// Abilities: Berserk.
void BattleEffect_RunBerserk(struct BattleEffectParams *args) {
  BattleEffect_RunMirroredWall(args, MIRRORED_WALL_BERSERK);
}
/// The wall rises with a long gentle shake and the recovery sound.
///
/// Abilities: Spasm.
void BattleEffect_RunSpasm(struct BattleEffectParams *args) {
  BattleEffect_RunMirroredWall(args, MIRRORED_WALL_SPASM);
}
/// A wide wall of fire rises without shaking the screen.
///
/// Abilities: Recovery.
void BattleEffect_RunRecovery(struct BattleEffectParams *args) {
  BattleEffect_RunMirroredWall(args, MIRRORED_WALL_RECOVERY);
}
/// Raises a mirrored wall at the first target's feet while the target glows.
void BattleEffect_RunMirroredWall(struct BattleEffectParams *args, s32 mode) {
  struct BattleEffectState *state;
  void *canvas;
  u8 *resource;
  s32 x;
  struct ScreenPoint screenPosition;
  BlitterPair draw;
  s32 frame;
  s32 picture;

  state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  state->args = args;
  BattleEffect_StartCanvasLayer(0);

  resource = Resource_GetTableEntry(RESOURCE_EARTH_WALL_SHEET);
  Iwram_CopyWords(BG_PLTT, resource, SHEET_PALETTE_BYTES);
  resource += SHEET_PALETTE_BYTES;
  Decompress_TaggedLz(resource, state->sheet);
  resource = Resource_GetTableEntry(RESOURCE_MARS_DJINN_SHEET);
  Iwram_CopyWords(BG_PLTT, resource, SHEET_PALETTE_BYTES);
  if (mode == MIRRORED_WALL_RECOVERY) {
    resource = Resource_GetTableEntry(RESOURCE_FIREBALL_SHEET);
    Iwram_CopyWords(BG_PLTT, resource, SHEET_PALETTE_BYTES);
  }

  Projection_Unit(state->args->targets[0], &screenPosition);
  if (mode == MIRRORED_WALL_SPASM) {
    REG_BG2PA = Q_8_8(1);
    x = CANVAS_SIZE / 2 - screenPosition.x;
    REG_BG2X = x << 8;
  } else {
    REG_BG2PA = Q_8_8(0.8);
    x = (-screenPosition.x * 4) / 5 + CANVAS_SIZE / 2;
    REG_BG2X = x << 8;
  }

  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_LIGHTEN);
  draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  if (mode == MIRRORED_WALL_RECOVERY) {
    state->shakeFrames = 0;
    Sound_PlayFar(SE_POWER_UP);
  } else if (mode == MIRRORED_WALL_BERSERK) {
    state->shakeFrames = 8;
    Sound_PlayFar(SE_POWER_UP);
  } else {
    state->shakeFrames = 32;
  }

  for (frame = 0; frame != MIRRORED_WALL_FRAMES; frame++) {
    if (frame == 0) {
      if (mode == MIRRORED_WALL_RECOVERY)
        BattleEffect_TintBattler(state->args->targets[0], SPRITE_COLOR_WHITE,
                                 TINT_KEEP, 0, 32);
      else
        BattleEffect_TintBattler(state->args->targets[0], SPRITE_COLOR_RED_TINT,
                                 TINT_KEEP, 0, 32);
    }
    if (frame == 24)
      EFFECT_SHOW_DAMAGE(0);
    if (frame == 8 && mode == MIRRORED_WALL_SPASM)
      Sound_PlayFar(SE_RECOVERY);

    // The wall rises through its first three pictures, then flickers between
    // the top two; each half is drawn mirrored about the canvas's middle.
    if (frame <= 31) {
      picture = frame / 4;
      if (picture > 2)
        picture = (picture & 1) + 1;
      if (frame <= 27) {
        draw[0](canvas, state->sheet + gMirroredWall_Pictures[picture],
                CANVAS_SIZE / 2 - gMirroredWall_PictureWidths[picture],
                (screenPosition.y - gMirroredWall_PictureHeights[picture]) + 8,
                gMirroredWall_PictureWidths[picture],
                gMirroredWall_PictureHeights[picture]);
        draw[1](canvas, state->sheet + gMirroredWall_Pictures[picture],
                CANVAS_SIZE / 2,
                (screenPosition.y - gMirroredWall_PictureHeights[picture]) + 8,
                gMirroredWall_PictureWidths[picture],
                gMirroredWall_PictureHeights[picture]);
      }
    }
    if (mode == MIRRORED_WALL_SPASM)
      BattleCamera_Shake(2, 2);
    else
      BattleCamera_Shake(16, 16);
    EFFECT_END_FRAME(state);
  }

  EFFECT_FINISH_CANVAS();
}
