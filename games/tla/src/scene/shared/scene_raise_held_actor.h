/// Frost freezes one of Daila's puddles into a pillar of ice, which then takes
/// its priority from its cell and is no longer drawn underfoot.
void Scene_RaiseHeldActor(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *pillar;
  s32 flags;

  FlagBlock_ApplyFlags(gDailaRooms_PillarFrozenFlags);
  pillar = work->movedBlock;
  flags = ACTOR_PRIORITY_AUTOMATIC;
  flags |= pillar->priorityFlags;
  flags &= (u8)~ACTOR_PRIORITY_UNDERFOOT;
  pillar->priorityFlags = flags;
}
