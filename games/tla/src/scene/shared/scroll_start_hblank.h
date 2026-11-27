/* Starts the HBlank scroll handler and its event. */
void scScrollStartHBlank(void)
{
	Sint32 pri;

	syIntFunction(IRQ_HBLANK, 0, scScrollHBlank);
	pri = EVENT_PASS_VBLANK;
	syFieldSetTickEvent(scLayerTrackScroll, pri);
}
