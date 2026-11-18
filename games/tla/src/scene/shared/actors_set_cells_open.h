/// Actor_SetCellOpen for each actor of a list ended by ACTOR_LIST_END.
void Actors_SetCellsOpen(const u16 *list) {
  const u16 *spec;
  u16 id;
  struct Actor *actor;
  s16 mode;
  s16 flag;
  s32 first;

  first = list[0];
  if (first == ACTOR_LIST_END) {
    return;
  }
  spec = list;
  do {
    id = spec[0];
    actor = Actor_Get(id);
    mode = spec[1];
    flag = spec[2];
    actor->collisionFlags &= ~ACTOR_SOLID;
    Actor_SetSpriteFlags(actor, 0);
    Actor_SetCellOpen(id, (u16)mode, (u16)flag, actor->layer);
    spec += 3;
    first = spec[0];
  } while (first != ACTOR_LIST_END);
}
