/// @file
/// Routines several scenes' overlays each keep a copy of, written once: a
/// scene expands the macro where its copy sits. The placeholder names are the
/// first copy's.
#ifndef ALCHEMY_SCENE_SHARED_ROUTINES_H
#define ALCHEMY_SCENE_SHARED_ROUTINES_H

/// 12 scenes: anemos_inner_sanctum/move, jupiter_lighthouse_aerie/s1e78,
/// jupiter_lighthouse_aerie_beacon/s4ec8, jupiter_lighthouse_room_4/s1258,
/// jupiter_lighthouse_room_5/s1c88, jupiter_lighthouse_room_6/s0f5c,
/// jupiter_lighthouse_room_7/s1f74, jupiter_lighthouse_room_8/s1600,
/// jupiter_lighthouse_room_9/s1018, jupiter_lighthouse_room_m/s3b7c,
/// mars_lighthouse_entrance/s3cf8, mars_lighthouse_star_room/s3804.
#define DEFINE_FUNC_02001250(name, data1, call1)                               \
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
    Iwram_ClearWords(table, 4);                                                \
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
    table->tile = Resource_FindFreeEntry();                                    \
    Vram_Load(table->tile, 0x100, buffer);                                     \
    Heap_Release(buffer);                                                      \
    Task_AddFrame(call1, 0x480);                                               \
    return table->count;                                                       \
  }

/// 14 scenes: jupiter_lighthouse_aerie/s17a8,
/// jupiter_lighthouse_aerie_beacon/s44cc, jupiter_lighthouse_entrance/dist,
/// jupiter_lighthouse_room_1/s0674, jupiter_lighthouse_room_2/s0d94,
/// jupiter_lighthouse_room_3/s1300, jupiter_lighthouse_room_4/s0b50,
/// jupiter_lighthouse_room_5/s1650, jupiter_lighthouse_room_6/s0854,
/// jupiter_lighthouse_room_7/s16d4, jupiter_lighthouse_room_8/s1178,
/// jupiter_lighthouse_room_9/s0910, jupiter_lighthouse_room_m/s3474,
/// mars_lighthouse_room_f/s3d80.
#define DEFINE_FUNC_02000AAC(name, data1, data2, data3, data4, data5, data6,   \
                             call1, call2)                                     \
  void name(void) {                                                            \
    struct MapWork *map = Ram_HeapSlots->mapWork;                              \
    s32 *view = &map->viewX;                                                   \
    s32 viewX, viewZ, tile, viewY;                                             \
    struct LightOam *oam;                                                      \
    s32 i;                                                                     \
    viewX = view[0] & 0xffff0000;                                              \
    viewZ = view[1] & 0xffff0000;                                              \
    tile = gResourceTableEntries[data1].offset >> 5;                           \
    viewY = map->origin[1];                                                    \
    oam = data2;                                                               \
                                                                               \
    for (i = 0; i < data3; i++) {                                              \
      struct Actor *actor = data4[i];                                          \
      if (actor != 0 && actor->x != 0) {                                       \
        s32 big, ox, y, z, oy, depth;                                          \
        s32 frame;                                                             \
        u32 *word;                                                             \
        ox = actor->x - viewX;                                                 \
        y = actor->y - viewY + 0x80000;                                        \
        z = actor->z - viewZ - viewY;                                          \
        oy = z - y;                                                            \
        depth = ((y + z) >> 16) + 58;                                          \
        big = data5;                                                           \
        if (big == 0) {                                                        \
          s32 x, sy;                                                           \
          frame = actor->timer;                                                \
          x = ox >> 16;                                                        \
          sy = oy >> 16;                                                       \
          ox = x - 8;                                                          \
          oy = sy - 16;                                                        \
          if ((u32)(x + 7) > 334 || oy <= -16 || oy > 239)                     \
            continue;                                                          \
          ox &= 0x1ff;                                                         \
          oy &= 0xff;                                                          \
          word = (u32 *)oam;                                                   \
          *word++ = 0;                                                         \
          *word++ = oy | (ox << 16) | 0x40002000;                              \
          *word = (tile + frame * 8) | 0x800;                                  \
          if (data6 == -1)                                                     \
            oam->priority = call1(actor);                                      \
          else                                                                 \
            oam->priority = data6;                                             \
          call2(oam, depth);                                                   \
          oam++;                                                               \
        } else {                                                               \
          s32 x, sy;                                                           \
          frame = actor->timer;                                                \
          x = ox >> 16;                                                        \
          sy = oy >> 16;                                                       \
          ox = x - 8;                                                          \
          oy = sy - 64;                                                        \
          if ((u32)(x + 23) > 350 || oy <= -64 || oy > 175)                    \
            continue;                                                          \
          ox &= 0x1ff;                                                         \
          oy &= 0xff;                                                          \
          word = (u32 *)oam;                                                   \
          *word++ = 0;                                                         \
          *word++ = oy | (ox << 16) | 0xc000a000;                              \
          *word = (tile + frame * 8) | 0x800;                                  \
          if (data6 == -1)                                                     \
            oam->priority = call1(actor);                                      \
          else                                                                 \
            oam->priority = data6;                                             \
          call2(oam, depth);                                                   \
          oam++;                                                               \
        }                                                                      \
      }                                                                        \
    }                                                                          \
  }

/// 12 scenes: jupiter_lighthouse_aerie, jupiter_lighthouse_basement/s098c,
/// jupiter_lighthouse_entrance/dist, jupiter_lighthouse_room_1/s0674,
/// jupiter_lighthouse_room_2/s0d94, jupiter_lighthouse_room_3/s07b0,
/// jupiter_lighthouse_room_5, jupiter_lighthouse_room_6/s0854,
/// jupiter_lighthouse_room_7/s16d4, jupiter_lighthouse_room_8/s07e8,
/// jupiter_lighthouse_room_9/s0910, jupiter_lighthouse_room_m/s3474.
#define DEFINE_FUNC_0200091C(name)                                             \
  void name(void) {                                                            \
    struct PositionedActor *actor9 = ((struct PositionedActor *)Actor_Get(9)); \
    struct PositionedActor *actor10 =                                          \
        ((struct PositionedActor *)Actor_Get(10));                             \
    struct PositionedActor *actor23 =                                          \
        ((struct PositionedActor *)Actor_Get(23));                             \
    struct FieldSprite *sprite = actor23->animation;                           \
                                                                               \
    Actor_SetPosition(24, 0x2000000, 0xf80000);                                \
    Actor_SetPosition(23, 0x2000000, 0xf80000);                                \
    Actor_SetPosition(9, 0x1d80000, 0x1000000);                                \
    Actor_SetPosition(10, 0x2280000, 0x1000000);                               \
    ((struct PositionedActor *)Actor_Get(24))->scaleX = 0;                     \
    actor23->motionFlags = 0;                                                  \
    actor23->y.fixed = actor23->terrainHeight;                                 \
    actor9->motionFlags = 0;                                                   \
    actor9->y.fixed = actor9->terrainHeight;                                   \
    actor10->motionFlags = 0;                                                  \
    actor10->y.fixed = actor10->terrainHeight;                                 \
    if (Event_GetFlag(0xa0a)) {                                                \
      actor9->y.fixed += 0x66640;                                              \
      {                                                                        \
        s32 step = 0x1eb80;                                                    \
                                                                               \
        actor23->y.fixed += step;                                              \
      }                                                                        \
      actor23->x.fixed -= 0x28f40;                                             \
      sprite->rotation += 0x800;                                               \
      Animation_ApplyChildValues(((struct PositionedActor *)Actor_Get(9)), 4); \
      Animation_ApplyChildValues(((struct PositionedActor *)Actor_Get(11)),    \
                                 4);                                           \
    }                                                                          \
    if (Event_GetFlag(0xa0b)) {                                                \
      actor10->y.fixed += 0x66640;                                             \
      {                                                                        \
        s32 step = 0x1eb80;                                                    \
                                                                               \
        actor23->y.fixed += step;                                              \
      }                                                                        \
      actor23->x.fixed += 0x28f40;                                             \
      sprite->rotation -= 0x800;                                               \
      Animation_ApplyChildValues(((struct PositionedActor *)Actor_Get(10)),    \
                                 4);                                           \
      Animation_ApplyChildValues(((struct PositionedActor *)Actor_Get(12)),    \
                                 4);                                           \
    }                                                                          \
    if (Event_GetFlag(0xa0a) && Event_GetFlag(0xa0b)) {                        \
      actor9->y.fixed += 0x199900;                                             \
      actor10->y.fixed += 0x199900;                                            \
      {                                                                        \
        s32 step = 0x1b8480;                                                   \
                                                                               \
        actor23->y.fixed += step;                                              \
      }                                                                        \
      actor9->motionFlags = 4;                                                 \
      actor10->motionFlags = 4;                                                \
      actor23->motionFlags = 4;                                                \
    }                                                                          \
  }

/// 16 scenes: anemos_inner_sanctum/s2f48, jupiter_lighthouse_aerie/s2640,
/// jupiter_lighthouse_aerie_beacon/s539c, jupiter_lighthouse_basement/s185c,
/// jupiter_lighthouse_entrance/s1674, jupiter_lighthouse_room_1/s1704,
/// jupiter_lighthouse_room_2/s1e24, jupiter_lighthouse_room_3/s1f50,
/// jupiter_lighthouse_room_5/s2450, jupiter_lighthouse_room_6/s1724,
/// jupiter_lighthouse_room_7/s273c, jupiter_lighthouse_room_8/s1dc8,
/// jupiter_lighthouse_room_9/s17e0, jupiter_lighthouse_room_m/s4344,
/// mars_lighthouse_entrance/s42f8, mars_lighthouse_star_room/s3e04.
#define DEFINE_FUNC_0200180C(name, data1, call1, call2)                        \
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

/// 5 scenes: jupiter_lighthouse_room_4, jupiter_lighthouse_room_5,
/// jupiter_lighthouse_room_6, jupiter_lighthouse_room_8,
/// jupiter_lighthouse_room_9.
#define DEFINE_FUNC_02000410(name)                                             \
  void name(void) {                                                            \
    struct EventWork *work = Ram_HeapSlots->eventWork;                         \
    struct Actor *actor = Actor_Get(gMainRoutineWork.currentOwner);            \
    struct Actor *linked = actor->watchedObject;                               \
    struct Actor *shadow;                                                      \
    u8 *p;                                                                     \
    s32 angle;                                                                 \
    s32 i;                                                                     \
                                                                               \
    Event_Start();                                                             \
    FieldEvent_ResolvePendingActions(0);                                       \
    i = 0;                                                                     \
    work->unkd46 = i;                                                          \
                                                                               \
    actor->motionFlags = 4;                                                    \
    Actor_SetSingleChildFlags(actor, 0);                                       \
                                                                               \
    while (*((u8 *)linked + 0x63) != 0) {                                      \
      actor->x = linked->x;                                                    \
      actor->y = linked->y + i;                                                \
      actor->z = linked->z;                                                    \
      if (i <= 0x3ffff)                                                        \
        i += 0x1999;                                                           \
      actor->facing = (u8)gFrameCount << 12;                                   \
      p = &actor->priorityFlags;                                               \
      *p ^= 2 << (gFrameCount & 1);                                            \
      System_Wait(1);                                                          \
    }                                                                          \
    work->unkd44 = 1;                                                          \
    work->unkd46 = 1;                                                          \
    angle = linked->facing;                                                    \
    actor->x = (((linked->x >> 19) & ~1) + (angle >> 13) - 1) << 19;           \
    actor->z = (((linked->z >> 19) & ~1) - ((angle >> 13) & 2) + 1) << 19;     \
    actor->velocityY = 0x60000;                                                \
    actor->priorityFlags = 1;                                                  \
    actor->motionFlags = 3;                                                    \
    Actor_SetSingleChildFlags(actor, 1);                                       \
    FieldCamera_MoveTo(-1, -1, -1, 0);                                         \
    shadow = FieldCamera_GetActor();                                           \
    shadow->speedLimit = 0x80000;                                              \
    shadow->acceleration = 0x10000;                                            \
    shadow->motionFlags = 0;                                                   \
    FieldActor_SetMoveTarget(shadow, actor->x,                                 \
                             Map_GetTerrainHeight(0, actor->x, actor->z),      \
                             actor->z);                                        \
    i = 0;                                                                     \
    if (actor->y > actor->terrainHeight + 0x100000) {                          \
      do {                                                                     \
        System_Wait(1);                                                        \
        i++;                                                                   \
        if (i > 59)                                                            \
          break;                                                               \
      } while (actor->y > actor->terrainHeight + 0x100000);                    \
    }                                                                          \
    Event_PlaySound(127);                                                      \
    i = 0;                                                                     \
    if (actor->velocityY != 0) {                                               \
      do {                                                                     \
        System_Wait(1);                                                        \
        i++;                                                                   \
        if (i > 59)                                                            \
          break;                                                               \
      } while (actor->velocityY != 0);                                         \
    }                                                                          \
    Actor_WaitForMove(shadow);                                                 \
    Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 1;              \
    FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);                 \
    work->unkd46 = 1;                                                          \
    work->exit = 0;                                                            \
    Event_End();                                                               \
  }

/// 6 scenes: jupiter_lighthouse_aerie/s2640, jupiter_lighthouse_room_2/s1e24,
/// jupiter_lighthouse_room_3/s1f50, jupiter_lighthouse_room_m/s4344,
/// mars_lighthouse_room_f/s3d80, mars_lighthouse_star_room/s3e04.
#define DEFINE_FUNC_02001E24(name, call1)                                      \
  void name(void) {                                                            \
    struct Actor *actor = Actor_Get(gMainRoutineWork.currentOwner);            \
    u32 i;                                                                     \
    struct EffectOptions options;                                              \
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
    *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x100;                  \
    Event_StartScreenTransition();                                             \
    Event_WaitTransitionFrames();                                              \
    Event_PlaySound(204);                                                      \
    actor->motionFlags = 3;                                                    \
    FieldEvent_Wait(24);                                                       \
    options.palette = 7;                                                       \
    options.update = Effect_AdvanceMotion;                                     \
    options.startScaleX = 0xa3d7;                                              \
    options.startScaleY = 0xa3d7;                                              \
    i = 0;                                                                     \
    do {                                                                       \
      s32 angle;                                                               \
      s32 velocity[3];                                                         \
                                                                               \
      angle = i << 12;                                                         \
      velocity[0] = Math_Cosine(angle);                                        \
      velocity[1] = 0;                                                         \
      velocity[2] = Math_Sin(angle);                                           \
      velocity[0] += velocity[0] >> 1;                                         \
      velocity[0] += (Random16Far() * 0xc000 >> 16) - 0x6000;                  \
      velocity[2] += (Random16Far() * 0x6000 >> 16) - 0x3000;                  \
      call1(actor->x, actor->y, actor->z, velocity[0], velocity[1],            \
            velocity[2], 0x1090001, &options);                                 \
      i++;                                                                     \
    } while (i <= 16);                                                         \
    Event_PlaySound(188);                                                      \
    Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x101);             \
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 49);                     \
    SetWorkTriple(0x50000, 0x50000, 0x10000);                                  \
    SetWorkTriple(-1, -1, 0xe666);                                             \
    MapGrid_WaitForShakeToSettle();                                            \
    Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x100);             \
    actor->gravity = 0x10000;                                                  \
    actor->bounce = 0x4000;                                                    \
    FieldEvent_Wait(10);                                                       \
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);                      \
    Event_End();                                                               \
  }

