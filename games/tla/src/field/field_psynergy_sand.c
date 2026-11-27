/* Sand: the leader sinking into the ground and moving as sand, the grains it
   throws up, the event that ends it off sand, and the dust puff it ends with.
   Part of field_psynergy.c. */

/* The grains of sand thrown up round the leader, and how many of them are drawn
   as it sinks and as it rises. */
#define SAND_GRAIN_COUNT		64
#define SAND_SINK_GRAINS		33
#define SAND_RISE_GRAINS		30
/* The leader's picture, 32 pixels square; Sand takes only a leader drawn so. */
#define SAND_PICTURE_SIZE		32
/* The phase the leader's sunk picture rises from, 1.5 less each tick. */
#define SAND_RISE_PHASE			92
/* An OAM attribute's high byte: the bits below its top two, the shape in
   attribute 0 and the size in attribute 1, and the size bits of a sprite 32
   pixels square. */
#define OAM_BELOW_SHAPE_OR_SIZE 63
#define OAM_SIZE_32X32_BYTE		(OBJ_SIZE_32X32 >> 24)

/* Sand's state (heap slot HEAP_SLOT_BATTLE_EFFECT): the leader's picture and
   the copy it sinks into the ground, the grains of sand thrown up round it and
   the sprite the sinking leader is drawn with. */
typedef struct SAND_STATE {
	/* The leader's picture, 8 bits a pixel. */
	Uint8 picture[SAND_PICTURE_SIZE * SAND_PICTURE_SIZE];
	/* The picture with its columns sunk by a wave. */
	Uint8					sunken[SAND_PICTURE_SIZE * SAND_PICTURE_SIZE];
	EFFECT_SPR				grainSprs[SAND_GRAIN_COUNT];
	FIELD_PSYNERGY_PARTICLE grains[SAND_GRAIN_COUNT];
	EFFECT_SPR				leader;
	Uint8					unused[4];
} SAND_STATE;

/* The ROM's callers here are compiled against this form, which returns an
   Sint32 where the definition returns a Uint8. */
Sint32 fdMapGridIsClimbableMetatile(Sint32 x, Sint32 z, Sint32 layer);

/* fdActorCheckBlocked's bit for another solid actor overlapping the actor. */
#define BLOCKED_BY_ACTOR 2

/* Why the Sand ends (fdPsynergyWatchSandGround). */
typedef enum SAND_END {
	SAND_GOES_ON = 0,
	SAND_END_UPPER_LEVEL = 1, /* the leader is on a scene's upper level */
	SAND_END_OFF_SAND = 2,
	/* climbing as sand, it left the climbable metatiles */
	SAND_END_OFF_CLIMB = 3,
	/* climbing as sand, the metatile above is not one */
	SAND_END_FACING_WALL = 4
} SAND_END;

/* Event of the Sand: ends it (sound ABILITY_SAND | ACT_PACKED) when the leader,
   not blocked by another actor, stands off sand or on a scene's upper level
   (MainRoutineStateWork.onUpperLevel), or while it climbs as sand it leaves a
   climbable metatile or faces one that is not. */
void fdPsynergyWatchSandGround(void)
{
	FIELD_STATE		   *field = Ram_HeapSlots->fieldState;
	MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;
	ACTOR			   *actor = fdActorLookup(party->leaderActorId);
	Sint32				reason = SAND_GOES_ON;

	if(party->onUpperLevel != 0)
		reason = SAND_END_UPPER_LEVEL;
	if(fdMapGridIsSandMetatile(actor->x.fixed, actor->z.fixed, actor->layer) ==
		0)
		reason = SAND_END_OFF_SAND;
	if(party->movementMode == LEADER_MOVEMENT_SAND && actor->userValue != 0 &&
		actor->tmr == 0) {
		FIXED x;
		FIXED z;

		if(fdMapGridIsClimbableMetatile(actor->x.fixed, actor->z.fixed, 2) == 0)
			reason = SAND_END_OFF_CLIMB;
		x = (actor->x.fixed & FIELD_METATILE_MASK) + toFIXED(8);
		z = (actor->z.fixed & FIELD_METATILE_MASK) - toFIXED(8);
		if(fdMapGridIsClimbableMetatile(x, z, 2) == 0)
			reason = SAND_END_FACING_WALL;
	}
	if(reason != SAND_GOES_ON &&
		(fdActorCheckBlocked(actor) & BLOCKED_BY_ACTOR) == 0)
		field->psynergy = ABILITY_SAND | ACT_PACKED;
}

