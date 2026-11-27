/* Timed map changes: the tileset animations that redraw a map's tiles in turn,
   the blend script that changes over time how its layers blend, and the palette
   cycles, which nothing starts. Part of field.c. */

#include "field_internal.h"

/* A tileset animation script's commands, a halfword each where a source tile
   would stand (fdTilesetAnimUpdate and fdTilesetAnimStartAll). */
#define TILESET_ANIM_LOOP				   0xffff /* back to the start of the script */
#define TILESET_ANIM_OPCODE_MASK		   0xff00 /* the high byte: the command */
#define TILESET_ANIM_JUMP				   0xfe00 /* low byte: the halfword pair to jump to */
#define TILESET_ANIM_STOP				   0xff /* TILESET_ANIM_JUMP's low byte to stop */
/* The low byte: the command's argument. */
#define TILESET_ANIM_ARGUMENT_MASK		   0xff
/* A layout with wide tiles animates 64-byte (256-colour) tiles in char block 2;
   others animate 16-colour ones in char block 1. */
#define TILESET_ANIM_WIDE_TILE_BYTES	   (TILE_SIZE_4BPP * 2)
/* low four bits: the animation it starts */
#define TILESET_ANIM_MARKER				   0xfd00
#define TILESET_ANIM_MARKER_PAUSED		   0x80 /* a marker's bit: start it paused */
#define TILESET_ANIM_MARKER_ANIMATION_MASK 0xf /* a marker's animation */
/* The first source tile read from MapGridAnimTiles rather than VRAM, for
   32-byte and for 64-byte (wide) tiles: both bases put it at its start. */
#define TILESET_ANIM_RAM_TILE			   1536
#define TILESET_ANIM_RAM_WIDE_TILE		   512

/* The blend script's commands (fdBlendScriptRun): as the tileset animations',
   and a BLDCNT value, told apart by its top four bits. */
#define BLEND_SCRIPT_LOOP		   0xffff
#define BLEND_SCRIPT_OPCODE_MASK   0xff00
#define BLEND_SCRIPT_JUMP		   0xfe00
#define BLEND_SCRIPT_STOP		   0xff
#define BLEND_SCRIPT_ARGUMENT_MASK 0xff
#define BLEND_SCRIPT_BLDCNT_MASK   0xf000
#define BLEND_SCRIPT_BLDCNT		   (BLDCNT_TGT2_OBJ | BLDCNT_TGT2_BD)

/* BLDCNT's effect, bits 7 and 6 (BLDCNT_EFFECT_NONE to
   BLDCNT_EFFECT_DARKEN). */
#define BLDCNT_EFFECT_MASK 0x00c0

