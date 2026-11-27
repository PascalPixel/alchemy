// Bringing the battle back behind a cleared canvas, and tinting a battler. Part of battle_effect.c.

// Shows the battle background, as dark as the canvas's fade left it, behind a cleared canvas, and
// fades out the action's sound effects.
void btEffectSetupBlendedDisplay(void)
{
    BATTLE_STATE *battle = RamHeapSlots->battleState;
    Uint8        *canvas = RamHeapSlots->battleCanvas;

    REG_DISPCNT          = DISPCNT_MODE_1;
    BgScrollWork[1].y    = BATTLE_BACKGROUND_SCROLL_Y;
    btBackgroundLoadStub(BATTLE_BACKGROUND_FIXED, battle->bg, BACKGROUND_DARKENED_LEVEL);
    IWRAM_FILL_WORDS(canvas, CANVAS_BYTES, 0);
    IWRAM_FILL_WORDS((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES, 0);
    sySynch(1);
    REG_BLDALPHA = EFFECT_BLEND_CANVAS(CANVAS_BLEND_LEVEL);
    REG_BLDCNT   = BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
    REG_DISPCNT  = EFFECT_DISPCNT;
    ProjectionWork.centerY = BATTLE_PROJECTION_CENTER_Y;
    sySoundPlayStub(SOUND_FADE_OUT_ACTION_SE);
}

// Tints one unit's battle object and sets its pose, for every sprite it is made of, and starts that
// unit's flash timer.
//
// palette: a SPR_COL_MODE, SPRITE_COLOR_NORMAL for the unit's own palette,
//                      or TINT_KEEP.
// animation: the pose to start, or TINT_KEEP. timerSlot: the state->actorTimers entry to set to
// timerTicks, or TINT_KEEP.
void btEffectTintBattler(
    Sint32 unitId, Sint32 palette, Sint32 animation, Sint32 timerSlot, Sint32 timerTicks)
{
    BATTLE_OBJECT       *slot;
    SPR                 *group;
    BATTLE_EFFECT_STATE *state;
    Sint32               groupIndex;

    slot  = btObjectGetStub(unitId);
    state = RamHeapSlots->battleEffect;

    for (groupIndex = 0; (group = btActorGetSpriteStub(slot->object, groupIndex)) != NULL;
         groupIndex++)
    {
        if (timerSlot != TINT_KEEP)
            state->actorTimers[timerSlot] = timerTicks;
        if (slot->fading == 0)
        {
            if (palette != TINT_KEEP)
            {
                Sint32 partIndex;

                for (partIndex = 0; partIndex != group->count; partIndex++)
                {
                    SPR_ANIMATION *part = group->entries[partIndex];

                    if (part != NULL && part != slot->shadowEntry && part != slot->animationEntry)
                    {
                        Uint8 oldPalette = part->colorMode;

                        if (palette == SPRITE_COLOR_NORMAL)
                            part->colorMode = btMotionGetSlotPaletteStub(unitId);
                        else
                            part->colorMode = palette;
                        if (part->colorMode != oldPalette)
                            part->picture = SPRITE_PICTURE_NONE;
                    }
                }
            }
            if (animation != TINT_KEEP)
                fdSpriteStartAnimationStub(group, animation);
        }
    }
}
