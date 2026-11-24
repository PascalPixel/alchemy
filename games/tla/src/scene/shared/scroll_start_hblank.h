/// Starts the HBlank scroll handler and its task.
void Scroll_StartHBlank(void) {
  s32 priority;

  Irq_SetHandler(IRQ_HBLANK, 0, Scroll_HBlank);
  priority = TASK_PASS_VBLANK;
  FieldTask_AddFrame(Layer_TrackScroll, priority);
}