/* Each frame: runs the 16 tileset animations. */
void fdTilesetAnimUpdate(void)
{
	MAP_GRID_SCROLL_STATE *gridState = MapGridState[0];
	Uint32				   channelIndex;

	for(channelIndex = 0; channelIndex < ARRAY_COUNT(gridState->anms);
		channelIndex++) {
		const Uint16 *commands;
		Uint32		  sourceTileOrOpcode;
		Uint32		  tileCount;
		Uint32		  destinationTile;

		/* A tileset animation with no script, or paused by a script, keeps its
		   place. */
		if((gridState->anms + channelIndex)->start == NULL ||
			(gridState->anms + channelIndex)->paused != 0)
			continue;
		/* While the wait lasts the tileset animation only counts down; when it
		   is over, commands run in this same frame until one sets a new wait or
		   stops. */
	next:
		if((gridState->anms + channelIndex)->tmr == 0) {
			commands = (gridState->anms + channelIndex)->cursor;
			sourceTileOrOpcode = *commands++;
			if(sourceTileOrOpcode == TILESET_ANIM_LOOP) {
				(gridState->anms + channelIndex)->cursor =
					(gridState->anms + channelIndex)->start;
				goto next;
			}
			/* TILESET_ANIM_STOP leaves the cursor on itself, so the tileset
			   animation stays stopped until it is started again;
			   TILESET_ANIM_JUMP jumps to the halfword pair its low byte
			   names. */
			if((sourceTileOrOpcode & TILESET_ANIM_OPCODE_MASK) ==
				TILESET_ANIM_JUMP) {
				if((sourceTileOrOpcode & TILESET_ANIM_ARGUMENT_MASK) ==
					TILESET_ANIM_STOP)
					continue;
				(gridState->anms + channelIndex)->cursor =
					(gridState->anms + channelIndex)->start +
					(sourceTileOrOpcode & TILESET_ANIM_ARGUMENT_MASK) * 2;
				goto next;
			}
			tileCount = *commands++;
			destinationTile = commands[0];
			(gridState->anms + channelIndex)->tmr = commands[1];
			/* A source tile from TILESET_ANIM_RAM_TILE
			   (TILESET_ANIM_RAM_WIDE_TILE for 64-byte tiles) is read from RAM,
			   at the start of MapGridAnimTiles. */
			if(gridState->wideTiles == 0) {
				if(sourceTileOrOpcode >= TILESET_ANIM_RAM_TILE)
					syDMAXCopy(
						Ram_DecodeBuffer + sourceTileOrOpcode * TILE_SIZE_4BPP,
						BG_CHAR_BLOCK(1) + destinationTile * TILE_SIZE_4BPP,
						DMA_COPY_WORDS(tileCount * (TILE_SIZE_4BPP / 4)),
						REG_DMA3);
				else
					syDMAXCopy(
						BG_CHAR_BLOCK(1) + sourceTileOrOpcode * TILE_SIZE_4BPP,
						BG_CHAR_BLOCK(1) + destinationTile * TILE_SIZE_4BPP,
						DMA_COPY_WORDS(tileCount * (TILE_SIZE_4BPP / 4)),
						REG_DMA3);
			} else {
				if(sourceTileOrOpcode >= TILESET_ANIM_RAM_WIDE_TILE)
					syDMAXCopy(Ram_MetatileTiles +
							sourceTileOrOpcode * TILESET_ANIM_WIDE_TILE_BYTES,
						BG_CHAR_BLOCK(2) +
							destinationTile * TILESET_ANIM_WIDE_TILE_BYTES,
						DMA_COPY_WORDS(
							tileCount * (TILESET_ANIM_WIDE_TILE_BYTES / 4)),
						REG_DMA3);
				else
					syDMAXCopy(BG_CHAR_BLOCK(2) +
							sourceTileOrOpcode * TILESET_ANIM_WIDE_TILE_BYTES,
						BG_CHAR_BLOCK(2) +
							destinationTile * TILESET_ANIM_WIDE_TILE_BYTES,
						DMA_COPY_WORDS(
							tileCount * (TILESET_ANIM_WIDE_TILE_BYTES / 4)),
						REG_DMA3);
			}
			/* A copy with a wait of 0 lets the next command run at once. */
			(gridState->anms + channelIndex)->cursor += 4;
			goto next;
		} else {
			(gridState->anms + channelIndex)->tmr--;
		}
	}
}

/* Resumes tileset animation channelIndex by clearing its paused byte. */
void fdTilesetAnimResume(Uint32 channelIndex)
{
	MAP_GRID_SCROLL_STATE *gridState = MapGridState[0];
	TILESET_ANIM		  *channel = &gridState->anms[channelIndex];

	channel->paused = 0;
}

/* Pauses tileset animation channelIndex by setting its paused byte. */
void fdTilesetAnimPause(Uint32 channelIndex)
{
	MAP_GRID_SCROLL_STATE *gridState = MapGridState[0];
	TILESET_ANIM		  *channel = &gridState->anms[channelIndex];

	channel->paused = 1;
}

/* Starts the tileset animations a map's animation script lists: each
   TILESET_ANIM_MARKER starts the tileset animation its low four bits name there
   (paused with TILESET_ANIM_MARKER_PAUSED). */
