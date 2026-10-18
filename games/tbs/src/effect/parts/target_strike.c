// Part of effect.c, the battle effect object, compiled only through it.
// Monster bites, fangs and slashes on one target.

// Target strikes, actor attacks and tiled scene transitions.
// The runners coordinate their own drawing callbacks and release them
// before returning their drawing buffers.

// Target strikes: repeated slashes or columns of light on the first target.
//
// One runner, BattleFx_RunTargetStrike, plays all twelve strike styles. Each
// style's row gives its kind (0 for a sideways slash, 1 for a column of
// light), its number of hits, its extras (smoke, an absorbing drain, glints,
// sparks or one heavy hit), its palette, the frames each picture is held, the
// column's composite mode and the scene's duration. The wrappers above it
// name the style of each effect.
//
// The scene runs in three phases:
//
// - Setup loads the pictures and the style's palette, then parks every
//   particle, idle, where it will start: 64 smoke puffs at the target's feet,
//   128 motes and 512 sparks 20 pixels up its body, each with a random
//   velocity. A particle is idle while its variant is -1.
// - Each frame first draws the strike (a slash sweep or a column) and, on a
//   hit frame, shakes and flashes the target and lights the next group of
//   particles. Glints are then scattered around the target, and the lit
//   smoke, motes and sparks are drawn and moved.
// - The finish releases the canvas layer.
//
// Several steps consume random numbers, so their order within a frame is part
// of the effect.

// The strike sparks live after the motes in gMapCellBuffer.
#define TARGET_STRIKE_SPARKS ((struct EffectStep *)(Ram_MapCellBuffer + 0x3800))

// gTargetStrike_Records has seven entries for each style.
#define STRIKE_VALUE(mode, field) gTargetStrike_Records[(mode)][(field)]
enum {
  STRIKE_KIND,         // STRIKE_KIND_SLASH or STRIKE_KIND_COLUMN
  STRIKE_HITS,         // how many times the target is struck
  STRIKE_FLAGS,        // STRIKE_SMOKE and so on
  STRIKE_COLOUR,       // background palette, 0 to 5 (see below)
  STRIKE_IMAGE_FRAMES, // frames each strike image is shown
  STRIKE_COMPOSITE,    // how the columns are drawn (COMPOSITE_MAX or _ADDITIVE)
  STRIKE_FRAMES        // length of the effect in frames
};
// STRIKE_KIND_* and the STRIKE_SMOKE flags are in effect_table.h.

// Rows of gTargetStrike_Records used by these effect entry points.
enum {
  STRIKE_STYLE_BLUE_SPARK_COLUMNS = 0,
  STRIKE_STYLE_LIME_SMOKE_COLUMN = 1,
  STRIKE_STYLE_EMBER_DRAIN_COLUMNS = 3,
  STRIKE_STYLE_MARS_DRAIN_COLUMNS = 4,
  STRIKE_STYLE_LIGHTNING_GLINT_COLUMNS = 5,
  STRIKE_STYLE_LIGHTNING_GLINT_SLASH = 6,
  STRIKE_STYLE_LIME_SMOKE_SLASH = 9,
  STRIKE_STYLE_MARS_SMOKE_COLUMN = 10
};

// Abilities: Numbing Sting (kind 313).
//
// Battle effect 313: one slash with glints, lightning colours.
DEFINE_EFFECT_KIND(BattleFx_RunNumbingSting, BattleFx_RunTargetStrike,
                   STRIKE_STYLE_LIGHTNING_GLINT_SLASH)

// Abilities: Cannibal Fang (kind 316).
//
// Battle effect 316: three ember columns that drain the target into the user.
DEFINE_EFFECT_KIND(BattleFx_RunCannibalFang, BattleFx_RunTargetStrike,
                   STRIKE_STYLE_EMBER_DRAIN_COLUMNS)

