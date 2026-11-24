/// @file
/// The Mars Lighthouse flame throwers' puffs, which four of its rooms keep a
/// copy of, with the views of the ride state and heap slots the routine reads
/// them through.

/// A flame thrower of the ride state, as FlameThrowers_Puff reads it.
typedef struct Entry {
  s16 active;
  s16 timer; ///< frames to its next puff
  u16 flags; ///< enum FlameThrowerFlags: which flame, or none
  /// Puffs even off screen, and its flames keep their sprites.
  s16 offScreen;
  struct Actor *actor;
  FIXED reach; ///< 16.16 pixels, from the flags' low four bits
} Entry;

/// The ride state as FlameThrowers_Puff reads it: a flame reaching its stop
/// sets stopped, which calls back once.
typedef struct State {
  u8 unusedBeforeStopped[6];
  s16 stopped;
  s16 wasStopped;
  u8 unusedAfterWasStopped[4];
  s16 runsDuringPsynergy; ///< FlameThrowers_SetMode: not paused by Psynergy
  void (*onStopped)();
  Entry entries[4];
} State;

/// The heap slots as FlameThrowers_Puff reads them: the field state through a
/// union, which keeps its read after the stores before it.
typedef struct HeapSlotsView {
  u8 unusedBeforeMapGrid[32];
  struct MapState *mapGrid;
  u8 unusedBeforeFieldState[108 - 36];
  union {
    s32 word;
    void *pointer;
  } fieldState;
} HeapSlotsView;

/// Each frame, every flame thrower in view sends its flame: forward, landing
/// forward or back, back, or a spark, by its flags, every ninth frame. They
/// pause while a menu is open, or a Psynergy is used unless the ride state runs
/// them through it; reaching a stop calls the stop's callback once.
void FlameThrowers_Puff(void) {
  s32 saved[2];
  State *state;
  Entry *entry;
  struct MapState *map;
  s32 *view;
  s32 cameraX, cameraZ;
  s32 v;
  s32 i;

  state = (State *)FLAME_THROWERS_STATE;
  map = ((HeapSlotsView *)Ram_HeapSlots)->mapGrid;
  entry = state->entries;
  view = &map->viewX;
  cameraX = view[0];
  cameraX &= FIELD_PIXEL_MASK;
  cameraZ = view[1] & FIELD_PIXEL_MASK;
  saved[1] = map->origin[1];
  saved[0] = ((HeapSlotsView *)Ram_HeapSlots)->fieldState.word;
  if (state->stopped != 0 && state->wasStopped == 0 && state->onStopped != 0)
    state->onStopped();
  v = (u16)state->stopped;
  state->wasStopped = v;
  state->stopped = 0;
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
    if (((struct FieldState *)saved[0])->psynergy != 0)
      flag |= 1;
    mark = &actor->paused;
    *mark = flag;
    if (state->runsDuringPsynergy != 0)
      *mark = GameFlag_Get(FLAG_MENU_OPEN);
    dz = actor->z.fixed;
    dx = actor->x.fixed - cameraX;
    y = actor->y.fixed;
    dz -= cameraZ;
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
