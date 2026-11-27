/* Drawing helpers several effects share (a sprite taken from the pool, the grey
   ramp, shaded discs, the mirrored palette), then the Ulysses summon, the swirl
   of Poison Beat and Spinning Beat, and Spider Web. Part of battle_effect.c. */

/* Takes the first free sprite of the actor system's pool with a new animation
   entry. Its callers also pass the sprite's OAM shape and mode, which it leaves
   for the caller to set. */
void *btEffectSprAcquire(Sint32 width, Sint32 height, Uint32 shape, Sint32 mode)
{
	SPR	  *spr = RamHeapSlots->sprs;
	SPR	  *found = NULL;
	Sint32 i = 0;

	while(i < ANIM_OBJECT_COUNT) {
		if(spr->width == 0) {
			found = spr;
			break;
		}
		i++;
		spr++;
	}
	spr->entries[0] = btAnimationObjectAllocateEmpty();
	spr->width = width;
	spr->height = height;
	spr->scale = toFIXED(1);
	spr->retainCacheEntry = 0;
	spr->ofsY = 0;
	spr->ofsX = 0;
	spr->cnt = 1;
	spr->flg = 0;
	spr->rotation = 0;
	fdSprStartAnimationStub(spr, 0);
	return found;
}

/* Fills BG palette colours 1 to 63, the canvas's levels, with a grey ramp, each
   level i / 2. */
void btGraphicsLoadGreyRamp(void)
{
	Sint32	i = 0;
	Uint16 *color = (Uint16 *)&BG_PLTT_COLOR(0, 1);

	do {
		{
			Sint32 level = i / 2;

			i++;
			*color = RGB(level, level, level);
		}
		color++;
	} while(i != CANVAS_LEVEL_BRIGHTEST);
}

/* Draws the upper left quarter of a shaded disc into a map `size` / 2 bytes
   wide and `size` rows tall, centred on its bottom right corner and squeezed to
   half width: each ring is darker than the one inside it by a cosine falloff,
   scaled by `scale` (toFIXED(1) at full strength) and clamped at
   CANVAS_LEVEL_BRIGHTEST. */
void btEffectDrawShadedCircles(Uint8 *map, Sint32 size, FIXED scale)
{
	Sint32 half = size / 2;
	Sint32 center = half - 1;
	Sint32 last = size - 1;
	Sint32 radius;

	syWordsClearEntry(map, half * size);
	for(radius = 0; radius != last; radius++) {
		Sint32 x = radius;
		Sint32 d = radius;
		Sint32 y = 0;
		Sint32 shade;
		Sint32 col;

		shade = FIXEDtoINT(
			syCos(radius * DEGtoANG(90) / last) * CANVAS_LEVEL_BRIGHTEST);
		shade = shade * scale / (toFIXED(1));

		if(shade > CANVAS_LEVEL_BRIGHTEST)
			shade = CANVAS_LEVEL_BRIGHTEST;
		for(; x >= y; y++) {
			col = center - x / 2;
			map[half * (last - y) + col] = shade;
			col = center - y / 2;
			map[half * (last - x) + col] = shade;
			d = d - y * 2 - 1;
			if(d < 0) {
				d = d + x * 2 - 2;
				x--;
			}
		}
	}
}

/* Draws the whole shaded disc btEffectDrawShadedCircles draws a quarter of,
   mirrored into the four quarters of a map `size` bytes wide and twice `size`
   rows tall. */
void btEffectDrawShadedCirclesMirrored(Uint8 *map, Sint32 size, Sint32 scale)
{
	Sint32 center = size / 2 - 1;
	Sint32 last = size - 1;
	Sint32 radius;

	syWordsClearEntry(map, size * size * 2);
	for(radius = 0; radius != last; radius++) {
		Sint32 x = radius;
		Sint32 d = radius;
		Sint32 y = 0;
		Sint32 shade;
		Sint32 col;
		Sint32 row;

		shade = FIXEDtoINT(
			syCos(radius * DEGtoANG(90) / last) * CANVAS_LEVEL_BRIGHTEST);
		shade = shade * scale / (toFIXED(1));
		if(shade > CANVAS_LEVEL_BRIGHTEST)
			shade = CANVAS_LEVEL_BRIGHTEST;
		for(; x >= y; y++) {
			col = center - x / 2;
			row = last - y;
			map[row * size + col] = shade;
			map[(size * 2 - row - 1) * size + col] = shade;
			map[row * size + (size - col) - 1] = shade;
			map[(size * 2 - row - 1) * size + (size - col) - 1] = shade;
			col = center - y / 2;
			row = last - x;
			map[row * size + col] = shade;
			map[(size * 2 - row - 1) * size + col] = shade;
			map[row * size + (size - col) - 1] = shade;
			map[(size * 2 - row - 1) * size + (size - col) - 1] = shade;
			d = d - y * 2 - 1;
			if(d < 0) {
				d = d + x * 2 - 2;
				x--;
			}
		}
	}
}

/* Mirrors the first 64 background colours, keeping the backdrop colour. */
void btPaletteMirrorBgCols(void)
{
	Uint16 saved[SHEET_PALETTE_COLORS];
	Uint16 flipped[SHEET_PALETTE_COLORS];
	Sint32 i;

	syWordsCopy(saved, (void *)BG_PLTT, SHEET_PALETTE_BYTES);
	for(i = 1; i != SHEET_PALETTE_COLORS; i++)
		flipped[i] = saved[SHEET_PALETTE_COLORS - i];
	flipped[0] = saved[0];
	syWordsCopy((void *)BG_PLTT, flipped, SHEET_PALETTE_BYTES);
	REG_BLDALPHA = BLEND_ALPHA(16, 8);
}

/* The mage is effect sprites 0 to 6, on UlyssesObjectX and UlyssesObjectZ, and
   sprite 7 is a white flake: its animations from ULYSSES_FLAKE_ANIMATION are
   the flake turning. */
#define ULYSSES_MAGE_SPRITES	7
#define ULYSSES_FLAKE_SPRITE	ULYSSES_MAGE_SPRITES
#define ULYSSES_FLAKE_ANIMATION 7

/* The particle the last flake flies by: the state's last. */
#define ULYSSES_FLAKE (state->particles[ARRAY_COUNT(state->particles) - 1])

/* A tick event of Ulysses's end (btEffectRunUlysses adds and removes it): the
   white flake flutters away from where the mage stood, its speed in depth
   rising for 20 ticks and then falling, drifting sideways from tick 12, and
   turning through its eight pictures, seven ticks each. */
void btUlyssesFlutterFlake(void)
{
	BATTLE_EFFECT_STATE *state = RamHeapSlots->battleEff;
	SPR_SCALE			 scale = UlyssesParticleScale;
	SPR_PLACEMENT		 place;

	place.groundHeight = 0;

	place.height = 0;

	place.x = ULYSSES_FLAKE.x;
	place.y = ULYSSES_FLAKE.y;
	fdRenderApplyProjectedPlacementStub(
		state->sprs[ULYSSES_FLAKE_SPRITE], &place.x, &scale.x, 0);
	ULYSSES_FLAKE.x += ULYSSES_FLAKE.velX;
	ULYSSES_FLAKE.y += ULYSSES_FLAKE.velY;
	if(state->frm <= 19)
		ULYSSES_FLAKE.velY += toFIXED(1) / 16;
	else
		ULYSSES_FLAKE.velY -= toFIXED(1) / 16;
	if(state->frm > 11)
		ULYSSES_FLAKE.velX += toFIXED(1) / 16;
	fdSprStartAnimationStub(state->sprs[ULYSSES_FLAKE_SPRITE],
		state->frm / 7 % 8 + ULYSSES_FLAKE_ANIMATION);
	state->frm++;
}

