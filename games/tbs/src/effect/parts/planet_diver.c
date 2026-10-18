// Part of effect.c, the battle effect object, compiled only through it.
// Planet Diver, Haunt and the orbiting spirals.

// - Planet Diver flashes the user, throws it into the air, turns it into a
//   ball of fire that falls on the target and bursts into 256 chips that
//   bounce on the ground. The impact chips reuse the map buffer that other
//   effects address as an array of EffectStep records.
// - The closing skulls (Haunt) gather around each target in turn, eight
//   skulls scattered in 3D that step towards a point above the target while
//   the group spins. They take 64 slots of PARTICLES per target, of which 8
//   are used.
// - The orbiting spirals circle above each target in turn, four of them a
//   quarter turn apart.
//
// The skulls and spirals ripple the effect layer throughout, through the
// HBlank DMA of BG2's affine origin.

// The leap that falls as a ball of fire.

// One chip of the impact, laid over the same 28-byte records as PARTICLES
// but with its velocities in other places than struct EffectStep's.
struct ImpactChip {
  s32 x; // canvas position, 16.16 fixed point
  s32 y;
  s32 velocityX;
  s32 unused0c;
  s32 velocityY;
  s32 unused14;
  s32 life; // frames left
};

#define PLANET_DIVER_IMPACT_CHIPS ((struct ImpactChip *)Ram_MapCellBuffer)

#define PLANET_DIVER_FRAMES 88
#define PLANET_DIVER_CLEARED_CHIPS 1024 // map-buffer chip slots cleared first
#define PLANET_DIVER_CHIPS 256          // chips the fireball bursts into

