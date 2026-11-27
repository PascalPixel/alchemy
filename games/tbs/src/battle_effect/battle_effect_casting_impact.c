/* The casting impacts, the Djinn attacks, weapons' unleashes and enemy
   abilities that open the same way: sparks close in on the caster, a streak
   shoots at the target and a ring of light bursts over it in a spray of sparks,
   and then each impact plays its own effect over the target (enum
   CASTING_IMPACT). Part of battle_effect.c. */

/* Steps background colours 0 to 63 one level towards a palette resource. */
void btEffectStepPaletteToResource(Sint32 resourceId)
{
	Uint16	tgt[SHEET_PALETTE_COLORS];
	Uint16 *color = (Uint16 *)BG_PLTT;
	Sint32	i;

	syWordsCopy(tgt, syResourceGetTableEntry(resourceId), SHEET_PALETTE_BYTES);
	tgt[0] = 0;
	for(i = 0; i != SHEET_PALETTE_COLORS; i++, color++) {
		Sint32 red = RGB_RED(*color);
		Sint32 green = RGB_GREEN(*color);
		Sint32 blue = RGB_BLUE(*color);
		Sint32 targetRed = RGB_RED(tgt[i]);
		Sint32 targetGreen = RGB_GREEN(tgt[i]);
		Sint32 targetBlue = RGB_BLUE(tgt[i]);

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
		tgt[i] = RGB(red, green, blue);
	}
	syWordsCopy(BG_PLTT, tgt, SHEET_PALETTE_BYTES);
}

/* Clears the prelude image, stops its events and releases its blitters before
   the next effect takes over. */
static inline void btCastingImpactEndPrelude(
	BATTLE_EFFECT_STATE *state, void *canvas)
{
	syWordsClear(BG_CHAR_BLOCK(1), CANVAS_BYTES);
	syWordsClear(canvas, CANVAS_BYTES);
	state->args->prelude = 0;
	syCloseEvent(btEffectDarkenBackgroundStep);
	syCloseEvent(btEffectTransferCanvas);
	EFFECT_RELEASE_BLITTERS();
}

/* The casting impacts, by effect kind less EFFECT_RANGE_CASTING_IMPACT: each is
   named for the first ability whose AbilityBattleEffects entry selects it; the
   four no ability selects are named UNUSED_ and for the effect they hand off
   to. */
typedef enum CASTING_IMPACT {
	CASTING_IMPACT_BANE = 0,	  /* smoke */
	CASTING_IMPACT_SMOG,		  /* smoke */
	CASTING_IMPACT_MIST,		  /* smoke */
	CASTING_IMPACT_FEVER,		  /* smoke */
	CASTING_IMPACT_FLINT,		  /* nothing */
	CASTING_IMPACT_SCORCH,		  /* flash bursts rise from the target */
	CASTING_IMPACT_SQUALL,		  /* two bolts strike among glints */
	CASTING_IMPACT_SAP,			  /* motes fly back to the caster */
	CASTING_IMPACT_TERRA_STRIKE,  /* a rock spire grows and sinks */
	CASTING_IMPACT_POISON_CLOUD,  /* smoke */
	CASTING_IMPACT_DEADLY_POISON, /* smoke */
	/* also Death Size; the reaper swings its scythe */
	CASTING_IMPACT_MORTAL_DANGER,
	CASTING_IMPACT_BAD_OMEN,	 /* skulls rise from the target */
	CASTING_IMPACT_LIFE_NOURISH, /* motes fly back to the caster */
	/* a blue flame column falls onto the target */
	CASTING_IMPACT_AQUA_SOCK,
	CASTING_IMPACT_UNUSED_ICE_CHIPS, /* hands off */
	CASTING_IMPACT_FROST_BITE,		 /* ice chips converge on the target */
	CASTING_IMPACT_UNUSED_GATHERING, /* hands off */
	CASTING_IMPACT_LIFE_LEECH,		 /* motes fly back to the caster */
	CASTING_IMPACT_PSY_LEECH,		 /* motes fly back to the caster */
	CASTING_IMPACT_BROIL,			 /* ember streaks spread sideways */
	CASTING_IMPACT_MELTDOWN,		 /* nothing; hands to the stat reduction */
	CASTING_IMPACT_HEAT_MIRAGE,		 /* also Heat Flash; smoke */
	CASTING_IMPACT_BARRAGE,			 /* flash bursts rise from the target */
	CASTING_IMPACT_UNUSED_MYSTIC_FLAME, /* hands off */
	CASTING_IMPACT_ACID_BATH,			/* smoke */
	CASTING_IMPACT_UNUSED_COMETS,		/* hands off */
	CASTING_IMPACT_STUN_VOLTAGE,		/* two bolts strike among glints */
	CASTING_IMPACT_BLINDING_SMOG,		/* smoke */
	CASTING_IMPACT_MURK,				/* smoke */
	CASTING_IMPACT_CYCLONE_SLASH,		/* a tornado stands over the target */
	CASTING_IMPACT_PSYPHON_SEAL,		/* a rune opens over the target */
	CASTING_IMPACT_ASURA,				/* a masked swordsman slashes */
	CASTING_IMPACT_FORCIBLE_ARM,		/* a flame falls diagonally */
	CASTING_IMPACT_TRUNCHEON_FIST,		/* a flame falls diagonally */
	CASTING_IMPACT_SEVERE_BLOW,			/* hands off */
} CASTING_IMPACT;

/* The impact orb's pictures, packed at half width, after the streak's. */
#define CASTING_IMPACT_ORB_PICTURES	 (6 * PICTURE_BYTES(FIRE_STREAK))
/* The Djinni's picture, in Ram_MapGridMetatileBuffer after the impact's own:
   past room for the six glints and six flash bursts, more than any impact loads
   (Squall's glints and lightning bolts are the most). */
#define CASTING_IMPACT_DJINNI_WIDTH	 20
#define CASTING_IMPACT_DJINNI_HEIGHT 40
#define CASTING_IMPACT_DJINNI_PICTURE \
	(GLINT_SET_BYTES + 6 * PICTURE_BYTES(FLASH_BURST))
#define CASTING_IMPACT_GATHER_TICKS 32 /* ticks the sparks gather */
#define CASTING_IMPACT_SPARKS		64 /* sparks in state->particles */
/* How far either side of the caster the gathering sparks' depth is clamped. */
#define CASTING_IMPACT_SPARK_DEPTH	60
/* A spark scattered over the target lives this long at the least, counted down
   two a tick (the draw visits each spark twice); it is drawn a mote size
   smaller for every 16 of life lost. */
#define CASTING_IMPACT_SPARK_LIFE	32
/* The impact pictures' particles, in Ram_MapGridMetatileBuffer after the
   Djinni. */
#define CASTING_IMPACT_PARTICLES \
	((PARTICLE *)(Ram_MapGridMetatileBuffer + 16384))
#define CASTING_IMPACT_PARTICLE_COUNT  64
/* The impact particles that carry the rising pictures: flash bursts, skulls or
   puffs of smoke. */
#define CASTING_IMPACT_RISING_PICTURES 16
/* The ticks a rising puff of smoke lives, four to each of its six pictures,
   and a flash burst, two to each of its six. */
#define CASTING_IMPACT_RISING_LIFE	   24
#define CASTING_IMPACT_BURST_LIFE	   12
/* Broil's ember streaks. */
#define BROIL_STREAKS				   12

/* Asura's sprite sheet: the masked swordsman with his sword low, then swung up,
   then the red crescent his stroke leaves. */
#define ASURA_SHEET_LOW_WIDTH		80
#define ASURA_SHEET_LOW_HEIGHT		96
#define ASURA_SHEET_SWUNG_WIDTH		80
#define ASURA_SHEET_SWUNG_HEIGHT	104
#define ASURA_SHEET_CRESCENT_WIDTH	128
#define ASURA_SHEET_CRESCENT_HEIGHT 91
#define ASURA_SHEET_SWUNG			PICTURE_BYTES(ASURA_SHEET_LOW)
/* How tall the swordsman with his sword low is drawn. */
/* BUG: as tall as the swung picture, 8 rows more than the low one, so the draw
   reads on into the swung picture's top rows: the tip of the raised sword, a
   few pixels at the right edge, under the low swordsman's feet. */
