/// @file
/// Particles, sparks and palette cycles several scenes copy.
/// Each routine is a macro a scene expands where its copy sits, with the
/// names of its own data and calls.
#ifndef ALCHEMY_SCENE_SHARED_EFFECTS_H
#define ALCHEMY_SCENE_SHARED_EFFECTS_H

/// Sparks rising from an actor, every eighth frame.
#define DEFINE_ACTOR_EMIT_RISING_SPARKS(name, call1)                           \
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

/// Updates the falling particles and their sprites.
#define DEFINE_PARTICLES_UPDATE_FALLING(name, call1)                           \
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

/// Draws the dust around the leader.
#define DEFINE_GROUND_PARTICLES_DRAW(name, data1)                              \
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

/// A drop falls from an actor every sixteenth frame.
#define DEFINE_ACTOR_EMIT_WATER_DROP(name, data1)                              \
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

/// Mist rises from an actor every other frame.
#define DEFINE_ACTOR_EMIT_MIST(name)                                           \
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
      FieldEffect_Spawn(x + ((8 - (gFrameCount & 15)) << 16), y + 0x1a0000,    \
                        object->z, 0, -drift, 0, 0xb0000, params);             \
    }                                                                          \
    return 0;                                                                  \
  }

/// Actor_EmitRisingSparks with the spark settings in a plain array.
#define DEFINE_ACTOR_EMIT_RISING_SPARKS_B(name, call1)                         \
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

/// Bubbles rise from an actor every eighth frame.
#define DEFINE_ACTOR_EMIT_BUBBLES(name, call1)                                 \
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

/// The leader rises out of the water, unless the party keeps its place.
#define DEFINE_LEADER_RISE_FROM_WATER(name, call1)                             \
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
      FieldActor_SetSpriteColor(Actor_Get(*owner), 9);                         \
      Actor_SetSpriteFlags(Actor_Get(*owner), 0);                              \
      Event_StartScreenTransition();                                           \
      Event_PlaySound(228);                                                    \
      leader->update = (void (*)(void))call1;                                  \
      Actor_SetSpeed(*owner, 0x6666, 0x3333);                                  \
      Actor_WalkByAndWait(*owner, 0, 8);                                       \
      FieldActor_SetSpriteColor(Actor_Get(*owner), 0);                         \
      Actor_SetSpriteFlags(Actor_Get(*owner), 1);                              \
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

/// The leader spins up and away to `entrance`.
#define DEFINE_LEADER_SPIN_AWAY(name, call1, call2)                            \
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

/// Aims an actor at the actor it watches, to reach it in ten steps.
#define DEFINE_ACTOR_AIM_AT_WATCHED(name)                                      \
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

/// The leader sinks away to `entrance`.
#define DEFINE_LEADER_SINK_AWAY(name, call1)                                   \
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
    FieldActor_SetSpriteColor(Actor_Get(*owner), 9);                           \
    Actor_SetSpriteFlags(Actor_Get(*owner), 0);                                \
    leader->update = 0;                                                        \
    Event_FinishScreenTransition();                                            \
    Event_WaitTransitionFrames();                                              \
    FieldEvent_SetExitRequest(entrance);                                       \
    Event_End();                                                               \
  }

/// Four notes rise from an actor, one each quarter of a second.
#define DEFINE_ACTOR_EMIT_NOTES(name, call1)                                   \
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

/// Steps an actor toward its target; 0 when it gets there.
#define DEFINE_ACTOR_STEP_TO_WATCHED(name)                                     \
  s32 name(struct Actor *object) {                                             \
    u8 left;                                                                   \
                                                                               \
    object->x += object->bounce;                                               \
    object->y += object->unk4c;                                                \
    object->z += object->gravity;                                              \
    left = --BYTE_AT(object, 98);                                              \
    return left != 0;                                                          \
  }

