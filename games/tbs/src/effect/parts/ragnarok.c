// Part of effect.c, the battle effect object, compiled only through it.
// Ragnarok.

// The flame blade (Ragnarok) and the falling sword (Titan Blade): a huge
// sword cuts or drops onto the targets amid flashes, sparks and dust.
//
// Both scenes draw a large sword picture onto the effect canvas and time
// everything else by frame number: sounds, screen shakes, palette fades and
// the targets' reactions each happen on a fixed frame.
//
// - Ragnarok swings a 64x128 blade down over 10 frames, pauses until frame
//   20 and then creeps on until frame 39. Two columns of light strike at
//   frames 16 and 24: each is a large flash, a ring of 12 small flashes and
//   256 sparks that bounce on the ground. The targets recoil as each
//   column's sparks fly.
// - Titan Blade raises embers from frame 28 while a pillar of fire scrolls
//   over the target. The sword then drops onto the target, a ring of
//   flashes bursts from frame 56, sparks fly from frame 60 and the embers
//   swell as they clear. The canvas is wiped at frames 9 and 60.
//
// Flashes live in work->particles and the sword pictures in the work sheet.
// Sparks and embers share PARTICLES: Ragnarok keeps 340 sparks per column
// there, Titan Blade 684 embers and then its sparks.

#define FLAME_BLADE_FLASH_CELLS                                                \
  (64 * 128) // Place flash graphics after the 64x128 sword image.

#define RAGNAROK_FRAMES 80        // frames in the effect
#define RAGNAROK_COLUMNS 2        // columns that strike
#define RAGNAROK_SEEDED 3         // columns given particles
#define RAGNAROK_FLASHES 16       // flashes kept per column
#define RAGNAROK_RING 12          // of which drawn as the ring
#define RAGNAROK_SPARKS 340       // sparks kept per column
#define RAGNAROK_DRAWN_SPARKS 256 // of which flown

// gRagnarok_StrikeColumns has three columns for each side.
#define RAGNAROK_COLUMN_X(side, column)                                        \
  gRagnarok_StrikeColumns[(side)][(column)]