#define ASURA_LOW_DRAW_HEIGHT \
	WHEN(FIX_ASURA_SWORD_LOW_DRAW_HEIGHT, ASURA_SHEET_LOW_HEIGHT, \
		ASURA_SHEET_SWUNG_HEIGHT)
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

/* Terra Strike's rock spire at its tallest. */
#define ROCK_SPIRE_WIDTH		  32
#define ROCK_SPIRE_HEIGHT		  96
/* The flame Forcible Arm and Truncheon Fist drop. */
#define FALLING_FLAME_WIDTH		  32
#define FALLING_FLAME_HEIGHT	  64
/* Bad Omen's skulls rise faster by the tick, the slowest by toFIXED(1) / 32
   more a tick, written as a shift of 1, and each of the next three twice as
   fast as the one before. */
#define BAD_OMEN_SKULL_LIFT_SHIFT 11

/* Where Mortal Danger draws `picture`, one of the reaper's seven pictures
   (REAPER_PICTURE_X), for the casting `side`. */
#define CASTING_IMPACT_PICTURE_X(side, picture) \
	ReaperPictureX[(side)][(picture)]

/* Gathers sparks around the caster, launches the Djinni and draws the impact on
   the first target.

   Abilities: Bane; Smog; Mist; Fever; Flint; Scorch; Squall; Sap; Terra Strike;
   Poison Cloud; Deadly Poison; Mortal Danger, Death Size; Bad Omen; Life
   Nourish; Aqua Sock; Frost Bite; Life Leech; Psy Leech; Broil; Meltdown; Heat
   Mirage, Heat Flash; Barrage; Acid Bath; Stun Voltage; Blinding Smog; Murk;
   Cyclone Slash; Psyphon Seal; Asura; Forcible Arm; Truncheon Fist; Severe
   Blow. */
