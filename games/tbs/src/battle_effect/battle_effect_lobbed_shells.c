/* Lobbed shells (Mad Spatter, Decompose, Bone Chiller ...). Part of
   battle_effect.c. */

/* Lobbed shells: a shell arcs from the user onto each target and bursts into
   smoke or shards, in seven colourings. */

/* The lobbed shells' styles (btEffectRunLobbedShells), each named for the
   effect that plays it, and its row of LobbedShellsStyles. */
typedef enum LOBBED_SHELLS_MODE {
	LOBBED_SHELLS_BONE_CHILLER,	 /* blue, bursting into smoke */
	LOBBED_SHELLS_STICKY_GOO,	 /* the spider web's colours, smoke */
	LOBBED_SHELLS_STICKY_POISON, /* lime, smoke */
	LOBBED_SHELLS_ROTTEN_BLOOD,	 /* green, shards */
	LOBBED_SHELLS_MAD_SPATTER,	 /* gold, shards */
	LOBBED_SHELLS_DECOMPOSE,	 /* green, smoke */
	LOBBED_SHELLS_POISON_INK	 /* Mars Djinn colours, smoke */
} LOBBED_SHELLS_MODE;

#include "battle_effect/effect_mad_spatter.c"

#include "battle_effect/effect_poison_shells.c"

/* The bursts, after the shells in PARTICLES. */
#define LOBBED_SHELLS_BURSTS	  (PARTICLES + 256)
#define LOBBED_SHELLS_SLOTS		  16  /* particles reserved for each target */
#define LOBBED_SHELLS_BURST_SLOTS 256 /* burst particles after the shells */
#define LOBBED_SHELLS_SMOKE_PICTURES \
	16000 /* where the smoke starts in the sheet */
/* GoldShellSheet's shell as it tumbles, upright and then turned on its side. */
#define LOBBED_SHELL_WIDTH			  24
#define LOBBED_SHELL_HEIGHT			  32
/* The shell's tumbling pictures, and the shards' pictures after them; it turns
   on its side from picture LOBBED_SHELL_UPRIGHT_PICTURES, a picture every three
   ticks. */
#define LOBBED_SHELL_PICTURES		  10
#define LOBBED_SHELL_UPRIGHT_PICTURES 5
#define LOBBED_SHELLS_SHARD_PICTURES \
	(LOBBED_SHELL_PICTURES * PICTURE_BYTES(LOBBED_SHELL))
/* The pieces a shell bursts into: puffs of smoke, or shards. */
#define LOBBED_SHELLS_SMOKE_PUFFS	   4
#define LOBBED_SHELLS_SHARDS		   16
/* The ticks a burst's piece is drawn for. */
#define LOBBED_SHELLS_BURST_TICKS	   45
/* Each target's shell is lobbed this many ticks after the last one's, and
   lands LOBBED_SHELL_FLIGHT_TICKS later, as its target flashes. */
#define LOBBED_SHELLS_TARGET_TICKS	   8
#define LOBBED_SHELL_FLIGHT_TICKS	   17
/* The ticks the effect runs on after the last shell is lobbed: smoke drifts
   longer than shards fall. */
#define LOBBED_SHELLS_SMOKE_END_TICKS  72
#define LOBBED_SHELLS_SHARDS_END_TICKS 56

/* Nonzero when the style's shells burst into shards instead of smoke. */
#define LOBBED_SHELLS_SHATTER(mode) \
	LobbedShellsStyles[(mode)][LOBBED_SHELLS_STYLE_SHATTER]

/* Lobs a shell from the user at each target in turn; when it lands the target
   flashes and the shell bursts into smoke that drifts up or shards that fly up
   and fall. */
