/// Runs FlagBlock_ApplyFlags, then raises the held actor above the room.
void Scene_RaiseHeldActor(void) {
  u8 *event = Ram_HeapSlots->eventWork;
  struct Actor *object;
  s32 flags;

  FlagBlock_ApplyFlags(gDailaInteriors_PillarFrozenFlags);
  object = *(struct Actor **)(event + 0xcbc);
  flags = 1;
  flags |= object->priorityFlags;
  flags &= 0xfd;
  object->priorityFlags = flags;
}