/* How a picture is stored. */
#define PICTURE_FORMAT_TAGGED_LZ 1 /* one tagged LZ stream */
#define PICTURE_FORMAT_COMPRESSED \
	3 /* compressed, then run through the decoder */

/* A sprite layer (SPR_ANIMATION) as fdPsynergyDecodePicture reads it: how its
   pictures are coded and the one shown. */
typedef struct PICTURE_SOURCE {
	Uint8		 unused[5];	 /* no code names it */
	Uint8		 decodeMode; /* its colorMode */
	Uint8		 pri;		 /* no code here names it */
	Uint8		 format;	 /* its mode */
	const void **pictures;
	Uint8		 unusedAfterPictures[10]; /* no code names it */
	Uint8		 picture;
} PICTURE_SOURCE;

typedef void (*PICTURE_DECODER)(
	const void *source, void *destination, Sint32 mode);

/* The ARM picture decoder fdPsynergyDecodePicture copies into RAM. */
extern const Uint8 BattleEffect_DecodePictureCode[];
/* The end of the part of the decoder that is copied. */
extern const Uint8 BattleEffect_DecodePictureCopyEnd[];
/* The decoder with its tables and its state (ld_script.ld). The block is larger
   than the copy, which stops before the state. */
extern Uint8 BattleEffect_DecodePictureBlockSize[];

/* Draws the current picture into a cleared buffer of the given size. The ARM
   picture decoder runs from RAM: when no copy is loaded yet, this loads one for
   the call and releases it afterwards. */
void fdPsynergyDecodePicture(PICTURE_SOURCE *src, void *buf, Sint32 size)
{
	Sint32			decoderLoaded = 1;
	PICTURE_DECODER decode = Ram_HeapSlots->frmScratch;

	if(decode == NULL) {
		syDMAXCopy(BattleEffect_DecodePictureCode,
			syHeapSlotAlloc(HEAP_SLOT_TICK_SCRATCH,
				(Sint32)BattleEffect_DecodePictureBlockSize),
			DMA_COPY_WORDS((Uint32)(BattleEffect_DecodePictureCopyEnd -
							   BattleEffect_DecodePictureCode) >>
				2),
			REG_DMA3);
		decode = Ram_HeapSlots->frmScratch;
		decoderLoaded = 0;
	}

	syWordsClearEntry(buf, size);
	if(src->format == PICTURE_FORMAT_TAGGED_LZ) {
		syDecompressTaggedLZ(src->pictures[src->picture], buf);
	} else if(src->format == PICTURE_FORMAT_COMPRESSED) {
		const void *stream =
			syDecompressSprPicture(src->pictures[src->picture], buf);

		if(stream != NULL)
			decode(stream, buf, 0);
	} else {
		decode(src->pictures[src->picture], buf, src->decodeMode);
	}

	if(!decoderLoaded)
		syHeapSlotRelease(HEAP_SLOT_TICK_SCRATCH);
}

#undef ACTOR_FIRST_LAYER
#undef PICTURE_FORMAT_TAGGED_LZ
#undef PICTURE_FORMAT_COMPRESSED

/* fdPictureShiftColumnsByWave as the Sand sinks the leader into the ground:
   each column's pixels below a wave line move down by phase (1.3 pixels a
   step), those above it stay. */
void fdPictureSinkColumnsByWave(Uint8 *src, Uint8 *dst, Sint32 phase)
{
	Sint32 width = SAND_PICTURE_SIZE;
	Sint32 height = SAND_PICTURE_SIZE;
	Sint32 column;
	Sint32 row;

	syWordsClear(dst, SAND_PICTURE_SIZE * SAND_PICTURE_SIZE);
	for(column = 0; column < width; column++) {
		Sint32 wave = sySin(column * (TRIG_TURN / 8)) * 8 + toFIXED(8);

		for(row = 0; row < height; row++) {
			Sint32 base = phase * (toFIXED(13) / 10);
			Sint32 rowOffset = row * toFIXED(4) - toFIXED(124);
			Sint32 shift = base + rowOffset - wave;

			shift = FIXEDtoINT(shift);
			if(shift < 0) {
				dst[row * width + column] = src[row * width + column];
			} else if(shift + row < height) {
				dst[((shift + row) << 5) + column] = src[row * width + column];
			}
		}
	}
}

