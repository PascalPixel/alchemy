/* Fire Puppet and Whiplash. Part of battle_effect.c. */

#define FIRE_PUPPET_TICKS	  103
/* The ghost's trail is PUPPET_SEGMENTS pairs of corners, laid in the map cell
   buffer: the corners, those btRenderTransformVertices makes of them at
   PUPPET_STRIP_VERTICES, and the trail's mesh at PUPPET_STRIP_MESH, whose quads
   each take 4 texels of StreakTexture across and its 32 down. */
#define PUPPET_SEGMENTS		  33
#define PUPPET_CORNERS		  (PUPPET_SEGMENTS * 2)
#define PUPPET_QUADS		  (PUPPET_SEGMENTS - 1)
#define PUPPET_STRIP_VERTICES (RamMapMetatileBuffer + 4096)
#define PUPPET_STRIP_MESH	  (RamMapMetatileBuffer + 8192)
/* The last particle keeps a copy of the target's place and scale. */
#define PUPPET_TARGET_COPY	  (state->particles[ARRAY_COUNT(state->particles) - 1])

/* The ghost rises at PUPPET_RISE_TICK, its trail curling a further
   PUPPET_CURL_SPEED a segment each tick up to PUPPET_CURL_MAX; it strikes at
   PUPPET_STRIKE_TICK, and from PUPPET_RETURN_TICK it uncurls and shrinks back
   into the caster over PUPPET_RETURN_TICKS. */
#define PUPPET_RISE_TICK		16
#define PUPPET_CURL_SPEED		44
#define PUPPET_CURL_MAX			1424
#define PUPPET_STRIKE_TICK		54
#define PUPPET_RETURN_TICK		74
#define PUPPET_RETURN_TICKS		32
/* The caster kneels, grey, until this tick. */
#define PUPPET_CASTER_RISE_TICK 100
/* The skull grows from half size by this much a step of the curl, to about full
   size at PUPPET_CURL_MAX. */
#define PUPPET_SKULL_GROWTH		23

/* The trail is PUPPET_WIDTH wide, pinched by up to PUPPET_PINCH along a sine
   that runs PUPPET_PINCH_STEP a segment, starting five segments before the
   caster: 33 pixels at the caster, 6 at its narrowest, near the middle, and 47
   at the skull. */
#define PUPPET_WIDTH	   50
#define PUPPET_PINCH	   44
#define PUPPET_PINCH_STEP  862
#define PUPPET_PINCH_START (5 * PUPPET_PINCH_STEP)
/* The trail sways up to PUPPET_SWAY pixels at the skull, less towards the
   caster, a wave that runs along it a segment a tick. */
#define PUPPET_SWAY		   16
/* The skull's turn at the trail's end, against the trail's own turn there. */
#define PUPPET_SKULL_TURN  22384

/* Where GhostSkullTexture is decoded in the effect's pictures. */
#define FIRE_PUPPET_GHOST_SKULL 16384

/* The caster kneels, grey, and its ghost rises from it: a trail of blue streaks
   that curls up and over the battle with a skull at its head. The skull dives
   on the targets, the screen flashes white and every target is thrown back;
   then the ghost shrinks back into the caster, who stands again.

   Abilities: Fire Puppet. */