void fdTilesetAnimStartAll(const Uint16 *script)
{
	MAP_GRID_SCROLL_STATE *gridState;
	Sint32				   channelsStarted;
	Uint32				   marker;

	gridState = MapGridState[0];
	channelsStarted = 0;
	DMA_FILL32(0, gridState->anms, sizeof(gridState->anms));
	/* Each tileset animation's script starts just after its marker and runs on
	   through the rest of the stream; the scan steps over the halfwords between
	   markers. */
	marker = *script++;
	/* The stream ends at the TILESET_ANIM_LOOP every script after a marker runs
	   on to. */
	while(marker != TILESET_ANIM_LOOP) {
		if((marker & TILESET_ANIM_OPCODE_MASK) == TILESET_ANIM_MARKER) {
			TILESET_ANIM *channel;
			Uint32 channelIndex = marker & TILESET_ANIM_MARKER_ANIMATION_MASK;
			Uint32 paused = 0;
			if(marker & TILESET_ANIM_MARKER_PAUSED)
				paused = 1;
			channel = &gridState->anms[channelIndex];
			channel->start = script;
			channel->cursor = script;
			channel->tmr = 0;
			channel->paused = paused;
			channelsStarted++;
		}
		marker = *script++;
	}
	/* A map with no tileset animations never adds the update. */
	if(channelsStarted != 0)
		sySetEvent(fdTilesetAnimUpdate, EVENT_PASS_FRAME);
}

/* Suspends the tileset animations, unless the world map's still image is
   shown. */
void fdTilesetAnimSuspendAll(void)
{
	if(((MAP_GRID_SCROLL_STATE *)MapGridState[0])->anmActive == 0)
		sySuspendEvent(fdTilesetAnimUpdate);
}

/* Resumes the tileset animations, unless the world map's still image is
   shown. */
void fdTilesetAnimResumeAll(void)
{
	if(((MAP_GRID_SCROLL_STATE *)MapGridState[0])->anmActive == 0)
		syResumeEvent(fdTilesetAnimUpdate);
}

/* Each frame: runs the map's blend script. */
void fdBlendScriptRun(void)
{
	MAP_GRID_SCROLL_STATE *gridState = MapGridState[0];
	BLEND_SCRIPT_STATE	  *blend;
	Uint16				  *commands;
	Uint32				   instruction;
	Uint32				   delayOrJumpIndex;
	Uint16				   delay;

	blend = &gridState->blend;
	if(blend->script == 0)
		return;
	if(blend->paused != 0)
		return;

	/* As with the tileset animations, commands run in one frame until a level
	   sets a wait; a BLDCNT command and the jumps take no time. */
again:
	delayOrJumpIndex = blend->delay;
	delay = delayOrJumpIndex;
	if(delay == 0) {
		commands = blend->cursor;
		instruction = *commands;
		commands++;
		if(instruction == BLEND_SCRIPT_LOOP) {
			blend->cursor = blend->script;
			goto again;
		}

		if((instruction & BLEND_SCRIPT_OPCODE_MASK) == BLEND_SCRIPT_JUMP) {
			delayOrJumpIndex = instruction & BLEND_SCRIPT_ARGUMENT_MASK;
			if(delayOrJumpIndex == BLEND_SCRIPT_STOP)
				return;
			blend->cursor = blend->script + delayOrJumpIndex * 2;
			goto again;
		}

		/* A BLDCNT value is told apart by its top four bits being 3, its second
		   target's sprite and backdrop bits. */
		if((instruction & BLEND_SCRIPT_BLDCNT_MASK) == BLEND_SCRIPT_BLDCNT) {
			REG_BLDCNT = instruction;
			gridState->blendCtrl = instruction;
			blend->cursor++;
			goto again;
		}

		/* An alpha blend takes the level as BLDALPHA; a fade to white or black
		   takes it as BLDY. */
		if((gridState->blendCtrl & BLDCNT_EFFECT_MASK) == BLDCNT_EFFECT_BLEND)
			REG_BLDALPHA = instruction;
		else
			REG_BLDY = instruction;
		blend->delay = commands[0];
		blend->cursor += 2;
		goto again;
	}
	blend->delay = delayOrJumpIndex - 1;
}

