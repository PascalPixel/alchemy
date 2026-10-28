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
/// Abilities: Poison Beat (BATTLE_EFFECT_POISON_BEAT).
void BattleEffect_RunPoisonBeat(struct BattleEffectParams *args) {
  BattleEffect_RunSwirlSwipe(args, SWIRL_SWIPE_POISON_BEAT);
}
/// A magenta swirl sweeps across the first target and throws it into the air.
///
/// Abilities: Spinning Beat (BATTLE_EFFECT_SPINNING_BEAT).
void BattleEffect_RunSpinningBeat(struct BattleEffectParams *args) {
  BattleEffect_RunSwirlSwipe(args, SWIRL_SWIPE_SPINNING_BEAT);
}
/// A swirl sweeps across the first target from the user's side in six images of
/// four frames each: the first four from the swirl sheet, the last two from the
/// tail sheet. The swipe lands on frame 8.
///
/// Called by BattleEffect_RunPoisonBeat and BattleEffect_RunSpinningBeat.
/// Decodes the tail sheet into Ram_MapGridCellBuffer, moves BG2X so the swipe
/// crosses the target, and sets work->shakeFrames on the hit; Poison Beat loads
/// its lime colours over the first 64 of the background palette, and the battle
/// shows its damage at once as the target recoils; in Spinning Beat the
/// target flashes, its actor is thrown up on frame 13 and the battle shows
/// the damage as it lands on frame 65.
///
/// mode: the swipe (enum SwirlSwipeMode).
void BattleEffect_RunSwirlSwipe(struct BattleEffectParams *args, s32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  struct Actor *actor;
  struct ScreenPoint position;
  /* FAKEMATCH(dead): the blitter kept in a pair, whose unused second word
   * fills the frame; as one DrawRectangle, 50 of 256 instructions differ in
   * tbs-en. */
  BlitterPair draw;
  s32 frames;
  s32 frame;
  s32 cell;
  void **slots;
  void **cursor;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  work->args = args;
  EFFECT_START_NARROW_CANVAS(0);
  if (work->args->side == EFFECT_SIDE_PARTY) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                        COMPOSITE_TRANSPARENT);
    draw[0] = (DrawRectangle)EFFECT_SLOT(slots, HEAP_SLOT_RAM_CODE);
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        COMPOSITE_TRANSPARENT);
    draw[0] = (DrawRectangle)EFFECT_SLOT(slots, HEAP_SLOT_RAM_CODE);
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
  // Line the canvas up so the swipe crosses the target: the target's x
  // lands at canvas x 16 against the enemies and 112 against the party.
  if (work->args->side == EFFECT_SIDE_ENEMIES)
    REG_BG2X = (16 - position.x) << 8;
  else
    REG_BG2X = (112 - position.x) << 8;
  frames = 74;
  if (mode != SWIRL_SWIPE_SPINNING_BEAT)
    frames = 48;
  for (frame = 0; frame != frames; frame++) {
    // Frame loop. The swipe's six images, from x columns chosen by the
    // side (six a side in gSpiderWeb_CellX); the trail fades after them.
    cell = frame / 4;
    if (cell <= 5) {
      if (cell <= 3)
        draw[0](canvas, work->sheet + gSpiderWeb_Cells[cell],
                gSpiderWeb_CellX[cell + work->args->side * 6],
                gSpiderWeb_CellBiasY[cell] + 32, gSpiderWeb_CellWidths[cell],
                gSpiderWeb_CellHeights[cell]);
      else
        draw[0](canvas, Ram_MapGridCellBuffer + gSpiderWeb_Cells[cell],
                gSpiderWeb_CellX[cell + work->args->side * 6],
                gSpiderWeb_CellBiasY[cell] + 32, gSpiderWeb_CellWidths[cell],
                gSpiderWeb_CellHeights[cell]);
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
    // shows the damage as it lands on frame 65. Its gravity, 0x7851, takes
    // about 0.47 pixel a frame off its speed each frame, and
    // verticalMotionPhase Q16_ONE / 4 is the quarter of its speed a bounce
    // keeps.
    if (mode == SWIRL_SWIPE_SPINNING_BEAT) {
      if (frame == 13) {
        actor->velocityY = 12 << 16;
        actor->gravity = 0x7851;
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
/// Abilities: Spider Web (BATTLE_EFFECT_SPIDER_WEB).
///
/// It fades in over 8 frames; the targets flash on frame 10, the battle shows
/// the damage on frame 32, and it fades out from frame 54.
///
/// Builds and frees its blitter four times a frame, one for each quarter's
/// flip.
void BattleEffect_RunSpiderWeb(struct BattleEffectParams *args) {
  struct ScreenPoint first;
  struct ScreenPoint last;
  void *canvas;
  struct BattleEffectWork *work;
  s32 size;
  s32 transparent;

  // As a plain local, frame's allocation priority (20 uses over 232
  // instructions) ties with size's and outranks transparent's (13 over 240).
  /* FAKEMATCH(pin): frame fixed to r9, as native; as a plain local it takes sl
   * and transparent r9, and 15 of 233 instructions differ in tbs-en. */
  register s32 frame __asm__("r9");
  s32 i;

  /* FAKEMATCH(temporary): size and transparent first hold the slot table's
   * and REG_BLDALPHA's addresses; as plain slot reads and REG_BLDALPHA, 19 of
   * 233 instructions differ in tbs-en. */
  size = (s32)&Ram_HeapSlots[HEAP_SLOT_BATTLE_CANVAS];
  canvas = *(void **)size;
  work = *(struct BattleEffectWork **)(size - 4);
  work->args = args;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(2));
  transparent = (s32)&REG_BLDALPHA;
  REG_BG2PA = Q_8_8(1);
  *(u16 *)transparent =
      BLEND_ALPHA(0, 16); // REG_BLDALPHA: invisible to start with
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
  // transparent is COMPOSITE_TRANSPARENT, and also the 1 that marks each
  // frame's canvas for transfer.
  frame = 0;
  transparent = 1;
  size = 32;
  do {
    // Frame loop. The quarters meet near (64, 72): top left, top right
    // mirrored, bottom left flipped, bottom right both. The web's share of
    // the blend rises 2 sixteenths a frame to all of it by frame 8, and from
    // frame 55 falls as fast, to none on frame 62.
    if (frame <= 8)
      REG_BLDALPHA = BLEND_ALPHA(frame << 1, 16);
    if (frame > 53)
      REG_BLDALPHA = BLEND_ALPHA(124 - (frame << 1), 16);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, transparent);
    EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE](canvas, work->sheet, 33, 41, size,
                                        size);
    HeapSlot_Release(HEAP_SLOT_RAM_CODE);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, transparent);
    EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE](canvas, work->sheet, 64, 41, size,
                                        size);
    HeapSlot_Release(HEAP_SLOT_RAM_CODE);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, transparent);
    EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE](canvas, work->sheet, 33, 72, size,
                                        size);
    HeapSlot_Release(HEAP_SLOT_RAM_CODE);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP,
                        transparent);
    EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE](canvas, work->sheet, 64, 72, size,
                                        size);
    HeapSlot_Release(HEAP_SLOT_RAM_CODE);
    if (frame == 32)
      EFFECT_SHOW_DAMAGE(SE_CRACK);
    EFFECT_FOR_EACH_TARGET(i, work->args) {
      if (frame == 10)
        BattleEffect_TintBattler(work->args->targets[i], SPRITE_COLOR_WHITE, -1,
                                 i, 8);
    }
    BattleEffect_CountDownBattlerTints();
    work->transferPending = transparent;
    wait(1);
    frame++;
  } while (frame != SPIDER_WEB_FRAMES);
  // Finish: the blitter was freed each time it drew.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

#undef SPIDER_WEB_FRAMES