// Abilities: Poisonous Bite (kind 318); Poison Fang (kind 323).
//
// Battle effects 318 and 323: one lime column with smoke.
DEFINE_EFFECT_KIND(BattleFx_RunPoisonousBite, BattleFx_RunTargetStrike,
                   STRIKE_STYLE_LIME_SMOKE_COLUMN)

// Abilities: Rabid Fang (kind 336).
//
// Battle effect 336: one column with smoke, Mars Djinn colours.
DEFINE_EFFECT_KIND(BattleFx_RunRabidFang, BattleFx_RunTargetStrike,
                   STRIKE_STYLE_MARS_SMOKE_COLUMN)

// Abilities: Electric Bite (kind 324).
//
// Battle effect 324: three lightning columns with glints.
DEFINE_EFFECT_KIND(BattleFx_RunElectricBite, BattleFx_RunTargetStrike,
                   STRIKE_STYLE_LIGHTNING_GLINT_COLUMNS)

// Abilities: Poison Tail (kind 325).
//
// Battle effect 325: one lime slash with smoke.
DEFINE_EFFECT_KIND(BattleFx_RunPoisonTail, BattleFx_RunTargetStrike,
                   STRIKE_STYLE_LIME_SMOKE_SLASH)

// Abilities: Vampiric Fang (kind 327).
//
// Battle effect 327: three columns that drain the target, Mars Djinn colours.
DEFINE_EFFECT_KIND(BattleFx_RunVampiricFang, BattleFx_RunTargetStrike,
                   STRIKE_STYLE_MARS_DRAIN_COLUMNS)

// Abilities: Double Fang (kind 332).
//
// Battle effect 332: two blue columns with sparks.
DEFINE_EFFECT_KIND(BattleFx_RunDoubleFang, BattleFx_RunTargetStrike,
                   STRIKE_STYLE_BLUE_SPARK_COLUMNS)

// Abilities: Mortal Blow (kind 333).
//
// Battle effect 333: one heavy slash with sparks, Mars Djinn colours.
DEFINE_EFFECT_KIND(BattleFx_RunMortalBlow, BattleFx_RunTargetStrike, 8)

// Abilities: Twin Beaks (kind 335).
//
// Battle effect 335: two slashes with sparks, lightning colours.
DEFINE_EFFECT_KIND(BattleFx_RunTwinBeaks, BattleFx_RunTargetStrike, 7)

// Abilities: Acid Bite (kind 337).
//
// Battle effect 337: three pink columns with smoke.
DEFINE_EFFECT_KIND(BattleFx_RunAcidBite, BattleFx_RunTargetStrike, 2)

// Abilities: Drain Fang (kind 386).
//
// Battle effect 386: three additive columns that drain the target with large
// motes, Mars Djinn colours.
DEFINE_EFFECT_KIND(BattleFx_RunDrainFang, BattleFx_RunTargetStrike, 0xB)

