/* Quake. Part of battle_effect.c. */

/* The columns of a QuakeTimings row. */
enum {
	QUAKE_TIMING_TICKS,		 /* ticks in the whole effect */
	QUAKE_TIMING_RISE_TICKS, /* ticks each pillar stands */
	QUAKE_TIMING_SHAKE		 /* how far the screen shakes */
};

/* The peak heights of Quake's rock pillars; only the first three are drawn. */
typedef struct ROCK_WALL_HEIGHTS {
	/* Each pillar's peak in pixels, negative, of which only the size counts. */
	Sint32 column[4];
} ROCK_WALL_HEIGHTS;

#define QUAKE_PILLAR_WIDTH	   40 /* a pillar's width in pixels */
/* The rock picture in the sprite sheet; the sprite sheet starts with the 40x16
   strip of ground cut from the background. */
#define QUAKE_WALL_PICTURE	   7680
/* The strip of ground that caps each pillar, cut from the battle background at
   (QUAKE_GROUND_X, QUAKE_GROUND_Y). */
#define QUAKE_GROUND_X		   32
#define QUAKE_GROUND_Y		   96
#define QUAKE_GROUND_HEIGHT	   16
/* A pillar's left edge on the canvas, and the canvas row it stands on. */
#define QUAKE_PILLAR_X(pillar) ((pillar) * QUAKE_PILLAR_WIDTH + 8)
#define QUAKE_PILLAR_FOOT	   112

/* Three rock pillars rise from the left, 4 ticks apart, and sink again,
   throwing the targets standing on them into the air.

   Abilities: Quake, Earthquake, Quake Sphere. */
void btEffectRunQuake(BATTLE_EFFECT_PARM *args)
{
	static Sint32 wallHeights[4] = {
		-32,
		-32,
		-32,
		-32,
	};
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 frm;
	Sint32				 pillar;
	DRAW_RECTANGLE		 draw;
	BATTLE_CAMERA		*camera;
	Sint32				 xOfs;
	ROCK_WALL_HEIGHTS	 heights;
	Sint32				 point[3];
	SCREEN_POINT		 screen;
	Sint32				 member;
	Sint32				 row;
	Sint32				 x;
	Sint32				 y;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
	state->args = args;
	btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	/* The canvas at its own width, opaque: the pillars hide what is behind
	   them. */
	REG_BG2PA = Q_8_8(1);
	REG_BLDCNT = 0;
	btResourceLoadAndDecompress(
		RESOURCE_ROCK_WALL_SHEET, state->sheet + QUAKE_WALL_PICTURE, 1, 1);

	/* Cut the strip of ground that caps each pillar from the battle
	   background. */
	syWordsCopy(Ram_MapGridMetatileBuffer, BG_CHAR_BLOCK(2), 2 * BG_CHAR_SIZE);
	for(row = 0; row != QUAKE_GROUND_HEIGHT; row++) {
		y = row + QUAKE_GROUND_Y;
		for(x = 0; x != QUAKE_PILLAR_WIDTH; x++) {
			Sint32 pixel = BACKGROUND_PIXEL_OFFSET(x + QUAKE_GROUND_X, y);

			state->sheet[row * QUAKE_PILLAR_WIDTH + x] =
				Ram_MapGridMetatileBuffer[pixel];
		}
	}

	if(state->args->side == EFFECT_SIDE_PARTY) {
		REG_BG2X = -(NARROW_CANVAS_PARTY_X << 8);
		xOfs = -NARROW_CANVAS_PARTY_X;
	} else {
		xOfs = 0;
	}
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_TRANSPARENT);
	draw = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
	EFFECT_FOR_EACH_TARGET(member, state->args)
		state->particles[member].variant = 0;
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
	heights = (*(const ROCK_WALL_HEIGHTS *)&wallHeights);
	state->shakeFrm = 128;
	sySndPlayStub(SE_RUMBLE);

	for(frm = 0; frm != QuakeTimings[state->args->variant][QUAKE_TIMING_TICKS];
		frm++) {
		EFFECT_VIEW_FROM_CAMERA(camera);
		if(frm == QuakeTimings[state->args->variant][QUAKE_TIMING_TICKS] - 16)
			EFFECT_SHOW_DAMAGE(SE_IMPACT);

		for(pillar = 0; pillar != 3; pillar++) {
			Sint32 riseFrm = pillar * 4 + 16;

			/* DEAD: never true, as the left side is 32 or more and riseFrm at
			   most 24; the ROM keeps the test and its store. */
			if((frm & 31) + 32 == riseFrm)
				heights.column[pillar] += 32;
			if(frm >= riseFrm &&
				frm < riseFrm +
						QuakeTimings[state->args->variant]
									[QUAKE_TIMING_RISE_TICKS]) {
				Sint32 height;
				Sint32 top;

				/* A pillar rises and sinks along a sine, once, twice or three
				   times by variant. */
				height = FIXEDtoINT(heights.column[pillar] *
					sySin((frm - riseFrm) * (TRIG_TURN / 64)));
				if(height < 0)
					height = -height;
				top = QUAKE_PILLAR_FOOT - height;
				draw(canvas, state->sheet + QUAKE_WALL_PICTURE,
					QUAKE_PILLAR_X(pillar), top, QUAKE_PILLAR_WIDTH, height);
				draw(canvas, state->sheet, QUAKE_PILLAR_X(pillar),
					QUAKE_PILLAR_FOOT - QUAKE_GROUND_HEIGHT - height,
					QUAKE_PILLAR_WIDTH, QUAKE_GROUND_HEIGHT);

				/* Throw up the targets standing on the pillar. */
				EFFECT_FOR_EACH_TARGET(member, state->args) {
					ACTOR *actor;

					actor = btObjectGetStub(state->args->tgts[member])->actor;
					SET_VECTOR(
						point, actor->x.fixed, actor->y.fixed, actor->z.fixed);
					btProjectionPoint(point, &screen);
					screen.x += xOfs;
					if(screen.x >= QUAKE_PILLAR_X(pillar) &&
						screen.x <=
							QUAKE_PILLAR_X(pillar) + QUAKE_PILLAR_WIDTH &&
						screen.y >= top) {
						actor->velY = toFIXED(12);
						actor->gravity = BATTLE_UNIT_GRAVITY;
					}
					if(actor->y.fixed < 0)
						btEffectTintBattler(state->args->tgts[member],
							SPRITE_COLOR_NORMAL, POSE_DAMAGE, TINT_KEEP, 0);
				}
			}
		}

		EFFECT_FOR_EACH_TARGET(member, state->args) {
			ACTOR	 *actor;
			PARTICLE *landing;

			actor = btObjectGetStub(state->args->tgts[member])->actor;
			landing = &state->particles[member];
			if(landing->variant == 0 && actor->y.fixed <= 0 &&
				actor->velY < 0) {
				landing->variant = 1;
				btEffectTintBattler(state->args->tgts[member],
					SPRITE_COLOR_WHITE, POSE_DAMAGE, member, 5);
			}
		}
		btCameraShake(QuakeTimings[state->args->variant][QUAKE_TIMING_SHAKE],
			QuakeTimings[state->args->variant][QUAKE_TIMING_SHAKE]);
		EFFECT_END_TICK(state);
	}

	EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#undef QUAKE_PILLAR_WIDTH
#undef QUAKE_WALL_PICTURE
#undef QUAKE_GROUND_X
#undef QUAKE_GROUND_Y
#undef QUAKE_GROUND_HEIGHT
#undef QUAKE_PILLAR_X
#undef QUAKE_PILLAR_FOOT
