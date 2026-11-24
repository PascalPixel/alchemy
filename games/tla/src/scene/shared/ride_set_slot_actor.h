/// Unused: sets the actor the flames check against in slot `index`.
UNUSED void Ride_SetSlotActor(u32 index, s32 value) {
  struct FlameHitState *state = RIDE_STATE;

  if (index <= 3)
    state->slots[index].id = value;
}
