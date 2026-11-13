/// @file
/// The cracked floor tiles of Jupiter Lighthouse (and the Mars Lighthouse
/// and Anemos rooms built the same way): the first step cracks one, the next
/// drops the leader through it to the floor below.
/// Each routine is a macro a scene expands where its copy sits, with the
/// names of its own data and calls.
#ifndef ALCHEMY_SCENE_SHARED_CRACKED_TILE_H
#define ALCHEMY_SCENE_SHARED_CRACKED_TILE_H

/// Finds the room's cracked tiles: every cell whose event is `kind` (whole) or
/// `kind + 1` (cracked), each given the next flag from `flag`; returns how
/// many.
#define DEFINE_CRACKED_TILES_FIND(name, data1, call1)                          \
  s32 name(u32 kind, s32 flag, s32 flagByte, s32 x, s32 z) {                   \
    struct CellTarget *table = &data1;                                         \
    struct CellTargetEntry *entry = table->entries;                            \
    struct CellMapWork *map;                                                   \
    struct CellMapLayer *layer;                                                \
    struct CellMapCell *cells;                                                 \
    struct CellMapCell *rows;                                                  \
    struct CellMapCell *cell;                                                  \
    s32 x0, z0, width, height, i, j;                                           \
    u32 *buffer;                                                               \
    u32 event, last;                                                           \
    s32 cx, cz;                                                                \
    s32 k;                                                                     \
                                                                               \
    Iwram_ClearWordsEntry(table, 4);                                           \
    table->kind = kind;                                                        \
    table->firstFlag = flag;                                                   \
    table->flagByte = flagByte;                                                \
    table->x = x;                                                              \
    table->z = z;                                                              \
    table->current = -1;                                                       \
    table->count = 0;                                                          \
    map = (struct CellMapWork *)Ram_HeapSlots->mapWork;                        \
    layer = &map->layers[0];                                                   \
    cells = layer->cells;                                                      \
    x0 = (layer->originX + map->minX) >> 20;                                   \
    z0 = (layer->originZ + map->minZ) >> 20;                                   \
    width = (map->maxX - map->minX) >> 20;                                     \
    height = (map->maxZ - map->minZ) >> 20;                                    \
    cells += ((map->minZ >> 20) << 7) + (map->minX >> 20);                     \
    /* Goto loops: as for loops (either one) loop.c moves the 64 of the cell   \
     */                                                                        \
    /* copy's masks out of them and 321 of 268 instructions differ. */         \
    j = 0;                                                                     \
    rows = cells;                                                              \
    if (j < height) {                                                          \
    row:                                                                       \
      cell = rows + (j << 7);                                                  \
      i = 0;                                                                   \
      if (i < width) {                                                         \
      column:                                                                  \
        event = cell->event;                                                   \
        if (event != 0 && event >= kind) {                                     \
          last = kind + 1;                                                     \
          if (event <= last) {                                                 \
            cx = i + x0;                                                       \
            entry->x = cx;                                                     \
            cz = j + z0;                                                       \
            entry->z = cz;                                                     \
            entry->flag = flag;                                                \
            flag++;                                                            \
            if (!Event_GetFlag(0x109)) {                                       \
              if (event == last)                                               \
                Event_SetFlag(entry->flag);                                    \
            } else if (Event_GetFlag(entry->flag)) {                           \
              MapGrid_CopyCells(table->x, table->z + 1, 1, 1,                  \
                                cx | (s16)(table->x & 64),                     \
                                cz | (s16)(table->z & 64));                    \
            }                                                                  \
            entry++;                                                           \
            table->count++;                                                    \
          }                                                                    \
        }                                                                      \
        i++;                                                                   \
        cell++;                                                                \
        if (i < width)                                                         \
          goto column;                                                         \
      }                                                                        \
      j++;                                                                     \
      if (j < height)                                                          \
        goto row;                                                              \
    }                                                                          \
    if (Event_GetFlag(0x109)) {                                                \
      struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);         \
      s32 leaderX = leader->x >> 20;                                           \
      s32 leaderZ = leader->z >> 20;                                           \
                                                                               \
      entry = table->entries;                                                  \
      k = 0;                                                                   \
      if (k < table->count) {                                                  \
      search:                                                                  \
        if (entry->x == leaderX && entry->z == leaderZ)                        \
          table->current = k;                                                  \
        entry++;                                                               \
        k++;                                                                   \
        if (k < table->count)                                                  \
          goto search;                                                         \
      }                                                                        \
    }                                                                          \
    buffer = Heap_Alloc(0x100);                                                \
    {                                                                          \
      u32 *word = buffer;                                                      \
                                                                               \
      k = 63;                                                                  \
    fill:                                                                      \
      *word++ = 0x11111111;                                                    \
      if (--k >= 0)                                                            \
        goto fill;                                                             \
    }                                                                          \
    table->tile = VramCache_FindFreeEntry();                                   \
    Vram_Load(table->tile, 0x100, buffer);                                     \
    Heap_Release(buffer);                                                      \
    Task_AddFrame(call1, 0x480);                                               \
    return table->count;                                                       \
  }