/* Sets up the sprite the Sand draws the sinking leader as: the leader's depth,
   priority, scale (three quarters on the world map) and place. */
void fdSandInitLeaderSpr(EFFECT_SPR *spr, Sint32 tiles)
{
	FIELD_EFFECT_STATE *scene = Ram_HeapSlots->fieldEffState;
	FIELD_STATE		   *field = Ram_HeapSlots->fieldState;
	FIXED				pos[3];

	fdEffectSprInit(spr, 16, 30, OBJ_SIZE_32X32, tiles);
	spr->depth = fdActorGetSprDepth(scene->caster);
	spr->oam.pri = fdActorGetSprPriority(scene->caster);
	spr->oam.fullCol = 1;
	spr->oam.pal = 0;
	if(field->mode == ACTOR_SYSTEM_WORLD_MAP) {
		spr->scaleX = toFIXED(3) / 4;
		spr->scaleY = toFIXED(3) / 4;
	} else {
		spr->scaleX = scene->caster->scaleX;
		spr->scaleY = scene->caster->scaleY;
	}
	pos[0] = scene->caster->x.fixed;
	pos[1] = scene->caster->y.fixed;
	pos[2] = scene->caster->z.fixed;
	fdCameraWorldToScreen(pos);
	spr->x = pos[0];
	spr->y = pos[2];
}

/* Draws fdSandInitLeaderSpr's sprite. */
void fdSandDrawLeaderSpr(void *spr)
{
	fdEffectSprDraw(spr);
}

/* One grain of sand thrown up round the leader as it sinks: 4 pixels out, or,
   cast off sand (high), 2 pixels up and 2 out and drifting more slowly; drawn
   small just in front of the leader. */
void fdSandSpawnGrain(EFFECT_SPR *spr, FIELD_PSYNERGY_PARTICLE *particle,
	Sint32 unused, Sint32 tiles, Sint32 high)
{
	ACTOR *mainActor =
		((FIELD_EFFECT_STATE *)Ram_HeapSlots->fieldEffState)->caster;
	FIXED pos[3];
	FIXED y;

	pos[0] = mainActor->x.fixed;
	pos[1] = y = mainActor->y.fixed;
	pos[2] = mainActor->z.fixed;
	if(high) {
		pos[1] = y + toFIXED(2);
		syVectorAddPolarOffset(toFIXED(2), syRandom(), pos);
	} else {
		syVectorAddPolarOffset(toFIXED(4), syRandom(), pos);
	}
	SET_POSITION(particle, pos[0], pos[1], pos[2]);
	pos[2] = 0;
	pos[0] = 0;
	if(high)
		syVectorAddPolarOffset(toFIXED(0.2), syRandom(), pos);
	else
		syVectorAddPolarOffset(toFIXED(0.8), syRandom(), pos);
	particle->velX = pos[0];
	particle->velY = 0;
	particle->velZ = pos[2];
	fdEffectSprInit(spr, 8, 8, OBJ_SIZE_16X16, tiles);
	spr->scaleX = toFIXED(1) / 5;
	spr->scaleY = toFIXED(1) / 5;
	spr->oam.pri = fdActorGetSprPriority(mainActor);
	spr->depth = fdActorGetSprDepth(mainActor) + 2;
	spr->oam.fullCol = 1;
	spr->oam.pal = 0;
}

/* Advances one particle by a tick. For its first 32 ticks its sprite steps
   through eight pictures of eight tiles from tiles, four ticks each, grows by
   a tenth of its size a tick to full size, is placed on screen at the
   particle's position and drawn, and the particle moves on; after that only its
   age counts on. Reached from fdPsynergyEndSand and fdPsynergySand. */
void fdGrowingParticleUpdate(EFFECT_SPR *spr, FIELD_PSYNERGY_PARTICLE *particle,
	Sint32 unused, Sint32 tiles)
{
	Sint32 pos[3];
	Sint32 scale;
	Sint32 picture;

	if((Uint32)particle->tmr <= 31) {
		picture = (particle->tmr >> 2) & 7;
		spr->oam.idx = tiles + picture * 8;
		scale = spr->scaleX + toFIXED(1) / 10;
		spr->scaleX = scale;
		if(scale > toFIXED(1))
			scale = toFIXED(1);
		spr->scaleX = scale;
		spr->scaleY = scale;
		pos[0] = particle->x;
		pos[1] = particle->y;
		pos[2] = particle->z;
		fdCameraWorldToScreen(pos);
		spr->x = pos[0];
		spr->y = pos[2];
		fdEffectSprDraw(spr);
		fdAdvanceWithGravity2D(&particle->x, 60, 0);
	}
	particle->tmr++;
}

