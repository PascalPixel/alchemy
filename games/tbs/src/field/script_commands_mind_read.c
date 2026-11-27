// Mind Read: scanline waves, shifting colours and flowing particles. Part of script_commands.c.

#include "script_commands_internal.h"

// The hue circle, in whole degrees and in degrees as 16.16.
#define HUE_DEGREES                  360
#define HUE_CIRCLE                   toFIXED(HUE_DEGREES)

// A line's offset in Mind Read's wave at a phase: a sine 4 pixels either way, a turn every 160
// phase steps.
#define MIND_READ_WAVE_OFFSET(phase) (sySin((Uint32)((phase) << 16) / (Uint32)160) >> 14)
// The wave's cycle runs to this count and starts again, 61 ticks; at counts 0, 8 and 16 a particle
// flies from the target to the caster.
#define MIND_READ_CYCLE_LAST         60
#define MIND_READ_PARTICLE_GAP       8
// The tint's channels run from this level, a quarter of the hue's own on top.
#define MIND_READ_TINT_LEVEL_MIN     4

// The HEAP_SLOT_MIND_READ heap slot (MindReadStateWork), 664 bytes.
typedef struct MIND_READ_STATE
{
    // Each scanline's BG0 horizontal scroll in pixels, two pages written in turn; 160 of each
    // page's 162 are used.
    Uint16 lines[2][162];
    Uint16 phase;    // the wave's phase, 4 more a tick
    Uint8  page;     // the page shown (the HBlank DMA's); the other is written
    Sint8  red;      // the hue as a colour (red, green, blue, 4 to 11 each)
    Sint8  green;
    Sint8  blue;
    Uint16 hue;      // hue, 0 to 359 degrees
    Uint16 srcId;    // the caster's actor id
    Uint16 targetId; // the target's actor id
    Uint8  delay;    // ticks before it starts
    Uint8  count;    // particle spawn cycle (0 to 60)
} MIND_READ_STATE;

// Starts the wave effect of casterId on targetId (actor ids): fades the palettes toward a hue that
// shifts with the tick counter, and starts its per-tick update (fdPsynergyUpdateMindReadWave) after
// 8 ticks.
void fdPsynergyStartMindReadWave(Sint32 casterId, Sint32 targetId)
{
    MIND_READ_STATE *state;

    state = syHeapSlotAllocateEWRAM(HEAP_SLOT_MIND_READ, sizeof(MIND_READ_STATE));
    fdPsynergyFadeToCastColor();
    DMA_FILL32(0, state, sizeof(MIND_READ_STATE));
    state->hue = ((Uint32)(TickCount << 1) % (Uint32)HUE_DEGREES);
    fdMindReadAdvanceTint();
    // PALETTE_TINT_CUSTOM: the BG colours tinted by the hue.
    fdPaletteFadeSetTarget(RGB(state->red, state->green, state->blue) | PALETTE_TINT_CUSTOM,
                           PALETTES_BG);
    fdPaletteFadeStartInterpolation(8);
    state->srcId    = casterId;
    state->targetId = targetId;
    state->delay    = 8;
    fdPsynergyGreyMindReadPalette();
    sySetEvent(fdPsynergyUpdateMindReadWave, EVENT_PASS_TICK);
}

// Stops the wave effect: restores the caster's colours and the palettes and releases the Psynergy's
// state.
void fdPsynergyStopMindReadWave(void)
{
    MIND_READ_STATE *state = MindReadStateWork;

    fdPsynergyRestoreMindReadPalette();
    syCloseEvent(fdPsynergyUpdateMindReadWave);
    // Back to SPRITE_COLOR_NORMAL.
    fdActorSetLayersColorMode(fdActorGetById(state->srcId), 1);
    fdPsynergyFadeFromCastColor();
    syHeapSlotRelease(HEAP_SLOT_MIND_READ);
}

