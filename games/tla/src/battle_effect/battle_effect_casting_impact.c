/* The casting impacts, the Djinn attacks, unleashes and enemy abilities that
   open the same way: sparks close in on the caster, a streak shoots at the
   target and the orb flashes over it in a spray of sparks, and then each impact
   plays its own effect over the target (CASTING_IMPACT). And the step of the
   background colours toward a palette. Part of battle_effect.c. */

/* Steps background colours 0 to 63 one level towards a palette resource. */
void btEffectStepPaletteToResource(Sint32 resourceId)
{
	Uint16	target[SHEET_PALETTE_COLORS];
	Uint16 *color = (Uint16 *)BG_PLTT;
	Sint32	i;

	syDMAXCopy(syResourceGetTableEntry(resourceId), target,
		DMA_COPY_WORDS(SHEET_PALETTE_BYTES / 4), REG_DMA3);
	target[0] = 0;
	for(i = 0; i != SHEET_PALETTE_COLORS; i++, color++) {
		Sint32 red = RGB_RED(*color);
		Sint32 green = RGB_GREEN(*color);
		Sint32 blue = RGB_BLUE(*color);
		Sint32 targetRed = RGB_RED(target[i]);
		Sint32 targetGreen = RGB_GREEN(target[i]);
		Sint32 targetBlue = RGB_BLUE(target[i]);

		if(red < targetRed)
			red++;
		else if(red > targetRed)
			red--;
		if(green < targetGreen)
			green++;
		else if(green > targetGreen)
			green--;
		if(blue < targetBlue)
			blue++;
		else if(blue > targetBlue)
			blue--;
		target[i] = RGB(red, green, blue);
	}
	syDMAXCopy(target, (void *)BG_PLTT,
		DMA_COPY_HALFWORDS(SHEET_PALETTE_COLORS), REG_DMA3);
}

/* The casting impacts, by effect kind less 100: each is named for the first
   ability whose effect entry selects it as a casting impact; those no ability
   selects are named UNUSED_ and for what they load or share. The kinds before
   CASTING_IMPACT_DJINN_KINDS are the Djinn's; from it on they are weapons'
   unleashes and enemies' abilities. */
typedef enum CASTING_IMPACT {
	CASTING_IMPACT_IMPACT_BANE = 0,
	CASTING_IMPACT_IMPACT_SMOG,
	CASTING_IMPACT_IMPACT_MIST,
	CASTING_IMPACT_IMPACT_FEVER,
	CASTING_IMPACT_IMPACT_FLINT,
	CASTING_IMPACT_IMPACT_SCORCH,
	CASTING_IMPACT_IMPACT_SQUALL,
	CASTING_IMPACT_IMPACT_SAP,
	CASTING_IMPACT_IMPACT_ECHO,
	CASTING_IMPACT_IMPACT_SOUR,
	CASTING_IMPACT_IMPACT_CHILL,
	CASTING_IMPACT_IMPACT_GEL,
	CASTING_IMPACT_IMPACT_STEEL,
	CASTING_IMPACT_IMPACT_FOG,
	CASTING_IMPACT_IMPACT_UNUSED_BOLTS,
	CASTING_IMPACT_IMPACT_UNUSED_SKULLS,
	CASTING_IMPACT_IMPACT_WHEEZE,
	CASTING_IMPACT_IMPACT_BLITZ,
	CASTING_IMPACT_IMPACT_TERRA_STRIKE = 38,
	CASTING_IMPACT_IMPACT_POISON_CLOUD,
	CASTING_IMPACT_IMPACT_POISON_DEATH,
	CASTING_IMPACT_IMPACT_MORTAL_DANGER, /* also Death Scythe */
	CASTING_IMPACT_IMPACT_BAD_OMEN,
	CASTING_IMPACT_IMPACT_LIFE_NOURISH,
	CASTING_IMPACT_IMPACT_AQUA_SOCK,
	CASTING_IMPACT_IMPACT_UNUSED_ICE_CHIPS,
	CASTING_IMPACT_IMPACT_FROST_BITE,
	CASTING_IMPACT_IMPACT_UNUSED_GATHERING,
	CASTING_IMPACT_IMPACT_LIFE_LEECH,
	CASTING_IMPACT_IMPACT_PSY_LEECH,
	CASTING_IMPACT_IMPACT_BROIL,
	CASTING_IMPACT_IMPACT_MELTDOWN,
	CASTING_IMPACT_IMPACT_HEAT_MIRAGE, /* also Heat Flash */
	CASTING_IMPACT_IMPACT_BARRAGE,
	CASTING_IMPACT_IMPACT_UNUSED_MYSTIC_FLAME,
	CASTING_IMPACT_IMPACT_ACID_BATH,
	CASTING_IMPACT_IMPACT_UNUSED_COMETS,
	CASTING_IMPACT_IMPACT_STUN_VOLTAGE,
	CASTING_IMPACT_IMPACT_BLINDING_SMOG,
	CASTING_IMPACT_IMPACT_MURK,
	CASTING_IMPACT_IMPACT_CYCLONE_SLASH,
	CASTING_IMPACT_IMPACT_PSYPHON_SEAL,
	CASTING_IMPACT_IMPACT_ASURA,
	CASTING_IMPACT_IMPACT_FORCIBLE_ARM, /* also Grand Golem */
	CASTING_IMPACT_IMPACT_TRUNCHEON_FIST,
	CASTING_IMPACT_IMPACT_UNUSED_SEVERE_BLOW,
	CASTING_IMPACT_IMPACT_POWER_DRIVE,
	CASTING_IMPACT_IMPACT_HEAT_SMASH,
	CASTING_IMPACT_IMPACT_STUN_CLOUD,
	CASTING_IMPACT_IMPACT_SEARING_FOG,
	CASTING_IMPACT_IMPACT_ICE_CRUSH,
	CASTING_IMPACT_IMPACT_SARGASSO,
	CASTING_IMPACT_IMPACT_EVIL_EYE,
	CASTING_IMPACT_IMPACT_VEIN_TAP,
	CASTING_IMPACT_IMPACT_SCORPIONFISH,
	CASTING_IMPACT_IMPACT_MOON_AIR,
	CASTING_IMPACT_IMPACT_FLASH_EDGE,
	CASTING_IMPACT_IMPACT_HIGH_VITALS,
	CASTING_IMPACT_IMPACT_FLARE_BURST,
	CASTING_IMPACT_IMPACT_HAMMERSPHERE,
	CASTING_IMPACT_IMPACT_FLASH_FORCE,
	CASTING_IMPACT_IMPACT_CRUCIBLE_FIRE,
	CASTING_IMPACT_IMPACT_REVERSE_STAR,
	CASTING_IMPACT_IMPACT_SUPERNOVA,
	CASTING_IMPACT_IMPACT_STUN_BOLT,
	CASTING_IMPACT_IMPACT_SARCOPHAGUS,
	CASTING_IMPACT_IMPACT_RAIDENS_WRATH,
	CASTING_IMPACT_IMPACT_UNUSED_TILED_CANVAS,
	CASTING_IMPACT_IMPACT_APOCALYPSE,
	CASTING_IMPACT_IMPACT_FLASH_PUNCH,
	CASTING_IMPACT_IMPACT_ARMOR_CRUSH,
	CASTING_IMPACT_IMPACT_STUN_JIP,
	CASTING_IMPACT_IMPACT_QUICK_SLASH,
	CASTING_IMPACT_IMPACT_BOSCA_HIT,
	CASTING_IMPACT_IMPACT_MOTHER_EARTH,
	CASTING_IMPACT_IMPACT_ROC,		 /* also Orc */
	CASTING_IMPACT_IMPACT_WILD_WOLF, /* also Fatal Fang */
	CASTING_IMPACT_IMPACT_DINOX,	 /* also Power Bite */
	CASTING_IMPACT_IMPACT_TERRIBLE_BITE,
	CASTING_IMPACT_IMPACT_CALL_ZOMBIE,
	CASTING_IMPACT_IMPACT_UNUSED_MERCURY_DJINNI
} CASTING_IMPACT;

/* The Djinn's kinds come before this one, the first unleash's. */
#define CASTING_IMPACT_DJINN_KINDS CASTING_IMPACT_IMPACT_TERRA_STRIKE

/* The impact orb's pictures, packed at half width, after the streak's. */
#define CASTING_IMPACT_ORB_PICTURES	  (6 * PICTURE_BYTES(FIRE_STREAK))
/* The Djinni's picture, in the effect's pictures after the impact's own. */
#define CASTING_IMPACT_DJINNI_WIDTH	  20
#define CASTING_IMPACT_DJINNI_HEIGHT  40
#define CASTING_IMPACT_DJINNI_PICTURE 15446
#define CASTING_IMPACT_GATHER_FRAMES  32 /* frames the sparks gather */
#define CASTING_IMPACT_SPARKS		  64 /* sparks in state->particles */
/* How far either side of the caster the gathering sparks' depth is clamped. */
#define CASTING_IMPACT_SPARK_DEPTH	  60
/* The impact pictures' particles, in the map cell buffer after the Djinni and
   Stun Jip's flame band. */
#define CASTING_IMPACT_IMPACT_PARTICLES \
	((PARTICLE *)(Ram_MapMetatileBuffer + 24576))
#define CASTING_IMPACT_IMPACT_PARTICLE_COUNT 64
/* A spark scattered over the target's life at the least, counted down two a
   frame (the draw visits each spark twice); it is drawn a mote size smaller for
   every CASTING_IMPACT_SPARK_SHRINK of life lost. */
#define CASTING_IMPACT_SPARK_LIFE			 32
#define CASTING_IMPACT_SPARK_SHRINK			 20

/* Asura's sprite sheet: the masked swordsman with his sword low, then swung up,
   then the red crescent his stroke leaves. */
#define ASURA_SHEET_LOW_WIDTH		80
#define ASURA_SHEET_LOW_HEIGHT		96
#define ASURA_SHEET_SWUNG_WIDTH		80
#define ASURA_SHEET_SWUNG_HEIGHT	104
#define ASURA_SHEET_CRESCENT_WIDTH	128
#define ASURA_SHEET_CRESCENT_HEIGHT 91
#define ASURA_SHEET_SWUNG			PICTURE_BYTES(ASURA_SHEET_LOW)
#define ASURA_SHEET_CRESCENT \
	(ASURA_SHEET_SWUNG + PICTURE_BYTES(ASURA_SHEET_SWUNG))

/* RedCrescentSheetB, Asura's crescent fading in four pictures the width of the
   canvas. */
#define ASURA_CRESCENT_FADE_A_HEIGHT 91
#define ASURA_CRESCENT_FADE_B_HEIGHT 59
#define ASURA_CRESCENT_FADE_C_HEIGHT 29
#define ASURA_CRESCENT_FADE_D_HEIGHT 26
#define ASURA_CRESCENT_FADE_B		 (CANVAS_SIZE * ASURA_CRESCENT_FADE_A_HEIGHT)
#define ASURA_CRESCENT_FADE_C \
	(ASURA_CRESCENT_FADE_B + CANVAS_SIZE * ASURA_CRESCENT_FADE_B_HEIGHT)
#define ASURA_CRESCENT_FADE_D \
	(ASURA_CRESCENT_FADE_C + CANVAS_SIZE * ASURA_CRESCENT_FADE_C_HEIGHT)

/* Sarcophagus's sprite sheet: the coffin, then seven pictures of the ghost
   rising out of it, one after another. */
#define SARCOPHAGUS_COFFIN_WIDTH   69
#define SARCOPHAGUS_COFFIN_HEIGHT  96
#define SARCOPHAGUS_GHOST_A		   PICTURE_BYTES(SARCOPHAGUS_COFFIN)
#define SARCOPHAGUS_GHOST_A_WIDTH  59
#define SARCOPHAGUS_GHOST_A_HEIGHT 24
#define SARCOPHAGUS_GHOST_B \
	(SARCOPHAGUS_GHOST_A + PICTURE_BYTES(SARCOPHAGUS_GHOST_A))
#define SARCOPHAGUS_GHOST_B_WIDTH  70
#define SARCOPHAGUS_GHOST_B_HEIGHT 27
#define SARCOPHAGUS_GHOST_C \
	(SARCOPHAGUS_GHOST_B + PICTURE_BYTES(SARCOPHAGUS_GHOST_B))
#define SARCOPHAGUS_GHOST_C_WIDTH  84
#define SARCOPHAGUS_GHOST_C_HEIGHT 55
#define SARCOPHAGUS_GHOST_D \
	(SARCOPHAGUS_GHOST_C + PICTURE_BYTES(SARCOPHAGUS_GHOST_C))
#define SARCOPHAGUS_GHOST_D_WIDTH  75
#define SARCOPHAGUS_GHOST_D_HEIGHT 57
#define SARCOPHAGUS_GHOST_E \
	(SARCOPHAGUS_GHOST_D + PICTURE_BYTES(SARCOPHAGUS_GHOST_D))
#define SARCOPHAGUS_GHOST_E_WIDTH  67
#define SARCOPHAGUS_GHOST_E_HEIGHT 86
#define SARCOPHAGUS_GHOST_F \
	(SARCOPHAGUS_GHOST_E + PICTURE_BYTES(SARCOPHAGUS_GHOST_E))
#define SARCOPHAGUS_GHOST_F_WIDTH  50
#define SARCOPHAGUS_GHOST_F_HEIGHT 65
#define SARCOPHAGUS_GHOST_G \
	(SARCOPHAGUS_GHOST_F + PICTURE_BYTES(SARCOPHAGUS_GHOST_F))
#define SARCOPHAGUS_GHOST_G_WIDTH  48
#define SARCOPHAGUS_GHOST_G_HEIGHT 29

/* Terra Strike's rock spire at its tallest. */
#define ROCK_SPIRE_WIDTH		  32
#define ROCK_SPIRE_HEIGHT		  96
/* The flame Forcible Arm and Truncheon Fist drop. */
#define FALLING_FLAME_WIDTH		  32
#define FALLING_FLAME_HEIGHT	  64
/* CloudPuffSheet: Chill's four puffs of cloud, each fainter than the last. */
#define CHILL_PUFF_WIDTH		  32
#define CHILL_PUFF_HEIGHT		  64
#define CHILL_PUFFS				  24 /* puffs Chill sends up */
/* Bad Omen's skulls rise faster by the frame, the slowest by toFIXED(1) / 32
   more a frame, written as a shift of 1, and each of the next three twice as
   fast as the one before. */
#define BAD_OMEN_SKULL_LIFT_SHIFT 11
/* The level the canvas flashes to, three times, in Flash Edge. */
#define FLASH_EDGE_FLASH_LEVEL	  47
/* How far Stun Jip's ring of fire and bands of flame and Armor Crush's rings of
   billows lean back. */
#define CASTING_IMPACT_RING_TILT  1000

/* ImpFaceSheet's 32x32 face, and the seven dimmer copies Wheeze makes after
   it. */
#define IMP_FACE_WIDTH			 32
#define IMP_FACE_HEIGHT			 32
#define IMP_FACE_FADES			 8
/* The Stun Bolt's four bolts, LightningBoltSheet's pictures centred in 32x128
   textures after the tornado's glints. */
#define STUN_BOLT_TEXTURE_WIDTH	 32
#define STUN_BOLT_TEXTURE_HEIGHT 128
#define STUN_BOLT_TEXTURES		 GLINT_SET_BYTES
#define STUN_BOLT_PICTURES		 4
/* Hammersphere's ball: a 48x48 shaded circle mirrored into a 48x96 picture. */
#define HAMMERSPHERE_BALL		 2048
#define HAMMERSPHERE_BALL_WIDTH	 48
#define HAMMERSPHERE_BALL_HEIGHT 96

/* Where SmokeSheet is decoded in the effect's pictures. */
#define CASTING_IMPACT_SMOKE_PICTURES		6153
/* Where FlashBurstSheet is decoded in the effect's pictures. */
#define CASTING_IMPACT_FLASH_BURST_PICTURES 3456
/* Where RuneSheet is decoded in the effect's pictures. */
#define CASTING_IMPACT_RUNE_PICTURES		4096
/* Where BoulderPicture is decoded in the effect's pictures: Mother Earth's
   boulder, 64 pixels square. */
#define CASTING_IMPACT_BOULDER_PICTURES		8192
#define BOULDER_PICTURE_SIZE				64
/* The shards Mother Earth's boulder breaks into. */
#define MOTHER_EARTH_SHARDS					32
/* Stun Jip's flame band, after the 128x128 fire ring. */
#define CASTING_IMPACT_FLAME_BAND			(128 * 128)

/* The impact particles that carry the rising pictures: flash bursts, skulls or
   puffs of smoke. */
#define CASTING_IMPACT_RISING_PICTURES 16
/* The frames a rising picture lives, and a flash burst, two to each picture. */
#define CASTING_IMPACT_RISING_LIFE	   24
#define CASTING_IMPACT_BURST_LIFE	   12
/* Power Drive's fireballs, and the frames each flies. */
#define POWER_DRIVE_FIREBALLS		   16
#define POWER_DRIVE_FIREBALL_LIFE	   24
/* Terrible Bite's flash bursts; Dinox's are CASTING_IMPACT_RISING_PICTURES. */
#define TERRIBLE_BITE_BURSTS		   24
/* Raiden's Wrath's pictures after its first, and Apocalypse's. */
#define RAIDENS_WRATH_PICTURES		   10
#define APOCALYPSE_PICTURES			   7
/* Raiden's Wrath's first picture, the warrior with his sword; Apocalypse's, the
   woman in green with her ribbons. */
#define RAIDENS_WRATH_WARRIOR_WIDTH	   96
#define RAIDENS_WRATH_WARRIOR_HEIGHT   88
#define APOCALYPSE_WOMAN_WIDTH		   60
#define APOCALYPSE_WOMAN_HEIGHT		   95
/* The rows of Sarcophagus's coffin dissolving at once, the lowest the most. */
#define SARCOPHAGUS_DISSOLVE_ROWS	   34
/* Broil's ember streaks, and the motes in each streak's trail of Crucible Fire
   and Supernova. */
#define BROIL_STREAKS				   12
#define CASTING_IMPACT_TRAIL_MOTES	   12
/* Stun Bolt's bolts, each turning about the target: STUN_BOLT_COUNT are given a
   spin and a picture, and the first STUN_BOLT_STRIKES strike, one starting each
   frame. */
#define STUN_BOLT_BOLTS				   ((PARTICLE *)(Ram_MapMetatileBuffer + 28160))
#define STUN_BOLT_COUNT				   32
#define STUN_BOLT_STRIKES			   20

/* Gathers sparks around the caster, launches the Djinni and draws the impact on
   the first target.

   Abilities: Bane; Smog; Mist; Fever; Flint; Scorch; Squall; Sap; Echo; Sour;
   Chill; Gel; Steel; Fog; Wheeze; Blitz; Terra Strike; Poison Cloud; Poison
   Death; Mortal Danger, Death Scythe; Bad Omen; Life Nourish; Aqua Sock; Frost
   Bite; Life Leech; Psy Leech; Broil; Meltdown; Heat Mirage, Heat Flash;
   Barrage; Acid Bath; Stun Voltage; Blinding Smog; Murk; Cyclone Slash; Psyphon
   Seal; Asura; Forcible Arm, Grand Golem; Truncheon Fist; Power Drive; Heat
   Smash; Stun Cloud; Searing Fog; Ice Crush; Sargasso; Evil Eye; Vein Tap;
   Scorpionfish; Moon Air; Flash Edge; High Vitals; Flare Burst; Hammersphere;
   Flash Force; Crucible Fire; Reverse Star; Supernova; Stun Bolt; Sarcophagus;
   Raiden's Wrath; Apocalypse; Flash Punch; Armor Crush; Stun Jip; Quick Slash;
   Bosca Hit; Mother Earth; Roc, Orc; Wild Wolf, Fatal Fang; Dinox, Power Bite;
   Terrible Bite; Call Zombie. */