/// 15 scenes: jupiter_lighthouse_aerie/s1e78,
/// jupiter_lighthouse_aerie_beacon/s4ec8, jupiter_lighthouse_entrance/s0eac,
/// jupiter_lighthouse_room_1/s0f3c, jupiter_lighthouse_room_2/s165c,
/// jupiter_lighthouse_room_3/s1a7c, jupiter_lighthouse_room_5/s1c88,
/// jupiter_lighthouse_room_6/s0f5c, jupiter_lighthouse_room_7/s1f74,
/// jupiter_lighthouse_room_8/s1600, jupiter_lighthouse_room_9/s1018,
/// jupiter_lighthouse_room_m/s3b7c, mars_lighthouse_entrance/s3cf8,
/// mars_lighthouse_room_f/s35b8, mars_lighthouse_star_room/s3804.
#define DEFINE_FUNC_020013E4(name)                                             \
  void name(void) {                                                            \
    struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);           \
    s32 x = object->x;                                                         \
    s32 z = object->z;                                                         \
                                                                               \
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x40000, 0x20000);           \
                                                                               \
    x >>= 20;                                                                  \
    z >>= 20;                                                                  \
    Actor_SetPositionAndCommit(gMainRoutineWork.currentOwner, (x << 4) + 8,    \
                               (z << 4) + 8);                                  \
    System_Wait(1);                                                            \
    FieldActor_SetPositionAndResetMotion(object, (x << 20) + PIXELS(8),        \
                                         object->y, (z << 20) + PIXELS(8));    \
    FieldEvent_Wait(4);                                                        \
    FieldCamera_GetActor()->y -= 0x80000;                                      \
  }

/// 5 scenes: jupiter_lighthouse_basement, jupiter_lighthouse_entrance,
/// jupiter_lighthouse_room_1, jupiter_lighthouse_room_2,
/// jupiter_lighthouse_room_3.
#define DEFINE_FUNC_020003CC(name)                                             \
  void name(void) {                                                            \
    struct EventWork *work = Ram_HeapSlots->eventWork;                         \
    struct Actor *actor = Actor_Get(gMainRoutineWork.currentOwner);            \
    struct Actor *linked = actor->watchedObject;                               \
    struct Actor *shadow;                                                      \
    s32 facing;                                                                \
    s32 i;                                                                     \
                                                                               \
    Event_Start();                                                             \
    FieldEvent_ResolvePendingActions(0);                                       \
    i = 0;                                                                     \
    work->unkd46 = 0;                                                          \
    actor->motionFlags = 4;                                                    \
    Actor_SetSingleChildFlags(actor, 0);                                       \
    while (*((u8 *)linked + 0x63) != 0) {                                      \
      actor->x = linked->x;                                                    \
      actor->y = linked->y + i;                                                \
      actor->z = linked->z;                                                    \
      if (i <= 0x3ffff)                                                        \
        i += 0x1999;                                                           \
      actor->facing = (u8)gFrameCount << 12;                                   \
      actor->priorityFlags ^= 2 << (gFrameCount & 1);                          \
      System_Wait(1);                                                          \
    }                                                                          \
    work->unkd44 = 1;                                                          \
    work->unkd46 = 1;                                                          \
    facing = linked->facing;                                                   \
    actor->x = (((linked->x >> 19) & ~1) + (facing >> 13) - 1) << 19;          \
    actor->z = (((linked->z >> 19) & ~1) - ((facing >> 13) & 2) + 1) << 19;    \
    actor->velocityY = 0x60000;                                                \
    actor->priorityFlags = 1;                                                  \
    actor->motionFlags = 3;                                                    \
    Actor_SetSingleChildFlags(actor, 1);                                       \
    FieldCamera_MoveTo(-1, -1, -1, 0);                                         \
    shadow = FieldCamera_GetActor();                                           \
    shadow->speedLimit = 0x80000;                                              \
    shadow->acceleration = 0x10000;                                            \
    shadow->motionFlags = 0;                                                   \
    FieldActor_SetMoveTarget(shadow, actor->x,                                 \
                             Map_GetTerrainHeight(0, actor->x, actor->z),      \
                             actor->z);                                        \
    i = 0;                                                                     \
    while (i <= 59 && actor->y > actor->terrainHeight + 0x100000) {            \
      i++;                                                                     \
      System_Wait(1);                                                          \
    }                                                                          \
    Event_PlaySound(127);                                                      \
    i = 0;                                                                     \
    while (i <= 59 && actor->velocityY != 0) {                                 \
      i++;                                                                     \
      System_Wait(1);                                                          \
    }                                                                          \
    Actor_WaitForMove(shadow);                                                 \
    Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 1;              \
    FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);                 \
    work->unkd46 = 1;                                                          \
    work->exit = 0;                                                            \
    Event_End();                                                               \
  }

/// 9 scenes: jupiter_lighthouse_aerie, jupiter_lighthouse_basement/s098c,
/// jupiter_lighthouse_room_1/s0674, jupiter_lighthouse_room_2/s0d94,
/// jupiter_lighthouse_room_3/s07b0, jupiter_lighthouse_room_4/s0b50,
/// jupiter_lighthouse_room_6/s0854, jupiter_lighthouse_room_7/s16d4,
/// jupiter_lighthouse_room_8/s07e8.
#define DEFINE_FUNC_020007E8(name, call1, call2, call3)                        \
  struct GustObject *name(struct GustObject *source) {                         \
    struct Vector3 position;                                                   \
    struct EffectOptions options;                                              \
    struct GustObject *object;                                                 \
                                                                               \
    Event_PlaySound(222);                                                      \
    position.x = source->x + Value2((s32 (*)())Iwram_MulQ16, 0x80000,          \
                                    Math_Cosine(source->angle));               \
    position.z = source->z + Value2((s32 (*)())Iwram_MulQ16, 0x80000,          \
                                    Math_Sin(source->angle));                  \
    object = ((struct GustObject *)FieldActor_Create(280, position.x,          \
                                                     source->y, position.z));  \
    call1(object, 2);                                                          \
    Actor_SetSingleChildFlags(object, 0);                                      \
    object->motionFlags = 0;                                                   \
    object->priorityFlags &= ~1;                                               \
    object->angle = source->angle;                                             \
    object->unk64 = 0;                                                         \
    object->unk62 = 0;                                                         \
    object->unk63 = 0;                                                         \
    object->update = call2;                                                    \
    object->unk30 = 0x18000;                                                   \
    options.priority = 1;                                                      \
    options.palette = 7;                                                       \
    call3(position.x, source->y, position.z - 0x60000, 0, 0, 0, 0x30000,       \
          &options);                                                           \
    return object;                                                             \
  }

/// 11 scenes: anemos_inner_sanctum/s2f48,
/// jupiter_lighthouse_aerie_beacon/s539c, jupiter_lighthouse_basement/s185c,
/// jupiter_lighthouse_entrance/s1674, jupiter_lighthouse_room_1/s1704,
/// jupiter_lighthouse_room_2/s1e24, jupiter_lighthouse_room_3/s1f50,
/// jupiter_lighthouse_room_7/s273c, jupiter_lighthouse_room_m/s4344,
/// mars_lighthouse_entrance/s42f8, mars_lighthouse_star_room/s3e04.
#define DEFINE_FUNC_020018BC(name, data1, call1)                               \
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

/// 5 scenes: jupiter_lighthouse_room_4/s1a20, jupiter_lighthouse_room_5/s2450,
/// jupiter_lighthouse_room_6/s1724, jupiter_lighthouse_room_8/s1dc8,
/// jupiter_lighthouse_room_9/s17e0.
#define DEFINE_FUNC_02001724(name, call1)                                      \
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
    *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x100;                  \
    Event_StartScreenTransition();                                             \
    Event_WaitTransitionFrames();                                              \
    Event_PlaySound(204);                                                      \
    actor->motionFlags = 3;                                                    \
    FieldEvent_Wait(24);                                                       \
    options.palette = 7;                                                       \
    options.update = Effect_AdvanceMotion;                                     \
    options.startScaleX = 0xa3d7;                                              \
    options.startScaleY = 0xa3d7;                                              \
    i = 0;                                                                     \
    do {                                                                       \
      s32 angle;                                                               \
      s32 velocity[3];                                                         \
                                                                               \
      angle = i << 12;                                                         \
      velocity[0] = Math_Cosine(angle);                                        \
      velocity[1] = 0;                                                         \
      velocity[2] = Math_Sin(angle);                                           \
      velocity[0] += velocity[0] >> 1;                                         \
      velocity[0] += (Random16Far() * 0xc000 >> 16) - 0x6000;                  \
      velocity[2] += (Random16Far() * 0x6000 >> 16) - 0x3000;                  \
      call1(actor->x, actor->y, actor->z, velocity[0], velocity[1],            \
            velocity[2], 0x1090001, &options);                                 \
      i++;                                                                     \
    } while (i <= 16);                                                         \
    Event_PlaySound(188);                                                      \
    Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x101);             \
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 49);                     \
    MapGrid_SetShake(0x50000, 0x50000, 0x10000);                               \
    MapGrid_SetShake(-1, -1, 0xe666);                                          \
    MapGrid_WaitForShakeToSettle();                                            \
    Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x100);             \
    actor->gravity = 0x10000;                                                  \
    actor->bounce = 0x4000;                                                    \
    FieldEvent_Wait(10);                                                       \
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);                      \
    Event_End();                                                               \
  }

/// 7 scenes: jupiter_lighthouse_basement/s16c4,
/// jupiter_lighthouse_room_1/s156c, jupiter_lighthouse_room_2/s1c8c,
/// jupiter_lighthouse_room_4/s1888, jupiter_lighthouse_room_5/s22b8,
/// jupiter_lighthouse_room_6/s158c, jupiter_lighthouse_room_7/s25a4.
#define DEFINE_FUNC_0200156C(name, id1, call1)                                 \
  void name(struct id1 *table, struct CellTargetEntry *entry) {                \
    struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);           \
    s32 x = (leader->x >> 20) | (s16)(table->x & 64);                          \
    s32 z = (leader->z >> 20) | (s16)(table->z & 64);                          \
                                                                               \
    call1();                                                                   \
    Event_PlaySound(229);                                                      \
    MapGrid_CopyCells(table->x, table->z + 2, 1, 1, x, z);                     \
    FieldEvent_Wait(12);                                                       \
    leader->facing = 0x4000;                                                   \
    leader->motionFlags = 0;                                                   \
    Actor_SetSingleChildFlags(Actor_Get(gMainRoutineWork.currentOwner), 0);    \
    Io_Write16(0xf1f, &REG_WINOUT);                                            \
    REG_DISPCNT |= 0x8000;                                                     \
    table->shown = 1;                                                          \
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 28);                     \
    FieldEvent_Wait(16);                                                       \
    x = 0;                                                                     \
    do {                                                                       \
      if (x == 5)                                                              \
        Event_PlaySound(204);                                                  \
      leader->scaleX += -0x400;                                                \
      leader->scaleY += -0x300;                                                \
      leader->y += -0x9999;                                                    \
      System_Wait(1);                                                          \
      x++;                                                                     \
    } while (x <= 39);                                                         \
    Actor_Get(gMainRoutineWork.currentOwner)->animationKind = 0;               \
    table->shown = 0;                                                          \
    *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x209;                  \
    Event_FinishScreenTransition();                                            \
    Event_WaitTransitionFrames();                                              \
  }

/// 16 scenes: anemos_inner_sanctum_entrance, jupiter_lighthouse_aerie/s17a8,
/// jupiter_lighthouse_aerie_beacon/s44cc, jupiter_lighthouse_basement/s098c,
/// jupiter_lighthouse_entrance/dist, jupiter_lighthouse_room_1/s0674,
/// jupiter_lighthouse_room_2/s0d94, jupiter_lighthouse_room_3/s1300,
/// jupiter_lighthouse_room_4/s0b50, jupiter_lighthouse_room_5/s1650,
/// jupiter_lighthouse_room_6/s0854, jupiter_lighthouse_room_7/s16d4,
/// jupiter_lighthouse_room_8/s1178, jupiter_lighthouse_room_9/s0910,
/// jupiter_lighthouse_room_m/s3474, mars_lighthouse_room_f/s3d80.
#define DEFINE_FUNC_02000A98(name, data1, data2, call1, data3, data4, data5)   \
  void name(void) {                                                            \
    u8 *buffer = BumpAlloc(0xc00);                                             \
                                                                               \
    Iwram_ClearWords(data1, 64);                                               \
    Resource_DecodeType01(gField_CachedTilesA, buffer);                        \
    data2 = Resource_FindFreeEntry();                                          \
    VramCache_Load(data2, 0xc00, buffer);                                      \
    Heap_Release(buffer);                                                      \
    Task_Add(call1, TASK_PASS_VBLANK);                                         \
    data3 = 0;                                                                 \
    data4 = 0;                                                                 \
    data5 = -1;                                                                \
  }

/// 14 scenes: jupiter_lighthouse_aerie, jupiter_lighthouse_aerie_beacon/s0198,
/// jupiter_lighthouse_basement, jupiter_lighthouse_entrance,
/// jupiter_lighthouse_room_1, jupiter_lighthouse_room_2,
/// jupiter_lighthouse_room_3, jupiter_lighthouse_room_4,
/// jupiter_lighthouse_room_5, jupiter_lighthouse_room_7,
/// jupiter_lighthouse_room_8, jupiter_lighthouse_room_9,
/// jupiter_lighthouse_room_m, mars_lighthouse_room_f.
#define DEFINE_FUNC_02000378(name, call1, call2)                               \
  void name(s32 id) {                                                          \
    struct Actor *object;                                                      \
    s32 height;                                                                \
                                                                               \
    object = Actor_Get(id);                                                    \
    object->motionFlags = 4;                                                   \
    height = object->terrainHeight;                                            \
    WORD_AT(object, 68) = 0;                                                   \
    object->y = height + 0x200000;                                             \
    object->priorityFlags |= 128;                                              \
    height = Map_GetTerrainHeight(BYTE_AT(object, 34), object->x, object->z);  \
    call1(object->x, object->z, 0, (height >> 19) + 6);                        \
    call2(object->x, object->z, 0, 128);                                       \
  }

/// 15 scenes: jupiter_lighthouse_aerie/s17a8,
/// jupiter_lighthouse_aerie_beacon/s44cc, jupiter_lighthouse_basement/s098c,
/// jupiter_lighthouse_entrance/dist, jupiter_lighthouse_room_1/s0674,
/// jupiter_lighthouse_room_2/s0d94, jupiter_lighthouse_room_3/s1300,
/// jupiter_lighthouse_room_4/s0b50, jupiter_lighthouse_room_5/s1650,
/// jupiter_lighthouse_room_6/s0854, jupiter_lighthouse_room_7/s16d4,
/// jupiter_lighthouse_room_8/s1178, jupiter_lighthouse_room_9/s0910,
/// jupiter_lighthouse_room_m/s3474, mars_lighthouse_room_f/s3d80.
#define DEFINE_FUNC_02000CE8(name, data1, data2, call1, data3, data4, data5)   \
  void name(void) {                                                            \
    u8 *buffer = Heap_Alloc(0xc00);                                            \
                                                                               \
    Iwram_ClearWords(data1, 64);                                               \
    Resource_DecodeType01(gField_CachedTilesB, buffer);                        \
    data2 = Resource_FindFreeEntry();                                          \
    Vram_Load(data2, 0x800, buffer);                                           \
    Heap_Release(buffer);                                                      \
    Task_AddFrame(call1, TASK_PASS_VBLANK);                                    \
    data3 = 0;                                                                 \
    data4 = 0;                                                                 \
    data5 = -1;                                                                \
  }

/// 15 scenes: jupiter_lighthouse_aerie/s17a8,
/// jupiter_lighthouse_aerie_beacon/s44cc, jupiter_lighthouse_basement/s098c,
/// jupiter_lighthouse_entrance/dist, jupiter_lighthouse_room_1/s0674,
/// jupiter_lighthouse_room_2/s0d94, jupiter_lighthouse_room_3/s1300,
/// jupiter_lighthouse_room_4/s0b50, jupiter_lighthouse_room_5/s1650,
/// jupiter_lighthouse_room_6/s0854, jupiter_lighthouse_room_7/s16d4,
/// jupiter_lighthouse_room_8/s1178, jupiter_lighthouse_room_9/s0910,
/// jupiter_lighthouse_room_m/s3474, mars_lighthouse_room_f/s3d80.
#define DEFINE_FUNC_02000D68(name, data1, data2, call1, data3, data4, data5)   \
  void name(void) {                                                            \
    u8 *buffer = BumpAlloc(0x800);                                             \
                                                                               \
    Iwram_ClearWords(data1, 64);                                               \
    Resource_DecodeType01(gField_CachedTilesC, buffer);                        \
    data2 = Resource_FindFreeEntry();                                          \
    Vram_Load(data2, 0x800, buffer);                                           \
    Heap_Release(buffer);                                                      \
    Task_AddFrame(call1, TASK_PASS_VBLANK);                                    \
    data3 = 0;                                                                 \
    data4 = 1;                                                                 \
    data5 = -1;                                                                \
  }

