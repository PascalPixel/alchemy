// Part of effect.c, the battle effect object, compiled only through it.
// Helm Splitter, formations and the impact burst.

// - Helm Splitter's fire swirl charge leaps the user at the first target,
//   plays the swirl over it and scatters bouncing sparks. The sparks live in
//   PARTICLES, whose 1024 slots are cleared first.
// - The impact burst plays a flash with large and small sparks and three
//   fragments, while the effect's objects settle into one of the formations
//   of BattleFx_PlaceFormationObjects. Large sparks and the fragments use
//   work->particles (0 to 63 and 61 to 63, the fragments overwriting the last
//   large sparks), small sparks PARTICLES.

#define HELM_SPLITTER_FRAMES 70
#define HELM_SPLITTER_STRIKE_FRAME 18 // the swirl strikes the target
// The pink star animation, after the swirl.
#define HELM_SPLITTER_STAR_CELLS 0x3e80
#define HELM_SPLITTER_CLEARED_SLOTS 1024 // map-buffer spark slots cleared first
#define HELM_SPLITTER_SPARKS 16          // sparks scattered by the strike
#define HELM_SPLITTER_SPARK_SLOTS 128    // spark slots moved and drawn

// Abilities: Helm Splitter (kind 51).
//
// The user leaps at the first target and strikes through a fire swirl; on
// frame 18 the target flashes and reels and 16 sparks scatter, bouncing
// off the ground. 70 frames.
void BattleFx_RunHelmSplitter(struct BattleEffectArgument *effect) {
  void **heapCache;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  u8 *graphics;
  DrawRectangle draw[2];
  struct EffectPosition position;

  struct EffectStep *sparks;
  struct EffectStep *spark;
  s32 *particleLife;
  s32 frame;
  s32 i;

  EFFECT_GET_SLOTS(heapCache, cursor, work, canvas);
  graphics = EFFECT_SLOT(heapCache, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  EFFECT_BEGIN_NARROW_CANVAS(0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, graphics, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_FireSwirlSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_PinkStarSheet,
                             work->sheet + HELM_SPLITTER_STAR_CELLS, 1, 0);
  BattleFx_FetchRectangleBlitters(work->effect->side, draw);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);

  // Clear the reusable spark pool before moving the user.
  particleLife = &PARTICLES->variant;
  for (i = 0; i != HELM_SPLITTER_CLEARED_SLOTS; i++) {
    *particleLife = 0;
    particleLife += 7;
  }
  sparks = PARTICLES;

  // The user leaps 80% of the way to the first target over 20 frames. The
  // distance is taken in 1/256 pixels so the squares fit in 32 bits.
  {
    struct MotionObject *object =
        GetBattleObjectSlotFar(work->effect->actor)->object;
    struct MotionObject *target =
        GetBattleObjectSlotFar(work->effect->actors[0])->object;
    s32 diffX = target->x - object->x;
    s32 startX = object->x;
    s32 deltaX = diffX * 80 / 100;
    s32 diffZ = target->z - object->z;
    s32 startZ = object->z;
    s32 deltaZ = diffZ * 80 / 100;
    s32 x = startX + deltaX;
    s32 z = startZ + deltaZ;
    s32 shortX = deltaX >> 8;
    s32 shortZ = deltaZ >> 8;
    s32 distance;

    distance = Iwram_Sqrt(shortX * shortX + shortZ * shortZ);
    distance = (distance << 8) / 20;
    object->acceleration = distance;
    object->speedLimit = distance;
    object->snapToTarget = 1;
    object->velocityY = 0x70000;
    object->verticalMotionStrength = 0xdeb8;
    object->verticalMotionPhase = 0;
    object->autoFaceMotion = 1;
    ObjectDispatch_InitFromTable6Far(object);
    Object_SetMoveTargetFar(object, x, 0, z);
    ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
  }

  // Play the swirl and impact, then let the sparks bounce and fade.
  for (frame = 0; frame != HELM_SPLITTER_FRAMES; frame++) {
    // The canvas follows the user across the screen.
    EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actor, &position);
    REG_BG2X = (80 - position.x) << 8;
    // Frames 8 to 23: the swirl plays its seven pictures, two frames each,
    // at the user's height, placed from the other edge on side 1.
    if (frame >= 8 && frame <= 23) {
      s32 cell = (frame - 8) / 2;

      if (cell > 6)
        cell = 6;
      if (work->effect->side == 0) {
        draw[0](canvas, work->sheet + gHelmSplitter_Cells[cell],
                gHelmSplitter_CellBiasX[cell] + 30,
                gHelmSplitter_CellBiasY[cell] + position.y - 60,
                gHelmSplitter_CellWidths[cell],
                gHelmSplitter_CellHeights[cell]);
      } else {
        draw[0](canvas, work->sheet + gHelmSplitter_Cells[cell],
                -gHelmSplitter_CellBiasX[cell] -
                    gHelmSplitter_CellWidths[cell] + 108,
                gHelmSplitter_CellBiasY[cell] + position.y - 60,
                gHelmSplitter_CellWidths[cell],
                gHelmSplitter_CellHeights[cell]);
      }
    }
    // The strike: the battle shows the damage, the target flashes and
    // reels, the camera shakes and 16 sparks fly from (64, 80), each living
    // 16 to 31 frames.
    if (frame == HELM_SPLITTER_STRIKE_FRAME) {
      BattleEventRuntime_BeginPhaseFar(SE_HEAVY_IMPACT);
      ObjectGroup_UpdateMembers(work->effect->actors[0], SPRITE_COLOR_WHITE,
                                POSE_DAMAGE, 0, 8);
      BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 6);
      work->shakeFrames = 4;
      for (i = 0; i != HELM_SPLITTER_SPARKS; i++) {
        s32 force = (Random16() & 63) + 256;
        s32 angle = Random16() & 0xffff;

        spark = &sparks[i];
        spark->x = 0x400000;
        spark->y = 0x500000;
        spark->velocityX = (Trig_Sin(angle) * force) >> 7;
        spark->velocityY = -(Trig_Cos(angle) * force) >> 6;
        spark->variant = (Random16() & 15) + 16;
      }
    }
    // A spark below row 104 bounces up at half its speed instead of being
    // drawn; one on the canvas is drawn with the pink star animation, each
    // spark at its own point of the six-picture cycle.
    for (i = 0; i != HELM_SPLITTER_SPARK_SLOTS; i++) {
      spark = &sparks[i];
      if (spark->variant > 0) {
        spark->variant--;
        EffectStep_AdvanceWithGravity2D(spark, 60, 0);
        if (spark->y > 0x680000) {
          spark->velocityY = -spark->velocityY / 2;
        } else if ((spark->x >= 0 && spark->x <= 0x7effff) && spark->y >= 0) {
          s32 sx = spark->x >> 16;
          s32 sy = spark->y >> 16;

          draw[0](canvas,
                  work->sheet + HELM_SPLITTER_STAR_CELLS +
                      (((frame + i) / 4 % 6) << 8),
                  sx - 8, sy - 8, 16, 16);
        }
      }
    }
    Camera_ApplyShake(8, 8);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

