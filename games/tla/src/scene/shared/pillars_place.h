/// Places each pillar of a list of three-halfword entries (actor, kind,
/// flag) ended by 0xffff, its collision bit cleared, raised or sunk by its
/// flag (Actor_SetCellOpen).
void Pillars_Place(const u16 *list) {
  const u16 *entry;

  if (list[0] == 0xffff)
    return;
  entry = list;
  do {
    u32 id = entry[0];
    struct Actor *object = Actor_Get(id);
    s16 kind = entry[1];
    s16 flag = entry[2];

    object->collisionFlags &= ~1;
    Actor_SetSpriteFlags(object, 0);
    Actor_SetCellOpen(id, (u16)kind, (u16)flag, object->terrainId);
    entry += 3;
  } while (entry[0] != 0xffff);
}