// Event of the wave effect: once its delay has run out, writes the next page of scanline offsets (a
// sine wave of 4 pixels either way), steps the colour fade and the hue every other tick, keeps the
// caster's colours cycling, and at the start of each 61-tick cycle sends three particles
// (SPRITE_CRESCENT), 8 ticks apart, from the target to the caster, with SE_PSYNERGY_START.
void fdPsynergyUpdateMindReadWave(void)
{
    MIND_READ_STATE    *state = MindReadStateWork;
    Uint16             *line;
    Uint32              scanline;
    ACTOR              *caster;
    ACTOR              *target;
    ACTOR              *particle;
    WAVE_EFFECT_SPR    *sprite;
    VECTOR3             from;
    VECTOR3             to;
    ANIMATION_METADATA *meta;

    if (state->delay != 0)
    {
        state->delay--;
        return;
    }

    // The scanlines' offsets on the hidden page; then show that page.
    line = state->lines[state->page ^ 1];
    for (scanline = 0; scanline < DISPLAY_HEIGHT; scanline++)
        *line++ = MIND_READ_WAVE_OFFSET(state->phase + scanline * 8);
    state->phase += 4;
    state->page  ^= 1;
    // Every other tick the tint takes the next hue, in one tick.
    if (state->page != 0)
    {
        fdPaletteFadeSetTarget(RGB(state->red, state->green, state->blue) | PALETTE_TINT_CUSTOM,
                               PALETTES_BG);
        fdPaletteFadeStartInterpolation(1);
        fdMindReadAdvanceTint();
    }
    // The caster's colours cycle.
    fdActorSetLayersColorMode(fdActorGetById(state->srcId), 0);

    if (state->count == 0 || state->count == MIND_READ_PARTICLE_GAP
        || state->count == 2 * MIND_READ_PARTICLE_GAP)
    {
        caster = fdActorGetById(state->srcId);
        target = fdActorGetById(state->targetId);
        if (caster != 0 && target != 0)
        {
            // From 2 pixels below each one's sprite height (its metadata's spriteTop).
            from.x = caster->x.fixed;
            meta =
                syResourceGetMetadataRecordStub((Sint16)fdFindActorPlacement(state->srcId)->sprite);
            from.y = caster->y.fixed + toFIXED(meta->spriteTop) - toFIXED(2);
            from.z = caster->z.fixed;
            to.x   = target->x.fixed;
            meta   = syResourceGetMetadataRecordStub(
                (Sint16)fdFindActorPlacement(state->targetId)->sprite);
            to.y     = target->y.fixed + toFIXED(meta->spriteTop) - toFIXED(2);
            to.z     = target->z.fixed;
            particle = fdActorCreateStub(SPRITE_CRESCENT, to.x, to.y, to.z);
            if (particle != 0)
            {
                sprite                 = (WAVE_EFFECT_SPR *)particle->sprite;
                particle->motionFlags  = 0;
                // 0.64 pixels a tick
                particle->speedLimit   = toFIXED(16) / 25;
                particle->acceleration = toFIXED(16) / 25;
                particle->facing       = syAtan(from.z - to.z, from.x - to.x);
                particle->update       = fdMindReadUpdateCrescent;
                // No shadow or outline, sprite priority 1.
                sprite->flags          = 0;
                sprite->priority       = 1;
                fdActorSetMoveTargetStub(particle, from.x, from.y, from.z);
            }
        }
    }

    if (state->count == 0)
        sySoundPlayStub(SE_PSYNERGY_START);
    if (++state->count > MIND_READ_CYCLE_LAST)
        state->count = 0;
}

// What Mind Read's wave uses: BG0's line scroll, the hue cycle and BG WINDOW_PALETTE.

// The grey Mind Read writes over the window palette's white, and its black and the colours after
// it.
#define MIND_READ_GREY RGB(28, 28, 28)

// Event: restarts the HBlank DMA that writes BG0's horizontal scroll for each scanline from the
// wave's shown page.
void fdMindReadStartRipple(void)
{
    MIND_READ_STATE *state = MindReadStateWork;
    if (!state->delay)
    {
        Uint16          *line  = state->lines[state->page];
        volatile Uint16 *dma   = REG_DMA0;
        dma[DMA_CNT_H_INDEX]  &= ~DMA_START_TIMING_DRQ_REPEAT;
        dma[DMA_CNT_H_INDEX]  &= ~DMA_ENABLE;
        (void)dma[DMA_CNT_H_INDEX];
        // One halfword at each HBlank (repeating, destination reloaded).
        syDMAXCopy(line, REG_BG0HOFS, DMA_HBLANK_HALFWORD_PER_LINE, (volatile Uint32 *)dma);
    }
}

// Moves the wave's hue on 4 degrees and works out its colour, each channel 4 to 11.
void fdMindReadAdvanceTint(void)
{
    MIND_READ_STATE *state = MindReadStateWork;
    Sint32           r     = 0;
    Sint32           g     = 0;
    Sint32           b     = 0;
    Sint32           offset;
    Uint8           *pr;
    Uint8           *pg;
    Uint8           *pb;

    fdEffectCalculateHueChannels(toFIXED(state->hue), &r, &g, &b);

    // Squeeze 0 to 31 into 4 to 11; offset walks red, green and blue.
    offset      = offsetof(MIND_READ_STATE, red);
    pr          = (Uint8 *)state + offset;
    *pr         = (FIXEDtoINT(r) >> 2) + MIND_READ_TINT_LEVEL_MIN;
    offset     += 1;
    pg          = (Uint8 *)state + offset;
    *pg         = (FIXEDtoINT(g) >> 2) + MIND_READ_TINT_LEVEL_MIN;
    offset     += 1;
    pb          = (Uint8 *)state + offset;
    *pb         = (FIXEDtoINT(b) >> 2) + MIND_READ_TINT_LEVEL_MIN;

    state->hue += 4;

    *pr        &= RGB_LEVEL_MAX;
    *pg        &= RGB_LEVEL_MAX;
    *pb        &= RGB_LEVEL_MAX;

    if (state->hue >= HUE_DEGREES)
    {
        state->hue = 0;
    }
}

