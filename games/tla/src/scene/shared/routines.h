/// @file
/// Routines several scenes' overlays each keep a copy of, written once: a
/// scene expands the macro where its copy sits. The placeholder names are the
/// first copy's.
#ifndef ALCHEMY_SCENE_SHARED_ROUTINES_H
#define ALCHEMY_SCENE_SHARED_ROUTINES_H

/// 12 copies: anemos_inner_sanctum/move, jupiter_lighthouse_aerie/s1e78,
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
    table->tile = Resource_FindFreeEntry();                                    \
    Vram_Load(table->tile, 0x100, buffer);                                     \
    Heap_Release(buffer);                                                      \
    Task_AddFrame(call1, 0x480);                                               \
    return table->count;                                                       \
  }

/// 14 copies: jupiter_lighthouse_aerie/s17a8,
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

/// 12 copies: jupiter_lighthouse_aerie, jupiter_lighthouse_basement/s098c,
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

/// 16 copies: anemos_inner_sanctum/s2f48, jupiter_lighthouse_aerie/s2640,
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

/// 5 copies: jupiter_lighthouse_room_4, jupiter_lighthouse_room_5,
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

/// 6 copies: jupiter_lighthouse_aerie/s2640, jupiter_lighthouse_room_2/s1e24,
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

/// 15 copies: jupiter_lighthouse_aerie/s1e78,
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

/// 5 copies: jupiter_lighthouse_basement, jupiter_lighthouse_entrance,
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

/// 9 copies: jupiter_lighthouse_aerie, jupiter_lighthouse_basement/s098c,
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

/// 11 copies: anemos_inner_sanctum/s2f48,
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

/// 5 copies: jupiter_lighthouse_room_4/s1a20, jupiter_lighthouse_room_5/s2450,
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

/// 7 copies: jupiter_lighthouse_basement/s16c4,
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

/// 16 copies: anemos_inner_sanctum_entrance, jupiter_lighthouse_aerie/s17a8,
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
    Iwram_ClearWordsEntry(data1, 64);                                          \
    Resource_DecodeType01(gField_CachedTilesA, buffer);                        \
    data2 = Resource_FindFreeEntry();                                          \
    VramCache_Load(data2, 0xc00, buffer);                                      \
    Heap_Release(buffer);                                                      \
    Task_Add(call1, TASK_PASS_VBLANK);                                         \
    data3 = 0;                                                                 \
    data4 = 0;                                                                 \
    data5 = -1;                                                                \
  }

/// 14 copies: jupiter_lighthouse_aerie, jupiter_lighthouse_aerie_beacon/s0198,
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

/// 15 copies: jupiter_lighthouse_aerie/s17a8,
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
    Iwram_ClearWordsEntry(data1, 64);                                          \
    Resource_DecodeType01(gField_CachedTilesB, buffer);                        \
    data2 = Resource_FindFreeEntry();                                          \
    Vram_Load(data2, 0x800, buffer);                                           \
    Heap_Release(buffer);                                                      \
    Task_AddFrame(call1, TASK_PASS_VBLANK);                                    \
    data3 = 0;                                                                 \
    data4 = 0;                                                                 \
    data5 = -1;                                                                \
  }

/// 15 copies: jupiter_lighthouse_aerie/s17a8,
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
    Iwram_ClearWordsEntry(data1, 64);                                          \
    Resource_DecodeType01(gField_CachedTilesC, buffer);                        \
    data2 = Resource_FindFreeEntry();                                          \
    Vram_Load(data2, 0x800, buffer);                                           \
    Heap_Release(buffer);                                                      \
    Task_AddFrame(call1, TASK_PASS_VBLANK);                                    \
    data3 = 0;                                                                 \
    data4 = 1;                                                                 \
    data5 = -1;                                                                \
  }

/// 12 copies: jupiter_lighthouse_aerie_beacon/s0198,
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
      Iwram_CopyWordsEntry(data2, (void *)&BG_PLTT_COLOR(12, 0), 32);          \
      Iwram_CopyWordsEntry(data3, (void *)&BG_PLTT_COLOR(13, 0), 32);          \
    }                                                                          \
    if (Event_GetFlag(0xa00) || mode == 1) {                                   \
      Counter_Set(0);                                                          \
      Task_Add(call1, 1152);                                                   \
    } else {                                                                   \
      Dma_Set(data4, (void *)&BG_PLTT_COLOR(12, 2), DMA_COPY_WORDS(2),         \
              REG_DMA3);                                                       \
    }                                                                          \
  }

/// 4 copies: eastern_alhafra/s4004, jupiter_lighthouse_room_3/s0dcc,
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

/// 3 copies: mars_lighthouse_entrance/mars_lighthouse_entrance,
/// mars_lighthouse_room_m, mars_lighthouse_star_room.
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

/// 11 copies: jupiter_lighthouse_aerie/s17a8,
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

/// 7 copies: jupiter_lighthouse_room_2/s0d94, jupiter_lighthouse_room_3/s07b0,
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

/// 19 copies: airs_rock/airs_rock, airs_rock_inside/airs_rock_inside,
/// jupiter_lighthouse_aerie, jupiter_lighthouse_aerie_beacon/s0198,
/// jupiter_lighthouse_basement, jupiter_lighthouse_entrance/dist,
/// jupiter_lighthouse_room_1, jupiter_lighthouse_room_2,
/// jupiter_lighthouse_room_4, jupiter_lighthouse_room_5,
/// jupiter_lighthouse_room_6, jupiter_lighthouse_room_7,
/// jupiter_lighthouse_room_8, jupiter_lighthouse_room_9,
/// jupiter_lighthouse_room_m, magma_rock, magma_rock_inside/magma_rock_inside,
/// mars_lighthouse_room_f, sea_of_time.
#define DEFINE_FUNC_020003D0(name)                                             \
  s32 name(s32 *a, s32 *b) {                                                   \
    s32 dx = (a[0] - b[0]) >> 16;                                              \
    s32 dz = (a[2] - b[2]) >> 16;                                              \
                                                                               \
    return Iwram_Sqrt(dx * dx + dz * dz);                                      \
  }

/// 3 copies: mars_lighthouse_entrance/s2270, mars_lighthouse_room_f,
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

/// 3 copies: mars_lighthouse_entrance/mars_lighthouse_entrance,
/// mars_lighthouse_room_f, mars_lighthouse_room_m.
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

/// 4 copies: airs_rock_inside, tundaria_tower/events_f, yampi_desert,
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

/// 4 copies: eastern_alhafra/s44c4, izumo_ruins/s0038,
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

/// 4 copies: jupiter_lighthouse_aerie/s24a8, jupiter_lighthouse_room_8/s1c30,
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

/// 4 copies: mars_lighthouse_entrance/s263c, mars_lighthouse_room_f/s328c,
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

/// 4 copies: mars_lighthouse_entrance/s263c, mars_lighthouse_room_f/s328c,
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

/// 2 copies: islets/s3410, mars_lighthouse_room_f/s3d80.
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

/// 2 copies: jupiter_lighthouse_room_3/s0dcc, jupiter_lighthouse_room_8/s0c44.
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

/// 4 copies: mars_lighthouse_entrance/s2270, mars_lighthouse_room_f/ride,
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

/// 4 copies: jupiter_lighthouse_aerie_beacon/s44cc,
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

/// 4 copies: eastern_alhafra, magma_rock/s34ac, magma_rock_inside/s46e8,
/// prox/prox.
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

/// 4 copies: mars_lighthouse_entrance/s263c, mars_lighthouse_room_f/s3d80,
/// mars_lighthouse_star_room/s2148, prox/prox.
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

/// 2 copies: magma_rock_inside/s3678, trial_road/s15a8.
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

/// 2 copies: jupiter_lighthouse_room_7, jupiter_lighthouse_room_m.
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

/// 4 copies: jupiter_lighthouse_room_5/s2450, jupiter_lighthouse_room_6/s1724,
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

/// 3 copies: mars_lighthouse_entrance/s263c, mars_lighthouse_room_m/s2708,
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

/// 2 copies: magma_rock/s22ac, magma_rock_inside/s3678.
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

/// 3 copies: mars_lighthouse_entrance/mars_lighthouse_entrance,
/// mars_lighthouse_room_f, mars_lighthouse_star_room.
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

/// 2 copies: magma_rock/s22ac, magma_rock_inside/s3678.
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

/// 6 copies: anemos_inner_sanctum, islet_cave, mars_lighthouse_aerie/s3370,
/// mars_lighthouse_entrance/s42f8, mars_lighthouse_room_m, yampi_desert_cave.
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

/// 2 copies: apojii_islands/apojii_islands, aqua_rock/aqua_rock.
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

/// 4 copies: airs_rock, gaia_rock_inside, gondowan_cliffs, treasure_isle.
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

/// 4 copies: mars_lighthouse_entrance/mars_lighthouse_entrance,
/// mars_lighthouse_entrance/mars_lighthouse_entrance, mars_lighthouse_room_f,
/// mars_lighthouse_room_f.
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

/// 3 copies: apojii_islands/apojii_islands, aqua_rock/aqua_rock,
/// aqua_rock_inside/aqua_rock_inside.
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

/// 3 copies: apojii_islands/apojii_islands, aqua_rock/aqua_rock,
/// aqua_rock_inside/aqua_rock_inside.
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

/// 2 copies: dehkan_plateau/s0bc0, madra_catacombs.
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

/// 4 copies: dehkan_plateau/s0bc0, magma_rock_inside/s46e8, trial_road/s15a8,
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

/// 3 copies: jupiter_lighthouse_aerie_beacon/s44cc,
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

/// 3 copies: airs_rock_inside/s2770, aqua_rock_inside/aqua_rock_inside,
/// gaia_rock_inside/s07f4.
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

/// 3 copies: magma_rock_inside/magma_rock_inside, mars_lighthouse_entrance,
/// treasure_isle/treasure_isle.
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

/// 2 copies: apojii_islands/apojii_islands, aqua_rock_inside/aqua_rock_inside.
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

/// 5 copies: gondowan_cliffs, staff_credit, world_map, world_map, world_map.
#define DEFINE_FUNC_02000038_58(name)                                          \
  s32 name(struct Actor *object) {                                             \
    u16 *tile = (u16 *)((u8 *)Ram_HeapSlots->mapWork + 286);                   \
    struct TileSprite *sprite = object->sprite;                                \
                                                                               \
    sprite->tile = *tile;                                                      \
    sprite->frame = 0;                                                         \
    return 1;                                                                  \
  }

/// 2 copies: airs_rock/airs_rock, magma_rock_inside/magma_rock_inside.
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

/// 4 copies: staff_credit, staff_credit, world_map/s3b38, world_map/s3b38.
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

/// 2 copies: eastern_alhafra/s44c4, mars_lighthouse_room_f/s3d80.
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

/// 2 copies: apojii_islands/apojii_islands, aqua_rock_inside/aqua_rock_inside.
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

/// 2 copies: eastern_alhafra/s44c4, gabomba_statue/s2394.
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

/// 2 copies: jupiter_lighthouse_room_5, lemuria_interiors/s12dc.
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

/// 2 copies: jupiter_lighthouse_room_7/s16d4, mars_lighthouse_entrance/s42f8.
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

/// 2 copies: yampi_desert, yampi_desert_cave.
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

/// 3 copies: airs_rock_inside/s2770, aqua_rock_inside/aqua_rock_inside,
/// gaia_rock_inside/s07f4.
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

/// 2 copies: staff_credit, world_map/s3b38.
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

/// 2 copies: magma_rock, magma_rock_inside/magma_rock_inside.
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

/// 2 copies: apojii_islands/apojii_islands, aqua_rock_inside/aqua_rock_inside.
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

/// 3 copies: contigo/contigo, lemuria, magma_rock_inside/s033c.
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

/// 2 copies: eastern_alhafra_ship, lemuria_interiors/s12dc.
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

/// 2 copies: contigo/contigo, lemuria.
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

/// 2 copies: staff_credit, world_map/s3b38.
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

/// 2 copies: staff_credit, world_map/s3b38.
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

/// 3 copies: airs_rock_inside/s2770, aqua_rock_inside/aqua_rock_inside,
/// gaia_rock_inside/s07f4.
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

/// 2 copies: aqua_rock_inside/aqua_rock_inside,
/// magma_rock_inside/magma_rock_inside.
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

/// 4 copies: dehkan_plateau/dehkan_plateau, kibombo/s2f34,
/// shrine_of_the_sea_god/s1340, yampi_desert.
#define DEFINE_FUNC_02001F38(name, call1)                                      \
  void name(s32 id) {                                                          \
    call1(id);                                                                 \
    Actor_SetPartAttribute(Actor_GetById(id), 15);                             \
    Actor_SetSpritePriority(id, 1);                                            \
    Actor_GetById(id)->priorityFlags |= 2;                                     \
  }

/// 3 copies: jupiter_lighthouse_aerie_beacon/s44cc,
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

/// 4 copies: mars_lighthouse_entrance/s263c, mars_lighthouse_room_f/s328c,
/// mars_lighthouse_room_m/s2708, mars_lighthouse_star_room/s2148.
#define DEFINE_FUNC_02002CE8(name, call1)                                      \
  s32 name(struct Actor *object) {                                             \
    call1(object);                                                             \
    SET_POSITION(object, 0, 0, 0);                                             \
    SET_VELOCITY(object, 0, 0, 0);                                             \
    return 0;                                                                  \
  }

/// 3 copies: mars_lighthouse_room_f/ride, mars_lighthouse_room_m/s2118,
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

/// 2 copies: aqua_rock_inside/s3844, taopo_swamp.
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

/// 2 copies: airs_rock/airs_rock, magma_rock/s22ac.
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

/// 2 copies: mars_lighthouse_entrance/mars_lighthouse_entrance,
/// mars_lighthouse_room_f.
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

/// 2 copies: izumo_ruins/s0974, magma_rock/s34ac.
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

/// 2 copies: kibombo_mountains, settlements/s0280.
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

/// 2 copies: mars_lighthouse_entrance/s263c, mars_lighthouse_room_f/s328c.
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

/// 2 copies: staff_credit, world_map/s3b38.
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

/// 2 copies: apojii_islands, aqua_rock_inside.
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

/// 2 copies: kibombo_mountains, taopo_swamp.
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

/// 3 copies: mars_lighthouse_room_f/flash, mars_lighthouse_room_m/s2708,
/// mars_lighthouse_star_room/s2148.
#define DEFINE_FUNC_020024AC(name, call1)                                      \
  s32 name(void) {                                                             \
    if (((struct RideWork *)(gData_020023c4 + 0x188))->active) {               \
      call1();                                                                 \
      return 1;                                                                \
    }                                                                          \
    return 0;                                                                  \
  }

/// 3 copies: mars_lighthouse_room_f/ride, mars_lighthouse_room_m/s2118,
/// mars_lighthouse_star_room/ride_handle.
#define DEFINE_FUNC_02001F78(name)                                             \
  void name(s32 index, s32 value) {                                            \
    struct RideWork *work = (struct RideWork *)(gData_020023c4 + 0x188);       \
    struct RideEntry *entry = &work->entries[index];                           \
                                                                               \
    entry->d = (value & 15) << 16;                                             \
    entry->b = value & 0xf0;                                                   \
  }

/// 2 copies: mars_lighthouse_star_room, mars_lighthouse_star_room.
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

/// 2 copies: aqua_rock_inside/s3844, taopo_swamp.
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

/// 2 copies: magma_rock_inside/magma_rock_inside, mars_lighthouse_entrance.
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

/// 2 copies: apojii_islands/apojii_islands, aqua_rock_inside/aqua_rock_inside.
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

/// 2 copies: daila, daila_interiors.
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

/// 2 copies: dehkan_plateau/dehkan_plateau, yampi_desert.
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

/// 2 copies: magma_rock, magma_rock_inside/magma_rock_inside.
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

/// 3 copies: mars_lighthouse_room_f/ride, mars_lighthouse_room_m/s2118,
/// mars_lighthouse_star_room/ride_handle.
#define DEFINE_FUNC_02001FB8(name)                                             \
  void name(u32 index, s32 value) {                                            \
    struct RideWork *work = (struct RideWork *)(gData_020023c4 + 0x188);       \
                                                                               \
    if (index <= 3)                                                            \
      work->slots[index].value = value;                                        \
  }

/// 2 copies: staff_credit, world_map/s3b38.
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

/// 2 copies: dehkan_plateau/dehkan_plateau, madra_catacombs.
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

/// 2 copies: mars_lighthouse_entrance/mars_lighthouse_entrance,
/// mars_lighthouse_room_f.
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

/// 2 copies: mars_lighthouse_room_m, mars_lighthouse_star_room.
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

/// 2 copies: contigo/contigo, kibombo.
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

/// 2 copies: dehkan_plateau/dehkan_plateau, madra_catacombs.
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

/// 2 copies: islets, prox/prox.
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

/// 2 copies: magma_rock, magma_rock_inside/magma_rock_inside.
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

/// 2 copies: mars_lighthouse_entrance/mars_lighthouse_entrance,
/// mars_lighthouse_room_f.
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

/// 2 copies: mars_lighthouse_entrance/mars_lighthouse_entrance,
/// mars_lighthouse_room_f.
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

/// 2 copies: mars_lighthouse_entrance/s263c, mars_lighthouse_room_f/s3d80.
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

/// 2 copies: aqua_rock_inside/s3844, taopo_swamp.
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

/// 2 copies: mars_lighthouse_entrance/mars_lighthouse_entrance,
/// mars_lighthouse_star_room.
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

/// 2 copies: mars_lighthouse_entrance/mars_lighthouse_entrance,
/// mars_lighthouse_room_f.
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

/// 2 copies: mars_lighthouse_room_m, mars_lighthouse_star_room.
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

/// 2 copies: staff_credit, world_map/s3b38.
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

/// 2 copies: trial_road/s0218, trial_road/s0218.
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

/// 2 copies: apojii_islands/apojii_islands, settlements/s0280.
#define DEFINE_FUNC_02000E90(name, data1, data2, data3)                        \
  void name(void) {                                                            \
    struct MapLayer *layer = &Ram_HeapSlots->mapWork->layers[2];               \
    s16 y = layer->y >> 16;                                                    \
                                                                               \
    data1 = 1448 - y;                                                          \
    data2 = layer->x >> 16;                                                    \
    data3 = (s16)(layer->x >> 16) - (gFrameCount >> 2);                        \
  }

/// 2 copies: staff_credit, world_map/s3b38.
#define DEFINE_FUNC_020019D4(name, call1)                                      \
  void name(struct Actor *object) {                                            \
    u32 frame = gFrameCount;                                                   \
                                                                               \
    if (frame & 1)                                                             \
      Animation_ApplyChildValues(object, (frame >> 1) % 6);                    \
    if ((gFrameCount & 15) == 0)                                               \
      call1(object);                                                           \
  }

/// 2 copies: staff_credit, world_map/s3b38.
#define DEFINE_FUNC_02001E58(name, data1, data2, data3, data4, call1)          \
  void name(void) {                                                            \
    Resource_DecodeType01(gWorldMapPackedTilesC, data1);                       \
    data2 = Resource_FindFreeEntry();                                          \
    Vram_Load(data2, 0x100, 0);                                                \
    data3 = -1;                                                                \
    data4 = 10;                                                                \
    Task_AddFrame(call1, TASK_PASS_VBLANK - 10);                               \
  }

/// 2 copies: apojii_islands/apojii_islands, settlements/s0280.
#define DEFINE_FUNC_02000ED0(name, call1, call2)                               \
  void name(void) {                                                            \
    s32 priority;                                                              \
                                                                               \
    Irq_SetHandler(1, 0, call1);                                               \
    priority = 0xc80;                                                          \
    FieldTask_AddFrame(call2, priority);                                       \
  }

