/* Charon, and the function that builds the quad strips other effects map their
   textures on. Part of battle_effect.c. */

/* The attack's particles in the effect state: the drops, each target's curse
   (where on the screen it caught the target, and the ticks since, -1 before)
   and its home, then the four columns of light; the last particle's variant
   holds the shockwave's radius. */
#define DROPS					 (state->particles)
#define CURSES					 (state->particles + 16)
#define HOMES					 (state->particles + 32)
#define COLUMNS					 (state->particles + 48)
#define WAVE_RADIUS				 (state->particles[63].variant)
/* The drops' particles, how many of them fall, and their size: shaded circles
   of this radius, in the act's pictures. */
#define CHARON_DROP_SLOTS		 16
#define CHARON_DROPS			 12
#define CHARON_DROP_RADIUS		 16
/* The columns of light: each this tall and the next this much lower, its left
   end rounded by a shaded quarter circle of size CHARON_COLUMN_END. */
#define CHARON_COLUMNS			 4
#define CHARON_COLUMN_HEIGHT	 30
#define CHARON_COLUMN_END		 56
/* The summon's sprites in the effect state: the red dome's until
   CHARON_ARRIVAL_TICK, then Charon's. */
#define OBJECTS					 ((SPR **)state->sprs)
/* Charon (CrimsonBeastTiles) as a grid of sprites four across and three down,
   its wisp (WhiteWispsTiles, in a sprite palette of its own) and the 64x64
   sprite the red orb is revealed in. */
#define CHARON_BEAST_SPRITES	 12
#define CHARON_BEAST_COLUMNS	 4
#define CHARON_BEAST_SPRITE_SIZE 32
#define CHARON_WISP				 12
#define CHARON_REVEAL			 13
#define CHARON_OBJECTS			 14
#define CHARON_WISP_PALETTE		 14
/* The dark shards (TumblingShardTiles, in a sprite palette of their own): the
   pictures of an 8x8 shard tumbling, and the sprites that swarm with them. A
   shard's tumble counts CHARON_SHARD_TURN to a turn of its pictures. */
#define CHARON_SHARD_TICKS		 16
#define CHARON_SHARDS			 48
#define CHARON_SHARD_PALETTE	 15
#define CHARON_SHARD_TURN		 256

/* The map cell buffer: the shards' sprites from its start, where the attack
   later decodes FlashBurstSheet (SPRITE_BUFFER). At 16384, a list of
   CHARON_HIDDEN_SPRITES sprites built at tick 0, which the summon never reads;
   from the tick before CHARON_ORB_TICK the red orb's 64x64 picture
   (RedOrbTiles) over it (ORB_BUFFER, read back as REVEAL_BUFFER) and the order
   it is revealed in after it. Then the particles. */
#define SPRITES					((SPR **)Ram_MapMetatileBuffer)
#define SPRITE_BUFFER			MapGridMetatileBuffer
#define OAM_BUFFER				((Uint32 *)(MapGridMetatileBuffer + 16384))
#define CHARON_HIDDEN_SPRITES	64
#define ORB_BUFFER				(MapGridMetatileBuffer + 16384)
#define REVEAL_BUFFER			(Ram_MapMetatileBuffer + 16384)
#define CHARON_PARTICLES		((PARTICLE *)(Ram_MapMetatileBuffer + 16896))
/* The swarm: CHARON_SWARM particles fly up from CHARON_ARRIVAL_TICK, the
   shards ride those from CHARON_SWARM_SHARDS on, the first CHARON_LARGE_SHARDS
   a quarter larger, and each CHARON_SWARM_GROUP particles tumble a step faster
   than the group before. */
#define CHARON_SWARM			128
#define CHARON_SWARM_SHARDS		(CHARON_PARTICLES + 64)
#define CHARON_LARGE_SHARDS		10
#define CHARON_SWARM_GROUP		16
/* The attack's particles: the motes of each target's curse, the flash bursts
   (FlashBurstSheet's six pictures, four ticks each) and the sparks among
   them. */
#define CHARON_CURSE_MOTES		16 /* for each target */
#define CHARON_MOTE_COUNT		(ENEMY_BATTLE_SIZE * CHARON_CURSE_MOTES)
#define CHARON_BURSTS			(CHARON_PARTICLES + CHARON_MOTE_COUNT)
#define CHARON_BURST_COUNT		32
#define CHARON_BURSTS_DRAWN		16
#define CHARON_BURST_TICKS		24
#define CHARON_SPARKS			(CHARON_BURSTS + CHARON_BURST_COUNT)
#define CHARON_SPARK_COUNT		128
/* The colours of the 256-colour sprites: the red dome's (RedDomeTiles) from
   CHARON_DOME_FIRST_COLOR, Charon's from the first, and the red orb's
   (RedOrbTiles) in the two palettes from CHARON_ORB_PALETTE. */
#define CHARON_DOME_FIRST_COLOR 32
#define CHARON_DOME_COLORS		160
#define CHARON_BEAST_COLORS		192
#define CHARON_ORB_PALETTE		12
#define CHARON_ORB_FIRST_COLOR	(CHARON_ORB_PALETTE * PALETTE_COLORS)
#define CHARON_ORB_COLORS		32
/* The red orb is revealed in its sprite two pixels at a time, the same pair in
   each of its tiles, in a shuffled order of the tile's pairs. */
#define CHARON_REVEAL_TILES		(OBJ_64X64_8BPP_BYTES / TILE_SIZE_8BPP)
#define CHARON_DISSOLVE_PAIRS	(TILE_SIZE_8BPP / 2)
/* The battle background's tiles from column 4 of row 5, where the rune seal is
   stamped. */
#define CHARON_SEAL_TILES		(BG_CHAR_BLOCK(2) + (5 * 32 + 4) * 64)
/* The rune seal's colours take a background palette of their own: a seal pixel
   of colour c is stamped as the byte c + CHARON_SEAL_COLOR, which wraps to
   colour c of that palette. */
#define CHARON_SEAL_PALETTE		14
#define CHARON_SEAL_COLOR \
	(CHARON_SEAL_PALETTE * PALETTE_COLORS - BG_PLTT_COLORS)

extern const SCALE CharonSwarmScale;
extern const SCALE CharonPulseScale;
extern const SCALE CharonRevealScale;
extern const SCALE CharonFinaleScale;

/* Where the pictures that load once are decoded in the effect's sprite sheet:
   the rune seal as it is stamped (RuneSealSheet, 184 by 80), then at
   CHARON_ARRIVAL_TICK Charon's, its wisp's and BlueGlowSheet's in turn. */
#define CHARON_BEAST_PICTURES 9216
#define CHARON_RUNE_SEAL	  9216
#define RUNE_SEAL_WIDTH		  184
#define RUNE_SEAL_HEIGHT	  80
/* Where each act decodes its own sprite sheet in the effect's pictures:
   SwirlTexture with CharmRingSheet, CurseSealSheet, FireRingTexture or shaded
   circles. */
#define CHARON_ACT_PICTURES	  4096
/* The finale's brightest block, which the four columns of light are drawn from,
   and GlowRingTexture made all bright. */
#define CHARON_BRIGHT_BLOCK	  8192
#define CHARON_GLOW_RING	  12288
/* The bytes of either, a 64x64 texture. */
#define CHARON_TEXTURE_BYTES  (64 * 64)

/* The ring of motes the seal sends out: CHARON_RING_MOTES motes of mote picture
   CHARON_RING_MOTE (4 pixels wide, 8 tall), drawn from CHARON_RING_SHADES
   copies in the battle sprite sheet, each 7 levels darker than the last. */
#define CHARON_RING_MOTES	   256
#define CHARON_RING_MOTE	   4
#define CHARON_RING_SHADES	   10
#define CHARON_RING_MOTE_BYTES (CHARON_RING_MOTE * CHARON_RING_MOTE * 2)

/* The opening's swirl: it shrinks from CHARON_SWIRL_SCALE by CHARON_SCALE_STEP
   a tick until the seal is stamped, then grows again from half its size. A
   second swirl, mirrored, turns the other way inside it, and from the seal on
   the charm ring turns with a smaller mirrored one inside it. The swirls are
   drawn only while brighter than CHARON_SWIRL_SHADE_MIN. */
#define CHARON_SWIRL_SCALE		 115536
#define CHARON_SCALE_STEP		 1524
#define CHARON_INNER_SWIRL_SCALE 42768
#define CHARON_CHARM_RING_SCALE	 22768
#define CHARON_INNER_RING_SCALE	 12768
#define CHARON_SWIRL_SHADE_MIN	 (-48)
/* The curse seal falls at tick 200 and the swirl under it at 232, both from
   CHARON_FALL_SCALE: the seal shrinks CHARON_SCALE_STEP a tick down to
   CHARON_SEAL_SCALE_MIN, the swirl CHARON_SWIRL_SHRINK. */
#define CHARON_FALL_SCALE		 100000
#define CHARON_SEAL_SCALE_MIN	 13800
#define CHARON_SWIRL_SHRINK		 1200
/* The glow ring that rises at each cursed target grows this much a step. */
#define CHARON_GLOW_RING_GROWTH	 1016

#define CHARON_TICKS \
	394 /* the ticks of Charon's coming, unless A or B skips it */
#define CHARON_ATTACK_TICKS		 180 /* ticks of the attack that follows */
#define CHARON_FLASHES			 3 /* the backdrop's flashes, at CharonFlashTicks */
/* Charon's coming, in ticks. From CHARON_DRAIN_TICK the backdrop's red drains
   away and its blue comes back; the rune seal is stamped on the ground after
   the canvas flashes (CHARON_SEAL_TICK), and the swirls fade from
   CHARON_SWIRL_FADE_TICK. The backdrop's green and then its red drain away
   from CHARON_GREEN_DRAIN_TICK, and in a white flash Charon appears among a
   swarm of shards (CHARON_ARRIVAL_TICK); the backdrop fades toward a red dusk
   from CHARON_DUSK_TICK. The curse seal falls turning (CHARON_CURSE_TICK),
   the swirl turns under it from CHARON_CURSE_SWIRL_TICK, and it fades from
   CHARON_CURSE_FADE_TICK; the red orb is revealed from CHARON_ORB_TICK and is
   gone at CHARON_ORB_GONE_TICK. A white flash brings three rings of fire
   (CHARON_RINGS_TICK); the orb comes back magenta and swells
   (CHARON_MAGENTA_TICK), and the dark disc rimmed in magenta swells over the
   canvas from CHARON_DISC_TICK. Charon's own colours are tinted toward violet
   from CHARON_VIOLET_TICK, back from CHARON_UNVIOLET_TICK, and toward red
   from CHARON_REDDEN_TICK. */