/* The mage's sprite scale, one to one. */
extern const SCALE UlyssesMageScale;

/* Where UlyssesSilhouette, then FlutteringFlakeSheet's pictures, are decoded in
   the effect's pictures, and the silhouette's size. */
#define ULYSSES_PICTURES			 6017
#define ULYSSES_SILHOUETTE_WIDTH	 80
#define ULYSSES_SILHOUETTE_HEIGHT	 112
/* FlutteringFlakeSheet's eighteen pictures of the white flake turning. */
#define ULYSSES_FLAKE_PICTURES		 18
/* Where RuneCircleSheet and YellowOrbSheet are decoded in the effect's
   pictures. */
#define ULYSSES_RUNE_CIRCLE_PICTURES 10113
#define ULYSSES_YELLOW_ORB_PICTURES	 7831
/* FadingCrescentSheet, in the map cell buffer: a crescent of light closing to a
   disc and shrinking, seven pictures. */
#define ULYSSES_CRESCENT_WIDTH		 64
#define ULYSSES_CRESCENT_HEIGHT		 64
/* A crescent's scale `t` ticks into its sweep: a sine that turns a
   sixty-fourth of a turn a tick, at 48 sixty-fourths, so it swells to three
   quarters of its size and shrinks a little. */
#define ULYSSES_CRESCENT_SCALE(t)	 ((sySin((t) << 10) * 48) / 64)

/* ticks of the runes, the summoning and the fire */
#define ULYSSES_TICKS				   326
/* The flame bursts and the rune circles open, the second
   ULYSSES_RUNE_CIRCLE_INTERVAL ticks after the first; the mage's silhouette
   glows in them, and the mage appears. */
#define ULYSSES_RUNE_CIRCLES_TICK	   76
#define ULYSSES_RUNE_CIRCLE_INTERVAL   16
#define ULYSSES_SILHOUETTE_TICK		   92
#define ULYSSES_APPEAR_TICK			   108
/* The rune circles' colours fade, then the strikes begin. */
#define ULYSSES_RUNE_CIRCLES_FADE_TICK 124
#define ULYSSES_STRIKES_TICK		   132
/* The fire flares over the mage, and the flake flutters away. */
#define ULYSSES_FLARE_TICK			   266
#define ULYSSES_FLAKE_TICK			   288

/* The flake drifts down, swaying 8 pixels either way, in 1/65536 turns a
   tick. */
#define ULYSSES_FLAKE_SWAY			860
/* The silhouette swells and shrinks back as it zooms in, in 1/65536 turns a
   tick. */
#define ULYSSES_SILHOUETTE_SWELL	800
/* The silhouette is drawn in the two BG palettes from ULYSSES_GLOW_PALETTE,
   which glow violet: brighter over its first ULYSSES_GLOW_TICKS, and gone
   ULYSSES_GLOW_END ticks after it shows. */
#define ULYSSES_GLOW_PALETTE		8
#define ULYSSES_GLOW_COLORS			32
#define ULYSSES_GLOW_TICKS			16
#define ULYSSES_GLOW_END			32
/* Colour `i` of the glow's at `level` (0 to 16): violet, brighter up the ramp,
   its red three quarters of its blue. */
#define ULYSSES_GLOW_RED(i, level)	(((i) * (((level) * 9 * 4) / 16)) / 64)
#define ULYSSES_GLOW_BLUE(i, level) (((i) * (level) * 3) / 64)

/* The rune circles: the first grows to three quarters of its size, holds and
   spreads out from ULYSSES_RUNE_CIRCLE_HOLD_TICKS; the second rises from lying
   flat over ULYSSES_RUNE_CIRCLE_TILT_TICKS, then shrinks. */
#define ULYSSES_RUNE_CIRCLE_HOLD_TICKS 40
#define ULYSSES_RUNE_CIRCLE_TILT_TICKS 24
#define ULYSSES_RUNE_CIRCLE_TILT_X(t) \
	(ULYSSES_RUNE_CIRCLE_TILT_TICKS * 682 - (t) * 682)
#define ULYSSES_RUNE_CIRCLE_TILT_Z(t) \
	((t) * 169 + TRIG_TURN - ULYSSES_RUNE_CIRCLE_TILT_TICKS * 169)

/* A flame of the opening burst and of the flare: six pictures, three or four
   ticks each. */
#define ULYSSES_BURST_FLAME_TICKS 18
#define ULYSSES_FLARE_FLAME_TICKS 24
#define ULYSSES_FLARE_FLAMES	  32 /* flames of the flare, in a column */

/* The mage's six strikes, each ULYSSES_STRIKE_INTERVAL ticks after the last,
   by the strike's own tick: violet flames at the mage's hand, then a flake
   flies, a crescent of light sweeps round the targets and closes, and the
   strike lands, with a burst of light and sparks. */
#define ULYSSES_STRIKES				  6
#define ULYSSES_STRIKE_INTERVAL		  8
#define ULYSSES_STRIKE_FLAME_TICKS	  36
#define ULYSSES_STRIKE_FLAKE_TICK	  12
#define ULYSSES_STRIKE_FLAKE_TICKS	  32
#define ULYSSES_STRIKE_CRESCENT_TICK  44
#define ULYSSES_STRIKE_CRESCENT_TICKS 22
#define ULYSSES_STRIKE_CLOSE_TICK	  59
#define ULYSSES_STRIKE_POWER_TICK	  60
#define ULYSSES_STRIKE_HIT_TICK		  64
#define ULYSSES_STRIKE_BURST_TICKS	  12
/* The sparks each strike throws, in the effect's pictures. */
#define ULYSSES_SPARKS				  19352
#define ULYSSES_STRIKE_SPARKS		  16
#define ULYSSES_SPARK_COUNT			  (ULYSSES_STRIKES * ULYSSES_STRIKE_SPARKS)

/* A white flake drifts down, a flame bursts and a ring of red runes opens with
   the mage's silhouette glowing violet in it; the mage appears, and six times
   violet flames gather at the mage's hand and a crescent of light sweeps round
   the foes and strikes; then fire flares over the mage, who fades, and a white
   flake flutters away.

   Abilities: Ulysses. */