/// 2 copies: yampi_desert/placements, yampi_desert/placements.
#define DEFINE_FUNC_02000648(name, data1)                                      \
  void name(s32 a, s32 b) {                                                    \
    Func_0200388c(data1, a, b);                                                \
    if (a == 1) {                                                              \
      if (gData_02004808 == 0)                                                 \
        gData_02004808 = 210;                                                  \
    }                                                                          \
  }

/// 2 copies: airs_rock/airs_rock, airs_rock_inside/airs_rock_inside.
#define DEFINE_FUNC_02000D18(name)                                             \
  s32 name(s32 *a, s32 *b) {                                                   \
    s32 dx = (*a++ - *b++) >> 16;                                              \
    s32 dy = (*a - *b + 0x60000) >> 16;                                        \
                                                                               \
    return Iwram_Sqrt(dx * dx + dy * dy);                                      \
  }

/// 2 copies: aqua_rock_inside/s3844, taopo_swamp.
#define DEFINE_FUNC_02002778(name)                                             \
  void name(s32 i, s32 id) {                                                   \
    struct VentWork *work = VENT_WORK;                                         \
                                                                               \
    if ((u32)i <= 3)                                                           \
      work->ids[i] = id;                                                       \
  }

/// 2 copies: champa_interiors, daila_interiors.
#define DEFINE_FUNC_0200014C(name)                                             \
  void name(void) {                                                            \
    struct EventWork *work = Ram_HeapSlots->eventWork;                         \
                                                                               \
    work->screenEffect = 0x209;                                                \
    GameFlag_ClearBit(0x12f);                                                  \
  }

/// 2 copies: contigo/contigo, shaman_village/s00b8.
#define DEFINE_FUNC_0200098C(name)                                             \
  void name(void) {                                                            \
    Event_Start();                                                             \
    Event_ResolvePendingActions(0);                                            \
    FieldMessage_Show((s32)MsgShamanEntranceLocked, 1);                        \
    Event_End();                                                               \
  }

/// 2 copies: mars_lighthouse_entrance/mars_lighthouse_entrance,
/// mars_lighthouse_room_f.
#define DEFINE_FUNC_02001A1C(name)                                             \
  s32 name(struct Actor *a, struct Actor *b) {                                 \
    if (ABS(a->x - b->x) < 0x80000 && ABS(a->y - b->y) < 0x80000 &&            \
        ABS(a->z - b->z) < 0x80000)                                            \
      return 1;                                                                \
    return 0;                                                                  \
  }

/// 2 copies: mars_lighthouse_room_m/s2118,
/// mars_lighthouse_star_room/ride_handle.
#define DEFINE_FUNC_02001DD0(name, call1)                                      \
  void name(void) {                                                            \
    struct RideWork *work = (struct RideWork *)(gData_020023c4 + 0x188);       \
                                                                               \
    call1(work->handle);                                                       \
    work->handle = NULL;                                                       \
  }

/// 2 copies: staff_credit, world_map/s3b38.
#define DEFINE_FUNC_02001A0C(name)                                             \
  void name(struct Actor *object) {                                            \
    u32 frame = gFrameCount;                                                   \
                                                                               \
    if (frame & 1)                                                             \
      Animation_ApplyChildValues(object, (frame >> 1) % 6);                    \
  }

/// 2 copies: staff_credit, world_map/s3b38.
#define DEFINE_FUNC_02001EB8(name, data1, data2, data3, call1)                 \
  void name(void) {                                                            \
    Resource_DecodeType01(gWorldMapPackedTilesC, data1);                       \
    Resource_ResetEntry(data2);                                                \
    data3 = -1;                                                                \
    FieldTask_Remove(call1);                                                   \
  }

/// 6 copies: gabomba_catacombs, gabomba_catacombs, gabomba_catacombs,
/// gabomba_catacombs, gabomba_catacombs, gabomba_catacombs.
#define DEFINE_FUNC_020001E0(name, n1, n2, n3, n4, n5, n6, n7, n8, n9)         \
  void name(void) {                                                            \
    struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);       \
                                                                               \
    Func_02001274_(n1, n2, 0, n3, 3, 56, n4);                                  \
    Actor_SetPosition(n5, PIXELS(n6), PIXELS(n7));                             \
    if (leader->x >> 20 == n8 && leader->z >> 20 == n9) {                      \
      LEADER_HOP();                                                            \
      Actor_OffsetPositionAndResetMotion(gMainRoutineWork.currentOwner, 16,    \
                                         0);                                   \
      leader->velocityY = PIXELS(6);                                           \
      Actor_WaitForMoveById(gMainRoutineWork.currentOwner);                    \
      Event_End();                                                             \
    }                                                                          \
  }

/// 8 copies: treasure_isle/treasure_isle, treasure_isle/treasure_isle,
/// treasure_isle/treasure_isle, treasure_isle/treasure_isle,
/// treasure_isle/treasure_isle, treasure_isle/treasure_isle,
/// treasure_isle/treasure_isle, treasure_isle/treasure_isle.
#define DEFINE_FUNC_02000914(name, n1, n2)                                     \
  void name(void) {                                                            \
    s32 t0;                                                                    \
                                                                               \
    t0 = ((s32)Actor_Get(n1));                                                 \
    FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9) =                                   \
        ((-13 & FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9)) | 4);                  \
    FIELD_AT((t0 + 0x23), u8, 0) = (0xfe & FIELD_AT((t0 + 0x23), u8, 0));      \
    Event_SetFlag(n2);                                                         \
  }

/// 6 copies: mars_lighthouse_aerie,
/// mars_lighthouse_entrance/mars_lighthouse_entrance,
/// mars_lighthouse_entrance/mars_lighthouse_entrance,
/// mars_lighthouse_entrance/mars_lighthouse_entrance,
/// mars_lighthouse_entrance/mars_lighthouse_entrance,
/// mars_lighthouse_entrance/mars_lighthouse_entrance.
#define DEFINE_FUNC_020001A0(name, call1, call2, n1, call3)                    \
  void name(void) {                                                            \
    call1();                                                                   \
    call2();                                                                   \
    {                                                                          \
      u8 *party = (u8 *)&gMainRoutineWork;                                     \
                                                                               \
      party[0x2c2] = 2;                                                        \
    }                                                                          \
    FieldEvent_SetExitRequest(n1);                                             \
    FieldEvent_Wait(40);                                                       \
    call3();                                                                   \
  }

/// 8 copies: gabomba_catacombs, gabomba_catacombs, gabomba_catacombs,
/// gabomba_catacombs, gabomba_catacombs, gabomba_catacombs, gabomba_catacombs,
/// gabomba_catacombs.
#define DEFINE_FUNC_020006CC(name, n1, n2, n3, n4, n5, n6)                     \
  void name(void) {                                                            \
    struct Actor *object = Actor_GetById(n1);                                  \
                                                                               \
    GameFlag_SetBit(n2);                                                       \
    object->motionFlags = 0;                                                   \
    object->priorityFlags = 0;                                                 \
    MapGrid_CopyCellAttributes(n3, n4, 1, 1, n5, n6);                          \
  }

/// 6 copies: sea_of_time, sea_of_time, sea_of_time, sea_of_time, sea_of_time,
/// sea_of_time.
#define DEFINE_FUNC_02001480(name, n1, n2)                                     \
  void name(void) {                                                            \
    struct Actor *object;                                                      \
                                                                               \
    Event_Start();                                                             \
    Event_ResolvePendingActions(0);                                            \
    object = Actor_GetById(n1);                                                \
    Func_0200141c(object->x, Actor_GetById(n1)->z, n1);                        \
    GameFlag_SetBit(n2);                                                       \
    Func_02001fb4();                                                           \
  }

/// 2 copies: kandorean_temple/s2ec4, kandorean_temple/s2ec4.
#define DEFINE_FUNC_02003684(name, n1, n2, n3, n4, n5, n6, n7, data1)          \
  void name(void) {                                                            \
    struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);       \
    struct Actor *first = Actor_GetById(n1);                                   \
    struct Actor *second = Actor_GetById(n2);                                  \
                                                                               \
    Event_Start();                                                             \
    Event_ResolvePendingActions(0);                                            \
    Func_02005388(162);                                                        \
    FieldCamera_MoveTo(-1, -1, -1, 0);                                         \
    wait(1);                                                                   \
    leader->motionFlags = 0;                                                   \
    Actor_SetPositionAndResetMotion_(                                          \
        Actor_GetById(gMainRoutineWork.currentOwner), PIXELS(n3), PIXELS(-20), \
        PIXELS(n4));                                                           \
    Actor_SetSingleChildFlags_(Actor_GetById(gMainRoutineWork.currentOwner),   \
                               0);                                             \
    Actor_SetPositionAndResetMotion_(Actor_GetById(n1), PIXELS(n3),            \
                                     PIXELS(-20), PIXELS(n5));                 \
    Actor_SetPositionAndResetMotion_(Actor_GetById(n2), PIXELS(n3),            \
                                     PIXELS(-20), PIXELS(n4));                 \
    Actor_SetSpritePriority(n1, 2);                                            \
    Actor_SetSpritePriority(n2, 3);                                            \
    {                                                                          \
      u8 *flags = &second->priorityFlags;                                      \
      s32 c = 2 | *flags;                                                      \
                                                                               \
      *flags = c;                                                              \
    }                                                                          \
    second->scaleY = 0x400;                                                    \
    Actor_SetModeById(n1, 4);                                                  \
    Actor_SetModeById(n2, 2);                                                  \
    ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 521;            \
    Event_StartScreenTransition();                                             \
    Event_WaitTransitionFrames();                                              \
    /* A goto loop, as in Func_0200393c. */                                    \
  rise:                                                                        \
    first->y += PIXELS(4);                                                     \
    leader->y += PIXELS(4);                                                    \
    Func_02000cd8(n2, 0x4ccc, 0x4ccc);                                         \
    wait(1);                                                                   \
    if (first->y < 0)                                                          \
      goto rise;                                                               \
    MapGrid_CopyCellAttributes(0, 0, 1, 1, n6, n7);                            \
    data1 = 2000;                                                              \
    ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->value = 516;            \
    Event_End();                                                               \
  }

/// 4 copies: mars_lighthouse_entrance/mars_lighthouse_entrance,
/// mars_lighthouse_entrance/mars_lighthouse_entrance, mars_lighthouse_room_f,
/// mars_lighthouse_room_f.
#define DEFINE_FUNC_02001C74(name, call1, call2, n1, call3, call4)             \
  void name(s32 *position, s32 speed, s32 mode) {                              \
    struct Actor *object = call1(366, position[0], position[1], position[2]);  \
                                                                               \
    if (object != NULL) {                                                      \
      object->bounce = 0;                                                      \
      object->gravity = 0;                                                     \
      object->unk4c = speed;                                                   \
      call2(object, n1);                                                       \
      object->update = call3;                                                  \
      if (mode)                                                                \
        object->unk5c[0] = 2;                                                  \
      call4(object, 2);                                                        \
    }                                                                          \
  }

/// 4 copies: airs_rock_inside, anemos_inner_sanctum_entrance/spawn, caverns,
/// sea_of_time/object.
#define DEFINE_FUNC_020000D0_2(name, n1)                                       \
  struct Actor *name(s32 x, s32 y, s32 z, s32 type) {                          \
    struct Actor *object = Import_ActorCreate(type, x, y, z);                  \
                                                                               \
    if (object != NULL) {                                                      \
      Scene_SetActorSpritePriority(object, 1);                                 \
      object->motionFlags = 0;                                                 \
      object->collisionFlags = 8;                                              \
      Actor_SetSingleChildFlags(object, 0);                                    \
      Actor_SetPartAttribute(object, 15);                                      \
      object->priorityFlags |= n1;                                             \
      return object;                                                           \
    }                                                                          \
    return NULL;                                                               \
  }

/// 7 copies: dehkan_plateau/dehkan_plateau, dehkan_plateau/dehkan_plateau,
/// dehkan_plateau/dehkan_plateau, dehkan_plateau/dehkan_plateau,
/// dehkan_plateau/dehkan_plateau, shrine_of_the_sea_god/s1340, yampi_desert.
#define DEFINE_FUNC_02001F94(name, n1, call1, n2, call2)                       \
  void name(void) {                                                            \
    u32 i;                                                                     \
                                                                               \
    for (i = 0; i <= n1; i++)                                                  \
      call1(i + n2);                                                           \
    call2();                                                                   \
  }

/// 6 copies: daila, daila, daila, daila, daila, daila.
#define DEFINE_FUNC_02000924(name, n1, n2, n3, n4, n5)                         \
  void name(void) {                                                            \
    Import_EventStart();                                                       \
    Event_ResolvePendingActions(0);                                            \
    MapGrid_CopyCellAttributes(n1, n2, 1, 1, n3, n2);                          \
    Actor_SetHorizontalPositionWithTerrain(n4, 0, 0);                          \
    GameFlag_SetBit(n5);                                                       \
    Import_EventEnd();                                                         \
  }

/// 3 copies: islet_cave, islet_cave, islet_cave.
#define DEFINE_FUNC_02000AEC(name, n1, n2, n3)                                 \
  void name(void) {                                                            \
    struct Actor *object0;                                                     \
    s32 value1;                                                                \
    struct Actor *object2;                                                     \
    s32 value3;                                                                \
    struct Actor *object4;                                                     \
                                                                               \
    object0 = Actor_GetById(n1);                                               \
    value1 = object0->x;                                                       \
    object2 = Actor_GetById(n1);                                               \
    value3 = object2->z;                                                       \
    Func_020009f8(n1, n2, 0);                                                  \
    wait(2);                                                                   \
    object4 = Actor_GetById(n1);                                               \
    Map_CopyCellAttributeRect((value1 >> 20), ((value3 >> 20) - 1), 1, 3,      \
                              (object4->x >> 20), ((value3 >> 20) - 1));       \
    Map_CopyCellAttributeRect(0, 0, 1, 3, (value1 >> 20),                      \
                              ((value3 >> 20) - 1));                           \
    GameFlag_ClearBit(n3);                                                     \
    return;                                                                    \
  }

/// 3 copies: islet_cave, islet_cave, islet_cave.
#define DEFINE_FUNC_02000CF0(name, n1, n2)                                     \
  void name(void) {                                                            \
    struct Actor *object0;                                                     \
    s32 value1;                                                                \
    struct Actor *object2;                                                     \
    s32 value3;                                                                \
    struct Actor *object4;                                                     \
                                                                               \
    object0 = Actor_GetById(n1);                                               \
    value1 = object0->x;                                                       \
    object2 = Actor_GetById(n1);                                               \
    value3 = object2->z;                                                       \
    Func_020009f8(n1, -80, 0);                                                 \
    wait(2);                                                                   \
    object4 = Actor_GetById(n1);                                               \
    Map_CopyCellAttributeRect((value1 >> 20), ((value3 >> 20) - 1), 1, 3,      \
                              (object4->x >> 20), ((value3 >> 20) - 1));       \
    Map_CopyCellAttributeRect(0, 0, 1, 3, (value1 >> 20),                      \
                              ((value3 >> 20) - 1));                           \
    GameFlag_ClearBit(n2);                                                     \
    return;                                                                    \
  }

/// 2 copies: jupiter_lighthouse_room_5, mars_lighthouse_aerie.
#define DEFINE_FUNC_020001A4(name, n1, data1, call1, n2)                       \
  void name(void) {                                                            \
    struct Actor *center;                                                      \
    struct OrbitEffect *effect;                                                \
    struct FieldSprite *sprite;                                                \
    s32 angle;                                                                 \
                                                                               \
    if (Event_GetFlag(n1) == 0 && gFrameCount % 3 != 0)                        \
      return;                                                                  \
    center = Actor_Get(23);                                                    \
    if (Event_GetFlag(n1) != 0) {                                              \
      u32 random;                                                              \
                                                                               \
      random = Random16Far();                                                  \
      effect = CreateOrbitEffect(0x2a0, center->x,                             \
                                 (s32)((u32)(random << 8) >> 16 << 16) +       \
                                     center->y - 0x1c0000,                     \
                                 center->z);                                   \
    } else {                                                                   \
      u32 random;                                                              \
                                                                               \
      random = Random16Far();                                                  \
      effect = CreateOrbitEffect(0x2a0, center->x,                             \
                                 (s32)((u32)(random << 6) >> 16 << 16) +       \
                                     center->y - 0x1c0000,                     \
                                 center->z);                                   \
    }                                                                          \
    if (effect != 0) {                                                         \
      sprite = effect->sprite;                                                 \
      Actor_StartScript(effect, data1);                                        \
      Actor_SetSpriteColor(effect, 1);                                         \
      effect->motionFlags = 0;                                                 \
      angle = Random16Far() & 0xffff000;                                       \
      effect->angle = angle;                                                   \
      effect->unk66 = 0;                                                       \
      effect->update = (void (*)(struct OrbitEffect *))call1;                  \
      effect->radius =                                                         \
          Math_Sin((u32)(Random16Far() * 0xffff) >> 20) * 24 >> 16;            \
      sprite->nextFrame = 0;                                                   \
      sprite->priority = n2;                                                   \
    }                                                                          \
  }

/// 5 copies: mars_lighthouse_entrance/mars_lighthouse_entrance,
/// mars_lighthouse_entrance/s2270, mars_lighthouse_room_f/flash,
/// mars_lighthouse_room_m/s2118, mars_lighthouse_star_room/ride_handle.
#define DEFINE_FUNC_020012BC(name, n1, n2, n3)                                 \
  void name(struct Actor *object) {                                            \
    switch (gFrameCount & 7) {                                                 \
    case 0:                                                                    \
      Animation_ApplyChildValues(object, n1);                                  \
      break;                                                                   \
    case n2:                                                                   \
      Animation_ApplyChildValues(object, n3);                                  \
      break;                                                                   \
    }                                                                          \
  }

/// 6 copies: shrine_of_the_sea_god/s1340, shrine_of_the_sea_god/s1340,
/// shrine_of_the_sea_god/s1340, shrine_of_the_sea_god/s1340,
/// shrine_of_the_sea_god/s1340, shrine_of_the_sea_god/s1340.
#define DEFINE_FUNC_020027F8(name, n1, n2, n3)                                 \
  void name(void) {                                                            \
    Func_0200279c();                                                           \
    Camera_MoveWithinMapBounds(n1 << 16, 16 << 16, n2 << 16, 1);               \
    Actor_SetPositionAndReset(14, n1, n2);                                     \
    Actor_GetById(14)->state = n3;                                             \
    Func_020027d4();                                                           \
  }

/// 5 copies: gabomba_statue, gabomba_statue, gabomba_statue, gabomba_statue,
/// gabomba_statue.
#define DEFINE_FUNC_02001728(name, n1, n2)                                     \
  void name(struct Actor *actor) {                                             \
    Func_020056b4();                                                           \
    FieldEvent_ResolvePendingActions(0);                                       \
    Event_PlaySound(581);                                                      \
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0x19999);           \
    Actor_MoveToAndWait(gMainRoutineWork.currentOwner, n1, n2);                \
    Event_End();                                                               \
  }

