/* The Flora summon, Sabre Dance and Backstab, with the function that sets a
   battler's sprite priority and an unused one that sends a unit outward. Part
   of battle_effect.c. */

#include "enemy_ids.h"

/* Flora's summon. */

/* Flora is twelve 32x32 sprites in 256 colours, three across and four down, the
   first sprites of the effect state; then come her thirty-two 16x16 petals, the
   first fifteen each holding one of the tumbling petal's pictures
   (PinkPetalsTiles). */
#define FLORA_FIGURE_PIECES		  12
#define FLORA_FIGURE_COLUMNS	  3
#define FLORA_PETAL_SPRITES		  FLORA_FIGURE_PIECES
#define FLORA_PETAL_PICTURE_COUNT 15
#define FLORA_PETAL_COUNT		  32
#define FLORA_PETAL_SIZE		  16
#define FLORA_PETAL_PALETTE		  15
#define FLORA_COLORS			  240
/* A petal's phase counts its tumbling pictures in 256ths: its high byte is the
   picture it shows. */
#define FLORA_PHASE_ONE			  256
/* Petals 0 to 5 blow across the sky on paths of their own before Flora comes; 6
   to 15 stream from her hand towards the foes from FLORA_LAUNCH_TICK; 16 to 31
   burst out around her as she comes and again as she leaves. */
#define FLORA_PATH_PETALS		  6
#define FLORA_BURST_PETAL		  16
/* The storm: petals set out above the foes, of which one more falls each tick,
   sweeping down over them, until FLORA_STORM_DRAWN have fallen. Each foe is
   struck every FLORA_HIT_INTERVAL ticks, the first time at a random moment. */
#define FLORA_STORM_TICKS		  128
#define FLORA_STORM_PETALS		  128
#define FLORA_STORM_DRAWN		  64
#define FLORA_HIT_INTERVAL		  15
/* Where the storm's fifteen petal pictures are unpacked to a byte a pixel in
   the effect's pictures. */
#define FLORA_STORM_PICTURES	  4096

/* Flora's petals in EffectScratch: the tile number of each of the petal's
   pictures, each petal's phase, position and velocity, and the object palette
   as it fades. */
typedef struct FLORA_WORK {
	Uint16 tiles[FLORA_PETAL_PICTURE_COUNT];
	Uint16 phase[FLORA_PETAL_COUNT];
	Uint16 padding;
	Sint32 x[FLORA_PETAL_COUNT];
	Sint32 y[FLORA_PETAL_COUNT];
	Sint32 vx[FLORA_PETAL_COUNT];
	Sint32 vy[FLORA_PETAL_COUNT];
	Uint16 palette[FLORA_COLORS];
} FLORA_WORK;

/* The petals' state, kept in the particles' memory. */
#define FLORA_STATE ((FLORA_WORK *)petals)
/* Petal i's phase, read at FloraPetalTiles, FLORA_WORK's tile numbers from the
   second on. */
#define FLORA_PETAL_PHASE(i) \
	FloraPetalTiles[FLORA_PETAL_PICTURE_COUNT - 1 + (i)]

/* After the battle flashes away (SUMMON_OPENING_TICKS), the sky scene plays
   for FLORA_TICKS; A or B cuts it short after its first sixteen. */
#define FLORA_TICKS				 300
/* The canvas, shown at its own width FLORA_CANVAS_X pixels right of the
   screen's left edge until then, widens to twice it. */
#define FLORA_WIDE_TICK			 108
#define FLORA_CANVAS_X			 60
/* Flora appears in a flash of FLORA_FLASH_TICKS and fades in from white over
   FLORA_FADE_IN_TICKS. */
#define FLORA_APPEAR_TICK		 112
#define FLORA_FLASH_TICKS		 8
#define FLORA_FADE_IN_TICKS		 32
/* Petals stream from her hand from FLORA_LAUNCH_TICK, two ticks apart, as
   glows flare there; at FLORA_LEAVE_TICK she flies away. */
#define FLORA_LAUNCH_TICK		 223
#define FLORA_LEAVE_TICK		 252
/* The glows' pictures (FloraPetalPictures): shaded discs `height` pixels tall
   and half as wide, one after another. */
#define FLORA_GLOW_BYTES(height) ((height) / 2 * (height))
/* Where the glows flare on the canvas, by Flora's hand. */
#define FLORA_GLOW_X			 44
#define FLORA_GLOW_Y			 78
/* Ten copies of mote picture 3, each 7 levels dimmer than the one before, laid
   over the tenth mote picture from its 21st byte. Flora never draws either. */
#define FLORA_DIMMED_MOTE		 3
#define FLORA_DIMMED_MOTE_BYTES	 (FLORA_DIMMED_MOTE * FLORA_DIMMED_MOTE * 2)
#define FLORA_DIMMED_MOTE_COPIES 10
#define FLORA_DIMMED_MOTES		 590
/* The ring of motes: eight arms of four motes, spinning faster and faster from
   FLORA_RING_TICK, its spin at the start a 128th of a turn a tick. */
#define FLORA_RING_MOTES		 32
#define FLORA_RING_ARMS			 8
#define FLORA_ARM_MOTES			 (FLORA_RING_MOTES / FLORA_RING_ARMS)
#define FLORA_RING_SPIN			 (TRIG_TURN / 128)
#define FLORA_RING_TICK			 156
#define FLORA_RING_TICKS		 75
/* The four rings of light (FireRingTexture): three as Flora appears and one as
   she leaves. */
#define FLORA_LIGHT_RINGS		 4
/* The centre of the ring of motes and of the wheel, on the canvas. */
#define FLORA_RING_X			 48
#define FLORA_RING_Y			 92
/* The wheel: a ring of dots and one three quarters its size, joined by four
   spokes, which from FLORA_WHEEL_TICK turns, tilts flat and slides to the
   rings' centre, narrowing as it brightens to FLORA_WHEEL_GLOW; as Flora leaves
   it widens and fades. */
#define FLORA_WHEEL_DOTS		 128
#define FLORA_WHEEL_SPOKES		 4
#define FLORA_WHEEL_GLOW		 36
#define FLORA_WHEEL_TICK		 186
#define FLORA_WHEEL_TICKS		 80

/* The effect state's sprite sheet. */
static inline Uint8 *SheetOf(BATTLE_EFFECT_STATE *owner)
{
	/* FAKEMATCH(wrapper): the sprite sheet's address through this inline. */
	return owner->sheet;
}

/* Flora: the battle flashes away to a violet sky where petals blow by. Flora
   appears in a flash amid rings of light and a burst of petals and circles; she
   drifts left as a ring of motes spins and a wheel of light turns by her hand,
   where glows flare and petals stream out, and flies away as a last ring of
   light flies out. A or B skips the sky scene. Then a storm of petals sweeps
   over the foes.

   Abilities: Flora. */
