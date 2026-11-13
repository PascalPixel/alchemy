/// @file
/// What the rooms of Jupiter Lighthouse share: the glow sprites over actors,
/// the palettes lit with the lighthouse, doors and once-only events.
/// Each routine is a macro a scene expands where its copy sits, with the
/// names of its own data and calls.
#ifndef ALCHEMY_SCENE_SHARED_JUPITER_LIGHTHOUSE_H
#define ALCHEMY_SCENE_SHARED_JUPITER_LIGHTHOUSE_H

/// Draws a glow sprite over each listed actor.
#define DEFINE_LIGHT_SPRITES_DRAW(name, data1, data2, data3, data4, data5, data6, \
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

/// Puts actors 9, 10, 23 and 24 where flags 0xa0a and 0xa0b leave them.
#define DEFINE_JUPITER_LIGHTHOUSE_PLACE_RAISED_ACTORS(name)                    \
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
      Actor_SetSpriteColorMode(((struct PositionedActor *)Actor_Get(9)), 4); \
      Actor_SetSpriteColorMode(((struct PositionedActor *)Actor_Get(11)),    \
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
      Actor_SetSpriteColorMode(((struct PositionedActor *)Actor_Get(10)),    \
                                 4);                                           \
      Actor_SetSpriteColorMode(((struct PositionedActor *)Actor_Get(12)),    \
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

/// Sets up the glow sprites from the field's cached tiles A.
#define DEFINE_LIGHT_SPRITES_INIT_A(name, data1, data2, call1, data3, data4, data5) \
  void name(void) {                                                            \
    u8 *buffer = BumpAlloc(0xc00);                                             \
                                                                               \
    Iwram_ClearWordsEntry(data1, 64);                                          \
    Decompress_TaggedLz(gField_CachedTilesA, buffer);                        \
    data2 = VramCache_FindFreeEntry();                                          \
    VramCache_Load(data2, 0xc00, buffer);                                      \
    Heap_Release(buffer);                                                      \
    Task_Add(call1, TASK_PASS_VBLANK);                                         \
    data3 = 0;                                                                 \
    data4 = 0;                                                                 \
    data5 = -1;                                                                \
  }

/// Lifts an actor out of its cell and opens the cells it stood on.
#define DEFINE_ACTOR_RAISE_FROM_CELL(name, call1, call2)                       \
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

/// Sets up the glow sprites from the field's cached tiles B.
#define DEFINE_LIGHT_SPRITES_INIT_B(name, data1, data2, call1, data3, data4, data5) \
  void name(void) {                                                            \
    u8 *buffer = Heap_Alloc(0xc00);                                            \
                                                                               \
    Iwram_ClearWordsEntry(data1, 64);                                          \
    Decompress_TaggedLz(gField_CachedTilesB, buffer);                        \
    data2 = VramCache_FindFreeEntry();                                          \
    Vram_Load(data2, 0x800, buffer);                                           \
    Heap_Release(buffer);                                                      \
    Task_AddFrame(call1, TASK_PASS_VBLANK);                                    \
    data3 = 0;                                                                 \
    data4 = 0;                                                                 \
    data5 = -1;                                                                \
  }

/// Sets up the large glow sprites from the field's cached tiles C.
#define DEFINE_LIGHT_SPRITES_INIT_C(name, data1, data2, call1, data3, data4, data5) \
  void name(void) {                                                            \
    u8 *buffer = BumpAlloc(0x800);                                             \
                                                                               \
    Iwram_ClearWordsEntry(data1, 64);                                          \
    Decompress_TaggedLz(gField_CachedTilesC, buffer);                        \
    data2 = VramCache_FindFreeEntry();                                          \
    Vram_Load(data2, 0x800, buffer);                                           \
    Heap_Release(buffer);                                                      \
    Task_AddFrame(call1, TASK_PASS_VBLANK);                                    \
    data3 = 0;                                                                 \
    data4 = 1;                                                                 \
    data5 = -1;                                                                \
  }

/// Loads the room's two palettes and starts their cycle once the lighthouse is
/// lit (flag 0xa00).
#define DEFINE_JUPITER_LIGHTHOUSE_LOAD_PALETTES(name, data1, data2, data3, call1, data4) \
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

/// Adds an actor to the glow list, its frame in `value`.
#define DEFINE_LIGHT_SPRITES_ADD(name, data1, data2)                           \
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

/// LightSprites_Add with the count kept in a plain global.
#define DEFINE_LIGHT_SPRITES_ADD_COUNTED(name, data1, data2)                   \
  void name(s32 id, s32 value) {                                               \
    struct Actor *object = Actor_Get(id);                                      \
                                                                               \
    if (object != NULL) {                                                      \
      object->timer = value;                                                   \
      data1[data2] = object;                                                   \
      data2++;                                                                 \
    }                                                                          \
  }

/// Runs `call1` once when flag `n1` is set, clearing it, then `call2`.
#define DEFINE_SCENE_RUN_IF_FLAG_CLEARED(name, n1, call1, call2)               \
  void name(void) {                                                            \
    if (Event_GetFlag(n1) == 1) {                                              \
      Event_ClearFlag(n1);                                                     \
      call1();                                                                 \
    }                                                                          \
    call2();                                                                   \
  }

/// Runs `call1(1)` the first time, setting flag `n1`.
#define DEFINE_SCENE_RUN_ONCE(name, n1, call1)                                 \
  void name(void) {                                                            \
    if (!Event_GetFlag(n1)) {                                                  \
      Event_SetFlag(n1);                                                       \
      call1(1);                                                                \
    }                                                                          \
  }

/// Sets flag `n2` and opens the way once actor `n1` stands on cell (`n3`,
/// `n4`).
#define DEFINE_JUPITER_LIGHTHOUSE_CHECK_BLOCK_ON_CELL(name, n1, n2, n3, n4, call1) \
  void name(void) {                                                            \
    struct Actor *object = Actor_Get(n1);                                      \
                                                                               \
    if (!Event_GetFlag(n2) && object->x >> 20 == n3 &&                         \
        object->z >> 20 == n4) {                                               \
      Event_SetFlag(n2);                                                       \
      call1(1);                                                                \
    }                                                                          \
  }

/// JupiterLighthouse_LoadPalettes as the aerie and room 3 compile it.
#define DEFINE_JUPITER_LIGHTHOUSE_LOAD_PALETTES_ZERO(name, data1, data2, data3, data4, call1, data5) \
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

/// Opens a door three cells high, with its sound when `mode` is 1.
#define DEFINE_JUPITER_LIGHTHOUSE_OPEN_DOOR(name, n1, n2, n3, n4, n5, n6, n7, n8, n9, n10, \
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

/// Closes a door three cells high.
#define DEFINE_JUPITER_LIGHTHOUSE_CLOSE_DOOR(name, n1, n2, n3, n4, n5, n6, n7, n8, n9, n10) \
  void name(void) {                                                            \
    Event_PlaySound(134);                                                      \
    MapGrid_CopyCellsLayered(n1, n2, n3, 3, n4, n5);                           \
    System_Wait(6);                                                            \
    MapGrid_CopyCellsLayered(n6, 42, n3, 3, n4, n5);                           \
    MapGrid_CopyCellAttributes(n7, n8, n3, 1, n9, n10);                        \
  }

#endif
