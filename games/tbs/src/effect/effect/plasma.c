// Part of effect.c, the battle effect object, compiled only through it.
// Plasma, and the spark groups of Blast and Dynamite.

// Pillars of lightning and the sparks they throw up.

#define LIGHTNING_PILLAR_SPARK_COUNT 1024 // sparks

#define PLASMA_SPACING 8           // frames between pillars
#define PLASMA_SECOND_IMAGE 0x1500 // the second picture in the sheet

// gPlasma_Columns has seven pillars for each side.
#define PLASMA_COLUMN_X(side, pillar) gPlasma_Columns[(side)][(pillar)]

// Abilities: Plasma, Shine Plasma, Spark Plasma, Thunderclap, Thunderbolt,
// Thunderstorm (kind 18).
//
// Battle effect 18: pillars of lightning strike one every 8 frames, each
// with a white flash, throwing sparks up from the ground and knocking the
// targets back; the battle shows the damage on frame 32.
// effect->variant picks how many pillars (gPlasma_Counts), how many
// sparks each throws and how long the screen shakes (gPlasma_Sparks);
// with variant 2 the camera also swings round for the first 64 frames.
// effect->side picks the row of pillar positions.
void BattleFx_RunPlasma(struct BattleEffectArgument *effect) {
  BlitterPair draw;
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 i;
  u8 *sprites;
  s32 count;
  s32 frame;
  s32 k;
  s32 start;
  s32 *life;
  struct EffectStep *spark;
  struct EffectStep *sparks;

  slots = &gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
  cursor = slots;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  sprites = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(1);
  REG_BLDALPHA = BLEND_ALPHA(16, 16); // an even mix
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  draw[0] = (DrawRectangle)EFFECT_SLOT(slots, HEAP_SLOT_BLITTER);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_MAX);
  draw[1] = (DrawRectangle)EFFECT_SLOT(slots, HEAP_SLOT_BLITTER_ALTERNATE);
  Resource_LoadAndDecompress((s32)&ResourceId_LightningPillarSheet, work->sheet,
                             1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);
  if (work->effect->variant != 2) {
    void *palette = Resource_GetTableEntry((s32)&ResourceId_VioletPaletteB);
    Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  }

  // Every spark starts idle.
  life = &((struct EffectStep *)gMapCellBuffer)->variant;
  for (k = 0; k != LIGHTNING_PILLAR_SPARK_COUNT; k++) {
    *life = 0;
    life += 7;
  }
  sparks = gMapCellBuffer;
  work->transferMode = CANVAS_COPY_AND_FADE; // trails keep half a frame
  work->transferValue = 50;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer,
                                EFFECT_CALLBACK_ORDER);
  count = gPlasma_Counts[work->effect->variant];

  for (frame = 0; frame != count * 8 - count + 48; frame++) {
    // Variant 2 swings the camera round for the first 64 frames.
    if (work->effect->variant == 2 && frame <= 63) {
      struct CameraWork *camera = (struct CameraWork *)gCameraWork;
      s32 speed;

      if (frame <= 55)
        speed = 256;
      else
        speed = 704 - frame * 8;
      if (work->effect->side == 1)
        camera->scroll -= speed;
      else
        camera->scroll += speed;
    }
    if (frame == 32)
      BattleEventRuntime_BeginPhaseFar(SE_HEAVY_IMPACT);

    // Pillar i strikes on frame 8i with a flash, shows its first image the
    // frame after and its second for two frames after that.
    for (i = 0; i != count; i++) {
      start = i * PLASMA_SPACING;
      if (frame == start) {
        AudioCommand_PlayFar(SE_HEAVY_IMPACT);
        Iwram_FillWords(canvas, 0x4000, 0x10101010);
      }
      if (frame >= start && frame < start + 9) {
        if (frame >= start + 1 && frame < start + 2)
          draw[0](canvas, work, PLASMA_COLUMN_X(work->effect->side, i) - 24, 0,
                  48, 112);
        if (frame >= start + 2 && frame < start + 4)
          draw[0](canvas, work->sheet + PLASMA_SECOND_IMAGE,
                  PLASMA_COLUMN_X(work->effect->side, i) - 24, 0, 48, 112);

        // Throw sparks from the pillar's foot (y 104), anywhere in the
        // upper half circle, and shake the screen.
        if (frame == start + 2) {
          s32 spawned = 0;

          for (k = 0; k != LIGHTNING_PILLAR_SPARK_COUNT; k++) {
            spark = &sparks[k];
            if (spark->variant == 0) {
              s32 speed;
              s32 angle;

              speed = (Random16() & 0x3ff) + 32;
              angle = (Random16() & 0x7fff) - 0x4000;

              spark->x = PLASMA_COLUMN_X(work->effect->side, i) << 16;
              spark->y = 104 << 16;
              spark->velocityX = (Trig_Sin(angle) * speed) >> 7;
              spark->velocityY = -((Trig_Cos(angle) * speed) << 1) >> 7;
              spark->variant = (Random16() & 7) + 32;
              spawned++;
              if (spawned ==
                  gPlasma_Sparks[work->effect->variant][PLASMA_SPARKS])
                break;
            }
          }
          work->shakeFrames =
              gPlasma_Sparks[work->effect->variant][PLASMA_SHAKE_FRAMES];
        }
      }
      // Two frames after the sparks the targets recoil, their motion first.
      if (frame == start + 4) {
        EFFECT_FOR_EACH_TARGET(k, work->effect) {
          BattleMotion_ApplyVariantMotionFar(work->effect->actors[k], 1);
          ObjectGroup_UpdateMembers(work->effect->actors[k], SPRITE_COLOR_WHITE,
                                    POSE_DAMAGE, k, 8);
        }
      }
    }

    // Sparks: bounce off the ground at half speed, and shrink as their lives
    // run out; only those on the canvas are drawn.
    for (k = 0; k != LIGHTNING_PILLAR_SPARK_COUNT; k++) {
      spark = &sparks[k];
      if (spark->variant > 0) {
        spark->variant--;
        EffectStep_AdvanceWithGravity2D(spark, 60, 0x1000);
        if (spark->y > 104 << 16) {
          spark->velocityY = -spark->velocityY / 2;
        } else {
          s32 x = spark->x >> 16;
          s32 y;
          s32 size;

          if (spark->x >= 0 && x <= 119 && spark->y >= 0) {
            y = spark->y >> 16;
            size = spark->variant / 8 + 1;
            EFFECT_DRAW_MOTE(draw[k & 1], canvas, sprites, x, y, size);
          }
        }
      }
    }
    Camera_ApplyShake(8, 16);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

// Spark bursts.
//
// A spark burst is a large flash, a ring of small flashes that fade, and a
// spray of sparks that fall and bounce on the ground. The spark groups scene
// strikes one to three such bursts, 8 frames apart, and the sword effects
// build their strikes from the same parts:
//
// - A flash is an EffectStep whose x and y are an offset from its burst's
//   centre and whose variant counts down its life. It is drawn only for its
//   last 18 frames, from a 32x64 flash picture chosen by its age.
// - A spark moves in 16.16 fixed point under its own gravity, and its variant
//   is its remaining life. Below the floor it bounces back at half speed
//   instead of being drawn; it shrinks as its life runs out.
// - A strike makes every target flash and recoil at once.
//
// The spark groups scene keeps its flashes in work->particles, sixteen per
// burst (only the first twelve are drawn), and its sparks in gMapCellBuffer,
// one run per burst.

// Abilities: Blast, Nova, Supernova, Fire Bomb, Cluster Bomb, Carpet Bomb (kind
// 15).
//
// Battle effect 15: bursts at (60, 48), the canvas behind the battle's
// first layer (priority 1).
DEFINE_EFFECT_KIND(BattleFx_RunNova, BattleFx_RunSparkGroups, 0)

// Abilities: Dynamite (kind 338).
//
// Battle effect 338: orange bursts at (60, 64), the canvas in front
// (priority 0).
DEFINE_EFFECT_KIND(BattleFx_RunDynamite, BattleFx_RunSparkGroups, 1)

#define SPARK_GROUPS_FLASHES 16 // flashes kept per burst
#define SPARK_GROUPS_SPACING 8  // frames between bursts
#define SPARK_GROUPS_RING 12    // of which drawn as the ring
#define SPARK_FLASH_DRAWN 18    // a flash is drawn for its last 18 frames

// One to three bursts, 8 frames apart: each flashes, rings itself with
// twelve smaller flashes that fade, and throws out sparks that bounce on
// the ground; every burst knocks the targets back.
// effect->variant picks the shape from gSparkGroups_Shapes: sparks per
// burst, how many bursts, and each burst's offset across. Variant 2 also
// scrolls the camera, and variant 3 flashes the whole canvas on frame 4.
// kind : 0 at (60, 48), the battle shows the damage on frame 24;
//        1 at (60, 64), orange; the damage shows on frame 2;
//        2 below the user, in the LightningBolt colours, with the sparks
//          rising instead of falling; the damage shows on frame 2.
void BattleFx_RunSparkGroups(struct BattleEffectArgument *effect, s32 kind) {
  struct EffectPosition screenPosition;
  BlitterPair draw;
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 i;
  s32 frame;
  u8 *graphics;
  s32 originX;
  s32 originY;
  s32 j;
  s32 start;
  struct EffectStep *point;
  struct EffectStep *spark;
  struct EffectStep *sparks;

  slots = gWorkSlot + HEAP_SLOT_BATTLE_EFFECT;
  cursor = slots;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  graphics = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  if (kind == 0) {
    BattleFx_BeginCanvasLayer(1);
    originX = 60;
    originY = 48;
  } else if (kind == 1) {
    BattleFx_BeginCanvasLayer(0);
    originX = 60;
    originY = 64;
  } else {
    BattleFx_BeginCanvasLayer(0);
    EffectPosition_ApplyStepAndYOffset(work->effect->actor, &screenPosition);
    originX = screenPosition.x / 2;
    originY = screenPosition.y + 48;
  }
  REG_BLDALPHA = BLEND_ALPHA(16, 16); // an even mix
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
  draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, graphics, 0, 0);
  if (kind == 1) {
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                    128);
  } else if (kind == 2) {
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_LightningBoltSheet),
                    128);
  }
  sparks = gMapCellBuffer;

  // For each burst: sixteen ring flashes (the first twelve are drawn) at
  // random angles, each lying further out (index * 2) and waiting longer
  // before it shows (life index / 2 + 25) the later it comes; and its
  // sparks, thrown in every direction, twice as hard up and down as across,
  // at speeds of 32 to 1055, living 32 to 39 frames.
  for (i = 0;
       i != gSparkGroups_Shapes[work->effect->variant][SPARK_GROUPS_BURSTS];
       i++) {
    for (j = 0; j != SPARK_GROUPS_FLASHES; j++) {
      s32 radius;
      s32 angle;

      point = &work->particles[i * SPARK_GROUPS_FLASHES + j];
      radius = j * 2;
      angle = Random16() & 0xffff;
      point->x = Trig_Sin(angle) * radius;
      point->y = -(Trig_Cos(angle) * radius);
      point->variant = j / 2 + 25;
    }
    for (j = 0;
         j != gSparkGroups_Shapes[work->effect->variant][SPARK_GROUPS_SPARKS];
         j++) {
      s32 speed;
      s32 angle;

      spark = &sparks[i * gSparkGroups_Shapes[work->effect->variant]
                                             [SPARK_GROUPS_SPARKS]] +
              j;
      speed = (Random16() & 0x3ff) + 32;
      angle = Random16() & 0xffff;
      if (work->effect->side == 1)
        spark->x = (originX -
                    gSparkGroups_Shapes[work->effect->variant]
                                       [SPARK_GROUPS_OFFSET + i] +
                    28)
                   << 16;
      else
        spark->x = (originX +
                    gSparkGroups_Shapes[work->effect->variant]
                                       [SPARK_GROUPS_OFFSET + i] -
                    28)
                   << 16;
      spark->y = originY << 16;
      spark->velocityX = (Trig_Sin(angle) * speed) >> 6;
      spark->velocityY = -((Trig_Cos(angle) * speed) << 1) >> 6;
      spark->variant = (Random16() & 7) + 32;
    }
  }
  work->transferMode = CANVAS_COPY_AND_FADE; // trails keep 3/4 a frame
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer,
                                EFFECT_CALLBACK_ORDER);

  for (frame = 0;
       frame !=
       gSparkGroups_Shapes[work->effect->variant][SPARK_GROUPS_BURSTS] *
               SPARK_GROUPS_SPACING +
           56;
       frame++) {
    struct CameraWork *camera =
        *(struct CameraWork **)(gWorkSlot + HEAP_SLOT_CAMERA);

    if (work->effect->variant == 2 && frame <= 51) {
      if (work->effect->side == 0)
        camera->scroll += 256;
      else
        camera->scroll -= 256;
    }
    /* FAKEMATCH: this fill calls the fixed entry itself;
     * through Iwram_FillWords a spilled invariant ahead of the frame loop is
     * built in r2 where the ROM uses r1 (3 of 776 instructions differ). */
    if (work->effect->variant == 3 && frame == 4)
      Iwram_FillWordsEntry(canvas, 0x4000, 0x3f3f3f3f);
    if (kind == 1 || kind == 2) {
      if (frame == 2)
        BattleEventRuntime_BeginPhaseFar(SE_145);
    } else {
      if (frame == 2)
        AudioCommand_PlayFar(SE_145);
      if (frame == 24)
        BattleEventRuntime_BeginPhaseFar(SE_HEAVY_IMPACT);
    }

    // Burst i starts on frame 8i with a two-frame flash, then its ring and
    // sparks; the targets reel 6 frames in.
    for (i = 0;
         i != gSparkGroups_Shapes[work->effect->variant][SPARK_GROUPS_BURSTS];
         i++) {
      start = i * SPARK_GROUPS_SPACING;
      if (frame == start)
        work->shakeFrames = 12;
      if (frame >= start && frame < start + 2) {
        if (work->effect->side == 1)
          draw[0](canvas, work,
                  originX -
                      gSparkGroups_Shapes[work->effect->variant]
                                         [SPARK_GROUPS_OFFSET + i] +
                      12,
                  originY - 32, 32, 64);
        else
          draw[0](canvas, work,
                  originX +
                      gSparkGroups_Shapes[work->effect->variant]
                                         [SPARK_GROUPS_OFFSET + i] -
                      44,
                  originY - 32, 32, 64);
      }
      if (frame >= start) {
        for (j = 0; j != SPARK_GROUPS_RING; j++) {
          s32 x;
          s32 y;

          point = &work->particles[i * SPARK_GROUPS_FLASHES] + j;
          y = HI(point->y) + originY;
          if (work->effect->side == 1)
            x = HI(point->x) + originX -
                gSparkGroups_Shapes[work->effect->variant]
                                   [SPARK_GROUPS_OFFSET + i] +
                28;
          else
            x = HI(point->x) + originX +
                gSparkGroups_Shapes[work->effect->variant]
                                   [SPARK_GROUPS_OFFSET + i] -
                28;
          if (point->variant >= 0 && point->variant < SPARK_FLASH_DRAWN)
            draw[0](canvas,
                    work->sheet +
                        (gSparkGroups_FlashCells[point->variant / 3] << 11),
                    x - 16, y - 32, 32, 64);
          if (point->variant > 0)
            point->variant--;
          else
            point->variant = -1;
        }
      }
      // The sparks fly (rising for kind 2). Below y 108 a spark bounces back
      // at half speed instead of being drawn; otherwise it is drawn while on
      // the canvas, shrinking with its life, alternate sparks additively.
      if (frame > start + 5) {
        s32 gravity;

        if (kind == 2)
          gravity = -0x1000;
        else
          gravity = 0x1000;
        for (j = 0;
             j !=
             gSparkGroups_Shapes[work->effect->variant][SPARK_GROUPS_SPARKS];
             j++) {
          spark = &sparks[i * gSparkGroups_Shapes[work->effect->variant]
                                                 [SPARK_GROUPS_SPARKS]] +
                  j;
          if (spark->variant > 0) {
            EffectStep_AdvanceWithGravity2D(spark, 60, gravity);
            spark->variant--;
            if (spark->y > 0x6c0000) {
              spark->velocityY = -spark->velocityY / 2;
            } else if (spark->x >= 0 && spark->x < 0x7f0000 && spark->y >= 0) {
              s32 y = spark->y >> 16;
              s32 x = spark->x >> 16;
              s32 size = spark->variant / 5 + 1;

              EFFECT_DRAW_MOTE(draw[j & 1], canvas, graphics, x, y, size);
            }
          }
        }
      }
      EFFECT_FOR_EACH_TARGET(j, work->effect) {
        if (frame == start + 6) {
          ObjectGroup_UpdateMembers(work->effect->actors[j], SPRITE_COLOR_WHITE,
                                    POSE_DAMAGE, j, 10);
          BattleMotion_ApplyVariantMotionFar(work->effect->actors[j], 4);
        }
      }
    }
    Camera_ApplyShake(16, 16);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}
