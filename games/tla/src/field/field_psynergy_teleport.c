/* Teleport: the leader leaving in a burst of sparks and arriving in a whirl of
   them, and the cast that opens the picture of the world. Part of
   field_psynergy.c. */

#define WHIRL_COUNT				   100 /* sparks that whirl round the leader */
/* The ticks a spark flies before it vanishes. */
#define TELEPORT_SPARK_TICKS	   20
/* Before the sparks, the leader spins for TELEPORT_SPIN_TICKS, its turn each
   tick TELEPORT_SPIN_ACCELERATION (1/65536 turns) faster than the last. */
#define TELEPORT_SPIN_TICKS		   50
#define TELEPORT_SPIN_ACCELERATION 60

/* The stages of fdTeleportPlayWhirl. */
typedef enum WHIRL_STAGE {
	/* more and more sparks fall in round the hidden leader */
	WHIRL_FALLS_IN = 0,
	WHIRL_LEADER_GROWS = 1,
	WHIRL_DIES_AWAY = 2
} WHIRL_STAGE;

/* The stages of fdTeleportPlaySparks. */
typedef enum SPARK_STAGE {
	SPARKS_BURST = 0, /* more and more sparks burst from the spinning leader */
	SPARKS_LEADER_SHRINKS = 1,
	SPARKS_DIE_AWAY = 2
} SPARK_STAGE;

/* Teleport's whirl (heap slot battleEffect): the decoded spark picture, then
   the 100 sparks and their sprites. */
typedef struct TELEPORT_WHIRL_STATE {
	Uint8		picture[1024];
	WHIRL_SPARK sparks[WHIRL_COUNT];
	EFFECT_SPR	sprs[WHIRL_COUNT];
} TELEPORT_WHIRL_STATE;

/* The arriving half of a teleport: a whirl of sparks, the coloured orbs of
   RESOURCE_COLOURED_ORBS, falls in round the hidden leader with
   SE_SPARKLE_FALL, the leader spins and grows back to full size inside it, and
   the whirl dies away. The scenes with Teleport circles call it (Mars
   Lighthouse, the Anemos Inner Sanctum, the Yampi Desert and Islet caves,
   Contigo). */
void fdTeleportPlayWhirl(void)
{
	Sint32				  pos[3];
	TELEPORT_WHIRL_STATE *teleportWhirlState =
		syHeapSlotAlloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(*teleportWhirlState));
	ACTOR		*actor = fdActorLookup(fdPartyGetLeaderActor());
	WHIRL_SPARK *spark;
	EFFECT_SPR	*spr;
	Sint32		 next;
	Sint32		 state;
	Sint32		 entry;
	Sint32		 cnt;
	Sint32		 tiles;
	Sint32		 i;
	Sint32		 ang;
	Sint32		 frm;
	FIXED		 radius;

	syDecompressTaggedLZ(
		syResourceGetTableEntry(RESOURCE_COLOURED_ORBS), teleportWhirlState);
	entry = syVRAMCacheFindFreeEntry();
	tiles = syVRAMCacheLoad(entry, 1024, teleportWhirlState);
	for(i = 0, spark = teleportWhirlState->sparks; i < WHIRL_COUNT;
		i++, spark++) {
		spr = &teleportWhirlState->sprs[i];
		fdEffectSprInit(spr, 4, 4, 0,
			tiles + (i & (ORB_PICTURES - 1)) * OBJ_TILES_PER_8BPP_TILE);
		spr->oam.fullCol = 1;
		spr->oam.pal = 0;
		spr->oam.pri = 0;
		spr->depth = OAM_DEPTH_WINDOW;
		spark->age = -1;
	}
	sySndPlayStub(SE_SPARKLE_FALL);
	sySynch(20);
	actor->scaleX = 0;
	actor->scaleY = 0;
	fdActorSetSprFlagsStub(actor, 0);

	cnt = 2;
	state = WHIRL_FALLS_IN;
	next = 0;
	frm = 0;
	do {
		if(cnt != 0) {
			radius = toFIXED(80);
			i = cnt;
			do {
				ang = syRandom();
				spark = &teleportWhirlState->sparks[next];
				spark->age = 0;
				SET_POSITION(spark, actor->x.fixed,
					actor->y.fixed + syRandom() * 20, actor->z.fixed);
				syVectorAddPolarOffset(syRandom() << 3, ang, &spark->x);
				next++;
				spark->ang = ang;
				spark->radius = radius;
				next %= WHIRL_COUNT;
				i--;
			} while(i != 0);
		}

		spark = teleportWhirlState->sparks;
		spr = teleportWhirlState->sprs;
		for(i = 0; i < WHIRL_COUNT; i++, spr++, spark++) {
			if(spark->age >= 0) {
				if((Uint32)spark->age < TELEPORT_SPARK_TICKS) {
					Sint32 sparkAng = spark->ang;
					Sint32 sparkRadius = spark->radius;
					pos[0] = spark->x;
					pos[1] = spark->y;
					pos[2] = spark->z;
					syVectorAddPolarOffset(sparkRadius, sparkAng, pos);
					fdCameraWorldToScreen(pos);
					spr->x = pos[0];
					spr->y = pos[2];
					fdEffectSprDraw(spr);
					spark->ang += TRIG_TURN / 32;
					spark->radius += -toFIXED(4);
				}
				if(++spark->age == TELEPORT_SPARK_TICKS)
					spark->age = -1;
			}
		}

		switch(state) {
		case WHIRL_FALLS_IN:
			if(frm == 20)
				cnt = 3;
			if(frm == 30) {
				cnt = 5;
				state = WHIRL_LEADER_GROWS;
				frm = -1;
			}
			break;
		case WHIRL_LEADER_GROWS:
			actor->facing += DEGtoANG(45);
			actor->scaleX = toFIXED(1) * frm / 30;
			if(actor->scaleX > toFIXED(1))
				actor->scaleX = toFIXED(1);
			actor->scaleY = actor->scaleX;
			if(frm == 40) {
				state = WHIRL_DIES_AWAY;
				frm = -1;
			}
			break;
		case WHIRL_DIES_AWAY:
			if(frm == 0) {
				actor->facing = DEGtoANG(90);
				fdActorSetSprFlagsStub(actor, SPRITE_FLAG_SHADOW);
				cnt = 3;
			}
			if(frm == 10)
				cnt = 1;
			if(frm == 20)
				cnt = 0;
			if(frm == 40)
				state = FIELD_PSYNERGY_FINISHED;
			break;
		}
		sySynch(1);
		frm++;
	} while(state != FIELD_PSYNERGY_FINISHED);
	syVRAMCacheFreeEntry(entry);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
	fdPsynergyMarkWorldMapTilesUsed();
}

