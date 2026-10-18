// Part of effect.c, the battle effect object, compiled only through it.
// The walls of Berserk, Spasm and Recovery.

// The canvas blitters, and the 48-frame wall effects.

// Builds the two usual routines that draw onto the 128x128 canvas, both
// clipped on both axes.
// flip : nonzero draws mirrored.
// out  : [0] keeps the higher colour index, [1] adds the colours (up to 63).
void BattleFx_FetchRectangleBlitters(s32 flip, BlitterPair out) {
  if (flip == 0) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                        COMPOSITE_MAX);
    out[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                        COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                        COMPOSITE_ADDITIVE);
    out[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                        COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X,
                        COMPOSITE_MAX);
    out[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                        COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X,
                        COMPOSITE_ADDITIVE);
    out[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
  }
}

// Abilities: Berserk (kind 353).
//
// Battle effect 353: the wall rises wide and shakes the screen hard.
DEFINE_EFFECT_KIND(BattleFx_RunBerserk, BattleFx_RunMirroredWall, 1)

// Abilities: Spasm (kind 348).
//
// Battle effect 348: the wall rises with a long gentle shake and the
// recovery sound.
DEFINE_EFFECT_KIND(BattleFx_RunSpasm, BattleFx_RunMirroredWall, 0)

// Abilities: Recovery (kind 389).
//
// Battle effect 389: a wide wall of fire, without a shake.
DEFINE_EFFECT_KIND(BattleFx_RunRecovery, BattleFx_RunMirroredWall, 2)

// Raises a symmetrical wall (the EarthWall images) at the first target's
// feet for 48 frames; the target glows and the battle shows the result on
// frame 24.
// mode : 0 actual size, a gentle shake for 32 frames
//        1 1.25 times as wide, a hard shake for 8 frames
//        2 1.25 times as wide, no shake, in the Fireball colours
void BattleFx_RunMirroredWall(struct BattleEffectArgument *effect, s32 mode) {
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  u8 *palette;
  s32 x;
  struct EffectPosition screenPosition;
  BlitterPair draw;
  void **slots;
  s32 frame;
  s32 cell;

  cursor = gBattleFxWork;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);

  // The EarthWall images, coloured with the Mars Djinn palette (the Fireball
  // palette in mode 2).
  palette = Resource_GetTableEntry((s32)&ResourceId_EarthWallSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Resource_DecodeType01(palette, work);
  palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  if (mode == 2) {
    palette = Resource_GetTableEntry((s32)&ResourceId_FireballSheet);
    Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  }

  // Scroll BG2 so the canvas's x 64 is on the target.
  EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actors[0],
                                              &screenPosition);
  if (mode == 0) {
    REG_BG2PA = Q_8_8(1);
    x = 64 - screenPosition.x;
    REG_BG2X = x << 8;
  } else {
    REG_BG2PA = Q_8_8(0.8); // shown 1.25 times as wide
    x = (-screenPosition.x * 4) / 5 + 64;
    REG_BG2X = x << 8;
  }

  // The left half as drawn, the right half mirrored, keeping the higher
  // colour.
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                      COMPOSITE_MAX);
  slots = gWorkSlot;
  draw[0] = slots[HEAP_SLOT_BLITTER];
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X,
                      COMPOSITE_MAX);
  draw[1] = slots[HEAP_SLOT_BLITTER_ALTERNATE];
  work->transferMode = CANVAS_COPY_AND_FADE; // trails keep half a frame
  work->transferValue = 50;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0x480);

  if (mode == 2) {
    work->shakeFrames = 0;
    AudioCommand_PlayFar(SE_212);
  } else if (mode == 1) {
    work->shakeFrames = 8;
    AudioCommand_PlayFar(SE_212);
  } else {
    work->shakeFrames = 32;
  }

  for (frame = 0; frame != 48; frame++) {
    if (frame == 0) {
      if (mode == 2)
        ObjectGroup_UpdateMembers(work->effect->actors[0], SPRITE_COLOR_WHITE,
                                  -1, 0, 32);
      else
        ObjectGroup_UpdateMembers(work->effect->actors[0],
                                  SPRITE_COLOR_RED_TINT, -1, 0, 32);
    }
    if (frame == 24)
      BattleEventRuntime_BeginPhaseFar(0);
    if (frame == 8 && mode == 0)
      AudioCommand_PlayFar(SE_RECOVERY);

    // Images 0, 1 and 2, four frames each, then 1 and 2 by turns, up to
    // frame 27, standing 8 pixels below the target's feet.
    if (frame <= 31) {
      cell = frame / 4;
      if (cell > 2)
        cell = (cell & 1) + 1;
      if (frame <= 27) {
        draw[0](canvas, work->sheet + gEarthWall_CellSourceOffsets[cell],
                64 - gEarthWall_CellWidths[cell],
                (screenPosition.y - gEarthWall_CellHeights[cell]) + 8,
                gEarthWall_CellWidths[cell], gEarthWall_CellHeights[cell]);
        draw[1](canvas, work->sheet + gEarthWall_CellSourceOffsets[cell], 64,
                (screenPosition.y - gEarthWall_CellHeights[cell]) + 8,
                gEarthWall_CellWidths[cell], gEarthWall_CellHeights[cell]);
      }
    }
    if (mode == 0)
      Camera_ApplyShake(2, 2);
    else
      Camera_ApplyShake(16, 16);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}