void btEffectRunCastingImpact(BATTLE_EFFECT_PARM *action, Sint32 kind)
{
	Uint8				*pictures;
	PARTICLE			*impactParticles;
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 frame;
	BATTLE_CAMERA		*camera;
	Uint8				*sprites;
	FIXED				 scrollPos;
	Sint32				 scrollSpeed;
	Sint32				 duration;
	SCREEN_POINT		 casterPos;
	SCREEN_POINT		 targetPos;
	Sint32				 djinnPos[3];
	Sint32				 djinnStep[3];
	SCREEN_POINT		 sparkScreen;
	SCREEN_POINT		 projected;
	SCREEN_POINT		 targetPoint;
	BLITTER_PAIR		 blitters;
	MODEL_TEXTURE		 stunBoltTexture;
	MODEL_TEXTURE		 reverseStarTexture;
	MODEL_TEXTURE		 boscaHitTexture;
	MODEL_TEXTURE		 stunJipTexture;
	MODEL_TEXTURE		 armorCrushTexture;
	Sint32				 i;
	Sint32				 v;
	Sint32				*vec;

	pictures = Ram_MapMetatileBuffer;
	impactParticles = CASTING_IMPACT_IMPACT_PARTICLES;
	state = Ram_HeapSlots->battleEff;
	canvas = Ram_HeapSlots->battleCanvas;
	camera = Ram_HeapSlots->cameraState;
	sprites = Ram_HeapSlots->battleSheet;
	scrollPos = 0;
	scrollSpeed = 0;

	if(kind == CASTING_IMPACT_IMPACT_MORTAL_DANGER ||
		kind == CASTING_IMPACT_IMPACT_TERRA_STRIKE ||
		kind == CASTING_IMPACT_IMPACT_ASURA ||
		kind == CASTING_IMPACT_IMPACT_SARCOPHAGUS ||
		kind == CASTING_IMPACT_IMPACT_RAIDENS_WRATH ||
		kind == CASTING_IMPACT_IMPACT_UNUSED_TILED_CANVAS ||
		kind == CASTING_IMPACT_IMPACT_APOCALYPSE)
		btEffectStartTiledCanvas(BGCNT_WRAP);
	else
		btEffectStartCanvasLayer(0);

	REG_BLDALPHA = EFFECT_BLEND_ADD;
	/* Setup: the pictures. */
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprites, 0, 0);
	btResourceLoadAndDecompress(RESOURCE_FIRE_STREAK_SHEET, state->sheet, 1, 0);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_ORB_SHEET, pictures, 1, 0);
	btEffectPackYellowOrbs(pictures, state->sheet + CASTING_IMPACT_ORB_PICTURES,
		YELLOW_ORB_WIDTH, YELLOW_ORB_PICTURES * YELLOW_ORB_HEIGHT);

	if(kind == CASTING_IMPACT_IMPACT_SCORCH ||
		kind == CASTING_IMPACT_IMPACT_BARRAGE ||
		kind == CASTING_IMPACT_IMPACT_FLARE_BURST) {
		btResourceLoadAndDecompress(RESOURCE_FLASH_BURST_SHEET, pictures, 1, 0);
	} else if(kind == CASTING_IMPACT_IMPACT_BAD_OMEN ||
		kind == CASTING_IMPACT_IMPACT_UNUSED_SKULLS) {
		btResourceLoadAndDecompress(RESOURCE_SKULL_SHEET, pictures, 1, 0);
	} else if(kind == CASTING_IMPACT_IMPACT_WHEEZE) {
		Sint32 j;
		Sint32 v;
		Uint8 *src;

		btResourceLoadAndDecompress(RESOURCE_IMP_FACE_SHEET, pictures, 1, 0);
		for(i = 1; i != IMP_FACE_FADES; i++) {
			Sint32 fade;

			j = 0;
			fade = i * 8;
			src = pictures;
			for(; j != PICTURE_BYTES(IMP_FACE); j++) {
				v = *src++;
				if(v == 0) {
					pictures[j + i * PICTURE_BYTES(IMP_FACE)] = 0;
					continue;
				}
				v -= fade;
				if(v <= 0)
					v = 1;
				pictures[j + i * PICTURE_BYTES(IMP_FACE)] = v;
			}
		}
	} else if(kind == CASTING_IMPACT_IMPACT_CHILL) {
		btResourceLoadAndDecompress(RESOURCE_CLOUD_PUFF_SHEET, pictures, 1, 0);
	} else if(kind == CASTING_IMPACT_IMPACT_SQUALL ||
		kind == CASTING_IMPACT_IMPACT_STUN_VOLTAGE ||
		kind == CASTING_IMPACT_IMPACT_UNUSED_BOLTS ||
		kind == CASTING_IMPACT_IMPACT_BLITZ) {
		btResourceLoadAndDecompress(RESOURCE_TORNADO_SHEET, pictures, 1, 0);
		btResourceLoadAndDecompress(
			RESOURCE_LIGHTNING_BOLT_SHEET, pictures + GLINT_SET_BYTES, 1, 0);
	} else if(kind == CASTING_IMPACT_IMPACT_STUN_CLOUD) {
		btResourceLoadAndDecompress(RESOURCE_TORNADO_SHEET, pictures, 1, 0);
		btResourceLoadAndDecompress(RESOURCE_SMOKE_SHEET,
			pictures + CASTING_IMPACT_SMOKE_PICTURES, 1, 0);
	} else if(kind == CASTING_IMPACT_IMPACT_PSYPHON_SEAL) {
		btResourceLoadAndDecompress(RESOURCE_RUNE_SHEET, pictures, 1, 1);
	} else if(kind == CASTING_IMPACT_IMPACT_TERRA_STRIKE) {
		btResourceLoadAndDecompress(RESOURCE_ROCK_SPIRE_SHEET, pictures, 1, 1);
	} else if(kind == CASTING_IMPACT_IMPACT_AQUA_SOCK) {
		btResourceLoadAndDecompress(RESOURCE_BLUE_FLAME_SHEET, pictures, 1, 0);
	} else if(kind == CASTING_IMPACT_IMPACT_CYCLONE_SLASH) {
		btResourceLoadAndDecompress(RESOURCE_TORNADO_SHEET, pictures, 1, 0);
	} else if(kind == CASTING_IMPACT_IMPACT_FROST_BITE) {
		btResourceLoadAndDecompress(RESOURCE_ICE_CHIP_SHEET, pictures, 1, 0);
	} else if(kind == CASTING_IMPACT_IMPACT_BROIL) {
		btResourceLoadAndDecompress(
			RESOURCE_EMBER_STREAK_SHEET, pictures, 1, 0);
	} else if(kind == CASTING_IMPACT_IMPACT_ARMOR_CRUSH) {
		btResourceLoadAndDecompress(RESOURCE_BILLOW_TEXTURE, pictures, 0, 0);
	} else if(kind == CASTING_IMPACT_IMPACT_STUN_JIP) {
		btResourceLoadAndDecompress(RESOURCE_FLAME_BAND_TEXTURE,
			pictures + CASTING_IMPACT_FLAME_BAND, 0, 0);
	} else if(kind == CASTING_IMPACT_IMPACT_STUN_BOLT) {
		Sint32 j;
		Sint32 k;

		btResourceLoadAndDecompress(RESOURCE_TORNADO_SHEET, pictures, 1, 0);
		btResourceLoadAndDecompress(
			RESOURCE_LIGHTNING_BOLT_SHEET, state->sheet, 1, 1);
		syWordsClear(pictures + STUN_BOLT_TEXTURES,
			STUN_BOLT_PICTURES * PICTURE_BYTES(STUN_BOLT_TEXTURE));
		for(i = 0; i != STUN_BOLT_PICTURES; i++) {
			for(j = 0; j != LIGHTNING_BOLT_HEIGHT; j++) {
				for(k = 0; k != LIGHTNING_BOLT_WIDTH; k++)
					pictures[STUN_BOLT_TEXTURES +
						(STUN_BOLT_TEXTURE_WIDTH - LIGHTNING_BOLT_WIDTH) / 2 +
						i * PICTURE_BYTES(STUN_BOLT_TEXTURE) +
						j * STUN_BOLT_TEXTURE_WIDTH + k] =
						state->sheet[i * LIGHTNING_BOLT_PICTURE_BYTES +
							j * LIGHTNING_BOLT_WIDTH + k];
			}
		}
		btResourceLoadAndDecompress(
			RESOURCE_FIRE_STREAK_SHEET, state->sheet, 1, 0);
	} else if(kind == CASTING_IMPACT_IMPACT_FORCIBLE_ARM ||
		kind == CASTING_IMPACT_IMPACT_TRUNCHEON_FIST) {
		btResourceLoadAndDecompress(RESOURCE_FLAME_SHEET_A, pictures, 1, 0);
	} else if(kind == CASTING_IMPACT_IMPACT_SARGASSO ||
		kind == CASTING_IMPACT_IMPACT_REVERSE_STAR) {
		btResourceLoadAndDecompress(RESOURCE_SWIRL_TEXTURE, pictures, 0, 0);
	} else if(kind == CASTING_IMPACT_IMPACT_WILD_WOLF ||
		kind == CASTING_IMPACT_IMPACT_DINOX ||
		kind == CASTING_IMPACT_IMPACT_TERRIBLE_BITE) {
		btResourceLoadAndDecompress(
			RESOURCE_BLUE_DROPLET_SHEET, pictures, 0, 0);
		btResourceLoadAndDecompress(RESOURCE_FLASH_BURST_SHEET,
			pictures + CASTING_IMPACT_FLASH_BURST_PICTURES, 1, 0);
	} else if(kind == CASTING_IMPACT_IMPACT_FLASH_FORCE) {
		Sint32 k;
		Sint32 i;

		btResourceLoadAndDecompress(
			RESOURCE_RUNE_SHEET, pictures + CASTING_IMPACT_RUNE_PICTURES, 1, 0);
		syWordsClear(pictures, RUNE_TEXTURE_SIZE * RUNE_TEXTURE_SIZE);
		for(i = 0; i != RUNE_HALF_HEIGHT; i++) {
			for(k = 0; k != RUNE_HALF_WIDTH; k++) {
				pictures[RUNE_TEXTURE_RUNE + i * RUNE_TEXTURE_SIZE + k] =
					pictures[CASTING_IMPACT_RUNE_PICTURES +
						i * RUNE_HALF_WIDTH + k];
				pictures[RUNE_TEXTURE_RUNE + 2 * RUNE_HALF_WIDTH - 1 +
					i * RUNE_TEXTURE_SIZE - k] =
					pictures[CASTING_IMPACT_RUNE_PICTURES +
						i * RUNE_HALF_WIDTH + k];
			}
		}
	} else if(kind == CASTING_IMPACT_IMPACT_HAMMERSPHERE) {
		Sint32 i;
		Sint32 k;

		btEffectDrawShadedCircles(
			pictures, HAMMERSPHERE_BALL_WIDTH, toFIXED(1));
		for(i = 0; i != HAMMERSPHERE_BALL_HEIGHT / 2; i++) {
			for(k = 0; k != HAMMERSPHERE_BALL_WIDTH / 2; k++) {
				pictures[HAMMERSPHERE_BALL + i * HAMMERSPHERE_BALL_WIDTH + k] =
					pictures[i * (HAMMERSPHERE_BALL_WIDTH / 2) + k];
				pictures[HAMMERSPHERE_BALL + i * HAMMERSPHERE_BALL_WIDTH +
					HAMMERSPHERE_BALL_WIDTH - 1 - k] =
					pictures[i * (HAMMERSPHERE_BALL_WIDTH / 2) + k];
				pictures[HAMMERSPHERE_BALL +
					(HAMMERSPHERE_BALL_HEIGHT - 1 - i) *
						HAMMERSPHERE_BALL_WIDTH +
					k] = pictures[i * (HAMMERSPHERE_BALL_WIDTH / 2) + k];
				pictures[HAMMERSPHERE_BALL +
					(HAMMERSPHERE_BALL_HEIGHT - 1 - i) *
						HAMMERSPHERE_BALL_WIDTH +
					HAMMERSPHERE_BALL_WIDTH - 1 - k] =
					pictures[i * (HAMMERSPHERE_BALL_WIDTH / 2) + k];
			}
		}
	} else if(kind == CASTING_IMPACT_IMPACT_MOTHER_EARTH) {
		btResourceLoadAndDecompress(RESOURCE_BOULDER_SHEET, pictures, 1, 1);
		btResourceLoadAndDecompress(RESOURCE_BOULDER_PICTURE,
			pictures + CASTING_IMPACT_BOULDER_PICTURES, 1, 1);
	} else if(kind == CASTING_IMPACT_IMPACT_VEIN_TAP) {
		REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, 12);
	} else if(kind == CASTING_IMPACT_IMPACT_ICE_CRUSH) {
		btResourceLoadAndDecompress(RESOURCE_CRYSTAL_SHEET, pictures, 1, 0);
	} else if(kind == CASTING_IMPACT_IMPACT_POWER_DRIVE) {
		btResourceLoadAndDecompress(RESOURCE_FIREBALL_SHEET, pictures, 1, 1);
	} else if(kind != CASTING_IMPACT_IMPACT_MORTAL_DANGER &&
		kind != CASTING_IMPACT_IMPACT_ASURA) {
		btResourceLoadAndDecompress(RESOURCE_SMOKE_SHEET, pictures, 1, 0);
	}

	/* The small Djinni of the impact's element when a Djinni's entrance came
	   first (action->prelude), else only its sprite sheet's colours. */
	switch(kind) {
	case CASTING_IMPACT_IMPACT_BANE:
	case CASTING_IMPACT_IMPACT_FLINT:
	case CASTING_IMPACT_IMPACT_SAP:
	case CASTING_IMPACT_IMPACT_ECHO:
	case CASTING_IMPACT_IMPACT_STEEL:
	case CASTING_IMPACT_IMPACT_TERRA_STRIKE:
	case CASTING_IMPACT_IMPACT_POISON_CLOUD:
	case CASTING_IMPACT_IMPACT_POISON_DEATH:
	case CASTING_IMPACT_IMPACT_MORTAL_DANGER:
	case CASTING_IMPACT_IMPACT_BAD_OMEN:
	case CASTING_IMPACT_IMPACT_LIFE_NOURISH:
	case CASTING_IMPACT_IMPACT_FORCIBLE_ARM:
	case CASTING_IMPACT_IMPACT_EVIL_EYE:
	case CASTING_IMPACT_IMPACT_VEIN_TAP:
	case CASTING_IMPACT_IMPACT_HAMMERSPHERE:
	case CASTING_IMPACT_IMPACT_SARCOPHAGUS:
	case CASTING_IMPACT_IMPACT_MOTHER_EARTH:
	case CASTING_IMPACT_IMPACT_DINOX:
	case CASTING_IMPACT_IMPACT_TERRIBLE_BITE:
		syDMAXCopy(syResourceGetTableEntry(RESOURCE_BLAST_SHEET),
			(void *)BG_PLTT, DMA_COPY_WORDS(SHEET_PALETTE_BYTES / 4), REG_DMA3);
		if(action->prelude == 1)
			btResourceLoadAndDecompress(RESOURCE_VENUS_DJINN_SMALL_SHEET,
				pictures + CASTING_IMPACT_DJINNI_PICTURE, 1, 0);
		break;
	case CASTING_IMPACT_IMPACT_MIST:
	case CASTING_IMPACT_IMPACT_SOUR:
	case CASTING_IMPACT_IMPACT_CHILL:
	case CASTING_IMPACT_IMPACT_GEL:
	case CASTING_IMPACT_IMPACT_FOG:
	case CASTING_IMPACT_IMPACT_AQUA_SOCK:
	case CASTING_IMPACT_IMPACT_UNUSED_ICE_CHIPS:
	case CASTING_IMPACT_IMPACT_FROST_BITE:
	case CASTING_IMPACT_IMPACT_UNUSED_GATHERING:
	case CASTING_IMPACT_IMPACT_LIFE_LEECH:
	case CASTING_IMPACT_IMPACT_PSY_LEECH:
	case CASTING_IMPACT_IMPACT_STUN_CLOUD:
	case CASTING_IMPACT_IMPACT_SEARING_FOG:
	case CASTING_IMPACT_IMPACT_ICE_CRUSH:
	case CASTING_IMPACT_IMPACT_SARGASSO:
	case CASTING_IMPACT_IMPACT_FLASH_FORCE:
	case CASTING_IMPACT_IMPACT_REVERSE_STAR:
	case CASTING_IMPACT_IMPACT_CALL_ZOMBIE:
		if(action->prelude == 1)
			btResourceLoadAndDecompress(RESOURCE_MERCURY_DJINN_SMALL_SHEET,
				pictures + CASTING_IMPACT_DJINNI_PICTURE, 1, 1);
		else
			syDMAXCopy(
				syResourceGetTableEntry(RESOURCE_MERCURY_DJINN_SMALL_SHEET),
				(void *)BG_PLTT, DMA_COPY_WORDS(SHEET_PALETTE_BYTES / 4),
				REG_DMA3);
		break;
	case CASTING_IMPACT_IMPACT_FEVER:
	case CASTING_IMPACT_IMPACT_SCORCH:
	case CASTING_IMPACT_IMPACT_UNUSED_BOLTS:
	case CASTING_IMPACT_IMPACT_UNUSED_SKULLS:
	case CASTING_IMPACT_IMPACT_BROIL:
	case CASTING_IMPACT_IMPACT_MELTDOWN:
	case CASTING_IMPACT_IMPACT_HEAT_MIRAGE:
	case CASTING_IMPACT_IMPACT_BARRAGE:
	case CASTING_IMPACT_IMPACT_UNUSED_MYSTIC_FLAME:
	case CASTING_IMPACT_IMPACT_ACID_BATH:
	case CASTING_IMPACT_IMPACT_TRUNCHEON_FIST:
	case CASTING_IMPACT_IMPACT_UNUSED_SEVERE_BLOW:
	case CASTING_IMPACT_IMPACT_POWER_DRIVE:
	case CASTING_IMPACT_IMPACT_HEAT_SMASH:
	case CASTING_IMPACT_IMPACT_SCORPIONFISH:
	case CASTING_IMPACT_IMPACT_FLARE_BURST:
	case CASTING_IMPACT_IMPACT_CRUCIBLE_FIRE:
	case CASTING_IMPACT_IMPACT_ARMOR_CRUSH:
	case CASTING_IMPACT_IMPACT_STUN_JIP:
		if(action->prelude == 1)
			btResourceLoadAndDecompress(RESOURCE_MARS_DJINN_SMALL_SHEET,
				pictures + CASTING_IMPACT_DJINNI_PICTURE, 1, 1);
		else
			syDMAXCopy(syResourceGetTableEntry(RESOURCE_MARS_DJINN_SMALL_SHEET),
				(void *)BG_PLTT, DMA_COPY_WORDS(SHEET_PALETTE_BYTES / 4),
				REG_DMA3);
		break;
	case CASTING_IMPACT_IMPACT_SMOG:
	case CASTING_IMPACT_IMPACT_SQUALL:
	case CASTING_IMPACT_IMPACT_WHEEZE:
	case CASTING_IMPACT_IMPACT_BLITZ:
	case CASTING_IMPACT_IMPACT_UNUSED_COMETS:
	case CASTING_IMPACT_IMPACT_STUN_VOLTAGE:
	case CASTING_IMPACT_IMPACT_BLINDING_SMOG:
	case CASTING_IMPACT_IMPACT_MURK:
	case CASTING_IMPACT_IMPACT_CYCLONE_SLASH:
	case CASTING_IMPACT_IMPACT_PSYPHON_SEAL:
	case CASTING_IMPACT_IMPACT_ASURA:
	case CASTING_IMPACT_IMPACT_MOON_AIR:
	case CASTING_IMPACT_IMPACT_FLASH_EDGE:
	case CASTING_IMPACT_IMPACT_HIGH_VITALS:
	case CASTING_IMPACT_IMPACT_SUPERNOVA:
	case CASTING_IMPACT_IMPACT_STUN_BOLT:
	case CASTING_IMPACT_IMPACT_RAIDENS_WRATH:
	case CASTING_IMPACT_IMPACT_APOCALYPSE:
	case CASTING_IMPACT_IMPACT_FLASH_PUNCH:
	case CASTING_IMPACT_IMPACT_QUICK_SLASH:
		if(action->prelude == 1)
			btResourceLoadAndDecompress(RESOURCE_JUPITER_DJINN_SMALL_SHEET,
				pictures + CASTING_IMPACT_DJINNI_PICTURE, 1, 1);
		else
			syDMAXCopy(
				syResourceGetTableEntry(RESOURCE_JUPITER_DJINN_SMALL_SHEET),
				(void *)BG_PLTT, DMA_COPY_WORDS(SHEET_PALETTE_BYTES / 4),
				REG_DMA3);
		break;
	case CASTING_IMPACT_IMPACT_BOSCA_HIT:
	case CASTING_IMPACT_IMPACT_ROC:
	case CASTING_IMPACT_IMPACT_WILD_WOLF:
		syDMAXCopy(syResourceGetTableEntry(RESOURCE_DUST_PUFF_SHEET),
			(void *)BG_PLTT, DMA_COPY_WORDS(SHEET_PALETTE_BYTES / 4), REG_DMA3);
		break;
	case CASTING_IMPACT_IMPACT_UNUSED_MERCURY_DJINNI:
		btResourceLoadAndDecompress(RESOURCE_MERCURY_DJINN_SMALL_SHEET,
			pictures + CASTING_IMPACT_DJINNI_PICTURE, 1, 1);
		break;
	}

	/* The rings of fire. */
	if(kind == CASTING_IMPACT_IMPACT_CRUCIBLE_FIRE ||
		kind == CASTING_IMPACT_IMPACT_SUPERNOVA ||
		kind == CASTING_IMPACT_IMPACT_STUN_JIP ||
		kind == CASTING_IMPACT_IMPACT_QUICK_SLASH ||
		kind == CASTING_IMPACT_IMPACT_BOSCA_HIT)
		btResourceLoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE, pictures, 0, 0);
	/* Trails: Bad Omen's last longer than the others'. */
	state->transferMode = CANVAS_COPY_AND_FADE;
	if(kind == CASTING_IMPACT_IMPACT_BAD_OMEN)
		state->transferValue = CANVAS_FADE_THREE_QUARTERS;
	else
		state->transferValue = CANVAS_FADE_HALF;
	sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);
	btEffectPosProjectScaledBattler(action->tgts[0], &targetPos);
	btEffectPosProjectScaledBattler(action->user, &casterPos);
	btEffectBuildCanvasBlitters(action->side, blitters);
	{
		MOTION_OBJECT *caster;
		Sint32		   savedVelocityX;
		Sint32		   savedVelocityY;
		Sint32		   savedVelocityZ;
		Sint32		   savedAcceleration;
		Sint32		   savedVerticalStrength;
		MOTION_OBJECT *targetObject;
		Sint32		   n;
		Sint32		   m;
		Sint32		   t;
		Sint32		   r;
		Sint32		   x;
		Sint32		   y;
		Sint32		   h;
		Sint32		   count;
		Sint32		   start;
		Sint32		   half;
		FIXED		   height;
		Sint32		   angle;
		Sint32		   columnTop;
		Sint32		   scroll;
		Sint32		   life;
		Sint32		   row;
		Sint32		   bursts;
		Sint32		   fade;
		Sint32		   scale;
		void		  *matrix;
		RENDER_MODEL  *model;
		const Uint8	  *table;
		Sint32		   k;
		PARTICLE	  *p;

		state->fadeFrm = BACKGROUND_DARKEN_FRAMES;
		state->fadeStep = 0;
		caster = btObjectGetStub(action->user)->obj;
		for(i = 0; i != CASTING_IMPACT_SPARKS; i++) {
			state->particles[i].x = (syRandom() & 63) + 32;
			state->particles[i].y = 0;
			state->particles[i].z = 0;
			state->particles[i].velX = syRandom() & TRIG_ANGLE_MASK;
			state->particles[i].velY = syRandom() & TRIG_ANGLE_MASK;
			state->particles[i].velZ = syRandom() & TRIG_ANGLE_MASK;
		}
		/* Stop the caster and place the Djinni overhead. */
		fdActorSetAnimationSpeedStub(caster, 0);
		/* FAKEMATCH(dead): vec set to djinnPos, a store nothing reads, before
		 * djinnPos is filled. */
		vec = djinnPos;
		djinnPos[0] = caster->x;
		djinnPos[1] = caster->y + toFIXED(80);
		djinnPos[2] = caster->z;
		savedVelocityX = caster->velX;
		savedVelocityY = caster->velY;
		savedVelocityZ = caster->velZ;
		savedAcceleration = caster->acceleration;
		savedVerticalStrength = caster->verticalMotionStrength;
		caster->velX = 0;
		caster->velY = 0;
		caster->velZ = 0;
		caster->acceleration = 0;
		caster->verticalMotionStrength = 0;
		btEffectPosProjectScaledBattler(action->user, &casterPos);
		casterPos.x = casterPos.x / 2;
		sySndPlayStub(SE_POWER_UP);

		/* Phase 1, the gathering (CASTING_IMPACT_GATHER_FRAMES). */
		for(frame = 0; frame != CASTING_IMPACT_GATHER_FRAMES; frame++) {
			PARTICLE *p;
			Sint32	  n;

			count = 0;
			p = state->particles;
			for(i = 0; i != CASTING_IMPACT_SPARKS; i++, p++) {
				if(p->x >= 0) {
					if(frame >= i / 4) {
						n = 5;
						syInitMatrix();
						syRotZ(p->velZ);
						syRotX(p->velX);
						syRotY(p->velY);
						btEffectPosProjectPoint(p, &sparkScreen);
						sparkScreen.x = sparkScreen.x / 2 + casterPos.x;
						/* Djinni kinds circle the caster's chest; the rest
						   circle lower, the unused Severe Blow lowest. */
						if(kind < CASTING_IMPACT_DJINN_KINDS)
							sparkScreen.y = sparkScreen.y + casterPos.y - 8;
						else if(kind ==
							CASTING_IMPACT_IMPACT_UNUSED_SEVERE_BLOW)
							sparkScreen.y = sparkScreen.y + casterPos.y + 44;
						else
							sparkScreen.y = sparkScreen.y + casterPos.y + 12;
						/* DEAD: the depth is clamped either side of the caster
						   and moved to start at 0, but never read; the ROM
						   keeps the clamp and the move. */
						CLAMP(sparkScreen.depth, -CASTING_IMPACT_SPARK_DEPTH,
							CASTING_IMPACT_SPARK_DEPTH);
						sparkScreen.depth += CASTING_IMPACT_SPARK_DEPTH;
						EFFECT_DRAW_MOTE(blitters[1], canvas, sprites,
							sparkScreen.x, sparkScreen.y, n);
						p->x -= 4;
					}
					count++;
				}
			}
			if(kind < CASTING_IMPACT_DJINN_KINDS &&
				count < CASTING_IMPACT_SPARKS) {
				syInitMatrix();
				syLookAtLoad(camera->eye, camera->focus);
				btEffectPosProjectPoint(djinnPos, &sparkScreen);
				sparkScreen.x = sparkScreen.x / 2;
				blitters[0](canvas, pictures + CASTING_IMPACT_DJINNI_PICTURE,
					sparkScreen.x - CASTING_IMPACT_DJINNI_WIDTH / 2,
					sparkScreen.y - 4, CASTING_IMPACT_DJINNI_WIDTH,
					CASTING_IMPACT_DJINNI_HEIGHT);
				/* FAKEMATCH(dead): vec set to djinnPos again, a store nothing
				 * reads. */
				vec = djinnPos;
			}
			EFFECT_PRESENT_CANVAS(state);
		}

		/* Phase 2: place BG2 so the impact stands over the target. */
		if(kind == CASTING_IMPACT_IMPACT_MORTAL_DANGER) {
			REG_BG2PA = Q_8_8(1);
			if(action->side == EFFECT_SIDE_ENEMIES)
				REG_BG2X = (32 - targetPos.x) << 8;
			else
				REG_BG2X = (96 - targetPos.x) << 8;
		} else if(kind == CASTING_IMPACT_IMPACT_SARCOPHAGUS) {
			REG_BG2PA = Q_8_8(1);
			if(action->side == EFFECT_SIDE_ENEMIES)
				REG_BG2X = (CANVAS_SIZE / 2 - targetPos.x) << 8;
			else
				REG_BG2X = (CANVAS_SIZE / 2 - targetPos.x) << 8;
		} else if(kind == CASTING_IMPACT_IMPACT_APOCALYPSE) {
			REG_BG2PA = Q_8_8(1);
			if(action->side == EFFECT_SIDE_ENEMIES) {
				scrollPos = -toFIXED(64);
				scrollSpeed = toFIXED(2);
			} else {
				scrollPos = -toFIXED(56);
				scrollSpeed = -toFIXED(2);
			}
			REG_BG2X = (FIXEDtoINT(scrollPos)) << 8;
		} else if(kind == CASTING_IMPACT_IMPACT_ASURA ||
			kind == CASTING_IMPACT_IMPACT_RAIDENS_WRATH) {
			REG_BG2PA = Q_8_8(1);
			if(action->side == EFFECT_SIDE_ENEMIES) {
				scrollPos = -toFIXED(128);
				scrollSpeed = toFIXED(12);
			} else {
				scrollPos = 0;
				scrollSpeed = -toFIXED(12);
			}
			REG_BG2X = (FIXEDtoINT(scrollPos)) << 8;
		}
		if(kind == CASTING_IMPACT_IMPACT_TERRA_STRIKE) {
			REG_BG2PA = Q_8_8(1);
			REG_BG2X = (CANVAS_SIZE / 2 - targetPos.x) << 8;
			EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
			syWordsClear((void *)BG_CHAR_BLOCK(1), CANVAS_VIEW_BYTES);
			syWordsClear(canvas, CANVAS_VIEW_BYTES);
			REG_BLDCNT = 0;
		}
		if(kind == CASTING_IMPACT_IMPACT_PSYPHON_SEAL) {
			REG_BG2PA = Q_8_8(1);
			if(action->side == EFFECT_SIDE_ENEMIES)
				REG_BG2X = (32 - targetPos.x) << 8;
			else
				REG_BG2X = (96 - targetPos.x) << 8;
		}
		/* These four unused impacts (ice chips, gathering, Mystic Flame,
		   comets) clear the canvas and hand off to another effect without
		   giving the caster its motion back: the ice chips to Blizzard's
		   volley, the Mystic Flame one to Mystic Flame's orbs and the comets to
		   PROJECTILE_VOLLEY_IMPACT_COMETS; the gathering ends here. */
		if(kind == CASTING_IMPACT_IMPACT_UNUSED_ICE_CHIPS ||
			kind == CASTING_IMPACT_IMPACT_UNUSED_GATHERING ||
			kind == CASTING_IMPACT_IMPACT_UNUSED_MYSTIC_FLAME ||
			kind == CASTING_IMPACT_IMPACT_UNUSED_COMETS) {
			syWordsClear((void *)BG_CHAR_BLOCK(1), CANVAS_VIEW_BYTES);
			syWordsClear(canvas, CANVAS_VIEW_BYTES);
			action->prelude = 0;
			syCloseEvent(btEffectDarkenBackgroundStep);
			syCloseEvent(btEffectTransferCanvas);
			EFFECT_RELEASE_BLITTERS();
			fdActorSelectAnimationStub(caster, POSE_CAST);
			if(kind == CASTING_IMPACT_IMPACT_UNUSED_ICE_CHIPS)
				btEffectRunProjectileVolley(action, PROJECTILE_VOLLEY_BLIZZARD);
			if(kind == CASTING_IMPACT_IMPACT_UNUSED_MYSTIC_FLAME)
				btEffectRunMysticFlame(action);
			if(kind == CASTING_IMPACT_IMPACT_UNUSED_COMETS)
				btEffectRunProjectileVolley(
					action, PROJECTILE_VOLLEY_IMPACT_COMETS);
			return;
		}
		fdActorSetAnimationSpeedStub(caster, ANIM_STEP_NORMAL);
		caster->velX = savedVelocityX;
		caster->velY = savedVelocityY;
		caster->velZ = savedVelocityZ;
		caster->acceleration = savedAcceleration;
		caster->verticalMotionStrength = savedVerticalStrength;
		/* Unused: The Broken Seal's Severe Blow, with the caster's motion back,
		   clears the canvas and hands off to btEffectRunSparkGroups' rising
		   sparks. */
		if(kind == CASTING_IMPACT_IMPACT_UNUSED_SEVERE_BLOW) {
			syWordsClear((void *)BG_CHAR_BLOCK(1), CANVAS_VIEW_BYTES);
			syWordsClear(canvas, CANVAS_VIEW_BYTES);
			action->prelude = 0;
			syCloseEvent(btEffectDarkenBackgroundStep);
			syCloseEvent(btEffectTransferCanvas);
			EFFECT_RELEASE_BLITTERS();
			action->variant = SPARK_GROUPS_SEVERE_BLOW;
			btEffectRunSparkGroups(action, SPARK_GROUPS_RISING);
			return;
		}
		syWordsClear(canvas, CANVAS_VIEW_BYTES);
		syWordsClear((void *)BG_CHAR_BLOCK(1), CANVAS_VIEW_BYTES);
		/* Hammersphere's colours 1 to 63 are greys, from black to white. */
		if(kind == CASTING_IMPACT_IMPACT_HAMMERSPHERE) {
			for(i = 1; i != SHEET_PALETTE_COLORS; i++) {
				v = RGB_LEVEL_MAX - (FIXEDtoINT(syCos(i << 8) << 5));
				BG_PLTT[i] = RGB(v, v, v);
			}
		}
		/* The Djinni flies to the target in six steps. */
		targetObject = btObjectGetStub(action->tgts[0])->obj;
		djinnStep[0] = (targetObject->x - djinnPos[0]) / 6;
		djinnStep[1] = (targetObject->y - djinnPos[1] + toFIXED(30)) / 6;
		djinnStep[2] = (targetObject->z - djinnPos[2]) / 6;
		for(i = 0; i != CASTING_IMPACT_SPARKS; i++)
			state->particles[i].variant = 0;
		/* Sparks scatter from the target. */
		if(kind != CASTING_IMPACT_IMPACT_AQUA_SOCK) {
			half = btUnitGetEffectHeightStub(action->tgts[0]) / 2;
			i = 0;
			p = state->particles;
			for(; i != CASTING_IMPACT_SPARKS / 2; i++, p++) {
				SET_POSITION(p, targetObject->x, half, targetObject->z);
				if(kind == CASTING_IMPACT_IMPACT_PSYPHON_SEAL) {
					p->velX = ((syRandom() & 255) - 127) << 12;
					p->velY = ((syRandom() & 255) - 64) << 10;
					p->velZ = ((syRandom() & 255) - 127) << 12;
				} else {
					p->velX = ((syRandom() & 255) - 127) << 12;
					p->velY = ((syRandom() & 255) - 64) << 12;
					p->velZ = ((syRandom() & 255) - 127) << 12;
				}
				p->velX = p->velX / 2;
				p->velY = p->velY / 2;
				p->velZ = p->velZ / 2;
				p->variant = i / 2 + CASTING_IMPACT_SPARK_LIFE;
			}
		}
		/* Mortal Danger, Raiden's Wrath, Apocalypse, Asura and Sarcophagus load
		   their own pictures. */
		if(kind == CASTING_IMPACT_IMPACT_MORTAL_DANGER) {
			btResourceLoadAndDecompress(
				RESOURCE_COUNTER_REVEAL_SHEET, state->sheet, 1, 1);
			btResourceLoadAndDecompress(
				RESOURCE_RED_CRESCENT_SHEET_A, pictures, 1, 0);
			REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, 14);
		}
		if(kind == CASTING_IMPACT_IMPACT_RAIDENS_WRATH) {
			btResourceLoadAndDecompress(
				RESOURCE_RAIDENS_WRATH_SHEET, pictures, 1, 1);
			REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, 8);
		}
		if(kind == CASTING_IMPACT_IMPACT_APOCALYPSE) {
			btResourceLoadAndDecompress(
				RESOURCE_APOCALYPSE_SHEET, pictures, 1, 1);
			REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, 8);
		}
		if(kind == CASTING_IMPACT_IMPACT_ASURA) {
			btResourceLoadAndDecompress(
				RESOURCE_BLUE_BEAST_SHEET, state->sheet, 1, 1);
			btResourceLoadAndDecompress(
				RESOURCE_RED_CRESCENT_SHEET_B, pictures, 1, 0);
			REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, 14);
		}
		if(kind == CASTING_IMPACT_IMPACT_SARCOPHAGUS) {
			btResourceLoadAndDecompress(
				RESOURCE_SARCOPHAGUS_SHEET, pictures, 1, 1);
			REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, 14);
		}

		/* The impact's own particles fly out from the target, Power Drive's to
		   the sides; the kinds listed first set theirs later or have none. */
		if(kind == CASTING_IMPACT_IMPACT_SAP ||
			kind == CASTING_IMPACT_IMPACT_LIFE_NOURISH ||
			kind == CASTING_IMPACT_IMPACT_LIFE_LEECH ||
			kind == CASTING_IMPACT_IMPACT_VEIN_TAP ||
			kind == CASTING_IMPACT_IMPACT_HIGH_VITALS ||
			kind == CASTING_IMPACT_IMPACT_FLARE_BURST ||
			kind == CASTING_IMPACT_IMPACT_MORTAL_DANGER ||
			kind == CASTING_IMPACT_IMPACT_ASURA ||
			kind == CASTING_IMPACT_IMPACT_PSY_LEECH ||
			kind == CASTING_IMPACT_IMPACT_STEEL ||
			kind == CASTING_IMPACT_IMPACT_SUPERNOVA ||
			kind == CASTING_IMPACT_IMPACT_STUN_BOLT ||
			kind == CASTING_IMPACT_IMPACT_SARCOPHAGUS ||
			kind == CASTING_IMPACT_IMPACT_RAIDENS_WRATH ||
			kind == CASTING_IMPACT_IMPACT_APOCALYPSE) {
		} else if(kind == CASTING_IMPACT_IMPACT_POWER_DRIVE) {
			for(i = 0; i != CASTING_IMPACT_IMPACT_PARTICLE_COUNT; i++) {
				PARTICLE *fireball;

				fireball = &impactParticles[i];
				SET_POSITION(
					fireball, targetObject->x, toFIXED(20), targetObject->z);
				if(targetObject->x < 0)
					fireball->velX = -((syRandom() & 255) + 128) << 11;
				else
					fireball->velX = ((syRandom() & 255) + 128) << 11;
				fireball->velY = ((syRandom() & 255) - 127) << 10;
				fireball->velZ = 0;
				fireball->variant = 0;
			}
		} else {
			PARTICLE *p;

			if(kind == CASTING_IMPACT_IMPACT_BAD_OMEN ||
				kind == CASTING_IMPACT_IMPACT_UNUSED_SKULLS)
				height = 0;
			else
				height = toFIXED(20);
			p = impactParticles;
			for(i = 0; i != CASTING_IMPACT_IMPACT_PARTICLE_COUNT; i++, p++) {
				SET_POSITION(p, targetObject->x, height, targetObject->z);
				if(kind == CASTING_IMPACT_IMPACT_SCORCH ||
					kind == CASTING_IMPACT_IMPACT_BARRAGE) {
					p->velX = ((syRandom() & 255) - 127) << 11;
					p->velY = (syRandom() & 255) << 11;
					p->velZ = ((syRandom() & 255) - 127) << 11;
				} else if(kind == CASTING_IMPACT_IMPACT_STUN_CLOUD ||
					kind == CASTING_IMPACT_IMPACT_SEARING_FOG ||
					kind == CASTING_IMPACT_IMPACT_SCORPIONFISH ||
					kind == CASTING_IMPACT_IMPACT_MOON_AIR ||
					kind == CASTING_IMPACT_IMPACT_WHEEZE ||
					kind == CASTING_IMPACT_IMPACT_DINOX ||
					kind == CASTING_IMPACT_IMPACT_CHILL) {
					p->velX = ((syRandom() & 255) - 127) << 13;
					p->velY = ((syRandom() & 255) - 127) << 12;
					p->velZ = ((syRandom() & 255) - 127) << 13;
				} else if(kind == CASTING_IMPACT_IMPACT_TERRIBLE_BITE) {
					p->velX = ((syRandom() & 255) - 127) << 14;
					p->velY = ((syRandom() & 255) - 127) << 13;
					p->velZ = ((syRandom() & 255) - 127) << 14;
				} else if(kind == CASTING_IMPACT_IMPACT_ACID_BATH) {
					p->velX = ((syRandom() & 255) - 127) << 11;
					p->velY = (syRandom() & 127) << 10;
					p->velZ = ((syRandom() & 255) - 127) << 11;
				} else {
					p->velX = ((syRandom() & 255) - 127) << 10;
					p->velY = (syRandom() & 127) << 10;
					p->velZ = ((syRandom() & 255) - 127) << 10;
				}
				p->velX = p->velX / 4;
				p->velY = p->velY / 4;
				p->velZ = p->velZ / 4;
				p->variant = 0;
			}
		}

		/* These wave BG2 a line at a time while they play. */
		if(kind == CASTING_IMPACT_IMPACT_MIST ||
			kind == CASTING_IMPACT_IMPACT_FEVER ||
			kind == CASTING_IMPACT_IMPACT_BAD_OMEN ||
			kind == CASTING_IMPACT_IMPACT_UNUSED_SKULLS ||
			kind == CASTING_IMPACT_IMPACT_HEAT_MIRAGE ||
			kind == CASTING_IMPACT_IMPACT_EVIL_EYE ||
			kind == CASTING_IMPACT_IMPACT_MURK ||
			kind == CASTING_IMPACT_IMPACT_BLINDING_SMOG ||
			kind == CASTING_IMPACT_IMPACT_SEARING_FOG ||
			kind == CASTING_IMPACT_IMPACT_FLASH_PUNCH)
			sySetEvent(btEffectArmBg2AffineHBlankDMA, EVENT_PASS_VBLANK);

		/* How long each impact plays, in frames. */
		if(kind == CASTING_IMPACT_IMPACT_FLINT ||
			kind == CASTING_IMPACT_IMPACT_SCORCH ||
			kind == CASTING_IMPACT_IMPACT_SQUALL ||
			kind == CASTING_IMPACT_IMPACT_BLITZ ||
			kind == CASTING_IMPACT_IMPACT_BARRAGE ||
			kind == CASTING_IMPACT_IMPACT_CYCLONE_SLASH ||
			kind == CASTING_IMPACT_IMPACT_STUN_VOLTAGE ||
			kind == CASTING_IMPACT_IMPACT_UNUSED_BOLTS ||
			kind == CASTING_IMPACT_IMPACT_FORCIBLE_ARM ||
			kind == CASTING_IMPACT_IMPACT_TRUNCHEON_FIST ||
			kind == CASTING_IMPACT_IMPACT_UNUSED_MERCURY_DJINNI ||
			kind == CASTING_IMPACT_IMPACT_SUPERNOVA ||
			kind == CASTING_IMPACT_IMPACT_ARMOR_CRUSH ||
			kind == CASTING_IMPACT_IMPACT_STUN_JIP ||
			kind == CASTING_IMPACT_IMPACT_FLASH_EDGE ||
			kind == CASTING_IMPACT_IMPACT_POWER_DRIVE ||
			kind == CASTING_IMPACT_IMPACT_QUICK_SLASH ||
			kind == CASTING_IMPACT_IMPACT_BOSCA_HIT ||
			kind == CASTING_IMPACT_IMPACT_WILD_WOLF ||
			kind == CASTING_IMPACT_IMPACT_DINOX)
			duration = 32;
		else if(kind == CASTING_IMPACT_IMPACT_BANE ||
			kind == CASTING_IMPACT_IMPACT_SMOG ||
			kind == CASTING_IMPACT_IMPACT_MIST ||
			kind == CASTING_IMPACT_IMPACT_FEVER ||
			kind == CASTING_IMPACT_IMPACT_FOG ||
			kind == CASTING_IMPACT_IMPACT_TERRA_STRIKE ||
			kind == CASTING_IMPACT_IMPACT_POISON_CLOUD ||
			kind == CASTING_IMPACT_IMPACT_SCORPIONFISH ||
			kind == CASTING_IMPACT_IMPACT_MOON_AIR ||
			kind == CASTING_IMPACT_IMPACT_POISON_DEATH ||
			kind == CASTING_IMPACT_IMPACT_HEAT_MIRAGE ||
			kind == CASTING_IMPACT_IMPACT_EVIL_EYE ||
			kind == CASTING_IMPACT_IMPACT_SEARING_FOG ||
			kind == CASTING_IMPACT_IMPACT_ACID_BATH ||
			kind == CASTING_IMPACT_IMPACT_MURK ||
			kind == CASTING_IMPACT_IMPACT_PSYPHON_SEAL ||
			kind == CASTING_IMPACT_IMPACT_AQUA_SOCK ||
			kind == CASTING_IMPACT_IMPACT_ICE_CRUSH ||
			kind == CASTING_IMPACT_IMPACT_RAIDENS_WRATH ||
			kind == CASTING_IMPACT_IMPACT_APOCALYPSE ||
			kind == CASTING_IMPACT_IMPACT_FLASH_PUNCH ||
			kind == CASTING_IMPACT_IMPACT_WHEEZE ||
			kind == CASTING_IMPACT_IMPACT_CHILL)
			duration = 48;
		else if(kind == CASTING_IMPACT_IMPACT_MELTDOWN ||
			kind == CASTING_IMPACT_IMPACT_HEAT_SMASH ||
			kind == CASTING_IMPACT_IMPACT_SOUR ||
			kind == CASTING_IMPACT_IMPACT_GEL ||
			kind == CASTING_IMPACT_IMPACT_ROC ||
			kind == CASTING_IMPACT_IMPACT_CALL_ZOMBIE)
			duration = 20;
		else if(kind == CASTING_IMPACT_IMPACT_MORTAL_DANGER ||
			kind == CASTING_IMPACT_IMPACT_ASURA ||
			kind == CASTING_IMPACT_IMPACT_BROIL ||
			kind == CASTING_IMPACT_IMPACT_CRUCIBLE_FIRE ||
			kind == CASTING_IMPACT_IMPACT_STUN_BOLT ||
			kind == CASTING_IMPACT_IMPACT_STUN_JIP ||
			kind == CASTING_IMPACT_IMPACT_HAMMERSPHERE ||
			kind == CASTING_IMPACT_IMPACT_TERRIBLE_BITE)
			duration = 40;
		else if(kind == CASTING_IMPACT_IMPACT_SARCOPHAGUS)
			duration = 72;
		else if(kind == CASTING_IMPACT_IMPACT_BLINDING_SMOG ||
			kind == CASTING_IMPACT_IMPACT_BAD_OMEN ||
			kind == CASTING_IMPACT_IMPACT_UNUSED_SKULLS ||
			kind == CASTING_IMPACT_IMPACT_STUN_CLOUD)
			duration = 64;
		else if(kind == CASTING_IMPACT_IMPACT_ECHO ||
			kind == CASTING_IMPACT_IMPACT_SARGASSO ||
			kind == CASTING_IMPACT_IMPACT_FLASH_FORCE ||
			kind == CASTING_IMPACT_IMPACT_MOTHER_EARTH)
			duration = 54;
		else
			duration = 80;

		/* Phase 3, the impact, for duration frames. */
		for(frame = 0; frame != duration; frame++) {
			PARTICLE *p;
			PARTICLE *particle;

			if(kind != CASTING_IMPACT_IMPACT_MORTAL_DANGER &&
				kind != CASTING_IMPACT_IMPACT_ASURA &&
				kind != CASTING_IMPACT_IMPACT_SUPERNOVA &&
				kind != CASTING_IMPACT_IMPACT_SARCOPHAGUS &&
				kind != CASTING_IMPACT_IMPACT_RAIDENS_WRATH &&
				kind != CASTING_IMPACT_IMPACT_APOCALYPSE) {
				/* The wave: each line sways 0 to 2 canvas pixels sideways. */
				Sint32 *scanline;

				scanline = state->bg2xLines;
				for(i = 0; i != DISPLAY_HEIGHT; i++) {
					Sint32 phase =
						frame * (TRIG_TURN / 16) + i * (TRIG_TURN / 32);

					*scanline++ =
						CANVAS_WAVE_BG2X(toFIXED(4) - sySin(phase) * 4);
				}
			}
			/* Step the background palette towards the impact's colours. */
			switch(kind) {
			case CASTING_IMPACT_IMPACT_CHILL:
				btEffectStepPaletteToResource(RESOURCE_CLOUD_PUFF_SHEET);
				break;
			case CASTING_IMPACT_IMPACT_POWER_DRIVE:
				btEffectStepPaletteToResource(RESOURCE_FIREBALL_SHEET);
				break;
			case CASTING_IMPACT_IMPACT_SEARING_FOG:
				btEffectStepPaletteToResource(RESOURCE_GREY_PALETTE_A);
				break;
			case CASTING_IMPACT_IMPACT_FORCIBLE_ARM:
				btEffectStepPaletteToResource(RESOURCE_FLAME_SHEET_A);
				break;
			case CASTING_IMPACT_IMPACT_AQUA_SOCK:
			case CASTING_IMPACT_IMPACT_STUN_CLOUD:
				btEffectStepPaletteToResource(RESOURCE_BLUE_FLAME_SHEET);
				break;
			case CASTING_IMPACT_IMPACT_PSYPHON_SEAL:
				btEffectStepPaletteToResource(RESOURCE_RUNE_SHEET);
				break;
			case CASTING_IMPACT_IMPACT_TERRA_STRIKE:
				btEffectStepPaletteToResource(RESOURCE_ROCK_SPIRE_SHEET);
				break;
			case CASTING_IMPACT_IMPACT_BANE:
			case CASTING_IMPACT_IMPACT_POISON_DEATH:
			case CASTING_IMPACT_IMPACT_SCORPIONFISH:
				btEffectStepPaletteToResource(RESOURCE_MARS_DJINN_SHEET);
				break;
			case CASTING_IMPACT_IMPACT_BAD_OMEN:
			case CASTING_IMPACT_IMPACT_LIFE_NOURISH:
			case CASTING_IMPACT_IMPACT_ACID_BATH:
				btEffectStepPaletteToResource(RESOURCE_PINK_BURST_SHEET);
				break;
			case CASTING_IMPACT_IMPACT_LIFE_LEECH:
			case CASTING_IMPACT_IMPACT_HIGH_VITALS:
				btEffectStepPaletteToResource(RESOURCE_ICE_BLOCK_SHEET);
				break;
			case CASTING_IMPACT_IMPACT_PSY_LEECH:
				btEffectStepPaletteToResource(RESOURCE_BLAST_SHEET);
				break;
			case CASTING_IMPACT_IMPACT_MIST:
			case CASTING_IMPACT_IMPACT_MURK:
			case CASTING_IMPACT_IMPACT_MOON_AIR:
			case CASTING_IMPACT_IMPACT_REVERSE_STAR:
			case CASTING_IMPACT_IMPACT_STUN_JIP:
				btEffectStepPaletteToResource(RESOURCE_TAN_PALETTE);
				break;
			case CASTING_IMPACT_IMPACT_SMOG:
			case CASTING_IMPACT_IMPACT_FOG:
			case CASTING_IMPACT_IMPACT_BLINDING_SMOG:
			case CASTING_IMPACT_IMPACT_FLASH_PUNCH:
				btEffectStepPaletteToResource(RESOURCE_CYAN_PALETTE);
				break;
			case CASTING_IMPACT_IMPACT_FEVER:
			case CASTING_IMPACT_IMPACT_BROIL:
			case CASTING_IMPACT_IMPACT_HEAT_MIRAGE:
			case CASTING_IMPACT_IMPACT_FLARE_BURST:
				btEffectStepPaletteToResource(RESOURCE_EMBER_STREAK_SHEET);
				break;
			case CASTING_IMPACT_IMPACT_POISON_CLOUD:
			case CASTING_IMPACT_IMPACT_SUPERNOVA:
				btEffectStepPaletteToResource(RESOURCE_LIME_PALETTE);
				break;
			case CASTING_IMPACT_IMPACT_SCORCH:
			case CASTING_IMPACT_IMPACT_BARRAGE:
				btEffectStepPaletteToResource(RESOURCE_FLASH_BURST_SHEET);
				break;
			case CASTING_IMPACT_IMPACT_CRUCIBLE_FIRE:
			case CASTING_IMPACT_IMPACT_ARMOR_CRUSH:
			case CASTING_IMPACT_IMPACT_TERRIBLE_BITE:
				btEffectStepPaletteToResource(RESOURCE_ORANGE_PALETTE_B);
				break;
			case CASTING_IMPACT_IMPACT_VEIN_TAP:
				btEffectStepPaletteToResource(RESOURCE_RED_PALETTE_B);
				break;
			case CASTING_IMPACT_IMPACT_STUN_BOLT:
				btEffectStepPaletteToResource(RESOURCE_VIOLET_PALETTE_G);
				break;
			}
			/* Follow the caster for the first six frames. */
			if(frame < 6) {
				btEffectPosProjectScaledBattler(action->user, &casterPos);
				casterPos.x = casterPos.x / 2;
				casterPos.y += 8;
			}
			/* The streak shoots from the caster. */
			if(kind != CASTING_IMPACT_IMPACT_MORTAL_DANGER &&
				kind != CASTING_IMPACT_IMPACT_TERRA_STRIKE &&
				kind != CASTING_IMPACT_IMPACT_ASURA &&
				kind != CASTING_IMPACT_IMPACT_FORCIBLE_ARM &&
				kind != CASTING_IMPACT_IMPACT_TRUNCHEON_FIST &&
				kind != CASTING_IMPACT_IMPACT_HAMMERSPHERE &&
				kind != CASTING_IMPACT_IMPACT_ICE_CRUSH &&
				kind != CASTING_IMPACT_IMPACT_REVERSE_STAR &&
				kind != CASTING_IMPACT_IMPACT_SARCOPHAGUS &&
				kind != CASTING_IMPACT_IMPACT_RAIDENS_WRATH &&
				kind != CASTING_IMPACT_IMPACT_APOCALYPSE &&
				kind != CASTING_IMPACT_IMPACT_FLASH_PUNCH &&
				kind != CASTING_IMPACT_IMPACT_MOTHER_EARTH &&
				kind != CASTING_IMPACT_IMPACT_WILD_WOLF && frame < 12) {
				if(action->side == EFFECT_SIDE_ENEMIES)
					blitters[0](canvas,
						state->sheet + frame / 2 * PICTURE_BYTES(FIRE_STREAK),
						casterPos.x - 28, casterPos.y - 40, FIRE_STREAK_WIDTH,
						FIRE_STREAK_HEIGHT);
				else
					blitters[0](canvas,
						state->sheet + frame / 2 * PICTURE_BYTES(FIRE_STREAK),
						casterPos.x - 20, casterPos.y - 40, FIRE_STREAK_WIDTH,
						FIRE_STREAK_HEIGHT);
			}
			if(kind != CASTING_IMPACT_IMPACT_MORTAL_DANGER &&
				kind != CASTING_IMPACT_IMPACT_TERRA_STRIKE &&
				kind != CASTING_IMPACT_IMPACT_ASURA &&
				kind != CASTING_IMPACT_IMPACT_SUPERNOVA &&
				kind != CASTING_IMPACT_IMPACT_SARCOPHAGUS &&
				kind != CASTING_IMPACT_IMPACT_RAIDENS_WRATH &&
				kind != CASTING_IMPACT_IMPACT_APOCALYPSE &&
				kind != CASTING_IMPACT_IMPACT_MOTHER_EARTH) {
				/* The orb flashes over the target and sparks scatter. */
				if(frame >= 4 && frame < 16)
					blitters[1](canvas,
						state->sheet + CASTING_IMPACT_ORB_PICTURES +
							((frame - 4) / 2) *
								PICTURE_BYTES(YELLOW_ORB_PACKED),
						targetPos.x / 2 - 8, casterPos.y - 24,
						YELLOW_ORB_PACKED_WIDTH, YELLOW_ORB_PACKED_HEIGHT);
				syInitMatrix();
				syLookAtLoad(camera->eye, camera->focus);
				if(frame > 3) {
					if(kind == CASTING_IMPACT_IMPACT_HAMMERSPHERE) {
						PARTICLE *spark;

						for(i = 0; i != CASTING_IMPACT_SPARKS; i++) {
							spark = &state->particles[i];
							if(spark->variant > 0) {
								Sint32 n;

								n = (spark->variant >> 2) + 1;
								if(n > MOTE_PICTURES)
									n = MOTE_PICTURES;
								btEffectPosProjectPoint(spark, &projected);
								projected.x = projected.x / 2;
								EFFECT_DRAW_MOTE(blitters[0], canvas, sprites,
									projected.x, projected.y, n);
								/* DEAD: only Hammersphere reaches this arm, so
								   the Flash Punch test never holds; the ROM
								   keeps the compare and both arms. */
								{
									FIXED gravity;

									if(kind ==
										CASTING_IMPACT_IMPACT_FLASH_PUNCH)
										gravity = 0;
									else
										gravity = -toFIXED(1) / 32;
									btParticleAdvanceWithGravity3D(
										spark, 62, gravity);
								}
								spark->variant -= 2;
							}
						}
					} else {
						for(i = 0; i != CASTING_IMPACT_SPARKS; i++) {
							p = &state->particles[i / 2];
							if(p->variant > 0) {
								Sint32 n;

								n = p->variant / CASTING_IMPACT_SPARK_SHRINK +
									2;
								btEffectPosProjectPoint(p, &projected);
								projected.x = projected.x / 2;
								EFFECT_DRAW_MOTE(blitters[(i / 2) & 1], canvas,
									sprites, projected.x, projected.y, n);
								btParticleAdvanceWithGravity3D(
									p, 62, -toFIXED(1) / 32);
								p->variant--;
							}
						}
					}
				}
			}

			/* Motes burst from the target and fly back into the caster, who
			   glows white: Flare Burst's at once and twice as fast. */
			if(kind == CASTING_IMPACT_IMPACT_SAP ||
				kind == CASTING_IMPACT_IMPACT_LIFE_NOURISH ||
				kind == CASTING_IMPACT_IMPACT_LIFE_LEECH ||
				kind == CASTING_IMPACT_IMPACT_VEIN_TAP ||
				kind == CASTING_IMPACT_IMPACT_HIGH_VITALS ||
				kind == CASTING_IMPACT_IMPACT_FLARE_BURST ||
				kind == CASTING_IMPACT_IMPACT_PSY_LEECH ||
				kind == CASTING_IMPACT_IMPACT_STEEL ||
				kind == CASTING_IMPACT_IMPACT_REVERSE_STAR) {
				Sint32		   burstFrame;
				MOTION_OBJECT *casterObject;
				Sint32		   half;
				Sint32		   dx;
				Sint32		   dy;
				Sint32		   dz;
				Sint32		   slot;

				burstFrame = 12;
				if(frame == 12)
					sySndPlayStub(SE_SWIRL);
				if(kind == CASTING_IMPACT_IMPACT_FLARE_BURST)
					burstFrame = 0;
				if(frame == 50)
					btEffectTintBattler(action->user, SPRITE_COLOR_WHITE,
						TINT_KEEP, TINT_KEEP, 0);
				if(frame == 79)
					btEffectTintBattler(action->user, SPRITE_COLOR_NORMAL,
						TINT_KEEP, TINT_KEEP, 0);
				if(frame == burstFrame) {
					PARTICLE *p;

					p = impactParticles;
					for(i = 0; i != CASTING_IMPACT_IMPACT_PARTICLE_COUNT;
						i++, p++) {
						SET_POSITION(
							p, targetObject->x, toFIXED(20), targetObject->z);
						p->velX = ((syRandom() & 255) - 128) << 10;
						p->velY = ((syRandom() & 255) - 128) << 10;
						p->velZ = ((syRandom() & 255) - 128) << 10;
						if(kind == CASTING_IMPACT_IMPACT_FLARE_BURST) {
							p->velX <<= 1;
							p->velY <<= 1;
							p->velZ <<= 1;
						}
						p->variant = 0;
					}
				}
				if(frame >= burstFrame) {
					p = impactParticles;
					casterObject = btObjectGetStub(action->user)->obj;
					half = btUnitGetEffectHeightStub(action->user) / 2;
					slot = kind == CASTING_IMPACT_IMPACT_FLARE_BURST;
					for(i = 0; i != CASTING_IMPACT_IMPACT_PARTICLE_COUNT / 2;
						i++, p++) {
						if(p->variant >= 0) {
							n = (i & 1) + 6;
							btEffectPosProjectPoint(p, &projected);
							projected.x = projected.x >> 1;
							EFFECT_DRAW_MOTE(blitters[slot], canvas, sprites,
								projected.x, projected.y, n);
							btParticleAdvanceWithGravity3D(p, 62, 0);
							if(frame > i + 22) {
								dx = (casterObject->x - p->x) >> 8;
								dy = (casterObject->y + half - p->y) >> 8;
								dz = (casterObject->z - p->z) >> 8;
								p->velX += dx;
								p->velY += dy;
								p->velZ += dz;
								if((dx > -4096 && dx < 4096) &&
									(dz > -4096 && dz < 4096))
									p->variant = -1;
							}
						}
					}
				}
			}

			/* The impact's own effect. */
			if(kind == CASTING_IMPACT_IMPACT_SAP ||
				kind == CASTING_IMPACT_IMPACT_LIFE_NOURISH ||
				kind == CASTING_IMPACT_IMPACT_LIFE_LEECH ||
				kind == CASTING_IMPACT_IMPACT_VEIN_TAP ||
				kind == CASTING_IMPACT_IMPACT_HIGH_VITALS ||
				kind == CASTING_IMPACT_IMPACT_PSY_LEECH ||
				kind == CASTING_IMPACT_IMPACT_STEEL) {
				/* Their motes are drawn above. */
			} else if(kind == CASTING_IMPACT_IMPACT_POWER_DRIVE) {
				/* Fireballs fly out to either side. */
				PARTICLE *p;

				p = impactParticles;
				for(i = 0; i != POWER_DRIVE_FIREBALLS; i++, p++) {
					if(frame >= i + 4 &&
						p->variant < POWER_DRIVE_FIREBALL_LIFE) {
						btEffectPosProjectPoint(p, &projected);
						projected.x = projected.x / 2;
						blitters[0](canvas, pictures,
							projected.x - FLAMEBALL_WIDTH / 2,
							projected.y - FLAMEBALL_HEIGHT / 2, FLAMEBALL_WIDTH,
							FLAMEBALL_HEIGHT);
						btParticleAdvanceWithGravity3D(
							p, PARTICLE_DAMPING_NONE, 0);
						p->variant++;
					}
				}
			} else if(kind == CASTING_IMPACT_IMPACT_MELTDOWN ||
				kind == CASTING_IMPACT_IMPACT_HEAT_SMASH ||
				kind == CASTING_IMPACT_IMPACT_SOUR ||
				kind == CASTING_IMPACT_IMPACT_ROC ||
				kind == CASTING_IMPACT_IMPACT_CALL_ZOMBIE) {
				/* Draw nothing. */
			} else if(kind == CASTING_IMPACT_IMPACT_SQUALL ||
				kind == CASTING_IMPACT_IMPACT_STUN_VOLTAGE ||
				kind == CASTING_IMPACT_IMPACT_UNUSED_BOLTS ||
				kind == CASTING_IMPACT_IMPACT_BLITZ) {
				/* Bolts strike among glints: Squall's and Stun Voltage's two
				   over the target, Blitz's four turning about it. */
				Sint32 y;

				if(kind == CASTING_IMPACT_IMPACT_BLITZ) {
					if(frame == 4)
						state->shakeFrm = 12;
					if(frame == 8)
						sySndPlayStub(SE_RISE_WITH_SPARKS);
				}
				if(kind == CASTING_IMPACT_IMPACT_SQUALL ||
					kind == CASTING_IMPACT_IMPACT_STUN_VOLTAGE) {
					if(frame >= 6 && frame < 20) {
						for(i = 0; i != 2; i++)
							blitters[0](canvas,
								pictures + GLINT_SET_BYTES +
									(((frame + i * 3) / 2) & 3) *
										LIGHTNING_BOLT_PICTURE_BYTES,
								targetPos.x / 2 - 8, 0, LIGHTNING_BOLT_WIDTH,
								104);
					}
				} else if(kind == CASTING_IMPACT_IMPACT_BLITZ &&
					(frame >= 6 && frame < 20)) {
					Sint32 x;

					for(i = 0; i != 4; i++) {
						Sint32 m;

						angle = frame * 1848 + i * (DEGtoANG(90));
						m = ((frame + i * 3) / 2) & 3;
						x = targetPos.x / 2 + (FIXEDtoINT(sySin(angle) << 5));
						/* DEAD: the cosine is computed and dropped; the ROM
						   keeps the call. */
						syCos(angle);
						blitters[0](canvas,
							pictures + GLINT_SET_BYTES +
								m * LIGHTNING_BOLT_PICTURE_BYTES,
							x - 8, 0, LIGHTNING_BOLT_WIDTH, 104);
					}
				}
				if(frame >= 8 && frame < 24) {
					Sint32 angle;
					Sint32 centerY;
					Sint32 radiusY;

					if(kind == CASTING_IMPACT_IMPACT_UNUSED_BOLTS) {
						radiusY = 12;
						centerY = 24;
					} else {
						centerY = 0;
						radiusY = 32;
					}
					for(i = 0; i != 3; i++) {
						Sint32 m;
						Sint32 x;
						Uint32 flip;

						m = i & 3;
						angle = syRandom() & TRIG_ANGLE_MASK;
						x = (FIXEDtoINT(sySin(angle) << 3)) + targetPos.x / 2 -
							BattleEffect_GlintPictureWidths[m] / 2;
						y = (FIXEDtoINT(syCos(angle) * radiusY)) + centerY -
							BattleEffect_GlintPictureHeights[m] / 2;
						EFFECT_RELEASE_BLITTERS();
						flip = syRandom();
						EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
							CastingImpact_SquallGlintFlips[flip & 3] |
								EFFECT_CLIP_BOUNDS,
							BLITTER_ADDITIVE);
						ALT_BLITTER(canvas,
							pictures + BattleEffect_GlintPictures[m], x, y + 56,
							BattleEffect_GlintPictureWidths[m],
							BattleEffect_GlintPictureHeights[m]);
						syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
						btEffectBuildCanvasBlitters(action->side, blitters);
					}
				}
			} else if(kind == CASTING_IMPACT_IMPACT_STUN_CLOUD) {
				/* Glints sparkle and puffs of smoke rise. */
				Sint32 angle;

				if(frame >= 8 && frame < 24) {
					Sint32 y;

					for(i = 0; i != 3; i++) {
						Sint32 m;
						Sint32 x;
						Uint32 flip;

						m = i & 3;
						angle = syRandom() & TRIG_ANGLE_MASK;
						x = (FIXEDtoINT(sySin(angle) << 4)) + targetPos.x / 2 -
							BattleEffect_GlintPictureWidths[m] / 2;
						y = (FIXEDtoINT(syCos(angle) << 4)) -
							BattleEffect_GlintPictureHeights[m] / 2;
						EFFECT_RELEASE_BLITTERS();
						flip = syRandom();
						EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
							CastingImpact_SapGlintFlips[flip & 3] |
								EFFECT_CLIP_BOUNDS,
							BLITTER_MAX);
						ALT_BLITTER(canvas,
							pictures + BattleEffect_GlintPictures[m], x, y + 80,
							BattleEffect_GlintPictureWidths[m],
							BattleEffect_GlintPictureHeights[m]);
						syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
						btEffectBuildCanvasBlitters(action->side, blitters);
					}
				}
				p = impactParticles;
				for(i = 0; i != CASTING_IMPACT_RISING_PICTURES; i++, p++) {
					if(frame >= i + 4) {
						Sint32 life;

						life = p->variant;
						if(life < CASTING_IMPACT_RISING_LIFE) {
							Sint32 m;

							m = life / 4;
							btEffectPosProjectPoint(p, &projected);
							projected.x = projected.x / 2;
							blitters[1](canvas,
								pictures + CASTING_IMPACT_SMOKE_PICTURES +
									m * PICTURE_BYTES(SMOKE_PUFF),
								projected.x - SMOKE_PUFF_WIDTH / 2,
								projected.y - SMOKE_PUFF_HEIGHT / 2,
								SMOKE_PUFF_WIDTH, SMOKE_PUFF_HEIGHT);
							btParticleAdvanceWithGravity3D(
								p, 60, toFIXED(1) / 16);
							p->variant++;
						}
					}
				}
				if(frame == 6)
					sySndPlayStub(SE_RISE_WITH_SPARKS);
			} else if(kind == CASTING_IMPACT_IMPACT_AQUA_SOCK) {
				/* A blue column falls onto the target and sparks fly from
				   it. */
				Sint32 count;

				EFFECT_RELEASE_BLITTERS();
				if(frame >= 0 && frame < 24) {
					Sint32 x;

					x = targetPos.x / 2;
					columnTop = frame * 32 - 232;
					scroll = frame * 16 - 48;
					if(columnTop > 0)
						columnTop = 0;
					while(scroll > BLUE_FLAME_COLUMN_HEIGHT)
						scroll -= BLUE_FLAME_COLUMN_HEIGHT;
					EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
						EFFECT_CLIP_BOUNDS, BLITTER_MAX);
					ALT_BLITTER(canvas, pictures, x - 8,
						columnTop + scroll - BLUE_FLAME_COLUMN_HEIGHT,
						BLUE_FLAME_COLUMN_WIDTH, BLUE_FLAME_COLUMN_HEIGHT);
					ALT_BLITTER(canvas, pictures, x - 8, columnTop + scroll,
						BLUE_FLAME_COLUMN_WIDTH,
						BLUE_FLAME_COLUMN_HEIGHT - scroll);
					ALT_BLITTER(canvas,
						pictures + PICTURE_BYTES(BLUE_FLAME_COLUMN), x - 17,
						columnTop + 47, BLUE_FLAME_SPLASH_WIDTH,
						BLUE_FLAME_SPLASH_HEIGHT);
					syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
					if(frame == 8)
						state->shakeFrm = 8;
					if(frame > 1) {
						PARTICLE *p;

						i = 0;
						p = state->particles;
						count = 0;
						for(; i != CASTING_IMPACT_SPARKS; i++, p++) {
							if(p->variant == 0) {
								count++;
								SET_POSITION(p, targetObject->x, toFIXED(20),
									targetObject->z);
								p->velX = ((syRandom() & 255) - 127) << 12;
								p->velY = ((syRandom() & 255) - 64) << 10;
								p->velZ = ((syRandom() & 255) - 127) << 12;
								p->variant = i / 2 + CASTING_IMPACT_SPARK_LIFE;
								if(count == 4)
									break;
							}
						}
					}
				}
				btEffectBuildCanvasBlitters(action->side, blitters);
			} else if(kind == CASTING_IMPACT_IMPACT_PSYPHON_SEAL) {
				/* The rune's two halves, the second mirrored. */
				Sint32 x;

				EFFECT_RELEASE_BLITTERS();
				if(frame >= 4 && frame < 24) {
					Sint32 h;

					x = targetPos.x / 2;
					h = RUNE_HALF_HEIGHT;
					EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
						EFFECT_CLIP_BOUNDS, BLITTER_MAX);
					ALT_BLITTER(canvas, pictures, x - RUNE_HALF_WIDTH, h,
						RUNE_HALF_WIDTH, h);
					syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
					EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
						EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
					ALT_BLITTER(canvas, pictures, x, h, RUNE_HALF_WIDTH, h);
					syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
				}
				btEffectBuildCanvasBlitters(action->side, blitters);
			} else if(kind == CASTING_IMPACT_IMPACT_CYCLONE_SLASH) {
				/* A tornado stands over the target. */
				Sint32 m;

				if(frame > 15)
					REG_BLDALPHA = BLEND_ALPHA(32 - frame, BLEND_WEIGHT_MAX);
				if(frame > 5) {
					Sint32 x;
					Sint32 pictureOffset;

					x = targetPos.x / 2;
					m = frame / 2 % 3;
					x -= TORNADO_STRIP_WIDTH / 2;
					pictureOffset = m * (2 * PICTURE_BYTES(TORNADO_STRIP));
					blitters[0](canvas,
						pictures + GLINT_SET_BYTES + pictureOffset, x, 16,
						TORNADO_STRIP_WIDTH, TORNADO_STRIP_HEIGHT);
					blitters[0](canvas,
						pictures + GLINT_SET_BYTES +
							6 * PICTURE_BYTES(TORNADO_STRIP) +
							m * PICTURE_BYTES(TORNADO_STRIP),
						x, 48, TORNADO_STRIP_WIDTH, TORNADO_STRIP_HEIGHT);
					{
						Uint8 *lowerStrip;

						lowerStrip = pictures + GLINT_SET_BYTES +
							PICTURE_BYTES(TORNADO_STRIP) + pictureOffset;
						blitters[0](canvas, lowerStrip, x, 80,
							TORNADO_STRIP_WIDTH, TORNADO_STRIP_HEIGHT);
					}
				}
			} else if(kind == CASTING_IMPACT_IMPACT_SCORCH ||
				kind == CASTING_IMPACT_IMPACT_BARRAGE ||
				kind == CASTING_IMPACT_IMPACT_FLARE_BURST) {
				/* Flash bursts rise. */
				if(frame < 24) {
					p = impactParticles;
					for(i = 0; i != CASTING_IMPACT_RISING_PICTURES; i++, p++) {
						if(frame >= i / 2 + 4) {
							Sint32 life;

							life = p->variant;
							if(life < CASTING_IMPACT_BURST_LIFE) {
								Sint32 half;

								half = life / 2;
								btEffectPosProjectPoint(p, &projected);
								projected.x = projected.x / 2;
								blitters[0](canvas,
									pictures +
										half * PICTURE_BYTES(FLASH_BURST),
									projected.x - FLASH_BURST_WIDTH / 2,
									projected.y - FLASH_BURST_HEIGHT / 2,
									FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
								btParticleAdvanceWithGravity3D(
									p, 60, toFIXED(1) / 16);
								p->variant++;
							}
						}
					}
				}
			} else if(kind == CASTING_IMPACT_IMPACT_FLINT) {
				/* Draw nothing. */
			} else if(kind == CASTING_IMPACT_IMPACT_ECHO) {
				/* Drawn below. */
			} else if(kind == CASTING_IMPACT_IMPACT_MORTAL_DANGER) {
				/* The reaper swings its scythe and its crescent fades. */
				y = (FIXEDtoINT(targetPos.y)) +
					(FIXEDtoINT(syCos(frame * (TRIG_TURN / 128)) << 2)) + 16;
				if(frame < 4) {
					blitters[0](canvas, state->sheet,
						Reaper_PictureX[action->side * 7],
						y + Reaper_PictureY[0], REAPER_SHEET_RAISED_WIDTH,
						REAPER_SHEET_RAISED_HEIGHT);
				} else {
					if(frame < 8)
						blitters[0](canvas, state->sheet,
							Reaper_PictureX[action->side * 7],
							y + Reaper_PictureY[0], REAPER_SHEET_RAISED_WIDTH,
							REAPER_SHEET_RAISED_HEIGHT);
					EFFECT_DRAW_PICTURE(blitters[0], canvas, state->sheet,
						REAPER_SHEET_SWUNG,
						Reaper_PictureX[action->side * 7 + 1],
						y + Reaper_PictureY[1]);
					if(frame >= 4 && frame < 6)
						syWordsFill(canvas, CANVAS_BYTES,
							CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
					if(frame >= 6 && frame < 8)
						EFFECT_DRAW_PICTURE(blitters[0], canvas, state->sheet,
							REAPER_SHEET_CRESCENT,
							Reaper_PictureX[action->side * 7 + 2],
							y + Reaper_PictureY[2]);
					if(frame >= 8 && frame < 10)
						EFFECT_DRAW_PICTURE(blitters[0], canvas, pictures,
							RED_CRESCENT_FADE_A,
							Reaper_PictureX[action->side * 7 + 3],
							y + Reaper_PictureY[3]);
					if(frame >= 10 && frame < 12)
						EFFECT_DRAW_PICTURE(blitters[0], canvas, pictures,
							RED_CRESCENT_FADE_B,
							Reaper_PictureX[action->side * 7 + 4],
							y + Reaper_PictureY[4]);
					if(frame >= 12 && frame < 14)
						EFFECT_DRAW_PICTURE(blitters[0], canvas, pictures,
							RED_CRESCENT_FADE_C,
							Reaper_PictureX[action->side * 7 + 5],
							y + Reaper_PictureY[5]);
					if(frame >= 14 && frame < 16)
						EFFECT_DRAW_PICTURE(blitters[0], canvas, pictures,
							RED_CRESCENT_FADE_D,
							Reaper_PictureX[action->side * 7 + 6],
							y + Reaper_PictureY[6]);
				}
			} else if(kind == CASTING_IMPACT_IMPACT_SARCOPHAGUS) {
				/* The coffin drops over the target and crumbles from the bottom
				   as the ghost rises out of it. */
				Sint32 h;
				Sint32 y;

				REG_BLDCNT = 0;
				EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
				EFFECT_RELEASE_BLITTERS();
				EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
					BLITTER_TRANSPARENT);
				blitters[0] = Ram_HeapSlots->ramCode;
				if(frame == 0) {
					for(i = 0; i != 16 * SARCOPHAGUS_COFFIN_WIDTH; i++)
						state->sheet[i] = syRandom() & 31;
				}
				y = frame * 28 - SARCOPHAGUS_COFFIN_HEIGHT;
				if(y > 16)
					y = 16;
				/* FAKEMATCH(dead): a store the next line overwrites, under a
				 * test never true. */
				if(y == 300)
					h = 0;
				h = SARCOPHAGUS_COFFIN_HEIGHT;
				if(frame > 7) {
					/* Each row down the band dissolves one level more than the
					   last. */
					Sint32 level;

					i = 0;
					level = 0;
					row = 112 - frame * 2;
					for(; i != SARCOPHAGUS_DISSOLVE_ROWS; i++, level++, row++) {
						if(row >= 0 && row < SARCOPHAGUS_COFFIN_HEIGHT) {
							Sint32 k;
							Sint32 threshold;
							/* FAKEMATCH(dead): threshold set through its own
							 * address; as threshold = level. */
							for(k = 0, *&threshold = level;
								k != SARCOPHAGUS_COFFIN_WIDTH; k++) {
								if(state->sheet[(row & 15) *
										   SARCOPHAGUS_COFFIN_WIDTH +
									   k] < threshold)
									pictures[row * SARCOPHAGUS_COFFIN_WIDTH +
										k] = 0;
							}
						}
					}
				}
				blitters[0](
					canvas, pictures, 29, y, SARCOPHAGUS_COFFIN_WIDTH, h);
				if(frame > 5) {
					switch((frame - 6) / 4) {
					case 0:
						EFFECT_DRAW_PICTURE(blitters[0], canvas, pictures,
							SARCOPHAGUS_GHOST_A, 32, 92);
						break;
					case 1:
						EFFECT_DRAW_PICTURE(blitters[0], canvas, pictures,
							SARCOPHAGUS_GHOST_B, 30, 87);
						break;
					case 2:
						EFFECT_DRAW_PICTURE(blitters[0], canvas, pictures,
							SARCOPHAGUS_GHOST_C, 23, 57);
						break;
					case 3:
						EFFECT_DRAW_PICTURE(blitters[0], canvas, pictures,
							SARCOPHAGUS_GHOST_D, 27, 55);
						break;
					case 4:
						EFFECT_DRAW_PICTURE(blitters[0], canvas, pictures,
							SARCOPHAGUS_GHOST_E, 28, 19);
						break;
					case 5:
						EFFECT_DRAW_PICTURE(blitters[0], canvas, pictures,
							SARCOPHAGUS_GHOST_F, 33, 22);
						break;
					case 6:
						EFFECT_DRAW_PICTURE(blitters[0], canvas, pictures,
							SARCOPHAGUS_GHOST_G, 41, 21);
						break;
					}
				}
				syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
				btEffectBuildCanvasBlitters(action->side, blitters);
			} else if(kind == CASTING_IMPACT_IMPACT_WILD_WOLF ||
				kind == CASTING_IMPACT_IMPACT_DINOX ||
				kind == CASTING_IMPACT_IMPACT_TERRIBLE_BITE) {
				/* Fangs close on the target, each picture drawn beside its
				   mirror; Dinox and Terrible Bite add flash bursts. */
				if(frame < 20) {
					Sint32 m;

					if(frame < 8)
						m = CastingImpact_BitePictures[frame / 2];
					else
						m = 3;
					syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
					{
						Sint32 flags;

						syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
						flags = EFFECT_CLIP_BOUNDS;
						EFFECT_LOAD_BLITTER(
							HEAP_SLOT_RAM_CODE, flags, BLITTER_ADDITIVE);
						(Ram_HeapSlots->ramCode)(canvas,
							pictures + m * PICTURE_BYTES(TARGET_STRIKE_BITE),
							targetPos.x / 2 - TARGET_STRIKE_BITE_WIDTH,
							TARGET_STRIKE_BITE_Y, TARGET_STRIKE_BITE_WIDTH,
							TARGET_STRIKE_BITE_HEIGHT);
						syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
						flags = EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X;
						EFFECT_LOAD_BLITTER(
							HEAP_SLOT_RAM_CODE, flags, BLITTER_ADDITIVE);
					}
					(Ram_HeapSlots->ramCode)(canvas,
						pictures + m * PICTURE_BYTES(TARGET_STRIKE_BITE),
						targetPos.x / 2, TARGET_STRIKE_BITE_Y,
						TARGET_STRIKE_BITE_WIDTH, TARGET_STRIKE_BITE_HEIGHT);
					syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
					btEffectBuildCanvasBlitters(action->side, blitters);
				}
				if((kind == CASTING_IMPACT_IMPACT_DINOX ||
					   kind == CASTING_IMPACT_IMPACT_TERRIBLE_BITE) &&
					frame < 24) {
					if(kind == CASTING_IMPACT_IMPACT_DINOX)
						bursts = CASTING_IMPACT_RISING_PICTURES;
					else
						bursts = TERRIBLE_BITE_BURSTS;
					for(i = 0; i != bursts; i++) {
						PARTICLE *p;

						p = &impactParticles[i];
						if(frame >= i / 2 + 4) {
							Sint32 life;

							life = p->variant;
							if(life <= 9) {
								Sint32 half;

								half = life / 2;
								btEffectPosProjectPoint(p, &projected);
								projected.x = projected.x / 2;
								blitters[0](canvas,
									pictures +
										CASTING_IMPACT_FLASH_BURST_PICTURES +
										half * PICTURE_BYTES(FLASH_BURST),
									projected.x - FLASH_BURST_WIDTH / 2,
									projected.y - FLASH_BURST_HEIGHT / 2,
									FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
								btParticleAdvanceWithGravity3D(
									p, PARTICLE_DAMPING_NONE, toFIXED(1) / 16);
								p->variant++;
							}
						}
					}
				}
			} else if(kind == CASTING_IMPACT_IMPACT_RAIDENS_WRATH) {
				/* The warrior slides in and strikes, a picture every two
				   frames. */
				Sint32 m;

				EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
				EFFECT_RELEASE_BLITTERS();
				if(action->side == EFFECT_SIDE_ENEMIES)
					EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
						BLITTER_TRANSPARENT);
				else
					EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
						EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
						BLITTER_TRANSPARENT);
				blitters[0] = Ram_HeapSlots->ramCode;
				scrollPos += scrollSpeed;
				if(frame > 6)
					scrollSpeed = scrollSpeed * 48 / DAMPING_SCALE;
				REG_BG2X = (FIXEDtoINT(scrollPos)) << 8;
				t = frame - 28;
				if(t >= 0 && t < BLEND_WEIGHT_MAX)
					REG_BLDALPHA = ((t / 2 + 8) << 8) | (BLEND_WEIGHT_MAX - t);
				if(frame >= 4 && frame < 8)
					syWordsFill(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(1));
				if(frame > 3) {
					if(action->side == EFFECT_SIDE_ENEMIES)
						blitters[0](canvas, pictures, 32, 21,
							RAIDENS_WRATH_WARRIOR_WIDTH,
							RAIDENS_WRATH_WARRIOR_HEIGHT);
					else
						blitters[0](canvas, pictures, 0, 21,
							RAIDENS_WRATH_WARRIOR_WIDTH,
							RAIDENS_WRATH_WARRIOR_HEIGHT);
				}
				m = frame / 2;
				if(m < RAIDENS_WRATH_PICTURES) {
					if(action->side == EFFECT_SIDE_ENEMIES)
						blitters[0](canvas,
							pictures + CastingImpact_RaidensWrathPictures[m],
							CastingImpact_RaidensWrathX[m],
							CastingImpact_RaidensWrathY[m],
							CastingImpact_RaidensWrathWidths[m],
							CastingImpact_RaidensWrathHeights[m]);
					else
						blitters[0](canvas,
							pictures + CastingImpact_RaidensWrathPictures[m],
							CANVAS_SIZE - CastingImpact_RaidensWrathX[m] -
								CastingImpact_RaidensWrathWidths[m],
							CastingImpact_RaidensWrathY[m],
							CastingImpact_RaidensWrathWidths[m],
							CastingImpact_RaidensWrathHeights[m]);
				}
				syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
				btEffectBuildCanvasBlitters(action->side, blitters);
			} else if(kind == CASTING_IMPACT_IMPACT_APOCALYPSE) {
				/* The woman slides in, a picture every three frames, and three
				   bursts flash. */
				Sint32 m;

				EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
				if(frame == 7) {
					state->shakeFrm = 16;
					sySndPlayStub(SE_HEAVY_IMPACT);
				}
				if(frame == 11)
					sySndPlayStub(SE_HEAVY_IMPACT);
				if(frame == 15)
					sySndPlayStub(SE_HEAVY_IMPACT);
				EFFECT_RELEASE_BLITTERS();
				if(action->side == EFFECT_SIDE_ENEMIES)
					EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
						BLITTER_TRANSPARENT);
				else
					EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
						EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
						BLITTER_TRANSPARENT);
				blitters[0] = Ram_HeapSlots->ramCode;
				scrollPos += scrollSpeed;
				if(frame == 5)
					scrollPos += toFIXED(32);
				if(frame > 6)
					scrollSpeed = scrollSpeed * 48 / DAMPING_SCALE;
				REG_BG2X = (FIXEDtoINT(scrollPos)) << 8;
				t = frame - 29;
				if(t >= 0 && t < BLEND_WEIGHT_MAX)
					REG_BLDALPHA = ((t / 2 + 8) << 8) | (BLEND_WEIGHT_MAX - t);
				if(frame > 4)
					blitters[0](canvas, pictures, 35, 17,
						APOCALYPSE_WOMAN_WIDTH, APOCALYPSE_WOMAN_HEIGHT);
				if(frame == 5)
					syWordsFill(canvas, CANVAS_BYTES,
						CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
				if(frame >= 0 && (m = frame / 3) < APOCALYPSE_PICTURES) {
					if(action->side == EFFECT_SIDE_ENEMIES)
						blitters[0](canvas,
							pictures + CastingImpact_ApocalypsePictures[m],
							CastingImpact_ApocalypseX[m],
							CastingImpact_ApocalypseY[m] - 16,
							CastingImpact_ApocalypseWidths[m],
							CastingImpact_ApocalypseHeights[m]);
					else
						blitters[0](canvas,
							pictures + CastingImpact_ApocalypsePictures[m],
							CANVAS_SIZE - CastingImpact_ApocalypseX[m] -
								CastingImpact_ApocalypseWidths[m],
							CastingImpact_ApocalypseY[m] - 16,
							CastingImpact_ApocalypseWidths[m],
							CastingImpact_ApocalypseHeights[m]);
				}
				for(i = 0; i != 3; i++) {
					Sint32 m;

					t = frame - 8 - i * 2;
					if(t >= 0 && t <= 8) {
						m = t / 3;
						if(action->side == EFFECT_SIDE_ENEMIES)
							blitters[0](canvas,
								pictures +
									CastingImpact_ApocalypseBurstPictures[m],
								CastingImpact_ApocalypseBurstPoints[i][0] -
									CastingImpact_ApocalypseBurstWidths[m] / 2,
								CastingImpact_ApocalypseBurstPoints[i][1] -
									CastingImpact_ApocalypseBurstHeights[m] /
										2 -
									16,
								CastingImpact_ApocalypseBurstWidths[m],
								CastingImpact_ApocalypseBurstHeights[m]);
						else
							blitters[0](canvas,
								pictures +
									CastingImpact_ApocalypseBurstPictures[m],
								CANVAS_SIZE -
									CastingImpact_ApocalypseBurstPoints[i][0] -
									CastingImpact_ApocalypseBurstWidths[m] / 2,
								CastingImpact_ApocalypseBurstPoints[i][1] -
									CastingImpact_ApocalypseBurstHeights[m] /
										2 -
									16,
								CastingImpact_ApocalypseBurstWidths[m],
								CastingImpact_ApocalypseBurstHeights[m]);
					}
				}
				syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
				btEffectBuildCanvasBlitters(action->side, blitters);
			} else if(kind == CASTING_IMPACT_IMPACT_ASURA) {
				/* The swordsman slides in and swings, and his red crescent
				   fades. */
				Sint32 t;

				scrollPos += scrollSpeed;
				if(frame > 6)
					scrollSpeed = scrollSpeed * 48 / DAMPING_SCALE;
				REG_BG2X = (FIXEDtoINT(scrollPos)) << 8;
				if(frame == 0)
					REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, 8);
				t = frame - 24;
				if(t >= 0 && t < BLEND_WEIGHT_MAX)
					REG_BLDALPHA = EFFECT_BLEND_CANVAS(BLEND_WEIGHT_MAX - t);
				if(frame >= 4 && frame < 6)
					syWordsFill(canvas, CANVAS_BYTES,
						CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
				if(frame < 4) {
					if(action->side == EFFECT_SIDE_PARTY)
						blitters[0](canvas, state->sheet, 0, 24,
							ASURA_SHEET_LOW_WIDTH, 104);
					else
						blitters[0](canvas, state->sheet, 48, 24,
							ASURA_SHEET_LOW_WIDTH, 104);
				} else {
					if(frame < 8) {
						if(action->side == EFFECT_SIDE_PARTY)
							blitters[0](canvas, state->sheet, 0, 24,
								ASURA_SHEET_LOW_WIDTH, 104);
						else
							blitters[0](canvas, state->sheet, 48, 24,
								ASURA_SHEET_LOW_WIDTH, 104);
					}
					if(action->side == EFFECT_SIDE_PARTY)
						blitters[0](canvas, state->sheet + ASURA_SHEET_SWUNG,
							16, 16, ASURA_SHEET_SWUNG_WIDTH,
							ASURA_SHEET_SWUNG_HEIGHT);
					else
						blitters[0](canvas, state->sheet + ASURA_SHEET_SWUNG,
							32, 16, ASURA_SHEET_SWUNG_WIDTH,
							ASURA_SHEET_SWUNG_HEIGHT);
					if(frame >= 6 && frame < 8)
						blitters[0](canvas, state->sheet + ASURA_SHEET_CRESCENT,
							0, 16, ASURA_SHEET_CRESCENT_WIDTH,
							ASURA_SHEET_CRESCENT_HEIGHT);
					if(frame >= 8 && frame < 10)
						blitters[0](canvas, pictures, 0, 16, CANVAS_SIZE,
							ASURA_CRESCENT_FADE_A_HEIGHT);
					if(frame >= 10 && frame < 12)
						blitters[0](canvas, pictures + ASURA_CRESCENT_FADE_B, 0,
							16, CANVAS_SIZE, ASURA_CRESCENT_FADE_B_HEIGHT);
					if(frame >= 12 && frame < 14)
						blitters[0](canvas, pictures + ASURA_CRESCENT_FADE_C, 0,
							16, CANVAS_SIZE, ASURA_CRESCENT_FADE_C_HEIGHT);
					if(frame >= 14 && frame < 16)
						blitters[0](canvas, pictures + ASURA_CRESCENT_FADE_D, 0,
							16, CANVAS_SIZE, ASURA_CRESCENT_FADE_D_HEIGHT);
				}
			} else if(kind == CASTING_IMPACT_IMPACT_BROIL) {
				/* Ember streaks spread sideways. */
				Sint32 m;

				for(i = 0; i != BROIL_STREAKS; i++) {
					if(frame >= i + 6 && frame < i + 18) {
						Sint32 x;

						m = (frame - i - 6) / 2;
						x = targetPos.x / 2 -
							BattleEffect_EmberStreakPictureWidths[m] / 2;
						if(i & 1)
							x += (i + 1) / 2 * 3;
						else
							x -= (i + 1) / 2 * 3;
						{
							Sint32 mirrored;

							mirrored = 1;
							if(i != 0)
								mirrored = ((i - 1) & 3) > 1;
							blitters[mirrored](canvas,
								pictures + BattleEffect_EmberStreakPictures[m],
								x, BattleEffect_EmberStreakPictureBiasY[m] + 48,
								BattleEffect_EmberStreakPictureWidths[m],
								BattleEffect_EmberStreakPictureHeights[m]);
						}
					}
				}
			} else if(kind == CASTING_IMPACT_IMPACT_FROST_BITE) {
				/* Ice chips close in on the target. */
				if(frame == 0) {
					PARTICLE *p;

					p = impactParticles;
					for(i = 0; i != CASTING_IMPACT_IMPACT_PARTICLE_COUNT;
						i++, p++) {
						SET_POSITION(p, (syRandom() & 127) + 32, 0, 0);
						p->velX = syRandom() & TRIG_ANGLE_MASK;
						p->velY = syRandom() & TRIG_ANGLE_MASK;
						p->velZ = syRandom() & TRIG_ANGLE_MASK;
					}
					/* The last chip's y, not its radius, is set to 159: it
					   flies 159 pixels off its line, so the target's last
					   flinch, when its radius runs out, comes with no chip on
					   it. XXX (no evidence): whether its radius was meant, as
					   159 is the largest the others draw (32 + 127); only the
					   author's intent would say. */
					impactParticles[CASTING_IMPACT_IMPACT_PARTICLE_COUNT - 1]
						.y = 159;
				}
				p = impactParticles;
				for(i = 0; i != CASTING_IMPACT_IMPACT_PARTICLE_COUNT;
					i++, p++) {
					if(p->x >= 0 && frame >= i / 2) {
						Sint32 m;

						m = i & 3;
						syInitMatrix();
						syRotX(p->velX);
						syRotY(p->velY);
						btEffectPosProjectPoint(p, &projected);
						projected.x = projected.x / 2 + targetPos.x / 2;
						projected.y = projected.y + targetPos.y + 32;
						blitters[1](canvas,
							pictures + CastingImpact_IceChipPictures[m],
							projected.x - 4, projected.y - 4, 8, 8);
						p->x -= 6;
						if(p->x < 0 &&
							((i & 7) == 0 ||
								i ==
									CASTING_IMPACT_IMPACT_PARTICLE_COUNT - 1)) {
							sySndPlayStub(SE_IMPACT);
							btEffectTintBattler(action->tgts[0],
								SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 4);
						}
					}
				}
			} else if(kind == CASTING_IMPACT_IMPACT_TERRA_STRIKE) {
				/* A rock spire rises under the target and sinks. */
				if(frame >= 5 && frame < 50) {
					if(frame > 25)
						h = 196 - frame * 4;
					else
						h = frame * 16 - 64;
					if(h > ROCK_SPIRE_HEIGHT)
						h = ROCK_SPIRE_HEIGHT;
					blitters[0](
						canvas, pictures, 48, 104 - h, ROCK_SPIRE_WIDTH, h);
				}
			} else if(kind == CASTING_IMPACT_IMPACT_HAMMERSPHERE) {
				/* A ball falls onto the target, rests there a moment, then
				   bounces back up. */
				PARTICLE *p;

				p = impactParticles;
				if(frame == 0) {
					p->x = toFIXED(targetPos.x / 2);
					p->y = INTtoFIXED(targetPos.y + 48);
					if(action->side == EFFECT_SIDE_ENEMIES)
						p->velX = -toFIXED(3);
					else
						p->velX = toFIXED(3);
					p->velY = toFIXED(8);
					p->x += -p->velX * 12;
					p->y += -p->velY * 12;
				}
				if(frame <= 150) {
					REG_BLDCNT = 0;
					EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
				} else {
					EFFECT_SET_TRANSFER(
						state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
					REG_BLDCNT =
						BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
				}
				if(frame < 32) {
					blitters[1](canvas, pictures + HAMMERSPHERE_BALL,
						FIXEDtoINT(p->x) - HAMMERSPHERE_BALL_WIDTH / 2,
						FIXEDtoINT(p->y) - HAMMERSPHERE_BALL_HEIGHT,
						HAMMERSPHERE_BALL_WIDTH, HAMMERSPHERE_BALL_HEIGHT);
					if(frame < 12)
						btParticleAdvanceWithGravity2D(
							impactParticles, PARTICLE_DAMPING_NONE, 0);
					else if(frame == 12)
						p->velY = -p->velY / 3;
					else if(frame < 13 || frame > 18)
						btParticleAdvanceWithGravity2D(impactParticles,
							PARTICLE_DAMPING_NONE, toFIXED(1) / 4);
				}
				if(frame == 11) {
					sySndPlayStub(SE_CLATTER);
					state->shakeFrm = 16;
				}
			} else if(kind == CASTING_IMPACT_IMPACT_FORCIBLE_ARM ||
				kind == CASTING_IMPACT_IMPACT_TRUNCHEON_FIST) {
				/* A flame falls diagonally onto the target over six frames. */
				Sint32 x;
				Sint32 y;

				if(frame < 6) {
					if(action->side == EFFECT_SIDE_ENEMIES) {
						x = targetPos.x / 2 + (6 - frame) * 3 * 2;
						y = targetPos.y - (6 - frame) * 3 * 4 + 24;
					} else {
						x = targetPos.x / 2 - (6 - frame) * 3 * 2;
						y = targetPos.y - (6 - frame) * 3 * 4 + 24;
					}
					blitters[1](canvas, pictures, x - FALLING_FLAME_WIDTH / 2,
						y - FALLING_FLAME_HEIGHT / 2, FALLING_FLAME_WIDTH,
						FALLING_FLAME_HEIGHT);
				}
			} else if(kind == CASTING_IMPACT_IMPACT_ICE_CRUSH) {
				/* A crystal falls diagonally onto the target and shatters into
				   Prism's shards. */
				Sint32 x;
				Sint32 y;

				if(frame > 31)
					REG_BLDALPHA = BLEND_ALPHA(48 - frame, BLEND_WEIGHT_MAX);
				if(frame < 14) {
					PARTICLE *p;

					if(action->side == EFFECT_SIDE_ENEMIES) {
						x = targetPos.x / 2 + (14 - frame) * 3 * 2;
						y = targetPos.y - (14 - frame) * 3 * 4 + 24;
					} else {
						x = targetPos.x / 2 - (14 - frame) * 3 * 2;
						y = targetPos.y - (14 - frame) * 3 * 4 + 24;
					}
					blitters[1](canvas, pictures, x - CRYSTAL_WIDTH / 2,
						y - CRYSTAL_HEIGHT / 2, CRYSTAL_WIDTH, CRYSTAL_HEIGHT);
					if(frame == 13) {
						sySndPlayStub(SE_HEAVY_IMPACT);
						state->shakeFrm = 8;
						btEffectStartActorMotion(action->tgts[0], 1, toFIXED(2),
							toFIXED(8), toFIXED(3), 150);
						p = impactParticles;
						table = Prism_ShardStarts;
						for(i = 0; i != SHARDS_PER_CRYSTAL; i++, p++) {
							p->x = INTtoFIXED(*table++ + x - 40);
							p->y = toFIXED(*table++);
							p->velX = ((syRandom() & 127) - 64) << 11;
							p->velY = -(syRandom() & 127) << 11;
							p->z = 32;
							p->variant = 0;
						}
					}
				} else {
					for(i = 0; i != SHARDS_PER_CRYSTAL; i++) {
						Sint32 m;

						particle = &impactParticles[i];
						{
							Sint32 shape;

							shape = i % 5 * 3;
							m = shape + particle->variant / SHARD_TURN % 3;
						}
						blitters[i > 2 ? 0 : 1](canvas,
							pictures + CRYSTAL_SHARD_PICTURES +
								Prism_ShardPictures[m],
							(FIXEDtoINT(particle->x)) -
								Prism_ShardWidths[m] / 2,
							(FIXEDtoINT(particle->y)) -
								Prism_ShardHeights[m] / 2,
							Prism_ShardWidths[m], Prism_ShardHeights[m]);
						btParticleAdvanceWithGravity2D(
							particle, PARTICLE_DAMPING_NONE, toFIXED(1) / 8);
						particle->variant += particle->z;
						if(particle->z > 1 && (frame & 1))
							particle->z--;
					}
				}
			} else if(kind == CASTING_IMPACT_IMPACT_MOTHER_EARTH) {
				/* A boulder falls diagonally onto the target and breaks into
				   Spire's shards. */
				Sint32 x;
				Sint32 y;

				if(frame == 0) {
					PARTICLE *p;

					btResourceLoadAndDecompress(RESOURCE_BOULDER_PICTURE,
						pictures + CASTING_IMPACT_BOULDER_PICTURES, 1, 1);
					REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, 4);
					EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
					REG_BG2PA = Q_8_8(1);
					p = state->particles;
					for(i = 0; i != MOTHER_EARTH_SHARDS; i++, p++) {
						SET_POSITION(p, INTtoFIXED((syRandom() & 31) + 48),
							INTtoFIXED((syRandom() & 31) + 64), syRandom());
						p->velX = ((syRandom() & 63) - 32) << 12;
						p->velY = -((syRandom() & 127) + 16) << 12;
						p->velZ = (syRandom() & 15) + 16;
						p->variant = 0;
					}
				}
				if(frame == 14) {
					btEffectStartActorMotion(action->tgts[0], 1, toFIXED(2),
						toFIXED(8), toFIXED(3), 150);
					sySndPlayStub(SE_HEAVY_IMPACT);
					state->shakeFrm = 8;
				}
				i = 0;
				particle = state->particles;
				if(frame < 14) {
					if(action->side == EFFECT_SIDE_ENEMIES) {
						x = targetPos.x + (14 - frame) * 3 * 2;
						y = targetPos.y - (14 - frame) * 3 * 4 + 24;
					} else {
						x = targetPos.x - (14 - frame) * 3 * 2;
						y = targetPos.y - (14 - frame) * 3 * 4 + 24;
					}
					blitters[0](canvas,
						pictures + CASTING_IMPACT_BOULDER_PICTURES,
						(CANVAS_SIZE - BOULDER_PICTURE_SIZE) / 2,
						y - BOULDER_PICTURE_SIZE / 2, BOULDER_PICTURE_SIZE,
						BOULDER_PICTURE_SIZE);
					REG_BG2X = (CANVAS_SIZE / 2 - x) << 8;
				} else {
					for(; i != MOTHER_EARTH_SHARDS; i++, particle++) {
						Sint32 m;
						Sint32 w;
						Sint32 h;
						Sint32 left;
						Sint32 top;

						m = i % 5 * 3;
						m += particle->z / SHARD_TURN % 3;
						w = Spire_ShardWidths[m];
						left = (FIXEDtoINT(particle->x)) - w / 2;
						top = FIXEDtoINT(particle->y);
						h = Spire_ShardHeights[m];
						top -= h / 2;
						blitters[0](canvas,
							pictures + BOULDER_SHARD_PICTURES +
								Spire_ShardPictures[m],
							left, top, w, h);
						btParticleAdvanceWithGravity2D(
							particle, 63, toFIXED(1) / 2);
						particle->z += particle->velZ;
						if((frame & 1) && particle->velZ > 0)
							particle->velZ--;
					}
				}
			} else if(kind == CASTING_IMPACT_IMPACT_STUN_BOLT) {
				/* Glints sparkle and bolts turn about the target, one striking
				   each frame. */
				Sint32 fade;
				Sint32 angle;

				if(frame == 0) {
					PARTICLE *p;

					p = STUN_BOLT_BOLTS;
					for(i = 0; i != STUN_BOLT_COUNT; i++, p++) {
						p->z = syRandom();
						p->velZ = (syRandom() & 255) + 128;
						if(syRandom() & 1)
							p->velZ = -p->velZ;
						p->variant = syRandom() & 3;
					}
				}
				if(frame == 4)
					state->shakeFrm = 8;
				if(frame == 6)
					sySndPlayStub(SE_RISE_WITH_SPARKS);
				if(frame == 24)
					sySndPlayStub(SOUND_FADE_OUT_ACTION_SE);
				if(frame >= 8 && frame < 24) {
					Sint32 y;

					for(i = 0; i != 3; i++) {
						Sint32 m;
						Sint32 x;
						Uint32 flip;

						m = i & 3;
						angle = syRandom() & TRIG_ANGLE_MASK;
						x = (FIXEDtoINT(sySin(angle) << 4)) + targetPos.x / 2 -
							BattleEffect_GlintPictureWidths[m] / 2;
						y = (FIXEDtoINT(syCos(angle) << 4)) -
							BattleEffect_GlintPictureHeights[m] / 2;
						EFFECT_RELEASE_BLITTERS();
						flip = syRandom();
						EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
							CastingImpact_StunBoltGlintFlips[flip & 3] |
								EFFECT_CLIP_BOUNDS,
							BLITTER_MAX);
						ALT_BLITTER(canvas,
							pictures + BattleEffect_GlintPictures[m], x, y + 80,
							BattleEffect_GlintPictureWidths[m],
							BattleEffect_GlintPictureHeights[m]);
						syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
						btEffectBuildCanvasBlitters(action->side, blitters);
					}
				}
				if(frame > 3) {
					PARTICLE	 *p;
					RENDER_MODEL *model;

					matrix = syHeapAllocEWRAM(32);
					model = btRenderAllocateModelList(1);
					stunBoltTexture.logWidth = 5;
					stunBoltTexture.logHeight = 7;
					model->kind = MODEL_SHADED_MAX;
					model->texture = &stunBoltTexture;
					model->mesh = BattleEffect_ColumnMesh;
					model->vertices = matrix;
					for(i = 0; i != STUN_BOLT_STRIKES; i++) {
						PARTICLE *p;

						p = &STUN_BOLT_BOLTS[i];
						if(frame >= i && frame < i + 8) {
							fade = 0;
							if(frame == i)
								fade = -16;
							else if(frame >= i + 4)
								fade = -((frame - i - 4) * 16);
							model->shade = fade;
							{
								Sint32 life = p->variant;

								stunBoltTexture.pixels = pictures +
									STUN_BOLT_TEXTURES +
									life * PICTURE_BYTES(STUN_BOLT_TEXTURE);
								p->variant =
									(life + 1) & (STUN_BOLT_PICTURES - 1);
							}
							syInitMatrix();
							syTranslate(INTtoFIXED(targetPos.x / 2 - 64),
								INTtoFIXED(targetPos.y - 64), 0);
							syRotZ(p->z);
							p->z += p->velZ;
							syScale(toFIXED(4), toFIXED(1), toFIXED(1));
							btRenderTransformVertices(
								BattleEffect_TaperedBeamCorners, matrix, 4);
							btRenderDrawModelList(model);
						}
					}
					syHeapRelease(model);
					syHeapRelease(matrix);
				}
			} else if(kind == CASTING_IMPACT_IMPACT_SARGASSO ||
				kind == CASTING_IMPACT_IMPACT_FLASH_FORCE ||
				kind == CASTING_IMPACT_IMPACT_REVERSE_STAR) {
				/* A swirl, or Flash Force's rune, turns open on the ground
				   under the target; Reverse Star's leans back, and a glow grows
				   at the target's feet. */
				void		 *matrix;
				RENDER_MODEL *model;
				Sint32		  fade;

				matrix = syHeapAllocEWRAM(512);
				model = btRenderAllocateModelList(1);
				fade = 0;
				if(frame > 39)
					fade = (40 - frame) * 4;
				else
					state->shakeFrm = 1;
				reverseStarTexture.logWidth = 6;
				reverseStarTexture.logHeight = 6;
				reverseStarTexture.pixels = pictures;
				model->kind = MODEL_SHADED_ADDITIVE;
				model->texture = &reverseStarTexture;
				model->mesh = BattleEffect_SquareMesh;
				model->vertices = matrix;
				model->shade = fade;
				syInitMatrix();
				syTranslate(INTtoFIXED(targetPos.x / 2 - 64),
					INTtoFIXED(targetPos.y - 64), 0);
				syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1) / 2);
				if(frame == 8)
					sySndPlayStub(SE_LAUNCH);
				if(fade >= -63) {
					fade = frame * (toFIXED(1) / 16);
					if(kind == CASTING_IMPACT_IMPACT_REVERSE_STAR) {
						syRotX(-TRIG_TURN / 32);
						syRotY(-frame * (TRIG_TURN / 32));
						syRotX(-frame << 4);
					} else {
						syRotX(-TRIG_TURN / 16);
						syRotY(frame * (TRIG_TURN / 32));
					}
					syMatrixScaleUniform(fade);
					btRenderTransformVertices(
						BattleEffect_GroundSquareCorners, matrix, 4);
					btRenderDrawModelList(model);
					if(kind != CASTING_IMPACT_IMPACT_REVERSE_STAR) {
						syRotY(-frame * (TRIG_TURN / 16));
						syMatrixScaleUniform(toFIXED(1) / 2);
						btRenderTransformVertices(
							BattleEffect_GroundSquareCorners, matrix, 4);
						btRenderDrawModelList(model);
					}
				}
				syHeapRelease(model);
				syHeapRelease(matrix);
				if(kind == CASTING_IMPACT_IMPACT_REVERSE_STAR) {
					Sint32 m;
					Sint32 halfSize;

					m = frame / 4;
					halfSize = m * 2;
					fade = toFIXED(1);
					if(frame > 39)
						fade = toFIXED(7) / 2 - frame * (toFIXED(1) / 16);
					if(halfSize > 1 && fade > 0) {
						btEffectDrawShadedCircles(state->sheet, m * 4, fade);
						syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
						btCanvasBlitMirroredQuarters(
							state->sheet, targetPos.x / 2, targetPos.y, m * 4);
						EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
							EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP,
							BLITTER_ADDITIVE);
					}
				}
			} else if(kind == CASTING_IMPACT_IMPACT_CRUCIBLE_FIRE ||
				kind == CASTING_IMPACT_IMPACT_SUPERNOVA ||
				kind == CASTING_IMPACT_IMPACT_QUICK_SLASH ||
				kind == CASTING_IMPACT_IMPACT_BOSCA_HIT) {
				/* Rings of fire spread under the target, Supernova's standing,
				   and Crucible Fire and Supernova throw out streaks of
				   sparks. */
				void  *matrix;
				Sint32 t;

				matrix = syHeapAllocEWRAM(512);
				model = btRenderAllocateModelList(1);
				boscaHitTexture.logWidth = 7;
				boscaHitTexture.logHeight = 7;
				boscaHitTexture.pixels = pictures;
				model->texture = &boscaHitTexture;
				model->kind = MODEL_SHADED_ADDITIVE;
				model->mesh = BattleEffect_LargeSquareMesh;
				model->vertices = matrix;
				if(kind == CASTING_IMPACT_IMPACT_QUICK_SLASH ||
					kind == CASTING_IMPACT_IMPACT_BOSCA_HIT) {
					Sint32 fade;
					Sint32 scale;

					for(i = 0; i != 3; i++) {
						t = frame - i * 2;
						if(t >= 0) {
							scale = t * 3 * (toFIXED(1) / 16);
							fade = 0;
							if(t > 7)
								fade = (8 - t) * 8;
							model->shade = fade;
							if(fade > -64) {
								syInitMatrix();
								syTranslate(INTtoFIXED(targetPos.x / 2 - 64),
									INTtoFIXED(targetPos.y - 64), 0);
								syScale(
									toFIXED(1) / 2, toFIXED(1), toFIXED(1) / 2);
								syRotZ(i * (DEGtoANG(90)) + DEGtoANG(45));
								syRotX(-TRIG_TURN / 16);
								syMatrixScaleUniform(scale >> 1);
								btRenderTransformVertices(
									BattleEffect_GroundSquareCorners, matrix,
									4);
								btRenderDrawModelList(model);
							}
						}
					}
				} else if(kind == CASTING_IMPACT_IMPACT_SUPERNOVA) {
					Sint32 fade;
					Sint32 scale;

					t = frame - 4;
					if(t >= 0) {
						scale = t * (toFIXED(1) / 8);
						fade = 0;
						if(t > 7)
							fade = (8 - t) * 8;
						model->shade = fade;
						if(fade >= -63) {
							syInitMatrix();
							syTranslate(INTtoFIXED(targetPos.x / 2 - 64),
								INTtoFIXED(targetPos.y - 64), 0);
							syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1) / 2);
							syRotX(DEGtoANG(90));
							syMatrixScaleUniform(scale);
							btRenderTransformVertices(
								BattleEffect_GroundSquareCorners, matrix, 4);
							btRenderDrawModelList(model);
						}
					}
				} else {
					for(i = 0; i != 3; i++) {
						t = frame - i * 4;
						if(t >= 0) {
							Sint32 fade;

							fade = 0;
							if(t > 7)
								fade = (8 - t) * 8;
							model->shade = fade;
							if(fade > -64) {
								syInitMatrix();
								syTranslate(INTtoFIXED(targetPos.x / 2 - 64),
									INTtoFIXED(targetPos.y - 64), 0);
								syScale(
									toFIXED(1) / 2, toFIXED(1), toFIXED(1) / 2);
								syRotZ(i * (DEGtoANG(90)) + DEGtoANG(45));
								syRotX(-TRIG_TURN / 16);
								syMatrixScaleUniform(t * 3 * (toFIXED(1) / 16));
								btRenderTransformVertices(
									BattleEffect_GroundSquareCorners, matrix,
									4);
								btRenderDrawModelList(model);
							}
						}
					}
				}
				syHeapRelease(model);
				syHeapRelease(matrix);
				if(kind != CASTING_IMPACT_IMPACT_QUICK_SLASH &&
					kind != CASTING_IMPACT_IMPACT_BOSCA_HIT) {
					Sint32 x;

					if(frame == 4) {
						PARTICLE *p;

						for(i = 0; i != CASTING_IMPACT_IMPACT_PARTICLE_COUNT;
							i++) {
							PARTICLE *p;

							p = &impactParticles[i];
							p->x = syRandom();
							p->y = 0;
							p->velY = ((syRandom() & 3) + 3) << 17;
						}
					}
					if(frame > 4) {
						for(i = 0; i != CASTING_IMPACT_IMPACT_PARTICLE_COUNT;
							i++) {
							PARTICLE *p;
							Sint32	  k;

							p = &impactParticles[i];
							for(k = 0; k != CASTING_IMPACT_TRAIL_MOTES; k++) {
								r = (FIXEDtoINT(p->y)) - k * 2;
								if(r >= 0) {
									Sint32 n;

									x = targetPos.x / 2 +
										(FIXEDtoINT(sySin(p->x) * r));
									y = targetPos.y +
										(FIXEDtoINT(syCos(p->x) * r));
									n = 2 - k / 8;
									EFFECT_DRAW_MOTE(
										blitters[0], canvas, sprites, x, y, n);
								}
							}
							p->y += p->velY;
							p->velY = p->velY * 62 / DAMPING_SCALE;
						}
					}
				}
			} else if(kind == CASTING_IMPACT_IMPACT_STUN_JIP) {
				/* A ring of fire spreads and two bands of flame turn about the
				   target. */
				void  *matrix;
				Sint32 t;

				matrix = syHeapAllocEWRAM(512);
				model = btRenderAllocateModelList(1);
				stunJipTexture.logWidth = 7;
				stunJipTexture.logHeight = 7;
				stunJipTexture.pixels = pictures;
				model->kind = MODEL_SHADED_ADDITIVE;
				model->texture = &stunJipTexture;
				model->mesh = BattleEffect_LargeSquareMesh;
				model->vertices = matrix;
				t = frame - 4;
				if(t >= 0) {
					Sint32 fade;
					Sint32 scale;

					scale = t * (toFIXED(1) / 4);
					fade = 0;
					if(t > 7)
						fade = (8 - t) * 8;
					model->shade = fade;
					if(fade >= -63) {
						syInitMatrix();
						syTranslate(
							INTtoFIXED(targetPos.x / 2 - 64), toFIXED(40), 0);
						syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1) / 2);
						syRotX(CASTING_IMPACT_RING_TILT);
						syMatrixScaleUniform(scale);
						btRenderTransformVertices(
							BattleEffect_GroundSquareCorners, matrix, 4);
						btRenderDrawModelList(model);
					}
				}
				stunJipTexture.logWidth = 7;
				stunJipTexture.logHeight = 4;
				stunJipTexture.pixels = pictures + CASTING_IMPACT_FLAME_BAND;
				model->mesh = CastingImpact_BandMesh;
				for(i = 0; i != 2; i++) {
					Sint32 start;

					start = CastingImpact_StunJipBandStarts[i] + 4;
					if(frame > start) {
						fade = (start - frame) * 8;
						if(fade > -16)
							fade = -16;
						if(fade > -64) {
							scale = (frame - start) *
									CastingImpact_StunJipBandGrowth[i] * 1500 +
								toFIXED(131) / 512;
							model->shade = fade;
							syInitMatrix();
							syTranslate(INTtoFIXED(targetPos.x / 2 - 60),
								toFIXED(CastingImpact_StunJipBandHeights[i]),
								0);
							syScale(scale * 2, scale * 4, scale / 4);
							syRotX(CASTING_IMPACT_RING_TILT);
							if(i & 1)
								syRotY((i * 2 + frame) * (TRIG_TURN / 16));
							else
								syRotY((i * 2 - frame) * (TRIG_TURN / 16));
							btRenderTransformVertices(
								BattleEffect_OctagonBandCorners, matrix, 16);
							btRenderDrawModelList(model);
						}
					}
				}
				syHeapRelease(model);
				syHeapRelease(matrix);
				if(frame == 4)
					state->shakeFrm = 8;
			} else if(kind == CASTING_IMPACT_IMPACT_ARMOR_CRUSH) {
				/* Four rings of billows spread and turn about the target. */
				Sint32 scale;

				matrix = syHeapAllocEWRAM(512);
				model = btRenderAllocateModelList(1);
				model->texture = &armorCrushTexture;
				armorCrushTexture.logWidth = 6;
				armorCrushTexture.logHeight = 6;
				model->vertices = matrix;
				armorCrushTexture.pixels = pictures;
				model->kind = MODEL_SHADED_ADDITIVE;
				model->mesh = BattleEffect_RingMesh;
				for(i = 0; i != 4; i++) {
					start = CastingImpact_ArmorCrushWaveStarts[i] + 4;
					model->scrollV = (frame * 8 + i * 32) & TEXTURE_SCROLL_MASK;
					if(frame == start + 4)
						sySndPlayStub(SE_LAUNCH);
					if(frame > start) {
						fade = (start - frame) * 8 + 56;
						if(fade > -16)
							fade = -16;
						if(fade > -64) {
							scale = (frame - start) *
									CastingImpact_ArmorCrushWaveGrowth[i] *
									2000 +
								toFIXED(1) / 4;
							model->shade = fade;
							syInitMatrix();
							syTranslate(INTtoFIXED(targetPos.x / 2 - 60),
								toFIXED(40), 0);
							syScale(scale,
								CastingImpact_ArmorCrushWaveStretch[i] * scale *
									2,
								scale / 4);
							syRotX(CASTING_IMPACT_RING_TILT);
							if(i & 1)
								syRotY((i * 8 + frame) * (TRIG_TURN / 64));
							else
								syRotY((i * 8 - frame) * (TRIG_TURN / 64));
							btRenderTransformVertices(
								BattleEffect_RingCorners, matrix, 32);
							btRenderDrawModelList(model);
						}
					}
				}
				syHeapRelease(model);
				syHeapRelease(matrix);
				if(frame == 4)
					state->shakeFrm = 20;
			} else if(kind == CASTING_IMPACT_IMPACT_UNUSED_SKULLS) {
				/* Unused: skulls spiral in on the target. */
				if(frame == 0) {
					PARTICLE *p;

					p = impactParticles;
					for(i = 0; i != CASTING_IMPACT_RISING_PICTURES; i++, p++) {
						p->variant = -i * 2;
						p->x = syRandom();
						p->velX = (syRandom() & 511) + 512;
						p->z = i * 2 + 80;
					}
				}
				/* DEAD: a dissolve laid out as Wheeze's face and its fades are:
				   the first 32x32 picture is copied over the third wherever the
				   second holds the level frame / 2 * 2 or one more, darkest
				   first. Here the pictures are the three 24x24 skulls, which
				   end before the third block, so nothing drawn changes; the ROM
				   keeps the loop. */
				for(i = 0; i != PICTURE_BYTES(IMP_FACE); i++) {
					if(pictures[PICTURE_BYTES(IMP_FACE) + i] >> 1 == frame / 2)
						pictures[2 * PICTURE_BYTES(IMP_FACE) + i] = pictures[i];
				}
				btEffectPosProjectScaledBattler(action->tgts[0], &targetPoint);
				targetPoint.x = targetPoint.x / 2;
				for(i = 0; i != CASTING_IMPACT_RISING_PICTURES; i++) {
					PARTICLE *p;

					p = &impactParticles[i];
					if(p->variant >= 0 && p->z > 0) {
						Sint32 m;
						Sint32 x;
						Sint32 y;

						m = i % 3;
						x = targetPoint.x + (FIXEDtoINT(sySin(p->x) * p->z));
						y = targetPoint.y + (FIXEDtoINT(syCos(p->x) * p->z));
						p->z -= 4;
						p->x += p->velX;
						/* DEAD: only the unused skulls reach this arm, so the
						   Wheeze test never holds; the ROM keeps the compare
						   and the face's draw. */
						if(kind == CASTING_IMPACT_IMPACT_WHEEZE)
							blitters[0](canvas, pictures,
								x - IMP_FACE_WIDTH / 2, y - IMP_FACE_HEIGHT / 2,
								IMP_FACE_WIDTH, IMP_FACE_HEIGHT);
						else
							blitters[0](canvas,
								pictures + m * PICTURE_BYTES(SKULL),
								x - SKULL_WIDTH / 2, y - SKULL_HEIGHT / 2,
								SKULL_WIDTH, SKULL_HEIGHT);
					}
					p->variant++;
				}
			} else if(kind == CASTING_IMPACT_IMPACT_WHEEZE) {
				/* Imp faces rise and fade. */
				p = impactParticles;
				for(i = 0; i != CASTING_IMPACT_IMPACT_PARTICLE_COUNT / 2;
					i++, p++) {
					if(frame >= i / 2 + 4 &&
						p->variant < CASTING_IMPACT_RISING_LIFE) {
						Sint32 m;

						m = 0;
						if(p->variant >= CASTING_IMPACT_RISING_LIFE / 2)
							m = (p->variant - CASTING_IMPACT_RISING_LIFE / 2) /
								2;
						btEffectPosProjectPoint(p, &projected);
						projected.x = projected.x / 2;
						blitters[1](canvas,
							pictures + m * PICTURE_BYTES(IMP_FACE),
							projected.x - IMP_FACE_WIDTH / 2,
							projected.y - IMP_FACE_HEIGHT / 2, IMP_FACE_WIDTH,
							IMP_FACE_HEIGHT);
						btParticleAdvanceWithGravity3D(p, 62, toFIXED(1) / 32);
						btParticleAdvanceWithGravity3D(p, 62, toFIXED(1) / 32);
						p->variant++;
					}
				}
				if(frame == 4)
					state->shakeFrm = 12;
				if(frame == 8)
					sySndPlayStub(SE_CURSE);
			} else if(kind == CASTING_IMPACT_IMPACT_CHILL) {
				/* Puffs of cloud rise and fade. */
				if(frame == 0) {
					EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
					REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, 6);
				}
				p = impactParticles;
				for(i = 0; i != CHILL_PUFFS; i++, p++) {
					if(frame >= i / 2 + 4 &&
						p->variant < CASTING_IMPACT_RISING_LIFE) {
						Sint32 m;

						m = 0;
						if(p->variant >= CASTING_IMPACT_RISING_LIFE / 2)
							m = (p->variant - CASTING_IMPACT_RISING_LIFE / 2) /
								3;
						btEffectPosProjectPoint(p, &projected);
						projected.x = projected.x / 2;
						blitters[0](canvas,
							pictures + m * PICTURE_BYTES(CHILL_PUFF),
							projected.x - CHILL_PUFF_WIDTH / 2,
							projected.y - CHILL_PUFF_HEIGHT / 2,
							CHILL_PUFF_WIDTH, CHILL_PUFF_HEIGHT);
						btParticleAdvanceWithGravity3D(p, 58, toFIXED(1) / 16);
						p->variant++;
					}
				}
				if(frame == 4)
					state->shakeFrm = 12;
				if(frame == 8)
					sySndPlayStub(SE_CURSE);
			} else if(kind == CASTING_IMPACT_IMPACT_BAD_OMEN) {
				/* Skulls rise from the target and fade. */
				Sint32 m;

				if(frame > 47)
					REG_BLDALPHA = BLEND_ALPHA(64 - frame, BLEND_WEIGHT_MAX);
				p = impactParticles;
				for(i = 0; i != CASTING_IMPACT_RISING_PICTURES; i++, p++) {
					m = i % 3;
					btEffectPosProjectPoint(p, &projected);
					projected.x = projected.x / 2;
					blitters[i & 1](canvas, pictures + m * PICTURE_BYTES(SKULL),
						projected.x - SKULL_WIDTH / 2,
						projected.y - SKULL_HEIGHT / 2, SKULL_WIDTH,
						SKULL_HEIGHT);
					btParticleAdvanceWithGravity3D(
						p, 60, 1 << ((i & 3) + BAD_OMEN_SKULL_LIFT_SHIFT));
					p->variant++;
				}
			} else if(kind == CASTING_IMPACT_IMPACT_FLASH_EDGE) {
				/* The canvas flashes three times. */
				if(frame == 18)
					sySndPlayStub(SOUND_FADE_OUT_ACTION_SE);
				for(i = 0; i != 3; i++) {
					if(frame == i * 3 + 3) {
						sySndPlayStub(SE_POWER_UP);
						syWordsFill(canvas, CANVAS_BYTES,
							CANVAS_FILL_WORD(FLASH_EDGE_FLASH_LEVEL));
					}
				}
			} else if(kind == CASTING_IMPACT_IMPACT_SCORPIONFISH) {
				/* Puffs of smoke rise. */
				if(frame == 8)
					sySndPlayStub(SE_CURSE);
				p = impactParticles;
				for(i = 0; i != CASTING_IMPACT_IMPACT_PARTICLE_COUNT / 2;
					i++, p++) {
					if(frame >= i / 2 + 4) {
						Sint32 life;

						life = p->variant;
						if(life < CASTING_IMPACT_RISING_LIFE) {
							Sint32 m;

							m = life / 4;
							btEffectPosProjectPoint(p, &projected);
							projected.x = projected.x / 2;
							blitters[1](canvas,
								pictures + m * PICTURE_BYTES(SMOKE_PUFF),
								projected.x - SMOKE_PUFF_WIDTH / 2,
								projected.y - SMOKE_PUFF_HEIGHT / 2,
								SMOKE_PUFF_WIDTH, SMOKE_PUFF_HEIGHT);
							btParticleAdvanceWithGravity3D(
								p, 62, toFIXED(1) / 32);
							btParticleAdvanceWithGravity3D(
								p, 62, toFIXED(1) / 32);
							p->variant++;
						}
					}
				}
			} else if(kind != CASTING_IMPACT_IMPACT_UNUSED_MERCURY_DJINNI) {
				/* The smoke impacts: a puff rises each frame. */
				Sint32 m;

				p = impactParticles;
				for(i = 0; i != CASTING_IMPACT_RISING_PICTURES; i++, p++) {
					if(frame >= i + 4) {
						Sint32 life;

						life = p->variant;
						if(life < CASTING_IMPACT_RISING_LIFE) {
							m = life / 4;
							btEffectPosProjectPoint(p, &projected);
							projected.x = projected.x / 2;
							/* DEAD: Scorpionfish has its own arm above, so only
							   Searing Fog passes this test; the ROM keeps both
							   compares. */
							if(kind == CASTING_IMPACT_IMPACT_SEARING_FOG ||
								kind == CASTING_IMPACT_IMPACT_SCORPIONFISH)
								blitters[1](canvas,
									pictures + m * PICTURE_BYTES(SMOKE_PUFF),
									projected.x - SMOKE_PUFF_WIDTH / 2,
									projected.y - SMOKE_PUFF_HEIGHT / 2,
									SMOKE_PUFF_WIDTH, SMOKE_PUFF_HEIGHT);
							else
								blitters[i & 1](canvas,
									pictures + m * PICTURE_BYTES(SMOKE_PUFF),
									projected.x - SMOKE_PUFF_WIDTH / 2,
									projected.y - SMOKE_PUFF_HEIGHT / 2,
									SMOKE_PUFF_WIDTH, SMOKE_PUFF_HEIGHT);
							if(kind == CASTING_IMPACT_IMPACT_ACID_BATH)
								btParticleAdvanceWithGravity3D(
									p, 60, toFIXED(1) / 64);
							else
								btParticleAdvanceWithGravity3D(
									p, 60, toFIXED(1) / 16);
							p->variant++;
						}
					}
				}
			}

			/* Echo's blow lands at frame 24, not 3: its Djinni stays over the
			   target and rises from frame 20, and sparks burst as the target is
			   struck. */
			if(kind == CASTING_IMPACT_IMPACT_ECHO && frame < 44) {
				Sint32 half;

				btEffectPosProjectPoint(djinnPos, &projected);
				projected.x = projected.x / 2;
				blitters[1](canvas, pictures + CASTING_IMPACT_DJINNI_PICTURE,
					projected.x - CASTING_IMPACT_DJINNI_WIDTH / 2,
					projected.y - 4, CASTING_IMPACT_DJINNI_WIDTH,
					CASTING_IMPACT_DJINNI_HEIGHT);
				if(frame == 23)
					(btObjectGetStub(action->user)->obj)->velY = toFIXED(3);
				if(frame > 19)
					djinnPos[1] += toFIXED(4);
				if(frame == 23) {
					PARTICLE *p;

					half = btUnitGetEffectHeightStub(action->tgts[0]) / 2 +
						toFIXED(30);
					p = state->particles;
					for(i = 0; i != CASTING_IMPACT_SPARKS; i++, p++) {
						SET_POSITION(p, targetObject->x, half, targetObject->z);
						p->velX = ((syRandom() & 255) - 127) << 11;
						p->velY = ((syRandom() & 255) - 64) << 11;
						p->velZ = ((syRandom() & 255) - 127) << 11;
						p->variant = i / 2 + CASTING_IMPACT_SPARK_LIFE;
					}
				}
				if(frame == 24) {
					btEffectStartActorMotion(
						action->tgts[0], 1, 0, 0, toFIXED(9), 100);
					btEffectTintBattler(
						action->tgts[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 4);
					EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
					state->shakeFrm = 8;
				}
				if(frame >= 24 && frame < 36)
					blitters[1](canvas,
						state->sheet + CASTING_IMPACT_ORB_PICTURES +
							((frame - 24) / 2) *
								PICTURE_BYTES(YELLOW_ORB_PACKED),
						targetPos.x / 2 - 8, 28, YELLOW_ORB_PACKED_WIDTH,
						YELLOW_ORB_PACKED_HEIGHT);
			}
			/* Power Drive strikes four times. */
			if(kind == CASTING_IMPACT_IMPACT_POWER_DRIVE) {
				for(i = 0; i != 4; i++) {
					if(frame == i * 4 + 8) {
						btEffectTintBattler(action->tgts[0], SPRITE_COLOR_WHITE,
							POSE_DAMAGE, 0, 2);
						state->shakeFrm = 2;
						sySndPlayStub(SE_IMPACT);
					}
				}
			}
			/* A Djinni kind's Djinni flies to the target in six frames. */
			if(kind < CASTING_IMPACT_DJINN_KINDS && frame <= 5) {
				btEffectPosProjectPoint(djinnPos, &projected);
				projected.x = projected.x / 2;
				blitters[1](canvas, pictures + CASTING_IMPACT_DJINNI_PICTURE,
					projected.x - CASTING_IMPACT_DJINNI_WIDTH / 2,
					projected.y - 4, CASTING_IMPACT_DJINNI_WIDTH,
					CASTING_IMPACT_DJINNI_HEIGHT);
				djinnPos[0] += djinnStep[0];
				djinnPos[1] += djinnStep[1];
				djinnPos[2] += djinnStep[2];
			}
			/* The sounds. */
			if(kind != CASTING_IMPACT_IMPACT_ECHO && frame == 3)
				EFFECT_SHOW_DAMAGE(HIT_SOUND_NONE);
			if(frame == 4) {
				if(kind == CASTING_IMPACT_IMPACT_STUN_JIP)
					sySndPlayStub(SE_CRASH);
				else if(kind == CASTING_IMPACT_IMPACT_FLARE_BURST ||
					kind == CASTING_IMPACT_IMPACT_CRUCIBLE_FIRE ||
					kind == CASTING_IMPACT_IMPACT_TERRIBLE_BITE)
					sySndPlayStub(SE_CLATTER);
				else
					sySndPlayStub(SE_HEAVY_IMPACT);
			}
			if(frame == 6) {
				if(kind == CASTING_IMPACT_IMPACT_FLINT ||
					kind == CASTING_IMPACT_IMPACT_SCORCH ||
					kind == CASTING_IMPACT_IMPACT_SAP ||
					kind == CASTING_IMPACT_IMPACT_LIFE_NOURISH ||
					kind == CASTING_IMPACT_IMPACT_LIFE_LEECH ||
					kind == CASTING_IMPACT_IMPACT_VEIN_TAP ||
					kind == CASTING_IMPACT_IMPACT_HIGH_VITALS ||
					kind == CASTING_IMPACT_IMPACT_FLARE_BURST ||
					kind == CASTING_IMPACT_IMPACT_PSY_LEECH ||
					kind == CASTING_IMPACT_IMPACT_STEEL ||
					kind == CASTING_IMPACT_IMPACT_BARRAGE ||
					kind == CASTING_IMPACT_IMPACT_TRUNCHEON_FIST ||
					kind == CASTING_IMPACT_IMPACT_UNUSED_MERCURY_DJINNI ||
					kind == CASTING_IMPACT_IMPACT_POWER_DRIVE ||
					kind == CASTING_IMPACT_IMPACT_CRUCIBLE_FIRE ||
					kind == CASTING_IMPACT_IMPACT_SUPERNOVA ||
					kind == CASTING_IMPACT_IMPACT_FLASH_EDGE ||
					kind == CASTING_IMPACT_IMPACT_TERRIBLE_BITE) {
					btMotionApplyKnockbackStub(action->tgts[0], KNOCKBACK_HURL);
					state->shakeFrm = 12;
				} else if(kind == CASTING_IMPACT_IMPACT_ECHO ||
					kind == CASTING_IMPACT_IMPACT_HAMMERSPHERE ||
					kind == CASTING_IMPACT_IMPACT_SARCOPHAGUS) {
					state->shakeFrm = 8;
				} else if(kind == CASTING_IMPACT_IMPACT_BROIL ||
					kind == CASTING_IMPACT_IMPACT_AQUA_SOCK ||
					kind == CASTING_IMPACT_IMPACT_FORCIBLE_ARM) {
					btMotionApplyKnockbackStub(action->tgts[0], KNOCKBACK_HOP);
					state->shakeFrm = 2;
				} else if(kind == CASTING_IMPACT_IMPACT_CYCLONE_SLASH ||
					kind == CASTING_IMPACT_IMPACT_TERRA_STRIKE) {
					btMotionApplyKnockbackStub(
						action->tgts[0], KNOCKBACK_LAUNCH);
					state->shakeFrm = 16;
				} else if(kind == CASTING_IMPACT_IMPACT_ROC ||
					kind == CASTING_IMPACT_IMPACT_CALL_ZOMBIE ||
					kind == CASTING_IMPACT_IMPACT_WILD_WOLF ||
					kind == CASTING_IMPACT_IMPACT_DINOX ||
					kind == CASTING_IMPACT_IMPACT_QUICK_SLASH ||
					kind == CASTING_IMPACT_IMPACT_BOSCA_HIT ||
					kind == CASTING_IMPACT_IMPACT_RAIDENS_WRATH ||
					kind == CASTING_IMPACT_IMPACT_SCORPIONFISH) {
					btMotionApplyKnockbackStub(
						action->tgts[0], KNOCKBACK_NUDGE);
					state->shakeFrm = 12;
				}
			}
			if(frame == 6)
				btEffectTintBattler(
					action->tgts[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 4);
			if(frame == 14)
				btEffectTintBattler(
					action->tgts[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 4);
			btCameraShake(8, 8);
			EFFECT_END_FRAME(state);
		}

		/* Phase 4: the impacts that lower a stat clear the canvas and hand over
		   to its effect, as the unused ones do. */
		if(kind == CASTING_IMPACT_IMPACT_MELTDOWN ||
			kind == CASTING_IMPACT_IMPACT_HEAT_SMASH ||
			kind == CASTING_IMPACT_IMPACT_SOUR ||
			kind == CASTING_IMPACT_IMPACT_CHILL ||
			kind == CASTING_IMPACT_IMPACT_GEL ||
			kind == CASTING_IMPACT_IMPACT_ICE_CRUSH ||
			kind == CASTING_IMPACT_IMPACT_HAMMERSPHERE ||
			kind == CASTING_IMPACT_IMPACT_ARMOR_CRUSH) {
			syWordsClear((void *)BG_CHAR_BLOCK(1), CANVAS_VIEW_BYTES);
			syWordsClear(canvas, CANVAS_VIEW_BYTES);
			action->prelude = 0;
			syCloseEvent(btEffectDarkenBackgroundStep);
			syCloseEvent(btEffectTransferCanvas);
			EFFECT_RELEASE_BLITTERS();
			if(kind == CASTING_IMPACT_IMPACT_MELTDOWN ||
				kind == CASTING_IMPACT_IMPACT_CHILL ||
				kind == CASTING_IMPACT_IMPACT_ICE_CRUSH ||
				kind == CASTING_IMPACT_IMPACT_HAMMERSPHERE ||
				kind == CASTING_IMPACT_IMPACT_ARMOR_CRUSH)
				btEffectRunImpair(action);
			else if(kind == CASTING_IMPACT_IMPACT_SOUR ||
				kind == CASTING_IMPACT_IMPACT_HEAT_SMASH)
				btEffectRunWeaken(action);
			else if(kind == CASTING_IMPACT_IMPACT_GEL)
				btEffectRunDull(action);
			return;
		}
		if(kind == CASTING_IMPACT_IMPACT_MIST ||
			kind == CASTING_IMPACT_IMPACT_FEVER ||
			kind == CASTING_IMPACT_IMPACT_BAD_OMEN ||
			kind == CASTING_IMPACT_IMPACT_UNUSED_SKULLS ||
			kind == CASTING_IMPACT_IMPACT_HEAT_MIRAGE ||
			kind == CASTING_IMPACT_IMPACT_EVIL_EYE ||
			kind == CASTING_IMPACT_IMPACT_BLINDING_SMOG ||
			kind == CASTING_IMPACT_IMPACT_MURK ||
			kind == CASTING_IMPACT_IMPACT_SEARING_FOG ||
			kind == CASTING_IMPACT_IMPACT_FLASH_PUNCH)
			syCloseEvent(btEffectArmBg2AffineHBlankDMA);
		EFFECT_FINISH_CANVAS();
	}
}

