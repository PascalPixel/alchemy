/* The walls of Berserk, Spasm and Recovery. Part of battle_effect.c. */

#define MIRRORED_WALL_TICKS 48 /* ticks of the wall */

/* The mirrored wall's modes (btEffectRunMirroredWall), each named for the
   effect that plays it. */
typedef enum MIRRORED_WALL_MODE {
	MIRRORED_WALL_SPASM,   /* actual size, a gentle shake for 32 ticks */
	MIRRORED_WALL_BERSERK, /* 1.25 times as wide, a hard shake for 8 ticks */
	/* 1.25 times as wide, no shake, in the Fireball colours */
	MIRRORED_WALL_RECOVERY
} MIRRORED_WALL_MODE;

/* Builds the two usual canvas blitters, the lightening one and the additive
   one, both drawing mirrored when flip is nonzero. */
void btEffectBuildCanvasBlitters(Sint32 flip, BLITTER_PAIR out)
{
	if(flip == 0) {
		EFFECT_LOAD_BLITTER(
			HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
		out[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
		EFFECT_LOAD_BLITTER(
			HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS, COMPOSITE_ADDITIVE);
		out[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
	} else {
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
			EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_LIGHTEN);
		out[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
			EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_ADDITIVE);
		out[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
	}
}

/* The wall rises wide and shakes the screen hard.

   Abilities: Berserk. */
void btEffectRunBerserk(BATTLE_EFFECT_PARM *args)
{
	btEffectRunMirroredWall(args, MIRRORED_WALL_BERSERK);
}

/* The wall rises with a long gentle shake and the recovery sound.

   Abilities: Spasm. */
void btEffectRunSpasm(BATTLE_EFFECT_PARM *args)
{
	btEffectRunMirroredWall(args, MIRRORED_WALL_SPASM);
}

/* A wide wall of fire rises without shaking the screen.

   Abilities: Recovery. */
void btEffectRunRecovery(BATTLE_EFFECT_PARM *args)
{
	btEffectRunMirroredWall(args, MIRRORED_WALL_RECOVERY);
}

/* Raises a mirrored wall at the first target's feet while the target glows. */
void btEffectRunMirroredWall(BATTLE_EFFECT_PARM *args, Sint32 mode)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Uint8				*resource;
	Sint32				 x;
	SCREEN_POINT		 screenPos;
	BLITTER_PAIR		 draw;
	Sint32				 frm;
	Sint32				 picture;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	state->args = args;
	btEffectStartCanvasLayer(0);

	resource = syResourceGetTableEntry(RESOURCE_EARTH_WALL_SHEET);
	syWordsCopy(BG_PLTT, resource, SHEET_PALETTE_BYTES);
	resource += SHEET_PALETTE_BYTES;
	syDecompressTaggedLZ(resource, state->sheet);
	resource = syResourceGetTableEntry(RESOURCE_MARS_DJINN_SHEET);
	syWordsCopy(BG_PLTT, resource, SHEET_PALETTE_BYTES);
	if(mode == MIRRORED_WALL_RECOVERY) {
		resource = syResourceGetTableEntry(RESOURCE_FIREBALL_SHEET);
		syWordsCopy(BG_PLTT, resource, SHEET_PALETTE_BYTES);
	}

	btProjectionUnit(state->args->tgts[0], &screenPos);
	if(mode == MIRRORED_WALL_SPASM) {
		REG_BG2PA = Q_8_8(1);
		x = CANVAS_SIZE / 2 - screenPos.x;
		REG_BG2X = x << 8;
	} else {
		REG_BG2PA = Q_8_8(0.8);
		x = (-screenPos.x * 4) / 5 + CANVAS_SIZE / 2;
		REG_BG2X = x << 8;
	}

	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
	draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
		EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_LIGHTEN);
	draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

	if(mode == MIRRORED_WALL_RECOVERY) {
		state->shakeFrm = 0;
		sySndPlayStub(SE_POWER_UP);
	} else if(mode == MIRRORED_WALL_BERSERK) {
		state->shakeFrm = 8;
		sySndPlayStub(SE_POWER_UP);
	} else {
		state->shakeFrm = 32;
	}

	for(frm = 0; frm != MIRRORED_WALL_TICKS; frm++) {
		if(frm == 0) {
			if(mode == MIRRORED_WALL_RECOVERY)
				btEffectTintBattler(
					state->args->tgts[0], SPRITE_COLOR_WHITE, TINT_KEEP, 0, 32);
			else
				btEffectTintBattler(state->args->tgts[0], SPRITE_COLOR_RED_TINT,
					TINT_KEEP, 0, 32);
		}
		if(frm == 24)
			EFFECT_SHOW_DAMAGE(0);
		if(frm == 8 && mode == MIRRORED_WALL_SPASM)
			sySndPlayStub(SE_RECOVERY);

		/* The wall rises through its first three pictures, then flickers
		   between the top two; each half is drawn mirrored about the canvas's
		   middle. */
		if(frm <= 31) {
			picture = frm / 4;
			if(picture > 2)
				picture = (picture & 1) + 1;
			if(frm <= 27) {
				draw[0](canvas, state->sheet + MirroredWallPictures[picture],
					CANVAS_SIZE / 2 - MirroredWallPictureWidths[picture],
					(screenPos.y - MirroredWallPictureHeights[picture]) + 8,
					MirroredWallPictureWidths[picture],
					MirroredWallPictureHeights[picture]);
				draw[1](canvas, state->sheet + MirroredWallPictures[picture],
					CANVAS_SIZE / 2,
					(screenPos.y - MirroredWallPictureHeights[picture]) + 8,
					MirroredWallPictureWidths[picture],
					MirroredWallPictureHeights[picture]);
			}
		}
		if(mode == MIRRORED_WALL_SPASM)
			btCameraShake(2, 2);
		else
			btCameraShake(16, 16);
		EFFECT_END_TICK(state);
	}

	EFFECT_FINISH_CANVAS();
}