// Arranges work->objects in a formation; x and z are its origin in 16.16
// pixels.
// formation: 0 = nine in a 3x3 grid at 32-pixel spacing;
//            1 = twelve in rows of 1, 3, 4 and 4, from 16 left and 32 up;
//            2 = eight in rows of 3, 2 and 3, from 16 right;
//            3 = eight drawn at 1.25 times their size.
// Any other value places nothing.
void BattleFx_PlaceFormationObjects(s32 formation, s32 x, s32 z) {
  static struct SpriteScale normalScale = {{{0x10000, 0x10000}}};
  static struct SpriteScale largeScale = {{{0x14000, 0x14000}}};
  struct BattleEffectWork *work = WORK_SLOT(HEAP_SLOT_BATTLE_EFFECT);
  struct SpriteScale normal = normalScale;
  struct SpriteScale big = largeScale;
  struct SpritePlacement pos;
  s32 i;

  pos.groundHeight = 0;
  pos.height = 255 << 16;
  switch (formation) {
  case 0:
    for (i = 0; i != 9; i++) {
      pos.x = (i % 3 << 21) + x; // 32-pixel spacing
      pos.y = (i / 3 << 21) + z;
      Object_ApplyProjectedPlacementFar(work->objects[i], &pos, &normal, 0);
    }
    break;
  case 1:
    for (i = 0; i != 12; i++) {
      pos.x = (gFormation_TwelveX[i] << 16) + x - (16 << 16);
      pos.y = (gFormation_TwelveZ[i] << 16) + z - (32 << 16);
      Object_ApplyProjectedPlacementFar(work->objects[i], &pos, &normal, 0);
    }
    break;
  case 2:
    for (i = 0; i != 8; i++) {
      pos.x = (gFormation_RightEightX[i] << 16) + x + (16 << 16);
      pos.y = (gFormation_RightEightZ[i] << 16) + z;
      Object_ApplyProjectedPlacementFar(work->objects[i], &pos, &normal, 0);
    }
    break;
  case 3:
    for (i = 0; i != 8; i++) {
      pos.x = (gFormation_SmallEightX[i] << 16) + x;
      pos.y = (gFormation_SmallEightZ[i] << 16) + z;
      Object_ApplyProjectedPlacementFar(work->objects[i], &pos, &big, 0);
    }
    break;
  }
}

