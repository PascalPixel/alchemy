// Running summon and table effects, and shared target helpers. Part of battle_effect.c.

// Calls the empty btEffectNoOperation count times: a busy delay.
void btEffectCallNoOperation(Sint32 count)
{
	Sint32 i;

	for (i = 0; i != count; i++)
		btEffectNoOperation();
}

// Runs one of the large battle effects, the summons and the scenes of a few abilities: takes the
// effect's sprite sheet, state and canvas, runs the effect of args->kind (SUMMON_EFFECT_ID) and
// frees them again.
void btEffectRunSummon(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;

	syHeapSlotAllocate(HEAP_SLOT_BATTLE_SHEET, MOTE_SET_BYTES);
	syHeapSlotAllocate(HEAP_SLOT_BATTLE_EFFECT, BATTLE_EFFECT_STATE_SIZE);
	syHeapSlotAllocate(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
	state                      = RamHeapSlots->battleEffect;
	state->args                = args;
	state->framesSinceTransfer = 1;
	if (args->kind == SUMMON_EFFECT_NONE)
	{
		btEffectRunMeteor(args);
	}
	else
	{
		switch (args->kind)
		{
			case SUMMON_EFFECT_RAMSES         : btEffectRunRamses(args); break;
			case SUMMON_EFFECT_NEREID         : btEffectRunNereid(args); break;
			case SUMMON_EFFECT_KIRIN          : btEffectRunKirin(args); break;
			case SUMMON_EFFECT_ATALANTA       : btEffectRunAtalanta(args); break;
			case SUMMON_EFFECT_CYBELE         : btEffectRunCybele(args); break;
			case SUMMON_EFFECT_NEPTUNE        : btEffectRunNeptune(args); break;
			case SUMMON_EFFECT_TIAMAT         : btEffectRunTiamat(args); break;
			case SUMMON_EFFECT_PROCNE         : btEffectRunProcne(args); break;
			case SUMMON_EFFECT_JUDGMENT       : btEffectRunJudgment(args); break;
			case SUMMON_EFFECT_BOREAS         : btEffectRunBoreas(args); break;
			case SUMMON_EFFECT_METEOR         : btEffectRunMeteor(args); break;
			case SUMMON_EFFECT_THOR           : btEffectRunThor(args); break;
			case SUMMON_EFFECT_ZAGAN          : btEffectRunZagan(args); break;
			case SUMMON_EFFECT_MEGAERA        : btEffectRunMegaera(args); break;
			case SUMMON_EFFECT_FLORA          : btEffectRunFlora(args); break;
			case SUMMON_EFFECT_MOLOCH         : btEffectRunMoloch(args); break;
			case SUMMON_EFFECT_ULYSSES        : btEffectRunUlysses(args); break;
			case SUMMON_EFFECT_HAURES         : btEffectRunHaures(args); break;
			case SUMMON_EFFECT_ECLIPSE        : btEffectRunEclipse(args); break;
			case SUMMON_EFFECT_COATLICUE      : btEffectRunCoatlicue(args); break;
			case SUMMON_EFFECT_DAEDALUS       : btEffectRunDaedalus(args); break;
			case SUMMON_EFFECT_AZUL           : btEffectRunAzul(args); break;
			case SUMMON_EFFECT_CATASTROPHE    : btEffectRunCatastrophe(args); break;
			case SUMMON_EFFECT_CHARON         : btEffectRunCharon(args); break;
			case SUMMON_EFFECT_IRIS           : btEffectRunIris(args); break;
			case SUMMON_EFFECT_DAEDALUS_STRIKE: btEffectRunDaedalusStrike(args); break;
			case SUMMON_EFFECT_TRIDENT        : btEffectRunTrident(args); break;
			case SUMMON_EFFECT_CRUEL_RUIN     : btEffectRunCruelRuin(args); break;
			case SUMMON_EFFECT_METEOR_BLOW    : btEffectRunMeteorBlow(args); break;
		}
	}
	syHeapSlotRelease(HEAP_SLOT_BATTLE_CANVAS);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_SHEET);
}

// Runs an ordinary battle effect from BattleEffectHandlers, with the sprite sheet, state and canvas
// it lends.
void btEffectRunFromTable(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;

	syHeapSlotAllocate(HEAP_SLOT_BATTLE_SHEET, MOTE_SET_BYTES);
	syHeapSlotAllocate(HEAP_SLOT_BATTLE_EFFECT, BATTLE_EFFECT_STATE_SIZE);
	syHeapSlotAllocate(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
	state                      = RamHeapSlots->battleEffect;
	state->args                = args;
	state->framesSinceTransfer = 1;
	BattleEffectHandlers[args->kind - 1](args);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_CANVAS);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_SHEET);
}

// Builds window 0's right edge for every line and restarts the DMA that writes it.
void btEffectBuildWindowEdgeTable(void)
{
	Uint16 *baseRightEdge;
	Uint16 *scanlineEdges;
	Uint8  *edgeOffsets;
	Sint32  scanline;
	Sint32  rightEdge;

	baseRightEdge = WINDOW_EDGE_RIGHT;
	scanlineEdges = (Uint16 *)WINDOW_EDGE_LINES;
	edgeOffsets   = WINDOW_EDGE_INSETS;
	for (scanline = 0; scanline != DISPLAY_HEIGHT; scanline++)
	{
		if (scanline >= 8 && scanline < 8 + WINDOW_EDGE_INSET_LINES)
		{
			rightEdge = *baseRightEdge - edgeOffsets[scanline - 8];
			CLAMP(rightEdge, 0, DISPLAY_WIDTH);
			scanlineEdges[scanline] = rightEdge;
		}
		else
		{
			scanlineEdges[scanline] = WIN_RANGE(255, 241);
		}
	}
	{
		volatile Uint16 *channel   = REG_DMA0;

		channel[DMA_CONTROL_HALF] &= (Uint16)~DMA_START_TIMING_DRQ_REPEAT;
		channel[DMA_CONTROL_HALF] &= (Uint16)~DMA_ENABLE;
		(void)channel[DMA_CONTROL_HALF];
		syDMAXCopy(WINDOW_EDGE_LINES,
		           (void *)&REG_WIN0H,
		           DMA_HBLANK_HALFWORD_PER_LINE,
		           (volatile Uint32 *)channel);
	}
}

// Shows the battle objects of every target with HP left.
void btEffectShowLivingTargets(BATTLE_EFFECT_PARM *args)
{
	Sint16 list[UNIT_MAX];
	Sint32 count;
	Sint32 i;

	count = 0;
	for (i = 0; i != args->targetCount; i++)
	{
		if (syUnitGet(args->targets[i])->hp > 0)
			list[count++] = args->targets[i];
	}
	list[count] = BATTLE_UNIT_LIST_END;
	btActorSpawnObjectsForListStub(list, 0);
}