/// 12 scenes: jupiter_lighthouse_aerie_beacon/s0198,
/// jupiter_lighthouse_basement, jupiter_lighthouse_entrance,
/// jupiter_lighthouse_room_1, jupiter_lighthouse_room_2,
/// jupiter_lighthouse_room_4, jupiter_lighthouse_room_5,
/// jupiter_lighthouse_room_6, jupiter_lighthouse_room_7,
/// jupiter_lighthouse_room_8, jupiter_lighthouse_room_9,
/// jupiter_lighthouse_room_m.
#define DEFINE_FUNC_020002F4(name, data1, data2, data3, call1, data4)          \
  void name(s32 mode) {                                                        \
    data1 = 1;                                                                 \
    if (mode != 2) {                                                           \
      Iwram_CopyWords(data2, (void *)&BG_PLTT_COLOR(12, 0), 32);               \
      Iwram_CopyWords(data3, (void *)&BG_PLTT_COLOR(13, 0), 32);               \
    }                                                                          \
    if (Event_GetFlag(0xa00) || mode == 1) {                                   \
      Counter_Set(0);                                                          \
      Task_Add(call1, 1152);                                                   \
    } else {                                                                   \
      Dma_Set(data4, (void *)&BG_PLTT_COLOR(12, 2), DMA_COPY_WORDS(2),         \
              REG_DMA3);                                                       \
    }                                                                          \
  }

/// 4 scenes: eastern_alhafra/s4004, jupiter_lighthouse_room_3/s0dcc,
/// jupiter_lighthouse_room_8/s0c44, mars_lighthouse_room_f/s3d80.
#define DEFINE_FUNC_02000DAC(name, data1, data2, data3)                        \
  struct Actor *name(s32 *direction, s32 *slot, s32 *kind) {                   \
    struct EventWork *work = Ram_HeapSlots->eventWork;                         \
    struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);           \
    struct Actor **entry;                                                      \
    struct Actor *object;                                                      \
    u32 i, k;                                                                  \
    s32 x, z, left, top, right, bottom;                                        \
                                                                               \
    *direction = leader->facing >> 12;                                         \
    i = 8;                                                                     \
    entry = &work->objects[8];                                                 \
  outer:                                                                       \
    object = *entry;                                                           \
    k = 0;                                                                     \
  inner:                                                                       \
    if (((struct ObjectAnimationState *)object->sprite)->value28[0] ==         \
        data1[k]) {                                                            \
      x = (PIXELS(leader->x) + (data2[*direction] >> 16)) >> 4;                \
      z = (PIXELS(leader->z) + (s16)data2[*direction]) >> 4;                   \
      left = (PIXELS(object->x) + data3[k][0]) >> 4;                           \
      top = (PIXELS(object->z) + data3[k][1]) >> 4;                            \
      right = (PIXELS(object->x) + data3[k][2]) >> 4;                          \
      bottom = (PIXELS(object->z) + data3[k][3]) >> 4;                         \
      if (left <= x && x < right && top <= z && z < bottom) {                  \
        *kind = k;                                                             \
        if (k & 1) {                                                           \
          if (left != leader->x >> 20) {                                       \
            *slot = i;                                                         \
            return object;                                                     \
          }                                                                    \
        } else {                                                               \
          if (top != leader->z >> 20) {                                        \
            *slot = i;                                                         \
            return object;                                                     \
          }                                                                    \
        }                                                                      \
      }                                                                        \
    }                                                                          \
    k++;                                                                       \
    if (k <= 5)                                                                \
      goto inner;                                                              \
    entry++;                                                                   \
    i++;                                                                       \
    if (i <= 63)                                                               \
      goto outer;                                                              \
    return NULL;                                                               \
  }

/// 3 scenes: mars_lighthouse_entrance, mars_lighthouse_room_m,
/// mars_lighthouse_star_room.
#define DEFINE_FUNC_020015EC(name, id1, call1, call2)                          \
  void name(struct Actor *object) {                                            \
    struct id1 *work = (struct id1 *)(gData_020023c4 + 0x188);                 \
    struct Actor *actor = Actor_Get(gMainRoutineWork.currentOwner);            \
    struct EventWork *event = Ram_HeapSlots->eventWork;                        \
    s32 dx = object->bounce;                                                   \
    s32 dz;                                                                    \
    s32 i;                                                                     \
    s32 direction;                                                             \
    s32 id;                                                                    \
                                                                               \
    object->x += dx;                                                           \
    object->y += object->gravity;                                              \
    dz = object->unk4c;                                                        \
    object->z += dz;                                                           \
    object->bounce = dx - dx / 18;                                             \
    object->unk4c = dz - dz / 16;                                              \
    object->scaleX += object->speedLimit;                                      \
    object->scaleY += object->acceleration;                                    \
    ((struct SpriteParts *)object->sprite)->unk12 += object->timer;            \
    if (actor->y == actor->terrainHeight && call1(object, actor) &&            \
        gMainRoutineWork.renderMode == 0) {                                    \
      if (object->unk4c != 0) {                                                \
        work->value68 = 0;                                                     \
        {                                                                      \
          u16 *cell = &work->direction;                                        \
                                                                               \
          if (object->unk4c > 0) {                                             \
            direction = 1;                                                     \
            *cell = direction;                                                 \
          } else {                                                             \
            direction = 0xffff;                                                \
            *cell = direction;                                                 \
          }                                                                    \
        }                                                                      \
      }                                                                        \
      object->priorityFlags = 2;                                               \
      {                                                                        \
        s32 speed = object->unk4c;                                             \
                                                                               \
        object->unk4c = 0;                                                     \
        object->z -= speed;                                                    \
        object->gravity += speed >> 2;                                         \
      }                                                                        \
      work->landed = 1;                                                        \
      event->exit = work->value0a;                                             \
    }                                                                          \
    for (i = 0; i < 4 && (id = work->slots[i].id) != 0; i++) {                 \
      actor = Actor_Get(id);                                                   \
                                                                               \
      if (actor->y == actor->terrainHeight && call2(object, actor)) {          \
        if (work->slots[i].blocked == 0) {                                     \
          s32 speed;                                                           \
                                                                               \
          object->priorityFlags = 2;                                           \
          speed = object->unk4c;                                               \
          object->unk4c = 0;                                                   \
          object->z -= speed;                                                  \
          object->gravity += speed >> 2;                                       \
        } else {                                                               \
          work->blocked = 1;                                                   \
        }                                                                      \
      }                                                                        \
    }                                                                          \
  }

/// 11 scenes: jupiter_lighthouse_aerie/s17a8,
/// jupiter_lighthouse_basement/s098c, jupiter_lighthouse_entrance/dist,
/// jupiter_lighthouse_room_1/s0674, jupiter_lighthouse_room_3/s1300,
/// jupiter_lighthouse_room_4/s0b50, jupiter_lighthouse_room_5/s1650,
/// jupiter_lighthouse_room_8/s1178, jupiter_lighthouse_room_9/s0910,
/// jupiter_lighthouse_room_m/s3474, mars_lighthouse_room_f/s3d80.
#define DEFINE_FUNC_02000DF0(name, data1, data2)                               \
  void name(s32 id, s32 value) {                                               \
    struct Actor *object = Actor_Get(id);                                      \
                                                                               \
    if (object != NULL) {                                                      \
      s16 *count;                                                              \
                                                                               \
      object->timer = value;                                                   \
      count = &data1;                                                          \
      data2[*count] = object;                                                  \
      (*count)++;                                                              \
    }                                                                          \
  }

/// 7 scenes: jupiter_lighthouse_room_2/s0d94, jupiter_lighthouse_room_3/s07b0,
/// jupiter_lighthouse_room_6/s0854, jupiter_lighthouse_room_7/s16d4,
/// jupiter_lighthouse_room_8/s07e8, jupiter_lighthouse_room_9/s0910,
/// jupiter_lighthouse_room_m/s3474.
#define DEFINE_FUNC_020008C0(name, call1)                                      \
  s32 name(struct Actor *object) {                                             \
    struct JupiterLighthouseAerieSpark spark;                                  \
    s32 frame = gFrameCount & 7;                                               \
                                                                               \
    if (frame == 0) {                                                          \
      s32 x;                                                                   \
      s32 y;                                                                   \
      s32 speed;                                                               \
                                                                               \
      spark.word[0] = 3;                                                       \
      spark.word[2] = 0xb333;                                                  \
      spark.word[3] = 0xb333;                                                  \
      spark.word[1] = 14;                                                      \
      x = object->x + (((((u32)Random16Far() * 9) >> 16) - 4) << 16);          \
      y = object->y + ((32 - (((u32)Random16Far() * 20) >> 16)) << 16);        \
      speed = (s32)(((((u32)Random16Far() * 5) >> 16) << 16) + 0x50000) / 10;  \
      call1(x, y, object->z, 0, speed, frame, 0xb0000, &spark);                \
    }                                                                          \
    return 0;                                                                  \
  }

/// 19 scenes: airs_rock, airs_rock_inside, jupiter_lighthouse_aerie,
/// jupiter_lighthouse_aerie_beacon/s0198, jupiter_lighthouse_basement,
/// jupiter_lighthouse_entrance/dist, jupiter_lighthouse_room_1,
/// jupiter_lighthouse_room_2, jupiter_lighthouse_room_4,
/// jupiter_lighthouse_room_5, jupiter_lighthouse_room_6,
/// jupiter_lighthouse_room_7, jupiter_lighthouse_room_8,
/// jupiter_lighthouse_room_9, jupiter_lighthouse_room_m, magma_rock,
/// magma_rock_inside, mars_lighthouse_room_f, sea_of_time.
#define DEFINE_FUNC_020003D0(name)                                             \
  s32 name(s32 *a, s32 *b) {                                                   \
    s32 dx = (a[0] - b[0]) >> 16;                                              \
    s32 dz = (a[2] - b[2]) >> 16;                                              \
                                                                               \
    return Iwram_Sqrt(dx * dx + dz * dz);                                      \
  }

/// 3 scenes: mars_lighthouse_entrance/s2270, mars_lighthouse_room_f,
/// mars_lighthouse_room_m/s2118.
#define DEFINE_FUNC_0200218C(name, call1)                                      \
  void name(u16 *targets, u16 *stops, s32 a2, s32 a3) {                        \
    struct MarsLighthouseBeam *beam =                                          \
        (struct MarsLighthouseBeam *)(gData_020023c4 + 0x188);                 \
    struct MarsLighthouseBeamTarget *target = beam->targets;                   \
    s32 id;                                                                    \
    s32 i;                                                                     \
    s32 value;                                                                 \
    s32 one;                                                                   \
    s32 flags;                                                                 \
    s32 height;                                                                \
    struct Actor *object;                                                      \
    s32 alpha;                                                                 \
    s32 control;                                                               \
                                                                               \
    if (Event_GetFlag(0x109) == 0) {                                           \
      for (i = 0; i < 4 && (id = *targets++) > 0; i++) {                       \
        flags = *targets;                                                      \
        height = flags & 15;                                                   \
        flags &= 0xf0;                                                         \
        object = Actor_Get(id);                                                \
        targets++;                                                             \
        if (object != NULL) {                                                  \
          Actor_SetSingleChildFlags(object, 0);                                \
          ((u8 *)object)[98] = one = 1;                                        \
          if ((flags & 0x80) == 0)                                             \
            object->collisionFlags = 0;                                        \
          target->active = one;                                                \
          target->flags = flags;                                               \
          target->object = Actor_Get(id);                                      \
          target->height = height << 16;                                       \
          target->index = i;                                                   \
          target++;                                                            \
        }                                                                      \
      }                                                                        \
      if (stops != NULL) {                                                     \
        for (i = 0; i < 4; i++) {                                              \
          value = *stops++;                                                    \
          beam->stops[i].unk00 = value;                                        \
          if (value <= 0)                                                      \
            break;                                                             \
          beam->stops[i].unk02 = *stops++;                                     \
        }                                                                      \
      }                                                                        \
    }                                                                          \
    beam->unk10 = a2;                                                          \
    beam->unk0a = a3;                                                          \
    if (REG_BLDCNT == 0) {                                                     \
      alpha = 0xc08;                                                           \
      REG_BLDALPHA = alpha;                                                    \
      control = 0x3f10;                                                        \
      REG_BLDCNT = control;                                                    \
    }                                                                          \
    Task_AddFrame(call1, 0x480);                                               \
  }

/// 3 scenes: mars_lighthouse_entrance, mars_lighthouse_room_f,
/// mars_lighthouse_room_m.
#define DEFINE_FUNC_02001B64(name, id1, call1, call2)                          \
  void name(struct Actor *object) {                                            \
    struct id1 *work = (struct id1 *)(gData_020023c4 + 0x188);                 \
    struct Actor *other = Actor_Get(gMainRoutineWork.currentOwner);            \
    struct EventWork *event = Ram_HeapSlots->eventWork;                        \
    s32 dx = object->bounce;                                                   \
    s32 dz;                                                                    \
    s32 i;                                                                     \
    struct RideDirection *direction;                                           \
                                                                               \
    object->z += dx;                                                           \
    object->y += object->gravity;                                              \
    dz = object->unk4c;                                                        \
    object->x += dz;                                                           \
    object->bounce = dx - dx / 18;                                             \
    object->unk4c = dz - dz / 16;                                              \
    object->scaleX += object->speedLimit;                                      \
    object->scaleY += object->acceleration;                                    \
    ((struct SpriteParts *)object->sprite)->unk12 += object->timer;            \
    if (other->y == other->terrainHeight && call1(object, other) &&            \
        gMainRoutineWork.renderMode == 0) {                                    \
      if (object->unk4c != 0) {                                                \
        work->direction.unk02 = 0;                                             \
        direction = &work->direction;                                          \
        if (object->unk4c > 0)                                                 \
          direction->unk00 = 1;                                                \
        else                                                                   \
          direction->unk00 = 0xffff;                                           \
      }                                                                        \
      object->priorityFlags = 2;                                               \
      object->z -= object->bounce;                                             \
      object->gravity += object->bounce >> 2;                                  \
      object->unk4c = 0;                                                       \
      work->landed = 1;                                                        \
      event->exit = work->value0a;                                             \
    }                                                                          \
    for (i = 0; i < 4; i++) {                                                  \
      s32 id = work->slots[i].id;                                              \
                                                                               \
      if (id == 0)                                                             \
        break;                                                                 \
      other = Actor_Get(id);                                                   \
      if (other->y == other->terrainHeight && call2(object, other)) {          \
        if (work->slots[i].blocked == 0) {                                     \
          object->priorityFlags = 2;                                           \
          object->z -= object->unk4c;                                          \
          object->gravity += object->unk4c >> 2;                               \
          object->unk4c = 0;                                                   \
        } else {                                                               \
          work->blocked = 1;                                                   \
        }                                                                      \
      }                                                                        \
    }                                                                          \
  }

/// 4 scenes: airs_rock_inside/s0038, tundaria_tower/events_f, yampi_desert,
/// yampi_desert_cave.
#define DEFINE_FUNC_02000128(name, call1, call2)                               \
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

