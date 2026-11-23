/// Looks up the cell Burst is cast at in an (actor, cell, value) table ended by
/// ACTOR_LIST_END: on a match, makes the row's statue the target and returns
/// its value.
s32 Statue_FindUnderBurst(u16 *table) {
  struct FieldEffectWork *watch = Ram_HeapSlots->fieldEffectWork;
  s32 result = 0;
  s32 cell = MapCell_GetAttributes(MAPGRID_LAYER_GROUND, FIELD_CELL(watch->x),
                                   FIELD_CELL(watch->z)) >>
             8;
  u32 id;

  for (; *table != ACTOR_LIST_END; result = *table++) {
    id = *table++;
    if (cell == *table++) {
      watch->child = Actor_Get(id);
      result = *table;
      break;
    }
  }
  return result;
}
