/* @file */
/* Sand: the leader sinking into the ground and moving as sand, the grains it */
/* throws up, the task that ends it off sand, and the dust puff it ends with. */
/* Part of field_psynergy.c. */

/* The grains of sand thrown up round the leader, and how many of them are */
/* drawn as it sinks and as it rises. */
#define SAND_GRAIN_COUNT 64
#define SAND_SINK_GRAINS 33
#define SAND_RISE_GRAINS 30
/* The leader's picture, 32 pixels square; Sand takes only a leader drawn so. */
#define SAND_PICTURE_SIZE 32
/* The phase the leader's sunk picture rises from, 1.5 less each frame. */
#define SAND_RISE_PHASE 92
/* An OAM attribute's high byte: the bits below its top two, the shape in */
/* attribute 0 and the size in attribute 1, and the size bits of a sprite 32 */
/* pixels square. */
#define OAM_BELOW_SHAPE_OR_SIZE 63
#define OAM_SIZE_32X32_BYTE		(OBJ_SIZE_32X32 >> 24)

/* Sand's state (heap slot HEAP_SLOT_BATTLE_EFFECT): the leader's picture and */
/* the copy it sinks into the ground, the grains of sand thrown up round it and
 */
/* the sprite the sinking leader is drawn with. */
typedef struct SAND_STATE {
	/* The leader's picture, 8 bits a pixel. */
	u8 picture[SAND_PICTURE_SIZE * SAND_PICTURE_SIZE];
	/* The picture with its columns sunk by a wave. */
	u8 sunken[SAND_PICTURE_SIZE * SAND_PICTURE_SIZE];
	EFFECT_SPR grainSprs[SAND_GRAIN_COUNT];
	FIELD_PSYNERGY_PARTICLE grains[SAND_GRAIN_COUNT];
	EFFECT_SPR leader;
	u8 unused[4];
} SAND_STATE;

/* The ROM's callers here are compiled against this form, which returns an */
/* s32 where the definition returns a u8. */
s32 fdMapGridIsClimbableCell(s32 x, s32 z, s32 layer);

/* fdActorCheckBlocked's bit for another solid actor overlapping the actor. */
#define BLOCKED_BY_ACTOR 2

/* Why the Sand ends (fdPsynergyWatchSandGround). */
typedef enum SAND_END {
	SAND_GOES_ON = 0,
	SAND_END_UPPER_LEVEL = 1, /* the leader is on a scene's upper level */
	SAND_END_OFF_SAND = 2,
	SAND_END_OFF_CLIMB = 3,	 /* climbing as sand, it left the climbable cells */
	SAND_END_FACING_WALL = 4 /* climbing as sand, the cell above is not one */
} SAND_END;

/* Task of the Sand: ends it (cue ABILITY_SAND | ACT_PACKED) when the leader, */
/* not blocked by another actor, stands off sand or on a scene's upper level */
/* (MainRoutineStateWork.onUpperLevel), or while it climbs as sand it leaves a
 */
