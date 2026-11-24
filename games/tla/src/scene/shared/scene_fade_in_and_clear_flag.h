/// Sets the screen to open with the window transition, and the area's name not
/// to show (FLAG_SHOW_AREA_NAME).
void Scene_FadeInAndClearFlag(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;

  state->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  GameFlag_Clear(FLAG_SHOW_AREA_NAME);
}