struct PENDING_CUE;

/* Gives an actor's first sprite layer another sprite's animations. */
void fdActorSetFirstSprAnimationStub(ACTOR *actor, Sint32 idx);
/* Sets a sprite's first layer to a SPR_COL_MODE and redraws it. */
void fdSprSetFirstAnimationColModeStub(void *anm, Sint32 value);

/* Ends Sand: the leader rises out of the ground in a spray of grains and takes
   its own sprite back; on sand, the place's Sand script then runs. */
void fdPsynergyEndSand(void)
{
	SAND_STATE *state =
		syHeapSlotAlloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(SAND_STATE));
	FIELD_EFFECT_STATE		*effect = Ram_HeapSlots->fieldEffState;
	FIELD_STATE				*field = Ram_HeapSlots->fieldState;
	Sint32					 pictureEntry;
	ACTOR					*owner = effect->caster;
	Sint32					 particleEntry;
	Sint32					 particleTiles;
	Sint32					 spr;
	FIELD_PSYNERGY_PARTICLE *particle;
	EFFECT_SPR				*particleSpr;
	Uint8					*anm;
	Sint32					 pictureOfs;
	Sint32					 frm;
	Sint32					 i;
	Sint32					 delay;
	Sint32					 mask;

	fdPsynergyMarkWorldMapTilesUsed();
	MainRoutineStateWork.sandActive = 0;
	fdActorSetOthersPaused(owner, 1, 0);
	syCloseEvent(fdPsynergyWatchSandGround);
	syDecompressTaggedLZ(syResourceGetTableEntry(RESOURCE_DUST_PUFF_A), state);
	particleEntry = syVRAMCacheFindFreeEntry();
	particleTiles = syVRAMCacheLoad(particleEntry, 2048, state);
	i = 0;
	particle = state->grains;
	delay = 0;
	particleSpr = state->grainSprs;
	for(; i < SAND_GRAIN_COUNT; i++) {
		fdSandSpawnGrain(
			particleSpr, particle, i, particleTiles, field->standing);
		particle->tmr = delay;
		particleSpr++;
		delay -= 2;
		particle++;
	}
	fdCameraMoveWithinMapBounds(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	owner->motionFlg = 0;
	mask = OAM_BELOW_SHAPE_OR_SIZE;
	fdActorSetPosAndResetMotionStub(
		owner, owner->x.fixed, owner->y.fixed + toFIXED(10), owner->z.fixed);
	fdActorSetFirstSprAnimationStub(owner, MainRoutineStateWork.leaderActorId);
	/* The image's OAM shape and size: square, 32 by 32 pixels. */
	((Uint8 *)owner->spr)[5] &= mask;
	mask &= ((Uint8 *)owner->spr)[7];
	mask |= OAM_SIZE_32X32_BYTE;
	((Uint8 *)owner->spr)[7] = mask;
	if(field->mode == ACTOR_SYSTEM_WORLD_MAP)
		((SPR *)owner->spr)->scale = toFIXED(3) / 4;
	owner->facing = DEGtoANG(90);
	fdActorSelectAnimationStub(owner, 0);
	fdSprSetFirstAnimationColModeStub(owner->spr, SPRITE_COLOR_HIDDEN);
	sySynch(1);
	fdSprSetFirstAnimationColModeStub(owner->spr, SPRITE_COLOR_NORMAL);
	fdPsynergyDecodePicture(((SPR *)owner->spr)->entries[0], state->picture,
		sizeof(state->picture));
	fdSprSetFirstAnimationColModeStub(owner->spr, SPRITE_COLOR_HIDDEN);
	pictureEntry = syVRAMCacheFindFreeEntry();
	spr = syVRAMCacheLoad(pictureEntry, sizeof(state->picture), NULL);
	fdSandInitLeaderSpr(&state->leader, spr);
	sySynch(1);

	frm = 0;
	for(; frm < 78; frm++) {
		if(frm == 0)
			sySndPlayStub(SE_SAND_END);
		if(frm <= 64) {
			fdPictureSinkColumnsByWave(
				state->picture, state->sunken, SAND_RISE_PHASE - frm * 3 / 2);
			pictureOfs = spr * TILE_SIZE_4BPP;
			((void (*)(void *, Sint32, Sint32,
				void *))Ram_HeapSlots->pictureUploader)(state->sunken,
				SAND_PICTURE_SIZE, SAND_PICTURE_SIZE,
				(Uint8 *)OBJ_VRAM0 + pictureOfs);
			fdSandDrawLeaderSpr(&state->leader);
		}
		if(frm == 66) {
			fdActorSelectAnimationStub(owner, 1);
			fdSprSetFirstAnimationColModeStub(owner->spr, SPRITE_COLOR_NORMAL);
			fdActorSetSprFlagsStub(
				fdActorGetById(effect->casterId), SPRITE_FLAG_SHADOW);
			fdCameraFollowActor(effect->casterId, 1);
			sySndPlayStub(SE_SAND_EMERGE);
			if(field->mode == ACTOR_SYSTEM_WORLD_MAP) {
				MainRoutineStateWork.movementMode = LEADER_MOVEMENT_WORLD_MAP;
				owner->motionFlg = ACTOR_FALLS;
			} else {
				MainRoutineStateWork.movementMode = LEADER_MOVEMENT_WALK;
				owner->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
			}
			if(field->standing == 0)
				fdRunPsynergyScript(
					fdFindPsynergyCue(PSYNERGY_CUE(PSYNERGY_PHASE_FADES),
						effect->fieldPsynergy,
						fdSelectPsynergyTarget(effect->fieldPsynergy)),
					effect->casterId, -1);
		}
		i = 0;
		particle = state->grains;
		particleSpr = state->grainSprs;
		for(; i < SAND_RISE_GRAINS; i++) {
			fdGrowingParticleUpdate(particleSpr, particle, i, particleTiles);
			particleSpr++;
			particle++;
		}
		sySynch(1);
	}
	fdActorSetOthersPaused(owner, 0, ANIM_STEP_NORMAL);
	syVRAMCacheFreeEntry(pictureEntry);
	syVRAMCacheFreeEntry(particleEntry);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
}