/// 4 scenes: eastern_alhafra/s44c4, izumo_ruins/s0038,
/// jupiter_lighthouse_room_7/s16d4, mars_lighthouse_entrance/s42f8.
#define DEFINE_FUNC_02000128__S00A4(name, call1, call2)                        \
  void name(s32 id, u32 kind, s32 flag, s32 layer) {                           \
    struct Actor *object = Actor_Get(id);                                      \
    struct MapCell *cell = Ram_HeapSlots->mapWork->layers[layer].cells;        \
                                                                               \
    cell += call1(object);                                                     \
    if (Event_GetFlag(flag)) {                                                 \
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

/// 4 scenes: jupiter_lighthouse_aerie/s24a8, jupiter_lighthouse_room_8/s1c30,
/// jupiter_lighthouse_room_9/s1648, jupiter_lighthouse_room_m/s41ac.
#define DEFINE_FUNC_02001648(name, id1, call1)                                 \
  void name(struct id1 *table, struct CellTargetEntry *entry) {                \
    struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);           \
    s32 x = (leader->x >> 20) | (s16)(table->x & 64);                          \
    s32 z = (leader->z >> 20) | (s16)(table->z & 64);                          \
                                                                               \
    call1();                                                                   \
    Event_PlaySound(229);                                                      \
    MapGrid_CopyCells(table->x, table->z + 2, 1, 1, x, z);                     \
    FieldEvent_Wait(12);                                                       \
    leader->facing = 0x4000;                                                   \
    leader->motionFlags = 0;                                                   \
    Actor_SetSingleChildFlags(Actor_Get(gMainRoutineWork.currentOwner), 0);    \
    Io_Write16(0xf1f, &REG_WINOUT);                                            \
    REG_DISPCNT |= 0x8000;                                                     \
    x = 0;                                                                     \
    table->shown = 1;                                                          \
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 28);                     \
    FieldEvent_Wait(16);                                                       \
    do {                                                                       \
      if (x == 5)                                                              \
        Event_PlaySound(204);                                                  \
      leader->scaleX += -0x400;                                                \
      leader->scaleY += -0x300;                                                \
      leader->y += -0x9999;                                                    \
      System_Wait(1);                                                          \
      x++;                                                                     \
    } while (x <= 39);                                                         \
    Actor_Get(gMainRoutineWork.currentOwner)->animationKind = 0;               \
    table->shown = 0;                                                          \
    *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x209;                  \
    Event_FinishScreenTransition();                                            \
    Event_WaitTransitionFrames();                                              \
  }

/// 4 scenes: mars_lighthouse_entrance/s263c, mars_lighthouse_room_f/s328c,
/// mars_lighthouse_room_m/s2708, mars_lighthouse_star_room/s2148.
#define DEFINE_FUNC_02002B98(name, call1)                                      \
  s32 name(struct Actor *object) {                                             \
    s32 frame = object->timer;                                                 \
                                                                               \
    if (frame == 0) {                                                          \
      object->y = Map_GetTerrainHeight(0, object->x, object->z) + 0x100000;    \
      object->scaleX = frame;                                                  \
      object->scaleY = frame;                                                  \
      object->speedLimit = 0x10000;                                            \
      object->acceleration = 0x10000;                                          \
      Actor_SetSpritePriorityDirect(object, 1);                                \
    } else if (frame <= 30) {                                                  \
      if (frame == 30) {                                                       \
        Event_PlaySound(136);                                                  \
        call1(object->watchedObject, 2);                                       \
      }                                                                        \
    } else if (frame <= 60) {                                                  \
      object->scaleX = 0x10000;                                                \
      object->scaleY = 0x10000;                                                \
      {                                                                        \
        s32 half = (object->timer / 2) & 1;                                    \
                                                                               \
        Animation_ApplyChildValues(object->watchedObject, half * 7);           \
      }                                                                        \
    } else {                                                                   \
      call1(object->watchedObject, 1);                                         \
      Event_PlaySound(184);                                                    \
      object->y = Map_GetTerrainHeight(0, object->x, object->z);               \
      object->watchedObject = 0;                                               \
      return 0;                                                                \
    }                                                                          \
    object->timer++;                                                           \
    return 1;                                                                  \
  }

/// 4 scenes: mars_lighthouse_entrance/s263c, mars_lighthouse_room_f/s328c,
/// mars_lighthouse_room_m/s2708, mars_lighthouse_star_room/s2148.
#define DEFINE_FUNC_02002C44(name, call1)                                      \
  s32 name(struct Actor *object) {                                             \
    s32 frame = object->timer;                                                 \
                                                                               \
    if (frame == 0) {                                                          \
      Event_PlaySound(136);                                                    \
      call1(object->watchedObject, 2);                                         \
      object->y = Map_GetTerrainHeight(0, object->x, object->z) + 0x100000;    \
      object->scaleX = frame;                                                  \
      object->scaleY = frame;                                                  \
      object->speedLimit = 0x10000;                                            \
      object->acceleration = 0x8000;                                           \
      Actor_SetSpritePriorityDirect(object, 1);                                \
    } else if (frame <= 32) {                                                  \
      object->scaleX = 0x10000;                                                \
      object->scaleY = 0x10000;                                                \
      {                                                                        \
        s32 half = (object->timer / 2) & 1;                                    \
                                                                               \
        Animation_ApplyChildValues(object->watchedObject, half * 7);           \
      }                                                                        \
    } else {                                                                   \
      call1(object->watchedObject, 1);                                         \
      Event_PlaySound(184);                                                    \
      object->y = Map_GetTerrainHeight(0, object->x, object->z);               \
      object->watchedObject = 0;                                               \
      return 0;                                                                \
    }                                                                          \
    object->timer++;                                                           \
    return 1;                                                                  \
  }

/// 2 scenes: islets/s3410, mars_lighthouse_room_f/s3d80.
#define DEFINE_FUNC_02003A5C(name, data1, call1, call2, call3, data2)          \
  void name(struct PushTarget target) {                                        \
    u8 *map = (u8 *)((struct HeapSlotWords *)Ram_HeapSlots)->mapWork;          \
    struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);           \
    struct Actor *object = Actor_Get(target.id);                               \
    s32 cell[3];                                                               \
    s32 from[3];                                                               \
    s32 footprint;                                                             \
    s32 width;                                                                 \
    s32 height;                                                                \
    s32 originX;                                                               \
    s32 originZ;                                                               \
    s32 direction;                                                             \
    s32 speed;                                                                 \
    s32 acceleration;                                                          \
                                                                               \
    direction = leader->facing >> 16;                                          \
    footprint = target.footprint;                                              \
    speed = leader->speedLimit;                                                \
    acceleration = leader->acceleration;                                       \
    from[0] = object->x;                                                       \
    from[2] = object->z;                                                       \
    cell[0] = (object->x + (data1[footprint][0] << 16)) >> 20;                 \
    cell[2] = (object->z + (data1[footprint][1] << 16)) >> 20;                 \
    width = (ABS(data1[footprint][0]) + ABS(data1[footprint][2])) >> 4;        \
    height = (ABS(data1[footprint][1]) + ABS(data1[footprint][3])) >> 4;       \
    call1(0, cell[0], cell[2], width, height, 0);                              \
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x1999);             \
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 8);                      \
    System_Wait(15);                                                           \
    {                                                                          \
      s32 deltaX = (target.x - from[0]) >> 17;                                 \
      s32 deltaZ = (target.z - from[2]) >> 17;                                 \
                                                                               \
      Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, deltaX,        \
                                 deltaZ);                                      \
    }                                                                          \
    Actor_Get(gMainRoutineWork.currentOwner)->update = call2;                  \
    System_Wait(4);                                                            \
    if ((u32)(direction - 6) <= 7)                                             \
      call3(object, 3);                                                        \
    else                                                                       \
      call3(object, 2);                                                        \
    Event_PlaySound(239);                                                      \
    Actor_SetSpeed(target.id, 0x8000, 0x1999);                                 \
    FieldActor_SetMoveTarget(object, target.x, target.y, target.z);            \
    FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);                 \
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);                      \
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x4ccc, 0x1999);             \
    Actor_SetDestinationOffset(gMainRoutineWork.currentOwner,                  \
                               (data2[direction] << 16) >> 31,                 \
                               (data2[direction] << 16) >> 17);                \
    if (target.callback != 0)                                                  \
      target.callback();                                                       \
    FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);                 \
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);                      \
    leader->update = 0;                                                        \
    leader->speedLimit = speed;                                                \
    leader->acceleration = acceleration;                                       \
    Actor_WaitForMove(object);                                                 \
    Event_PlaySound(298);                                                      \
    Event_PlaySound(213);                                                      \
    FieldActor_SetPositionAndResetMotion(object, target.x, object->y,          \
                                         target.z);                            \
    call3(object, 1);                                                          \
    target.x = target.x + (data1[footprint][0] << 16);                         \
    target.z = target.z + (data1[footprint][1] << 16);                         \
    target.x >>= 20;                                                           \
    target.z >>= 20;                                                           \
    originX = *(s32 *)(map + 328) >> 20;                                       \
    originZ = *(s32 *)(map + 332) >> 20;                                       \
    MapGrid_CopyCellAttributes(target.x, target.z, width, height,              \
                               originX + target.x, originZ + target.z);        \
    call1(0, target.x, target.z, width, height, 255);                          \
    call1(2, target.x, target.z, width, height, 255);                          \
    cell[0] = (from[0] + (data1[footprint][0] << 16)) >> 20;                   \
    cell[2] = (from[2] + (data1[footprint][1] << 16)) >> 20;                   \
    originX += cell[0];                                                        \
    originZ += cell[2];                                                        \
    MapGrid_CopyCellAttributes(originX, originZ, width, height, cell[0],       \
                               cell[2]);                                       \
    call1(2, cell[0], cell[2], width, height, 0);                              \
    Event_NoOp();                                                              \
  }

/// 2 scenes: jupiter_lighthouse_room_3/s0dcc, jupiter_lighthouse_room_8/s0c44.
#define DEFINE_FUNC_02000EC8(name, data1, call1, call2, data2)                 \
  void name(struct PushTarget target) {                                        \
    u8 *map = (u8 *)((struct HeapSlotWords *)Ram_HeapSlots)->mapWork;          \
    struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);           \
    struct Actor *object = Actor_Get(target.id);                               \
    s32 cell[3];                                                               \
    s32 from[3];                                                               \
    s32 footprint;                                                             \
    s32 width;                                                                 \
    s32 height;                                                                \
    s32 originX;                                                               \
    s32 originZ;                                                               \
    s32 direction;                                                             \
    s32 speed;                                                                 \
    s32 acceleration;                                                          \
                                                                               \
    direction = leader->facing >> 16;                                          \
    footprint = target.footprint;                                              \
    speed = leader->speedLimit;                                                \
    acceleration = leader->acceleration;                                       \
    from[0] = object->x;                                                       \
    from[2] = object->z;                                                       \
    cell[0] = (object->x + (data1[footprint][0] << 16)) >> 20;                 \
    cell[2] = (object->z + (data1[footprint][1] << 16)) >> 20;                 \
    width = (ABS(data1[footprint][0]) + ABS(data1[footprint][2])) >> 4;        \
    height = (ABS(data1[footprint][1]) + ABS(data1[footprint][3])) >> 4;       \
    Map_FillCellRectByte2(0, cell[0], cell[2], width, height, 0);              \
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x1999);             \
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 8);                      \
    System_Wait(15);                                                           \
    {                                                                          \
      s32 deltaX = (target.x - from[0]) >> 17;                                 \
      s32 deltaZ = (target.z - from[2]) >> 17;                                 \
                                                                               \
      Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, deltaX,        \
                                 deltaZ);                                      \
    }                                                                          \
    Actor_Get(gMainRoutineWork.currentOwner)->update = call1;                  \
    System_Wait(4);                                                            \
    if ((u32)(direction - 6) <= 7)                                             \
      call2(object, 3);                                                        \
    else                                                                       \
      call2(object, 2);                                                        \
    Event_PlaySound(239);                                                      \
    Actor_SetSpeed(target.id, 0x8000, 0x1999);                                 \
    FieldActor_SetMoveTarget(object, target.x, target.y, target.z);            \
    FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);                 \
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);                      \
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x4ccc, 0x1999);             \
    Actor_SetDestinationOffset(gMainRoutineWork.currentOwner,                  \
                               (data2[direction] << 16) >> 31,                 \
                               (data2[direction] << 16) >> 17);                \
    if (target.callback != 0)                                                  \
      target.callback();                                                       \
    FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);                 \
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);                      \
    leader->update = 0;                                                        \
    leader->speedLimit = speed;                                                \
    leader->acceleration = acceleration;                                       \
    Actor_WaitForMove(object);                                                 \
    Event_PlaySound(298);                                                      \
    Event_PlaySound(213);                                                      \
    FieldActor_SetPositionAndResetMotion(object, target.x, object->y,          \
                                         target.z);                            \
    call2(object, 1);                                                          \
    target.x = target.x + (data1[footprint][0] << 16);                         \
    target.z = target.z + (data1[footprint][1] << 16);                         \
    target.x >>= 20;                                                           \
    target.z >>= 20;                                                           \
    originX = *(s32 *)(map + 328) >> 20;                                       \
    originZ = *(s32 *)(map + 332) >> 20;                                       \
    MapGrid_CopyCellAttributes(target.x, target.z, width, height,              \
                               originX + target.x, originZ + target.z);        \
    Map_FillCellRectByte2(0, target.x, target.z, width, height, 255);          \
    Map_FillCellRectByte2(2, target.x, target.z, width, height, 255);          \
    cell[0] = (from[0] + (data1[footprint][0] << 16)) >> 20;                   \
    cell[2] = (from[2] + (data1[footprint][1] << 16)) >> 20;                   \
    originX += cell[0];                                                        \
    originZ += cell[2];                                                        \
    MapGrid_CopyCellAttributes(originX, originZ, width, height, cell[0],       \
                               cell[2]);                                       \
    Map_FillCellRectByte2(2, cell[0], cell[2], width, height, 0);              \
    Event_NoOp();                                                              \
  }

/// 4 scenes: mars_lighthouse_entrance/s2270, mars_lighthouse_room_f/ride,
/// mars_lighthouse_room_m/s2118, mars_lighthouse_star_room/ride_handle.
#define DEFINE_FUNC_02001FCC(name)                                             \
  void name(void) {                                                            \
    struct MarsLighthouseEventWork *work = Ram_HeapSlots->eventWork;           \
    s32 leader = FieldParty_GetLeaderActor();                                  \
    s32 count;                                                                 \
    s32 i;                                                                     \
    s16 hp[8];                                                                 \
                                                                               \
    (void *)Actor_Get(leader);                                                 \
    count = Party_CountMembers();                                              \
    for (i = 0; i < count; i++)                                                \
      hp[i] = Unit_Get(gMainRoutineWork.activeOwners[i])->hp;                  \
    BattleParty_ApplyHealthDelta(-10, 0);                                      \
    work->downCount = 0;                                                       \
    work->standingCount = 0;                                                   \
    Actor_SetAttachedEffect(leader, 258);                                      \
    for (i = 0; i < count; i++) {                                              \
      struct MainRoutineWork *main = &gMainRoutineWork;                        \
      struct BattleUnit *unit = Unit_Get(main->activeOwners[i]);               \
                                                                               \
      if (unit->hp > 0) {                                                      \
        work->standingCount++;                                                 \
      } else if (hp[i] != 0) {                                                 \
        work->downOwners[(s16)work->downCount++] = main->activeOwners[i];      \
        work->unk16a = 0xffff;                                                 \
        ((u8 *)unit)[0x131] = 0;                                               \
      }                                                                        \
    }                                                                          \
  }

/// 4 scenes: jupiter_lighthouse_aerie_beacon/s44cc,
/// jupiter_lighthouse_entrance/dist, jupiter_lighthouse_room_9/s0910,
/// jupiter_lighthouse_room_m/s3474.
#define DEFINE_FUNC_020007A4(name, call1, call2, call3)                        \
  struct GustObject *name(struct GustObject *source) {                         \
    struct Vector3 position;                                                   \
    struct EffectOptions options;                                              \
    struct GustObject *object;                                                 \
                                                                               \
    Event_PlaySound(222);                                                      \
    position.x = source->x + Value2((s32 (*)())Iwram_MulQ16, 0x80000,          \
                                    Math_Cosine(source->angle));               \
    position.z = source->z + Iwram_MulQ16(0x80000, Math_Sin(source->angle));   \
    object = ((struct GustObject *)FieldActor_Create(280, position.x,          \
                                                     source->y, position.z));  \
    call1(object, 2);                                                          \
    Actor_SetSingleChildFlags(object, 0);                                      \
    object->motionFlags = 0;                                                   \
    object->priorityFlags &= ~1;                                               \
    object->angle = source->angle;                                             \
    object->unk64 = 0;                                                         \
    object->unk62 = 0;                                                         \
    object->unk63 = 0;                                                         \
    object->update = call2;                                                    \
    object->unk30 = 0x18000;                                                   \
    options.priority = 1;                                                      \
    options.palette = 7;                                                       \
    call3(position.x, source->y, position.z - 0x60000, 0, 0, 0, 0x30000,       \
          &options);                                                           \
    return object;                                                             \
  }

