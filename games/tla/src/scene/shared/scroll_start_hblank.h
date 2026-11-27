/* Starts the HBlank scroll handler and its task. */
void scScrollStartHBlank(void) {
	s32 priority;

	syIntFunction(IRQ_HBLANK, 0, scScrollHBlank);
	priority = TASK_PASS_VBLANK;
	syFieldTaskAddFrame(scLayerTrackScroll, priority);
}