#define CHARON_DRAIN_TICK		 32
#define CHARON_SEAL_TICK		 56
#define CHARON_SWIRL_FADE_TICK	 96
#define CHARON_GREEN_DRAIN_TICK	 108
#define CHARON_ARRIVAL_TICK		 132
#define CHARON_DUSK_TICK		 168
#define CHARON_CURSE_TICK		 200
#define CHARON_VIOLET_TICK		 214
#define CHARON_CURSE_SWIRL_TICK	 232
#define CHARON_ORB_TICK			 244
#define CHARON_UNVIOLET_TICK	 251
#define CHARON_CURSE_FADE_TICK	 254
#define CHARON_RINGS_TICK		 302
#define CHARON_ORB_GONE_TICK	 310
#define CHARON_MAGENTA_TICK		 330
#define CHARON_DISC_TICK		 347
#define CHARON_REDDEN_TICK		 348
/* As the red drains, the backdrop's green is held this many levels darker, and
   its blue comes back from as dark; at the seal, half as dark in green and as
   dark in blue. */
#define CHARON_DRAIN_DIMMING	 20
/* The rune seal glows this many levels brighter than its own colours, pulsing
   by CHARON_SEAL_GLOW_PULSE either way. */
#define CHARON_SEAL_GLOW		 16
#define CHARON_SEAL_GLOW_PULSE	 12
/* The red dusk the backdrop fades toward: its own colours this many levels
   darker in green and blue and CHARON_DUSK_REDDENING redder, and after the
   flash that brings the rings of fire, CHARON_DEEP_DUSK_DIMMING darker. */
#define CHARON_DUSK_DIMMING		 16
#define CHARON_DEEP_DUSK_DIMMING 20
#define CHARON_DUSK_REDDENING	 8
/* The deep red the wisp darkens to from white. */
#define CHARON_WISP_RED_MIN		 12
#define CHARON_WISP_GREEN_MIN	 4
#define CHARON_WISP_BLUE_MIN	 4
/* The dark disc rimmed in magenta swells at this x on the canvas, and from
   CHARON_DISC_SWAY_TICK sways as far either side of it. */
#define CHARON_DISC_X			 54
#define CHARON_DISC_SWAY_TICK	 350
/* The top of the attack's ramp, a pink violet: full red with these levels of
   green and blue. */
#define CHARON_RAMP_GREEN		 8
#define CHARON_RAMP_BLUE		 27
/* Each flash as the targets are drawn in fades the background back to this many
   levels darker in green, and half that in red and blue. */
#define CHARON_FLASH_DIMMING	 20

/* Charon: over a red wasteland a violet whirlpool turns about a red dome that
   rises out of it; in a white flash among dark shards Charon appears and
   slashes across the vortex, and a black shadow sweeps from the left over the
   foes.

   Abilities: Charon. */