/// 4 scenes: eastern_alhafra, magma_rock/s34ac, magma_rock_inside/s46e8, prox.
#define DEFINE_FUNC_0200379C(name, call1)                                      \
  void name(void) {                                                            \
    struct ParticleWork *work = Ram_HeapSlots->menuRuntime;                    \
    struct Particle *particle = work->particles;                               \
    struct ParticleSprite *sprite = work->sprites;                             \
    s32 i;                                                                     \
    s32 step = -0x8000;                                                        \
                                                                               \
    i = 63;                                                                    \
  next:                                                                        \
    if ((u32)particle->age <= 19) {                                            \
      u16 *base = &work->tile;                                                 \
      s32 mask = 7;                                                            \
      s32 tile = (u16)((*base + ((particle->age >> 2) & mask) * 8) & 0x3ff);   \
      s32 attr = sprite->attr;                                                 \
                                                                               \
      sprite->attr = (attr & ~0x3ff) | tile;                                   \
      call1(sprite, particle);                                                 \
      BattleEffect_StepVector3(particle, 63, step);                            \
      particle->age++;                                                         \
    }                                                                          \
    i--;                                                                       \
    sprite++;                                                                  \
    particle++;                                                                \
    if (i >= 0)                                                                \
      goto next;                                                               \
  }

/// 4 scenes: mars_lighthouse_entrance/s263c, mars_lighthouse_room_f/s3d80,
/// mars_lighthouse_star_room/s2148, prox.
#define DEFINE_FUNC_02002D04(name, data1, call1)                               \
  void name(struct Actor *object) {                                            \
    s32 angle;                                                                 \
    s32 turn;                                                                  \
                                                                               \
    if (object->timer != 0) {                                                  \
      object->timer--;                                                         \
      return;                                                                  \
    }                                                                          \
    *(u8 *)((u8 *)object + 0x5a) = 0;                                          \
    if (Event_GetFlag(0x106))                                                  \
      angle = -1;                                                              \
    else                                                                       \
      angle = data1[(gInput.held >> 4) & 15];                                  \
    if (angle == -1) {                                                         \
      call1(object, 9);                                                        \
      return;                                                                  \
    }                                                                          \
    turn = (s16)(angle - object->facing);                                      \
    if (turn > 0x1000)                                                         \
      turn = 0x1000;                                                           \
    if (turn < -0x1000)                                                        \
      turn = -0x1000;                                                          \
    object->facing += turn;                                                    \
    call1(object, 2);                                                          \
    Actor_SetAnimationSpeed(object, 48);                                       \
  }

/// 2 scenes: magma_rock_inside/s3678, trial_road/s15a8.
#define DEFINE_FUNC_0200406C(name, call1, call2, call3, call4, call5, call6)   \
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
        Actor_SetPartAttribute(Actor_GetById(id), 7);                          \
        wait(2);                                                               \
        call3(trap, 0);                                                        \
        Actor_SetPartAttribute(Actor_GetById(id), 0);                          \
        wait(2);                                                               \
        Actor_SetPartAttribute(Actor_GetById(id), 7);                          \
        wait(4);                                                               \
        Actor_SetPartAttribute(Actor_GetById(id), 0);                          \
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

/// 2 scenes: jupiter_lighthouse_room_7, jupiter_lighthouse_room_m.
#define DEFINE_FUNC_020012FC(name)                                             \
  void name(void) {                                                            \
    struct EventWork *work = Ram_HeapSlots->eventWork;                         \
    struct Actor *actor = Actor_Get(gMainRoutineWork.currentOwner);            \
    struct Actor *linked = actor->watchedObject;                               \
    struct Actor *shadow;                                                      \
    u8 *p;                                                                     \
    s32 angle;                                                                 \
    s32 i;                                                                     \
                                                                               \
    Event_Start();                                                             \
    FieldEvent_ResolvePendingActions(0);                                       \
    i = 0;                                                                     \
    work->unkd46 = i;                                                          \
                                                                               \
    actor->motionFlags = 4;                                                    \
    Actor_SetSingleChildFlags(actor, 0);                                       \
                                                                               \
    while (*((u8 *)linked + 0x63) != 0) {                                      \
      actor->x = linked->x;                                                    \
      actor->y = linked->y + i;                                                \
      actor->z = linked->z;                                                    \
      if (i <= 0x3ffff)                                                        \
        i += 0x1999;                                                           \
      actor->facing = (u8)gFrameCount << 12;                                   \
      p = &actor->priorityFlags;                                               \
      *p ^= 2 << (gFrameCount & 1);                                            \
      System_Wait(1);                                                          \
    }                                                                          \
    work->unkd44 = 1;                                                          \
    work->unkd46 = 1;                                                          \
    angle = linked->facing;                                                    \
    actor->x = (((linked->x >> 19) & ~1) + (angle >> 13) - 1) << 19;           \
    actor->z = (((linked->z >> 19) & ~1) - ((angle >> 13) & 2) + 1) << 19;     \
    actor->velocityY = 0x60000;                                                \
    actor->priorityFlags = 1;                                                  \
    actor->motionFlags = 3;                                                    \
    Actor_SetSingleChildFlags(actor, 1);                                       \
    Camera_MoveWithinMapBounds_(-1, -1, -1, 0);                                \
    shadow = FieldCamera_GetActor();                                           \
    shadow->speedLimit = 0x80000;                                              \
    shadow->acceleration = 0x10000;                                            \
    shadow->motionFlags = 0;                                                   \
    FieldActor_SetMoveTarget(shadow, actor->x,                                 \
                             Map_GetTerrainHeight(0, actor->x, actor->z),      \
                             actor->z);                                        \
    i = 0;                                                                     \
    if (actor->y > actor->terrainHeight + 0x100000) {                          \
      do {                                                                     \
        System_Wait(1);                                                        \
        i++;                                                                   \
        if (i > 59)                                                            \
          break;                                                               \
      } while (actor->y > actor->terrainHeight + 0x100000);                    \
    }                                                                          \
    Event_PlaySound(127);                                                      \
    i = 0;                                                                     \
    if (actor->velocityY != 0) {                                               \
      do {                                                                     \
        System_Wait(1);                                                        \
        i++;                                                                   \
        if (i > 59)                                                            \
          break;                                                               \
      } while (actor->velocityY != 0);                                         \
    }                                                                          \
    Actor_WaitForMove(shadow);                                                 \
    Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 1;              \
    FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);                 \
    work->unkd46 = 1;                                                          \
    work->exit = 0;                                                            \
    Event_End();                                                               \
  }

/// 4 scenes: jupiter_lighthouse_room_5/s2450, jupiter_lighthouse_room_6/s1724,
/// jupiter_lighthouse_room_8/s1dc8, jupiter_lighthouse_room_9/s17e0.
#define DEFINE_FUNC_0200196C(name, data1, call1)                               \
  void name(s32 x, s32 z, s32 effect) {                                        \
    struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);           \
    struct JupiterLighthouseRoom8EventWork *work = Ram_HeapSlots->eventWork;   \
    s32 dx = ((s32)FieldGameFlag_GetByte(data1.flagByte));                     \
    s32 dz = ((s32)FieldGameFlag_GetByte(data1.flagByte + 8));                 \
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

/// 3 scenes: mars_lighthouse_entrance/s263c, mars_lighthouse_room_m/s2708,
/// mars_lighthouse_star_room/s2148.
#define DEFINE_FUNC_02002AD0(name, call1)                                      \
  void name(struct Actor *self) {                                              \
    struct Actor *actor = Actor_Get(((u8 *)self)[0x62]);                       \
    s32 angle = (s16)self->unk66;                                              \
    struct FieldSprite *sprite = actor->sprite;                                \
    /* volatile: the call reads position[0] before [1] and [2] (as a plain */  \
    /* array, 2 of 86 differ in tla-en). */                                    \
    volatile s32 position[3];                                                  \
                                                                               \
    position[0] = self->x;                                                     \
    position[1] = self->y - PIXELS(12);                                        \
    position[2] = self->z;                                                     \
    FieldVector_AddPolarOffset(PIXELS(32), angle, (struct Vector3 *)position); \
    actor->facing = angle;                                                     \
    actor->motionFlags = 0;                                                    \
    FieldActor_SetPositionAndResetMotion(actor, position[0], position[1],      \
                                         position[2]);                         \
    actor->update = (void (*)(void))call1;                                     \
    actor->actionFlags = 0;                                                    \
    actor->speedLimit = 0x80000;                                               \
    actor->acceleration = 0x2000;                                              \
    sprite->rotation = angle - 0x4000;                                         \
    actor->scaleX = 0;                                                         \
    /* A once-only block: its end holds the order around it (as the plain */   \
    /* statement, 4 of 86 differ in tla-en). */                                \
    do {                                                                       \
      actor->scaleY = 0;                                                       \
    } while (0);                                                               \
    actor->timer = 0;                                                          \
    actor->watchedObject = self->watchedObject;                                \
    Event_PlaySound(104);                                                      \
  }

/// 2 scenes: magma_rock/s22ac, magma_rock_inside/s3678.
#define DEFINE_FUNC_020030BC(name, call1, call2, call3, call4, call5)          \
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
        ObjectVisual_CopyAttributes(id, 9);                                    \
    }                                                                          \
    list += 2;                                                                 \
    if (*list != 0xffff)                                                       \
      goto next;                                                               \
  done:                                                                        \
    System_Wait(10);                                                           \
  }

/// 3 scenes: mars_lighthouse_entrance, mars_lighthouse_room_f,
/// mars_lighthouse_star_room.
#define DEFINE_FUNC_02001A9C(name, call1)                                      \
  void name(struct Actor *object) {                                            \
    struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);           \
    s32 dx = object->bounce;                                                   \
    s32 dz;                                                                    \
    struct SpriteParts *parts;                                                 \
    s32 rate;                                                                  \
    s32 scale;                                                                 \
                                                                               \
    object->x += dx;                                                           \
    object->y += object->gravity;                                              \
    dz = object->unk4c;                                                        \
    object->z += dz;                                                           \
    object->bounce = dx - dx / 18;                                             \
    object->unk4c = dz - dz / 16;                                              \
    rate = object->speedLimit;                                                 \
    scale = object->scaleX;                                                    \
    parts = object->sprite;                                                    \
    object->scaleX = scale + rate;                                             \
    object->scaleY += object->acceleration;                                    \
    parts->unk12 += object->timer;                                             \
    if (leader->y == leader->terrainHeight && call1(object, leader)) {         \
      object->z -= object->unk4c;                                              \
      object->gravity += object->unk4c >> 2;                                   \
      object->unk4c = 0;                                                       \
    }                                                                          \
  }

/// 2 scenes: magma_rock/s22ac, magma_rock_inside/s3678.
#define DEFINE_FUNC_02002D08(name, call1, call2, call3, call4, call5)          \
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

/// 6 scenes: anemos_inner_sanctum/s0038, islet_cave,
/// mars_lighthouse_aerie/s3370, mars_lighthouse_entrance/s42f8,
/// mars_lighthouse_room_m, yampi_desert_cave.
#define DEFINE_FUNC_020001D4(name)                                             \
  s32 name(u16 *list) {                                                        \
    s32 count = 0;                                                             \
                                                                               \
    while (*list != 0xffff) {                                                  \
      list++;                                                                  \
      count++;                                                                 \
    }                                                                          \
    count--;                                                                   \
    return count;                                                              \
  }

/// 2 scenes: apojii_islands, aqua_rock.
#define DEFINE_FUNC_02001940(name, data1)                                      \
  void name(void) {                                                            \
    struct MapWork *view = Ram_HeapSlots->mapWork;                             \
    struct GroundParticleWork *work = Ram_HeapSlots->unk70[1];                 \
    s32 *camera = &view->viewX;                                                \
    struct GroundParticle *particle = work->particles;                         \
    s32 cameraX = camera[0];                                                   \
    s32 cameraZ = camera[1];                                                   \
    u32 i;                                                                     \
                                                                               \
    for (i = 0; i < 64; i++, particle++) {                                     \
      s16 *frame;                                                              \
      s32 x;                                                                   \
      s32 y;                                                                   \
                                                                               \
      if (--particle->timer == 0xffff)                                         \
        continue;                                                              \
      if (GameFlag_Test(0x166))                                                \
        particle->timer++;                                                     \
      frame = &data1[5 * particle->timer];                                     \
      x = (particle->posX - cameraX) / 0x10000 + *frame++;                     \
      y = (particle->posZ - particle->posY - cameraZ) / 0x10000 + *frame++;    \
      if ((x >= -16 && x <= 239) && y >= -32 && y <= 159) {                    \
        particle->priority = 1;                                                \
        particle->x = x;                                                       \
        particle->y = y;                                                       \
        particle->tile = work->tileBase + *(u16 *)frame;                       \
        frame++;                                                               \
        particle->shape = *(u8 *)frame;                                        \
        particle->size = *(u8 *)(frame + 1);                                   \
        Oam_AddSpriteAtDepth(particle, 240);                                   \
      }                                                                        \
      if (particle->timer == 0) {                                              \
        s32 *leader = work->leader;                                            \
                                                                               \
        if (leader != NULL) {                                                  \
          x = leader[0] + Random16Far() * 240 - 0x780000;                      \
          y = leader[2] + Random16Far() * 160 - 0x500000;                      \
          particle->posX = x;                                                  \
          particle->posZ = y;                                                  \
          particle->posY = Map_GetTerrainHeight(0, x, y);                      \
          particle->timer = 16;                                                \
        } else {                                                               \
          particle->posX = 0;                                                  \
          particle->posZ = 0;                                                  \
          particle->posY = 0;                                                  \
          particle->timer = 16;                                                \
        }                                                                      \
      }                                                                        \
    }                                                                          \
  }

/// 4 scenes: airs_rock/s00d0, gaia_rock_inside/s00d0, gondowan_cliffs,
/// treasure_isle/s00d0.
#define DEFINE_FUNC_020000D0(name)                                             \
  struct Actor *name(s32 a0, s32 a1, s32 a2, s32 a3) {                         \
    struct Actor *t0;                                                          \
                                                                               \
    t0 = FieldActor_Create(a3, a0, a1, a2);                                    \
    if (t0 != 0) {                                                             \
      Scene_SetActorSpritePriority(t0, 1);                                     \
      t0->motionFlags = 0;                                                     \
      t0->collisionFlags = 8;                                                  \
      Actor_SetSingleChildFlags(t0, 0);                                        \
      Actor_SetSpriteColor(t0, 0xf);                                           \
      t0->priorityFlags = 0x22 | t0->priorityFlags;                            \
      return t0;                                                               \
    }                                                                          \
    return 0;                                                                  \
  }

/// 2 scenes: mars_lighthouse_entrance, mars_lighthouse_room_f.
#define DEFINE_FUNC_02001E60(name, call1, call2, call3, call4)                 \
  void name(s32 *position, s32 speed, s32 mode) {                              \
    struct Actor *object = call1(366, position[0], position[1], position[2]);  \
                                                                               \
    if (object != NULL) {                                                      \
      object->bounce = 0;                                                      \
      object->gravity = 0;                                                     \
      object->unk4c = -speed;                                                  \
      call2(object, 3);                                                        \
      object->update = call3;                                                  \
      if (mode)                                                                \
        object->unk5c[0] = 2;                                                  \
      call4(object, 2);                                                        \
    }                                                                          \
  }

