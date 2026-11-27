// The Catastrophe summon: the chained demon over a red sky, its sword's orb landing far below, and
// lightning and smoke sweeping over the foes. Part of battle_effect.c.

// A corner of a strip as an effect builds it, in signed bytes: the smoke band of Catastrophe's last
// act, Haures's bands.
typedef struct MESH_VERTEX
{
	Sint8 x;
	Sint8 y;
	Sint8 z;
} MESH_VERTEX;

// Where FlashBurstSheet is decoded in the effect's pictures.
#define CATASTROPHE_FLASH_BURST_PICTURES  6144

// Where Catastrophe decodes its pictures: JaggedStreakSheet in act one; PentagonSealTexture after
// TriangleSealTexture, in the pictures in act one and the map cell buffer in act two; SwirlTexture
// and then FireRingTexture in the map cell buffer; and TornadoSheet (act two) or ClawSlashSheet
// (act three), whose glints and slash the last acts sweep across.
#define CATASTROPHE_JAGGED_STREAKS        8192
#define CATASTROPHE_PENTAGON_SEAL         4096
#define CATASTROPHE_RING                  8192
#define CATASTROPHE_SWEEP_PICTURES        2048

// LightningTexture's two 32x48 forks, which flash in act one's sky.
#define LIGHTNING_FORK_WIDTH              32
#define LIGHTNING_FORK_HEIGHT             48
// JaggedStreakSheet's three streaks, and the four 32x128 textures act two copies
// LightningBoltSheet's bolts into, each bolt in the middle of its own, after TornadoSheet's glints.
#define JAGGED_STREAK_WIDTH               32
#define JAGGED_STREAK_HEIGHT              128
#define BOLT_TEXTURE_WIDTH                32
#define BOLT_TEXTURE_HEIGHT               128
#define CATASTROPHE_BOLT_TEXTURES         (CATASTROPHE_SWEEP_PICTURES + GLINT_SET_BYTES)
#define CATASTROPHE_BOLT_TEXTURE_COUNT    4

// Catastrophe's sprites, all 32x32: the chained demon (ChainedDemonTiles), its mirrored copy's six,
// the golden shield, and the eleven of the sword (its hilt's five, then its white blade's six). The
// demon's lowest four are copies of its top row, sharing their tiles: those show the sprite sheet's
// last four pictures, and the top row's own between CATASTROPHE_ROW_SWAP_TICK and
// CATASTROPHE_ROW_RESTORE_TICK.
#define CATASTROPHE_DEMON                 0
#define CATASTROPHE_DEMON_TILED           24
#define CATASTROPHE_DEMON_SWAPPED         24
#define CATASTROPHE_DEMON_SWAPPED_COUNT   4
#define CATASTROPHE_DEMON_COUNT           28
#define CATASTROPHE_MIRRORED_DEMON        28
#define CATASTROPHE_MIRRORED_DEMON_COUNT  6
#define CATASTROPHE_SHIELD                34
#define CATASTROPHE_SHIELD_COUNT          5
#define CATASTROPHE_SWORD                 39
#define CATASTROPHE_HILT_COUNT            5
#define CATASTROPHE_BLADE                 44
#define CATASTROPHE_BLADE_COUNT           6
#define CATASTROPHE_SWORD_COUNT           (CATASTROPHE_HILT_COUNT + CATASTROPHE_BLADE_COUNT)
#define CATASTROPHE_SPRITES               50
// ChainedDemonTiles' colours, OBJ palette 0 to 191.
#define CATASTROPHE_DEMON_COLORS          192
// Where the decoded ChainedDemonTiles keeps its last four pictures in the map cell buffer, and
// where they are kept again, after the top row's.
#define CATASTROPHE_DEMON_LOWEST_PICTURES (CATASTROPHE_DEMON_TILED * OBJ_32X32_8BPP_BYTES)
#define CATASTROPHE_DEMON_KEPT_PICTURES   (CATASTROPHE_DEMON_SWAPPED_COUNT * OBJ_32X32_8BPP_BYTES)
// The OBJ palettes of the shield, the sword's hilt and its blade.
#define CATASTROPHE_SHIELD_PALETTE        12
#define CATASTROPHE_HILT_PALETTE          13
#define CATASTROPHE_BLADE_PALETTE         14
// The ticks of act one, unless A or B skips it, and its turns. Out of the dark the sword's blade
// comes down and shows dark blue (CATASTROPHE_BLADE_TICK), gleams pale and darkens again
// (CATASTROPHE_GLEAM_TICK); the chained demon comes out of the dark in blue
// (CATASTROPHE_EMERGE_TICK), is held there from CATASTROPHE_BLUE_TICK and darkens
// (CATASTROPHE_DARKEN_TICK); it shows in its colours, washed with blue, and darkens to black
// (CATASTROPHE_GLIMPSE_TICK). Then it flashes white (CATASTROPHE_REVEAL_TICK) and its colours come
// back out of the white from CATASTROPHE_REVEAL_FADE_TICK, while the view bobs
// (CATASTROPHE_BOB_TICK) and sinks (CATASTROPHE_SINK_TICK). The shield and the sword draw apart
// (CATASTROPHE_DRAW_BACK_TICK) and the demon swings (CATASTROPHE_SWING_TICK to
// CATASTROPHE_SWING_END_TICK), reddening as its three strikes flash from CATASTROPHE_STRIKE_TICK;
// the canvas flickers white from CATASTROPHE_FLICKER_TICK, the seal opens below
// (CATASTROPHE_SEAL_TICK) and the orb swells over it as the red haze dims (CATASTROPHE_ORB_TICK),
// the view dives after the blade (CATASTROPHE_DIVE_TICK), and the blade's orb lands in a white
// flash and a ring of fire (CATASTROPHE_LANDING_TICK), the canvas flashing twice more from
// CATASTROPHE_AFTERFLASH_TICK.
#define CATASTROPHE_ACT_ONE_TICKS         637
#define CATASTROPHE_BLADE_TICK            106
#define CATASTROPHE_GLEAM_TICK            154
#define CATASTROPHE_EMERGE_TICK           226
#define CATASTROPHE_BLUE_TICK             242
#define CATASTROPHE_DARKEN_TICK           256
#define CATASTROPHE_GLIMPSE_TICK          282
#define CATASTROPHE_REVEAL_TICK           324
#define CATASTROPHE_REVEAL_FADE_TICK      332
#define CATASTROPHE_BOB_TICK              370
#define CATASTROPHE_SINK_TICK             390
#define CATASTROPHE_DRAW_BACK_TICK        420
#define CATASTROPHE_SWING_TICK            472
#define CATASTROPHE_SWING_END_TICK        486
#define CATASTROPHE_STRIKE_TICK           489
#define CATASTROPHE_DIVE_TICK             506
#define CATASTROPHE_FLICKER_TICK          537
#define CATASTROPHE_SEAL_TICK             552
#define CATASTROPHE_ORB_TICK              554
#define CATASTROPHE_LANDING_TICK          602
#define CATASTROPHE_AFTERFLASH_TICK       630
// The demon's lowest four sprites show its top row's pictures from CATASTROPHE_ROW_SWAP_TICK until
// CATASTROPHE_ROW_RESTORE_TICK.
#define CATASTROPHE_ROW_SWAP_TICK         339
#define CATASTROPHE_ROW_RESTORE_TICK      450
// From this tick the canvas is drawn in front of the sprites.
#define CATASTROPHE_CANVAS_FRONT_TICK     440
// The jagged streaks that burst where the orb lands start five times their width and narrow by this
// much a tick from ten ticks on, to their own.
#define CATASTROPHE_LANDING_BURST_SHRINK  61845
// As the demon swings its sword (ticks 472 to 485), the shield moves right and the sword left by
// CATASTROPHE_SWING_DRIFT a tick, less a sixteenth of a pixel more each tick, and both sink
// CATASTROPHE_SWING_SINK a tick.
#define CATASTROPHE_SWING_DRIFT           120000
#define CATASTROPHE_SWING_SINK            (toFIXED(1) + 10000)
// From CATASTROPHE_DIVE_TICK the view dives after the sword's blade, the demon rising out of sight,
// at twice a speed that starts at CATASTROPHE_DIVE_SPEED_START and grows by
// CATASTROPHE_DIVE_ACCELERATION a tick to CATASTROPHE_DIVE_SPEED_MAX, ten ticks on.
#define CATASTROPHE_DIVE_SPEED_START      50768
#define CATASTROPHE_DIVE_ACCELERATION     7296
#define CATASTROPHE_DIVE_SPEED_MAX        120000
// Where the objects' first 240 colours are kept in the effect's pictures, which the objects fade
// from.
#define CATASTROPHE_SAVED_OBJ_PALETTE     20480
#define CATASTROPHE_SAVED_OBJ_COLORS      240
// Act one's claps of thunder, each flashing the backdrop blue, and the ticks after
// CATASTROPHE_CANVAS_FLASH_START on which the canvas flashes white.
#define CATASTROPHE_FLASHES               18
#define CATASTROPHE_CANVAS_FLASHES        17
#define CATASTROPHE_CANVAS_FLASH_START    242
// The level the canvas flashes to: on those ticks, every other tick from 537 to 592, and at 630 and
// 634.
#define CATASTROPHE_FLASH_LEVEL           31
// Act one's three strikes, each of up to CATASTROPHE_STRIKE_STEPS steps (their ticks' rows one
// longer, for the 65535 that ends them), and the sparks each step throws.
#define CATASTROPHE_STRIKES               3
#define CATASTROPHE_STRIKE_STEPS          12
#define CATASTROPHE_STRIKE_SPARKS         10
// The orb's disc is drawn this much brighter than full, its middle held at the brightest level.
#define CATASTROPHE_ORB_BRIGHTNESS        (toFIXED(1) + 30000)
// Act two's orb is centred at this x on the canvas, and at this y as it starts, before it rises and
// swells; its sparks fly out from (x, y).
#define CATASTROPHE_ORB_CENTER            60
// The seal: the pentagon laid back by CATASTROPHE_SEAL_TILT, its three triangles turned up from its
// edges by as much as CATASTROPHE_SEAL_SIDE_TILT, and its largest scale.
#define CATASTROPHE_SEAL_TILT             18768
#define CATASTROPHE_SEAL_SIDE_TILT        19712
#define CATASTROPHE_SEAL_SCALE_MAX        140600
// The swirl over the seal rises this much a tick for its first CATASTROPHE_SWIRL_RISE_TICKS ticks.
#define CATASTROPHE_SWIRL_RISE            120000
#define CATASTROPHE_SWIRL_RISE_TICKS      7
// The seal's scale as act two opens, under the orb, before it swells.
#define CATASTROPHE_AERIAL_SEAL_SCALE     (toFIXED(1) + 20000)
// How fast the rings of fire grow, a tick.
#define CATASTROPHE_RING_GROWTH           12192
// Act two's bolts, and the sparks after them in state->particles.
#define CATASTROPHE_BOLTS                 16
#define CATASTROPHE_SPARKS                48
// Late in act two the canvas is cleared each tick to this pale level, the land washing out, and
// then to the brightest, white.
#define CATASTROPHE_WASH_LEVEL            16
// Act three's buffers in the map cell buffer: the smoke band's corners at its start, their
// transformed vertices, its strip mesh, LightningBoltSheet's four bolts, and the particles: the
// flash bursts thrown off the band, then the debris; and the ticks each lives.
#define CATASTROPHE_SWEEP_VERTICES        4096
#define CATASTROPHE_SWEEP_STRIP           8192
#define CATASTROPHE_BOLT_PICTURES         16384
#define CATASTROPHE_PARTICLES                                                                      \
	((PARTICLE *)(RamMapMetatileBuffer + CATASTROPHE_BOLT_PICTURES                                 \
	              + 4 * LIGHTNING_BOLT_PICTURE_BYTES))