// Works out red, green and blue (0 to 31 each, 16.16) for a hue in degrees (16.16).
void fdEffectCalculateHueChannels(FIXED hue, FIXED *r, FIXED *g, FIXED *b)
{
    *r = fdEffectHueChannelRamp(hue + HUE_CIRCLE / 3, 0, toFIXED(1) * RGB_LEVEL_MAX);
    *g = fdEffectHueChannelRamp(hue, 0, toFIXED(1) * RGB_LEVEL_MAX);
    *b = fdEffectHueChannelRamp(hue - HUE_CIRCLE / 3, 0, toFIXED(1) * RGB_LEVEL_MAX);
}

// One colour channel's strength at a hue angle: rising from 0 to 60 degrees, high from 60 to 180,
// falling from 180 to 240, low elsewhere.
Sint32 fdEffectHueChannelRamp(Sint32 angle, Sint32 low, FIXED high)
{
    angle = fdFixedPointRemainder(angle, HUE_CIRCLE);
    if (angle < HUE_CIRCLE / 6)
        return syDivFX(HUE_CIRCLE / 6, syMulFX(high, angle));
    if (angle >= HUE_CIRCLE / 6 && angle < HUE_CIRCLE / 2)
        return high;
    if (angle >= HUE_CIRCLE / 2 && angle < HUE_CIRCLE * 2 / 3)
        return syDivFX(HUE_CIRCLE / 6, syMulFX(high, HUE_CIRCLE * 2 / 3 - angle));
    return low;
}

// The 16.16 remainder of value divided by divisor (0 for a zero divisor).
FIXED fdFixedPointRemainder(FIXED value, FIXED divisor)
{
    FIXED quot;

    if (divisor == 0)
        return 0;
    if (divisor & 0xf0000000)
        divisor = -divisor;
    quot = syDivFX(divisor, value);
    // Less the quotient's whole part times divisor.
    return value - syMulFX(quot & 0xffff0000, divisor);
}

// Per-tick update: once the actor has arrived (no destination left on any axis), releases it:
// FieldEffectUntargetedActorScript is a lone SC_RELEASE.
void fdMindReadUpdateCrescent(ACTOR *actor)
{
    Sint32 targetY;
    Sint32 targetX;

    targetX = actor->targetX;
    if (targetX == ACTOR_NO_TARGET)
    {
        targetY = actor->targetY;
        if ((targetY == targetX) && (actor->targetZ == targetY))
        {
            fdActorStartScriptStub(actor, FieldEffectUntargetedActorScript);
        }
    }
}

// Turns WINDOW_PALETTE's white, black, window colours and border greys one grey, and starts the BG0
// line scroll (fdMindReadStartRipple) at each VBlank.
void fdPsynergyGreyMindReadPalette(void)
{
    volatile Uint16 *p;

    // Window mode 1: messages show as thoughts until fdPsynergyRestoreMindReadPalette clears it.
    WINDOW_STATE->mindRead  = 1;
    p                       = &BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_WHITE_COLOR);
    *p                      = MIND_READ_GREY;
    p                      += WINDOW_BLACK_COLOR - WINDOW_WHITE_COLOR;
    *p                      = MIND_READ_GREY;
    p                      += 1;
    *p                      = MIND_READ_GREY;
    p                      += 1;
    *p                      = MIND_READ_GREY;
    p                      += 1;
    *p                      = MIND_READ_GREY;
    p                      += 1;
    *p                      = MIND_READ_GREY;
    p                      += 1;
    *p                      = MIND_READ_GREY;
    p                      += 1;
    *p                      = MIND_READ_GREY;
    p                      += 1;
    *p                      = MIND_READ_GREY;
    p                      += 1;
    *p                      = MIND_READ_GREY;
    p                      += 1;
    *p                      = MIND_READ_GREY;
    sySetEvent(fdMindReadStartRipple, EVENT_PASS_VBLANK);
}

// Stops the BG0 line scroll and restores WINDOW_PALETTE.
void fdPsynergyRestoreMindReadPalette(void)
{
    void *state;

    state = WindowStateWork[0];
    syCloseEvent(fdMindReadStartRipple);
    BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_WHITE_COLOR)        = RGB_WHITE;
    BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BLACK_COLOR)        = RGB(0, 0, 0);
    BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BORDER_DARK_COLOR)  = RGB(10, 10, 10);
    BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BORDER_LIGHT_COLOR) = RGB(20, 20, 20);
    mnWindowColorApplyStub(MainRoutineStateWork.windowHue, MainRoutineStateWork.windowBrightness);
    ((WIN_STATE *)state)->mindRead = 0;
}

#undef MIND_READ_GREY
#undef MIND_READ_WAVE_OFFSET
#undef MIND_READ_CYCLE_LAST
#undef MIND_READ_PARTICLE_GAP
#undef MIND_READ_TINT_LEVEL_MIN

#undef HUE_DEGREES
#undef HUE_CIRCLE
