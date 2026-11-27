// Starts the HBlank scroll handler and its event.
void scScrollStartHBlank(void)
{
	Sint32 priority;

	syIntFunction(IRQ_HBLANK, 0, scScrollHBlank);
	priority = EVENT_PASS_VBLANK;
	syFieldSetTickEvent(scLayerTrackScroll, priority);
}