/// 2 copies: shrine_of_the_sea_god/s1340, shrine_of_the_sea_god/s1340.
#define DEFINE_FUNC_02002830(name, n1, n2, n3, n4, n5, n6)                     \
  void name(void) {                                                            \
    struct ShrineOfTheSeaGodObject *block = Actor_GetById(14);                 \
    u8 *state;                                                                 \
    s32 flag;                                                                  \
                                                                               \
    Func_0200279c();                                                           \
    ShrineOfTheSeaGod_CamMove(n1 << 16, 16 << 16, 216 << 16, 1);               \
    ShrineOfTheSeaGod_ObjWalk(14, n1, 216);                                    \
    flag = GameFlag_Test(0x207);                                               \
    state = &block->state;                                                     \
    if (flag) {                                                                \
      block->facing = n2;                                                      \
      ShrineOfTheSeaGod_ObjEmote(14, 258);                                     \
      Event_Wait(20);                                                          \
      *state = n3;                                                             \
      GameFlag_SetBit(0x206);                                                  \
    } else {                                                                   \
      Event_Wait(5);                                                           \
      Camera_MoveWithinMapBounds(n4 << 16, 16 << 16, 216 << 16, 1);            \
      Sound_Play(153);                                                         \
      Actor_SetSingleChildFlags(Actor_GetById(14), 0);                         \
      ShrineOfTheSeaGod_ObjSpeed(14, 0x4cccc, 0x26666);                        \
      block->speed = 0x80000;                                                  \
      Actor_SetPositionAndCommit(14, n5, 216);                                 \
      Actor_SetSingleChildFlags(Actor_GetById(14), 1);                         \
      Event_Wait(10);                                                          \
      ShrineOfTheSeaGod_ObjSpeed(14, 0x19999, 0xcccc);                         \
      ShrineOfTheSeaGod_ObjWalk(14, n4, 216);                                  \
      *state = n6;                                                             \
    }                                                                          \
    Func_020027d4();                                                           \
  }

/// 3 copies: gabomba_statue, gabomba_statue, gabomba_statue.
#define DEFINE_FUNC_02001284(name, n1, n2, n3, n4, n5, n6, n7)                 \
  void name(void) {                                                            \
    struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);           \
    s32 x = leader->x >> 20;                                                   \
    s32 z = leader->z >> 20;                                                   \
                                                                               \
    if (z == n1) {                                                             \
      if (x == n2 && (gInput.held & 16) == 0)                                  \
        return;                                                                \
      if (x == n3 && (gInput.held & 32) == 0)                                  \
        return;                                                                \
    } else if (x == n4 && z == n5 && (gInput.held & 64) == 0) {                \
      return;                                                                  \
    }                                                                          \
    Func_020011bc(n6, n7);                                                     \
  }

/// 2 copies: shrine_of_the_sea_god/s1340, shrine_of_the_sea_god/s1340.
#define DEFINE_FUNC_02002B50(name, n1, n2, n3, n4, n5)                         \
  void name(void) {                                                            \
    struct ShrineOfTheSeaGodObject *block = Actor_GetById(14);                 \
    u8 *state;                                                                 \
    s32 flag;                                                                  \
                                                                               \
    Func_0200279c();                                                           \
    flag = GameFlag_Test(0x207);                                               \
    state = &block->state;                                                     \
    if (flag) {                                                                \
      block->facing = n1;                                                      \
      ShrineOfTheSeaGod_ObjEmote(14, 258);                                     \
      Event_Wait(20);                                                          \
      *state = n2;                                                             \
      GameFlag_SetBit(0x206);                                                  \
    } else {                                                                   \
      Camera_MoveWithinMapBounds(n3 << 16, 16 << 16, 216 << 16, 1);            \
      Sound_Play(153);                                                         \
      Actor_SetSingleChildFlags(Actor_GetById(14), 0);                         \
      ShrineOfTheSeaGod_ObjSpeed(14, 0x4cccc, 0x26666);                        \
      block->speed = 0x80000;                                                  \
      Actor_SetPositionAndCommit(14, n4, 216);                                 \
      Actor_SetSingleChildFlags(Actor_GetById(14), 1);                         \
      Event_Wait(10);                                                          \
      ShrineOfTheSeaGod_ObjSpeed(14, 0x19999, 0xcccc);                         \
      ShrineOfTheSeaGod_ObjWalk(14, n3, 216);                                  \
      *state = n5;                                                             \
    }                                                                          \
    Func_020027d4();                                                           \
  }

/// 2 copies: lemurian_ship, lemurian_ship.
#define DEFINE_FUNC_02001138(name, n1, n2, n3, n4)                             \
  void name(void) {                                                            \
    struct LemurianShipEventWork *work = Ram_HeapSlots->eventWork;             \
    struct LemurianShipObject *object =                                        \
        ((struct LemurianShipObject *)Actor_GetById(n1));                      \
    u32 i;                                                                     \
                                                                               \
    Event_Start();                                                             \
    Event_ResolvePendingActions(0);                                            \
    work->field16a = 0;                                                        \
    GameFlag_ClearBit(n2);                                                     \
    GameFlag_ClearBit(n3);                                                     \
    Event_Wait(20);                                                            \
    Sound_Play(186);                                                           \
    for (i = 0; i <= 63; i++) {                                                \
      object->y += 0x4000;                                                     \
      wait(1);                                                                 \
    }                                                                          \
    Actor_SetSpritePriority(n1, 2);                                            \
    Map_CopyCellPictureIndicesRect(4, 123, n4, 69, 1, 1);                      \
    MapGrid_CopyCellAttributes(36, 0, 1, 1, n4, 9);                            \
    Sound_Play(188);                                                           \
    MapGrid_CopyCellsTo(7, 124, 31, 64, 3, 2);                                 \
    GameFlag_ClearBit(0x982);                                                  \
    Event_Wait(20);                                                            \
    Event_End();                                                               \
  }

/// 4 copies: gondowan_cliffs/s03cc, kibombo_mountains, kibombo_mountains,
/// kibombo_mountains.
#define DEFINE_FUNC_020000EC(name, n1, n2, n3, n4, n5)                         \
  void name(void) {                                                            \
    struct Actor *object = Actor_Get(n1);                                      \
                                                                               \
    object->priorityFlags = 0;                                                 \
    object->motionFlags = 0;                                                   \
    MapGrid_CopyCellAttributes(n2, n3, 1, 1, n4, n3);                          \
    Event_SetFlag(n5);                                                         \
  }

/// 2 copies: garoh/garoh, garoh/garoh.
#define DEFINE_FUNC_02000390(name, data1, n1, n2)                              \
  void name(void) {                                                            \
    struct BattleEffectBuffers *buffers = Ram_HeapSlots->block32;              \
    const u8 *table = (const u8 *)data1;                                       \
    u32 i = 0;                                                                 \
    u16 *dst = &buffers->palette[n1];                                          \
    s32 blue = 4;                                                              \
    s32 red = 0;                                                               \
    volatile u16 *color;                                                       \
    s32 green;                                                                 \
                                                                               \
  loop:                                                                        \
    color = BG_PLTT + n1 + i;                                                  \
    green = blue - 2;                                                          \
    *color = *(const u16 *)(table + blue) << 10 |                              \
             *(const u16 *)(table + green) << 5 | *(const u16 *)(table + red); \
    *dst++ = *color;                                                           \
    i++;                                                                       \
    blue += 6;                                                                 \
    red += 6;                                                                  \
    if (i < n2)                                                                \
      goto loop;                                                               \
  }

/// 3 copies: lemurian_ship, lemurian_ship, lemurian_ship.
#define DEFINE_FUNC_02000BF0(name, n1)                                         \
  void name(s32 id) {                                                          \
    Event_Start();                                                             \
    Event_ResolvePendingActions(0);                                            \
    Actor_SetAngleToward(id, gMainRoutineWork.currentOwner, 0);                \
    Actor_StartRepeatedMotion(id, 2);                                          \
    Actor_SetAttachedEffect(id, 0x102);                                        \
    Event_Wait(20);                                                            \
    gMainRoutineWork.battleRule24b = 2;                                        \
    Field_RequestBattleFromEncounterTable(11, 0);                              \
    GameFlag_SetBit(id + n1);                                                  \
  }

/// 2 copies: islet_cave, islet_cave.
#define DEFINE_FUNC_02000B4C(name, n1, n2, n3)                                 \
  void name(void) {                                                            \
    struct Actor *object0;                                                     \
    s32 value1;                                                                \
    struct Actor *object2;                                                     \
    s32 value3;                                                                \
    struct Actor *object4;                                                     \
                                                                               \
    object0 = Actor_GetById(n1);                                               \
    value1 = object0->x;                                                       \
    object2 = Actor_GetById(n1);                                               \
    value3 = object2->z;                                                       \
    Func_020009f8(n1, -n2, 0);                                                 \
    wait(2);                                                                   \
    object4 = Actor_GetById(n1);                                               \
    Map_CopyCellAttributeRect((value1 >> 20), ((value3 >> 20) - 1), 1, 3,      \
                              (object4->x >> 20), ((value3 >> 20) - 1));       \
    Map_CopyCellAttributeRect(0, 0, 1, 3, (value1 >> 20),                      \
                              ((value3 >> 20) - 1));                           \
    GameFlag_SetBit(n3);                                                       \
    return;                                                                    \
  }

/// 2 copies: islet_cave, islet_cave.
#define DEFINE_FUNC_02000C8C(name, n1, n2)                                     \
  void name(void) {                                                            \
    struct Actor *object0;                                                     \
    s32 value1;                                                                \
    struct Actor *object2;                                                     \
    s32 value3;                                                                \
    struct Actor *object4;                                                     \
                                                                               \
    object0 = Actor_GetById(n1);                                               \
    value1 = object0->x;                                                       \
    object2 = Actor_GetById(n1);                                               \
    value3 = object2->z;                                                       \
    Func_020009f8(n1, 80, 0);                                                  \
    wait(2);                                                                   \
    object4 = Actor_GetById(n1);                                               \
    Map_CopyCellAttributeRect((value1 >> 20), ((value3 >> 20) - 1), 1, 3,      \
                              (object4->x >> 20), ((value3 >> 20) - 1));       \
    Map_CopyCellAttributeRect(0, 0, 1, 3, (value1 >> 20),                      \
                              ((value3 >> 20) - 1));                           \
    GameFlag_SetBit(n2);                                                       \
    return;                                                                    \
  }

/// 2 copies: taopo_swamp, taopo_swamp.
#define DEFINE_FUNC_02001684(name, n1, n2)                                     \
  void name(void) {                                                            \
    if ((gFrameCount & 7) == 0 && (u32)Random16Far() < 0x4000) {               \
      s32 offset = Random16Far() << 5;                                         \
      s32 x = offset + n1;                                                     \
      s32 z = n2 - offset;                                                     \
      s32 y = 0xa00000;                                                        \
      struct Actor *object = FieldActor_Create(0x12d, x, y, z);                \
      s32 scale = 0x547a;                                                      \
                                                                               \
      if (object != NULL) {                                                    \
        object->scaleX = scale;                                                \
        object->scaleY = scale;                                                \
        object->motionFlags = 2;                                               \
        object->terrainHeight = 0xffc00000;                                    \
        object->gravity = 0x7333;                                              \
        Actor_StartScript(object, gData_02002a6c);                             \
        Actor_SetSingleChildFlags(object, 0);                                  \
      }                                                                        \
    }                                                                          \
  }

/// 2 copies: airs_rock_inside/s2770, airs_rock_inside/s2770.
#define DEFINE_FUNC_02002980(name, n1)                                         \
  void name(void) {                                                            \
    struct Actor *object = Actor_GetById(Party_GetLeaderActor());              \
    s32 x = object->x >> 20;                                                   \
    s32 y = object->y >> 20;                                                   \
    s32 z = object->z >> 20;                                                   \
    s32 key = (x << 16) + z;                                                   \
                                                                               \
    if (key != gData_02006c0c) {                                               \
      gData_02006c0c = key;                                                    \
      gData_020073dc = 1;                                                      \
      gData_020073e0 = 3;                                                      \
      gData_020073e4 = x;                                                      \
      gData_020073e8 = z - y + n1;                                             \
      CopyCells(x, gData_020073e8, 1, 3, 73, 72);                              \
      CopyCells(72, 72, gData_020073dc, gData_020073e0, gData_020073e4,        \
                gData_020073e8);                                               \
    }                                                                          \
  }

/// 2 copies: gabomba_statue, gabomba_statue.
#define DEFINE_FUNC_020012EC(name, n1, n2, n3, n4, n5, n6, n7, n8)             \
  void name(void) {                                                            \
    struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);           \
    s32 x = leader->x >> 20;                                                   \
    s32 z = leader->z >> 20;                                                   \
                                                                               \
    if (x == n1) {                                                             \
      if (z == n2 && (gInput.held & 128) == 0)                                 \
        return;                                                                \
      if (z == n3 && (gInput.held & 64) == 0)                                  \
        return;                                                                \
    } else if (z == n4) {                                                      \
      if (x == n5 && (gInput.held & 16) == 0)                                  \
        return;                                                                \
      if (x == n6 && (gInput.held & 32) == 0)                                  \
        return;                                                                \
    }                                                                          \
    Func_020011bc(n7, n8);                                                     \
  }

/// 2 copies: gabomba_statue, gabomba_statue.
#define DEFINE_FUNC_02001B38_2(name, n1, n2)                                   \
  void name(void) {                                                            \
    u32 i = 0;                                                                 \
                                                                               \
  next:                                                                        \
    {                                                                          \
      s32 id = i + n1;                                                         \
      struct Actor *object = Actor_Get(id);                                    \
                                                                               \
      Func_02001adc(id);                                                       \
      object->motionFlags = 0;                                                 \
      object->y = PIXELS(36);                                                  \
      i++;                                                                     \
    }                                                                          \
    if (i < n2)                                                                \
      goto next;                                                               \
    Func_02001b0c();                                                           \
  }

/// 2 copies: lemurian_ship_deck, lemurian_ship_deck.
#define DEFINE_FUNC_02000188(name, n1, n2)                                     \
  void name(void) {                                                            \
    if ((gFrameCount & 7) == 0) {                                              \
      s16 last = OBJ_PALETTE[n1];                                              \
      s32 i;                                                                   \
                                                                               \
      /* The loop is written with goto so that it carries no loop notes: the   \
       * bytes */                                                              \
      /* rebuild the palette base on every pass, and a for or do loop hoists   \
       * it */                                                                 \
      /* and steps two pointers (differ 30). */                                \
      i = n1;                                                                  \
    loop:                                                                      \
      OBJ_PALETTE[i] = OBJ_PALETTE[i - 1];                                     \
      i--;                                                                     \
      if (i != n2)                                                             \
        goto loop;                                                             \
      OBJ_PALETTE[n2] = last;                                                  \
    }                                                                          \
  }

/// 2 copies: mars_lighthouse_entrance/mars_lighthouse_entrance,
/// mars_lighthouse_entrance/mars_lighthouse_entrance.
#define DEFINE_FUNC_0200085C(name, n1, n2, n3, n4, n5, n6, n7, n8, n9)         \
  void name(s32 state, s32 frame) {                                            \
    if (state == 1) {                                                          \
      Event_PlaySound(0x23e);                                                  \
      PaletteFade_ApplyTarget(0x10000, 0);                                     \
      PaletteFade_Interpolate(60);                                             \
      MapGrid_CopyCellsLayered(n1, n2, 8, n3, n1, n4);                         \
      MapGrid_CopyCellAttributes(n5, n6, 8, n7, n1, n6);                       \
      Actor_SetPosition(n8, 0, 0);                                             \
      Flag_Set(n9);                                                            \
    }                                                                          \
    if (frame > 96 && frame < 272 && frame % 30 == 0)                          \
      Func_02000414(n8);                                                       \
    if (frame == 276) {                                                        \
      PaletteFade_ApplyTarget(0x7fff, 0);                                      \
      PaletteFade_Interpolate(20);                                             \
    }                                                                          \
  }

/// 3 copies: aqua_rock/aqua_rock, aqua_rock/aqua_rock, aqua_rock/aqua_rock.
#define DEFINE_FUNC_020010F4(name, n1, n2, n3, n4)                             \
  void name(s32 unused, s32 id) {                                              \
    struct Actor *object = Actor_GetById(id);                                  \
                                                                               \
    object->priorityFlags = 0;                                                 \
    object->motionFlags = 0;                                                   \
    Func_02002960_(n1, n2, 1, 1, n3, n2);                                      \
    GameFlag_SetBit(n4);                                                       \
  }

/// 3 copies: kibombo/s2f34, shrine_of_the_sea_god/s1340, yampi_desert.
#define DEFINE_FUNC_02002D10(name, n1, call1)                                  \
  void name(void) {                                                            \
    if (GameFlag_Test(265)) {                                                  \
      if (GameFlag_Test(n1) == 0)                                              \
        call1();                                                               \
    } else {                                                                   \
      GameFlag_SetBit(n1);                                                     \
    }                                                                          \
  }

/// 2 copies: kibombo/s2f34, kibombo/s2f34.
#define DEFINE_FUNC_02003B20(name, n1, n2, n3, n4, n5, n6, n7)                 \
  void name(void) {                                                            \
    struct KibomboObject *leader =                                             \
        Actor_GetById(gMainRoutineWork.currentOwner);                          \
    s32 x = leader->x >> 20;                                                   \
    s32 z = leader->z >> 20;                                                   \
                                                                               \
    if (z == n1) {                                                             \
      if (x == n2 && (gInput.held & 16) == 0)                                  \
        return;                                                                \
      if (x == n3 && (gInput.held & 32) == 0)                                  \
        return;                                                                \
    } else if (x == n4 && z == n5 && (gInput.held & 64) == 0) {                \
      return;                                                                  \
    }                                                                          \
    Func_02003a08(n6, n7);                                                     \
  }

/// 2 copies: magma_rock/s0ff4, magma_rock/s0ff4.
#define DEFINE_FUNC_020019E8(name, n1, n2, n3)                                 \
  void name(s32 mode, s32 cue) {                                               \
    struct MagmaRockWork *work = Ram_HeapSlots->effectWork;                    \
    struct MainRoutineWork *party = &gMainRoutineWork;                         \
                                                                               \
    {                                                                          \
      s32 scene = party->scene;                                                \
                                                                               \
      if (scene == (s32)&SceneId_Ov691Room1)                                   \
        work->target = Actor_Get(n1);                                          \
      else if (scene == (s32)&SceneId_Ov691Room2)                              \
        work->target = Actor_Get(n2);                                          \
      Func_020014bc(mode, cue);                                                \
    }                                                                          \
    Event_SetFlag(n3);                                                         \
  }

/// 2 copies: airs_rock_inside/s2770, airs_rock_inside/s2770.
#define DEFINE_FUNC_020047B0(name, n1)                                         \
  void name(void) {                                                            \
    if (GameFlag_Test(0x8d4)) {                                                \
      Event_Start();                                                           \
      Event_ResolvePendingActions(0);                                          \
      FieldMessage_Show(MsgCheckedTheTablet, 1);                               \
      Event_End();                                                             \
    } else {                                                                   \
      Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, n1, 136);       \
      Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 264, 136);      \
      Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);       \
      FieldEvent_Wait(10);                                                     \
      Func_02004534();                                                         \
    }                                                                          \
  }

/// 2 copies: ankohl_ruins, ankohl_ruins.
#define DEFINE_FUNC_02001DDC(name, n1, n2, n3, n4, n5)                         \
  void name(void) {                                                            \
    if (AnkohlRuins_FlagCheck(n1) == 0) {                                      \
      Event_SetFlag(n1);                                                       \
      Event_PlaySound(584);                                                    \
      Event_Start();                                                           \
      FieldEvent_ResolvePendingActions(0);                                     \
      MapGrid_CopyCells(26, 70, 1, 3, n2, n3);                                 \
      FieldEvent_Wait(20);                                                     \
      Event_PlaySound(181);                                                    \
      MapGrid_CopyCells(26, 67, 1, 3, n2, n3);                                 \
      MapGrid_CopyCellAttributes(n4, n5, 1, 1, n2, n5);                        \
      Event_End();                                                             \
    }                                                                          \
  }

/// 2 copies: aqua_rock_inside/aqua_rock_inside,
/// aqua_rock_inside/aqua_rock_inside.
#define DEFINE_FUNC_02001CD8(name, n1)                                         \
  void name(void) {                                                            \
    if (Event_GetFlag(0x9ae)) {                                                \
      Event_Start();                                                           \
      FieldEvent_ResolvePendingActions(0);                                     \
      Message_ShowCentered(MsgCheckedTheTabletIt, 1);                          \
      Event_End();                                                             \
    } else {                                                                   \
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, n1, 184);             \
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 264, 184);            \
      Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);     \
      FieldEvent_Wait(10);                                                     \
      Func_02001a1c();                                                         \
    }                                                                          \
  }

