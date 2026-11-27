/* The canvas's usual pair of blitters; the beams of light of Revive, Dew and
   Spark; and the lobbed shells (Mad Spatter, Mud, Bone Chiller ...). Part of
   battle_effect.c. */

/* The lobbed shells' styles (btEffectRunLobbedShells), each named for the
   effect that plays it, and its row of LobbedShells_Styles. */
typedef enum LOBBED_SHELLS_MODE {
	LOBBED_SHELLS_BONE_CHILLER,	 /* blue, bursting into smoke */
	LOBBED_SHELLS_STICKY_GOO,	 /* the spider web's colours, smoke */
	LOBBED_SHELLS_STICKY_POISON, /* lime, smoke */
	LOBBED_SHELLS_ROTTEN_BLOOD,	 /* green, shards */
	LOBBED_SHELLS_MAD_SPATTER,	 /* gold, shards */
	LOBBED_SHELLS_DECOMPOSE,	 /* green, smoke */
	LOBBED_SHELLS_POISON_INK,	 /* Mars Djinn colours, smoke */
	LOBBED_SHELLS_MUD,			 /* gold, four to a target, shards */
	LOBBED_SHELLS_RISING_VENOM,	 /* lime and glowing, smoke */
	LOBBED_SHELLS_FORMIC_ACID	 /* pink, smoke */
} LOBBED_SHELLS_MODE;

/* Builds the two usual canvas blitters, clipped and mirrored when flip is
   nonzero: out[0] keeps the brighter level, out[1] adds the levels. */