/* Starts a map's blend script, unless it is empty. Clears the map state's
   three-word blend state first, then adds fdBlendScriptRun as an event. */
void fdBlendScriptStart(Uint16 *script)
{
	BLEND_SCRIPT_STATE *blend;
	Sint32				hasScript;
	volatile Uint32		zero;

	/* A fixed-source DMA of the zero word clears script, cursor, delay and
	   paused; a script that is only its end marker leaves the blend unset. */
	hasScript = 0;
	zero = 0;
	blend = &((MAP_GRID_SCROLL_STATE *)MapGridState[0])->blend;
	syDMAXCopy((const void *)&zero, blend, DMA_FILL_WORDS(3), REG_DMA3);
	if(*script != BLEND_SCRIPT_LOOP) {
		blend->script = script;
		blend->cursor = script;
		blend->delay = 0;
		blend->paused = 0;
		hasScript = 1;
	}
	if(hasScript)
		sySetEvent(fdBlendScriptRun, EVENT_PASS_FRAME);
}

/* Suspends the blend script. The lighthouse scenes and Vale's Babi script reach
   it through fdBlendScriptSuspendStub. */
void fdBlendScriptSuspend(void)
{
	sySuspendEvent(fdBlendScriptRun);
}

/* Resumes the blend script, through fdBlendScriptResumeStub. */
void fdBlendScriptResume(void)
{
	syResumeEvent(fdBlendScriptRun);
}

/* Unused: allocates the palette cycling queue (HEAP_SLOT_PALETTE_CYCLES) with
   its four entries cleared. */
UNUSED void fdPaletteCycleInit(void)
{
	PALETTE_CYCLE_STATE *paletteCycles;
	PALETTE_CYCLE		*cycle;
	Uint16				 cycleIndex;
	Uint16				 colorIndex;

	paletteCycles = syHeapSlotAllocEWRAM(
		HEAP_SLOT_PALETTE_CYCLES, sizeof(PALETTE_CYCLE_STATE));
	cycle = paletteCycles->entries;
	for(cycleIndex = 0; cycleIndex != ARRAY_COUNT(paletteCycles->entries);
		cycleIndex++) {
		cycle->src = 0;
		cycle->pos = 0;
		cycle->tmr = 0;
		cycle->delay = 0;
		cycle->cnt = 0;
		for(colorIndex = 0; colorIndex != ARRAY_COUNT(cycle->cols);
			colorIndex++) {
			cycle->cols[colorIndex] = 0;
		}
		cycle++;
	}
	paletteCycles->cnt = 0;
}

/* Unused: adds a palette cycle: colorCount colours of background palette
   paletteNumber from firstColor, rotated by one every delay + 1 frames. Returns
   0, or -1 when all four cycles are taken. The cycle keeps a copy of the
   colours as they were, which every step rotates from. */
UNUSED Sint32 fdPaletteCycleAdd(
	Sint16 paletteNumber, Sint16 firstColor, Sint16 delay, Sint16 colorCount)
{
	PALETTE_CYCLE_STATE *paletteQueue;
	PALETTE_CYCLE		*cycle;
	Uint16				*paletteColors;
	Uint32				 cycleIndex;
	Uint16				 colorCount16;

	paletteQueue = PaletteCycleStateWork;
	cycleIndex = paletteQueue->cnt;
	if(cycleIndex >= ARRAY_COUNT(paletteQueue->entries))
		return -1;
	/* The cycle saves the colours as they are now. */
	cycle = &paletteQueue->entries[cycleIndex];
	paletteColors = (Uint16 *)BG_PLTT +
		(((Uint16)paletteNumber * PALETTE_COLORS) + (Uint16)firstColor);
	colorCount16 = colorCount;
	cycle->pos = 0;
	cycle->tmr = 0;
	cycle->cnt = colorCount16;
	cycle->src = paletteColors;
	cycle->delay = delay;
	syDMAXCopy(
		paletteColors, cycle->cols, DMA_COPY_HALFWORDS(colorCount16), REG_DMA3);
	paletteQueue->cnt++;
	return 0;
}