/// Moves an orbiting actor round its centre as its circle grows.
#define DEFINE_ORBIT_UPDATE(name)                                              \
  s32 name(struct Orbit *object) {                                             \
    s32 z;                                                                     \
                                                                               \
    object->angle += object->speed;                                            \
    object->x = IwramMulQ16(Trig_Cos(object->angle), object->radius);          \
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

/// Shows an actor's parts.
#define DEFINE_ACTOR_SHOW_PARTS(name, call1)                                   \
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

/// Follows the third map layer's scroll.
#define DEFINE_LAYER_TRACK_SCROLL(name, data1, data2, data3)                   \
  void name(void) {                                                            \
    struct MapLayer *layer = &Ram_HeapSlots->mapWork->layers[2];               \
    s16 y = layer->y >> 16;                                                    \
                                                                               \
    data1 = 1448 - y;                                                          \
    data2 = layer->x >> 16;                                                    \
    data3 = (s16)(layer->x >> 16) - (gFrameCount >> 2);                        \
  }

/// Starts the HBlank scroll handler and its task.
#define DEFINE_SCROLL_START_HBLANK(name, call1, call2)                         \
  void name(void) {                                                            \
    s32 priority;                                                              \
                                                                               \
    Irq_SetHandler(1, 0, call1);                                               \
    priority = 0xc80;                                                          \
    FieldTask_AddFrame(call2, priority);                                       \
  }

/// Orbs rise round actor 23, faster once flag `n1` is set.
#define DEFINE_ORBS_SPAWN_RISING(name, n1, data1, call1, n2)                   \
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
      FieldActor_SetSpriteColor(effect, 1);                                    \
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

/// Smoke rises from an actor on the frames `n1` picks.
#define DEFINE_ACTOR_EMIT_SMOKE(name, n1, call1, data1)                        \
  void name(struct Actor *object) {                                            \
    struct Actor *spawned;                                                     \
                                                                               \
    if ((gFrameCount & n1) == 0) {                                             \
      spawned = FieldActor_Create(14, object->x, object->y, object->z);        \
      if (spawned != 0) {                                                      \
        Actor_SetSpriteFlags(spawned, 0);                                      \
        call1(spawned, 1);                                                     \
        Actor_StartScript(spawned, data1);                                     \
      }                                                                        \
    }                                                                          \
  }

/// Throws `count` rocks out from a point.
#define DEFINE_ROCKS_SPAWN(name)                                               \
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

/// Sets palette colours from a list and saves both palettes.
#define DEFINE_PALETTE_SET_COLORS(name, data1, data2, data3, data4, call1)     \
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
        SET_RGB(((u16 *)BG_PLTT)[index], red, green, blue);                    \
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

/// Palette_SetColors, then applies the palette fade.
#define DEFINE_PALETTE_SET_COLORS_FADE(name, data1, data2, data3, data4,       \
                                       call1)                                  \
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
        SET_RGB(((u16 *)BG_PLTT)[index], red, green, blue);                    \
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

/// Starts a palette cycle's task.
#define DEFINE_PALETTE_CYCLE_START(name, data1, data2, call1, data3, data4,    \
                                   n1, data5, n2, data6, call2)                \
  void name(void) {                                                            \
    data1 = 0;                                                                 \
    data2 = call1(data3);                                                      \
    data4 = n1;                                                                \
    data5 = n2;                                                                \
    data6 = 1;                                                                 \
    Task_AddFrame(call2, TASK_PASS_VBLANK);                                    \
  }

/// PaletteCycle_Start with a VBlank task.
#define DEFINE_PALETTE_CYCLE_START_VBLANK(name, data1, data2, call1, data3,    \
                                          data4, n1, data5, n2, data6, call2)  \
  void name(void) {                                                            \
    data1 = 0;                                                                 \
    data2 = call1(data3);                                                      \
    data4 = n1;                                                                \
    data5 = n2;                                                                \
    data6 = 1;                                                                 \
    Task_Add(call2, TASK_PASS_VBLANK);                                         \
  }

/// Moves an orbiter two steps inward round its anchor.
#define DEFINE_ORBITER_SPIRAL(name, call1)                                     \
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

/// Steps a five-colour palette cycle, unless an event holds the field.
#define DEFINE_PALETTE_CYCLE_STEP(name, data1, data2, data3)                   \
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

/// PaletteCycle_Step at a colour address given.
#define DEFINE_PALETTE_CYCLE_STEP_AT(name, data1, data2, data3, n1)            \
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

/// Steps a palette cycle that fades as it slows.
#define DEFINE_PALETTE_CYCLE_FADE(name, data1, data2, data3, data4, data5,     \
                                  data6, data7, data8, data9)                  \
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
      SET_RGB(*dst, red, green, blue);                                         \
    }                                                                          \
    dst = &BG_PLTT[data7[data4]];                                              \
    if (data8 != 0)                                                            \
      src = data9 + data4 * 3;                                                 \
    else                                                                       \
      src = data3 + data4 * 3;                                                 \
    red = *src++;                                                              \
    green = *src++;                                                            \
    blue = *src++;                                                             \
    SET_RGB(*dst, red, green, blue);                                           \
    data8 ^= 1;                                                                \
    if (++data6 == data4)                                                      \
      data6 = 0;                                                               \
  }

#endif
