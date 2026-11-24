/// @file
/// Colosseum Final 1, 2 and 3, and Venus Lighthouse: who stands at a place.

/// Returns the placed actor (8 to 65) whose cell, and 16-pixel step of
/// height, is that of the 16.16 position, or 0. self is not read.
static void *Actor_FindAtPosition(const struct FieldPosition *position,
                                  void *self) {
  struct FieldState *state = gFieldState;
  struct Actor **slots = state->actors;
  u32 i;

  for (i = ACTOR_FIRST_PLACED;
       i < sizeof(state->actors) / sizeof(state->actors[0]); i++) {
    struct Actor *actor = slots[i];

    // UB: Actor_ClearAll leaves unused slots null.
#ifdef UBFIX
    if (actor == NULL)
      continue;
#endif
    if (FIELD_CELL(position->x) == FIELD_CELL(actor->x.fixed) &&
        FIELD_CELL(position->y) == FIELD_CELL(actor->y.fixed) &&
        FIELD_CELL(position->z) == FIELD_CELL(actor->z.fixed)) {
      return actor;
    }
  }
  return 0;
}
