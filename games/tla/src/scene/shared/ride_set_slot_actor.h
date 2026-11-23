/// Unused: sets the actor the flames check against in slot `index`.
UNUSED void Ride_SetSlotActor(u32 index, s32 value) {
  struct FlameHitWork *work = RIDE_WORK;

  if (index <= 3)
    work->slots[index].id = value;
}