// Abilities: Planet Diver (kind 60).
//
// The user flashes and leaps into the air, turns into a ball of fire, falls
// onto the first target and bursts into chips that bounce on the ground. 88
// frames. effect->side says which way the ball falls; actors[0] is where it
// lands.
void BattleFx_RunPlanetDiver(struct BattleEffectArgument *effect) {
  struct EffectPosition screenPosition;
  BlitterPair draw;
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  u8 *sprites;
  struct MotionObject *target;
  s32 dx;
  struct MotionObject *actor;
  u8 *palette;
  s32 frame;
  s32 i;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  sprites = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  Resource_DecodeType01(
      Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA), sprites);
  // A sheet resource is its palette followed by its packed images.
  palette = Resource_GetTableEntry((s32)&ResourceId_FlashBurstSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Resource_DecodeType01(palette, work);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
  draw[0] = EFFECT_SLOT(slots, HEAP_SLOT_BLITTER);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_MAX);
  draw[1] = EFFECT_SLOT(slots, HEAP_SLOT_BLITTER_ALTERNATE);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE,
                        75); // trails keep 3/4 a frame

  for (i = 0; i != PLANET_DIVER_CLEARED_CHIPS; i++)
    PLANET_DIVER_IMPACT_CHIPS[i].life = 0;
  actor = GetBattleObjectSlotFar(work->effect->actor)->object;
  target = GetBattleObjectSlotFar(work->effect->actors[0])->object;
  // The ball falls 15 pixels a frame towards the middle of the field.
  if (actor->x > 0)
    dx = -(15 << 16);
  else
    dx = 15 << 16;

  for (frame = 0; frame != PLANET_DIVER_FRAMES; frame++) {
    struct BattleCamera *camera = gCameraWork;

    EFFECT_VIEW_FROM_CAMERA(camera);
    // The user's screen position is taken on frame 0 and from frame 18 on,
    // and held in between.
    if (frame > 17 || frame == 0) {
      EffectPosition_ProjectBattler(work->effect->actor, &screenPosition);
      screenPosition.x /= 2;
    }

    // The flash: frames 2 and 3 on the user, frames 4 to 15 spreading in
    // 16 directions.
    if (frame >= 2 && frame <= 3)
      draw[0](canvas, work, screenPosition.x - 16, screenPosition.y - 64, 32,
              64);
    if (frame >= 4 && frame <= 15) {
      for (i = 0; i != 16; i++) {
        s32 x = screenPosition.x + (frame * Trig_Sin(i << 12) >> 16);
        s32 y = screenPosition.y + (frame * Trig_Cos(i << 12) >> 16) - frame;

        draw[0](canvas, work->sheet + ((frame - 4) / 2 << 11), x - 16, y - 64,
                32, 64);
      }
    }
    // Frame 4: the user leaps, heading out past its own position.
    if (frame == 4) {
      actor->velocityY = 20 << 16;
      actor->acceleration = 1 << 16;
      actor->speedLimit = 3 << 16;
      actor->verticalMotionStrength = 0xab85;
      actor->autoFaceMotion = 0;
      actor->snapToTarget = 0;
      Object_SetMoveTargetFar(actor, actor->x * 3, 0, actor->z);
      ObjectDispatch_ApplyArgumentToChildrenFar(actor, 2);
      work->shakeFrames = frame;
      AudioCommand_PlayFar(SE_136);
    }
    // Frame 16: switch to the fireball images and line the user up with
    // the target's depth.
    if (frame == 16) {
      palette = Resource_GetTableEntry((s32)&ResourceId_FlameballSheet);
      Iwram_CopyWords((void *)BG_PLTT, palette, 128);
      palette += 128;
      Resource_DecodeType01(palette, work);
      actor->verticalMotionStrength = 0;
      actor->velocityX = 0;
      actor->velocityY = 0;
      actor->z = target->z;
      ObjectDispatch_InitFromTable6Far(actor);
    }
    // From frame 18: fall 15 pixels across and 8 down a frame; on reaching
    // the ground, throw out 256 chips, and the target recoils and flashes
    // as the battle shows the damage.
    if (frame > 17) {
      if (actor->y > 0) {
        actor->x += dx;
        actor->y -= 8 << 16;
        if (work->effect->side == 0) {
          draw[0](canvas, work, screenPosition.x - 20, screenPosition.y - 52,
                  40, 64);
          screenPosition.x -= 8;
        } else {
          draw[1](canvas, work, screenPosition.x - 26, screenPosition.y - 52,
                  40, 64);
          screenPosition.y += 8;
        }
      }
      if (actor->y < 0) {
        actor->y = 0;
        for (i = 0; i != PLANET_DIVER_CHIPS; i++) {
          struct ImpactChip *chip = &PLANET_DIVER_IMPACT_CHIPS[i];
          s32 speed;
          s32 angle;

          speed = 0x3ff;
          speed &= Random16();
          angle = Random16() & 0xffff;
          chip->x = screenPosition.x << 16;
          chip->y = (screenPosition.y - 24) << 16;
          chip->velocityX = Trig_Sin(angle) * (speed + 32) >> 6;
          chip->velocityY = -(Trig_Cos(angle) * (speed + 32) << 1) >> 6;
          chip->life = (Random16() & 7) + 32;
        }
        work->shakeFrames = 8;
        BattleEventRuntime_BeginPhaseFar(SE_145);
        BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 4);
        ObjectGroup_UpdateMembers(work->effect->actors[0], SPRITE_COLOR_WHITE,
                                  POSE_DAMAGE, 0, 8);
      }
    }

    // Chips: slow to 56/64 of their speed each frame, fall, and bounce at
    // half speed off y 112; only those on the canvas are drawn.
    for (i = 0; i != PLANET_DIVER_CHIPS; i++) {
      struct ImpactChip *chip = &PLANET_DIVER_IMPACT_CHIPS[i];
      s32 y;

      if (chip->life > 0) {
        chip->x += chip->velocityX;
        y = chip->y + chip->velocityY;
        chip->life--;
        chip->y = y;
        chip->velocityX = chip->velocityX * 56 / 64;
        chip->velocityY = chip->velocityY * 56 / 64 + 0x2000;
        if (chip->y > 112 << 16) {
          chip->velocityY = -chip->velocityY / 2;
        } else if ((chip->x >= 0 && chip->x < 127 << 16) && chip->y >= 0) {
          s32 size = chip->life / 8 + 1;

          EFFECT_DRAW_MOTE(draw[i & 1], canvas, sprites, (chip->x >> 16),
                           (chip->y >> 16), size);
        }
      }
    }
    Camera_ApplyShake(16, 16);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

// Closing skulls.

#define HAUNT_SCATTERED 512 // skull slots scattered at the start
#define HAUNT_SKULLS 8      // skulls drawn around each target
#define HAUNT_SLOTS 64      // particles reserved for each target