/* climbable cell or faces one that is not. */
void fdPsynergyWatchSandGround(void) {
	FIELD_STATE *field = Ram_HeapSlots->fieldState;
	MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;
	ACTOR *actor = fdActorLookup(party->leaderActorId);
	s32 reason = SAND_GOES_ON;

	if(party->onUpperLevel != 0)
		reason = SAND_END_UPPER_LEVEL;
	if(fdMapGridIsSandCell(actor->x.fixed, actor->z.fixed, actor->layer) == 0)
		reason = SAND_END_OFF_SAND;
	if(party->movementMode == LEADER_MOVEMENT_SAND && actor->userValue != 0 &&
	   actor->tmr == 0) {
		FIXED x;
		FIXED z;

		if(fdMapGridIsClimbableCell(actor->x.fixed, actor->z.fixed, 2) == 0)
			reason = SAND_END_OFF_CLIMB;
		x = (actor->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
		z = (actor->z.fixed & FIELD_CELL_MASK) - toFIXED(8);
		if(fdMapGridIsClimbableCell(x, z, 2) == 0)
			reason = SAND_END_FACING_WALL;
	}
	if(reason != SAND_GOES_ON &&
	   (fdActorCheckBlocked(actor) & BLOCKED_BY_ACTOR) == 0)
		field->psynergy = ABILITY_SAND | ACT_PACKED;
}

/* How a picture is stored. */
#define PICTURE_FORMAT_TAGGED_LZ 1 /* one tagged LZ stream */
#define PICTURE_FORMAT_COMPRESSED                                              \
	3 /* compressed, then run through the decoder */

/* A sprite layer (SPR_ANIMATION) as fdPsynergyDecodePicture */
/* reads it: how its pictures are coded and the one shown. */
typedef struct PICTURE_SOURCE {
	u8 unused[5];  /* no code names it */
	u8 decodeMode; /* its colorMode */
	u8 pri;		   /* no code here names it */
	u8 format;	   /* its mode */
	const void **pictures;
	u8 unusedAfterPictures[10]; /* no code names it */
	u8 picture;
} PICTURE_SOURCE;

typedef void (*PICTURE_DECODER)(const void *source, void *destination,
								s32 mode);

/* The ARM picture decoder fdPsynergyDecodePicture copies into RAM. */
extern const u8 BattleEffect_DecodePictureCode[];
/* The end of the part of the decoder that is copied. */
extern const u8 BattleEffect_DecodePictureCopyEnd[];
/* The decoder with its tables and its state (ld_script.ld). The block is */
/* larger than the copy, which stops before the state. */
extern u8 BattleEffect_DecodePictureBlockSize[];

/* Draws the current picture into a cleared buffer of the given */
/* size. The ARM picture decoder runs from RAM: when no copy is loaded yet, */
/* this loads one for the call and releases it afterwards. */
void fdPsynergyDecodePicture(PICTURE_SOURCE *source, void *buffer, s32 size) {
	s32 decoderLoaded = 1;
	PICTURE_DECODER decode = Ram_HeapSlots->frmScratch;

	if(decode == NULL) {
		syDMAXCopy(BattleEffect_DecodePictureCode,
				   syHeapSlotAlloc(HEAP_SLOT_FRAME_SCRATCH,
								   (s32)BattleEffect_DecodePictureBlockSize),
				   DMA_COPY_WORDS((u32)(BattleEffect_DecodePictureCopyEnd -
										BattleEffect_DecodePictureCode) >>
								  2),
				   REG_DMA3);
		decode = Ram_HeapSlots->frmScratch;
		decoderLoaded = 0;
	}

	syWordsClearEntry(buffer, size);
	if(source->format == PICTURE_FORMAT_TAGGED_LZ) {
		syDecompressTaggedLz(source->pictures[source->picture], buffer);
	}
	else if(source->format == PICTURE_FORMAT_COMPRESSED) {
		const void *stream =
			syDecompressSprPicture(source->pictures[source->picture], buffer);

		if(stream != NULL)
			decode(stream, buffer, 0);
	}
	else {
		decode(source->pictures[source->picture], buffer, source->decodeMode);
	}

	if(!decoderLoaded)
		syHeapSlotRelease(HEAP_SLOT_FRAME_SCRATCH);
}
#undef ACTOR_FIRST_LAYER
#undef PICTURE_FORMAT_TAGGED_LZ
#undef PICTURE_FORMAT_COMPRESSED

/* fdPictureShiftColumnsByWave as the Sand sinks the leader into the ground: */
/* each column's pixels below a wave line move down by phase (1.3 pixels a */
/* step), those above it stay. */
void fdPictureSinkColumnsByWave(u8 *source, u8 *destination, s32 phase) {
	s32 width = SAND_PICTURE_SIZE;
	s32 height = SAND_PICTURE_SIZE;
	s32 column;
	s32 row;

	syWordsClear(destination, SAND_PICTURE_SIZE * SAND_PICTURE_SIZE);
	for(column = 0; column < width; column++) {
		s32 wave = sySin(column * (TRIG_TURN / 8)) * 8 + toFIXED(8);

		for(row = 0; row < height; row++) {
			s32 base = phase * (toFIXED(13) / 10);
			s32 rowOffset = row * toFIXED(4) - toFIXED(124);
			s32 shift = base + rowOffset - wave;

			shift = FIXEDtoINT(shift);
			if(shift < 0) {
				destination[row * width + column] =
					source[row * width + column];
			}
			else if(shift + row < height) {
				destination[((shift + row) << 5) + column] =
					source[row * width + column];
			}
		}
	}
}

/* Sets up the sprite the Sand draws the sinking leader as: the leader's */
/* depth, priority, scale (three quarters on the world map) and place. */
void fdSandInitLeaderSpr(EFFECT_SPR *sprite, s32 tiles) {
	FIELD_EFFECT_STATE *scene = Ram_HeapSlots->fieldEffState;
	FIELD_STATE *field = Ram_HeapSlots->fieldState;
	FIXED position[3];

	fdEffectSprInit(sprite, 16, 30, OBJ_SIZE_32X32, tiles);
	sprite->depth = fdActorGetSprDepth(scene->caster);
	sprite->oam.pri = fdActorGetSprPriority(scene->caster);
	sprite->oam.fullCol = 1;
	sprite->oam.pal = 0;
	if(field->mode == ACTOR_SYSTEM_WORLD_MAP) {
		sprite->scaleX = FIXED_ONE * 3 / 4;
		sprite->scaleY = FIXED_ONE * 3 / 4;
	}
	else {
		sprite->scaleX = scene->caster->scaleX;
		sprite->scaleY = scene->caster->scaleY;
	}
	position[0] = scene->caster->x.fixed;
	position[1] = scene->caster->y.fixed;
	position[2] = scene->caster->z.fixed;
	fdCameraWorldToScr(position);
	sprite->x = position[0];
	sprite->y = position[2];
}

/* Draws fdSandInitLeaderSpr's sprite. */
void fdSandDrawLeaderSpr(void *sprite) { fdEffectSprDraw(sprite); }

/* One grain of sand thrown up round the leader as it sinks: 4 pixels out, */
/* or, cast off sand (high), 2 pixels up and 2 out and drifting more slowly; */
/* drawn small just in front of the leader. */
void fdSandSpawnGrain(EFFECT_SPR *sprite, FIELD_PSYNERGY_PARTICLE *particle,
					  s32 unused, s32 tiles, s32 high) {
	ACTOR *mainActor =
		((FIELD_EFFECT_STATE *)Ram_HeapSlots->fieldEffState)->caster;
	FIXED position[3];
	FIXED y;

	position[0] = mainActor->x.fixed;
	position[1] = y = mainActor->y.fixed;
	position[2] = mainActor->z.fixed;
	if(high) {
		position[1] = y + toFIXED(2);
		syVectorAddPolarOffset(toFIXED(2), syRandom(), position);
	}
	else {
		syVectorAddPolarOffset(toFIXED(4), syRandom(), position);
	}
	SET_POSITION(particle, position[0], position[1], position[2]);
	position[2] = 0;
	position[0] = 0;
	if(high)
		syVectorAddPolarOffset(toFIXED(0.2), syRandom(), position);
	else
		syVectorAddPolarOffset(toFIXED(0.8), syRandom(), position);
	particle->velX = position[0];
	particle->velY = 0;
	particle->velZ = position[2];
	fdEffectSprInit(sprite, 8, 8, OBJ_SIZE_16X16, tiles);
	sprite->scaleX = FIXED_ONE / 5;
	sprite->scaleY = FIXED_ONE / 5;
	sprite->oam.pri = fdActorGetSprPriority(mainActor);
	sprite->depth = fdActorGetSprDepth(mainActor) + 2;
	sprite->oam.fullCol = 1;
	sprite->oam.pal = 0;
}

/* Advances one particle by a frame. For its first 32 frames its sprite */
/* steps through eight pictures of eight tiles from tiles, four frames each, */
/* grows by a tenth of its size a frame to full size, is placed on screen at */
/* the particle's position and drawn, and the particle moves on; after that */
/* only its age counts on. Reached from fdPsynergyEndSand and */
/* fdPsynergySand. */
void fdGrowingParticleUpdate(EFFECT_SPR *sprite,
							 FIELD_PSYNERGY_PARTICLE *particle, s32 unused,
							 s32 tiles) {
	s32 position[3];
	s32 scale;
	s32 picture;

	if((u32)particle->tmr <= 31) {
		picture = (particle->tmr >> 2) & 7;
		sprite->oam.idx = tiles + picture * 8;
		scale = sprite->scaleX + FIXED_ONE / 10;
		sprite->scaleX = scale;
		if(scale > FIXED_ONE)
			scale = FIXED_ONE;
		sprite->scaleX = scale;
		sprite->scaleY = scale;
		position[0] = particle->x;
		position[1] = particle->y;
		position[2] = particle->z;
		fdCameraWorldToScr(position);
		sprite->x = position[0];
		sprite->y = position[2];
		fdEffectSprDraw(sprite);
		fdAdvanceWithGravity2D(&particle->x, 60, 0);
	}
	particle->tmr++;
}

struct PENDING_EVENT;

/* Gives an actor's first sprite layer another sprite's animations. */
void fdActorSetFirstSprAnimationStub(ACTOR *actor, s32 index);
/* Sets a sprite's first layer to a SPR_COL_MODE and redraws it. */
void fdSprSetFirstAnimationColModeStub(void *animation, s32 value);

/* Ends Sand: the leader rises out of the ground in a spray of grains and takes
 */
/* its own sprite back; on sand, the place's Sand event then runs. */
void fdPsynergyEndSand(void) {
	SAND_STATE *state =
		syHeapSlotAlloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(SAND_STATE));
	FIELD_EFFECT_STATE *effect = Ram_HeapSlots->fieldEffState;
	FIELD_STATE *field = Ram_HeapSlots->fieldState;
	s32 pictureEntry;
	ACTOR *owner = effect->caster;
	s32 particleEntry;
	s32 particleTiles;
	s32 sprite;
	FIELD_PSYNERGY_PARTICLE *particle;
	EFFECT_SPR *particleSprite;
	u8 *animation;
	s32 pictureOffset;
	s32 frame;
	s32 i;
	s32 delay;
	s32 mask;

	fdPsynergyMarkWorldMapTilesUsed();
	MainRoutineStateWork.sandActive = 0;
	fdActorSetOthersPaused(owner, 1, 0);
	syCloseEvent(fdPsynergyWatchSandGround);
	syDecompressTaggedLz(syResourceGetTableEntry(RESOURCE_DUST_PUFF_A), state);
	particleEntry = syVramCacheFindFreeEntry();
	particleTiles = syVramCacheLoad(particleEntry, 2048, state);
	i = 0;
	particle = state->grains;
	delay = 0;
	particleSprite = state->grainSprs;
	for(; i < SAND_GRAIN_COUNT; i++) {
		fdSandSpawnGrain(particleSprite, particle, i, particleTiles,
						 field->standing);
		particle->tmr = delay;
		particleSprite++;
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
	((u8 *)owner->spr)[5] &= mask;
	mask &= ((u8 *)owner->spr)[7];
	mask |= OAM_SIZE_32X32_BYTE;
	((u8 *)owner->spr)[7] = mask;
	if(field->mode == ACTOR_SYSTEM_WORLD_MAP)
		((SPR *)owner->spr)->scale = FIXED_ONE * 3 / 4;
	owner->facing = DEGtoANG(90);
	fdActorSelectAnimationStub(owner, 0);
	fdSprSetFirstAnimationColModeStub(owner->spr, SPRITE_COLOR_HIDDEN);
	sySynch(1);
	fdSprSetFirstAnimationColModeStub(owner->spr, SPRITE_COLOR_NORMAL);
	fdPsynergyDecodePicture(((SPR *)owner->spr)->entries[0], state->picture,
							sizeof(state->picture));
	fdSprSetFirstAnimationColModeStub(owner->spr, SPRITE_COLOR_HIDDEN);
	pictureEntry = syVramCacheFindFreeEntry();
	sprite = syVramCacheLoad(pictureEntry, sizeof(state->picture), NULL);
	fdSandInitLeaderSpr(&state->leader, sprite);
	sySynch(1);

	frame = 0;
	for(; frame < 78; frame++) {
		if(frame == 0)
			sySndPlayStub(SE_SAND_END);
		if(frame <= 64) {
			fdPictureSinkColumnsByWave(state->picture, state->sunken,
									   SAND_RISE_PHASE - frame * 3 / 2);
			pictureOffset = sprite * TILE_SIZE_4BPP;
			((void (*)(void *, s32, s32,
					   void *))Ram_HeapSlots->pictureUploader)(
				state->sunken, SAND_PICTURE_SIZE, SAND_PICTURE_SIZE,
				(u8 *)OBJ_VRAM0 + pictureOffset);
			fdSandDrawLeaderSpr(&state->leader);
		}
		if(frame == 66) {
			fdActorSelectAnimationStub(owner, 1);
			fdSprSetFirstAnimationColModeStub(owner->spr, SPRITE_COLOR_NORMAL);
			fdActorSetSprFlagsStub(fdActorGetById(effect->casterId),
								   SPRITE_FLAG_SHADOW);
			fdCameraFollowActor(effect->casterId, 1);
			sySndPlayStub(SE_SAND_EMERGE);
			if(field->mode == ACTOR_SYSTEM_WORLD_MAP) {
				MainRoutineStateWork.movementMode = LEADER_MOVEMENT_WORLD_MAP;
				owner->motionFlg = ACTOR_FALLS;
			}
			else {
				MainRoutineStateWork.movementMode = LEADER_MOVEMENT_WALK;
				owner->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
			}
			if(field->standing == 0)
				fdRunPsynergyEvent(
					scFindPsynergyTrigger(
						PSYNERGY_TRIGGER(PSYNERGY_PHASE_FADES),
						effect->fieldPsynergy,
						scSelectPsynergyTarget(effect->fieldPsynergy)),
					effect->casterId, -1);
		}
		i = 0;
		particle = state->grains;
		particleSprite = state->grainSprs;
		for(; i < SAND_RISE_GRAINS; i++) {
			fdGrowingParticleUpdate(particleSprite, particle, i, particleTiles);
			particleSprite++;
			particle++;
		}
		sySynch(1);
	}
	fdActorSetOthersPaused(owner, 0, ANIM_STEP_NORMAL);
	syVramCacheFreeEntry(pictureEntry);
	syVramCacheFreeEntry(particleEntry);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
}

/* Sand cast on a target: runs fdPsynergySand. */
void fdPsynergySandOnTarget(void) { fdPsynergySand(); }

/* Sand: the leader spins and sinks into the ground in a spray of grains, then
 */
/* moves on as a mound of sand (LEADER_MOVEMENT_SAND, or */
/* LEADER_MOVEMENT_WORLD_SAND on the World Map) until */
/* fdPsynergyWatchSandGround ends it. Off sand it rises again at once; a */
/* leader whose sprite is not 32 by 32 pixels does nothing. */
void fdPsynergySand(void) {
	SAND_STATE *state =
		syHeapSlotAlloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(SAND_STATE));
	FIELD_EFFECT_STATE *effect = Ram_HeapSlots->fieldEffState;
	FIELD_STATE *field = Ram_HeapSlots->fieldState;
	ACTOR *owner;
	s32 pictureEntry;
	s32 particleEntry;
	s32 particleTiles;
	FIXED savedY;
	u8 savedFlags;
	u8 *motionFlags;
	s32 pictureTiles;
	EFFECT_SPR *sprite;
	FIELD_PSYNERGY_PARTICLE *particle;
	s32 frame;
	s32 delay;
	s32 i;

	owner = effect->caster;
	savedY = owner->y.fixed;
	motionFlags = &owner->motionFlg;
	savedFlags = *motionFlags;
	if(((SPR *)owner->spr)->width == SAND_PICTURE_SIZE &&
	   ((SPR *)owner->spr)->height == SAND_PICTURE_SIZE) {
		fdPsynergyMarkWorldMapTilesUsed();
		fdActorSetOthersPaused(owner, 1, 0);
		if(fdMapGridIsSandCell(owner->x.fixed, owner->z.fixed, owner->layer) ==
		   0)
			field->standing = 1;
		else
			field->standing = 0;
		fdPsynergyFadeToCastCol();
		syDecompressTaggedLz(syResourceGetTableEntry(RESOURCE_DUST_PUFF_A),
							 state);
		particleEntry = syVramCacheFindFreeEntry();
		particleTiles = syVramCacheLoad(particleEntry, 2048, state);
		i = 0;
		particle = state->grains;
		delay = -12;
		sprite = state->grainSprs;
		for(; i < SAND_GRAIN_COUNT; i++) {
			fdSandSpawnGrain(sprite, particle, i, particleTiles,
							 field->standing);
			particle->tmr = delay;
			sprite++;
			delay -= 2;
			particle++;
		}
		fdCameraMoveWithinMapBounds(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
		fdBattleEffectSetObjectUpdateMode(effect->caster, CASTER_LOOK_NORMAL);
		fdActorSetPosAndResetMotionStub(owner, owner->x.fixed, owner->y.fixed,
										owner->z.fixed);
		*motionFlags = 0;
		sySynch(2);
		for(frame = 0; frame < 40; frame++) {
			if(frame > 0)
				owner->y.fixed += toFIXED(9) / 40;
			if(frame & 31)
				sySndPlayStub(SE_SPIRAL);
			owner->facing += DEGtoANG(45);
			sySynch(1);
		}
		owner->facing = DEGtoANG(90);
		sySynch(1);
		fdPsynergyDecodePicture(
			(PICTURE_SOURCE *)((SPR *)owner->spr)->entries[0], state->picture,
			sizeof(state->picture));
		pictureEntry = syVramCacheFindFreeEntry();
		pictureTiles =
			syVramCacheLoad(pictureEntry, sizeof(state->picture), NULL);
		fdSandInitLeaderSpr(&state->leader, pictureTiles);
		frame = 15;
		for(; frame <= 92; frame++) {
			fdPictureSinkColumnsByWave(state->picture, state->sunken,
									   frame * 3 / 2);
			((void (*)(void *, s32, s32,
					   void *))Ram_HeapSlots->pictureUploader)(
				state->sunken, SAND_PICTURE_SIZE, SAND_PICTURE_SIZE,
				(u8 *)OBJ_VRAM0 + pictureTiles * TILE_SIZE_4BPP);
			fdSandDrawLeaderSpr(&state->leader);
			i = 0;
			particle = state->grains;
			sprite = state->grainSprs;
			for(; i < SAND_SINK_GRAINS; i++) {
				fdGrowingParticleUpdate(sprite, particle, i, particleTiles);
				sprite++;
				particle++;
			}
			if(frame == 16)
				fdSprSetFirstAnimationColModeStub(owner->spr,
												  SPRITE_COLOR_HIDDEN);
			if(frame == 45)
				sySndPlayStub(SE_WHOOSH);
			if(frame == 60) {
				fdActorSetFirstSprAnimationStub(owner, SPRITE_SAND_RIPPLE);
				/* The image's OAM shape and size: 16 by 16 pixels. */
				((FIELD_SPR *)owner->spr)->shape = OBJ_SHAPE_SQUARE;
				((FIELD_SPR *)owner->spr)->size = 1;
				fdActorSelectAnimationStub(owner, 1);
				fdActorSetSprFlagsStub(fdActorGetById(effect->casterId), 0);
				fdCameraFollowActor(effect->casterId, 1);
				owner->y.fixed = savedY;
				*motionFlags = savedFlags;
			}
			if(frame == 61)
				fdSprSetFirstAnimationColModeStub(owner->spr,
												  SPRITE_COLOR_NORMAL);
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
			sySetEvent(fdPsynergyWatchSandGround, TASK_PASS_FRAME);
		fdActorSetOthersPaused(owner, 0, ANIM_STEP_NORMAL);
		syVramCacheFreeEntry(pictureEntry);
		syVramCacheFreeEntry(particleEntry);
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
