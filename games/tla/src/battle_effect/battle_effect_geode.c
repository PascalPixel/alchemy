/* Geode. Part of battle_effect.c. */

#define GEODE_TICKS 74

/* The clod of earth: the battlefield's picture wrapped round a cylinder of
   GEODE_CYLINDER_RADIUS, GEODE_CLOD_RINGS rings of GEODE_CLOD_RING_VERTICES
   stacked GEODE_RING_SPACING apart, which over GEODE_ROLL_TICKS rolls up into
   a ball of GEODE_CLOD_RADIUS, its rings from pole to pole. */
#define GEODE_CLOD_RINGS		 16
#define GEODE_CLOD_RING_VERTICES 16
#define GEODE_CLOD_VERTICES		 (GEODE_CLOD_RINGS * GEODE_CLOD_RING_VERTICES)
#define GEODE_RING_SPACING		 8
#define GEODE_CLOD_TOP			 (-GEODE_CLOD_RINGS * GEODE_RING_SPACING / 2)
#define GEODE_RING_TURN			 (DEGtoANG(180) / (GEODE_CLOD_RINGS - 1))
#define GEODE_CYLINDER_RADIUS	 56
#define GEODE_CLOD_RADIUS		 48
#define GEODE_ROLL_TICKS		 16
/* From GEODE_TUMBLE_TICK the clod tumbles forward, faster and then slower,
   along a sine that advances GEODE_TUMBLE_STEP a tick. */
#define GEODE_TUMBLE_TICK		 8
#define GEODE_TUMBLE_STEP		 924
/* BG2 shows the canvas at zoom / GEODE_ZOOM_ONE times its width: twice, the
   battlefield at its own size, while the clod is flat, and once when it is
   round. */
#define GEODE_ZOOM_ONE			 32

/* From GEODE_FLIGHT_TICK the clod flies onto the target over
   GEODE_FLIGHT_TICKS, the Djinni streaking behind it, and lands at
   GEODE_CRASH_TICK. */
#define GEODE_FLIGHT_TICK  36
#define GEODE_FLIGHT_TICKS 12
#define GEODE_CRASH_TICK   (GEODE_FLIGHT_TICK + GEODE_FLIGHT_TICKS)
/* The billows lean back GEODE_BILLOW_TILT. */
#define GEODE_BILLOW_TILT  1000

/* The effect's pictures open with the clod's projected corners, then its
   corners and its mesh; BillowTexture and VenusDjinnSmallSheet are decoded
   further on. */
#define GEODE_CLOD_CORNERS (GEODE_CLOD_VERTICES * sizeof(PROJECTED_VERTEX))
#define GEODE_CLOD_MESH \
	(GEODE_CLOD_CORNERS + GEODE_CLOD_VERTICES * sizeof(MODEL_VERTEX))
#define GEODE_BILLOW		 18192
#define GEODE_DJINNI_PICTURE 26048 /* the small Venus Djinni */

/* The battlefield itself rolls up into a clod of earth over the caster,
   spinning and tumbling, and flies onto the target with the Venus Djinni
   streaking behind it in fire; it lands in a white flash, billows of fire burst
   up and the screen shakes, and the backdrop comes back.

   Abilities: Geode. */
