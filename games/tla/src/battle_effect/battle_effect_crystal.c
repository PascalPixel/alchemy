/* Crystal. Part of battle_effect.c. */

#define CRYSTAL_TICKS		  103
/* From this tick the canvas fades out, two sixteenths a tick, from the level
   it is shown at. */
#define CRYSTAL_FADE_TICK	  96
#define CRYSTAL_BLEND_LEVEL	  ((CRYSTAL_TICKS - CRYSTAL_FADE_TICK) * 2)
/* The orbs of light that rise around the target. */
#define CRYSTAL_ORBS		  32
/* Sixteen shaded discs, 2 to 32 pixels across, the orbs' pictures, drawn into
   the map cell buffer; where each starts is kept in the effect's pictures at
   CRYSTAL_DISC_OFFSETS. */
#define CRYSTAL_DISCS		  16
#define CRYSTAL_DISC_OFFSETS  10000
/* Where the cog's picture is made in the map cell buffer: CogTexture's quarter
   mirrored into a picture twice as wide and as tall. */
#define CRYSTAL_COG_PICTURE	  16384
/* The cog's largest scale, about 1.74, which the model is drawn at half of. */
#define CRYSTAL_COG_SCALE_MAX 114000
/* The cog's tilt about the x axis, about 16 degrees. */
#define CRYSTAL_COG_TILT	  3000

/* Where VenusDjinnSmallSheet is decoded in the effect's pictures. */
#define CRYSTAL_VENUS_DJINN_SMALL_PICTURES 26048

/* A Venus Djinni appears and its small sprite dives onto the target; a glowing
   cog spins and grows flat on the ground under the targets while orbs of light
   rise around them, pulsing, and each target flashes white as it is healed.

   Abilities: Crystal. */
