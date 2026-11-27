/* The two ways into an effect, a summon or an ordinary ability, with the sheet,
   state and canvas they lend it; and the helpers the summons share for window
   0's ragged edge and the targets' sprites. Part of battle_effect.c. */

#include "battle_effect/effect_call_no_operation.c"

/* Runs one of the large battle effects, the summons: takes the effect's sheet,
   state and canvas, runs it and frees them again. */
void btEffectRunSummon(BATTLE_EFFECT_PARM *args)
{
	syHeapSlotAlloc(HEAP_SLOT_BATTLE_SHEET, MOTE_SET_BYTES);
	syHeapSlotAlloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(BATTLE_EFFECT_STATE));
	syHeapSlotAlloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);

	if(args->kind == SUMMON_EFFECT_NONE) {
		btEffectRunMeteor(args);
	} else {
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

/* Runs an ordinary battle effect from BattleEffectHandlers, with the sprite
   sheet, state and canvas it lends. */
void btEffectRunFromTable(BATTLE_EFFECT_PARM *args)
{
	Sint32				 kind;
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
		BattleEffectHandlers[kind - 1](args);

	syHeapSlotRelease(HEAP_SLOT_BATTLE_CANVAS);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_SHEET);
}

/* The first screen line of window 0's ragged edge. */
#define WINDOW_EDGE_FIRST_LINE 8

/* Builds window 0's right edge for every line, ragged by each line's inset, and
   restarts the DMA that writes it; the summons run it each VBlank while the
   window opens across the screen. */
void btEffectBuildWindowEdgeTable(void)
{
	Uint16 *baseRightEdge;
	Uint16 *scanlineEdges;
	Uint8  *edgeOffsets;
	Sint32	scanline;
	Sint32	rightEdge;

	baseRightEdge = WINDOW_EDGE_RIGHT;
	scanlineEdges = (Uint16 *)WINDOW_EDGE_LINES;
	edgeOffsets = WINDOW_EDGE_INSETS;
	for(scanline = 0; scanline != DISPLAY_HEIGHT; scanline++) {
		if(scanline >= WINDOW_EDGE_FIRST_LINE &&
			scanline < WINDOW_EDGE_FIRST_LINE + WINDOW_EDGE_INSET_LINES) {
			rightEdge =
				*baseRightEdge - edgeOffsets[scanline - WINDOW_EDGE_FIRST_LINE];
			CLAMP(rightEdge, 0, DISPLAY_WIDTH);
			scanlineEdges[scanline] = rightEdge;
		} else {
			/* Empty: the left edge is past the screen's right. */
			scanlineEdges[scanline] = WIN_RANGE(255, 241);
		}
	}
	{
		volatile Uint16 *channel = REG_DMA0;

		btDMAStop(channel);
		syDMAXCopy(WINDOW_EDGE_LINES, &REG_WIN0H, DMA_HBLANK_HALFWORD_PER_LINE,
			(volatile Uint32 *)channel);
	}
}

/* Spawns the battle sprites of every unit with HP left on the targets' side. */
void btEffectShowLivingTargets(BATTLE_EFFECT_PARM *args)
{
	Sint16 list[UNIT_MAX];
	Sint32 cnt;
	Sint32 i;

	cnt = 0;
	if(args->tgts[0] >= FIRST_ENEMY_UNIT) {
		for(i = 0; i != ENEMY_BATTLE_SIZE; i++) {
			Sint32 id = i + FIRST_ENEMY_UNIT;

			if(gmUnitGetStub(id)->hp > 0)
				list[cnt++] = id;
		}
	} else {
		for(i = 0; i != PARTY_UNIT_COUNT; i++) {
			if(gmUnitGetStub(i)->hp > 0)
				list[cnt++] = i;
		}
	}
	list[cnt] = BATTLE_UNIT_LIST_END;
	btObjectSpawnObjectsForListStub(list, 0);
}

/* Shows the battle background, darkened as a canvas leaves it, behind a cleared
   canvas. */
void btEffectSetupBlendedDisp(void)
{
	void **slots = &HeapSlots[HEAP_SLOT_BATTLE];
	Uint8 *canvas =
		HEAP_SLOT_FROM(slots, HEAP_SLOT_BATTLE, HEAP_SLOT_BATTLE_CANVAS);
	BATTLE_STATE   *battle = slots[0];
	volatile Uint32 fill;

	REG_DISPCNT = DISPCNT_MODE_1;
	BgScrollWork[1].y = BATTLE_BACKGROUND_SCROLL_Y;
	btBackgroundLoadStub(
		BATTLE_BACKGROUND_FIXED, battle->bg, BACKGROUND_DARKENED_LEVEL);
	DMA_FILL32_CELL(fill, 0, canvas, CANVAS_BYTES);
	DMA_FILL32_CELL(fill, 0, BG_CHAR_BLOCK(1), CANVAS_BYTES);
	sySynch(1);
	REG_BLDALPHA = EFFECT_BLEND_CANVAS(CANVAS_BLEND_LEVEL);
	REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND |
		BLDCNT_TGT2_ALL;
	REG_DISPCNT = EFFECT_DISPCNT;
	ProjectionWork.centerY = BATTLE_PROJECTION_CENTER_Y;
}

/* Tints one unit's sprite: every part but its shadow takes `palette`
   (SPRITE_COLOR_NORMAL gives the unit its own back), the sprite starts
   `anm`, and timer `tmrSlot` of the effect counts `tmrTicks` down to
   the tint's end (btEffectCountDownBattlerTints); TINT_KEEP leaves any of the
   three as it is. */
void btEffectTintBattler(
	Sint32 unitId, Sint32 palette, Sint32 anm, Sint32 tmrSlot, Sint32 tmrTicks)
{
	BATTLE_OBJECT		*slot;
	SPR					*group;
	BATTLE_EFFECT_STATE *state;
	Sint32				 groupIdx;

	slot = btObjectGetStub(unitId);
	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);

	for(groupIdx = 0;
		(group = btActorGetSprStub(slot->actor, groupIdx)) != NULL;
		groupIdx++) {
		if(tmrSlot != TINT_KEEP) {
			state->actorTmrs[tmrSlot] = tmrTicks;
		}
		if(slot->fading == 0) {
			if(palette != TINT_KEEP) {
				Sint32 partIdx;

				for(partIdx = 0; partIdx != group->cnt; partIdx++) {
					SPR_ANIMATION *part = group->entries[partIdx];

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
			if(anm != TINT_KEEP)
				fdSprStartAnimationStub(group, anm);
		}
	}
}

/* Opens the tiled canvas at priority 1 and closes it again at once. */
UNUSED void btEffectOpenAndCloseTiledCanvas(void)
{
	btEffectStartTiledCanvas(BGCNT_PRIORITY(1));
	btEffectEndCanvasLayer();
}

#undef WINDOW_EDGE_FIRST_LINE
