/* Fume. Part of battle_effect.c. */

/* One corner of the plume: signed x, y and depth. */
typedef struct CORNER {
	Sint8 x;
	Sint8 y;
	Sint8 z;
} CORNER;

#define FUME_TICKS			   50
/* The tick the plume's head reaches the target and the target is hit. */
#define FUME_HIT_TICK		   22
/* The plume is FUME_SEGMENTS pairs of corners, laid in the map cell buffer: the
   corners, those btRenderTransformVertices makes of them at
   FUME_STRIP_VERTICES, and the plume's mesh at FUME_STRIP_MESH, whose quads
   each take 4 texels of SmokeBandSheet's flames across and its 32 down. */
#define FUME_SEGMENTS		   33
#define FUME_CORNERS		   (FUME_SEGMENTS * 2)
#define FUME_QUADS			   (FUME_SEGMENTS - 1)
#define FUME_STRIP_VERTICES	   (RamMapMetatileBuffer + 4096)
#define FUME_STRIP_MESH		   (RamMapMetatileBuffer + 8192)
/* The plume arcs from the caster over to the target. Its head goes
   FUME_ARC_GROWTH further each tick, and from the level's FUME_TAIL_START its
   tail follows, each up to FUME_ARC_SPAN; a point's angle on the arc is
   FUME_SEGMENTS times its place. */
#define FUME_ARC_GROWTH		   44
#define FUME_ARC_SPAN		   988
/* The plume thickens 4 pixels a segment from its tail up to FUME_THICKNESS,
   times the level's width. */
#define FUME_THICKNESS		   24
/* The plume sways up to FUME_SWAY pixels at its head, less towards its tail, a
   wave that runs along it a segment a tick. */
#define FUME_SWAY			   4
/* From this tick the plume dims, 8 shades a tick. */
#define FUME_DIM_TICK		   34
/* The head's scale, seven eighths of the plume's width. */
#define FUME_HEAD_SCALE(width) ((width) * 28 / 32)

/* The fireballs that burst from the target: FUME_BURSTS_MAX are set out at the
   hit, four more showing each tick, and a level draws FumeLevels' FUME_SPARKS
   of them. Each shows FlashBurstSheet's six pictures in turn,
   FUME_BURST_PICTURE_TICKS apiece, for FUME_BURST_TICKS. */
#define FUME_BURSTS_MAX			 32
#define FUME_BURST_PICTURE_TICKS 4
#define FUME_BURST_TICKS		 24

/* Where FlashBurstSheet is decoded in the effect's pictures. */
#define FUME_FLASH_BURST_PICTURES 2048
/* Where ClawSlashSheet, a dragon's head of flame, is decoded in the effect's
   pictures. */
#define FUME_DRAGON_HEAD_PICTURES 16384

/* A plume of flames rises from the caster and arcs over onto the target, a
   dragon's head of fire at its front. As the head strikes, the screen flashes
   white, the target is thrown back and fireballs burst from it; the plume
   follows its head down and fades. The stronger the level, the thicker the
   plume, the more fireballs and the harder the shake.

   Abilities: Fume, Serpent Fume, Dragon Fume. */