#undef CASTING_IMPACT_DJINN_KINDS
#undef CASTING_IMPACT_ORB_PICTURES
#undef CASTING_IMPACT_DJINNI_WIDTH
#undef CASTING_IMPACT_DJINNI_HEIGHT
#undef CASTING_IMPACT_DJINNI_PICTURE
#undef CASTING_IMPACT_GATHER_FRAMES
#undef CASTING_IMPACT_SPARKS
#undef CASTING_IMPACT_SPARK_DEPTH
#undef CASTING_IMPACT_IMPACT_PARTICLES
#undef CASTING_IMPACT_IMPACT_PARTICLE_COUNT
#undef CASTING_IMPACT_SPARK_LIFE
#undef CASTING_IMPACT_SPARK_SHRINK
#undef ASURA_SHEET_LOW_WIDTH
#undef ASURA_SHEET_LOW_HEIGHT
#undef ASURA_SHEET_SWUNG_WIDTH
#undef ASURA_SHEET_SWUNG_HEIGHT
#undef ASURA_SHEET_CRESCENT_WIDTH
#undef ASURA_SHEET_CRESCENT_HEIGHT
#undef ASURA_SHEET_SWUNG
#undef ASURA_SHEET_CRESCENT
#undef ASURA_CRESCENT_FADE_A_HEIGHT
#undef ASURA_CRESCENT_FADE_B_HEIGHT
#undef ASURA_CRESCENT_FADE_C_HEIGHT
#undef ASURA_CRESCENT_FADE_D_HEIGHT
#undef ASURA_CRESCENT_FADE_B
#undef ASURA_CRESCENT_FADE_C
#undef ASURA_CRESCENT_FADE_D
#undef SARCOPHAGUS_COFFIN_WIDTH
#undef SARCOPHAGUS_COFFIN_HEIGHT
#undef SARCOPHAGUS_GHOST_A
#undef SARCOPHAGUS_GHOST_A_WIDTH
#undef SARCOPHAGUS_GHOST_A_HEIGHT
#undef SARCOPHAGUS_GHOST_B
#undef SARCOPHAGUS_GHOST_B_WIDTH
#undef SARCOPHAGUS_GHOST_B_HEIGHT
#undef SARCOPHAGUS_GHOST_C
#undef SARCOPHAGUS_GHOST_C_WIDTH
#undef SARCOPHAGUS_GHOST_C_HEIGHT
#undef SARCOPHAGUS_GHOST_D
#undef SARCOPHAGUS_GHOST_D_WIDTH
#undef SARCOPHAGUS_GHOST_D_HEIGHT
#undef SARCOPHAGUS_GHOST_E
#undef SARCOPHAGUS_GHOST_E_WIDTH
#undef SARCOPHAGUS_GHOST_E_HEIGHT
#undef SARCOPHAGUS_GHOST_F
#undef SARCOPHAGUS_GHOST_F_WIDTH
#undef SARCOPHAGUS_GHOST_F_HEIGHT
#undef SARCOPHAGUS_GHOST_G
#undef SARCOPHAGUS_GHOST_G_WIDTH
#undef SARCOPHAGUS_GHOST_G_HEIGHT
#undef ROCK_SPIRE_WIDTH
#undef ROCK_SPIRE_HEIGHT
#undef FALLING_FLAME_WIDTH
#undef FALLING_FLAME_HEIGHT
#undef CHILL_PUFF_WIDTH
#undef CHILL_PUFF_HEIGHT
#undef CHILL_PUFFS
#undef BAD_OMEN_SKULL_LIFT_SHIFT
#undef FLASH_EDGE_FLASH_LEVEL
#undef CASTING_IMPACT_RING_TILT
#undef IMP_FACE_WIDTH
#undef IMP_FACE_HEIGHT
#undef IMP_FACE_FADES
#undef STUN_BOLT_TEXTURE_WIDTH
#undef STUN_BOLT_TEXTURE_HEIGHT
#undef STUN_BOLT_TEXTURES
#undef STUN_BOLT_PICTURES
#undef HAMMERSPHERE_BALL
#undef HAMMERSPHERE_BALL_WIDTH
#undef HAMMERSPHERE_BALL_HEIGHT
#undef CASTING_IMPACT_SMOKE_PICTURES
#undef CASTING_IMPACT_FLASH_BURST_PICTURES
#undef CASTING_IMPACT_RUNE_PICTURES
#undef CASTING_IMPACT_BOULDER_PICTURES
#undef BOULDER_PICTURE_SIZE
#undef MOTHER_EARTH_SHARDS
#undef CASTING_IMPACT_FLAME_BAND
#undef CASTING_IMPACT_RISING_PICTURES
#undef CASTING_IMPACT_RISING_LIFE
#undef CASTING_IMPACT_BURST_LIFE
#undef POWER_DRIVE_FIREBALLS
#undef POWER_DRIVE_FIREBALL_LIFE
#undef TERRIBLE_BITE_BURSTS
#undef RAIDENS_WRATH_PICTURES
#undef APOCALYPSE_PICTURES
#undef RAIDENS_WRATH_WARRIOR_WIDTH
#undef RAIDENS_WRATH_WARRIOR_HEIGHT
#undef APOCALYPSE_WOMAN_WIDTH
#undef APOCALYPSE_WOMAN_HEIGHT
#undef SARCOPHAGUS_DISSOLVE_ROWS
#undef BROIL_STREAKS
#undef CASTING_IMPACT_TRAIL_MOTES
#undef STUN_BOLT_BOLTS
#undef STUN_BOLT_COUNT
#undef STUN_BOLT_STRIKES
