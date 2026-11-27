/* Bringing the battle back behind a cleared canvas, and tinting a battler. Part
   of battle_effect.c. */

/* Shows the battle background, as dark as the canvas's fade left it, behind a
   cleared canvas, and fades out the action's sound effects. */
void btEffectSetupBlendedDisp(void)
{
	BATTLE_STATE *battle = Ram_HeapSlots->battleState;
	Uint8		 *canvas = Ram_HeapSlots->battleCanvas;

	REG_DISPCNT = DISPCNT_MODE_1;
	BgScrollWork[1].y = BATTLE_BACKGROUND_SCROLL_Y;
	btBackgroundLoadStub(
		BATTLE_BACKGROUND_FIXED, battle->bg, BACKGROUND_DARKENED_LEVEL);
	IWRAM_FILL_WORDS(canvas, CANVAS_BYTES, 0);
	IWRAM_FILL_WORDS((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES, 0);
	sySynch(1);
	REG_BLDALPHA = EFFECT_BLEND_CANVAS(CANVAS_BLEND_LEVEL);
	REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND |
		BLDCNT_TGT2_ALL;
	REG_DISPCNT = EFFECT_DISPCNT;
	ProjectionWork.centerY = BATTLE_PROJECTION_CENTER_Y;
	sySndPlayStub(SOUND_FADE_OUT_ACTION_SE);
}

/* Tints one unit's battle object and sets its pose, for every sprite it is made
   of, and starts that unit's flash timer.

   palette: a SPR_COL_MODE, SPRITE_COLOR_NORMAL for the unit's own palette,
			or TINT_KEEP.
   animation: the pose to start, or TINT_KEEP. timerSlot: the state->actorTmrs
   entry to set to timerFrames, or TINT_KEEP. */
void btEffectTintBattler(Sint32 unitId, Sint32 palette, Sint32 animation,
	Sint32 timerSlot, Sint32 timerFrames)
{
	BATTLE_OBJECT		*slot;
	SPR					*group;
	BATTLE_EFFECT_STATE *state;
	Sint32				 groupIndex;

	slot = btObjectGetStub(unitId);
	state = Ram_HeapSlots->battleEff;

	for(groupIndex = 0;
		(group = btActorGetSprStub(slot->obj, groupIndex)) != NULL;
		groupIndex++) {
		if(timerSlot != TINT_KEEP)
			state->actorTmrs[timerSlot] = timerFrames;
		if(slot->fading == 0) {
			if(palette != TINT_KEEP) {
				Sint32 partIndex;

				for(partIndex = 0; partIndex != group->cnt; partIndex++) {
					SPR_ANIMATION *part = group->entries[partIndex];

					if(part != NULL && part != slot->shadowEntry &&
						part != slot->anmEntry) {
						Uint8 oldPalette = part->colMode;

						if(palette == SPRITE_COLOR_NORMAL)
							part->colMode = btMotionGetSlotPaletteStub(unitId);
						else
							part->colMode = palette;
						if(part->colMode != oldPalette)
							part->picture = SPRITE_PICTURE_NONE;
					}
				}
			}
			if(animation != TINT_KEEP)
				fdSprStartAnimationStub(group, animation);
		}
	}
}
