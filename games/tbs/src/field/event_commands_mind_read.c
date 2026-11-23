/// @file
/// Mind Read: scanline waves, shifting colours and flowing particles.
// Part of event_commands.c.

#include "event_commands_internal.h"

/// The hue circle, in whole degrees and in degrees as 16.16.
#define HUE_DEGREES 360
#define HUE_CIRCLE toFIXED(HUE_DEGREES)

/// A line's offset in Mind Read's wave at a phase: a sine 4 pixels either way,
/// a turn every 160 phase steps.
#define MIND_READ_WAVE_OFFSET(phase)                                           \
  (Trig_Sin((u32)((phase) << 16) / (u32)160) >> 14)
/// The wave's cycle runs to this count and starts again, 61 frames; at counts
/// 0, 8 and 16 a particle flies from the target to the caster.
#define MIND_READ_CYCLE_LAST 60
#define MIND_READ_PARTICLE_GAP 8
/// The tint's channels run from this level, a quarter of the hue's own on top.
#define MIND_READ_TINT_LEVEL_MIN 4

/// The HEAP_SLOT_MIND_READ heap slot (gMindReadWork), 664 bytes.
struct MindReadWork {
  /// Each scanline's BG0 horizontal scroll in pixels, two pages written in
  /// turn; 160 of each page's 162 are used.
  u16 lines[2][162];
  u16 phase; ///< the wave's phase, 4 more a frame
  u8 page;   ///< the page shown (the HBlank DMA's); the other is written
  s8 red;    ///< the hue as a colour (red, green, blue, 4 to 11 each)
  s8 green;
  s8 blue;
  u16 hue;      ///< hue, 0 to 359 degrees
  u16 sourceId; ///< the caster's actor id
  u16 targetId; ///< the target's actor id
  u8 delay;     ///< frames before it starts
  u8 counter;   ///< particle spawn cycle (0 to 60)
};

/// Starts the wave effect of casterId on targetId (actor ids): fades the
/// palettes toward a hue that shifts with the frame counter, and starts its
/// per-frame update (FieldPsynergy_UpdateMindReadWave) after 8 frames.
void FieldPsynergy_StartMindReadWave(s32 casterId, s32 targetId) {
  struct MindReadWork *work;

  work = HeapSlot_AllocEwram(HEAP_SLOT_MIND_READ, sizeof(struct MindReadWork));
  FieldPsynergy_FadeToCastColor();
  Dma_Fill32(0, work, sizeof(struct MindReadWork));
  work->hue = ((u32)(gFrameCount << 1) % (u32)HUE_DEGREES);
  FieldEffect_AdvanceHueCycle();
  // PALETTE_TINT_CUSTOM: the BG colours tinted by the hue.
  PaletteFade_SetTarget(RGB(work->red, work->green, work->blue) |
                            PALETTE_TINT_CUSTOM,
                        PALETTES_BG);
  PaletteFade_StartInterpolation(8);
  work->sourceId = casterId;
  work->targetId = targetId;
  work->delay = 8;
  FieldPsynergy_GreyMindReadPalette();
  Task_Add(FieldPsynergy_UpdateMindReadWave, TASK_PASS_FRAME);
}

/// Stops the wave effect: restores the caster's colours and the palettes and
/// releases the Psynergy's work.
void FieldPsynergy_StopMindReadWave(void) {
  struct MindReadWork *work = gMindReadWork;

  FieldPsynergy_RestoreMindReadPalette();
  Task_Remove(FieldPsynergy_UpdateMindReadWave);
  // Back to SPRITE_COLOR_NORMAL.
  Actor_SetSpritePalette(Actor_GetById(work->sourceId), 1);
  FieldPsynergy_FadeFromCastColor();
  HeapSlot_Release(HEAP_SLOT_MIND_READ);
}