/// 3 scenes: apojii_islands, aqua_rock, aqua_rock_inside.
#define DEFINE_FUNC_02001140(name, data1)                                      \
  void name(struct Actor *object) {                                            \
    s32 phase = gFrameCount & 15;                                              \
                                                                               \
    if (phase == 0) {                                                          \
      struct Actor *child =                                                    \
          Import_ActorCreate_(0x10d, object->x, object->y, object->z);         \
                                                                               \
      if (child != 0) {                                                        \
        struct FieldSprite *sprite = child->sprite;                            \
                                                                               \
        Actor_StartScript(child, data1);                                       \
        child->motionFlags = 0;                                                \
        child->y = -0x78000;                                                   \
        child->terrainId = 1;                                                  \
        if (sprite != 0) {                                                     \
          Animation_ApplyChildArgument(sprite, 2);                             \
          sprite->nextFrame = 0;                                               \
          sprite->priority = 2;                                                \
        }                                                                      \
      }                                                                        \
    }                                                                          \
  }

/// 3 scenes: apojii_islands, aqua_rock, aqua_rock_inside.
#define DEFINE_FUNC_020013D8(name)                                             \
  s32 name(struct Actor *object) {                                             \
    s32 params[10];                                                            \
                                                                               \
    if (gFrameCount & 1) {                                                     \
      s32 drift;                                                               \
      s32 x;                                                                   \
      s32 y;                                                                   \
                                                                               \
      params[1] = 7;                                                           \
      if ((gFrameCount & 2) == 0)                                              \
        params[1] = 5;                                                         \
      params[2] = 0xcccc;                                                      \
      params[3] = 0xcccc;                                                      \
      params[0] = 0;                                                           \
      drift = ((u32)(Random16Far() << 3) >> 16) * 0x3333;                      \
      x = object->x;                                                           \
      y = object->y;                                                           \
      Func_0200015c(x + ((8 - (gFrameCount & 15)) << 16), y + 0x1a0000,        \
                    object->z, 0, -drift, 0, 0xb0000, params);                 \
    }                                                                          \
    return 0;                                                                  \
  }

/// 2 scenes: dehkan_plateau/s0bc0, madra_catacombs.
#define DEFINE_FUNC_02000038(name, call1, call2, call3)                        \
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

/// 4 scenes: dehkan_plateau/s0bc0, magma_rock_inside/s46e8, trial_road/s15a8,
/// tundaria_tower/s29d0.
#define DEFINE_FUNC_02000BC0(name, call1, call2)                               \
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

/// 3 scenes: jupiter_lighthouse_aerie_beacon/s44cc,
/// jupiter_lighthouse_room_1/s0674, mars_lighthouse_room_f.
#define DEFINE_FUNC_0200090C(name, call1)                                      \
  s32 name(struct Actor *object) {                                             \
    s32 params[10];                                                            \
                                                                               \
    if ((gFrameCount & 7) == 0) {                                              \
      s32 x;                                                                   \
      s32 y;                                                                   \
                                                                               \
      params[0] = 3;                                                           \
      params[2] = 0xb333;                                                      \
      params[3] = 0xb333;                                                      \
      params[1] = 14;                                                          \
      x = object->x + ((((Random16Far() * 9) >> 16) - 4) << 16);               \
      y = object->y + ((32 - ((Random16Far() * 20) >> 16)) << 16);             \
      call1(x, y, object->z, 0,                                                \
            (s32)((((Random16Far() * 5) >> 16) << 16) + 0x50000) / 10, 0,      \
            0xb0000, params);                                                  \
    }                                                                          \
    return 0;                                                                  \
  }

/// 3 scenes: airs_rock_inside/s2770, aqua_rock_inside, gaia_rock_inside/s07f4.
#define DEFINE_FUNC_02001548(name, call1)                                      \
  void name(struct Actor *object) {                                            \
    s32 phase = gFrameCount & 7;                                               \
                                                                               \
    if (phase == 0) {                                                          \
      struct EffectOptions options;                                            \
      s32 position[3];                                                         \
                                                                               \
      options.palette = 7;                                                     \
      options.type = 0x2a1;                                                    \
      options.priority = 1;                                                    \
      position[0] = object->x + (((Random16Far() & 31) - 16) << 16);           \
      position[1] = object->y + ((Random16Far() & 31) << 16) + 0x80000;        \
      position[2] = object->z;                                                 \
      call1(position[0], position[1], position[2] + 0x40000, 0, 0x8000, phase, \
            EFFECT_USE_TYPE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,        \
            &options);                                                         \
    }                                                                          \
  }

/// 3 scenes: magma_rock_inside, mars_lighthouse_entrance/calls, treasure_isle.
#define DEFINE_FUNC_02000168(name)                                             \
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

/// 2 scenes: apojii_islands, aqua_rock_inside.
#define DEFINE_FUNC_02001500(name, call1)                                      \
  void name(void) {                                                            \
    struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);           \
    s32 flag = Event_GetFlag(0x109);                                           \
    s32 *owner = &gMainRoutineWork.currentOwner;                               \
    s32 offset;                                                                \
                                                                               \
    if (flag == 0) {                                                           \
      Event_Start();                                                           \
      FieldEvent_ResolvePendingActions(0);                                     \
      Camera_MoveWithinMapBounds_(-1, -1, -1, 0);                              \
      leader->motionFlags = flag;                                              \
      Actor_SetPosition(*owner, ((s16 *)&leader->x)[1] << 16,                  \
                        (((s16 *)&leader->z)[1] << 16) - 0x100000);            \
      Actor_SetSpriteColor(Actor_Get(*owner), 9);                              \
      Actor_SetSingleChildFlags(Actor_Get(*owner), 0);                         \
      Event_StartScreenTransition();                                           \
      Event_PlaySound(228);                                                    \
      leader->update = (void (*)(void))call1;                                  \
      Actor_SetSpeed(*owner, 0x6666, 0x3333);                                  \
      Actor_WalkByAndWait(*owner, 0, 8);                                       \
      Actor_SetSpriteColor(Actor_Get(*owner), 0);                              \
      Actor_SetSingleChildFlags(Actor_Get(*owner), 1);                         \
      ((struct FieldSprite *)leader->sprite)->priority = 1;                    \
      offset = 10;                                                             \
      Actor_WalkByAndWait(*owner, 0, offset);                                  \
      leader->motionFlags = 3;                                                 \
      leader->update = (void (*)(void))flag;                                   \
      Event_NoOp();                                                            \
      Event_WaitTransitionFrames();                                            \
      Event_End();                                                             \
    }                                                                          \
  }

/// 3 scenes: gondowan_cliffs, staff_credit, world_map.
#define DEFINE_FUNC_02000038_58(name)                                          \
  s32 name(struct Actor *object) {                                             \
    u16 *tile = (u16 *)((u8 *)Ram_HeapSlots->mapWork + 286);                   \
    struct TileSprite *sprite = object->sprite;                                \
                                                                               \
    sprite->tile = *tile;                                                      \
    sprite->frame = 0;                                                         \
    return 1;                                                                  \
  }

/// 2 scenes: airs_rock, magma_rock_inside.
#define DEFINE_FUNC_020003E8(name, call1)                                      \
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

/// 2 scenes: staff_credit, world_map/s3b38.
#define DEFINE_FUNC_020011F4(name, call1)                                      \
  void name(struct Actor *object) {                                            \
    if ((gFrameCount & 2) != 0) {                                              \
      Animation_ApplyChildValues(object, 7);                                   \
    } else {                                                                   \
      Animation_ApplyChildValues(object, 0);                                   \
    }                                                                          \
    if ((gFrameCount & 15) == 0) {                                             \
      call1(object);                                                           \
    }                                                                          \
  }

/// 2 scenes: eastern_alhafra/s44c4, mars_lighthouse_room_f/s3d80.
#define DEFINE_FUNC_02004774(name, call1, data1, call2)                        \
  void name(struct Actor *object) {                                            \
    s32 position[3];                                                           \
    struct Actor *hit;                                                         \
    u32 i;                                                                     \
    s32 angle = object->facing >> 12;                                          \
                                                                               \
    angle = ((angle + 2) & 12) << 12;                                          \
    position[0] = object->x;                                                   \
    position[1] = object->y;                                                   \
    position[2] = object->z;                                                   \
    FieldVector_AddPolarOffset(0x100000, angle, position);                     \
    hit = call1(position, 1);                                                  \
    if (hit != 0) {                                                            \
      i = 0;                                                                   \
    next:                                                                      \
      if (*((struct ObjectAnimationState *)hit->sprite)->value28 == data1[i])  \
        return;                                                                \
      i++;                                                                     \
      if (i <= 5)                                                              \
        goto next;                                                             \
      FieldActor_SetPositionAndResetMotion(object, object->x, object->y,       \
                                           object->z);                         \
    }                                                                          \
    position[0] = object->x;                                                   \
    position[1] = object->y;                                                   \
    position[2] = object->z;                                                   \
    if (call2(object, position) > 0)                                           \
      FieldActor_SetPositionAndResetMotion(object, object->x, object->y,       \
                                           object->z);                         \
  }

/// 2 scenes: apojii_islands, aqua_rock_inside.
#define DEFINE_FUNC_02001338(name, call1, call2)                               \
  void name(s32 entrance) {                                                    \
    struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);           \
    s32 i;                                                                     \
                                                                               \
    Event_Start();                                                             \
    FieldEvent_ResolvePendingActions(0);                                       \
    FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);            \
    leader->motionFlags = 0;                                                   \
    leader->priorityFlags |= 0x20;                                             \
    Event_PlaySound(215);                                                      \
    call1(leader, 18);                                                         \
    Event_PlaySound(612);                                                      \
    for (i = 0; i < 60; i++) {                                                 \
      s32 angle;                                                               \
                                                                               \
      if (i == 30)                                                             \
        Event_FinishScreenTransition();                                        \
      leader->y -= 0x4000;                                                     \
      angle = leader->facing;                                                  \
      leader->facing = angle + 0x1000;                                         \
      if ((i & 7) == 0)                                                        \
        call2(Actor_Get(15));                                                  \
      System_Wait(1);                                                          \
    }                                                                          \
    Event_End();                                                               \
    FieldEvent_SetExitRequest(entrance);                                       \
  }

/// 2 scenes: eastern_alhafra/s44c4, gabomba_statue/s2394.
#define DEFINE_FUNC_02004868(name, call1)                                      \
  void name(u16 *list, s32 mode, s32 count) {                                  \
    struct GabombaStatueEffectWork *work = Ram_HeapSlots->effectWork;          \
                                                                               \
    while (*list != 0xffff) {                                                  \
      u32 entry = *list;                                                       \
      s32 id = work->objectId;                                                 \
                                                                               \
      if (id == entry) {                                                       \
        struct Actor *object = Actor_Get(id);                                  \
        s32 a;                                                                 \
        s32 flag;                                                              \
                                                                               \
        list++;                                                                \
        a = list[0];                                                           \
        flag = list[1];                                                        \
        if (count < 8)                                                         \
          object->scaleY -= 0x1f00;                                            \
        if (mode == 1) {                                                       \
          Event_SetFlag(flag);                                                 \
          call1(id, a, flag, object->terrainId);                               \
          object->scaleY = 0x10000;                                            \
        }                                                                      \
        break;                                                                 \
      }                                                                        \
      list += 3;                                                               \
    }                                                                          \
  }

/// 2 scenes: jupiter_lighthouse_room_5, lemuria_interiors/s12dc.
#define DEFINE_FUNC_020000C8(name)                                             \
  void name(struct Actor *object) {                                            \
    struct AnimatedSprite *sprite = (struct AnimatedSprite *)object->sprite;   \
    struct SpritePictures *pictures = sprite->pictures;                        \
                                                                               \
    switch (object->timer & 3) {                                               \
    case 0:                                                                    \
      pictures->picture = 7;                                                   \
      sprite->picture = 1;                                                     \
      sprite->nextPicture = 2;                                                 \
      break;                                                                   \
    case 1:                                                                    \
      sprite->picture = 1;                                                     \
      sprite->nextPicture = 0;                                                 \
      break;                                                                   \
    case 2:                                                                    \
      pictures->picture = 7;                                                   \
      sprite->picture = 1;                                                     \
      sprite->nextPicture = 0;                                                 \
      break;                                                                   \
    case 3:                                                                    \
      pictures->picture = 0;                                                   \
      sprite->picture = 1;                                                     \
      sprite->nextPicture = 0;                                                 \
      break;                                                                   \
    }                                                                          \
    object->timer++;                                                           \
  }

/// 2 scenes: jupiter_lighthouse_room_7/s16d4, mars_lighthouse_entrance/s42f8.
#define DEFINE_FUNC_020019DC(name, call1)                                      \
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

/// 2 scenes: yampi_desert, yampi_desert_cave.
#define DEFINE_FUNC_02000BC0_69(name, call1)                                   \
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

/// 3 scenes: airs_rock_inside/s2770, aqua_rock_inside, gaia_rock_inside/s07f4.
#define DEFINE_FUNC_020014D0(name)                                             \
  s32 name(struct Actor *object) {                                             \
    struct Actor *target = object->watchedObject;                              \
    s32 steps;                                                                 \
    s32 from;                                                                  \
                                                                               \
    object->bounce = (target->x - object->x) / (steps = 10);                   \
    from = object->y;                                                          \
    object->unk4c = (target->y - from + 0x100000) / steps;                     \
    object->gravity = (target->z - object->z) / steps;                         \
    BYTE_AT(object, 98) = steps;                                               \
    return 0;                                                                  \
  }

/// 2 scenes: staff_credit, world_map/s3b38.
#define DEFINE_FUNC_02001364(name)                                             \
  void name(void) {                                                            \
    struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);           \
    s32 x = *(s16 *)((u8 *)object + 10);                                       \
    s32 z = *(s16 *)((u8 *)object + 18);                                       \
                                                                               \
    if (gFrameCount % 3 == 0) {                                                \
      switch ((u32)(Random16Far() << 2) >> 16) {                               \
      case 0:                                                                  \
        CameraMove((x << 16) - 0x10000, -1, (z << 16) + 0x10000, 1);           \
        break;                                                                 \
      case 1: {                                                                \
        Camera_MoveWithinMapBounds((x << 16) + 0x10000, -1,                    \
                                   (z << 16) - 0x10000, 1);                    \
        break;                                                                 \
      }                                                                        \
      case 2:                                                                  \
        CameraMove((x << 16) + 0x10000, -1, (z << 16) + 0x10000, 1);           \
        break;                                                                 \
      case 3:                                                                  \
        CameraMove((x << 16) - 0x10000, -1, (z << 16) - 0x10000, 1);           \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
  }

/// 2 scenes: magma_rock, magma_rock_inside.
#define DEFINE_FUNC_0200074C(name)                                             \
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

/// 2 scenes: apojii_islands, aqua_rock_inside.
#define DEFINE_FUNC_02001464(name, call1)                                      \
  void name(s32 entrance) {                                                    \
    struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);           \
    s32 *owner = &gMainRoutineWork.currentOwner;                               \
                                                                               \
    Event_Start();                                                             \
    FieldEvent_ResolvePendingActions(0);                                       \
    Event_PlaySound(228);                                                      \
    leader->update = (void (*)(void))call1;                                    \
    leader->motionFlags = 0;                                                   \
    leader->speedLimit = 0x3333;                                               \
    Actor_SetAnimation(*owner, 2);                                             \
    Actor_SetDestinationOffset(*owner, 0, -8);                                 \
    FieldActor_WaitForMoveById(*owner);                                        \
    Actor_SetSpriteColor(Actor_Get(*owner), 9);                                \
    Actor_SetSingleChildFlags(Actor_Get(*owner), 0);                           \
    leader->update = 0;                                                        \
    Event_FinishScreenTransition();                                            \
    Event_WaitTransitionFrames();                                              \
    FieldEvent_SetExitRequest(entrance);                                       \
    Event_End();                                                               \
  }

/// 3 scenes: contigo, lemuria, magma_rock_inside/s033c.
#define DEFINE_FUNC_02000038_75(name)                                          \
  s32 name(s32 *a, s32 *b) {                                                   \
    s32 dx = (*a++ - *b++) >> 16;                                              \
    s32 dy = (*a++ - *b++) >> 16;                                              \
    s32 dz = (*a - *b) >> 16;                                                  \
    s32 dxsq = dx * dx;                                                        \
    s32 dysq = dy * dy;                                                        \
    s32 dzsq = dz * dz;                                                        \
                                                                               \
    return Iwram_Sqrt(dxsq + dysq + dzsq);                                     \
  }

