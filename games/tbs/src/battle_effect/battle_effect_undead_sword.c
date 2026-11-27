// Undead Sword. Part of battle_effect.c.

#define UNDEAD_SWORD_TICKS         54

// The lime column sheet's left half of a jagged, pointed blade of light, drawn twice, the second
// mirrored beside it.
#define LIME_COLUMN_WIDTH          14
#define LIME_COLUMN_HEIGHT         80
// The blade's foot: the canvas row it stands on, which the puffs billow from.
#define UNDEAD_SWORD_FOOT_Y        112
// The puffs' pictures, after the half blade.
#define UNDEAD_SWORD_PUFF_SHEET    PICTURE_BYTES(LIME_COLUMN)
#define UNDEAD_SWORD_PUFFS         16
// A puff's seven pictures, three ticks each, the last held until it goes.
#define UNDEAD_SWORD_PUFF_PICTURES 7
#define UNDEAD_SWORD_PUFF_TICKS    29

// A blade of lime light thrusts up out of the ground under the first target and sinks again, amid
// puffs of lime smoke that billow up round it.
//
// Abilities: Undead Sword.
void btEffectRunUndeadSword(BATTLE_EFFECT_PARM *args)
{
	void               **slots;
	void               **slot;
	BATTLE_EFFECT_STATE *state;
	void                *canvas;
	void                *sprites;
	DRAW_RECTANGLE       draw[2];
	SCREEN_POINT         screenPos;
	PARTICLE            *puff;
	PARTICLE            *particle;
	Sint32               frame;
	Sint32               i;
	Sint32               height;
	Sint32               composite;
	Sint32               x;
	Uint8               *src;
	Sint32               speed;
	Sint32               angle;
	Sint32               picture;
	Sint32               size;

	slots       = &HeapSlots[HEAP_SLOT_BATTLE_EFFECT];
	slot        = slots;
	state       = *slot++;
	canvas      = *slot;
	state->args = args;
	sprites     = HEAP_SLOT_FROM(slots, HEAP_SLOT_BATTLE_EFFECT, HEAP_SLOT_BATTLE_SHEET);
	EFFECT_START_NARROW_CANVAS(0);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprites, 0, 0);
	btResourceLoadAndDecompress(RESOURCE_LIME_COLUMN_SHEET, state->sheet, 1, 1);
	btResourceLoadAndDecompress(RESOURCE_BLAST_SHEET, state->sheet + UNDEAD_SWORD_PUFF_SHEET, 1, 0);
	btEffectBuildCanvasBlitters(state->args->side, draw);
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

	// The canvas is scrolled to stand its middle over the target.
	btProjectionUnit(state->args->targets[0], &screenPos);
	REG_BG2X = (CANVAS_SIZE / 2 - screenPos.x) << 8;

	// The puffs burst out from the blade's foot and drift upward.
	for (i = 0, puff = state->particles; i != UNDEAD_SWORD_PUFFS; i++)
	{
		speed           = (syRandom() & 0x1ff) + 128;
		angle           = syRandom() & TRIG_ANGLE_MASK;
		puff->x         = toFIXED(CANVAS_SIZE / 2);
		puff->y         = toFIXED(UNDEAD_SWORD_FOOT_Y);
		puff->velocityX = (sySin(angle) * speed) >> 8;
		puff->velocityY = (syCos(angle) * speed) >> 9;
		puff->variant   = syRandom() & 7;
		puff++;
	}
	state->shakeFrame = 8;
	for (frame = 0; frame != UNDEAD_SWORD_TICKS; frame++)
	{
		// The strike, with a second 8-tick shake.
		if (frame == 10)
		{
			state->shakeFrame = 8;
			EFFECT_SHOW_DAMAGE(SE_POWER_UP);
			btMotionApplyKnockbackStub(state->args->targets[0], KNOCKBACK_NUDGE);
			btEffectTintBattler(state->args->targets[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 8);
		}

		// The blade thrusts up 12 pixels a tick and sinks 6 a tick. While it grows it is added onto
		// its fading trail and glows; at full height it only lightens it.
		if (frame > 7)
		{
			if (frame <= 31)
				height = frame * 12 - 96;
			else
				height = 272 - frame * 6;
			if (height > 0)
			{
				if (height > LIME_COLUMN_HEIGHT)
				{
					height    = LIME_COLUMN_HEIGHT;
					composite = COMPOSITE_LIGHTEN;
				}
				else
				{
					composite = COMPOSITE_ADDITIVE;
				}
				for (i = 0, x = 50; i != 2; i++, x += LIME_COLUMN_WIDTH)
				{
					if (i == 0)
						EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, composite);
					else
						EFFECT_LOAD_BLITTER(
							HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, composite);
					// DEAD: draw[0] is fetched afresh before it is read; the ROM keeps the store.
					draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
					EFFECT_BLITTER(HEAP_SLOT_RAM_CODE)(canvas,
					                                   state->sheet,
					                                   x,
					                                   UNDEAD_SWORD_FOOT_Y - height,
					                                   LIME_COLUMN_WIDTH,
					                                   height);
					syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
				}
			}
		}

		// The puffs billow out two a tick from tick 8.
		btEffectBuildCanvasBlitters(state->args->side, draw);
		for (i = 0, particle = state->particles; i != UNDEAD_SWORD_PUFFS; i++)
		{
			if (frame >= i / 2 + 8 && particle->variant < UNDEAD_SWORD_PUFF_TICKS)
			{
				Sint32 drawX;
				Sint32 drawY;

				picture = particle->variant / 3;
				drawX   = FIXEDtoINT(particle->x);
				drawY   = FIXEDtoINT(particle->y);
				if (picture > UNDEAD_SWORD_PUFF_PICTURES - 1)
					picture = UNDEAD_SWORD_PUFF_PICTURES - 1;
				src  = state->sheet + UndeadSwordPuffPictures[picture] + UNDEAD_SWORD_PUFF_SHEET;
				size = UndeadSwordPuffSizes[picture];
				draw[0](canvas, src, drawX - size / 2, drawY - size / 2, size, size);
				particle->variant++;
				btParticleAdvanceWithGravity2D(particle, 62, -toFIXED(1) / 8);
			}
			particle++;
		}
		EFFECT_RELEASE_BLITTERS();
		if (frame <= 7)
			btCameraShake(2, 2);
		else
			btCameraShake(16, 16);
		EFFECT_END_TICK(state);
	}
	syCloseEvent(btEffectTransferCanvas);
	btEffectEndCanvasLayer();
}

#undef UNDEAD_SWORD_TICKS
#undef LIME_COLUMN_WIDTH
#undef LIME_COLUMN_HEIGHT
#undef UNDEAD_SWORD_FOOT_Y
#undef UNDEAD_SWORD_PUFF_SHEET
#undef UNDEAD_SWORD_PUFFS
#undef UNDEAD_SWORD_PUFF_PICTURES
#undef UNDEAD_SWORD_PUFF_TICKS