void btEffectRunFlora(BATTLE_EFFECT_PARM *args)
{
	Uint8					*canvas;
	BATTLE_EFFECT_STATE		*state;
	DRAW_RECTANGLE			 draw[2];
	Sint32					 frame;
	Sint32					 savedScroll;
	BATTLE_BACKGROUND_STATE *backdrop;
	Uint8					*motes;
	Sint32					 bgX;
	FIXED					 posX;
	FIXED					 posY;
	Sint32					 angle;
	Sint32					 spin;
	Sint32					 spread;
	Sint32					 twist;
	Uint8					*petals;
	Sint32					 i;
	Sint32					 j;
	Sint32					 order;
	Sint32					 size;
	Sint32					 dx;
	Sint32					 dy;
	Uint32					 shown;
	SPR_PLACEMENT			 place;
	SCALE					 floraScale;
	MODEL_TEXTURE			 texture;
	RENDER_MODEL			*model;
	void					*verts;
	PARTICLE				*p;
	Uint8					*cells;
	Uint16					*phase;
	Uint8					*src;
	Uint8					*resource;
	SCREEN_POINT			 out;
	SCREEN_POINT			 pos;
	Uint8					 dot[2];
	SCREEN_POINT			 ringScreen;
	SCREEN_POINT			 ringPoint;
	SCREEN_POINT			 fadeRingScreen;
	SCREEN_POINT			 fadeRingPoint;

	canvas = RamHeapSlots->battleCanvas;
	state = RamHeapSlots->battleEffect;
	savedScroll = BgScrollWork[1].x;
	backdrop = RamHeapSlots->battleBg;
	petals = EffectScratch;
	src = RamMapMetatileBuffer;
	motes = RamHeapSlots->battleSheet;
	btEffectStartCanvasLayer(0);
	btEffectConfigureEffectDisplay();
	btEffectFillWhiteBand();
	state->transferMode = CANVAS_COPY;
	sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);
	/* The battle flashes away; then the sky, the petals and Flora load. */
	bgX = -80;
	for(frame = 0; frame != SUMMON_OPENING_TICKS; frame++) {
		if(frame == SUMMON_OPENING_LOAD_TICK) {
			ProjectionWork.centerY = SUMMON_PROJECTION_CENTER_Y;
			btEffectShowLivingTargets(state->args);
			btBackgroundLoadStub(
				BATTLE_BACKGROUND_FIXED, RESOURCE_VIOLET_SKY_BACKDROP, 0);
			state->scrollTimer = 0;
			state->scrollInterval = 1;
			state->scrollStepX = -3;
			state->scrollStepY = 0;
			sySetEvent(btEffectAdvanceScrollOnInterval, EVENT_PASS_VBLANK);
			backdrop->busy = 1;
			resource = syResourceGetTableEntry(RESOURCE_PINK_PETALS_TILES);
			syWordsCopyEntry((void *)&OBJ_PLTT_COLOR(FLORA_PETAL_PALETTE, 0),
				resource, PALETTE_BANK_BYTES);
			resource += PALETTE_BANK_BYTES;
			syDecompressTaggedLZ(resource, src);
			cells = RamMapMetatileBuffer;
			for(i = 0; i != FLORA_PETAL_PICTURE_COUNT; i++) {
				SPR *object = btEffectSpriteCreateWithTiles(
					16, 16, OBJ_SIZE_16X16, OBJ_PALETTE(FLORA_PETAL_PALETTE));
				state->sprites[FLORA_PETAL_SPRITES + i] = object;
				object->image.priority = 2;
				syWordsCopyEntry((void *)OBJ_TILES(object),
					cells + i * OBJ_16X16_4BPP_BYTES, OBJ_16X16_4BPP_BYTES);
				FLORA_STATE->tiles[i] =
					((SPR *)state->sprites[FLORA_PETAL_SPRITES + i])
						->image.tile;
			}
			for(i = 0; i != FLORA_PETAL_COUNT - FLORA_PETAL_PICTURE_COUNT;
				i++) {
				SPR *object = btEffectSpriteAcquire(
					16, 16, OBJ_SIZE_16X16, OBJ_PALETTE(FLORA_PETAL_PALETTE));
				state->sprites[FLORA_PETAL_SPRITES + FLORA_PETAL_PICTURE_COUNT +
					i] = object;
				syWordsCopyEntry(object, state->sprites[FLORA_PETAL_SPRITES],
					offsetof(SPR, lastNo));
			}
			/* Each petal starts at a picture of its own. */
			i = 0;
			phase = &FLORA_PETAL_PHASE(0);
			for(; i != FLORA_PETAL_COUNT; i++, phase++) {
				*phase = (syRandom() % FLORA_PETAL_PICTURE_COUNT) << 8;
				((SPR *)state->sprites[FLORA_PETAL_SPRITES + i])->image.tile =
					FLORA_STATE->tiles[*phase / FLORA_PHASE_ONE];
			}
			resource =
				syResourceGetTableEntry(RESOURCE_GREEN_HAIRED_MAIDEN_TILES);
			resource += FLORA_COLORS * 2;
			syDecompressTaggedLZ(resource, src);
			cells = RamMapMetatileBuffer;
			for(i = 0; i != FLORA_FIGURE_PIECES; i++) {
				SPR *object = btEffectSpriteCreateWithTiles(
					32, 32, OBJ_SIZE_32X32 | OBJ_256_COLORS, 0);
				object->image.priority = 2;
				state->sprites[i] = object;
				syWordsCopyEntry((void *)OBJ_TILES(object),
					cells + i * OBJ_32X32_8BPP_BYTES, OBJ_32X32_8BPP_BYTES);
			}
			btResourceLoadAndDecompress(
				RESOURCE_FIRE_RING_TEXTURE, state->sheet, 0, 0);
		}
		bgX += 4;
		REG_BG2X = bgX << 8;
		EFFECT_PRESENT_CANVAS(state);
	}
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, src, 0, 0);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, motes, 0, 0);
	/* DEAD: the dimmed mote copies, which nothing reads; the ROM keeps the
	   loop. */
	size = FLORA_DIMMED_MOTE;
	for(i = 0; i != FLORA_DIMMED_MOTE_COPIES; i++) {
		for(j = 0; j != FLORA_DIMMED_MOTE_BYTES; j++) {
			Sint32 v = src[BattleEffectMotePictures[size - 1] + j];

			if(v > 0) {
				v = CANVAS_LEVEL_BRIGHTEST - i * 7;
				if(v <= 0) {
					v = 1;
				}
			}
			motes[i * FLORA_DIMMED_MOTE_BYTES + FLORA_DIMMED_MOTES + j] = v;
		}
	}
	REG_DISPCNT = EFFECT_DISPCNT;
	REG_BG2PA = Q_8_8(1);
	REG_BLDALPHA = EFFECT_BLEND_CANVAS(CANVAS_BLEND_LEVEL);
	REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	REG_BG2X = 0;
	/* DEAD: Flora's place, and the ring's spin, spread and twist, are each set
	   again before they are read; the ROM keeps the stores. */
	posX = 128;
	posY = 44;
	angle = 0;
	spin = FLORA_RING_SPIN;
	spread = 0;
	twist = 0;
	/* DEAD: the petals laid out in rows of eight, in 65536ths of a pixel; the
	   first tick places every petal again; the ROM keeps the loop. */
	for(i = 0; i != FLORA_PETAL_COUNT; i++) {
		Sint32 row = i / 8;
		Sint32 leftX = 256;
		Sint32 columnX;
		Sint32 rowY;

		columnX = (i - row * 8) << 5;
		rowY = row * 22;
		FLORA_STATE->x[i] = columnX + leftX;
		FLORA_STATE->y[i] = rowY + 128;
	}
	EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
	draw[0] = RamHeapSlots->ramCode;
	for(frame = 0; frame != FLORA_TICKS &&
		(!(PerWork.repeat & (KEY_A | KEY_B)) || frame <= 16);
		frame++) {
		if(frame == 0) {
			syWordsClear((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
			syWordsClearEntry(canvas, CANVAS_BYTES);
			for(i = 0; i != FLORA_COLORS; i++) {
				FLORA_STATE->palette[i] = RGB_WHITE;
			}
			syWordsCopyEntry(
				(void *)OBJ_PLTT, FloraSpritePalettes, FLORA_COLORS * 2);
			REG_BG2PA = Q_8_8(1);
			btGraphicsLoadGreyRamp();
			REG_BG2X = -(FLORA_CANVAS_X << 8);
		}
		/* The canvas's colours turn a level a tick towards Mars Djinn's, then
		   tan, then cyan. */
		if(frame > 285) {
			btEffectStepPaletteToResource(RESOURCE_CYAN_PALETTE);
		} else if(frame > 199) {
			btEffectStepPaletteToResource(RESOURCE_TAN_PALETTE);
		} else if(frame > 180) {
			btEffectStepPaletteToResource(RESOURCE_MARS_DJINN_SHEET);
		}
		if(frame == FLORA_WIDE_TICK) {
			REG_BG2PA = Q_8_8(0.5);
			REG_BG2X = 0;
			/* A quarter of each of the four glows: 8, 16, 32 and 48 pixels
			   tall. */
			btEffectDrawShadedCircles(src, 8, toFIXED(1));
			btEffectDrawShadedCircles(
				RamMapMetatileBuffer + FLORA_GLOW_BYTES(8), 16, toFIXED(3) / 4);
			btEffectDrawShadedCircles(RamMapMetatileBuffer +
					FLORA_GLOW_BYTES(8) + FLORA_GLOW_BYTES(16),
				32, toFIXED(3) / 4);
			btEffectDrawShadedCircles(RamMapMetatileBuffer +
					FLORA_GLOW_BYTES(8) + FLORA_GLOW_BYTES(16) +
					FLORA_GLOW_BYTES(32),
				48, toFIXED(3) / 4);
		}
		/* A glow of a random size flares by her hand each tick until she
		   leaves. */
		if((Uint32)(frame - FLORA_LAUNCH_TICK) <=
			FLORA_LEAVE_TICK - FLORA_LAUNCH_TICK) {
			Sint32 k = syRandom() & 3;
			Sint32 n = FloraPetalSizes[k] * 2;

			btCanvasBlitMirroredQuarters(
				src + FloraPetalPictures[k], FLORA_GLOW_X, FLORA_GLOW_Y, n);
		}
		/* Flora fades in from white, a level a tick. */
		shown = frame - FLORA_APPEAR_TICK;
		if(shown < FLORA_FADE_IN_TICKS) {
			Uint16 *resource =
				syResourceGetTableEntry(RESOURCE_GREEN_HAIRED_MAIDEN_TILES);

			for(i = 0; i != FLORA_COLORS; i++, resource++) {
				Sint32 c = FLORA_STATE->palette[i];
				Sint32 b = RGB_BLUE((Uint16)c);
				Sint32 g = RGB_GREEN((Uint16)c);
				Sint32 r = RGB_RED(c);
				Sint32 t = *resource;
				Sint32 tb = RGB_BLUE((Uint16)t);
				Sint32 tg = RGB_GREEN((Uint16)t);
				Sint32 tr = RGB_RED(t);

				if(b > tb) {
					b--;
				}
				if(g > tg) {
					g--;
				}
				if(r > tr) {
					r--;
				}
				FLORA_STATE->palette[i] = RGB(r, g, b);
			}
			syWordsCopyEntry(
				(void *)OBJ_PLTT, FloraSpritePalettes, FLORA_COLORS * 2);
		}
		if(shown < FLORA_FLASH_TICKS) {
			syWordsFillEntry(canvas, CANVAS_BYTES,
				CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST) -
					shown * CANVAS_FILL_WORD(1) * 8);
		}
		floraScale = FloraUnitScale;
		dy = 0;
		place.groundHeight = 0;
		dx = 0;
		place.height = toFIXED(255);
		/* Flora circles about (152, 44), 24 pixels out across and 20 up and
		   down, then drifts left and down; from tick 222 she trembles and the
		   sky shakes, and as she leaves she flies up and away to the right. */
		if(frame <= 119) {
			posX = sySin(TRIG_TURN * 205 / 256) * 24 + toFIXED(152);
			posY = syCos(TRIG_TURN * 205 / 256) * 20 + toFIXED(44);
		} else if(frame <= 179) {
			Sint32 a = frame * 3 * 256 - TRIG_TURN * 19 / 32;

			posX = sySin(a) * 24 + toFIXED(152);
			posY = syCos(a) * 20 + toFIXED(44);
		} else if(frame <= 221) {
			posX += -toFIXED(9) / 8;
			posY += toFIXED(1) / 2;
		} else if(frame < FLORA_LEAVE_TICK) {
			Sint32 m = frame % 6;

			if(m == 1) {
				dx = 1;
			}
			if(m == 2) {
				dx = 1;
			}
			if(m == 3) {
				dx = 2;
				dy = -1;
			}
			if(m == 4) {
				dx = 2;
				dy = -1;
			}
			if(m == 5) {
				dx = 2;
				dy = -1;
			}
			if(m == 0) {
				dx = 0;
				dy = 0;
			}
			if(frame % 4 == 0) {
				BgScrollWork[1].y = BATTLE_BACKGROUND_SCROLL_Y - 2;
			}
			if(frame % 4 == 1) {
				BgScrollWork[1].y = BATTLE_BACKGROUND_SCROLL_Y - 2;
			}
			if(frame % 4 == 2) {
				BgScrollWork[1].y = BATTLE_BACKGROUND_SCROLL_Y + 2;
			}
			if(frame % 4 == 3) {
				BgScrollWork[1].y = BATTLE_BACKGROUND_SCROLL_Y + 2;
			}
		} else {
			BgScrollWork[1].y = BATTLE_BACKGROUND_SCROLL_Y;
			posX += toFIXED(8);
			posY += -toFIXED(6);
		}
		if(frame >= FLORA_APPEAR_TICK) {
			for(i = 0; i != FLORA_FIGURE_PIECES; i++) {
				place.x = posX + toFIXED(i % FLORA_FIGURE_COLUMNS * 32 + dx);
				place.y = posY + toFIXED(i / FLORA_FIGURE_COLUMNS * 32 + dy);
				fdRenderApplyProjectedPlacementStub(state->sprites[i],
					sySpritePlacementValues(&place), (Sint32 *)&floraScale, 0);
			}
		}
		/* Petals 0 to 5 blow along paths of their own and are put away off the
		   screen as Flora appears. */
		for(i = 0; i != FLORA_PETAL_COUNT; i++) {
			Sint32 launchFrame =
				FLORA_LAUNCH_TICK - FLORA_PATH_PETALS * 2 + i * 2;

			if(i == 0) {
				if(frame == 0) {
					FLORA_STATE->x[i] = toFIXED(240);
					FLORA_STATE->y[i] = toFIXED(48);
				} else if(frame <= 47) {
					FLORA_STATE->x[i] += -toFIXED(3);
					FLORA_STATE->y[i] += toFIXED(3) / 2;
				} else if(frame <= 79) {
					FLORA_STATE->x[i] += -toFIXED(1);
					FLORA_STATE->y[i] += -toFIXED(1);
				} else if(frame < FLORA_APPEAR_TICK) {
					FLORA_STATE->x[i] += toFIXED(3);
					/* DEAD: y moves by 0; the ROM keeps its load and store. */
					FLORA_STATE->y[i] += 0;
				}
				if(frame == FLORA_APPEAR_TICK) {
					FLORA_STATE->x[i] = toFIXED(256);
				}
			} else if(i == 1) {
				if(frame <= 27) {
					FLORA_STATE->x[i] = toFIXED(256);
					FLORA_STATE->y[i] = toFIXED(48);
				} else if(frame == 28) {
					FLORA_STATE->x[i] = toFIXED(16);
					FLORA_STATE->y[i] = toFIXED(32);
				} else if(frame <= 62) {
					FLORA_STATE->x[i] += toFIXED(1.83924);
					FLORA_STATE->y[i] += toFIXED(1.99183);
				} else if(frame <= 79) {
					FLORA_STATE->x[i] += toFIXED(1.61036);
					FLORA_STATE->y[i] += toFIXED(0.4026);
				} else if(frame <= 141) {
					FLORA_STATE->x[i] += toFIXED(3);
					/* DEAD: y moves by 0; the ROM keeps its load and store. */
					FLORA_STATE->y[i] += 0;
				}
				if(frame == FLORA_APPEAR_TICK) {
					FLORA_STATE->x[i] = toFIXED(256);
				}
			} else if(i == 2) {
				if(frame <= 49) {
					FLORA_STATE->x[i] = toFIXED(256);
					FLORA_STATE->y[i] = toFIXED(48);
				} else if(frame == 50) {
					FLORA_STATE->x[i] = 0;
					FLORA_STATE->y[i] = toFIXED(124);
				} else if(frame <= 62) {
					FLORA_STATE->x[i] += toFIXED(4);
					FLORA_STATE->y[i] += -toFIXED(5) / 4;
				} else if(frame <= 93) {
					FLORA_STATE->x[i] += toFIXED(3);
					FLORA_STATE->y[i] += toFIXED(3) / 4;
				} else if(frame <= 125) {
					FLORA_STATE->x[i] += toFIXED(3);
					FLORA_STATE->y[i] += -toFIXED(1);
				}
				if(frame == FLORA_APPEAR_TICK) {
					FLORA_STATE->x[i] = toFIXED(256);
				}
			} else if(i == 3) {
				if(frame <= 49) {
					FLORA_STATE->x[i] = toFIXED(256);
					FLORA_STATE->y[i] = toFIXED(48);
				} else if(frame == 50) {
					FLORA_STATE->x[i] = 0;
					FLORA_STATE->y[i] = toFIXED(108);
				} else if(frame <= 103) {
					FLORA_STATE->x[i] += toFIXED(3);
					FLORA_STATE->y[i] += -toFIXED(5) / 4;
				} else if(frame <= 135) {
					FLORA_STATE->x[i] += toFIXED(1);
					FLORA_STATE->y[i] += toFIXED(1);
				}
				if(frame == FLORA_APPEAR_TICK) {
					FLORA_STATE->x[i] = toFIXED(256);
				}
			} else if(i == 4) {
				if(frame <= 59) {
					FLORA_STATE->x[i] = toFIXED(256);
					FLORA_STATE->y[i] = toFIXED(48);
				} else if(frame == 60) {
					FLORA_STATE->x[i] = toFIXED(16);
					FLORA_STATE->y[i] = toFIXED(32);
				} else if(frame <= 82) {
					FLORA_STATE->x[i] += toFIXED(4);
					FLORA_STATE->y[i] += toFIXED(3);
				} else if(frame <= 135) {
					FLORA_STATE->x[i] += toFIXED(4);
				}
				if(frame == FLORA_APPEAR_TICK) {
					FLORA_STATE->x[i] = toFIXED(256);
				}
			} else if(i == 5) {
				if(frame <= 85) {
					FLORA_STATE->x[i] = toFIXED(256);
					FLORA_STATE->y[i] = toFIXED(48);
				} else if(frame == 86) {
					FLORA_STATE->x[i] = toFIXED(144);
					FLORA_STATE->y[i] = toFIXED(32);
				} else if(frame <= 135) {
					FLORA_STATE->x[i] += toFIXED(1);
					FLORA_STATE->y[i] += toFIXED(3);
				}
				if(frame == FLORA_APPEAR_TICK) {
					FLORA_STATE->x[i] = toFIXED(256);
				}
			} else if(i < FLORA_BURST_PETAL) {
				/* Petals 6 to 15 wait off the screen; then each streams down
				   and to the left from Flora's hand, and starts again from
				   there each time it leaves the screen, until she leaves. */
				if(frame == 0) {
					FLORA_STATE->x[i] = toFIXED(248);
					FLORA_STATE->y[i] = frame;
				} else if(frame == launchFrame) {
					FLORA_STATE->x[i] = toFIXED(syRandom() & 7) + toFIXED(92);
					FLORA_STATE->y[i] = toFIXED(syRandom() & 7) + toFIXED(104);
					FLORA_STATE->vx[i] =
						(syRandom() & TRIG_ANGLE_MASK) * 12 - toFIXED(12);
					FLORA_STATE->vy[i] = toFIXED(4);
				} else if(frame >= launchFrame) {
					if(FLORA_STATE->x[i] >= -toFIXED(16) &&
						FLORA_STATE->y[i] <= toFIXED(168)) {
						FLORA_STATE->x[i] += FLORA_STATE->vx[i];
						FLORA_STATE->y[i] += FLORA_STATE->vy[i];
					} else if(frame < FLORA_LEAVE_TICK) {
						FLORA_STATE->x[i] =
							toFIXED(syRandom() & 7) + toFIXED(92);
						FLORA_STATE->y[i] =
							toFIXED(syRandom() & 7) + toFIXED(104);
					}
				}
			} else if(i < FLORA_PETAL_COUNT) {
				/* Petals 16 to 31 burst out in every direction as Flora appears
				   and again from her hand as she leaves, at 1 to 3 pixels a
				   tick (in 8.8), slowing until they are put away off the
				   screen. */
				if(frame == 0) {
					FLORA_STATE->x[i] = toFIXED(248);
					FLORA_STATE->y[i] = frame;
				} else if(frame == FLORA_APPEAR_TICK) {
					Uint32 a = syRandom() & TRIG_ANGLE_MASK;
					Sint32 speed = ((syRandom() & 255) << 1) + Q_8_8(1);

					FLORA_STATE->x[i] = toFIXED(syRandom() & 7) + toFIXED(168);
					FLORA_STATE->y[i] = toFIXED(syRandom() & 7) + toFIXED(92);
					FLORA_STATE->vx[i] = sySin(a) * speed / Q_8_8(1);
					FLORA_STATE->vy[i] = syCos(a) * speed / Q_8_8(1);
				} else if(frame == FLORA_LEAVE_TICK) {
					Uint32 a = syRandom() & TRIG_ANGLE_MASK;
					Sint32 speed = ((syRandom() & 255) << 1) + Q_8_8(1);

					FLORA_STATE->x[i] = toFIXED(syRandom() & 7) + toFIXED(92);
					FLORA_STATE->y[i] = toFIXED(syRandom() & 7) + toFIXED(104);
					FLORA_STATE->vx[i] = sySin(a) * speed / Q_8_8(1);
					FLORA_STATE->vy[i] = syCos(a) * speed / Q_8_8(1);
				} else if(frame >= FLORA_APPEAR_TICK) {
					FLORA_STATE->x[i] += FLORA_STATE->vx[i];
					FLORA_STATE->y[i] += FLORA_STATE->vy[i];
					FLORA_STATE->vx[i] =
						FLORA_STATE->vx[i] * 63 / DAMPING_SCALE;
					FLORA_STATE->vy[i] =
						FLORA_STATE->vy[i] * 63 / DAMPING_SCALE;
					if((Uint32)(FLORA_STATE->vx[i] + toFIXED(1) - 1) <=
							2 * (toFIXED(1) - 1) &&
						(Uint32)(FLORA_STATE->vy[i] + toFIXED(1) - 1) <=
							2 * (toFIXED(1) - 1)) {
						FLORA_STATE->x[i] = toFIXED(256);
						FLORA_STATE->y[i] = 0;
					}
				}
			}
			/* Each petal tumbles, those on paths an eighth of a picture a tick
			   and the rest half a picture. */
			if(i < FLORA_PATH_PETALS) {
				FLORA_PETAL_PHASE(i) =
					(FLORA_PETAL_PHASE(i) + FLORA_PHASE_ONE / 8) %
					(FLORA_PETAL_PICTURE_COUNT << 8);
			} else {
				FLORA_PETAL_PHASE(i) =
					(FLORA_PETAL_PHASE(i) + FLORA_PHASE_ONE / 2) %
					(FLORA_PETAL_PICTURE_COUNT << 8);
			}
			((SPR *)state->sprites[FLORA_PETAL_SPRITES + i])->image.tile =
				FLORA_STATE->tiles[FLORA_PETAL_PHASE(i) >> 8];
		}
		for(i = 0; i != FLORA_PETAL_COUNT; i++) {
			place.x = FLORA_STATE->x[i] - toFIXED(FLORA_PETAL_SIZE / 2);
			place.y = FLORA_STATE->y[i] - toFIXED(FLORA_PETAL_SIZE / 2);
			fdRenderApplyProjectedPlacementStub(
				state->sprites[FLORA_PETAL_SPRITES + i],
				sySpritePlacementValues(&place), (Sint32 *)&floraScale, 0);
		}
		if(frame == 114) {
			sySoundPlayStub(SE_POWER_UP);
		}
		verts = syHeapAllocateEWRAM(512);
		model = btRenderAllocateModelList(1);
		texture.logWidth = 7;
		texture.logHeight = 7;
		texture.pixels = state->sheet;
		model->kind = MODEL_SHADED_ADDITIVE;
		model->texture = &texture;
		model->mesh = BattleEffectLargeSquareMesh;
		model->vertices = verts;
		/* Each ring of light grows an eighth of its size a tick as it turns,
		   and fades from its eighth tick. */
		for(i = 0; i != FLORA_LIGHT_RINGS; i++) {
			Sint32 age = frame - FloraBloomTicks[i];

			if(age >= 0) {
				Sint32 scale = age * (toFIXED(1) / 8);
				Sint32 shade = 0;

				if(age > 7) {
					shade = (8 - age) * 6;
				}
				model->shade = shade;
				if(shade > MODEL_SHADE_INVISIBLE) {
					syInitMatrix();
					syTranslate(
						toFIXED(FloraBloomX[i]), toFIXED(FloraBloomY[i]), 0);
					syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1) / 2);
					syRotZ(FloraBloomRolls[i]);
					syRotX(FloraBloomPitches[i]);
					syMatrixScaleUniform(scale);
					syRotY(frame * (TRIG_TURN / 32));
					btRenderTransformVertices(
						BattleEffectGroundSquareCorners, verts, 4);
					btRenderDrawModelList(model);
				}
			}
		}
		syHeapRelease(model);
		syHeapRelease(verts);
		syInitMatrix();
		syRotX(TRIG_TURN * 37 / 64);
		syRotY(TRIG_TURN * 13 / 32);
		if(frame == FLORA_RING_TICK) {
			spin = FLORA_RING_SPIN;
			angle = 0;
		}
		syRotZ(angle);
		if(frame == FLORA_RING_TICK) {
			sySoundPlayStub(SE_SWIRL);
		}
		if(frame == FLORA_LAUNCH_TICK) {
			sySoundPlayStub(SE_LAUNCH);
		}
		if(frame == FLORA_LEAVE_TICK) {
			sySoundPlayStub(SE_POWER_UP);
		}
		/* The ring of motes closes on its centre as it spins; until tick 194
		   its arms twist and spread, and from tick 208 the motes of each arm
		   shrink into the centre one by one, the outermost first. */
		if((Uint32)(frame - FLORA_RING_TICK) < FLORA_RING_TICKS) {
			Sint32 ringRadius = 80 - (frame - FLORA_RING_TICK) / 2;

			if(frame == FLORA_RING_TICK) {
				spread = 0;
				twist = 0;
			}
			if(frame <= 194) {
				twist += TRIG_TURN / 2048;
				spread += 3;
			}
			spin += TRIG_TURN / 4096;
			angle += spin;
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
				BLITTER_ADDITIVE);
			draw[1] = RamHeapSlots->ramCodePaired;
			for(i = 0; i != FLORA_RING_MOTES; i++) {
				Sint32 a =
					(i / FLORA_ARM_MOTES) * (TRIG_TURN / FLORA_RING_ARMS);
				Sint32 m = i % FLORA_ARM_MOTES;
				Sint32 b = m * twist;
				Sint32 radius = ringRadius + m * spread / 30;
				Sint32 x;
				Sint32 y;

				size = 7;
				for(order = 0; order != FLORA_ARM_MOTES; order++) {
					if(frame >= 208 + order * 4 &&
						i % FLORA_ARM_MOTES == FLORA_ARM_MOTES - 1 - order) {
						Sint32 late = frame - order * 4 - 208;

						radius = ringRadius - late * 4;
						if(radius < 0) {
							radius = 0;
						}
						size = 7 - late;
						if(size <= 2) {
							size = 3;
						}
					}
				}
				pos.x = FIXEDtoINT(sySin(a + b) * radius);
				pos.y = FIXEDtoINT(syCos(a + b) * radius);
				pos.depth = 0;
				btEffectPosProjectPoint(&pos, &out);
				out.x >>= 1;
				x = out.x + FLORA_RING_X;
				y = out.y + FLORA_RING_Y;
				draw[1](canvas, motes + BattleEffectMotePictures[size - 1],
					x - size / 2, y - size, size, size * 2);
			}
			syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
		}
		if((Uint32)(frame - FLORA_WHEEL_TICK) < FLORA_WHEEL_TICKS) {
			Sint32 glow = frame - FLORA_WHEEL_TICK;
			Sint32 radius;
			Sint32 level;
			Sint32 centerX;
			Sint32 outerY;
			Sint32 innerY;
			Sint32 driftX;
			Sint32 driftY;
			Sint32 x1;
			Sint32 y1;
			Sint32 x2;
			Sint32 y2;

			if(frame > 247) {
				glow = 532 - frame * 2;
			} else if(glow > FLORA_WHEEL_GLOW) {
				glow = FLORA_WHEEL_GLOW;
			}
			radius = 64 - glow;
			level = glow;
			driftX = FLORA_WHEEL_GLOW / 2 - level / 2;
			driftY = FLORA_WHEEL_GLOW - level;
			dot[0] = level;
			dot[1] = level;
			syRotZ(frame * (TRIG_TURN / 64));
			if(frame <= 221) {
				syRotX(driftY * (TRIG_TURN / 64));
			}
			if(driftX < 0) {
				driftX = 0;
			}
			if(driftY < 0) {
				driftY = 0;
			}
			centerX = driftX + FLORA_RING_X;
			outerY = FLORA_RING_Y - driftY;
			innerY = FLORA_RING_Y - 4 - driftY;
			for(i = 0; i != FLORA_WHEEL_DOTS; i++) {
				ringPoint.x = FIXEDtoINT(
					sySin(i * (TRIG_TURN / FLORA_WHEEL_DOTS)) * radius);
				ringPoint.y = FIXEDtoINT(
					syCos(i * (TRIG_TURN / FLORA_WHEEL_DOTS)) * radius);
				ringPoint.depth = 0;
				btEffectPosProjectPoint(&ringPoint, &ringScreen);
				ringScreen.x >>= 1;
				x1 = ringScreen.x + centerX;
				y1 = ringScreen.y + outerY;
				x2 = ringScreen.x * 3 / 4 + centerX;
				y2 = ringScreen.y * 3 / 4 + innerY;
				draw[0](canvas, dot, x1 - 1, y1 - 1, 1, 2);
				draw[0](canvas, dot, x2 - 1, y2 - 1, 1, 2);
				if((i & (FLORA_WHEEL_DOTS / FLORA_WHEEL_SPOKES - 1)) == 0) {
					btEffectDrawClippedCanvasLine(x1, y1, x2, y2, glow);
					btEffectDrawClippedCanvasLine(x1, y1 + 1, x2, y2 + 1, glow);
				}
			}
			/* An echo of the wheel spreads from it and fades until Flora
			   leaves. */
			if((Uint32)(frame - 228) <= FLORA_LEAVE_TICK - 228) {
				Sint32 x1b;
				Sint32 y1b;
				Sint32 x2b;
				Sint32 y2b;

				level = 256 - frame;
				radius = frame - 204;
				dot[0] = level;
				dot[1] = level;
				for(i = 0; i != FLORA_WHEEL_DOTS; i++) {
					fadeRingPoint.x = FIXEDtoINT(
						sySin(i * (TRIG_TURN / FLORA_WHEEL_DOTS)) * radius);
					fadeRingPoint.y = FIXEDtoINT(
						syCos(i * (TRIG_TURN / FLORA_WHEEL_DOTS)) * radius);
					fadeRingPoint.depth = 0;
					btEffectPosProjectPoint(&fadeRingPoint, &fadeRingScreen);
					fadeRingScreen.x >>= 1;
					x1b = centerX + fadeRingScreen.x;
					y1b = fadeRingScreen.y + outerY;
					x2b = fadeRingScreen.x * 3 / 4 + centerX;
					y2b = fadeRingScreen.y * 3 / 4 + innerY;
					draw[0](canvas, dot, x1b - 1, y1b - 1, 1, 2);
					draw[0](canvas, dot, x2b - 1, y2b - 1, 1, 2);
					if((i & (FLORA_WHEEL_DOTS / FLORA_WHEEL_SPOKES - 1)) == 0) {
						btEffectDrawClippedCanvasLine(
							x1b, y1b, x2b, y2b, level);
						btEffectDrawClippedCanvasLine(
							x1b, y1b + 1, x2b, y2b + 1, level);
					}
				}
			}
		}
		EFFECT_PRESENT_CANVAS(state);
	}
	/* The battle comes back for the storm. */
	syCloseEvent(btEffectAdvanceScrollOnInterval);
	BgScrollWork[1].x = savedScroll;
	backdrop->busy = 0;
	btEffectSetupBlendedDisplay();
	for(i = 0; i != FLORA_PETAL_SPRITES + FLORA_PETAL_COUNT; i++) {
		fdSpriteDestroyStub(state->sprites[i]);
	}
	syPaletteLoadBasicSpriteColors();
	syDMAXCopy((const void *)OBJ_PLTT,
		(void *)&BG_PLTT_COLOR(BASIC_COLORS_PALETTE, 0),
		DMA_COPY_HALFWORDS(PALETTE_COLORS), REG_DMA3);
	BG_PLTT_COLOR(BASIC_COLORS_PALETTE, TEXT_BACKGROUND_COLOR) =
		BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);
	EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
	REG_BG2X = 0;
	REG_BLDCNT = 0;
	REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	REG_BG2PA = Q_8_8(0.5);
	{
		Sint32 picture;
		Sint32 tileRow;
		Sint32 tileColumn;
		Sint32 k;
		Sint32 m;
		Sint32 t;
		Sint32 hi;

		resource = syResourceGetTableEntry(RESOURCE_PINK_PETALS_TILES);
		syWordsCopyEntry((void *)BG_PLTT, resource, PALETTE_BANK_BYTES);
		resource += PALETTE_BANK_BYTES;
		syDecompressTaggedLZ(resource, SheetOf(state));
		/* From here petals points at the storm's petal pictures. */
		petals = state->sheet + FLORA_STORM_PICTURES;
		/* Each 8x8 tile of the petal's pictures, two pixels a byte, is unpacked
		   to a byte a pixel at its place in its 16x16 picture. */
		i = 0;
		for(picture = 0; picture != FLORA_PETAL_PICTURE_COUNT; picture++) {
			for(tileRow = 0; tileRow != 2; tileRow++) {
				for(tileColumn = 0; tileColumn != 2; tileColumn++) {
					Sint32 offset;

					t = picture * 2;
					t += tileRow;
					t <<= 4;
					t += tileColumn;
					k = 0;
					offset = t << 3;
					for(; k != 8; k++, offset += FLORA_PETAL_SIZE) {
						for(m = 0; m != 4; m++) {
							hi = state->sheet[i] >> 4 & 0xf;
							petals[offset + m * 2] = state->sheet[i] & 0xf;
							petals[offset + m * 2 + 1] = hi;
							i++;
						}
					}
				}
			}
		}
	}
	for(frame = 0; frame != FLORA_STORM_TICKS; frame++) {
		if(frame == 0) {
			p = (PARTICLE *)src;
			for(i = 0; i != FLORA_STORM_PETALS; i++, p++) {
				p->x = INTtoFIXED((syRandom() & 63) + 96);
				p->y = toFIXED(-(syRandom() & 63));
				p->velocityX = -((syRandom() & 255) + 128) << 7;
				p->velocityY = ((syRandom() & 255) + 128) << 9;
				p->variant = syRandom() & 31;
			}
			for(i = 0; i != args->targetCount; i++) {
				state->particles[i].variant = (syRandom() & 15) + 1;
			}
		}
		/* Each petal tumbles a picture every four ticks as it falls. */
		p = (PARTICLE *)src;
		for(i = 0; i != FLORA_STORM_DRAWN; i++, p++) {
			if(frame > i) {
				draw[0](canvas,
					state->sheet + FLORA_STORM_PICTURES +
						(p->variant / 4 % FLORA_PETAL_PICTURE_COUNT << 8),
					FIXED_INTEGER_PART(p->x) - 4, FIXED_INTEGER_PART(p->y),
					FLORA_PETAL_SIZE, FLORA_PETAL_SIZE);
				btParticleAdvanceWithGravity2D(p, PARTICLE_DAMPING_NONE, 0);
				p->velocityX += -toFIXED(1) / 16;
				p->variant++;
			}
		}
		if(frame == 56) {
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
		}
		if((Uint32)(frame - 57) <= 50) {
			for(i = 0; i != args->targetCount; i++) {
				PARTICLE *s = &state->particles[i];

				if(--s->variant == 0) {
					s->variant = FLORA_HIT_INTERVAL;
					state->shakeFrame = 4;
					sySoundPlayStub(SE_IMPACT);
					btMotionApplyKnockbackStub(
						args->targets[i], KNOCKBACK_NUDGE);
					btEffectTintBattler(
						args->targets[i], SPRITE_COLOR_WHITE, TINT_KEEP, i, 8);
				}
			}
		}
		btCameraShake(2, 2);
		EFFECT_END_TICK(state);
	}
	EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