/// 2 scenes: eastern_alhafra_ship, lemuria_interiors/s12dc.
#define DEFINE_FUNC_02000438(name, call1)                                      \
  void name(s32 id) {                                                          \
    struct Actor *object = Actor_GetById(id);                                  \
                                                                               \
    call1(object->x + 0x80000, object->y + 0x1c0000, object->z, 0x1000, 15);   \
    Sound_Play(151);                                                           \
    Event_Wait(15);                                                            \
    call1(object->x - 0x80000, object->y + 0x1c0000, object->z, 0xf000, 15);   \
    Sound_Play(151);                                                           \
    Event_Wait(15);                                                            \
    call1(object->x + 0x80000, object->y + 0x1c0000, object->z, 0x1000, 15);   \
    Sound_Play(151);                                                           \
    Event_Wait(15);                                                            \
    call1(object->x - 0x80000, object->y + 0x1c0000, object->z, 0xf000, 15);   \
    Sound_Play(151);                                                           \
    Event_Wait(15);                                                            \
  }

/// 2 scenes: contigo, lemuria.
#define DEFINE_FUNC_02000070(name, call1)                                      \
  s32 name(struct Actor *self, struct Actor *other, s32 range) {               \
    s32 result = 0;                                                            \
    s32 *to;                                                                   \
    s32 *from;                                                                 \
                                                                               \
    to = &other->x;                                                            \
    from = &self->x;                                                           \
    if (call1(to, from) < range) {                                             \
      u32 angle = (u16)ArcTan2(other->z - self->z, *to - *from);               \
      u32 left = (angle - 0x1000) & 0xf000;                                    \
      u32 right = (angle + 0x1000) & 0xf000;                                   \
      u32 ahead = angle & 0xf000;                                              \
      u32 facing = self->facing & 0xf000;                                      \
                                                                               \
      if (ahead == facing || right == facing || left == facing)                \
        result = 1;                                                            \
    }                                                                          \
    return result;                                                             \
  }

/// 2 scenes: staff_credit, world_map/s3b38.
#define DEFINE_FUNC_02001A34(name, call1)                                      \
  void name(struct Actor *object) {                                            \
    struct Actor *parent = object->watchedObject;                              \
    s32 step = ++object->timer;                                                \
    s32 scale;                                                                 \
    s32 rise;                                                                  \
                                                                               \
    if (step > 31) {                                                           \
      call1(object);                                                           \
      return;                                                                  \
    }                                                                          \
    scale = Math_Sin(step << 10);                                              \
    object->scaleX = scale;                                                    \
    object->scaleY = scale;                                                    \
                                                                               \
    rise = 0x10000;                                                            \
    object->x = parent->x;                                                     \
    object->y += rise;                                                         \
    object->z = parent->z + (rise - scale) * 5 + 0x80000;                      \
  }

/// 2 scenes: staff_credit, world_map/s3b38.
#define DEFINE_FUNC_02001A80(name, call1)                                      \
  void name(struct Actor *object) {                                            \
    struct Actor *parent = object->watchedObject;                              \
    s32 step = ++object->timer;                                                \
    s32 scale;                                                                 \
    s32 rise;                                                                  \
                                                                               \
    if (step > 31) {                                                           \
      call1(object);                                                           \
      return;                                                                  \
    }                                                                          \
    scale = Math_Sin(step << 10);                                              \
    object->scaleX = scale;                                                    \
    object->scaleY = -scale;                                                   \
                                                                               \
    rise = 0x10000;                                                            \
    object->x = parent->x;                                                     \
    object->y += rise;                                                         \
    object->z = parent->z - (rise - scale) * 5 + 0x100000;                     \
  }

/// 3 scenes: airs_rock_inside/s2770, aqua_rock_inside, gaia_rock_inside/s07f4.
#define DEFINE_FUNC_0200151C(name)                                             \
  s32 name(struct Actor *object) {                                             \
    u8 left;                                                                   \
                                                                               \
    object->x += object->bounce;                                               \
    object->y += object->unk4c;                                                \
    object->z += object->gravity;                                              \
    left = --BYTE_AT(object, 98);                                              \
    return left != 0;                                                          \
  }

/// 2 scenes: aqua_rock_inside, magma_rock_inside.
#define DEFINE_FUNC_0200147C(name)                                             \
  s32 name(struct Orbit *object) {                                             \
    s32 z;                                                                     \
                                                                               \
    object->angle += object->speed;                                            \
    object->x = IwramMulQ16(Math_Cosine(object->angle), object->radius);       \
    z = IwramMulQ16(Math_Sin(object->angle), object->radius);                  \
    object->x += object->centreX;                                              \
    object->z = (z >> 1) + object->centreZ;                                    \
    if (object->growth != 141 && (gFrameCount & 1)) {                          \
      object->radius = object->growth * Math_Sin(object->phase << 10);         \
      object->growth += 10;                                                    \
    }                                                                          \
    if (object->speed == 0)                                                    \
      return 0;                                                                \
    if (object->growth == 141)                                                 \
      object->speed -= 128;                                                    \
    return 1;                                                                  \
  }

/// 4 scenes: dehkan_plateau, kibombo/s2f34, shrine_of_the_sea_god/s1340,
/// yampi_desert.
#define DEFINE_FUNC_02001F38(name, call1)                                      \
  void name(s32 id) {                                                          \
    call1(id);                                                                 \
    Actor_SetPartAttribute(Actor_GetById(id), 15);                             \
    Actor_SetSpritePriority(id, 1);                                            \
    Actor_GetById(id)->priorityFlags |= 2;                                     \
  }

/// 3 scenes: jupiter_lighthouse_aerie_beacon/s44cc,
/// jupiter_lighthouse_room_6/s0854, jupiter_lighthouse_room_7/s16d4.
#define DEFINE_FUNC_02000EA0(name, data1, data2)                               \
  void name(s32 id, s32 value) {                                               \
    struct Actor *object = Actor_Get(id);                                      \
                                                                               \
    if (object != NULL) {                                                      \
      object->timer = value;                                                   \
      data1[data2] = object;                                                   \
      data2++;                                                                 \
    }                                                                          \
  }

/// 4 scenes: mars_lighthouse_entrance/s263c, mars_lighthouse_room_f/s328c,
/// mars_lighthouse_room_m/s2708, mars_lighthouse_star_room/s2148.
#define DEFINE_FUNC_02002CE8(name, call1)                                      \
  s32 name(struct Actor *object) {                                             \
    call1(object);                                                             \
    SET_POSITION(object, 0, 0, 0);                                             \
    SET_VELOCITY(object, 0, 0, 0);                                             \
    return 0;                                                                  \
  }

/// 3 scenes: mars_lighthouse_room_f/ride, mars_lighthouse_room_m/s2118,
/// mars_lighthouse_star_room/ride_handle.
#define DEFINE_FUNC_02001F50(name)                                             \
  s32 name(s32 limit) {                                                        \
    struct RideWork *work = (struct RideWork *)(gData_020023c4 + 0x188);       \
                                                                               \
    if (!Event_GetFlag(260)) {                                                 \
      if (++work->counter >= limit)                                            \
        work->counter = 0;                                                     \
    }                                                                          \
    return work->counter;                                                      \
  }

/// 2 scenes: aqua_rock_inside/s3844, taopo_swamp.
#define DEFINE_FUNC_020025D4(name, call1)                                      \
  void name(void) {                                                            \
    struct VentWork *work = VENT_WORK;                                         \
    struct VentEntry *entry = work->entries;                                   \
    s32 i;                                                                     \
                                                                               \
    work->hook();                                                              \
    for (i = 15; i >= 0; i--, entry++) {                                       \
      if (entry->active != 0) {                                                \
        if (entry->timer <= 0) {                                               \
          call1(&entry->x, entry->value);                                      \
          entry->timer = 9;                                                    \
        } else {                                                               \
          entry->timer--;                                                      \
        }                                                                      \
      }                                                                        \
    }                                                                          \
  }

/// 2 scenes: airs_rock, magma_rock/s22ac.
#define DEFINE_FUNC_0200244C(name)                                             \
  void name(s32 height) {                                                      \
    struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);           \
                                                                               \
    if (!Event_GetFlag(0x109)) {                                               \
      object->motionFlags = 0;                                                 \
      Actor_SetSingleChildFlags(object, 0);                                    \
      object->y += height;                                                     \
      object->speedLimit = 0x10000;                                            \
      object->velocityY = 0;                                                   \
      gMainRoutineWork.renderMode = 2;                                         \
      object->actionFlags = 1;                                                 \
      FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 0);               \
      Map_CenterOnFollowedPoint();                                             \
      System_Wait(1);                                                          \
    }                                                                          \
  }

/// 2 scenes: mars_lighthouse_entrance, mars_lighthouse_room_f.
#define DEFINE_FUNC_0200206C(name, call1, call2, call3)                        \
  void name(s32 *position, s32 speed) {                                        \
    struct Actor *object =                                                     \
        call1(285, position[0], position[1], position[2] + 0xfffe0000);        \
    s32 spin;                                                                  \
                                                                               \
    if (object != NULL) {                                                      \
      object->bounce = Math_Cosine((u16)(Random16Far() * 3 >> 4) + 0x3000);    \
      object->gravity = (u16)Random16Far();                                    \
      object->unk4c = (u16)(Random16Far() << 1) + speed;                       \
      spin = (u16)Random16Far() + 0xffff8000;                                  \
      object->timer = spin;                                                    \
      call2(object, 2);                                                        \
      object->update = (void (*)(void))call3;                                  \
      Animation_SetStateFlags(object, 1);                                      \
    }                                                                          \
  }

/// 2 scenes: izumo_ruins/s0974, magma_rock/s34ac.
#define DEFINE_FUNC_02000BC0_89(name, call1, call2)                            \
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

/// 2 scenes: kibombo_mountains, settlements/s0280.
#define DEFINE_FUNC_02000058(name)                                             \
  s32 name(struct Actor *object) {                                             \
    s32 step = object->riseCounter & 63;                                       \
    struct FieldSprite *sprite = object->sprite;                               \
                                                                               \
    object->motionFlags = 3;                                                   \
    if (step == 0)                                                             \
      object->velocityY = PIXELS(3);                                           \
    if (step == 16)                                                            \
      object->velocityY = PIXELS(2);                                           \
    if (step == 24)                                                            \
      object->velocityY = PIXELS(3);                                           \
    sprite->rotation = Math_Cosine(step << 12) / 64;                           \
    object->riseCounter++;                                                     \
    return -1;                                                                 \
  }

/// 2 scenes: mars_lighthouse_entrance/s263c, mars_lighthouse_room_f/s328c.
#define DEFINE_FUNC_02002F4C(name, call1, call2)                               \
  void name(s32 id, s32 target, s32 kind, s32 a3, const void *script) {        \
    struct Actor *source = Actor_Get(id);                                      \
    struct Actor *object = Actor_Get(target);                                  \
                                                                               \
    FieldActor_SetPositionAndResetMotion(object, source->x, source->y,         \
                                         source->z);                           \
    Actor_StartScript(object, script);                                         \
    Actor_SetSingleChildFlags(object, 0);                                      \
    object->motionFlags = 0;                                                   \
    call1(object, 1);                                                          \
    object->watchedObject = source;                                            \
    object->timer = 0;                                                         \
    object->unk66 = a3;                                                        \
    BYTE_AT(object, 98) = kind;                                                \
    object->update = call2;                                                    \
  }

/// 2 scenes: staff_credit, world_map/s3b38.
#define DEFINE_FUNC_020012AC(name, call1, call2)                               \
  void name(void) {                                                            \
    struct Actor *object = Actor_Get(FieldParty_GetLeaderActor());             \
    s32 i;                                                                     \
                                                                               \
    Event_PlaySound(212);                                                      \
    call1(object, 1);                                                          \
    for (i = 0; i < 20; i++) {                                                 \
      s32 hit = call2(&object->x);                                             \
                                                                               \
      if (hit != 0 && SceneEvent_FindActiveRecord(2, hit) != 0)                \
        break;                                                                 \
      System_Wait(1);                                                          \
    }                                                                          \
    call1(object, 2);                                                          \
  }

/// 2 scenes: apojii_islands/object, aqua_rock_inside/s00d0.
#define DEFINE_FUNC_020000D0_93()                                              \
  struct Actor *Func_020000d0(s32 x, s32 y, s32 z, s32 kind) {                 \
    struct Actor *object = FieldActor_Create(kind, x, y, z);                   \
                                                                               \
    if (object != 0) {                                                         \
      Scene_SetActorSpritePriority(object, 1);                                 \
      object->motionFlags = 0;                                                 \
      object->collisionFlags = 8;                                              \
      Actor_SetSingleChildFlags(object, 0);                                    \
      Actor_SetSpriteColor(object, 15);                                        \
      object->priorityFlags |= 0x22;                                           \
      return object;                                                           \
    }                                                                          \
    return 0;                                                                  \
  }

/// 2 scenes: kibombo_mountains, taopo_swamp.
#define DEFINE_FUNC_0200015C(name)                                             \
  void name(s32 id) {                                                          \
    s32 n = 60;                                                                \
    struct Actor *object = Actor_Get(id);                                      \
                                                                               \
    object->motionFlags = 3;                                                   \
    while (n != 0) {                                                           \
      System_Wait(1);                                                          \
      if (object->velocityY == 0)                                              \
        break;                                                                 \
      n--;                                                                     \
    }                                                                          \
    System_Wait(10);                                                           \
    object->motionFlags = 0;                                                   \
  }

/// 3 scenes: mars_lighthouse_room_f/flash, mars_lighthouse_room_m/s2708,
/// mars_lighthouse_star_room/s2148.
#define DEFINE_FUNC_020024AC(name, call1)                                      \
  s32 name(void) {                                                             \
    if (((struct RideWork *)(gData_020023c4 + 0x188))->active) {               \
      call1();                                                                 \
      return 1;                                                                \
    }                                                                          \
    return 0;                                                                  \
  }

/// 3 scenes: mars_lighthouse_room_f/ride, mars_lighthouse_room_m/s2118,
/// mars_lighthouse_star_room/ride_handle.
#define DEFINE_FUNC_02001F78(name)                                             \
  void name(s32 index, s32 value) {                                            \
    struct RideWork *work = (struct RideWork *)(gData_020023c4 + 0x188);       \
    struct RideEntry *entry = &work->entries[index];                           \
                                                                               \
    entry->d = (value & 15) << 16;                                             \
    entry->b = value & 0xf0;                                                   \
  }

/// 1 scenes: mars_lighthouse_star_room.
#define DEFINE_FUNC_0200196C_97(name, call1)                                   \
  void name(s32 *position, s32 speed, s32 flag) {                              \
    struct Actor *object =                                                     \
        FieldActor_Create(366, position[0], position[1], position[2]);         \
                                                                               \
    if (object != NULL) {                                                      \
      object->bounce = 0;                                                      \
      object->gravity = 0;                                                     \
      object->unk4c = -speed;                                                  \
      Func_02001928(object, 3);                                                \
      object->update = call1;                                                  \
      if (flag != 0)                                                           \
        object->unk5c[0] = 2;                                                  \
      Func_020041cc(object, 2);                                                \
    }                                                                          \
  }

/// 2 scenes: aqua_rock_inside/s3844, taopo_swamp.
#define DEFINE_FUNC_020024F8(name, call1, data1)                               \
  void name(struct Actor *object, s32 priority) {                              \
    object->motionFlags = 0;                                                   \
    object->collisionFlags = 0;                                                \
    object->priorityFlags = 2;                                                 \
    ((struct FieldSprite *)object->sprite)->priority = priority;               \
    Actor_SetSingleChildFlags(object, 0);                                      \
    call1(object, 2);                                                          \
    Actor_StartScript(object, data1);                                          \
    Actor_SetSpriteColor(object, 10);                                          \
    object->timer = 0;                                                         \
    object->speedLimit = 0;                                                    \
    object->acceleration = 0;                                                  \
  }

