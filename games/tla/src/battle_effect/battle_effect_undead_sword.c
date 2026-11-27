/* Undead Sword, Plume Edge and Diamond Berg, and the bodies of Dreamtide and
   Watery Grave and of Waft and Aroma. Part of battle_effect.c. */

/* The water sheet's modes (btEffectRunWaterSheet). */
typedef enum WATER_SHEET_MODE {
	WATER_SHEET_DREAMTIDE,
	WATER_SHEET_WATERY_GRAVE
} WATER_SHEET_MODE;

/* The wind funnel's modes (btEffectRunWindFunnel). */
typedef enum WIND_FUNNEL_MODE {
	WIND_FUNNEL_WAFT,
	WIND_FUNNEL_AROMA
} WIND_FUNNEL_MODE;

/* The sparks, and the frames each lasts. */
#define UNDEAD_SWORD_SPARK_COUNT 16
#define UNDEAD_SWORD_SPARK_LIFE	 28
/* LimeColumnSheet: one column of light, 14 by 80. */
#define LIME_COLUMN_WIDTH		 14
#define LIME_COLUMN_HEIGHT		 80
/* Where BlastSheet is decoded, after the column. */
#define UNDEAD_SWORD_SPARK_SHEET PICTURE_BYTES(LIME_COLUMN)

#define UNDEAD_SWORD_FRAMES		54 /* frames of the columns */
#define UNDEAD_SWORD_HIT_FRAME	10 /* the targets are hit */
#define UNDEAD_SWORD_RISE_FRAME 8  /* the columns break the ground */
#define UNDEAD_SWORD_PEAK_FRAME 31 /* the columns stop rising */
/* The canvas line the columns stand on and the sparks start from. */
#define UNDEAD_SWORD_GROUND		112

/* Two mirrored columns of light rise at the first target's feet and sink again,
   scattering sparks that drift upward.

   Abilities: Undead Sword. */
void btEffectRunUndeadSword(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	void				*sprites;
	DRAW_RECTANGLE		 draw[2];
	SCREEN_POINT		 screenPosition;
	PARTICLE			*spark;
	PARTICLE			*particle;
	Sint32				 frame;
	Sint32				 i;
	Sint32				 height;
	Sint32				 mode;
	Sint32				 x;
	Uint8				*src;
	Sint32				 speed;
	Sint32				 angle;
	Sint32				 picture;
	Sint32				 size;

	state = Ram_HeapSlots->battleEff;
	canvas = Ram_HeapSlots->battleCanvas;
	sprites = Ram_HeapSlots->battleSheet;

	/* Open the canvas and load the pictures. */
	EFFECT_START_NARROW_CANVAS(0);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprites, 0, 0);
	btResourceLoadAndDecompress(RESOURCE_LIME_COLUMN_SHEET, state->sheet, 1, 1);
	btResourceLoadAndDecompress(
		RESOURCE_BLAST_SHEET, state->sheet + UNDEAD_SWORD_SPARK_SHEET, 1, 0);
	EFFECT_START_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

	/* Slide the canvas so that its middle is under the target. */
	btEffectPosProjectBattler(args->tgts[0], &screenPosition);
	REG_BG2X = (CANVAS_SIZE / 2 - screenPosition.x) << 8;

	/* The sparks start together at the columns' foot and fly apart, upward
	   faster than sideways. */
	for(i = 0, spark = state->particles; i != UNDEAD_SWORD_SPARK_COUNT; i++) {
		speed = (syRandom() & 0x1ff) + 128;
		angle = syRandom() & TRIG_ANGLE_MASK;
		spark->x = toFIXED(CANVAS_SIZE / 2);
		spark->y = toFIXED(UNDEAD_SWORD_GROUND);
		spark->velX = (sySin(angle) * speed) >> 8;
		spark->velY = (syCos(angle) * speed) >> 9;
		spark->variant = syRandom() & 7;
		spark++;
	}
	state->shakeFrm = 8;

	for(frame = 0; frame != UNDEAD_SWORD_FRAMES; frame++) {
		/* The hit: the damage shown, a shake, knock-back and a flash. */
		if(frame == UNDEAD_SWORD_HIT_FRAME) {
			state->shakeFrm = 8;
			EFFECT_SHOW_DAMAGE(SE_POWER_UP);
			btMotionApplyKnockbackStub(args->tgts[0], KNOCKBACK_NUDGE);
			btEffectTintBattler(
				args->tgts[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 8);
		}

		/* The columns: up 12 pixels a frame until frame 31, then down 6. */
		if(frame >= UNDEAD_SWORD_RISE_FRAME) {
			if(frame <= UNDEAD_SWORD_PEAK_FRAME)
				height = (frame - UNDEAD_SWORD_RISE_FRAME) * 12;
			else
				height = 272 - frame * 6;
			if(height > 0) {
				if(height > LIME_COLUMN_HEIGHT) {
					height = LIME_COLUMN_HEIGHT;
					mode = BLITTER_MAX;
				} else {
					mode = BLITTER_ADDITIVE;
				}
				/* The column, then its mirror image on its right. */
				for(i = 0, x = CANVAS_SIZE / 2 - LIME_COLUMN_WIDTH; i != 2;
					i++, x += LIME_COLUMN_WIDTH) {
					if(i == 0)
						EFFECT_LOAD_BLITTER(
							HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, mode);
					else
						EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
							EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, mode);
					(draw[0] = Ram_HeapSlots->ramCode)(canvas, state->sheet, x,
						UNDEAD_SWORD_GROUND - height, LIME_COLUMN_WIDTH,
						height);
					syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
				}
			}
		}

		/* The sparks: a new blast picture every third frame, seven in all; the
		   pairs leave one frame apart from frame 8. */
		btEffectBuildCanvasBlitters(args->side, draw);
		for(i = 0, particle = state->particles; i != UNDEAD_SWORD_SPARK_COUNT;
			i++) {
			if(frame >= i / 2 + UNDEAD_SWORD_RISE_FRAME &&
				particle->variant <= UNDEAD_SWORD_SPARK_LIFE) {
				Sint32 drawX;
				Sint32 drawY;

				picture = particle->variant / 3;
				drawX = FIXED_INTEGER_PART(particle->x);
				drawY = FIXED_INTEGER_PART(particle->y);
				if(picture > 6)
					picture = 6;
				src = state->sheet + UndeadSword_PuffPictures[picture] +
					UNDEAD_SWORD_SPARK_SHEET;
				size = UndeadSword_PuffSizes[picture];
				draw[0](canvas, src, drawX - size / 2, drawY - size / 2, size,
					size);
				particle->variant++;
				btParticleAdvanceWithGravity2D(particle, 62, -toFIXED(1) / 8);
			}
			particle++;
		}
		EFFECT_RELEASE_BLITTERS();

		if(frame < UNDEAD_SWORD_RISE_FRAME)
			btCameraShake(4, 4);
		else
			btCameraShake(16, 16);
		EFFECT_END_FRAME(state);
	}

	syCloseEvent(btEffectTransferCanvas);
	btEffectEndCanvasLayer();
}

/* Where FlashBurstSheet is decoded in the effect's pictures: its six pictures,
   in Mercury's colours, are a puff of foam bursting and fading. */
#define PLUME_EDGE_FOAM_PICTURES 8192

#define PLUME_EDGE_FRAMES 80

/* Where BillowTexture is decoded in the effect's pictures. */
#define PLUME_EDGE_BILLOW 18192

/* The puffs of foam drawn, one more each frame from frame 0, each for
   PLUME_EDGE_FOAM_FRAMES: its six pictures, three frames apiece. */
#define PLUME_EDGE_FOAM_PUFFS	32
#define PLUME_EDGE_FOAM_FRAMES	18
/* The geyser lifts the target this many frames, two pixels a frame. */
#define PLUME_EDGE_LIFT_FRAMES	24
/* The frame the geyser bursts into its drops; it is drawn until then. */
#define PLUME_EDGE_BURST_FRAME	32
/* The drops it bursts into, in PARTICLES from PLUME_EDGE_FIRST_DROP. */
#define PLUME_EDGE_FIRST_DROP	256
#define PLUME_EDGE_DROPS		256
/* The geyser's foot, below the target's place; it rises 8 pixels a frame. */
#define PLUME_EDGE_GEYSER_FOOT	8
#define PLUME_EDGE_GEYSER_SPEED 8