#define IMPACT_SHARD_CELLS 0x59d8 // Fragment graphics: four 12x24 images

#define IMPACT_BURST_FRAMES 72
#define IMPACT_BURST_FRAGMENTS 3       // fragments, work->particles 61 to 63
#define IMPACT_BURST_DRAWN_LARGE 30    // large sparks drawn
#define IMPACT_BURST_DRAWN_SMALL 60    // small sparks drawn
#define IMPACT_BURST_LARGE_SPARKS 64   // large sparks thrown
#define IMPACT_BURST_SMALL_SPARKS 64   // small sparks thrown
#define IMPACT_BURST_FIRST_FRAGMENT 61 // the first fragment in work->particles

// Open an impact flash and scatter sparks and fragments over 72 frames.
// Also rearrange the objects into formation during the first 36 frames.
// formation: layout used by BattleFx_PlaceFormationObjects
// x, y: impact position in 16.16 pixels
void BattleFx_RunImpactBurst(s32 formation, s32 x, s32 y) {
  void **heap;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle additive;
  DrawRectangle maxBlend;
  u8 *sheet;
  s32 orgX;
  struct EffectStep *particle;
  s32 frame;
  s32 i;

  // Set up work and canvas; double BG2 horizontally and blend BG1 and BG2
  // over the layers below.
  EFFECT_GET_SLOTS(heap, cursor, work, canvas);
  sheet = heap[2];
  orgX = x;
  x += 40 << 16;
  x /= 2; // Canvas uses half the horizontal width.
  REG_BG2PA = Q_8_8(0.5);
  REG_BG2X = 0;
  REG_BLDCNT =
      BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
  maxBlend = heap[7];
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  additive = heap[8];
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_FlareSheet, work->sheet, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_FlareImage,
                             work->sheet + IMPACT_SHARD_CELLS, 0, 0);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);

  // Large sparks: speed 256-511, lifetime 16-31 frames; use the first 30.
  for (i = 0; i != IMPACT_BURST_LARGE_SPARKS; i++) {
    s32 speed = (Random16() & 255) + 256;
    s32 angle = Random16() & 0xffff;

    work->particles[i].x = x;
    work->particles[i].y = y;
    work->particles[i].velocityX = (Trig_Sin(angle) * speed) >> 7;
    work->particles[i].velocityY = -((Trig_Cos(angle) * speed) >> 6);
    work->particles[i].variant = (Random16() & 15) + 16;
  }

  // Three fragments, 120 degrees apart.
  for (i = 0; i != IMPACT_BURST_FRAGMENTS; i++) {
    work->particles[IMPACT_BURST_FIRST_FRAGMENT + i].x = x;
    work->particles[IMPACT_BURST_FIRST_FRAGMENT + i].y = y;
    work->particles[IMPACT_BURST_FIRST_FRAGMENT + i].velocityX =
        (Trig_Sin(i * 0x5555) * 32) >> 6;
    work->particles[IMPACT_BURST_FIRST_FRAGMENT + i].velocityY =
        -((Trig_Cos(i * 0x5555) * 32) >> 5);
  }

  // Small sparks: speed 32-287, lifetime 20-35 frames.
  for (i = 0; i != IMPACT_BURST_SMALL_SPARKS; i++) {
    s32 speed = (Random16() & 255) + 32;
    s32 angle = Random16() & 0xffff;

    PARTICLES[i].x = x;
    PARTICLES[i].y = y;
    PARTICLES[i].velocityX = (Trig_Sin(angle) * speed) >> 6;
    PARTICLES[i].velocityY = -((Trig_Cos(angle) * speed) >> 5);
    PARTICLES[i].variant = (Random16() & 15) + 20;
  }

  for (frame = 0; frame != IMPACT_BURST_FRAMES; frame++) {
    if (frame == 4)
      AudioCommand_PlayFar(SE_154);
    if (frame == 32)
      AudioCommand_PlayFar(SE_212);

    // Flash: advance the image every five frames starting at frame 8.
    if (frame <= 47) {
      s32 cell = (frame - 8) / 5;
      u32 width;
      u32 height;

      if (cell < 0)
        cell = 0;
      // UB: width is assigned in one argument and read in another.
#ifdef UBFIX
      width = gImpactBurst_CellWidths[cell];
      height = gImpactBurst_CellHeights[cell];
      maxBlend(canvas, work->sheet + gImpactBurst_Cells[cell],
               (x >> 16) - (width >> 1), (y >> 16) - (height >> 1), width,
               height);
#else
      maxBlend(canvas, work->sheet + gImpactBurst_Cells[cell],
               (x >> 16) - ((width = gImpactBurst_CellWidths[cell]) >> 1),
               (y >> 16) - ((height = gImpactBurst_CellHeights[cell]) >> 1),
               width, height);
#endif
    }

    // Large sparks: the first 30 join two every other frame, their motes
    // shrinking with the life they have left.
    for (i = 0, particle = work->particles; i != IMPACT_BURST_DRAWN_LARGE;
         i++, particle++) {
      if (frame > i / 2 && particle->variant > 0) {
        s32 size;
        s32 px;
        s32 py;

        particle->variant--;
        EffectStep_AdvanceWithGravity2D(particle, 60, 0);
        size = particle->variant / 16 + 3;
        px = HI(particle->x);
        py = HI(particle->y);
        EFFECT_DRAW_MOTE(additive, canvas, sheet, px, py, size);
      }
    }

    // Small sparks start at frame 36.
    for (i = 0, particle = PARTICLES; i != IMPACT_BURST_DRAWN_SMALL;
         i++, particle++) {
      if (frame > 35 && particle->variant > 0) {
        s32 size;
        s32 px;
        s32 py;

        particle->variant--;
        EffectStep_AdvanceWithGravity2D(particle, 60, 0);
        size = particle->variant / 16 + 1;
        px = HI(particle->x);
        py = HI(particle->y);
        EFFECT_DRAW_MOTE(additive, canvas, sheet, px, py, size);
      }
    }

    // Fragments run on frames 36-63; advance the image every seven frames.
    for (i = 0; i != IMPACT_BURST_FRAGMENTS; i++) {
      struct EffectStep *shard =
          &work->particles[IMPACT_BURST_FIRST_FRAGMENT + i];

      if (frame >= 36 && frame <= 63) {
        s32 cell;
        s32 px;
        s32 py;

        EffectStep_AdvanceWithGravity2D(shard, 64, 0);
        cell = (frame - 36) / 7;
        px = HI(shard->x);
        py = HI(shard->y);
        maxBlend(canvas, work->sheet + cell * 0x120 + IMPACT_SHARD_CELLS,
                 px - 6, py - 12, 12, 24);
      }
    }

    if (frame <= 35)
      BattleFx_PlaceFormationObjects(formation, orgX, y);
    EFFECT_PRESENT_FRAME(work);
  }

  // Finish by clearing background character block 1.
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
  EFFECT_RELEASE_BLITTERS();
}