/// Run each frame: when the leader steps onto a whole tile it cracks,
/// and onto a cracked one other than the one it cracked last, the leader falls
/// through it; 1 when it falls.
#define DEFINE_CRACKED_TILES_STEP(name, data1, call1, call2)                   \
  s32 name(void) {                                                             \
    struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);           \
    s32 x = object->x >> 20;                                                   \
    s32 z = object->z >> 20;                                                   \
    struct CellTarget *work = &data1;                                          \
    struct CellTargetEntry *entry = work->entries;                             \
    s32 i;                                                                     \
                                                                               \
    for (i = 0; i < work->count; i++, entry++) {                               \
      if (entry->x == x && entry->z == z) {                                    \
        if (Event_GetFlag(entry->flag) == 0) {                                 \
          call1(work, entry);                                                  \
          Event_SetFlag(entry->flag);                                          \
          work->current = i;                                                   \
          break;                                                               \
        }                                                                      \
        if (i == work->current)                                                \
          break;                                                               \
        work->current = i;                                                     \
        call2(work, entry);                                                    \
        GameFlag_SetByte(work->flagByte, x);                                   \
        GameFlag_SetByte(work->flagByte + 8, z);                               \
        return 1;                                                              \
      }                                                                        \
    }                                                                          \
    return 0;                                                                  \
  }

/// Stops the leader in the middle of its cell and raises the camera.
#define DEFINE_LEADER_CENTER_IN_CELL(name)                                     \
  void name(void) {                                                            \
    struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);           \
    s32 x = object->x;                                                         \
    s32 z = object->z;                                                         \
                                                                               \
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x40000, 0x20000);           \
                                                                               \
    x >>= 20;                                                                  \
    z >>= 20;                                                                  \
    Actor_MoveToAndWaitById(gMainRoutineWork.currentOwner, (x << 4) + 8,       \
                            (z << 4) + 8);                                     \
    System_Wait(1);                                                            \
    FieldActor_SetPositionAndResetMotion(object, (x << 20) + PIXELS(8),        \
                                         object->y, (z << 20) + PIXELS(8));    \
    FieldEvent_Wait(4);                                                        \
    EventCamera_GetActor()->y -= 0x80000;                                      \
  }

/// On landing below a cracked tile, puts the leader on the cell the flag bytes
/// saved, offset from (x, z), and starts the screen effect.
#define DEFINE_CRACKED_TILE_PLACE_LANDING(name, data1, call1)                  \
  void name(s32 x, s32 z, s32 effect) {                                        \
    struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);           \
    struct JupiterLighthouseRoom8EventWork *work = Ram_HeapSlots->eventWork;   \
    s32 dx = FieldGameFlag_GetByte(data1.flagByte);                            \
    s32 dz = FieldGameFlag_GetByte(data1.flagByte + 8);                        \
                                                                               \
    if (dx != 0 || dz != 0) {                                                  \
      struct Actor *marker;                                                    \
                                                                               \
      GameFlag_SetByte(data1.flagByte, 0);                                     \
      GameFlag_SetByte(data1.flagByte + 8, 0);                                 \
      leader->x = ((dx + x) << 20) + 0x80000;                                  \
      leader->z = ((dz + z) << 20) + 0x80000;                                  \
      marker = work->marker;                                                   \
      marker->x = leader->x;                                                   \
      marker->z = leader->z;                                                   \
      Map_CenterOnFollowedPoint();                                             \
      call1();                                                                 \
      ((struct JupiterLighthouseRoom8EventWork *)Ram_HeapSlots->eventWork)     \
          ->screenEffect = effect;                                             \
    }                                                                          \
  }

