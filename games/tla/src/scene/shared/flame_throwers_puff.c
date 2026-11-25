/// @file
/// The Mars Lighthouse flame throwers' puffs, which four of its rooms keep a
/// copy of.

/// Each frame, every flame thrower in view sends its flame (forward, back, east
/// or west) or a spark, as its flags say, every ninth frame. They pause while a
/// menu is open, or while a Psynergy is used unless FlameThrowers_SetMode keeps
/// them running; the first frame a flame meets a blocked stop calls the
/// state's callback.
void FlameThrowers_Puff(void) {
  FIXED height;
  struct FieldState *field;
  struct FlameHitState *state;
  struct FlameThrower *entry;
  struct MapState *map;
  s32 cameraX, cameraZ;
  s32 i;

  state = FLAME_THROWERS_STATE;
  map = Ram_HeapSlots->mapGrid;
  entry = state->entries;
  cameraX = MapState_View(map)[0];
  cameraX &= FIELD_PIXEL_MASK;
  cameraZ = MapState_View(map)[1] & FIELD_PIXEL_MASK;
  height = map->origin[1];
  field = Ram_HeapSlots->fieldState;
  if (state->blocked != 0 && state->wasBlocked == 0 && state->onBlocked != 0)
    state->onBlocked();
  state->wasBlocked = state->blocked;
  state->blocked = 0;
  for (i = 0; i < 4; i++, entry++) {
    struct Actor *actor;
    s32 flag, x, y, offScreen, value;
    FIXED dx, dz;
    u8 *mark;

    if (entry->active == 0)
      continue;
    actor = entry->actor;
    if (actor == 0)
      continue;
    flag = GameFlag_Get(FLAG_MENU_OPEN);
    if (field->psynergy != 0)
      flag |= 1;
    mark = &actor->paused;
    *mark = flag;
    if (state->runsDuringPsynergy != 0)
      *mark = GameFlag_Get(FLAG_MENU_OPEN);
    dx = actor->x.fixed - cameraX;
    y = actor->y.fixed - height;
    dz = actor->z.fixed - cameraZ - height;
    y = dz - y;
    offScreen = entry->offScreen;
    x = FIXEDtoINT(dx);
    y = FIXEDtoINT(y) - 8;
    // Unless it puffs off screen, only one in view: x from -7 to 327, y from
    // -47 to 239.
    if (offScreen == 0) {
      if ((u32)(x + 7) > 334)
        continue;
      if (y <= -48)
        continue;
      if (y > 239)
        continue;
    }
    y = (u16)entry->timer;
    if (entry->timer <= 0) {
      y = entry->flags & FLAME_THROWER_FLAG_BITS;
      switch (y) {
      case FLAME_SPARK:
        FlameThrower_SendSpark((void *)&entry->actor->x, entry->reach,
                               offScreen);
        break;
      case FLAME_BACKWARD:
        FlameThrower_SendBackward(&entry->actor->x.fixed, entry->reach,
                                  offScreen);
        break;
      case FLAME_WEST:
        FlameThrower_SendWest(&entry->actor->x.fixed, entry->reach, offScreen);
        break;
      case FLAME_EAST:
        FlameThrower_SendEast(&entry->actor->x.fixed, entry->reach, offScreen);
        break;
      case FLAME_FORWARD:
        FlameThrower_SendForward(&entry->actor->x.fixed, entry->reach,
                                 offScreen);
        break;
      }
      value = 8; // the next in nine frames
    } else {
      value = y - 1;
    }
    entry->timer = value;
  }
}
