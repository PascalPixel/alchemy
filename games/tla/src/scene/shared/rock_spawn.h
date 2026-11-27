/* Throws `cnt` rocks from a 16.16 point, as Burst breaks stone: each starts 8
   to 16 pixels out at a random angle and flies up at 5 pixels a tick and out
   along that angle at 2 to 4. */
void scRockSpawn(Sint32 x, Sint32 y, Sint32 z, Sint32 cnt)
{
	ROCK_STATE *state;
	Sint32		i;

	state = RamHeapSlots->menuState;
	for(i = 0; i < cnt; i++) {
		Sint32 ang = syRandom();
		ROCK  *rock = &state->rocks[state->next];

		rock->tmr = 0;
		rock->pos[0] = x;
		rock->pos[1] = y;
		rock->pos[2] = z;
		syFieldVectorAddPolarOffsetFX(
			(syRandom() << 3) + toFIXED(8), ang, rock->pos);
		rock->vel[0] = 0;
		rock->vel[1] = toFIXED(5);
		rock->vel[2] = 0;
		syFieldVectorAddPolarOffsetFX(
			(syRandom() << 1) + toFIXED(2), ang, rock->vel);
		state->next++;
		state->next &= ARRAY_COUNT(state->rocks) - 1;
	}
}