/// 3 copies: islets, islets, izumo_ruins.
#define DEFINE_FUNC_020011B8(name, n1)                                         \
  void name(void) {                                                            \
    struct Actor *object0;                                                     \
                                                                               \
    object0 = Actor_Get(gMainRoutineWork.currentOwner);                        \
    object0->terrainId = n1;                                                   \
    return;                                                                    \
  }

/// 3 copies: jupiter_lighthouse_room_1, jupiter_lighthouse_room_6,
/// jupiter_lighthouse_room_7.
#define DEFINE_FUNC_02000160(name, n1, call1, call2)                           \
  void name(void) {                                                            \
    if (Event_GetFlag(n1) == 1) {                                              \
      Event_ClearFlag(n1);                                                     \
      call1();                                                                 \
    }                                                                          \
    call2();                                                                   \
  }

/// 3 copies: magma_rock/s0ff4, magma_rock/s0ff4, magma_rock/s0ff4.
#define DEFINE_FUNC_02001A8C(name, n1, n2)                                     \
  void name(s32 mode, s32 cue) {                                               \
    struct MagmaRockWork *work = Ram_HeapSlots->effectWork;                    \
                                                                               \
    work->target = Actor_Get(n1);                                              \
    Func_020014bc(mode, cue);                                                  \
    Event_SetFlag(n2);                                                         \
  }

/// 2 copies: magma_rock_inside/magma_rock_inside,
/// magma_rock_inside/magma_rock_inside.
#define DEFINE_FUNC_02003348(name, n1)                                         \
  void name(void) {                                                            \
    if (Event_GetFlag(0x9fa)) {                                                \
      Event_Start();                                                           \
      FieldEvent_ResolvePendingActions(0);                                     \
      Message_ShowCentered(MsgBlazeTabletInscription, 1);                      \
      Event_End();                                                             \
    } else {                                                                   \
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, n1, 136);             \
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 264, 136);            \
      Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);     \
      FieldEvent_Wait(10);                                                     \
      Func_020030cc();                                                         \
    }                                                                          \
  }

/// 3 copies: mars_lighthouse_room_f, mars_lighthouse_room_f,
/// mars_lighthouse_room_f.
#define DEFINE_FUNC_02000444(name, n1, n2, n3)                                 \
  void name(s32 state, s32 unused) {                                           \
    if (state == 1) {                                                          \
      MapGrid_CopyCells(29, 56, 3, 4, n1, 50);                                 \
      MapGrid_CopyCellsLayered(93, 56, 3, 4, n2, 50);                          \
      Event_SetFlag(n3);                                                       \
    }                                                                          \
  }

/// 2 copies: garoh/garoh, garoh/garoh.
#define DEFINE_FUNC_02000EB0(name, n1, n2, n3, n4)                             \
  void name(void) {                                                            \
    struct Actor *object = Actor_Get(n1);                                      \
    s32 x = object->x >> 20;                                                   \
    s32 raw;                                                                   \
    s32 z;                                                                     \
                                                                               \
    raw = object->z;                                                           \
    z = raw >> 20;                                                             \
    if (!Flag_IsSet(n2) && x == n3 && z == n4) {                               \
      Func_02000e78(n1, object);                                               \
      Flag_Set(n2);                                                            \
    }                                                                          \
  }

/// 2 copies: izumo_interiors, izumo_interiors.
#define DEFINE_FUNC_02000038_2(name, n1, n2)                                   \
  s32 name(void) {                                                             \
    struct Actor *object = Actor_GetById(n1);                                  \
                                                                               \
    if ((u32)(object->x - n2) <= 0x80000) {                                    \
      s32 z = object->z;                                                       \
                                                                               \
      if (z >= 0x26c0000) {                                                    \
        if (z <= 0x2740000)                                                    \
          return 0;                                                            \
      }                                                                        \
    }                                                                          \
    return 1;                                                                  \
  }

/// 2 copies: mars_lighthouse_room_f, mars_lighthouse_room_f.
#define DEFINE_FUNC_02000CF4(name, n1, n2)                                     \
  void name(s32 unused, s32 id) {                                              \
    Func_02000c80(id);                                                         \
    Flag_Set(n1);                                                              \
    if (Func_020027dc(n2) != 64) {                                             \
      struct Actor *object = Actor_Get(id);                                    \
                                                                               \
      WORD_AT(object, 108) = 0;                                                \
      Animation_ApplyChildValues(object, 0);                                   \
      Actor_SetAnimation(id, 6);                                               \
      Flag_Clear(n1);                                                          \
      Func_02000cb8(id);                                                       \
    }                                                                          \
  }

/// 2 copies: trial_road/s0218, trial_road/s0218.
#define DEFINE_FUNC_02000218(name, n1, n2, n3, n4, n5, n6)                     \
  void name(s32 side, s32 phase) {                                             \
    if (side == 0) {                                                           \
      if (phase == 3)                                                          \
        Func_0802b450(0, 120, 1, 2, n1, phase);                                \
      MapGrid_CopyCellsLayered(127, 3, 1, 2, n2, 3);                           \
      MapGrid_CopyCellAttributes(n3, 127, 1, 1, n1, 8);                        \
    } else {                                                                   \
      if (phase == 3)                                                          \
        Func_0802b450(0, 120, 1, 2, n4, phase);                                \
      MapGrid_CopyCellsLayered(127, 3, 1, 2, n5, 3);                           \
      MapGrid_CopyCellAttributes(n6, 127, 1, 1, n4, 8);                        \
    }                                                                          \
  }

/// 2 copies: yampi_desert_cave, yampi_desert_cave.
#define DEFINE_FUNC_02000E14(name, n1, id1, n2)                                \
  void name(s32 arg0) {                                                        \
    s32 result0;                                                               \
                                                                               \
    result0 = Event_GetFlag(n1);                                               \
    if (result0 != 0)                                                          \
      goto id1;                                                                \
    if (arg0 != 3)                                                             \
      goto id1;                                                                \
    Actor_SetHorizontalPositionWithTerrain(n2, 0, 0);                          \
    GameFlag_SetBit(n1);                                                       \
  id1:                                                                         \
    return;                                                                    \
  }

/// 3 copies: anemos_inner_sanctum/s0ac8, gaia_rock_inside/s1f68,
/// gaia_rock_inside/s1f68.
#define DEFINE_FUNC_02001644(name, n1)                                         \
  void name(void) {                                                            \
    s32 t0;                                                                    \
                                                                               \
    t0 = ((s32)Actor_Get(gMainRoutineWork.currentOwner));                      \
    FIELD_AT((t0 + 0x22), u8, 0) = n1;                                         \
  }

/// 3 copies: apojii_islands/apojii_islands, aqua_rock/aqua_rock,
/// aqua_rock/aqua_rock.
#define DEFINE_FUNC_02000780(name, n1)                                         \
  void name(struct Actor *object) {                                            \
    if (gFrameCount & 2)                                                       \
      Animation_ApplyChildValues(object, n1);                                  \
    else                                                                       \
      Animation_ApplyChildValues(object, 0);                                   \
  }

/// 2 copies: islets, lemurian_ship_deck/s0b84.
#define DEFINE_FUNC_02000DF8(name, n1, call1, data1)                           \
  void name(struct Actor *object) {                                            \
    struct Actor *spawned;                                                     \
                                                                               \
    if ((gFrameCount & n1) == 0) {                                             \
      spawned = FieldActor_Create(14, object->x, object->y, object->z);        \
      if (spawned != 0) {                                                      \
        Actor_SetSingleChildFlags(spawned, 0);                                 \
        call1(spawned, 1);                                                     \
        Actor_StartScript(spawned, data1);                                     \
      }                                                                        \
    }                                                                          \
  }

/// 3 copies: jupiter_lighthouse_room_1, jupiter_lighthouse_room_6,
/// jupiter_lighthouse_room_7.
#define DEFINE_FUNC_0200013C(name, n1, call1)                                  \
  void name(void) {                                                            \
    if (!Event_GetFlag(n1)) {                                                  \
      Event_SetFlag(n1);                                                       \
      call1(1);                                                                \
    }                                                                          \
  }

/// 2 copies: kandorean_temple/s2ec4, kandorean_temple/s2ec4.
#define DEFINE_FUNC_02000CF8(n1, n2, n3, n4, n5)                               \
  do {                                                                         \
    leader->y += -0x6c000;                                                     \
    statue->y += -0x6c000;                                                     \
    ride->y += -0x2b333;                                                       \
    limit = 0x1800;                                                            \
    Func_02000cf8(n1, 0x30000, limit);                                         \
    Func_02000cf8(n2, 0x24000, limit);                                         \
    Func_02000cf8(n3, 0x18000, limit);                                         \
    Func_02000cf8(n4, 0xc000, limit);                                          \
    Func_02000cf8(n5, 0, limit);                                               \
    wait(1);                                                                   \
  } while (Actor_GetById(n5)->scaleY > limit);

/// 2 copies: madra_catacombs, madra_catacombs.
#define DEFINE_FUNC_02000C94(name, n1)                                         \
  void name(void) {                                                            \
    Event_Start();                                                             \
    Event_ResolvePendingActions(0);                                            \
    FieldCamera_MoveTo(-1, -1, -1, 0);                                         \
    Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner),    \
                              0);                                              \
    Actor_SetPartAttribute(Actor_GetById(gMainRoutineWork.currentOwner), 15);  \
    wait(1);                                                                   \
    Event_StartScreenTransition();                                             \
    Event_WaitTransitionFrames();                                              \
    Event_Wait(40);                                                            \
    Import_EventSetExitRequest(n1);                                            \
  }

/// 3 copies: magma_rock, magma_rock/s0ff4, magma_rock_inside/magma_rock_inside.
#define DEFINE_FUNC_02000DA4(name, n1)                                         \
  void name(struct Actor *object) {                                            \
    if (gFrameCount & object->unk5c[7])                                        \
      Actor_SetSpriteColor(object, n1);                                        \
    else                                                                       \
      Actor_SetSpriteColor(object, 0);                                         \
  }

/// 2 copies: yampi_desert_cave, yampi_desert_cave.
#define DEFINE_FUNC_02000DAC_2(name, n1)                                       \
  void name(void) {                                                            \
    Func_02000898();                                                           \
    Func_02001958();                                                           \
    {                                                                          \
      u8 *party = (u8 *)&gMainRoutineWork;                                     \
                                                                               \
      party[0x2c2] = 2;                                                        \
    }                                                                          \
    Event_SetDestination((s32)&SceneId_Ov662Room1, n1);                        \
    Event_Wait(40);                                                            \
    Func_020008fc();                                                           \
  }

/// 2 copies: aqua_rock/aqua_rock, aqua_rock/aqua_rock.
#define DEFINE_FUNC_0200119C(name, n1, n2)                                     \
  void name(s32 unused, s32 id) {                                              \
    struct Actor *object = Actor_GetById(id);                                  \
    s32 column = object->x >> 20;                                              \
                                                                               \
    if (column == 53) {                                                        \
      wait(30);                                                                \
      object->z += 0x10000;                                                    \
      GameFlag_SetBit(n1);                                                     \
      Map_CopyCellAttributeRect(54, 58, 1, 1, column, n2);                     \
    }                                                                          \
  }

/// 2 copies: kibombo, kibombo.
#define DEFINE_FUNC_02002C34(name, n1, n2, n3, n4, call1, n5, n6, n7)          \
  void name(void) {                                                            \
    struct KibomboEventWork *work = Ram_HeapSlots->eventWork;                  \
                                                                               \
    if (work->field166 == 0 && work->field15a == 0 && work->field15e == 0 &&   \
        (gFrameCount & 7) == 0) {                                              \
      Func_02002b18(9, 0, ((Random16Far() * 10) >> 16) - 5);                   \
      Func_02002b18(n1, 1, ((Random16Far() * n2) >> 16) - n3);                 \
      Func_02002b18(n4, 2, ((Random16Far() * 4) >> 16) - 2);                   \
      call1(n5, 3, ((Random16Far() * n6) >> 16) - n7);                         \
    }                                                                          \
  }

/// 2 copies: islet_cave, islet_cave.
#define DEFINE_FUNC_02000508(name, n1, n2)                                     \
  void name(void) {                                                            \
    Func_020000f4();                                                           \
    Func_02001334();                                                           \
                                                                               \
    gMainRoutineWork.unk2c2 = 1;                                               \
    Event_Wait(40);                                                            \
    Actor_SetHorizontalPositionWithTerrain(gMainRoutineWork.currentOwner,      \
                                           PIXELS(n1), PIXELS(n2));            \
    Func_020004ac();                                                           \
  }

/// 2 copies: lemurian_ship, lemurian_ship.
#define DEFINE_FUNC_02001380(name, n1)                                         \
  void name(s32 id) {                                                          \
    struct LemurianShipObject *object =                                        \
        ((struct LemurianShipObject *)Actor_GetById(id));                      \
                                                                               \
    ((struct LemurianShipObject *)Actor_GetById(id))->priorityFlags |= 0x20;   \
    Actor_SetSpritePriority(id, n1);                                           \
    object->priorityFlags &= ~2;                                               \
    object->timer = 1;                                                         \
  }

/// 2 copies: shrine_of_the_sea_god/s1340, shrine_of_the_sea_god/s1340.
#define DEFINE_FUNC_0200291C(name, n1, n2, n3, n4)                             \
  void name(void) {                                                            \
    Func_0200279c();                                                           \
    Camera_MoveWithinMapBounds(n1 << 16, 16 << 16, n2 << 16, 1);               \
    ShrineOfTheSeaGod_ObjWalk(14, n3, 248);                                    \
    ShrineOfTheSeaGod_ObjWalk(14, n1, 248);                                    \
    Actor_SetPositionAndReset(14, n1, n2);                                     \
    Actor_GetById(14)->state = n4;                                             \
    Func_020027d4();                                                           \
  }

/// 2 copies: anemos_inner_sanctum/s0ac8, anemos_inner_sanctum/s0ac8.
#define DEFINE_FUNC_0200178C(name, n1)                                         \
  void name(void) {                                                            \
    s32 t0;                                                                    \
                                                                               \
    t0 = Func_020030e0();                                                      \
    if (t0 != 0) {                                                             \
      FieldEvent_SetExitRequest(n1);                                           \
    }                                                                          \
  }

/// 2 copies: champa_cliff_caves, champa_cliff_caves.
#define DEFINE_FUNC_02000038_3(name, n1)                                       \
  void name(struct RiseObject *object) {                                       \
    object->scale += n1;                                                       \
    if (object->scale >= 0x10000) {                                            \
      object->scale = 0x10000;                                                 \
      object->update = 0;                                                      \
    }                                                                          \
    object->scaleCopy = object->scale;                                         \
  }

/// 2 copies: garoh/s12a8, garoh/s12a8.
#define DEFINE_FUNC_02001418(name, n1, n2)                                     \
  void name(s32 actor) {                                                       \
    BattleEffect_LoadActionEffectResources(144, 1);                            \
    FieldPsynergy_SetupActorPair(actor, 0);                                    \
    FieldPsynergy_RunHandler(n1);                                              \
    BlendBack(n2, 5, 3, 3, 19, 5);                                             \
    BattleEffect_StopMainActor();                                              \
    BattleEffect_CleanupSceneObjects();                                        \
  }

/// 2 copies: jupiter_lighthouse_room_1, jupiter_lighthouse_room_7.
#define DEFINE_FUNC_02000100(name, n1, n2, n3, n4)                             \
  void name(void) {                                                            \
    struct Actor *object = Actor_Get(n1);                                      \
                                                                               \
    if (!Event_GetFlag(n2) && object->x >> 20 == n3 &&                         \
        object->z >> 20 == n4) {                                               \
      Event_SetFlag(n2);                                                       \
      Func_02000060(1);                                                        \
    }                                                                          \
  }

/// 2 copies: kandorean_temple/kandorean_temple,
/// kandorean_temple/kandorean_temple.
#define DEFINE_FUNC_02000BF4(name, data1, n1, n2, n3, n4)                      \
  void name(void) {                                                            \
    s32 position[3];                                                           \
                                                                               \
    if (data1 != 0 && gFrameCount % n1 == 0) {                                 \
      SET_VECTOR(position, n2, 0, n3);                                         \
      Func_0200098c(position, n4);                                             \
    }                                                                          \
  }

/// 2 copies: mars_lighthouse_room_m, mars_lighthouse_room_m.
#define DEFINE_FUNC_02001608(name, n1, n2)                                     \
  void name(void) {                                                            \
    gData_02003938 = 0;                                                        \
    gData_02003934 = Func_020016e8(gData_02003928);                            \
    gData_02003924 = n1;                                                       \
    gData_02003920 = n2;                                                       \
    gData_02003918 = 1;                                                        \
    Task_AddFrame(Func_0200170c, 0x480);                                       \
  }

/// 2 copies: sea_of_time, sea_of_time.
#define DEFINE_FUNC_020015CC(name, n1)                                         \
  void name(void) {                                                            \
    Event_Start();                                                             \
    Event_ResolvePendingActions(0);                                            \
    Import_EventSetExitRequest(n1);                                            \
    Event_FinishScreenTransition();                                            \
    Event_WaitTransitionFrames();                                              \
    Func_02001fb4();                                                           \
  }

/// 2 copies: shaman_village/s1fec, shaman_village/s1fec.
#define DEFINE_FUNC_020043C8(name, n1, n2)                                     \
  void name(void) {                                                            \
    s32 variant = ((struct ShamanVillageObject *)Actor_GetById(                \
                       gMainRoutineWork.currentOwner))                         \
                      ->sprite->priority;                                      \
                                                                               \
    Actor_SetSpritePriority(n1, variant);                                      \
    Actor_SetSpritePriority(n2, variant);                                      \
  }

/// 2 copies: taopo_swamp, taopo_swamp.
#define DEFINE_FUNC_020007BC(name, n1, n2, n3)                                 \
  void name(s32 a, s32 id) {                                                   \
    Func_02002840_(n1, n2, 1, 1, n3, n2);                                      \
    FieldActor_SetSpritePriority(id, 2);                                       \
    Actor_Get(id)->priorityFlags &= 0xfd;                                      \
    Func_02002778(0, id);                                                      \
    Event_SetFlag(0x221);                                                      \
    Event_ClearFlag(0x222);                                                    \
  }

/// 2 copies: trial_road/s15a8, trial_road/s15a8.
#define DEFINE_FUNC_0200163C(name, n1, n2)                                     \
  void name(void) {                                                            \
    MapGrid_CopyCells(n1, 30, 1, 1, 10, 11);                                   \
    Map_CopyCellsRect(n1, 30, 1, 1, 18, 7);                                    \
    Map_CopyCellsRect(n1, 30, 1, 1, 18, 15);                                   \
    Map_CopyCellsRect(n2, 30, 1, 1, 74, 11);                                   \
    Map_CopyCellsRect(n2, 30, 1, 1, 82, 7);                                    \
    Map_CopyCellsRect(n2, 30, 1, 1, 82, 15);                                   \
  }

/// 2 copies: jupiter_lighthouse_room_2, jupiter_lighthouse_room_2.
#define DEFINE_FUNC_020003F4(name, n1)                                         \
  void name(void) {                                                            \
    struct EventWork *event = Ram_HeapSlots->eventWork;                        \
    u8 *effect = Ram_HeapSlots->effectWork;                                    \
                                                                               \
    event->unk16a = n1;                                                        \
    effect[52] = 1;                                                            \
  }

/// 2 copies: magma_rock_inside/magma_rock_inside,
/// magma_rock_inside/magma_rock_inside.
#define DEFINE_FUNC_02001CA0(name, n1, n2)                                     \
  void name(s32 a, s32 b) {                                                    \
    struct Actor **work = Ram_HeapSlots->effectWork;                           \
                                                                               \
    work[5] = Actor_Get(n1);                                                   \
    Func_02001190(a, b);                                                       \
    Event_SetFlag(n2);                                                         \
  }

