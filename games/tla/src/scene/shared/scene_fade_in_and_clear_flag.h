/// Fades the screen in and clears flag 0x12f.
void Scene_FadeInAndClearFlag(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  work->transition = 0x209;
  GameFlag_Clear(FLAG_SHOW_AREA_NAME);
}
