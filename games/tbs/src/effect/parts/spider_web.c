// Part of effect.c, the battle effect object, compiled only through it.
// Poison Beat, Spinning Beat and Spider Web.

// Abilities: Poison Beat (kind 382).
//
// Battle effect 382: a lime swipe that knocks the target back.
DEFINE_EFFECT_KIND(BattleFx_RunPoisonBeat, BattleFx_RunSwirlSwipe, 0)

// Abilities: Spinning Beat (kind 383).
//
// Battle effect 383: a magenta swipe that throws the target into the air.
DEFINE_EFFECT_KIND(BattleFx_RunSpinningBeat, BattleFx_RunSwirlSwipe, 1)

// A swirling swipe across the first target, six images four frames each,
// from the user's side: the first four from the swirl sheet, the last two
// from the tail sheet. The swipe lands on frame 8.
// mode 0 : lime colours; the battle shows the damage at once and the target
//          recoils. 48 frames.
// mode 1 : the target flashes, is thrown up on frame 13 and the battle shows
//          the damage as it lands on frame 65. 74 frames.
void BattleFx_RunSwirlSwipe(struct BattleEffectArgument *effect, s32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  struct MotionObject *object;
  struct EffectPosition position;
  /* FAKEMATCH: only draw[0] is used: the second word fills
   * four bytes of the frame */
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
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE,
                        75); // trails keep 3/4 a frame
  object = GetBattleObjectSlotFar(work->effect->actors[0])->object;
  EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &position);
  // Line the canvas up so the swipe crosses the target.
  if (work->effect->side == 0)
    REG_BG2X = (16 - position.x) << 8;
  else
    REG_BG2X = (112 - position.x) << 8;
  frames = 74;
  if (mode != 1)
    frames = 48;
  for (frame = 0; frame != frames; frame++) {
    cell = frame / 4;
    if (cell <= 5) {
      if (cell <= 3)
        draw[0](canvas, work->sheet + gSwirlSwipe_Cells[cell],
                gSwirlSwipe_CellX[cell + work->effect->side * 6],
                gSwirlSwipe_CellBiasY[cell] + 32, gSwirlSwipe_CellWidths[cell],
                gSwirlSwipe_CellHeights[cell]);
      else
        draw[0](canvas, Ram_MapCellBuffer + gSwirlSwipe_Cells[cell],
                gSwirlSwipe_CellX[cell + work->effect->side * 6],
                gSwirlSwipe_CellBiasY[cell] + 32, gSwirlSwipe_CellWidths[cell],
                gSwirlSwipe_CellHeights[cell]);
    }
    if (frame == 8) {
      if (mode == 0) {
        BattleEventRuntime_BeginPhaseFar(SE_133);
        BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 1);
      } else {
        AudioCommand_PlayFar(SE_HEAVY_IMPACT);
        ObjectGroup_UpdateMembers(work->effect->actors[0], SPRITE_COLOR_WHITE,
                                  POSE_DAMAGE, 0, 4);
      }
      work->shakeFrames = 8;
    }
    if (mode == 1) {
      if (frame == 13) {
        object->velocityY = 0xc0000;
        object->verticalMotionStrength = 0x7851;
        object->verticalMotionPhase = 0x4000;
      }
      if (frame == 65) {
        work->shakeFrames = 4;
        BattleEventRuntime_BeginPhaseFar(SE_HEAVY_IMPACT);
      }
    }
    Camera_ApplyShake(8, 8);
    EFFECT_END_FRAME(work);
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#define SPIDER_WEB_FRAMES 63 // frames in the effect

// Abilities: Spider Web (kind 345).
//
// Battle effect 345: a spider web spreads over the targets, drawn as four
// mirrored 32x32 quarters around the point between the first and last
// target. It fades in over 8 frames; the targets flash on frame 10, the
// battle shows the damage on frame 32, and it fades out from frame 54.
void BattleFx_RunSpiderWeb(struct BattleEffectArgument *effect) {
  struct EffectPosition first;
  struct EffectPosition last;
  void *canvas;
  struct BattleEffectWork *work;
  s32 size;
  s32 transparent;

  // Plain, the counter's priority (20 refs over 232 insns) ties size and
  // outranks transparent (13 over 240).
  /* FAKEMATCH: pins the frame counter to r9, as native; plain it takes sl and
   * transparent r9 (differ 30), and no plain shape ranks transparent above it
   * (for/while/u32, declaration order, a bound variable, early init, argument
   * use, 19k permuter candidates). */
  register s32 frame __asm__("r9");
  s32 i;

  /* FAKEMATCH: size and transparent first hold the slot
   * table and blend register addresses, sharing their registers 2026-10-11:
   * plain slot reads and REG_BLDALPHA differ 56; the reads alone 40, the
   * register alone 50. */
  size = (s32)&Ram_WorkSlot[HEAP_SLOT_BATTLE_CANVAS];
  canvas = *(void **)size;
  work = *(struct BattleEffectWork **)(size - 4);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(2);
  transparent = (s32)&REG_BLDALPHA;
  REG_BG2PA = Q_8_8(1);
  *(u16 *)transparent = 0x1000; // REG_BLDALPHA: invisible to start with
  EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &first);
  EffectPosition_ApplyStepAndYOffset(
      work->effect->actors[work->effect->count - 1], &last);
  first.x += (last.x - first.x) / 2; // centre the canvas between them
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
    if (frame <= 8)
      REG_BLDALPHA = (frame << 1) | 0x1000;
    if (frame > 53)
      REG_BLDALPHA = (0x7c - (frame << 1)) | 0x1000;
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, transparent);
    ((DrawRectangle)Ram_WorkSlot[HEAP_SLOT_BLITTER])(canvas, work, 33, 41, size,
                                                     size);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, transparent);
    ((DrawRectangle)Ram_WorkSlot[HEAP_SLOT_BLITTER])(canvas, work, 64, 41, size,
                                                     size);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, transparent);
    ((DrawRectangle)Ram_WorkSlot[HEAP_SLOT_BLITTER])(canvas, work, 33, 72, size,
                                                     size);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP,
                        transparent);
    ((DrawRectangle)Ram_WorkSlot[HEAP_SLOT_BLITTER])(canvas, work, 64, 72, size,
                                                     size);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
    if (frame == 32)
      BattleEventRuntime_BeginPhaseFar(SE_143);
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
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  BattleFx_EndCanvasLayer();
}