/// 2 scenes: magma_rock_inside, mars_lighthouse_entrance/calls.
#define DEFINE_FUNC_0200012C(name)                                             \
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

/// 2 scenes: apojii_islands, aqua_rock_inside.
#define DEFINE_FUNC_02001308(name, call1)                                      \
  void name(s32 id) {                                                          \
    struct Actor *object = Actor_Get(id);                                      \
                                                                               \
    if (object != 0) {                                                         \
      u8 *meta = ResourceMetadata_Register(object->sprite, 0x17d);             \
                                                                               \
      meta[5] = 0;                                                             \
      meta[6] = 0;                                                             \
      call1(object, 0);                                                        \
      call1(object, 2);                                                        \
    }                                                                          \
  }

/// 2 scenes: daila, daila_interiors.
#define DEFINE_FUNC_02000704(name, call1, data1)                               \
  void name(void) {                                                            \
    u8 *event = Ram_HeapSlots->eventWork;                                      \
    struct Actor *object;                                                      \
    s32 flags;                                                                 \
                                                                               \
    call1(data1);                                                              \
    object = *(struct Actor **)(event + 0xcbc);                                \
    flags = 1;                                                                 \
    flags |= object->priorityFlags;                                            \
    flags &= 0xfd;                                                             \
    object->priorityFlags = flags;                                             \
  }

/// 2 scenes: dehkan_plateau, yampi_desert.
#define DEFINE_FUNC_02001514(name)                                             \
  void name(s32 show, s32 first, u32 count) {                                  \
    u32 i;                                                                     \
                                                                               \
    for (i = 0; i < count; i++) {                                              \
      struct Actor *actor = Actor_GetById(first + i);                          \
                                                                               \
      if (show)                                                                \
        actor->priorityFlags &= ~0x10;                                         \
      else                                                                     \
        actor->priorityFlags |= 0x10;                                          \
    }                                                                          \
  }

/// 2 scenes: magma_rock, magma_rock_inside.
#define DEFINE_FUNC_02000668(name, call1)                                      \
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

/// 3 scenes: mars_lighthouse_room_f/ride, mars_lighthouse_room_m/s2118,
/// mars_lighthouse_star_room/ride_handle.
#define DEFINE_FUNC_02001FB8(name)                                             \
  void name(u32 index, s32 value) {                                            \
    struct RideWork *work = (struct RideWork *)(gData_020023c4 + 0x188);       \
                                                                               \
    if (index <= 3)                                                            \
      work->slots[index].value = value;                                        \
  }

/// 2 scenes: staff_credit, world_map/s3b38.
#define DEFINE_FUNC_02001274(name, call1, call2)                               \
  void name(struct Actor *object, s32 mode) {                                  \
    if (mode == 0) {                                                           \
      object->update = 0;                                                      \
      Animation_ApplyChildValues(object, 0);                                   \
      Event_PlaySound(390);                                                    \
    } else if (mode == 1) {                                                    \
      object->update = (void (*)(void))call1;                                  \
    } else {                                                                   \
      object->update = (void (*)(void))call2;                                  \
      Event_PlaySound(291);                                                    \
    }                                                                          \
  }

/// 2 scenes: dehkan_plateau, madra_catacombs.
#define DEFINE_FUNC_020004A4(name)                                             \
  void name(struct Actor *object) {                                            \
    u8 *flags = &object->motionFlags;                                          \
                                                                               \
    *flags = 3;                                                                \
    do {                                                                       \
      wait(1);                                                                 \
    } while (object->velocityY != 0);                                          \
    Sound_Play(188);                                                           \
    wait(10);                                                                  \
    *flags = 0;                                                                \
  }

/// 2 scenes: mars_lighthouse_entrance, mars_lighthouse_room_f.
#define DEFINE_FUNC_0200202C(name, call1, data1)                               \
  void name(struct Actor *object, s32 unused) {                                \
    object->motionFlags = 0;                                                   \
    object->collisionFlags = 0;                                                \
    Actor_SetSingleChildFlags(object, 0);                                      \
    call1(object, 2);                                                          \
    Actor_StartScript(object, data1);                                          \
    Actor_SetSpriteColor(object, 10);                                          \
    object->timer = 0;                                                         \
    object->speedLimit = 0;                                                    \
    object->acceleration = 0;                                                  \
  }

/// 2 scenes: mars_lighthouse_room_m, mars_lighthouse_star_room.
#define DEFINE_FUNC_02001B38(name, call1, data1)                               \
  void name(struct Actor *object) {                                            \
    object->motionFlags = 0;                                                   \
    object->collisionFlags = 0;                                                \
    Actor_SetSingleChildFlags(object, 0);                                      \
    call1(object, 2);                                                          \
    Actor_StartScript(object, data1);                                          \
    Actor_SetSpriteColor(object, 10);                                          \
    object->timer = 0;                                                         \
    object->speedLimit = 0;                                                    \
    object->acceleration = 0;                                                  \
  }

/// 2 scenes: contigo, kibombo.
#define DEFINE_FUNC_020002E8(name)                                             \
  s32 name(void) {                                                             \
    u16 turn;                                                                  \
    s32 facing;                                                                \
                                                                               \
    turn = Actor_GetById(gMainRoutineWork.currentOwner)->facing + 0x5fff;      \
    facing = 1;                                                                \
    if (turn >= 0x3fff)                                                        \
      facing = 0;                                                              \
    return facing;                                                             \
  }

/// 2 scenes: dehkan_plateau, madra_catacombs.
#define DEFINE_FUNC_020008F4(name, call1, call2, call3, data1, call4, call5)   \
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

/// 2 scenes: islets/scene_tables, prox.
#define DEFINE_FUNC_020003DC(name)                                             \
  s32 name(void) {                                                             \
    u16 turn;                                                                  \
    s32 facing;                                                                \
                                                                               \
    turn = Actor_Get(gMainRoutineWork.currentOwner)->facing + 0x5fff;          \
    facing = 1;                                                                \
    if (turn >= 0x3fff)                                                        \
      facing = 0;                                                              \
    return facing;                                                             \
  }

/// 2 scenes: magma_rock, magma_rock_inside.
#define DEFINE_FUNC_020005CC(name, call1, call2)                               \
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

/// 2 scenes: mars_lighthouse_entrance, mars_lighthouse_room_f.
#define DEFINE_FUNC_02001A80_117(name)                                         \
  s32 name(struct Actor *a, struct Actor *b) {                                 \
    s32 dy;                                                                    \
                                                                               \
    if (ABS(a->x - b->x) < 0xc0000) {                                          \
      dy = a->y - b->y;                                                        \
      if (dy > -0x80000 && dy < 0x180000 && ABS(a->z - b->z) < 0xc0000)        \
        return 1;                                                              \
    }                                                                          \
    return 0;                                                                  \
  }

/// 2 scenes: mars_lighthouse_entrance, mars_lighthouse_room_f.
#define DEFINE_FUNC_02001F38_118(name)                                         \
  s32 name(struct Actor *a, struct Actor *b) {                                 \
    s32 dy;                                                                    \
                                                                               \
    if (ABS(a->x - b->x) < 0xc0000) {                                          \
      dy = a->y - b->y;                                                        \
      if (dy > 0 && dy < 0x100000 && ABS(a->z - b->z) < 0x80000)               \
        return 1;                                                              \
    }                                                                          \
    return 0;                                                                  \
  }

/// 2 scenes: mars_lighthouse_entrance/s263c, mars_lighthouse_room_f/s3d80.
#define DEFINE_FUNC_02003284(name)                                             \
  void name(void) {                                                            \
    u8 *event = Ram_HeapSlots->eventWork;                                      \
                                                                               \
    if (S16_AT(event, 0xca2)) {                                                \
      Flag_Set(0x106);                                                         \
      Leader_StopMoving();                                                     \
      Event_ResolveAllPendingActions();                                        \
      Flag_Clear(0x106);                                                       \
    }                                                                          \
  }

/// 2 scenes: aqua_rock_inside/s3844, taopo_swamp.
#define DEFINE_FUNC_0200273C(name)                                             \
  s32 name(s32 limit) {                                                        \
    struct VentWork *work = VENT_WORK;                                         \
                                                                               \
    if (Event_GetFlag(0x104) == 0) {                                           \
      if (++work->counter >= limit)                                            \
        work->counter = 0;                                                     \
    }                                                                          \
    return work->counter;                                                      \
  }

/// 2 scenes: mars_lighthouse_entrance, mars_lighthouse_star_room.
#define DEFINE_FUNC_020002D4(name)                                             \
  void name(s32 id) {                                                          \
    struct Actor *object = Actor_Get(id);                                      \
                                                                               \
    if (object != NULL) {                                                      \
      Actor_SetSingleChildFlags(object, 0);                                    \
      object->collisionFlags = 0;                                              \
      object->motionFlags = 0;                                                 \
    }                                                                          \
  }

/// 2 scenes: mars_lighthouse_entrance, mars_lighthouse_room_f.
#define DEFINE_FUNC_02001C44(name, data1)                                      \
  void name(struct Actor *object, s32 unused) {                                \
    object->motionFlags = 0;                                                   \
    object->collisionFlags = 0;                                                \
    Actor_SetSingleChildFlags(object, 0);                                      \
    Actor_StartScript(object, data1);                                          \
    object->timer = 0;                                                         \
    object->speedLimit = 0;                                                    \
    object->acceleration = 0;                                                  \
  }

/// 2 scenes: mars_lighthouse_room_m, mars_lighthouse_star_room.
#define DEFINE_FUNC_02001750(name, data1)                                      \
  void name(struct Actor *object) {                                            \
    object->motionFlags = 0;                                                   \
    object->collisionFlags = 0;                                                \
    Actor_SetSingleChildFlags(object, 0);                                      \
    Actor_StartScript(object, data1);                                          \
    object->timer = 0;                                                         \
    object->speedLimit = 0;                                                    \
    object->acceleration = 0;                                                  \
  }

/// 2 scenes: staff_credit, world_map/s3b38.
#define DEFINE_FUNC_0200122C(name, data1, data2, call1)                        \
  void name(struct Actor *object) {                                            \
    if (gFrameCount & 1) {                                                     \
      Animation_ApplyChildValues(object, data1[data2]);                        \
      data2 = (data2 + 1) & 3;                                                 \
    }                                                                          \
    if (!(gFrameCount & 3)) {                                                  \
      call1(object);                                                           \
    }                                                                          \
  }

/// 1 scenes: trial_road/s0218.
#define DEFINE_FUNC_02000728(name, call1)                                      \
  void name(s32 a, s32 id) {                                                   \
    struct MainRoutineWork *party = &gMainRoutineWork;                         \
    u8 mode;                                                                   \
                                                                               \
    mode = party->renderMode;                                                  \
                                                                               \
    if (mode == 0)                                                             \
      call1(id);                                                               \
  }

/// 2 scenes: apojii_islands, settlements/s0280.
#define DEFINE_FUNC_02000E90(name, data1, data2, data3)                        \
  void name(void) {                                                            \
    struct MapLayer *layer = &Ram_HeapSlots->mapWork->layers[2];               \
    s16 y = layer->y >> 16;                                                    \
                                                                               \
    data1 = 1448 - y;                                                          \
    data2 = layer->x >> 16;                                                    \
    data3 = (s16)(layer->x >> 16) - (gFrameCount >> 2);                        \
  }

/// 2 scenes: staff_credit, world_map/s3b38.
#define DEFINE_FUNC_020019D4(name, call1)                                      \
  void name(struct Actor *object) {                                            \
    u32 frame = gFrameCount;                                                   \
                                                                               \
    if (frame & 1)                                                             \
      Animation_ApplyChildValues(object, (frame >> 1) % 6);                    \
    if ((gFrameCount & 15) == 0)                                               \
      call1(object);                                                           \
  }

/// 2 scenes: staff_credit, world_map/s3b38.
#define DEFINE_FUNC_02001E58(name, data1, data2, data3, data4, call1)          \
  void name(void) {                                                            \
    Resource_DecodeType01(gWorldMapPackedTilesC, data1);                       \
    data2 = Resource_FindFreeEntry();                                          \
    Vram_Load(data2, 0x100, 0);                                                \
    data3 = -1;                                                                \
    data4 = 10;                                                                \
    Task_AddFrame(call1, TASK_PASS_VBLANK - 10);                               \
  }

/// 2 scenes: apojii_islands, settlements/s0280.
#define DEFINE_FUNC_02000ED0(name, call1, call2)                               \
  void name(void) {                                                            \
    s32 priority;                                                              \
                                                                               \
    Irq_SetHandler(1, 0, call1);                                               \
    priority = 0xc80;                                                          \
    FieldTask_AddFrame(call2, priority);                                       \
  }

/// 1 scenes: yampi_desert/placements.
#define DEFINE_FUNC_02000648(name, data1)                                      \
  void name(s32 a, s32 b) {                                                    \
    Func_0200388c(data1, a, b);                                                \
    if (a == 1) {                                                              \
      if (gData_02004808 == 0)                                                 \
        gData_02004808 = 210;                                                  \
    }                                                                          \
  }

/// 2 scenes: airs_rock, airs_rock_inside.
#define DEFINE_FUNC_02000D18(name)                                             \
  s32 name(s32 *a, s32 *b) {                                                   \
    s32 dx = (*a++ - *b++) >> 16;                                              \
    s32 dy = (*a - *b + 0x60000) >> 16;                                        \
                                                                               \
    return Iwram_Sqrt(dx * dx + dy * dy);                                      \
  }

/// 2 scenes: aqua_rock_inside/s3844, taopo_swamp.
#define DEFINE_FUNC_02002778(name)                                             \
  void name(s32 i, s32 id) {                                                   \
    struct VentWork *work = VENT_WORK;                                         \
                                                                               \
    if ((u32)i <= 3)                                                           \
      work->ids[i] = id;                                                       \
  }

/// 2 scenes: champa_interiors, daila_interiors.
#define DEFINE_FUNC_0200014C(name)                                             \
  void name(void) {                                                            \
    struct EventWork *work = Ram_HeapSlots->eventWork;                         \
                                                                               \
    work->screenEffect = 0x209;                                                \
    GameFlag_ClearBit(0x12f);                                                  \
  }

/// 2 scenes: contigo, shaman_village/s00b8.
#define DEFINE_FUNC_0200098C(name)                                             \
  void name(void) {                                                            \
    Event_Start();                                                             \
    Event_ResolvePendingActions(0);                                            \
    FieldMessage_Show((s32)MsgShamanEntranceLocked, 1);                        \
    Event_End();                                                               \
  }

/// 2 scenes: mars_lighthouse_entrance, mars_lighthouse_room_f.
#define DEFINE_FUNC_02001A1C(name)                                             \
  s32 name(struct Actor *a, struct Actor *b) {                                 \
    if (ABS(a->x - b->x) < 0x80000 && ABS(a->y - b->y) < 0x80000 &&            \
        ABS(a->z - b->z) < 0x80000)                                            \
      return 1;                                                                \
    return 0;                                                                  \
  }

/// 2 scenes: mars_lighthouse_room_m/s2118,
/// mars_lighthouse_star_room/ride_handle.
#define DEFINE_FUNC_02001DD0(name, call1)                                      \
  void name(void) {                                                            \
    struct RideWork *work = (struct RideWork *)(gData_020023c4 + 0x188);       \
                                                                               \
    call1(work->handle);                                                       \
    work->handle = NULL;                                                       \
  }

/// 2 scenes: staff_credit, world_map/s3b38.
#define DEFINE_FUNC_02001A0C(name)                                             \
  void name(struct Actor *object) {                                            \
    u32 frame = gFrameCount;                                                   \
                                                                               \
    if (frame & 1)                                                             \
      Animation_ApplyChildValues(object, (frame >> 1) % 6);                    \
  }

/// 2 scenes: staff_credit, world_map/s3b38.
#define DEFINE_FUNC_02001EB8(name, data1, data2, data3, call1)                 \
  void name(void) {                                                            \
    Resource_DecodeType01(gWorldMapPackedTilesC, data1);                       \
    Resource_ResetEntry(data2);                                                \
    data3 = -1;                                                                \
    FieldTask_Remove(call1);                                                   \
  }

#endif
