/// Runs FlagBlock_ApplyFlags, then raises the held actor above the room.
void Scene_RaiseHeldActor(void) {
  u8 *event = Ram_HeapSlots->fieldWork;
  struct Actor *actor;
  s32 flags;

  FlagBlock_ApplyFlags(gDailaRooms_PillarFrozenFlags);
  actor = *(struct Actor **)(event + 0xcbc);
  flags = 1;
  flags |= actor->priorityFlags;
  flags &= 0xfd;
  actor->priorityFlags = flags;
}
