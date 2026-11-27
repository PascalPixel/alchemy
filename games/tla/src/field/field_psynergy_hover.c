/* @file */
/* Hover: its glow, the cast and its end. */
/* Part of field_psynergy.c. */

/* The levels in HoverGlow_Levels's cycle, and the steps of the glow's */
/* colours. */
#define HOVER_GLOW_WAVE_LENGTH 96
#define HOVER_GLOW_STEPS	   24
/* The brightness level at which fdHoverGlowGetCol leaves the wave as it is. */
#define HOVER_GLOW_NEUTRAL_LEVEL 7

/* A cycle of 96 colour levels (31 down to 0, 32 at 0, then 0 up to 31), */
/* read a third of a turn apart for red, green and blue. */
extern const u8 HoverGlow_Levels[];

/* The colour of Hover's glow (fdHoverGlowUpdate) for a step of */
/* its 24-step cycle and a brightness level (HOVER_GLOW_NEUTRAL_LEVEL leaves */
/* it as it is): three channels of */
/* the wave, each raised or lowered by the level and held to 0..31, packed as */
/* RGB555. */
s32 fdHoverGlowGetCol(s32 counter, s32 level) {
	s16 phase;
	s16 bias;
	s16 red;
	s16 green;
	s16 blue;
	s16 redLevel;
	s32 step;
	const u8 *wave;

	step = (counter + HOVER_GLOW_STEPS / 2) % HOVER_GLOW_STEPS;
	wave = HoverGlow_Levels;
	phase = step * (HOVER_GLOW_WAVE_LENGTH / HOVER_GLOW_STEPS);
	redLevel = wave[(s16)(phase % HOVER_GLOW_WAVE_LENGTH)];
	bias = level - HOVER_GLOW_NEUTRAL_LEVEL;
	red = redLevel + bias;
	green =
		wave[(phase + HOVER_GLOW_WAVE_LENGTH / 3) % HOVER_GLOW_WAVE_LENGTH] +
		bias;
	blue = wave[(phase + HOVER_GLOW_WAVE_LENGTH * 2 / 3) %
				HOVER_GLOW_WAVE_LENGTH] +
		   bias;
	if(red < 0)
		red = 0;
	if(red > RGB_LEVEL_MAX)
		red = RGB_LEVEL_MAX;
	if(green < 0)
		green = 0;
	if(green > RGB_LEVEL_MAX)
		green = RGB_LEVEL_MAX;
	if(blue < 0)
		blue = 0;
	if(blue > RGB_LEVEL_MAX)
		blue = RGB_LEVEL_MAX;
	return RGB(red, green, blue);
}

/* The glow's colours: sprite palette CANVAS_PALETTE. */
#define HOVER_GLOW_PALETTE ((u16 *)&OBJ_PLTT_COLOR(CANVAS_PALETTE, 0))

/* The glow's strength, in frames: full at 112, and it fades out a frame at a */
/* time once the player leaves the place that feeds it. */
#define GLOW_FULL 112
/* The glow's strength set as the ring grows, before the glow is held. */
#define GLOW_RISING 100
/* The field Psynergy the glow asks for when it runs out: Hover, without */
/* paying PP. */
#define GLOW_SPENT_CUE (ABILITY_HOVER | ACT_PACKED)
/* The frames the ring under the leader takes to grow, or to shrink away. */
#define HOVER_RING_GROW_FRAMES 32

/* Hover's state (heap slot HEAP_SLOT_BATTLE_EFFECT): the glowing ring under */
/* the hovering leader, its scale, tile and stage. */
typedef struct HOVER_STATE {
	u8 tiles[4096]; /* the ring's picture, decoded to be loaded */
	EFFECT_SPR spr;
	s32 scale;
	s16 resource;
	u16 tile;
	u16 unused; /* cleared as Hover starts; nothing reads it */
	u16 phase;
	s16 fading;
} HOVER_STATE;

s32 fdMapGridIsHoverCell(s32 x, s32 z, s32 layer);

/* Hover's frame callback: keeps the glow's strength full while the player */
/* stands where fdMapGridIsHoverCell allows, and runs it down otherwise (once */
/* it is fading, the glow shrinks with it and the event takes its cue when it */
/* is spent). It cycles HOVER_GLOW_PALETTE through fdHoverGlowGetCol's colours,
 */