void btEffectRunLobbedShells(BATTLE_EFFECT_PARM *args, Sint32 mode)
{
	SCREEN_POINT		 pos;
	BLITTER_PAIR		 draw;
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 frm;
	Sint32				 k;
	Sint32				 total;
	ACTOR				*src;
	BATTLE_CAMERA		*camera;
	Uint8				*resource;
	Sint32				 i;
	FIXED				 height;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
	state->args = args;
	btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	REG_BLDALPHA = EFFECT_BLEND_ADD;

	resource = syResourceGetTableEntry(RESOURCE_GOLD_SHELL_SHEET);
	syWordsCopy(BG_PLTT, resource, SHEET_PALETTE_BYTES);
	resource += SHEET_PALETTE_BYTES;
	syDecompressTaggedLZ(resource, state->sheet);
	resource = syResourceGetTableEntry(RESOURCE_SMOKE_SHEET);
	syWordsCopy(BG_PLTT, resource, SHEET_PALETTE_BYTES);
	resource += SHEET_PALETTE_BYTES;
	syDecompressTaggedLZ(resource, state->sheet + LOBBED_SHELLS_SMOKE_PICTURES);
	if(mode == LOBBED_SHELLS_BONE_CHILLER)
		resource = syResourceGetTableEntry(RESOURCE_BLUE_PALETTE);
	else if(mode == LOBBED_SHELLS_STICKY_GOO)
		resource = syResourceGetTableEntry(RESOURCE_SPIDER_WEB_SHEET);
	else if(mode == LOBBED_SHELLS_STICKY_POISON)
		resource = syResourceGetTableEntry(RESOURCE_LIME_PALETTE);
	else if(mode == LOBBED_SHELLS_ROTTEN_BLOOD)
		resource = syResourceGetTableEntry(RESOURCE_GREEN_PALETTE);
	else if(mode == LOBBED_SHELLS_MAD_SPATTER)
		resource = syResourceGetTableEntry(RESOURCE_GOLD_SHELL_SHEET);
	else if(mode == LOBBED_SHELLS_POISON_INK)
		resource = syResourceGetTableEntry(RESOURCE_MARS_DJINN_SHEET);
	else
		resource = syResourceGetTableEntry(RESOURCE_GREEN_PALETTE);
	syWordsCopy(BG_PLTT, resource, SHEET_PALETTE_BYTES);

	/* Each target's shell starts at the user. */
	EFFECT_FREE_PARTICLES(i, PARTICLES, PARTICLE_POOL_COUNT);
	src = btObjectGetStub(state->args->user)->actor;
	height = toFIXED(4);
	EFFECT_FOR_EACH_TARGET(k, state->args) {
		ACTOR *tgt = btObjectGetStub(state->args->tgts[k])->actor;

		for(i = 0; i != LOBBED_SHELLS_SLOTS; i++) {
			PARTICLE *shot = &PARTICLES[k * LOBBED_SHELLS_SLOTS + i];

			SET_POSITION(shot, src->x.fixed, height, src->z.fixed);
			SET_VELOCITY(shot, (tgt->x.fixed - src->x.fixed) >> 4, toFIXED(4),
				(tgt->z.fixed - src->z.fixed) >> 4);
			shot->variant = 0;
		}
	}
	EFFECT_FREE_PARTICLES(i, LOBBED_SHELLS_BURSTS, LOBBED_SHELLS_BURST_SLOTS);

	if(args->side == EFFECT_SIDE_ENEMIES) {
		EFFECT_LOAD_BLITTER(
			HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
		draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
		if(LOBBED_SHELLS_SHATTER(mode) == 0) {
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
				COMPOSITE_ADDITIVE);
			draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
		} else {
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
				EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_LIGHTEN);
			draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
		}
	} else {
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
			EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_LIGHTEN);
		draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
		if(LOBBED_SHELLS_SHATTER(mode) == 0) {
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
				COMPOSITE_ADDITIVE);
			draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
		} else {
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
				COMPOSITE_LIGHTEN);
			draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
		}
	}
	EFFECT_START_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
	if(LOBBED_SHELLS_SHATTER(mode) == 0)
		total = state->args->tgtCnt * LOBBED_SHELLS_TARGET_TICKS +
			LOBBED_SHELLS_SMOKE_END_TICKS;
	else
		total = state->args->tgtCnt * LOBBED_SHELLS_TARGET_TICKS +
			LOBBED_SHELLS_SHARDS_END_TICKS;
	sySndPlayStub(SE_CURSE);

	for(frm = 0; frm != total; frm++) {
		PARTICLE *burst;

		EFFECT_VIEW_FROM_CAMERA(camera);
		k = 0;
		if(state->args->tgtCnt != 0) {
			do {
				if(frm >= k * LOBBED_SHELLS_TARGET_TICKS) {
					PARTICLE *shot = &PARTICLES[k * LOBBED_SHELLS_SLOTS];

					if(frm ==
						k * LOBBED_SHELLS_TARGET_TICKS +
							LOBBED_SHELL_FLIGHT_TICKS) {
						btEffectTintBattler(state->args->tgts[k],
							SPRITE_COLOR_WHITE, POSE_DAMAGE, k, 16);
						EFFECT_SHOW_DAMAGE(SE_IMPACT);
					}
					if(shot->variant >= 0) {
						Sint32 step;

						/* FAKEMATCH(dead): an empty once-only loop. */
						do {
						} while(0);
						/* The shell tumbles as it flies. */
						step = (frm - k * LOBBED_SHELLS_TARGET_TICKS) / 3;
						if(step > LOBBED_SHELL_PICTURES - 1)
							step = LOBBED_SHELL_PICTURES - 1;
						btProjectionPoint(shot, &pos);
						pos.x >>= 1;
						if(step >= LOBBED_SHELL_UPRIGHT_PICTURES)
							draw[0](canvas,
								state->sheet +
									step * PICTURE_BYTES(LOBBED_SHELL),
								pos.x - LOBBED_SHELL_HEIGHT / 2,
								pos.y - LOBBED_SHELL_WIDTH / 2,
								LOBBED_SHELL_HEIGHT, LOBBED_SHELL_WIDTH);
						else
							draw[0](canvas,
								state->sheet +
									step * PICTURE_BYTES(LOBBED_SHELL),
								pos.x - LOBBED_SHELL_WIDTH / 2,
								pos.y - LOBBED_SHELL_HEIGHT / 2,
								LOBBED_SHELL_WIDTH, LOBBED_SHELL_HEIGHT);
						if(shot->variant == 0)
							btParticleAdvanceWithGravity3D(
								shot, 63, -toFIXED(1) / 2);
						/* On the ground the shell stops and bursts. */
						if(shot->y < 0) {
							Sint32 cnt;
							/* FAKEMATCH(temporary): records, a pointer to
							 * LobbedShellsStyles for the one read; as
							 * LOBBED_SHELLS_SHATTER(mode). */
							Sint8 *records = LobbedShellsStyles[0];
							Sint32 groupSlots = 8;

							shot->y = 0;
							shot->variant = 1;
							if(records[mode << 1] == 0)
								cnt = LOBBED_SHELLS_SMOKE_PUFFS;
							else
								cnt = LOBBED_SHELLS_SHARDS;
							/* Each target's pieces take four groups of eight
							   slots, room for four shells as The Lost Age's Mud
							   lobs; a shell's 16 shards fill two. */
							for(i = 0; i != cnt; i++) {
								PARTICLE *burst;
								burst = &LOBBED_SHELLS_BURSTS[(k << 2) *
										groupSlots +
									i];

								SET_POSITION(burst, shot->x, shot->y, shot->z);
								if(LOBBED_SHELLS_SHATTER(mode) == 0) {
									SET_VELOCITY(burst,
										((syRandom() & 63) - 32) << 11, 0,
										((syRandom() & 63) - 32) << 11);
								} else {
									SET_VELOCITY(burst,
										((syRandom() & 63) - 32) << 13,
										((syRandom() & 31) + 32) << 12,
										((syRandom() & 63) - 32) << 13);
								}
								burst->variant = 0;
							}
						}
					}
				}
				k++;
			} while(k != state->args->tgtCnt);
		}
		/* The bursts. */
		for(i = 0; i != LOBBED_SHELLS_BURST_SLOTS; i++) {
			burst = &LOBBED_SHELLS_BURSTS[i];
			if(burst->variant >= 0 &&
				burst->variant < LOBBED_SHELLS_BURST_TICKS && burst->y >= 0) {
				btProjectionPoint(burst, &pos);
				pos.x >>= 1;
				if(LOBBED_SHELLS_SHATTER(mode) == 0) {
					draw[1](canvas,
						state->sheet +
							burst->variant / 8 * PICTURE_BYTES(SMOKE_PUFF) +
							LOBBED_SHELLS_SMOKE_PICTURES,
						pos.x - SMOKE_PUFF_WIDTH / 2,
						pos.y - SMOKE_PUFF_HEIGHT / 2, SMOKE_PUFF_WIDTH,
						SMOKE_PUFF_HEIGHT);
				} else {
					Sint32 picture = burst->variant / 5;
					Sint32 flip;

					/* Every second shard tumbles through the other nine
					   pictures. */
					if(i & 1)
						picture += 9;
					flip = args->side;
					draw[burst->velX > 0 ? flip ^ 1 : flip](canvas,
						state->sheet + LobbedShellsShardPictures[picture] +
							LOBBED_SHELLS_SHARD_PICTURES,
						pos.x - LobbedShellsShardWidths[picture] / 2,
						pos.y - LobbedShellsShardHeights[picture] / 2,
						LobbedShellsShardWidths[picture],
						LobbedShellsShardHeights[picture]);
				}
				/* Smoke rises; shards fall. */
				if(LOBBED_SHELLS_SHATTER(mode) == 0)
					btParticleAdvanceWithGravity3D(burst, 62, toFIXED(1) / 32);
				else
					btParticleAdvanceWithGravity3D(burst, 62, -toFIXED(1) / 2);
				burst->variant++;
			}
		}
		btCameraShake(2, 2);
		EFFECT_END_TICK(state);
	}

	EFFECT_FINISH_CANVAS();
}

#undef LOBBED_SHELLS_BURSTS
#undef LOBBED_SHELLS_SLOTS
#undef LOBBED_SHELLS_BURST_SLOTS
#undef LOBBED_SHELLS_SMOKE_PICTURES
#undef LOBBED_SHELLS_SHATTER
#undef LOBBED_SHELL_WIDTH
#undef LOBBED_SHELL_HEIGHT
#undef LOBBED_SHELL_PICTURES
#undef LOBBED_SHELLS_SHARD_PICTURES
#undef LOBBED_SHELLS_SMOKE_PUFFS
#undef LOBBED_SHELLS_SHARDS
#undef LOBBED_SHELLS_BURST_TICKS
#undef LOBBED_SHELL_UPRIGHT_PICTURES
#undef LOBBED_SHELLS_TARGET_TICKS
#undef LOBBED_SHELL_FLIGHT_TICKS
#undef LOBBED_SHELLS_SMOKE_END_TICKS
#undef LOBBED_SHELLS_SHARDS_END_TICKS