/// 2 copies: magma_rock_inside/magma_rock_inside,
/// magma_rock_inside/magma_rock_inside.
#define DEFINE_FUNC_0200217C(name, n1, n2, n3, n4)                             \
  void name(void) {                                                            \
    if (!Event_GetFlag(n1)) {                                                  \
      Actor_SetSpriteColor(Actor_Get(n2), 15);                                 \
      Actor_SetPosition(n2, PIXELS(n3), PIXELS(n4));                           \
      Actor_Get(n2)->scaleX = Actor_Get(n2)->scaleY = 0x1000;                  \
    }                                                                          \
  }

/// 2 copies: airs_rock_inside/s2770, airs_rock_inside/s2770.
#define DEFINE_FUNC_02003DD4(name, n1, n2)                                     \
  void name(void) {                                                            \
    Map_CopyCellAttributeRect(n1, 17, 1, 1, 8, 19);                            \
    Map_CopyCellAttributeRect(n1, 17, 1, 1, 24, 19);                           \
    Call6((void (*)())Map_CopyCellPictureIndicesRect, n2, 17, 72, 19, 1, 2);   \
    Map_CopyCellPictureIndicesRect(n2, 17, 88, 19, 1, 2);                      \
  }

/// 2 copies: jupiter_lighthouse_room_4, jupiter_lighthouse_room_4.
#define DEFINE_FUNC_020001DC(name, n1, call1)                                  \
  void name(s32 item) {                                                        \
    Event_SetFlag(n1);                                                         \
    Event_PlaySound(134);                                                      \
    call1();                                                                   \
    PartyInventory_Remove(item);                                               \
  }

/// 2 copies: kibombo/s2f34, kibombo/s2f34.
#define DEFINE_FUNC_020032B0(name, n1)                                         \
  void name(void) {                                                            \
    u32 i;                                                                     \
                                                                               \
    for (i = 0; i <= 7; i++)                                                   \
      Actor_SetModeById(i + 18, n1);                                           \
  }

/// 2 copies: lemurian_ship, lemurian_ship.
#define DEFINE_FUNC_020010D8(name, n1, n2)                                     \
  void name(void) {                                                            \
    Sound_Play(106);                                                           \
    Actor_SetModeById(n1, 2);                                                  \
    GameFlag_SetBit(n2);                                                       \
    Func_02000e2c();                                                           \
  }

/// 5 copies: jupiter_lighthouse_basement/s1094,
/// jupiter_lighthouse_entrance/s0eac, jupiter_lighthouse_room_1/s0f3c,
/// jupiter_lighthouse_room_2/s165c, jupiter_lighthouse_room_3/s1a7c.
#define DEFINE_FUNC_020011A0(name, data1, call1)                               \
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
            /* FAKEMATCH(barrier): an empty asm on the stored count; as no     \
             * asm, 2 of 268 instructions differ in tla-en. */                 \
            asm("" : "=m"(table->count) : "m"(table->count) : "r3");           \
            /* FAKEMATCH(barrier): an empty asm on the word over the count; as \
             * no asm, 2 of 268 instructions differ in tla-en. */              \
            asm("" : "=m"(*(s32 *)&table->z));                                 \
          }                                                                    \
        }                                                                      \
      columnNext:                                                              \
        /* FAKEMATCH(barrier): an empty asm lengthening the table's life; as   \
         * no asm, 224 of 268 instructions differ in tla-en. */                \
        asm("");                                                               \
        /* FAKEMATCH(barrier): an empty asm lengthening the table's life; as   \
         * no asm, 224 of 268 instructions differ in tla-en. */                \
        asm("");                                                               \
        /* FAKEMATCH(barrier): an empty asm lengthening the table's life; as   \
         * no asm, 224 of 268 instructions differ in tla-en. */                \
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
    table->tile = Resource_FindFreeEntry();                                    \
    Vram_Load(table->tile, 0x100, buffer);                                     \
    Heap_Release(buffer);                                                      \
    Task_AddFrame(call1, 0x480);                                               \
    return table->count;                                                       \
  }

/// 7 copies: jupiter_lighthouse_room_4, jupiter_lighthouse_room_6,
/// jupiter_lighthouse_room_7, jupiter_lighthouse_room_8,
/// jupiter_lighthouse_room_9, jupiter_lighthouse_room_m,
/// mars_lighthouse_room_f.
#define DEFINE_FUNC_02000628(name, call1, call2, data1, data2)                 \
  void name(struct Actor *self) {                                              \
    u8 *work = Ram_HeapSlots->eventWork;                                       \
    struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);           \
    s32 position[3];                                                           \
    s32 distance;                                                              \
    s32 near;                                                                  \
                                                                               \
    if (*(s16 *)(work + 358) != 0 || *(s16 *)(work + 346) != 0 ||              \
        *(s16 *)(work + 350) != 0 || *(s16 *)(work + 360) != 0) {              \
      Actor_SetAnimationSpeed(self, 0);                                        \
      return;                                                                  \
    }                                                                          \
    Actor_SetAnimationSpeed(self, 16);                                         \
    if ((++*(u16 *)&self->timer & 31) == 31)                                   \
      Event_PlaySound(231);                                                    \
    {                                                                          \
      s8 *to = self->sprite;                                                   \
      s8 *from = leader->sprite;                                               \
      u32 a = from[9] & 12;                                                    \
      to[9] = (s8)((to[9] & ~12) | a);                                         \
    }                                                                          \
    if (call1(self->x, self->z, 2) != 255) {                                   \
      position[0] = self->x;                                                   \
      position[1] = self->y;                                                   \
      position[2] = self->z;                                                   \
      Camera_WorldToScreen(position);                                          \
      distance = position[0];                                                  \
      if (distance > 0x1100000)                                                \
        goto stop;                                                             \
      if (distance < -0x200000)                                                \
        goto stop;                                                             \
      {                                                                        \
        if (*((u8 *)self + 0x62) == 0) {                                       \
          distance = leader->y - self->y;                                      \
          if (distance < 0)                                                    \
            distance = self->y - leader->y;                                    \
          near = 0;                                                            \
          if (call2(&leader->x, &self->x) <= 12 && distance < 0xc0000)         \
            near = 1;                                                          \
          if (near && !Event_GetFlag(260)) {                                   \
            leader->facing = self->facing;                                     \
            leader->watchedObject = self;                                      \
            leader->priorityFlags &= ~1;                                       \
            {                                                                  \
              s32 delay = 200;                                                 \
              *(u16 *)(work + 362) = delay;                                    \
            }                                                                  \
            gMainRoutineWork.renderMode = 0;                                   \
            *((u8 *)self + 0x62) = 1;                                          \
            *((u8 *)self + 0x63) = 1;                                          \
          }                                                                    \
        }                                                                      \
        {                                                                      \
          s32 (*multiply)(s32, s32);                                           \
          s32 cosine = Math_Cosine(self->facing);                              \
          s32 speed = self->speedLimit;                                        \
          /* FAKEMATCH(dead): the routine address taken inside a once-only     \
           * block, so the speed loads first; as a plain call, 2 of 197        \
           * instructions differ in tla-en. */                                 \
          do {                                                                 \
            multiply = Iwram_MulQ16;                                           \
          } while (0);                                                         \
          self->x += multiply(cosine, speed);                                  \
        }                                                                      \
        self->z += Iwram_MulQ16(Math_Sin(self->facing), self->speedLimit);     \
        return;                                                                \
      }                                                                        \
    }                                                                          \
  stop:                                                                        \
    {                                                                          \
      s32 zero = 0;                                                            \
      *((u8 *)self + 0x63) = zero;                                             \
      self->update = zero;                                                     \
      Actor_StartScript(self, data1);                                          \
      Event_PlaySound(228);                                                    \
      data2 = zero;                                                            \
    }                                                                          \
  }

/// 4 copies: mars_lighthouse_entrance/s263c, mars_lighthouse_room_f/s3d80,
/// mars_lighthouse_star_room/s2148, prox/prox.
#define DEFINE_FUNC_02002DC4(name, call1, data1, call2, call3, call4)          \
  void name(void) {                                                            \
    struct Position probe;                                                     \
    s32 startBehavior;                                                         \
    s32 x;                                                                     \
    s32 z;                                                                     \
    s32 startX;                                                                \
    s32 startZ;                                                                \
    u8 *layer;                                                                 \
    s32 radius;                                                                \
    struct Actor *actor;                                                       \
    struct Position *position;                                                 \
    struct Position *next;                                                     \
    s32 heading;                                                               \
    s32 nextBehavior;                                                          \
    s32 distance;                                                              \
    struct Actor *other;                                                       \
    u8 *flags;                                                                 \
    s32 i;                                                                     \
    s32 dx;                                                                    \
    s32 dz;                                                                    \
                                                                               \
    actor = ObjectTable_Get(gMainRoutineWork.currentOwner);                    \
                                                                               \
  top:                                                                         \
    call1();                                                                   \
    actor->speedLimit = PIXELS(1);                                             \
    actor->acceleration = PIXELS(1) / 2;                                       \
    actor->targetX = 0x80000000;                                               \
    actor->targetZ = 0x80000000;                                               \
    actor->velocityX = 0;                                                      \
    actor->velocityZ = 0;                                                      \
    position = &probe;                                                         \
    position->x = startX = (actor->x & 0xfff00000) + PIXELS(8);                \
    position->y = actor->y;                                                    \
    position->z = startZ = (actor->z & 0xfff00000) + PIXELS(8);                \
    x = startX;                                                                \
    z = startZ;                                                                \
    layer = &actor->terrainId;                                                 \
    startBehavior = MapGrid_GetCellBehaviorAt(*layer, x, z);                   \
    dz = z - actor->z;                                                         \
    dx = x - actor->x;                                                         \
    distance = Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz);                    \
    if (distance < PIXELS(4)) {                                                \
      heading = data1[(gInput.held >> 4) & 15];                                \
      if ((heading << 16) == -0x10000) {                                       \
        return;                                                                \
      }                                                                        \
    } else {                                                                   \
      heading = (s16)ArcTan2(z - actor->z, x - actor->x);                      \
      heading = (s16)((heading + 0x2000) & -0x4000);                           \
    }                                                                          \
    FieldVector_AddPolarOffset(PIXELS(16), heading, &probe);                   \
    nextBehavior =                                                             \
        MapGrid_GetCellBehaviorAt(*layer, position->x, position->z);           \
    if (nextBehavior == 255 ||                                                 \
        Map_GetTerrainHeight(*layer, position->x, position->z) - actor->y >    \
            PIXELS(8)) {                                                       \
      actor->facing = heading;                                                 \
      actor->velocityX = 0;                                                    \
      actor->velocityZ = 0;                                                    \
      actor->x = startX;                                                       \
      actor->z = startZ;                                                       \
      goto done;                                                               \
    }                                                                          \
    position->x = startX;                                                      \
    position->z = startZ;                                                      \
    actor->speedLimit = PIXELS(2);                                             \
    actor->acceleration = 0x1999;                                              \
    actor->timer = 0;                                                          \
    FieldActor_SetMoveTarget(actor, startX, actor->y, startZ);                 \
    call2(actor, 2);                                                           \
    Actor_SetAnimationSpeed(actor, 48);                                        \
    Actor_WaitForMove(actor);                                                  \
    actor->update = call3;                                                     \
    goto test;                                                                 \
  body:                                                                        \
    if (Map_GetTerrainHeight(*layer, next->x, next->z) - actor->y >            \
        PIXELS(8)) {                                                           \
      goto stop;                                                               \
    }                                                                          \
    radius = actor->radius - 2;                                                \
    other = Ram_HeapSlots->scriptObjects;                                      \
    flags = &other->collisionFlags;                                            \
    for (i = 0; i < 64; i++, flags += 128, other++) {                          \
      if (other->script != NULL && (*flags & 1) && other != actor) {           \
        /* FAKEMATCH(barrier): the empty asm clobbering sfp and r4 and reading \
         * next and actor; as no asm, 85 of 304 instructions differ in tla-en. \
         */                                                                    \
        asm(""                                                                 \
            :                                                                  \
            : "r"(next), "r"(next), "r"(actor), "r"(actor), "r"(actor),        \
              "r"(actor), "r"(actor), "r"(actor), "r"(actor), "r"(actor)       \
            : SFP_CLOBBER, "r4");                                              \
        if (call4(&other->x, radius, &probe, other->radius - 2) >= 0) {        \
          goto stop;                                                           \
        }                                                                      \
      }                                                                        \
    }                                                                          \
    x = next->x;                                                               \
    z = next->z;                                                               \
    actor->speedLimit = PIXELS(2);                                             \
    actor->acceleration = 0x1999;                                              \
    FieldActor_SetMoveTarget(actor, next->x, next->y, next->z);                \
    Actor_WaitForMove(actor);                                                  \
    if (nextBehavior != startBehavior) {                                       \
      goto blocked;                                                            \
    }                                                                          \
  test:                                                                        \
    {                                                                          \
      struct Position *step;                                                   \
                                                                               \
      step = &probe;                                                           \
      FieldVector_AddPolarOffset(PIXELS(16), heading, step);                   \
      /* FAKEMATCH(dead): the unread store ends CSE's knowledge of the         \
       * address, so next rebuilds it; as no store, 59 of 304 instructions     \
       * differ in tla-en.                                                     \
       */                                                                      \
      step = NULL;                                                             \
    }                                                                          \
    next = &probe;                                                             \
    nextBehavior = MapGrid_GetCellBehaviorAt(*layer, next->x, next->z);        \
    if (nextBehavior != 255) {                                                 \
      goto body;                                                               \
    }                                                                          \
  stop:                                                                        \
    actor->speedLimit = PIXELS(2);                                             \
    actor->acceleration = PIXELS(1);                                           \
    FieldActor_SetMoveTarget(actor, x, actor->y, z);                           \
    Actor_WaitForMove(actor);                                                  \
    System_Wait(2);                                                            \
    goto top;                                                                  \
                                                                               \
  blocked:                                                                     \
    System_Wait(10);                                                           \
  done:                                                                        \
    actor->update = NULL;                                                      \
    actor->actionFlags |= 1;                                                   \
    actor->acceleration = PIXELS(1) / 4;                                       \
    call2(actor, 1);                                                           \
  }

/// 4 copies: mars_lighthouse_entrance/s3884, mars_lighthouse_room_f/s3d80,
/// mars_lighthouse_star_room/set_data, prox/prox.
#define DEFINE_FUNC_02003390(name, call1, call2, call3, call4)                 \
  void name(void) {                                                            \
    struct Position probe;                                                     \
    s32 heading;                                                               \
    s32 startBehavior;                                                         \
    s32 x;                                                                     \
    s32 z;                                                                     \
    s32 radius;                                                                \
    struct Actor *actor;                                                       \
    struct Position *next;                                                     \
    s32 nextBehavior;                                                          \
    u8 *layer;                                                                 \
    struct Actor *other;                                                       \
    struct Position *position;                                                 \
    u8 *flags;                                                                 \
    s32 i;                                                                     \
    s32 cell;                                                                  \
    s32 angle;                                                                 \
    s32 zero;                                                                  \
    struct Position *step;                                                     \
    s32 startX;                                                                \
    s32 startZ;                                                                \
                                                                               \
    actor = ObjectTable_Get(gMainRoutineWork.currentOwner);                    \
    heading = (s16)((actor->facing + 0x2000) & -0x4000);                       \
                                                                               \
  top:                                                                         \
    call1();                                                                   \
    actor->speedLimit = PIXELS(1);                                             \
    actor->acceleration = PIXELS(1) / 2;                                       \
    actor->targetX = 0x80000000;                                               \
    actor->targetZ = 0x80000000;                                               \
    /* Default: one zero, spilled and reloaded at each use. */                 \
    zero = 0;                                                                  \
    actor->velocityX = zero;                                                   \
    actor->velocityZ = zero;                                                   \
    /* FAKEMATCH(temporary): cell holds the masked x; as one expression, 2 of  \
     * 246 instructions differ in tla-en. */                                   \
    cell = actor->x & 0xfff00000;                                              \
    /* FAKEMATCH(temporary): i holds half a cell here and the search's count   \
     * later; as a separate PIXELS(8), 18 of 246 instructions differ in        \
     * tla-en. */                                                              \
    i = PIXELS(8);                                                             \
    position = &probe;                                                         \
    position->x = startX = cell + i;                                           \
    position->y = actor->y;                                                    \
    position->z = startZ = (actor->z & 0xfff00000) + i;                        \
    x = startX;                                                                \
    z = startZ;                                                                \
    startBehavior = MapGrid_GetCellBehaviorAt(*(layer = &actor->terrainId),    \
                                              startX, startZ);                 \
    FieldVector_AddPolarOffset(PIXELS(16), heading, position);                 \
    nextBehavior =                                                             \
        MapGrid_GetCellBehaviorAt(*layer, position->x, position->z);           \
    if (nextBehavior == 255 ||                                                 \
        Map_GetTerrainHeight(*layer, position->x, position->z) - actor->y >    \
            i) {                                                               \
      actor->facing = heading;                                                 \
      actor->velocityX = 0;                                                    \
      actor->velocityZ = 0;                                                    \
      actor->x = startX;                                                       \
      actor->z = startZ;                                                       \
      goto done;                                                               \
    }                                                                          \
    position->x = x;                                                           \
    position->z = z;                                                           \
    actor->speedLimit = PIXELS(2);                                             \
    actor->acceleration = 0x1999;                                              \
    actor->timer = zero;                                                       \
    call2(actor, 2);                                                           \
    Actor_SetAnimationSpeed(actor, 48);                                        \
    actor->update = call3;                                                     \
    /* FAKEMATCH(barrier): the empty asm clobbering sfp and reading            \
     * nextBehavior twice; as no asm, 18 of 246 instructions differ in tla-en. \
     */                                                                        \
    asm("" : : "r"(nextBehavior), "r"(nextBehavior) : SFP_CLOBBER);            \
    goto test;                                                                 \
  body:                                                                        \
    if (Map_GetTerrainHeight(*layer, next->x, next->z) - actor->y >            \
        PIXELS(8)) {                                                           \
      goto stop;                                                               \
    }                                                                          \
    radius = actor->radius - 2;                                                \
    other = Ram_HeapSlots->scriptObjects;                                      \
    flags = &other->collisionFlags;                                            \
    for (i = 0; i < 64; i++, flags += 128, other++) {                          \
      if (other->script != NULL && (*flags & 1) && other != actor) {           \
        /* FAKEMATCH(barrier): the empty asm clobbering sfp and r4 and reading \
         * next and actor; as no asm, 170 of 246 instructions differ in        \
         * tla-en. */                                                          \
        asm(""                                                                 \
            :                                                                  \
            : "r"(next), "r"(next), "r"(actor), "r"(actor), "r"(actor),        \
              "r"(actor), "r"(actor), "r"(actor), "r"(actor), "r"(actor)       \
            : SFP_CLOBBER, "r4");                                              \
        if (call4(&other->x, radius, &probe, other->radius - 2) >= 0) {        \
          goto stop;                                                           \
        }                                                                      \
      }                                                                        \
    }                                                                          \
    x = next->x;                                                               \
    z = next->z;                                                               \
    actor->speedLimit = PIXELS(2);                                             \
    actor->acceleration = 0x1999;                                              \
    FieldActor_SetMoveTarget(actor, next->x, next->y, next->z);                \
    Actor_WaitForMove(actor);                                                  \
    if (nextBehavior != startBehavior) {                                       \
      goto blocked;                                                            \
    }                                                                          \
  test:                                                                        \
    /* FAKEMATCH(barrier): the asm tied to heading, with an r4 clobber so PRE  \
     * leaves it, loads the angle ahead of the address; as heading in the      \
     * call, 44 of 246 instructions differ in tla-en. */                       \
    asm("" : "=r"(angle) : "0"(heading) : "r4");                               \
    step = &probe;                                                             \
    FieldVector_AddPolarOffset(PIXELS(16), angle, step);                       \
    /* FAKEMATCH(dead): the unread store ends CSE's knowledge of the address,  \
     * so next rebuilds it; as &probe in the call, 97 of 246 instructions      \
     * differ in tla-en. */                                                    \
    step = NULL;                                                               \
    next = &probe;                                                             \
    nextBehavior = MapGrid_GetCellBehaviorAt(*layer, next->x, next->z);        \
    if (nextBehavior != 255) {                                                 \
      goto body;                                                               \
    }                                                                          \
  stop:                                                                        \
    actor->speedLimit = PIXELS(2);                                             \
    actor->acceleration = PIXELS(1);                                           \
    FieldActor_SetMoveTarget(actor, x, actor->y, z);                           \
    Actor_WaitForMove(actor);                                                  \
    System_Wait(2);                                                            \
    goto top;                                                                  \
                                                                               \
  blocked:                                                                     \
    System_Wait(10);                                                           \
  done:                                                                        \
    actor->update = NULL;                                                      \
    actor->actionFlags |= 1;                                                   \
    actor->acceleration = PIXELS(1) / 4;                                       \
    call2(actor, 1);                                                           \
  }

