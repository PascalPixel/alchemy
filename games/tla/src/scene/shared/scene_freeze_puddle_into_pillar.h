/// Frost freezes one of the puddles in Daila's streets or its sanctuary into a
/// pillar of ice, which then takes its priority from its cell and is no longer
/// drawn underfoot.
void Scene_FreezePuddleIntoPillar(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  struct Actor *pillar;
  s32 flags;

  FlagBlock_ApplyFlags(gDailaRooms_PillarFrozenFlags);
  pillar = state->movedBlock;
  flags = ACTOR_PRIORITY_AUTOMATIC;
  flags |= pillar->priorityFlags;
  flags &= (u8)~ACTOR_PRIORITY_UNDERFOOT;
  pillar->priorityFlags = flags;
}