void btEffectRunUlysses(BATTLE_EFFECT_PARM *args)
{
	Uint8					*canvas;
	BATTLE_BACKGROUND_STATE *background;
	BATTLE_EFFECT_STATE		*state;
	Sint32					 frm;
	Uint8					*sprs;
	Sint32					 flakeAway;
	Sint32					 skip;
	DRAW_RECTANGLE			*blit;

	struct {
		Sint32 x;
		Sint32 z;
	} mage;

	Sint32		  loaded;
	Sint32		  draw;
	Sint32		  i;
	Uint8		 *cells;
	Uint8		 *picture;
	SPR_PLACEMENT place;
	BLITTER_PAIR  blits;
	MODEL_TEXTURE texture;
	MODEL_TEXTURE quadTexture;
	SCALE		  mageScale;
	BATTLE_STATE *battle;
	Sint32		  mask = RGB_LEVEL_MAX;

	canvas = RamHeapSlots->battleCanvas;
	state = RamHeapSlots->battleEff;
	background = RamHeapSlots->battleBg;
	sprs = RamHeapSlots->battleSheet;
	flakeAway = 0;
	skip = 0;
	btEffectShowLivingTargets(state->args);
	/* The battle background darkens to black, its green first. */
	for(frm = 0; frm != 32; frm++) {
		volatile Uint16 *c = (volatile Uint16 *)&BG_PLTT_COLOR(6, 0);

		for(i = 0; i != BATTLE_BACKGROUND_COLORS; i++, c++) {
			Sint32 v = *c;
			Sint32 r = mask & v;
			Sint32 g;
			Sint32 b;

			g = (Uint16)RGB_GREEN((Uint16)v);
			b = (Uint16)RGB_BLUE((Uint16)v);
			if(g == 0) {
				if(b > 0) {
					b--;
				}
				if(r > 0) {
					r--;
				}
			}
			if(g > 0) {
				g--;
			}
			*c = RGB(r, g, b);
		}
		sySynch(1);
	}
	btEffectStartCanvasLayer(0);
	IO_WRITE_QUEUE_PUSH((Uint32)&REG_DISPCNT,
		DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_OBJ_ON,
		IO_WRITE_HALFWORD);
	sySynch(1);
	/* BG1 shows a canvas-sized picture from BG character block 2, as
	   btEffectBuildCanvasTilemap writes it: the rune circles. */
	{
		Uint32 *map = (Uint32 *)BG_SCREEN_BLOCK(BATTLE_BG1_SCREEN_BLOCK);
		Uint32	v;
		Sint32	y;
		Sint32	x;

		syWordsFill(map, 4 * MAP_ROW_BYTES, -1);
		map += 4 * MAP_ROW_WORDS;
		syWordsFill(map, 2 * MAP_ROW_BYTES,
			MAP_ENTRY_PAIR(BG1_CLEAR_TILE, BG1_CLEAR_TILE));
		v = MAP_ENTRY_PAIR(BG1_BLOCK2_FIRST_TILE, BG1_BLOCK2_FIRST_TILE + 1);
		map += 2 * MAP_ROW_WORDS;
		for(y = 0; y != CANVAS_TILES; y++) {
			for(x = 0; x != 8; x++) {
				*map++ = v;
				v += MAP_ENTRY_PAIR(2, 2);
			}
			for(x = 0; x != 8; x++) {
				*map++ = MAP_ENTRY_PAIR(BG1_CLEAR_TILE, BG1_CLEAR_TILE);
			}
		}
	}
	btEffectCreateSprs(ULYSSES_MAGE_SPRITES + 1, SPRITE_ULYSSES, 3);
	REG_BG2PA = Q_8_8(0.5);
	REG_BG2PA = Q_8_8(1);
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND |
		BLDCNT_TGT2_ALL;
	REG_BG1CNT = BGCNT_PRIORITY(3) | BGCNT_256_COLORS |
		BGCNT_SCREEN_BASE(BATTLE_BG1_SCREEN_BLOCK);
	sySynch(1);
	blit = blits;
	btEffectBuildCanvasBlitters(0, blit);
	/* The silhouette, in the glow's palettes, goes to BG2's tiles and stays
	   there while the rune circles go to BG1's. */
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
	cells = state->sheet + ULYSSES_PICTURES;
	btResourceLoadAndDecompress(RESOURCE_ULYSSES_SILHOUETTE, cells, 0, 0);
	for(i = 0; i != PICTURE_BYTES(ULYSSES_SILHOUETTE); i++, cells++) {
		if(*cells) {
			*cells += ULYSSES_GLOW_PALETTE * PALETTE_COLORS;
		}
	}
	picture = state->sheet + ULYSSES_PICTURES;
	blits[0](canvas, picture, 24, 8, ULYSSES_SILHOUETTE_WIDTH,
		ULYSSES_SILHOUETTE_HEIGHT);
	EFFECT_PRESENT_CANVAS(state);
	syCloseEvent(btEffectTransferCanvas);
	btResourceLoadAndDecompress(
		RESOURCE_EMBER_STREAK_SHEET, state->sheet, 1, 0);
	btResourceLoadAndDecompress(RESOURCE_FLUTTERING_FLAKE_SHEET, picture, 1, 1);
	btResourceLoadAndDecompress(RESOURCE_BLUE_GLOW_SHEET, sprs, 0, 0);
	loaded = 0;
	mage.x = toFIXED(200);
	mage.z = toFIXED(84);
	background->busy = 1;
	syWordsClear(canvas, CANVAS_VIEW_BYTES);
	REG_DISPCNT = DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
		DISPCNT_OBJ_ON | DISPCNT_WIN0_ON | DISPCNT_WIN1_ON;
	sySynch(1);
	for(frm = 0; frm != ULYSSES_TICKS; frm++) {
		PARTICLE *particle;
		/* Once A or B is pressed, odd ticks are not drawn or waited for: the
		   rest plays twice as fast. */
		draw = 1;
		if(PerWork.repeat & (KEY_A | KEY_B)) {
			skip = 1;
		}
		if((frm & draw) && skip == 1) {
			draw = 0;
		}
		if(frm == 0) {
			PARTICLE *p;

			if(!loaded) {
				sySetEvent(btEffectProcessPendingBlock2Transfer,
					EVENT_PASS_VBLANK + 15);
				sySetEvent(btDispApplyBg2Affine, EVENT_PASS_VBLANK + 127);
				btResourceLoadAndDecompress(RESOURCE_RUNE_CIRCLE_SHEET,
					state->sheet + ULYSSES_RUNE_CIRCLE_PICTURES, 1, 0);
				loaded = 1;
			}
			BgScrollWork[1].x = -112;
			REG_BLDALPHA = EFFECT_BLEND_ADD;
			REG_DISPCNT = EFFECT_DISPCNT;
			REG_BG2CNT =
				BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
			REG_BG1CNT =
				BGCNT_256_COLORS | BGCNT_SCREEN_BASE(BATTLE_BG1_SCREEN_BLOCK);
			EFFECT_SET_TRANSFER(
				state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
			syWordsCopyEntry((void *)BG_PLTT,
				syResourceGetTableEntry(RESOURCE_MARS_DJINN_SHEET),
				SHEET_PALETTE_BYTES);
			for(i = 0; i != 9; i++) {
				p = &state->particles[i];
				p->variant = -2 * i;
				p->x = (syRandom() & 15) + 60;
				p->y = (syRandom() & 15) + 44;
			}
		}
		/* The white flake drifts down. */
		if(frm < ULYSSES_RUNE_CIRCLES_TICK) {
			Sint32 k = (frm / 4) % ULYSSES_FLAKE_PICTURES;
			Sint32 x = 60 - (FIXEDtoINT(sySin(frm * ULYSSES_FLAKE_SWAY) << 3));
			Sint32 y = frm - 8;
			Uint8 *src = state->sheet + ULYSSES_PICTURES + UlyssesPictures[k];
			Uint8  w = UlyssesPictureWidths[k];
			Sint32 left = x - (w >> 1);
			Uint8  h = UlyssesPictureHeights[k];

			blit[1](canvas, src, left, y - (h >> 1), w, h);
		}
		/* A flame bursts where it lands. */
		if(frm >= ULYSSES_RUNE_CIRCLES_TICK && frm < ULYSSES_APPEAR_TICK) {
			PARTICLE *p;

			for(i = 0, p = state->particles; i != 4; i++, p++) {
				if((Uint32)p->variant < ULYSSES_BURST_FLAME_TICKS) {
					Sint32 k = p->variant / 3;
					Sint32 side = i & 1;
					Uint8 *src =
						state->sheet + BattleEffectEmberStreakPictures[k];
					Uint8 w = BattleEffectEmberStreakPictureWidths[k];

					blit[side](canvas, src, p->x - (w >> 1),
						p->y + BattleEffectEmberStreakPictureBiasY[k] - 32, w,
						BattleEffectEmberStreakPictureHeights[k]);
				}
				p->variant++;
			}
		}
		if(frm == ULYSSES_APPEAR_TICK) {
			REG_BLDALPHA = BLEND_ALPHA(16, 0);
		}
		if(frm >= ULYSSES_APPEAR_TICK && frm <= 116) {
			REG_BLDALPHA = BLEND_ALPHA(16, (frm - ULYSSES_APPEAR_TICK) * 2);
		}
		/* The silhouette zooms in, swelling and shrinking back, and glows. */
		if(frm >= ULYSSES_SILHOUETTE_TICK && frm < ULYSSES_STRIKES_TICK) {
			Sint32 t = frm - ULYSSES_SILHOUETTE_TICK;
			FIXED  distance =
				syCos(t * ULYSSES_SILHOUETTE_SWELL) / 3 + toFIXED(1);
			Sint32 zoom;
			Sint32 level = -1;

			if(distance < toFIXED(33) / 32) {
				distance = toFIXED(33) / 32;
			}
			zoom = toFIXED(256) / distance;
			state->bg2pa = zoom;
			state->bg2pd = zoom;
			state->bg2x = (64 - toFIXED(184) / distance) << 8;
			state->bg2y = (64 - toFIXED(76) / distance) << 8;
			if(t < ULYSSES_GLOW_TICKS) {
				level = t;
			} else if(frm >= 116 && frm <= ULYSSES_RUNE_CIRCLES_FADE_TICK) {
				level = ULYSSES_GLOW_END * 2 - t * 2;
			}
			if(level != -1) {
				for(i = 0; i != ULYSSES_GLOW_COLORS; i++) {
					Sint32 red = ULYSSES_GLOW_RED(i, level);
					Sint32 blue = ULYSSES_GLOW_BLUE(i, level);

					((Uint16 *)&BG_PLTT_COLOR(ULYSSES_GLOW_PALETTE, 0))[i] =
						RGB(red, 0, blue);
				}
			}
			if(frm == 116) {
				REG_DISPCNT = DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP |
					DISPCNT_BG0_ON | DISPCNT_BG1_ON | DISPCNT_OBJ_ON |
					DISPCNT_WIN0_ON | DISPCNT_WIN1_ON;
			}
		}
		/* The rune circles' colours fade by 3 a tick. */
		if(frm >= ULYSSES_RUNE_CIRCLES_FADE_TICK &&
			frm < ULYSSES_STRIKES_TICK) {
			volatile Uint16 *c = (volatile Uint16 *)BG_PLTT;

			for(i = 0; i != SHEET_PALETTE_COLORS; i++, c++) {
				Sint32 v = *c;
				Sint32 low = RGB_LEVEL_MAX;
				Sint32 r;
				Sint32 g;
				Sint32 b;

				low &= v;
				r = low - 3;
				g = ((Uint16)RGB_GREEN((Uint16)v)) - 3;
				b = ((Uint16)RGB_BLUE((Uint16)v)) - 3;

				if(r < 0)
					r = 0;
				if(g < 0)
					g = 0;
				if(b < 0)
					b = 0;
				*c = RGB(r, g, b);
			}
		}
		if(frm == ULYSSES_STRIKES_TICK) {
			if(loaded == 1) {
				syCloseEvent(btEffectProcessPendingBlock2Transfer);
				syCloseEvent(btDispApplyBg2Affine);
				sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK + 15);
				btResourceLoadAndDecompress(RESOURCE_YELLOW_ORB_SHEET,
					state->sheet + ULYSSES_YELLOW_ORB_PICTURES, 1, 0);
				loaded = 0;
			}
			REG_DISPCNT = DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
				DISPCNT_OBJ_ON | DISPCNT_WIN0_ON | DISPCNT_WIN1_ON;
			REG_BG2X = -(8 << 8);
			REG_BG2Y = -(BATTLE_VIEW_TOP << 8);
			REG_BG2PA = Q_8_8(0.8);
			REG_BG2PD = Q_8_8(1);
			REG_BG2CNT =
				BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
			REG_BLDALPHA = EFFECT_BLEND_ADD;
			syWordsCopyEntry((void *)BG_PLTT,
				syResourceGetTableEntry(RESOURCE_RUNE_SHEET),
				SHEET_PALETTE_BYTES);
		}
		if(frm == ULYSSES_STRIKES_TICK + 1) {
			PARTICLE *p;
			PARTICLE *spark;

			syWordsFill(canvas, CANVAS_VIEW_BYTES, 0);
			syWordsClear((void *)BG_CHAR_BLOCK(1), CANVAS_VIEW_BYTES);
			REG_DISPCNT = DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
				DISPCNT_BG2_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
				DISPCNT_WIN1_ON;
			spark = (PARTICLE *)(state->sheet + ULYSSES_SPARKS);
			for(i = 0; i != ULYSSES_STRIKES; i++) {
				p = &state->particles[i];
				p->x = toFIXED(UlyssesParticleStarts[i][0]);
				p->variant = -ULYSSES_STRIKE_INTERVAL * i;
				p->y = toFIXED(UlyssesParticleStarts[i][1]);
				p->z = UlyssesParticleStarts[i][2] << 6;
				p->velX = UlyssesParticleStarts[i][3] * (toFIXED(1) / 2);
				p->velY = UlyssesParticleStarts[i][4] * (toFIXED(1) / 2);
				p->velZ = 0;
			}
			for(i = 0; i != ULYSSES_SPARK_COUNT; i++) {
				spark[i].variant = -1;
			}
			btResourceLoadAndDecompress(
				RESOURCE_FADING_CRESCENT_SHEET, MapGridMetatileBuffer, 0, 0);
		}
		if(frm == ULYSSES_RUNE_CIRCLES_TICK) {
			sySndPlayStub(SE_ITEM_BREAK);
		}
		if(frm == ULYSSES_FLARE_TICK) {
			sySndPlayStub(SE_ITEM_BREAK);
		}

		if(frm > ULYSSES_STRIKES_TICK && frm < ULYSSES_FLARE_TICK) {
			void		 *verts;
			RENDER_MODEL *model;
			PARTICLE	 *sparks;
			Uint8		 *crescents = MapGridMetatileBuffer;
			PARTICLE	 *spark;
			Sint32		  shade;
			Sint32		  j;

			verts = syHeapAllocEWRAM(32);
			model = btRenderAllocateModelList(1);
			shade = 0;
			sparks = (PARTICLE *)(state->sheet + ULYSSES_SPARKS);
			if(frm > 196) {
				btEffectStepPaletteToResource(RESOURCE_MARS_DJINN_SHEET);
			}
			texture.logWidth = 6;
			texture.logHeight = 6;
			model->kind = MODEL_SHADED_ADDITIVE;
			model->shade = shade;
			model->texture = &texture;
			model->mesh = BattleEffectSquareMesh;
			model->vertices = verts;
			for(i = 0, particle = state->particles; i != ULYSSES_STRIKES;
				particle++, i++) {
				Sint32	  delay = i * ULYSSES_STRIKE_INTERVAL;
				PARTICLE *anchor = particle + 8;
				Sint32	  t;

				if(++particle->variant == 0) {
					sySndPlayStub(SE_ITEM_BREAK);
				}
				t = particle->variant;
				if(t == ULYSSES_STRIKE_POWER_TICK) {
					sySndPlayStub(SE_POWER_UP);
					t = particle->variant;
				}
				if(t == ULYSSES_STRIKE_HIT_TICK) {
					sySndPlayStub(SE_HEAVY_IMPACT);
					t = particle->variant;
				}
				/* Violet flames at the mage's hand. */
				if((Uint32)t < ULYSSES_STRIKE_FLAME_TICKS) {
					Sint32 k = (t / 6) % 6;
					Uint8 *src =
						state->sheet + BattleEffectEmberStreakPictures[k];
					Uint8 w = BattleEffectEmberStreakPictureWidths[k];

					blits[0](canvas, src, FIXEDtoINT(particle->x) - (w >> 1),
						FIXEDtoINT(particle->y) +
							BattleEffectEmberStreakPictureBiasY[k] - 36,
						w, BattleEffectEmberStreakPictureHeights[k]);
					t = particle->variant;
				}
				/* A flake flies out. */
				if((Uint32)(t - ULYSSES_STRIKE_FLAKE_TICK) <
					ULYSSES_STRIKE_FLAKE_TICKS) {
					Sint32 k = ((t - ULYSSES_STRIKE_FLAKE_TICK) / 4) %
						ULYSSES_FLAKE_PICTURES;
					Uint8 *src =
						state->sheet + ULYSSES_PICTURES + UlyssesPictures[k];
					Sint32 x = FIXEDtoINT(particle->x);
					Uint8  w = UlyssesPictureWidths[k];
					Sint32 left = x - (w >> 1);
					Sint32 y = FIXEDtoINT(particle->y);
					Uint8  h = UlyssesPictureHeights[k];

					blit[1](canvas, src, left, y - (h >> 1), w, h);
					t = particle->variant;
				}
				/* The crescent of light sweeps round the targets, wobbling, and
				   closes from ULYSSES_STRIKE_CLOSE_TICK. */
				if((Uint32)(t - ULYSSES_STRIKE_CRESCENT_TICK) <
					ULYSSES_STRIKE_CRESCENT_TICKS) {
					FIXED scale = ULYSSES_CRESCENT_SCALE(
						t - ULYSSES_STRIKE_CRESCENT_TICK);
					Sint32 vx;
					Sint32 x = particle->x;
					FIXED  mx = x - toFIXED(64);
					FIXED  my = particle->y - toFIXED(64);
					Sint32 closing = t - ULYSSES_STRIKE_CLOSE_TICK;

					if(closing < 0) {
						closing = 0;
					}
					vx = particle->velX;
					particle->y += particle->velY;
					x += vx;
					particle->x = x;
					if(x < toFIXED(64)) {
						particle->velX = vx + toFIXED(1) / 2;
					}
					if(particle->y < toFIXED(60)) {
						particle->velY += toFIXED(1) / 2;
					} else {
						particle->velY -= toFIXED(1) / 2;
					}
					if(particle->z >= 0) {
						particle->z +=
							FIXEDtoINT(sySin(particle->velZ) * 3 << 8);
					} else {
						particle->z -=
							FIXEDtoINT(sySin(particle->velZ) * 3 << 8);
					}
					particle->velZ += TRIG_TURN / 64;
					if(draw) {
						/* As an integer sum, the offset first. */
						texture.pixels = (Uint8 *)(closing *
								PICTURE_BYTES(ULYSSES_CRESCENT) +
							(Sint32)crescents);
						syInitMatrix();
						syTranslate(mx, my, 0);
						syRotX(TRIG_TURN * 3 / 4);
						syRotY(particle->z);
						syMatrixScaleUniform(scale);
						btRenderTransformVertices(
							BattleEffectGroundSquareCorners, verts, 4);
						btRenderDrawModelList(model);
					}
					t = particle->variant;
				}
				/* The strike lands: sparks fly, and every target is jolted and
				   flashes. */
				if(t == ULYSSES_STRIKE_HIT_TICK) {
					anchor->x = FIXEDtoINT(particle->x);
					anchor->y = FIXEDtoINT(particle->y);
					j = 0;
					{
						Sint32 first = i * ULYSSES_STRIKE_SPARKS;

						/* As an integer sum, the offset first. */
						spark = (PARTICLE *)(first * (Sint32)sizeof(*spark) +
							(Sint32)sparks);
					}
					for(; j != ULYSSES_STRIKE_SPARKS; j++, spark++) {
						Sint32 angle = syRandom() & TRIG_ANGLE_MASK;
						Sint32 speed = (syRandom() & 15) + 8;

						spark->variant = (syRandom() & 7) + 8;
						spark->x = particle->x;
						spark->y = particle->y;
						spark->velX = (sySin(angle) * speed) / 4;
						spark->velY = (syCos(angle) * speed) / 4;
					}
					for(j = 0; j != args->tgtCnt; j++) {
						Sint32 lift = (syRandom() << 2) + toFIXED(1);

						btEffectStartActorMotion(args->tgts[j], 1, toFIXED(2),
							toFIXED(8), lift, delay + (syRandom() & 31) + 52);
						btEffectTintBattler(
							args->tgts[j], SPRITE_COLOR_WHITE, TINT_KEEP, j, 4);
					}
				}
				/* A burst of light where it landed. */
				if(draw &&
					(Uint32)(particle->variant - ULYSSES_STRIKE_HIT_TICK) <
						ULYSSES_STRIKE_BURST_TICKS) {
					Sint32 k =
						((particle->variant - ULYSSES_STRIKE_HIT_TICK) / 2) % 6;

					blit[1](canvas,
						state->sheet + ULYSSES_YELLOW_ORB_PICTURES +
							k * PICTURE_BYTES(YELLOW_ORB),
						anchor->x - YELLOW_ORB_WIDTH / 2,
						anchor->y - YELLOW_ORB_HEIGHT / 2, YELLOW_ORB_WIDTH,
						YELLOW_ORB_HEIGHT);
				}
			}
			for(i = 0; i != ULYSSES_SPARK_COUNT; i++) {
				spark = &sparks[i];
				if(spark->variant >= 0) {
					Sint32 n = spark->variant / 4 + 2;

					if(draw) {
						blit[1](canvas, sprs + BattleEffectFlarePictures[n - 1],
							FIXEDtoINT(spark->x) - n, FIXEDtoINT(spark->y) - n,
							n * 2, n * 2);
					}
					btParticleAdvanceWithGravity2D(spark, 60, toFIXED(1) / 8);
					spark->variant--;
				}
			}
			syHeapRelease(model);
			syHeapRelease(verts);
		}

		if(draw && frm < ULYSSES_STRIKES_TICK) {
			void		 *verts;
			RENDER_MODEL *model;
			Sint32		  j;

			verts = syHeapAllocEWRAM(32);
			model = btRenderAllocateModelList(1);
			/* FAKEMATCH(dead): j takes tick * 3 + 1 here and 0 below before
			 * any read. */
			j = frm * 3 + 1;
			model->shade = 0;
			EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
			quadTexture.logWidth = 7;
			quadTexture.logHeight = 7;
			model->texture = &quadTexture;
			quadTexture.pixels = state->sheet + ULYSSES_RUNE_CIRCLE_PICTURES;
			model->kind = MODEL_SHADED_ADDITIVE;
			model->mesh = BattleEffectLargeSquareMesh;
			model->vertices = verts;
			j = 0;
			for(; j != 2; j++) {
				Sint32 t = frm - ULYSSES_RUNE_CIRCLES_TICK -
					j * ULYSSES_RUNE_CIRCLE_INTERVAL;

				if(t >= 0) {
					Sint32 scale;

					syInitMatrix();
					syTranslate(toFIXED(8), 0, 0);
					syRotX(DEGtoANG(90));
					if(j == 0) {
						scale = t * (toFIXED(3) / 64) - toFIXED(9) / 8;
						if(t < ULYSSES_RUNE_CIRCLE_HOLD_TICKS) {
							scale = t * (toFIXED(1) / 32);
							if(scale > toFIXED(3) / 4) {
								scale = toFIXED(3) / 4;
							}
						}
						syMatrixScaleUniform(scale);
						syRotY(-t * (TRIG_TURN / 128));
					} else {
						if(t >= ULYSSES_RUNE_CIRCLE_TILT_TICKS) {
							scale = toFIXED(7) / 4 - t * (toFIXED(1) / 32);
							syMatrixScaleUniform(scale);
						} else {
							syRotX(ULYSSES_RUNE_CIRCLE_TILT_X(t));
							syRotZ(ULYSSES_RUNE_CIRCLE_TILT_Z(t));
						}
						syRotY(t * (TRIG_TURN / 128));
					}
					btRenderTransformVertices(
						BattleEffectGroundSquareCorners, verts, 4);
					btRenderDrawModelList(model);
				}
			}
			syHeapRelease(model);
			syHeapRelease(verts);
		}

		/* The mage stands from ULYSSES_APPEAR_TICK, swaying once the strikes
		   begin. */
		if(frm >= ULYSSES_APPEAR_TICK && frm < ULYSSES_FLAKE_TICK) {
			mageScale = UlyssesMageScale;
			if(frm > ULYSSES_STRIKES_TICK - 1) {
				Sint32 u = frm - ULYSSES_STRIKES_TICK;

				mage.x = (sySin(u * (TRIG_TURN / 32)) << 1) + toFIXED(200);
				mage.z = (sySin(u * (TRIG_TURN / 64)) << 1) + toFIXED(84);
			}
			if(draw) {
				place.height = place.groundHeight = 0;
				for(i = 0; i != ULYSSES_MAGE_SPRITES; i++) {
					place.x = toFIXED(UlyssesObjectX[i]) + mage.x - toFIXED(32);
					place.y = toFIXED(UlyssesObjectZ[i]) + mage.z - toFIXED(32);
					fdRenderApplyProjectedPlacementStub(
						state->sprs[i], &place.x, (Sint32 *)&mageScale, 0);
				}
			}
		}
		/* Fire flares over the mage in a column of flames. */
		if(frm >= ULYSSES_FLARE_TICK) {
			if(frm == ULYSSES_FLARE_TICK) {
				PARTICLE *p;

				REG_BG2X = -(NARROW_CANVAS_PARTY_X << 8);
				REG_BG2PA = Q_8_8(1);
				REG_BG2PD = Q_8_8(1);
				for(i = 0, p = state->particles; i != ULYSSES_FLARE_FLAMES;
					i++, p++) {
					p->x = syRandom() % 48 + 40;
					p->y = syRandom() % 96 + 16;
					p->variant = -i / 2;
				}
			}
			for(i = 0, particle = state->particles; i != ULYSSES_FLARE_FLAMES;
				i++, particle++) {
				if((Uint32)particle->variant < ULYSSES_FLARE_FLAME_TICKS &&
					draw) {
					Sint32 k = (particle->variant / 4) % 6;
					Uint8 *src =
						state->sheet + BattleEffectEmberStreakPictures[k];
					Uint8 w = BattleEffectEmberStreakPictureWidths[k];

					blits[0](canvas, src, particle->x - (w >> 1),
						particle->y + BattleEffectEmberStreakPictureBiasY[k] -
							48,
						w, BattleEffectEmberStreakPictureHeights[k]);
				}
				particle->variant++;
			}
		}
		if(frm == ULYSSES_FLAKE_TICK) {
			sySetEvent(btUlyssesFlutterFlake, EVENT_PASS_TICK);
			ULYSSES_FLAKE.x = toFIXED(188);
			ULYSSES_FLAKE.y = toFIXED(96);
			ULYSSES_FLAKE.velX = 0;
			ULYSSES_FLAKE.velY = 0;
			state->frm = 0;
			flakeAway = 1;
		}
		if(draw) {
			EFFECT_END_TICK(state);
		}
	}

	EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
	REG_BG1CNT = BGCNT_PRIORITY(3) | BGCNT_256_COLORS |
		BGCNT_SCREEN_BASE(BATTLE_BG1_SCREEN_BLOCK);
	sySynch(1);
	/* FAKEMATCH(dead): a once-only block. */
	{
		void *dst = RamHeapSlots->battleCanvas;

		IWRAM_FILL_WORDS(dst, CANVAS_BYTES, 0);
	}
	IWRAM_FILL_WORDS((void *)BG_CHAR_BLOCK(1), CANVAS_VIEW_BYTES, 0);
	syWordsFill((void *)BG_CHAR_BLOCK(2), BATTLE_BACKGROUND_BYTES, 0);
	sySynch(1);
	REG_BLDALPHA = EFFECT_BLEND_CANVAS(CANVAS_BLEND_LEVEL);
	REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND |
		BLDCNT_TGT2_ALL;
	REG_DISPCNT = EFFECT_DISPCNT;
	syCloseEvent(btEffectTransferCanvas);
	EFFECT_RELEASE_BLITTERS();
	background->busy = 0;
	battle = RamHeapSlots->battleState;
	sySndPlayStub(SOUND_FADE_OUT_ACTION_SE);
	BgScrollWork[1].x = state->savedBg1X;
	BgScrollWork[1].y = state->savedBg1Y;
	ProjectionWork.centerX = BATTLE_PROJECTION_CENTER_X;
	ProjectionWork.centerY = BATTLE_PROJECTION_CENTER_Y;
	syWordsClear((void *)&BG_PLTT_COLOR(6, 0), 256);
	sySynch(1);
	btBackgroundLoadStub(BATTLE_BACKGROUND_ZOOMED, battle->bg, -1);
	REG_BG2CNT = BGCNT_PRIORITY(3) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
		BGCNT_SCREEN_BASE(7);
	syWordsClear((void *)BG_CHAR_BLOCK(1), CANVAS_VIEW_BYTES);
	syCloseEvent(btEffectDarkenBackgroundStep);
	BgScrollWork[1].y = BATTLE_BACKGROUND_SCROLL_Y;
	IO_WRITE_QUEUE_PUSH((Uint32)&REG_DISPCNT,
		DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON |
			DISPCNT_OBJ_ON | DISPCNT_WIN0_ON | DISPCNT_WIN1_ON,
		IO_WRITE_HALFWORD);
	REG_BLDCNT = 0;
	sySynch(1);
	/* The battle background's colours come back, red and blue first. */
	for(frm = 0; frm != 40; frm++) {
		Uint16			*src = battle->pal;
		volatile Uint16 *dst = (volatile Uint16 *)&BG_PLTT_COLOR(6, 0);

		for(i = 0; i != BATTLE_BACKGROUND_COLORS; i++, src++, dst++) {
			Sint32 v = *src;
			Sint32 r = RGB_LEVEL_MAX;
			Sint32 g;
			Sint32 b;
			Sint32 nr;
			Sint32 ng;
			Sint32 nb;

			r &= v;
			g = (Uint16)RGB_GREEN((Uint16)v);
			b = (Uint16)RGB_BLUE((Uint16)v);
			nr = r + frm - 30;
			ng = g + frm - 40;
			nb = b + frm - 30;
			if(nr < 0) {
				nr = 0;
			} else if(nr > r) {
				nr = r;
			}
			if(ng < 0) {
				ng = 0;
			} else if(ng > g) {
				ng = g;
			}
			if(nb < 0) {
				nb = 0;
			} else if(nb > b) {
				nb = b;
			}
			*dst = RGB(nr, ng, nb);
		}
		sySynch(1);
	}
	IO_WRITE_QUEUE_PUSH((Uint32)&REG_DISPCNT,
		DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG2_ON |
			DISPCNT_OBJ_ON | DISPCNT_WIN0_ON | DISPCNT_WIN1_ON,
		IO_WRITE_HALFWORD);
	if(flakeAway == 1) {
		syCloseEvent(btUlyssesFlutterFlake);
	}
	for(i = 0; i != ULYSSES_MAGE_SPRITES + 1; i++) {
		fdSprDestroyStub(state->sprs[i]);
	}
}

