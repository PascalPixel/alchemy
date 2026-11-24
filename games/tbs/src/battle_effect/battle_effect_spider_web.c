/// @file
/// Poison Beat, Spinning Beat and Spider Web.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

/// The swirl swipe's modes (BattleEffect_RunSwirlSwipe), each named for the
/// effect that plays it.
enum SwirlSwipeMode {
  SWIRL_SWIPE_POISON_BEAT,  ///< lime; the target recoils at once, 48 frames
  SWIRL_SWIPE_SPINNING_BEAT ///< magenta; the target is thrown up, 74 frames
};

/// A lime swirl sweeps across the first target and knocks it back.
///
/// Abilities: Poison Beat.
void BattleEffect_RunPoisonBeat(struct BattleEffectParams *args) {
  BattleEffect_RunSwirlSwipe(args, SWIRL_SWIPE_POISON_BEAT);
}
/// A magenta swirl sweeps across the first target and throws it into the air.
///
/// Abilities: Spinning Beat.
void BattleEffect_RunSpinningBeat(struct BattleEffectParams *args) {
  BattleEffect_RunSwirlSwipe(args, SWIRL_SWIPE_SPINNING_BEAT);
}
/// A swirl sweeps across the first target from the user's side.
void BattleEffect_RunSwirlSwipe(struct BattleEffectParams *args, s32 mode) {
  struct BattleEffectState *state;
  void *canvas;
  struct Actor *actor;
  struct ScreenPoint position;
  /* LEFTOVER(frame): draw[1], the idle half of the pair. */
  BlitterPair draw;
  s32 frames;
  s32 frame;
  s32 picture;

  state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  state->args = args;
  EFFECT_START_NARROW_CANVAS(0);
  if (state->args->side == EFFECT_SIDE_PARTY) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                        COMPOSITE_TRANSPARENT);
    draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        COMPOSITE_TRANSPARENT);
    draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  }
  Resource_LoadAndDecompress(RESOURCE_MAGENTA_SWIRL_SHEET, state->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_MAGENTA_TAIL_SHEET, Ram_MapGridCellBuffer,
                             1, 0);
  if (mode == SWIRL_SWIPE_POISON_BEAT) {
    void *palette = Resource_GetTableEntry(RESOURCE_LIME_PALETTE);
    Iwram_CopyWords(BG_PLTT, palette, SHEET_PALETTE_BYTES);
  }
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE,
                        CANVAS_FADE_THREE_QUARTERS);
  actor = GetBattleObjectSlotFar(state->args->targets[0])->actor;
  Projection_ScaledUnit(state->args->targets[0], &position);
  if (state->args->side == EFFECT_SIDE_ENEMIES)
    REG_BG2X = (16 - position.x) << 8;
  else
    REG_BG2X = (112 - position.x) << 8;
  frames = 74;
  if (mode != SWIRL_SWIPE_SPINNING_BEAT)
    frames = 48;
  for (frame = 0; frame != frames; frame++) {
    // The swirl's four pictures, then the two of its tail, four frames each.
    picture = frame / 4;
    if (picture <= 5) {
      if (picture <= 3)
        draw[0](canvas, state->sheet + gSpiderWeb_Pictures[picture],
                gSpiderWeb_PictureX[picture + state->args->side * 6],
                gSpiderWeb_PictureBiasY[picture] + 32,
                gSpiderWeb_PictureWidths[picture],
                gSpiderWeb_PictureHeights[picture]);
      else
        draw[0](canvas, Ram_MapGridCellBuffer + gSpiderWeb_Pictures[picture],
                gSpiderWeb_PictureX[picture + state->args->side * 6],
                gSpiderWeb_PictureBiasY[picture] + 32,
                gSpiderWeb_PictureWidths[picture],
                gSpiderWeb_PictureHeights[picture]);
    }
    if (frame == 8) {
      if (mode == SWIRL_SWIPE_POISON_BEAT) {
        EFFECT_SHOW_DAMAGE(SE_IMPACT);
        BattleMotion_ApplyKnockbackFar(state->args->targets[0], KNOCKBACK_HOP);
      } else {
        Sound_PlayFar(SE_HEAVY_IMPACT);
        BattleEffect_TintBattler(state->args->targets[0], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, 0, 4);
      }
      state->shakeFrames = 8;
    }
    // Spinning Beat throws the target into the air.
    if (mode == SWIRL_SWIPE_SPINNING_BEAT) {
      if (frame == 13) {
        actor->velocityY = toFIXED(12);
        actor->gravity = toFIXED(47) / 100;
        actor->bounce = FIXED_ONE / 4;
      }
      if (frame == 65) {
        state->shakeFrames = 4;
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      }
    }
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(state);
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#define SPIDER_WEB_FRAMES 63
/// SpiderWebSheet's quarter of the web, drawn four times mirrored.
#define SPIDER_WEB_QUARTER_WIDTH 32
#define SPIDER_WEB_QUARTER_HEIGHT 32
/// The web's middle on the canvas, which the canvas puts over the targets'
/// middle; the quarters to its left and above take its column and row too.
#define SPIDER_WEB_CENTER_X (CANVAS_SIZE / 2)
#define SPIDER_WEB_CENTER_Y 72
#define SPIDER_WEB_LEFT (SPIDER_WEB_CENTER_X - SPIDER_WEB_QUARTER_WIDTH + 1)
#define SPIDER_WEB_TOP (SPIDER_WEB_CENTER_Y - SPIDER_WEB_QUARTER_HEIGHT + 1)

/// A spider web spreads over the targets in four mirrored quarters.
///
/// Abilities: Spider Web.
void BattleEffect_RunSpiderWeb(struct BattleEffectParams *args) {
  struct ScreenPoint first;
  struct ScreenPoint last;
  void *canvas;
  struct BattleEffectState *state;
  s32 frame;
  s32 i;

  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  state->args = args;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(2));
  REG_BG2PA = Q_8_8(1);
  REG_BLDALPHA = EFFECT_BLEND_CANVAS(0); // invisible to start with
  Projection_ScaledUnit(state->args->targets[0], &first);
  Projection_ScaledUnit(state->args->targets[state->args->targetCount - 1],
                        &last);
  first.x += (last.x - first.x) / 2;
  REG_BG2X = (SPIDER_WEB_CENTER_X - first.x) << 8;
  Resource_LoadAndDecompress(RESOURCE_SPIDER_WEB_SHEET, state->sheet, 1, 1);
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
  Sound_PlayFar(SE_CRACK);
  for (frame = 0; frame != SPIDER_WEB_FRAMES; frame++) {
    if (frame <= 8)
      REG_BLDALPHA = EFFECT_BLEND_CANVAS(frame << 1);
    if (frame > 53)
      REG_BLDALPHA = EFFECT_BLEND_CANVAS(124 - (frame << 1));
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                        COMPOSITE_TRANSPARENT);
    EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE](
        canvas, state->sheet, SPIDER_WEB_LEFT, SPIDER_WEB_TOP,
        SPIDER_WEB_QUARTER_WIDTH, SPIDER_WEB_QUARTER_HEIGHT);
    HeapSlot_Release(HEAP_SLOT_RAM_CODE);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        COMPOSITE_TRANSPARENT);
    EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE](
        canvas, state->sheet, SPIDER_WEB_CENTER_X, SPIDER_WEB_TOP,
        SPIDER_WEB_QUARTER_WIDTH, SPIDER_WEB_QUARTER_HEIGHT);
    HeapSlot_Release(HEAP_SLOT_RAM_CODE);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y,
                        COMPOSITE_TRANSPARENT);
    EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE](
        canvas, state->sheet, SPIDER_WEB_LEFT, SPIDER_WEB_CENTER_Y,
        SPIDER_WEB_QUARTER_WIDTH, SPIDER_WEB_QUARTER_HEIGHT);
    HeapSlot_Release(HEAP_SLOT_RAM_CODE);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP,
                        COMPOSITE_TRANSPARENT);
    EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE](
        canvas, state->sheet, SPIDER_WEB_CENTER_X, SPIDER_WEB_CENTER_Y,
        SPIDER_WEB_QUARTER_WIDTH, SPIDER_WEB_QUARTER_HEIGHT);
    HeapSlot_Release(HEAP_SLOT_RAM_CODE);
    if (frame == 32)
      EFFECT_SHOW_DAMAGE(SE_CRACK);
    EFFECT_FOR_EACH_TARGET(i, state->args) {
      if (frame == 10)
        BattleEffect_TintBattler(state->args->targets[i], SPRITE_COLOR_WHITE,
                                 TINT_KEEP, i, 8);
    }
    EFFECT_END_FRAME(state);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

#undef SPIDER_WEB_FRAMES
#undef SPIDER_WEB_CENTER_X
#undef SPIDER_WEB_CENTER_Y
#undef SPIDER_WEB_LEFT
#undef SPIDER_WEB_TOP
#undef SPIDER_WEB_QUARTER_WIDTH
#undef SPIDER_WEB_QUARTER_HEIGHT
