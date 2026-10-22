/// @file
/// Mind Read: scanline waves, shifting colours and flowing particles.
// Part of psynergy_effects.c.

#include "../psynergy_effects_internal.h"

/// Work slot 22 of the wave effect, 0x298 bytes (gBattleBgFxWork). It lives
/// from RunSceneTransitionEffect to FieldEffect_FinishSceneAndReleaseHeapBlock,
/// after the conversation Mind Read reads.
struct WaveFxWork {
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
/// per-frame update (FieldEffect_UpdateMindReadWave) after 8 frames.
///
/// Abilities: Mind Read (FIELD_KIND_MIND_READ).
///
/// The effect stays up until FieldEffect_ClearChildValueOnMismatch ends it
/// after the after event; meanwhile the message windows show thoughts
/// (Ui_FillBank15PaletteGrey sets the window mode).
void RunSceneTransitionEffect(s32 x, s32 y) {
  struct WaveFxWork *work;

  work = Runtime_AllocateBlock(HEAP_SLOT_MIND_READ, 0x298);
  FieldEffect_FadeToEffectColor();
  Dma_Fill32(0, work, 0x298);
  work->hue = ((u32)(gFrameCount << 1) % (u32)360);
  FieldEffect_AdvanceHueCycle();
  // 0x200000 is PALETTE_TRANSFORM_TINT: the BG colours tinted by the hue.
  ColorBuffer_SetTarget(RGB(work->red, work->green, work->blue) | 0x200000, 1);
  ColorBuffer_StartInterpolation(8);
  work->sourceId = x;
  work->targetId = y;
  work->delay = 8;
  Ui_FillBank15PaletteGrey();
  Scheduler_AddOrUpdateCallback(FieldEffect_UpdateMindReadWave, 0xc80);
}

/// Stops the wave effect: restores the caster's colours and the palettes and
/// releases the effect's work.
void FieldEffect_FinishSceneAndReleaseHeapBlock(void) {
  void *work;

  work = *(void **)((u32)&gBattleBgFxWork);
  Ui_SetBank15PaletteAndClearRenderMode();
  Scheduler_RemoveCallback(FieldEffect_UpdateMindReadWave);
  Animation_ApplyChildPalette((struct MotionObject *)Object_GetById(
                                  ((struct WaveFxWork *)work)->sourceId),
                              1);
  FieldEffect_FadeFromEffectColor();
  Runtime_ReleaseHeapBlock(HEAP_SLOT_MIND_READ);
}

/// Scheduler callback of the wave effect: once its delay has run out, writes
/// the next page of scanline offsets (a sine wave of 4 pixels either way),
/// steps the colour fade and the hue, keeps the caster's colours lit, and
/// every 60 frames sends three particles (object 281) from the target to the
/// caster, with sound 130.
void FieldEffect_UpdateMindReadWave(void) {
  struct WaveFxWork *work = gBattleBgFxWork;
  u16 *line;
  u32 scanline;
  struct FieldActor *caster;
  struct FieldActor *target;
  struct FieldActor *particle;
  struct WaveFxSprite *sprite;
  struct EffectVector from;
  struct EffectVector to;
  struct AnimationMetadata *meta;

  if (work->delay != 0) {
    work->delay--;
    return;
  }

  // The 160 scanlines' offsets, a sine wave of 4 pixels either way (a 16.16
  // sine >> 14), on the hidden page; then show that page.
  line = work->lines[work->page ^ 1];
  for (scanline = 0; scanline < 160; scanline++)
    *line++ =
        Trig_Sin(((u32)((work->phase + scanline * 8) << 16) / (u32)160)) >> 14;
  work->phase += 4;
  work->page ^= 1;
  // Every other frame the tint takes the next hue, in one frame.
  if (work->page != 0) {
    ColorBuffer_SetTarget(RGB(work->red, work->green, work->blue) | 0x200000,
                          1);
    ColorBuffer_StartInterpolation(1);
    FieldEffect_AdvanceHueCycle();
  }
  Animation_ApplyChildPalette(
      (struct MotionObject *)Object_GetById(work->sourceId), 0);

  // At steps 0, 8 and 16 of the cycle, a particle from the target to the
  // caster.
  if (work->counter == 0 || work->counter == 8 || work->counter == 16) {
    caster = Object_GetById(work->sourceId);
    target = Object_GetById(work->targetId);
    if (caster != 0 && target != 0) {
      // From 2 pixels below each one's sprite height (its metadata's boxX).
      from.x = caster->x.fixed;
      meta = Resource_GetMetadataRecordFar(
          (s16)Scene_FindActorPlacement(work->sourceId)->sprite);
      from.y = caster->y.fixed + ((s8)meta->boxX << 16) - (2 << 16);
      from.z = caster->z.fixed;
      to.x = target->x.fixed;
      meta = Resource_GetMetadataRecordFar(
          (s16)Scene_FindActorPlacement(work->targetId)->sprite);
      to.y = target->y.fixed + ((s8)meta->boxX << 16) - (2 << 16);
      to.z = target->z.fixed;
      particle = (struct FieldActor *)Object_CreateFar(281, to.x, to.y, to.z);
      if (particle != 0) {
        sprite = (struct WaveFxSprite *)particle->sprite;
        particle->motionFlags = 0;
        particle->speed = 0xa3d7; // 0.64 pixels a frame
        particle->acceleration = 0xa3d7;
        particle->facing = ArcTan2(from.z - to.z, from.x - to.x);
        particle->update = FieldEffect_SetCallbackWhenTargetUnset;
        // No shadow or outline, sprite priority 1.
        sprite->palette = 0;
        sprite->mode = 1;
        Object_SetMoveTargetFar((struct ObjectRuntime *)particle, from.x,
                                from.y, from.z);
      }
    }
  }

  if (work->counter == 0)
    AudioCommand_PlayFar(SE_130);
  if (++work->counter > 60)
    work->counter = 0;
}

// What Mind Read's wave uses: BG0's scanline scroll, the hue cycle and BG
// palette 15.

#define MIND_READ_GREY RGB(28, 28, 28)

/// Scheduler callback: restarts the HBlank DMA that writes BG0's horizontal
/// scroll for each scanline from the wave's shown page.
void FieldEffect_ArmBg0HBlankDma(void) {
  struct WaveFxWork *work = gBattleBgFxWork;
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
  struct WaveFxWork *work = gBattleBgFxWork;
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

/// Per-frame update: once the object has arrived (no destination left on any
/// axis), frees it: gFieldEffect_UntargetedObjectScript is a lone SC_FREE.
/// Mind Read's particles end this way at the caster.
void FieldEffect_SetCallbackWhenTargetUnset(union FieldObject *fieldObject) {
  struct ObjectRuntime *object = (struct ObjectRuntime *)fieldObject;
  s32 targetY;
  s32 targetX;

  targetX = object->targetX;
  if (targetX == 0x80000000) // no destination
  {
    targetY = object->targetY;
    if ((targetY == targetX) && (object->targetZ == targetY)) {
      ObjectDispatch_InitializeFar((struct DispatchObject *)object,
                                   gFieldEffect_UntargetedObjectScript);
    }
  }
}

/// Turns colours 1 and 3 to 12 of BG palette 15 grey and starts the BG0
/// scanline scroll (FieldEffect_ArmBg0HBlankDma) at each VBlank.
void Ui_FillBank15PaletteGrey(void) {
  volatile u16 *p;

  // Window mode 1: messages show as thoughts until
  // Ui_SetBank15PaletteAndClearRenderMode clears it.
  ((struct UiRenderWork *)gWindowWork[0])->mode = 1;
  p = &BG_PLTT_COLOR(15, 1);
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
  // 0x480 is SCHEDULER_KEY_VBLANK.
  Scheduler_AddOrUpdateCallback(FieldEffect_ArmBg0HBlankDma, 0x480);
}

/// Stops the BG0 scanline scroll and restores BG palette 15.
void Ui_SetBank15PaletteAndClearRenderMode(void) {
  void *work;

  work = gWindowWork[0];
  Scheduler_RemoveCallback(FieldEffect_ArmBg0HBlankDma);
  // Palette 15's white, black and two greys.
  BG_PLTT_COLOR(15, 1) = RGB(31, 31, 31);
  BG_PLTT_COLOR(15, 3) = 0;
  BG_PLTT_COLOR(15, 11) = RGB(10, 10, 10);
  BG_PLTT_COLOR(15, 12) = RGB(20, 20, 20);
  PaletteGlow_UpdateFar(gGameState.paletteGlow[0], gGameState.paletteGlow[1]);
  ((struct UiRenderWork *)work)->mode = 0;
}

#undef MIND_READ_GREY