#undef ULYSSES_MAGE_SPRITES
#undef ULYSSES_FLAKE_SPRITE
#undef ULYSSES_FLAKE_ANIMATION
#undef ULYSSES_FLAKE
#undef ULYSSES_PICTURES
#undef ULYSSES_SILHOUETTE_WIDTH
#undef ULYSSES_SILHOUETTE_HEIGHT
#undef ULYSSES_FLAKE_PICTURES
#undef ULYSSES_RUNE_CIRCLE_PICTURES
#undef ULYSSES_YELLOW_ORB_PICTURES
#undef ULYSSES_CRESCENT_WIDTH
#undef ULYSSES_CRESCENT_HEIGHT
#undef ULYSSES_CRESCENT_SCALE
#undef ULYSSES_TICKS
#undef ULYSSES_RUNE_CIRCLES_TICK
#undef ULYSSES_RUNE_CIRCLE_INTERVAL
#undef ULYSSES_SILHOUETTE_TICK
#undef ULYSSES_APPEAR_TICK
#undef ULYSSES_RUNE_CIRCLES_FADE_TICK
#undef ULYSSES_STRIKES_TICK
#undef ULYSSES_FLARE_TICK
#undef ULYSSES_FLAKE_TICK
#undef ULYSSES_FLAKE_SWAY
#undef ULYSSES_SILHOUETTE_SWELL
#undef ULYSSES_GLOW_PALETTE
#undef ULYSSES_GLOW_COLORS
#undef ULYSSES_GLOW_TICKS
#undef ULYSSES_GLOW_END
#undef ULYSSES_GLOW_RED
#undef ULYSSES_GLOW_BLUE
#undef ULYSSES_RUNE_CIRCLE_HOLD_TICKS
#undef ULYSSES_RUNE_CIRCLE_TILT_TICKS
#undef ULYSSES_RUNE_CIRCLE_TILT_X
#undef ULYSSES_RUNE_CIRCLE_TILT_Z
#undef ULYSSES_BURST_FLAME_TICKS
#undef ULYSSES_FLARE_FLAME_TICKS
#undef ULYSSES_FLARE_FLAMES
#undef ULYSSES_STRIKES
#undef ULYSSES_STRIKE_INTERVAL
#undef ULYSSES_STRIKE_FLAME_TICKS
#undef ULYSSES_STRIKE_FLAKE_TICK
#undef ULYSSES_STRIKE_FLAKE_TICKS
#undef ULYSSES_STRIKE_CRESCENT_TICK
#undef ULYSSES_STRIKE_CRESCENT_TICKS
#undef ULYSSES_STRIKE_CLOSE_TICK
#undef ULYSSES_STRIKE_POWER_TICK
#undef ULYSSES_STRIKE_HIT_TICK
#undef ULYSSES_STRIKE_BURST_TICKS
#undef ULYSSES_SPARKS
#undef ULYSSES_STRIKE_SPARKS
#undef ULYSSES_SPARK_COUNT