// Strikes the first target STRIKE_HITS times with a sideways slash or a
// column of light, with the extras its flags choose, and lets the battle
// show the damage on the hits.
// mode : the style, 0 to 11, a row of gTargetStrike_Records.
void BattleFx_RunTargetStrike(struct BattleEffectArgument *effect, s32 mode) {
  struct EffectPosition ground;
  struct EffectPosition targetPosition;
  struct EffectPosition alternate;
  struct EffectPosition position;
  BlitterPair draw;
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 k;
  u8 *particleSheet;
  struct BattleCamera *camera;
  struct MotionObject *target;
  struct MotionObject *source;
  s32 resource;
  s32 i;
  s32 wispSheet = 0x3200;
  DrawRectangle *lower = &gBlitters[HEAP_SLOT_BLITTER_ALTERNATE];

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  particleSheet = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  camera = EFFECT_SLOT(slots, HEAP_SLOT_CAMERA);
  work->effect = effect;
  if (mode == 8)
    BattleFx_BeginCanvasLayer(0);
  else
    BattleFx_BeginCanvasLayer(1);
  REG_BLDALPHA = BLEND_ALPHA(16, 16); // an even mix

  // The images: glints at the start of the sheet, the slashes or the
  // columns at 0xc80, smoke at 0x3200, and the style's palette.
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, particleSheet,
                             0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet, 1, 0);
  if (STRIKE_VALUE(mode, STRIKE_KIND) == STRIKE_KIND_SLASH)
    Resource_LoadAndDecompress((s32)&ResourceId_BeamSequenceImage,
                               work->sheet + 0xc80, 0, 0);
  else
    Resource_LoadAndDecompress((s32)&ResourceId_TwelveModeImage,
                               work->sheet + 0xc80, 0, 0);
  switch (STRIKE_VALUE(mode, STRIKE_COLOUR)) {
  case 0:
    resource = (s32)&ResourceId_BlueBeamSheet;
    break;
  case 1:
    resource = (s32)&ResourceId_LimePalette;
    break;
  case 2:
    resource = (s32)&ResourceId_PinkPalette;
    break;
  case 3:
    resource = (s32)&ResourceId_EmberStreakSheet;
    break;
  case 4:
    resource = (s32)&ResourceId_MarsDjinnSheet;
    break;
  case 5:
  default:
    resource = (s32)&ResourceId_LightningBoltSheet;
    break;
  }
  Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(resource), 128);
  Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, work->sheet + 0x3200,
                             1, 0);

  // Park every particle, idle, where it will start: smoke at the target's
  // feet, motes and sparks 20 pixels up its body, each with a random
  // velocity.
  for (k = 0; k != 1; k++) {
    target = GetBattleObjectSlotFar(work->effect->actors[k])->object;
    for (i = 0; i != 64; i++) {
      struct EffectStep *step = &work->particles[i];

      EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actors[k],
                                                  &ground);
      SET_POSITION(step, (ground.x / 2) << 16, 0x500000, 0);
      SET_VELOCITY(step, ((Random16() & 255) - 128) << 9,
                   ((Random16() & 255) - 128) << 9, 0);
      step->variant = -1;
    }
    for (i = 0; i != 128; i++) {
      struct EffectStep *step = &PARTICLES[k * 128 + i];

      SET_POSITION(step, target->x, 0x140000, target->z);
      SET_VELOCITY(step, ((Random16() & 255) - 128) << 11,
                   ((Random16() & 255) - 128) << 11,
                   ((Random16() & 255) - 128) << 11);
      step->variant = -1;
    }
    for (i = 0; i != 512; i++) {
      TARGET_STRIKE_SPARKS[i].x = target->x;
      TARGET_STRIKE_SPARKS[i].y = 0x140000;
      TARGET_STRIKE_SPARKS[i].z = target->z;
      if (STRIKE_VALUE(mode, STRIKE_KIND) == STRIKE_KIND_COLUMN) {
        TARGET_STRIKE_SPARKS[i].velocityX = ((Random16() & 255) - 128) << 11;
        TARGET_STRIKE_SPARKS[i].velocityY = ((Random16() & 255) - 128) << 11;
        TARGET_STRIKE_SPARKS[i].velocityZ = ((Random16() & 255) - 128) << 11;
      } else {
        if (STRIKE_VALUE(mode, STRIKE_FLAGS) & 16) {
          TARGET_STRIKE_SPARKS[i].velocityX = (Random16() & 0x1ff) << 11;
          TARGET_STRIKE_SPARKS[i].velocityY = ((Random16() & 0x1ff) - 256)
                                              << 11;
          TARGET_STRIKE_SPARKS[i].velocityZ = ((Random16() & 0x1ff) - 256)
                                              << 11;
        } else {
          TARGET_STRIKE_SPARKS[i].velocityX = (Random16() & 255) << 11;
          TARGET_STRIKE_SPARKS[i].velocityY = ((Random16() & 255) - 128) << 11;
          TARGET_STRIKE_SPARKS[i].velocityZ = ((Random16() & 255) - 128) << 11;
        }
        if (TARGET_STRIKE_SPARKS[i].x >
            0) // slashes throw sparks towards the middle
          TARGET_STRIKE_SPARKS[i].velocityX =
              -TARGET_STRIKE_SPARKS[i].velocityX;
      }
      TARGET_STRIKE_SPARKS[i].variant = -1;
    }
  }

  work->transferMode = CANVAS_COPY_AND_FADE; // trails keep 3/4 a frame
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer,
                                EFFECT_CALLBACK_ORDER);

  // Strike halfway between the target's middle and its feet.
  EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &targetPosition);
  EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actors[0],
                                              &alternate);
  targetPosition.y += (alternate.y - targetPosition.y) / 2;

  for (frame = 0; frame != STRIKE_VALUE(mode, STRIKE_FRAMES); frame++) {
    s32 span = STRIKE_VALUE(mode, STRIKE_HITS) *
               STRIKE_VALUE(mode, STRIKE_IMAGE_FRAMES);
    s32 length = span * 4;
    s32 glint = 0;

    if (STRIKE_VALUE(mode, STRIKE_KIND) == STRIKE_KIND_SLASH) {
      // Slashes: six images sweep across the target from the user's side,
      // once per hit; the fifth image of each sweep is the hit.
      if (frame < span * 6) {
        s32 picture = frame / STRIKE_VALUE(mode, STRIKE_IMAGE_FRAMES) % 6;

        if (work->effect->side == 1) {
          EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                              EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                              COMPOSITE_MAX);
          gBlitters[HEAP_SLOT_BLITTER](
              canvas, work->sheet + gTargetStrike_Offsets[picture],
              targetPosition.x / 2 - (gTargetStrike_Reach[picture] / 2) -
                  gTargetStrike_Widths[picture] + 8,
              targetPosition.y - (gTargetStrike_Heights[picture] / 2),
              gTargetStrike_Widths[picture], gTargetStrike_Heights[picture]);
        } else {
          EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS,
                              COMPOSITE_MAX);
          gBlitters[HEAP_SLOT_BLITTER](
              canvas, work->sheet + gTargetStrike_Offsets[picture],
              targetPosition.x / 2 + (gTargetStrike_Reach[picture] / 2) - 8,
              targetPosition.y - (gTargetStrike_Heights[picture] / 2),
              gTargetStrike_Widths[picture], gTargetStrike_Heights[picture]);
        }
        Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
        if (frame % (STRIKE_VALUE(mode, STRIKE_IMAGE_FRAMES) * 6) ==
            STRIKE_VALUE(mode, STRIKE_IMAGE_FRAMES) * 4) {
          if (mode == 8) {
            BattleEventRuntime_BeginPhaseFar(SE_HEAVY_IMPACT);
          } else {
            AudioCommand_PlayFar(SE_133);
            BattleEventRuntime_BeginPhaseFar(SE_133);
          }
          // A heavy hit lights every spark at once; otherwise the hit
          // lights the next 32. Each hit also starts the next 8 smoke
          // puffs.
          if (STRIKE_VALUE(mode, STRIKE_FLAGS) & STRIKE_HEAVY) {
            work->shakeFrames = 8;
            EFFECT_STRIKE_TARGET(work->effect, 0, 12, 4);
            for (i = 0; i != 512; i++)
              TARGET_STRIKE_SPARKS[i].variant = (Random16() & 15) + 15;
          } else {
            work->shakeFrames = 4;
            ObjectGroup_UpdateMembers(work->effect->actors[0],
                                      SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 8);
            for (i = 0; i != 32; i++) {
              struct EffectStep *step =
                  &TARGET_STRIKE_SPARKS
                      [frame / (STRIKE_VALUE(mode, STRIKE_IMAGE_FRAMES) * 6) *
                           32 +
                       i];

              step->variant = (Random16() & 15) + 7;
            }
          }
          for (i = 0; i != 8; i++) {
            struct EffectStep *step =
                &work->particles[frame /
                                     (STRIKE_VALUE(mode, STRIKE_IMAGE_FRAMES) *
                                      6) *
                                     16 +
                                 i];

            step->variant = 0;
          }
        }
      }
      // Slashes show their glints on frames 12 to 31; columns while stage 3
      // is drawn.
      if (frame >= 12 && frame <= 31)
        glint = 1;
    } else if (frame < length + 4) {
      // Columns: a column of light, drawn as two mirrored halves, rises
      // through its stages on the target once per hit, then holds stage 3
      // for 4 frames.
      s32 stage;

      if (frame < length) {
        s32 picture = frame / STRIKE_VALUE(mode, STRIKE_IMAGE_FRAMES);

        while (picture > 4)
          picture -= 4;
        stage = gTargetStrike_ColumnStages[picture];
      } else {
        stage = 3;
      }
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS,
                          STRIKE_VALUE(mode, STRIKE_COMPOSITE));
      gBlitters[HEAP_SLOT_BLITTER](canvas, work->sheet + stage * 864 + 0xc80,
                                   targetPosition.x / 2 - 18, 56, 18, 48);
      Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                          STRIKE_VALUE(mode, STRIKE_COMPOSITE));
      gBlitters[HEAP_SLOT_BLITTER](canvas, work->sheet + stage * 864 + 0xc80,
                                   targetPosition.x / 2, 56, 18, 48);
      Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
      if (frame % (STRIKE_VALUE(mode, STRIKE_IMAGE_FRAMES) * 4) ==
          STRIKE_VALUE(mode, STRIKE_IMAGE_FRAMES) * 3) {
        // A hit: the target flashes; the last hit lets the battle show the
        // damage, the others only play the hit sound. Each hit lights the
        // next sparks, smoke puffs and motes.
        ObjectGroup_UpdateMembers(work->effect->actors[0], SPRITE_COLOR_WHITE,
                                  POSE_DAMAGE, 0, 8);
        work->shakeFrames = 4;
        if (frame > (STRIKE_VALUE(mode, STRIKE_HITS) * 4 - 4) *
                        STRIKE_VALUE(mode, STRIKE_IMAGE_FRAMES))
          BattleEventRuntime_BeginPhaseFar(SE_133);
        else
          AudioCommand_PlayFar(SE_133);
        for (i = 0; i != 64; i++) {
          struct EffectStep *step =
              &TARGET_STRIKE_SPARKS
                  [frame / (STRIKE_VALUE(mode, STRIKE_IMAGE_FRAMES) * 6) * 64 +
                   i];

          step->variant = (Random16() & 15) + 7;
        }
        for (i = 0; i != 8; i++) {
          struct EffectStep *step =
              &work->particles
                   [frame / (STRIKE_VALUE(mode, STRIKE_IMAGE_FRAMES) * 6) * 16 +
                    i];

          step->variant = 0;
        }
        for (i = 0; i != 16; i++)
          PARTICLES[frame / (STRIKE_VALUE(mode, STRIKE_IMAGE_FRAMES) * 6) * 16 +
                    i]
              .variant = 0;
      }
      if (stage == 3)
        glint = 1;
    }

    // Glints: three a frame, scattered at random around the target in
    // random flips.
    if ((STRIKE_VALUE(mode, STRIKE_FLAGS) & STRIKE_GLINTS) && glint) {
      for (i = 0; i != 3; i++) {
        s32 image = frame & 3;
        s32 angle = Random16() & 0xffff;
        s32 range = 31;
        s32 radius = Random16() & range;
        s32 x;
        s32 y;

        x = targetPosition.x / 2 + (Trig_Sin(angle) * (radius + 4) >> 17) -
            (gBattleFx_GlintCellWidths[image] / 2);
        y = targetPosition.y - (Trig_Cos(angle) * (radius + 4) >> 17) -
            (gBattleFx_GlintCellHeights[image] / 2);
        EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                            gTargetStrike_GlintDrawFlags[Random16() & 3] |
                                EFFECT_CLIP_BOUNDS,
                            COMPOSITE_ADDITIVE);
        (*lower)(canvas, work->sheet + gBattleFx_GlintCells[image], x, y,
                 gBattleFx_GlintCellWidths[image],
                 gBattleFx_GlintCellHeights[image]);
        Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
      }
    }

    EFFECT_VIEW_FROM_CAMERA(camera);
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
    draw[0] = gBlitters[HEAP_SLOT_BLITTER];
    EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE, EFFECT_CLIP_BOUNDS,
                        COMPOSITE_ADDITIVE);
    draw[1] = *lower;
    for (k = 0; k != 1; k++) {
      source = GetBattleObjectSlotFar(work->effect->actor)->object;
      // Smoke puffs rise and spread through six images over 24 frames.
      if (STRIKE_VALUE(mode, STRIKE_FLAGS) & STRIKE_SMOKE) {
        for (i = 0; i != 64; i++) {
          struct EffectStep *step = &work->particles[i];

          if (step->variant >= 0 && step->variant <= 23) {
            s32 picture = step->variant / 4;

            draw[i & 1](canvas, work->sheet + picture * 1152 + wispSheet,
                        HI(step->x) - 12, HI(step->y) - 24, 24, 48);
            EffectStep_AdvanceWithGravity2D(step, 62, -0x400);
            step->variant++;
          }
        }
      }
      // Motes burst out of the target, then after 10 frames are pulled
      // into the user, who glows from frame 55 to 90; a mote goes out when
      // it reaches the user.
      if (STRIKE_VALUE(mode, STRIKE_FLAGS) & STRIKE_ABSORB) {
        s32 size = 3;

        if (mode == 11)
          size = 8;
        if (frame == 55)
          ObjectGroup_UpdateMembers(work->effect->actor, SPRITE_COLOR_WHITE, -1,
                                    -1, 0);
        if (frame == 90)
          ObjectGroup_UpdateMembers(work->effect->actor, 0, -1, -1, 0);
        for (i = 0; i != 64; i++) {
          struct EffectStep *step = &PARTICLES[i];

          if (step->variant >= 0) {
            EffectPosition_ApplyBaseAndYOffset(step, &position);
            position.x >>= 1;
            draw[1](canvas, particleSheet + gBattleFx_MoteCells[size - 1],
                    position.x - ((u32)size >> 1), position.y - size, size,
                    size * 2);
            EffectStep_AdvanceWithGravity3D(step, 60, 0);
            step->variant++;
            if (step->variant > 10) {
              s32 dx = (source->x - step->x) >> 8;
              s32 dy = (source->y - step->y + 0x140000) >> 8;
              s32 dz = (source->z - step->z) >> 8;

              step->velocityX += dx;
              step->velocityY += dy;
              step->velocityZ += dz;
              if ((dx >= -0xfff && dx <= 0xfff) &&
                  (dz >= -0xfff && dz <= 0xfff))
                step->variant = -1;
            }
          }
        }
      }
      // Sparks fly out and shrink as their lives run out.
      if (STRIKE_VALUE(mode, STRIKE_FLAGS) & STRIKE_SPARKS) {
        for (i = 0; i != 128; i++) {
          struct EffectStep *step = &TARGET_STRIKE_SPARKS[i];
          s32 size = step->variant;

          if (size >= 0) {
            EffectPosition_ApplyBaseAndYOffset(step, &position);
            position.x >>= 1;
            EffectStep_AdvanceWithGravity3D(step, 60, 0);
            size >>= 3;
            size++;
            EFFECT_DRAW_MOTE(draw[1], canvas, particleSheet, position.x,
                             position.y, size);
            step->variant--;
          }
        }
      }
    }
    EFFECT_RELEASE_BLITTERS();
    if (STRIKE_VALUE(mode, STRIKE_FLAGS) & STRIKE_HEAVY)
      Camera_ApplyShake(8, 8);
    else
      Camera_ApplyShake(2, 4);
    EFFECT_END_FRAME(work);
  }

  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  BattleFx_EndCanvasLayer();
}
