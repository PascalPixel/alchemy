/* @file */
/* The two ways into an effect, a summon or an ordinary ability, with the */
/* sheet, state and canvas they lend it; and the helpers the summons share for
 */
/* window 0's ragged edge and the targets' sprites. */
/* Part of battle_effect.c. */

/* Calls the empty btEffectDoNothing count times: a busy delay. */
void btEffectCallDoNothing(s32 count) {
	s32 i;

	for(i = 0; i != count; i++)
		btEffectDoNothing();
}

/* Runs one of the large battle effects, the summons: takes the effect's sheet,
 */
/* state and canvas, runs it and frees them again. */
void btEffectRunSummon(BATTLE_EFFECT_PARM *args) {
	syHeapSlotAlloc(HEAP_SLOT_BATTLE_SHEET, MOTE_SET_BYTES);
	syHeapSlotAlloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(BATTLE_EFFECT_STATE));
	syHeapSlotAlloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);

	if(args->kind == SUMMON_EFFECT_NONE) {
		btEffectRunMeteor(args);
	}
	else {
		switch(args->kind) {
		case SUMMON_EFFECT_RAMSES:
			btEffectRunRamses(args);
			break;
		case SUMMON_EFFECT_NEREID:
			btEffectRunNereid(args);
			break;
		case SUMMON_EFFECT_KIRIN:
			btEffectRunKirin(args);
			break;
		case SUMMON_EFFECT_ATALANTA:
			btEffectRunAtalanta(args);
			break;
		case SUMMON_EFFECT_CYBELE:
			btEffectRunCybele(args);
			break;
		case SUMMON_EFFECT_NEPTUNE:
			btEffectRunNeptune(args);
			break;
		case SUMMON_EFFECT_TIAMAT:
			btEffectRunTiamat(args);
			break;
		case SUMMON_EFFECT_PROCNE:
			btEffectRunProcne(args);
			break;
		case SUMMON_EFFECT_JUDGMENT:
			btEffectRunJudgment(args);
			break;
		case SUMMON_EFFECT_BOREAS:
			btEffectRunBoreas(args);
			break;
		case SUMMON_EFFECT_METEOR:
			btEffectRunMeteor(args);
			break;
		case SUMMON_EFFECT_THOR:
			btEffectRunThor(args);
			break;
		}
	}

	syHeapSlotRelease(HEAP_SLOT_BATTLE_CANVAS);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_SHEET);
}

/* Runs an ordinary battle effect from BattleEffect_Handlers, with the sprite */
/* sheet, state and canvas it lends. */
void btEffectRunFromTable(BATTLE_EFFECT_PARM *args) {
	s32 kind;
	BATTLE_EFFECT_STATE *state;

	syHeapSlotAlloc(HEAP_SLOT_BATTLE_SHEET, MOTE_SET_BYTES);
	syHeapSlotAlloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(BATTLE_EFFECT_STATE));
	syHeapSlotAlloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	state->args = args;
	kind = args->kind;
	if(kind == BATTLE_EFFECT_NONE)
		args->variant = EFFECT_VARIANT_BASIC;
	else
		BattleEffect_Handlers[kind - 1](args);

	syHeapSlotRelease(HEAP_SLOT_BATTLE_CANVAS);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_SHEET);
}

/* The first screen line of window 0's ragged edge. */
#define WINDOW_EDGE_FIRST_LINE 8

/* Builds window 0's right edge for every line, ragged by each line's inset, */
/* and restarts the DMA that writes it; the summons run it each VBlank while */
/* the window opens across the screen. */
void btEffectBuildWindowEdgeTable(void) {
	u16 *baseRightEdge;
	u16 *scanlineEdges;
	u8 *edgeOffsets;
	s32 scanline;
	s32 rightEdge;

	baseRightEdge = WINDOW_EDGE_RIGHT;
	scanlineEdges = (u16 *)WINDOW_EDGE_LINES;
	edgeOffsets = WINDOW_EDGE_INSETS;
	for(scanline = 0; scanline != DISPLAY_HEIGHT; scanline++) {
		if(scanline >= WINDOW_EDGE_FIRST_LINE &&
		   scanline < WINDOW_EDGE_FIRST_LINE + WINDOW_EDGE_INSET_LINES) {
			rightEdge =
				*baseRightEdge - edgeOffsets[scanline - WINDOW_EDGE_FIRST_LINE];
			CLAMP(rightEdge, 0, DISPLAY_WIDTH);
			scanlineEdges[scanline] = rightEdge;
		}
		else {
			/* Empty: the left edge is past the screen's right. */
			scanlineEdges[scanline] = WIN_RANGE(255, 241);
		}
	}
	{
		volatile u16 *channel = REG_DMA0;

		btDmaStop(channel);
		syDMAXCopy(WINDOW_EDGE_LINES, &REG_WIN0H, DMA_HBLANK_HALFWORD_PER_LINE,
				   (volatile u32 *)channel);
	}
}