/* Unused: stops the palette cycles and frees their queue
   (HEAP_SLOT_PALETTE_CYCLES). */
UNUSED void fdPaletteCycleStop(void)
{
	syCloseEvent(fdPaletteCycleStep);
	syHeapSlotRelease(HEAP_SLOT_PALETTE_CYCLES);
}

/* Unused: starts running the palette cycles each frame. */
UNUSED void fdPaletteCycleStart(void)
{
	sySetEvent(fdPaletteCycleStep, EVENT_PASS_FRAME);
}

/* Each frame: advances each palette cycle whose timer has run out by one
   colour, writing the rotated colours back to the palette. */
void fdPaletteCycleStep(void)
{
	PALETTE_CYCLE_STATE *paletteQueue = PaletteCycleStateWork;
	Uint8				 cycleIndex;
	/* BUG: four queued cycles become zero here, so none advance. */
	Uint16 activeCycles = paletteQueue->cnt & 3;
	Uint16 rotatedColors[PALETTE_COLORS];

	for(cycleIndex = 0; cycleIndex < activeCycles; cycleIndex++) {
		PALETTE_CYCLE *cycle = &paletteQueue->entries[cycleIndex];
		if(cycle->tmr == 0) {
			Uint16	rotationOffset = cycle->pos;
			Uint16	colorCount = cycle->cnt;
			void   *paletteColors = cycle->src;
			Uint16 *savedColors = cycle->cols;
			Uint8	colorIndex;

			/* Each saved colour moves rotationOffset places down, the first
			   ones wrapping round to the end. */
			for(colorIndex = colorCount - rotationOffset;
				colorIndex < colorCount; colorIndex++)
				rotatedColors[colorIndex] = *savedColors++;
			for(colorIndex = 0; colorIndex < colorCount - rotationOffset;
				colorIndex++)
				rotatedColors[colorIndex] = *savedColors++;
			syDMAXCopy(rotatedColors, paletteColors,
				DMA_COPY_HALFWORDS(colorCount), REG_DMA3);
			/* The next step rotates one place further, back to none after a
			   full turn, and waits delay frames. */
			rotationOffset++;
			if(rotationOffset >= colorCount)
				rotationOffset = 0;
			cycle->pos = rotationOffset;
			cycle->tmr = cycle->delay;
		} else {
			cycle->tmr--;
		}
	}
}

#undef TILESET_ANIM_LOOP
#undef TILESET_ANIM_OPCODE_MASK
#undef TILESET_ANIM_JUMP
#undef TILESET_ANIM_STOP
#undef TILESET_ANIM_ARGUMENT_MASK
#undef TILESET_ANIM_MARKER
#undef TILESET_ANIM_MARKER_PAUSED
#undef TILESET_ANIM_MARKER_ANIMATION_MASK
#undef TILESET_ANIM_RAM_TILE
#undef TILESET_ANIM_RAM_WIDE_TILE
#undef BLEND_SCRIPT_LOOP
#undef BLEND_SCRIPT_OPCODE_MASK
#undef BLEND_SCRIPT_JUMP
#undef TILESET_ANIM_WIDE_TILE_BYTES
#undef BLEND_SCRIPT_STOP
#undef BLEND_SCRIPT_ARGUMENT_MASK
#undef BLEND_SCRIPT_BLDCNT_MASK
#undef BLEND_SCRIPT_BLDCNT
#undef BLDCNT_EFFECT_MASK