/// 18 copies: anemos_inner_sanctum/move, jupiter_lighthouse_aerie/s1e78,
/// jupiter_lighthouse_aerie_beacon/s4ec8, jupiter_lighthouse_basement/s1094,
/// jupiter_lighthouse_entrance/s0eac, jupiter_lighthouse_room_1/s0f3c,
/// jupiter_lighthouse_room_2/s165c, jupiter_lighthouse_room_3/s1a7c,
/// jupiter_lighthouse_room_4/s1258, jupiter_lighthouse_room_5/s1c88,
/// jupiter_lighthouse_room_6/s0f5c, jupiter_lighthouse_room_7/s1f74,
/// jupiter_lighthouse_room_8/s1600, jupiter_lighthouse_room_9/s1018,
/// jupiter_lighthouse_room_m/s3b7c, mars_lighthouse_entrance/s3cf8,
/// mars_lighthouse_room_f/s35b8, mars_lighthouse_star_room/s3804.
#define DEFINE_FUNC_0200146C(name, call1)                                      \
  void name(struct CellTarget *table, struct CellTargetEntry *entry) {         \
    struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);           \
    s16 sourceX;                                                               \
    s32 leaderX = leader->x;                                                   \
    s32 x;                                                                     \
    s32 z;                                                                     \
                                                                               \
    /* FAKEMATCH(barrier): passes the source column through an asm reading the \
     * leader's x, so the x load ties the 64 mask's copy and wins on order; as \
     * (leader->x >> 20) | (s16)(table->x & 64), 2 of 49 instructions differ   \
     * in tla-en. */                                                           \
    asm("" : "=r"(sourceX) : "0"(table->x), "r"(leaderX));                     \
    x = (leaderX >> 20) | (s16)(sourceX & 64);                                 \
    z = CellTarget_CellWithBit(leader->z, table->z & 64);                      \
    call1();                                                                   \
    Event_PlaySound(161);                                                      \
    MapGrid_CopyCells(table->x, table->z + 1, 1, 1, x, z);                     \
    FieldEvent_Wait(12);                                                       \
  }

/// 5 copies: jupiter_lighthouse_basement, jupiter_lighthouse_entrance/dist,
/// jupiter_lighthouse_room_1/s0674, jupiter_lighthouse_room_2/s0d94,
/// jupiter_lighthouse_room_3/s07b0.
#define DEFINE_FUNC_020005E4(name, call1, call2, data1, data2)                 \
  void name(struct Actor *self) {                                              \
    struct EventWork *work = Ram_HeapSlots->eventWork;                         \
    struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);           \
    s32 position[3];                                                           \
    s32 distance;                                                              \
    s32 near;                                                                  \
                                                                               \
    if (work->unk166 != 0 || *(s16 *)&work->unk15a != 0 ||                     \
        *(s16 *)&work->unk15c[2] != 0 || *(s16 *)work->unk168 != 0) {          \
      Actor_SetAnimationSpeed(self, 0);                                        \
      return;                                                                  \
    }                                                                          \
    Actor_SetAnimationSpeed(self, 16);                                         \
    if ((++*(u16 *)&self->timer & 31) == 31)                                   \
      Event_PlaySound(231);                                                    \
    ((struct FieldSprite *)self->sprite)->priority =                           \
        ((struct FieldSprite *)leader->sprite)->priority;                      \
    if (call1(self->x, self->z, 2) != 255) {                                   \
      position[0] = self->x;                                                   \
      position[1] = self->y;                                                   \
      position[2] = self->z;                                                   \
      Camera_WorldToScreen(position);                                          \
      distance = position[0];                                                  \
      if (distance > 0x1100000)                                                \
        goto stop;                                                             \
      if (distance < -0x200000)                                                \
        goto stop;                                                             \
      {                                                                        \
        if (*((u8 *)self + 0x62) == 0) {                                       \
          distance = leader->y - self->y;                                      \
          if (distance < 0)                                                    \
            distance = self->y - leader->y;                                    \
          near = 0;                                                            \
          if (call2(&leader->x, &self->x) <= 12 && distance < 0xc0000)         \
            near = 1;                                                          \
          if (near && !Event_GetFlag(260)) {                                   \
            leader->facing = self->facing;                                     \
            leader->watchedObject = self;                                      \
            leader->priorityFlags &= ~1;                                       \
            work->unk16a = 200;                                                \
            gMainRoutineWork.renderMode = 0;                                   \
            *((u8 *)self + 0x62) = 1;                                          \
            *((u8 *)self + 0x63) = 1;                                          \
          }                                                                    \
        }                                                                      \
        /* IwramMulQ16 reached from IwramIrqMain rather than at its fixed      \
         * address: */                                                         \
        /* the address is a linked word read after the cosine, as the native   \
         */                                                                    \
        /* reads it after the speed (at Iwram_MulQ16 it is built before). */   \
        self->x += ((s32 (*)(s32, s32))(IwramIrqMain + Iwram_MulQ16Offset))(   \
            Math_Cosine(self->facing), self->speedLimit);                      \
        self->z += ((s32 (*)(s32, s32))(IwramIrqMain + Iwram_MulQ16Offset))(   \
            Math_Sin(self->facing), self->speedLimit);                         \
        return;                                                                \
      }                                                                        \
    }                                                                          \
  stop:                                                                        \
    {                                                                          \
      s32 zero = 0;                                                            \
                                                                               \
      /* FAKEMATCH(temporary): one word zero clears the ride byte, then the    \
       * update; as self->update = 0; *((u8 *)self + 0x63) = 0;, 4 of 197      \
       * instructions differ in tla-en (byte first, 15). */                    \
      *((u8 *)self + 0x63) = zero;                                             \
      self->update = (void *)zero;                                             \
    }                                                                          \
    Actor_StartScript(self, data1);                                            \
    Event_PlaySound(228);                                                      \
    data2 = 0;                                                                 \
  }

/// 4 copies: mars_lighthouse_entrance/mars_lighthouse_entrance,
/// mars_lighthouse_room_f, mars_lighthouse_room_m, mars_lighthouse_star_room.
#define DEFINE_FUNC_02001BF8(name, call1, call2, call3, call4, call5)          \
  void name(void) {                                                            \
    s32 saved[2];                                                              \
    State *state;                                                              \
    Entry *entry;                                                              \
    Camera *camera;                                                            \
    Position *position;                                                        \
    s32 cameraX, cameraZ;                                                      \
    s32 v;                                                                     \
    s32 i;                                                                     \
                                                                               \
    state = (State *)(gData_020023c4 + 0x188);                                 \
    camera = IWRAM->camera;                                                    \
    entry = state->entries;                                                    \
    position = &camera->position;                                              \
    cameraX = position->x;                                                     \
    cameraX &= 0xffff0000;                                                     \
    cameraZ = position->z & 0xffff0000;                                        \
    saved[1] = (*(s32 **)camera)[1];                                           \
    saved[0] = IWRAM->unk_6c.word;                                             \
    if (state->unk_6 != 0 && state->unk_8 == 0 && state->callback != 0)        \
      state->callback();                                                       \
    v = (u16)state->unk_6;                                                     \
    state->unk_8 = v;                                                          \
    state->unk_6 = 0;                                                          \
    for (i = 3; i >= 0; i--, entry++) {                                        \
      Actor *actor;                                                            \
      s32 flag, x, y, unk6, value, dx, dz;                                     \
      u8 *mark;                                                                \
                                                                               \
      if (entry->active == 0)                                                  \
        continue;                                                              \
      actor = entry->actor;                                                    \
      if (actor == 0)                                                          \
        continue;                                                              \
      flag = GameFlag_Test(0x106);                                             \
      if (*(s16 *)(saved[0] + 358) != 0)                                       \
        flag |= 1;                                                             \
      mark = &actor->flag5b;                                                   \
      *mark = flag;                                                            \
      if (state->unk_e != 0)                                                   \
        *mark = GameFlag_Test(0x106);                                          \
      dx = actor->x;                                                           \
      dz = actor->z;                                                           \
      dx -= cameraX;                                                           \
      /* FAKEMATCH(barrier): the empty asm on dx changes its allocation rank;  \
       * as no asm, 94 of 182 instructions differ in tla-en. */                \
      asm("" : : "r"(dx));                                                     \
      y = actor->y;                                                            \
      dz -= cameraZ;                                                           \
      y = dz - y;                                                              \
      unk6 = entry->unk_6;                                                     \
      x = dx >> 16;                                                            \
      y = (y >> 16) - 8;                                                       \
      if (unk6 == 0) {                                                         \
        if ((u32)(x + 7) > 334)                                                \
          continue;                                                            \
        if (y <= -48)                                                          \
          continue;                                                            \
        if (y > 239)                                                           \
          continue;                                                            \
      }                                                                        \
      y = (u16)entry->timer;                                                   \
      if (entry->timer <= 0) {                                                 \
        y = entry->flags & 240;                                                \
        switch (y) {                                                           \
        case 128:                                                              \
          call1(&entry->actor->x, entry->unk_c, unk6);                         \
          break;                                                               \
        case 48:                                                               \
          call2(&entry->actor->x, entry->unk_c, unk6);                         \
          break;                                                               \
        case 32:                                                               \
          call3(&entry->actor->x, entry->unk_c, unk6);                         \
          break;                                                               \
        case 16:                                                               \
          call4(&entry->actor->x, entry->unk_c, unk6);                         \
          break;                                                               \
        case 0:                                                                \
          call5(&entry->actor->x, entry->unk_c, unk6);                         \
          break;                                                               \
        }                                                                      \
        value = 8;                                                             \
      } else {                                                                 \
        value = y - 1;                                                         \
      }                                                                        \
      entry->timer = value;                                                    \
    }                                                                          \
  }

/// 3 copies: mars_lighthouse_entrance/s263c, mars_lighthouse_room_f/flash,
/// mars_lighthouse_star_room/s2148.
#define DEFINE_FUNC_0200274C(name, id1, call1, call2, call3)                   \
  void name(struct Actor *actor) {                                             \
    struct Vector3 position;                                                   \
    struct WorkEvent *work = Ram_HeapSlots->eventWork;                         \
    s32 state = actor->timer;                                                  \
                                                                               \
    if (state == 0) {                                                          \
      s32 dz;                                                                  \
      s32 dx;                                                                  \
      s32 tileX;                                                               \
      struct MapCell *cellsA;                                                  \
      struct MapCell *cellsB;                                                  \
      s32 widthA;                                                              \
      s32 widthB;                                                              \
      s32 offset;                                                              \
      s32 zB;                                                                  \
      s32 tileZ;                                                               \
      s32 heightA;                                                             \
      s32 heightB;                                                             \
      s32 x;                                                                   \
      s32 z;                                                                   \
      s32 xA;                                                                  \
      s32 xB;                                                                  \
      s32 zA;                                                                  \
      s32 flag;                                                                \
      struct MapWork *map;                                                     \
      struct MapCell *cell;                                                    \
      struct WorkEvent *work2;                                                 \
      struct id1 *other;                                                       \
      struct Actor *target;                                                    \
                                                                               \
      flag = Event_GetFlag(0x106);                                             \
      if (work->unk166 != 0)                                                   \
        flag |= 1;                                                             \
      actor->movementState = flag;                                             \
                                                                               \
      position.x = actor->x;                                                   \
      position.y = actor->y;                                                   \
      position.z = actor->z;                                                   \
      FieldVector_AddPolarOffset(0x80000, actor->facing, &position);           \
      if (call1(0, position.x, position.z) == 7) {                             \
        actor->targetZ = 0x80000000;                                           \
        actor->targetY = 0x80000000;                                           \
        actor->targetX = 0x80000000;                                           \
        actor->acceleration = 0x4000;                                          \
        actor->timer = 1;                                                      \
        Event_PlaySound(145);                                                  \
        /* FAKEMATCH(wrapper): forwarded, so each argument builds its own      \
         * 0x50000; as a direct call, 6 of 362 instructions differ in tla-en.  \
         */                                                                    \
        MapGrid_SetShake(0x50000, 0x50000, 0x10000);                           \
        /* FAKEMATCH(wrapper): forwarded, so each argument builds its own -1;  \
         * as a direct call, 17 of 362 instructions differ in tla-en. */       \
        MapGrid_SetShake(-1, -1, 0xe666);                                      \
        if (actor->watchedObject != NULL)                                      \
          ((void (*)(struct Actor *))actor->watchedObject)(actor);             \
      }                                                                        \
                                                                               \
      map = Ram_HeapSlots->mapWork;                                            \
      tileX = actor->x >> 20;                                                  \
      tileZ = actor->z >> 20;                                                  \
      cellsA = &map->layers[1].cells[tileX + (tileZ << 7)];                    \
      cellsB = &map->layers[2].cells[tileX + (tileZ << 7)];                    \
      widthA = *(s32 *)&map->layers[1].unk08[0] >> 20;                         \
      heightA = *(s32 *)&map->layers[1].unk08[4] >> 20;                        \
      widthB = *(s32 *)&map->layers[2].unk08[0] >> 20;                         \
      heightB = *(s32 *)&map->layers[2].unk08[4] >> 20;                        \
                                                                               \
      for (dz = -1, zB = tileZ + heightB - 2, z = tileZ - 1,                   \
          zA = tileZ + heightA - 1;                                            \
           dz <= 1; zB++, z++, zA++, dz++) {                                   \
        for (dx = -1, xB = tileX + widthB - 1, x = tileX - 1,                  \
            xA = tileX + widthA - 1;                                           \
             dx <= 1; xB++, x++, xA++, dx++) {                                 \
          offset = ((dz << 7) + dx) * 4;                                       \
          cell = (struct MapCell *)(offset + (s32)cellsA);                     \
          if (cell->collisionCode == 77) {                                     \
            cell->collisionCode = 0;                                           \
            MapGrid_CopyCellsLayered(64, 64, 1, 1, xA, zA);                    \
            MapGrid_CopyCellAttributes(64, 64, 1, 1, x, z);                    \
            Event_PlaySound(0x23d);                                            \
            call2((x << 20) + 0x80000, actor->y, (z << 20) + 0x80000,          \
                  actor->facing);                                              \
          }                                                                    \
          cell = (struct MapCell *)(offset + (s32)cellsB);                     \
          if (cell->collisionCode == 77) {                                     \
            cell->collisionCode = 0;                                           \
            MapGrid_CopyCellsLayered(64, 64, 1, 2, xB, zB);                    \
            MapGrid_CopyCellAttributes(64, 64, 1, 1, x, z);                    \
            Event_PlaySound(0x23c);                                            \
            call2((x << 20) + 0x80000, actor->y, (z << 20) + 0x80000,          \
                  actor->facing);                                              \
          }                                                                    \
        }                                                                      \
      }                                                                        \
                                                                               \
      if (actor->scaleX < 0x10000) {                                           \
        actor->scaleX += 0x1000;                                               \
        actor->scaleY += 0x1000;                                               \
      }                                                                        \
      FieldActor_SetMoveTarget(actor, position.x, position.y, position.z);     \
                                                                               \
      work2 = Ram_HeapSlots->eventWork;                                        \
      other = (struct id1 *)(gData_020023c4 + 0x188);                          \
      target = Actor_Get(gMainRoutineWork.currentOwner);                       \
      if (ABS(actor->x - target->x) < 0x140000 &&                              \
          (u32)(actor->y - target->y + 0x7ffff) <= 0x1ffffe &&                 \
          ABS(actor->z - target->z) < 0x140000) {                              \
        other->unk04 = 2;                                                      \
        other->unk6c = actor;                                                  \
        work2->unk154 = other->unk0a + 2;                                      \
      }                                                                        \
    } else if (state == 1) {                                                   \
      actor->movementState = 0;                                                \
      if (actor->scaleX > 0) {                                                 \
        actor->scaleX -= 0x1000;                                               \
        actor->scaleY -= 0x1000;                                               \
        call3(actor);                                                          \
      } else {                                                                 \
        actor->z = 0;                                                          \
        actor->y = 0;                                                          \
        actor->x = 0;                                                          \
        actor->velocityZ = 0;                                                  \
        actor->velocityY = 0;                                                  \
        actor->velocityX = 0;                                                  \
        actor->update = NULL;                                                  \
      }                                                                        \
    }                                                                          \
  }

/// 3 copies: jupiter_lighthouse_basement/s185c,
/// jupiter_lighthouse_entrance/s1674, jupiter_lighthouse_room_1/s1704.
#define DEFINE_FUNC_02001674(name, call1)                                      \
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
      cosine = Math_Cosine(angle);                                             \
      /* FAKEMATCH(dead): spawn is always 1, a constant GCSE only folds after  \
       * its partial redundancy pass, so that pass leaves the velocity array's \
       * address in the loop, built after the cosine call as the native has    \
       * it; as the plain body, 4 of 169 instructions differ in tla-en. */     \
      if (spawn) {                                                             \
        velocity[0] = cosine;                                                  \
        velocity[1] = 0;                                                       \
        velocity[2] = Math_Sin(angle);                                         \
        velocity[0] += velocity[0] >> 1;                                       \
        velocity[0] += (Random16Far() * 0xc000 >> 16) - 0x6000;                \
        velocity[2] += (Random16Far() * 0x6000 >> 16) - 0x3000;                \
        call1(actor->x, actor->y, actor->z, velocity[0], velocity[1],          \
              velocity[2], 0x1090001, &options);                               \
      }                                                                        \
      i++;                                                                     \
    }                                                                          \
    if (i <= 16)                                                               \
      goto loop;                                                               \
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