// Abilities: Ragnarok (kind 58).
//
// A huge flaming blade swings down over the targets and two columns of light
// strike through them. The sword is drawn for 40 frames: it swings down over
// 10 frames, pauses until frame 20, then creeps on. The columns flash at
// frames 16 and 24 and scatter sparks six frames later, as the targets
// recoil; the battle shows the damage on frame 40. Column x comes from
// gRagnarok_StrikeColumns: first two of three per side. PARTICLES holds 340
// sparks per column.
void BattleFx_RunRagnarok(struct BattleEffectArgument *effect) {
  void **heapCache;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 col;
  u8 *sheet;
  struct MotionObject *user;
  DrawRectangle blit[2];
  s32 frame;
  s32 i;

  EFFECT_GET_SLOTS(heapCache, cursor, work, canvas);
  sheet = EFFECT_SLOT(heapCache, HEAP_SLOT_BATTLE_SHEET);

  user = GetBattleObjectSlotFar(effect->actor)->object;
  effect->variant = 1;
  work->effect = effect;
  BattleFx_BeginCanvasLayer(1);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  BattleFx_FetchRectangleBlitters(work->effect->side, blit);
  // The user takes its action pose and animates fast until the end.
  ObjectDispatch_ApplyArgumentToChildrenFar(user, 2);
  ObjectDispatch_ApplyValueToChildrenFar(user, 48);

  Resource_LoadAndDecompress((s32)&ResourceId_FlameBladeSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             work->sheet + FLAME_BLADE_FLASH_CELLS, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);

  //------------------------------------------------------------------
  // Prepare 16 flashes and 340 sparks per column.
  //------------------------------------------------------------------
  col = 0;
  do {
    // Flashes: i * 2 pixels from the center, lifetime 25-32.
    i = 0;
    do {
      struct EffectStep *flash = &work->particles[col * RAGNAROK_FLASHES + i];

      s32 radius = i * 2;
      s32 angle = Random16() & 0xffff;

      flash->x = Trig_Sin(angle) * radius;
      flash->y = -(Trig_Cos(angle) * radius);
      flash->variant = i / 2 + 25;
      i++;
    } while (i != RAGNAROK_FLASHES);

    // Sparks: random direction and speed from column y = 88, lifetime 32-39.
    i = 0;
    do {
      struct EffectStep *spark = &PARTICLES[col * RAGNAROK_SPARKS + i];

      s32 speed;
      s32 angle;

      speed = (Random16() & 0x1ff) + 32;
      angle = Random16() & 0xffff;
      spark->x = RAGNAROK_COLUMN_X(work->effect->side, col) << 16;
      spark->y = 88 << 16;
      spark->velocityX = (Trig_Sin(angle) * speed) >> 6;
      spark->velocityY = -(Trig_Cos(angle) * speed * 2) >> 6;
      spark->variant = (Random16() & 7) + 32;
      i++;
    } while (i != RAGNAROK_SPARKS);
    col++;
  } while (col != RAGNAROK_SEEDED);

  work->transferMode = CANVAS_COPY_AND_FADE;
  work->transferValue = 75; // Reduce to 3/4 on each transfer for an afterimage.
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer,
                                EFFECT_CALLBACK_ORDER);

  frame = 0;
  do {
    // Sounds and the first shake; the battle shows the damage on frame 40.
    if (frame == 4) {
      AudioCommand_PlayFar(SE_212);
    }
    if (frame == 8) {
      work->shakeFrames = frame;
    }
    if (frame == 18) {
      AudioCommand_PlayFar(SE_145);
    }
    if (frame == 40) {
      BattleEventRuntime_BeginPhaseFar(SE_HEAVY_IMPACT);
    }

    //------------------------------------------------------------------
    // Swing down over 10 frames, pause until frame 20, then advance slowly.
    // Do not draw below y = 104.
    //------------------------------------------------------------------
    if (frame <= 39) {
      s32 height;
      s32 x;
      s32 top;

      height = 128;
      if (work->effect->side == 1) {
        if (frame <= 9) {
          x = frame * 10 - 8;
          top = frame * 16 - 128;
        } else if (frame > 20) {
          x = frame + 62;
          top = frame * 2 - 24;
        } else {
          x = 82;
          top = 16;
        }
      } else {
        if (frame <= 9) {
          x = 128 - frame * 10;
          top = frame * 16 - 128;
        } else if (frame > 20) {
          x = 58 - frame;
          top = frame * 2 - 24;
        } else {
          x = 38;
          top = 16;
        }
      }
      if (top + 128 > 104) {
        height = height - top - 24;
      }
      if (height > 0) {
        blit[0](canvas, work, x - 32, top, 64, height);
      }
    }

    // From frame 17 the palette fades towards the blast colours.
    if (frame > 16) {
      BattleFx_StepPaletteToResource((s32)&ResourceId_BlastSheet);
    }

    //------------------------------------------------------------------
    // Two columns: flashes at start, sparks at start + 6.
    //------------------------------------------------------------------
    col = 0;
    do {
      s32 start = col * 8 + 16;

      if (frame == start) {
        work->shakeFrames = 12;
      }
      if (frame >= start) {
        // Large flash for the first two frames, then 12 small flashes.
        if (frame < start + 2) {
          blit[0](canvas, work->sheet + FLAME_BLADE_FLASH_CELLS,
                  RAGNAROK_COLUMN_X(work->effect->side, col) - 16, 56, 32, 64);
        }
        i = 0;
        do {
          struct EffectStep *flash =
              &work->particles[col * RAGNAROK_FLASHES + i];

          s32 x;
          s32 life;
          s32 y;

          y = HI(flash->y);
          x = HI(flash->x) + RAGNAROK_COLUMN_X(work->effect->side, col);
          life = flash->variant;
          if (life >= 0 && life <= 17) {
            blit[0](canvas,
                    work->sheet + (gRagnarok_FlashCells[life / 3] << 11) +
                        FLAME_BLADE_FLASH_CELLS,
                    x - 16, y + 56, 32, 64);
          }
          life = flash->variant;
          flash->variant = life > 0 ? life - 1 : -1;
          i++;
        } while (i != RAGNAROK_RING);
      }

      // Sparks bounce below y = 108 at half speed; draw only within x = 0-126.
      if (frame > start + 5) {
        i = 0;
        do {
          struct EffectStep *spark = &PARTICLES[col * RAGNAROK_SPARKS + i];

          if (spark->variant > 0) {
            s32 x;
            s32 y;
            s32 size;

            EffectStep_AdvanceWithGravity2D(spark, 64, 0x1000);
            spark->variant--;
            y = spark->y;
            if (y > (108 << 16)) {
              spark->velocityY = -spark->velocityY / 2;
            } else {
              x = spark->x;
              if ((x >= 0 && x <= 0x7effff) && y >= 0) {
                u8 *cel;
                s32 lane;
                s32 py = y >> 16;
                s32 px = x >> 16;

                // Size follows lifetime; alternate comparison and additive
                // compositing.
                size = spark->variant / 5 + 1;
                lane = i & 1;
                cel = sheet + gBattleFx_MoteCells[size - 1];
                px -= size / 2;
                py -= size;
                blit[lane](canvas, cel, px, py, size, size * 2);
              }
            }
          }
          i++;
        } while (i != RAGNAROK_DRAWN_SPARKS);
      }

      // As the column's sparks start to fly, every target flashes and
      // recoils.
      EFFECT_FOR_EACH_TARGET(i, work->effect) {
        if (frame == start + 6) {
          ObjectGroup_UpdateMembers(work->effect->actors[i], SPRITE_COLOR_WHITE,
                                    POSE_DAMAGE, i, 10);
          BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 4);
        }
      }
      col++;
    } while (col != RAGNAROK_COLUMNS);

    Camera_ApplyShake(16, 16);
    EFFECT_END_FRAME(work);
    frame++;
  } while (frame != RAGNAROK_FRAMES);

  ObjectDispatch_ApplyValueToChildrenFar(user, 16);
  EFFECT_FINISH_CANVAS();
}
