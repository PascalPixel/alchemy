/* Burst: its glow and sparks, and its explosions on the target. Part of
   field_psynergy.c. */

/* The sparks that fly into Burst's glow, and its explosions round the
   target. */
#define BURST_SPARK_COUNT			  32
#define BURST_EXPLOSION_COUNT		  32
/* The tiles of one of an explosion's eight pictures, which follow each other
   from BURST_STATE.burstTile. */
#define BURST_EXPLOSION_PICTURE_TILES 4
/* The OBJ palette the explosions are drawn in. */
#define BURST_EXPLOSION_PALETTE		  15
/* How much of a screen shake is kept each tick. */
#define BURST_SHAKE_DECAY			  (toFIXED(9) / 10)
/* The explosions the last and largest blast starts, on its first tick and
   again on its fifth. */
#define BURST_FINAL_EXPLOSIONS		  10
/* The sparks drawn when Burst is cast at nothing. */
#define BURST_SPARKS_AT_NOTHING		  12

/* Burst's state (heap slot HEAP_SLOT_BATTLE_EFFECT): the glow disc that swells
   in front of the caster on the canvas sprite, the 32 sparks that fly into it,
   the 32 explosions round the target, and the stage of the cast. */
typedef struct BURST_STATE {
	/* The explosion's eight pictures, unpacked before they are loaded. */
	Uint8 tiles[8 * BURST_EXPLOSION_PICTURE_TILES * TILE_SIZE_4BPP];
	FIELD_PSYNERGY_PARTICLE sparks[BURST_SPARK_COUNT];
	EFFECT_SPR				sparkSprs[BURST_SPARK_COUNT];
	EFFECT_SPR				burstSprs[BURST_EXPLOSION_COUNT];
	FIELD_PSYNERGY_PARTICLE bursts[BURST_EXPLOSION_COUNT];
	/* the stage of the cast; FIELD_PSYNERGY_FINISHED once it ends */
	Sint16 phase;
	Sint16 tmr;	  /* ticks into the stage */
	Sint16 shake; /* passed to the target's script each tick (BURST_STAGE) */
	Uint16 burstTile; /* the explosion's first tile */
	Sint16 nextBurst; /* the explosion slot used next */
	Sint16 newBursts; /* explosions to start this tick */
	Sint8  palMode;	  /* an BURST_PALETTE_MODE */
} BURST_STATE;

/* BURST_STATE.palMode: what fdPsynergyUpdateBurst does to the palettes. */
typedef enum BURST_PALETTE_MODE {
	BURST_PALETTE_KEPT,		 /* nothing */
	BURST_PALETTE_SHINE,	 /* cycles the glow's greens each tick */
	BURST_PALETTE_EXPLOSIONS /* loads the explosions' palette, once */
} BURST_PALETTE_MODE;

/* BURST_STATE.phase: the stages of the cast, each lasting the ticks its case
   counts. */
typedef enum BURST_PHASE {
	BURST_PHASE_START,		  /* SE_SPIN */
	BURST_PHASE_SWELL,		  /* the glow grows */
	BURST_PHASE_GATHER,		  /* the sparks fly into it */
	BURST_PHASE_FIRST_BLASTS, /* explosions start, the screen darkens */
	BURST_PHASE_BLASTS,		  /* an explosion every 8 ticks */
	BURST_PHASE_HUSH,		  /* a pause */
	BURST_PHASE_GREAT_BLAST	  /* ten explosions, ten more 4 ticks on */
} BURST_PHASE;

/* The palette of Burst's explosions, loaded into BURST_EXPLOSION_PALETTE. */
extern const Uint16 BurstExplosionPalette[];

/* Moves the state on to its next phase, its timer restarted. */
#define BURST_NEXT_PHASE(state) ((state)->phase++, (state)->tmr = -1)

/* Burst each tick: shines the glow, draws the sparks and the explosions, and
   moves the cast on through its stages, shaking the screen at each blast. */