/// Task of the wave effect: once its delay has run out, writes
/// the next page of scanline offsets (a sine wave of 4 pixels either way),
/// steps the colour fade and the hue every other frame, keeps the caster's
/// colours cycling, and at the start of each 61-frame cycle sends three
/// particles (SPRITE_CRESCENT), 8 frames apart, from the target to the caster,
/// with SE_PSYNERGY_START.
void FieldPsynergy_UpdateMindReadWave(void) {
  struct MindReadWork *work = gMindReadWork;
  u16 *line;
  u32 scanline;
  struct Actor *caster;
  struct Actor *target;
  struct Actor *particle;
  struct WaveEffectSprite *sprite;
  struct Vector3 from;
  struct Vector3 to;
  struct AnimationMetadata *meta;

  if (work->delay != 0) {
    work->delay--;
    return;
  }

  // The scanlines' offsets on the hidden page; then show that page.
  line = work->lines[work->page ^ 1];
  for (scanline = 0; scanline < DISPLAY_HEIGHT; scanline++)
    *line++ = MIND_READ_WAVE_OFFSET(work->phase + scanline * 8);
  work->phase += 4;
  work->page ^= 1;
  // Every other frame the tint takes the next hue, in one frame.
  if (work->page != 0) {
    PaletteFade_SetTarget(RGB(work->red, work->green, work->blue) |
                              PALETTE_TINT_CUSTOM,
                          PALETTES_BG);
    PaletteFade_StartInterpolation(1);
    FieldEffect_AdvanceHueCycle();
  }
  // The caster's colours cycle.
  Actor_SetSpritePalette(Actor_GetById(work->sourceId), 0);

  if (work->counter == 0 || work->counter == MIND_READ_PARTICLE_GAP ||
      work->counter == 2 * MIND_READ_PARTICLE_GAP) {
    caster = Actor_GetById(work->sourceId);
    target = Actor_GetById(work->targetId);
    if (caster != 0 && target != 0) {
      // From 2 pixels below each one's sprite height (its metadata's boxX).
      from.x = caster->x.fixed;
      meta = Resource_GetMetadataRecordFar(
          (s16)Scene_FindActorPlacement(work->sourceId)->sprite);
      from.y = caster->y.fixed + toFIXED(meta->boxX) - toFIXED(2);
      from.z = caster->z.fixed;
      to.x = target->x.fixed;
      meta = Resource_GetMetadataRecordFar(
          (s16)Scene_FindActorPlacement(work->targetId)->sprite);
      to.y = target->y.fixed + toFIXED(meta->boxX) - toFIXED(2);
      to.z = target->z.fixed;
      particle = Actor_CreateFar(SPRITE_CRESCENT, to.x, to.y, to.z);
      if (particle != 0) {
        sprite = (struct WaveEffectSprite *)particle->sprite;
        particle->motionFlags = 0;
        particle->speedLimit = toFIXED(16) / 25; // 0.64 pixels a frame
        particle->acceleration = toFIXED(16) / 25;
        particle->facing = Trig_Atan(from.z - to.z, from.x - to.x);
        particle->update = FieldEffect_ReleaseOnArrival;
        // No shadow or outline, sprite priority 1.
        sprite->flags = 0;
        sprite->priority = 1;
        Actor_SetMoveTargetFar(particle, from.x, from.y, from.z);
      }
    }
  }

  if (work->counter == 0)
    Sound_PlayFar(SE_PSYNERGY_START);
  if (++work->counter > MIND_READ_CYCLE_LAST)
    work->counter = 0;
}

// What Mind Read's wave uses: BG0's line scroll, the hue cycle and BG
// WINDOW_PALETTE.

/// The grey Mind Read writes over the window palette's white, and its black and
/// the colours after it.
#define MIND_READ_GREY RGB(28, 28, 28)

/// Task: restarts the HBlank DMA that writes BG0's horizontal
/// scroll for each scanline from the wave's shown page.
void FieldEffect_ArmBg0HBlankDma(void) {
  struct MindReadWork *work = gMindReadWork;
  if (!work->delay) {
    u16 *line = work->lines[work->page];
    volatile u16 *dma = REG_DMA0;
    dma[5] &= ~DMA_START_TIMING_DRQ_REPEAT;
    dma[5] &= ~DMA_ENABLE;
    (void)dma[5];
    // One halfword at each HBlank (repeating, destination reloaded).
    Dma_Set(line, REG_BG0HOFS, DMA_HBLANK_HALFWORD_PER_LINE,
            (volatile u32 *)dma);
  }
}

/// Moves the wave's hue on 4 degrees and works out its colour, each channel
/// 4 to 11.
void FieldEffect_AdvanceHueCycle(void) {
  struct MindReadWork *work = gMindReadWork;
  s32 r = 0;
  s32 g = 0;
  s32 b = 0;
  s32 offset;
  u8 *pr;
  u8 *pg;
  u8 *pb;

  FieldEffect_CalculateHueChannels(INTtoFIXED(work->hue), &r, &g, &b);

  // Squeeze 0 to 31 into 4 to 11; offset walks red, green and blue.
  offset = offsetof(struct MindReadWork, red);
  pr = (u8 *)work + offset;
  *pr = (FIXEDtoINT(r) >> 2) + MIND_READ_TINT_LEVEL_MIN;
  offset += 1;
  pg = (u8 *)work + offset;
  *pg = (FIXEDtoINT(g) >> 2) + MIND_READ_TINT_LEVEL_MIN;
  offset += 1;
  pb = (u8 *)work + offset;
  *pb = (FIXEDtoINT(b) >> 2) + MIND_READ_TINT_LEVEL_MIN;

  work->hue += 4;

  *pr &= RGB_LEVEL_MAX;
  *pg &= RGB_LEVEL_MAX;
  *pb &= RGB_LEVEL_MAX;

  if (work->hue >= HUE_DEGREES) {
    work->hue = 0;
  }
}

