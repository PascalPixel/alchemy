// Part of effect.c, the battle effect object, compiled only through it.
// The Kirin summon.

// Object formations that sweep, rise or break into flares.
//
// These effects move a formation of the effect's animation objects
// (work->objects, created by BattleFx_SpawnObjects) across the battlefield
// while the canvas layer (BG2) draws particles behind and around them.
//
// The object row runs in two stages. In the sweep stage nine objects ride in
// on a sine wave while the backdrop scrolls, a ring of motes opens around
// them and a trail of fire follows. In the rise stage the row slides across
// the targets; each target the row passes throws a burst of 32 motes and
// plays its hit reaction, and the fire sheet slides off behind it.
//
// The formation sweep also runs in two stages. The first places eight objects
// at one and a half times their size, opens the window with an edge table
// built every frame, sprays particles under gravity, converges a ring of
// flares, then breaks into a burst of light. During it a frame callback and
// an H-blank interrupt change BG2's horizontal scale. The second stage drops
// five blue flame columns onto the targets; each column throws sparks when it
// reaches the ground.

#define KIRIN_OBJECTS 9 // objects in the rows

// Abilities: Kirin (scene kind 3).
//
// Large battle effect 3 (BattleFx_RunSceneEffect): over a scrolling forest a
// row of objects rides in on a wave trailing fire, then sweeps across the
// targets, each throwing a burst of motes as the row passes it.
//
// The scene runs in two stages. In the sweep (120 frames; A or B ends it
// after frame 16), KIRIN_OBJECTS objects ride in on a sine wave while the
// backdrop scrolls, a ring of motes opens around them and a trail of fire
// follows. In the rise (96 frames) the row slides across the targets; each
// target the row passes throws a burst of 32 motes and plays its hit
// reaction, and the fire sheet slides off behind it. The battle shows the
// damage at the end.
void BattleFx_RunKirin(struct BattleEffectArgument *effect) {
  static struct SpriteScale sweepScale = {{{0x10000, 0x10000}}};
  static struct SpriteScale riseScale = {{{0x10000, 0x10000}}};
  void **cursor;
  struct SpritePlacement place;
  struct EffectPosition unit;
  u8 jitterRow[16];
  u8 hitRow[14];
  struct SpritePlacement risePlace;
  void *canvas;
  struct BattleEffectWork *work;
  DrawRectangle blit;
  u8 *sheet;
  s32 baseX;
  s32 grow;
  s32 savedX;
  struct BattleBackgroundView *control;
  s32 shift;
  s32 wave;
  s32 frame;
  s32 i;

  cursor = &gBattleFxWork[1];
  canvas = cursor[0];
  work = cursor[-1];
  sheet = cursor[1];
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  BattleFx_ConfigureEffectDisplay();
  REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);
  PLTT_BACKDROP = 0;
  BG_PLTT_COLOR(0, 1) = 0;
  work->transferMode = CANVAS_COPY;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer,
                                EFFECT_CALLBACK_ORDER);
  BattleFx_WipeCanvas(1, 0);
  BattleFx_SpawnObjects(KIRIN_OBJECTS, 0x175, 1);
  gProjection.centerY = 240;
  BattleFx_SelectLivingTargets(work->effect);
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG1 |
              WININ_WIN1_BG2 | WININ_WIN1_CLR;
  REG_WIN0H = WIN_RANGE(0, 202);
  WaitFrames(1);
  BattleBackground_LoadFar(1, (s32)&ResourceId_ForestBackdrop, 0);
  BattleFx_WipeCanvas(1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_FireBlobSheet, work->sheet, 1, 1);
  EFFECT_SHOW_WIDE_CANVAS(14);

  baseX = 0;
  grow = 0;
  savedX = gBgScroll[1].x;
  control = cursor[4];
  shift = 0;
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, baseX);
  control->busy = 1;

  i = 0;
  do {
    struct EffectStep *fire = &work->particles[i];

    // The 64 fire particles start at random places, speeds and delays.
    fire->x = (Random16() & 31) + 16;
    fire->y = ((Random16() & 31) + 48) << 16;
    fire->velocityY = ((Random16() & 31) - 16) << 16;
    fire->variant = Random16() % 48 + 2;
    i++;
  } while (i != 64);

  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                      COMPOSITE_ADDITIVE);
  blit =
      (DrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER];
  REG_BG2CNT = BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
               BGCNT_SCREEN_BASE(7);

  // The sweep stage: 120 frames, which A or B ends after frame 16.
  for (frame = 0; frame != 120; frame++) {
    wave = 0;
    if (frame == 0) {
      AudioCommand_PlayFar(SE_136);
    }
    if (frame == 26) {
      AudioCommand_PlayFar(SE_141);
    }
    if (frame == 40) {
      AudioCommand_PlayFar(SE_154);
    }
    if (frame == 72) {
      AudioCommand_PlayFar(SE_154);
    }
    if (frame == 104) {
      AudioCommand_PlayFar(SE_154);
    }
    if ((gKeysRepeat & (KEY_A | KEY_B)) != 0 && frame > 16) {
      break;
    }

    // Speed the forest's scroll up over frames 24 to 55, to at most 24
    // pixels a frame.
    if (frame >= 24 && frame <= 55) {
      shift++;
    }
    if (shift > 24) {
      shift = 24;
    }
    if (frame <= 135) {
      gBgScroll[1].x -= shift;
      grow += shift;
    }

    // From frame 8 the row bobs on a sine wave (32 pixels high, 96 after
    // frame 104) and shakes the screen every 32 frames; after frame 103 it
    // lifts away to the left.
    if (frame <= 149) {
      struct SpriteScale pair = sweepScale;
      s32 lift;

      lift = 0;
      if (frame > 103) {
        lift = frame * 16 - 1664;
      }
      if (frame >= 8 && frame <= 31) {
        baseX = baseX + shift - 8;
      }
      if (frame > 7) {
        s32 amp;
        s32 ang;

        amp = 96;
        if (frame <= 104) {
          amp = 32;
        }
        ang = (frame * 1024 - 0x2000) & 0xFFFF;
        if (ang > 0x8000) {
          ang -= 0x8000;
        }
        wave = (Trig_Sin(ang) * amp) >> 16;
        if ((frame & 31) == 8) {
          work->shakeFrames = 4;
        }
      }
      place.groundHeight = 0;
      place.height = 255 << 16;
      i = 0;
      do {
        place.x = ((baseX + gKirin_Columns[i] - lift) << 16) + (224 << 16);
        place.y = ((gKirin_Rows[i] - wave) << 16) + (144 << 15);
        Object_ApplyProjectedPlacementFar(work->objects[i], &place, &pair, 0);
        i++;
      } while (i != KIRIN_OBJECTS);
    }

    // Over the first 27 frames a ring of motes opens around the row, eight
    // more motes each frame.
    if (frame <= 26) {
      s32 size = grow + 4;
      s32 count = frame * 8;

      if (size > 10) {
        size = 10;
      }
      if (count > 64) {
        count = 64;
      }
      for (i = 0; i != count; i++) {
        s32 ang = i << 10;
        s32 x = ((Trig_Sin(ang) * (grow * 2 + 8)) >> 16) + grow + 96;
        s32 y = ((Trig_Cos(ang) * (grow * 12 + 48)) >> 16) + 64;

        EFFECT_DRAW_MOTE(blit, canvas, sheet, x, y, size);
      }
    }

    if (frame == 24) {
      EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 50);
    }
    if (frame == 28) {
      REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
                   BGCNT_SCREEN_BASE(7);
    }

    // From frame 18 the 48 fire particles trail the row; each waits out its
    // delay first, and one that leaves the canvas restarts behind the row.
    if (frame > 17) {
      i = 0;
      do {
        struct EffectStep *fire = &work->particles[i];

        if (fire->variant == 0) {
          s32 size = i % 3 + 1;

          blit(canvas, sheet + gBattleFx_MoteCells[size - 1], fire->x,
               HI(fire->y) - size, size, size * 2);
          fire->x += 2;
          fire->y += fire->velocityY;
          fire->velocityY = fire->velocityY * 48 / 64;
        } else {
          fire->variant--;
        }
        if (fire->x > 128 || fire->variant == 1) {
          fire->x = (Random16() & 31) + baseX + 172;
          fire->y = ((Random16() & 31) - wave + 56) << 16;
          fire->velocityY = ((Random16() & 31) - 16) << 15;
        }
        i++;
      } while (i != 48);
    }

    // From frame 32 a column of six random fire blobs slides up to 40
    // pixels left.
    if (frame > 31) {
      s32 rise = (frame - 32) / 2;
      s32 y;

      if (rise > 40) {
        rise = 40;
      }
      i = 0;
      y = 0;
      do {
        blit(canvas, work->sheet + (Random16() & 3) * 0x600, 120 - rise, y, 48,
             32);
        i++;
        y += 18;
      } while (i != 6);
    }

    // While the shake timer runs, jitter the forest's vertical scroll;
    // otherwise hold it at 32.
    if (work->shakeFrames > 0) {
      work->shakeFrames--;
      gBgScroll[1].y = (Random16() & 7) + 28;
    } else {
      gBgScroll[1].y = 32;
    }
    EFFECT_PRESENT_FRAME(work);
  }

  // The rise stage: restore the backdrop and display, put the objects behind
  // the background, and clear the hit marks and the burst motes.
  gBgScroll[1].x = savedX;
  control->busy = 0;
  BattleFx_SetupBlendedDisplay();
  REG_WIN0H = WIN_RANGE(0, 240);

  i = 0;
  do {
    struct AnimationObject *object = work->objects[i];

    object->image.priority = 3;
    i++;
  } while (i != KIRIN_OBJECTS);

  {
    s32 rowX;
    u8 *hit;
    u8 *jitter;

    rowX = 224;
    hit = hitRow;
    i = 0;
    do {
      hit[i] = 0;
      i++;
    } while (i != 14);
    jitter = jitterRow;
    i = 0;
    do {
      jitter[i] = Random16() & 31;
      i++;
    } while (i != 16);
    i = 0;
    do {
      PARTICLES[i].variant = 0;
      i++;
    } while (i != 160 << 1);

    EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);
    REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
                 BGCNT_SCREEN_BASE(7);
    REG_BLDALPHA = BLEND_ALPHA(16, 16);

    frame = 0;
    do {
      // The row slides 16 pixels left a frame on a sine 64 pixels high for
      // the first nine frames and 32 after.
      if (frame <= 23) {
        struct SpriteScale riseScaleCopy = riseScale;
        s32 ang;
        s32 rise;

        rowX -= 16;
        if (frame <= 8) {
          ang = frame * 2048 + 0x4000;
          if (ang > 0x8000) {
            ang -= 0x8000;
          }
          rise = ((Trig_Sin(ang) * 64)) >> 16;
        } else {
          ang = frame * 2048 + 0x4000;
          if (ang > 0x8000) {
            ang -= 0x8000;
          }
          rise = ((Trig_Sin(ang) * 32)) >> 16;
        }
        risePlace.groundHeight = 0;
        risePlace.height = 255 << 16;
        i = 0;
        do {
          risePlace.x = (rowX + gKirin_Columns[i]) << 16;
          risePlace.y = ((gKirin_Rows[i] - rise) << 16) + (144 << 15);
          Object_ApplyProjectedPlacementFar(work->objects[i], &risePlace,
                                            &riseScaleCopy, 0);
          i++;
        } while (i != KIRIN_OBJECTS);
      }

      if (frame == 8) {
        work->shakeFrames = frame;
        AudioCommand_PlayFar(SE_145);
      }
      if (frame == 11) {
        AudioCommand_PlayFar(SE_145);
      }
      if (frame == 46) {
        AudioCommand_PlayFar(SE_137);
      }

      EFFECT_FOR_EACH_TARGET(i, work->effect) {
        // A target the row passes for the first time throws a burst of 32
        // motes and plays its hit reaction.
        if (hit[i] == 0) {
          EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actors[i],
                                                      &unit);
          if (unit.x > rowX) {
            s32 k;

            hit[i] = 1;
            k = 0;
            do {
              struct EffectStep *burst = &PARTICLES[i * 32 + k];
              s32 up;

              burst->x = unit.x << 15;
              burst->y = (unit.y - 16) << 16;
              burst->velocityX = ((Random16() & 255) - 128) << 10;
              up = (Random16() & 255) - 192;
              burst->velocityY = up << 11;
              burst->x += burst->velocityX * 4;
              burst->y += up << 13;
              burst->variant = (Random16() & 15) + 8;
              k++;
            } while (k != 32);
            BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 1);
            AudioCommand_PlayFar(SE_HEAVY_IMPACT);
          }
        }
      }

      i = 0;
      do {
        // Each burst mote is drawn and moved until its life runs out.
        struct EffectStep *burst = &PARTICLES[i];

        if (burst->variant > 0) {
          s32 size = 3;

          EFFECT_DRAW_MOTE(blit, canvas, sheet, HI(burst->x), HI(burst->y),
                           size);
          burst->x += burst->velocityX;
          burst->y += burst->velocityY;
          burst->variant--;
        }
        i++;
      } while (i != 192);

      if (frame == 48) {
        AudioCommand_PlayFar(SE_136);
      }
      // From frame 41 the fire sheet, 16 jittered rows, slides off to the
      // left 12 pixels a frame.
      if (frame > 40) {
        s32 slide = (frame - 40) * 12;
        s32 y;

        EFFECT_SET_TRANSFER(work, CANVAS_COPY, 75);
        i = 0;
        y = -8;
        do {
          blit(canvas, work->sheet + (Random16() & 3) * 0x600,
               jitter[i] - slide + 120, y, 48, 32);
          i++;
          y += 8;
        } while (i != 16);
      }
      if (frame > 64) {
        work->transferMode = CANVAS_COPY_AND_FADE;
      }
      // Frame 58: every target turns black in its damage pose.
      if (frame == 58) {
        EFFECT_FOR_EACH_TARGET(i, work->effect) {
          ObjectGroup_UpdateMembers(work->effect->actors[i], SPRITE_COLOR_BLACK,
                                    POSE_DAMAGE, -1, 0);
        }
      }

      Camera_ApplyShake(8, 8);
      EFFECT_END_FRAME(work);
      frame++;
    } while (frame != 96);
  }

  BattleEventRuntime_BeginPhaseFar(SE_HEAVY_IMPACT);
  i = 0;
  do {
    ResourceObject_ReleaseFar(work->objects[i]);
    i++;
  } while (i != KIRIN_OBJECTS);
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}
