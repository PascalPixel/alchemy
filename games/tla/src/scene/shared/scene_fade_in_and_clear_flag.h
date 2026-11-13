/// Fades the screen in and clears flag 0x12f.
void Scene_FadeInAndClearFlag(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  work->screenEffect = 0x209;
  GameFlag_Clear(FLAG_SHOW_AREA_NAME);
}
