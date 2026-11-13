/// Starts the HBlank scroll handler and its task.
void Scroll_StartHBlank(void) {
  s32 priority;

  Irq_SetHandler(1, 0, Scroll_HBlank);
  priority = 0xc80;
  FieldTask_AddFrame(Layer_TrackScroll, priority);
}
