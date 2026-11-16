/// Runs FlagBlock_ApplyFlags, then raises the held actor above the room.
void Scene_RaiseHeldActor(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *actor;
  s32 flags;

  FlagBlock_ApplyFlags(gDailaRooms_PillarFrozenFlags);
  actor = work->movedBlock;
  flags = 1;
  flags |= actor->priorityFlags;
  flags &= 0xfd;
  actor->priorityFlags = flags;
}
