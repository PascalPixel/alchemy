/// @file
/// A rolling fireball, which four Mars Lighthouse rooms keep a copy of. The
/// includer defines ABS and its own Actor_ScatterCrumbs before it.

/// Ice a rolling fireball breaks beside its path: the small crystals on the
/// middle layer crack, the tall ones on the overhead layer crumble.
enum { CELL_BEHAVIOR_BREAKABLE_ICE = 77 };

/// A fireball rolls forward, breaking the ice in the cells around it, and
/// carries the leader along if it hits him. Where the ground 8 pixels ahead is
/// a rounded square its path ends: it crashes, calls the routine its path left
/// in watchedActor, then shrinks away in crumbs.
void Actor_RollAndCrumble(struct Actor *actor) {
  struct Vector3 position;
  struct FieldState *field = Ram_HeapSlots->fieldState;
  s32 state = actor->timer;

  if (state == 0) {
    s32 dz;
    s32 dx;
    s32 tileX;
    struct MapCell *middleCells;
    struct MapCell *overheadCells;
    s32 middleOffsetX;
    s32 overheadOffsetX;
    s32 offset;
    s32 overheadZ;
    s32 tileZ;
    s32 middleOffsetZ;
    s32 overheadOffsetZ;
    s32 x;
    s32 z;
    s32 middleX;
    s32 overheadX;
    s32 middleZ;
    s32 paused;
    struct MapState *map;
    struct MapCell *cell;
    struct FieldState *fieldState;
    struct FlameHitState *hit;
    struct Actor *leader;

    paused = Event_GetFlag(FLAG_MENU_OPEN);
    if (field->psynergy != 0)
      paused |= 1;
    actor->paused = paused;

    position.x = actor->x.fixed;
    position.y = actor->y.fixed;
    position.z = actor->z.fixed;
    FieldVector_AddPolarOffset(8, actor->facing, &position);
    if (MapGrid_GetTerrainKindOrBumpAt(0, position.x, position.z) ==
        TERRAIN_ROUNDED_SQUARE) {
      actor->targetZ = ACTOR_NO_TARGET;
      actor->targetY = ACTOR_NO_TARGET;
      actor->targetX = ACTOR_NO_TARGET;
      actor->acceleration = toFIXED(1) / 4;
      actor->timer = 1;
      Event_PlaySound(SE_CRASH);
      MapGrid_SetShake(5, 5, 1);
      MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
      if (actor->watchedActor != NULL)
        ((void (*)(struct Actor *))actor->watchedActor)(actor);
    }

    map = Ram_HeapSlots->mapGrid;
    tileX = FIELD_CELL(actor->x.fixed);
    tileZ = FIELD_CELL(actor->z.fixed);
    middleCells =
        &map->layers[MAPGRID_LAYER_MIDDLE].cells[tileX + tileZ * MAPGRID_SIZE];
    overheadCells = &map->layers[MAPGRID_LAYER_OVERHEAD]
                         .cells[tileX + tileZ * MAPGRID_SIZE];
    middleOffsetX = FIELD_CELL(map->layers[MAPGRID_LAYER_MIDDLE].offsetX);
    middleOffsetZ = FIELD_CELL(map->layers[MAPGRID_LAYER_MIDDLE].offsetY);
    overheadOffsetX = FIELD_CELL(map->layers[MAPGRID_LAYER_OVERHEAD].offsetX);
    overheadOffsetZ = FIELD_CELL(map->layers[MAPGRID_LAYER_OVERHEAD].offsetY);

    for (dz = -1, overheadZ = tileZ + overheadOffsetZ - 2, z = tileZ - 1,
        middleZ = tileZ + middleOffsetZ - 1;
         dz <= 1; overheadZ++, z++, middleZ++, dz++) {
      for (dx = -1, overheadX = tileX + overheadOffsetX - 1, x = tileX - 1,
          middleX = tileX + middleOffsetX - 1;
           dx <= 1; overheadX++, x++, middleX++, dx++) {
        offset = ((dz << 7) + dx) * 4;
        cell = (struct MapCell *)(offset + (s32)middleCells);
        if (cell->collisionCode == CELL_BEHAVIOR_BREAKABLE_ICE) {
          cell->collisionCode = CELL_BEHAVIOR_NORMAL;
          MapGrid_CopyCellsLayered(64, 64, 1, 1, middleX, middleZ);
          MapGrid_CopyCellAttributes(64, 64, 1, 1, x, z);
          Event_PlaySound(SE_ICE_CRACK);
          IceChips_Spawn(FIELD_CELL_CENTER(x), actor->y.fixed,
                         FIELD_CELL_CENTER(z), actor->facing);
        }
        cell = (struct MapCell *)(offset + (s32)overheadCells);
        if (cell->collisionCode == CELL_BEHAVIOR_BREAKABLE_ICE) {
          cell->collisionCode = CELL_BEHAVIOR_NORMAL;
          MapGrid_CopyCellsLayered(64, 64, 1, 2, overheadX, overheadZ);
          MapGrid_CopyCellAttributes(64, 64, 1, 1, x, z);
          Event_PlaySound(SE_CRUMBLE);
          IceChips_Spawn(FIELD_CELL_CENTER(x), actor->y.fixed,
                         FIELD_CELL_CENTER(z), actor->facing);
        }
      }
    }

    if (actor->scaleX < FIXED_ONE) {
      actor->scaleX += FIXED_ONE / 16;
      actor->scaleY += FIXED_ONE / 16;
    }
    FieldActor_SetMoveTargetFX(actor, position.x, position.y, position.z);

    fieldState = Ram_HeapSlots->fieldState;
    hit = FLAME_THROWERS_STATE;
    leader = Actor_Get(gMainRoutineState.leaderActorId);
    if (ABS(actor->x.fixed - leader->x.fixed) < toFIXED(20) &&
        (u32)(actor->y.fixed - leader->y.fixed + (toFIXED(8) - 1)) <=
            (toFIXED(32) - 2) &&
        ABS(actor->z.fixed - leader->z.fixed) < toFIXED(20)) {
      // A fireball's hit (a flame's is 1) raises STEP_TRIGGER_FIREBALL_HIT.
      hit->active = 2;
      hit->car = actor;
      fieldState->stepTrigger = hit->hitTrigger + (STEP_TRIGGER_FIREBALL_HIT -
                                                   STEP_TRIGGER_FLAME_HIT);
    }
  } else if (state == 1) {
    actor->paused = 0;
    if (actor->scaleX > 0) {
      actor->scaleX -= FIXED_ONE / 16;
      actor->scaleY -= FIXED_ONE / 16;
      Actor_ScatterCrumbs(actor);
    } else {
      actor->z.fixed = 0;
      actor->y.fixed = 0;
      actor->x.fixed = 0;
      actor->velocityZ = 0;
      actor->velocityY = 0;
      actor->velocityX = 0;
      actor->update = NULL;
    }
  }
}