void btEffectRunCastingImpact(BATTLE_EFFECT_PARM *args, Sint32 mode)
{
	Sint32				 pictureOfs;
	SCREEN_POINT		 casterPos;
	SCREEN_POINT		 tgtPos;
	POS					 djinniPos;
	Sint32				 phase;
	Sint32				 i;
	Sint32				*scanline;
	Sint32				 gathering;
	Sint32				 djinniX;
	DRAW_RECTANGLE		*slotDraw;
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 frm;
	BATTLE_CAMERA		*camera;
	Uint8				*sprs;
	FIXED				 scrollPos;
	Sint32				 scrollSpeed;
	Sint32				 duration;
	POS					*pos;
	ACTOR				*casterActor;
	PARTICLE			*particle;
	Sint32				 pairX;
	Sint32				 stripX;
	POS					 vel;
	SCREEN_POINT		 sparkScreen;
	BLITTER_PAIR		 blitters;
	SCREEN_POINT		 projected;
	Uint8				*pictures;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
	sprs = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
	pictures = Ram_MapGridMetatileBuffer;
	state->args = args;
	if(mode == CASTING_IMPACT_MORTAL_DANGER ||
		mode == CASTING_IMPACT_TERRA_STRIKE || mode == CASTING_IMPACT_ASURA) {
		btEffectStartTiledCanvas(0);
	} else {
		btEffectStartCanvasLayer(0);
	}
	REG_BLDALPHA = EFFECT_BLEND_ADD;

	/* Setup: the pictures. */
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprs, 0, 0);
	btResourceLoadAndDecompress(RESOURCE_FIRE_STREAK_SHEET, state, 1, 0);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_ORB_SHEET, pictures, 1, 0);
	btEffectPackYellowOrbs(pictures, state->sheet + CASTING_IMPACT_ORB_PICTURES,
		YELLOW_ORB_WIDTH, YELLOW_ORB_PICTURES * YELLOW_ORB_HEIGHT);
	if(mode == CASTING_IMPACT_SCORCH || mode == CASTING_IMPACT_BARRAGE) {
		btResourceLoadAndDecompress(RESOURCE_FLASH_BURST_SHEET, pictures, 1, 0);
	} else if(mode == CASTING_IMPACT_BAD_OMEN) {
		btResourceLoadAndDecompress(RESOURCE_SKULL_SHEET, pictures, 1, 0);
	} else if(mode == CASTING_IMPACT_SQUALL ||
		mode == CASTING_IMPACT_STUN_VOLTAGE) {
		btResourceLoadAndDecompress(RESOURCE_TORNADO_SHEET, pictures, 1, 0);
		btResourceLoadAndDecompress(
			RESOURCE_LIGHTNING_BOLT_SHEET, pictures + GLINT_SET_BYTES, 1, 0);
	} else if(mode == CASTING_IMPACT_PSYPHON_SEAL) {
		btResourceLoadAndDecompress(RESOURCE_RUNE_SHEET, pictures, 1, 1);
	} else if(mode == CASTING_IMPACT_TERRA_STRIKE) {
		btResourceLoadAndDecompress(RESOURCE_ROCK_SPIRE_SHEET, pictures, 1, 1);
	} else if(mode == CASTING_IMPACT_AQUA_SOCK) {
		btResourceLoadAndDecompress(RESOURCE_BLUE_FLAME_SHEET, pictures, 1, 0);
	} else if(mode == CASTING_IMPACT_CYCLONE_SLASH) {
		btResourceLoadAndDecompress(RESOURCE_TORNADO_SHEET, pictures, 1, 0);
	} else if(mode == CASTING_IMPACT_FROST_BITE) {
		btResourceLoadAndDecompress(RESOURCE_ICE_CHIP_SHEET, pictures, 1, 0);
	} else if(mode == CASTING_IMPACT_BROIL) {
		btResourceLoadAndDecompress(
			RESOURCE_EMBER_STREAK_SHEET, pictures, 1, 0);
	} else if(mode == CASTING_IMPACT_FORCIBLE_ARM ||
		mode == CASTING_IMPACT_TRUNCHEON_FIST) {
		btResourceLoadAndDecompress(RESOURCE_FLAME_SHEET_A, pictures, 1, 0);
	} else if(mode != CASTING_IMPACT_MORTAL_DANGER &&
		mode != CASTING_IMPACT_ASURA) {
		btResourceLoadAndDecompress(RESOURCE_SMOKE_SHEET, pictures, 1, 0);
	}
	/* The Djinni of the impact's element. */
	switch(mode) {
	case CASTING_IMPACT_BANE:
	case CASTING_IMPACT_FLINT:
	case CASTING_IMPACT_SAP:
	case CASTING_IMPACT_TERRA_STRIKE:
	case CASTING_IMPACT_POISON_CLOUD:
	case CASTING_IMPACT_DEADLY_POISON:
	case CASTING_IMPACT_MORTAL_DANGER:
	case CASTING_IMPACT_BAD_OMEN:
	case CASTING_IMPACT_LIFE_NOURISH:
	case CASTING_IMPACT_FORCIBLE_ARM:
		btResourceLoadAndDecompress(RESOURCE_VENUS_DJINN_SMALL_SHEET,
			pictures + CASTING_IMPACT_DJINNI_PICTURE, 1, 1);
		break;
	case CASTING_IMPACT_MIST:
	case CASTING_IMPACT_AQUA_SOCK:
	case CASTING_IMPACT_UNUSED_ICE_CHIPS:
	case CASTING_IMPACT_FROST_BITE:
	case CASTING_IMPACT_UNUSED_GATHERING:
	case CASTING_IMPACT_LIFE_LEECH:
	case CASTING_IMPACT_PSY_LEECH:
		btResourceLoadAndDecompress(RESOURCE_MERCURY_DJINN_SMALL_SHEET,
			pictures + CASTING_IMPACT_DJINNI_PICTURE, 1, 1);
		break;
	case CASTING_IMPACT_FEVER:
	case CASTING_IMPACT_SCORCH:
	case CASTING_IMPACT_BROIL:
	case CASTING_IMPACT_MELTDOWN:
	case CASTING_IMPACT_HEAT_MIRAGE:
	case CASTING_IMPACT_BARRAGE:
	case CASTING_IMPACT_UNUSED_MYSTIC_FLAME:
	case CASTING_IMPACT_ACID_BATH:
	case CASTING_IMPACT_TRUNCHEON_FIST:
	case CASTING_IMPACT_SEVERE_BLOW:
		btResourceLoadAndDecompress(RESOURCE_MARS_DJINN_SMALL_SHEET,
			pictures + CASTING_IMPACT_DJINNI_PICTURE, 1, 1);
		break;
	case CASTING_IMPACT_SMOG:
	case CASTING_IMPACT_SQUALL:
	case CASTING_IMPACT_UNUSED_COMETS:
	case CASTING_IMPACT_STUN_VOLTAGE:
	case CASTING_IMPACT_BLINDING_SMOG:
	case CASTING_IMPACT_MURK:
	case CASTING_IMPACT_CYCLONE_SLASH:
	case CASTING_IMPACT_PSYPHON_SEAL:
	case CASTING_IMPACT_ASURA:
		btResourceLoadAndDecompress(RESOURCE_JUPITER_DJINN_SMALL_SHEET,
			pictures + CASTING_IMPACT_DJINNI_PICTURE, 1, 1);
		break;
	/* Unused: impact mode 100, which this game never dispatches
	   (btEffectRunByIdRange passes only the 36 impacts); The Lost Age's
	   dispatcher reaches it, and its twin of this function loads the same
	   Mercury sprite sheet for it. */
	case EFFECT_RANGE_CASTING_IMPACT:
		btResourceLoadAndDecompress(RESOURCE_MERCURY_DJINN_SMALL_SHEET,
			pictures + CASTING_IMPACT_DJINNI_PICTURE, 1, 1);
		break;
	}
	/* Trails: Bad Omen's last longer than the others'. */
	state->transferMode = CANVAS_COPY_AND_FADE;
	if(mode == CASTING_IMPACT_BAD_OMEN) {
		state->transferValue = CANVAS_FADE_THREE_QUARTERS;
	} else {
		state->transferValue = CANVAS_FADE_HALF;
	}
	sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);
	btProjectionScaledUnit(state->args->tgts[0], &tgtPos);
	btProjectionScaledUnit(state->args->user, &casterPos);
	btEffectBuildCanvasBlitters(state->args->side, blitters);
	state->fadeFrm = BACKGROUND_DARKEN_TICKS;
	state->fadeStep = 0;
	casterActor = btObjectGetStub(state->args->user)->actor;

	for(i = 0; i != CASTING_IMPACT_SPARKS; i++) {
		particle = &state->particles[i];
		SET_POSITION(particle, (syRandom() & 63) + 32, 0, 0);
		SET_VELOCITY(particle, syRandom() & TRIG_ANGLE_MASK,
			syRandom() & TRIG_ANGLE_MASK, syRandom() & TRIG_ANGLE_MASK);
	}
	/* Stop the caster and place the Djinni overhead. */
	fdActorSetAnimationSpeedStub(casterActor, 0);
	/* FAKEMATCH(temporary): position, a pointer standing in for djinniPos's
	 * address. */
	pos = &djinniPos;
	SET_POSITION(pos, casterActor->x.fixed, casterActor->y.fixed + toFIXED(80),
		casterActor->z.fixed);
	{
		Sint32 savedVelX = casterActor->velX;
		Sint32 savedVelY = casterActor->velY;
		Sint32 savedVelZ = casterActor->velZ;
		Sint32 savedAcceleration = casterActor->acceleration;
		Sint32 savedGravity = casterActor->gravity;

		SET_VELOCITY(casterActor, 0, 0, 0);
		casterActor->acceleration = 0;
		casterActor->gravity = 0;
		btProjectionScaledUnit(state->args->user, &casterPos);
		casterPos.x = casterPos.x / 2;
		sySndPlayStub(SE_POWER_UP);

		/* Phase 1, the gathering (CASTING_IMPACT_GATHER_TICKS). */
		for(frm = 0; frm != CASTING_IMPACT_GATHER_TICKS; frm++) {
			gathering = 0;
			for(i = 0; i != CASTING_IMPACT_SPARKS; i++) {
				PARTICLE *spark = &state->particles[i];
				if(spark->x >= 0) {
					if(frm >= i / 4) {
						{
							Sint32 size = 5;
							syInitMatrix();
							syRotZ(spark->velZ);
							syRotX(spark->velX);
							syRotY(spark->velY);
							btProjectionPoint(spark, &sparkScreen);
							sparkScreen.x = sparkScreen.x / 2 + casterPos.x;
							/* Djinni kinds circle the caster's chest; the rest
							   circle lower, Severe Blow lowest. */
							if(mode <= CASTING_IMPACT_SAP)
								sparkScreen.y = sparkScreen.y + casterPos.y - 8;
							else if(mode == CASTING_IMPACT_SEVERE_BLOW)
								sparkScreen.y =
									sparkScreen.y + casterPos.y + 44;
							else
								sparkScreen.y =
									sparkScreen.y + casterPos.y + 12;
							/* DEAD: the depth is clamped to -60..60 and moved
							   to 0..120 but never read; the ROM keeps the clamp
							   and the move. */
							CLAMP(sparkScreen.depth,
								-CASTING_IMPACT_SPARK_DEPTH,
								CASTING_IMPACT_SPARK_DEPTH);
							sparkScreen.depth += CASTING_IMPACT_SPARK_DEPTH;
							EFFECT_DRAW_MOTE(blitters[1], canvas, sprs,
								sparkScreen.x, sparkScreen.y, size);
						}
						spark->x -= 4;
					}
					gathering++;
				}
			}
			if(mode <= CASTING_IMPACT_SAP) {
				/* The Djinni shows over the caster once a spark has closed
				   in. */
				if(gathering < CASTING_IMPACT_SPARKS) {
					EFFECT_VIEW_FROM_CAMERA(camera);
					btProjectionPoint(&djinniPos, &sparkScreen);
					sparkScreen.x /= 2;
					djinniX = sparkScreen.x;
					blitters[0](canvas,
						pictures + CASTING_IMPACT_DJINNI_PICTURE,
						sparkScreen.x - CASTING_IMPACT_DJINNI_WIDTH / 2,
						sparkScreen.y - 4, CASTING_IMPACT_DJINNI_WIDTH,
						CASTING_IMPACT_DJINNI_HEIGHT);
				}
			}
			EFFECT_PRESENT_CANVAS(state);
		}
		/* FAKEMATCH(dead): position set again to the &djinniPos it holds. */
		pos = &djinniPos;

		/* Phase 2: place BG2 so the impact stands over the target. */
		if(mode == CASTING_IMPACT_MORTAL_DANGER) {
			REG_BG2PA = Q_8_8(1);
			if(state->args->side == EFFECT_SIDE_ENEMIES) {
				REG_BG2X = (32 - tgtPos.x) << 8;
			} else {
				REG_BG2X = (96 - tgtPos.x) << 8;
			}
		} else {
			if(mode == CASTING_IMPACT_ASURA) {
				REG_BG2PA = Q_8_8(1);
				if(state->args->side == EFFECT_SIDE_ENEMIES) {
					scrollPos = -toFIXED(128);
					scrollSpeed = toFIXED(12);
				} else {
					scrollPos = toFIXED(8);
					scrollSpeed = -toFIXED(12);
				}
				REG_BG2X = FIXEDtoINT(scrollPos) << 8;
			}
		}
		if(mode == CASTING_IMPACT_TERRA_STRIKE) {
			REG_BG2PA = Q_8_8(1);
			REG_BG2X = (CANVAS_SIZE / 2 - tgtPos.x) << 8;
			state->transferMode = CANVAS_COPY_AND_CLEAR;
			state->transferValue = 0;
			syWordsClear(BG_CHAR_BLOCK(1), CANVAS_BYTES);
			syWordsClear(canvas, CANVAS_BYTES);
			REG_BLDCNT = 0;
		}
		if(mode == CASTING_IMPACT_PSYPHON_SEAL) {
			REG_BG2PA = Q_8_8(1);
			if(state->args->side == EFFECT_SIDE_ENEMIES) {
				REG_BG2X = (32 - tgtPos.x) << 8;
			} else {
				REG_BG2X = (96 - tgtPos.x) << 8;
			}
		}
		/* The four unused impacts clear the canvas and hand off to another
		   effect without giving the caster its motion back: the ice chips to
		   Blizzard's volley, the Mystic Flame one to Mystic Flame's orbs and
		   the comets to PROJECTILE_VOLLEY_IMPACT_COMETS; the gathering ends
		   here. */
		if(mode == CASTING_IMPACT_UNUSED_ICE_CHIPS ||
			mode == CASTING_IMPACT_UNUSED_GATHERING ||
			mode == CASTING_IMPACT_UNUSED_MYSTIC_FLAME ||
			mode == CASTING_IMPACT_UNUSED_COMETS) {
			btCastingImpactEndPrelude(state, canvas);
			fdActorSelectAnimationStub(casterActor, POSE_CAST);
			if(mode == CASTING_IMPACT_UNUSED_ICE_CHIPS) {
				btEffectRunProjectileVolley(args, PROJECTILE_VOLLEY_BLIZZARD);
			}
			if(mode == CASTING_IMPACT_UNUSED_MYSTIC_FLAME) {
				btEffectRunMysticFlame(args);
			}
			if(mode != CASTING_IMPACT_UNUSED_COMETS) {
				return;
			}
			btEffectRunProjectileVolley(args, PROJECTILE_VOLLEY_IMPACT_COMETS);
			return;
		}
		fdActorSetAnimationSpeedStub(casterActor, ANIM_STEP_NORMAL);
		SET_VELOCITY(casterActor, savedVelX, savedVelY, savedVelZ);
		casterActor->acceleration = savedAcceleration;
		casterActor->gravity = savedGravity;
	}
	/* Severe Blow, with the caster's motion back, clears the canvas and hands
	   off to btEffectRunSparkGroups' rising sparks, with
	   SPARK_GROUPS_SEVERE_BLOW. */
	if(mode == CASTING_IMPACT_SEVERE_BLOW) {
		btCastingImpactEndPrelude(state, canvas);
		args->variant = SPARK_GROUPS_SEVERE_BLOW;
		btEffectRunSparkGroups(args, SPARK_GROUPS_RISING);
		return;
	}

	{
		ACTOR *tgtActor = btObjectGetStub(state->args->tgts[0])->actor;

		SET_POSITION(&vel, (tgtActor->x.fixed - djinniPos.x) / 6,
			(tgtActor->y.fixed - djinniPos.y + toFIXED(30)) / 6,
			(tgtActor->z.fixed - djinniPos.z) / 6);
		for(i = 0; i != CASTING_IMPACT_SPARKS; i++) {
			particle = &state->particles[i];
			particle->variant = 0;
		}
		/* Sparks scatter from the target. */
		if(mode != CASTING_IMPACT_AQUA_SOCK) {
			Sint32 height = btUnitGetEffectHeightStub(state->args->tgts[0]) / 2;
			for(i = 0; i != CASTING_IMPACT_SPARKS / 2; i++) {
				particle = &state->particles[i];
				SET_POSITION(
					particle, tgtActor->x.fixed, height, tgtActor->z.fixed);
				if(mode == CASTING_IMPACT_PSYPHON_SEAL) {
					SET_VELOCITY(particle, ((syRandom() & 255) - 127) << 12,
						((syRandom() & 255) - 64) << 10,
						((syRandom() & 255) - 127) << 12);
				} else {
					SET_VELOCITY(particle, ((syRandom() & 255) - 127) << 12,
						((syRandom() & 255) - 64) << 12,
						((syRandom() & 255) - 127) << 12);
				}
				particle->variant = i / 2 + CASTING_IMPACT_SPARK_LIFE;
			}
		}
		/* Mortal Danger and Asura replace the caster's overlay and the impact's
		   pictures. */
		if(mode == CASTING_IMPACT_MORTAL_DANGER) {
			btResourceLoadAndDecompress(RESOURCE_REAPER_SHEET, state, 1, 1);
			btResourceLoadAndDecompress(
				RESOURCE_RED_CRESCENT_SHEET_A, pictures, 1, 0);
			REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, 14);
		}
		if(mode == CASTING_IMPACT_ASURA) {
			btResourceLoadAndDecompress(
				RESOURCE_BLUE_BEAST_SHEET, state->imageStorage, 1, 1);
			btResourceLoadAndDecompress(
				RESOURCE_RED_CRESCENT_SHEET_B, pictures, 1, 0);
			REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, 14);
		}
		/* The impact's own particles fly out from the target. */
		if(mode != CASTING_IMPACT_SAP && mode != CASTING_IMPACT_LIFE_NOURISH &&
			mode != CASTING_IMPACT_LIFE_LEECH &&
			mode != CASTING_IMPACT_MORTAL_DANGER &&
			mode != CASTING_IMPACT_ASURA && mode != CASTING_IMPACT_PSY_LEECH) {
			Sint32 height = 0;
			if(mode != CASTING_IMPACT_BAD_OMEN)
				height = toFIXED(20);
			for(i = 0; i != CASTING_IMPACT_PARTICLE_COUNT; i++) {
				particle = &CASTING_IMPACT_PARTICLES[i];
				SET_POSITION(
					particle, tgtActor->x.fixed, height, tgtActor->z.fixed);
				if(mode == CASTING_IMPACT_SCORCH ||
					mode == CASTING_IMPACT_BARRAGE) {
					SET_VELOCITY(particle, ((syRandom() & 255) - 127) << 11,
						(syRandom() & 255) << 11,
						((syRandom() & 255) - 127) << 11);
				} else if(mode == CASTING_IMPACT_ACID_BATH) {
					SET_VELOCITY(particle, ((syRandom() & 255) - 127) << 11,
						(syRandom() & 127) << 10,
						((syRandom() & 255) - 127) << 11);
				} else {
					SET_VELOCITY(particle, ((syRandom() & 255) - 127) << 10,
						(syRandom() & 127) << 10,
						((syRandom() & 255) - 127) << 10);
				}
				particle->variant = 0;
			}
		}
		/* Mist, Fever, Bad Omen, Heat Mirage, Blinding Smog and Murk wave BG2
		   by scanline. */
		if((mode >= CASTING_IMPACT_MIST && mode <= CASTING_IMPACT_FEVER) ||
			mode == CASTING_IMPACT_BAD_OMEN ||
			mode == CASTING_IMPACT_HEAT_MIRAGE || mode == CASTING_IMPACT_MURK ||
			mode == CASTING_IMPACT_BLINDING_SMOG) {
			sySetEvent(btEffectArmBg2AffineHBlankDMA, EVENT_PASS_VBLANK);
		}

		/* Unused: the last test, for impact mode 100, which this game never
		   dispatches; The Lost Age's twin lists mode 100 in the same test. */
		if((mode >= CASTING_IMPACT_FLINT && mode <= CASTING_IMPACT_SQUALL) ||
			mode == CASTING_IMPACT_BARRAGE ||
			mode == CASTING_IMPACT_CYCLONE_SLASH ||
			mode == CASTING_IMPACT_STUN_VOLTAGE ||
			mode == CASTING_IMPACT_FORCIBLE_ARM ||
			mode == CASTING_IMPACT_TRUNCHEON_FIST ||
			mode == EFFECT_RANGE_CASTING_IMPACT) {
			duration = 32;
		} else if((mode >= CASTING_IMPACT_BANE &&
					  mode <= CASTING_IMPACT_FEVER) ||
			mode == CASTING_IMPACT_TERRA_STRIKE ||
			mode == CASTING_IMPACT_POISON_CLOUD ||
			mode == CASTING_IMPACT_DEADLY_POISON ||
			mode == CASTING_IMPACT_HEAT_MIRAGE ||
			mode == CASTING_IMPACT_ACID_BATH || mode == CASTING_IMPACT_MURK ||
			mode == CASTING_IMPACT_PSYPHON_SEAL ||
			mode == CASTING_IMPACT_AQUA_SOCK) {
			duration = 48;
		} else if(mode == CASTING_IMPACT_MELTDOWN) {
			duration = 20;
		} else if(mode == CASTING_IMPACT_MORTAL_DANGER ||
			mode == CASTING_IMPACT_ASURA || mode == CASTING_IMPACT_BROIL) {
			duration = 40;
		} else if(mode == CASTING_IMPACT_BLINDING_SMOG ||
			mode == CASTING_IMPACT_BAD_OMEN) {
			duration = 64;
		} else {
			duration = 80;
		}
		/* Phase 3, the impact, for duration ticks. */
		for(frm = 0; frm != duration; frm++) {
			if(mode != CASTING_IMPACT_MORTAL_DANGER) {
				if(mode != CASTING_IMPACT_ASURA) {
					FIXED base;
					scanline = state->bg2xLines;
					i = 0;
					base = toFIXED(4);
					phase = frm * (TRIG_TURN / 16);
					for(; i != DISPLAY_HEIGHT; i++) {
						Sint32 offset =
							CANVAS_WAVE_BG2X(base - sySin(phase) * 4);
						*scanline++ = offset;
						phase += TRIG_TURN / 32;
					}
				}
			}
			/* Follow the caster for the first three ticks. */
			if(frm <= 2) {
				btProjectionScaledUnit(state->args->user, &casterPos);
				casterPos.x = casterPos.x / 2;
				casterPos.y += 16;
			}
			if(mode != CASTING_IMPACT_MORTAL_DANGER &&
				mode != CASTING_IMPACT_TERRA_STRIKE &&
				mode != CASTING_IMPACT_ASURA &&
				mode != CASTING_IMPACT_FORCIBLE_ARM &&
				mode != CASTING_IMPACT_TRUNCHEON_FIST) {
				if(frm <= 11) {
					if(state->args->side == EFFECT_SIDE_ENEMIES) {
						blitters[0](canvas,
							state->sheet + frm / 2 * PICTURE_BYTES(FIRE_STREAK),
							casterPos.x - 32, casterPos.y - 40,
							FIRE_STREAK_WIDTH, FIRE_STREAK_HEIGHT);
					} else {
						blitters[0](canvas,
							state->sheet + frm / 2 * PICTURE_BYTES(FIRE_STREAK),
							casterPos.x, casterPos.y - 40, FIRE_STREAK_WIDTH,
							FIRE_STREAK_HEIGHT);
					}
				}
			}
			/* Step the background palette towards the impact's colours. */
			switch(mode) {
			case CASTING_IMPACT_FORCIBLE_ARM:
				btEffectStepPaletteToResource(RESOURCE_FLAME_SHEET_A);
				break;
			case CASTING_IMPACT_AQUA_SOCK:
				btEffectStepPaletteToResource(RESOURCE_BLUE_FLAME_SHEET);
				break;
			case CASTING_IMPACT_PSYPHON_SEAL:
				btEffectStepPaletteToResource(RESOURCE_RUNE_SHEET);
				break;
			case CASTING_IMPACT_TERRA_STRIKE:
				btEffectStepPaletteToResource(RESOURCE_ROCK_SPIRE_SHEET);
				break;
			case CASTING_IMPACT_BANE:
			case CASTING_IMPACT_DEADLY_POISON:
				btEffectStepPaletteToResource(RESOURCE_MARS_DJINN_SHEET);
				break;
			case CASTING_IMPACT_BAD_OMEN:
			case CASTING_IMPACT_LIFE_NOURISH:
			case CASTING_IMPACT_ACID_BATH:
				btEffectStepPaletteToResource(RESOURCE_PINK_BURST_SHEET);
				break;
			case CASTING_IMPACT_LIFE_LEECH:
				btEffectStepPaletteToResource(RESOURCE_ICE_BLOCK_SHEET);
				break;
			case CASTING_IMPACT_PSY_LEECH:
				btEffectStepPaletteToResource(RESOURCE_BLAST_SHEET);
				break;
			case CASTING_IMPACT_MIST:
			case CASTING_IMPACT_MURK:
				btEffectStepPaletteToResource(RESOURCE_TAN_PALETTE);
				break;
			case CASTING_IMPACT_SMOG:
			case CASTING_IMPACT_BLINDING_SMOG:
				btEffectStepPaletteToResource(RESOURCE_CYAN_PALETTE);
				break;
			case CASTING_IMPACT_FEVER:
			case CASTING_IMPACT_BROIL:
			case CASTING_IMPACT_HEAT_MIRAGE:
				btEffectStepPaletteToResource(RESOURCE_EMBER_STREAK_SHEET);
				break;
			case CASTING_IMPACT_POISON_CLOUD:
				btEffectStepPaletteToResource(RESOURCE_LIME_PALETTE);
				break;
			case CASTING_IMPACT_SCORCH:
			case CASTING_IMPACT_BARRAGE:
				btEffectStepPaletteToResource(RESOURCE_FLASH_BURST_SHEET);
				break;
			}
			/* The orb flashes over the target and sparks scatter. */
			if(mode != CASTING_IMPACT_MORTAL_DANGER &&
				mode != CASTING_IMPACT_TERRA_STRIKE &&
				mode != CASTING_IMPACT_ASURA) {
				if(frm >= 4 && frm <= 15) {
					blitters[1](canvas,
						state->sheet +
							(frm - 4) / 2 * PICTURE_BYTES(YELLOW_ORB_PACKED) +
							CASTING_IMPACT_ORB_PICTURES,
						tgtPos.x / 2 - 8, casterPos.y - 24,
						YELLOW_ORB_PACKED_WIDTH, YELLOW_ORB_PACKED_HEIGHT);
				}
				EFFECT_VIEW_FROM_CAMERA(camera);
				if(frm > 3) {
					for(i = 0; i != CASTING_IMPACT_SPARKS * 2; i++) {
						Sint32	  idx = i / 2;
						PARTICLE *spark = &state->particles[idx];
						Sint32	  size = spark->variant;
						if(size > 0) {
							btProjectionPoint(spark, &projected);
							size >>= 4;
							size++;
							projected.x /= 2;
							EFFECT_DRAW_MOTE(blitters[idx & 1], canvas, sprs,
								projected.x, projected.y, size);
							btParticleAdvanceWithGravity3D(
								spark, 60, -toFIXED(1) / 16);
							spark->variant--;
						}
					}
				}
			}
			/* The impact's own effect. */
			if(mode == CASTING_IMPACT_SAP ||
				mode == CASTING_IMPACT_LIFE_NOURISH ||
				mode == CASTING_IMPACT_LIFE_LEECH ||
				mode == CASTING_IMPACT_PSY_LEECH) {
				/* Motes burst from the target and fly back into the caster, who
				   glows white. */
				if(frm == 50) {
					btEffectTintBattler(state->args->user, SPRITE_COLOR_WHITE,
						TINT_KEEP, TINT_KEEP, 0);
				}
				if(frm == 79) {
					btEffectTintBattler(state->args->user, SPRITE_COLOR_NORMAL,
						TINT_KEEP, TINT_KEEP, 0);
				}
				if(frm == 12) {
					for(i = 0; i != CASTING_IMPACT_PARTICLE_COUNT; i++) {
						particle = &CASTING_IMPACT_PARTICLES[i];
						SET_POSITION(particle, tgtActor->x.fixed, toFIXED(20),
							tgtActor->z.fixed);
						SET_VELOCITY(particle, ((syRandom() & 255) - 128) << 10,
							((syRandom() & 255) - 128) << 10,
							((syRandom() & 255) - 128) << 10);
						particle->variant = 0;
					}
				}
				if(frm > 11) {
					PARTICLE *mote;
					ACTOR	 *caster;
					Sint32	  height;
					caster = btObjectGetStub(state->args->user)->actor;
					height = btUnitGetEffectHeightStub(state->args->user) / 2;
					for(i = 0; i != CASTING_IMPACT_PARTICLE_COUNT / 2; i++) {
						mote = &CASTING_IMPACT_PARTICLES[i];
						if(mote->variant >= 0) {
							Sint32 size = (i & 1) + 6;
							btProjectionPoint(mote, &projected);
							projected.x >>= 1;
							EFFECT_DRAW_MOTE(blitters[0], canvas, sprs,
								projected.x, projected.y, size);
							btParticleAdvanceWithGravity3D(mote, 62, 0);
							if(frm > i + 22) {
								Sint32 dx = (caster->x.fixed - mote->x) >> 8;
								Sint32 dy =
									(caster->y.fixed + height - mote->y) >> 8;
								Sint32 dz = (caster->z.fixed - mote->z) >> 8;
								mote->velX += dx;
								mote->velY += dy;
								mote->velZ += dz;
								if((dx >= -4095 && dx <= 4095) &&
									(dz >= -4095 && dz <= 4095))
									mote->variant = -1;
							}
						}
					}
				}
			} else if(mode == CASTING_IMPACT_MELTDOWN) {
				/* Draw nothing; finish with btEffectRunImpair. */
			} else if(mode == CASTING_IMPACT_SQUALL ||
				mode == CASTING_IMPACT_STUN_VOLTAGE) {
				/* Two bolts strike the target. */
				if(frm >= 6 && frm <= 19) {
					for(i = 0; i != 2; i++) {
						blitters[0](canvas,
							(((frm + i * 3) / 2) & 3) *
									LIGHTNING_BOLT_PICTURE_BYTES +
								(pictures + GLINT_SET_BYTES),
							tgtPos.x / 2 - 8, 0, LIGHTNING_BOLT_WIDTH, 104);
					}
				}
				if(frm >= 8 && frm <= 23) {
					for(i = 0; i != 3; i++) {
						Sint32 image = i & 3;
						Sint32 angle = syRandom() & TRIG_ANGLE_MASK;
						Sint32 imageX;
						Sint32 imageY;
						Sint32 drawFlg;
						imageX = FIXEDtoINT(sySin(angle) << 3) + tgtPos.x / 2 -
							BattleEffectGlintPictureWidths[image] / 2;
						imageY = FIXEDtoINT(syCos(angle) << 5) -
							BattleEffectGlintPictureHeights[image] / 2;
						EFFECT_RELEASE_BLITTERS();
						drawFlg = syRandom();
						EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
							CastingImpactGlintDrawFlags[drawFlg & 3] |
								EFFECT_CLIP_BOUNDS,
							COMPOSITE_LIGHTEN);
						EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED)(canvas,
							pictures + BattleEffectGlintPictures[image], imageX,
							imageY + 56, BattleEffectGlintPictureWidths[image],
							BattleEffectGlintPictureHeights[image]);
						syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
						btEffectBuildCanvasBlitters(
							state->args->side, blitters);
					}
				}
			} else if(mode == CASTING_IMPACT_AQUA_SOCK) {
				/* A flame column falls onto the target. */
				Sint32 fall;
				Sint32 scroll;
				EFFECT_RELEASE_BLITTERS();
				if(frm >= 0 && frm <= 23) {
					DRAW_RECTANGLE *draw;
					Sint32			width;
					Sint32			columnX;
					Sint32			columnY;
					Sint32			top;
					Sint32			originX;

					originX = tgtPos.x / 2;
					fall = frm * 32 - 232;
					scroll = frm * 16 - 48;
					if(fall > 0)
						fall = 0;
					while(scroll > BLUE_FLAME_COLUMN_HEIGHT)
						scroll -= BLUE_FLAME_COLUMN_HEIGHT;
					EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
						EFFECT_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
					draw = (DRAW_RECTANGLE *)HeapSlots;
					draw += HEAP_SLOT_RAM_CODE_PAIRED;
					columnX = originX - 8;
					columnY = fall + scroll;
					top = columnY - BLUE_FLAME_COLUMN_HEIGHT;
					width = BLUE_FLAME_COLUMN_WIDTH;
					EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED)(canvas, pictures,
						columnX, top, width, BLUE_FLAME_COLUMN_HEIGHT);
					EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED)(canvas, pictures,
						columnX, columnY, width,
						BLUE_FLAME_COLUMN_HEIGHT - scroll);
					EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED)(canvas,
						pictures + PICTURE_BYTES(BLUE_FLAME_COLUMN),
						originX - width, fall + 47, BLUE_FLAME_SPLASH_WIDTH,
						BLUE_FLAME_SPLASH_HEIGHT);
					syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
					if(frm == 8) {
						state->shakeFrm = frm;
					}
					if(frm > 1) {
						Sint32	  emitted = 0;
						PARTICLE *spark;
						for(i = 0; i != CASTING_IMPACT_SPARKS; i++) {
							if((spark = &state->particles[i])->variant == 0) {
								SET_POSITION(spark, tgtActor->x.fixed,
									toFIXED(20), tgtActor->z.fixed);
								SET_VELOCITY(spark,
									((syRandom() & 255) - 127) << 12,
									((syRandom() & 255) - 64) << 10,
									((syRandom() & 255) - 127) << 12);
								spark->variant =
									i / 2 + CASTING_IMPACT_SPARK_LIFE;
								emitted++;
								if(emitted == 4)
									break;
							}
						}
					}
				}
				btEffectBuildCanvasBlitters(state->args->side, blitters);
			} else if(mode == CASTING_IMPACT_PSYPHON_SEAL) {
				Sint32 height;
				EFFECT_RELEASE_BLITTERS();
				/* The rune's two halves, the second mirrored. */
				if(frm >= 4 && frm <= 23) {
					pairX = tgtPos.x / 2;
					height = RUNE_HALF_HEIGHT;
					EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
						EFFECT_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
					slotDraw = (DRAW_RECTANGLE *)HeapSlots;
					slotDraw[HEAP_SLOT_RAM_CODE_PAIRED](canvas, pictures,
						pairX - RUNE_HALF_WIDTH, 48, RUNE_HALF_WIDTH, height);
					syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
					EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
						EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
						COMPOSITE_LIGHTEN);
					EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED)(
						canvas, pictures, pairX, 48, RUNE_HALF_WIDTH, height);
					syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
				}
				btEffectBuildCanvasBlitters(state->args->side, blitters);
			} else if(mode == CASTING_IMPACT_CYCLONE_SLASH) {
				/* A tornado stands over the target. */
				Sint32 image;
				if(frm > 15) {
					REG_BLDALPHA = EFFECT_BLEND_CANVAS(32 - frm);
				}
				if(frm > 5) {
					stripX = tgtPos.x / 2 - TORNADO_STRIP_WIDTH / 2;
					image = frm / 2 % 3;
					pictureOfs = image * (2 * PICTURE_BYTES(TORNADO_STRIP));
					blitters[0](canvas, pictures + GLINT_SET_BYTES + pictureOfs,
						stripX, 16, TORNADO_STRIP_WIDTH, TORNADO_STRIP_HEIGHT);
					blitters[0](canvas,
						image * PICTURE_BYTES(TORNADO_STRIP) +
							(pictures + GLINT_SET_BYTES +
								6 * PICTURE_BYTES(TORNADO_STRIP)),
						stripX, 48, TORNADO_STRIP_WIDTH, TORNADO_STRIP_HEIGHT);
					blitters[0](canvas,
						pictureOfs +
							(pictures + GLINT_SET_BYTES +
								PICTURE_BYTES(TORNADO_STRIP)),
						stripX, 80, TORNADO_STRIP_WIDTH, TORNADO_STRIP_HEIGHT);
				}
			} else if(mode == CASTING_IMPACT_SCORCH ||
				mode == CASTING_IMPACT_BARRAGE) {
				PARTICLE *burst;
				for(i = 0; i != CASTING_IMPACT_RISING_PICTURES; i++) {
					burst = &CASTING_IMPACT_PARTICLES[i];
					/* Flash bursts rise. */
					if(frm >= i / 2 + 4) {
						Sint32 age = burst->variant;
						if(age < CASTING_IMPACT_BURST_LIFE) {
							Sint32 image = age / 2;
							btProjectionPoint(burst, &projected);
							projected.x /= 2;
							blitters[0](canvas,
								pictures + image * PICTURE_BYTES(FLASH_BURST),
								projected.x - FLASH_BURST_WIDTH / 2,
								projected.y - FLASH_BURST_HEIGHT / 2,
								FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
							btParticleAdvanceWithGravity3D(
								burst, 60, toFIXED(1) / 16);
							burst->variant++;
						}
					}
				}
			} else if(mode == CASTING_IMPACT_FLINT) {
				/* Draw nothing. */
			} else if(mode == CASTING_IMPACT_MORTAL_DANGER) {
				Sint32 imageY;
				Sint32 sway;
				/* Condemn's reaper and crescent. */
				/* DEAD: a sine whose result is dropped; the ROM keeps the
				   call. */
				sySin(frm * (TRIG_TURN / 128));
				sway = FIXEDtoINT(syCos(frm * (TRIG_TURN / 128)) << 2);
				/* tgtPos.y is in whole pixels, so the shift leaves 0 for any
				   target on the screen: the sprites stand at a fixed height. */
				imageY = FIXEDtoINT(tgtPos.y) + sway + 16;
				if(frm <= 3) {
					blitters[0](canvas, state->sheet,
						CASTING_IMPACT_PICTURE_X(state->args->side, 0),
						imageY + ReaperPictureY[0], REAPER_SHEET_RAISED_WIDTH,
						REAPER_SHEET_RAISED_HEIGHT);
				} else {
					if(frm <= 7) {
						blitters[0](canvas, state->sheet,
							CASTING_IMPACT_PICTURE_X(state->args->side, 0),
							imageY + ReaperPictureY[0],
							REAPER_SHEET_RAISED_WIDTH,
							REAPER_SHEET_RAISED_HEIGHT);
					}
					EFFECT_DRAW_PICTURE(blitters[0], canvas, state->sheet,
						REAPER_SHEET_SWUNG,
						CASTING_IMPACT_PICTURE_X(state->args->side, 1),
						imageY + ReaperPictureY[1]);
					if(frm == 4 || frm == 5) {
						syWordsFill(canvas, CANVAS_BYTES,
							CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
					}
					if(frm == 6 || frm == 7) {
						EFFECT_DRAW_PICTURE(blitters[0], canvas, state->sheet,
							REAPER_SHEET_CRESCENT,
							CASTING_IMPACT_PICTURE_X(state->args->side, 2),
							imageY + ReaperPictureY[2]);
					}
					if(frm == 8 || frm == 9) {
						EFFECT_DRAW_PICTURE(blitters[0], canvas, pictures,
							RED_CRESCENT_FADE_A,
							CASTING_IMPACT_PICTURE_X(state->args->side, 3),
							imageY + ReaperPictureY[3]);
					}
					if(frm == 10 || frm == 11) {
						EFFECT_DRAW_PICTURE(blitters[0], canvas, pictures,
							RED_CRESCENT_FADE_B,
							CASTING_IMPACT_PICTURE_X(state->args->side, 4),
							imageY + ReaperPictureY[4]);
					}
					if(frm == 12 || frm == 13) {
						EFFECT_DRAW_PICTURE(blitters[0], canvas, pictures,
							RED_CRESCENT_FADE_C,
							CASTING_IMPACT_PICTURE_X(state->args->side, 5),
							imageY + ReaperPictureY[5]);
					}
					if(frm == 14 || frm == 15) {
						EFFECT_DRAW_PICTURE(blitters[0], canvas, pictures,
							RED_CRESCENT_FADE_D,
							CASTING_IMPACT_PICTURE_X(state->args->side, 6),
							imageY + ReaperPictureY[6]);
					}
				}
			} else if(mode == CASTING_IMPACT_ASURA) {
				/* The swordsman slides in and swings, and his red crescent
				   fades. */
				scrollPos += scrollSpeed;
				if(frm > 6) {
					scrollSpeed = scrollSpeed * 48 / DAMPING_SCALE;
				}
				REG_BG2X = FIXEDtoINT(scrollPos) << 8;
				{
					Sint32 fade;
					fade = frm - 16;
					if(fade >= 0 && fade <= 15) {
						REG_BLDALPHA =
							EFFECT_BLEND_CANVAS(BLEND_WEIGHT_MAX - fade);
					}
				}
				if(frm == 4 || frm == 5) {
					syWordsFill(canvas, CANVAS_BYTES,
						CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
				}
				if(frm <= 3) {
					if(state->args->side == EFFECT_SIDE_PARTY) {
						blitters[0](canvas, state->imageStorage, 0, 24,
							ASURA_SHEET_LOW_WIDTH, ASURA_LOW_DRAW_HEIGHT);
					} else {
						blitters[0](canvas, state->imageStorage, 48, 24,
							ASURA_SHEET_LOW_WIDTH, ASURA_LOW_DRAW_HEIGHT);
					}
				} else {
					if(frm <= 7) {
						if(state->args->side == EFFECT_SIDE_PARTY) {
							blitters[0](canvas, state->imageStorage, 0, 24,
								ASURA_SHEET_LOW_WIDTH, ASURA_LOW_DRAW_HEIGHT);
						} else {
							blitters[0](canvas, state->imageStorage, 48, 24,
								ASURA_SHEET_LOW_WIDTH, ASURA_LOW_DRAW_HEIGHT);
						}
					}
					if(state->args->side == EFFECT_SIDE_PARTY) {
						blitters[0](canvas,
							state->imageStorage + ASURA_SHEET_SWUNG, 16, 16,
							ASURA_SHEET_SWUNG_WIDTH, ASURA_SHEET_SWUNG_HEIGHT);
					} else {
						blitters[0](canvas,
							state->imageStorage + ASURA_SHEET_SWUNG, 32, 16,
							ASURA_SHEET_SWUNG_WIDTH, ASURA_SHEET_SWUNG_HEIGHT);
					}
					if(frm == 6 || frm == 7) {
						blitters[0](canvas,
							state->imageStorage + ASURA_SHEET_CRESCENT, 0, 16,
							ASURA_SHEET_CRESCENT_WIDTH,
							ASURA_SHEET_CRESCENT_HEIGHT);
					}
					if(frm == 8 || frm == 9) {
						blitters[0](canvas, pictures, 0, 16, CANVAS_SIZE,
							ASURA_CRESCENT_FADE_A_HEIGHT);
					}
					if(frm == 10 || frm == 11) {
						blitters[0](canvas, pictures + ASURA_CRESCENT_FADE_B, 0,
							16, CANVAS_SIZE, ASURA_CRESCENT_FADE_B_HEIGHT);
					}
					if(frm == 12 || frm == 13) {
						blitters[0](canvas, pictures + ASURA_CRESCENT_FADE_C, 0,
							16, CANVAS_SIZE, ASURA_CRESCENT_FADE_C_HEIGHT);
					}
					if(frm == 14 || frm == 15) {
						blitters[0](canvas, pictures + ASURA_CRESCENT_FADE_D, 0,
							16, CANVAS_SIZE, ASURA_CRESCENT_FADE_D_HEIGHT);
					}
				}
			} else if(mode == CASTING_IMPACT_BROIL) {
				/* Ember streaks. */
				for(i = 0; i != BROIL_STREAKS; i++) {
					if(frm >= i + 6 && frm < i + 18) {
						Sint32 image = (frm - i - 6) / 2;
						Sint32 imageX = tgtPos.x / 2 -
							BattleEffectPuffArcPictureWidths[image] / 2;
						Sint32 mirrored;
						if(i & 1)
							imageX += (1 + i) / 2 * 3;
						else
							imageX -= (i + 1) / 2 * 3;
						mirrored = 1;
						if(i != 0) {
							mirrored = 0;
							if(((i - 1) & 3) > 1)
								mirrored = 1;
						}
						blitters[mirrored](canvas,
							pictures + BattleEffectPuffArcPictures[image],
							imageX, BattleEffectPuffArcPictureBiasY[image] + 48,
							BattleEffectPuffArcPictureWidths[image],
							BattleEffectPuffArcPictureHeights[image]);
					}
				}
			} else if(mode == CASTING_IMPACT_FROST_BITE) {
				/* Ice chips close in on the target. */
				if(frm == 0) {
					for(i = 0; i != CASTING_IMPACT_PARTICLE_COUNT; i++) {
						particle = &CASTING_IMPACT_PARTICLES[i];
						SET_POSITION(particle, (syRandom() & 127) + 32, 0, 0);
						SET_VELOCITY(particle, syRandom() & TRIG_ANGLE_MASK,
							syRandom() & TRIG_ANGLE_MASK, syRandom() & 0xffff);
					}
					/* Chip 63's y, not its radius, is set to 159, so it flies
					   up to 159 pixels off its line and the target gives the
					   last flinch with no chip on it, as the last chip crosses
					   the sky above. XXX (no evidence): whether its radius was
					   meant, as 159 is the largest the others draw (32 + 127);
					   only the author's intent would say. */
					CASTING_IMPACT_PARTICLES[CASTING_IMPACT_PARTICLE_COUNT - 1]
						.y = 159;
				}
				{
					PARTICLE *chip;
					for(i = 0; i != CASTING_IMPACT_PARTICLE_COUNT; i++) {
						chip = &CASTING_IMPACT_PARTICLES[i];
						if(chip->x >= 0 && frm >= i / 2) {
							Sint32 image = i & 3;
							syInitMatrix();
							syRotX(chip->velX);
							syRotY(chip->velY);
							btProjectionPoint(chip, &projected);
							projected.x = projected.x / 2 + tgtPos.x / 2;
							projected.y += tgtPos.y + 32;
							blitters[1](canvas,
								pictures + CastingImpactIceChipPictures[image],
								projected.x - 4, projected.y - 4, 8, 8);
							chip->x -= 6;
							if(chip->x < 0 &&
								((i & 7) == 0 ||
									i == CASTING_IMPACT_PARTICLE_COUNT - 1)) {
								sySndPlayStub(SE_IMPACT);
								btEffectTintBattler(state->args->tgts[0],
									SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 4);
							}
						}
					}
				}
			} else if(mode == CASTING_IMPACT_TERRA_STRIKE) {
				Sint32 height;
				/* A rock spire rises under the target and sinks. */
				if(frm >= 5 && frm <= 49) {
					if(frm > 25) {
						height = 196 - (frm << 2);
					} else {
						height = (frm << 4) - 64;
					}
					if(height > ROCK_SPIRE_HEIGHT) {
						height = ROCK_SPIRE_HEIGHT;
					}
					blitters[0](canvas, pictures, 48, 104 - height,
						ROCK_SPIRE_WIDTH, height);
				}
			} else if(mode == CASTING_IMPACT_FORCIBLE_ARM ||
				mode == CASTING_IMPACT_TRUNCHEON_FIST) {
				/* A flame falls diagonally onto the target over six ticks. */
				Sint32 imageX;
				Sint32 imageY;
				if(frm <= 5) {
					if(state->args->side == EFFECT_SIDE_ENEMIES) {
						imageX = tgtPos.x / 2 + (6 - frm) * 6;
						imageY = tgtPos.y - (6 - frm) * 12 + 24;
					} else {
						imageX = tgtPos.x / 2 - (6 - frm) * 6;
						imageY = tgtPos.y - (6 - frm) * 12 + 24;
					}
					blitters[1](canvas, pictures,
						imageX - FALLING_FLAME_WIDTH / 2,
						imageY - FALLING_FLAME_HEIGHT / 2, FALLING_FLAME_WIDTH,
						FALLING_FLAME_HEIGHT);
				}
			} else if(mode == CASTING_IMPACT_BAD_OMEN) {
				/* Skulls rise from the target and fade. */
				if(frm > 47) {
					REG_BLDALPHA = EFFECT_BLEND_CANVAS(64 - frm);
				}
				{
					PARTICLE *skull;
					for(i = 0; i != CASTING_IMPACT_RISING_PICTURES; i++) {
						Sint32 image = i % 3;

						skull = &CASTING_IMPACT_PARTICLES[i];
						btProjectionPoint(skull, &projected);
						projected.x /= 2;
						blitters[i & 1](canvas,
							pictures + image * PICTURE_BYTES(SKULL),
							projected.x - SKULL_WIDTH / 2,
							projected.y - SKULL_HEIGHT / 2, SKULL_WIDTH,
							SKULL_HEIGHT);
						btParticleAdvanceWithGravity3D(skull, 60,
							1 << ((i & 3) + BAD_OMEN_SKULL_LIFT_SHIFT));
						skull->variant++;
					}
				}
			} else if(mode != EFFECT_RANGE_CASTING_IMPACT) {
				/* Unused: the test leaves impact mode 100 without smoke, a mode
				   this game never dispatches; The Lost Age's twin makes the
				   same test. The smoke impacts: a puff rises each tick. */
				PARTICLE *puff;
				for(i = 0; i != CASTING_IMPACT_RISING_PICTURES; i++) {
					puff = &CASTING_IMPACT_PARTICLES[i];
					if(frm >= i + 4) {
						Sint32 age = puff->variant;
						if(age < CASTING_IMPACT_RISING_LIFE) {
							Sint32 image = age / 4;
							btProjectionPoint(puff, &projected);
							projected.x /= 2;
							blitters[i & 1](canvas,
								pictures + image * PICTURE_BYTES(SMOKE_PUFF),
								projected.x - SMOKE_PUFF_WIDTH / 2,
								projected.y - SMOKE_PUFF_HEIGHT / 2,
								SMOKE_PUFF_WIDTH, SMOKE_PUFF_HEIGHT);
							if(mode == CASTING_IMPACT_ACID_BATH)
								btParticleAdvanceWithGravity3D(
									puff, 60, toFIXED(1) / 64);
							else
								btParticleAdvanceWithGravity3D(
									puff, 60, toFIXED(1) / 16);
							puff->variant++;
						}
					}
				}
			}
			/* From Bane to Sap the Djinni flies to the target in six ticks. */
			if(mode <= CASTING_IMPACT_SAP) {
				if(frm <= 5) {
					btProjectionPoint(&djinniPos, &projected);
					projected.x /= 2;
					djinniX = projected.x;
					blitters[1](canvas,
						pictures + CASTING_IMPACT_DJINNI_PICTURE,
						djinniX - CASTING_IMPACT_DJINNI_WIDTH / 2,
						projected.y - 4, CASTING_IMPACT_DJINNI_WIDTH,
						CASTING_IMPACT_DJINNI_HEIGHT);
					pos->x += vel.x;
					pos->y += vel.y;
					pos->z += vel.z;
				}
			}
			/* The sounds. The damage shows with no hit sound of its own: the
			   crash a tick later is the effect's. */
			if(frm == 3) {
				EFFECT_SHOW_DAMAGE(-1);
			}
			if(frm == 4) {
				sySndPlayStub(SE_HEAVY_IMPACT);
			}
			if(frm == 6) {
				/* Unused: the last test, for impact mode 100, which this game
				   never dispatches; The Lost Age's twin lists mode 100 in the
				   same test. */
				if(mode == CASTING_IMPACT_FLINT ||
					mode == CASTING_IMPACT_SCORCH ||
					mode == CASTING_IMPACT_SAP ||
					mode == CASTING_IMPACT_LIFE_NOURISH ||
					mode == CASTING_IMPACT_LIFE_LEECH ||
					mode == CASTING_IMPACT_PSY_LEECH ||
					mode == CASTING_IMPACT_BARRAGE ||
					mode == CASTING_IMPACT_TRUNCHEON_FIST ||
					mode == EFFECT_RANGE_CASTING_IMPACT) {
					btMotionApplyKnockbackStub(
						state->args->tgts[0], KNOCKBACK_HURL);
					state->shakeFrm = 8;
				} else if(mode == CASTING_IMPACT_BROIL ||
					mode == CASTING_IMPACT_AQUA_SOCK ||
					mode == CASTING_IMPACT_FORCIBLE_ARM) {
					btMotionApplyKnockbackStub(
						state->args->tgts[0], KNOCKBACK_HOP);
					state->shakeFrm = 2;
				} else if(mode == CASTING_IMPACT_CYCLONE_SLASH ||
					mode == CASTING_IMPACT_TERRA_STRIKE) {
					btMotionApplyKnockbackStub(
						state->args->tgts[0], KNOCKBACK_LAUNCH);
					state->shakeFrm = 8;
				}
			}
			if(frm == 6) {
				btEffectTintBattler(state->args->tgts[0], SPRITE_COLOR_WHITE,
					POSE_DAMAGE, 0, 4);
			}
			if(frm == 14) {
				btEffectTintBattler(state->args->tgts[0], SPRITE_COLOR_WHITE,
					POSE_DAMAGE, 0, 4);
			}
			btCameraShake(8, 8);
			EFFECT_END_TICK(state);
		}
	}
	/* Phase 4: Meltdown hands the cleared canvas to the stat reduction
	   (Impair), as btCastingImpactEndPrelude does. */
	if(mode == CASTING_IMPACT_MELTDOWN) {
		syWordsClear(BG_CHAR_BLOCK(1), CANVAS_BYTES);
		syWordsClear(canvas, CANVAS_BYTES);
		state->args->prelude = 0;
		syCloseEvent(btEffectDarkenBackgroundStep);
		syCloseEvent(btEffectTransferCanvas);
		EFFECT_RELEASE_BLITTERS();
		btEffectRunImpair(args);
	} else {
		if(mode == CASTING_IMPACT_MIST || mode == CASTING_IMPACT_FEVER ||
			mode == CASTING_IMPACT_BAD_OMEN ||
			mode == CASTING_IMPACT_HEAT_MIRAGE ||
			mode == CASTING_IMPACT_BLINDING_SMOG ||
			mode == CASTING_IMPACT_MURK) {
			syCloseEvent(btEffectArmBg2AffineHBlankDMA);
		}
		EFFECT_FINISH_CANVAS();
	}
}

#undef CASTING_IMPACT_ORB_PICTURES
#undef CASTING_IMPACT_DJINNI_PICTURE
#undef CASTING_IMPACT_GATHER_TICKS
#undef CASTING_IMPACT_SPARKS
#undef CASTING_IMPACT_SPARK_DEPTH
#undef CASTING_IMPACT_SPARK_LIFE
#undef CASTING_IMPACT_PARTICLES
#undef CASTING_IMPACT_PARTICLE_COUNT
#undef CASTING_IMPACT_PICTURE_X
#undef BAD_OMEN_SKULL_LIFT_SHIFT