/* The sparks of Teleport's leaving burst. */
#define TELEPORT_SPARK_COUNT 100

/* Teleport's burst of sparks (heap slot battleEffect): the decoded picture,
   then the sparks and their sprites. */
typedef struct TELEPORT_SPARKS_STATE {
	Uint8					picture[1024];
	FIELD_PSYNERGY_PARTICLE particles[TELEPORT_SPARK_COUNT];
	EFFECT_SPR				sprs[TELEPORT_SPARK_COUNT];
} TELEPORT_SPARKS_STATE;

/* The leaving half of a teleport, fdTeleportPlayWhirl's companion in the same
   scenes: the leader raises both arms in changing colours, spins faster and
   faster as sparks burst from it, shrinks away and is hidden. */
void fdTeleportPlaySparks(void)
{
	TELEPORT_SPARKS_STATE *teleportSparksState =
		syHeapSlotAlloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(*teleportSparksState));
	Sint32					 owner = fdPartyGetLeaderActor();
	ACTOR					*actor = fdActorLookup(fdPartyGetLeaderActor());
	FIELD_PSYNERGY_PARTICLE *particle;
	EFFECT_SPR				*spr;
	Sint32					 state;
	Sint32					 entry;
	Sint32					 cnt;
	Sint32					 tiles;
	Sint32					 i;
	Sint32					 spin;
	Sint32					 ang;
	Sint32					 next;
	Sint32					 frm;
	Sint32					 age;

	sySynch(20);
	fdPsynergyFadeToCastCol();
	syDecompressTaggedLZ(
		syResourceGetTableEntry(RESOURCE_COLOURED_ORBS), teleportSparksState);
	entry = syVRAMCacheFindFreeEntry();
	tiles = syVRAMCacheLoad(entry, 1024, teleportSparksState);
	for(i = 0, particle = teleportSparksState->particles;
		i < TELEPORT_SPARK_COUNT; i++, particle++) {
		spr = &teleportSparksState->sprs[i];
		fdEffectSprInit(spr, 4, 4, 0,
			tiles + (i & (ORB_PICTURES - 1)) * OBJ_TILES_PER_8BPP_TILE);
		spr->oam.fullCol = 1;
		spr->oam.pal = 0;
		spr->oam.pri = 0;
		spr->depth = OAM_DEPTH_WINDOW;
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
	for(spin = 0, frm = 0; frm < TELEPORT_SPIN_TICKS;
		frm++, spin += TELEPORT_SPIN_ACCELERATION) {
		actor->facing += spin + FACING_STEP;
		sySynch(1);
	}

	cnt = 2;
	state = SPARKS_BURST;
	next = 0;
	frm = 0;
	do {
		for(i = 0; i < cnt; i++) {
			ang = syRandom();
			particle = &teleportSparksState->particles[next];
			particle->tmr = 0;
			SET_POSITION(particle, actor->x.fixed,
				actor->y.fixed + syRandom() * 20, actor->z.fixed);
			syVectorAddPolarOffset(syRandom() << 3, ang, &particle->x);
			if(next & 1) {
				particle->velX = 0;
				particle->velY = -toFIXED(4);
				particle->velZ = 0;
				syVectorAddPolarOffset((syRandom() << 2) + toFIXED(4), ang,
					&teleportSparksState->particles[next].velX);
			} else {
				particle->velX = 0;
				particle->velY = toFIXED(5);
				particle->velZ = 0;
				syVectorAddPolarOffset((syRandom() << 2) + toFIXED(4), ang,
					&teleportSparksState->particles[next].velX);
			}
			next++;
			next %= TELEPORT_SPARK_COUNT;
		}

		for(i = 0, particle = teleportSparksState->particles,
		spr = teleportSparksState->sprs;
			i < TELEPORT_SPARK_COUNT; i++, spr++, particle++) {
			age = particle->tmr;
			if(age >= 0) {
				if((Uint32)age < TELEPORT_SPARK_TICKS) {
					fdEffectSprDrawAtWorldPos(spr, particle);
					if(i & 1)
						fdAdvanceWithGravity3D(
							&particle->x, 63, toFIXED(1) / 2);
					else
						fdAdvanceWithGravity3D(
							&particle->x, 63, -(toFIXED(5) / 8));
				}
				if(++particle->tmr == TELEPORT_SPARK_TICKS)
					particle->tmr = -1;
			}
		}

		switch(state) {
		case SPARKS_BURST:
			if(frm == 10)
				sySndPlayStub(SE_KOLIMA_BRIDGE_POST);
			actor->facing += DEGtoANG(45);
			if(frm == 40)
				cnt = 3;
			if(frm == 50)
				cnt = 4;
			if(frm == 60) {
				cnt = 5;
				state = SPARKS_LEADER_SHRINKS;
				frm = -1;
			}
			break;
		case SPARKS_LEADER_SHRINKS:
			actor->facing += DEGtoANG(45);
			actor->scaleX = toFIXED(1) - toFIXED(1) * frm / 30;
			if(actor->scaleX < 0)
				actor->scaleX = 0;
			actor->scaleY = actor->scaleX;
			fdActorSetSprFlagsStub(actor, 0);
			if(frm == 30) {
				state = SPARKS_DIE_AWAY;
				frm = -1;
			}
			break;
		case SPARKS_DIE_AWAY:
			cnt = 0;
			if(frm == 20)
				state = FIELD_PSYNERGY_FINISHED;
			break;
		}
		sySynch(1);
		frm++;
	} while(state != FIELD_PSYNERGY_FINISHED);
	syVRAMCacheFreeEntry(entry);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
}

/* Runs fdTeleportPlaySparks, fades the palettes back to the saved ones and
   waits 10 ticks. */
void fdTeleportPlaySparksAndFade(void)
{
	fdTeleportPlaySparks();
	fdPsynergyFadeFromCastCol();
	sySynch(10);
}

/* Teleport cast on a target: does nothing. */
void fdPsynergyTeleportOnTarget(void)
{
}

/* Teleport: raises FIELD_RAISED_CUE_TELEPORT, which shows the picture of the
   whole world for the player to choose where to go. */
void fdPsynergyTeleport(void)
{
	/* DEAD: the leader looked up and never used; the ROM keeps the calls. */
	fdActorLookup(fdPartyGetLeaderActor());
	FieldStateWork->raisedCue = FIELD_RAISED_CUE_TELEPORT;
}

#undef WHIRL_COUNT
#undef TELEPORT_SPARK_TICKS
#undef TELEPORT_SPIN_TICKS
#undef TELEPORT_SPIN_ACCELERATION
#undef TELEPORT_SPARK_COUNT