void btEffectRunFirePuppet(BATTLE_EFFECT_PARM *args)
{
	Uint8				*mesh;
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 frm;
	Uint8				*motes;
	BATTLE_CAMERA		*camera;
	MOTION_OBJECT		*tgt;
	SCREEN_POINT		 caster;
	MODEL_TEXTURE		 texture;
	RENDER_MODEL		*model;
	Sint32				 endX;
	Sint32				 endY;
	Sint32				 i;

	mesh = PUPPET_STRIP_MESH;
	state = RamHeapSlots->battleEff;
	canvas = RamHeapSlots->battleCanvas;
	camera = RamHeapSlots->cameraState;
	motes = RamHeapSlots->battleSheet;
	btEffectStartCanvasLayer(0);
	if(args->side == EFFECT_SIDE_ENEMIES) {
		EFFECT_LOAD_BLITTER(
			HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
	} else {
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
			EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
	}
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	tgt = btObjectGetStub(args->tgts[0])->obj;
	/* The skull's place, set at the trail's last segment. */
	endX = 0;
	endY = 0;
	/* DEAD: nothing reads the copy; the ROM keeps the stores. */
	PUPPET_TARGET_COPY.x = tgt->x;
	PUPPET_TARGET_COPY.y = tgt->y;
	PUPPET_TARGET_COPY.z = tgt->z;
	PUPPET_TARGET_COPY.velX = tgt->scaleX;
	PUPPET_TARGET_COPY.velY = tgt->scaleY;
	btResourceLoadAndDecompress(RESOURCE_STREAK_TEXTURE, state->sheet, 0, 0);
	/* DEAD: Fire Puppet draws none of these motes; the ROM keeps the load. */
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, motes, 0, 0);
	btResourceLoadAndDecompress(RESOURCE_GHOST_SKULL_TEXTURE,
		state->sheet + FIRE_PUPPET_GHOST_SKULL, 0, 0);
	/* The ghost takes the Mercury Djinn's blues. */
	syWordsCopyEntry((void *)BG_PLTT,
		syResourceGetTableEntry(RESOURCE_MERCURY_DJINN_SHEET),
		SHEET_PALETTE_BYTES);
	REG_BG2PA = Q_8_8(0.5);
	btRenderBuildQuadStrip(mesh, 4, 32, PUPPET_QUADS);

	for(frm = 0; frm != FIRE_PUPPET_TICKS; frm++) {
		EFFECT_VIEW_FROM_CAMERA(camera);
		if(frm == 0) {
			btEffectTintBattler(
				args->user, SPRITE_COLOR_GREY, POSE_PARTY_HELD, 0, -1);
		}
		if(frm == 8) {
			sySndPlayStub(SE_CURSE);
		}
		/* The caster takes its own colours and its first animation, a party
		   member's stance between turns: it stands again. */
		if(frm == PUPPET_CASTER_RISE_TICK) {
			btEffectTintBattler(
				args->user, SPRITE_COLOR_NORMAL, POSE_STILL, 0, -1);
		}
		if(frm == PUPPET_STRIKE_TICK) {
			syWordsFillEntry(
				canvas, CANVAS_BYTES, CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
			sySndPlayStub(SE_POWER_UP);
			EFFECT_SHOW_DAMAGE(SE_POWER_UP);
			state->shakeFrm = 8;
			for(i = 0; i != args->tgtCnt; i++) {
				btEffectStartActorMotion(
					args->tgts[i], 1, toFIXED(2), toFIXED(4), toFIXED(4), 150);
				btEffectTintBattler(
					args->tgts[i], SPRITE_COLOR_WHITE, POSE_DAMAGE, i, 8);
			}
		}
		if(frm >= PUPPET_RISE_TICK) {
			Sint32 curl;
			Sint32 j;
			MODEL_VERTEX(*corners)
			[2] = (MODEL_VERTEX(*)[2])RamMapMetatileBuffer;
			Uint8 *projected = PUPPET_STRIP_VERTICES;
			Sint32 shade;

			curl = (frm - PUPPET_RISE_TICK) * PUPPET_CURL_SPEED;
			if(curl > PUPPET_CURL_MAX) {
				curl = PUPPET_CURL_MAX;
			}
			if(frm >= PUPPET_RETURN_TICK) {
				curl = PUPPET_CURL_MAX -
					(frm - PUPPET_RETURN_TICK) * PUPPET_CURL_SPEED;
			}

			/* The trail curls about a point beside its root: segment j lies j *
			   curl round it, its two corners the trail's width apart. */
			for(j = 0; j != PUPPET_SEGMENTS; j++) {
				Sint32 width = PUPPET_WIDTH -
					(FIXEDtoINT(
						sySin(j * PUPPET_PINCH_STEP + PUPPET_PINCH_START) *
						PUPPET_PINCH));
				Sint32 sway = (FIXEDtoINT(sySin((frm + j) * (TRIG_TURN / 32)) *
								  PUPPET_SWAY)) *
					j / (PUPPET_SEGMENTS - 1);
				Sint32 outer;

				i = 0;
				outer = width + sway;
				for(; i != 2; i++) {
					corners[j][i].x = (FIXEDtoINT(syCos(j * curl) *
										  (i * width + sway + 16))) -
						outer / 2 - 16;
					corners[j][i].y = -(FIXEDtoINT(
						sySin(j * curl) * (i * width + j + sway + 32)));
					corners[j][i].z = 0;
				}
				/* The skull's place, the middle of the last segment. */
				if(j == PUPPET_SEGMENTS - 1) {
					endX = (FIXEDtoINT(syCos(curl * (PUPPET_SEGMENTS - 1)) *
							   (width / 2 + sway + 16))) -
						(width + sway) / 2 - 16;
					endY = -(FIXEDtoINT(sySin(curl * (PUPPET_SEGMENTS - 1)) *
						(width / 2 + j + sway + 32)));
				}
			}

			/* The trail, its streaks running along it 4 texels a tick. */
			model = btRenderAllocateModelList(1);
			shade = 0;
			texture.logWidth = 7;
			texture.logHeight = 5;
			texture.pixels = state->sheet;
			model->kind = MODEL_SHADED_MAX;
			model->mesh = mesh;
			model->vertices = projected;
			model->shade = shade;
			model->texture = &texture;
			btEffectPosProjectScaledBattler(args->user, &caster);
			model->scrollV = 0;
			model->scrollU = TEXTURE_SCROLL_MASK & ~(frm << 2);
			syInitMatrix();
			syTranslate(INTtoFIXED(caster.x / 2 - CANVAS_SIZE / 2),
				toFIXED(caster.y) - toFIXED(42), 0);
			syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1) / 2);
			if(args->side == EFFECT_SIDE_PARTY) {
				syRotY(DEGtoANG(180));
			}
			syRotZ(DEGtoANG(90));
			if(frm >= PUPPET_RETURN_TICK) {
				syMatrixScaleUniform(toFIXED(1) -
					(frm - PUPPET_RETURN_TICK) *
						(toFIXED(1) / PUPPET_RETURN_TICKS));
			}
			btRenderTransformVertices(
				RamMapMetatileBuffer, PUPPET_STRIP_VERTICES, PUPPET_CORNERS);
			btRenderDrawModelList(model);

			/* The skull at the trail's end, swelling from half its size as the
			   trail curls out. */
			model->mesh = BattleEffectSquareMesh;
			texture.logWidth = 6;
			texture.logHeight = 6;
			model->vertices = projected;
			model->scrollU = 0;
			model->scrollV = 0;
			texture.pixels = state->sheet + FIRE_PUPPET_GHOST_SKULL;
			model->shade = 0;
			syTranslate(toFIXED(endX), toFIXED(endY), 0);
			syRotZ(PUPPET_SKULL_TURN - curl * (PUPPET_SEGMENTS - 1));
			syMatrixScaleUniform(curl * PUPPET_SKULL_GROWTH + toFIXED(1) / 2);
			btRenderTransformVertices(BattleEffectSquareCorners, projected, 4);
			btRenderDrawModelList(model);
			syHeapRelease(model);
		}
		btEffectCountDownBattlerTints();
		btCameraShake(4, 4);
		EFFECT_PRESENT_CANVAS(state);
	}
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
	syCloseEvent(btEffectTransferCanvas);
	btEffectEndCanvasLayer();
}