void btEffectRunCrystal(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	/* LEFTOVER(frame): draw[1], the pair's idle second blitter, which the ROM's
	 * tick reserves and never touches; as one DRAW_RECTANGLE. */
	DRAW_RECTANGLE draw[2];
	Uint16		  *discOffsets;
	Sint32		   center[3];
	Sint32		   other[3];
	SCREEN_POINT   screen;
	MODEL_TEXTURE  texture;
	Sint32		   frm;
	Sint32		   i;
	Sint32		   size;
	Sint32		   ofs;
	Uint8		  *cells;

	state = RamHeapSlots->battleEff;
	canvas = RamHeapSlots->battleCanvas;
	discOffsets = (Uint16 *)(state->sheet + CRYSTAL_DISC_OFFSETS);
	cells = MapGridMetatileBuffer;
	syWordsClear(cells, 32768);
	for(ofs = 0, i = 0, size = 2; i != CRYSTAL_DISCS; size += 2, i++) {
		/* Each disc is drawn after the one before it in the map cell buffer. */
		btEffectDrawShadedCircles(
			(Uint8 *)(ofs + (Sint32)cells), size, toFIXED(1));
		discOffsets[i] = ofs;
		ofs += (i * 2 + 3) * size / 2;
	}
	btResourceLoadAndDecompress(RESOURCE_COG_TEXTURE, state->sheet, 0, 0);
	btPictureMirrorIntoQuadrants(
		state->sheet, RamMapMetatileBuffer + CRYSTAL_COG_PICTURE, 64, 64);
	btEffectStartCanvasLayer(0);
	btEffectRunDjinniEntrance(args,
		DJINNI_ENTRANCE(ELEMENT_VENUS, args->side ^ 1, DJINNI_ANCHOR_TARGET_X),
		center, other);
	btResourceLoadAndDecompress(RESOURCE_VENUS_DJINN_SMALL_SHEET,
		state->sheet + CRYSTAL_VENUS_DJINN_SMALL_PICTURES, 1, 0);
	syWordsCopyEntry((void *)BG_PLTT,
		syResourceGetTableEntry(RESOURCE_BLAST_SHEET), SHEET_PALETTE_BYTES);
	REG_BLDALPHA = EFFECT_BLEND_CANVAS(CRYSTAL_BLEND_LEVEL);
	if(args->side == EFFECT_SIDE_PARTY) {
		EFFECT_LOAD_BLITTER(
			HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
	} else {
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
			EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_ADDITIVE);
	}
	draw[0] = RamHeapSlots->ramCode;
	{
		BATTLE_CAMERA *camera;
		MOTION_OBJECT *tgt;

		EFFECT_SET_TRANSFER(
			state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
		sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);
		camera = RamHeapSlots->cameraState;
		tgt = btObjectGetStub(args->tgts[0])->obj;

		for(frm = 0; frm != CRYSTAL_TICKS; frm++) {
			/* The orbs start in a ring around the first target: velocityX holds
			   the phase of an orb's pulse, velocityY its rising speed,
			   velocityZ its size. */
			if(frm == 0) {
				PARTICLE *orb;
				for(i = 0, orb = state->particles; i != CRYSTAL_ORBS;
					i++, orb++) {
					Sint32 ang = syRandom();
					Sint32 radius = (syRandom() & 63) + 80;

					SET_POSITION(orb, tgt->x + ((sySin(ang) * radius) >> 1),
						toFIXED(8), (syCos(ang) * radius) >> 1);
					orb->velX = syRandom();
					orb->velY = (syRandom() & (toFIXED(1) - 1)) + toFIXED(4);
					orb->velZ = (syRandom() & 7) + 4;
					orb->variant = -1;
				}
			}
			if(frm >= CRYSTAL_FADE_TICK) {
				REG_BLDALPHA = EFFECT_BLEND_CANVAS(CRYSTAL_TICKS * 2 - frm * 2);
			}
			syInitMatrix();
			syLookAtLoad(camera->eye, camera->focus);

			/* The small Djinni dives, swaying, onto the target, drawn twice
			   over its first four ticks. */
			if(frm <= 19) {
				Sint32 x = (FIXEDtoINT(sySin(frm * (TRIG_TURN / 32)) * 8)) +
					center[0] / 2 - 10;
				Sint32 y = (FIXEDtoINT(syCos(frm * (TRIG_TURN / 32)) * 4)) +
					center[1] - 24;

				y += frm * 3;

				draw[0](canvas,
					state->sheet + CRYSTAL_VENUS_DJINN_SMALL_PICTURES, x, y,
					SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
				if(frm <= 3) {
					draw[0](canvas,
						state->sheet + CRYSTAL_VENUS_DJINN_SMALL_PICTURES, x, y,
						SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
				}
			}

			/* The orbs rise, one more every two ticks from tick 48, their
			   sizes pulsing. */
			if(frm > 47) {
				PARTICLE *orb;
				for(i = 0, orb = state->particles; i != CRYSTAL_ORBS;
					i++, orb++) {
					if(frm >= i * 2 + 48) {
						Sint32 width;

						btEffectPosProjectPoint(&orb->x, &screen);
						screen.x >>= 1;
						width = orb->velZ;
						width += FIXEDtoINT(sySin(orb->velX) * width / 2);
						orb->velX += toFIXED(1) / 64;
						width *= 2;
						if((Uint32)(width - 2) <= 61) {
							Sint32 disc = (width - 2) / 2;
							Sint32 x = screen.x;
							Sint32 y = screen.y;

							/* DEAD: never true, as every orb's variant stays
							   -1; the ROM keeps the test and the jitter. */
							if(orb->variant == 0) {
								x += (syRandom() & 7) - 4;
								y += (syRandom() & 7) - 4;
							}
							btCanvasBlitMirroredQuarters(
								(Uint8 *)((Sint16)discOffsets[disc] +
									(Sint32)cells),
								x, y, width);
						}
						orb->y += orb->velY;
					}
				}
			}

			EFFECT_FOR_EACH_TARGET(i, args) {
				if(frm == i * 4 + 64) {
					sySndPlayStub(SE_RECOVERY);
					btEffectTintBattler(
						args->tgts[i], SPRITE_COLOR_WHITE, TINT_KEEP, i, 24);
				}
			}

			/* The cog, spinning and growing from tick 16. */
			syInitMatrix();
			ProjectionWork.centerY = 104;
			{
				void		 *vertices = syHeapAllocEWRAM(32);
				RENDER_MODEL *model = btRenderAllocateModelList(1);

				model->kind = MODEL_SHADED_MAX;
				model->texture = &texture;
				model->mesh = BattleEffectLargeSquareMesh;
				model->vertices = vertices;
				texture.logWidth = 7;
				texture.logHeight = 7;
				texture.pixels = RamMapMetatileBuffer + CRYSTAL_COG_PICTURE;
				if(frm > 15) {
					FIXED scale = frm * (toFIXED(1) / 16) - toFIXED(1);

					if(scale > CRYSTAL_COG_SCALE_MAX) {
						scale = CRYSTAL_COG_SCALE_MAX;
					}
					model->shade = 0;
					syRotX(-CRYSTAL_COG_TILT);
					syTranslate(INTtoFIXED(center[0] / 2 - CANVAS_SIZE / 2),
						toFIXED(40), 0);
					syMatrixScaleUniform(scale >> 1);
					syRotY(frm * (TRIG_TURN / 128));
					btRenderTransformVertices(
						BattleEffectGroundSquareCorners, vertices, 4);
					btRenderDrawModelList(model);
				}
				syHeapRelease(model);
				syHeapRelease(vertices);
			}
			ProjectionWork.centerX = BATTLE_PROJECTION_CENTER_X;
			ProjectionWork.centerY = BATTLE_PROJECTION_CENTER_Y;
			EFFECT_END_TICK(state);
		}
	}
	EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#undef CRYSTAL_TICKS
#undef CRYSTAL_FADE_TICK
#undef CRYSTAL_BLEND_LEVEL
#undef CRYSTAL_ORBS
#undef CRYSTAL_DISCS
#undef CRYSTAL_DISC_OFFSETS
#undef CRYSTAL_COG_PICTURE
#undef CRYSTAL_COG_SCALE_MAX
#undef CRYSTAL_COG_TILT
#undef CRYSTAL_VENUS_DJINN_SMALL_PICTURES