/* The swirl swipe's modes (btEffectRunSwirlSwipe), each named for the effect
   that plays it. */
typedef enum SWIRL_SWIPE_MODE {
	SWIRL_SWIPE_POISON_BEAT,  /* lime; the target recoils at once, 48 ticks */
	SWIRL_SWIPE_SPINNING_BEAT /* magenta; the target is thrown up, 74 ticks */
} SWIRL_SWIPE_MODE;

/* A lime swirl sweeps across the first target and knocks it back.

   Abilities: Poison Beat. */
void btEffectRunPoisonBeat(BATTLE_EFFECT_PARM *args)
{
	btEffectRunSwirlSwipe(args, SWIRL_SWIPE_POISON_BEAT);
}

/* A magenta swirl sweeps across the first target and throws it into the air.

   Abilities: Spinning Beat. */
void btEffectRunSpinningBeat(BATTLE_EFFECT_PARM *args)
{
	btEffectRunSwirlSwipe(args, SWIRL_SWIPE_SPINNING_BEAT);
}

/* A swirl sweeps across the first target from the user's side. */
void btEffectRunSwirlSwipe(BATTLE_EFFECT_PARM *args, Sint32 mode)
{
	HEAP_SLOTS			*heap;
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	MOTION_OBJECT		*tgt;
	SCREEN_POINT		 pos;
	/* LEFTOVER(frame): draw[1], the pair's idle second blitter, which the ROM's
	 * tick reserves and never touches; as one DRAW_RECTANGLE. */
	DRAW_RECTANGLE draw[2];
	Sint32		   ticks;
	Sint32		   frm;
	Sint32		   picture;

	/* The canvas at its own width, and a transparent blitter, mirrored when the
	   targets are the party. */
	heap = RamHeapSlots;
	state = (BATTLE_EFFECT_STATE *)heap->battleEff;
	canvas = heap->battleCanvas;
	EFFECT_START_NARROW_CANVAS(0);
	if(args->side == EFFECT_SIDE_PARTY) {
		EFFECT_LOAD_BLITTER(
			HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_TRANSPARENT);
		draw[0] = heap->ramCode;
	} else {
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
			EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_TRANSPARENT);
		draw[0] = heap->ramCode;
	}
	btResourceLoadAndDecompress(
		RESOURCE_MAGENTA_SWIRL_SHEET, state->sheet, 1, 1);
	btResourceLoadAndDecompress(
		RESOURCE_MAGENTA_TAIL_SHEET, RamMapMetatileBuffer, 1, 0);
	if(mode == SWIRL_SWIPE_POISON_BEAT) {
		LOAD_BG_PALETTE(RESOURCE_LIME_PALETTE);
	}
	EFFECT_START_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

	/* The canvas is placed on the first target. */
	tgt = btObjectGetStub(args->tgts[0])->obj;
	btEffectPosProjectScaledBattler(args->tgts[0], &pos);
	if(args->side == EFFECT_SIDE_ENEMIES)
		REG_BG2X = (16 - pos.x) << 8;
	else
		REG_BG2X = (112 - pos.x) << 8;
	ticks = 74;
	if(mode != SWIRL_SWIPE_SPINNING_BEAT)
		ticks = 48;

	for(frm = 0; frm != ticks; frm++) {
		/* The swirl: six pictures, four ticks each, the last two from the
		   tail. */
		picture = frm / 4;
		if(picture <= 5) {
			if(picture <= 3)
				draw[0](canvas, state->sheet + SwirlSwipePictures[picture],
					SwirlSwipePictureX[picture + args->side * 6],
					SwirlSwipePictureBiasY[picture] + 32,
					SwirlSwipePictureWidths[picture],
					SwirlSwipePictureHeights[picture]);
			else
				draw[0](canvas,
					RamMapMetatileBuffer + SwirlSwipePictures[picture],
					SwirlSwipePictureX[picture + args->side * 6],
					SwirlSwipePictureBiasY[picture] + 32,
					SwirlSwipePictureWidths[picture],
					SwirlSwipePictureHeights[picture]);
		}
		if(frm == 8) {
			if(mode == SWIRL_SWIPE_POISON_BEAT) {
				EFFECT_SHOW_DAMAGE(SE_IMPACT);
				btMotionApplyKnockbackStub(args->tgts[0], KNOCKBACK_HOP);
			} else {
				sySndPlayStub(SE_HEAVY_IMPACT);
				btEffectTintBattler(
					args->tgts[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 4);
			}
			state->shakeFrm = 8;
		}
		/* Spinning Beat throws the target up at tick 13; it lands at tick
		   65. */
		if(mode == SWIRL_SWIPE_SPINNING_BEAT) {
			if(frm == 13) {
				tgt->velY = toFIXED(12);
				tgt->verticalMotionStrength = toFIXED(47) / 100;
				tgt->verticalMotionPhase = toFIXED(1) / 4;
			}
			if(frm == 65) {
				state->shakeFrm = 4;
				EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
			}
		}
		btCameraShake(8, 8);
		EFFECT_END_TICK(state);
	}

	EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#define SPIDER_WEB_TICKS		63
/* SpiderWebSheet's quarter of the web, a square drawn four times mirrored. */
#define SPIDER_WEB_QUARTER_SIZE 32

/* Builds the transparent blitter for one quarter. */
#define LOAD_QUARTER(flags) \
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, flags, BLITTER_TRANSPARENT)

