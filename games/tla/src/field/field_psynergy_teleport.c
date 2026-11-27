/* @file */
/* Teleport: the leader leaving in a burst of sparks and arriving in a whirl */
/* of them, and the cast that opens the picture of the world. */
/* Part of field_psynergy.c. */

#define WHIRL_COUNT 100 /* sparks that whirl round the leader */
/* The frames a spark flies before it vanishes. */
#define TELEPORT_SPARK_FRAMES 20
/* Before the sparks, the leader spins for TELEPORT_SPIN_FRAMES, its turn each
 */
/* frame TELEPORT_SPIN_ACCELERATION (1/65536 turns) faster than the last. */
#define TELEPORT_SPIN_FRAMES	   50
#define TELEPORT_SPIN_ACCELERATION 60

/* The stages of fdTeleportPlayWhirl. */
typedef enum WHIRL_STAGE {
	WHIRL_FALLS_IN =
		0, /* more and more sparks fall in round the hidden leader */
	WHIRL_LEADER_GROWS = 1,
	WHIRL_DIES_AWAY = 2
} WHIRL_STAGE;

/* The stages of fdTeleportPlaySparks. */
typedef enum SPARK_STAGE {
	SPARKS_BURST = 0, /* more and more sparks burst from the spinning leader */
	SPARKS_LEADER_SHRINKS = 1,
	SPARKS_DIE_AWAY = 2
} SPARK_STAGE;

/* Teleport's whirl (heap slot battleEffect): the decoded spark picture, */
/* then the 100 sparks and their sprites. */
typedef struct TELEPORT_WHIRL_STATE {
	u8 picture[1024];
	WHIRL_SPARK sparks[WHIRL_COUNT];
	EFFECT_SPR sprs[WHIRL_COUNT];
} TELEPORT_WHIRL_STATE;

/* The arriving half of a teleport: a whirl of sparks, the coloured orbs of */
/* RESOURCE_COLOURED_ORBS, falls in round the hidden leader with */
/* SE_SPARKLE_FALL, the leader spins and grows back to full size inside it, and
 */
/* the whirl dies away. The scenes with Teleport circles call it (Mars */
/* Lighthouse, the Anemos Inner Sanctum, the Yampi Desert and Islet caves, */
/* Contigo). */
void fdTeleportPlayWhirl(void) {
	s32 position[3];
	TELEPORT_WHIRL_STATE *teleportWhirlState =
		syHeapSlotAlloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(*teleportWhirlState));
	ACTOR *actor = fdActorLookup(fdPartyGetLeaderActor());
	WHIRL_SPARK *spark;
	EFFECT_SPR *sprite;
	s32 next;
	s32 state;
	s32 entry;
	s32 count;
	s32 tiles;
	s32 i;
	s32 angle;
	s32 frame;
	FIXED radius;

	syDecompressTaggedLz(syResourceGetTableEntry(RESOURCE_COLOURED_ORBS),
						 teleportWhirlState);
	entry = syVramCacheFindFreeEntry();
	tiles = syVramCacheLoad(entry, 1024, teleportWhirlState);
	for(i = 0, spark = teleportWhirlState->sparks; i < WHIRL_COUNT;
		i++, spark++) {
		sprite = &teleportWhirlState->sprs[i];
		fdEffectSprInit(sprite, 4, 4, 0,
						tiles +
							(i & (ORB_PICTURES - 1)) * OBJ_TILES_PER_8BPP_TILE);
		sprite->oam.fullCol = 1;
		sprite->oam.pal = 0;
		sprite->oam.pri = 0;
		sprite->depth = OAM_DEPTH_WINDOW;
		spark->age = -1;
	}
	sySndPlayStub(SE_SPARKLE_FALL);
	sySynch(20);
	actor->scaleX = 0;
	actor->scaleY = 0;
	fdActorSetSprFlagsStub(actor, 0);

	count = 2;
	state = WHIRL_FALLS_IN;
	next = 0;
	frame = 0;
	do {
		if(count != 0) {
			radius = toFIXED(80);
			i = count;
			do {
				angle = syRandom();
				spark = &teleportWhirlState->sparks[next];
				spark->age = 0;
				SET_POSITION(spark, actor->x.fixed,
							 actor->y.fixed + syRandom() * 20, actor->z.fixed);
				syVectorAddPolarOffset(syRandom() << 3, angle, &spark->x);
				next++;
				spark->ang = angle;
				spark->radius = radius;
				next %= WHIRL_COUNT;
				i--;
			} while(i != 0);
		}

		spark = teleportWhirlState->sparks;
		sprite = teleportWhirlState->sprs;
		for(i = 0; i < WHIRL_COUNT; i++, sprite++, spark++) {
			if(spark->age >= 0) {
				if((u32)spark->age < TELEPORT_SPARK_FRAMES) {
					s32 sparkAngle = spark->ang;
					s32 sparkRadius = spark->radius;
					position[0] = spark->x;
					position[1] = spark->y;
					position[2] = spark->z;
					syVectorAddPolarOffset(sparkRadius, sparkAngle, position);
					fdCameraWorldToScr(position);
					sprite->x = position[0];
					sprite->y = position[2];
					fdEffectSprDraw(sprite);
					spark->ang += TRIG_TURN / 32;
					spark->radius += -toFIXED(4);
				}
				if(++spark->age == TELEPORT_SPARK_FRAMES)
					spark->age = -1;
			}
		}

		switch(state) {
		case WHIRL_FALLS_IN:
			if(frame == 20)
				count = 3;
			if(frame == 30) {
				count = 5;
				state = WHIRL_LEADER_GROWS;
				frame = -1;
			}
			break;
		case WHIRL_LEADER_GROWS:
			actor->facing += DEGtoANG(45);
			actor->scaleX = FIXED_ONE * frame / 30;
			if(actor->scaleX > FIXED_ONE)
				actor->scaleX = FIXED_ONE;
			actor->scaleY = actor->scaleX;
			if(frame == 40) {
				state = WHIRL_DIES_AWAY;
				frame = -1;
			}
			break;
		case WHIRL_DIES_AWAY:
			if(frame == 0) {
				actor->facing = DEGtoANG(90);
				fdActorSetSprFlagsStub(actor, SPRITE_FLAG_SHADOW);
				count = 3;
			}
			if(frame == 10)
				count = 1;
			if(frame == 20)
				count = 0;
			if(frame == 40)
				state = FIELD_PSYNERGY_FINISHED;
			break;
		}
		sySynch(1);
		frame++;
	} while(state != FIELD_PSYNERGY_FINISHED);
	syVramCacheFreeEntry(entry);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
	fdPsynergyMarkWorldMapTilesUsed();
}