/* Sand cast on a target: runs fdPsynergySand. */
void fdPsynergySandOnTarget(void)
{
	fdPsynergySand();
}

/* Sand: the leader spins and sinks into the ground in a spray of grains, then
   moves on as a mound of sand (LEADER_MOVEMENT_SAND, or
   LEADER_MOVEMENT_WORLD_SAND on the World Map) until fdPsynergyWatchSandGround
   ends it. Off sand it rises again at once; a leader whose sprite is not 32 by
   32 pixels does nothing. */
void fdPsynergySand(void)
{
	SAND_STATE *state =
		syHeapSlotAlloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(SAND_STATE));
	FIELD_EFFECT_STATE		*effect = Ram_HeapSlots->fieldEffState;
	FIELD_STATE				*field = Ram_HeapSlots->fieldState;
	ACTOR					*owner;
	Sint32					 pictureEntry;
	Sint32					 particleEntry;
	Sint32					 particleTiles;
	FIXED					 savedY;
	Uint8					 savedFlg;
	Uint8					*motionFlg;
	Sint32					 pictureTiles;
	EFFECT_SPR				*spr;
	FIELD_PSYNERGY_PARTICLE *particle;
	Sint32					 frm;
	Sint32					 delay;
	Sint32					 i;

	owner = effect->caster;
	savedY = owner->y.fixed;
	motionFlg = &owner->motionFlg;
	savedFlg = *motionFlg;
	if(((SPR *)owner->spr)->width == SAND_PICTURE_SIZE &&
		((SPR *)owner->spr)->height == SAND_PICTURE_SIZE) {
		fdPsynergyMarkWorldMapTilesUsed();
		fdActorSetOthersPaused(owner, 1, 0);
		if(fdMapGridIsSandMetatile(
			   owner->x.fixed, owner->z.fixed, owner->layer) == 0)
			field->standing = 1;
		else
			field->standing = 0;
		fdPsynergyFadeToCastCol();
		syDecompressTaggedLZ(
			syResourceGetTableEntry(RESOURCE_DUST_PUFF_A), state);
		particleEntry = syVRAMCacheFindFreeEntry();
		particleTiles = syVRAMCacheLoad(particleEntry, 2048, state);
		i = 0;
		particle = state->grains;
		delay = -12;
		spr = state->grainSprs;
		for(; i < SAND_GRAIN_COUNT; i++) {
			fdSandSpawnGrain(spr, particle, i, particleTiles, field->standing);
			particle->tmr = delay;
			spr++;
			delay -= 2;
			particle++;
		}
		fdCameraMoveWithinMapBounds(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
		fdBattleEffectSetObjectUpdateMode(effect->caster, CASTER_LOOK_NORMAL);
		fdActorSetPosAndResetMotionStub(
			owner, owner->x.fixed, owner->y.fixed, owner->z.fixed);
		*motionFlg = 0;
		sySynch(2);
		for(frm = 0; frm < 40; frm++) {
			if(frm > 0)
				owner->y.fixed += toFIXED(9) / 40;
			if(frm & 31)
				sySndPlayStub(SE_SPIRAL);
			owner->facing += DEGtoANG(45);
			sySynch(1);
		}
		owner->facing = DEGtoANG(90);
		sySynch(1);
		fdPsynergyDecodePicture(
			(PICTURE_SOURCE *)((SPR *)owner->spr)->entries[0], state->picture,
			sizeof(state->picture));
		pictureEntry = syVRAMCacheFindFreeEntry();
		pictureTiles =
			syVRAMCacheLoad(pictureEntry, sizeof(state->picture), NULL);
		fdSandInitLeaderSpr(&state->leader, pictureTiles);
		frm = 15;
		for(; frm <= 92; frm++) {
			fdPictureSinkColumnsByWave(
				state->picture, state->sunken, frm * 3 / 2);
			((void (*)(void *, Sint32, Sint32,
				void *))Ram_HeapSlots->pictureUploader)(state->sunken,
				SAND_PICTURE_SIZE, SAND_PICTURE_SIZE,
				(Uint8 *)OBJ_VRAM0 + pictureTiles * TILE_SIZE_4BPP);
			fdSandDrawLeaderSpr(&state->leader);
			i = 0;
			particle = state->grains;
			spr = state->grainSprs;
			for(; i < SAND_SINK_GRAINS; i++) {
				fdGrowingParticleUpdate(spr, particle, i, particleTiles);
				spr++;
				particle++;
			}
			if(frm == 16)
				fdSprSetFirstAnimationColModeStub(
					owner->spr, SPRITE_COLOR_HIDDEN);
			if(frm == 45)
				sySndPlayStub(SE_WHOOSH);
			if(frm == 60) {
				fdActorSetFirstSprAnimationStub(owner, SPRITE_SAND_RIPPLE);
				/* The image's OAM shape and size: 16 by 16 pixels. */
				((FIELD_SPR *)owner->spr)->shape = OBJ_SHAPE_SQUARE;
				((FIELD_SPR *)owner->spr)->size = 1;
				fdActorSelectAnimationStub(owner, 1);
				fdActorSetSprFlagsStub(fdActorGetById(effect->casterId), 0);
				fdCameraFollowActor(effect->casterId, 1);
				owner->y.fixed = savedY;
				*motionFlg = savedFlg;
			}
			if(frm == 61)
				fdSprSetFirstAnimationColModeStub(
					owner->spr, SPRITE_COLOR_NORMAL);
			sySynch(1);
		}
		if(field->mode == ACTOR_SYSTEM_WORLD_MAP)
			MainRoutineStateWork.movementMode = LEADER_MOVEMENT_WORLD_SAND;
		else
			MainRoutineStateWork.movementMode = LEADER_MOVEMENT_SAND;
		owner->tmr = 0;
		owner->userValue = 0;
		field->sandTrailPri = 3;
		MainRoutineStateWork.sandActive = 1;
		if(field->standing == 0)
			sySetEvent(fdPsynergyWatchSandGround, EVENT_PASS_TICK);
		fdActorSetOthersPaused(owner, 0, ANIM_STEP_NORMAL);
		syVRAMCacheFreeEntry(pictureEntry);
		syVRAMCacheFreeEntry(particleEntry);
		fdPsynergyFadeFromCastCol();
	}
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
	if(field->standing != 0)
		fdPsynergyEndSand();
}

#undef SAND_GRAIN_COUNT
#undef SAND_SINK_GRAINS
#undef SAND_RISE_GRAINS
#undef SAND_PICTURE_SIZE
#undef SAND_RISE_PHASE
#undef OAM_BELOW_SHAPE_OR_SIZE
#undef OAM_SIZE_32X32_BYTE
#undef BLOCKED_BY_ACTOR
