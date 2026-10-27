/// @file
/// Mind Read: scanline waves, shifting colours and flowing particles.
// Part of event_commands.c.

#include "../event_commands_internal.h"

/// The HEAP_SLOT_MIND_READ heap slot (gMindReadWork), 0x298 bytes. It lives
/// from FieldPsynergy_StartMindReadWave to
/// FieldPsynergy_FinishWaveAndReleaseHeapBlock, after the conversation Mind
/// Read reads.
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
  u16 sourceId; ///< the caster
  u16 targetId; ///< the target
  u8 delay;     ///< frames before it starts
  u8 counter;   ///< particle spawn cycle (0 to 60)
};

/// Starts the wave effect of caster x on target y (actor ids): fades the
/// palettes toward a hue that shifts with the frame counter, and starts its
/// per-frame update (FieldPsynergy_UpdateMindReadWave) after 8 frames.
///
/// Abilities: Mind Read (FIELD_PSYNERGY_MIND_READ).
///
/// The Psynergy stays up until FieldPsynergy_FinishMindRead ends it
/// after the after event; meanwhile the message windows show thoughts
/// (FieldPsynergy_GreyMindReadPalette sets the window mode).
void FieldPsynergy_StartMindReadWave(s32 x, s32 y) {
  struct MindReadWork *work;

  work = AllocHeapSlotEwram(HEAP_SLOT_MIND_READ, 0x298);
  FieldPsynergy_FadeToCastColor();
  Dma_Fill32(0, work, 0x298);
  work->hue = ((u32)(gFrameCount << 1) % (u32)360);
  FieldEffect_AdvanceHueCycle();
  // 0x200000 is PALETTE_TINT_CUSTOM: the BG colours tinted by the hue.
  PaletteFade_SetTarget(RGB(work->red, work->green, work->blue) | 0x200000, 1);
  PaletteFade_StartInterpolation(8);
  work->sourceId = x;
  work->targetId = y;
  work->delay = 8;
  FieldPsynergy_GreyMindReadPalette();
  Task_Add(FieldPsynergy_UpdateMindReadWave, TASK_PASS_FRAME);
}

/// Stops the wave effect: restores the caster's colours and the palettes and
/// releases the Psynergy's work.
void FieldPsynergy_FinishWaveAndReleaseHeapBlock(void) {
  void *work;

  work = *(void **)((u32)&gMindReadWork);
  FieldPsynergy_RestoreMindReadPalette();
  Task_Remove(FieldPsynergy_UpdateMindReadWave);
  Anim_ApplyChildPalette(Actor_GetById(((struct MindReadWork *)work)->sourceId),
                         1);
  FieldPsynergy_FadeFromCastColor();
  FreeHeapSlot(HEAP_SLOT_MIND_READ);
}

/// Task of the wave effect: once its delay has run out, writes
/// the next page of scanline offsets (a sine wave of 4 pixels either way),
/// steps the colour fade and the hue, keeps the caster's colours lit, and
/// every 60 frames sends three particles (sprite 281) from the target to the
/// caster, with sound 130.
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

  // The 160 scanlines' offsets, a sine wave of 4 pixels either way (a 16.16
  // sine >> 14), on the hidden page; then show that page.
  line = work->lines[work->page ^ 1];
  for (scanline = 0; scanline < DISPLAY_HEIGHT; scanline++)
    *line++ =
        Trig_Sin(((u32)((work->phase + scanline * 8) << 16) / (u32)160)) >> 14;
  work->phase += 4;
  work->page ^= 1;
  // Every other frame the tint takes the next hue, in one frame.
  if (work->page != 0) {
    PaletteFade_SetTarget(RGB(work->red, work->green, work->blue) | 0x200000,
                          1);
    PaletteFade_StartInterpolation(1);
    FieldEffect_AdvanceHueCycle();
  }
  Anim_ApplyChildPalette(Actor_GetById(work->sourceId), 0);

  // At steps 0, 8 and 16 of the cycle, a particle from the target to the
  // caster.
  if (work->counter == 0 || work->counter == 8 || work->counter == 16) {
    caster = Actor_GetById(work->sourceId);
    target = Actor_GetById(work->targetId);
    if (caster != 0 && target != 0) {
      // From 2 pixels below each one's sprite height (its metadata's boxX).
      from.x = caster->x.fixed;
      meta = Resource_GetMetadataRecordFar(
          (s16)Scene_FindActorTemplate(work->sourceId)->sprite);
      from.y = caster->y.fixed + ((s8)meta->boxX << 16) - (2 << 16);
      from.z = caster->z.fixed;
      to.x = target->x.fixed;
      meta = Resource_GetMetadataRecordFar(
          (s16)Scene_FindActorTemplate(work->targetId)->sprite);
      to.y = target->y.fixed + ((s8)meta->boxX << 16) - (2 << 16);
      to.z = target->z.fixed;
      particle = Actor_CreateFar(281, to.x, to.y, to.z);
      if (particle != 0) {
        sprite = (struct WaveEffectSprite *)particle->sprite;
        particle->motionFlags = 0;
        particle->speedLimit = 0xa3d7; // 0.64 pixels a frame
        particle->acceleration = 0xa3d7;
        particle->facing = ArcTan2(from.z - to.z, from.x - to.x);
        particle->update = FieldEffect_SetCallbackWhenTargetUnset;
        // No shadow or outline, sprite priority 1.
        sprite->flags = 0;
        sprite->priority = 1;
        Actor_SetMoveTargetFar(particle, from.x, from.y, from.z);
      }
    }
  }

  if (work->counter == 0)
    Sound_PlaySongFar(SE_PSYNERGY_START);
  if (++work->counter > 60)
    work->counter = 0;
}