/// The leader drops in from above through a cracked tile, lands with a burst of
/// dust and a shake.
#define DEFINE_CRACKED_TILE_DROP_IN(name, call1)                               \
  void name(void) {                                                            \
    struct Actor *actor = Actor_Get(gMainRoutineWork.currentOwner);            \
    u32 i;                                                                     \
    struct EffectOptions options;                                              \
                                                                               \
    Event_Start();                                                             \
    FieldEvent_ResolvePendingActions(0);                                       \
    FieldCamera_MoveTo(-1, -1, -1, 0);                                         \
    Map_CenterOnFollowedPoint();                                               \
    System_Wait(1);                                                            \
    actor->y = 0x820000;                                                       \
    actor->gravity = 0x8000;                                                   \
    actor->bounce = 0;                                                         \
    actor->motionFlags = 0;                                                    \
    ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;      \
    Event_StartScreenTransition();                                             \
    Event_WaitTransitionFrames();                                              \
    Event_PlaySound(SE_DUST_PUFF);                                             \
    actor->motionFlags = 3;                                                    \
    FieldEvent_Wait(24);                                                       \
    options.palette = 7;                                                       \
    options.update = FieldEffect_AdvanceMotion;                                \
    options.startScaleX = 0xa3d7;                                              \
    options.startScaleY = 0xa3d7;                                              \
    i = 0;                                                                     \
    do {                                                                       \
      s32 angle;                                                               \
      s32 velocity[3];                                                         \
                                                                               \
      angle = i << 12;                                                         \
      velocity[0] = Trig_Cos(angle);                                           \
      velocity[1] = 0;                                                         \
      velocity[2] = Math_Sin(angle);                                           \
      velocity[0] += velocity[0] >> 1;                                         \
      velocity[0] += (Random16() * 0xc000 >> 16) - 0x6000;                     \
      velocity[2] += (Random16() * 0x6000 >> 16) - 0x3000;                     \
      call1(actor->x, actor->y, actor->z, velocity[0], velocity[1],            \
            velocity[2], 0x1090001, &options);                                 \
      i++;                                                                     \
    } while (i <= 16);                                                         \
    Event_PlaySound(SE_THUMP);                                                 \
    Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x101);             \
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 49);                     \
    MapGrid_SetShake(0x50000, 0x50000, 0x10000);                               \
    MapGrid_SetShake(-1, -1, 0xe666);                                          \
    FieldMapGrid_WaitForShakeToSettle();                                       \
    Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x100);             \
    actor->gravity = 0x10000;                                                  \
    actor->bounce = 0x4000;                                                    \
    FieldEvent_Wait(10);                                                       \
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);                      \
    Event_End();                                                               \
  }

/// Drops the leader through a cracked tile: it gives way and the leader spins
/// down into it while the screen fades.
#define DEFINE_CRACKED_TILE_FALL(name, id1, call1)                             \
  void name(struct id1 *table, struct CellTargetEntry *entry) {                \
    struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);           \
    s32 x = (leader->x >> 20) | (s16)(table->x & 64);                          \
    s32 z = (leader->z >> 20) | (s16)(table->z & 64);                          \
                                                                               \
    call1();                                                                   \
    Event_PlaySound(SE_SPLASH);                                                \
    MapGrid_CopyCells(table->x, table->z + 2, 1, 1, x, z);                     \
    FieldEvent_Wait(12);                                                       \
    leader->facing = 0x4000;                                                   \
    leader->motionFlags = 0;                                                   \
    Actor_SetSpriteFlags(Actor_Get(gMainRoutineWork.currentOwner), 0);         \
    Io_Write16(0xf1f, &REG_WINOUT);                                            \
    REG_DISPCNT |= 0x8000;                                                     \
    table->shown = 1;                                                          \
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 28);                     \
    FieldEvent_Wait(16);                                                       \
    x = 0;                                                                     \
    do {                                                                       \
      if (x == 5)                                                              \
        Event_PlaySound(SE_DUST_PUFF);                                         \
      leader->scaleX += -0x400;                                                \
      leader->scaleY += -0x300;                                                \
      leader->y += -0x9999;                                                    \
      System_Wait(1);                                                          \
      x++;                                                                     \
    } while (x <= 39);                                                         \
    Actor_Get(gMainRoutineWork.currentOwner)->animationKind = 0;               \
    table->shown = 0;                                                          \
    ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;      \
    Event_FinishScreenTransition();                                            \
    Event_WaitTransitionFrames();                                              \
  }