/// Works out red, green and blue (0 to 31 each, 16.16) for a hue in degrees
/// (16.16).
void FieldEffect_CalculateHueChannels(FIXED hue, FIXED *r, FIXED *g, FIXED *b) {
  *r = FieldEffect_HueChannelRamp(hue + HUE_CIRCLE / 3, 0,
                                  FIXED_ONE * RGB_LEVEL_MAX);
  *g = FieldEffect_HueChannelRamp(hue, 0, FIXED_ONE * RGB_LEVEL_MAX);
  *b = FieldEffect_HueChannelRamp(hue - HUE_CIRCLE / 3, 0,
                                  FIXED_ONE * RGB_LEVEL_MAX);
}

/// One colour channel's strength at a hue angle: rising from 0 to 60 degrees,
/// high from 60 to 180, falling from 180 to 240, low elsewhere.
s32 FieldEffect_HueChannelRamp(s32 angle, s32 low, FIXED high) {
  angle = FixedPoint_Remainder(angle, HUE_CIRCLE);
  if (angle < HUE_CIRCLE / 6)
    return Iwram_RatioMulQ14(HUE_CIRCLE / 6, Iwram_MulFX(high, angle));
  if (angle >= HUE_CIRCLE / 6 && angle < HUE_CIRCLE / 2)
    return high;
  if (angle >= HUE_CIRCLE / 2 && angle < HUE_CIRCLE * 2 / 3)
    return Iwram_RatioMulQ14(HUE_CIRCLE / 6,
                             Iwram_MulFX(high, HUE_CIRCLE * 2 / 3 - angle));
  return low;
}

/// The 16.16 remainder of value divided by divisor (0 for a zero divisor).
FIXED FixedPoint_Remainder(FIXED value, FIXED divisor) {
  FIXED quot;

  if (divisor == 0)
    return 0;
  if (divisor & 0xf0000000)
    divisor = -divisor;
  quot = Iwram_RatioMulQ14(divisor, value);
  // Less the quotient's whole part times divisor.
  return value - Iwram_MulFX(quot & 0xffff0000, divisor);
}

/// Per-frame update: once the actor has arrived (no destination left on any
/// axis), releases it: gFieldEffect_UntargetedActorScript is a lone SC_RELEASE.
void FieldEffect_ReleaseOnArrival(struct Actor *actor) {
  s32 targetY;
  s32 targetX;

  targetX = actor->targetX;
  if (targetX == ACTOR_NO_TARGET) {
    targetY = actor->targetY;
    if ((targetY == targetX) && (actor->targetZ == targetY)) {
      Actor_StartScriptFar(actor, gFieldEffect_UntargetedActorScript);
    }
  }
}

/// Turns WINDOW_PALETTE's white, black, window colours and border greys one
/// grey, and starts the BG0 line scroll (FieldEffect_ArmBg0HBlankDma) at each
/// VBlank.
void FieldPsynergy_GreyMindReadPalette(void) {
  volatile u16 *p;

  // Window mode 1: messages show as thoughts until
  // FieldPsynergy_RestoreMindReadPalette clears it.
  WINDOW_WORK->mindRead = 1;
  p = &BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_WHITE_COLOR);
  *p = MIND_READ_GREY;
  p += WINDOW_BLACK_COLOR - WINDOW_WHITE_COLOR;
  *p = MIND_READ_GREY;
  p += 1;
  *p = MIND_READ_GREY;
  p += 1;
  *p = MIND_READ_GREY;
  p += 1;
  *p = MIND_READ_GREY;
  p += 1;
  *p = MIND_READ_GREY;
  p += 1;
  *p = MIND_READ_GREY;
  p += 1;
  *p = MIND_READ_GREY;
  p += 1;
  *p = MIND_READ_GREY;
  p += 1;
  *p = MIND_READ_GREY;
  p += 1;
  *p = MIND_READ_GREY;
  Task_Add(FieldEffect_ArmBg0HBlankDma, TASK_PASS_VBLANK);
}

/// Stops the BG0 line scroll and restores WINDOW_PALETTE.
void FieldPsynergy_RestoreMindReadPalette(void) {
  void *work;

  work = gWindowWork[0];
  Task_Remove(FieldEffect_ArmBg0HBlankDma);
  BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_WHITE_COLOR) = RGB_WHITE;
  BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BLACK_COLOR) = RGB(0, 0, 0);
  BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BORDER_DARK_COLOR) = RGB(10, 10, 10);
  BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BORDER_LIGHT_COLOR) = RGB(20, 20, 20);
  WindowColor_ApplyFar(gMainRoutineWork.windowHue,
                       gMainRoutineWork.windowBrightness);
  ((struct WindowWork *)work)->mindRead = 0;
}

#undef MIND_READ_GREY
#undef MIND_READ_WAVE_OFFSET
#undef MIND_READ_CYCLE_LAST
#undef MIND_READ_PARTICLE_GAP
#undef MIND_READ_TINT_LEVEL_MIN

#undef HUE_DEGREES
#undef HUE_CIRCLE