// What Mind Read's wave uses: BG0's scanline scroll, the hue cycle and BG
// WINDOW_PALETTE.

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

  FieldEffect_CalculateHueChannels(work->hue << 16, &r, &g, &b);

  // Squeeze 0 to 31 into 4 to 11; offset walks red, green and blue (0x28b).
  offset = 0x28b;
  pr = (u8 *)work + offset;
  *pr = (r >> 18) + 4;
  offset += 1;
  pg = (u8 *)work + offset;
  *pg = (g >> 18) + 4;
  offset += 1;
  pb = (u8 *)work + offset;
  *pb = (b >> 18) + 4;

  work->hue += 4;

  *pr &= 0x1f;
  *pg &= 0x1f;
  *pb &= 0x1f;

  if (work->hue >= 360) {
    work->hue = 0;
  }
}

/// Works out red, green and blue (0 to 31 each, 16.16) for a hue in degrees
/// (16.16).
void FieldEffect_CalculateHueChannels(s32 hue, s32 *r, s32 *g, s32 *b) {
  *r = FieldEffect_HueChannelRamp(hue + (120 << 16), 0, 31 << 16);
  *g = FieldEffect_HueChannelRamp(hue, 0, 31 << 16);
  *b = FieldEffect_HueChannelRamp(hue - (120 << 16), 0, 31 << 16);
}

/// One colour channel's strength at a hue angle: rising from 0 to 60 degrees,
/// high from 60 to 180, falling from 180 to 240, low elsewhere.
s32 FieldEffect_HueChannelRamp(s32 angle, s32 low, s32 high) {
  angle = FixedPoint_Remainder(angle, 360 << 16);
  if (angle < 60 << 16)
    return Iwram_RatioMulQ14(60 << 16, Iwram_MulQ16(high, angle));
  if (angle >= 60 << 16 && angle < 180 << 16)
    return high;
  if (angle >= 180 << 16 && angle < 240 << 16)
    return Iwram_RatioMulQ14(60 << 16, Iwram_MulQ16(high, (240 << 16) - angle));
  return low;
}

/// The 16.16 remainder of value divided by divisor (0 for a zero divisor).
s32 FixedPoint_Remainder(s32 value, s32 divisor) {
  s32 quot;

  if (divisor == 0)
    return 0;
  if (divisor & 0xf0000000)
    divisor = -divisor;
  quot = Iwram_RatioMulQ14(divisor, value);
  // Less the quotient's whole part times divisor.
  return value - Iwram_MulQ16(quot & 0xffff0000, divisor);
}

/// Per-frame update: once the actor has arrived (no destination left on any
/// axis), releases it: gFieldEffect_UntargetedActorScript is a lone SC_RELEASE.
/// Mind Read's particles end this way at the caster.
void FieldEffect_SetCallbackWhenTargetUnset(struct Actor *actor) {
  struct Actor *object = (struct Actor *)actor;
  s32 targetY;
  s32 targetX;

  targetX = object->targetX;
  if (targetX == 0x80000000) // no destination
  {
    targetY = object->targetY;
    if ((targetY == targetX) && (object->targetZ == targetY)) {
      Actor_StartScriptFar(object, gFieldEffect_UntargetedActorScript);
    }
  }
}

/// Turns colours 1 and 3 to 12 of WINDOW_PALETTE grey and starts the BG0
/// scanline scroll (FieldEffect_ArmBg0HBlankDma) at each VBlank.
void FieldPsynergy_GreyMindReadPalette(void) {
  volatile u16 *p;

  // Window mode 1: messages show as thoughts until
  // FieldPsynergy_RestoreMindReadPalette clears it.
  ((struct WindowWork *)gWindowWork[0])->mindRead = 1;
  p = &BG_PLTT_COLOR(WINDOW_PALETTE, 1);
  *p = MIND_READ_GREY;
  p += 2;
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
  // 0x480 is TASK_PASS_VBLANK.
  Task_Add(FieldEffect_ArmBg0HBlankDma, TASK_PASS_VBLANK);
}

/// Stops the BG0 scanline scroll and restores WINDOW_PALETTE.
void FieldPsynergy_RestoreMindReadPalette(void) {
  void *work;

  work = gWindowWork[0];
  Task_Remove(FieldEffect_ArmBg0HBlankDma);
  // Palette 15's white, black and two greys.
  BG_PLTT_COLOR(WINDOW_PALETTE, 1) = RGB(31, 31, 31);
  BG_PLTT_COLOR(WINDOW_PALETTE, 3) = 0;
  BG_PLTT_COLOR(WINDOW_PALETTE, 11) = RGB(10, 10, 10);
  BG_PLTT_COLOR(WINDOW_PALETTE, 12) = RGB(20, 20, 20);
  WindowColor_ApplyFar(gSaveBlock.windowColor[0], gSaveBlock.windowColor[1]);
  ((struct WindowWork *)work)->mindRead = 0;
}

#undef MIND_READ_GREY