// Abilities: Haunt (kind 39).
//
// Skulls close in on each target in turn. Around each target, 32 frames apart,
// eight skulls appear one every 4 frames, scattered up to 64 pixels away, and
// close in on a point 20 pixels above the target while the group spins; the
// target flashes 32 frames into its turn, and a skull that reaches the centre
// is no longer drawn. The effect layer ripples throughout. PARTICLES holds 64
// skulls per target, of which the first 8 are used.
void BattleFx_RunHaunt(struct BattleEffectArgument *effect) {
  struct EffectPosition screenPosition;
  s32 point[3];
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  BlitterPair draw;
  struct BattleCamera *camera;
  u8 *palette;
  s32 i;
  s32 k;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  palette = Resource_GetTableEntry((s32)&ResourceId_SkullSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  palette += 128;
  Resource_DecodeType01(palette, work);
  palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
  draw[0] = EFFECT_SLOT(slots, HEAP_SLOT_BLITTER);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  draw[1] = EFFECT_SLOT(slots, HEAP_SLOT_BLITTER_ALTERNATE);
  Scheduler_AddOrUpdateCallback(BattleFx_ArmBg2AffineHBlankDma,
                                EFFECT_CALLBACK_ORDER);
  // 4 off every pixel each frame.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_DARKEN, 0x04040404);

  for (i = 0; i != HAUNT_SCATTERED; i++) {
    PARTICLES[i].x = ((Random16() & 255) - 127) << 15;
    PARTICLES[i].y = ((Random16() & 255) - 127) << 15;
    PARTICLES[i].z = ((Random16() & 255) - 127) << 15;
  }
  AudioCommand_PlayFar(SE_142);

  for (frame = 0; frame != work->effect->count * 32 + 96; frame++) {
    s32 *row;

    camera = gCameraWork;
    if (frame == 96) // the battle shows the damage
      BattleEventRuntime_BeginPhaseFar(0);

    // Ripple the canvas sideways, line by line.
    row = work->bg2xLines;
    if (work->effect->side == 0) {
      for (i = 0; i != 160; i++)
        *row++ = (0x60000 - Trig_Sin((frame + i) << 11) * 6) >> 10;
    } else {
      for (i = 0; i != 160; i++)
        *row++ = Trig_Sin((frame + i) << 11) * 6 >> 10;
    }

    EFFECT_FOR_EACH_TARGET(k, work->effect) {
      struct MotionObject *target =
          GetBattleObjectSlotFar(work->effect->actors[k])->object;

      // Centre on the point 20 pixels above the target and spin the group
      // about it.
      EFFECT_VIEW_FROM_CAMERA(camera);
      SET_VECTOR(point, target->x, 20 << 16, target->z);
      SceneTransform_ApplyPosition(point);
      if (frame > k * 32) {
        SceneTransform_ApplyPitch(frame << 9);
        if (frame == k * 32 + 32)
          ObjectGroup_UpdateMembers(work->effect->actors[k], SPRITE_COLOR_WHITE,
                                    POSE_DAMAGE, k, 32);

        // Each skull steps one pixel nearer the centre a frame; skulls take
        // one of three images in turn.
        for (i = 0; i != HAUNT_SKULLS; i++) {
          struct EffectStep *skull = &PARTICLES[k * HAUNT_SLOTS + i];

          if (frame > (k * 8 + i) * 4) {
            s32 x = (skull->x >> 8) * (skull->x >> 8);
            s32 y = (skull->y >> 8) * (skull->y >> 8);
            s32 z = (skull->z >> 8) * (skull->z >> 8);
            s32 dist = Iwram_Sqrt(x + y + z) >> 8;

            if (dist != 0) {
              EffectPosition_ProjectPoint(skull, &screenPosition);
              screenPosition.x >>= 1;
              draw[0](canvas, work->sheet + i % 3 * 576, screenPosition.x - 12,
                      screenPosition.y - 12, 24, 24);
              skull->x -= skull->x / dist;
              skull->y -= skull->y / dist;
              skull->z -= skull->z / dist;
              skull->variant++;
            }
          }
        }
      }
    }
    EFFECT_END_FRAME(work);
  }

  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  Scheduler_RemoveCallback(BattleFx_ArmBg2AffineHBlankDma);
  EFFECT_RELEASE_BLITTERS();
  BattleFx_EndCanvasLayer();
}

#define ORBITING_SPIRALS_LIFE 60 // frames each target's spirals turn

// Abilities: none (kind 34).
//
// Four spirals circle 40 pixels above each target in turn, 16 frames apart, for
// 60 frames each, 16 pixels out from the centre; each target takes its damage
// pose halfway through. The effect layer ripples throughout. Signed sine/cosine
// results are scaled by multiplication: negative left shifts were undefined,
// while these products stay within signed 32 bits.
void BattleFx_RunOrbitingSpirals(struct BattleEffectArgument *effect) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  void *palette;
  BlitterPair draw;
  s32 point[3];
  struct EffectPosition screen;
  s32 member;
  s32 offsetX;
  s32 frame;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  work->effect = effect;
  EFFECT_BEGIN_NARROW_CANVAS(0);
  palette = Resource_GetTableEntry((s32)&ResourceId_SpiralSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  Resource_DecodeType01((u8 *)palette + 128, work);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
  draw[0] = EFFECT_SLOT(slots, HEAP_SLOT_BLITTER);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, COMPOSITE_MAX);
  draw[1] = EFFECT_SLOT(slots, HEAP_SLOT_BLITTER_ALTERNATE);
  Scheduler_AddOrUpdateCallback(BattleFx_ArmBg2AffineHBlankDma,
                                EFFECT_CALLBACK_ORDER);
  // Trails keep half a frame.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);
  // On the party's side the canvas is scrolled 104 pixels left.
  if (work->effect->side == 1) {
    REG_BG2X = -0x6800;
    offsetX = -112;
  } else {
    offsetX = 0;
  }
  for (frame = 0; frame != (work->effect->count * 16) + 48; frame++) {
    struct BattleCamera *camera;
    s32 *row;
    s32 i;

    // Ripple the canvas sideways, line by line.
    camera = gCameraWork;
    row = work->bg2xLines;
    if (work->effect->side == 0) {
      s32 angle;
      s32 base;

      for (i = 0, base = 0x80000, angle = frame << 10; i != 160; i++) {
        *row++ = (base - ((Trig_Sin(angle) * 8))) >> 10;
        angle += 1024;
      }
    } else {
      s32 angle;

      for (i = 0, angle = frame << 10; i != 160; i++) {
        *row++ = (((Trig_Sin(angle) * 8)) >> 10) - 0x7000;
        angle += 1024;
      }
    }
    EFFECT_VIEW_FROM_CAMERA(camera);
    member = 0;
    if (work->effect->count != 0) {
      while (member != work->effect->count) {
        struct MotionObject *obj;

        obj = GetBattleObjectSlotFar(work->effect->actors[member])->object;
        if (frame > member * 16 &&
            frame < (member * 16) + ORBITING_SPIRALS_LIFE) {
          s32 spin;

          if (frame == (member * 16) + 32) {
            ObjectGroup_UpdateMembers(work->effect->actors[member], 0,
                                      POSE_DAMAGE, -1, 0);
          }
          SET_VECTOR(point, obj->x, 40 << 16, obj->z);
          EffectPosition_ProjectPoint(point, &screen);

          // The spirals (32x32) a quarter turn apart, through four images,
          // turned upside down every other 16 frames.
          for (i = 0; i != 4; i++) {
            s32 x;
            s32 y;
            s32 half;

            spin = (frame << 9) + (i << 14);
            x = (screen.x + (((Trig_Sin(spin) * 16)) >> 16)) + offsetX;
            y = screen.y + (((Trig_Cos(spin) * 16)) >> 16);
            half = frame / 16;
            draw[half & 1](canvas,
                           work->sheet + (((frame / 4) - (half * 4)) << 10),
                           x - 16, y - 16, 32, 32);
          }
        }
        member++;
      }
    }
    EFFECT_PRESENT_FRAME(work);
  }
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  Scheduler_RemoveCallback(BattleFx_ArmBg2AffineHBlankDma);
  EFFECT_RELEASE_BLITTERS();
  BattleFx_EndCanvasLayer();
}

#undef PLANET_DIVER_IMPACT_CHIPS
#undef PLANET_DIVER_FRAMES
#undef PLANET_DIVER_CLEARED_CHIPS
#undef PLANET_DIVER_CHIPS
#undef HAUNT_SCATTERED
#undef HAUNT_SKULLS
#undef HAUNT_SLOTS
#undef ORBITING_SPIRALS_LIFE