void btEffectRunFume(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	/* LEFTOVER(frame): blit[1], the pair's idle second blitter, which the ROM's
	 * tick reserves and never touches; as one DRAW_RECTANGLE. */
	DRAW_RECTANGLE blit[2];
	Sint32		   j;
	Sint32		   frm;
	Sint32		   endX;
	Sint32		   endY;

	struct {
		Sint32 head;
		Sint32 tail;
	} arc;

	SCREEN_POINT  caster;
	SCREEN_POINT  tgt;
	MODEL_TEXTURE texture;
	RENDER_MODEL *model;
	PARTICLE	 *burst;
	PARTICLE	 *particle;
	Sint32		  i;
	void		 *vertices;
	Uint8		 *mesh;
	CORNER		 *corners;

	state = RamHeapSlots->battleEff;
	canvas = RamHeapSlots->battleCanvas;
	btEffectStartCanvasLayer(BGCNT_PRIORITY(1));
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	blit[0] = RamHeapSlots->ramCode;
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	/* The head's place, set at the plume's last segment. */
	endX = 0;
	endY = 0;
	btEffectPosProjectBattler(args->user, &caster);
	btEffectPosProjectBattler(args->tgts[0], &tgt);
	btResourceLoadAndDecompress(RESOURCE_SMOKE_BAND_SHEET, state->sheet, 1, 1);
	btResourceLoadAndDecompress(RESOURCE_FLASH_BURST_SHEET,
		state->sheet + FUME_FLASH_BURST_PICTURES, 1, 0);
	btResourceLoadAndDecompress(RESOURCE_CLAW_SLASH_SHEET,
		state->sheet + FUME_DRAGON_HEAD_PICTURES, 1, 0);
	REG_BG2PA = Q_8_8(0.5);
	btRenderBuildQuadStrip(FUME_STRIP_MESH, 4, 32, FUME_QUADS);

	for(frm = 0; frm != FUME_TICKS; frm++) {
		if(frm == 0) {
			arc.head = 0;
			arc.tail = 0;
			/* No fireball shows until the hit. */
			for(i = 0; i != ARRAY_COUNT(state->particles); i++) {
				state->particles[i].variant = FUME_BURST_TICKS;
			}
		}
		if(frm == FUME_HIT_TICK) {
			syWordsFillEntry(
				canvas, CANVAS_BYTES, CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
			EFFECT_SHOW_DAMAGE(SE_CLATTER);
			state->shakeFrm = 8;
			btEffectStartActorMotion(
				args->tgts[0], 1, toFIXED(2), toFIXED(4), toFIXED(2), 120);
			btEffectTintBattler(
				args->tgts[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 20);
			for(i = 0; i != FUME_BURSTS_MAX; i++) {
				burst = &state->particles[i];
				burst->variant = -i / 4 + 2;
				burst->x = toFIXED(tgt.x / 2);
				burst->y = toFIXED(tgt.y + (syRandom() & 15) - 32);
				burst->velX =
					((syRandom() % FumeLevels[args->variant][FUME_SPREAD]) -
						(FumeLevels[args->variant][FUME_SPREAD] >> 1))
					<< 12;
				burst->velY = (192 - (syRandom() & 255)) << 10;
			}
		}
		for(i = 0; i != FumeLevels[args->variant][FUME_SPARKS]; i++) {
			particle = &state->particles[i];
			if((Uint32)particle->variant < FUME_BURST_TICKS) {
				if(particle->variant > 1) {
					blit[0](canvas,
						state->sheet + FUME_FLASH_BURST_PICTURES +
							particle->variant / FUME_BURST_PICTURE_TICKS *
								PICTURE_BYTES(FLASH_BURST),
						FIXED_INTEGER_PART(particle->x) - FLASH_BURST_WIDTH / 2,
						FIXED_INTEGER_PART(particle->y) -
							FLASH_BURST_HEIGHT / 2,
						FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
				}
				btParticleAdvanceWithGravity2D(
					particle, 60, -FumeLevels[args->variant][FUME_LIFT]);
			}
			particle->variant++;
		}

		vertices = FUME_STRIP_VERTICES;
		mesh = FUME_STRIP_MESH;
		corners = (CORNER *)MapGridMetatileBuffer;
		/* DEAD: tick never falls below 0; the ROM keeps the compare and
		   branch. */
		if(frm >= 0) {
			Sint32 width;
			Sint32 step;
			Sint32 sweep;
			Sint32 vertex;
			Sint32 shade;

			width = FumeLevels[args->variant][FUME_WIDTH];
			arc.head = frm * FUME_ARC_GROWTH;
			if(arc.head > FUME_ARC_SPAN) {
				arc.head = FUME_ARC_SPAN;
			}
			if(frm >= FumeLevels[args->variant][FUME_TAIL_START]) {
				arc.tail = (frm - FumeLevels[args->variant][FUME_TAIL_START]) *
					FUME_ARC_GROWTH;
				if(arc.tail > FUME_ARC_SPAN) {
					arc.tail = FUME_ARC_SPAN;
				}
			}

			/* The plume's corners, two a segment, stepping evenly from the
			   tail's angle towards the head's on an ellipse from the caster to
			   the target. */
			for(j = 0, step = arc.head - arc.tail, sweep = 0, vertex = 0;
				j != FUME_SEGMENTS; sweep += step, vertex += 2, j++) {
				Sint32 distance = caster.x / 8 - tgt.x / 8;
				Sint32 ang = sweep + arc.tail * FUME_SEGMENTS;
				Sint32 sway = (FIXEDtoINT(sySin((frm + j) * (TRIG_TURN / 16)) *
								  FUME_SWAY)) *
					j / (FUME_SEGMENTS - 1);
				Sint32 height;
				Sint32 thickness;
				Sint32 radiusX;
				Sint32 radiusY;
				Sint32 half;
				Sint32 baseY;

				if(distance < 0) {
					distance = -distance;
				}
				height = FumeLevels[args->variant][FUME_HEIGHT];
				thickness = j * 4;
				if(thickness > FUME_THICKNESS) {
					thickness = FUME_THICKNESS;
				}
				thickness = thickness * width / toFIXED(1);
				i = 0;
				half = thickness / -2;
				baseY = height + half;
				radiusY = baseY + sway;
				radiusX = distance + half + sway;
				for(; i != 2; i++) {
					corners[vertex + i].x =
						(FIXEDtoINT(syCos(ang) * radiusX)) - distance;
					corners[vertex + i].y = -(FIXEDtoINT(sySin(ang) * radiusY));
					corners[vertex + i].z = 0;
					radiusY += thickness;
					radiusX += thickness;
				}
				if(j == FUME_SEGMENTS - 1) {
					endX =
						(FIXEDtoINT((distance + sway) * syCos(ang))) - distance;
					endY = -(FIXEDtoINT((height + sway) * sySin(ang)));
				}
			}

			shade = 0;
			if(frm >= FUME_DIM_TICK) {
				shade = (FUME_DIM_TICK - frm) * 8;
			}
			if(shade > MODEL_SHADE_INVISIBLE) {
				model = btRenderAllocateModelList(1);
				model->kind = MODEL_SHADED_MAX;
				model->texture = &texture;
				model->shade = shade;
				model->scrollV = 0;
				model->vertices = vertices;
				if(arc.head > arc.tail) {
					/* The plume, its flames running along it 8 texels a
					   tick. */
					texture.logWidth = 6;
					texture.logHeight = 5;
					texture.pixels = state->sheet;
					model->mesh = mesh;
					model->scrollU = TEXTURE_SCROLL_MASK & ~(frm << 3);
					syInitMatrix();
					syTranslate(INTtoFIXED(caster.x / 2 - CANVAS_SIZE / 2),
						toFIXED(caster.y) - toFIXED(64), 0);
					syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1) / 2);
					if(args->side == EFFECT_SIDE_PARTY) {
						syRotY(DEGtoANG(180));
					}
					syMatrixScaleUniform(toFIXED(4));
					btRenderTransformVertices(corners, vertices, FUME_CORNERS);
					btRenderDrawModelList(model);

					/* The head at the plume's end, turned with the arc, until
					   it strikes. */
					if(frm <= FUME_HIT_TICK) {
						model->mesh = BattleEffectSquareMesh;
						texture.logWidth = 6;
						texture.logHeight = 6;
						model->scrollU = 0;
						texture.pixels =
							state->sheet + FUME_DRAGON_HEAD_PICTURES;
						syTranslate(toFIXED(endX), toFIXED(endY), 0);
						syRotZ(DEGtoANG(90) - arc.head * (FUME_SEGMENTS - 1));
						syMatrixScaleUniform(FUME_HEAD_SCALE(width));
						btRenderTransformVertices(
							BattleEffectSquareCorners, vertices, 4);
						btRenderDrawModelList(model);
					}
				}
				syHeapRelease(model);
			}
		}
		btEffectCountDownBattlerTints();
		btCameraShake(args->variant * 2 + 2, args->variant * 2 + 2);
		EFFECT_PRESENT_CANVAS(state);
	}
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
	syCloseEvent(btEffectTransferCanvas);
	btEffectEndCanvasLayer();
}

#undef FUME_TICKS
#undef FUME_HIT_TICK
#undef FUME_SEGMENTS
#undef FUME_CORNERS
#undef FUME_QUADS
#undef FUME_STRIP_VERTICES
#undef FUME_STRIP_MESH
#undef FUME_ARC_GROWTH
#undef FUME_ARC_SPAN
#undef FUME_THICKNESS
#undef FUME_SWAY
#undef FUME_DIM_TICK
#undef FUME_HEAD_SCALE
#undef FUME_BURSTS_MAX
#undef FUME_BURST_PICTURE_TICKS
#undef FUME_BURST_TICKS
#undef FUME_FLASH_BURST_PICTURES
#undef FUME_DRAGON_HEAD_PICTURES
