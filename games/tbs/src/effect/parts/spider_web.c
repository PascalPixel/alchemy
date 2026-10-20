/// @file
/// Poison Beat, Spinning Beat and Spider Web.
/// Part of effect.c, the battle effect object, compiled only through it.

#include "../effect_internal.h"

// - The swirl swipe (Poison Beat, Spinning Beat) sweeps a swirl across the
//   first target; its two modes differ in colour, length and how the target
//   reacts.
// - The spider web spreads four mirrored quarters of a web over the targets
//   and fades it in and out.

/// A lime swipe that knocks the target back.
///
/// Abilities: Poison Beat (kind 382).
void BattleFx_RunPoisonBeat(struct BattleEffectArgument *effect) {
  BattleFx_RunSwirlSwipe(effect, 0);
}
/// A magenta swipe that throws the target into the air.
///
/// Abilities: Spinning Beat (kind 383).
void BattleFx_RunSpinningBeat(struct BattleEffectArgument *effect) {
  BattleFx_RunSwirlSwipe(effect, 1);
}
/// A swirling swipe across the first target, six images four frames each,
/// from the user's side: the first four from the swirl sheet, the last two
/// from the tail sheet. The swipe lands on frame 8.
///
/// Called by BattleFx_RunPoisonBeat and BattleFx_RunSpinningBeat. Decodes
/// the tail sheet into Ram_MapCellBuffer, moves BG2X so the swipe crosses
/// the target, and sets work->shakeFrames on the hit; mode 0 loads its
/// colours over the first 64 of the background palette, and mode 1 throws
/// the target's object into the air.
///
/// mode 0 : lime colours; the battle shows the damage at once and the target
///          recoils. 48 frames.
/// mode 1 : the target flashes, is thrown up on frame 13 and the battle shows
///          the damage as it lands on frame 65. 74 frames.
void BattleFx_RunSwirlSwipe(struct BattleEffectArgument *effect, s32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  struct MotionObject *object;
  struct EffectPosition position;
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
  work->effect = effect;
  EFFECT_BEGIN_NARROW_CANVAS(0);
  if (work->effect->side == 1) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS,
                        COMPOSITE_TRANSPARENT);
    draw[0] = (DrawRectangle)EFFECT_SLOT(slots, HEAP_SLOT_BLITTER);
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        COMPOSITE_TRANSPARENT);
    draw[0] = (DrawRectangle)EFFECT_SLOT(slots, HEAP_SLOT_BLITTER);
  }
  Resource_LoadAndDecompress((s32)&ResourceId_MagentaSwirlSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_MagentaTailSheet,
                             Ram_MapCellBuffer, 1, 0);
  if (mode == 0) {
    void *palette = Resource_GetTableEntry((s32)&ResourceId_LimePalette);
    Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  }
  // Trails keep 3/4 a frame.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);
  object = GetBattleObjectSlotFar(work->effect->actors[0])->object;
  EffectPosition_ProjectScaledBattler(work->effect->actors[0], &position);
  // Line the canvas up so the swipe crosses the target: the target's x
  // lands at canvas x 16 against the enemies and 112 against the party.
  if (work->effect->side == 0)
    REG_BG2X = (16 - position.x) << 8;
  else
    REG_BG2X = (112 - position.x) << 8;
  frames = 74;
  if (mode != 1)
    frames = 48;
  for (frame = 0; frame != frames; frame++) {
    // Frame loop. The swipe's six images, from x columns chosen by the
    // side (six a side in gSpiderWeb_CellX); the trail fades after them.
    cell = frame / 4;
    if (cell <= 5) {
      if (cell <= 3)
        draw[0](canvas, work->sheet + gSpiderWeb_Cells[cell],
                gSpiderWeb_CellX[cell + work->effect->side * 6],
                gSpiderWeb_CellBiasY[cell] + 32, gSpiderWeb_CellWidths[cell],
                gSpiderWeb_CellHeights[cell]);
      else
        draw[0](canvas, Ram_MapCellBuffer + gSpiderWeb_Cells[cell],
                gSpiderWeb_CellX[cell + work->effect->side * 6],
                gSpiderWeb_CellBiasY[cell] + 32, gSpiderWeb_CellWidths[cell],
                gSpiderWeb_CellHeights[cell]);
    }
    if (frame == 8) {
      if (mode == 0) {
        EFFECT_SHOW_DAMAGE(SE_133);
        BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 1);
      } else {
        AudioCommand_PlayFar(SE_HEAVY_IMPACT);
        ObjectGroup_UpdateMembers(work->effect->actors[0], SPRITE_COLOR_WHITE,
                                  POSE_DAMAGE, 0, 4);
      }
      work->shakeFrames = 8;
    }
    // Mode 1 throws the target up at 12 pixels a frame on frame 13, and
    // shows the damage as it lands on frame 65. Its gravity, 0x7851, takes
    // about 0.47 pixel a frame off its speed each frame, and
    // verticalMotionPhase 0x4000 is the quarter of its speed a bounce keeps.
    if (mode == 1) {
      if (frame == 13) {
        object->velocityY = 0xc0000;
        object->verticalMotionStrength = 0x7851;
        object->verticalMotionPhase = 0x4000;
      }
      if (frame == 65) {
        work->shakeFrames = 4;
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      }
    }
    Camera_ApplyShake(8, 8);
    EFFECT_END_FRAME(work);
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#define SPIDER_WEB_FRAMES 63