void btEffectBuildCanvasBlitters(Sint32 flip, DRAW_RECTANGLE *out)
{
	if(flip == 0) {
		EFFECT_LOAD_BLITTER(
			HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
		out[0] = Ram_HeapSlots->ramCode;
		EFFECT_LOAD_BLITTER(
			HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
		out[1] = Ram_HeapSlots->ramCodePaired;
	} else {
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
			EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
		out[0] = Ram_HeapSlots->ramCode;
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
			EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_ADDITIVE);
		out[1] = Ram_HeapSlots->ramCodePaired;
	}
}

/* Motes drift down over each target and a column of light falls on it, in the
   Venus Djinni's colours; with a prelude, a Venus Djinni appears first.

   Abilities: Revive, Water of Life, Lich; Quartz. */
void btEffectRunRevive(BATTLE_EFFECT_PARM *args)
{
	btEffectRunDjinnBeams(args, ELEMENT_VENUS);
}

/* Dew plays Revive's beams of light in the Mercury Djinni's colours, and with a
   prelude a Mercury Djinni appears first.

   Abilities: Dew. */
void btEffectRunDew(BATTLE_EFFECT_PARM *args)
{
	btEffectRunDjinnBeams(args, ELEMENT_MERCURY);
}

/* Spark plays Revive's beams of light in the Mars Djinni's colours, and with a
   prelude a Mars Djinni appears first.

   Abilities: Spark. */
void btEffectRunSpark(BATTLE_EFFECT_PARM *args)
{
	btEffectRunDjinnBeams(args, ELEMENT_MARS);
}

#define DJINN_BEAMS_MOTES		  512 /* motes in PARTICLES */
/* Each target's own run of the motes, of which the first eight are drawn. */
#define DJINN_BEAMS_TARGET_MOTES  64
#define DJINN_BEAMS_MOTES_DRAWN	  8
/* Each target's motes and column start this many frames after the last's. */
#define DJINN_BEAMS_TARGET_FRAMES 16
/* How far a mote sways either side, and the Djinni across and up and down, in
   pixels. */
#define DJINN_BEAMS_MOTE_SWAY	  16
#define DJINN_BEAMS_DJINNI_SWAY_X 20
#define DJINN_BEAMS_DJINNI_SWAY_Y 4

/* The motes (CrescentMoonSheet), then the Djinni, after the column's pictures
   (EarthWallSheet). */
#define DJINN_BEAMS_MOTE_PICTURES	   10000
#define DJINN_BEAMS_MOTE_WIDTH		   24
#define DJINN_BEAMS_MOTE_HEIGHT		   24
#define DJINN_BEAMS_DJINNI_PICTURE	   26048
/* The column of light is one row of EarthWallSheet's 6x1 strip, bright in the
   middle and dimming to orange at its edge, after the 5x1 strip at the sprite
   sheet's start; each half of the column draws it, the second mirrored. */
#define DJINN_BEAMS_COLUMN_STRIP	   5
#define DJINN_BEAMS_COLUMN_STRIP_WIDTH 6

/* When there is a prelude the Djinni of the element appears, sways and rises
   away; then over each target in turn, sixteen frames apart, motes drift down
   swaying, and from frame 72 a column of light falls on it, flashing it, and
   narrows from frame 88. */
void btEffectRunDjinnBeams(BATTLE_EFFECT_PARM *args, Sint32 element)
{
	Sint32				 djinn[3];
	Sint32				 djinnSize[3];
	SCREEN_POINT		 one;
	SCREEN_POINT		 screenPosition;
	Sint32				 ground[3];
	Sint32				 point[3];
	DRAW_RECTANGLE		 draw[2];
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 frame;
	Sint32				 offsetX;
	Sint32				 i;
	Sint32				 k;
	HEAP_SLOTS			*heap = Ram_HeapSlots;

	state = Ram_HeapSlots->battleEff;
	canvas = Ram_HeapSlots->battleCanvas;

	btEffectStartCanvasLayer(0);
	if(args->prelude == 1)
		btEffectRunDjinniEntrance(args,
			DJINNI_ENTRANCE(
				element, args->side ^ args->prelude, DJINNI_ANCHOR_OVER_TARGET),
			djinn, djinnSize);
	btResourceLoadAndDecompress(RESOURCE_EARTH_WALL_SHEET, state->sheet, 1, 1);
	btResourceLoadAndDecompress(RESOURCE_CRESCENT_MOON_SHEET,
		state->sheet + DJINN_BEAMS_MOTE_PICTURES, 1, 1);
	if(element == ELEMENT_VENUS)
		btResourceLoadAndDecompress(RESOURCE_VENUS_DJINN_SHEET,
			state->sheet + DJINN_BEAMS_DJINNI_PICTURE, 1, 1);
	else if(element == ELEMENT_MERCURY)
		btResourceLoadAndDecompress(RESOURCE_MERCURY_DJINN_SHEET,
			state->sheet + DJINN_BEAMS_DJINNI_PICTURE, 1, 1);
	else
		btResourceLoadAndDecompress(RESOURCE_MARS_DJINN_SHEET,
			state->sheet + DJINN_BEAMS_DJINNI_PICTURE, 1, 1);

	/* The motes start 80 pixels up, each at its own phase of the sway. */
	for(i = 0; i != DJINN_BEAMS_MOTES; i++) {
		SET_POSITION(&PARTICLES[i], 0, toFIXED(80), (syRandom() | -32) << 14);
		PARTICLES[i].variant = syRandom() & 255;
	}

	/* With one target, BG2 is shifted so that it stands in the canvas's
	   middle. */
	REG_BG2PA = Q_8_8(1);
	if(args->tgtCnt == 1) {
		btEffectPosProjectScaledBattler(args->tgts[0], &one);
		offsetX = CANVAS_SIZE / 2 - one.x;
	} else {
		if(args->side == EFFECT_SIDE_PARTY)
			offsetX = -NARROW_CANVAS_PARTY_X;
		else
			offsetX = 0;
	}
	REG_BG2X = offsetX << 8;
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

	for(frame = 0; frame != args->tgtCnt * DJINN_BEAMS_TARGET_FRAMES + 116;
		frame++) {
		BATTLE_CAMERA *camera = Ram_HeapSlots->cameraState;

		if(frame == 64)
			sySndPlayStub(SE_POWER_UP);
		if(frame == 80)
			EFFECT_SHOW_DAMAGE(0);

		/* The Djinni sways, then rises away from frame 32. */
		if(args->prelude == 1) {
			Sint32 angle = frame * (TRIG_TURN / 32);
			Sint32 x = FIXEDtoINT(-sySin(angle) * DJINN_BEAMS_DJINNI_SWAY_X) +
				djinn[0] + offsetX - DJINNI_WIDTH / 2;
			Sint32 y = FIXEDtoINT(syCos(angle) * DJINN_BEAMS_DJINNI_SWAY_Y) +
				djinn[1] - 24;

			btEffectBuildCanvasBlitters(args->side ^ 1, draw);
			if(frame > 32)
				y = y - frame * 2 + 64;
			draw[1](canvas, state->sheet + DJINN_BEAMS_DJINNI_PICTURE, x, y,
				DJINNI_WIDTH, DJINNI_HEIGHT);
			/* Added twice in the first frames, it flashes as it appears. */
			if(frame <= 3)
				draw[1](canvas, state->sheet + DJINN_BEAMS_DJINNI_PICTURE, x, y,
					DJINNI_WIDTH, DJINNI_HEIGHT);
			EFFECT_RELEASE_BLITTERS();
		}
		EFFECT_LOAD_BLITTER(
			HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
		draw[0] = Ram_HeapSlots->ramCode;
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
			EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
		draw[1] = heap->ramCodePaired;

		/* A mote every eight frames drifts down a pixel a frame, swaying, until
		   it is 8 pixels from the ground. */
		EFFECT_FOR_EACH_TARGET(k, args) {
			MOTION_OBJECT *target = btObjectGetStub(args->tgts[k])->obj;
			Sint32		   start = k * DJINN_BEAMS_TARGET_FRAMES;

			EFFECT_VIEW_FROM_CAMERA(camera);
			SET_VECTOR(point, target->x, 0, target->z);
			syMatrixTranslateVector(point);
			if(frame > start) {
				for(i = 0; i != DJINN_BEAMS_MOTES_DRAWN; i++) {
					PARTICLE *mote =
						&PARTICLES[k * DJINN_BEAMS_TARGET_MOTES + i];

					if(frame > i * 8 + start && mote->y > toFIXED(8)) {
						Sint32 sway;

						btEffectPosProjectPoint(mote, &screenPosition);
						screenPosition.x += offsetX;
						sway =
							FIXEDtoINT(sySin(mote->variant * (TRIG_TURN / 64)) *
								DJINN_BEAMS_MOTE_SWAY);
						if(i & 1)
							screenPosition.x -= sway;
						else
							screenPosition.x += sway;
						draw[i & 1](canvas,
							state->sheet +
								DjinnBeams_MotePictures[mote->variant / 8 & 7] *
									PICTURE_BYTES(DJINN_BEAMS_MOTE) +
								DJINN_BEAMS_MOTE_PICTURES,
							screenPosition.x - DJINN_BEAMS_MOTE_WIDTH / 2,
							screenPosition.y - DJINN_BEAMS_MOTE_HEIGHT / 2,
							DJINN_BEAMS_MOTE_WIDTH, DJINN_BEAMS_MOTE_HEIGHT);
						mote->y -= toFIXED(1);
						mote->variant++;
					}
				}
			}
		}

		/* The column of light: from the top of the screen to the target's feet,
		   drawn in two mirrored halves, six pixels wide until frame 88, then
		   narrowing a pixel every three frames; its glow flickers at the
		   feet. */
		EFFECT_FOR_EACH_TARGET(i, args) {
			Sint32 start = i * DJINN_BEAMS_TARGET_FRAMES;

			if(frame >= start + 72) {
				MOTION_OBJECT *target = btObjectGetStub(args->tgts[i])->obj;

				EFFECT_VIEW_FROM_CAMERA(camera);
				if(frame == start + 72)
					btEffectTintBattler(args->tgts[i], SPRITE_COLOR_GRB,
						TINT_KEEP, TINT_KEEP, 0);
				if(frame == start + 88)
					btEffectTintBattler(args->tgts[i], SPRITE_COLOR_NORMAL,
						TINT_KEEP, TINT_KEEP, 0);
				SET_VECTOR(ground, target->x, 0, target->z);
				btEffectPosProjectPoint(ground, &screenPosition);
				screenPosition.x += offsetX;
				if(frame < start + 104) {
					Sint32 pic = frame / 4;
					Sint32 width = DJINN_BEAMS_COLUMN_STRIP_WIDTH;

					if(frame > start + 88)
						width = DJINN_BEAMS_COLUMN_STRIP_WIDTH -
							(frame - start - 88) / 3;
					if(pic > 2)
						pic = (pic & 1) + 1;
					if(frame < start + 100) {
						draw[0](canvas,
							state->sheet + DjinnBeams_HeadPictures[pic],
							screenPosition.x - DjinnBeams_HeadWidths[pic],
							screenPosition.y - DjinnBeams_HeadHeights[pic] + 8,
							DjinnBeams_HeadWidths[pic],
							DjinnBeams_HeadHeights[pic]);
						draw[1](canvas,
							state->sheet + DjinnBeams_HeadPictures[pic],
							screenPosition.x,
							screenPosition.y - DjinnBeams_HeadHeights[pic] + 8,
							DjinnBeams_HeadWidths[pic],
							DjinnBeams_HeadHeights[pic]);
					}
					/* Every row of the column, from the top down to the feet,
					   is the same strip. */
					for(k = 0; k != screenPosition.y; k++) {
						draw[0](canvas, state->sheet + DJINN_BEAMS_COLUMN_STRIP,
							screenPosition.x - width, k, width, 1);
						draw[1](canvas, state->sheet + DJINN_BEAMS_COLUMN_STRIP,
							screenPosition.x, k, width, 1);
					}
				}
			}
		}
		EFFECT_RELEASE_BLITTERS();
		EFFECT_PRESENT_CANVAS(state);
	}

	syCloseEvent(btEffectTransferCanvas);
	btEffectEndCanvasLayer();
}

/* Gold shells arc from the user onto each target in turn and burst into shards.

   Abilities: Mad Spatter. */
void btEffectRunMadSpatter(BATTLE_EFFECT_PARM *args)
{
	btEffectRunLobbedShells(args, LOBBED_SHELLS_MAD_SPATTER);
}

/* A Venus Djinni appears, then four gold shells arc onto each target and burst
   into shards.

   Abilities: Mud. */
void btEffectRunMud(BATTLE_EFFECT_PARM *args)
{
	btEffectRunLobbedShells(args, LOBBED_SHELLS_MUD);
}

/* Green shells arc from the user onto each target in turn and burst into smoke,
   then Dull's effect plays.

   Abilities: Decompose. */
void btEffectRunDecompose(BATTLE_EFFECT_PARM *args)
{
	btEffectRunLobbedShells(args, LOBBED_SHELLS_DECOMPOSE);
}

/* Blue shells arc from the user onto each target in turn and burst into smoke.

   Abilities: Bone Chiller. */
void btEffectRunBoneChiller(BATTLE_EFFECT_PARM *args)
{
	btEffectRunLobbedShells(args, LOBBED_SHELLS_BONE_CHILLER);
}

/* Shells in the spider web's colours arc from the user onto each target in turn
   and burst into smoke.

   Abilities: Sticky Goo. */
void btEffectRunStickyGoo(BATTLE_EFFECT_PARM *args)
{
	btEffectRunLobbedShells(args, LOBBED_SHELLS_STICKY_GOO);
}

/* Shells in Mars Djinn colours arc from the user onto each target in turn and
   burst into smoke.

   Abilities: Poison Ink. */
void btEffectRunPoisonInk(BATTLE_EFFECT_PARM *args)
{
	btEffectRunLobbedShells(args, LOBBED_SHELLS_POISON_INK);
}

/* Lime shells arc from the user onto each target in turn and burst into smoke.

   Abilities: Sticky Poison. */
void btEffectRunStickyPoison(BATTLE_EFFECT_PARM *args)
{
	btEffectRunLobbedShells(args, LOBBED_SHELLS_STICKY_POISON);
}

/* Green shells arc from the user onto each target in turn and burst into
   shards, then Impair's effect plays.

   Abilities: Rotten Blood. */
void btEffectRunRottenBlood(BATTLE_EFFECT_PARM *args)
{
	btEffectRunLobbedShells(args, LOBBED_SHELLS_ROTTEN_BLOOD);
}

/* Glowing lime shells arc from the user onto each target in turn and burst into
   smoke.

   Abilities: Rising Venom. */
void btEffectRunRisingVenom(BATTLE_EFFECT_PARM *args)
{
	btEffectRunLobbedShells(args, LOBBED_SHELLS_RISING_VENOM);
}

/* Pink shells arc from the user onto each target in turn and burst into smoke.

   Abilities: Formic Acid. */
void btEffectRunFormicAcid(BATTLE_EFFECT_PARM *args)
{
	btEffectRunLobbedShells(args, LOBBED_SHELLS_FORMIC_ACID);
}

/* The bursts, after the shells in PARTICLES: LOBBED_SHELLS_PIECE_SLOTS for each
   shell, LOBBED_SHELLS_TARGET_SHELLS shells' worth for each target. */
#define LOBBED_SHELLS_BURSTS		(PARTICLES + 256)
#define LOBBED_SHELLS_PIECE_SLOTS	8
#define LOBBED_SHELLS_TARGET_SHELLS 4 /* Mud's shells for each target */
/* Piece `n` of the burst of target `target`'s shell `shell`. */
#define LOBBED_SHELLS_PIECE(target, shell, n) \
	LOBBED_SHELLS_BURSTS[((target) * LOBBED_SHELLS_TARGET_SHELLS + (shell)) * \
			LOBBED_SHELLS_PIECE_SLOTS + \
		(n)]
#define LOBBED_SHELLS_SLOTS		  16  /* particles reserved for each target */
#define LOBBED_SHELLS_BURST_SLOTS 256 /* burst particles after the shells */
#define LOBBED_SHELLS_SMOKE_PICTURES \
	16000 /* the smoke pictures in the sprite sheet */
/* GoldShellSheet's shell as it tumbles, upright and then turned on its side. */
#define LOBBED_SHELL_WIDTH	  24
#define LOBBED_SHELL_HEIGHT	  32
/* The shell's tumbling pictures, and the shards' pictures after them. */
#define LOBBED_SHELL_PICTURES 10
#define LOBBED_SHELLS_SHARD_PICTURES \
	(LOBBED_SHELL_PICTURES * PICTURE_BYTES(LOBBED_SHELL))
/* The pieces a shell bursts into: puffs of smoke, or shards. */
#define LOBBED_SHELLS_SMOKE_PUFFS  4
#define LOBBED_SHELLS_SHARDS	   16
/* The frames a burst's piece is drawn for. */
#define LOBBED_SHELLS_BURST_FRAMES 45
/* A shard tumbles through nine pictures, five frames each: the large shard's,
   or for every second shard the small shard's, which follow them. */
#define LOBBED_SHELLS_SMALL_SHARD  9

/* A style's record in LobbedShells_Styles: nonzero when its shells burst into
   shards instead of smoke, the blitter its shells are drawn with (0 keeps the
   brighter level, 1 adds), its shells for each target, and the frames the
   effect runs past the targets' share. */
#define LOBBED_SHELLS_SHATTER(mode)		  LobbedShells_Styles[(mode) * 4]
#define LOBBED_SHELLS_SHELL_BLITTER(mode) LobbedShells_Styles[(mode) * 4 + 1]
#define LOBBED_SHELLS_SHELLS(mode)		  LobbedShells_Styles[(mode) * 4 + 2]
#define LOBBED_SHELLS_FRAMES(mode)		  LobbedShells_Styles[(mode) * 4 + 3]

/* Lobs shells from the user at each target in turn, eight frames apart, one
   each or Mud's four; they tumble as they fly, and when one lands the target
   flashes and the shell bursts into smoke that drifts up or shards that fly up
   and fall. Mud's Venus Djinni appears first; Rotten Blood and Decompose go on
   into Impair's and Dull's effects. */
void btEffectRunLobbedShells(BATTLE_EFFECT_PARM *args, Sint32 mode)
{
	SCREEN_POINT		 position;
	Sint32				 preludeSize[3];
	Sint32				 preludeOrigin[3];
	DRAW_RECTANGLE		 draw[2];
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 frame;
	Sint32				 i;
	Sint32				 k;
	Sint32				 total;
	MOTION_OBJECT		*source;
	BATTLE_CAMERA		*camera;
	Uint8				*palette;
	FIXED				 height;

	state = Ram_HeapSlots->battleEff;
	canvas = Ram_HeapSlots->battleCanvas;
	camera = Ram_HeapSlots->cameraState;

	if(mode == LOBBED_SHELLS_MUD) {
		btEffectStartCanvasLayer(0);
		btEffectRunDjinniEntrance(args,
			DJINNI_ENTRANCE(ELEMENT_VENUS + DJINNI_ENTRANCE_LONG, args->side,
				DJINNI_ANCHOR_OVER_USER),
			preludeOrigin, preludeSize);
		REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
			BGCNT_SCREEN_BASE(7);
	} else {
		btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
		REG_BLDALPHA = EFFECT_BLEND_ADD;
	}
	btResourceLoadAndDecompress(RESOURCE_GOLD_SHELL_SHEET, state->sheet, 1, 1);
	btResourceLoadAndDecompress(RESOURCE_SMOKE_SHEET,
		state->sheet + LOBBED_SHELLS_SMOKE_PICTURES, 1, 1);
	if(mode == LOBBED_SHELLS_BONE_CHILLER)
		palette = syResourceGetTableEntry(RESOURCE_BLUE_PALETTE);
	else if(mode == LOBBED_SHELLS_STICKY_GOO)
		palette = syResourceGetTableEntry(RESOURCE_SPIDER_WEB_SHEET);
	else if(mode == LOBBED_SHELLS_STICKY_POISON)
		palette = syResourceGetTableEntry(RESOURCE_LIME_PALETTE);
	else if(mode == LOBBED_SHELLS_RISING_VENOM)
		palette = syResourceGetTableEntry(RESOURCE_LIME_PALETTE);
	else if(mode == LOBBED_SHELLS_ROTTEN_BLOOD)
		palette = syResourceGetTableEntry(RESOURCE_GREEN_PALETTE);
	else if(mode == LOBBED_SHELLS_MAD_SPATTER || mode == LOBBED_SHELLS_MUD)
		palette = syResourceGetTableEntry(RESOURCE_GOLD_SHELL_SHEET);
	else if(mode == LOBBED_SHELLS_POISON_INK)
		palette = syResourceGetTableEntry(RESOURCE_MARS_DJINN_SHEET);
	else if(mode == LOBBED_SHELLS_FORMIC_ACID)
		palette = syResourceGetTableEntry(RESOURCE_PINK_BURST_SHEET);
	else
		palette = syResourceGetTableEntry(RESOURCE_GREEN_PALETTE);
	syWordsCopyEntry((void *)BG_PLTT, palette, SHEET_PALETTE_BYTES);

	/* The shells leave the user at the style's height, each aimed at its target
	   with a little spread in depth. */
	EFFECT_FREE_PARTICLES(i, PARTICLES, PARTICLE_POOL_COUNT);
	source = btObjectGetStub(args->user)->obj;
	if(mode == LOBBED_SHELLS_MUD)
		height = toFIXED(40);
	else if(mode == LOBBED_SHELLS_RISING_VENOM)
		height = toFIXED(20);
	else
		height = toFIXED(4);
	EFFECT_FOR_EACH_TARGET(k, args) {
		MOTION_OBJECT *target =
			btObjectGetStub(args->tgts[k % args->tgtCnt])->obj;

		for(i = 0; i != LOBBED_SHELLS_SLOTS; i++) {
			PARTICLE *shot = &PARTICLES[k * LOBBED_SHELLS_SLOTS + i];
			FIXED	  aimZ;

			SET_POSITION(shot, source->x, height, source->z);
			aimZ = target->z + toFIXED(((syRandom() & 31) - 16) * 2);
			SET_VELOCITY(shot, (target->x - source->x) >> 4, toFIXED(4),
				(aimZ - source->z) >> 4);
			shot->variant = 0;
		}
	}
	EFFECT_FREE_PARTICLES(i, LOBBED_SHELLS_BURSTS, LOBBED_SHELLS_BURST_SLOTS);

	btEffectBuildCanvasBlitters(args->side, draw);
	if(mode == LOBBED_SHELLS_MAD_SPATTER || mode == LOBBED_SHELLS_MUD) {
		EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
		REG_BLDCNT = 0;
	} else {
		EFFECT_SET_TRANSFER(
			state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
	}
	sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);
	total = LOBBED_SHELLS_FRAMES(mode) + args->tgtCnt * 8;
	sySndPlayStub(SE_CURSE);

	for(frame = 0; frame != total; frame++) {
		PARTICLE *burst;
		PARTICLE *piece;

		EFFECT_VIEW_FROM_CAMERA(camera);

		/* The shells: each starts four frames after the last, flies under
		   gravity and tumbles through its pictures; on the ground it bursts. */
		EFFECT_FOR_EACH_TARGET(k, args) {
			for(i = 0; i != LOBBED_SHELLS_SHELLS(mode); i++) {
				Sint32 start = k * 8 + i * 4;

				if(frame >= start) {
					PARTICLE *shot = &PARTICLES[k * LOBBED_SHELLS_SLOTS + i];

					if(frame == start + 17) {
						btEffectTintBattler(args->tgts[k % args->tgtCnt],
							SPRITE_COLOR_WHITE, POSE_DAMAGE, k, 16);
						EFFECT_SHOW_DAMAGE(SE_IMPACT);
					}
					if(shot->variant >= 0) {
						Sint32 step = (frame - start) / 3;

						if(step > LOBBED_SHELL_PICTURES - 1)
							step = LOBBED_SHELL_PICTURES - 1;
						btEffectPosProjectPoint(shot, &position);
						position.x >>= 1;
						position.y -= 8;
						if(step > 4)
							draw[LOBBED_SHELLS_SHELL_BLITTER(mode)](canvas,
								state->sheet +
									step * PICTURE_BYTES(LOBBED_SHELL),
								position.x - LOBBED_SHELL_HEIGHT / 2,
								position.y - LOBBED_SHELL_WIDTH / 2,
								LOBBED_SHELL_HEIGHT, LOBBED_SHELL_WIDTH);
						else
							draw[LOBBED_SHELLS_SHELL_BLITTER(mode)](canvas,
								state->sheet +
									step * PICTURE_BYTES(LOBBED_SHELL),
								position.x - LOBBED_SHELL_WIDTH / 2,
								position.y - LOBBED_SHELL_HEIGHT / 2,
								LOBBED_SHELL_WIDTH, LOBBED_SHELL_HEIGHT);
						if(shot->variant == 0)
							btParticleAdvanceWithGravity3D(
								shot, 63, -toFIXED(1) / 2);
						if(shot->y < 0) {
							Sint32 count;
							Sint32 n;

							shot->y = 0;
							shot->variant = 1;
							if(LOBBED_SHELLS_SHATTER(mode) == 0)
								count = LOBBED_SHELLS_SMOKE_PUFFS;
							else
								count = LOBBED_SHELLS_SHARDS;
							/* Mud's LOBBED_SHELLS_SHARDS share half their slots
							   with the next shell's burst, and the later burst
							   takes them. */
							for(n = 0; n != count; n++) {
								piece = &LOBBED_SHELLS_PIECE(k, i, n);
								SET_POSITION(piece, shot->x, shot->y, shot->z);
								if(LOBBED_SHELLS_SHATTER(mode) == 0) {
									SET_VELOCITY(piece,
										((syRandom() & 63) - 32) << 11, 0,
										((syRandom() & 63) - 32) << 11);
								} else {
									SET_VELOCITY(piece,
										((syRandom() & 63) - 32) << 13,
										((syRandom() & 31) + 32) << 12,
										((syRandom() & 63) - 32) << 13);
								}
								piece->variant = 0;
							}
						}
					}
				}
			}
		}

		/* The bursts: smoke rises slowly and spreads; shards fly up, fall and
		   tumble, facing the way they move. */
		for(i = 0, burst = LOBBED_SHELLS_BURSTS; i != LOBBED_SHELLS_BURST_SLOTS;
			burst++, i++) {
			if(burst->variant >= 0 &&
				burst->variant < LOBBED_SHELLS_BURST_FRAMES && burst->y >= 0) {
				btEffectPosProjectPoint(burst, &position);
				position.x >>= 1;
				if(LOBBED_SHELLS_SHATTER(mode) == 0) {
					draw[1](canvas,
						state->sheet +
							burst->variant / 8 * PICTURE_BYTES(SMOKE_PUFF) +
							LOBBED_SHELLS_SMOKE_PICTURES,
						position.x - SMOKE_PUFF_WIDTH / 2,
						position.y - SMOKE_PUFF_HEIGHT / 2, SMOKE_PUFF_WIDTH,
						SMOKE_PUFF_HEIGHT);
				} else {
					Sint32 picture = burst->variant / 5;
					Sint32 flip;

					if(i & 1)
						picture += LOBBED_SHELLS_SMALL_SHARD;
					flip = args->side;
					draw[burst->velX > 0 ? flip ^ 1 : flip](canvas,
						state->sheet + LobbedShells_ShardPictures[picture] +
							LOBBED_SHELLS_SHARD_PICTURES,
						position.x - LobbedShells_ShardWidths[picture] / 2,
						position.y - LobbedShells_ShardHeights[picture] / 2,
						LobbedShells_ShardWidths[picture],
						LobbedShells_ShardHeights[picture]);
				}
				if(LOBBED_SHELLS_SHATTER(mode) == 0)
					btParticleAdvanceWithGravity3D(burst, 62, toFIXED(1) / 32);
				else
					btParticleAdvanceWithGravity3D(burst, 62, -toFIXED(1) / 2);
				burst->variant++;
			}
		}
		btCameraShake(4, 4);
		EFFECT_END_FRAME(state);
	}

	syCloseEvent(btEffectTransferCanvas);
	EFFECT_RELEASE_BLITTERS();

	if(mode == LOBBED_SHELLS_ROTTEN_BLOOD || mode == LOBBED_SHELLS_DECOMPOSE) {
		syWordsClear((void *)BG_CHAR_BLOCK(1), CANVAS_VIEW_BYTES);
		syWordsClear(canvas, CANVAS_VIEW_BYTES);
		syCloseEvent(btEffectDarkenBackgroundStep);
		if(mode == LOBBED_SHELLS_ROTTEN_BLOOD)
			btEffectRunImpair(args);
		else
			btEffectRunDull(args);
	} else {
		btEffectEndCanvasLayer();
	}
}

#undef DJINN_BEAMS_MOTES
#undef DJINN_BEAMS_TARGET_MOTES
#undef DJINN_BEAMS_MOTES_DRAWN
#undef DJINN_BEAMS_TARGET_FRAMES
#undef DJINN_BEAMS_MOTE_SWAY
#undef DJINN_BEAMS_DJINNI_SWAY_X
#undef DJINN_BEAMS_DJINNI_SWAY_Y
#undef DJINN_BEAMS_MOTE_PICTURES
#undef DJINN_BEAMS_MOTE_WIDTH
#undef DJINN_BEAMS_MOTE_HEIGHT
#undef DJINN_BEAMS_DJINNI_PICTURE
#undef DJINN_BEAMS_COLUMN_STRIP
#undef DJINN_BEAMS_COLUMN_STRIP_WIDTH
#undef LOBBED_SHELLS_BURSTS
#undef LOBBED_SHELLS_PIECE_SLOTS
#undef LOBBED_SHELLS_TARGET_SHELLS
#undef LOBBED_SHELLS_PIECE
#undef LOBBED_SHELLS_SLOTS
#undef LOBBED_SHELLS_BURST_SLOTS
#undef LOBBED_SHELLS_SMOKE_PICTURES
#undef LOBBED_SHELL_WIDTH
#undef LOBBED_SHELL_HEIGHT
#undef LOBBED_SHELL_PICTURES
#undef LOBBED_SHELLS_SHARD_PICTURES
#undef LOBBED_SHELLS_SMOKE_PUFFS
#undef LOBBED_SHELLS_SHARDS
#undef LOBBED_SHELLS_BURST_FRAMES
#undef LOBBED_SHELLS_SMALL_SHARD
#undef LOBBED_SHELLS_SHATTER
#undef LOBBED_SHELLS_SHELL_BLITTER
#undef LOBBED_SHELLS_SHELLS
#undef LOBBED_SHELLS_FRAMES