#define CATASTROPHE_BURSTS        64
#define CATASTROPHE_DEBRIS        100
#define CATASTROPHE_BURST_LIFE    12
#define CATASTROPHE_DEBRIS_LIFE   16
// The debris each target throws when struck, and each bolt where it lands.
#define CATASTROPHE_TARGET_DEBRIS 16
#define CATASTROPHE_BOLT_DEBRIS   12
// The smoke band's rows of two corners, and its wave's phase a tick.
#define CATASTROPHE_SWEEP_ROWS    12
#define CATASTROPHE_SWEEP_WAVE    6096
// The sweep's scale, the smoke band's and the slash's, and the band's own further scale, turned
// upright.
#define CATASTROPHE_SWEEP_SCALE   (toFIXED(1) - 10000)
#define CATASTROPHE_BAND_SCALE    274144
// Act three opens CATASTROPHE_PAN pixels right of the battle and pans back at CATASTROPHE_PAN_SPEED
// pixels a tick, slowing to a stop from tick 62.
#define CATASTROPHE_PAN           1088
#define CATASTROPHE_PAN_SPEED     16

// Catastrophe: over a red sky the chained demon with its golden shield spreads its wings and drives
// down a sword of light; the blade's orb falls on the land far below and bursts in a ring and a
// white flash; then violet lightning and smoke sweep in from the right over the foes. Holding A or
// B shortens each act.
//
// Abilities: Catastrophe.
void btEffectRunCatastrophe(BATTLE_EFFECT_PARM *args)
{
	Uint8                   *canvas;
	BATTLE_EFFECT_STATE     *state;
	DRAW_RECTANGLE           blit[2];
	Sint32                   savedScroll;
	BATTLE_BACKGROUND_STATE *background;
	Uint8                   *sprites;
	BATTLE_STATE            *battle;
	FIXED                    cameraZ;
	Sint32                   swapped;
	Sint32                   drift;
	Sint32                   sway;
	Sint32                   flashTimer;
	Sint32                   flash;
	void                    *verts;
	Sint32                   shade;
	RENDER_MODEL            *model;
	RENDER_MODEL            *aerialModel;
	RENDER_MODEL            *sweepModel;
	Sint32                   frame;
	Sint32                   i;
	Sint32                   j;
	PARTICLE                *p;
	PARTICLE                *q;
	SPR                     *object;
	MODEL_TEXTURE            aerialTexture;
	Sint16                   list[UNIT_MAX];
	FIXED                    place[4];
	SCREEN_POINT             pos;
	SCALE                    spriteScale;
	MODEL_TEXTURE            texture;
	MODEL_TEXTURE            sweepTexture;
	PARTICLE                *bolt;
	PARTICLE                *spark;
	PARTICLE                *burst;
	Uint16                  *backdropColor;
	Uint8                   *resource;
	Uint8                   *cellBuffer = RamMapMetatileBuffer;
	Sint32                   span       = toFIXED(8);
	Sint32                   rowStep    = 8;

	canvas                              = RamHeapSlots->battleCanvas;
	state                               = RamHeapSlots->battleEffect;
	savedScroll                         = BgScrollWork[1].x;
	background                          = RamHeapSlots->battleBg;
	sprites                             = RamHeapSlots->battleSheet;
	battle                              = RamHeapSlots->battleState;
	syVRAMCacheFreeEntry(battle->arrowTiles);
	fdActorShadowFreeTilesStub();
	EFFECT_START_NARROW_CANVAS(BGCNT_WRAP);
	btEffectConfigureEffectDisplay();
	PLTT_BACKDROP       = 0;
	BG_PLTT_COLOR(0, 1) = 0;
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
	blit[0]             = RamHeapSlots->ramCode;
	state->transferMode = CANVAS_COPY;
	sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);
	btEffectWipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_FILL);
	list[0] = BATTLE_UNIT_LIST_END;
	btActorSpawnObjectsForListStub(list, 0);
	ProjectionWork.centerY = SUMMON_PROJECTION_CENTER_Y;
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
	btBackgroundLoadStub(BATTLE_BACKGROUND_FIXED, RESOURCE_RED_HAZE_BACKDROP, 0);
	btPaletteBrightenSavedBattleBg(-RGB_LEVEL_MAX, -RGB_LEVEL_MAX, -RGB_LEVEL_MAX);
	background->busy  = 1;
	BgScrollWork[1].x = 0;
	REG_BG2PA         = Q_8_8(0.5);
	REG_BLDALPHA      = EFFECT_BLEND_ADD;
	REG_BLDCNT        = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	// BG1 shows the red haze as one picture 128 wide and 240 tall, each row twice across: its upper
	// 15 rows are the battle background's left half (from tile 512, at BG character block 2), its
	// lower 15 the right half.
	{
		Uint16 *map = (Uint16 *)BG_SCREEN_BLOCK(BATTLE_BG1_SCREEN_BLOCK);
		Sint32  row;
		Sint32  column;

		for (row = 0; row != 30; row++)
		{
			if (row > 14)
			{
				for (column = 0; column != 32; column++)
				{
					*map++ = row * 32 + (column & 15) + 48;
				}
			}
			else
			{
				for (column = 0; column != 32; column++)
				{
					*map++ = row * 32 + (column & 15) + 512;
				}
			}
		}
	}
	{
		Uint8 *cells;

		resource = syResourceGetTableEntry(RESOURCE_CHAINED_DEMON_TILES);
		syWordsCopy((void *)OBJ_PLTT, resource, CATASTROPHE_DEMON_COLORS * 2);
		resource += CATASTROPHE_DEMON_COLORS * 2;
		syDecompressTaggedLZ(resource, cellBuffer);
		cells = RamMapMetatileBuffer;
		for (i = 0; i != CATASTROPHE_DEMON_TILED; cells += OBJ_32X32_8BPP_BYTES, i++)
		{
			object = btEffectSpriteCreateWithTiles(32, 32, OBJ_SIZE_32X32 | OBJ_256_COLORS, 0);
			object->image.priority = 2;
			state->sprites[i]      = object;
			syWordsCopy((void *)OBJ_TILES(object), cells, OBJ_32X32_8BPP_BYTES);
		}
		{
			Uint8 *cells = RamMapMetatileBuffer + CATASTROPHE_DEMON_LOWEST_PICTURES;

			for (i = 0; i != CATASTROPHE_DEMON_SWAPPED_COUNT; i++)
			{
				object = btEffectSpriteAcquire(32, 32, OBJ_SIZE_32X32 | OBJ_256_COLORS, 0);
				state->sprites[CATASTROPHE_DEMON_SWAPPED + i] = object;
				syWordsCopy(object, state->sprites[i], offsetof(SPR, lastNo));
				syWordsCopy((void *)OBJ_TILES(state->sprites[CATASTROPHE_DEMON_SWAPPED + i]),
				            cells + i * OBJ_32X32_8BPP_BYTES,
				            OBJ_32X32_8BPP_BYTES);
			}
		}
		syWordsCopy(cellBuffer + CATASTROPHE_DEMON_KEPT_PICTURES,
		            RamMapMetatileBuffer + CATASTROPHE_DEMON_LOWEST_PICTURES,
		            CATASTROPHE_DEMON_SWAPPED_COUNT * OBJ_32X32_8BPP_BYTES);
	}
	for (i = 0; i != CATASTROPHE_MIRRORED_DEMON_COUNT; i++)
	{
		object = btEffectSpriteAcquire(32, 32, OBJ_SIZE_32X32 | OBJ_256_COLORS, 0);
		state->sprites[CATASTROPHE_MIRRORED_DEMON + i] = object;
		syWordsCopy(object, state->sprites[i], offsetof(SPR, lastNo));
	}
	{
		Uint8 *cells;

		resource = syResourceGetTableEntry(RESOURCE_GOLDEN_SCALES_TILES);
		syWordsCopy(
			(void *)&OBJ_PLTT_COLOR(CATASTROPHE_SHIELD_PALETTE, 0), resource, PALETTE_BANK_BYTES);
		resource += PALETTE_BANK_BYTES;
		syDecompressTaggedLZ(resource, state->sheet);
		cells = state->sheet;
		for (i = 0; i != CATASTROPHE_SHIELD_COUNT; cells += OBJ_32X32_4BPP_BYTES, i++)
		{
			object = btEffectSpriteCreateWithTiles(
				32, 32, OBJ_SIZE_32X32, OBJ_PALETTE(CATASTROPHE_SHIELD_PALETTE));
			object->image.priority                 = 1;
			state->sprites[CATASTROPHE_SHIELD + i] = object;
			syWordsCopy((void *)OBJ_TILES(object), cells, OBJ_32X32_4BPP_BYTES);
		}
	}
	{
		Uint8 *cells;

		resource = syResourceGetTableEntry(RESOURCE_PURPLE_ARMOUR_TILES);
		syWordsCopy(
			(void *)&OBJ_PLTT_COLOR(CATASTROPHE_HILT_PALETTE, 0), resource, PALETTE_BANK_BYTES);
		resource += PALETTE_BANK_BYTES;
		syDecompressTaggedLZ(resource, state->sheet);
		cells = state->sheet;
		for (i = 0; i != CATASTROPHE_HILT_COUNT; cells += OBJ_32X32_4BPP_BYTES, i++)
		{
			object = btEffectSpriteCreateWithTiles(
				32, 32, OBJ_SIZE_32X32, OBJ_PALETTE(CATASTROPHE_HILT_PALETTE));
			object->image.priority                = 1;
			state->sprites[CATASTROPHE_SWORD + i] = object;
			syWordsCopy((void *)OBJ_TILES(object), cells, OBJ_32X32_4BPP_BYTES);
		}
	}
	{
		Uint8 *cells;

		resource = syResourceGetTableEntry(RESOURCE_WHITE_SPIKES_TILES);
		syWordsCopy(
			(void *)&OBJ_PLTT_COLOR(CATASTROPHE_BLADE_PALETTE, 0), resource, PALETTE_BANK_BYTES);
		resource += PALETTE_BANK_BYTES;
		syDecompressTaggedLZ(resource, state->sheet);
		cells = state->sheet;
		for (i = 0; i != CATASTROPHE_BLADE_COUNT; cells += OBJ_32X32_4BPP_BYTES, i++)
		{
			object = btEffectSpriteCreateWithTiles(
				32, 32, OBJ_SIZE_32X32, OBJ_PALETTE(CATASTROPHE_BLADE_PALETTE));
			object->image.priority                = 1;
			state->sprites[CATASTROPHE_BLADE + i] = object;
			syWordsCopy((void *)OBJ_TILES(object), cells, OBJ_32X32_4BPP_BYTES);
		}
	}
	syWordsCopy(state->sheet + CATASTROPHE_SAVED_OBJ_PALETTE,
	            (void *)OBJ_PLTT,
	            CATASTROPHE_SAVED_OBJ_COLORS * 2);
	btResourceLoadAndDecompress(RESOURCE_LIGHTNING_TEXTURE, state->sheet, 0, 0);
	btResourceLoadAndDecompress(
		RESOURCE_JAGGED_STREAK_SHEET, state->sheet + CATASTROPHE_JAGGED_STREAKS, 0, 0);
	syWordsCopy((void *)BG_PLTT,
	            syResourceGetTableEntry(RESOURCE_LIGHTNING_BOLT_SHEET),
	            SHEET_PALETTE_BYTES);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprites, 0, 0);
	btResourceLoadAndDecompress(
		RESOURCE_SWIRL_TEXTURE, RamMapMetatileBuffer + CATASTROPHE_RING, 0, 0);
	cameraZ    = 0;
	swapped    = 0;
	drift      = 0;
	sway       = 0;
	flashTimer = 0;
	flash      = 0;
	EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
	blit[0] = RamHeapSlots->ramCode;
	// Act one: the demon breaks out of its chains and drives down its sword.
	frame   = 0;
	while (frame != CATASTROPHE_ACT_ONE_TICKS)
	{
		Sint32 level;
		Sint32 d;

		// The backdrop fades in over the first 16 ticks; A or B skips the act.
		if (frame <= 15)
		{
			Sint32 v = frame * 2 - 30;

			btPaletteBrightenSavedBattleBg(v, v, v);
		}
		if (PerWork.repeat & (KEY_A | KEY_B))
		{
			break;
		}
		if (frame == 0)
		{
			REG_BG2CNT =
				BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
			for (i = CATASTROPHE_STRIKES; i != ARRAY_COUNT(state->particles); i++)
			{
				state->particles[i].variant = -1;
			}
		}
		if (frame == CATASTROPHE_CANVAS_FRONT_TICK)
		{
			REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
		}
		if (frame <= 369)
		{
			cameraZ = toFIXED(frame * 334 / 379) + -toFIXED(282);
		}
		else if ((Uint32)(d = frame - CATASTROPHE_BOB_TICK) <= 19)
		{
			cameraZ = sySin(d * (TRIG_TURN / 40)) * 9 + toFIXED(43);
		}
		else if ((Uint32)(frame - CATASTROPHE_SINK_TICK) <= 95)
		{
			cameraZ += -toFIXED(1) / 2;
		}
		else if ((Uint32)(frame - CATASTROPHE_SWING_END_TICK) <= 19)
		{
			cameraZ += -toFIXED(1);
		}
		else if ((Uint32)(frame - CATASTROPHE_DIVE_TICK) <= 47)
		{
			Sint32 v = (frame - CATASTROPHE_DIVE_TICK) * CATASTROPHE_DIVE_ACCELERATION
			         + CATASTROPHE_DIVE_SPEED_START;

			if (v > CATASTROPHE_DIVE_SPEED_MAX)
			{
				v = CATASTROPHE_DIVE_SPEED_MAX;
			}
			cameraZ -= v << 1;
		}
		if (frame <= 379)
		{
			BgScrollWork[1].y = 104 - frame * 104 / 379;
		}
		else
		{
			Sint32 v;

			BgScrollWork[1].y = v = (frame - 380) / 4;
			if ((Uint32)toFIXED(v) > toFIXED(104))
			{
				BgScrollWork[1].y = 104;
			}
		}
		{
			Sint32 swayFrame = frame - CATASTROPHE_BOB_TICK;
			if ((Uint32)swayFrame <= 59)
			{
				sway = sySin(swayFrame * (TRIG_TURN / 240) / 2) << 4;
			}
		}
		if ((Uint32)(frame - CATASTROPHE_DRAW_BACK_TICK) <= 51)
		{
			drift  = (CATASTROPHE_DRAW_BACK_TICK - frame) * (toFIXED(1) / 16);
			sway  += -toFIXED(1) / 4;
		}
		else if ((Uint32)(frame - CATASTROPHE_SWING_TICK) <= 13)
		{
			Sint32 swingFrame  = frame - CATASTROPHE_SWING_TICK;

			drift              = drift - swingFrame * (toFIXED(1) / 16) + CATASTROPHE_SWING_DRIFT;
			sway              += CATASTROPHE_SWING_SINK;
		}
		else if ((Uint32)(frame - CATASTROPHE_SWING_END_TICK) <= 8)
		{
			drift += toFIXED(0.125);
			sway  -= sySin((frame - CATASTROPHE_SWING_END_TICK) * (TRIG_TURN / 512));
		}
		else if ((Uint32)(frame - CATASTROPHE_ORB_TICK) <= 29)
		{
			sway += -toFIXED(1);
		}
		else if ((Uint32)(frame - 584) <= 9)
		{
			sway += -toFIXED(8);
		}
		if (frame == 0)
		{
			flashTimer = -1;
			flash      = 0;
		}
		// Each clap of thunder flashes the backdrop blue, fading over its length.
		if (flash < CATASTROPHE_FLASHES)
		{
			level = -1;
			if (frame == CatastropheFlashTicks[flash])
			{
				flashTimer = CatastropheFlashLengths[flash];
				switch (flash & 3)
				{
					case 0: sySoundPlayStub(SE_TOPPLE); break;
					case 1: sySoundPlayStub(SE_THUNDER); break;
					case 2: sySoundPlayStub(SE_BANE_ESCAPE); break;
					case 3: sySoundPlayStub(SE_THUNDER_LOUD); break;
				}
			}
			if (flashTimer >= 0)
			{
				level = flashTimer * RGB_LEVEL_MAX / CatastropheFlashLengths[flash];
				if (--flashTimer == -1)
				{
					flash++;
				}
			}
			if (level >= 0)
			{
				Uint16 *dest  = (Uint16 *)&BG_PLTT_COLOR(6, 0);
				backdropColor = battle->palette;

				for (i = 0; i != BATTLE_BACKGROUND_COLORS; i++)
				{
					Sint32 r = RGB_RED(*backdropColor);
					Sint32 g = RGB_GREEN(*backdropColor);
					Sint32 b = RGB_BLUE(*backdropColor) + level;

					if (r > RGB_LEVEL_MAX)
					{
						r = RGB_LEVEL_MAX;
					}
					if (g > RGB_LEVEL_MAX)
					{
						g = RGB_LEVEL_MAX;
					}
					if (b > RGB_LEVEL_MAX)
					{
						b = RGB_LEVEL_MAX;
					}
					backdropColor++;
					*dest++ = RGB(r, g, b);
				}
			}
		}
		// The sprites' colours step through these levels from their saved ones, and the backdrop's
		// too from CATASTROPHE_REVEAL_TICK, when both flash white.
		{
			Sint32 fadeRed;
			Sint32 fadeGreen;
			Sint32 fadeBlue;
			Sint32 apply[2] = {0, 0};

			fadeRed         = 0;
			fadeGreen       = 0;
			fadeBlue        = 0;
			if (frame == 0)
			{
				fadeGreen = -RGB_LEVEL_MAX;
				fadeRed   = fadeGreen;
				fadeBlue  = fadeGreen;
				apply[0]  = 1;
			}
			else if (frame == CATASTROPHE_BLADE_TICK)
			{
				fadeGreen = -RGB_LEVEL_MAX;
				fadeRed   = fadeGreen;
				fadeBlue  = -15;
				apply[0]  = 1;
			}
			else if ((Uint32)(frame - CATASTROPHE_GLEAM_TICK) <= 59)
			{
				Sint32 h  = (frame - CATASTROPHE_GLEAM_TICK) / 2;

				fadeGreen = 5 - h * 2;
				fadeRed   = fadeGreen;
				fadeBlue  = 5 - h;
				apply[0]  = 1;
			}
			else if ((Uint32)(frame - CATASTROPHE_EMERGE_TICK) <= 15)
			{
				fadeGreen = (frame - CATASTROPHE_EMERGE_TICK) / 2 - RGB_LEVEL_MAX;
				fadeRed   = fadeGreen;
				fadeBlue  = frame - 257;
				apply[0]  = 1;
			}
			else if (frame == CATASTROPHE_BLUE_TICK)
			{
				fadeRed   = -24;
				fadeGreen = -RGB_LEVEL_MAX;
				apply[0]  = 1;
			}
			else if ((Uint32)(frame - CATASTROPHE_DARKEN_TICK) <= 21)
			{
				Sint32 d  = frame - CATASTROPHE_DARKEN_TICK;

				fadeRed   = -24 - d * 2;
				fadeGreen = -RGB_LEVEL_MAX - d * 2;
				fadeBlue  = -d;
				apply[0]  = 1;
			}
			else if ((Uint32)(frame - CATASTROPHE_GLIMPSE_TICK) <= 40)
			{
				Sint32 d  = frame - CATASTROPHE_GLIMPSE_TICK;

				fadeGreen = -d;
				fadeRed   = fadeGreen;
				fadeBlue  = 16 - d;
				apply[0]  = 1;
			}
			else if (frame == CATASTROPHE_REVEAL_TICK)
			{
				fadeRed   = RGB_LEVEL_MAX;
				fadeGreen = RGB_LEVEL_MAX;
				fadeBlue  = RGB_LEVEL_MAX;
				apply[0]  = 1;
				apply[1]  = 1;
			}
			else if ((Uint32)(frame - CATASTROPHE_REVEAL_FADE_TICK) <= 59)
			{
				fadeGreen = 30 - (frame - CATASTROPHE_REVEAL_FADE_TICK) / 2;
				apply[0]  = 1;
				fadeRed   = fadeGreen;
				fadeBlue  = fadeGreen;
				apply[1]  = 1;
			}
			else if ((Uint32)(frame - CATASTROPHE_STRIKE_TICK) <= 42)
			{
				Sint32 d  = frame - CATASTROPHE_STRIKE_TICK;

				fadeRed   = d / 4;
				fadeGreen = d / 6;
				fadeBlue  = d / 16;
				apply[0]  = 1;
			}
			for (j = 0; j != 2; j++)
			{
				if (apply[j] != 0)
				{
					Uint16 *src;
					Uint16 *dest;
					Sint32  n;

					if (j == 0)
					{
						src  = (Uint16 *)(state->sheet + CATASTROPHE_SAVED_OBJ_PALETTE + 2);
						n    = CATASTROPHE_SAVED_OBJ_COLORS;
						dest = (Uint16 *)&OBJ_PLTT_COLOR(0, 1);
					}
					else
					{
						dest = (Uint16 *)&BG_PLTT_COLOR(6, 0);
						src  = battle->palette;
						n    = BATTLE_BACKGROUND_COLORS;
					}
					for (i = 1; i != n; i++)
					{
						Sint32 r = RGB_RED(*src) + fadeRed;
						Sint32 g = RGB_GREEN(*src) + fadeGreen;
						Sint32 b = RGB_BLUE(*src) + fadeBlue;

						if (r > RGB_LEVEL_MAX)
						{
							r = RGB_LEVEL_MAX;
						}
						if (g > RGB_LEVEL_MAX)
						{
							g = RGB_LEVEL_MAX;
						}
						if (b > RGB_LEVEL_MAX)
						{
							b = RGB_LEVEL_MAX;
						}
						if (r < 0)
						{
							r = 0;
						}
						if (g < 0)
						{
							g = 0;
						}
						if (b < 0)
						{
							b = 0;
						}
						*dest++ = RGB(r, g, b);
						src++;
					}
				}
			}
		}
		// The red haze dims by a 64th every other tick, to about half.
		if ((Uint32)(frame - CATASTROPHE_ORB_TICK) <= 31 && !((frame - CATASTROPHE_ORB_TICK) & 1))
		{
			Sint32 dimFrame = frame - CATASTROPHE_ORB_TICK;

			btColorBufferScaleStub(battle->palette,
			                       (void *)&BG_PLTT_COLOR(6, 0),
			                       toFIXED(1) - dimFrame * (toFIXED(1) / 64),
			                       BATTLE_BACKGROUND_COLORS);
		}
		// The top row's pictures stand in for the lowest four's.
		if (swapped == 0)
		{
			if (frame == CATASTROPHE_ROW_SWAP_TICK)
			{
				swapped = 1;
				for (i = 0; i != CATASTROPHE_DEMON_SWAPPED_COUNT; i++)
				{
					void *tiles = (void *)OBJ_TILES(state->sprites[CATASTROPHE_DEMON_SWAPPED + i]);

					syWordsCopy(tiles, cellBuffer + i * OBJ_32X32_8BPP_BYTES, OBJ_32X32_8BPP_BYTES);
				}
			}
		}
		else if (frame == CATASTROPHE_ROW_RESTORE_TICK)
		{
			Uint8 *cells;

			swapped = 0;
			cells   = RamMapMetatileBuffer + CATASTROPHE_DEMON_KEPT_PICTURES;
			for (i = 0; i != CATASTROPHE_DEMON_SWAPPED_COUNT; i++)
			{
				syWordsCopy((void *)OBJ_TILES(state->sprites[CATASTROPHE_DEMON_SWAPPED + i]),
				            cells + i * OBJ_32X32_8BPP_BYTES,
				            OBJ_32X32_8BPP_BYTES);
			}
		}
		if (frame <= 593)
		{
			spriteScale = CatastropheUnitScale;
			place[3]    = 0;
			place[1]    = toFIXED(255);

			for (i = 0; i != CATASTROPHE_DEMON_COUNT; i++)
			{
				place[0] = toFIXED(CatastropheDemonX[i]) + toFIXED(8);
				place[2] = toFIXED(CatastropheDemonZ[i]) + cameraZ;
				if (place[2] > toFIXED(16) - 1)
				{
					if (place[2] <= toFIXED(168))
					{
						fdRenderApplyProjectedPlacementStub(
							state->sprites[i], place, (Sint32 *)&spriteScale, 0);
					}
				}
			}
			for (i = 0; i != CATASTROPHE_SHIELD_COUNT; i++)
			{
				Sint32 row = i / 2;

				place[0]   = toFIXED((i - row * 2) * 32) + drift + toFIXED(72);
				place[2]   = toFIXED(row * 32) + cameraZ + sway + toFIXED(60);
				if (place[2] > toFIXED(16) - 1)
				{
					if (place[2] <= toFIXED(168))
					{
						fdRenderApplyProjectedPlacementStub(state->sprites[CATASTROPHE_SHIELD + i],
						                                    place,
						                                    (Sint32 *)&spriteScale,
						                                    0);
					}
				}
			}
			for (i = 0; i != CATASTROPHE_SWORD_COUNT; i++)
			{
				place[0] = toFIXED(CatastropheArmourX[i]) - drift + toFIXED(132);
				place[2] = toFIXED(CatastropheArmourZ[i]) + cameraZ + sway + toFIXED(29);
				if (place[2] > toFIXED(16) - 1)
				{
					if (place[2] <= toFIXED(168))
					{
						fdRenderApplyProjectedPlacementStub(state->sprites[CATASTROPHE_SWORD + i],
						                                    place,
						                                    (Sint32 *)&spriteScale,
						                                    0);
					}
				}
			}
			spriteScale.x = -(toFIXED(1) + 1);
			for (i = 0; i != CATASTROPHE_MIRRORED_DEMON_COUNT; i++)
			{
				place[0] = toFIXED(CatastropheMirroredDemonX[i]) + toFIXED(8);
				place[2] = toFIXED(CatastropheMirroredDemonZ[i]) + cameraZ;
				if (place[2] > toFIXED(16) - 1)
				{
					if (place[2] <= toFIXED(168))
					{
						fdRenderApplyProjectedPlacementStub(
							state->sprites[CATASTROPHE_MIRRORED_DEMON + i],
							place,
							(Sint32 *)&spriteScale,
							0);
					}
				}
			}
		}
		verts             = syHeapAllocateEWRAM(512);
		model             = btRenderAllocateModelList(1);
		texture.logWidth  = 5;
		shade             = 0;
		texture.logHeight = 6;
		model->mesh       = CatastropheForkMesh;
		model->texture    = &texture;
		model->kind       = MODEL_SHADED_MAX;
		model->vertices   = verts;
		// Forks of lightning flash in the sky, each fading over eight ticks.
		for (i = 0; i != 7; i++)
		{
			Sint32 st = CatastropheForkStarts[i];

			if (frame >= st && frame < st + 8)
			{
				Sint32 c       = CatastropheForkPictures[i];

				shade          = (st - frame) << 3;
				model->shade   = shade;
				texture.pixels = state->sheet + c * PICTURE_BYTES(LIGHTNING_FORK);
				syInitMatrix();
				syTranslate(
					INTtoFIXED(CatastropheForkX[i] - 48), INTtoFIXED(CatastropheForkY[i] - 44), 0);
				syRotZ(0);
				syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1) / 2);
				btRenderTransformVertices(BattleEffectSquareCorners, verts, 4);
				btRenderDrawModelList(model);
			}
		}
		if (frame == CATASTROPHE_STRIKE_TICK)
		{
			for (i = 0; i != CATASTROPHE_STRIKES; i++)
			{
				state->particles[i].variant = 0;
			}
		}
		for (j = CATASTROPHE_STRIKES; j != ARRAY_COUNT(state->particles); j++)
		{
			p = &state->particles[j];
			if (p->variant >= 0)
			{
				Sint32 n = (p->variant >> 3) + 1;

				blit[0](canvas,
				        sprites + BattleEffectMotePictures[n - 1],
				        FIXED_INTEGER_PART(p->x) - n / 2,
				        FIXED_INTEGER_PART(p->y) - n,
				        n,
				        n * 2);
				btParticleAdvanceWithGravity2D(p, 63, 0);
				p->variant--;
			}
		}
		for (i = 0; i != CATASTROPHE_CANVAS_FLASHES; i++)
		{
			Uint16 t = CatastropheCanvasFlashTicks[i];

			if (frame == t + CATASTROPHE_CANVAS_FLASH_START)
			{
				EFFECT_SET_TRANSFER(
					state, CANVAS_COPY_AND_CLEAR, CANVAS_FILL_WORD(CATASTROPHE_FLASH_LEVEL));
			}
			if (frame == t + CATASTROPHE_CANVAS_FLASH_START + 1)
			{
				EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
			}
		}
		if ((Uint32)(frame - CATASTROPHE_FLICKER_TICK) <= 55)
		{
			if (frame & 1)
			{
				EFFECT_SET_TRANSFER(
					state, CANVAS_COPY_AND_CLEAR, CANVAS_FILL_WORD(CATASTROPHE_FLASH_LEVEL));
			}
			else
			{
				EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
			}
		}
		if (frame == CATASTROPHE_REVEAL_TICK)
		{
			sySoundPlayStub(SE_CURSE_FALL);
		}
		if (frame == CATASTROPHE_SEAL_TICK + 8)
		{
			sySoundPlayStub(SE_SHIMMER);
		}
		if (frame == CATASTROPHE_LANDING_TICK)
		{
			sySoundPlayStub(SE_CRASH);
		}
		texture.logWidth  = 5;
		texture.logHeight = 7;
		model->shade      = shade;
		model->kind       = MODEL_SHADED_MAX;
		model->mesh       = BattleEffectColumnMesh;
		model->texture    = &texture;
		model->vertices   = verts;
		// From CATASTROPHE_STRIKE_TICK three strikes step through their tables at half speed: each
		// step turns its jagged streak, plays its sound and throws sparks.
		if ((Uint32)(frame - CATASTROPHE_STRIKE_TICK) <= 130)
		{
			shade = 0;
			for (i = 0; i != 3; i++)
			{
				Sint32 off;
				Sint32 index;

				p     = &state->particles[i];
				off   = i * CATASTROPHE_STRIKE_STEPS;
				index = p->variant;

				if (frame
				    == (CatastropheStrikeTicks[i * (CATASTROPHE_STRIKE_STEPS + 1) + index] - 492)
				               / 2
				           + CATASTROPHE_STRIKE_TICK)
				{
					Sint32        count = 0;
					const Sint16 *table = CatastropheStrikeRolls;
					Sint32        n     = off + index;
					Sint32        k     = n * 2;
					Sint32        a     = *(const Sint16 *)((Sint32)table + k);
					Uint8        *src;
					Sint32        bx;
					Sint32        height;
					Sint32        width;
					Sint32        x;
					Sint32        x0;
					Sint32        y0;
					Sint32        y;

					if (a != 0)
					{
						SET_POSITION(
							p, CatastropheStrikeX[n] << 3, CatastropheStrikeY[n] << 3, a << 8);
					}
					p->velocityY = CatastropheStrikeSpeedsY[p->variant + off];
					p->velocityZ = CatastropheStrikeTurns[p->variant + off];
					p->velocityX = CatastropheStrikePictures[p->variant + off];
					p->variant++;
					if (p->velocityX == -1)
					{
						continue;
					}
					width  = 10;
					height = 20;
					src    = sprites + BattleEffectMotePictures[MOTE_PICTURES - 1];
					x0     = p->x / 8 - 5;
					bx     = span - drift;
					x      = x0 + (FIXEDtoINT(bx / 2));
					y0     = p->y / 8 - 10;
					y      = y0 + (FIXEDtoINT(cameraZ + sway));
					blit[0](canvas, src, x, y, width, height);
					switch (i)
					{
						case 0: sySoundPlayStub(SE_SOL_SANCTUM_LIGHT_UP); break;
						case 1: sySoundPlayStub(SE_FALLING_LIGHT); break;
						case 2: sySoundPlayStub(SE_WHIRLWIND); break;
					}
					{
						Sint32 half;
						Sint32 j;

						j    = CATASTROPHE_STRIKES;
						q    = &state->particles[CATASTROPHE_STRIKES];
						half = (span - drift) / 2;
						for (; j != ARRAY_COUNT(state->particles); j++, q++)
						{
							if (q->variant < 0)
							{
								Sint32 angle;
								Sint32 speed;

								q->x         = p->x * (toFIXED(1) / 8) + half;
								q->y         = p->y * (toFIXED(1) / 8) + cameraZ + sway;
								angle        = syRandom();
								speed        = (syRandom() & 255) + 127;
								q->velocityX = sySin(angle) * speed >> 6;
								q->velocityY = syCos(angle) * speed >> 6;
								q->variant   = (syRandom() & 7) + 16;
								if (++count == CATASTROPHE_STRIKE_SPARKS)
								{
									break;
								}
							}
						}
					}
				}
				if (p->velocityX != -1)
				{
					Sint32 cell    = p->velocityX;

					model->shade   = shade;
					texture.pixels = state->sheet + CATASTROPHE_JAGGED_STREAKS
					               + cell * PICTURE_BYTES(JAGGED_STREAK);
					syInitMatrix();
					{
						Sint32 t = p->x / 8;
						Sint32 b = span - drift;

						syTranslate(INTtoFIXED(t - CANVAS_SIZE / 2) + b / 2,
						            INTtoFIXED(p->y / 8 - CANVAS_SIZE / 2) + cameraZ + sway,
						            0);
					}
					syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1));
					syRotZ(p->z);
					syScale(toFIXED(4), toFIXED(1) + 20000, toFIXED(1) / 2);
					btRenderTransformVertices(BattleEffectLongBeamCorners, verts, 4);
					btRenderDrawModelList(model);
					p->y += p->velocityY * 2;
					p->z += p->velocityZ * 2;
				}
			}
		}
		if (frame == CATASTROPHE_LANDING_TICK)
		{
			syWordsFillEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
		}
		if (frame == CATASTROPHE_AFTERFLASH_TICK)
		{
			syWordsFillEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(CATASTROPHE_FLASH_LEVEL));
		}
		if (frame == CATASTROPHE_AFTERFLASH_TICK + 4)
		{
			syWordsFillEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(CATASTROPHE_FLASH_LEVEL));
		}
		if ((Uint32)(frame - CATASTROPHE_SEAL_TICK) <= 43)
		{
			p = state->particles;
			if (!(frame & 1))
			{
				p->y = (syRandom() & 0x3fff) + toFIXED(0.375);
				p->z = syRandom() % 3 * PICTURE_BYTES(JAGGED_STREAK) + CATASTROPHE_JAGGED_STREAKS;
			}
			texture.pixels = state->sheet + p->z;
			syInitMatrix();
			syTranslate((span - drift) / 2 + toFIXED(4), cameraZ + toFIXED(204), 0);
			syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1));
			syRotZ(p->y);
			syScale(toFIXED(4), toFIXED(1) + 20000, toFIXED(1) / 2);
			btRenderTransformVertices(BattleEffectLongBeamCorners, verts, 4);
			btRenderDrawModelList(model);
		}
		// Jagged streaks burst where the orb lands, and at the afterflashes.
		if ((Uint32)(frame - CATASTROPHE_LANDING_TICK) <= 15
		    || (Uint32)(frame - CATASTROPHE_AFTERFLASH_TICK) <= 3)
		{
			Sint32 n      = 1;
			Sint32 sx     = toFIXED(5);
			Sint32 base   = syRandom() % 3;
			Sint32 spread = 8;

			if (frame > CATASTROPHE_LANDING_TICK + 9)
			{
				sx -= (frame - (CATASTROPHE_LANDING_TICK + 10)) * CATASTROPHE_LANDING_BURST_SHRINK;
			}
			if (sx < toFIXED(1))
			{
				sx = toFIXED(1);
			}
			if (frame <= 613)
			{
				if (frame > 605)
				{
					n = 3;
				}
				else if (frame > 603)
				{
					n = 2;
				}
			}
			model->kind  = MODEL_SHADED_MAX;
			model->shade = shade;
			for (i = 0; i != n; i++)
			{
				PARTICLE *p4 = &state->particles[i];

				SET_POSITION(p4,
				             toFIXED(syRandom() % spread - spread / 2),
				             cameraZ + toFIXED(200),
				             (base + i) % 3 * PICTURE_BYTES(JAGGED_STREAK)
				                 + CATASTROPHE_JAGGED_STREAKS);
				texture.pixels = state->sheet + p4->z;
				syInitMatrix();
				syTranslate(toFIXED(1), p4->y, 0);
				syScale(sx, toFIXED(1), toFIXED(1) / 2);
				syTranslate(p4->x, 0, 0);
				syScale(toFIXED(1) / 2, toFIXED(2), toFIXED(1));
				syRotZ(DEGtoANG(180));
				btRenderTransformVertices(BattleEffectLongBeamCorners, verts, 4);
				btRenderDrawModelList(model);
			}
		}
		if (frame == CATASTROPHE_SEAL_TICK)
		{
			btResourceLoadAndDecompress(
				RESOURCE_SWIRL_TEXTURE, RamMapMetatileBuffer + CATASTROPHE_RING, 0, 0);
		}
		if (frame == CATASTROPHE_SEAL_TICK - 1)
		{
			btResourceLoadAndDecompress(RESOURCE_TRIANGLE_SEAL_TEXTURE, state->sheet, 0, 0);
			btResourceLoadAndDecompress(
				RESOURCE_PENTAGON_SEAL_TEXTURE, state->sheet + CATASTROPHE_PENTAGON_SEAL, 0, 0);
		}
		if (frame == CATASTROPHE_LANDING_TICK)
		{
			btResourceLoadAndDecompress(
				RESOURCE_FIRE_RING_TEXTURE, RamMapMetatileBuffer + CATASTROPHE_RING, 0, 0);
		}
		if ((Uint32)(frame - CATASTROPHE_LANDING_TICK) <= 6)
		{
			cameraZ += toFIXED(16);
		}
		// From CATASTROPHE_SEAL_TICK the seal: the pentagon spins as it swells, its triangles fold
		// up from its edges from tick 580, and the swirl spins over it.
		{
			Sint32 sealFrame = frame - CATASTROPHE_SEAL_TICK;

			if ((Uint32)sealFrame <= 58)
			{
				Sint32 s = sealFrame * 4424;
				Sint32 a;
				Sint32 b;

				shade = -16;
				if (s > CATASTROPHE_SEAL_SCALE_MAX)
				{
					s = CATASTROPHE_SEAL_SCALE_MAX;
				}
				texture.logWidth  = 6;
				texture.logHeight = 6;
				model->shade      = shade;
				model->kind       = MODEL_SHADED_ADDITIVE;
				model->texture    = &texture;
				model->mesh       = BattleEffectSquareMesh;
				model->vertices   = verts;
				texture.pixels    = state->sheet + CATASTROPHE_PENTAGON_SEAL;
				syInitMatrix();
				b = span - drift;
				syTranslate(b / 2 + toFIXED(5), cameraZ + toFIXED(230), 0);
				syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1) / 2);
				syRotX(CATASTROPHE_SEAL_TILT);
				a = -frame * (TRIG_TURN / 64);
				syRotZ(a);
				syMatrixScaleUniform(s);
				syTranslate(0, -toFIXED(15), 0);
				btRenderTransformVertices(BattleEffectSquareCorners, verts, 4);
				syTranslate(0, toFIXED(15), 0);
				btRenderDrawModelList(model);
				texture.pixels = state->sheet;
				for (i = 0; i != 3; i++)
				{
					syPushMatrix();
					syRotZ(i * (DEGtoANG(120)) + DEGtoANG(60));
					{
						Sint32 phase = i * (DEGtoANG(120)) + DEGtoANG(60);
						model->shade = -24 - (FIXEDtoINT(syCos(phase + a) << 3));
					}
					syTranslate(0, -toFIXED(16), 0);
					if (frame > 579)
					{
						Sint32 v = (frame - 580) * 1096;

						if (v > CATASTROPHE_SEAL_SIDE_TILT)
						{
							v = CATASTROPHE_SEAL_SIDE_TILT;
						}
						syRotX(-v);
					}
					btRenderTransformVertices(BattleEffectStandingSquareCorners, verts, 4);
					btRenderDrawModelList(model);
					syPopMatrix();
				}
				if (frame > 589)
				{
					s += (frame - 590) * 11060;
				}
				if (s > 0 && frame <= 601)
				{
					shade = -16;
					if (frame > 589)
					{
						shade -= (frame - 590) * 2;
					}
					if (shade > MODEL_SHADE_INVISIBLE)
					{
						model->shade   = shade;
						texture.pixels = RamMapMetatileBuffer + CATASTROPHE_RING;
						syInitMatrix();
						if (frame > CATASTROPHE_ORB_TICK - 1)
						{
							Sint32 v = (CATASTROPHE_ORB_TICK - frame) * CATASTROPHE_SWIRL_RISE;

							if (v < -CATASTROPHE_SWIRL_RISE_TICKS * CATASTROPHE_SWIRL_RISE)
							{
								v = -CATASTROPHE_SWIRL_RISE_TICKS * CATASTROPHE_SWIRL_RISE;
							}
							syTranslate(0, v, 0);
						}
						syTranslate((span - drift) / 2 + toFIXED(5), cameraZ + toFIXED(220), 0);
						syScale(toFIXED(1), 32000, toFIXED(1));
						syRotZ(frame * (TRIG_TURN / 32));
						syMatrixScaleUniform(s);
						btRenderTransformVertices(BattleEffectSquareCorners, verts, 4);
						btRenderDrawModelList(model);
					}
				}
			}
		}
		// From the landing a ring of fire spreads, fading ten ticks on.
		if (frame > 601)
		{
			Sint32 s =
				(frame - CATASTROPHE_LANDING_TICK) * CATASTROPHE_RING_GROWTH + toFIXED(0.125);

			shade = 0;
			if (frame > 611)
			{
				shade -= (frame - 612) * 8;
			}
			if (shade > MODEL_SHADE_INVISIBLE)
			{
				model->shade      = shade;
				model->mesh       = BattleEffectLargeSquareMesh;
				model->kind       = MODEL_SHADED_ADDITIVE;
				texture.pixels    = RamMapMetatileBuffer + CATASTROPHE_RING;
				texture.logWidth  = 7;
				texture.logHeight = 7;
				syInitMatrix();
				syTranslate(toFIXED(1), -toFIXED(16), 0);
				syScale(toFIXED(1), 32000, toFIXED(1));
				syRotZ(frame * (TRIG_TURN / 64));
				syMatrixScaleUniform(s);
				btRenderTransformVertices(BattleEffectSquareCorners, verts, 4);
				btRenderDrawModelList(model);
			}
		}
		syHeapRelease(model);
		syHeapRelease(verts);
		// The orb swells, rising a little, and from the tick before the landing it falls.
		if ((Uint32)(frame - CATASTROPHE_ORB_TICK) <= 51)
		{
			Sint32 radius = frame - (CATASTROPHE_ORB_TICK - 1);
			Sint32 y      = 90 - (frame - CATASTROPHE_ORB_TICK) / 2;

			if (frame > 600)
			{
				y += (frame - 601) * 16;
			}
			if (y <= 63)
			{
				y = 64;
			}
			if (radius <= 38)
			{
				btEffectDrawShadedCircles(cellBuffer, radius, CATASTROPHE_ORB_BRIGHTNESS);
			}
			else
			{
				radius = 38;
			}
			btCanvasBlitMirroredQuarters(cellBuffer, 62, y, radius);
		}
		EFFECT_PRESENT_CANVAS(state);
		frame++;
	}
	for (i = 0; i != CATASTROPHE_SPRITES; i++)
	{
		fdSpriteDestroyStub(state->sprites[i]);
	}
	fdActorShadowLoadTilesStub();
	battle->arrowTiles = syVRAMCacheAllocateEntry(128);
	btEffectShowLivingTargets(state->args);
	for (i = 0; i != args->targetCount; i++)
	{
		btEffectTintBattler(args->targets[i], SPRITE_COLOR_GRB, TINT_KEEP, TINT_KEEP, 0);
		btEffectTintBattler(args->targets[i], SPRITE_COLOR_NORMAL, TINT_KEEP, TINT_KEEP, 0);
	}
	syPaletteLoadBasicSpriteColors();
	btEffectSetupBlendedDisplay();
	ProjectionWork.centerY = SUMMON_PROJECTION_CENTER_Y;
	state->fadeFrame       = 0;
	state->fadeStep        = 0;
	// Act two: the land seen from the sky, where the orb falls and bursts.
	BgScrollWork[1].y      = BATTLE_BACKGROUND_SCROLL_Y;
	btBackgroundLoadStub(BATTLE_BACKGROUND_FIXED, RESOURCE_AERIAL_VIEW_BACKDROP, 0);
	btPaletteBrightenSavedBattleBg(0, -16, -16);
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	btResourceLoadAndDecompress(RESOURCE_LIGHTNING_BOLT_SHEET, cellBuffer, 1, 1);
	btResourceLoadAndDecompress(
		RESOURCE_TORNADO_SHEET, state->sheet + CATASTROPHE_SWEEP_PICTURES, 1, 0);
	syWordsClearEntry(state->sheet + CATASTROPHE_BOLT_TEXTURES,
	                  CATASTROPHE_BOLT_TEXTURE_COUNT * PICTURE_BYTES(BOLT_TEXTURE));
	for (i = 0; i != CATASTROPHE_BOLT_TEXTURE_COUNT; i++)
	{
		{
			Sint32 y;
			Sint32 boltTexture = i * PICTURE_BYTES(BOLT_TEXTURE);
			Uint8 *cells       = cellBuffer;

			for (y = 0; y != LIGHTNING_BOLT_HEIGHT; y++)
			{
				Uint8 *line   = (Uint8 *)state + (boltTexture + y * BOLT_TEXTURE_WIDTH);
				Sint32 offset = i * LIGHTNING_BOLT_PICTURE_BYTES + y * LIGHTNING_BOLT_WIDTH;
				Uint8 *dest   = line
				            + (offsetof(BATTLE_EFFECT_STATE, sheet) + CATASTROPHE_BOLT_TEXTURES
				               + (BOLT_TEXTURE_WIDTH - LIGHTNING_BOLT_WIDTH) / 2);
				Uint8 *src = cells + offset;
				Sint32 x;

				for (x = 0; x != LIGHTNING_BOLT_WIDTH; x++)
				{
					*dest++ = *src++;
				}
			}
		}
	}
	btResourceLoadAndDecompress(RESOURCE_TRIANGLE_SEAL_TEXTURE, cellBuffer, 0, 0);
	btResourceLoadAndDecompress(
		RESOURCE_PENTAGON_SEAL_TEXTURE, RamMapMetatileBuffer + CATASTROPHE_PENTAGON_SEAL, 0, 0);
	btResourceLoadAndDecompress(
		RESOURCE_FIRE_RING_TEXTURE, RamMapMetatileBuffer + CATASTROPHE_RING, 0, 0);
	{
		FIXED  rise;
		Sint32 top;
		void  *aerialVertices;
		Sint32 aerialShade;
		Sint32 radius;
		Sint32 ry;

		rise = 0;
		EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
		frame = 0;
		if (!(PerWork.repeat & (KEY_A | KEY_B)))
		{
			do
			{
				if (frame == 0)
				{
					syWordsFillEntry(canvas, CANVAS_BYTES, 0);
					rise = -toFIXED(64);
					btPaletteBrightenSavedBattleBg(0, -16, -16);
					EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
					syWordsCopy((void *)BG_PLTT,
					            syResourceGetTableEntry(RESOURCE_VIOLET_PALETTE_F),
					            SHEET_PALETTE_BYTES);
				}
				if (rise <= toFIXED(16) - 1)
				{
					rise += toFIXED(1);
				}
				{
					Sint32 launchFrame = frame - 96;

					if ((Uint32)launchFrame <= 23)
					{
						btPaletteBrightenSavedBattleBg(-launchFrame, -16, frame - 112);
						state->shakeFrame = 8;
					}
				}
				if (frame > 139)
				{
					state->transferMode = CANVAS_COPY_AND_CLEAR;
					if (frame > 153)
					{
						state->transferValue = CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST);
					}
					else
					{
						state->transferValue = CANVAS_FILL_WORD(CATASTROPHE_WASH_LEVEL);
					}
					btPaletteBrightenBgEntries(1, 1, 1);
				}
				if (frame == 80 || frame == 83)
				{
					Uint16 *c = (Uint16 *)&BG_PLTT_COLOR(6, 0);

					for (i = 0; i != BATTLE_BACKGROUND_COLORS; i++, c++)
					{
						*c ^= RGB_WHITE;
					}
				}
				if (frame == 82)
				{
					state->shakeFrame = 8;
					sySoundPlayStub(SE_BANE_ESCAPE);
					IWRAM_FILL_WORDS(
						canvas, CANVAS_BYTES, CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
				}
				if (frame == 96)
				{
					sySoundPlayStub(SE_LAUNCH);
				}
				if (frame == 128)
				{
					Uint16 *c;

					state->shakeFrame = 16;
					c                 = (Uint16 *)&BG_PLTT_COLOR(6, 0);
					for (i = 0; i != BATTLE_BACKGROUND_COLORS; i++, c++)
					{
						*c ^= RGB_WHITE;
					}
				}
				if (frame > 128)
				{
					Sint32 d = (frame - 128) << 1;

					btPaletteBrightenSavedBattleBg(d - 11, frame - 136, 3 - d);
				}
				radius = 16;
				ry     = FIXEDtoINT(rise);
				if (frame > 92)
				{
					radius = frame - 75;
				}
				if (frame <= 79)
				{
					radius += FIXEDtoINT(sySin(frame * (TRIG_TURN / 32)) * 2);
				}
				if (radius <= 64)
				{
					btEffectDrawShadedCircles(state->sheet, radius, CATASTROPHE_ORB_BRIGHTNESS);
				}
				else
				{
					radius = 64;
				}
				top = ry - radius / 2 + CATASTROPHE_ORB_CENTER;
				btCanvasBlitMirroredQuarters(state->sheet, CATASTROPHE_ORB_CENTER, top, radius);
				// From tick 96 glints sparkle around the orb.
				if (frame > 95)
				{
					{
						for (i = 0; i != 4; i++)
						{
							Sint32 n     = i & 3;
							Sint32 angle = syRandom();
							Sint32 x =
								(FIXEDtoINT(sySin(angle) * radius) >> 1) + CATASTROPHE_ORB_CENTER;
							Sint32 y;
							Uint32 flip;

							x -= BattleEffectGlintPictureWidths[n] >> 1;
							y  = top + (FIXEDtoINT(syCos(angle) * radius) >> 1)
							  - (BattleEffectGlintPictureHeights[n] >> 1);
							flip = syRandom();
							EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
							                    CatastropheGlintFlips[flip & 3]
							                        | EFFECT_CLIP_BOUNDS,
							                    BLITTER_ADDITIVE);
							(RamHeapSlots->ramCodePaired)(canvas,
							                              state->sheet + CATASTROPHE_SWEEP_PICTURES
							                                  + BattleEffectGlintPictures[n],
							                              x,
							                              y,
							                              BattleEffectGlintPictureWidths[n],
							                              BattleEffectGlintPictureHeights[n]);
							syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
						}
					}
				}
				aerialVertices = syHeapAllocateEWRAM(512);
				aerialModel    = btRenderAllocateModelList(1);
				// Until tick 127 the seal turns under the orb, swelling from tick 72, its triangles
				// folding down from tick 64.
				if (frame <= 127)
				{
					Sint32 s = CATASTROPHE_AERIAL_SEAL_SCALE;
					Sint32 a;

					if (frame > 71)
					{
						s = (frame - 72) * (toFIXED(1) / 32) + CATASTROPHE_AERIAL_SEAL_SCALE;
					}
					aerialShade = -16;
					if (frame > 119)
					{
						aerialShade = 944 - frame * 8;
					}
					if (s > CATASTROPHE_SEAL_SCALE_MAX)
					{
						s = CATASTROPHE_SEAL_SCALE_MAX;
					}
					aerialTexture.logWidth  = 6;
					aerialTexture.logHeight = 6;
					aerialModel->shade      = aerialShade;
					aerialModel->kind       = MODEL_SHADED_MAX;
					aerialModel->texture    = &aerialTexture;
					aerialModel->mesh       = BattleEffectSquareMesh;
					aerialModel->vertices   = aerialVertices;
					aerialTexture.pixels    = RamMapMetatileBuffer + CATASTROPHE_PENTAGON_SEAL;
					syInitMatrix();
					syTranslate(-toFIXED(2), rise, 0);
					syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1) / 2);
					syRotX(CATASTROPHE_SEAL_TILT);
					a = frame * (TRIG_TURN / 128);
					syRotZ(a);
					syMatrixScaleUniform(s);
					syTranslate(0, -toFIXED(15), 0);
					btRenderTransformVertices(BattleEffectSquareCorners, aerialVertices, 4);
					syTranslate(0, toFIXED(15), 0);
					btRenderDrawModelList(aerialModel);
					aerialTexture.pixels = cellBuffer;
					for (i = 0; i != 3; i++)
					{
						Sint32 k         = i * (DEGtoANG(120)) + DEGtoANG(60);
						Sint32 sideShade = aerialShade + (FIXEDtoINT(syCos(a + k) << 3)) - 16;
						Sint32 v;

						syPushMatrix();
						syRotZ(i * (DEGtoANG(120)) + DEGtoANG(60));
						aerialModel->shade = sideShade;
						syTranslate(0, -toFIXED(16), 0);
						v = CATASTROPHE_SEAL_SIDE_TILT;
						if (frame > 63)
						{
							v -= (frame - 64) * (TRIG_TURN / 64);
						}
						if (v < 0)
						{
							v = 0;
						}
						syRotX(-v);
						btRenderTransformVertices(
							BattleEffectStandingSquareCorners, aerialVertices, 4);
						btRenderDrawModelList(aerialModel);
						syPopMatrix();
					}
				}
				// Two rings of fire spread from ticks 80 and 88, the second at half the speed.
				for (i = 0; i != 2; i++)
				{
					Sint32 delay = i * 8;
					Sint32 start = delay + 80;
					Sint32 s;

					if (frame < start)
					{
						continue;
					}
					aerialShade = 0;
					if (i == 0)
					{
						s = (frame - start) * CATASTROPHE_RING_GROWTH + toFIXED(0.125);
						if (frame >= delay + 96)
						{
							aerialShade = (start - frame) * 8 + 128;
						}
					}
					else
					{
						s = (frame - start) * (CATASTROPHE_RING_GROWTH / 2) + toFIXED(0.125);
						if (frame >= delay + 112)
						{
							aerialShade = (start - frame) * 8 + 256;
						}
					}
					if (aerialShade > MODEL_SHADE_INVISIBLE)
					{
						aerialModel->shade      = aerialShade;
						aerialModel->mesh       = BattleEffectLargeSquareMesh;
						aerialModel->kind       = MODEL_SHADED_MAX;
						aerialTexture.pixels    = RamMapMetatileBuffer + CATASTROPHE_RING;
						aerialTexture.logWidth  = 7;
						aerialTexture.logHeight = 7;
						syInitMatrix();
						syTranslate(0, toFIXED(16), 0);
						syScale(toFIXED(1), 32000, toFIXED(1));
						syRotZ(frame * (TRIG_TURN / 64));
						syMatrixScaleUniform(s);
						btRenderTransformVertices(BattleEffectSquareCorners, aerialVertices, 4);
						btRenderDrawModelList(aerialModel);
					}
				}
				aerialTexture.logWidth  = 5;
				aerialTexture.logHeight = 7;
				aerialModel->kind       = MODEL_SHADED_MAX;
				aerialModel->mesh       = BattleEffectColumnMesh;
				shade                   = 0;
				aerialModel->shade      = shade;
				if (frame == 0)
				{
					p = state->particles;
					for (i = 0; i != CATASTROPHE_BOLTS; p++, i++)
					{
						p->z         = CatastropheBoltRolls[i];
						p->velocityZ = (syRandom() & 255) + 128;
						if (syRandom() & 1)
						{
							p->velocityZ = -p->velocityZ;
						}
						p->variant = syRandom() & 3;
					}
				}
				for (i = 0; i != 8; i++)
				{
					if (frame == 128 + i * 4)
					{
						sySoundPlayStub(SE_CLATTER);
					}
				}
				// The bolts shoot out from the middle, one a tick from tick 128, each turning as it
				// grows.
				p = state->particles;
				for (i = 0; i != CATASTROPHE_BOLTS; i++, p++)
				{
					Sint32 startFrame = i + 128;

					if (frame >= startFrame && frame < i + 144)
					{
						Sint32 s = (frame - startFrame) * (toFIXED(1) / 8);

						if (s > toFIXED(1))
						{
							s = toFIXED(1);
						}
						aerialTexture.pixels = state->sheet + CATASTROPHE_BOLT_TEXTURES
						                     + p->variant * PICTURE_BYTES(BOLT_TEXTURE);
						syInitMatrix();
						syRotZ(p->z);
						p->z += p->velocityZ;
						syScale(s << 2, s, toFIXED(1));
						btRenderTransformVertices(
							BattleEffectTaperedBeamCorners, aerialVertices, 4);
						btRenderDrawModelList(aerialModel);
					}
				}
				syHeapRelease(aerialModel);
				syHeapRelease(aerialVertices);
				// At tick 134 sparks ring the orb and twinkle out.
				if (frame > 133)
				{
					if (frame == 134)
					{
						PARTICLE *p;

						i = 0;
						p = &state->particles[CATASTROPHE_BOLTS];
						for (; i != CATASTROPHE_SPARKS; i++, p++)
						{
							Sint32 random   = syRandom();
							Sint32 angle    = i * (TRIG_TURN / CATASTROPHE_SPARKS);
							Sint32 distance = (random & 31) + 32;

							SET_POSITION(
								p,
								(FIXEDtoINT(sySin(angle) * distance)) + CATASTROPHE_ORB_CENTER,
								(FIXEDtoINT(syCos(angle) * distance)) + CATASTROPHE_ORB_CENTER,
								(syRandom() & 3) + 5);
							p->variant = -(syRandom() & 15);
						}
					}
					i     = 0;
					spark = &state->particles[CATASTROPHE_BOLTS];
					for (; i != CATASTROPHE_SPARKS; i++, spark++)
					{
						if ((Uint32)spark->variant <= 7)
						{
							Sint32 n =
								(FIXEDtoINT(sySin(spark->variant * (TRIG_TURN / 16)) * spark->z))
								+ 1;

							blit[0](canvas,
							        sprites + BattleEffectMotePictures[n - 1],
							        spark->x - n / 2,
							        spark->y - n,
							        n,
							        n * 2);
						}
						spark->variant++;
					}
				}
				if (state->shakeFrame > 0)
				{
					Sint32 m           = 7;
					Sint32 a           = syRandom();
					Sint32 b           = syRandom();

					a                 &= m;
					b                 &= m;
					a                 -= 4;
					b                 += BATTLE_BACKGROUND_SCROLL_Y - 4;
					BgScrollWork[1].x  = a;
					BgScrollWork[1].y  = b;
					state->shakeFrame--;
				}
				else
				{
					BgScrollWork[1].x = 0;
					BgScrollWork[1].y = state->savedBg1Y;
				}
				EFFECT_PRESENT_CANVAS(state);
				frame++;
			} while (frame != 160 && !(PerWork.repeat & (KEY_A | KEY_B)));
		}
	}
	{
		FIXED     slide;
		Sint32    slideStep;
		Uint8    *bolts;
		void     *quads  = RamMapMetatileBuffer + CATASTROPHE_SWEEP_VERTICES;
		void     *strip  = RamMapMetatileBuffer + CATASTROPHE_SWEEP_STRIP;
		PARTICLE *bursts = CATASTROPHE_PARTICLES;
		PARTICLE *debris = &CATASTROPHE_PARTICLES[CATASTROPHE_BURSTS];

		// Act three: back on the battlefield, lightning and smoke sweep the foes.
		bolts            = RamMapMetatileBuffer + CATASTROPHE_BOLT_PICTURES;
		slide            = 0;
		slideStep        = 0;
		syWordsFill(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
		EFFECT_PRESENT_CANVAS(state);
		BgScrollWork[1].x = savedScroll;
		btBackgroundLoadStub(BATTLE_BACKGROUND_FIXED, battle->bg, BACKGROUND_DARKENED_LEVEL);
		btPaletteBrightenSavedBattleBg(-16, -16, -4);
		btResourceLoadAndDecompress(RESOURCE_SMOKE_BAND_SHEET, state->sheet, 1, 1);
		btResourceLoadAndDecompress(
			RESOURCE_CLAW_SLASH_SHEET, state->sheet + CATASTROPHE_SWEEP_PICTURES, 1, 0);
		btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprites, 0, 0);
		btResourceLoadAndDecompress(
			RESOURCE_FLASH_BURST_SHEET, state->sheet + CATASTROPHE_FLASH_BURST_PICTURES, 1, 0);
		btResourceLoadAndDecompress(RESOURCE_LIGHTNING_BOLT_SHEET, bolts, 1, 0);
		btRenderBuildQuadStrip(strip, 4, 32, 11);
		REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
		frame      = 0;
		do
		{
			if ((PerWork.repeat & (KEY_A | KEY_B)) && frame > 2 && frame <= 87)
			{
				frame = 88;
			}
			if (frame == 88)
			{
				syWordsFillEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
				EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(2));
				BgScrollWork[1].x      = savedScroll;
				ProjectionWork.centerX = BATTLE_PROJECTION_CENTER_X;
				state->scrollStepX     = 0;
				EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
			}
			if (frame == 0)
			{
				IWRAM_FILL_WORDS(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
				state->scrollStepX     = -CATASTROPHE_PAN_SPEED;
				ProjectionWork.centerX = BATTLE_PROJECTION_CENTER_X - CATASTROPHE_PAN;
				ProjectionWork.centerY = BATTLE_PROJECTION_CENTER_Y;
				BgScrollWork[1].x      = savedScroll + CATASTROPHE_PAN;
				btGraphicsLoadGreyRamp();
				EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(2));
				for (i = 0; i != CATASTROPHE_BURSTS + CATASTROPHE_DEBRIS; i++)
				{
					CATASTROPHE_PARTICLES[i].variant = -1;
				}
				slide     = toFIXED(128);
				slideStep = 0;
			}
			if (frame == 16)
			{
				EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
			}
			if ((Uint32)(frame - 16) <= 47)
			{
				btEffectStepPaletteToResource(RESOURCE_VIOLET_PALETTE_F);
			}
			// At tick 66 the targets are struck: thrown back, flashing white, debris flying from
			// each.
			if (frame == 66)
			{
				for (i = 0; i != args->targetCount; i++)
				{
					btEffectStartActorMotion(
						args->targets[i], 1, toFIXED(4), toFIXED(8), toFIXED(4), 150);
					btEffectTintBattler(args->targets[i], SPRITE_COLOR_WHITE, POSE_DAMAGE, i, 16);
					{
						Sint32 a = args->targets[i];
						Sint32 k = CATASTROPHE_TARGET_DEBRIS * sizeof(PARTICLE);
						btEffectPosProjectBattler(a, &pos);
						j = 0;
						q = (PARTICLE *)(i * k + (Sint32)debris);
					}
					for (; j != CATASTROPHE_TARGET_DEBRIS; j++, q++)
					{
						Sint32 angle;
						Sint32 speed;

						q->x         = toFIXED(pos.x / 2);
						q->y         = toFIXED(80);
						angle        = (syRandom() & 0x7fff) + DEGtoANG(90);
						speed        = (syRandom() & 127) + 128;
						q->velocityX = sySin(angle) * speed >> 5;
						q->velocityY = syCos(angle) * speed >> 4;
						q->variant   = CATASTROPHE_DEBRIS_LIFE;
					}
				}
				state->shakeFrame = 16;
				sySoundPlayStub(SE_CRASH);
			}
			if ((Uint32)(frame - 72) <= 15)
			{
				EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_BRIGHTEN, CANVAS_FILL_WORD(1));
				btPaletteBrightenBgEntries(1, 1, 1);
			}
			if (frame >= 0)
			{
				Sint32 x = 0;

				if (frame > 71)
				{
					x -= (frame - 72) * 8;
				}
				for (i = 0; i != 2; i++)
				{
					Sint32    sum = x + 116;
					PARTICLE *base =
						(PARTICLE *)((frame & 31) * (2 * sizeof(PARTICLE)) + (Sint32)bursts);
					PARTICLE *q = &base[i];
					Sint32    angle;
					Sint32    speed;

					q->x         = toFIXED(sum);
					q->y         = INTtoFIXED(syRandom() % 112 + 4);
					angle        = (syRandom() & 0x7fff) + DEGtoANG(180);
					speed        = (syRandom() & 127) + 128;
					q->velocityX = sySin(angle) * speed >> 6;
					q->velocityY = syCos(angle) * speed >> 6;
					q->variant   = 0;
				}
				burst = CATASTROPHE_PARTICLES;
				for (i = 0; i != CATASTROPHE_BURSTS; i++, burst++)
				{
					if ((Uint32)burst->variant < CATASTROPHE_BURST_LIFE)
					{
						blit[0](canvas,
						        state->sheet + CATASTROPHE_FLASH_BURST_PICTURES
						            + burst->variant / 2 * PICTURE_BYTES(FLASH_BURST),
						        FIXED_INTEGER_PART(burst->x) - FLASH_BURST_WIDTH / 2,
						        FIXED_INTEGER_PART(burst->y) - FLASH_BURST_HEIGHT / 2,
						        FLASH_BURST_WIDTH,
						        FLASH_BURST_HEIGHT);
						btParticleAdvanceWithGravity2D(burst, 63, 0);
						burst->variant++;
					}
				}
			}
			if (frame <= 87)
			{
				Sint32 off;
				Sint32 jitter = 0;
				Sint32 z;
				Sint32 y;
				Sint32 a;
				Sint32 b;
				Sint32 c;

				j = 0;
				z = 0;
				y = 0;
				for (; j != CATASTROPHE_SWEEP_ROWS; j++)
				{
					MESH_VERTEX *v;
					Sint32       x;

					off = j * rowStep;

					for (i = 0, x = 0, v = (MESH_VERTEX *)(off + (Sint32)cellBuffer); i != 2;
					     i++, x += 10, v++)
					{
						SET_POSITION(v,
						             x + (FIXEDtoINT(sySin((frame + j * 2) * (TRIG_TURN / 64)) * 2))
						                 + jitter - 5,
						             y,
						             z);
					}
					jitter  = jitter + (syRandom() & 7) - 4;
					y      -= 6;
				}
				sweepModel           = btRenderAllocateModelList(1);
				sweepModel->kind     = MODEL_SHADED_MAX;
				sweepModel->texture  = &sweepTexture;
				sweepModel->shade    = 0;
				sweepModel->scrollV  = 0;
				sweepModel->vertices = quads;
				if (frame == 16)
				{
					slideStep = -toFIXED(8);
				}
				if ((Uint32)(frame - 48) <= 5)
				{
					slideStep = toFIXED(8);
				}
				if (frame > 59)
				{
					slideStep = -toFIXED(16);
				}
				if (frame > 31)
				{
					slideStep = slideStep * 48 / DAMPING_SCALE;
				}
				slide                  += slideStep;
				sweepTexture.logWidth   = 6;
				sweepTexture.logHeight  = 5;
				sweepTexture.pixels     = state->sheet;
				sweepModel->mesh        = strip;
				sweepModel->scrollU     = (frame << 3) & TEXTURE_SCROLL_MASK;
				syInitMatrix();
				syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1) / 2);
				a = frame * CATASTROPHE_SWEEP_WAVE;
				b = frame * (TRIG_TURN / 8);
				c = sySin(a);
				syTranslate(slide, (c << 5) + syCos(b) * 24, 0);
				syMatrixScaleUniform(CATASTROPHE_SWEEP_SCALE);
				syPushMatrix();
				syRotZ(DEGtoANG(90));
				syMatrixScaleUniform(CATASTROPHE_BAND_SCALE);
				btRenderTransformVertices(cellBuffer, quads, 24);
				btRenderDrawModelList(sweepModel);
				syPopMatrix();
				sweepTexture.logWidth  = 6;
				sweepModel->mesh       = BattleEffectSquareMesh;
				sweepTexture.logHeight = 6;
				sweepModel->scrollU    = 0;
				sweepTexture.pixels    = state->sheet + CATASTROPHE_SWEEP_PICTURES;
				syTranslate(-toFIXED(2), (sySin(frame * (TRIG_TURN / 64)) << 2) - toFIXED(6), 0);
				syRotZ(-TRIG_TURN / 32);
				{
					FIXED  s0 = sySin(a);
					Sint32 y0 = (s0 << 5) + syCos(b) * 24;
					FIXED  s1;
					FIXED  y1;

					a  += CATASTROPHE_SWEEP_WAVE;
					s1  = sySin(a);
					b  += DEGtoANG(45);
					y1  = (s1 << 5) + syCos(b) * 24;
					syRotZ((y0 - y1) / 512 - TRIG_TURN / 64);
				}
				syScale(135536, 105536, toFIXED(1));
				btRenderTransformVertices(BattleEffectSquareCorners, quads, 4);
				btRenderDrawModelList(sweepModel);
				syHeapRelease(sweepModel);
				if (frame <= 87)
				{

					// Six bolts strike, one every eight ticks from tick 16, each with a clap, a
					// shake and debris thrown up where it lands.
					i    = 0;
					bolt = state->particles;

					for (; i != 6; bolt++, i++)
					{
						Sint32 strikeFrame = i * 8 + 16;

						if (frame == strikeFrame)
						{
							bolt->x           = (syRandom() & 31) + 64;
							bolt->variant     = syRandom();
							state->shakeFrame = 4;
							switch (i & 3)
							{
								case 0: sySoundPlayStub(SE_TOPPLE); break;
								case 1: sySoundPlayStub(SE_THUNDER); break;
								case 2: sySoundPlayStub(SE_BANE_ESCAPE); break;
								case 3: sySoundPlayStub(SE_THUNDER_LOUD); break;
							}
							j = 0;
							q = (PARTICLE *)((i & 7) * (CATASTROPHE_BOLT_DEBRIS * sizeof(PARTICLE))
							                 + (Sint32)debris);
							for (; j != CATASTROPHE_BOLT_DEBRIS; j++, q++)
							{
								Sint32 angle;
								Sint32 speed;

								q->x         = toFIXED(bolt->x);
								q->y         = toFIXED(104);
								angle        = (syRandom() & 0x7fff) + DEGtoANG(90);
								speed        = (syRandom() & 127) + 128;
								q->velocityX = sySin(angle) * speed >> 5;
								q->velocityY = syCos(angle) * speed >> 4;
								q->variant   = CATASTROPHE_DEBRIS_LIFE;
							}
						}
						if (frame >= strikeFrame && frame < strikeFrame + 8)
						{
							Uint8 *src = RamMapMetatileBuffer + CATASTROPHE_BOLT_PICTURES;

							blit[0](canvas,
							        ((bolt->variant / 2) & 3) * PICTURE_BYTES(LIGHTNING_BOLT) + src,
							        bolt->x - LIGHTNING_BOLT_WIDTH / 2,
							        0,
							        LIGHTNING_BOLT_WIDTH,
							        LIGHTNING_BOLT_HEIGHT);
							bolt->variant++;
						}
					}
					{
						PARTICLE *d = debris;

						for (i = 0; i != CATASTROPHE_DEBRIS; i++, d++)
						{
							if (d->variant >= 0)
							{
								Sint32 n = (d->variant >> 2) + 2;

								Sint32 h = n * 2;

								blit[0](canvas,
								        sprites + BattleEffectMotePictures[n - 1],
								        FIXED_INTEGER_PART(d->x) - n / 2,
								        FIXED_INTEGER_PART(d->y) - n,
								        n,
								        h);
								btParticleAdvanceWithGravity2D(d, 56, toFIXED(1) / 8);
								d->velocityX -= state->scrollStepX * (toFIXED(1) / 16);
								d->variant--;
							}
						}
					}
				}
			}
			ProjectionWork.centerX -= state->scrollStepX;
			BgScrollWork[1].x      += state->scrollStepX;
			for (i = 0; i != CATASTROPHE_PAN_SPEED; i++)
			{
				if (frame == i + 62)
				{
					state->scrollStepX++;
				}
			}
			btCameraApplyVerticalShake(8);
			btEffectCountDownBattlerTints();
			{
				Sint32 *pending = &state->transferPending;

				frame++;
				*pending = 1;
			}
			sySynch(1);
		} while (frame != 104);
		background->busy = 0;
		EFFECT_FINISH_CANVAS_ONE_BLITTER();
	}
}
