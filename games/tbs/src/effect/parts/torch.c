// Part of effect.c, the battle effect object, compiled only through it.
// Torch and Kite.

// Torch's rippling disc builds a radial picture by computing one quadrant
// and mirroring it into the other three, then bends it line by line through
// BG2's affine reference while the palette cycles. Each pixel holds its
// distance from the middle, so rotating the palette makes rings run outward.

#define TORCH_FRAMES 96

// Abilities: Torch (kind 214).
//
// A fiery disc fades in, its rings running outward while every scanline
// ripples, then fades out. The disc is a mirrored radial picture whose
// pixels hold their distance from the middle, so rotating the palette
// moves the rings. The scene runs for 96 frames: it fades in over the
// first 9, ripples throughout and fades out over the last 7.
void BattleFx_RunTorch(struct BattleEffectArgument *effect) {
  void **slots;
  struct BattleEffectWork *work;
  void *canvas;
  s32 screenY;
  s32 screenX;
  DrawRectangle draw[2];
  s32 x;
  s32 y;
  s32 dx;
  s32 dy;
  s32 cy;
  s32 d;
  s32 i;
  s32 t;
  s32 r;
  s32 g;
  s32 b;
  s32 frame;
  s32 wave;
  s32 level;
  u16 *pal;
  s32 angle;
  s32 *line;

  slots = gBattleFxWork;
  EFFECT_GET_WORK_AND_CANVAS(slots, work, canvas);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(BGCNT_WRAP);
  BattleFx_RunDjinniEntrance(effect, 6, work->effect->side, 2, &screenX,
                             &screenY);
  REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7) | BGCNT_WRAP;
  REG_BLDALPHA = BLEND_ALPHA(0, 16);
  REG_BG2PA = Q_8_8(1.0 / 1.5);
  BattleFx_FetchRectangleBlitters(work->effect->side, draw);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);

  // Fill the 128x128 sheet with each pixel's distance from the middle,
  // halved and held to 1 to 63, so the distance picks the colour. Rows count
  // 7/8 of a pixel each, so the rings are a little taller than wide. Build
  // one quadrant and mirror it into the other three.
  for (y = 0; y != 64; y++) {
    for (x = 0; x != 64; x++) {
      cy = y / 8 + 64;
      dy = y - cy;
      dx = x - 64;
      d = Iwram_Sqrt(dx * dx + dy * dy);
      d /= 2;
      if (d == 0)
        d = 1;
      if (d > 63)
        d = 63;
      work->sheet[y * 128 + x] = d;
      work->sheet[y * 128 + 127 - x] = d;
      work->sheet[(127 - y) * 128 + x] = d;
      work->sheet[(127 - y) * 128 + 127 - x] = d;
    }
  }

  // Colours 1 to 63: a fiery ramp that brightens to colour 31 and darkens
  // again, written to the background palette and to a copy in
  // Ram_MapCellBuffer that the frame loop rotates.
  pal = (u16 *)Ram_MapCellBuffer;
  for (i = 1; i != 64; i++) {
    if (i > 31)
      t = 64 - i;
    else
      t = i;
    r = t * 9;
    g = t * 7 - 42;
    b = t * 7 - 56;
    if (r < 0)
      r = 0;
    if (g < 0)
      g = 0;
    if (b < 0)
      b = 0;
    if (r > 255)
      r = 255;
    if (g > 255)
      g = 255;
    if (b > 250)
      b = 250;
    r >>= 3;
    g >>= 3;
    b >>= 3;
    pal[i] = ((u16 *)BG_PLTT)[i] = b << 10 | g << 5 | r;
  }

  draw[0](canvas, work->sheet, 0, 0, 128, 128);
  work->transferPending = 1;
  Scheduler_AddOrUpdateCallback(BattleFx_ArmBg2AffineHBlankDma,
                                EFFECT_CALLBACK_ORDER);

  // Fade the radial picture in, ripple each scanline, then fade it out.
  for (frame = 0; frame != TORCH_FRAMES; frame++) {
    if (frame <= 8) {
      level = frame * 2;
      wave = level;
      REG_BLDALPHA = BLEND_ALPHA(level, 16);
    } else {
      wave = frame * 2;
    }
    if (frame > 88)
      REG_BLDALPHA = BLEND_ALPHA(0xc0 - frame * 2, 16);

    // BG2's x reference for each of the 160 scanlines: a sine wave whose
    // phase advances with wave, which the HBlank DMA applies.
    line = work->bg2xLines;
    for (i = 0, angle = -(wave << 9); i != 160; i++) {
      *line++ = ((i << 18) - ((Trig_Sin(angle) * 128)) + 0x40000) >> 10;
      angle += 0x200;
    }

    // Rotate colours 1 to 63 by one, so the rings run outward, and queue
    // them for the background palette. The test against frame 127 never
    // holds within the 96 frames.
    if (frame > 127) {
      work->transferPending = 1;
    } else {
      pal[1] = pal[63];
      Dma_Set(pal + 62, pal + 63, 0x80a0003e, REG_DMA3);
      IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT[1], (u32)(pal + 1), 0x8000003f);
    }
    WaitFrames(1);
  }

  Scheduler_RemoveCallback(BattleFx_ArmBg2AffineHBlankDma);
  EFFECT_FINISH_CANVAS();
}

// Abilities: Kite (kind 217).
//
// Show the extended Jupiter Djinni prelude on the side opposite the effect.
void BattleFx_RunKite(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  s32 originX;
  s32 originY;

  work = WORK_SLOT(HEAP_SLOT_BATTLE_EFFECT);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  BattleFx_RunDjinniEntrance(effect, 7, work->effect->side ^ 1, 0, &originX,
                             &originY);
  BattleFx_EndCanvasLayer();
}

#undef TORCH_FRAMES