/// 3 copies: jupiter_lighthouse_room_3/s0dcc, jupiter_lighthouse_room_8/s0c44,
/// mars_lighthouse_room_f/s3d80.
#define DEFINE_FUNC_02000C44(name, call1, data1, data2, call2)                 \
  s32 name(struct PushTarget *target) {                                        \
    struct Actor *object;                                                      \
    s32 direction;                                                             \
    s32 position[3];                                                           \
    s32 footprint;                                                             \
    s32 width;                                                                 \
    s32 height;                                                                \
    s32 dx;                                                                    \
    s32 dz;                                                                    \
    s32 count;                                                                 \
    s32 row;                                                                   \
    s32 col;                                                                   \
    u8 *layer;                                                                 \
                                                                               \
    object = call1(&direction, &target->id, &target->footprint);               \
    if (object != 0) {                                                         \
      footprint = target->footprint;                                           \
      width = (ABS(data1[footprint][0]) + ABS(data1[footprint][2])) >> 4;      \
      height = (ABS(data1[footprint][1]) + ABS(data1[footprint][3])) >> 4;     \
      dx = data2[direction] & 0xffff0000;                                      \
      dz = data2[direction] << 16;                                             \
      target->callback = 0;                                                    \
      count = 0;                                                               \
      layer = &object->terrainId;                                              \
      *layer = 2;                                                              \
      target->x = object->x + dx;                                              \
      target->z = object->z + dz;                                              \
      position[1] = object->y;                                                 \
      for (;;) {                                                               \
        position[2] = target->z + (data1[footprint][1] << 16);                 \
        for (row = 0; row < height; row++) {                                   \
          position[0] = target->x + (data1[footprint][0] << 16);               \
          for (col = 0; col < width; col++) {                                  \
            /* FAKEMATCH(wrapper): the call through Func_02002338_; as         \
             * Func_02002338(object, position), 2 of 171 instructions differ   \
             * in tla-en. */                                                   \
            if (call2(object, position) == 2)                                  \
              goto blocked;                                                    \
            position[0] += 0x100000;                                           \
          }                                                                    \
          position[2] += 0x100000;                                             \
        }                                                                      \
        target->x += dx;                                                       \
        count++;                                                               \
        target->z += dz;                                                       \
      }                                                                        \
    blocked:                                                                   \
      *layer = 0;                                                              \
      if (count == 0)                                                          \
        return 0;                                                              \
      target->x = object->x + dx * count;                                      \
      target->y = object->y;                                                   \
      target->z = object->z + dz * count;                                      \
      return 1;                                                                \
    }                                                                          \
    return 0;                                                                  \
  }

/// 2 copies: staff_credit, world_map/s3b38.
#define DEFINE_FUNC_02001BE0(name, data1, data2, data3, data4)                 \
  void name(s32 progress) {                                                    \
    s32 level = ((progress * 30 + 0x8000) >> 16) + 1;                          \
    s32 kind = 0;                                                              \
    s32 count, base, i;                                                        \
                                                                               \
    if (progress != 0)                                                         \
      kind = 100;                                                              \
    if (level <= 15 && (gFrameCount & 15) <= 1)                                \
      kind = 200;                                                              \
    if (level <= 7 && (gFrameCount & 7) <= 1)                                  \
      kind = 200;                                                              \
    if (kind + level == data1)                                                 \
      return;                                                                  \
    data1 = kind + level;                                                      \
    Call2((void (*)())Iwram_ClearWordsEntry, (s32)(data2), (s32)(0x100));      \
    {                                                                          \
      s32 (*copy)(void *, const void *, s32) = Iwram_CopyWordsEntry;           \
                                                                               \
      if (kind == 0) {                                                         \
        copy(data2, data3[8], 0x80);                                           \
        goto done;                                                             \
      }                                                                        \
      if (kind == 100)                                                         \
        copy(data2, data3[4], 0x80);                                           \
      else                                                                     \
        copy(data2, data3[8], 0x80);                                           \
    }                                                                          \
    base = 0;                                                                  \
    count = level;                                                             \
    if (count > 7) {                                                           \
      Iwram_CopyWordsEntry(data2, data3, 32);                                  \
      base = 8;                                                                \
      count -= 8;                                                              \
    }                                                                          \
    if (level > 15) {                                                          \
      Iwram_CopyWordsEntry(data2[1], data3[1], 32);                            \
      count = level;                                                           \
      base = 16;                                                               \
      count -= 16;                                                             \
    }                                                                          \
    if (level > 23) {                                                          \
      Iwram_CopyWordsEntry(data2[2], data3[2], 32);                            \
      count = level;                                                           \
      base = 24;                                                               \
      count -= 24;                                                             \
    }                                                                          \
    if (level > 30) {                                                          \
      Iwram_CopyWordsEntry(data2[3], data3[3], 32);                            \
      base = 0;                                                                \
      count = 0;                                                               \
    }                                                                          \
    if (count != 0) {                                                          \
      i = 0;                                                                   \
      if (i >= count)                                                          \
        goto done;                                                             \
    loop:                                                                      \
      {                                                                        \
        s32 j;                                                                 \
        u8 *source = &data3[base / 8][4] + i / 2;                              \
        u8 *dest = &data2[base / 8][4] + i / 2;                                \
        /* FAKEMATCH(barrier): the empty asm on dest changes its allocation    \
         * rank; as no asm, 22 of 181 instructions differ in tla-en. */        \
        asm("" : "=r"(dest) : "0"(dest));                                      \
                                                                               \
        j = 2;                                                                 \
        if ((i & 1) == 0) {                                                    \
          j = 1;                                                               \
        even:                                                                  \
          *dest = (*dest & 240) | (*source & 15);                              \
          j++;                                                                 \
          source += 4;                                                         \
          dest += 4;                                                           \
          if (j > 3)                                                           \
            goto next;                                                         \
          goto even;                                                           \
        } else {                                                               \
        oddLoop:                                                               \
          *dest = (*dest & 15) | (*source & 240);                              \
          j--;                                                                 \
          source += 4;                                                         \
          dest += 4;                                                           \
          if (j >= 0)                                                          \
            goto oddLoop;                                                      \
        }                                                                      \
      }                                                                        \
    next:                                                                      \
      i++;                                                                     \
      if (i < count)                                                           \
        goto loop;                                                             \
    }                                                                          \
  done:                                                                        \
    Vram_Load(data4, 0x100, data2);                                            \
  }

/// 2 copies: magma_rock_inside/magma_rock_inside, venus_lighthouse.
#define DEFINE_FUNC_0200038C(name, data1, call1, call2)                        \
  void name(void) {                                                            \
    struct FieldPosition dst;                                                  \
    struct StagedActor *lead;                                                  \
    struct StagedActor *next;                                                  \
    struct StagedActor *blocker;                                               \
    s32 dir;                                                                   \
    u32 step;                                                                  \
    s32 rate;                                                                  \
                                                                               \
    lead = (struct StagedActor *)Actor_GetById(gMainRoutineWork.currentOwner); \
    dir = lead->facing >> 12;                                                  \
    step = data1[dir];                                                         \
    dst.x = lead->x.fixed + (step & 0xffff0000);                               \
    dst.y = lead->y.fixed;                                                     \
    step <<= 16;                                                               \
    dst.z = lead->z.fixed + step;                                              \
    next = Actor_FindInCell(&dst, lead);                                       \
    if (next == 0)                                                             \
      return;                                                                  \
                                                                               \
    step = data1[dir];                                                         \
    dst.x = next->x.fixed + (step & 0xffff0000);                               \
    dst.y = next->y.fixed;                                                     \
    step <<= 16;                                                               \
    dst.z = next->z.fixed + step;                                              \
    blocker = Actor_FindInCell(&dst, next);                                    \
    if (blocker != 0 && (blocker->collisionFlags & 1) != 0)                    \
      return;                                                                  \
                                                                               \
    dst.x = next->x.fixed;                                                     \
    dst.y = next->y.fixed + 0x100000;                                          \
    dst.z = next->z.fixed;                                                     \
    blocker = Actor_FindInCell(&dst, next);                                    \
    if (blocker != 0 && (blocker->collisionFlags & 1) != 0)                    \
      return;                                                                  \
                                                                               \
    next->transitionMode = 2;                                                  \
    step = data1[dir];                                                         \
    dst.x = next->x.fixed + (step & 0xffff0000);                               \
    dst.y = next->y.fixed;                                                     \
    step <<= 16;                                                               \
    dst.z = next->z.fixed + step;                                              \
    if (call1(next, &dst) > 0)                                                 \
      return;                                                                  \
                                                                               \
    if (next->transitionBusy != 0)                                             \
      return;                                                                  \
                                                                               \
    call2(lead, 8);                                                            \
    rate = 0x3333;                                                             \
    System_Wait(15);                                                           \
    Event_PlaySound(185);                                                      \
    next->moveRateX = rate;                                                    \
    next->moveRateZ = rate;                                                    \
    FieldActor_SetMoveTarget(next, dst.x, dst.y, dst.z);                       \
    lead->moveRateX = rate;                                                    \
    lead->moveRateZ = rate;                                                    \
    FieldActor_SetMoveTarget(lead, dst.x, dst.y, dst.z);                       \
    Actor_WaitForMove(next);                                                   \
    Event_NoOp();                                                              \
    next->x.fixed = dst.x;                                                     \
    {                                                                          \
      /* FAKEMATCH(temporary): z read ahead of velocityX's store; as dst.z at  \
       * its store, 4 of 175 instructions differ in tla-en. */                 \
      s32 z = dst.z;                                                           \
                                                                               \
      next->velocityX = 0;                                                     \
      next->z.fixed = z;                                                       \
      next->velocityZ = 0;                                                     \
    }                                                                          \
    lead->targetX = 0x80000000;                                                \
    lead->targetZ = 0x80000000;                                                \
    lead->velocityX = 0;                                                       \
    lead->velocityZ = 0;                                                       \
    lead->x.fixed = lead->x.part.pixel << 16;                                  \
    lead->z.fixed = lead->z.part.pixel << 16;                                  \
    call2(lead, 1);                                                            \
  }

/// 3 copies: mars_lighthouse_entrance/s263c, mars_lighthouse_room_f/flash,
/// mars_lighthouse_room_m/s2708.
#define DEFINE_FUNC_020029F4(name, data1)                                      \
  void name(s32 x, s32 y, s32 z, s32 speed) {                                  \
    s32 i;                                                                     \
                                                                               \
    i = 1;                                                                     \
  next:                                                                        \
    {                                                                          \
      struct Actor *object = FieldActor_Create(325, x, y, z);                  \
                                                                               \
      if (object != NULL) {                                                    \
        s32 angle;                                                             \
        s32 rise;                                                              \
        s32 radius;                                                            \
        u32 size;                                                              \
        s32 scale;                                                             \
                                                                               \
        rise = speed + (Random16Far() >> 4) - (Random16Far() >> 4);            \
        angle = 0x4000 + rise;                                                 \
        radius = Random16Far() * 8;                                            \
        size = Random16Far();                                                  \
        Actor_StartScript(object, data1);                                      \
        Actor_SetSingleChildFlags(object, 0);                                  \
        /* FAKEMATCH(temporary): the scale is taken before the velocity        \
         * stores, as in the native; as object->scaleX = (size >> 1) + 0x4000, \
         * 14 of 98 instructions differ in tla-en. */                          \
        scale = (size >> 1) + 0x4000;                                          \
        object->velocityY = rise + 0x14000;                                    \
        object->velocityZ = Iwram_MulQ16(radius, Math_Cosine(angle));          \
        object->velocityX = Iwram_MulQ16(radius, Math_Sin(angle));             \
        object->acceleration = 0;                                              \
        object->gravity = 0x547a;                                              \
        object->scaleX = scale;                                                \
        object->scaleY = object->scaleX;                                       \
        object->bounce = 0x8000;                                               \
        Animation_ApplyChildValues(object, 1);                                 \
      }                                                                        \
    }                                                                          \
    if (--i >= 0)                                                              \
      goto next;                                                               \
  }

/// 3 copies: eastern_alhafra, magma_rock_inside/s46e8, prox/prox.
#define DEFINE_FUNC_02003F48(name)                                             \
  void name(s32 x, s32 y, s32 z, s32 count) {                                  \
    struct RockWork *work;                                                     \
    s32 i;                                                                     \
                                                                               \
    /* FAKEMATCH(barrier): an empty asm before the work is read keeps the      \
     * spills of x and y at the entry; as an initializer, 4 of 88 instructions \
     * differ in tla-en. */                                                    \
    asm("");                                                                   \
    work = Ram_HeapSlots->menuRuntime;                                         \
    /* A goto loop carries no loop notes, so the offsets are rebuilt on every  \
     */                                                                        \
    /* pass, as the bytes have them; a for loop hoists them. */                \
    if (count > 0) {                                                           \
      i = count;                                                               \
    loop:;                                                                     \
      {                                                                        \
        s32 angle = Random16Far();                                             \
        struct Rock *rock = &work->rocks[work->next];                          \
                                                                               \
        rock->timer = 0;                                                       \
        rock->position[0] = x;                                                 \
        rock->position[1] = y;                                                 \
        rock->position[2] = z;                                                 \
        FieldVector_AddPolarOffset((Random16Far() << 3) + 0x80000, angle,      \
                                   rock->position);                            \
        rock->velocity[0] = 0;                                                 \
        rock->velocity[1] = 0x50000;                                           \
        rock->velocity[2] = 0;                                                 \
        FieldVector_AddPolarOffset((Random16Far() << 1) + 0x20000, angle,      \
                                   rock->velocity);                            \
        work->next++;                                                          \
        work->next &= 63;                                                      \
      }                                                                        \
      i--;                                                                     \
      if (i != 0)                                                              \
        goto loop;                                                             \
    }                                                                          \
  }

/// 3 copies: contigo, islet_cave, yampi_desert_cave.
#define DEFINE_FUNC_02000038_4(name, data1, data2, data3, data4, call1)        \
  void name(u16 *slots, u16 *colours, u16 *a, u16 *b) {                        \
    u8 *saved = Ram_HeapSlots->block32;                                        \
                                                                               \
    data1 = slots;                                                             \
    data2 = colours;                                                           \
    data3 = a;                                                                 \
    data4 = b;                                                                 \
    if (*slots != 0xffff && *colours != 0xffff) {                              \
    next:                                                                      \
      {                                                                        \
        int red = *colours++;                                                  \
        int index = *slots++;                                                  \
        int green = *colours++;                                                \
        int blue = *colours++;                                                 \
        int value;                                                             \
                                                                               \
        /* FAKEMATCH(dead): the colour's build and store in a once-only        \
         * block, so both lists are stepped right after their loads and the    \
         * next entry's marker loads before 0xffff is built; as plain          \
         * statements, 22 of 80 instructions differ in tla-en. */              \
        do {                                                                   \
          value = (blue << 10) | (green << 5) | red;                           \
          ((u16 *)BG_PLTT)[index] = value;                                     \
        } while (0);                                                           \
      }                                                                        \
      if (*slots != 0xffff && *colours != 0xffff)                              \
        goto next;                                                             \
    }                                                                          \
    Dma_Set((const void *)BG_PLTT, saved, DMA_COPY_WORDS(112), REG_DMA3);      \
    Dma_Set((const void *)OBJ_PLTT, saved + 448, DMA_COPY_WORDS(112),          \
            REG_DMA3);                                                         \
    BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);                         \
    if ((s8)gMainRoutineWork.unk2c2 == 2)                                      \
      call1();                                                                 \
  }

/// 3 copies: mars_lighthouse_aerie/s3370, mars_lighthouse_entrance/s42f8,
/// mars_lighthouse_room_m.
#define DEFINE_FUNC_0200154C(name, data1, data2, data3, data4, call1)          \
  void name(u16 *slots, u16 *colours, u16 *a, u16 *b) {                        \
    u8 *saved = Ram_HeapSlots->block32;                                        \
                                                                               \
    data1 = slots;                                                             \
    data2 = colours;                                                           \
    data3 = a;                                                                 \
    data4 = b;                                                                 \
    if (*slots != 0xffff && *colours != 0xffff) {                              \
    next:                                                                      \
      {                                                                        \
        int red = *colours++;                                                  \
        int index = *slots++;                                                  \
        int green = *colours++;                                                \
        int blue = *colours++;                                                 \
        int value;                                                             \
                                                                               \
        /* FAKEMATCH(dead): the colour's build and store in a once-only        \
         * block, so both lists are stepped right after their loads and the    \
         * next entry's marker loads before 0xffff is built; as plain          \
         * statements, 22 of 80 instructions differ in tla-en. */              \
        do {                                                                   \
          value = (blue << 10) | (green << 5) | red;                           \
          ((u16 *)BG_PLTT)[index] = value;                                     \
        } while (0);                                                           \
      }                                                                        \
      if (*slots != 0xffff && *colours != 0xffff)                              \
        goto next;                                                             \
    }                                                                          \
    Dma_Set((const void *)BG_PLTT, saved, DMA_COPY_WORDS(112), REG_DMA3);      \
    Dma_Set((const void *)OBJ_PLTT, saved + 448, DMA_COPY_WORDS(112),          \
            REG_DMA3);                                                         \
    PaletteFade_ApplyTarget(0x10000, 0);                                       \
    if ((s8)gMainRoutineWork.unk2c2 == 2)                                      \
      call1();                                                                 \
  }

/// 3 copies: dehkan_plateau/dehkan_plateau, dehkan_plateau/dehkan_plateau,
/// dehkan_plateau/dehkan_plateau.
#define DEFINE_FUNC_02001820(name, data1, n1, data2)                           \
  void name(void) {                                                            \
    const u16 *cell = &data1[0].present;                                       \
    s32 flag = 0x240;                                                          \
    u32 i;                                                                     \
    const u16 *dest;                                                           \
                                                                               \
    for (i = 0; i <= n1; i++) {                                                \
      if (GameFlag_Test(0x109) != 0) {                                         \
        if (GameFlag_Test(flag) != 0) {                                        \
          s32 x;                                                               \
          s32 y;                                                               \
                                                                               \
          cell = &data1[i].x;                                                  \
          x = *cell++;                                                         \
          y = *cell++;                                                         \
          /* FAKEMATCH(temporary): the destination table through a local set   \
           * here, which loads its first halfword before the 1 is built; as    \
           * gData_02003fd2[0], 2 of 56 instructions differ in tla-en. */      \
          dest = data2;                                                        \
          Map_CopyCellPictureIndicesRect(dest[0], dest[1], x, y, 1, 1);        \
        }                                                                      \
      } else {                                                                 \
        if (*cell != 0)                                                        \
          GameFlag_SetBit(flag);                                               \
        cell += 6;                                                             \
      }                                                                        \
      flag++;                                                                  \
    }                                                                          \
    if (GameFlag_Test(0x109) == 0)                                             \
      Func_02001804();                                                         \
  }

/// 6 copies: anemos_inner_sanctum, anemos_inner_sanctum,
/// mars_lighthouse_aerie/s3370, mars_lighthouse_aerie/s3370,
/// mars_lighthouse_entrance/s42f8, mars_lighthouse_entrance/s42f8.
#define DEFINE_FUNC_020000F4(name, data1, data2, call1, data3, data4, n1,      \
                             data5, n2, data6, call2)                          \
  void name(void) {                                                            \
    data1 = 0;                                                                 \
    data2 = call1(data3);                                                      \
    data4 = n1;                                                                \
    data5 = n2;                                                                \
    data6 = 1;                                                                 \
    Task_AddFrame(call2, TASK_PASS_VBLANK);                                    \
  }

/// 6 copies: contigo, contigo, islet_cave, islet_cave, yampi_desert_cave,
/// yampi_desert_cave.
#define DEFINE_FUNC_020000F4_2(name, data1, data2, call1, data3, data4, n1,    \
                               data5, n2, data6, call2)                        \
  void name(void) {                                                            \
    data1 = 0;                                                                 \
    data2 = call1(data3);                                                      \
    data4 = n1;                                                                \
    data5 = n2;                                                                \
    data6 = 1;                                                                 \
    Task_Add(call2, TASK_PASS_VBLANK);                                         \
  }

/// 3 copies: apojii_islands/apojii_islands, apojii_islands/apojii_islands,
/// apojii_islands/apojii_islands.
#define DEFINE_FUNC_02000A68(name, n1, n2, n3, n4, n5, n6, n7, n8, n9, n10)    \
  void name(void) {                                                            \
    struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);           \
                                                                               \
    Func_02001e0c_(n1, 38, n2, n3, n1, n4, 1);                                 \
    if (Event_GetFlag(n5) == 0) {                                              \
      Func_02001d04_(n6, PIXELS(n7), PIXELS(n8));                              \
      if (leader->x >> 20 == n9 && leader->z >> 20 == n10) {                   \
        Event_Start();                                                         \
        FieldEvent_ResolvePendingActions(0);                                   \
        Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x102);         \
        Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, 16, 0);      \
        leader->velocityY = 0x60000;                                           \
        FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);             \
        Event_End();                                                           \
      }                                                                        \
    }                                                                          \
  }

