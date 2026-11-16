/// @file
/// Gaia Rock's climbing walls, which its outside and inside overlays keep a
/// copy of.

/// Each frame while the leader is moved by the keys alone: the collision code
/// of the wall cell he stands before, 1 to 230, is his step trigger.
void GaiaRock_TriggerWallCell(void) {
  if (gMainRoutineWork.movementMode == LEADER_MOVEMENT_KEYS) {
    struct FieldWork *work = Ram_HeapSlots->fieldWork;
    struct MapWork *map = Ram_HeapSlots->mapGrid;
    struct Actor *actor = Actor_Get(gMainRoutineWork.leaderActorId);
    s32 x = actor->x.fixed / toFIXED(16);
    s32 z = (actor->z.fixed - actor->y.fixed + 0x7fff) / toFIXED(16);
    struct MapCell *cells = map->layers[2].cells;
    struct MapCell *cell = &cells[x + (z << 7)];
    s32 code = cell->collisionCode;

    if (code >= 1 && code <= 230)
      work->stepTrigger = code;
  }
}
