/// Ends the falling rocks: removes their task and frees their picture and
/// state.
void Rock_EndFalling(void) {
  struct RockState *state = Ram_HeapSlots->menuState;

  FieldTask_Remove(Particle_UpdateFalling);
  VramCache_FreeEntry(state->resource);
  HeapSlot_Release(HEAP_SLOT_MENU);
}
