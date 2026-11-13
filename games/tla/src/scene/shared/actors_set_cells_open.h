/// Actor_SetCellOpen for each actor of a list ended by 0xffff.
void Actors_SetCellsOpen(const u16 *list) {
  const u16 *spec;
  u16 id;
  struct Actor *object;
  s16 mode;
  s16 flag;
  s32 limit;
  s32 first;
  limit = 0xff;
  first = list[0];
  if (first == ((limit << 8) + 0xff)) {
    return;
  }
  spec = list;
  do {
    id = spec[0];
    object = Actor_Get(id);
    mode = spec[1];
    flag = spec[2];
    object->collisionFlags &= 0xfe;
    Actor_SetSpriteFlags(object, 0);
    Actor_SetCellOpen(id, (u16)mode, (u16)flag, object->terrainId);
    limit = 0xff;
    spec += 3;
    first = spec[0];
  } while (first != ((limit << 8) + 0xff));
}