/// CrackedTiles_Find as the lower floors' copies compile it.
#define DEFINE_CRACKED_TILES_FIND_LOWER(name, data1, call1)                    \
  s32 name(u32 kind, s32 flag, s32 flagByte, s32 x, s32 z) {                   \
    struct CellTarget *table = &data1;                                         \
    struct CellTargetEntry *entry = table->entries;                            \
    struct CellMapWork *map;                                                   \
    struct CellMapLayer *layer;                                                \
    struct CellMapCell *rows;                                                  \
    struct CellMapCell *cell;                                                  \
    s32 x0, z0, width, height, i, j;                                           \
    u32 *buffer;                                                               \
    u32 event, last;                                                           \
    s32 cz, cx;                                                                \
                                                                               \
    Iwram_ClearWordsEntry(table, 4);                                           \
    table->kind = kind;                                                        \
    table->firstFlag = flag;                                                   \
    table->flagByte = flagByte;                                                \
    table->x = x;                                                              \
    table->z = z;                                                              \
    table->current = -1;                                                       \
    table->count = 0;                                                          \
    map = (struct CellMapWork *)Ram_HeapSlots->mapWork;                        \
    layer = &map->layers[0];                                                   \
    rows = layer->cells;                                                       \
    x0 = (layer->originX + map->minX) >> 20;                                   \
    z0 = (layer->originZ + map->minZ) >> 20;                                   \
    width = (map->maxX - map->minX) >> 20;                                     \
    height = (map->maxZ - map->minZ) >> 20;                                    \
    rows += ((map->minZ >> 20) << 7) + (map->minX >> 20);                      \
    /* Goto loops: as for loops (either one) loop.c moves the 64 of the cell   \
     */                                                                        \
    /* copy's masks out of them and 321 of 268 instructions differ. */         \
    j = 0;                                                                     \
    if (j < height) {                                                          \
    row:                                                                       \
      cell = rows + (j << 7);                                                  \
      i = 0;                                                                   \
      if (i < width) {                                                         \
      column:                                                                  \
        event = cell->event;                                                   \
        if (event != 0 && event >= kind) {                                     \
          last = kind + 1;                                                     \
          if (event <= last) {                                                 \
            cx = i + x0;                                                       \
            entry->x = cx;                                                     \
            cz = j + z0;                                                       \
            entry->z = cz;                                                     \
            entry->flag = flag;                                                \
            flag++;                                                            \
            if (!Event_GetFlag(0x109)) {                                       \
              if (event == last)                                               \
                Event_SetFlag(entry->flag);                                    \
            } else if (Event_GetFlag(entry->flag)) {                           \
              s32 sourceX = table->x;                                          \
              s32 sourceZ = table->z;                                          \
                                                                               \
              cx |= (s16)(table->x & 64);                                      \
              cz |= (s16)(table->z & 64);                                      \
              MapGrid_CopyCells(sourceX, sourceZ + 1, 1, 1, cx, cz);           \
            }                                                                  \
            entry++;                                                           \
            table->count++;                                                    \
            /* FAKEMATCH(barrier): an empty asm on the stored count. */        \
            asm("" : "=m"(table->count) : "m"(table->count) : "r3");           \
            /* FAKEMATCH(barrier): an empty asm on the word over the count. */ \
            asm("" : "=m"(*(s32 *)&table->z));                                 \
          }                                                                    \
        }                                                                      \
      columnNext:                                                              \
        /* FAKEMATCH(barrier): an empty asm lengthening the table's life. */   \
        asm("");                                                               \
        /* FAKEMATCH(barrier): an empty asm lengthening the table's life. */   \
        asm("");                                                               \
        /* FAKEMATCH(barrier): an empty asm lengthening the table's life. */   \
        asm("");                                                               \
        i++;                                                                   \
        cell++;                                                                \
        if (i < width)                                                         \
          goto column;                                                         \
      }                                                                        \
    rowNext:                                                                   \
      j++;                                                                     \
      if (j < height)                                                          \
        goto row;                                                              \
    }                                                                          \
    if (Event_GetFlag(0x109)) {                                                \
      struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);         \
      s32 leaderX = leader->x >> 20;                                           \
      s32 leaderZ = leader->z >> 20;                                           \
      s32 k;                                                                   \
                                                                               \
      entry = table->entries;                                                  \
      k = 0;                                                                   \
      if (k < table->count) {                                                  \
      leaderRow:                                                               \
        do {                                                                   \
          if (entry->x == leaderX && entry->z == leaderZ)                      \
            table->current = k;                                                \
        leaderNext:                                                            \
          k++;                                                                 \
          entry++;                                                             \
          /* A cast read: gcse keeps it in the loop. */                        \
        } while (k < *(s16 *)&table->count);                                   \
      }                                                                        \
    }                                                                          \
    buffer = Heap_Alloc(0x100);                                                \
    {                                                                          \
      u32 *word = buffer;                                                      \
      s32 count = 63;                                                          \
                                                                               \
    fill:                                                                      \
      *word++ = 0x11111111;                                                    \
    fillNext:                                                                  \
      if (--count >= 0)                                                        \
        goto fill;                                                             \
    }                                                                          \
    table->tile = VramCache_FindFreeEntry();                                   \
    Vram_Load(table->tile, 0x100, buffer);                                     \
    Heap_Release(buffer);                                                      \
    Task_AddFrame(call1, 0x480);                                               \
    return table->count;                                                       \
  }