void fdPsynergyUpdateBurst(void)
{
	BURST_STATE				*state = (BURST_STATE *)RamHeapSlots->battleEff;
	FIELD_EFFECT_STATE		*scene = RamHeapSlots->fieldEffState;
	CANVAS_SPR_STATE		*canvas = RamHeapSlots->canvasSpr;
	FIELD_PSYNERGY_PARTICLE *particle;
	EFFECT_SPR				*spr;
	volatile Uint16			*palette;
	Sint32					 pos[3];
	Sint32					 i;

	state->shake = BURST_STAGE_QUIET;
	if(state->palMode == BURST_PALETTE_SHINE) {
		palette = (volatile Uint16 *)&OBJ_PLTT_COLOR(CANVAS_PALETTE, 0);
		/* The glow's 32 colours: red and blue full, green on a sine wave a 32nd
		   of a turn on from colour to colour, which moves on a colour every two
		   ticks. */
		for(i = 0; i < PALETTE_COLORS * 2; i++) {
			Sint32 red = RGB_LEVEL_MAX;
			Sint32 blue = RGB(0, 0, RGB_LEVEL_MAX);
			Uint32 ang = ((((Uint32)canvas->drawnFrm >> 1) + i) * TRIG_TURN +
							 TRIG_TURN * 9) /
				32;
			Sint32 green = RGB(0,
				FIXEDtoINT(sySin(ang) * CANVAS_SHINE_SWING) +
					CANVAS_SHINE_MIDDLE,
				0);

			syIOWrite16(green | blue | red, palette++);
		}
	} else if(state->palMode == BURST_PALETTE_EXPLOSIONS) {
		syDMAXCopy(BurstExplosionPalette,
			(void *)&OBJ_PLTT_COLOR(BURST_EXPLOSION_PALETTE, 0),
			DMA_COPY_HALFWORDS(16), REG_DMA3);
		state->palMode = BURST_PALETTE_KEPT;
	}
	pos[0] = scene->x;
	pos[1] = scene->y;
	pos[2] = scene->z;
	fdCameraWorldToScreen(pos);
	canvas->x = (Sint16)FIXEDtoINT(pos[0]);
	canvas->y = (Sint16)FIXEDtoINT(pos[2]);

	if(scene->aimAhead == 0) {
		particle = state->sparks;
		for(i = 0; i < BURST_SPARK_COUNT; i++) {
			if((Uint32)particle->tmr <= 29) {
				spr = &state->sparkSprs[i];
				spr->x = particle->x + toFIXED(canvas->x);
				spr->y = particle->y + toFIXED(canvas->y);
				fdEffectSprDraw(spr);
				fdAdvanceWithGravity2D(&particle->x, 60, 0);
			}
			particle->tmr++;
			particle++;
		}
		for(i = 0; i < state->newBursts; i++) {
			particle = &state->bursts[state->nextBurst];
			particle->tmr = 0;
			SET_POSITION(particle, scene->x, scene->y, scene->z);
			syVectorAddPolarOffset(
				syRandom() * 12 + toFIXED(4), syRandom(), &particle->x);
			/* UB: nextBurst is incremented and assigned, unsequenced. */
#if FIX_UNSEQUENCED_SIDE_EFFECT
			state->nextBurst = (state->nextBurst + 1) % BURST_EXPLOSION_COUNT;
#else
			state->nextBurst = ++state->nextBurst % BURST_EXPLOSION_COUNT;
#endif
		}
		state->newBursts = 0;
		i = 0;
		particle = state->bursts;
		for(; i < BURST_EXPLOSION_COUNT; i++, particle++) {
			if((Uint32)particle->tmr > 71)
				continue;
			spr = &state->burstSprs[i];
			switch(particle->tmr) {
			case 6:
				spr->oam.idx = state->burstTile + BURST_EXPLOSION_PICTURE_TILES;
				break;
			case 12:
				spr->oam.idx =
					state->burstTile + BURST_EXPLOSION_PICTURE_TILES * 2;
				break;
			case 22:
				spr->oam.idx =
					state->burstTile + BURST_EXPLOSION_PICTURE_TILES * 3;
				break;
			case 32:
				spr->oam.idx =
					state->burstTile + BURST_EXPLOSION_PICTURE_TILES * 4;
				break;
			case 42:
				spr->oam.idx =
					state->burstTile + BURST_EXPLOSION_PICTURE_TILES * 5;
				break;
			case 52:
				spr->oam.idx =
					state->burstTile + BURST_EXPLOSION_PICTURE_TILES * 6;
				break;
			case 62:
				spr->oam.idx =
					state->burstTile + BURST_EXPLOSION_PICTURE_TILES * 7;
				break;
			}
			fdEffectSprDrawAtWorldPos(spr, particle);
			particle->tmr++;
		}
		switch(state->phase) {
		case BURST_PHASE_START:
			if(state->tmr == 0)
				sySndPlayStub(SE_SPIN);
			if(state->tmr == 10)
				BURST_NEXT_PHASE(state);
			break;
		case BURST_PHASE_SWELL:
			canvas->scaleX += toFIXED(3) / 256;
			canvas->scaleY = canvas->scaleX;
			if(canvas->scaleX >= toFIXED(1) / 2) {
				state->phase++;
				state->tmr = 0;
			}
			break;
		case BURST_PHASE_GATHER:
			if(state->tmr == 40)
				BURST_NEXT_PHASE(state);
			break;
		case BURST_PHASE_FIRST_BLASTS:
			if(state->tmr == 0)
				state->palMode = BURST_PALETTE_EXPLOSIONS;
			if(state->tmr == 0)
				sySndPlayStub(SE_CLATTER);
			if((state->tmr & 7) == 0) {
				fdMapGridSetScreenShakeStub(
					toFIXED(3), toFIXED(3), BURST_SHAKE_DECAY);
				state->newBursts = 1;
				state->shake = BURST_STAGE_SMALL_BLAST;
			}
			canvas->command = CANVAS_UPLOAD_AND_DARKEN_FOUR_LEVELS;
			if(state->tmr == 16)
				BURST_NEXT_PHASE(state);
			break;
		case BURST_PHASE_BLASTS:
			if((state->tmr & 15) == 0)
				sySndPlayStub(SE_CLATTER);
			if((state->tmr & 7) == 0) {
				fdMapGridSetScreenShakeStub(
					toFIXED(3), toFIXED(3), BURST_SHAKE_DECAY);
				state->newBursts = 1;
				state->shake = BURST_STAGE_SMALL_BLAST;
			}
			if(state->tmr == 128)
				BURST_NEXT_PHASE(state);
			break;
		case BURST_PHASE_HUSH:
			if(state->tmr == 35)
				BURST_NEXT_PHASE(state);
			break;
		case BURST_PHASE_GREAT_BLAST:
			if(state->tmr == 0)
				state->newBursts = BURST_FINAL_EXPLOSIONS;
			if(state->tmr == 4)
				state->newBursts = BURST_FINAL_EXPLOSIONS;
			if(state->tmr == 0) {
				fdMapGridSetScreenShakeStub(
					toFIXED(12), toFIXED(12), BURST_SHAKE_DECAY);
				sySndPlayStub(SE_CRASH);
				if(state->tmr == 0)
					state->shake = BURST_STAGE_GREAT_BLAST;
			}
			if(state->tmr == 20)
				state->shake = BURST_STAGE_DYING_DOWN;
			if(state->tmr == 72) {
				state->phase = FIELD_PSYNERGY_FINISHED;
				state->tmr = -1;
			}
			break;
		}
	} else {
		particle = state->sparks;
		for(i = 0; i < BURST_SPARKS_AT_NOTHING; i++) {
			if((Uint32)particle->tmr <= 29) {
				spr = &state->sparkSprs[i];
				spr->x = particle->x + toFIXED(canvas->x);
				spr->y = particle->y + toFIXED(canvas->y);
				fdEffectSprDraw(spr);
				fdAdvanceWithGravity2D(&particle->x, 60, 0);
			}
			particle->tmr++;
			particle++;
		}
		for(i = 0; i < state->newBursts; i++) {
			particle = &state->bursts[state->nextBurst];
			particle->tmr = 0;
			SET_POSITION(particle, scene->x, scene->y, scene->z);
			/* UB: nextBurst is incremented and assigned, unsequenced. */
#if FIX_UNSEQUENCED_SIDE_EFFECT
			state->nextBurst = (state->nextBurst + 1) % BURST_EXPLOSION_COUNT;
#else
			state->nextBurst = ++state->nextBurst % BURST_EXPLOSION_COUNT;
#endif
		}
		state->newBursts = 0;
		i = 0;
		particle = state->bursts;
		for(; i < BURST_EXPLOSION_COUNT; i++, particle++) {
			if((Uint32)particle->tmr > 47)
				continue;
			spr = &state->burstSprs[i];
			switch(particle->tmr) {
			case 4:
				spr->oam.idx = state->burstTile + BURST_EXPLOSION_PICTURE_TILES;
				break;
			case 8:
				spr->oam.idx =
					state->burstTile + BURST_EXPLOSION_PICTURE_TILES * 2;
				break;
			case 14:
				spr->oam.idx =
					state->burstTile + BURST_EXPLOSION_PICTURE_TILES * 3;
				break;
			case 22:
				spr->oam.idx =
					state->burstTile + BURST_EXPLOSION_PICTURE_TILES * 4;
				break;
			case 28:
				spr->oam.idx =
					state->burstTile + BURST_EXPLOSION_PICTURE_TILES * 5;
				break;
			case 35:
				spr->oam.idx =
					state->burstTile + BURST_EXPLOSION_PICTURE_TILES * 6;
				break;
			case 41:
				spr->oam.idx =
					state->burstTile + BURST_EXPLOSION_PICTURE_TILES * 7;
				break;
			}
			fdEffectSprDrawAtWorldPos(spr, particle);
			particle->tmr++;
		}
		switch(state->phase) {
		case BURST_PHASE_START:
			if(state->tmr == 0)
				sySndPlayStub(SE_SPIN);
			if(state->tmr == 10)
				BURST_NEXT_PHASE(state);
			break;
		case BURST_PHASE_SWELL:
			canvas->scaleX += toFIXED(5) / 256;
			canvas->scaleY = canvas->scaleX;
			if(canvas->scaleX > toFIXED(0.3) - 1) {
				state->phase++;
				state->tmr = 0;
			}
			break;
		case BURST_PHASE_GATHER:
			if(state->tmr == 16)
				BURST_NEXT_PHASE(state);
			break;
		case BURST_PHASE_FIRST_BLASTS:
			if(state->tmr == 0) {
				state->palMode = BURST_PALETTE_EXPLOSIONS;
				if(state->tmr == 0) {
					fdMapGridSetScreenShakeStub(
						toFIXED(3), toFIXED(3), BURST_SHAKE_DECAY);
					state->newBursts = 1;
					sySndPlayStub(SE_CLATTER);
				}
			}
			canvas->command = CANVAS_UPLOAD_AND_DARKEN_FOUR_LEVELS;
			if(state->tmr == 16)
				BURST_NEXT_PHASE(state);
			break;
		case BURST_PHASE_BLASTS:
			/* Cast at nothing, the one blast is all: the caster lets go, and it
			   ends. */
			if(state->tmr == 5)
				fdBattleEffectSetObjectUpdateMode(
					scene->caster, CASTER_LOOK_NORMAL);
			if(state->tmr == 20) {
				state->phase = FIELD_PSYNERGY_FINISHED;
				state->tmr = -1;
			}
			break;
		}
	}
	state->tmr++;
}

