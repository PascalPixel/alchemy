// Part of effect.c, the battle effect object, compiled only through it.
// Titan Blade.

// Titan Blade raises embers from frame 28 while a pillar of fire scrolls over
// the target. The sword then drops onto the target, a ring of flashes bursts
// from frame 56, sparks fly from frame 60 and the embers swell as they clear.
// The canvas is wiped at frames 9 and 60. The flashes live in work->particles
// and the sword pictures in the work sheet; PARTICLES holds 684 embers and
// then the sparks.

#define FALLING_SWORD_SPARKS                                                   \
  ((struct EffectStep *)(Ram_MapCellBuffer +                                   \
                         TITAN_BLADE_DUST * sizeof(struct EffectStep)))

#define TITAN_BLADE_FRAMES 102
#define TITAN_BLADE_SWORD_IMAGE 20000 // the sword picture in the sheet
#define TITAN_BLADE_FLASHES 32        // flashes scattered
#define TITAN_BLADE_RING 16           // of which drawn as the ring
#define TITAN_BLADE_DUST 684       // dust slots in PARTICLES, before the sparks
#define TITAN_BLADE_FIRST_DUST 256 // dust lit at once on frame 28
#define TITAN_BLADE_DRAWN_DUST 342 // dust moved and drawn
#define TITAN_BLADE_SPARKS 340     // sparks after the dust

