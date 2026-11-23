/// Each frame, raises or lowers each geyser on its timer, writes its height to
/// the collision and cell maps, and carries its platform with a bob.
void Geyser_Update(void) {
  struct GeyserWork *work = GEYSER_WORK;
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct Geyser *entry = work->entries;
  s32 firstTerrain = work->firstTerrain;
  u8 *terrain = map->layers[0].terrainIds;
  struct MapCell *cells = map->layers[0].cells;
  s32 i = 0;
  s32 *collision;

  for (collision = (s32 *)Ram_MapCollision + firstTerrain; i < GEYSER_ENTRIES;
       collision++, entry++, i++) {
    struct Actor *actor;
    struct Actor *rider;
    struct MapCell *cell;
    s32 index;

    if (entry->id == 0)
      continue;
    actor = Actor_GetById(entry->id);
    if (actor->x.fixed == 0 && actor->z.fixed == 0)
      continue;
    if (entry->timer == entry->riseAt) {
      Sound_Play(SE_CURSE_FALL);
      entry->step = 4;
    }
    if (entry->timer == entry->fallAt) {
      Sound_Play(SOUND_FADE_OUT_EXIT_SE);
      entry->step = -8;
    }
    if (entry->step != 0) {
      entry->height += entry->step;
      if (entry->height >= entry->max) {
        entry->height = entry->max;
        entry->step = 0;
        entry->phase = 0;
      }
      if (entry->height <= entry->min) {
        entry->height = entry->min;
        entry->step = 0;
        entry->phase = 0;
      }
    }
    if (!GameFlag_Get(FLAG_FIELD_HANDLING_REQUESTS)) {
      if (++entry->timer >= entry->period)
        entry->timer = 0;
    }
    index =
        FIELD_CELL(actor->x.fixed) + FIELD_CELL(actor->z.fixed) * MAPGRID_SIZE;
    terrain[index] = (u8)work->firstTerrain + i;
    *collision = ((actor->y.part[0].integer + entry->height) / 8) << 8;
    cell = &cells[index];
    if (entry->step != 0)
      cell->attributeB = 0;
    else
      cell->attributeB |= MAP_CELL_JUMP;
    if (entry->height != 0)
      cell->attributeB |= MAP_CELL_WATER;
    else
      cell->attributeB = 0;
    rider = entry->rider;
    if (rider == NULL)
      continue;
    if (entry->height == 0) {
      rider->y.fixed = actor->y.fixed + -toFIXED(16);
    } else {
      FIXED wave;

      entry->phase += TRIG_TURN / 16;
      wave = Iwram_MulFX(toFIXED(2), Trig_Sin(entry->phase)) - toFIXED(14);
      rider->y.fixed = actor->y.fixed + toFIXED(entry->height) + wave;
    }
    rider->x.fixed = actor->x.fixed;
    rider->z.fixed = actor->z.fixed;
  }
}
