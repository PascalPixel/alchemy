/// @file
/// The routines the world map's flight and the staff credits share: the
/// flying sprites, the camera's jitter and the gauge.
/// Each routine is a macro a scene expands where its copy sits, with the
/// names of its own data and calls.
#ifndef ALCHEMY_SCENE_SHARED_FLIGHT_H
#define ALCHEMY_SCENE_SHARED_FLIGHT_H

/// Flickers an actor, running `call1` every sixteenth frame.
#define DEFINE_ACTOR_FLICKER(name, call1)                                      \
  void name(struct Actor *object) {                                            \
    if ((gFrameCount & 2) != 0) {                                              \
      Actor_SetSpriteColorMode(object, 7);                                   \
    } else {                                                                   \
      Actor_SetSpriteColorMode(object, 0);                                   \
    }                                                                          \
    if ((gFrameCount & 15) == 0) {                                             \
      call1(object);                                                           \
    }                                                                          \
  }

/// Jitters the camera round the leader.
#define DEFINE_CAMERA_JITTER(name)                                             \
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
        FieldCamera_MoveWithinMapBounds((x << 16) + 0x10000, -1,                    \
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

/// Grows an actor upward over 32 frames.
#define DEFINE_ACTOR_GROW_UP(name, call1)                                      \
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

/// Grows an actor downward over 32 frames.
#define DEFINE_ACTOR_GROW_DOWN(name, call1)                                    \
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

/// Searches round the leader for an event, for up to 20 frames.
#define DEFINE_LEADER_SEARCH(name, call1, call2)                               \
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

/// Sets an actor's update by mode.
#define DEFINE_ACTOR_SET_FLIGHT_MODE(name, call1, call2)                       \
  void name(struct Actor *object, s32 mode) {                                  \
    if (mode == 0) {                                                           \
      object->update = 0;                                                      \
      Actor_SetSpriteColorMode(object, 0);                                   \
      Event_PlaySound(390);                                                    \
    } else if (mode == 1) {                                                    \
      object->update = (void (*)(void))call1;                                  \
    } else {                                                                   \
      object->update = (void (*)(void))call2;                                  \
      Event_PlaySound(291);                                                    \
    }                                                                          \
  }

/// Cycles an actor's child values, running `call1` every fourth frame.
#define DEFINE_ACTOR_CYCLE_CHILD_VALUES(name, data1, data2, call1)             \
  void name(struct Actor *object) {                                            \
    if (gFrameCount & 1) {                                                     \
      Actor_SetSpriteColorMode(object, data1[data2]);                        \
      data2 = (data2 + 1) & 3;                                                 \
    }                                                                          \
    if (!(gFrameCount & 3)) {                                                  \
      call1(object);                                                           \
    }                                                                          \
  }

/// Animates an actor, running `call1` every sixteenth frame.
#define DEFINE_ACTOR_ANIMATE_AND_EMIT(name, call1)                             \
  void name(struct Actor *object) {                                            \
    u32 frame = gFrameCount;                                                   \
                                                                               \
    if (frame & 1)                                                             \
      Actor_SetSpriteColorMode(object, (frame >> 1) % 6);                    \
    if ((gFrameCount & 15) == 0)                                               \
      call1(object);                                                           \
  }

/// Sets up the gauge.
#define DEFINE_GAUGE_INIT(name, data1, data2, data3, data4, call1)             \
  void name(void) {                                                            \
    Decompress_TaggedLz(gWorldMapPackedTilesC, data1);                       \
    data2 = VramCache_FindFreeEntry();                                          \
    Vram_Load(data2, 0x100, 0);                                                \
    data3 = -1;                                                                \
    data4 = 10;                                                                \
    Task_AddFrame(call1, TASK_PASS_VBLANK - 10);                               \
  }

/// Animates an actor over six frames.
#define DEFINE_ACTOR_ANIMATE(name)                                             \
  void name(struct Actor *object) {                                            \
    u32 frame = gFrameCount;                                                   \
                                                                               \
    if (frame & 1)                                                             \
      Actor_SetSpriteColorMode(object, (frame >> 1) % 6);                    \
  }

/// Takes the gauge down.
#define DEFINE_GAUGE_END(name, data1, data2, data3, call1)                     \
  void name(void) {                                                            \
    Decompress_TaggedLz(gWorldMapPackedTilesC, data1);                       \
    VramCache_FreeEntry(data2);                                                \
    data3 = -1;                                                                \
    FieldTask_Remove(call1);                                                   \
  }

/// Redraws the gauge for `progress`.
#define DEFINE_GAUGE_DRAW(name, data1, data2, data3, data4)                    \
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

#endif