// Abilities: Titan Blade (kind 61).
//
// A huge sword drops onto the first target amid charging flashes, dust and
// sparks. Dust rises at the target's feet from frame 28 while a pillar of
// fire scrolls over it; the sword drops in, holds, sinks and plunges after
// frame 60. A ring of flashes bursts from frame 56, sparks fly from frame 60
// as the battle shows the damage, and every target flashes and recoils on
// frame 68. The canvas is wiped at frames 9 and 60.
void BattleFx_RunTitanBlade(struct BattleEffectArgument *effect) {
  void **heap;
  void **slotCursor;
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle blitters[2];
  u8 *sheet;
  struct EffectPosition screenPosition;
  s32 frame;
  s32 canvasCenterX;
  s32 step;
  s32 sparkOriginX;
  s32 i;

  EFFECT_GET_SLOTS(heap, slotCursor, work, canvas);
  sheet = heap[2];

  work->effect = effect;
  BattleFx_BeginCanvasLayer(1);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actors[0],
                                              &screenPosition);
  canvasCenterX = screenPosition.x / 2;

  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                      COMPOSITE_MAX);
  blitters[0] = heap[7];
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                      COMPOSITE_TRANSPARENT);
  blitters[1] = heap[8];

  Resource_LoadAndDecompress((s32)&ResourceId_SwordSheet,
                             work->sheet + TITAN_BLADE_SWORD_IMAGE, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_FirePillarSheetA, work->sheet, 1,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             work->sheet + (221 << 4), 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);

  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);

  i = 0;
  do {
    struct EffectStep *flash = &work->particles[i];

    s32 scale = i * 2;
    s32 ang = Random16() & 0xFFFF;

    flash->x = Trig_Sin(ang) * scale;
    flash->y = -(Trig_Cos(ang) * scale);
    flash->variant = i / 2 + 25;
    i++;
  } while (i != TITAN_BLADE_FLASHES);

  i = 0;
  do {
    PARTICLES[i].variant = -1;
    i++;
  } while (i != TITAN_BLADE_DUST);

  i = 0;
  // The sparks start at canvas y 88 below the target, thrown twice as fast
  // across as Ragnarok's.
  sparkOriginX = canvasCenterX << 16;
  do {
    struct EffectStep *spark = &FALLING_SWORD_SPARKS[i];

    s32 mag;
    s32 ang;

    /* FAKEMATCH: the mask is loaded into the magnitude before the random word
     * is taken in, as the native code does (ldr r6, =0x1ff; ands r6, r0).
     * 2026-10-13: Random16() & 0x1FF, 0x1FF & Random16() and mag = Random16()
     * followed by mag &= 0x1FF all differ 41 of 779, 780 instructions: the and
     * takes the random word as its destination, copied into r6, with the mask
     * in r3, and the pool offsets shift. */
    mag = 0x1FF;
    mag &= Random16();
    ang = Random16() & 0xFFFF;
    spark->x = sparkOriginX;
    spark->y = 176 << 15;
    spark->velocityX = (Trig_Sin(ang) * (mag + 32)) >> 5;
    spark->velocityY = -(Trig_Cos(ang) * (mag + 32)) >> 6;
    spark->variant = (Random16() & 7) + 32;
    i++;
  } while (i != TITAN_BLADE_SPARKS);

  frame = 0;
  do {
    // The palette fades towards the blast colours over frames 25 to 47,
    // then towards the lightning colours from frame 57.
    if (frame >= 25 && frame <= 47) {
      BattleFx_StepPaletteToResource((s32)&ResourceId_BlastSheet);
    }
    if (frame > 56) {
      BattleFx_StepPaletteToResource((s32)&ResourceId_LightningBoltSheet);
    }

    if (frame == 8) {
      work->shakeFrames = frame;
    }
    if (frame == 48) {
      work->shakeFrames = 8;
    }
    if (frame == 60) {
      work->shakeFrames = 16;
    }

    if (frame == 4) {
      AudioCommand_PlayFar(SE_212);
    }
    if (frame == 32) {
      AudioCommand_PlayFar(SE_164);
    }
    if (frame == 60) {
      AudioCommand_PlayFar(SE_145);
      BattleEventRuntime_BeginPhaseFar(SE_HEAVY_IMPACT);
    }

    // From frame 56: a ring of 16 flashes around the target.
    if (frame > 55) {
      i = 0;
      do {
        struct EffectStep *flash = &work->particles[i];

        s32 x;
        s32 life;
        s32 y;

        y = HI(flash->y);
        x = HI(flash->x) + canvasCenterX;
        life = flash->variant;
        if (life >= 0 && life <= 17) {
          blitters[0](canvas,
                      work->sheet + (gTitanBlade_FlashCells[life / 3] << 11) +
                          (221 << 4),
                      x - 16, y + 48, 32, 64);
        }
        life = flash->variant;
        flash->variant = life > 0 ? life - 1 : -1;
        i++;
      } while (i != TITAN_BLADE_RING);
    }

    // Dust at the target's feet: every free one of the first 256 on frame
    // 28.
    if (frame == 28) {
      i = 0;
      do {
        struct EffectStep *dust = &PARTICLES[i];

        if (dust->variant == -1) {
          s32 mag;
          s32 ang;

          mag = Random16() & 63;
          ang = Random16() & 0xFFFF;
          dust->x = ((Trig_Sin(ang) * mag) >> 3) + sparkOriginX;
          dust->y = ((Trig_Cos(ang) * mag) >> 2) + (192 << 15);
          dust->velocityX = ((Random16() & 63) - 32) << 14;
          dust->velocityY = (-(Random16() & 63) - 8) << 13;
          dust->variant = 0;
        }
        i++;
      } while (i != TITAN_BLADE_FIRST_DUST);
    }

    // Frames 32 to 63: up to 16 more dust a frame, and the 34-pixel pillar
    // of fire scrolls up through the target in two pieces that wrap at 104
    // rows.
    step = frame - 32;
    if (step >= 0 && step <= 31) {
      s32 cnt;

      cnt = 0;
      i = 0;
      do {
        struct EffectStep *dust = &PARTICLES[i];

        if (dust->variant == -1) {
          s32 mag;
          s32 ang;

          mag = Random16() & 63;
          ang = Random16() & 0xFFFF;
          dust->x = ((Trig_Sin(ang) * mag) >> 3) + sparkOriginX;
          dust->y = ((Trig_Cos(ang) * mag) >> 2) + (192 << 15);
          dust->velocityX = ((Random16() & 63) - 32) << 14;
          dust->velocityY = (-(Random16() & 63) - 8) << 13;
          dust->variant = 0;
          cnt++;
          if (cnt == 16) {
            break;
          }
        }
        i++;
      } while (i != TITAN_BLADE_DUST);
    }

    if (step >= 0 && step <= 31) {
      s32 phase = frame * 16;
      s32 x = canvasCenterX - 17;
      s32 width = 34;
      DrawRectangle blit = blitters[0];
      s32 high = 104;
      s32 size = (phase - 256) % high;

      blit(canvas, work, x, 4 - size, width, high);
      blit(canvas, work, x, 108 - size, width, size);
    }

    // Until frame 71, move and draw the first 342 dust motes; they swell
    // from frame 69, and one goes out once it falls below y 108.
    if (frame <= 71) {
      i = 0;
      do {
        struct EffectStep *dust = &PARTICLES[i];

        if (dust->variant >= 0) {
          s32 size;

          size = i % 3 + 2;
          if (dust->velocityY > 0) {
            size += 2;
          }
          if (frame > 68 && size <= 5) {
            size = 6;
          }
          if (frame > 70 && size <= 6) {
            size = 7;
          }
          if (frame > 72 && size <= 7) {
            size = 8;
          }
          if (frame > 74 && size <= 8) {
            size = 9;
          }
          if (frame > 76) {
            size = 10;
          }
          EFFECT_DRAW_MOTE(blitters[0], canvas, sheet, HI(dust->x), HI(dust->y),
                           size);

          dust->x += dust->velocityX;
          dust->y += dust->velocityY;
          if (frame > 80) {
            dust->velocityY += -32768;
          } else {
            dust->velocityY += gTitanBlade_DustGravity[i & 3];
          }
          dust->velocityX = dust->velocityX * 62 / 64;
          dust->velocityY = dust->velocityY * 62 / 64;
          dust->variant++;
          if (dust->velocityY > 0 && HI(dust->y) > 108) {
            dust->variant = -1;
          }
        }
        i++;
      } while (i != TITAN_BLADE_DRAWN_DUST);
    }

    // The 36x120 sword, until frame 95: it drops in over 10 frames, holds,
    // sinks slowly from frame 33 and plunges after frame 60. Rows below
    // y 108 are not drawn.
    if (frame <= 95) {
      s32 x;
      s32 high;
      s32 top;

      x = canvasCenterX - 18;
      high = 120;
      if (frame > 60) {
        top = frame * 8 - 450;
      } else if (frame > 32) {
        top = step / 2 + 16;
      } else if (frame <= 9) {
        top = frame * 16 - 128;
      } else {
        top = 16;
      }
      if (top + 120 > 108) {
        high = high - top - 12;
      }
      if (high > 0) {
        blitters[1](canvas, work->sheet + TITAN_BLADE_SWORD_IMAGE, x, top, 36,
                    high);
      }
    }

    // From frame 60 the sparks fly, bouncing at half speed below y 108.
    if (frame > 59) {
      i = 0;
      do {
        struct EffectStep *spark = &FALLING_SWORD_SPARKS[i];

        if (spark->variant > 0) {
          s32 x;
          s32 y;
          s32 size;

          EffectStep_AdvanceWithGravity2D(spark, 64, 128 << 6);
          y = spark->y;
          spark->variant--;
          if (y > (216 << 15)) {
            spark->velocityY = -spark->velocityY / 2;
          } else {
            x = spark->x;
            if ((x >= 0 && x <= 0x7effff) && y >= 0) {
              u8 *cell;

              size = spark->variant / 5 + 1;
              cell = sheet + gBattleFx_MoteCells[size - 1];
              x >>= 16;
              y >>= 16;
              x -= size / 2;
              y -= size;
              blitters[0](canvas, cell, x, y, size, size * 2);
            }
          }
        }
        i++;
      } while (i != TITAN_BLADE_SPARKS);
    }

    // Frame 68: every target flashes and recoils.
    if (frame == 68) {
      EFFECT_FOR_EACH_TARGET(i, work->effect) {
        ObjectGroup_UpdateMembers(work->effect->actors[i], SPRITE_COLOR_WHITE,
                                  POSE_DAMAGE, i, 16);
        BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 7);
      }
    }

    // The canvas is wiped as the sword lands and as it strikes.
    if (frame == 9) {
      Iwram_FillWords(canvas, 128 << 7, 0x3F3F3F3F);
    }
    if (frame == 60) {
      Iwram_FillWords(canvas, 128 << 7, 0x3F3F3F3F);
    }

    Camera_ApplyShake(16, 16);
    EFFECT_END_FRAME(work);
    frame++;
  } while (frame != TITAN_BLADE_FRAMES);

  EFFECT_FINISH_CANVAS();
}
