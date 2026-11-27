/* fdWeatherStopLightning, fdWeatherLastLightning and
   fdWeatherResumeLightning, the same in both games. The Broken Seal's
   field/script_commands_weather.c and The Lost Age's
   field/script_commands_motes.c include it where the functions stand. */

/* Stops the lightning: no flash for 32767 ticks, and none after. */
void fdWeatherStopLightning(void)
{
	FIELD_BLEND_STATE *state =
		syHeapSlotAllocEWRAM(HEAP_SLOT_FIELD_BLEND, sizeof(FIELD_BLEND_STATE));
	state->tmr = 32767;
	state->enabled = 0;
}

/* One last burst of lightning, its first flash two ticks from now (timer 12;
   fdWeatherUpdateLightning flashes at 11), then no more. */
void fdWeatherLastLightning(void)
{
	FIELD_BLEND_STATE *state =
		syHeapSlotAllocEWRAM(HEAP_SLOT_FIELD_BLEND, sizeof(FIELD_BLEND_STATE));
	state->tmr = 12;
	state->enabled = 0;
}

/* Restarts the lightning: the next burst ends in 128 ticks, its first flash
   118 ticks from now, and bursts repeat. */
void fdWeatherResumeLightning(void)
{
	FIELD_BLEND_STATE *state =
		syHeapSlotAllocEWRAM(HEAP_SLOT_FIELD_BLEND, sizeof(FIELD_BLEND_STATE));
	state->tmr = 128;
	state->enabled = 1;
}
