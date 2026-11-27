/* The weather on the field: The Broken Seal's rain kept whole, this game's
   rain, and the storm's lightning. Part of script_commands.c. */

#include "field/weather.h"

/* The weather's sprites: 16 by 16 pictures of 16 colours, four tiles each, in
   the last basic sprite palette (syPaletteLoadBasicSprCols loads 0 to 13). */
#define WEATHER_PICTURE_BYTES (SPRITE_TILES(16, 16) * TILE_SIZE_4BPP)
#define WEATHER_PALETTE		  13
/* OAM attribute 1's size (GBATEK, "LCD OBJ - OAM Attributes"): an
   OBJ_SHAPE_SQUARE sprite of size 1 is 16 by 16, an OBJ_SHAPE_HORIZONTAL one of
   size 2 32 by 16. */
#define OAM_SIZE_16X16		  1
#define OAM_SIZE_32X16		  2

/* The Broken Seal's rain each tick (fdGroundParticlesStart, which nothing
   calls), kept whole: each drop falls from up to the right, splashes and plays
   its RAIN_SPLASH_FRAMES ticks of WeatherRainSplashFrames on a random spot
   near the leader, held while FLAG_WEATHER_HELD is set. */
void fdGroundParticlesUpdate(void)
{
	MAP_STATE		 *map = Ram_HeapSlots->mapGrid;
	WEATHER_STATE	 *state = Ram_HeapSlots->weather;
	Sint32			  originX = syMapStateView(map)[0];
	Sint32			  originZ = syMapStateView(map)[1];
	WEATHER_PARTICLE *particle = state->particles;
	Uint32			  i;

	for(i = 0; i < WEATHER_PARTICLE_COUNT; i++, particle++) {
		const Sint16 *frm;
		Sint32		  x;
		Sint32		  y;

		if(--particle->tmr == GROUND_PARTICLE_FREED)
			continue;
		if(syGameFlagGet(FLAG_WEATHER_HELD))
			particle->tmr++;
		frm = &WeatherRainSplashFrames[particle->tmr * 5];
		x = (particle->posX - originX) / toFIXED(1) + *frm++;
		y = (particle->posZ - particle->posY - originZ) / toFIXED(1) + *frm++;
		if(x >= -16 && x < DISPLAY_WIDTH && y >= -32 && y < DISPLAY_HEIGHT) {
			particle->pri = 1;
			particle->x = x;
			particle->y = y;
			particle->tile = state->tileBase + *(Uint16 *)frm;
			frm++;
			particle->shape = *(Uint8 *)frm;
			particle->size = *(Uint8 *)(frm + 1);
			syOAMAddSprAtDepthClamped(particle, OAM_DEPTH_WINDOW);
		}
		if(particle->tmr == 0) {
			Sint32 *focus = map->origin;

			x = focus[0] + (syRandom() << 8) - toFIXED(128);
			y = focus[2] + (syRandom() << 8) - toFIXED(128);
			particle->posX = x;
			particle->posZ = y;
			particle->posY = fdMapGridGetTerrainHeightStub(0, x, y);
			particle->tmr = RAIN_SPLASH_FRAMES;
		}
	}
}

/* This game's rain each tick (fdWeatherStartRain), outside Mars Lighthouse and
   on its ledges: each of the 64 streaks falls down and to the right onto a spot
   on the ground, a pair of GroundParticlesOffsets a tick, and then starts
   again 12 ticks up from a random spot (x within -64 to +192, z within -128 to
   +128 pixels of the map's focus). FLAG_WEATHER_HELD holds them where they
   are. */
void fdWeatherUpdateRain(void)
{
	MAP_STATE		 *map = Ram_HeapSlots->mapGrid;
	WEATHER_STATE	 *state = Ram_HeapSlots->weather;
	Sint32			  originX = syMapStateView(map)[0];
	WEATHER_PARTICLE *particle = state->particles;
	Sint32			  originZ = syMapStateView(map)[1];
	Uint32			  i;

	for(i = 0; i < WEATHER_PARTICLE_COUNT; i++, particle++) {
		const Sint16 *frm;
		Sint32		  x;
		Sint32		  y;

		if(--particle->tmr == GROUND_PARTICLE_FREED)
			continue;
		if(syGameFlagGet(FLAG_WEATHER_HELD))
			particle->tmr++;
		/* UB: a streak started at 12 ticks reads its first offsets one pair
		   past the table's end, FieldEffectSparklePictures's first row: for
		   that tick it is drawn 2 pixels left of its spot and 12 below it.
		   UBFIX starts it at 11. */
		frm = &GroundParticlesOffsets[2 * particle->tmr];
		x = (particle->posX - originX) / toFIXED(1) + *frm++;
		y = (particle->posZ - particle->posY - originZ) / toFIXED(1) + *frm++;
		if(x >= -16 && x < DISPLAY_WIDTH && y >= -32 && y < DISPLAY_HEIGHT) {
			particle->pri = 1;
			particle->x = x;
			particle->y = y;
			particle->tile = state->tileBase;
			particle->shape = OBJ_SHAPE_HORIZONTAL;
			particle->size = OAM_SIZE_32X16;
			syOAMAddSprAtDepthClamped(particle, OAM_DEPTH_WINDOW);
		}
		if(particle->tmr == 0) {
			Sint32 *focus = map->origin;

			x = focus[0] + (syRandom() << 8) - toFIXED(64);
			y = focus[2] + (syRandom() << 8) - toFIXED(128);
			particle->posX = x;
			particle->posZ = y;
			particle->posY = fdMapGridGetTerrainHeightStub(0, x, y);
#if FIX_RAIN_STREAK_FIRST_TICK
			particle->tmr = 11;
#else
			particle->tmr = 12;
#endif
		}
	}
}

/* The storm each tick: lightning flashes the BG colours three times, 5 ticks
   apart, with thunder; while lightning is on, the next burst comes 51 to 549
   ticks later. While FLAG_WEATHER_HELD is set no burst comes. */
void fdWeatherUpdateLightning(void)
{
	FIELD_BLEND_STATE *state = Ram_HeapSlots->fieldBlend;
	PALETTE_FADE	  *tgt = Ram_HeapSlots->palFade;
	if(state->tmr < 0)
		return;
	if(syGameFlagGet(FLAG_WEATHER_HELD))
		state->tmr = 128;
	switch(state->tmr--) {
	case 0:
		/* The burst ends: arm the next, 150 ticks plus up to 399 less up to
		   99. */
		if(state->enabled != 0) {
			state->tmr = syRandomBelow(400) - syRandomBelow(100) + 150;
			if(state->loud != 0)
				sySndPlayStub(SE_THUNDER_LOUD);
			else
				sySndPlayStub(SE_THUNDER);
		}
		/* fall through */
	case 5:
	case 10:
		/* A tick after each flash: fade back to the dark colours over 12
		   ticks. */
		fdPaletteFadeSetTarget((Sint32)state, 1);
		syDMAXCopy(state->delta, tgt->delta,
			DMA_COPY_WORDS(sizeof tgt->delta / 4), REG_DMA3);
		tgt->duration = 12;
		tgt->step = 0;
		break;
	case 1:
	case 6:
	case 11:
		/* The flash: the lit colours, in one tick. */
		fdPaletteFadeSetTarget((Sint32)state->to, 1);
		tgt->duration = 1;
		tgt->step = 0;
		break;
	}
}