/// A spider web spreads over the targets, drawn as four mirrored 32x32 quarters
/// around the point between the first and last target.
///
/// Abilities: Spider Web (kind 345).
///
/// It fades in over 8 frames; the targets flash on frame 10, the battle shows
/// the damage on frame 32, and it fades out from frame 54.
///
/// Builds and frees its blitter four times a frame, one for each quarter's
/// flip.
void BattleFx_RunSpiderWeb(struct BattleEffectArgument *effect) {
  struct EffectPosition first;
  struct EffectPosition last;
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
  size = (s32)&Ram_WorkSlot[HEAP_SLOT_BATTLE_CANVAS];
  canvas = *(void **)size;
  work = *(struct BattleEffectWork **)(size - 4);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(2);
  transparent = (s32)&REG_BLDALPHA;
  REG_BG2PA = Q_8_8(1);
  *(u16 *)transparent = 0x1000; // REG_BLDALPHA: invisible to start with
  EffectPosition_ProjectScaledBattler(work->effect->actors[0], &first);
  EffectPosition_ProjectScaledBattler(
      work->effect->actors[work->effect->count - 1], &last);
  // Centre the canvas between the first and last target.
  first.x += (last.x - first.x) / 2;
  REG_BG2X = (64 - first.x) << 8;
  Resource_LoadAndDecompress((s32)&ResourceId_SpiderWebSheet, work->sheet, 1,
                             1);
  // Redraw from blank each frame.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  AudioCommand_PlayFar(SE_143);
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
      REG_BLDALPHA = BLEND_ALPHA(0x7c - (frame << 1), 16);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, transparent);
    EFFECT_BLITTERS[HEAP_SLOT_BLITTER](canvas, work->sheet, 33, 41, size, size);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, transparent);
    EFFECT_BLITTERS[HEAP_SLOT_BLITTER](canvas, work->sheet, 64, 41, size, size);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, transparent);
    EFFECT_BLITTERS[HEAP_SLOT_BLITTER](canvas, work->sheet, 33, 72, size, size);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP,
                        transparent);
    EFFECT_BLITTERS[HEAP_SLOT_BLITTER](canvas, work->sheet, 64, 72, size, size);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
    if (frame == 32)
      EFFECT_SHOW_DAMAGE(SE_143);
    EFFECT_FOR_EACH_TARGET(i, work->effect) {
      if (frame == 10)
        ObjectGroup_UpdateMembers(work->effect->actors[i], SPRITE_COLOR_WHITE,
                                  -1, i, 8);
    }
    ObjectGroup_TickMemberTimers();
    work->transferPending = transparent;
    WaitFrames(1);
    frame++;
  } while (frame != SPIDER_WEB_FRAMES);
  // Finish: the blitter was freed each time it drew.
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  BattleFx_EndCanvasLayer();
}

#undef SPIDER_WEB_FRAMES