void btEffectRunGeode(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	/* LEFTOVER(frame): blit[1], the pair's idle second blitter, which the ROM's
	 * tick reserves and never touches; as one DRAW_RECTANGLE. */
	DRAW_RECTANGLE			 blit[2];
	BATTLE_BACKGROUND_STATE *backdrop;
	Uint8					*projected;
	MODEL_VERTEX			*vertices;
	Uint8					*mesh;
	Sint32					 row;
	Sint32					 curl;
	Sint32					 pitch;
	BATTLE_STATE			*battle;
	Uint8					*picture;
	void					*caster;
	Sint32					 screenB[3];
	Sint32					 screenA[3];
	SCREEN_POINT			 src;
	SCREEN_POINT			 tgt;
	MODEL_TEXTURE			 texture;
	MODEL_TEXTURE			 billow;
	RENDER_MODEL			*model;
	RENDER_MODEL			*rings;
	Sint32					 frm;
	Sint32					 i;

	state = Ram_HeapSlots->battleEff;
	canvas = Ram_HeapSlots->battleCanvas;
	backdrop = Ram_HeapSlots->battleBg;
	btEffectStartCanvasLayer(BGCNT_PRIORITY(2));
	btEffectRunDjinniEntrance(args,
		DJINNI_ENTRANCE(
			ELEMENT_VENUS, args->side, DJINNI_ANCHOR_CENTRE_BRIGHTENING),
		screenA, screenB);
	btEffectConfigureEffectDisp();
	REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG2 |
		WININ_WIN1_OBJ | WININ_WIN1_CLR;
	REG_WINOUT = WINOUT_WIN01_BG0 | WINOUT_WIN01_OBJ | WINOUT_WIN01_CLR |
		WINOUT_WINOBJ_ALL;
	backdrop->busy = 1;
	if(args->side == EFFECT_SIDE_ENEMIES) {
		EFFECT_LOAD_BLITTER(
			HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
	} else {
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
			EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_ADDITIVE);
	}
	blit[0] = Ram_HeapSlots->ramCode;
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	REG_BLDCNT = 0;
	EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
	sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK + 15);
	sySetEvent(btDispApplyBg2Affine, EVENT_PASS_VBLANK + 127);
	projected = state->sheet;
	vertices = (MODEL_VERTEX *)(state->sheet + GEODE_CLOD_CORNERS);
	mesh = state->sheet + GEODE_CLOD_MESH;
	pitch = 0;
	battle = Ram_HeapSlots->battleState;
	caster = btObjectGetStub(args->user)->obj;
	BgScrollWork[1].x = -56;
	/* The battlefield's picture, the clod's texture: BG2's 256-colour tiles
	   copied a tile's line at a time into plain lines of pixels. */
	picture = MapGridMetatileBuffer;
	{
		Uint8 *cell = picture;
		Sint32 x;

		for(i = 0; i < BATTLE_BACKGROUND_HEIGHT; i++) {
			for(x = 0; x < BATTLE_BACKGROUND_WIDTH / 8 * TILE_SIZE_8BPP;
				x += TILE_SIZE_8BPP) {
				syWordsCopyEntry(cell,
					(Uint8 *)BG_CHAR_BLOCK(2) + (((i / 8) << 8) + (i & 7)) * 8 +
						x,
					8);
				cell += 8;
			}
		}
	}
	btResourceLoadAndDecompress(RESOURCE_VENUS_DJINN_SMALL_SHEET,
		state->sheet + GEODE_DJINNI_PICTURE, 1, 1);
	btResourceLoadAndDecompress(
		RESOURCE_BILLOW_TEXTURE, state->sheet + GEODE_BILLOW, 0, 0);
	syWordsCopyEntry((void *)BG_PLTT,
		syResourceGetTableEntry(RESOURCE_ORANGE_PALETTE_B),
		SHEET_PALETTE_BYTES);
	IO_WRITE_QUEUE_PUSH((Uint32)&REG_BG2PA, Q_8_8(0.5), IO_WRITE_HALFWORD);
	fdActorSetAnimationSpeedStub(caster, ANIM_STEP_NORMAL * 3);
	btMeshBuildGrid(mesh, GEODE_CLOD_RING_VERTICES, GEODE_CLOD_RINGS,
		BATTLE_BACKGROUND_WIDTH / GEODE_CLOD_RING_VERTICES, GEODE_RING_SPACING);

	for(frm = 0; frm != GEODE_TICKS; frm++) {
		btEffectPosProjectBattler(args->user, &src);
		btEffectPosProjectBattler(args->tgts[0], &tgt);

		/* The backdrop's colours come down from the entrance's white to its
		   own. */
		if(frm <= 11) {
			Uint16 *from = battle->pal;
			Uint16 *to = (Uint16 *)&BG_PLTT_COLOR(6, 0);

			for(i = 0; i != BATTLE_BACKGROUND_COLORS; i++, from++, to++) {
				Sint32 red = RGB_RED(*from);
				Sint32 green = RGB_GREEN(*from);
				Sint32 blue = RGB_BLUE(*from);
				Sint32 r = RGB_RED(*to);
				Sint32 g = RGB_GREEN(*to);
				Sint32 b = RGB_BLUE(*to);

				if(r > red + 3) {
					r -= 4;
				} else {
					r = red;
				}
				if(g > green + 3) {
					g -= 4;
				} else {
					g = green;
				}
				if(b > blue + 3) {
					b -= 4;
				} else {
					b = blue;
				}
				*to = RGB(r, g, b);
			}
		}
		if(frm == 1) {
			IO_WRITE_QUEUE_PUSH((Uint32)&REG_DISPCNT,
				DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
					DISPCNT_BG2_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
					DISPCNT_WIN1_ON,
				IO_WRITE_HALFWORD);
			btEffectBuildCanvasTilemap();
		}
		if(frm == 34) {
			syCloseEvent(btEffectTransferCanvas);
			sySetEvent(
				btEffectProcessPendingBlock2Transfer, EVENT_PASS_VBLANK + 15);
			IO_WRITE_QUEUE_PUSH((Uint32)&REG_DISPCNT,
				DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
					DISPCNT_BG1_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON |
					DISPCNT_WIN1_ON,
				IO_WRITE_HALFWORD);
			syWordsClearEntry(canvas, CANVAS_VIEW_BYTES);
			BgScrollWork[1].y = 78;
			BgScrollWork[1].x = CANVAS_SIZE / 2 - src.x;
			REG_BG1CNT &= ~BGCNT_PRIORITY(3);
			REG_BG1CNT |= BGCNT_PRIORITY(1);
			REG_BG2CNT =
				BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
		}
		if(frm == 35) {
			syCloseEvent(btDispApplyBg2Affine);
			syCloseEvent(btEffectProcessPendingBlock2Transfer);
			sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK + 15);
			IO_WRITE_QUEUE_PUSH((Uint32)&REG_BG2X, 0, IO_WRITE_WORD);
			IO_WRITE_QUEUE_PUSH(
				(Uint32)&REG_BG2Y, -(BATTLE_VIEW_TOP << 8), IO_WRITE_WORD);
			IO_WRITE_QUEUE_PUSH(
				(Uint32)&REG_BG2PA, Q_8_8(0.5), IO_WRITE_HALFWORD);
			IO_WRITE_QUEUE_PUSH((Uint32)&REG_BLDCNT,
				BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL,
				IO_WRITE_HALFWORD);
		}
		if(frm == 18) {
			btEffectTintBattler(args->user, TINT_KEEP, POSE_LEAP, 0, -1);
		}
		if(frm == 36) {
			EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
			IO_WRITE_QUEUE_PUSH(
				(Uint32)&REG_DISPCNT, EFFECT_DISPCNT, IO_WRITE_HALFWORD);
		}
		if((Uint32)(frm - 34) <= 5) {
			if(args->side == EFFECT_SIDE_ENEMIES) {
				REG_WIN0H = WIN_RANGE(32, DISPLAY_WIDTH);
				REG_WIN1H = WIN_RANGE(32, DISPLAY_WIDTH);
			} else {
				REG_WIN0H = WIN_RANGE(0, DISPLAY_WIDTH - 32);
				REG_WIN1H = WIN_RANGE(0, DISPLAY_WIDTH - 32);
			}
		} else if(frm == 40) {
			REG_WIN0H = WIN_RANGE(0, DISPLAY_WIDTH);
			REG_WIN1H = WIN_RANGE(0, DISPLAY_WIDTH);
		}

		/* The clod, now on BG1, flies onto the target with the Djinni behind
		   it, drawn twice to glow twice as bright in the canvas's orange, its
		   trail fading by half a tick. */
		if((Uint32)(frm - GEODE_FLIGHT_TICK) <= 25) {
			Sint32 t = frm - GEODE_FLIGHT_TICK;
			Sint32 x;
			Sint32 y;

			if(t > GEODE_FLIGHT_TICKS) {
				t = GEODE_FLIGHT_TICKS;
			}
			x = src.x + (tgt.x - src.x) * t / GEODE_FLIGHT_TICKS;
			y = t * 64 / GEODE_FLIGHT_TICKS + 16;
			BgScrollWork[1].x = CANVAS_SIZE / 2 - x;
			BgScrollWork[1].y = t * -46 / GEODE_FLIGHT_TICKS + 78;
			if(args->side == EFFECT_SIDE_ENEMIES) {
				x = x / 2 + 20;
			} else {
				x = x / 2 - 20;
			}
			blit[0](canvas, state->sheet + GEODE_DJINNI_PICTURE,
				x - SMALL_DJINNI_WIDTH / 2, y - 48, SMALL_DJINNI_WIDTH,
				SMALL_DJINNI_HEIGHT);
			blit[0](canvas, state->sheet + GEODE_DJINNI_PICTURE,
				x - SMALL_DJINNI_WIDTH / 2, y - 48, SMALL_DJINNI_WIDTH,
				SMALL_DJINNI_HEIGHT);
		}

		/* The battlefield rolls up into the clod over the caster. */
		if((Uint32)frm <= 34) {
			Sint32 zoom;
			Sint32 centerX;
			Sint32 centerY;
			Sint32 height;
			Sint32 ang;

			curl = frm;
			if(curl > GEODE_ROLL_TICKS) {
				curl = GEODE_ROLL_TICKS;
			}
			zoom =
				GEODE_ZOOM_ONE * 2 - curl * (GEODE_ZOOM_ONE / GEODE_ROLL_TICKS);
			/* DEAD: tick >= 0 is always true inside this range, so the
			   defaults are always overwritten; the ROM keeps both moves and the
			   compare. */
			centerX = DISPLAY_WIDTH / 2;
			centerY = -16;
			if(frm >= 0) {
				Sint32 f = frm;

				if(f > GEODE_ROLL_TICKS) {
					f = GEODE_ROLL_TICKS;
				}
				centerX = (src.x - DISPLAY_WIDTH / 2) * f / GEODE_ROLL_TICKS +
					DISPLAY_WIDTH / 2;
				centerY = 46 * f / GEODE_ROLL_TICKS - 16;
			}
			state->bg2x = (CANVAS_SIZE / 2 - centerX * GEODE_ZOOM_ONE / zoom)
				<< 8;
			state->bg2y = centerY << 8;
			{
				Sint32 ratio = Q_8_8(1) * GEODE_ZOOM_ONE / zoom;

				state->bg2pa = ratio;
				state->bg2pd = Q_8_8(1);
			}
			/* Each ring moves from its place on the cylinder toward its place
			   on the ball as the clod curls. */
			for(row = 0, height = GEODE_CLOD_TOP, ang = 0;
				row != GEODE_CLOD_RINGS; row++) {
				Sint32 ballY = FIXEDtoINT(-syCos(ang) * GEODE_CLOD_RADIUS);
				Sint32 ballRadius = FIXEDtoINT(sySin(ang) * GEODE_CLOD_RADIUS);
				Sint32 column;
				Sint32 y;
				Sint32 radius;
				y = (ballY - height) * curl / GEODE_ROLL_TICKS + height;
				radius = (ballRadius - GEODE_CYLINDER_RADIUS) * curl /
						GEODE_ROLL_TICKS +
					GEODE_CYLINDER_RADIUS;
				for(column = 0; column != GEODE_CLOD_RING_VERTICES; column++) {
					Sint32 turn =
						column * (TRIG_TURN / GEODE_CLOD_RING_VERTICES);

					vertices[row * GEODE_CLOD_RING_VERTICES + column].x =
						FIXEDtoINT(sySin(turn) * radius);
					vertices[row * GEODE_CLOD_RING_VERTICES + column].y = y;
					vertices[row * GEODE_CLOD_RING_VERTICES + column].z =
						FIXEDtoINT(syCos(turn) * radius);
				}
				height += GEODE_RING_SPACING;
				ang += GEODE_RING_TURN;
			}
			model = btRenderAllocateModelList(1);
			texture.logWidth = 8;
			texture.logHeight = 7;
			texture.pixels = picture;
			model->flg = MODEL_ONE_SIDED | MODEL_CLOCKWISE;
			model->kind = MODEL_TEXTURED;
			model->texture = &texture;
			model->mesh = mesh;
			model->vertices = projected;
			model->scrollU = 0;
			model->scrollV = 0;
			syInitMatrix();
			if(frm >= GEODE_TUMBLE_TICK) {
				pitch +=
					sySin((frm - GEODE_TUMBLE_TICK) * GEODE_TUMBLE_STEP) >> 5;
				syRotX(pitch * 3 / 2);
			}
			syRotY(frm * (TRIG_TURN / 128));
			btRenderTransformVertices(vertices, projected, GEODE_CLOD_VERTICES);
			btRenderDrawModelList(model);
			syHeapRelease(model);
		}
		if(frm == GEODE_CRASH_TICK) {
			sySndPlayStub(SE_CRASH);
			syWordsFillEntry(
				canvas, CANVAS_BYTES, CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
		}
		if(frm == 64) {
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
		}

		/* Billows of fire burst from where the clod lands: four flared rings
		   stretched tall that grow and fade, turning one way and the other. */
		if(frm >= GEODE_CRASH_TICK) {
			void *ring = syHeapAllocEWRAM(512);

			rings = btRenderAllocateModelList(1);
			billow.logWidth = 6;
			billow.logHeight = 6;
			billow.pixels = state->sheet + GEODE_BILLOW;
			rings->kind = MODEL_SHADED_ADDITIVE;
			rings->texture = &billow;
			rings->mesh = BattleEffectRingMesh;
			rings->vertices = ring;
			rings->scrollU = 0;
			rings->scrollV = 0;
			for(i = 0; i != 4; i++) {
				Sint32 start = GeodeWaveStarts[i] + GEODE_CRASH_TICK;
				FIXED  scale;
				Sint32 shade;

				if(frm <= start) {
					continue;
				}
				scale =
					(frm - start) * GeodeWaveGrowth[i] * 2000 + toFIXED(1) / 2;
				shade = (start - frm) * 8 + 56;
				if(shade > 0) {
					shade = 0;
				}
				if(shade <= MODEL_SHADE_INVISIBLE) {
					continue;
				}
				rings->shade = shade;
				syInitMatrix();
				syTranslate(INTtoFIXED(tgt.x / 2 - CANVAS_SIZE / 2),
					toFIXED(GeodeWaveHeights[i]), 0);
				syScale(scale >> 1, scale << 2, scale >> 1);
				syRotX(GEODE_BILLOW_TILT);
				if(i & 1) {
					syRotY((i * 4 + frm) * (TRIG_TURN / 32));
				} else {
					syRotY((i * 4 - frm) * (TRIG_TURN / 32));
				}
				btRenderTransformVertices(GeodeRingCorners, ring, 32);
				btRenderDrawModelList(rings);
			}
			syHeapRelease(rings);
			syHeapRelease(ring);
		}
		if(frm == 61) {
			backdrop->busy = 0;
			syWordsFillEntry(
				(void *)&BG_PLTT_COLOR(6, 0), BATTLE_BACKGROUND_COLORS * 2, 0);
		}
		if(frm == 62) {
			btBackgroundLoadStub(BATTLE_BACKGROUND_FIXED, battle->bg, -1);
		}
		if((Uint32)(frm - 63) <= 4) {
			battle->brightness = (frm - 62) * (toFIXED(1) / 8);
			btColBufferScaleStub(battle->pal, (void *)&BG_PLTT_COLOR(6, 0),
				battle->brightness, BATTLE_BACKGROUND_COLORS);
		}
		if(frm == GEODE_CRASH_TICK) {
			state->shakeFrm = 16;
		}
		if(frm >= GEODE_CRASH_TICK) {
			btCameraShake(8, 8);
		}
		EFFECT_END_TICK(state);
	}
	fdActorSetAnimationSpeedStub(caster, ANIM_STEP_NORMAL);
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
	syCloseEvent(btEffectTransferCanvas);
	btEffectEndCanvasLayer();
}