#define WHIPLASH_TICKS				52
/* The tick the lash strikes the target. */
#define WHIPLASH_HIT_TICK			18
/* SlashFlameSheet: the violet lash in five pictures, shown one after another
   for WHIPLASH_LASH_PICTURE_TICKS apiece. */
#define WHIPLASH_LASH_A				0
#define WHIPLASH_LASH_A_WIDTH		48
#define WHIPLASH_LASH_A_HEIGHT		96
#define WHIPLASH_LASH_B				(WHIPLASH_LASH_A + PICTURE_BYTES(WHIPLASH_LASH_A))
#define WHIPLASH_LASH_B_WIDTH		38
#define WHIPLASH_LASH_B_HEIGHT		92
#define WHIPLASH_LASH_C				(WHIPLASH_LASH_B + PICTURE_BYTES(WHIPLASH_LASH_B))
#define WHIPLASH_LASH_C_WIDTH		30
#define WHIPLASH_LASH_C_HEIGHT		76
#define WHIPLASH_LASH_D				(WHIPLASH_LASH_C + PICTURE_BYTES(WHIPLASH_LASH_C))
#define WHIPLASH_LASH_D_WIDTH		56
#define WHIPLASH_LASH_D_HEIGHT		96
#define WHIPLASH_LASH_E				(WHIPLASH_LASH_D + PICTURE_BYTES(WHIPLASH_LASH_D))
#define WHIPLASH_LASH_E_WIDTH		56
#define WHIPLASH_LASH_E_HEIGHT		88
#define WHIPLASH_LASH_PICTURE_TICKS 6
/* The motes the lash's crack sprays, kept as particles in the map cell
   buffer. */
