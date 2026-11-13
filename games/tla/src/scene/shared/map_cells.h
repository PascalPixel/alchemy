/// @file
/// Map cells under actors: opening and closing them, their shapes, and the
/// pillars, blocks, statues and traps that change them.
/// Each routine is a macro a scene expands where its copy sits, with the
/// names of its own data and calls.
#ifndef ALCHEMY_SCENE_SHARED_MAP_CELLS_H
#define ALCHEMY_SCENE_SHARED_MAP_CELLS_H

/// Opens or closes the cell under an actor by flag, setting its pose by `kind`.
/// 4 copies: airs_rock_inside, tundaria_tower/events_f, yampi_desert,
/// yampi_desert_cave.
#define DEFINE_ACTOR_SET_CELL_OPEN(name, call1, call2)                         \
  void name(s32 id, u32 kind, s32 flag, s32 layer) {                           \
    struct Actor *object = Actor_GetById(id);                                  \
    struct MapCell *cell = Ram_HeapSlots->mapWork->layers[layer].cells;        \
                                                                               \
    cell += call1(object);                                                     \
    if (GameFlag_Test(flag)) {                                                 \
      cell->collisionCode = 0;                                                 \
      object->priorityFlags |= 2;                                              \
      switch (kind) {                                                          \
      case 0:                                                                  \
        call2(object, 2);                                                      \
        break;                                                                 \
      case 1:                                                                  \
        call2(object, 4);                                                      \
        break;                                                                 \
      case 2:                                                                  \
        call2(object, 6);                                                      \
        break;                                                                 \
      }                                                                        \
    } else {                                                                   \
      cell->collisionCode = 255;                                               \
      switch (kind) {                                                          \
      case 0:                                                                  \
        call2(object, 1);                                                      \
        break;                                                                 \
      case 1:                                                                  \
        call2(object, 3);                                                      \
        break;                                                                 \
      case 2:                                                                  \
        call2(object, 5);                                                      \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
  }

/// Checks the trap under the leader: the first step sets it off, the second
/// drops it.
/// 2 copies: magma_rock_inside/s3678, trial_road/s15a8.
#define DEFINE_TRAPS_CHECK(name, call1, call2, call3, call4, call5, call6)     \
  void name(const u16 *table) {                                                \
    struct Actor *leader;                                                      \
    struct Actor *trap;                                                        \
    s32 position[3];                                                           \
    s32 id;                                                                    \
    s32 flag;                                                                  \
    s32 second;                                                                \
                                                                               \
    if (call1() != 0)                                                          \
      return;                                                                  \
    leader = Actor_GetById(gMainRoutineWork.currentOwner);                     \
    position[0] = leader->x;                                                   \
    position[1] = leader->y;                                                   \
    position[2] = leader->z;                                                   \
    trap = call2(position, 0);                                                 \
    if (trap != 0)                                                             \
      goto test;                                                               \
    return;                                                                    \
  next:                                                                        \
    id = table[0];                                                             \
    if (Actor_GetById(id) != trap) {                                           \
      table += 2;                                                              \
      goto test;                                                               \
    }                                                                          \
    {                                                                          \
      flag = table[1];                                                         \
      Event_Start();                                                           \
      if (!GameFlag_Test(flag)) {                                              \
        Sound_Play(125);                                                       \
        Actor_SetSpriteColor(Actor_GetById(id), 7);                          \
        wait(2);                                                               \
        call3(trap, 0);                                                        \
        Actor_SetSpriteColor(Actor_GetById(id), 0);                          \
        wait(2);                                                               \
        Actor_SetSpriteColor(Actor_GetById(id), 7);                          \
        wait(4);                                                               \
        Actor_SetSpriteColor(Actor_GetById(id), 0);                          \
        call4(0);                                                              \
        GameFlag_SetBit(flag);                                                 \
      } else {                                                                 \
        second = flag + 1;                                                     \
        if (!GameFlag_Test(second)) {                                          \
          leader->motionFlags = 0;                                             \
          Sound_Play(185);                                                     \
          MapGrid_SetShake(0x10000, 0x40000, 0x10000);                         \
          call4(0);                                                            \
          wait(8);                                                             \
          call3(trap, 2);                                                      \
          call5(-2, trap->terrainId);                                          \
          call4(1);                                                            \
          wait(16);                                                            \
          call5(-2, trap->terrainId);                                          \
          wait(4);                                                             \
          MapGrid_SetShake(-1, -1, 0xe666);                                    \
          wait(8);                                                             \
          leader->motionFlags = 3;                                             \
          wait(5);                                                             \
          Actor_SetPositionAndResetMotion(trap, 0, 0, 0);                      \
          wait(2);                                                             \
          Sound_Play(188);                                                     \
          call6();                                                             \
          wait(20);                                                            \
          GameFlag_SetBit(second);                                             \
        }                                                                      \
      }                                                                        \
      Event_End();                                                             \
      return;                                                                  \
    }                                                                          \
  test:                                                                        \
    if (*table != 0xffff)                                                      \
      goto next;                                                               \
  }

/// Puts the pillars of a list back where their flags left them.
/// 2 copies: magma_rock/s22ac, magma_rock_inside/s3678.
#define DEFINE_PILLARS_RESTORE(name, call1, call2, call3, call4, call5)        \
  void name(u16 *list) {                                                       \
    struct MapWork *map = Ram_HeapSlots->mapWork;                              \
    struct Actor *object;                                                      \
    s32 id;                                                                    \
    s32 flag;                                                                  \
    s32 nextFlag;                                                              \
    struct MapCell *cells;                                                     \
    u8 *shapes;                                                                \
    struct MapCell *cell;                                                      \
    s32 index;                                                                 \
    u8 *grid = gMapShapeGrid; /* the shape grid, read once */                  \
                                                                               \
    Actor_MoveToObject(8, gMainRoutineWork.currentOwner);                      \
    Actor_MoveToObject(9, gMainRoutineWork.currentOwner);                      \
    Actor_MoveToObject(10, gMainRoutineWork.currentOwner);                     \
    System_Wait(1);                                                            \
    call1(8);                                                                  \
    call1(9);                                                                  \
    call1(10);                                                                 \
    Actor_SetAnimation(9, 0);                                                  \
    System_Wait(1);                                                            \
    Actor_SetPosition(8, 0, 0);                                                \
    Actor_SetPosition(9, 0, 0);                                                \
    Actor_SetPosition(10, 0, 0);                                               \
    System_Wait(1);                                                            \
    if (*list == 0xffff)                                                       \
      goto done;                                                               \
  next:                                                                        \
    id = list[0];                                                              \
    object = Actor_Get(id);                                                    \
    flag = list[1];                                                            \
    nextFlag = flag + 1;                                                       \
    cells = map->layers[object->terrainId].cells;                              \
    shapes = grid + (cells - (struct MapCell *)Ram_MapCellBuffer);             \
    object->unk5c[0] = 3;                                                      \
    if (Event_GetFlag(nextFlag)) {                                             \
      Actor_SetPosition(id, 0, 0);                                             \
    } else {                                                                   \
      s32 shape;                                                               \
      u8 *slot;                                                                \
      s32 height;                                                              \
                                                                               \
      index = call2(object);                                                   \
      cell = &cells[index];                                                    \
      slot = shapes + index;                                                   \
      shape = call3(object->terrainId, object->x, object->z);                  \
      height = Map_GetTerrainHeight(object->terrainId, object->x, object->z);  \
      call4(id);                                                               \
      cell->attributeB = 128;                                                  \
      *slot = call5(object->terrainId, shape, (height >> 19) + 4);             \
      if (Event_GetFlag(flag))                                                 \
        Actor_CopyGraphics(id, 9);                                    \
    }                                                                          \
    list += 2;                                                                 \
    if (*list != 0xffff)                                                       \
      goto next;                                                               \
  done:                                                                        \
    System_Wait(10);                                                           \
  }

/// Pillars_Restore without the reset, lifting the leader onto the ground.
/// 2 copies: magma_rock/s22ac, magma_rock_inside/s3678.
#define DEFINE_PILLARS_RESTORE_RAISED(name, call1, call2, call3, call4, call5) \
  void name(const u16 *table) {                                                \
    struct MapWork *map = Ram_HeapSlots->mapWork;                              \
    struct Actor *object;                                                      \
    s32 id;                                                                    \
    s32 flag;                                                                  \
    s32 nextFlag;                                                              \
    struct MapCell *cells;                                                     \
    u8 *shapes;                                                                \
    struct MapCell *cell;                                                      \
    s32 index;                                                                 \
    struct Actor *leader;                                                      \
    s32 ground;                                                                \
                                                                               \
    if (*table == 0xffff)                                                      \
      goto done;                                                               \
  next:                                                                        \
    id = table[0];                                                             \
    object = Actor_Get(id);                                                    \
    flag = table[1];                                                           \
    nextFlag = flag + 1;                                                       \
    cells = map->layers[object->terrainId].cells;                              \
    shapes = gMapShapeGrid + (cells - (struct MapCell *)Ram_MapCellBuffer);    \
    if (Event_GetFlag(nextFlag)) {                                             \
      Actor_SetPosition(id, 0, 0);                                             \
    } else {                                                                   \
      s32 shape;                                                               \
      u8 *slot;                                                                \
      s32 height;                                                              \
                                                                               \
      index = call1(object);                                                   \
      cell = &cells[index];                                                    \
      slot = shapes + index;                                                   \
      shape = call2(object->terrainId, object->x, object->z);                  \
      height = Map_GetTerrainHeight(object->terrainId, object->x, object->z);  \
      call3(id);                                                               \
      cell->attributeB = 128;                                                  \
      *slot = call4(object->terrainId, shape, (height >> 19) + 4);             \
      if (Event_GetFlag(flag))                                                 \
        call5(object, 0);                                                      \
    }                                                                          \
    table += 2;                                                                \
    if (*table != 0xffff)                                                      \
      goto next;                                                               \
  done:                                                                        \
    leader = Actor_Get(gMainRoutineWork.currentOwner);                         \
    ground = Map_GetTerrainHeight(leader->terrainId, leader->x, leader->z);    \
    if (leader->y < ground) {                                                  \
      leader->terrainHeight = ground;                                          \
      leader->y = ground;                                                      \
    }                                                                          \
  }

/// Raises the pillars of a list out of the ground, making their cells solid.
/// 2 copies: dehkan_plateau/s0bc0, madra_catacombs.
#define DEFINE_PILLARS_RAISE(name, call1, call2, call3)                        \
  void name(u16 *list) {                                                       \
    struct MapWork *map = Ram_HeapSlots->mapWork;                              \
    struct Actor *leader;                                                      \
    s32 height;                                                                \
                                                                               \
    while (((struct ListedActor *)list)->id != 0xffff) {                       \
      struct Actor *object = Actor_GetById(*list);                             \
      u8 *layer = &object->terrainId;                                          \
      s32 shape = call1(*layer, object->x, object->z);                         \
      s32 level = Map_GetTerrainHeight(*layer, object->x, object->z);          \
      s32 index;                                                               \
      u8 *cells;                                                               \
      u8 *shapes;                                                              \
      u8 newShape;                                                             \
      u8 *grid;                                                                \
                                                                               \
      level >>= 19;                                                            \
      index = call2(object);                                                   \
      cells = (u8 *)map->layers[*layer].cells;                                 \
      grid = Ram_MapShapeGrid;                                                 \
      shapes = grid + (((struct MapCell *)cells) -                             \
                       (struct MapCell *)Ram_MapCellBuffer);                   \
      object->motionFlags = 0;                                                 \
      object->collisionFlags = 0;                                              \
      Actor_SetSingleChildFlags(object, 0);                                    \
      level += 4;                                                              \
      newShape = call3(*layer, shape, level);                                  \
      shapes += index;                                                         \
      index <<= 2;                                                             \
      ((struct ShapeCell *)shapes)->shape = newShape;                          \
      cells += index;                                                          \
      ((struct MapCell *)cells)->attributeB |= 192;                            \
      cells += 0x200;                                                          \
      ((struct MapCell *)cells)->attributeB |= 64;                             \
      list++;                                                                  \
    }                                                                          \
    leader = Actor_GetById(gMainRoutineWork.currentOwner);                     \
    height = Map_GetTerrainHeight(leader->terrainId, leader->x, leader->z);    \
    if (leader->y < height) {                                                  \
      leader->terrainHeight = height;                                          \
      leader->y = height;                                                      \
    }                                                                          \
  }

/// Sets the shape of the cell under an actor `level` steps above its ground.
/// 4 copies: dehkan_plateau/s0bc0, magma_rock_inside/s46e8, trial_road/s15a8,
/// tundaria_tower/s29d0.
#define DEFINE_ACTOR_SET_CELL_SHAPE(name, call1, call2)                        \
  void name(s32 level, s32 layer, s32 owner) {                                 \
    struct Actor *object = Actor_GetById(owner);                               \
    u8 *shape =                                                                \
        Ram_MapShapeGrid + (Ram_HeapSlots->mapWork->layers[layer].cells -      \
                            (struct MapCell *)Ram_MapCellBuffer);              \
    s32 value = call1(object->terrainId, object->x, object->z);                \
    s32 height =                                                               \
        Map_GetTerrainHeight(object->terrainId, object->x, object->z) >> 19;   \
                                                                               \
    height += level;                                                           \
                                                                               \
    shape += object->x / 0x100000 + object->z / 0x100000 * 128;                \
    *shape = call2(object->terrainId, value, height);                          \
  }

/// Swaps a map cell's collision code and attribute for `value`, returning the
/// old.
/// 3 copies: magma_rock_inside/magma_rock_inside, mars_lighthouse_entrance,
/// treasure_isle/treasure_isle.
#define DEFINE_MAP_CELL_SWAP(name)                                             \
  s32 name(u32 layer, s32 x, s32 y, s32 value) {                               \
    struct MapWork *map = Ram_HeapSlots->mapWork;                              \
    struct MapCell *cell;                                                      \
    s32 old;                                                                   \
                                                                               \
    if (map == NULL)                                                           \
      return 0;                                                                \
    if (layer <= 2)                                                            \
      cell = map->layers[layer].cells;                                         \
    else                                                                       \
      cell = gMapCellBuffer;                                                   \
    cell += x + (y << 7);                                                      \
    old = (cell->collisionCode << 8) | cell->attributeB;                       \
    cell->collisionCode = value >> 8;                                          \
    cell->attributeB = value;                                                  \
    return old;                                                                \
  }

/// Marks the cells around each block of a list that stands on a raised cell.
/// 2 copies: airs_rock/airs_rock, magma_rock_inside/magma_rock_inside.
#define DEFINE_BLOCKS_MARK_CELLS(name, call1)                                  \
  void name(u16 *ids) {                                                        \
    struct Actor *object;                                                      \
    u8 *blocked;                                                               \
    struct MapWork *map;                                                       \
    struct MapCell **cells;                                                    \
    struct MapCell *cell;                                                      \
    s32 x, z;                                                                  \
                                                                               \
    while (*ids != 0xffff) {                                                   \
      object = Actor_Get(*ids);                                                \
      blocked = (u8 *)object + 0x63;                                           \
      *blocked = 0;                                                            \
      if (call1(0, object->x >> 20, object->z >> 20) & 0x40) {                 \
        map = Ram_HeapSlots->mapWork;                                          \
        if (map == NULL)                                                       \
          return;                                                              \
        cells = &map->layers[2].cells;                                         \
        z = (object->z - object->y) >> 20;                                     \
        x = object->x >> 20;                                                   \
        cell = *cells;                                                         \
        cell += x + ((z + 1) << 7);                                            \
        cell->collisionCode = 255;                                             \
        cell = *cells;                                                         \
        cell += x + ((z - 1) << 7);                                            \
        cell->collisionCode = 255;                                             \
        *blocked = 1;                                                          \
      }                                                                        \
      ids++;                                                                   \
    }                                                                          \
  }

/// Actor_SetCellOpen for each actor of a list ended by 0xffff.
/// 2 copies: jupiter_lighthouse_room_7/s16d4, mars_lighthouse_entrance/s42f8.
#define DEFINE_ACTORS_SET_CELLS_OPEN(name, call1)                              \
  void name(const u16 *list) {                                                 \
    const u16 *spec;                                                           \
    u16 id;                                                                    \
    struct Actor *object;                                                      \
    s16 mode;                                                                  \
    s16 flag;                                                                  \
    s32 limit;                                                                 \
    s32 first;                                                                 \
    limit = 0xff;                                                              \
    first = list[0];                                                           \
    if (first == ((limit << 8) + 0xff)) {                                      \
      return;                                                                  \
    }                                                                          \
    spec = list;                                                               \
    do {                                                                       \
      id = spec[0];                                                            \
      object = Actor_Get(id);                                                  \
      mode = spec[1];                                                          \
      flag = spec[2];                                                          \
      object->collisionFlags &= 0xfe;                                          \
      Actor_SetSingleChildFlags(object, 0);                                    \
      call1(id, (u16)mode, (u16)flag, object->terrainId);                      \
      limit = 0xff;                                                            \
      spec += 3;                                                               \
      first = spec[0];                                                         \
    } while (first != ((limit << 8) + 0xff));                                  \
  }

/// 2 copies: yampi_desert, yampi_desert_cave.
#define DEFINE_STATUE_SINK(name, call1)                                        \
  void name(u16 *list, s32 mode, s32 count) {                                  \
    struct GabombaStatueEffectWork *work = Ram_HeapSlots->effectWork;          \
                                                                               \
    while (*list != 0xffff) {                                                  \
      u32 entry = *list;                                                       \
      s32 id = work->objectId;                                                 \
                                                                               \
      if (id == entry) {                                                       \
        struct Actor *object = Actor_GetById(id);                              \
        s32 a;                                                                 \
        s32 flag;                                                              \
                                                                               \
        list++;                                                                \
        a = list[0];                                                           \
        flag = list[1];                                                        \
        if (count < 8)                                                         \
          object->scaleY -= 0x1f00;                                            \
        if (mode == 1) {                                                       \
          GameFlag_SetBit(flag);                                               \
          call1(id, a, flag, object->terrainId);                               \
          object->scaleY = 0x10000;                                            \
        }                                                                      \
        break;                                                                 \
      }                                                                        \
      list += 3;                                                               \
    }                                                                          \
  }

/// Clears the cells a block marked.
/// 2 copies: magma_rock, magma_rock_inside/magma_rock_inside.
#define DEFINE_BLOCK_CLEAR_CELLS(name)                                         \
  void name(struct Object0200074c *object) {                                   \
    struct MapWork *map;                                                       \
    struct MapCell **cells;                                                    \
    struct MapCell *cell;                                                      \
    s32 x;                                                                     \
    s32 z;                                                                     \
                                                                               \
    if (object->marked == 0)                                                   \
      return;                                                                  \
    map = Ram_HeapSlots->mapWork;                                              \
    if (map == NULL)                                                           \
      return;                                                                  \
    cells = &map->layers[2].cells;                                             \
    cell = *cells;                                                             \
    x = object->x >> 20;                                                       \
    z = (object->z - object->y) >> 20;                                         \
    cell += x + ((z + 1) << 7);                                                \
    cell->collisionCode = 0;                                                   \
    cell = *cells;                                                             \
    cell += x + ((z - 1) << 7);                                                \
    cell->collisionCode = 0;                                                   \
    object->marked = 0;                                                        \
  }

/// Actor_SetCellShape as Izumo's ruins and Magma Rock compile it.
/// 2 copies: izumo_ruins/s0974, magma_rock/s34ac.
#define DEFINE_ACTOR_SET_CELL_SHAPE_AT(name, call1, call2)                     \
  void name(s32 heightOffset, s32 layer, s32 id) {                             \
    struct Actor *actor = Actor_Get(id);                                       \
    s32 cell = Ram_HeapSlots->mapWork->layers[layer].cells -                   \
               (struct MapCell *)Ram_MapCellBuffer;                            \
    s32 z = actor->z;                                                          \
    u8 *shapes = gMapShapeGrid + cell;                                         \
    s32 value;                                                                 \
    s32 height;                                                                \
                                                                               \
    value = call1(actor->terrainId, actor->x, z);                              \
    height = Map_GetTerrainHeight(actor->terrainId, actor->x, actor->z) >> 19; \
    height += heightOffset;                                                    \
    shapes += actor->x / 0x100000 + (actor->z / 0x100000 << 7);                \
    *shapes = call2(actor->terrainId, value, height);                          \
  }

/// A map cell's collision code and attribute.
/// 2 copies: magma_rock_inside/magma_rock_inside, mars_lighthouse_entrance.
#define DEFINE_MAP_CELL_GET(name)                                              \
  s32 name(u32 layer, s32 x, s32 y) {                                          \
    struct MapWork *map = Ram_HeapSlots->mapWork;                              \
    struct MapCell *cell;                                                      \
                                                                               \
    if (map == NULL)                                                           \
      return 0;                                                                \
    if (layer <= 2)                                                            \
      cell = map->layers[layer].cells;                                         \
    else                                                                       \
      cell = gMapCellBuffer;                                                   \
    cell += x + (y << 7);                                                      \
    return (cell->collisionCode << 8) | cell->attributeB;                      \
  }

/// Raises the cells of a list's actors.
/// 2 copies: magma_rock, magma_rock_inside/magma_rock_inside.
#define DEFINE_ACTORS_RAISE_CELLS(name, call1)                                 \
  void name(u16 *list) {                                                       \
    for (; *list != 0xffff; list += 3) {                                       \
      struct Actor *object = Actor_Get(*list);                                 \
      s32 height =                                                             \
          Map_GetTerrainHeight(object->terrainId, object->x, object->z);       \
                                                                               \
      object->motionFlags = 0;                                                 \
      object->collisionFlags = 0;                                              \
      Actor_SetSingleChildFlags(object, 0);                                    \
      call1(object->terrainId, object->x >> 20, object->z >> 20,               \
            (height >> 19) + 4);                                               \
    }                                                                          \
  }

/// Runs a chain of calls on the held actor when there is one.
/// 2 copies: dehkan_plateau/dehkan_plateau, madra_catacombs.
#define DEFINE_SCENE_RUN_IF_HELD(name, call1, call2, call3, data1, call4, call5) \
  void name(void) {                                                            \
    struct Actor *object;                                                      \
                                                                               \
    if (call1() != 0) {                                                        \
      object = Actor_GetById(call2(gMainRoutineWork.currentOwner));            \
      call3(data1);                                                            \
      call4();                                                                 \
      call5(object);                                                           \
    }                                                                          \
  }

/// Sets the shape of a cell to the height given.
/// 2 copies: magma_rock, magma_rock_inside/magma_rock_inside.
#define DEFINE_MAP_CELL_SET_SHAPE(name, call1, call2)                          \
  s32 name(s32 layer, s32 x, s32 z, s32 height) {                              \
    struct MapWork *map = Ram_HeapSlots->mapWork;                              \
    s32 shape = call1(layer, x, z);                                            \
    u8 *out = Ram_MapShapeGrid + (map->layers[layer].cells -                   \
                                  (struct MapCell *)Ram_MapCellBuffer);        \
                                                                               \
    out += x + (z << 7);                                                       \
    *out = call2(layer, shape, height);                                        \
    return 0;                                                                  \
  }

/// Marks a pillar moved: it stops and its cell moves.
/// 4 copies: gondowan_cliffs/s03cc, kibombo_mountains, kibombo_mountains,
/// kibombo_mountains.
#define DEFINE_PILLAR_SET_MOVED(name, n1, n2, n3, n4, n5)                      \
  void name(void) {                                                            \
    struct Actor *object = Actor_Get(n1);                                      \
                                                                               \
    object->priorityFlags = 0;                                                 \
    object->motionFlags = 0;                                                   \
    MapGrid_CopyCellAttributes(n2, n3, 1, 1, n4, n3);                          \
    Event_SetFlag(n5);                                                         \
  }

/// Raises the cell under the held actor.
/// 2 copies: dehkan_plateau/s0bc0, madra_catacombs.
#define DEFINE_HELD_ACTOR_RAISE_CELL(name, call1, call2, call3)                \
  void name(void) {                                                            \
    struct Actor *held = HELD_OBJECT(Ram_HeapSlots->eventWork);                \
    u8 *layer;                                                                 \
    u8 *cells;                                                                 \
    u8 *shapes;                                                                \
    s32 shape;                                                                 \
    s32 level;                                                                 \
    s32 index;                                                                 \
    u8 newShape;                                                               \
                                                                               \
    if (held == NULL)                                                          \
      return;                                                                  \
    layer = &held->terrainId;                                                  \
    cells = (u8 *)Ram_HeapSlots->mapWork->layers[*layer].cells;                \
    shapes = Ram_MapShapeGrid + (((struct MapCell *)cells) -                   \
                                 (struct MapCell *)Ram_MapCellBuffer);         \
    shape = call1(*layer, held->x, held->z);                                   \
    level = Map_GetTerrainHeight(*layer, held->x, held->z);                    \
    index = call2(held);                                                       \
    level >>= 19;                                                              \
    level += 4;                                                                \
    newShape = call3(*layer, shape, level);                                    \
    shapes += index;                                                           \
    index <<= 2;                                                               \
    /* FAKEMATCH(signature): the grid byte stored through a one-field struct   \
     * view (struct ShapeCell), whose alias set lets sched2 sink it past the   \
     * cell read; as *shapes = newShape, 6 of 76 instructions differ in        \
     * tla-en. */                                                              \
    ((struct ShapeCell *)shapes)->shape = newShape;                            \
    cells += index;                                                            \
    ((struct MapCell *)cells)->attributeB |= 192;                              \
    cells += 0x200;                                                            \
    ((struct MapCell *)cells)->attributeB |= 64;                               \
  }

#endif