/// Cracks the tile under the leader, swapping in its cracked picture.
#define DEFINE_CRACKED_TILE_CRACK(name, call1)                                 \
  void name(struct CellTarget *table, struct CellTargetEntry *entry) {         \
    struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);           \
    s32 x;                                                                     \
    s32 z;                                                                     \
                                                                               \
    x = (leader->x >> 20) | (s16)(table->x & 64);                              \
    z = (leader->z >> 20) | (s16)(table->z & 64);                              \
    call1();                                                                   \
    Event_PlaySound(SE_RATTLE);                                                \
    MapGrid_CopyCells(table->x, table->z + 1, 1, 1, x, z);                     \
    FieldEvent_Wait(12);                                                       \
  }

/// CrackedTile_DropIn as the lower floors' copies compile it.
#define DEFINE_CRACKED_TILE_DROP_IN_LOWER(name, call1)                         \
  void name(void) {                                                            \
    struct Actor *actor = Actor_Get(gMainRoutineWork.currentOwner);            \
    u32 i;                                                                     \
    struct EffectOptions options;                                              \
    s32 spawn = 1;                                                             \
                                                                               \
    Event_Start();                                                             \
    FieldEvent_ResolvePendingActions(0);                                       \
    CamBounds(-1, -1, -1, 0);                                                  \
    Map_CenterOnFollowedPoint();                                               \
    System_Wait(1);                                                            \
    actor->y = 0x820000;                                                       \
    actor->gravity = 0x8000;                                                   \
    actor->bounce = 0;                                                         \
    actor->motionFlags = 0;                                                    \
    ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;      \
    Event_StartScreenTransition();                                             \
    Event_WaitTransitionFrames();                                              \
    Event_PlaySound(SE_DUST_PUFF);                                             \
    actor->motionFlags = 3;                                                    \
    FieldEvent_Wait(24);                                                       \
    options.palette = 7;                                                       \
    options.update = FieldEffect_AdvanceMotion;                                \
    options.startScaleX = 0xa3d7;                                              \
    options.startScaleY = 0xa3d7;                                              \
    i = 0;                                                                     \
    /* A goto loop carries no loop notes: as a do, for or while loop, loop.c   \
     */                                                                        \
    /* moves the velocity array's address out of the loop. */                  \
  loop:                                                                        \
    {                                                                          \
      s32 angle;                                                               \
      s32 velocity[3];                                                         \
      s32 cosine;                                                              \
                                                                               \
      angle = i << 12;                                                         \
      cosine = Trig_Cos(angle);                                                \
      /* FAKEMATCH(dead): spawn is always 1, a constant GCSE only folds after  \
       * its partial redundancy pass. */                                       \
      if (spawn) {                                                             \
        velocity[0] = cosine;                                                  \
        velocity[1] = 0;                                                       \
        velocity[2] = Math_Sin(angle);                                         \
        velocity[0] += velocity[0] >> 1;                                       \
        velocity[0] += (Random16() * 0xc000 >> 16) - 0x6000;                   \
        velocity[2] += (Random16() * 0x6000 >> 16) - 0x3000;                   \
        call1(actor->x, actor->y, actor->z, velocity[0], velocity[1],          \
              velocity[2], 0x1090001, &options);                               \
      }                                                                        \
      i++;                                                                     \
    }                                                                          \
    if (i <= 16)                                                               \
      goto loop;                                                               \
    Event_PlaySound(SE_THUMP);                                                 \
    Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x101);             \
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 49);                     \
    SetWorkTriple(0x50000, 0x50000, 0x10000);                                  \
    SetWorkTriple(-1, -1, 0xe666);                                             \
    FieldMapGrid_WaitForShakeToSettle();                                       \
    Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x100);             \
    actor->gravity = 0x10000;                                                  \
    actor->bounce = 0x4000;                                                    \
    FieldEvent_Wait(10);                                                       \
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);                      \
    Event_End();                                                               \
  }