#define WHIPLASH_MOTES				256

/* A lash of violet light swings out from the caster and cracks down on the
   target, which is thrown up and flashes, and motes spray from where the lash
   struck, half of them rising and half falling as they burn down.

   Abilities: Whiplash. */
void btEffectRunWhiplash(BATTLE_EFFECT_PARM *args)
{
	HEAP_SLOTS			*heap;
	BATTLE_EFFECT_STATE *state;
	Uint8				*canvas;
	SCREEN_POINT		 caster;
	Sint32				 frm;
	PARTICLE			*mote;
	/* LEFTOVER(frame): draw[1], the pair's idle second blitter, which the ROM's
	 * tick reserves and never touches; as one DRAW_RECTANGLE. */
	DRAW_RECTANGLE draw[2];
	Uint8		  *motes;
	Sint32		   i;

	heap = RamHeapSlots;
	state = (BATTLE_EFFECT_STATE *)heap->battleEff;
	canvas = heap->battleCanvas;
	motes = heap->battleSheet;

	btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	if(args->side == EFFECT_SIDE_ENEMIES) {
		EFFECT_LOAD_BLITTER(
			HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
		draw[0] = heap->ramCode;
	} else {
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
			EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
		draw[0] = heap->ramCode;
	}
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, motes, 0, 0);
	btResourceLoadAndDecompress(RESOURCE_SLASH_FLAME_SHEET, state->sheet, 0, 0);
	syWordsCopyEntry((void *)BG_PLTT,
		syResourceGetTableEntry(RESOURCE_VIOLET_PALETTE_F),
		SHEET_PALETTE_BYTES);
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

	for(frm = 0; frm != WHIPLASH_TICKS; frm++) {
		/* The lash, its pictures placed by the caster. */
		btEffectPosProjectBattler(args->user, &caster);
		if(args->side == EFFECT_SIDE_ENEMIES) {
			switch(frm / WHIPLASH_LASH_PICTURE_TICKS) {
			case 0:
				EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
					WHIPLASH_LASH_A, caster.x / 2 - 48, caster.y - 120);
				break;
			case 1:
				EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
					WHIPLASH_LASH_B, caster.x / 2 - 38, caster.y - 120);
				break;
			case 2:
				EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
					WHIPLASH_LASH_C, caster.x / 2 - 38, caster.y - 100);
				break;
			case 3:
				EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
					WHIPLASH_LASH_D, caster.x / 2 - 64, caster.y - 88);
				break;
			case 4:
				EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
					WHIPLASH_LASH_E, caster.x / 2 - 64, caster.y - 88);
				break;
			}
		} else {
			switch(frm / WHIPLASH_LASH_PICTURE_TICKS) {
			case 0:
				EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
					WHIPLASH_LASH_A, caster.x / 2, caster.y - 120);
				break;
			case 1:
				EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
					WHIPLASH_LASH_B, caster.x / 2, caster.y - 120);
				break;
			case 2:
				EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
					WHIPLASH_LASH_C, caster.x / 2, caster.y - 100);
				break;
			case 3:
				EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
					WHIPLASH_LASH_D, caster.x / 2, caster.y - 88);
				break;
			case 4:
				EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
					WHIPLASH_LASH_E, caster.x / 2, caster.y - 88);
				break;
			}
		}

		/* The hit, and the motes sprayed from 48 pixels out, towards the
		   target. */
		if(frm == WHIPLASH_HIT_TICK) {
			btMotionApplyKnockbackStub(args->tgts[0], KNOCKBACK_HOP);
			btEffectTintBattler(
				args->tgts[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 16);
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
			state->shakeFrm = 8;
			for(i = 0; i != WHIPLASH_MOTES; i++) {
				Sint32 ang;
				Sint32 speed;

				mote = &((PARTICLE *)RamMapMetatileBuffer)[i];
				if(args->side == EFFECT_SIDE_ENEMIES)
					mote->x = INTtoFIXED(caster.x / 2 - 48);
				else
					mote->x = INTtoFIXED(caster.x / 2 + 48);
				mote->y = toFIXED(caster.y - (syRandom() & 7));
				ang = (syRandom() & 0x7fff) + DEGtoANG(90);
				speed = syRandom() & 255;
				if(args->side == EFFECT_SIDE_ENEMIES)
					mote->velX = ((sySin(ang) * speed) >> 4) - toFIXED(8);
				else
					mote->velX = ((sySin(ang) * speed) >> 4) + toFIXED(8);
				mote->velY = (syCos(ang) * (speed + 64)) >> 5;
				mote->variant = (syRandom() & 15) + 16;
			}
		}
		if(frm >= WHIPLASH_HIT_TICK) {
			for(i = 0; i != WHIPLASH_MOTES; i++) {
				mote = &((PARTICLE *)RamMapMetatileBuffer)[i];
				if(mote->variant >= 0) {
					Sint32 size = (mote->variant >> 3) + 1;

					EFFECT_DRAW_MOTE(draw[0], canvas, motes,
						FIXED_INTEGER_PART(mote->x),
						FIXED_INTEGER_PART(mote->y), size);
					if(i & 1)
						btParticleAdvanceWithGravity2D(
							mote, 60, -toFIXED(1) / 8);
					else
						btParticleAdvanceWithGravity2D(
							mote, 60, toFIXED(1) / 16);
					mote->variant--;
				}
			}
		}

		btCameraShake(8, 8);
		EFFECT_END_TICK(state);
	}

	EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#undef FIRE_PUPPET_TICKS