/* Spawns the battle sprites of every unit with HP left on the targets' side. */
void btEffectShowLivingTargets(BATTLE_EFFECT_PARM *args) {
	s16 list[UNIT_MAX];
	s32 count;
	s32 i;

	count = 0;
	if(args->tgts[0] >= FIRST_ENEMY_UNIT) {
		for(i = 0; i != ENEMY_BATTLE_SIZE; i++) {
			s32 id = i + FIRST_ENEMY_UNIT;

			if(gmUnitGetStub(id)->hp > 0)
				list[count++] = id;
		}
	}
	else {
		for(i = 0; i != PARTY_UNIT_COUNT; i++) {
			if(gmUnitGetStub(i)->hp > 0)
				list[count++] = i;
		}
	}
	list[count] = BATTLE_UNIT_LIST_END;
	btObjectSpawnObjectsForListStub(list, 0);
}

/* Shows the battle background, darkened as a canvas leaves it, behind a */
/* cleared canvas. */
void btEffectSetupBlendedDisp(void) {
	void **slots = &HeapSlots[HEAP_SLOT_BATTLE];
	u8 *canvas =
		HEAP_SLOT_FROM(slots, HEAP_SLOT_BATTLE, HEAP_SLOT_BATTLE_CANVAS);
	BATTLE_STATE *battle = slots[0];
	volatile u32 fill;

	REG_DISPCNT = DISPCNT_MODE_1;
	BgScrollWork[1].y = BATTLE_BACKGROUND_SCROLL_Y;
	btBackgroundLoadStub(BATTLE_BACKGROUND_FIXED, battle->bg,
						 BACKGROUND_DARKENED_LEVEL);
	btDmaFill32Cell(fill, 0, canvas, CANVAS_BYTES);
	btDmaFill32Cell(fill, 0, BG_CHAR_BLOCK(1), CANVAS_BYTES);
	sySynch(1);
	REG_BLDALPHA = EFFECT_BLEND_CANVAS(CANVAS_BLEND_LEVEL);
	REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND |
				 BLDCNT_TGT2_ALL;
	REG_DISPCNT = EFFECT_DISPCNT;
	ProjectionWork.centerY = BATTLE_PROJECTION_CENTER_Y;
}

/* Tints one unit's sprite: every part but its shadow takes `palette` */
/* (SPRITE_COLOR_NORMAL gives the unit its own back), the sprite starts */
/* `animation`, and timer `timerSlot` of the effect counts `timerFrames` down */
/* to the tint's end (btEffectCountDownBattlerTints); TINT_KEEP leaves */
/* any of the three as it is. */
void btEffectTintBattler(s32 unitId, s32 palette, s32 animation, s32 timerSlot,
						 s32 timerFrames) {
	BATTLE_OBJECT *slot;
	SPR *group;
	BATTLE_EFFECT_STATE *state;
	s32 groupIndex;

	slot = btObjectGetStub(unitId);
	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);

	for(groupIndex = 0;
		(group = fdActorGetSprStub(slot->actor, groupIndex)) != NULL;
		groupIndex++) {
		if(timerSlot != TINT_KEEP) {
			state->actorTmrs[timerSlot] = timerFrames;
		}
		if(slot->fading == 0) {
			if(palette != TINT_KEEP) {
				s32 partIndex;

				for(partIndex = 0; partIndex != group->cnt; partIndex++) {
					SPR_ANIMATION *part = group->entries[partIndex];

					if(part != NULL && part != slot->shadowEntry &&
					   part != slot->anmEntry) {
						if(palette == SPRITE_COLOR_NORMAL)
							part->colMode = btObjectGetPaletteStub(unitId);
						else
							part->colMode = palette;
						part->picture = SPRITE_PICTURE_NONE;
					}
				}
			}
			if(animation != TINT_KEEP)
				fdSprStartAnimationStub(group, animation);
		}
	}
}

/* Opens the tiled canvas at priority 1 and closes it again at once. */
UNUSED void btEffectOpenAndCloseTiledCanvas(void) {
	btEffectStartTiledCanvas(BGCNT_PRIORITY(1));
	btEffectEndCanvasLayer();
}

#undef WINDOW_EDGE_FIRST_LINE