/// CrackedTile_Fall for a push event's cracked tile.
#define DEFINE_CRACKED_TILE_FALL_EVENT(name, call1)                            \
  void name(struct PushEvent *event) {                                         \
    struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);           \
    s32 x;                                                                     \
    s32 z;                                                                     \
    s32 mask;                                                                  \
                                                                               \
    mask = 64;                                                                 \
    x = object->x >> 20;                                                       \
    do {                                                                       \
    } while (0);                                                               \
    x |= (s16)(event->x & mask);                                               \
    z = object->z >> 20;                                                       \
    z |= (s16)(event->z & mask);                                               \
    call1();                                                                   \
    Event_PlaySound(SE_SPLASH);                                                \
    MapGrid_CopyCells(event->x, event->z + 2, 1, 1, x, z);                     \
    FieldEvent_Wait(12);                                                       \
    {                                                                          \
      u32 zero = 0;                                                            \
                                                                               \
      object->facing = 0x4000;                                                 \
      object->motionFlags = zero;                                              \
    }                                                                          \
    Actor_SetSpriteFlags(Actor_Get(gMainRoutineWork.currentOwner), 0);         \
    {                                                                          \
      /* FAKEMATCH(temporary): a word local holding 0xf1f, built with movs,    \
       * lsls, adds as the native does. */                                     \
      u32 winout = 0xf1f;                                                      \
                                                                               \
      REG_WINOUT = winout;                                                     \
    }                                                                          \
    do {                                                                       \
    } while (0);                                                               \
    REG_DISPCNT |= 0x8000;                                                     \
    event->busy = 1;                                                           \
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 28);                     \
    FieldEvent_Wait(16);                                                       \
    for (x = 0; x < 40; x++) {                                                 \
      if (x == 5)                                                              \
        Event_PlaySound(SE_DUST_PUFF);                                         \
      object->scaleX += -0x400;                                                \
      object->scaleY += -0x300;                                                \
      object->y += -0x9999;                                                    \
      System_Wait(1);                                                          \
    }                                                                          \
    do {                                                                       \
      Actor_Get(gMainRoutineWork.currentOwner)->animationKind = 0;             \
    } while (0);                                                               \
    event->busy = 0;                                                           \
    ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;      \
    Event_FinishScreenTransition();                                            \
    Event_WaitTransitionFrames();                                              \
  }

#endif