/* Unused: nothing calls it. Sends a unit outward: unless it is the Fusion
   Dragon or one of three of the Doom Dragon's forms, it moves 15 pixels further
   out on its own side, hopping if it stands under 10 pixels up; every unit then
   falls under a gravity of 0.6 pixels a tick without bouncing, and takes its
   damage pose. */
UNUSED void btEffectStartActorMotionOutward(Sint32 id)
{
	BATTLE_OBJECT *slot;
	MOTION_OBJECT *object;
	Uint32		   enemy;
	FIXED		   x;

	slot = btObjectGetStub(id);
	object = slot->object;
	enemy = syUnitGet(id)->enemyId;
	if(enemy != ENEMY_FUSION_DRAGON && enemy != ENEMY_DOOM_DRAGON_A &&
		enemy != ENEMY_DOOM_DRAGON_E && enemy != ENEMY_DOOM_DRAGON_H) {
		object->acceleration = toFIXED(2);
		object->speedLimit = toFIXED(8);
		if(object->y < toFIXED(10))
			object->velocityY += toFIXED(5) / 2;
		fdActorStartIdleStub(object);
		if(slot->anchorX > 0)
			x = object->x + toFIXED(15);
		else
			x = object->x - toFIXED(15);
		fdActorSetMoveTargetStub(object, x, 0, slot->anchorZ);
	}
	object->verticalMotionStrength = toFIXED(3) / 5;
	object->verticalMotionPhase = 0;
	object->autoFaceMotion = 0;
	fdActorSelectAnimationStub(object, POSE_DAMAGE);
}