/* flickers between two pictures every other frame, and draws the glow under */
/* the player, flattened to three quarters of its height. */
void fdHoverGlowUpdate(void) {
	HOVER_STATE *state = Ram_HeapSlots->battleEff;
	ACTOR *player = fdActorLookup(fdPartyGetLeaderActor());
	FIELD_STATE *field = Ram_HeapSlots->fieldState;
	u16 *palette;
	FIXED position[3];
	s32 level;
	s32 i;
	s32 scaleX;
	s32 scaleY;
	EFFECT_SPR *sprite;

	/* The glow's strength. */
	if(fdMapGridIsHoverCell(player->x.fixed, player->z.fixed, player->layer) ==
	   0) {
		if(field->glowHeld != 0)
			field->glow--;
	}
	else {
		field->glow = GLOW_FULL;
	}

	if(state->fading == 1) {
		level = field->glow;
		if(level > GLOW_FULL)
			level = GLOW_FULL;
		if(level < 0)
			level = 0;
		state->scale = level * (FIXED_ONE / 2) / GLOW_FULL + FIXED_ONE / 2;
		if(level == 0)
			field->psynergy = GLOW_SPENT_CUE;
	}

	/* Cycle the glow's colours; the last takes the 15th's level. */
	palette = HOVER_GLOW_PALETTE;
	for(i = 0; i < PALETTE_COLORS; i++) {
		syIoWrite16(fdHoverGlowGetCol(
						(((s16)state->phase >> 2) + i / 2) % HOVER_GLOW_STEPS,
						i == PALETTE_COLORS - 1 ? PALETTE_COLORS - 2 : i),
					palette++);
	}
	state->phase++;

	/* Draw it under the player. */
	scaleX = state->scale;
	scaleY = scaleX * 6 / 8;
	sprite = &state->spr;
	sprite->oam.idx = state->tile + ((FrameCount >> 1) & 1) * 32;
	sprite->oam.pri = fdActorGetSprPriority(player);
	sprite->depth = fdActorGetSprDepth(player) - 1;
	position[0] = player->x.fixed;
	position[1] = player->y.fixed;
	position[2] = player->z.fixed;
	fdCameraWorldToScr(position);
	sprite->x = position[0];
	sprite->y = position[2];
	sprite->scaleX = scaleX;
	sprite->scaleY = scaleY;
	fdEffectSprDraw(sprite);
}

/* Hover cast on a target: runs fdPsynergyHover. */
void fdPsynergyHoverOnTarget(void) { fdPsynergyHover(); }
#undef HOVER_GLOW_PALETTE
#undef GLOW_SPENT_CUE

/* Hover: a glowing ring grows under the leader, who rises and floats */
/* (LEADER_MOVEMENT_HOVER) while fdHoverGlowUpdate holds the glow up. Where it
 */
/* cannot hover (off a hover cell, or touching a solid actor) the ring shrinks
 */
