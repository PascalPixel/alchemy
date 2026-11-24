/// Unused: sets the actor the flames check against in slot `index`.
UNUSED void FlameThrowers_SetStop(u32 index, s32 value) {
  struct FlameHitState *state = FLAME_THROWERS_STATE;

  if (index <= 3)
    state->slots[index].id = value;
}