/* Burst cast on a target: runs fdPsynergyBurst. */
void fdPsynergyBurstOnTarget(void)
{
	fdPsynergyBurst();
}

#undef BURST_NEXT_PHASE

/* The tile of the sparks that fly into Burst's glow, one 8x8 tile of 256
   colours. */
extern const Uint8 BurstGlowTiles[];

Sint32 fdPsynergyGetCanvasPriority(ACTOR *actor, Sint32 facing);

/* Burst: a glow disc swells in front of the caster as sparks fly into it, then
   explosions burst round the target, shaking the screen, the last and largest
   at 12 pixels; the target's script runs each tick with the stage (shake).
   Cast at nothing (FIELD_EFFECT_STATE.aimAhead still set), it is shorter: 12
   sparks and one small explosion. */
void fdPsynergyBurst(void)
{
	BURST_STATE *state =
		syHeapSlotAlloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(BURST_STATE));
	FIELD_EFFECT_STATE		*scene = RamHeapSlots->fieldEffState;
	CANVAS_SPR_STATE		*canvas;
	EFFECT_SPR				*spr;
	FIELD_PSYNERGY_PARTICLE *particle;
	void					*pending;
	Sint32					 pos[3];
	Sint32					 sparkEntry;
	Sint32					 sparkTile;
	Sint32					 burstEntry;
	Sint32					 tile;
	Sint32					 radius;
	Sint32					 ang;
	Sint32					 kind;
	Sint32					 frm;
	Sint32 (*mul)(Sint32, Sint32);
	EFFECT_SPR *burstSpr;
	Sint32		i;

	scene->y += toFIXED(10);
	fdPsynergyFadeToCastCol();
	fdBattleEffectStartCanvasSpr();
	canvas = RamHeapSlots->canvasSpr;
	pos[0] = scene->x;
	pos[1] = scene->y;
	pos[2] = scene->z;
	fdCameraWorldToScreen(pos);
	canvas->x = (Sint16)FIXEDtoINT(pos[0]);
	canvas->y = (Sint16)FIXEDtoINT(pos[2]);
	canvas->pri = fdPsynergyGetCanvasPriority(scene->caster, scene->ang);
	canvas->depth = fdActorGetSprDepth(scene->caster) + 2;
	canvas->mode = CANVAS_SPRITE_256_COLORS;
	canvas->scaleX = 0;
	canvas->scaleY = 0;
	if(scene->ang == TRIG_TURN * 3 / 4)
		canvas->depth -= 4;

	sparkEntry = syVRAMCacheFindFreeEntry();
	sparkTile = syVRAMCacheLoad(sparkEntry, TILE_SIZE_8BPP, BurstGlowTiles);
	for(i = 0, particle = state->sparks, mul = syMulFX, spr = state->sparkSprs;
		i < BURST_SPARK_COUNT;) {
		fdEffectSprInit(spr, 4, 4, OBJ_SIZE_8X8, sparkTile);
		spr->oam.fullCol = 1;
		spr->oam.pal = 0;
		spr->oam.pri = canvas->pri;
		spr->depth = canvas->depth + 1;
		spr->scaleY = spr->scaleX = toFIXED(1);
		radius = syRandom() * 20 + toFIXED(20);
		ang = syRandom();
		particle->x = mul(syCos(ang), radius);
		particle->y = mul(sySin(ang), radius);
		radius /= 12;
		ang += DEGtoANG(180);
		particle->velX = mul(syCos(ang), radius);
		particle->velY = mul(sySin(ang), radius);
		particle->tmr = -i * 2;
		i++;
		spr++;
		particle++;
	}

	syDecompressTaggedLZ(
		syResourceGetTableEntry(RESOURCE_FIELD_EFFECT_EXPLOSION), state);
	burstEntry = syVRAMCacheFindFreeEntry();
	tile = syVRAMCacheLoad(burstEntry, sizeof(state->tiles), state);
	state->burstTile = tile;
	particle = state->bursts;
	burstSpr = state->burstSprs;
	for(i = 0; i < BURST_EXPLOSION_COUNT; i++) {
		fdEffectSprInit(burstSpr, 8, 8, OBJ_SIZE_16X16, tile);
		burstSpr->depth = canvas->depth + 1;
		burstSpr->oam.pri = canvas->pri;
		burstSpr->oam.fullCol = 0;
		burstSpr->oam.pal = BURST_EXPLOSION_PALETTE;
		particle->tmr = -1;
		burstSpr++;
		particle++;
	}
	syDecompressTaggedLZ(syResourceGetTableEntry(RESOURCE_GLOW_DISC),
		RamHeapSlots->battleCanvas);
	canvas->command = CANVAS_UPLOAD;
	sySndPlayStub(SE_PSYNERGY_START);
	sySynch(1);
	state->phase = BURST_PHASE_START;
	state->tmr = 0;
	state->palMode = BURST_PALETTE_SHINE;
	state->nextBurst = 0;
	state->newBursts = 0;
	sySetEvent(fdPsynergyUpdateBurst, EVENT_PASS_TICK);
	kind = scene->fieldPsynergy;
	pending = fdFindPsynergyCue(
		PSYNERGY_CUE(PSYNERGY_PHASE_ACTS), kind, fdSelectPsynergyTarget(kind));
	frm = 0;
	while(state->phase != FIELD_PSYNERGY_FINISHED) {
		sySynch(1);
		if(state->phase == FIELD_PSYNERGY_FINISHED)
			frm = FIELD_PSYNERGY_FINISHED;
		fdRunPsynergyScript(pending, state->shake, frm);
		frm++;
	}
	syCloseEvent(fdPsynergyUpdateBurst);
	syVRAMCacheFreeEntry(sparkEntry);
	syVRAMCacheFreeEntry(burstEntry);
	fdPsynergyFadeFromCastCol();
	fdBattleEffectEndCanvasSpr();
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
}