#undef PUPPET_SEGMENTS
#undef PUPPET_CORNERS
#undef PUPPET_QUADS
#undef PUPPET_STRIP_VERTICES
#undef PUPPET_STRIP_MESH
#undef PUPPET_TARGET_COPY
#undef PUPPET_RISE_TICK
#undef PUPPET_CURL_SPEED
#undef PUPPET_CURL_MAX
#undef PUPPET_STRIKE_TICK
#undef PUPPET_RETURN_TICK
#undef PUPPET_RETURN_TICKS
#undef PUPPET_CASTER_RISE_TICK
#undef PUPPET_SKULL_GROWTH
#undef PUPPET_WIDTH
#undef PUPPET_PINCH
#undef PUPPET_PINCH_STEP
#undef PUPPET_PINCH_START
#undef PUPPET_SWAY
#undef PUPPET_SKULL_TURN
#undef FIRE_PUPPET_GHOST_SKULL
#undef WHIPLASH_TICKS
#undef WHIPLASH_HIT_TICK
#undef WHIPLASH_LASH_A
#undef WHIPLASH_LASH_A_WIDTH
#undef WHIPLASH_LASH_A_HEIGHT
#undef WHIPLASH_LASH_B
#undef WHIPLASH_LASH_B_WIDTH
#undef WHIPLASH_LASH_B_HEIGHT
#undef WHIPLASH_LASH_C
#undef WHIPLASH_LASH_C_WIDTH
#undef WHIPLASH_LASH_C_HEIGHT
#undef WHIPLASH_LASH_D
#undef WHIPLASH_LASH_D_WIDTH
#undef WHIPLASH_LASH_D_HEIGHT
#undef WHIPLASH_LASH_E
#undef WHIPLASH_LASH_E_WIDTH
#undef WHIPLASH_LASH_E_HEIGHT
#undef WHIPLASH_LASH_PICTURE_TICKS
#undef WHIPLASH_MOTES
