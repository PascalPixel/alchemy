// Display, scroll and window HBlank DMA helpers. Part of battle_effect.c.

// Puts the battle screen back into the display state every effect starts from and ends with.
void btEffectConfigureEffectDisplay(void)
{
    REG_BLDCNT   = 0;
    REG_BLDALPHA = EFFECT_BLEND_CANVAS(CANVAS_BLEND_LEVEL);
    EFFECT_SET_BATTLE_WINDOWS();
    REG_WININ =
        EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG2 | WININ_WIN1_OBJ | WININ_WIN1_CLR;
    REG_WINOUT = EFFECT_WINOUT;
    IO_WRITE_QUEUE_PUSH((Uint32)&REG_DISPCNT, EFFECT_DISPCNT, IO_WRITE_HALFWORD);
    sySynch(1);
}

// Scrolls BG1 a step every few frames.
void btEffectAdvanceScrollOnInterval(void)
{
    BATTLE_EFFECT_STATE *state;

    state = RamHeapSlots->battleEffect;

    if (++state->scrollTimer == state->scrollInterval)
    {
        BgScrollWork[1].x  += state->scrollStepX;
        BgScrollWork[1].y  += state->scrollStepY;
        state->scrollTimer  = 0;
    }
}

// Slides the affine BG2 a step every few frames.
void btEffectAdvanceBg2OnInterval(void)
{
    BATTLE_EFFECT_STATE *state;

    state = RamHeapSlots->battleEffect;

    if (++state->scrollTimer == state->scrollInterval)
    {
        REG_BG2X            = state->bg2x;
        REG_BG2Y            = state->bg2y;
        state->bg2x        += state->scrollStepX;
        state->bg2y        += state->scrollStepY;
        state->scrollTimer  = 0;
    }
}

// Restarts the DMA that feeds window 0's edges, one a line, from the table the effect keeps in the
// map cell buffer.
void btEffectArmWin0HBlankDMA(void)
{
    volatile Uint16 *channel   = REG_DMA0;

    channel[DMA_CONTROL_HALF] &= (Uint16)~DMA_START_TIMING_DRQ_REPEAT;
    channel[DMA_CONTROL_HALF] &= (Uint16)~DMA_ENABLE;
    (void)channel[DMA_CONTROL_HALF];
    syDMAXCopy(RamMapMetatileBuffer,
               (void *)&REG_WIN0H,
               DMA_HBLANK_HALFWORD_PER_LINE,
               (volatile Uint32 *)channel);
}