/* The sparks of Teleport's leaving burst. */
#define TELEPORT_SPARK_COUNT 100

/* Teleport's burst of sparks (heap slot battleEffect): the decoded picture, */
/* then the sparks and their sprites. */
typedef struct TELEPORT_SPARKS_STATE {
	u8 picture[1024];
	FIELD_PSYNERGY_PARTICLE particles[TELEPORT_SPARK_COUNT];
	EFFECT_SPR sprs[TELEPORT_SPARK_COUNT];
} TELEPORT_SPARKS_STATE;

/* The leaving half of a teleport, fdTeleportPlayWhirl's companion in the same
 */
/* scenes: the leader raises both arms in changing colours, spins faster and */
/* faster as sparks burst from it, shrinks away and is hidden. */
void fdTeleportPlaySparks(void) {
	TELEPORT_SPARKS_STATE *teleportSparksState =
		syHeapSlotAlloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(*teleportSparksState));
	s32 owner = fdPartyGetLeaderActor();
	ACTOR *actor = fdActorLookup(fdPartyGetLeaderActor());
	FIELD_PSYNERGY_PARTICLE *particle;
	EFFECT_SPR *sprite;
	s32 state;
	s32 entry;
	s32 count;
	s32 tiles;
	s32 i;
	s32 spin;
	s32 angle;
	s32 next;
	s32 frame;
	s32 age;

	sySynch(20);
	fdPsynergyFadeToCastCol();
	syDecompressTaggedLz(syResourceGetTableEntry(RESOURCE_COLOURED_ORBS),
						 teleportSparksState);
	entry = syVramCacheFindFreeEntry();
	tiles = syVramCacheLoad(entry, 1024, teleportSparksState);
	for(i = 0, particle = teleportSparksState->particles;
		i < TELEPORT_SPARK_COUNT; i++, particle++) {
		sprite = &teleportSparksState->sprs[i];
		fdEffectSprInit(sprite, 4, 4, 0,
						tiles +
							(i & (ORB_PICTURES - 1)) * OBJ_TILES_PER_8BPP_TILE);
		sprite->oam.fullCol = 1;
		sprite->oam.pal = 0;
		sprite->oam.pri = 0;
		sprite->depth = OAM_DEPTH_WINDOW;
		particle->tmr = -1;
	}
	sySynch(10);
	fdActorFaceDirectionById(owner, FACING_SOUTH, 0);
	sySynch(20);
	fdActorSelectAnimationStub(actor, ANIM_LEADER_ARMS_RAISED);
	actor->update = fdActorCycleSprCols;
	sySndPlayStub(SE_PSYNERGY_SPARKLE);
	sySynch(30);
	sySndPlayStub(SE_SHIMMER);
	fdActorSelectAnimationStub(actor, ANIM_NOD);
	for(spin = 0, frame = 0; frame < TELEPORT_SPIN_FRAMES;
		frame++, spin += TELEPORT_SPIN_ACCELERATION) {
		actor->facing += spin + FACING_STEP;
		sySynch(1);
	}

	count = 2;
	state = SPARKS_BURST;
	next = 0;
	frame = 0;
	do {
		for(i = 0; i < count; i++) {
			angle = syRandom();
			particle = &teleportSparksState->particles[next];
			particle->tmr = 0;
			SET_POSITION(particle, actor->x.fixed,
						 actor->y.fixed + syRandom() * 20, actor->z.fixed);
			syVectorAddPolarOffset(syRandom() << 3, angle, &particle->x);
			if(next & 1) {
				particle->velX = 0;
				particle->velY = -toFIXED(4);
				particle->velZ = 0;
				syVectorAddPolarOffset(
					(syRandom() << 2) + toFIXED(4), angle,
					&teleportSparksState->particles[next].velX);
			}
			else {
				particle->velX = 0;
				particle->velY = toFIXED(5);
				particle->velZ = 0;
				syVectorAddPolarOffset(
					(syRandom() << 2) + toFIXED(4), angle,
					&teleportSparksState->particles[next].velX);
			}
			next++;
			next %= TELEPORT_SPARK_COUNT;
		}

		for(i = 0, particle = teleportSparksState->particles,
		sprite = teleportSparksState->sprs;
			i < TELEPORT_SPARK_COUNT; i++, sprite++, particle++) {
			age = particle->tmr;
			if(age >= 0) {
				if((u32)age < TELEPORT_SPARK_FRAMES) {
					fdEffectSprDrawAtWorldPos(sprite, particle);
					if(i & 1)
						fdAdvanceWithGravity3D(&particle->x, 63,
											   toFIXED(1) / 2);
					else
						fdAdvanceWithGravity3D(&particle->x, 63,
											   -(toFIXED(5) / 8));
				}
				if(++particle->tmr == TELEPORT_SPARK_FRAMES)
					particle->tmr = -1;
			}
		}

		switch(state) {
		case SPARKS_BURST:
			if(frame == 10)
				sySndPlayStub(SE_KOLIMA_BRIDGE_POST);
			actor->facing += DEGtoANG(45);
			if(frame == 40)
				count = 3;
			if(frame == 50)
				count = 4;
			if(frame == 60) {
				count = 5;
				state = SPARKS_LEADER_SHRINKS;
				frame = -1;
			}
			break;
		case SPARKS_LEADER_SHRINKS:
			actor->facing += DEGtoANG(45);
			actor->scaleX = FIXED_ONE - FIXED_ONE * frame / 30;
			if(actor->scaleX < 0)
				actor->scaleX = 0;
			actor->scaleY = actor->scaleX;
			fdActorSetSprFlagsStub(actor, 0);
			if(frame == 30) {
				state = SPARKS_DIE_AWAY;
				frame = -1;
			}
			break;
		case SPARKS_DIE_AWAY:
			count = 0;
			if(frame == 20)
				state = FIELD_PSYNERGY_FINISHED;
			break;
		}
		sySynch(1);
		frame++;
	} while(state != FIELD_PSYNERGY_FINISHED);
	syVramCacheFreeEntry(entry);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
}

/* Runs fdTeleportPlaySparks, fades the palettes back to the saved ones and */
/* waits 10 frames. */
void fdTeleportPlaySparksAndFade(void) {
	fdTeleportPlaySparks();
	fdPsynergyFadeFromCastCol();
	sySynch(10);
}

/* Teleport cast on a target: does nothing. */
void fdPsynergyTeleportOnTarget(void) {}

/* Teleport: raises FIELD_RAISED_TRIGGER_TELEPORT, which shows the picture of */
/* the whole world for the player to choose where to go. */
void fdPsynergyTeleport(void) {
	/* DEAD: the leader looked up and never used; the ROM keeps the calls. */
	fdActorLookup(fdPartyGetLeaderActor());
	FieldStateWork->raisedTrigger = FIELD_RAISED_TRIGGER_TELEPORT;
}
#undef WHIRL_COUNT
#undef TELEPORT_SPARK_FRAMES
#undef TELEPORT_SPIN_FRAMES
#undef TELEPORT_SPIN_ACCELERATION
#undef TELEPORT_SPARK_COUNT
