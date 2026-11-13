/// @file
/// Poison Beat, Spinning Beat and Spider Web.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

// - The swirl swipe (Poison Beat, Spinning Beat) sweeps a swirl across the
//   first target; its two modes differ in colour, length and how the target
//   reacts.
// - The spider web spreads four mirrored quarters of a web over the targets
//   and fades it in and out.

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
/// A swirl sweeps across the first target from the user's side in six images of
/// four frames each: the first four from the swirl sheet, the last two from the
/// tail sheet. The swipe lands on frame 8.
void BattleEffect_RunSwirlSwipe(struct BattleEffectParams *args, s32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  struct Actor *actor;
  struct ScreenPoint position;
  /* LEFTOVER(frame): draw[1], the idle half of the pair; the ROM's frame
   * reserves 32 bytes (sub sp, #32), 8 more than one blitter needs, and no
   * instruction addresses the word at sp+16; BlitterPair is the blitter set
   * every effect declares, and BattleEffect_FetchRectangleBlitters and 28
   * battle-effect files fill or call its second half; as one DrawRectangle
   * draw[1], 136 of 256 instructions differ in tbs-en. */
  BlitterPair draw;
  s32 frames;
  s32 frame;
  s32 picture;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  work->args = args;
  EFFECT_START_NARROW_CANVAS(0);
  if (work->args->side == EFFECT_SIDE_PARTY) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                        COMPOSITE_TRANSPARENT);
    draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        COMPOSITE_TRANSPARENT);
    draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  }
  Resource_LoadAndDecompress((s32)&ResourceId_MagentaSwirlSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_MagentaTailSheet,
                             Ram_MapGridCellBuffer, 1, 0);
  if (mode == SWIRL_SWIPE_POISON_BEAT) {
    void *palette = Resource_GetTableEntry((s32)&ResourceId_LimePalette);
    Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  }
  // Trails keep 3/4 a frame.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  actor = GetBattleObjectSlotFar(work->args->targets[0])->actor;
  Projection_ScaledUnit(work->args->targets[0], &position);
  // Line the canvas up so the swipe crosses the target.
  if (work->args->side == EFFECT_SIDE_ENEMIES)
    REG_BG2X = (16 - position.x) << 8;
  else
    REG_BG2X = (112 - position.x) << 8;
  frames = 74;
  if (mode != SWIRL_SWIPE_SPINNING_BEAT)
    frames = 48;
  for (frame = 0; frame != frames; frame++) {
    picture = frame / 4;
    if (picture <= 5) {
      if (picture <= 3)
        draw[0](canvas, work->sheet + gSpiderWeb_Pictures[picture],
                gSpiderWeb_PictureX[picture + work->args->side * 6],
                gSpiderWeb_PictureBiasY[picture] + 32,
                gSpiderWeb_PictureWidths[picture],
                gSpiderWeb_PictureHeights[picture]);
      else
        draw[0](canvas, Ram_MapGridCellBuffer + gSpiderWeb_Pictures[picture],
                gSpiderWeb_PictureX[picture + work->args->side * 6],
                gSpiderWeb_PictureBiasY[picture] + 32,
                gSpiderWeb_PictureWidths[picture],
                gSpiderWeb_PictureHeights[picture]);
    }
    if (frame == 8) {
      if (mode == SWIRL_SWIPE_POISON_BEAT) {
        EFFECT_SHOW_DAMAGE(SE_IMPACT);
        BattleMotion_ApplyKnockbackFar(work->args->targets[0], KNOCKBACK_HOP);
      } else {
        Sound_PlayFar(SE_HEAVY_IMPACT);
        BattleEffect_TintBattler(work->args->targets[0], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, 0, 4);
      }
      work->shakeFrames = 8;
    }
    // Spinning Beat throws the target up at 12 pixels a frame on frame 13, and
    // shows the damage as it lands on frame 65.
    if (mode == SWIRL_SWIPE_SPINNING_BEAT) {
      if (frame == 13) {
        actor->velocityY = px(12);
        actor->gravity = px(47) / 100;
        actor->bounce = Q16_ONE / 4;
      }
      if (frame == 65) {
        work->shakeFrames = 4;
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      }
    }
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#define SPIDER_WEB_FRAMES 63

/// A spider web spreads over the targets, drawn as four mirrored 32x32 quarters
/// around the point between the first and last target.
///
/// Abilities: Spider Web.
void BattleEffect_RunSpiderWeb(struct BattleEffectParams *args) {
  struct ScreenPoint first;
  struct ScreenPoint last;
  void *canvas;
  struct BattleEffectWork *work;
  s32 frame;
  s32 i;

  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  work->args = args;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(2));
  REG_BG2PA = Q_8_8(1);
  REG_BLDALPHA = BLEND_ALPHA(0, 16); // invisible to start with
  Projection_ScaledUnit(work->args->targets[0], &first);
  Projection_ScaledUnit(work->args->targets[work->args->targetCount - 1],
                        &last);
  // Centre the canvas between the first and last target.
  first.x += (last.x - first.x) / 2;
  REG_BG2X = (64 - first.x) << 8;
  Resource_LoadAndDecompress((s32)&ResourceId_SpiderWebSheet, work->sheet, 1,
                             1);
  // Redraw from blank each frame.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  Sound_PlayFar(SE_CRACK);
  for (frame = 0; frame != SPIDER_WEB_FRAMES; frame++) {
    if (frame <= 8)
      REG_BLDALPHA = BLEND_ALPHA(frame << 1, 16);
    if (frame > 53)
      REG_BLDALPHA = BLEND_ALPHA(124 - (frame << 1), 16);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                        COMPOSITE_TRANSPARENT);
    EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE](canvas, work->sheet, 33, 41, 32, 32);
    HeapSlot_Release(HEAP_SLOT_RAM_CODE);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        COMPOSITE_TRANSPARENT);
    EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE](canvas, work->sheet, 64, 41, 32, 32);
    HeapSlot_Release(HEAP_SLOT_RAM_CODE);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y,
                        COMPOSITE_TRANSPARENT);
    EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE](canvas, work->sheet, 33, 72, 32, 32);
    HeapSlot_Release(HEAP_SLOT_RAM_CODE);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP,
                        COMPOSITE_TRANSPARENT);
    EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE](canvas, work->sheet, 64, 72, 32, 32);
    HeapSlot_Release(HEAP_SLOT_RAM_CODE);
    if (frame == 32)
      EFFECT_SHOW_DAMAGE(SE_CRACK);
    EFFECT_FOR_EACH_TARGET(i, work->args) {
      if (frame == 10)
        BattleEffect_TintBattler(work->args->targets[i], SPRITE_COLOR_WHITE,
                                 TINT_KEEP, i, 8);
    }
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }
  // Finish: the blitter was freed each time it drew.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

#undef SPIDER_WEB_FRAMES