/* A geyser of water erupts under the target in billows of foam and lifts it;
   the user dashes in, and the geyser bursts into a spray of drops as the canvas
   flashes white.

   Abilities: Plume Edge. */
void btEffectRunPlumeEdge(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 frame;
	Uint8				*sheet;
	MOTION_OBJECT		*target;
	Sint32				 geyserY;
	DRAW_RECTANGLE		 draw[2];
	SCREEN_POINT		 position;
	PARTICLE			*particle;
	Sint32				 i;

	state = Ram_HeapSlots->battleEff;
	canvas = Ram_HeapSlots->battleCanvas;
	sheet = Ram_HeapSlots->battleSheet;
	btEffectStartCanvasLayer(0);
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	btEffectBuildCanvasBlitters(0, draw);
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	target = btObjectGetStub(args->tgts[0])->obj;
	geyserY = 0;
	/* DEAD: nothing here draws BillowTexture; the ROM keeps the call. */
	btResourceLoadAndDecompress(
		RESOURCE_BILLOW_TEXTURE, state->sheet + PLUME_EDGE_BILLOW, 0, 0);
	syWordsCopyEntry((void *)BG_PLTT,
		syResourceGetTableEntry(RESOURCE_MERCURY_DJINN_SMALL_SHEET),
		SHEET_PALETTE_BYTES);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sheet, 0, 0);
	btResourceLoadAndDecompress(
		RESOURCE_FIRE_PILLAR_SHEET_A, state->sheet, 1, 0);
	btResourceLoadAndDecompress(RESOURCE_FLASH_BURST_SHEET,
		state->sheet + PLUME_EDGE_FOAM_PICTURES, 1, 0);
	EFFECT_FREE_PARTICLES(i, PARTICLES, PARTICLE_POOL_COUNT);

	for(frame = 0; frame != PLUME_EDGE_FRAMES; frame++) {
		btEffectPosProjectBattler(args->tgts[0], &position);

		/* Frame 0: the puffs of foam are set to fan out upward from the target,
		   one more each frame, and the target takes its damage pose, its
		   gravity turned off. */
		if(frame == 0) {
			for(i = 0; i != ARRAY_COUNT(state->particles); i++) {
				Sint32 speed = (syRandom() & 0x3ff) + 32;
				Sint32 angle = (syRandom() & 0x7fff) - DEGtoANG(90);

				particle = &state->particles[i];
				particle->x = toFIXED(position.x / 2);
				particle->y = toFIXED(position.y);
				particle->velX = (sySin(angle) * speed) >> 8;
				particle->velY = -(syCos(angle) * speed * 2) >> 8;
				particle->variant = -i;
			}
			target->verticalMotionStrength = 0;
			target->y = 0;
			geyserY = position.y;
			btEffectTintBattler(args->tgts[0], TINT_KEEP, POSE_DAMAGE, 0, -1);
			state->shakeFrm = 4;
			sySndPlayStub(SE_CLATTER);
		}

		/* The burst: the geyser bursts into its drops, the target is knocked
		   back and flashes, and the whole canvas flashes white. */
		if(frame == PLUME_EDGE_BURST_FRAME) {
			for(i = 0; i != PLUME_EDGE_DROPS; i++) {
				Sint32 speed = (syRandom() & 0x3ff) + 32;
				Sint32 angle = syRandom();

				particle = &PARTICLES[PLUME_EDGE_FIRST_DROP + i];
				particle->x = toFIXED(position.x / 2);
				particle->y = INTtoFIXED(position.y - 16);
				particle->velX = (sySin(angle) * speed) >> 7;
				particle->velY = -(syCos(angle) * speed * 2) >> 7;
				particle->variant = (syRandom() & 7) + 48;
			}
			btMotionApplyKnockbackStub(args->tgts[0], KNOCKBACK_HURL);
			btEffectTintBattler(
				args->tgts[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 16);
			state->shakeFrm = 8;
			EFFECT_SHOW_DAMAGE(SE_CRASH);
			syWordsFillEntry(
				canvas, CANVAS_BYTES, CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
		}

		/* The geyser lifts the target while the user dashes in and leaps. */
		if(frame >= 0 && frame < PLUME_EDGE_LIFT_FRAMES)
			target->y += toFIXED(2);
		if(frame == 18)
			btEffectSetApproachMotion(args->user, args->tgts[0], 8, 80);
		if(frame == 21)
			btMotionApproachTargetStub(
				args->user, args->tgts[0], 12, toFIXED(8));

		/* The drops fall and shrink: a drop is drawn at its life / 6 pixels
		   wide, two pixels at the least. */
		particle = PARTICLES;
		for(i = 0; i != PLUME_EDGE_FIRST_DROP + PLUME_EDGE_DROPS;
			i++, particle++) {
			if(particle->variant >= 0) {
				Sint32 size = particle->variant / 6;

				if(size < 2)
					size = 2;
				particle->variant--;
				btParticleAdvanceWithGravity2D(particle, 60, toFIXED(1) / 8);
				EFFECT_DRAW_MOTE(draw[0], canvas, sheet,
					FIXED_INTEGER_PART(particle->x),
					FIXED_INTEGER_PART(particle->y), size);
			}
		}

		/* Each puff of foam billows out and fades through its six pictures. */
		for(i = 0; i != PLUME_EDGE_FOAM_PUFFS; i++) {
			particle = &state->particles[i];
			particle->variant++;
			if(particle->variant >= 0 &&
				particle->variant < PLUME_EDGE_FOAM_FRAMES) {
				Sint32 picture = particle->variant / 3;

				btParticleAdvanceWithGravity2D(particle, 60, -toFIXED(1) / 8);
				draw[0](canvas,
					state->sheet + PLUME_EDGE_FOAM_PICTURES +
						picture * PICTURE_BYTES(FLASH_BURST),
					FIXED_INTEGER_PART(particle->x) - FLASH_BURST_WIDTH / 2,
					FIXED_INTEGER_PART(particle->y) - FLASH_BURST_HEIGHT / 2,
					FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
			}
		}

		/* The geyser: its picture scrolls up from its foot, the part that has
		   risen drawn again below it. */
		if(frame >= 0 && frame < PLUME_EDGE_BURST_FRAME) {
			Sint32 scroll =
				(frame * PLUME_EDGE_GEYSER_SPEED) % FIRE_PILLAR_HEIGHT;

			draw[0](canvas, state->sheet,
				position.x / 2 - FIRE_PILLAR_WIDTH / 2,
				geyserY + PLUME_EDGE_GEYSER_FOOT - scroll - FIRE_PILLAR_HEIGHT,
				FIRE_PILLAR_WIDTH, FIRE_PILLAR_HEIGHT);
			draw[0](canvas, state->sheet,
				position.x / 2 - FIRE_PILLAR_WIDTH / 2,
				geyserY - scroll + PLUME_EDGE_GEYSER_FOOT, FIRE_PILLAR_WIDTH,
				scroll);
		}

		EFFECT_END_FRAME(state);
	}

	syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
	syCloseEvent(btEffectTransferCanvas);
	btEffectEndCanvasLayer();
}

/* Copies a width by height picture into a picture twice its size, mirrored into
   all four quadrants. */
void btPictureMirrorIntoQuadrants(
	Uint8 *src, Uint8 *dst, Sint32 width, Sint32 height)
{
	Sint32 x;
	Sint32 y;

	for(y = 0; y != height; y++) {
		for(x = 0; x != width; x++) {
			Uint8 c = src[y * width + x];

			dst[y * width * 2 + x] = c;
			dst[y * 2 * width + (width * 2 - 1 - x)] = c;
			dst[(height * 2 - 1 - y) * 2 * width + x] = c;
			dst[(height * 2 - 1 - y) * 2 * width + (width * 2 - 1 - x)] = c;
		}
	}
}

/* Abilities: Dreamtide. */
void btEffectRunDreamtide(BATTLE_EFFECT_PARM *args)
{
	btEffectRunWaterSheet(args, WATER_SHEET_DREAMTIDE);
}

/* Abilities: Watery Grave. */
void btEffectRunWateryGrave(BATTLE_EFFECT_PARM *args)
{
	btEffectRunWaterSheet(args, WATER_SHEET_WATERY_GRAVE);
}

/* A corner of the water sheet: x, y and depth, a byte each. */
typedef struct WATER_VERTEX {
	Sint32 x : 8;
	Sint32 y : 8;
	Sint32 z : 8;
} WATER_VERTEX;

/* The water's 16 by 8 vertices, at the head of the map cell buffer. */
#define WATER_VERTICES ((WATER_VERTEX *)Ram_MapMetatileBuffer)

/* One triangle of the water's mesh: three vertex indices, then a texture
   coordinate for each. */
typedef struct WATER_TRIANGLE {
	Uint32 vertexA : 8;
	Uint32 vertexB : 8;
	Uint32 vertexC : 8;
	Uint32 : 8;
	Uint32 u0 : 8;
	Uint32 v0 : 8;
	Uint32 u1 : 8;
	Uint32 v1 : 8;
	Uint32 u2 : 8;
	Uint32 v2 : 8;
} WATER_TRIANGLE;

/* The water's grid of quads, across and down. */
#define WATER_COLUMNS			 16
#define WATER_ROWS				 8
/* A quad of the grid spans this many texels across and down; the texture wraps
   every 64. */
#define WATER_QUAD_TEXELS_ACROSS 8
#define WATER_QUAD_TEXELS_DOWN	 16
#define WATER_QUADS				 ((WATER_ROWS - 1) * (WATER_COLUMNS - 1))
/* Where the water's mesh is built in the map cell buffer. */
#define WATER_MESH_OFFSET		 16384
/* The water's mesh as triangles, two to a quad. */
#define WATER_MESH \
	((WATER_TRIANGLE *)(Ram_MapMetatileBuffer + WATER_MESH_OFFSET))

/* Where SwirlTexture is decoded in the effect's pictures. */
#define WATER_SHEET_SWIRL 8192

/* Where SmokeSheet is decoded in the effect's pictures, and its six puffs. */
#define WATER_SHEET_SMOKE				12288
#define WATER_SHEET_SMOKE_PICTURE_COUNT 6

/* Where the 32x32 picture at the sprite sheet's start is mirrored into a 64x64
   one. */
#define WATER_SHEET_MIRRORED 1024

#define DREAMTIDE_FRAMES		 48
#define WATERY_GRAVE_FRAMES		 81
/* The camera turns WATER_SHEET_CAMERA_TURN a frame until its yaw reaches
   WATER_SHEET_CAMERA_LIMIT either way. */
#define WATER_SHEET_CAMERA_TURN	 128
#define WATER_SHEET_CAMERA_LIMIT 12000
/* The angle a wave's phase moves on from one column of the grid to the next,
   and the one each row's phase is the sine of a multiple of. */
#define WATER_WAVE_COLUMN_PHASE	 5000
#define WATER_WAVE_ROW_PHASE	 3553
/* Watery Grave's puffs of smoke, in state->particles; one is drawn only beyond
   WATERY_GRAVE_PUFF_DEPTH. */
#define WATERY_GRAVE_PUFFS		 20
#define WATERY_GRAVE_PUFF_DEPTH	 10
/* Watery Grave tilts its water towards the camera. */
#define WATERY_GRAVE_TILT		 4000
/* The water slides in from WATER_SHEET_START_X to WATER_SHEET_END_X, 8 pixels a
   frame. */
#define WATER_SHEET_START_X		 128
#define WATER_SHEET_END_X		 (-64)

/* The body of Dreamtide and Watery Grave: a sheet of water (a 16 by 8 grid of
   vertices, its mesh built here) rolls in under the targets in waves while the
   camera turns; Dreamtide strikes the first target with two splashes at frame
   17, Watery Grave strikes every target at frames 17 and 46 while puffs of
   smoke bob on the water. */
void btEffectRunWaterSheet(BATTLE_EFFECT_PARM *args, Sint32 mode)
{
	BATTLE_EFFECT_STATE *state;
	Sint32				 frame;
	/* LEFTOVER(frame): blit[1], the pair's idle second blitter, which the ROM's
	 * frame reserves and never touches; as one DRAW_RECTANGLE. */
	DRAW_RECTANGLE blit[2];
	void		  *canvas;
	BATTLE_CAMERA *camera;
	Sint16		   savedYaw;
	Sint32		   frames;
	SCREEN_POINT   screen;
	SCREEN_POINT   target;
	MODEL_TEXTURE  texture;
	RENDER_MODEL  *model;
	PARTICLE	  *puff;
	Sint32		   height;
	Sint32		   phase;
	Sint32		   row;
	Sint32		   column;
	Uint8		  *mesh;
	Sint32		   i;

	state = Ram_HeapSlots->battleEff;
	canvas = Ram_HeapSlots->battleCanvas;
	btEffectConfigureEffectDisp();
	btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	mesh = Ram_MapMetatileBuffer + WATER_MESH_OFFSET;
	if(mode == WATER_SHEET_DREAMTIDE) {
		REG_BLDALPHA = BLEND_ALPHA(16, 12);
	} else {
		REG_BLDALPHA = EFFECT_BLEND_ADD;
	}
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
	blit[0] = Ram_HeapSlots->ramCode;
	camera = Ram_HeapSlots->cameraState;
	savedYaw = camera->yaw;
	btResourceLoadAndDecompress(
		RESOURCE_CRACKED_TEXTURE_SHEET, state->sheet, 1, 1);
	btPictureMirrorIntoQuadrants(
		state->sheet, state->sheet + WATER_SHEET_MIRRORED, 32, 32);
	btResourceLoadAndDecompress(
		RESOURCE_SWIRL_TEXTURE, state->sheet + WATER_SHEET_SWIRL, 0, 0);
	if(mode == WATER_SHEET_WATERY_GRAVE) {
		Uint8 *pixel;

		btResourceLoadAndDecompress(
			RESOURCE_SMOKE_SHEET, state->sheet + WATER_SHEET_SMOKE, 1, 0);
		for(i = 0, pixel = state->sheet + WATER_SHEET_SMOKE;
			i != WATER_SHEET_SMOKE_PICTURE_COUNT * PICTURE_BYTES(SMOKE_PUFF);
			i++, pixel++) {
			if(*pixel != 0) {
				*pixel += 8;
				if(*pixel > CANVAS_LEVEL_BRIGHTEST) {
					*pixel = CANVAS_LEVEL_BRIGHTEST;
				}
			}
		}
		LOAD_BG_PALETTE(RESOURCE_CRYSTAL_SHEET);
	}

	/* The water's grid: 16 by 8 vertices, and a quad (two triangles) for each
	   cell but the last row and column. A row's first vertex index is also the
	   texture's v at its top, a row being as many texels down as it has
	   vertices. */
	for(row = 0; row != WATER_ROWS; row++) {
		Sint32 v = row * WATER_COLUMNS;
		/* FAKEMATCH(dead): the row's first vertex index with the row added and
		 * taken away again. */
		Sint32 q = v + row - row;

		for(column = 0; column != WATER_COLUMNS; column++) {
			Sint32 a = row * WATER_COLUMNS + column;
			/* FAKEMATCH(dead): the vertex's index with q added and v, its
			 * equal, taken away. */
			WATER_VERTEX *vertex =
				(WATER_VERTEX *)((Sint8 *)MapGridMetatileBuffer +
					(a + q - v) * 4);
			Sint32 u = column * WATER_QUAD_TEXELS_ACROSS;

			SET_POSITION(vertex, column * 11 - 82, 60, row * 24 - 84);
			if(row < WATER_ROWS - 1 && column < WATER_COLUMNS - 1) {
				WATER_TRIANGLE *quad = (WATER_TRIANGLE *)mesh +
					(row * (WATER_COLUMNS - 1) + column) * 2;
				Sint32 b = q + column + 1;
				Sint32 d = q + column + WATER_COLUMNS + 1;
				Sint32 c = q + column + WATER_COLUMNS;

				/* FAKEMATCH(dead): a store the frame count below overwrites. */
				frames = (column * 8 - 60) & 1;
				quad[0].u0 = u + WATER_QUAD_TEXELS_ACROSS - 1;
				quad[0].v0 = q;
				quad[0].u1 = u + WATER_QUAD_TEXELS_ACROSS - 1;
				quad[0].v1 = v + WATER_QUAD_TEXELS_DOWN - 1;
				quad[0].u2 = u;
				quad[0].v2 = v + WATER_QUAD_TEXELS_DOWN - 1;
				quad[1].u0 = u;
				quad[1].v0 = q;
				quad[1].u1 = u + WATER_QUAD_TEXELS_ACROSS - 1;
				quad[1].v1 = q;
				quad[1].u2 = u;
				quad[1].v2 = v + WATER_QUAD_TEXELS_DOWN - 1;
				quad[0].vertexA = b;
				quad[0].vertexB = d;
				quad[0].vertexC = c;
				quad[1].vertexA = a;
				quad[1].vertexB = b;
				quad[1].vertexC = c;
			}
		}
	}
	/* A triangle of three zero indices ends the mesh. */
	WATER_MESH[WATER_QUADS * 2].vertexA = 0;
	WATER_MESH[WATER_QUADS * 2].vertexB = 0;
	WATER_MESH[WATER_QUADS * 2].vertexC = 0;
	frames = DREAMTIDE_FRAMES;
	if(mode != WATER_SHEET_DREAMTIDE) {
		frames = WATERY_GRAVE_FRAMES;
	}

	for(frame = 0; frame != frames; frame++) {
		void  *vertices;
		Sint32 shade;
		Sint32 angle;

		if(frame == 0) {
			camera->yaw = savedYaw;
			for(i = 0; i != ARRAY_COUNT(state->particles); i++) {
				state->particles[i].variant = -1;
			}
		}
		if(args->side == EFFECT_SIDE_ENEMIES) {
			if(camera->yaw < WATER_SHEET_CAMERA_LIMIT) {
				camera->yaw += WATER_SHEET_CAMERA_TURN;
			}
		} else if(camera->yaw > -WATER_SHEET_CAMERA_LIMIT) {
			camera->yaw -= WATER_SHEET_CAMERA_TURN;
		}

		if(mode == WATER_SHEET_DREAMTIDE) {
			if(frame == 17) {
				EFFECT_SHOW_DAMAGE(SE_CLATTER);
				btEffectStartActorMotion(
					args->tgts[0], 1, 0, 0, toFIXED(8), 100);
				btEffectTintBattler(
					args->tgts[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 16);
				state->shakeFrm = 8;
			}
		} else {
			if(frame == 17) {
				sySndPlayStub(SE_CRASH);
				for(i = 0; i != args->tgtCnt; i++) {
					btMotionApplyKnockbackStub(args->tgts[i], KNOCKBACK_NUDGE);
					btEffectTintBattler(
						args->tgts[i], SPRITE_COLOR_WHITE, POSE_DAMAGE, i, 16);
				}
				state->shakeFrm = 8;
			}
			if(frame == 46) {
				EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
				for(i = 0; i != args->tgtCnt; i++) {
					btEffectStartActorMotion(
						args->tgts[i], 1, 0, 0, toFIXED(8), 100);
					btEffectTintBattler(
						args->tgts[i], SPRITE_COLOR_WHITE, POSE_DAMAGE, i, 16);
				}
				state->shakeFrm = 8;
			}
		}
		if(mode == WATER_SHEET_DREAMTIDE) {
			if((Uint32)(frame - 33) <= 14) {
				REG_BLDALPHA = BLEND_ALPHA(48 - frame, BLEND_WEIGHT_MAX);
			}
		} else if((Uint32)(frame - 65) <= 14) {
			REG_BLDALPHA = BLEND_ALPHA(80 - frame, BLEND_WEIGHT_MAX);
		}

		vertices = syHeapAllocEWRAM(1024);
		model = btRenderAllocateModelList(1);
		shade = 0;

		/* The waves rise, then ebb. */
		if(mode == WATER_SHEET_DREAMTIDE) {
			if(frame <= 31) {
				height = frame * 4;
				if(height > 64) {
					height = 64;
				}
			} else {
				height = 320 - frame * 8;
				if(height < 0) {
					height = 0;
				}
			}
		} else if(frame <= 63) {
			height = frame * 2;
			if(height > 64) {
				height = 64;
			}
		} else {
			height = 576 - frame * 8;
			if(height < 0) {
				height = 0;
			}
		}
		phase = frame * 2;
		row = 1;
		angle = frame * (TRIG_TURN / 32);
		for(; row != WATER_ROWS - 1; row++) {
			for(column = 0; column != WATER_COLUMNS; column++) {
				Sint32 amplitude = (16 - column) * height / 32;
				Sint32 y =
					(FIXEDtoINT(sySin(angle + column * WATER_WAVE_COLUMN_PHASE +
									sySin(row * WATER_WAVE_ROW_PHASE)) *
						amplitude)) -
					amplitude;

				WATER_VERTICES[row * WATER_COLUMNS + column].y = y + 64;
			}
		}
		/* A puff bobs at half the waves' height, with the row it rises in: the
		   puffs lie a quarter of a row apart from row 1. */
		i = 0;
		height /= 2;
		for(puff = state->particles; i != WATERY_GRAVE_PUFFS; i++, puff++) {
			if(puff->variant == 0) {
				puff->y = INTtoFIXED(
					(FIXEDtoINT(
						sySin(angle +
							sySin((i + 4) * (WATER_WAVE_ROW_PHASE / 4))) *
						height)) -
					height + 64);
			}
		}

		texture.logWidth = 6;
		texture.logHeight = 6;
		texture.pixels = state->sheet + WATER_SHEET_MIRRORED;
		syInitMatrix();
		if(mode != WATER_SHEET_DREAMTIDE) {
			syRotX(WATERY_GRAVE_TILT);
		}
		if(args->side == EFFECT_SIDE_ENEMIES) {
			syRotY(camera->yaw);
		} else {
			syRotY(camera->yaw + DEGtoANG(180));
		}
		{
			Sint32 shift = WATER_SHEET_START_X - frame * 8;

			if(shift < WATER_SHEET_END_X) {
				shift = WATER_SHEET_END_X;
			}
			syTranslate(toFIXED(shift), 0, 0);
		}
		model->kind = MODEL_SHADED_ADDITIVE;
		model->texture = &texture;
		model->mesh = mesh;
		model->vertices = vertices;
		model->shade = shade;
		model->scrollU = (frame + phase) & TEXTURE_SCROLL_MASK;
		model->scrollV = TEXTURE_SCROLL_MASK & ~frame;
		btRenderTransformVertices(
			MapGridMetatileBuffer, vertices, WATER_ROWS * WATER_COLUMNS);
		btRenderDrawModelList(model);

		if(frame == 0) {
			FIXED puffZ = -toFIXED(60);

			for(i = 0, puff = state->particles; i != WATERY_GRAVE_PUFFS;
				i++, puff++) {
				puff->variant = syRandom() % WATER_SHEET_SMOKE_PICTURE_COUNT;
				puff->z = puffZ;
				puff->x = -toFIXED(60);
				puff->velX = 0;
				puff->velY = 0;
				puff->velZ = 0;
				puffZ += toFIXED(6);
			}
		}
		if(mode == WATER_SHEET_WATERY_GRAVE) {
			for(i = 0; i != WATERY_GRAVE_PUFFS; i++) {
				PARTICLE *smoke = &state->particles[i];
				Sint32	  picture = smoke->variant;

				if(picture < 0) {
					continue;
				}
				if(btEffectPosProjectPoint(smoke, &screen) == 0 &&
					FIXED_INTEGER_PART(syScreenPointCoordinates(&screen)[2]) >
						WATERY_GRAVE_PUFF_DEPTH) {
					Sint32 x = FIXED_INTEGER_PART(
						syScreenPointCoordinates(&screen)[0]);
					Sint32 y;

					screen.x = x + 64;
					y = FIXED_INTEGER_PART(
						syScreenPointCoordinates(&screen)[1]);
					screen.y = y + 64;
					blit[0](canvas,
						state->sheet + WATER_SHEET_SMOKE +
							picture * PICTURE_BYTES(SMOKE_PUFF),
						x + 52, y + 40, SMOKE_PUFF_WIDTH, SMOKE_PUFF_HEIGHT);
					btParticleAdvanceWithGravity3D(smoke, 62, -toFIXED(1) / 8);
				}
				if(++smoke->variant == WATER_SHEET_SMOKE_PICTURE_COUNT) {
					smoke->variant = 0;
				}
			}
		} else {
			Sint32 t;

			btEffectPosProjectBattler(args->tgts[0], &target);
			texture.logWidth = 6;
			texture.logHeight = 6;
			model->mesh = BattleEffect_SquareMesh;
			model->vertices = vertices;
			model->scrollU = 0;
			model->scrollV = 0;
			for(i = 0; i != 2; i++) {
				t = frame - 16 - i * 4;
				if(t < 0) {
					continue;
				}
				texture.pixels = state->sheet + WATER_SHEET_SWIRL;
				if(t > 7) {
					shade = (8 - t) * 4;
				}
				model->shade = shade;
				if(shade <= MODEL_SHADE_INVISIBLE) {
					continue;
				}
				syInitMatrix();
				syTranslate(INTtoFIXED(target.x / 2 - 64),
					INTtoFIXED(target.y - 64), 0);
				syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1) / 2);
				syRotZ(i * (DEGtoANG(90)) + DEGtoANG(45));
				syRotX(TRIG_TURN / 16);
				syRotY(angle);
				syMatrixScaleUniform(t * (toFIXED(3) / 16));
				btRenderTransformVertices(
					BattleEffect_GroundSquareCorners, vertices, 4);
				btRenderDrawModelList(model);
			}
		}
		syHeapRelease(model);
		syHeapRelease(vertices);
		btCameraShake(8, 8);
		EFFECT_END_FRAME(state);
	}
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
	syCloseEvent(btEffectTransferCanvas);
	btEffectEndCanvasLayer();
}

/* Abilities: Waft. */
void btEffectRunWaft(BATTLE_EFFECT_PARM *args)
{
	btEffectRunWindFunnel(args, WIND_FUNNEL_WAFT);
}

/* Abilities: Aroma. */
void btEffectRunAroma(BATTLE_EFFECT_PARM *args)
{
	btEffectRunWindFunnel(args, WIND_FUNNEL_AROMA);
}

/* The funnel is a band of WIND_FUNNEL_SEGMENTS segments, each two corners
   WIND_FUNNEL_BAND apart in height, laid in the map cell buffer: the corners,
   then those btRenderTransformVertices makes of them, then the band's mesh. */
#define WIND_FUNNEL_SEGMENTS 33
#define WIND_FUNNEL_CORNERS	 (WIND_FUNNEL_SEGMENTS * 2)
#define WIND_FUNNEL_BAND	 24
/* The funnel's head goes round a circle of this radius. */
#define WIND_FUNNEL_RADIUS	 32
#define WIND_FUNNEL_VERTICES \
	(Ram_MapMetatileBuffer + WIND_FUNNEL_CORNERS * sizeof(MODEL_VERTEX))
#define WIND_FUNNEL_MESH \
	(WIND_FUNNEL_VERTICES + WIND_FUNNEL_CORNERS * sizeof(PROJECTED_VERTEX))
/* The band's quads, each 4 texels of StreakTexture across and its 32 down. */
#define WIND_FUNNEL_QUADS (WIND_FUNNEL_SEGMENTS - 1)

/* Where JupiterDjinnSmallSheet is decoded in the effect's pictures. */
#define WIND_FUNNEL_JUPITER_DJINN_SMALL_PICTURES 8192

/* How high the funnel's head stands, and how far it dips and rises again, an
   angle of WIND_FUNNEL_DIP_SPEED a frame. */
#define WAFT_FUNNEL_HEIGHT		 80
#define AROMA_FUNNEL_HEIGHT		 30
#define AROMA_FUNNEL_DIP		 20
#define WIND_FUNNEL_DIP_SPEED	 820
/* The funnel is drawn three times, each turned WIND_FUNNEL_SPREAD further. */
#define WIND_FUNNEL_SPREAD		 21000
/* Waft's funnels swell and shrink about WAFT_FUNNEL_SCALE, an angle of
   WAFT_FUNNEL_PULSE_SPEED a frame; Aroma's about full size. */
#define WAFT_FUNNEL_SCALE		 (toFIXED(2673) / 2048)
#define WAFT_FUNNEL_PULSE_SPEED	 964
#define AROMA_FUNNEL_PULSE_SPEED 682
/* Aroma's three funnels stand one above another, this far apart. */
#define AROMA_FUNNEL_SPACING	 16

/* The body of Waft and Aroma: after the Jupiter Djinni's entrance a small
   Djinni bobs and rises, while three funnels of wind, each a band whose head
   moves and whose tail follows where the head has been, twist over the targets
   and take on Waft's cyan or Aroma's tan; Waft strikes the first target at
   frame 28, Aroma touches each target in turn, four frames apart. */
void btEffectRunWindFunnel(BATTLE_EFFECT_PARM *args, Sint32 mode)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Uint8				*sprites;
	/* LEFTOVER(frame): blit[1], the pair's idle second blitter, which the ROM's
	 * frame reserves and never touches; as one DRAW_RECTANGLE. */
	DRAW_RECTANGLE blit[2];
	BATTLE_CAMERA *camera;
	SCREEN_POINT   other;
	SCREEN_POINT   screen;
	MODEL_TEXTURE  texture;
	RENDER_MODEL  *model;
	Sint32		   yaw;
	Sint32		   frame;
	Sint32		   frames;
	Sint32		   i;
	Sint32		   k;
	Uint8		  *vertices;
	Uint8		  *funnel;
	void (*transfer)(void);

	state = Ram_HeapSlots->battleEff;
	canvas = Ram_HeapSlots->battleCanvas;
	camera = Ram_HeapSlots->cameraState;
	sprites = Ram_HeapSlots->battleSheet;
	/* The funnel's corners, two to a segment, from its head. */
	funnel = MapGridMetatileBuffer;
	btEffectStartCanvasLayer(0);
	if(mode == WIND_FUNNEL_WAFT) {
		btEffectRunDjinniEntrance(args,
			DJINNI_ENTRANCE(
				ELEMENT_JUPITER, args->side, DJINNI_ANCHOR_OVER_TARGET),
			(Sint32 *)&screen, (Sint32 *)&other);
		REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
			BGCNT_SCREEN_BASE(7);
	} else {
		btEffectRunDjinniEntrance(args,
			DJINNI_ENTRANCE(
				ELEMENT_JUPITER, args->side, DJINNI_ANCHOR_TARGET_X),
			(Sint32 *)&screen, (Sint32 *)&other);
	}
	if(args->side == EFFECT_SIDE_ENEMIES) {
		EFFECT_LOAD_BLITTER(
			HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
	} else {
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
			EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
	}
	blit[0] = Ram_HeapSlots->ramCode;
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	transfer = btEffectTransferCanvas;
	sySetEvent(transfer, EVENT_PASS_VBLANK);
	yaw = 0; /* Default: set again at frame 0 before it is read. */
	frames = 36;
	if(mode == WIND_FUNNEL_AROMA && args->tgtCnt > 4) {
		frames = args->tgtCnt * 6 + 12;
	}
	btResourceLoadAndDecompress(RESOURCE_STREAK_TEXTURE, state->sheet, 0, 0);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprites, 0, 0);
	btResourceLoadAndDecompress(RESOURCE_JUPITER_DJINN_SMALL_SHEET,
		state->sheet + WIND_FUNNEL_JUPITER_DJINN_SMALL_PICTURES, 1, 1);
	btRenderBuildQuadStrip(WIND_FUNNEL_MESH, 4, 32, WIND_FUNNEL_QUADS);

	for(frame = 0; frame != frames; frame++) {
		Sint32		  shade;
		Sint32		  height;
		Sint32		  y;
		Sint32		  angle;
		MODEL_VERTEX *corner;
		Sint32		  x; /* The Djinni's x; at frame 0, the funnel's height. */

		if(frame == 0) {
			MODEL_VERTEX *head;
			Sint32		  row;
			Sint32		  radius;

			yaw = 0;
			/* FAKEMATCH(temporary): radius, WIND_FUNNEL_RADIUS held in a local
			 * set in both arms; as the constant at its use. */
			if(mode == WIND_FUNNEL_WAFT) {
				x = WAFT_FUNNEL_HEIGHT;
				radius = WIND_FUNNEL_RADIUS;
			} else {
				x = AROMA_FUNNEL_HEIGHT;
				radius = WIND_FUNNEL_RADIUS;
			}
			for(i = 0; i != WIND_FUNNEL_SEGMENTS; i++) {
				row = i * 8;
				for(k = 0; k != 2; k++) {
					head = (MODEL_VERTEX *)(MapGridMetatileBuffer + row) + k;
					head->y = -(x + k * WIND_FUNNEL_BAND);
					head->x = 0;
					head->z = radius;
				}
			}
			sySndPlayStub(SE_WIND_FUNNEL);
		}
		if(frame > 19) {
			if(mode == WIND_FUNNEL_WAFT) {
				btEffectStepPaletteToResource(RESOURCE_CYAN_PALETTE);
			} else if(mode == WIND_FUNNEL_AROMA) {
				btEffectStepPaletteToResource(RESOURCE_TAN_PALETTE);
			}
		}

		/* The Djinni, bobbing over the caster and rising from frame 8. */
		if(frame <= 79) {
			Sint32 y;

			x = (FIXEDtoINT(sySin(frame * (TRIG_TURN / 32)) * 8)) +
				screen.x / 2 - SMALL_DJINNI_WIDTH / 2;
			y = (FIXEDtoINT(syCos(frame * (TRIG_TURN / 32)) * 4)) + screen.y -
				24;

			if(frame > 7) {
				y -= (frame - 8) * 4;
			}
			blit[0](canvas,
				state->sheet + WIND_FUNNEL_JUPITER_DJINN_SMALL_PICTURES, x, y,
				SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
		}

		/* The funnel's tail follows its head: each segment takes the corners of
		   the one before, every frame but the even ones from 32 to 47. */
		if((frame & 1) || frame > 47 || frame <= 31) {
			MODEL_VERTEX *corner;
			Sint32		  i;
			for(i = WIND_FUNNEL_QUADS; i != 0; i--) {
				k = 0;
				corner = (MODEL_VERTEX *)MapGridMetatileBuffer + (i - 1) * 2;
				for(; k != 2; k++) {
					corner[2].x = corner[0].x;
					corner[2].y = corner[0].y;
					corner[2].z = corner[0].z;
					corner++;
				}
			}
		}
		/* The funnels turn a 32nd of a turn a frame, slowing from frame 24. */
		if(frame > 23) {
			yaw += TRIG_TURN / 32 - (frame - 24) * 16;
		} else {
			yaw += TRIG_TURN / 32;
		}
		if(mode == WIND_FUNNEL_WAFT) {
			angle = frame * (TRIG_TURN / 16);
			height = WAFT_FUNNEL_HEIGHT -
				(FIXEDtoINT(
					sySin(frame * WIND_FUNNEL_DIP_SPEED) * WAFT_FUNNEL_HEIGHT));
		} else {
			angle = yaw;
			height = AROMA_FUNNEL_HEIGHT -
				(FIXEDtoINT(
					sySin(frame * WIND_FUNNEL_DIP_SPEED) * AROMA_FUNNEL_DIP));
		}
		k = 0;
		corner = (MODEL_VERTEX *)funnel;
		y = -height;
		for(; k != 2; k++) {
			/* The (Uint32) casts make each division round down, as a shift. */
			corner->x =
				(Uint32)sySin(angle) / (toFIXED(1) / WIND_FUNNEL_RADIUS);
			corner->y = y;
			corner->z =
				(Uint32)syCos(angle) / (toFIXED(1) / WIND_FUNNEL_RADIUS);
			y -= WIND_FUNNEL_BAND;
			corner++;
		}

		if(mode == WIND_FUNNEL_WAFT) {
			if(frame == 28) {
				state->shakeFrm = 6;
				EFFECT_SHOW_DAMAGE(SE_IMPACT);
				btEffectTintBattler(
					args->tgts[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 8);
			}
		} else {
			if(frame == 12) {
				EFFECT_SHOW_DAMAGE(HIT_SOUND_NONE);
			}
			for(k = 0; k != args->tgtCnt; k++) {
				if(frame == k * 4 + 12) {
					sySndPlayStub(SE_RECOVERY);
					btEffectTintBattler(
						args->tgts[k], SPRITE_COLOR_WHITE, TINT_KEEP, k, 8);
				}
			}
		}

		syInitMatrix();
		syLookAtLoad(camera->eye, camera->focus);
		vertices = WIND_FUNNEL_VERTICES;
		model = btRenderAllocateModelList(1);
		shade = 0;
		if(frame <= 15) {
			shade = frame * 4 + MODEL_SHADE_INVISIBLE;
			if(shade > 0) {
				shade = 0;
			}
		} else if(frame > 27) {
			shade = (28 - frame) * 8;
			if(shade < MODEL_SHADE_INVISIBLE) {
				shade = MODEL_SHADE_INVISIBLE;
			}
		}
		texture.logWidth = 7;
		texture.logHeight = 5;
		texture.pixels = state->sheet;
		model->kind = MODEL_SHADED_MAX;
		model->texture = &texture;
		model->mesh = WIND_FUNNEL_MESH;
		model->scrollU = 0;
		model->scrollV = 0;
		ProjectionWork.centerY = 104;
		model->vertices = vertices;
		model->shade = shade;
		for(k = 0; k != WindFunnel_Counts[mode]; k++) {
			syInitMatrix();
			if(mode == WIND_FUNNEL_WAFT) {
				Sint32 scale;

				syTranslate(INTtoFIXED(screen.x / 2 - 64), toFIXED(40), 0);
				syRotX(k * (TRIG_TURN / 32));
				syRotY(k * WIND_FUNNEL_SPREAD + yaw);
				scale =
					WAFT_FUNNEL_SCALE - sySin(frame * WAFT_FUNNEL_PULSE_SPEED);
				/* From frame 28 the funnels widen as they fade. */
				if(frame > 27) {
					scale += (frame - 28) * (toFIXED(1) / 32);
				}
				syScale(scale * 3 / 2, toFIXED(1), scale * 3 / 2);
			} else {
				Sint32 scale;

				syTranslate(INTtoFIXED(screen.x / 2 - 64),
					toFIXED(k * AROMA_FUNNEL_SPACING) + toFIXED(16), 0);
				syRotX(k * (TRIG_TURN / 16) - TRIG_TURN / 32);
				syRotY(k * WIND_FUNNEL_SPREAD + yaw);
				scale =
					toFIXED(1) - sySin(frame * AROMA_FUNNEL_PULSE_SPEED) / 4;
				if(frame > 27) {
					scale += (frame - 28) * (toFIXED(1) / 32);
				}
				syScale(scale * 2, toFIXED(1), scale * 2);
			}
			btRenderTransformVertices(
				funnel, WIND_FUNNEL_VERTICES, WIND_FUNNEL_CORNERS);
			btRenderDrawModelList(model);
		}
		ProjectionWork.centerX = BATTLE_PROJECTION_CENTER_X;
		ProjectionWork.centerY = BATTLE_PROJECTION_CENTER_Y;
		syHeapRelease(model);
		btCameraShake(4, 4);
		EFFECT_END_FRAME(state);
	}
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
	syCloseEvent(transfer);
	btEffectEndCanvasLayer();
}

#define DIAMOND_BERG_FRAMES 148

/* Where CrystalBitmapSheet, the iceberg (48 by 104), is decoded in the map cell
   buffer. */
#define DIAMOND_BERG_CRYSTAL_BITMAP_PICTURES 20000
#define DIAMOND_BERG_WIDTH					 48
#define DIAMOND_BERG_HEIGHT					 104
/* The berg drops DIAMOND_BERG_DROP_SPEED lines a frame, then each of
   DIAMOND_BERG_BLOWS blows drives it DIAMOND_BERG_BLOW_DEPTH further, and the
   ground cuts it off at DIAMOND_BERG_GROUND. */
#define DIAMOND_BERG_DROP_SPEED				 16
#define DIAMOND_BERG_BLOWS					 8
#define DIAMOND_BERG_BLOW_DEPTH				 8
#define DIAMOND_BERG_GROUND					 108

/* Where BillowTexture is decoded in the map cell buffer, after the frost's
   FlameBandTexture. */
#define DIAMOND_BERG_BILLOW 4096

/* Where FireRingTexture is decoded in the effect's pictures. */
#define DIAMOND_BERG_FIRE_RING 12416

/* The shards: DIAMOND_BERG_CRASH_SHARDS when the berg lands, then
   DIAMOND_BERG_BLOW_SHARDS at each blow, in the slots after them; a shard
   bounces on DIAMOND_BERG_SHARD_GROUND and is drawn until it has spun
   DIAMOND_BERG_SHARD_SPIN. */
#define DIAMOND_BERG_CRASH_SHARDS		32
#define DIAMOND_BERG_BLOW_SHARDS		8
#define DIAMOND_BERG_SHARD_GROUND		104
#define DIAMOND_BERG_SHARD_SPIN			1024
/* The ripples of frost lean towards the camera, each starting at about a
   quarter of its size and growing by its row's growth times
   DIAMOND_BERG_RIPPLE_GROWTH a frame. */
#define DIAMOND_BERG_RIPPLE_TILT		3000
#define DIAMOND_BERG_RIPPLE_START_SCALE (toFIXED(131) / 512)
#define DIAMOND_BERG_RIPPLE_GROWTH		1400

/* An iceberg drops onto the target and is driven down in eight blows (every
   eighth frame from 31), each throwing eight shards and flaring a ring; the
   last shatters it in a flash. Ripples of frost spread beneath.

   Abilities: Diamond Berg. */
void btEffectRunDiamondBerg(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Uint8				*sprites;
	/* LEFTOVER(frame): blit[1], the pair's idle second blitter, which the ROM's
	 * frame reserves and never touches; as one DRAW_RECTANGLE. */
	DRAW_RECTANGLE blit[2];
	SCREEN_POINT   pos;
	SCREEN_POINT   target;
	MODEL_TEXTURE  texture;
	RENDER_MODEL  *model;
	Sint32		   centerX;
	Sint32		   top;
	Sint32		   frame;
	Sint32		   i;
	Sint32		   k;
	Uint8		  *cells;

	state = Ram_HeapSlots->battleEff;
	canvas = Ram_HeapSlots->battleCanvas;
	sprites = Ram_HeapSlots->battleSheet;
	btEffectStartCanvasLayer(0);
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	btEffectPosProjectBattler(args->tgts[0], &pos);
	centerX = pos.x / 2;
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
	blit[0] = Ram_HeapSlots->ramCode;
	btResourceLoadAndDecompress(RESOURCE_CRYSTAL_SHEET, state->sheet, 1, 0);
	cells = MapGridMetatileBuffer;
	btResourceLoadAndDecompress(RESOURCE_CRYSTAL_BITMAP_SHEET,
		cells + DIAMOND_BERG_CRYSTAL_BITMAP_PICTURES, 1, 1);
	btResourceLoadAndDecompress(RESOURCE_FLAME_BAND_TEXTURE, cells, 0, 0);
	/* DEAD: nothing draws BillowTexture, as the ripples read only the band's 16
	   rows before it; the ROM keeps the call. */
	btResourceLoadAndDecompress(
		RESOURCE_BILLOW_TEXTURE, cells + DIAMOND_BERG_BILLOW, 0, 0);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprites, 0, 0);
	btResourceLoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE,
		state->sheet + DIAMOND_BERG_FIRE_RING, 0, 0);
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	/* DEAD: frame 0 sets top again before anything reads it; the ROM keeps the
	   store. */
	top = -128;

	for(frame = 0; frame != DIAMOND_BERG_FRAMES; frame++) {
		if(frame == 0) {
			top = -DIAMOND_BERG_HEIGHT;
			for(i = 0; i != ARRAY_COUNT(state->particles); i++) {
				state->particles[i].variant = -1;
			}
		}
		if(frame <= 63) {
			btEffectPosProjectBattler(args->tgts[0], &target);
		}
		if(frame == 20) {
			btEffectSetApproachMotion(args->user, args->tgts[0], 8, 60);
		}
		if(frame == 2) {
			sySndPlayStub(SE_POWER_UP);
		}

		/* The berg: it drops, then each blow drives it further down. */
		if(frame <= 127) {
			Sint32 height = DIAMOND_BERG_HEIGHT;

			if(frame <= 8) {
				top += DIAMOND_BERG_DROP_SPEED;
			} else if((Uint32)(frame - 24) <= 64) {
				Sint32 t = frame - 24;

				if(t % 8 == 4) {
					sySndPlayStub(SE_POWER_UP);
				}
				if(t % 8 == 7) {
					state->shakeFrm = 4;
					if(frame <= 79) {
						sySndPlayStub(SE_CLATTER);
						btEffectSetApproachMotion(
							args->user, args->tgts[0], 16, 70);
					} else {
						btMotionApplyKnockbackStub(
							args->tgts[0], KNOCKBACK_HURL);
						EFFECT_SHOW_DAMAGE(SE_CLATTER);
						syWordsFillEntry(canvas, CANVAS_BYTES,
							CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
					}
					btEffectTintBattler(
						args->tgts[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 6);
					top += DIAMOND_BERG_BLOW_DEPTH;
					for(k = 0; k != DIAMOND_BERG_BLOW_SHARDS; k++) {
						PARTICLE *shard;

						shard = &state->particles
									 [((t - 7) / 8 * DIAMOND_BERG_BLOW_SHARDS +
										  k + DIAMOND_BERG_CRASH_SHARDS) &
										 (ARRAY_COUNT(state->particles) - 1)];
						shard->y = toFIXED(96);
						shard->x = toFIXED(centerX);
						shard->velX = ((syRandom() & 127) - 64) << 11;
						shard->velY = -((syRandom() & 127) + 64) << 11;
						shard->z = 32;
						shard->variant = 0;
					}
				}
			}
			/* The ground cuts off what of the berg is below it. */
			if(top + DIAMOND_BERG_HEIGHT > DIAMOND_BERG_GROUND) {
				height =
					height - top + (DIAMOND_BERG_GROUND - DIAMOND_BERG_HEIGHT);
			}
			if(height > 0) {
				blit[0](canvas,
					Ram_MapMetatileBuffer +
						DIAMOND_BERG_CRYSTAL_BITMAP_PICTURES,
					centerX - DIAMOND_BERG_WIDTH / 2, top, DIAMOND_BERG_WIDTH,
					height);
			}
		}
		if(frame == 7) {
			PARTICLE *shard;

			state->shakeFrm = 8;
			sySndPlayStub(SE_CRASH);
			for(k = 0, shard = state->particles; k != DIAMOND_BERG_CRASH_SHARDS;
				k++, shard++) {
				shard->y = toFIXED(96);
				shard->x = toFIXED(centerX);
				shard->velX = ((syRandom() & 127) - 64) << 12;
				shard->velY = -((syRandom() & 127) + 64) << 12;
				shard->z = 32;
				shard->variant = 0;
			}
		}

		/* The shards tumble and bounce; z is their spin. */
		if(frame > 6) {
			PARTICLE *shard;

			for(i = 0, shard = state->particles;
				i != ARRAY_COUNT(state->particles); i++, shard++) {
				Sint32 cell;
				Uint8 *pixels;
				Sint32 x;
				Sint32 y;
				Uint32 width;
				Uint32 height;

				if((Uint32)shard->variant >= DIAMOND_BERG_SHARD_SPIN) {
					continue;
				}
				cell = i % 5 * 3 + shard->variant / SHARD_TURN % 3;
				pixels = state->sheet + PRISM_SHARD_PICTURES +
					Prism_ShardPictures[cell];
				width = Prism_ShardWidths[cell];
				x = FIXED_INTEGER_PART(shard->x) - (width >> 1);
				y = FIXED_INTEGER_PART(shard->y);
				height = Prism_ShardHeights[cell];
				y -= height >> 1;
				blit[0](canvas, pixels, x, y, width, height);
				btParticleAdvanceWithGravity2D(
					shard, PARTICLE_DAMPING_NONE, toFIXED(1) / 4);
				if(FIXED_INTEGER_PART(shard->y) > DIAMOND_BERG_SHARD_GROUND) {
					shard->velY = -shard->velY / 2;
				}
				shard->variant += shard->z;
				if(shard->z > 1 && (frame & 1)) {
					shard->z--;
				}
			}
		}

		if(frame > 5) {
			void *vertices;

			vertices = syHeapAllocEWRAM(1024);
			model = btRenderAllocateModelList(1);
			texture.logWidth = 7;
			texture.logHeight = 7;
			texture.pixels = cells;
			model->kind = MODEL_SHADED_ADDITIVE;
			model->texture = &texture;
			model->flg = 0;
			model->mesh = BattleEffect_CylinderMesh;
			model->vertices = vertices;
			model->scrollU = 0;
			model->scrollV = 0;

			/* The frost ripples. */
			for(k = 0; k != 4; k++) {
				Sint32 start = DiamondBerg_RingStarts[k] + 6;
				Sint32 scale;
				Sint32 shade;

				if(frame <= start) {
					continue;
				}
				scale = (frame - start) * DiamondBerg_RingGrowth[k] *
						DIAMOND_BERG_RIPPLE_GROWTH +
					DIAMOND_BERG_RIPPLE_START_SCALE;
				shade = (start - frame) * 8 + 56;
				if(shade > 0) {
					shade = 0;
				}
				if(shade <= MODEL_SHADE_INVISIBLE) {
					continue;
				}
				model->shade = shade;
				syInitMatrix();
				syTranslate(INTtoFIXED(centerX - 64),
					toFIXED(DiamondBerg_RingHeights[k]), 0);
				syScale(scale, scale, scale);
				model->scrollU = ((frame + k * 5) & 7) << 4;
				syRotX(DIAMOND_BERG_RIPPLE_TILT);
				btRenderTransformVertices(
					BattleEffect_CylinderCorners, vertices, 32);
				btRenderDrawModelList(model);
			}

			/* A ring of light flares at each blow, at the target, faster at
			   each blow, and fades after four frames. */
			texture.logWidth = 7;
			texture.logHeight = 7;
			texture.pixels = state->sheet + DIAMOND_BERG_FIRE_RING;
			model->mesh = BattleEffect_LargeSquareMesh;
			model->scrollU = 0;
			model->scrollV = 0;
			for(k = 0; k != DIAMOND_BERG_BLOWS; k++) {
				Sint32 t = frame - 31 - k * 8;
				Sint32 scale;
				Sint32 shade;

				if(t < 0) {
					continue;
				}
				scale = (k * (toFIXED(1) / 32) + toFIXED(1) / 8) * t +
					toFIXED(1) / 2;
				shade = 0;
				if(t > 3) {
					shade = (4 - t) * 16;
				}
				model->shade = shade;
				if(shade <= MODEL_SHADE_INVISIBLE) {
					continue;
				}
				syInitMatrix();
				syTranslate(INTtoFIXED(target.x / 2 - 64),
					INTtoFIXED(target.y - 80), 0);
				syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1) / 2);
				syRotY(args->side * DEGtoANG(180) + DEGtoANG(180));
				syRotZ(DiamondBerg_ShardRolls[k]);
				syRotX(DiamondBerg_ShardPitches[k]);
				syMatrixScaleUniform(scale);
				btRenderTransformVertices(
					BattleEffect_GroundSquareCorners, vertices, 4);
				btRenderDrawModelList(model);
			}
			syHeapRelease(model);
			syHeapRelease(vertices);
		}
		btCameraShake(4, 4);
		EFFECT_END_FRAME(state);
	}
	EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#undef UNDEAD_SWORD_SPARK_COUNT
#undef UNDEAD_SWORD_SPARK_LIFE
#undef LIME_COLUMN_WIDTH
#undef LIME_COLUMN_HEIGHT
#undef UNDEAD_SWORD_SPARK_SHEET
#undef UNDEAD_SWORD_FRAMES
#undef UNDEAD_SWORD_HIT_FRAME
#undef UNDEAD_SWORD_RISE_FRAME
#undef UNDEAD_SWORD_PEAK_FRAME
#undef UNDEAD_SWORD_GROUND
#undef PLUME_EDGE_FOAM_PICTURES
#undef PLUME_EDGE_FRAMES
#undef PLUME_EDGE_BILLOW
#undef PLUME_EDGE_FOAM_PUFFS
#undef PLUME_EDGE_FOAM_FRAMES
#undef PLUME_EDGE_LIFT_FRAMES
#undef PLUME_EDGE_BURST_FRAME
#undef PLUME_EDGE_FIRST_DROP
#undef PLUME_EDGE_DROPS
#undef PLUME_EDGE_GEYSER_FOOT
#undef PLUME_EDGE_GEYSER_SPEED
#undef WATER_VERTICES
#undef WATER_COLUMNS
#undef WATER_ROWS
#undef WATER_QUAD_TEXELS_ACROSS
#undef WATER_QUAD_TEXELS_DOWN
#undef WATER_QUADS
#undef WATER_MESH_OFFSET
#undef WATER_MESH
#undef WATER_SHEET_SWIRL
#undef WATER_SHEET_SMOKE
#undef WATER_SHEET_SMOKE_PICTURE_COUNT
#undef WATER_SHEET_MIRRORED
#undef DREAMTIDE_FRAMES
#undef WATERY_GRAVE_FRAMES
#undef WATER_SHEET_CAMERA_TURN
#undef WATER_SHEET_CAMERA_LIMIT
#undef WATER_WAVE_COLUMN_PHASE
#undef WATER_WAVE_ROW_PHASE
#undef WATERY_GRAVE_PUFFS
#undef WATERY_GRAVE_PUFF_DEPTH
#undef WATERY_GRAVE_TILT
#undef WATER_SHEET_START_X
#undef WATER_SHEET_END_X
#undef WIND_FUNNEL_SEGMENTS
#undef WIND_FUNNEL_CORNERS
#undef WIND_FUNNEL_BAND
#undef WIND_FUNNEL_RADIUS
#undef WIND_FUNNEL_VERTICES
#undef WIND_FUNNEL_MESH
#undef WIND_FUNNEL_QUADS
#undef WIND_FUNNEL_JUPITER_DJINN_SMALL_PICTURES
#undef WAFT_FUNNEL_HEIGHT
#undef AROMA_FUNNEL_HEIGHT
#undef AROMA_FUNNEL_DIP
#undef WIND_FUNNEL_DIP_SPEED
#undef WIND_FUNNEL_SPREAD
#undef WAFT_FUNNEL_SCALE
#undef WAFT_FUNNEL_PULSE_SPEED
#undef AROMA_FUNNEL_PULSE_SPEED
#undef AROMA_FUNNEL_SPACING
#undef DIAMOND_BERG_FRAMES
#undef DIAMOND_BERG_CRYSTAL_BITMAP_PICTURES
#undef DIAMOND_BERG_WIDTH
#undef DIAMOND_BERG_HEIGHT
#undef DIAMOND_BERG_DROP_SPEED
#undef DIAMOND_BERG_BLOWS
#undef DIAMOND_BERG_BLOW_DEPTH
#undef DIAMOND_BERG_GROUND
#undef DIAMOND_BERG_BILLOW
#undef DIAMOND_BERG_FIRE_RING
#undef DIAMOND_BERG_CRASH_SHARDS
#undef DIAMOND_BERG_BLOW_SHARDS
#undef DIAMOND_BERG_SHARD_GROUND
#undef DIAMOND_BERG_SHARD_SPIN
#undef DIAMOND_BERG_RIPPLE_TILT
#undef DIAMOND_BERG_RIPPLE_START_SCALE
#undef DIAMOND_BERG_RIPPLE_GROWTH