/// 2 copies: dehkan_plateau/s2698, dehkan_plateau/s2698.
#define DEFINE_FUNC_0200353C(name, n1, n2, n3, n4, n5, n6)                     \
  void name(void) {                                                            \
    /* FAKEMATCH(pin): flag in r5, which leaves r6 to the entry; as a plain    \
     * local, 61 of 79 instructions differ in tla-en. */                       \
    register s32 flag asm("r5") = 800;                                         \
    s32 offset = 0;                                                            \
                                                                               \
    while (1) {                                                                \
      if (GameFlag_Test(flag)) {                                               \
        u16 *entry = (u16 *)((u8 *)gData_02004276 + offset);                   \
        s32 y;                                                                 \
        s32 x;                                                                 \
        s32 one;                                                               \
                                                                               \
        flag -= 800;                                                           \
        x = *entry++;                                                          \
        y = *entry++;                                                          \
        one = 1;                                                               \
        MapGrid_CopyCellsTo(n1, n2, x, y, one, one);                           \
        x = entry[0];                                                          \
        y = entry[1];                                                          \
        MapGrid_CopyCellsTo(n3, n4, x, y, one, 5);                             \
        if (flag == 1)                                                         \
          MapGrid_CopyCellsTo(67, 4, 69, 3, one, one);                         \
        x -= 64;                                                               \
        y += n5;                                                               \
        MapGrid_CopyCellsTo(n1, n6, x, y, one, 4);                             \
        return;                                                                \
      }                                                                        \
      if (flag == 863)                                                         \
        return;                                                                \
      offset += 8;                                                             \
      flag++;                                                                  \
    }                                                                          \
  }

/// 2 copies: dehkan_plateau/s0bc0, madra_catacombs.
#define DEFINE_FUNC_02000270(name, call1, call2, call3)                        \
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

/// 2 copies: gabomba_statue_chamber, gabomba_statue_chamber.
#define DEFINE_FUNC_02000F00(name, n1)                                         \
  void name(void) {                                                            \
    struct GabombaStatueChamberObject *leader =                                \
        ((struct GabombaStatueChamberObject *)Actor_GetById(8));               \
    s32 x = leader->x;                                                         \
    s32 z = leader->z;                                                         \
    /* FAKEMATCH(temporary): a local holding the 0 for the three byte stores,  \
     * set before the null test, so it takes r7; as the constant 0 in each     \
     * store, 20 of 58 instructions differ in tla-en. */                       \
    s32 zero = 0;                                                              \
    struct GabombaStatueChamberObject *object;                                 \
                                                                               \
    if ((gData_02002058 = object =                                             \
             ((struct GabombaStatueChamberObject *)Import_ActorCreate(         \
                 489, x, 0x120000, z))) != NULL) {                             \
      struct FieldSprite *sprite = object->sprite;                             \
      u8 *block;                                                               \
                                                                               \
      sprite->fullColor = 0;                                                   \
      sprite->palette = 0;                                                     \
      sprite->priority = 2;                                                    \
      object->motionFlags = zero;                                              \
      object->field5c = 1;                                                     \
      object->update = Func_02000ef0;                                          \
      sprite->nextFrame = zero;                                                \
      sprite->unk1b[0] = zero;                                                 \
      block = HeapSlot_AllocFar(68, 0x608);                                    \
      Icon_DrawItem(n1);                                                       \
      VramCache_Load(sprite->vramBlock, 128, block + 0x400);                   \
      HeapSlot_Release(68);                                                    \
    }                                                                          \
  }

/// 2 copies: aqua_rock_inside/aqua_rock_inside, gaia_rock_inside/s07f4.
#define DEFINE_FUNC_0200136C(name, call1)                                      \
  s32 name(struct Orbiter *orbiter) {                                          \
    struct Actor *anchor = orbiter->anchor;                                    \
    s16 *angle;                                                                \
                                                                               \
    orbiter->angle += (s32)(orbiter->speed << 16) >> 17;                       \
    orbiter->x = anchor->x;                                                    \
    orbiter->y = anchor->y + 0x80000;                                          \
    orbiter->z = anchor->z;                                                    \
    FieldVector_AddPolarOffset(orbiter->radius, orbiter->angle, &orbiter->x);  \
    /* FAKEMATCH(temporary): the angle's address, taken after the first push,  \
     * sets r2 from r8 before the second push reads it; as orbiter->angle,     \
     * 2 of 89 instructions differ in tla-en. */                               \
    angle = &orbiter->angle;                                                   \
    if (--orbiter->countdown == 0)                                             \
      return 0;                                                                \
    if (orbiter->radius == 0x20000)                                            \
      return 1;                                                                \
    call1(orbiter);                                                            \
    orbiter->radius -= 0x40000;                                                \
    orbiter->angle += (s32)(orbiter->speed << 16) >> 17;                       \
    orbiter->x = anchor->x;                                                    \
    orbiter->y = anchor->y + 0x80000;                                          \
    orbiter->z = anchor->z;                                                    \
    FieldVector_AddPolarOffset(orbiter->radius, *angle, &orbiter->x);          \
    call1(orbiter);                                                            \
    orbiter->radius -= 0x40000;                                                \
    return 1;                                                                  \
  }

/// 2 copies: taopo_swamp, taopo_swamp.
#define DEFINE_FUNC_02001424(name, call1, n1, call2)                           \
  void name(void) {                                                            \
    s32 timer = Func_02002730();                                               \
                                                                               \
    /* FAKEMATCH(signature): Func_0200271c called as returning s32, as in      \
     * Func_02001304; as a call to the void setter, 4 of 50 instructions       \
     * differ in tla-en. */                                                    \
    switch (timer) {                                                           \
    case 0:                                                                    \
      Vent_SetActive(0, 1);                                                    \
      Vent_SetActive(1, 1);                                                    \
      Func_02002978(170);                                                      \
      break;                                                                   \
    case 120:                                                                  \
      Vent_SetActive(0, 0);                                                    \
      Vent_SetActive(1, 0);                                                    \
      Func_02002978(-1);                                                       \
      break;                                                                   \
    }                                                                          \
    if ((u32)(timer - 11) <= 118) {                                            \
      if (Event_GetFlag(0x221))                                                \
        call1(n1);                                                             \
      else if (Event_GetFlag(0x220))                                           \
        call2(n1);                                                             \
    }                                                                          \
    Func_0200273c(480);                                                        \
  }

/// 2 copies: jupiter_lighthouse_aerie, jupiter_lighthouse_room_3.
#define DEFINE_FUNC_020004C0(name, data1, data2, data3, data4, call1, data5)   \
  void name(s32 mode) {                                                        \
    data1 = 1;                                                                 \
    if (mode != 2) {                                                           \
      Iwram_CopyWordsEntry(data2, (void *)&BG_PLTT_COLOR(12, 0), 32);          \
      Iwram_CopyWordsEntry(data3, (void *)&BG_PLTT_COLOR(13, 0), 32);          \
    }                                                                          \
    if (Event_GetFlag(0xa00) || mode == 1) {                                   \
      /* FAKEMATCH(temporary): a local holding 0, so the zero is built in r2   \
       * and the counter's address in r3; as the 0 stored directly, 6 of 44    \
       * instructions differ in tla-en. */                                     \
      s32 zero = 0;                                                            \
                                                                               \
      data4 = zero;                                                            \
      Task_Add(call1, 1152);                                                   \
    } else {                                                                   \
      Dma_Set(data5, (void *)&BG_PLTT_COLOR(12, 2), DMA_COPY_WORDS(2),         \
              REG_DMA3);                                                       \
    }                                                                          \
  }

/// 2 copies: madra, mikasalla.
#define DEFINE_FUNC_02000544(name, call1)                                      \
  void name(void) {                                                            \
    s32 id, a;                                                                 \
                                                                               \
    Event_Start();                                                             \
    Event_ResolvePendingActions(0);                                            \
    a = 192;                                                                   \
    id = 4;                                                                    \
    /* FAKEMATCH(barrier): the id held at an empty asm, so it comes between    \
     * the angle byte and its shift; as no asm, 2 of 11 instructions differ in \
     * tla-en. */                                                              \
    __asm__ volatile("" : "+r"(id));                                           \
    Actor_ArmCallback(id, a << 8, 0);                                          \
    call1();                                                                   \
  }

/// 2 copies: loho, loho.
#define DEFINE_FUNC_02000C64(name, n1, n2)                                     \
  void name(s32 id) {                                                          \
    s32 value = 768;                                                           \
    s32 kind = n1;                                                             \
                                                                               \
    /* FAKEMATCH(barrier): the empty asm on the value and kind, so the kind is \
     * set between the value's halves; as constant arguments, 2 of 7           \
     * instructions differ in tla-en. */                                       \
    asm("" : "+l"(value), "+l"(kind));                                         \
    Actor_ReturnHome(id, kind, value + n2);                                    \
  }

/// 2 copies: jupiter_lighthouse_room_6, jupiter_lighthouse_room_7.
#define DEFINE_FUNC_02000060(name, n1, n2, n3, n4, n5, n6, n7, n8, n9, n10,    \
                             n11)                                              \
  void name(s32 mode) {                                                        \
    if (mode == 1) {                                                           \
      Event_PlaySound(133);                                                    \
      MapGrid_CopyCellsLayered(n1, n2, n3, 3, n4, n5);                         \
      System_Wait(6);                                                          \
    }                                                                          \
    MapGrid_CopyCellsLayered(n6, n7, n3, 3, n4, n5);                           \
    MapGrid_CopyCellAttributes(n8, n9, n3, 1, n10, n11);                       \
  }

/// 2 copies: jupiter_lighthouse_room_6, jupiter_lighthouse_room_7.
#define DEFINE_FUNC_020000B4(name, n1, n2, n3, n4, n5, n6, n7, n8, n9, n10)    \
  void name(void) {                                                            \
    Event_PlaySound(134);                                                      \
    MapGrid_CopyCellsLayered(n1, n2, n3, 3, n4, n5);                           \
    System_Wait(6);                                                            \
    MapGrid_CopyCellsLayered(n6, 42, n3, 3, n4, n5);                           \
    MapGrid_CopyCellAttributes(n7, n8, n3, 1, n9, n10);                        \
  }

/// 9 copies: jupiter_lighthouse_aerie_beacon/s0198, jupiter_lighthouse_room_4,
/// jupiter_lighthouse_room_5, jupiter_lighthouse_room_6,
/// jupiter_lighthouse_room_7, jupiter_lighthouse_room_8,
/// jupiter_lighthouse_room_9, jupiter_lighthouse_room_m,
/// mars_lighthouse_room_f.
#define DEFINE_FUNC_0200027C(name, data1, data2, data3)                        \
  void name(void) {                                                            \
    u16 colors[16];                                                            \
    s32 phase = (data1 / 4) % 5;                                               \
    s32 i;                                                                     \
    s32 offset;                                                                \
    u16 *p;                                                                    \
                                                                               \
    if (data2 != 0) {                                                          \
      u8 *work = Ram_HeapSlots->eventWork;                                     \
      s16 *state = (s16 *)(work + 358);                                        \
                                                                               \
      if (*state != 0 && (*state & 0x3ff) != 153)                              \
        return;                                                                \
      if (*(s8 *)(work + 0xca4) != 0)                                          \
        return;                                                                \
      if (*(s16 *)(work + 350) != 0)                                           \
        return;                                                                \
    }                                                                          \
    i = 0;                                                                     \
    offset = 4;                                                                \
  next:                                                                        \
    p = colors;                                                                \
    {                                                                          \
      /* FAKEMATCH(temporary): the slot and the table address held in locals;  \
       * as colors[(phase + i) % 5] = *(u16 *)((u8 *)gData_02006a2c + offset), \
       * 2 of 83 instructions differ in tla-en. */                             \
      s32 slot = (phase + i) % 5;                                              \
      u8 *table = (u8 *)data3;                                                 \
                                                                               \
      colors[slot] = *(u16 *)(table + offset);                                 \
    }                                                                          \
    i++;                                                                       \
    offset += 2;                                                               \
    if (i <= 4)                                                                \
      goto next;                                                               \
    Dma_Set(p, (void *)0x05000184, 0x84000002, REG_DMA3);                      \
    data1++;                                                                   \
  }

/// 6 copies: jupiter_lighthouse_basement, jupiter_lighthouse_entrance,
/// jupiter_lighthouse_room_1, jupiter_lighthouse_room_2,
/// jupiter_lighthouse_room_3, trial_road/s15a8.
#define DEFINE_FUNC_02000238(name, data1, data2, data3, n1)                    \
  void name(void) {                                                            \
    u16 colors[16];                                                            \
    s32 phase = (data1 / 4) % 5;                                               \
    s32 i;                                                                     \
    s32 offset;                                                                \
    u16 *p;                                                                    \
                                                                               \
    if (data2 != 0) {                                                          \
      u8 *work = Ram_HeapSlots->eventWork;                                     \
      s16 *state = (s16 *)(work + 358);                                        \
                                                                               \
      if (*state != 0 && (*state & 0x3ff) != 153)                              \
        return;                                                                \
      if (*(s8 *)(work + 0xca4) != 0)                                          \
        return;                                                                \
      if (*(s16 *)(work + 350) != 0)                                           \
        return;                                                                \
    }                                                                          \
    i = 0;                                                                     \
    offset = 4;                                                                \
  next:                                                                        \
    p = colors;                                                                \
    {                                                                          \
      s32 slot = (phase + i) % 5;                                              \
      u8 *table = (u8 *)data3;                                                 \
                                                                               \
      colors[slot] = *(u16 *)(table + offset);                                 \
    }                                                                          \
    i++;                                                                       \
    offset += 2;                                                               \
    if (i <= 4)                                                                \
      goto next;                                                               \
    Dma_Set(p, (void *)n1, 0x84000002, REG_DMA3);                              \
    data1++;                                                                   \
  }

/// 4 copies: anemos_inner_sanctum, mars_lighthouse_aerie/s3370,
/// mars_lighthouse_entrance/s42f8, mars_lighthouse_room_m.
#define DEFINE_FUNC_020001F8(name, data1, data2, data3, data4, data5, data6,   \
                             data7, data8, data9)                              \
  void name(void) {                                                            \
    s32 step;                                                                  \
    u32 i;                                                                     \
    u16 *src;                                                                  \
    volatile u16 *dst;                                                         \
    int red, green, blue;                                                      \
                                                                               \
    step = 0;                                                                  \
    if (data1 != 0) {                                                          \
      if (--data1 == 0) {                                                      \
        step = 1;                                                              \
        data1 = data2;                                                         \
        if (data2 > 2)                                                         \
          data2--;                                                             \
      }                                                                        \
    }                                                                          \
    if (step == 0)                                                             \
      return;                                                                  \
    src = data3;                                                               \
    for (i = 0; i < data4; i++) {                                              \
      dst = &BG_PLTT[data5[data6 * data4 + i]];                                \
      red = *src++;                                                            \
      green = *src++;                                                          \
      blue = *src++;                                                           \
      /* FAKEMATCH(dead): the store in a once-only block, whose end holds the  \
       * order around it; as the plain statement, 2 of 120 instructions differ \
       * in tla-en. */                                                         \
      do {                                                                     \
        *dst = (blue << 10) | (green << 5) | red;                              \
      } while (0);                                                             \
    }                                                                          \
    dst = &BG_PLTT[data7[data4]];                                              \
    if (data8 != 0)                                                            \
      src = data9 + data4 * 3;                                                 \
    else                                                                       \
      src = data3 + data4 * 3;                                                 \
    red = *src++;                                                              \
    green = *src++;                                                            \
    /* FAKEMATCH(dead): the last read in a once-only block, whose end holds    \
     * the order around it; as the plain statement, 2 of 120 instructions      \
     * differ in tla-en. */                                                    \
    do {                                                                       \
      blue = *src++;                                                           \
    } while (0);                                                               \
    *dst = (blue << 10) | (green << 5) | red;                                  \
    data8 ^= 1;                                                                \
    if (++data6 == data4)                                                      \
      data6 = 0;                                                               \
  }

/// 2 copies: jupiter_lighthouse_aerie_beacon/s4ec8,
/// jupiter_lighthouse_room_3/s1a7c.
#define DEFINE_FUNC_02001DB8(name, call1)                                      \
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
    Event_PlaySound(229);                                                      \
    MapGrid_CopyCells(event->x, event->z + 2, 1, 1, x, z);                     \
    FieldEvent_Wait(12);                                                       \
    {                                                                          \
      u32 zero = 0;                                                            \
                                                                               \
      object->facing = 0x4000;                                                 \
      object->motionFlags = zero;                                              \
    }                                                                          \
    Actor_SetSingleChildFlags(Actor_Get(gMainRoutineWork.currentOwner), 0);    \
    {                                                                          \
      /* FAKEMATCH(temporary): a word local holding 0xf1f, built with movs,    \
       * lsls, adds as the native does; as a constant stored to the halfword   \
       * register, loaded from the pool, 20 of 131 instructions differ in      \
       * tla-en.                                                               \
       */                                                                      \
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
        Event_PlaySound(204);                                                  \
      object->scaleX += -0x400;                                                \
      object->scaleY += -0x300;                                                \
      object->y += -0x9999;                                                    \
      System_Wait(1);                                                          \
    }                                                                          \
    do {                                                                       \
      Actor_Get(gMainRoutineWork.currentOwner)->animationKind = 0;             \
    } while (0);                                                               \
    event->busy = 0;                                                           \
    *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x209;                  \
    Event_FinishScreenTransition();                                            \
    Event_WaitTransitionFrames();                                              \
  }

/// 2 copies: mars_lighthouse_room_f/s35b8, mars_lighthouse_star_room/s3804.
#define DEFINE_FUNC_02003BE8(name, call1)                                      \
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
    Event_PlaySound(229);                                                      \
    MapGrid_CopyCells(event->x, event->z + 2, 1, 1, x, z);                     \
    FieldEvent_Wait(12);                                                       \
    {                                                                          \
      u32 zero = 0;                                                            \
                                                                               \
      object->facing = 0x4000;                                                 \
      object->motionFlags = zero;                                              \
    }                                                                          \
    Actor_SetSingleChildFlags(Actor_Get(gMainRoutineWork.currentOwner), 0);    \
    {                                                                          \
      /* FAKEMATCH(temporary): 0xf1f held in a word local, so it is built with \
       * movs, lsls, adds as the native does (a plain store loads it from the  \
       * pool); as a plain store of the constant, 20 of 131 instructions       \
       * differ in tla-en. */                                                  \
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
        Event_PlaySound(204);                                                  \
      object->scaleX += -0x400;                                                \
      object->scaleY += -0x300;                                                \
      object->y += -0x9999;                                                    \
      System_Wait(1);                                                          \
    }                                                                          \
    do {                                                                       \
      Actor_Get(gMainRoutineWork.currentOwner)->animationKind = 0;             \
    } while (0);                                                               \
    event->busy = 0;                                                           \
    *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x209;                  \
    Event_FinishScreenTransition();                                            \
    Event_WaitTransitionFrames();                                              \
  }

/// 3 copies: mars_lighthouse_room_f, mars_lighthouse_room_m,
/// mars_lighthouse_star_room.
#define DEFINE_FUNC_02001928(name, data1)                                      \
  void name(struct Actor *object, s32 part) {                                  \
    /* FAKEMATCH(temporary): one zero for every cleared field; as six stores   \
     * of 0, 29 of 30 instructions differ in tla-en. */                        \
    s32 zero = 0;                                                              \
                                                                               \
    object->motionFlags = zero;                                                \
    object->collisionFlags = zero;                                             \
                                                                               \
    ((struct SpriteParts *)object->sprite)->part = part;                       \
    Actor_SetSingleChildFlags(object, 0);                                      \
    Actor_StartScript(object, data1);                                          \
    object->timer = zero;                                                      \
    object->speedLimit = zero;                                                 \
    object->acceleration = zero;                                               \
  }

#endif