/* Draws one quarter of the web with the blitter just built. */
#define DRAW_QUARTER(x, y) \
	(RamHeapSlots->ramCode)(canvas, state->sheet, x, y, size, size)

/* A spider web spreads over the targets in four mirrored quarters, fades, and
   Impair's effect follows.

   Abilities: Spider Web. */
void btEffectRunSpiderWeb(BATTLE_EFFECT_PARM *args)
{
	SCREEN_POINT		 middle;
	void				*canvas;
	BATTLE_EFFECT_STATE *state;
	volatile Uint16		*blend;
	Sint32				 size;
	Sint32				 flg;
	Sint32				 frm;
	Sint32				 i;

	/* The canvas, unseen at first, on the targets' middle. */
	canvas = RamHeapSlots->battleCanvas;
	state = RamHeapSlots->battleEff;
	EFFECT_START_NARROW_CANVAS(BGCNT_PRIORITY(2));
	REG_BLDALPHA = EFFECT_BLEND_CANVAS(0);
	btEffectGetTargetsMidpoint(&middle);
	REG_BG2X = (64 - middle.x) << 8;
	btResourceLoadAndDecompress(RESOURCE_SPIDER_WEB_SHEET, state->sheet, 1, 1);
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
	sySndPlayStub(SE_CRACK);
	blend = &REG_BLDALPHA;

	frm = 0;
	size = SPIDER_WEB_QUARTER_SIZE;
	for(; frm != SPIDER_WEB_TICKS; frm++) {
		/* It fades in over its first nine ticks and out over its last nine. */
		if(frm <= 8)
			*blend = EFFECT_BLEND_CANVAS(frm << 1);
		if(frm > 53)
			*blend = EFFECT_BLEND_CANVAS(124 - (frm << 1));

		/* The four quarters: the one picture as it is, mirrored, flipped, and
		   both. Each blitter is built, drawn with and released before the next
		   is built. */
		flg = EFFECT_CLIP_BOUNDS;
		LOAD_QUARTER(flg);
		DRAW_QUARTER(33, 41);
		syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
		flg = EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X;
		LOAD_QUARTER(flg);
		DRAW_QUARTER(64, 41);
		syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
		flg = EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y;
		LOAD_QUARTER(flg);
		DRAW_QUARTER(33, 72);
		syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
		flg = EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP;
		LOAD_QUARTER(flg);
		DRAW_QUARTER(64, 72);
		syHeapSlotRelease(HEAP_SLOT_RAM_CODE);

		/* Every target flashes at tick 10, and the damage shows at 32 with a
		   crack. */
		if(frm == 32)
			EFFECT_SHOW_DAMAGE(SE_CRACK);
		for(i = 0; i != args->tgtCnt; i++) {
			if(frm == 10)
				btEffectTintBattler(
					args->tgts[i], SPRITE_COLOR_WHITE, TINT_KEEP, i, 8);
		}
		EFFECT_END_TICK(state);
	}

	/* Both canvases are cleared, and Impair's effect follows: the targets'
	   Defense drops. */
	syCloseEvent(btEffectTransferCanvas);
	syWordsClear((void *)BG_CHAR_BLOCK(1), CANVAS_VIEW_BYTES);
	syWordsClear(canvas, CANVAS_VIEW_BYTES);
	args->prelude = 0;
	syCloseEvent(btEffectDarkenBackgroundStep);
	btEffectRunImpair(args);
}

#undef LOAD_QUARTER
#undef DRAW_QUARTER
#undef SPIDER_WEB_TICKS
#undef SPIDER_WEB_QUARTER_SIZE