void btEffectRunCharon(BATTLE_EFFECT_PARM *arg)
{
	Uint16					 tiles[CHARON_SHARD_TICKS];
	Sint16					 emptyList[UNIT_MAX];
	FIXED					 swarmPlace[4];
	FIXED					 pulsePlace[4];
	FIXED					 objectPlace[4];
	FIXED					 revealPlace[4];
	FIXED					 finalePlace[4];
	DRAW_RECTANGLE			 blitters[2];
	MODEL_TEXTURE			 sealTexture;
	MODEL_TEXTURE			 fireRingTexture;
	MODEL_TEXTURE			 swirlTexture;
	MODEL_TEXTURE			 glowRingTexture;
	Uint8					*canvas;
	BATTLE_EFFECT_STATE		*state;
	BATTLE_BACKGROUND_STATE *backgroundView;
	Sint32					 j;
	Sint32					 frm;
	Uint32					 savedScrollX;
	Uint8					*motePictures;
	BATTLE_STATE			*battle;
	DRAW_RECTANGLE			*blit;
	FIXED					 beastX;
	FIXED					 beastZ;
	Sint32					 sealFrm;
	Sint32					 red;
	Sint32					 green;
	Sint32					 blue;
	BATTLE_CAMERA			*camera;
	Sint32					 shake;
	Sint16					 savedYaw;
	Sint32					 i;
	Sint32					 tinted;
	Uint16					*palette;
	Uint8					*src;
	Sint32					 darkenFrm;
	SPR					   **sprs;
	VRAM_CACHE_ENTRY		*slots;
	VRAM_CACHE_ENTRY		*objectSlots;
	Sint32					 moteSize;
	Sint32					 phase;
	Sint32					 halfWidth;
	Sint32					 dimming;
	Sint32					 width;

	canvas = Ram_HeapSlots->battleCanvas;
	state = Ram_HeapSlots->battleEff;
	backgroundView = Ram_HeapSlots->battleBg;
	savedScrollX = BgScrollWork[1].x;
	motePictures = Ram_HeapSlots->battleSheet;
	sprs = (SPR **)MapGridMetatileBuffer;
	battle = Ram_HeapSlots->battleState;
	btEffectStartCanvasLayer(0);
	btEffectConfigureEffectDisp();
	REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
	PLTT_BACKDROP = 0;
	BG_PLTT_COLOR(0, 1) = 0;
	state->transferMode = CANVAS_COPY;
	{
		Sint32 order = EVENT_PASS_VBLANK;

		sySetEvent(btEffectTransferCanvas, order);
	}
	btEffectWipeCanvas(CANVAS_WIPE_ACROSS, CANVAS_WIPE_FILL);
	ProjectionWork.centerY = SUMMON_PROJECTION_CENTER_Y;
	backgroundView->busy = 1;
	BgScrollWork[1].x = 0;
	emptyList[0] = BATTLE_UNIT_LIST_END;
	btActorSpawnObjectsForListStub(emptyList, 0);
	REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG1 |
		WININ_WIN1_BG2 | WININ_WIN1_CLR;
	REG_WIN0H = WIN_RANGE(0, DISPLAY_WIDTH);
	sySynch(1);
	syPaletteLoadBasicSprCols();
	battle->keepBackdropPal = 1;
	btBackgroundLoadStub(
		BATTLE_BACKGROUND_FIXED, RESOURCE_AERIAL_VIEW_BACKDROP, 0);
	btPaletteBrightenSavedBattleBg(0, -20, -20);
	btEffectWipeCanvas(CANVAS_WIPE_ACROSS, CANVAS_WIPE_CLEAR);
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	REG_BG2PA = Q_8_8(0.5);
	REG_BG2X = 0;
	blit = blitters;
	btEffectBuildCanvasBlitters(0, blit);

	src = (Uint8 *)syResourceGetTableEntry(RESOURCE_RED_DOME_TILES);
	syWordsCopy((Uint16 *)&OBJ_PLTT_COLOR(2, 0), src, CHARON_DOME_COLORS * 2);
	src += CHARON_DOME_COLORS * 2;
	syDecompressTaggedLZ(src, state->sheet);
	for(i = 0; i != OBJ_64X64_8BPP_BYTES; i++) {
		if(state->sheet[i] != 0) {
			state->sheet[i] += CHARON_DOME_FIRST_COLOR;
		}
	}
	{
		SPR *object = btEffectSprCreateWithTiles(
			64, 64, OBJ_SIZE_64X64 | OBJ_256_COLORS, 0);

		OBJECTS[0] = object;
		object->image.pri = 3;
		slots = VramCacheEntries;
		syWordsCopy(OBJ_VRAM + slots[object->slot].ofs, state->sheet,
			OBJ_64X64_8BPP_BYTES);
	}
	OBJECTS[0]->image.objMode = 0;
	OBJECTS[0]->ofsY = 32;

	src = (Uint8 *)syResourceGetTableEntry(RESOURCE_TUMBLING_SHARD_TILES);
	syWordsCopy((Uint16 *)&OBJ_PLTT_COLOR(CHARON_SHARD_PALETTE, 0), src,
		PALETTE_BANK_BYTES);
	src += PALETTE_BANK_BYTES;
	syDecompressTaggedLZ(src, state->sheet);
	for(i = 0; i != CHARON_SHARD_TICKS; i++) {
		SPR *object = btEffectSprCreateWithTiles(
			16, 16, 0, OBJ_PALETTE(CHARON_SHARD_PALETTE));
		SPRITES[i] = object;
		object->image.pri = 1;
		syWordsCopy(OBJ_VRAM + slots[object->slot].ofs,
			state->sheet + i * TILE_SIZE_4BPP, TILE_SIZE_4BPP);
		tiles[i] = SPRITES[i]->image.tile;
	}
	for(i = 0; i != CHARON_SHARDS - CHARON_SHARD_TICKS; i++) {
		SPR *object =
			btEffectSprAcquire(8, 8, 0, OBJ_PALETTE(CHARON_SHARD_PALETTE));
		SPRITES[CHARON_SHARD_TICKS + i] = object;
		syWordsCopy(object, sprs[0], offsetof(SPR, lastNo));
	}
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, motePictures, 0, 0);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, state->sheet, 0, 0);
	for(j = 0; j != CHARON_RING_SHADES; j++) {
		moteSize = CHARON_RING_MOTE;
		for(i = 0; i != moteSize * moteSize * 2; i++) {
			Sint32 value =
				state->sheet[BattleEffectMotePictures[moteSize - 1] + i];

			if(value != 0) {
				value -= j * 7;
				if(value <= 0) {
					value = 1;
				}
			}
			motePictures[moteSize * moteSize * j * 2 + i] = value;
		}
	}
	btResourceLoadAndDecompress(RESOURCE_SWIRL_TEXTURE, state->sheet, 0, 0);
	btResourceLoadAndDecompress(
		RESOURCE_CHARM_RING_SHEET, state->sheet + CHARON_ACT_PICTURES, 1, 1);
	src = (Uint8 *)syResourceGetTableEntry(RESOURCE_BOLT_SHEET);
	syWordsCopy(BG_PLTT, src, SHEET_PALETTE_BYTES);
	REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
	REG_BG2X = Q_8_8(8);
	beastX = 0;
	beastZ = 0;

	for(frm = 0; frm != CHARON_TICKS && !(PerWork.repeat & (KEY_A | KEY_B));
		frm++) {
		Uint8 *order = REVEAL_BUFFER + (frm - CHARON_ORB_TICK) * 2;

		/* The backdrop's red drains away and its blue comes back. */
		darkenFrm = frm - CHARON_DRAIN_TICK;
		if(darkenFrm >= 0 && darkenFrm < 22) {
			btPaletteBrightenSavedBattleBg(-darkenFrm, -CHARON_DRAIN_DIMMING,
				frm - (CHARON_DRAIN_TICK + CHARON_DRAIN_DIMMING));
		}
		if(frm == CHARON_SEAL_TICK - 1) {
			btPaletteBrightenSavedBattleBg(
				0, -CHARON_DRAIN_DIMMING / 2, -CHARON_DRAIN_DIMMING);
		}
		if(frm == 4) {
			sySndPlayStub(SE_TREMOR);
		}
		if(frm == CHARON_SEAL_TICK - 1) {
			sySndPlayStub(SE_BANE_ESCAPE);
		}
		if(frm == 78) {
			sySndPlayStub(SE_GUSH);
		}
		if(frm == CHARON_ARRIVAL_TICK - 5) {
			sySndPlayStub(SE_POWER_UP);
		}
		if(frm == CHARON_ARRIVAL_TICK - 1) {
			sySndPlayStub(SE_MONSTER_ROAR);
		}
		if(frm == CHARON_CURSE_TICK + 6) {
			sySndPlayStub(SE_SWIRL);
		}
		if(frm == CHARON_ORB_TICK) {
			sySndPlayStub(SE_POWER_UP);
		}
		if(frm == CHARON_RINGS_TICK) {
			sySndPlayStub(SE_POWER_UP);
		}
		if(frm == CHARON_RINGS_TICK + 6) {
			sySndPlayStub(SE_POWER_UP);
		}
		if(frm == CHARON_RINGS_TICK + 10) {
			sySndPlayStub(SE_POWER_UP);
		}
		if(frm == CHARON_DISC_SWAY_TICK) {
			sySndPlayStub(SE_LAUNCH);
		}
		if(frm == 0) {
			EFFECT_SET_TRANSFER(
				state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
			REG_BLDALPHA = EFFECT_BLEND_ADD;
			/* A list of sprites off the screen's corner, each showing the shard
			   picture of a particle's tumble; the particles hold nothing
			   yet. */
			{
				Uint32 hidden;

				/* FAKEMATCH(temporary): the mask held in a halfword struct set
				 * in the for clause. */
				struct {
					Uint16 value;
				} mask;

				/* FAKEMATCH(temporary): tumble, a pointer to each particle's
				 * variant stepped a particle at a time; as a particle pointer
				 * walked. */
				Sint32 *tumble;
				Uint32 *b;
				Uint32 *a;

				for(i = 0, hidden = OBJ_X(DISPLAY_WIDTH) + DISPLAY_HEIGHT,
				mask.value = OBJ_PALETTE(CHARON_SHARD_PALETTE),
				tumble = &CHARON_PARTICLES[0].variant,
				b = (Uint32 *)(REVEAL_BUFFER + 4), a = OAM_BUFFER;
					i != CHARON_HIDDEN_SPRITES; i++) {
					Sint32 t;

					*a = hidden;
					t = *tumble;
					tumble += sizeof(PARTICLE) / sizeof(*tumble);
					*b = tiles[t / (CHARON_SHARD_TURN / CHARON_SHARD_TICKS)] |
						mask.value;
					b += 2;
					a += 2;
				}
			}
		}
		if(frm >= CHARON_ARRIVAL_TICK && frm < 240) {
			SCALE swarmScale = CharonSwarmScale;

			swarmScale.y = toFIXED(5) / 4;
			swarmScale.x = toFIXED(5) / 4;
			if(frm == CHARON_ARRIVAL_TICK) {
				for(i = 0; i != CHARON_SWARM; i++) {
					CHARON_PARTICLES[i].x = INTtoFIXED(syRandom() % 160 + 40);
					CHARON_PARTICLES[i].y = INTtoFIXED((syRandom() & 63) + 32);
					CHARON_PARTICLES[i].variant =
						syRandom() & (CHARON_SHARD_TURN - 1);
					CHARON_PARTICLES[i].velX = ((syRandom() & 255) - 127) << 9;
					CHARON_PARTICLES[i].velY = (-(syRandom() & 255) - 128) << 8;
				}
			}
			for(i = 0; i != CHARON_SHARDS; i++) {
				if(i == CHARON_LARGE_SHARDS) {
					swarmScale.y = toFIXED(1);
					swarmScale.x = toFIXED(1);
				}
				swarmPlace[3] = 0;
				swarmPlace[1] = toFIXED(255);
				swarmPlace[0] = CHARON_SWARM_SHARDS[i].x;
				swarmPlace[2] = CHARON_SWARM_SHARDS[i].y + toFIXED(32);
				fdRenderApplyProjectedPlacementStub(
					SPRITES[i], swarmPlace, &swarmScale.x, 0);
				SPRITES[i]->image.tile = tiles[CHARON_SWARM_SHARDS[i].variant /
					(CHARON_SHARD_TURN / CHARON_SHARD_TICKS)];
			}
			for(i = 0; i != CHARON_SWARM; i++) {
				if(CHARON_PARTICLES[i].y > -toFIXED(16) &&
					CHARON_PARTICLES[i].x > -toFIXED(16) &&
					CHARON_PARTICLES[i].x < toFIXED(256)) {
					CHARON_PARTICLES[i].x += CHARON_PARTICLES[i].velX;
					CHARON_PARTICLES[i].y += CHARON_PARTICLES[i].velY;
				}
				CHARON_PARTICLES[i].variant += i / CHARON_SWARM_GROUP + 2;
				if(CHARON_PARTICLES[i].variant >= CHARON_SHARD_TURN) {
					CHARON_PARTICLES[i].variant -= CHARON_SHARD_TURN;
				}
			}
		}
		sealFrm = frm - CHARON_SEAL_TICK;
		if(sealFrm >= 0 && sealFrm < 76) {
			SCALE pulseScale = CharonPulseScale;

			pulseScale.x = pulseScale.y =
				sySin(sealFrm << 7) * 2 + toFIXED(1) / 4;
			pulsePlace[3] = 0;
			pulsePlace[1] = toFIXED(255);
			pulsePlace[0] = toFIXED(114);
			/* DEAD: sealFrm is never negative here; the ROM keeps the test
			   and its other height. */
			pulsePlace[2] = sealFrm >= 0 ? toFIXED(78) : toFIXED(100);
			fdRenderApplyProjectedPlacementStub(
				OBJECTS[0], pulsePlace, &pulseScale.x, 0);
		}
		if(frm == CHARON_ARRIVAL_TICK - 1) {
			btBackgroundLoadStub(
				BATTLE_BACKGROUND_FIXED, RESOURCE_AERIAL_VIEW_BACKDROP, 0);
			syWordsFill((Uint16 *)&BG_PLTT_COLOR(6, 0),
				BATTLE_BACKGROUND_COLORS * 2, COLOR_PAIR(RGB_WHITE));
			syWordsFill(canvas, CANVAS_VIEW_BYTES,
				CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
		}
		if(frm >= CHARON_ARRIVAL_TICK && frm < CHARON_ARRIVAL_TICK + 15) {
			syWordsFill(canvas, CANVAS_VIEW_BYTES,
				CANVAS_FILL_WORD(
					CANVAS_LEVEL_BRIGHTEST - (frm - CHARON_ARRIVAL_TICK) * 4));
		}
		/* Every fourth tick the backdrop fades a step from the flash's white
		   to its own colours, and from CHARON_DUSK_TICK on toward a red dusk;
		   the wisp darkens from white to a deep red. */
		if((frm & 3) == 0 && (frm >= CHARON_ARRIVAL_TICK && frm < 240)) {
			Uint16 *tgt =
				syResourceGetTableEntry(RESOURCE_AERIAL_VIEW_BACKDROP);
			Uint16 *col = (Uint16 *)&BG_PLTT_COLOR(6, 0);

			for(i = 0; i != BATTLE_BACKGROUND_COLORS; i++, tgt++, col++) {
				Sint32 b = RGB_BLUE(*col);
				Sint32 g = RGB_GREEN(*col);
				Sint32 r = RGB_RED(*col);
				Sint32 tb;
				Sint32 tg;
				Sint32 tr;

				if(frm < CHARON_DUSK_TICK) {
					tb = RGB_BLUE(*tgt);
					tg = RGB_GREEN(*tgt);
					tr = RGB_RED(*tgt);
				} else {
					tb = RGB_BLUE(*tgt) - CHARON_DUSK_DIMMING;
					tg = RGB_GREEN(*tgt) - CHARON_DUSK_DIMMING;
					tr = RGB_RED(*tgt) + CHARON_DUSK_REDDENING;
				}
				if(tb < 0) {
					tb = 0;
				}
				if(tg < 0) {
					tg = 0;
				}
				if(tr > RGB_LEVEL_MAX) {
					tr = RGB_LEVEL_MAX;
				}
				if(b > tb) {
					b--;
				}
				if(g > tg) {
					g--;
				}
				if(r > tr) {
					r--;
				}
				*col = RGB(r, g, b);
			}
			for(i = 0; i != PALETTE_COLORS - 1; i++) {
				Sint32 r = RGB_LEVEL_MAX - (frm - CHARON_ARRIVAL_TICK) / 4;
				Sint32 g = r;
				Sint32 b = r;

				if(r > RGB_LEVEL_MAX) {
					r = RGB_LEVEL_MAX;
				}
				if(r < CHARON_WISP_RED_MIN) {
					r = CHARON_WISP_RED_MIN;
				}
				if(g > RGB_LEVEL_MAX) {
					g = RGB_LEVEL_MAX;
				}
				if(g < CHARON_WISP_GREEN_MIN) {
					g = CHARON_WISP_GREEN_MIN;
				}
				if(b > RGB_LEVEL_MAX) {
					b = RGB_LEVEL_MAX;
				}
				if(b < CHARON_WISP_BLUE_MIN) {
					b = CHARON_WISP_BLUE_MIN;
				}
				OBJ_PLTT_COLOR(CHARON_WISP_PALETTE, 1 + i) = RGB(r, g, b);
			}
		}
		if(frm >= CHARON_ARRIVAL_TICK && frm < 240) {
			Uint16 *tgt = syResourceGetTableEntry(RESOURCE_CRIMSON_BEAST_TILES);
			Uint16 *col = (Uint16 *)&OBJ_PLTT_COLOR(0, 0);

			for(i = 0; i != CHARON_BEAST_COLORS; i++, tgt++, col++) {
				Sint32 b = RGB_BLUE(*col);
				Sint32 g = RGB_GREEN(*col);
				Sint32 r = RGB_RED(*col);
				Sint32 tb = RGB_BLUE(*tgt);
				Sint32 tg = RGB_GREEN(*tgt);
				Sint32 tr = RGB_RED(*tgt);

				if(b > tb) {
					b--;
				}
				if(g > tg) {
					g--;
				}
				if(r > tr) {
					r--;
				}
				*col = RGB(r, g, b);
			}
		}
		/* Charon's own colours are tinted toward violet, back, and toward
		   red. */
		tinted = 0;
		if(frm >= CHARON_VIOLET_TICK && frm < 246) {
			Sint32 t = (frm - CHARON_VIOLET_TICK) / 4;

			red = t / 2;
			green = 0;
			blue = t;
			tinted = 1;
		}
		if(frm >= CHARON_UNVIOLET_TICK && frm < 283) {
			Sint32 t = (frm - CHARON_UNVIOLET_TICK) / 2;

			red = 4 - t / 4;
			green = -(t / 2);
			blue = 8 - t / 2;
			tinted = 1;
		}
		if(frm >= CHARON_REDDEN_TICK && frm < 412) {
			red = (frm - CHARON_REDDEN_TICK) / 4;
			green = red / 2 - 8;
			blue = red / 8 - 4;
			tinted = 1;
		}
		if(tinted == 1) {
			Uint16 *col = syResourceGetTableEntry(RESOURCE_CRIMSON_BEAST_TILES);

			for(i = 0; i != CHARON_BEAST_COLORS; i++, col++) {
				Sint32 r = RGB_RED(*col) + red;
				Sint32 g = RGB_GREEN(*col) + green;
				Sint32 b = RGB_BLUE(*col) + blue;

				if(r > RGB_LEVEL_MAX) {
					r = RGB_LEVEL_MAX;
				}
				if(g > RGB_LEVEL_MAX) {
					g = RGB_LEVEL_MAX;
				}
				if(b > RGB_LEVEL_MAX) {
					b = RGB_LEVEL_MAX;
				}
				if(r < 0) {
					r = 0;
				}
				if(g < 0) {
					g = 0;
				}
				if(b < 0) {
					b = 0;
				}
				OBJ_PLTT_COLOR(0, i) = RGB(r, g, b);
			}
		}
		if(frm >= CHARON_ARRIVAL_TICK && frm < 860) {
			if(frm == CHARON_ARRIVAL_TICK) {
				fdSprDestroyStub((SPR *)OBJECTS[0]);
				src = (Uint8 *)syResourceGetTableEntry(
					RESOURCE_CRIMSON_BEAST_TILES);
				syWordsFill((Uint16 *)&OBJ_PLTT_COLOR(0, 0),
					CHARON_BEAST_COLORS * 2, COLOR_PAIR(RGB_WHITE));
				src += CHARON_BEAST_COLORS * 2;
				syDecompressTaggedLZ(src, state->sheet + CHARON_BEAST_PICTURES);
				for(i = 0, objectSlots = VramCacheEntries;
					i != CHARON_BEAST_SPRITES; i++) {
					SPR *object = btEffectSprCreateWithTiles(
						CHARON_BEAST_SPRITE_SIZE, CHARON_BEAST_SPRITE_SIZE,
						OBJ_SIZE_32X32 | OBJ_256_COLORS, 0);

					object->image.pri = 3;
					OBJECTS[i] = object;
					syWordsCopy(OBJ_VRAM + objectSlots[object->slot].ofs,
						state->sheet + CHARON_BEAST_PICTURES +
							i * OBJ_32X32_8BPP_BYTES,
						OBJ_32X32_8BPP_BYTES);
				}
				src = (Uint8 *)syResourceGetTableEntry(
					RESOURCE_WHITE_WISPS_TILES);
				syWordsCopy((Uint16 *)&OBJ_PLTT_COLOR(CHARON_WISP_PALETTE, 0),
					src, PALETTE_BANK_BYTES);
				syWordsFill((Uint16 *)&OBJ_PLTT_COLOR(CHARON_WISP_PALETTE, 0),
					PALETTE_BANK_BYTES, COLOR_PAIR(RGB_WHITE));
				src += PALETTE_BANK_BYTES;
				syDecompressTaggedLZ(src, state->sheet + CHARON_BEAST_PICTURES);
				{
					SPR *object = btEffectSprCreateWithTiles(64, 64,
						OBJ_SIZE_64X64, OBJ_PALETTE(CHARON_WISP_PALETTE));
					VRAM_CACHE_ENTRY *tileSlots;

					OBJECTS[CHARON_WISP] = object;
					object->image.pri = 3;
					tileSlots = VramCacheEntries;
					{
						Uint8 *vram = OBJ_VRAM;

						vram += tileSlots[object->slot].ofs;
						syWordsCopy(vram, state->sheet + CHARON_BEAST_PICTURES,
							OBJ_64X64_4BPP_BYTES);
					}
					object = btEffectSprCreateWithTiles(
						64, 64, OBJ_SIZE_64X64 | OBJ_256_COLORS, 0);
					OBJECTS[CHARON_REVEAL] = object;
					object->image.pri = 2;
					object->ofsY = 32;
					syWordsFill(OBJ_VRAM + tileSlots[object->slot].ofs,
						OBJ_64X64_8BPP_BYTES, 0);
				}
				src =
					(Uint8 *)syResourceGetTableEntry(RESOURCE_VIOLET_PALETTE_E);
				syWordsCopy(BG_PLTT, src, SHEET_PALETTE_BYTES);
				btResourceLoadAndDecompress(RESOURCE_BLUE_GLOW_SHEET,
					state->sheet + CHARON_BEAST_PICTURES, 0, 0);
				syWordsCopy(motePictures, state->sheet + CHARON_BEAST_PICTURES,
					MOTE_SET_BYTES);
				btResourceLoadAndDecompress(RESOURCE_CURSE_SEAL_SHEET,
					state->sheet + CHARON_ACT_PICTURES, 1, 1);
				beastX = toFIXED(72);
				beastZ = toFIXED(56);
			}
			if(frm == CHARON_ORB_TICK - 1) {
				src = (Uint8 *)syResourceGetTableEntry(RESOURCE_RED_ORB_TILES);
				syWordsFill((Uint16 *)&OBJ_PLTT_COLOR(CHARON_ORB_PALETTE, 0),
					CHARON_ORB_COLORS * 2, 0);
				src += CHARON_ORB_COLORS * 2;
				syDecompressTaggedLZ(src, ORB_BUFFER);
				for(i = 0; i != OBJ_64X64_8BPP_BYTES; i++) {
					if(ORB_BUFFER[i] != 0) {
						ORB_BUFFER[i] += CHARON_ORB_FIRST_COLOR;
					}
				}
				/* The order the pixel pairs are revealed in, after the picture:
				   each pair is swapped with one picked at random. */
				for(i = 0; i != CHARON_DISSOLVE_PAIRS; i++) {
					REVEAL_BUFFER[OBJ_64X64_8BPP_BYTES + i] = i;
				}
				{
					Uint8 *slot;
					Sint32 orderOffset = OBJ_64X64_8BPP_BYTES;

					for(i = 0, slot = REVEAL_BUFFER + OBJ_64X64_8BPP_BYTES;
						i != CHARON_DISSOLVE_PAIRS;) {
						Uint8 *other;
						Sint32 swap;
						Sint32 value;
						Sint32 mask = CHARON_DISSOLVE_PAIRS - 1;

						/* FAKEMATCH(temporary): orderOffset, the constant set
						 * again in the loop; as other +=
						 * OBJ_64X64_8BPP_BYTES. */
						orderOffset = OBJ_64X64_8BPP_BYTES;
						other = (syRandom() & mask) + ORB_BUFFER;
						other += orderOffset;
						swap = *slot;
						value = *other;
						/* FAKEMATCH(dead): swap and value cast to the bytes
						 * they already are. */
						swap = (Uint8)swap;
						value = (Uint8)value;
						i++;
						*slot = value;
						*other = swap;
						slot++;
					}
				}
			}
			{
				FIXED objectScale[2];

				objectPlace[3] = 0;
				objectPlace[1] = toFIXED(255);
				if(frm < 160) {
					beastX += toFIXED(1) / 8;
					beastZ -= toFIXED(1) / 4;
				} else if(frm < 332) {
					beastX += toFIXED(1) / 8;
					beastZ += toFIXED(1) / 16;
				} else {
					beastX += toFIXED(1) / 8;
				}
				objectScale[1] = toFIXED(5) / 4;
				objectScale[0] = toFIXED(5) / 4;
				objectScale[0] = objectScale[1] = beastZ / 48 + toFIXED(1) / 4;
				objectPlace[0] = beastX + toFIXED(64);
				objectPlace[2] = toFIXED(144);
				fdRenderApplyProjectedPlacementStub(
					OBJECTS[CHARON_WISP], objectPlace, objectScale, 0);
				objectScale[1] = toFIXED(1);
				objectScale[0] = toFIXED(1);
				for(i = 0; i != CHARON_BEAST_SPRITES; i++) {
					objectPlace[0] = toFIXED((i % CHARON_BEAST_COLUMNS) *
										 CHARON_BEAST_SPRITE_SIZE) +
						beastX;
					objectPlace[2] = toFIXED((i / CHARON_BEAST_COLUMNS) *
										 CHARON_BEAST_SPRITE_SIZE) +
						beastZ;
					fdRenderApplyProjectedPlacementStub(
						OBJECTS[i], objectPlace, objectScale, 0);
				}
			}
			if(frm == CHARON_CURSE_TICK) {
				src = (Uint8 *)syResourceGetTableEntry(RESOURCE_BOLT_SHEET);
				syWordsCopy(BG_PLTT, src, SHEET_PALETTE_BYTES);
			}
			/* The curse seal falls turning onto the ground and fades; the swirl
			   turns under it. */
			if(frm >= CHARON_CURSE_TICK && frm < CHARON_RINGS_TICK) {
				void		 *texture = syHeapAllocEWRAM(32);
				RENDER_MODEL *model = btRenderAllocateModelList(1);
				Sint32		  t = frm - 200;
				Sint32		  shade;
				Sint32		  scale;

				shade = t * 4 + MODEL_SHADE_INVISIBLE;
				scale = CHARON_FALL_SCALE - t * CHARON_SCALE_STEP;
				if(frm > CHARON_CURSE_FADE_TICK - 1) {
					shade = (CHARON_CURSE_FADE_TICK - frm) * 8;
				}
				if(scale < CHARON_SEAL_SCALE_MIN) {
					scale = CHARON_SEAL_SCALE_MIN;
				}
				if(shade > 0) {
					shade = 0;
				}
				model->shade = shade;
				EFFECT_SET_TRANSFER(
					state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
				sealTexture.logWidth = 7;
				sealTexture.logHeight = 7;
				sealTexture.pixels = state->sheet + CHARON_ACT_PICTURES;
				model->kind = MODEL_SHADED_ADDITIVE;
				model->texture = &sealTexture;
				model->mesh = BattleEffectLargeSquareMesh;
				model->vertices = texture;
				if(frm <= 282) {
					syInitMatrix();
					syScale(scale * 2, scale * 2, scale * 2);
					syTranslate(-toFIXED(16), -toFIXED(8), 0);
					syRotX(TRIG_TURN * 7 / 8);
					syRotZ(DEGtoANG(90) - t * (TRIG_TURN / 256));
					syRotY(-t * (TRIG_TURN / 64));
					btRenderTransformVertices(
						BattleEffectGroundSquareCorners, texture, 4);
					btRenderDrawModelList(model);
				}
				if(frm > CHARON_CURSE_SWIRL_TICK - 1) {
					t = frm - CHARON_CURSE_SWIRL_TICK;
					shade = t * 2 + MODEL_SHADE_INVISIBLE;
					scale = CHARON_FALL_SCALE - t * CHARON_SWIRL_SHRINK;
					if(shade < MODEL_SHADE_INVISIBLE) {
						shade = MODEL_SHADE_INVISIBLE;
					}
					if(shade > 0) {
						shade = 0;
					}
					sealTexture.logWidth = 6;
					sealTexture.logHeight = 6;
					model->shade = shade;
					sealTexture.pixels = state->sheet;
					model->texture = &sealTexture;
					model->mesh = BattleEffectSquareMesh;
					model->kind = MODEL_SHADED_ADDITIVE;
					model->vertices = texture;
					syInitMatrix();
					syTranslate(-toFIXED(8), -toFIXED(8), 0);
					syScale(scale, scale, scale);
					syRotX(TRIG_TURN * 3 / 4);
					syRotY(-t * (TRIG_TURN / 64));
					btRenderTransformVertices(
						BattleEffectGroundSquareCorners, texture, 4);
					btRenderDrawModelList(model);
				}
				syHeapRelease(model);
				syHeapRelease(texture);
			}
			/* The red orb is revealed in its sprite, two pixel pairs of every
			   tile a tick, as its colours rise from black, and it shrinks away
			   to nothing. */
			if(frm >= CHARON_ORB_TICK && frm < CHARON_ORB_GONE_TICK) {
				SCALE revealScale = CharonRevealScale;

				if(frm < 260) {
					Uint8 *low = REVEAL_BUFFER;
					Uint8 *high = REVEAL_BUFFER + 1;
					/* FAKEMATCH(temporary): orderCopy, a copy of order set
					 * again in the loop. */
					Uint8 *orderCopy = order;

					for(i = 0; i != CHARON_REVEAL_TILES; i++) {
						Sint32 pair =
							(orderCopy = order)[OBJ_64X64_8BPP_BYTES] * 2;
						Sint32 tile = i * TILE_SIZE_8BPP;
						Sint32 pixel = tile + pair;
						Uint8 *lowByte = low + pixel;
						Uint8 *highByte = high + pixel;
						Sint32 texel = *lowByte | (*highByte << 8);

						*(Uint16 *)(OBJ_VRAM +
							(tile +
								VramCacheEntries[OBJECTS[CHARON_REVEAL]->slot]
									.ofs) +
							pair) = texel;
						pair = orderCopy[OBJ_64X64_8BPP_BYTES + 1] * 2;
						pixel = tile + pair;
						lowByte = low + pixel;
						highByte = high + pixel;
						texel = *lowByte | (*highByte << 8);
						*(Uint16 *)(OBJ_VRAM +
							(tile +
								VramCacheEntries[OBJECTS[CHARON_REVEAL]->slot]
									.ofs) +
							pair) = texel;
					}
				}
				revealScale.y = revealScale.x =
					310 * (toFIXED(1) / 32) - frm * (toFIXED(1) / 32);
				revealPlace[3] = 0;
				revealPlace[1] = toFIXED(255);
				revealPlace[0] = toFIXED(92);
				revealPlace[2] = toFIXED(72);
				fdRenderApplyProjectedPlacementStub(
					OBJECTS[CHARON_REVEAL], revealPlace, &revealScale.x, 0);
				{
					Uint16 *tgt =
						syResourceGetTableEntry(RESOURCE_RED_ORB_TILES);
					Uint16 *col =
						(Uint16 *)&OBJ_PLTT_COLOR(CHARON_ORB_PALETTE, 0);

					for(i = 0; i != CHARON_ORB_COLORS; i++, tgt++, col++) {
						Sint32 b = RGB_BLUE(*col);
						Sint32 g = RGB_GREEN(*col);
						Sint32 r = RGB_RED(*col);
						Sint32 tb = RGB_BLUE(*tgt);
						Sint32 tg = RGB_GREEN(*tgt);
						Sint32 tr = RGB_RED(*tgt);

						if(b < tb) {
							b++;
						}
						if(g < tg) {
							g++;
						}
						if(r < tr) {
							r++;
						}
						*col = RGB(r, g, b);
					}
				}
			}
			/* A white flash; the backdrop comes back and fades toward a deeper
			   dusk, and three rings of fire spread, eight ticks apart. */
			if(frm > CHARON_RINGS_TICK - 1) {
				if(frm == CHARON_RINGS_TICK) {
					syWordsFill(canvas, CANVAS_VIEW_BYTES,
						CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
					syWordsFill(
						(Uint16 *)&OBJ_PLTT_COLOR(CHARON_ORB_PALETTE, 0),
						CHARON_ORB_COLORS * 2, COLOR_PAIR(RGB_WHITE));
					syWordsCopy((Uint16 *)&BG_PLTT_COLOR(6, 0),
						syResourceGetTableEntry(RESOURCE_AERIAL_VIEW_BACKDROP),
						BATTLE_BACKGROUND_COLORS * 2);
					src = (Uint8 *)syResourceGetTableEntry(RESOURCE_BOLT_SHEET);
					syWordsCopy(BG_PLTT, src, SHEET_PALETTE_BYTES);
					btResourceLoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE,
						state->sheet + CHARON_ACT_PICTURES, 0, 0);
					btGraphicsLoadGreyRamp();
				}
				if(frm > CHARON_RINGS_TICK + 1) {
					Uint16 *tgt =
						syResourceGetTableEntry(RESOURCE_AERIAL_VIEW_BACKDROP);
					Uint16 *col = (Uint16 *)&BG_PLTT_COLOR(6, 0);

					for(i = 0; i != BATTLE_BACKGROUND_COLORS;
						i++, tgt++, col++) {
						Sint32 b = RGB_BLUE(*col);
						Sint32 g = RGB_GREEN(*col);
						Sint32 r = RGB_RED(*col);
						Sint32 tb = RGB_BLUE(*tgt) - CHARON_DEEP_DUSK_DIMMING;
						Sint32 tg = RGB_GREEN(*tgt) - CHARON_DEEP_DUSK_DIMMING;
						Sint32 tr = RGB_RED(*tgt) + CHARON_DUSK_REDDENING;

						if(tb < 0) {
							tb = 0;
						}
						if(tg < 0) {
							tg = 0;
						}
						if(tr > RGB_LEVEL_MAX) {
							tr = RGB_LEVEL_MAX;
						}
						if(b > tb) {
							b--;
						}
						if(g > tg) {
							g--;
						}
						if(r > tr) {
							r--;
						}
						*col = RGB(r, g, b);
					}
				}
				if(frm > CHARON_RINGS_TICK - 1) {
					Sint32		  k;
					Sint32		  x;
					Sint32		  stripShade;
					Sint32		  size;
					void		 *texture = syHeapAllocEWRAM(32);
					RENDER_MODEL *model = btRenderAllocateModelList(1);

					fireRingTexture.logWidth = 7;
					fireRingTexture.logHeight = 7;
					fireRingTexture.pixels = state->sheet + CHARON_ACT_PICTURES;
					model->kind = MODEL_SHADED_ADDITIVE;
					model->texture = &fireRingTexture;
					model->mesh = BattleEffectLargeSquareMesh;
					model->vertices = texture;
					for(k = 0; k != 3; k++) {
						x = frm - CHARON_RINGS_TICK - k * 8;
						if(x >= 0) {
							stripShade = x * 4 - 16;
							size = 140000 - x * 6192;
							if(size < 0) {
								size = 0;
							}
							if(stripShade > 0) {
								stripShade = 0;
							}
							model->shade = stripShade;
							syInitMatrix();
							syTranslate(-toFIXED(8), -toFIXED(8), 0);
							syScale(size / 2, size, size / 2);
							if(k == 0) {
								syRotZ(TRIG_TURN * 15 / 16);
							} else if(k == 1) {
								syRotZ(DEGtoANG(45));
							} else {
								syRotZ(TRIG_TURN * 7 / 8);
							}
							syRotX(TRIG_TURN / 32);
							btRenderTransformVertices(
								BattleEffectGroundSquareCorners, texture, 4);
							btRenderDrawModelList(model);
						}
					}
					syHeapRelease(model);
					syHeapRelease(texture);
				}
			}
			/* The orb turns magenta and swells from nothing, shaking from the
			   disc's tick on. */
			if(frm == CHARON_MAGENTA_TICK) {
				syWordsFill((Uint16 *)&OBJ_PLTT_COLOR(CHARON_ORB_PALETTE, 0),
					CHARON_ORB_COLORS * 2,
					COLOR_PAIR(RGB(RGB_LEVEL_MAX, 0, RGB_LEVEL_MAX)));
			}
			if(frm >= CHARON_MAGENTA_TICK && frm < 351) {
				SCALE  finaleScale = CharonFinaleScale;
				Sint32 dx = 0;
				Sint32 dz = 0;

				if(frm > CHARON_DISC_TICK - 1) {
					dx = 347 - frm;
					dz = (frm - CHARON_DISC_TICK) / 4;
				}
				finaleScale.x = finaleScale.y = frm * (toFIXED(1) / 32) -
					CHARON_MAGENTA_TICK * (toFIXED(1) / 32);
				finalePlace[3] = 0;
				finalePlace[1] = toFIXED(255);
				finalePlace[0] = toFIXED(dx) + toFIXED(92);
				finalePlace[2] = toFIXED(dz) + toFIXED(72);
				fdRenderApplyProjectedPlacementStub(
					OBJECTS[CHARON_REVEAL], finalePlace, &finaleScale.x, 0);
			}
			/* The canvas is drawn opaque in magenta: a dark disc rimmed in
			   magenta swells, and from CHARON_DISC_SWAY_TICK sways across; the
			   backdrop flashes white twice, 19 ticks apart, from ten ticks
			   on. */
			if(frm == CHARON_DISC_TICK) {
				EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
				REG_BLDCNT = 0;
				{
					Uint16 *entry = (Uint16 *)&BG_PLTT_COLOR(0, 1);
					for(i = 0; i != SHEET_PALETTE_COLORS - 1; i++, entry++) {
						Sint32 r = RGB_LEVEL_MAX - i;
						Sint32 b = r;

						if(r < 0) {
							r = 0;
						}
						if(b < 0) {
							b = 0;
						}
						*entry = RGB(r, 0, b);
					}
				}
			}
			if(frm > CHARON_DISC_TICK - 1) {
				Sint32 radius = (frm - (CHARON_DISC_TICK - 2)) * 4;
				Sint32 centerX = CHARON_DISC_X;

				if(frm >= CHARON_DISC_SWAY_TICK) {
					centerX =
						(FIXEDtoINT(-(sySin((frm - CHARON_DISC_SWAY_TICK) *
										  (TRIG_TURN / 64)) *
							CHARON_DISC_X))) +
						CHARON_DISC_X;
				}
				if(frm == 360) {
					state->transferMode = CANVAS_COPY;
				}
				if(frm <= 371) {
					btEffectDrawShadedCircles(
						state->sheet + CHARON_ACT_PICTURES, radius, toFIXED(1));
				} else {
					radius = 104;
				}
				syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
				btCanvasBlitMirroredQuarters(
					state->sheet + CHARON_ACT_PICTURES, centerX, 58, radius);
				EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
					EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
				blit[1] = Ram_HeapSlots->ramCodePaired;
			}
			for(j = 0; j != 2; j++) {
				Sint32 flashFrm = frm - (CHARON_DISC_TICK + 10) - j * 19;

				if(flashFrm == 0) {
					syWordsFill((Uint16 *)&BG_PLTT_COLOR(6, 0),
						BATTLE_BACKGROUND_COLORS * 2, COLOR_PAIR(RGB_WHITE));
				}
				if(flashFrm >= 0 && flashFrm < 160) {
					Uint16 *tgt =
						syResourceGetTableEntry(RESOURCE_AERIAL_VIEW_BACKDROP);
					Uint16 *col = (Uint16 *)&BG_PLTT_COLOR(6, 0);

					for(i = 0; i != BATTLE_BACKGROUND_COLORS;
						i++, tgt++, col++) {
						Sint32 b = RGB_BLUE(*col);
						Sint32 g = RGB_GREEN(*col);
						Sint32 r = RGB_RED(*col);
						Sint32 tb;
						Sint32 tg;
						Sint32 tr;

						if(flashFrm < 8) {
							tb = RGB_BLUE(*tgt);
							tg = RGB_GREEN(*tgt);
							tr = RGB_RED(*tgt);
						} else {
							tb = RGB_BLUE(*tgt) - CHARON_DUSK_DIMMING;
							tg = RGB_GREEN(*tgt) - CHARON_DUSK_DIMMING;
							tr = RGB_RED(*tgt) + CHARON_DUSK_REDDENING;
						}
						if(tb < 0) {
							tb = 0;
						}
						if(tg < 0) {
							tg = 0;
						}
						if(tr > RGB_LEVEL_MAX) {
							tr = RGB_LEVEL_MAX;
						}
						if(b > tb + 1) {
							b -= 2;
						}
						if(g > tg + 1) {
							g -= 2;
						}
						if(r > tr + 1) {
							r -= 2;
						}
						*col = RGB(r, g, b);
					}
				}
			}
		}
		/* As the seal is stamped a ring of motes spreads from it, darker as it
		   grows. */
		if(sealFrm >= 0 && sealFrm < 24) {
			Sint32 radius = frm * 2 - 104;
			Sint32 shade = (radius - 10) / 6;
			Sint32 x;
			Sint32 y;
			Sint32 w;
			Sint32 h;
			FIXED  cosine;

			if(shade < 0) {
				shade = 0;
			}
			w = CHARON_RING_MOTE;
			for(i = 0, h = CHARON_RING_MOTE * 2; i != CHARON_RING_MOTES; i++) {
				x = FIXEDtoINT(
					sySin(i * (TRIG_TURN / CHARON_RING_MOTES)) * radius * 2);
				cosine = syCos(i * (TRIG_TURN / CHARON_RING_MOTES));
				y = (FIXEDtoINT(cosine * radius)) + 68;
				x += 66;
				blitters[0](canvas,
					motePictures + shade * CHARON_RING_MOTE_BYTES, x - w / 2, y,
					w, h);
			}
		}
		/* Before Charon comes the backdrop's green drains away, then its
		   red. */
		if(frm >= 108 && frm < CHARON_ARRIVAL_TICK) {
			Uint16 *col;

			for(i = 0, col = (Uint16 *)&BG_PLTT_COLOR(6, 0);
				i != BATTLE_BACKGROUND_COLORS; i++, col++) {
				Sint32 r = RGB_RED(*col);
				Sint32 g = RGB_GREEN(*col);
				Sint32 b = RGB_BLUE(*col);

				g--;
				if(g < 0) {
					r--;
				}
				if(r > RGB_LEVEL_MAX) {
					r = RGB_LEVEL_MAX;
				} else if(r < 0) {
					r = 0;
				}
				if(g > RGB_LEVEL_MAX) {
					g = RGB_LEVEL_MAX;
				} else if(g < 0) {
					g = 0;
				}
				if(b > RGB_LEVEL_MAX) {
					b = RGB_LEVEL_MAX;
				} else if(b < 0) {
					b = 0;
				}
				*col = RGB(r, g, b);
			}
		}
		/* The rune seal is stamped into the battle background's own tiles, on
		   the ground, and its colours pulse for 32 ticks. */
		if(sealFrm >= 0 && sealFrm < 36) {
			if(frm == CHARON_SEAL_TICK) {
				Sint32 x;
				Sint32 y;

				EFFECT_SET_TRANSFER(
					state, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(4));
				src =
					(Uint8 *)syResourceGetTableEntry(RESOURCE_RUNE_SEAL_SHEET);
				src += PALETTE_BANK_BYTES;
				syDecompressTaggedLZ(src, state->sheet + CHARON_RUNE_SEAL);
				for(y = 0; y != RUNE_SEAL_HEIGHT; y++) {
					for(x = 0; x != RUNE_SEAL_WIDTH; x += 2) {
						Uint16 *dst = (Uint16 *)(CHARON_SEAL_TILES +
							((((y / 8) * 32 + x / 8) * 8 + (y & 7)) * 8 +
								(x & 7)));
						Uint8	hi = *dst >> 8;
						Uint8	lo = *dst;

						if(state->sheet[CHARON_RUNE_SEAL + y * RUNE_SEAL_WIDTH +
							   x + 1] != 0) {
							hi = state->sheet[CHARON_RUNE_SEAL +
									 y * RUNE_SEAL_WIDTH + x + 1] +
								CHARON_SEAL_COLOR;
						}
						if(state->sheet[CHARON_RUNE_SEAL + y * RUNE_SEAL_WIDTH +
							   x] != 0) {
							lo = state->sheet[CHARON_RUNE_SEAL +
									 y * RUNE_SEAL_WIDTH + x] +
								CHARON_SEAL_COLOR;
						}
						*dst = (hi << 8) | lo;
					}
				}
			}
			if(frm < CHARON_SEAL_TICK + 32) {
				Uint16 *col = syResourceGetTableEntry(RESOURCE_RUNE_SEAL_SHEET);
				Uint16 *entry =
					(Uint16 *)&BG_PLTT_COLOR(CHARON_SEAL_PALETTE, 0);

				for(i = 0; i != PALETTE_COLORS; i++, col++, entry++) {
					Sint32 add = (FIXEDtoINT(syCos(sealFrm << 9) *
									 CHARON_SEAL_GLOW_PULSE)) +
						CHARON_SEAL_GLOW;
					Sint32 r = RGB_RED(*col) + add;
					Sint32 g = RGB_GREEN(*col) + add;
					Sint32 b = RGB_BLUE(*col) + add;

					if(r > RGB_LEVEL_MAX) {
						r = RGB_LEVEL_MAX;
					}
					if(g > RGB_LEVEL_MAX) {
						g = RGB_LEVEL_MAX;
					}
					if(b > RGB_LEVEL_MAX) {
						b = RGB_LEVEL_MAX;
					}
					*entry = RGB(r, g, b);
				}
			}
		}
		/* The canvas flashes as the seal comes: half bright, then white. */
		if(frm == 54) {
			syWordsFill(canvas, CANVAS_VIEW_BYTES,
				CANVAS_FILL_WORD((CANVAS_LEVEL_BRIGHTEST + 1) / 2));
		}
		if(frm == 55) {
			syWordsFill(canvas, CANVAS_VIEW_BYTES,
				CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
		}
		/* Until Charon comes the swirl turns on the ground, a second inside it
		   the other way; from the seal on, the charm ring and a smaller one
		   inside it turn with them. */
		if(frm < CHARON_ARRIVAL_TICK) {
			Sint32 risingScale;
			Sint32 shade;
			Sint32 rise = toFIXED(1) / 2 - CHARON_SEAL_TICK * CHARON_SCALE_STEP;
			Sint32 base;
			Sint32 quarter;
			Sint32 twice;
			void  *texture = syHeapAllocEWRAM(32);
			RENDER_MODEL *model = btRenderAllocateModelList(1);

			EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
			twice = frm * 2;
			quarter = frm * (CHARON_SCALE_STEP / 4);
			shade = twice + MODEL_SHADE_INVISIBLE;
			base = quarter * 4;
			risingScale = CHARON_SWIRL_SCALE - base;
			if(frm >= CHARON_SEAL_TICK) {
				risingScale = base + rise;
			}
			if(shade >= -8) {
				shade = -8;
			}
			if(frm > CHARON_SWIRL_FADE_TICK - 1) {
				shade = 184 - frm * 2;
				model->shade = shade;
			}
			swirlTexture.logWidth = 6;
			swirlTexture.logHeight = 6;
			swirlTexture.pixels = state->sheet;
			model->kind = MODEL_SHADED_ADDITIVE;
			model->texture = &swirlTexture;
			model->mesh = BattleEffectSquareMesh;
			model->vertices = texture;
			if(shade > CHARON_SWIRL_SHADE_MIN) {
				model->shade = shade;
				syInitMatrix();
				syTranslate(toFIXED(4), 0, 0);
				syScale(risingScale, risingScale, risingScale);
				syRotX(DEGtoANG(45));
				if(frm >= CHARON_SEAL_TICK) {
					syRotY(frm * (TRIG_TURN / 64));
				} else {
					syRotY(-frm * (TRIG_TURN / 64));
				}
				btRenderTransformVertices(
					BattleEffectGroundSquareCorners, texture, 4);
				btRenderDrawModelList(model);
			}
			if(frm > CHARON_SWIRL_FADE_TICK - 1) {
				shade = 568 - frm * 6;
				model->shade = shade;
			}
			if(shade > CHARON_SWIRL_SHADE_MIN) {
				syInitMatrix();
				syTranslate(toFIXED(4), -toFIXED(16), 0);
				syMatrixScaleUniform(-CHARON_INNER_SWIRL_SCALE);
				syRotX(DEGtoANG(45));
				syRotY(-frm * (TRIG_TURN / 128));
				btRenderTransformVertices(
					BattleEffectGroundSquareCorners, texture, 4);
				btRenderDrawModelList(model);
			}
			if(frm >= CHARON_SEAL_TICK) {
				Sint32 ringShade = frm * 2 - 208;

				swirlTexture.pixels = state->sheet + CHARON_ACT_PICTURES;
				if(ringShade >= MODEL_SHADE_INVISIBLE / 2) {
					ringShade = MODEL_SHADE_INVISIBLE / 2;
				}
				model->shade = ringShade;
				syInitMatrix();
				syTranslate(toFIXED(2), -toFIXED(32), 0);
				syMatrixScaleUniform(CHARON_CHARM_RING_SCALE);
				syRotX(DEGtoANG(45));
				syRotY(frm * (TRIG_TURN / 64));
				btRenderTransformVertices(
					BattleEffectGroundSquareCorners, texture, 4);
				btRenderDrawModelList(model);
				/* FAKEMATCH(dead): twice raised to tick though nothing reads
				 * it again. */
				if(frm > twice) {
					twice = frm;
				}
				/* DEAD: the swirls' test of their shade, made of tick, which
				   is never that low; the ROM keeps the test. */
				if(frm > CHARON_SWIRL_SHADE_MIN) {
					syInitMatrix();
					syTranslate(toFIXED(2), -toFIXED(48), 0);
					syMatrixScaleUniform(-CHARON_INNER_RING_SCALE);
					syRotX(DEGtoANG(45));
					syRotY(-frm * (TRIG_TURN / 64));
					btRenderTransformVertices(
						BattleEffectGroundSquareCorners, texture, 4);
					btRenderDrawModelList(model);
				}
			}
			syHeapRelease(model);
			syHeapRelease(texture);
			/* DEAD: each test reads the count twice and drops the first read;
			   the ROM keeps both loads. */
			while(state->framesSinceTransfer, state->framesSinceTransfer <= 1) {
			}
		}
		state->transferPending = 1;
		sySynch(1);
	}

	sySndPlayStub(SE_WAVE);
	if(frm < CHARON_ARRIVAL_TICK) {
		fdSprDestroyStub((SPR *)OBJECTS[0]);
	} else {
		for(i = 0; i != CHARON_OBJECTS; i++) {
			fdSprDestroyStub((SPR *)OBJECTS[i]);
		}
	}
	for(i = 0; i != CHARON_SHARDS; i++) {
		fdSprDestroyStub((SPR *)SPRITES[i]);
	}
	EFFECT_RELEASE_BLITTERS();
	syPaletteLoadBasicSprCols();
	REG_DISPCNT = DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON;
	sySynch(1);
	battle->keepBackdropPal = 0;
	btBackgroundLoadStub(
		BATTLE_BACKGROUND_FIXED, battle->bg, BACKGROUND_DARKENED_LEVEL);
	IWRAM_FILL_WORDS(canvas, CANVAS_BYTES, 0);
	IWRAM_FILL_WORDS((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES, 0);
	ProjectionWork.centerY = BATTLE_PROJECTION_CENTER_Y;
	camera = Ram_HeapSlots->cameraState;
	shake = 0;
	savedYaw = camera->yaw;
	camera->yaw = shake;
	EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
	EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, 4);
	REG_BG2X = 0;
	REG_BG2CNT = BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
		BGCNT_SCREEN_BASE(7);
	/* The attack's ramp runs from a pink violet down to black. */
	for(i = 1; i != SHEET_PALETTE_COLORS; i++) {
		Sint32 q = i * 2 / 3;
		Sint32 r = RGB_LEVEL_MAX - q;
		Sint32 g = CHARON_RAMP_GREEN - i / 2;
		Sint32 b = CHARON_RAMP_BLUE - q;

		if(r < 0) {
			r = 0;
		}
		if(g < 0) {
			g = 0;
		}
		if(b < 0) {
			b = 0;
		}
		BG_PLTT[i] = RGB(r, g, b);
	}
	arg->tgts[arg->tgtCnt] = BATTLE_UNIT_LIST_END;
	btActorSpawnObjectsForListStub(arg->tgts, 0);
	btEffectDrawShadedCircles(state->sheet, CHARON_COLUMN_END, toFIXED(1));
	btEffectDrawShadedCircles(
		state->sheet + CHARON_ACT_PICTURES, CHARON_DROP_RADIUS, toFIXED(1));
	syWordsFill(state->sheet + CHARON_BRIGHT_BLOCK, CHARON_TEXTURE_BYTES,
		CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
	btResourceLoadAndDecompress(
		RESOURCE_GLOW_RING_TEXTURE, state->sheet + CHARON_GLOW_RING, 0, 0);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, motePictures, 0, 0);
	/* DEAD: i counts to MOTE_SET_BYTES by 35, and nothing reads it; the ROM
	   keeps the add and the test. */
	{
		Sint32 limit;

		i = 0;
		limit = MOTE_SET_BYTES;
	empty:
		i += 35;
		if(i != limit) {
			goto empty;
		}
	}
	for(i = 0; i != CHARON_TEXTURE_BYTES; i++) {
		if(state->sheet[CHARON_GLOW_RING + i] != 0) {
			state->sheet[CHARON_GLOW_RING + i] = CANVAS_LEVEL_BRIGHTEST;
		}
	}
	IO_WRITE_QUEUE_PUSH(
		(Uint32)&REG_DISPCNT, EFFECT_DISPCNT, IO_WRITE_HALFWORD);
	for(i = 0; i != CHARON_DROP_SLOTS; i++) {
		DROPS[i].variant = -i;
	}
	EFFECT_FREE_PARTICLES(
		i, CHARON_PARTICLES, CHARON_MOTE_COUNT + CHARON_BURST_COUNT);
	for(i = 0; i != arg->tgtCnt; i++) {
		ACTOR *actor = btObjectGetStub(arg->tgts[i])->obj;

		HOMES[i].x = actor->x.fixed;
		HOMES[i].y = actor->y.fixed;
		HOMES[i].z = actor->z.fixed;
	}

	width = CHARON_COLUMN_END;
	dimming = CHARON_FLASH_DIMMING;
	frm = 0;
	do {
		if((PerWork.repeat & (KEY_A | KEY_B)) && frm < 120) {
			frm = 120;
		}
		if(frm == 0) {
			sySndPlayStub(SE_WAVE);
			WAVE_RADIUS = 160;
			shake = 0;
			for(i = 0; i != arg->tgtCnt; i++) {
				CURSES[i].variant = -1;
			}
			for(i = 0; i != arg->tgtCnt; i++) {
				ACTOR *actor = btObjectGetStub(arg->tgts[i])->obj;

				btEffectTintBattler(arg->tgts[i], SPRITE_COLOR_NORMAL,
					POSE_DAMAGE, TINT_KEEP, -1);
				actor->x.fixed = -(i + 4) * toFIXED(128);
				actor->z.fixed = 0;
				actor->y.fixed = toFIXED(20);
				actor->gravity = 0;
			}
		}
		if(frm == 120) {
			EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
			/* The battle's own background comes back as dark as a canvas
			   effect's fade leaves it. */
			btColBufferScaleStub(battle->pal, (Uint16 *)&BG_PLTT_COLOR(6, 0),
				toFIXED(1) - BACKGROUND_DARKEN_TICKS * BACKGROUND_LEVEL_STEP,
				BATTLE_BACKGROUND_COLORS);
			for(i = 0; i != arg->tgtCnt; i++) {
				ACTOR *actor = btObjectGetStub(arg->tgts[i])->obj;

				SET_ACTOR_POSITION(actor, 0, toFIXED(124), 0);
				actor->gravity = 0;
			}
		}
		if(frm == 160) {
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
		}
		/* At tick 140 the curse breaks in a white flash: flash bursts and
		   sparks fly from the middle, and the targets are thrown up. */
		if(frm >= 140) {
			if(frm == 140) {
				for(i = 0; i != CHARON_BURST_COUNT; i++) {
					Sint32 speed = (syRandom() & 511) + 32;
					Sint32 ang = syRandom();

					CHARON_BURSTS[i].x = toFIXED(60);
					CHARON_BURSTS[i].y = toFIXED(60);
					CHARON_BURSTS[i].velX = (sySin(ang) * speed) >> 5;
					CHARON_BURSTS[i].velY = (syCos(ang) * speed) >> 5;
					CHARON_BURSTS[i].variant = 0;
				}
				for(i = 0; i != CHARON_SPARK_COUNT; i++) {
					Sint32 speed = (syRandom() & 255) + 32;
					Sint32 ang = syRandom();

					CHARON_SPARKS[i].x = toFIXED(60);
					CHARON_SPARKS[i].y = toFIXED(60);
					CHARON_SPARKS[i].velX = (sySin(ang) * speed) >> 6;
					CHARON_SPARKS[i].velY = (syCos(ang) * speed) >> 6;
					CHARON_SPARKS[i].variant = (syRandom() & 15) + 16;
				}
				btResourceLoadAndDecompress(
					RESOURCE_FLASH_BURST_SHEET, SPRITE_BUFFER, 1, 0);
				sySndPlayStub(SE_CLATTER);
				syWordsFill(canvas, CANVAS_VIEW_BYTES,
					CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
				REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
					BGCNT_SCREEN_BASE(7);
				EFFECT_SET_TRANSFER(
					state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
				for(i = 0; i != arg->tgtCnt; i++) {
					ACTOR *actor = btObjectGetStub(arg->tgts[i])->obj;

					SET_ACTOR_POSITION(actor, 0, toFIXED(32), HOMES[i].z);
				}
				for(i = 0; i != arg->tgtCnt; i++) {
					Sint32 roll = syRandom();
					Sint32 percent = (syRandom() & 31) + 120;
					Sint32 velocityY = roll * 2 + toFIXED(2);

					btEffectStartActorMotion(arg->tgts[i], 1, toFIXED(4),
						toFIXED(8), velocityY, percent);
					btEffectTintBattler(
						arg->tgts[i], SPRITE_COLOR_WHITE, POSE_DAMAGE, i, 32);
				}
				state->shakeFrm = 3;
			}
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
				EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, BLITTER_MAX);
			blitters[0] = (Ram_HeapSlots->ramCode);
			for(i = 0; i != CHARON_BURSTS_DRAWN; i++) {
				PARTICLE *p = &CHARON_BURSTS[i];

				if(p->variant >= 0 && p->variant < CHARON_BURST_TICKS) {
					Sint32 row = (p->variant / 4) * PICTURE_BYTES(FLASH_BURST);
					Uint8 *buf = SPRITE_BUFFER;
					Sint32 x = (FIXEDtoINT(p->x)) - FLASH_BURST_WIDTH / 2;

					row += (Sint32)buf;
					buf = (Uint8 *)row;
					blitters[0](canvas, buf, x,
						(FIXEDtoINT(p->y)) - FLASH_BURST_HEIGHT / 2,
						FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
					btParticleAdvanceWithGravity2D(p, 60, -toFIXED(1) / 8);
				}
				p->variant++;
			}
			for(i = 0; i != CHARON_SPARK_COUNT; i++) {
				PARTICLE *p = &CHARON_SPARKS[i];

				if(p->variant >= 0) {
					Sint32 size = (p->variant >> 3) + 2;
					Sint32 k;

					for(k = 0; k != 4; k++) {
						blitters[0](canvas,
							motePictures + BattleEffectMotePictures[size - 1],
							(FIXEDtoINT(p->x)) - size / 2,
							(FIXEDtoINT(p->y)) - size, size, size * 2);
						btParticleAdvanceWithGravity2D(p, 63, 0);
						if(p->y > toFIXED(112)) {
							p->velY = -p->velY;
						}
					}
					p->variant--;
				}
			}
			syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
		}
		/* The backdrop flashes white three times as the targets are drawn
		   in. */
		if(frm < 120) {
			for(i = 0; i != CHARON_FLASHES; i++) {
				if(frm == CharonFlashTicks[i]) {
					syWordsFill((Uint16 *)&BG_PLTT_COLOR(6, 0),
						BATTLE_BACKGROUND_COLORS * 2, COLOR_PAIR(RGB_WHITE));
				}
				if(frm >= CharonFlashTicks[i] + 1 &&
					frm < CharonFlashTicks[i] + 16) {
					Uint16 *tgt = Ram_HeapSlots->battleState->pal;
					Uint16 *col = (Uint16 *)&BG_PLTT_COLOR(6, 0);

					for(j = 0; j != BATTLE_BACKGROUND_COLORS; j++) {
						Sint32 tr = RGB_RED(tgt[j]) - dimming / 2;
						Sint32 tg = RGB_GREEN(tgt[j]) - CHARON_FLASH_DIMMING;
						Sint32 tb = RGB_BLUE(tgt[j]) - dimming / 2;
						Sint32 r = RGB_RED(col[j]);
						Sint32 g = RGB_GREEN(col[j]);
						Sint32 b = RGB_BLUE(col[j]);

						if(tr < 0) {
							tr = 0;
						}
						if(tg < 0) {
							tg = 0;
						}
						if(tb < 0) {
							tb = 0;
						}
						if(tr < r - 1) {
							r -= 2;
						}
						if(tg < g - 1) {
							g -= 2;
						}
						if(tb < b - 1) {
							b -= 2;
						}
						col[j] = RGB(r, g, b);
					}
				}
			}
		}
		if(frm > 120) {
			if(frm == 121) {
				BgScrollWork[1].x = savedScrollX;
				backgroundView->busy = 0;
				camera->yaw = savedYaw;
			}
			if(frm < 148) {
				Sint32 step = 8 - (frm - 120) / 6;
				Sint32 wave = WAVE_RADIUS;

				WAVE_RADIUS = wave - step;
				if(wave > 0) {
					btEffectDrawShadedCircles(state->sheet, wave, toFIXED(1));
					btCanvasBlitMirroredQuarters(state->sheet, 60, 60, wave);
				}
			}
		} else {
			if(frm >= 4 && frm < 26) {
				shake += 4;
			}
			if(frm > 107) {
				shake -= 8;
				if(shake < 0) {
					shake = 0;
				}
			}
			for(i = 0; i != arg->tgtCnt; i++) {
				ACTOR	 *actor = btObjectGetStub(arg->tgts[i])->obj;
				PARTICLE *curse = &CURSES[i];
				Sint32	  landed = 0;

				if(actor->y.fixed < toFIXED(100)) {
					if(curse->variant == -1) {
						actor->x.fixed += toFIXED(10);
					} else {
						actor->x.fixed += toFIXED(2);
					}
					if(curse->variant == -1) {
						if(actor->x.fixed > toFIXED(48)) {
							landed = 1;
						}
						if(landed == 1) {
							curse->variant = 0;
							btEffectPosProjectScaledBattler(
								arg->tgts[i], curse);
							sySndPlayStub(SE_CURSE);
							for(j = 0; j != CHARON_CURSE_MOTES; j++) {
								Sint32 ang = syRandom() & (TRIG_TURN / 2 - 1);
								Sint32 speed;
								PARTICLE *p =
									&CHARON_PARTICLES[i * CHARON_CURSE_MOTES +
										j];

								speed = (syRandom() & 255) + 256;

								/* The canvas is half the screen's width. */
								p->x = curse->x * (toFIXED(1) / 2);
								p->y = toFIXED(curse->y);
								p->velX = -(sySin(ang) * speed) >> 7;
								p->velY = (syCos(ang) * speed) >> 6;
								p->variant = (syRandom() & 15) + 16;
							}
						}
					}
				}
				if(curse->variant == 2 || curse->variant == 7) {
					btEffectTintBattler(
						arg->tgts[i], SPRITE_COLOR_WHITE, POSE_DAMAGE, i, 32);
				}
				/* FAKEMATCH(dead): landed set to 2 though nothing reads it
				 * again. */
				if(actor == 0) {
					landed = 2;
				}
				if(curse->variant == 6) {
					btEffectTintBattler(
						arg->tgts[i], SPRITE_COLOR_HIDDEN, TINT_KEEP, i, 32);
				}
				if(curse->variant == 8) {
					SET_ACTOR_POSITION(actor, 0, toFIXED(148), 0);
				}
			}
			if(frm >= 0) {
				BgScrollWork[1].x -= 12;
				EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
					EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, BLITTER_MAX);
				{
					PARTICLE *column;
					Sint32	  x;
					Sint32	  y;
					Sint32	  y2;
					for(i = 0, y2 = -26, y = 0, x = shake,
					phase = frm * (TRIG_TURN / 64), column = COLUMNS,
					halfWidth = width / 2;
						i != CHARON_COLUMNS;
						y2 += CHARON_COLUMN_HEIGHT, y += CHARON_COLUMN_HEIGHT,
					x += 8, phase += DEGtoANG(45), column++, i++) {
						column->x = x + (FIXEDtoINT(sySin(phase) * 4));
						column->y = y;
						if(i == 3) {
							column->x -= 8;
							column->y = y2;
							syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
							EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
								EFFECT_CLIP_BOUNDS, BLITTER_MAX);
						}
						(Ram_HeapSlots->ramCode)(canvas, state->sheet,
							column->x - halfWidth, column->y, halfWidth, width);
					}
				}
				syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
				for(i = 0; i != CHARON_DROPS; i++) {
					PARTICLE *drop = &DROPS[i];

					if(++drop->variant == 4) {
						drop->variant = 0;
					}
					if(drop->variant == 0) {
						drop->x = toFIXED(syRandom() % 24 + shake - i * 2);
						drop->y = toFIXED(80) + i * toFIXED(3);
						drop->velX =
							-(syRandom() & TRIG_ANGLE_MASK) - toFIXED(0.25);
						drop->velY = -toFIXED(1);
					}
					if(drop->variant >= 0) {
						btCanvasBlitMirroredQuarters(
							state->sheet + CHARON_ACT_PICTURES,
							FIXEDtoINT(drop->x), FIXEDtoINT(drop->y),
							CHARON_DROP_RADIUS);
						drop->x += drop->velX;
						drop->y += drop->velY;
					}
				}
				EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
					EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, BLITTER_MAX);
				blitters[0] = (Ram_HeapSlots->ramCode);
				for(i = 0; i != CHARON_COLUMNS; i++) {
					Sint32 width = 120 - COLUMNS[i].x;

					if(width > 0) {
						blitters[0](canvas, state->sheet + CHARON_BRIGHT_BLOCK,
							COLUMNS[i].x, i * CHARON_COLUMN_HEIGHT, width,
							CHARON_COLUMN_HEIGHT);
					}
				}
				syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
				EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
					EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, BLITTER_TRANSPARENT);
				blitters[0] = (Ram_HeapSlots->ramCode);
				for(i = 0; i != CHARON_MOTE_COUNT; i++) {
					PARTICLE *p = &CHARON_PARTICLES[i];

					if(p->variant >= 0) {
						Sint32 size = (p->variant >> 3) + 2;

						blitters[0](canvas,
							motePictures + BattleEffectMotePictures[size - 1],
							(FIXEDtoINT(p->x)) - size / 2,
							(FIXEDtoINT(p->y)) - size, size, size * 2);
						btParticleAdvanceWithGravity2D(p, 60, 0);
						p->variant--;
					}
				}
				syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
			}
			{
				void		 *texture = syHeapAllocEWRAM(32);
				RENDER_MODEL *model = btRenderAllocateModelList(1);

				glowRingTexture.logWidth = 6;
				glowRingTexture.logHeight = 6;
				glowRingTexture.pixels = state->sheet + CHARON_GLOW_RING;
				model->kind = MODEL_SHADED;
				model->mesh = BattleEffectSquareMesh;
				model->texture = &glowRingTexture;
				model->vertices = texture;
				for(i = 0; i != arg->tgtCnt; i++) {
					PARTICLE *curse = &CURSES[i];
					Sint32	  tmr = curse->variant;

					if(tmr >= 0 && tmr < 18) {
						Sint32 step = tmr * 3;
						Sint32 shade = step + MODEL_SHADE_INVISIBLE;
						Sint32 size =
							step * CHARON_GLOW_RING_GROWTH + toFIXED(1) / 32;

						curse->variant = tmr + 1;
						model->shade = shade;
						syInitMatrix();
						syTranslate(INTtoFIXED(curse->x / 2 - 56),
							INTtoFIXED(curse->y - 56), 0);
						syScale(size / 3, size, size);
						syRotX(TRIG_TURN * 3 / 4);
						btRenderTransformVertices(
							BattleEffectGroundSquareCorners, texture, 4);
						btRenderDrawModelList(model);
					}
					/* A release only sets its pool back to the block, so the
					   next target still draws from the same model. */
					syHeapRelease(model);
					syHeapRelease(texture);
				}
			}
		}
		btCameraApplyVerticalShake(8);
		btEffectCountDownBattlerTints();
		state->transferPending = 1;
		sySynch(1);
		frm++;
	} while(frm != CHARON_ATTACK_TICKS);
	/* DEAD: an empty loop over the targets; the ROM keeps its count and
	   test. */
	for(i = 0; i != arg->tgtCnt; i++) {
	}
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
	syCloseEvent(btEffectTransferCanvas);
	btEffectEndCanvasLayer();
}

/* One quad of a vertex strip: its upper and lower triangles. */
typedef struct STRIP_QUAD {
	MESH_TRIANGLE upper;
	MESH_TRIANGLE lower;
} STRIP_QUAD;

/* Builds count quads of a vertex strip two vertices apart, each width texels
   wide and height texels tall, and ends the list with an empty triangle. */
void btRenderBuildQuadStrip(
	STRIP_QUAD *quad, Sint32 width, Sint32 height, Sint32 cnt)
{
	Sint32 i;

	for(i = 0; i != cnt; i++) {
		quad[i].upper.uv[0][1] = 1;
		quad[i].upper.uv[2][1] = 1;
		quad[i].lower.uv[2][1] = 1;
		quad[i].upper.vertex[0] = i * 2;
		quad[i].upper.uv[0][0] = i * width;
		quad[i].upper.uv[1][0] = i * width;
		quad[i].upper.uv[2][0] = i * width + width - 1;
		quad[i].lower.uv[0][0] = i * width;
		quad[i].lower.uv[1][0] = i * width + width - 1;
		quad[i].lower.uv[2][0] = i * width + width - 1;
		quad[i].upper.vertex[1] = i * 2 + 1;
		quad[i].upper.vertex[2] = i * 2 + 2;
		quad[i].lower.vertex[0] = i * 2 + 1;
		quad[i].lower.vertex[2] = i * 2 + 2;
		quad[i].lower.vertex[1] = i * 2 + 3;
		quad[i].upper.uv[1][1] = height - 1;
		quad[i].lower.uv[0][1] = height - 1;
		quad[i].lower.uv[1][1] = height - 1;
	}
	SET_VECTOR(quad[cnt].upper.vertex, 0, 0, 0);
}

#undef SPRITES
#undef SPRITE_BUFFER
#undef OAM_BUFFER
#undef REVEAL_BUFFER
#undef ORB_BUFFER
#undef CHARON_SEAL_TILES
#undef DROPS
#undef CURSES
#undef HOMES
#undef COLUMNS
#undef WAVE_RADIUS
#undef OBJECTS