#define SABRE_DANCE_TICKS	  96
/* Of the particles set up, the first SABRE_DANCE_BLADES are drawn. */
#define SABRE_DANCE_PARTICLES 32
#define SABRE_DANCE_BLADES	  8
/* The blades turn and fall on the target. */
#define SABRE_DANCE_FALL_TICK 80
#define SABRE_DANCE_LAST_HIT  90

/* Dancing blades. The canvas is moved over the target. Eight pairs of crossed
   blades rise spinning from below it, and the target is struck eight times, six
   ticks apart from tick 20: each strike shakes the screen, flashes the target
   and knocks it up. At tick 80 the blades turn point down in a fan above the
   target and plunge into it; the last strike, at tick 90, lets the battle show
   the damage.

   Abilities: Sabre Dance. */
void btEffectRunSabreDance(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state = RamHeapSlots->battleEffect;
	MODEL_TEXTURE		 texture;
	SCREEN_POINT		 target;
	PARTICLE			*blade;
	RENDER_MODEL		*quad;
	void				*vertices;
	Sint32				 frame;
	Sint32				 i;
	Sint32				 strikeFrame;

	btEffectStartCanvasLayer(0);
	btResourceLoadAndDecompress(
		RESOURCE_CROSSED_DAGGERS_SHEET, state->sheet, 1, 1);
	EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
	REG_BLDCNT = 0;
	sySetEvent((Sint32)btEffectTransferCanvas, EVENT_PASS_VBLANK);

	/* Centre the canvas's x 64 on the target. */
	btObjectGetStub(args->targets[0]);
	btEffectPosProjectBattler(args->targets[0], &target);
	REG_BG2PA = Q_8_8(1);
	REG_BG2X = (64 - target.x) << 8;

	for(frame = 0; frame != SABRE_DANCE_TICKS; frame++) {
		/* The first tick lines the blades up below the canvas, each spinning
		   the other way from its neighbour. */
		if(frame == 0) {
			FIXED  y;
			Sint32 zero;

			for(i = 0, zero = 0, y = toFIXED(128), blade = state->particles;
				i != SABRE_DANCE_PARTICLES; i++, blade++) {
				SET_POSITION(
					blade, INTtoFIXED((syRandom() & 63) + 32), y, syRandom());
				blade->velocityX = zero;
				blade->velocityY = -((syRandom() & 31) + 32) << 10;
				blade->velocityZ = (syRandom() & 0x3ff) + toFIXED(1) / 64;
				if(i & 1)
					blade->velocityZ = -blade->velocityZ;
				blade->variant = zero;
				y += toFIXED(8);
			}
		}
		syInitMatrix();
		ProjectionWork.centerY = 104;

		for(i = 0, strikeFrame = 20; i != 8; i++, strikeFrame += 6) {
			if(frame == strikeFrame) {
				state->shakeFrame = 4;
				sySoundPlayStub(SE_IMPACT);
				btEffectTintBattler(
					args->targets[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 6);
				btEffectStartActorMotion(
					args->targets[0], 1, 0, 0, toFIXED(3), 100);
			}
		}
		if(frame == SABRE_DANCE_LAST_HIT) {
			state->shakeFrame = 4;
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
			btEffectTintBattler(
				args->targets[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 10);
			btEffectStartActorMotion(
				args->targets[0], 1, 0, 0, toFIXED(3), 100);
		}

		/* One textured quad, drawn once for each pair of blades. */
		vertices = syHeapAllocateEWRAM(32);
		quad = btRenderAllocateModelList(1);
		quad->shade = 0;
		texture.logWidth = 5;
		texture.logHeight = 5;
		quad->texture = &texture;
		texture.pixels = state->sheet;
		quad->kind = MODEL_SHADED;
		quad->mesh = SabreDanceDaggerMesh;
		quad->vertices = vertices;
		for(i = 0, blade = state->particles; i != SABRE_DANCE_BLADES;
			i++, blade++) {
			/* The fall: from 48 pixels above the canvas, fanned out by angle
			   and closing on the middle. */
			if(frame == SABRE_DANCE_FALL_TICK) {
				Sint32 angle;
				Sint32 drift;

				blade->y = -toFIXED(48);
				angle = i * (TRIG_TURN / 64) + (syRandom() & 0x3ff) -
					TRIG_TURN * 9 / 128;
				blade->x = angle * (toFIXED(1) / 64) + toFIXED(64);
				blade->z = angle;
				drift = -angle << 5;
				blade->velocityY = toFIXED(12);
				blade->velocityX = drift;
				blade->velocityZ = 0;
			}
			syInitMatrix();
			syTranslate(blade->x - toFIXED(64), blade->y - toFIXED(64), 0);
			syMatrixScaleUniform(toFIXED(1) / 2);
			syRotZ(blade->z);
			btRenderTransformVertices(SabreDanceDaggerCorners, vertices, 4);
			btRenderDrawModelList(quad);
			if(frame >= SABRE_DANCE_FALL_TICK)
				btParticleAdvanceWithGravity3D(blade, PARTICLE_DAMPING_NONE, 0);
			else
				btParticleAdvanceWithGravity3D(
					blade, PARTICLE_DAMPING_NONE, -toFIXED(1) / 16);
		}
		syHeapRelease(quad);
		syHeapRelease(vertices);
		ProjectionWork.centerX = BATTLE_PROJECTION_CENTER_X;
		ProjectionWork.centerY = BATTLE_PROJECTION_CENTER_Y;

		btCameraShake(4, 4);
		EFFECT_END_TICK(state);
	}

	syCloseEvent((Sint32)btEffectTransferCanvas);
	btEffectEndCanvasLayer();
}

/* Sets the OBJ priority of every motion record of the battle object, unless the
   object is fading out. */
void btObjectSetRecordPriority(Sint32 objectId, Sint32 priority)
{
	BATTLE_OBJECT *slot = btObjectGetStub(objectId);
	PLACED_SPR	  *record;
	Sint32		   i;

	for(i = 0; (record = btActorGetSpriteStub(slot->object, i)) != NULL; i++) {
		if(slot->fading == 0)
			record->priority = priority;
	}
}

/* Backstab's length; the caster leaps at the foe at BACKSTAB_LEAP_TICK and
   strikes at BACKSTAB_STRIKE_TICK. */
#define BACKSTAB_TICKS		 60
#define BACKSTAB_LEAP_TICK	 16
#define BACKSTAB_STRIKE_TICK 32
/* Of the puffs of smoke set up, the first SMOKE_DRAWN rise, each through
   SmokeSheet's six pictures, four ticks each. */
#define SMOKE_COUNT			 32
#define SMOKE_DRAWN			 24
#define STAR_COUNT			 16
/* the pink star's six 16x16 pictures, after the smoke's */
#define STAR_PICTURES		 16000

/* A pink star burst. Smoke puffs from the caster for its first 24 ticks while
   the caster stands just past the target; at tick 16 it leaps at the target,
   and at tick 32 the palette turns pink, the caster lands beside the target,
   which is knocked back and flashes, and sixteen pink stars burst from it for
   24 ticks. The battle keeps the sprites' priorities as the effect sets them
   throughout.

   Abilities: Backstab. */
void btEffectRunBackstab(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	Uint8				*canvas;
	SCREEN_POINT		 caster;
	SCREEN_POINT		 target;
	Sint32				 frame;
	/* LEFTOVER(frame): draw[1], the pair's idle second blitter, which the ROM's
	 * tick reserves and never touches; as one DRAW_RECTANGLE. */
	DRAW_RECTANGLE draw[2];
	MOTION_OBJECT *targetObject;
	MOTION_OBJECT *casterObject;
	BATTLE_STATE  *battle;
	PARTICLE	  *particle;
	Sint32		   i;

	state = RamHeapSlots->battleEffect;
	canvas = RamHeapSlots->battleCanvas;
	btEffectStartCanvasLayer(0);
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	btResourceLoadAndDecompress(
		RESOURCE_PINK_STAR_SHEET, state->sheet + STAR_PICTURES, 1, 0);
	btResourceLoadAndDecompress(RESOURCE_SMOKE_SHEET, state->sheet, 1, 1);
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
	draw[0] = RamHeapSlots->ramCode;
	EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	sySetEvent((Sint32)btEffectTransferCanvas, EVENT_PASS_VBLANK);
	targetObject = btObjectGetStub(args->targets[0])->object;
	casterObject = btObjectGetStub(args->user)->object;
	battle = RamHeapSlots->battleState;
	battle->keepObjectPriorities = 1;

	for(frame = 0; frame != BACKSTAB_TICKS; frame++) {
		btObjectSetRecordPriority(args->user, 3);

		/* The first tick sends the smoke out from the caster's feet. */
		if(frame == 0) {
			btEffectPosProjectBattler(args->user, &caster);
			for(i = 0; i != SMOKE_COUNT; i++) {
				Sint32 angle;
				Sint32 speed;

				particle = &state->particles[i];
				particle->variant = 0;
				particle->x = toFIXED(caster.x / 2);
				particle->y = INTtoFIXED(caster.y - 8);
				angle = syRandom();
				speed = (syRandom() & 255) + 127;
				particle->velocityX = (sySin(angle) * speed) >> 5;
				particle->velocityY = (syCos(angle) * speed) >> 5;
			}
			sySoundPlayStub(SE_WHOOSH);
		}

		/* The smoke rises through its six 24x48 pictures, slowing; the caster
		   starts 160 percent of the way out to the target. */
		/* DEAD: always true, as tick counts up from 0; the ROM keeps the
		   test. */
		if(frame >= 0) {
			PARTICLE *puff;

			for(i = 0, puff = state->particles; i != SMOKE_DRAWN; i++, puff++) {
				if(frame >= i / 8 && puff->variant < SMOKE_PUFF_TICKS) {
					draw[0](canvas,
						state->sheet +
							(puff->variant / 4) * PICTURE_BYTES(SMOKE_PUFF),
						FIXED_INTEGER_PART(puff->x) - 12,
						FIXED_INTEGER_PART(puff->y) - 24, SMOKE_PUFF_WIDTH,
						SMOKE_PUFF_HEIGHT);
					btParticleAdvanceWithGravity2D(puff, 48, -toFIXED(1) / 8);
					puff->variant++;
				}
			}
			if(frame == 0) {
				casterObject->z = targetObject->z;
				casterObject->x = targetObject->x * 160 / 100;
				casterObject->angle = targetObject->angle;
			}
		}

		if(frame == BACKSTAB_LEAP_TICK) {
			btEffectSetApproachMotion(args->user, args->targets[0], 16, 120);
			casterObject->velocityY = toFIXED(7);
		}

		/* The strike: pink palette, the caster lands at 120 percent and the
		   target is pushed to 80, and the stars burst upward from it. */
		if(frame == BACKSTAB_STRIKE_TICK) {
			syWordsCopyEntry((void *)BG_PLTT,
				syResourceGetTableEntry(RESOURCE_PINK_STAR_SHEET),
				SHEET_PALETTE_BYTES);
			casterObject->z = targetObject->z;
			casterObject->x = targetObject->x * 120 / 100;
			targetObject->x = targetObject->x * 80 / 100;
			casterObject->angle = targetObject->angle;
			btEffectPosProjectScaledBattler(args->targets[0], &target);
			for(i = 0, particle = &state->particles[SMOKE_COUNT];
				i != STAR_COUNT; i++, particle++) {
				Sint32 angle;
				Sint32 speed;

				particle->variant = (syRandom() & 15) + 16;
				particle->x = toFIXED(target.x / 2);
				particle->y = INTtoFIXED(target.y - 8);
				angle = (syRandom() & 0x7fff) + DEGtoANG(90);
				speed = (syRandom() & 255) + 127;
				particle->velocityX = (sySin(angle) * speed) >> 7;
				particle->velocityY = (syCos(angle) * speed) >> 5;
			}
			state->shakeFrame = 4;
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
			btEffectTintBattler(
				args->targets[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 16);
			btEffectStartActorMotion(
				args->targets[0], 1, toFIXED(8), toFIXED(20), toFIXED(6), 10);
		}

		/* The stars twinkle through their six pictures as they fly. */
		if(frame >= BACKSTAB_STRIKE_TICK && frame < BACKSTAB_STRIKE_TICK + 24) {
			PARTICLE *star;

			for(i = 0, star = &state->particles[SMOKE_COUNT]; i != STAR_COUNT;
				i++, star++) {
				if(star->variant >= 0) {
					draw[0](canvas,
						state->sheet + STAR_PICTURES +
							(((frame + i) / 4) % 6) * 256,
						FIXED_INTEGER_PART(star->x) - 8,
						FIXED_INTEGER_PART(star->y) - 8, 16, 16);
					btParticleAdvanceWithGravity2D(star, 56, 0);
					star->variant--;
				}
			}
		}

		btCameraShake(4, 4);
		EFFECT_END_TICK(state);
	}

	battle->keepObjectPriorities = 0;
	syCloseEvent((Sint32)btEffectTransferCanvas);
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
	btEffectEndCanvasLayer();
}

#undef FLORA_STATE
#undef FLORA_FIGURE_PIECES
#undef FLORA_FIGURE_COLUMNS
#undef FLORA_PETAL_SPRITES
#undef FLORA_PETAL_PICTURE_COUNT
#undef FLORA_PETAL_COUNT
#undef FLORA_PETAL_SIZE
#undef FLORA_PETAL_PALETTE
#undef FLORA_COLORS
#undef FLORA_PHASE_ONE
#undef FLORA_PATH_PETALS
#undef FLORA_BURST_PETAL
#undef FLORA_STORM_TICKS
#undef FLORA_STORM_PETALS
#undef FLORA_STORM_DRAWN
#undef FLORA_HIT_INTERVAL
#undef FLORA_STORM_PICTURES
#undef FLORA_PETAL_PHASE
#undef FLORA_TICKS
#undef FLORA_WIDE_TICK
#undef FLORA_CANVAS_X
#undef FLORA_APPEAR_TICK
#undef FLORA_FLASH_TICKS
#undef FLORA_FADE_IN_TICKS
#undef FLORA_LAUNCH_TICK
#undef FLORA_LEAVE_TICK
#undef FLORA_GLOW_BYTES
#undef FLORA_GLOW_X
#undef FLORA_GLOW_Y
#undef FLORA_DIMMED_MOTE
#undef FLORA_DIMMED_MOTE_BYTES
#undef FLORA_DIMMED_MOTE_COPIES
#undef FLORA_DIMMED_MOTES
#undef FLORA_RING_MOTES
#undef FLORA_RING_ARMS
#undef FLORA_ARM_MOTES
#undef FLORA_RING_SPIN
#undef FLORA_RING_TICK
#undef FLORA_RING_TICKS
#undef FLORA_LIGHT_RINGS
#undef FLORA_RING_X
#undef FLORA_RING_Y
#undef FLORA_WHEEL_DOTS
#undef FLORA_WHEEL_SPOKES
#undef FLORA_WHEEL_GLOW
#undef FLORA_WHEEL_TICK
#undef FLORA_WHEEL_TICKS
#undef SABRE_DANCE_TICKS
#undef SABRE_DANCE_PARTICLES
#undef SABRE_DANCE_BLADES
#undef SABRE_DANCE_FALL_TICK
#undef SABRE_DANCE_LAST_HIT
#undef BACKSTAB_TICKS
#undef BACKSTAB_LEAP_TICK
#undef BACKSTAB_STRIKE_TICK
#undef SMOKE_COUNT
#undef SMOKE_DRAWN
#undef STAR_COUNT
#undef STAR_PICTURES