/* away and the place's Hover event runs instead. */
void fdPsynergyHover(void) {
	HOVER_STATE *state =
		syHeapSlotAlloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(HOVER_STATE));
	FIELD_EFFECT_STATE *scene = Ram_HeapSlots->fieldEffState;
	FIELD_STATE *field = Ram_HeapSlots->fieldState;
	ACTOR *player = scene->caster;
	PENDING_EVENT *pending;
	EFFECT_SPR *sprite;
	ACTOR *other;
	FIXED position[3];
	s32 blocked;
	s32 slot;
	ACTOR **list;
	s32 tile;
	s32 i;
	s32 n;

	pending = scFindPsynergyTrigger(
		PSYNERGY_TRIGGER(PSYNERGY_PHASE_ACTS), scene->fieldPsynergy,
		scSelectPsynergyTarget(scene->fieldPsynergy));
	syDecompressTaggedLz(syResourceGetTableEntry(RESOURCE_RING_ORB), state);
	slot = syVramCacheFindFreeEntry();
	tile = syVramCacheLoad(slot, 2048, state);
	state->resource = slot;
	state->tile = tile;
	state->scale = 0;
	sprite = &state->spr;
	fdEffectSprInit(sprite, 15, 15, OBJ_SIZE_32X32, (s16)state->tile);
	sprite->oam.fullCol = 1;
	sprite->oam.pal = 0;
	fdActorSetPosAndResetMotionStub(player, player->x.fixed, player->y.fixed,
									player->z.fixed);
	fdBattleEffectSetObjectUpdateMode(player, CASTER_LOOK_NORMAL);
	fdActorSelectAnimationStub(player, ANIM_LEADER_RUN);
	sySndPlayStub(SE_PSYNERGY_START);
	sySynch(1);
	/* DEAD: nothing reads it; the ROM keeps the store. */
	state->unused = 0;
	state->phase = 0;
	state->fading = 0;
	sySetEvent(fdHoverGlowUpdate, TASK_PASS_FRAME);

	blocked = 0;
	if(fdMapGridIsHoverCell(player->x.fixed, player->z.fixed, player->layer) ==
	   0) {
		blocked = 1;
	}
	else {
		for(n = 0; n < FIELD_ACTOR_COUNT; n++) {
			other = field->actors[n];
			if(other != NULL && other->script != NULL &&
			   (other->collisionFlg & ACTOR_SOLID) && other != scene->caster) {
				/* DEAD: the overlap test reads the actors themselves; the ROM
				 * keeps */
				/* the stores. */
				position[0] = other->x.fixed;
				position[1] = other->groundHeight;
				position[2] = other->z.fixed;
				if(fdPsynergyCheckOverlap(&player->x.fixed, player->radius,
										  &other->x.fixed,
										  other->radius - 2) >= 0) {
					blocked = 1;
					break;
				}
			}
		}
	}

	if(blocked) {
		sySynch(5);
		sySndPlayStub(SE_GLITTER);
		for(i = 0; i < HOVER_RING_GROW_FRAMES; i++) {
			state->scale = FIXED_ONE * i / HOVER_RING_GROW_FRAMES;
			sySynch(1);
		}
		sySynch(10);
		if(pending != NULL)
			fdRunPsynergyEvent(pending, scene->casterId, scene->tgtId);
		for(i = HOVER_RING_GROW_FRAMES - 1; i != 0; i--) {
			state->scale = FIXED_ONE * i / HOVER_RING_GROW_FRAMES;
			sySynch(1);
		}
		sySynch(5);
		syCloseEvent(fdHoverGlowUpdate);
		syVramCacheFreeEntry(state->resource);
		syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
	}
	else {
		player->motionFlg = 0;
		player->y.fixed = player->groundHeight;
		state->scale = 0;
		field->glowHeld = 0;
		/* DEAD: fdHoverGlowUpdate holds the glow at GLOW_FULL from the next
		 * frame, */
		/* the leader standing on a hover cell; the ROM keeps the store. */
		field->glow = GLOW_RISING;
		for(i = 0; i < HOVER_RING_GROW_FRAMES; i++) {
			if(i == 10)
				sySndPlayStub(SE_GLITTER);
			player->y.fixed += toFIXED(1);
			state->scale = FIXED_ONE * i / HOVER_RING_GROW_FRAMES;
			sySynch(1);
		}
		player->motionFlg = ACTOR_FLOATS;
		player->y.fixed = player->groundHeight + toFIXED(73) / 2;
		player->gravity = toFIXED(3);
		MainRoutineStateWork.movementMode = LEADER_MOVEMENT_HOVER;
		state->fading = 1;
		field->glowHeld = 1;
		field->glow = GLOW_FULL;
	}
}

#undef GLOW_FULL
#undef GLOW_RISING
#undef HOVER_RING_GROW_FRAMES

/* Ends Hover: the leader drops back to the ground with a splash and the ring */
/* is freed. */
void fdPsynergyEndHover(void) {
	HOVER_STATE *state = (HOVER_STATE *)Ram_HeapSlots->battleEff;
	ACTOR *actor = fdActorGetById(fdPartyGetLeaderActor());
	s32 i;

	syCloseEvent(fdHoverGlowUpdate);
	fdActorSetPosAndResetMotionStub(actor, actor->x.fixed, actor->y.fixed,
									actor->z.fixed);
	actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	MainRoutineStateWork.movementMode = LEADER_MOVEMENT_WALK;
	for(i = 0; i < 60; i++) {
		if(actor->y.fixed <= actor->groundHeight)
			break;
		sySynch(1);
	}
	sySndPlayStub(SE_WATER);
	sySynch(1);
	for(i = 0; i < 20; i++) {
		if(actor->y.fixed <= actor->groundHeight)
			break;
		sySynch(1);
	}
	sySynch(1);
	for(i = 0; i < 20; i++) {
		if(actor->y.fixed <= actor->groundHeight)
			break;
		sySynch(1);
	}
	sySynch(1);
	actor->bounce = 0;
	actor->velY = 0;
	actor->y.fixed = actor->groundHeight;
	actor->gravity = toFIXED(1);
	syVramCacheFreeEntry(state->resource);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
}
