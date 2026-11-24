/// @file
/// The body of Thunder Mine and Earth Force, then Megaera and Void Beam.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

#include "sprite_ids.h"

/// Thunder Mine's sparks, in EXTRA_PARTICLES: those cleared, and of them those
/// that burst out as the ball arrives.
#define BALL_LIGHTNING_SPARKS_CLEARED 512
#define BALL_LIGHTNING_SPARK_COUNT 128
/// The canvas row Thunder Mine's sparks bounce at.
#define BALL_LIGHTNING_SPARK_FLOOR 111

/// Where TornadoSheet is decoded in the effect's pictures, for its glints.
#define BALL_LIGHTNING_TORNADO_PICTURES 2048
/// The bolts' four pictures (LightningBoltSheet), each copied into the middle
/// of a 32x128 cel, after the glints: a bolt is drawn as a column textured
/// with a cel.
#define BALL_LIGHTNING_BOLT_CELS                                               \
  (BALL_LIGHTNING_TORNADO_PICTURES + GLINT_SET_BYTES)
#define BALL_LIGHTNING_BOLT_PICTURE_COUNT 4
#define BALL_LIGHTNING_BOLT_CEL_WIDTH 32
#define BALL_LIGHTNING_BOLT_CEL_HEIGHT 128
#define BALL_LIGHTNING_BOLT_CEL_INSET                                          \
  ((BALL_LIGHTNING_BOLT_CEL_WIDTH - LIGHTNING_BOLT_WIDTH) / 2)
/// The bolts of lightning set up, and of them those drawn, each for eight
/// frames from the frame of its number.
#define BALL_LIGHTNING_BOLTS_SET_UP 63
#define BALL_LIGHTNING_BOLTS 40

#define BALL_LIGHTNING_FRAMES 50
/// The ball's size, which grows two pixels a frame from 16 to this.
#define BALL_LIGHTNING_SIZE 48
#define EARTH_FORCE_BALL_SIZE 64

/// The body of Thunder Mine and Earth Force: ball lightning. Thunder Mine's
/// ball of light grows at the caster as bolts of lightning crackle out of it
/// all round, and crosses to the target; Earth Force's dark ball, rimmed in
/// magenta, with bolts of lightning inside it, grows as it falls from above
/// the caster onto the target. As it arrives the screen flashes white and the
/// target is hurled away, and Thunder Mine's ball bursts into sparks that
/// bounce.
void BattleEffect_RunBallLightning(struct BattleEffectParams *args, s32 mode) {
  struct BattleEffectState *state;
  void *canvas;
  s32 frame;
  /* LEFTOVER(frame): draw[1], the pair's idle second blitter, which the ROM's
   * frame reserves and never touches; as one DrawRectangle. */
  DrawRectangle draw[2];
  u8 *sprites;
  s32 ballSize;
  s32 frames;
  s32 sparkFloor = BALL_LIGHTNING_SPARK_FLOOR;
  s32 x;
  s32 y;
  struct ScreenPoint target;
  struct ScreenPoint source;
  struct ModelTexture texture;
  struct RenderModel *model;
  struct Particle *ball;
  struct Particle *bolt;
  struct Particle *spark;
  struct Particle *dot;
  void *vertices;
  s32 i;
  s32 k;
  u8 *buffer;

  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sprites = Ram_HeapSlots->battleSheet;
  buffer = Ram_MapCellBuffer;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  if (mode == BALL_LIGHTNING_THUNDER_MINE) {
    REG_BLDALPHA = EFFECT_BLEND_ADD;
  } else {
    REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, BLEND_WEIGHT_MAX / 2);
  }
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  ballSize = 0;
  Resource_LoadAndDecompress(RESOURCE_TORNADO_SHEET,
                             state->sheet + BALL_LIGHTNING_TORNADO_PICTURES, 1,
                             0);
  Resource_LoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprites, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_LIGHTNING_BOLT_SHEET, buffer, 1, 1);
  if (mode == BALL_LIGHTNING_EARTH_FORCE) {
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry(RESOURCE_VIOLET_PALETTE_C),
                    SHEET_PALETTE_BYTES);
  }

  Iwram_ClearWordsEntry(state->sheet + BALL_LIGHTNING_BOLT_CELS,
                        BALL_LIGHTNING_BOLT_PICTURE_COUNT *
                            PICTURE_BYTES(BALL_LIGHTNING_BOLT_CEL));
  for (i = 0; i != BALL_LIGHTNING_BOLT_PICTURE_COUNT; i++) {
    for (k = 0; k != LIGHTNING_BOLT_HEIGHT; k++) {
      s32 j;

      for (j = 0; j != LIGHTNING_BOLT_WIDTH; j++) {
        state->sheet[BALL_LIGHTNING_BOLT_CELS + BALL_LIGHTNING_BOLT_CEL_INSET +
                     i * PICTURE_BYTES(BALL_LIGHTNING_BOLT_CEL) +
                     k * BALL_LIGHTNING_BOLT_CEL_WIDTH + j] =
            buffer[i * LIGHTNING_BOLT_PICTURE_BYTES + k * LIGHTNING_BOLT_WIDTH +
                   j];
      }
    }
  }
  if (mode == BALL_LIGHTNING_THUNDER_MINE) {
    EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  } else {
    EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
  }
  Task_Add(BattleEffect_TransferCanvas, TASK_PASS_VBLANK);
  if (mode == BALL_LIGHTNING_THUNDER_MINE) {
    frames = 24;
    /* FAKEMATCH(dead): the row Thunder Mine's sparks bounce at, stored again in
     * its arm, the value it holds; as the arm without it. */
    sparkFloor = BALL_LIGHTNING_SPARK_FLOOR;
  } else {
    frames = 40;
  }

  for (frame = 0; frame != BALL_LIGHTNING_FRAMES; frame++) {
    ball = state->particles;
    if (frame == 0) {
      EffectPosition_ProjectBattler(args->user, &source);
      EffectPosition_ProjectBattler(args->targets[0], &target);
      if (mode == BALL_LIGHTNING_THUNDER_MINE) {
        ball->x = INTtoFIXED(source.x);
        ball->y = INTtoFIXED(source.y - 32);
        ball->velocityX = (INTtoFIXED(target.x) - ball->x) / frames;
      } else {
        if (args->side == EFFECT_SIDE_ENEMIES) {
          ball->x = INTtoFIXED(source.x - 32);
        } else {
          ball->x = INTtoFIXED(source.x + 32);
        }
        ball->y = INTtoFIXED(source.y - 80);
        ball->velocityX = (INTtoFIXED(target.x - 32) - ball->x) / frames;
      }
      ball->velocityY = (INTtoFIXED(target.y - 32) - ball->y) / frames;
      Sound_PlayFar(SE_RISE_WITH_SPARKS);
      // Each bolt turns its own way at its own speed, on a picture of its own.
      for (i = 0, bolt = &state->particles[1]; i != BALL_LIGHTNING_BOLTS_SET_UP;
           i++, bolt++) {
        bolt->z = Random16();
        bolt->velocityZ = (Random16() & 255) + 128;
        if (Random16() & 1) {
          bolt->velocityZ = -bolt->velocityZ;
        }
        bolt->variant = Random16() & 3;
      }
      // UB: the variants are cleared through one s32 pointer stepped a whole
      // step at a time, past the step's scalar members.
#ifdef UBFIX
      for (i = 0; i != BALL_LIGHTNING_SPARKS_CLEARED; i++)
        EXTRA_PARTICLES[i].variant = -1;
#else
      {
        s32 *variant = &EXTRA_PARTICLES[0].variant;

        for (i = 0; i != BALL_LIGHTNING_SPARKS_CLEARED; i++) {
          *variant = -1;
          variant += sizeof(struct Particle) / sizeof(s32);
        }
      }
#endif
    }

    if (frame == frames) {
      s32 n;

      for (n = 0, spark = EXTRA_PARTICLES; n != BALL_LIGHTNING_SPARK_COUNT;
           n++, spark++) {
        s32 angle;
        s32 speed;

        spark->x = ball->x / 2;
        spark->y = ball->y;
        angle = Random16();
        speed = Random16() % 400 + 32;
        spark->velocityX = (Trig_Sin(angle) * speed) >> 7;
        spark->velocityY = (Trig_Cos(angle) * speed) >> 6;
        spark->variant = (Random16() & 7) + 16;
      }
      Iwram_FillWordsEntry(canvas, CANVAS_VIEW_BYTES,
                           CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
      state->shakeFrames = 16;
      BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_HURL);
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 8);
      if (mode == BALL_LIGHTNING_THUNDER_MINE) {
        EFFECT_SHOW_DAMAGE(SE_CRASH);
      } else {
        EFFECT_SHOW_DAMAGE(HIT_SOUND_NONE);
        Sound_PlayFar(SE_CRASH);
      }
    }

    // The ball, growing, with four glints about it; Earth Force's ball is a
    // shaded disc turned over, dark inside a bright rim.
    if (frame <= frames) {
      s32 largest;

      ball->variant++;
      x = FIXEDtoINT(ball->x) >> 1;
      y = FIXEDtoINT(ball->y);
      if (mode == BALL_LIGHTNING_THUNDER_MINE) {
        ballSize = frame * 2 + 16;
        largest = BALL_LIGHTNING_SIZE;
      } else {
        ballSize = frame * 2 + 16;
        largest = EARTH_FORCE_BALL_SIZE;
      }
      if (ballSize > largest) {
        ballSize = largest;
      } else {
        BattleEffect_DrawShadedCircles(buffer, ballSize,
                                       FIXED_ONE * 2673 / 2048);
        if (mode == BALL_LIGHTNING_EARTH_FORCE) {
          for (i = 0; i != ballSize * ballSize / 2; i++) {
            if (buffer[i] != 0) {
              buffer[i] = CANVAS_LEVEL_BRIGHTEST + 1 - buffer[i];
            }
          }
        }
      }
      Canvas_BlitMirroredQuarters(buffer, x, y, ballSize);
      for (i = 0; i != 4; i++) {
        // DEAD: i runs 0 to 3, so the mask changes nothing; the ROM keeps the
        // and.
        s32 picture = i & 3;
        s32 half = ballSize / 2;
        s32 angle = Random16();
        s32 glintX = x + (FIXEDtoINT(Trig_Sin(angle) * half)) -
                     (gBattleEffect_GlintPictureWidths[picture] >> 1);
        s32 glintY = y + (FIXEDtoINT(Trig_Cos(angle) * half)) -
                     (gBattleEffect_GlintPictureHeights[picture] >> 1);
        s32 flags = EFFECT_CLIP_BOUNDS | gBoltTrail_GlintFlips[Random16() & 3];
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, flags, BLITTER_MAX);
        ((DrawRectangle)Ram_HeapSlots->ramCodePaired)(
            canvas,
            state->sheet + BALL_LIGHTNING_TORNADO_PICTURES +
                gBattleEffect_GlintPictures[picture],
            glintX, glintY, gBattleEffect_GlintPictureWidths[picture],
            gBattleEffect_GlintPictureHeights[picture]);
        HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
      }
      Particle_AdvanceWithGravity2D(ball, PARTICLE_DAMPING_NONE, 0);
    }

    // Thunder Mine's sparks bounce, each drawn at eight steps of its flight a
    // frame, so that it streaks.
    if (mode == BALL_LIGHTNING_THUNDER_MINE && frame >= frames) {
      for (i = 0, dot = EXTRA_PARTICLES; i != BALL_LIGHTNING_SPARKS_CLEARED;
           i++, dot++) {
        if (dot->variant >= 0) {
          s32 size = (dot->variant >> 3) + 2;
          s32 step;

          for (step = 0; step != 8; step++) {
            draw[0](canvas, sprites + gBattleEffect_MotePictures[size - 1],
                    (FIXEDtoINT(dot->x)) - size / 2,
                    (FIXEDtoINT(dot->y)) - size, size, size * 2);
            Particle_AdvanceWithGravity2D(dot, 62, toFIXED(1) / 128);
          }
          if ((FIXEDtoINT(dot->y)) > sparkFloor) {
            dot->velocityY = -dot->velocityY * 48 / DAMPING_SCALE;
          }
          dot->variant--;
        }
      }
    }

    // The bolts of lightning about the ball, each shown for eight frames:
    // dimmed a quarter on its first, whole for three, then fading.
    if (frame <= frames) {
      vertices = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      texture.logWidth = 5;
      texture.logHeight = 7;
      // DEAD: both kinds are overwritten by the MODEL_SHADED_MAX below; the ROM
      // keeps the test and both stores.
      if (mode == BALL_LIGHTNING_THUNDER_MINE) {
        model->kind = MODEL_SHADED_MAX;
      } else {
        model->kind = MODEL_SHADED;
      }
      model->kind = MODEL_SHADED_MAX;
      model->texture = &texture;
      model->mesh = gBattleEffect_ColumnMesh;
      model->vertices = vertices;
      for (i = 0, bolt = &state->particles[1]; i != BALL_LIGHTNING_BOLTS;
           i++, bolt++) {
        s32 shade;

        if (frame < i || frame >= i + 8) {
          continue;
        }
        shade = 0;
        if (frame == i) {
          shade = MODEL_SHADE_INVISIBLE / 4;
        } else if (frame >= i + 4) {
          s32 fade = (frame - i) * 16 - 64;

          shade = -fade;
        }
        texture.pixels = state->sheet + BALL_LIGHTNING_BOLT_CELS +
                         bolt->variant * PICTURE_BYTES(BALL_LIGHTNING_BOLT_CEL);
        model->shade = shade;
        Matrix_Initialize();
        Matrix_Translate(ball->x / 2 - toFIXED(64), ball->y - toFIXED(64), 0);
        Matrix_RotateZ(bolt->z);
        bolt->z += bolt->velocityZ;
        if (mode == BALL_LIGHTNING_THUNDER_MINE) {
          Matrix_Scale(FIXED_ONE * 4, FIXED_ONE, FIXED_ONE);
          Render_TransformVertices(gBattleEffect_TaperedBeamCorners, vertices,
                                   4);
        } else {
          Matrix_Scale(ballSize * (FIXED_ONE / 32), ballSize * 312, FIXED_ONE);
          Render_TransformVertices(gBattleEffect_BeamCorners, vertices, 4);
        }
        Render_DrawModelList(model);
      }
      Heap_Release(model);
      Heap_Release(vertices);
    }
    BattleCamera_Shake(4, 4);
    EFFECT_END_FRAME(state);
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

/// Megaera's sparks and sparkles, in PARTICLES.
#define MEGAERA_SPARK_COUNT 512
/// Megaera's glints, bolts of lightning and sparkles, in the map cell buffer
/// from its second 16 KB on.
#define MEGAERA_PICTURES (Ram_MapCellBuffer + 16384)
extern const struct Scale gMegaera_UnitScale;

/// Where FlashBurstSheet, LightningBoltSheet and SparkleDots are decoded in the
/// effect's pictures.
#define MEGAERA_FLASH_BURST_PICTURES 14336
#define MEGAERA_LIGHTNING_BOLT_PICTURES GLINT_SET_BYTES
#define MEGAERA_SPARKLE_DOTS_PICTURES 11798

#define MEGAERA_FRAMES 170
/// Two swords fall point down among the foes, the second 16 frames after the
/// first, each landing at MEGAERA_SWORD_GROUND_Y with a flash, a bolt of
/// lightning and a burst of sparks that fall back and go out below
/// MEGAERA_SPARK_FLOOR. A sword's picture is 64x64 in 256 colours: as it
/// lands, its bottom row of tiles, the point, is cleared, so it stands buried.
#define MEGAERA_SWORDS 2
#define MEGAERA_SWORD_GROUND_Y 132
#define MEGAERA_SWORD_POINT_BYTES (64 * 8)
#define MEGAERA_SPARK_FLOOR 104
/// Each sword's bolt of lightning reaches down the canvas to this row, where
/// its sparks burst.
#define MEGAERA_BOLT_HEIGHT 112
/// From MEGAERA_FIRE_FRAME the field bursts into flame: two columns of fiery
/// streaks (WhirlBeamSheet's three 96x64 pictures) sway over it while billows
/// of fire rise, the burst's pictures from its third, four frames each.
#define MEGAERA_FIRE_FRAME 64
#define WHIRL_BEAM_WIDTH 96
#define WHIRL_BEAM_HEIGHT 64
#define MEGAERA_BILLOWS_SET_UP 32
#define MEGAERA_BILLOWS 24
#define MEGAERA_BILLOW_FRAMES 16
#define MEGAERA_BILLOW_PICTURES                                                \
  (MEGAERA_FLASH_BURST_PICTURES + 2 * PICTURE_BYTES(FLASH_BURST))
/// Particles given a place and a delay that nothing reads, as Nereid's are.
#define MEGAERA_UNREAD_FIRST 24
#define MEGAERA_UNREAD_COUNT 16
/// Megaera swoops in across the foes at MEGAERA_SWOOP_FRAME, trailing sparkles
/// for MEGAERA_TRAIL_FRAMES; each sparkle twinkles for MEGAERA_SPARKLE_FRAMES.
#define MEGAERA_SWOOP_FRAME 108
#define MEGAERA_TRAIL_FRAMES 24
#define MEGAERA_SPARKLE_FRAMES 36

/// Megaera: two swords fall point down among the foes, each with a flash, a
/// bolt of lightning and a burst of sparks; then the field bursts into flame,
/// and Megaera swoops in across the foes in a trail of sparkles, and bursts on
/// them. Holding A or B after frame 10 skips to the swoop.
void BattleEffect_RunMegaera(struct BattleEffectParams *args) {
  void *canvas;
  struct BattleEffectState *state;
  s32 frame;
  u8 *sprites;
  DrawRectangle *rects;
  struct Point2D position;
  struct Point2D velocity;
  DrawRectangle blitters[2];
  struct Scale scale;
  s32 placement[4];
  struct Particle *p;
  u8 *pictures;
  s32 i;
  s32 j;

  canvas = Ram_HeapSlots->battleCanvas;
  state = Ram_HeapSlots->battleEffect;
  sprites = Ram_HeapSlots->battleSheet;
  pictures = MEGAERA_PICTURES;
  BattleEffect_ConfigureEffectDisplay();
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  rects = blitters;
  BattleEffect_BuildCanvasBlitters(0, (void **)rects);
  Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET,
                             state->sheet + MEGAERA_FLASH_BURST_PICTURES, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_WHIRL_BEAM_SHEET, state->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprites, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_TORNADO_SHEET, pictures, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_LIGHTNING_BOLT_SHEET,
                             pictures + MEGAERA_LIGHTNING_BOLT_PICTURES, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_SPARKLE_DOTS,
                             pictures + MEGAERA_SPARKLE_DOTS_PICTURES, 0, 0);
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE,
                        CANVAS_FADE_THREE_QUARTERS);
  for (i = 0; i != ARRAY_COUNT(state->particles); i++) {
    state->particles[i].variant = -1;
  }
  // DEAD: particles laid along canvas rows 56 to 63, which nothing in Megaera
  // reads; the ROM keeps the loop.
  for (i = 0, p = &state->particles[MEGAERA_UNREAD_FIRST];
       i != MEGAERA_UNREAD_COUNT; i++, p++) {
    p->x = Random16() & 127;
    p->y = (Random16() & 7) + 56;
    p->variant = -(Random16() & 15);
  }
  for (i = 0, p = state->particles; i != MEGAERA_BILLOWS_SET_UP; i++, p++) {
    p->x = INTtoFIXED((Random16() & 31) + 28);
    p->y = INTtoFIXED((Random16() % 48) + 56);
    p->velocityX = ((Random16() & 255) - 128) << 12;
    p->velocityY = (16 - (Random16() & 127)) << 10;
    p->variant = ~i;
  }
  for (i = 0; i != MEGAERA_SPARK_COUNT; i++) {
    PARTICLES[i].variant = -1;
  }
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry(RESOURCE_MARS_DJINN_SHEET),
                       SHEET_PALETTE_BYTES);
  BattleEffect_ShowLivingTargets(state->args);
  wait(1);

  for (frame = 0; frame != MEGAERA_FRAMES; frame++) {
    if ((gInput.repeat & (KEY_A | KEY_B)) && frame > 10) {
      if (frame < MEGAERA_SWOOP_FRAME) {
        frame = MEGAERA_SWOOP_FRAME - 1;
      }
    }
    if (frame == MEGAERA_SWOOP_FRAME - 1) {
      Sound_PlayFar(SE_POWER_UP);
    }
    if (frame == 148) {
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    }
    if (frame == 0) {
      position.x = toFIXED(32);
      position.y = toFIXED(72);
      velocity.x = toFIXED(9);
      velocity.y = -toFIXED(2);
      BattleEffect_CreateSprites(MEGAERA_SWORDS, SPRITE_MEGAERA_SWORD, 2);
    }
    if (frame == MEGAERA_SWOOP_FRAME) {
      Sprite_DestroyFar(state->sprites[0]);
      Sprite_DestroyFar(state->sprites[1]);
      BattleEffect_CreateSprites(MEGAERA_OBJECTS, SPRITE_MEGAERA, 2);
    }

    // Megaera swoops, trailing sparkles.
    if (frame >= MEGAERA_SWOOP_FRAME) {
      position.x += velocity.x;
      position.y += velocity.y;
      velocity.x = velocity.x * 59 / DAMPING_SCALE;
      velocity.y = velocity.y * 59 / DAMPING_SCALE;
      BattleEffect_PlaceFormationSprites(SUMMON_FORMATION_MEGAERA, position.x,
                                         position.y);
      if (frame == MEGAERA_SWOOP_FRAME) {
        for (i = 0; i != MEGAERA_SPARK_COUNT; i++) {
          PARTICLES[i].variant = -1;
        }
      }
      {
        s32 trailFrame = frame - MEGAERA_SWOOP_FRAME;
        if ((u32)trailFrame < MEGAERA_TRAIL_FRAMES) {
          s32 count = 10 - trailFrame / 2;

          if (count > 0) {
            s32 half;

            for (i = 0, half = position.x / 2; i != count / 5; i++) {
              struct Particle *sparkle;
              s32 angle;
              s32 radius;

              sparkle = &PARTICLES[((trailFrame << 5) + i) &
                                   (MEGAERA_SPARK_COUNT - 1)];
              angle = Random16();
              radius = (Random16() & 31) + 16;
              sparkle->x = Trig_Sin(angle) * radius + half + toFIXED(16);
              sparkle->y = Trig_Cos(angle) * radius + position.y - toFIXED(8);
              sparkle->velocityX = 0;
              sparkle->velocityY = 0;
              sparkle->variant = Random16() & 15;
            }
          }
        }
      }
    }
    if ((u32)(frame - MEGAERA_FIRE_FRAME) <= 3) {
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                           CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
    }
    if (frame > 103) {
      BattleEffect_StepPaletteToResource(RESOURCE_TAN_PALETTE);
    }
    if (frame == MEGAERA_FIRE_FRAME) {
      Iwram_CopyWordsEntry((void *)BG_PLTT,
                           Resource_GetTableEntry(RESOURCE_ORANGE_PALETTE_B),
                           SHEET_PALETTE_BYTES);
    }

    // The field in flames: two columns of fiery streaks, each drawn again
    // below itself, sway until Megaera comes, and billows of fire rise.
    if (frame >= MEGAERA_FIRE_FRAME) {
      if (frame <= 111) {
        s32 picture = (frame / 4) % 3;
        s32 swayX = (FIXEDtoINT(Trig_Sin(frame * (TRIG_TURN / 32)) << 3)) - 16;
        s32 which = frame == MEGAERA_SWOOP_FRAME;
        DrawRectangle *draw = &blitters[which];
        u8 *cells = state->sheet + picture * PICTURE_BYTES(WHIRL_BEAM);

        (*draw)(canvas, cells, swayX, 0, WHIRL_BEAM_WIDTH, WHIRL_BEAM_HEIGHT);
        (*draw)(canvas, cells, swayX, WHIRL_BEAM_HEIGHT, WHIRL_BEAM_WIDTH, 52);
        {
          s32 flickerFrame = (frame / 2) % 3;
          u8 *flickerPixels;

          swayX = FIXEDtoINT(Trig_Sin(frame * (TRIG_TURN / 16) + DEGtoANG(180))
                             << 3);
          flickerPixels =
              state->sheet + flickerFrame * PICTURE_BYTES(WHIRL_BEAM);
          (*draw)(canvas, flickerPixels, swayX, 0, WHIRL_BEAM_WIDTH,
                  WHIRL_BEAM_HEIGHT);
          (*draw)(canvas, flickerPixels, swayX, WHIRL_BEAM_HEIGHT,
                  WHIRL_BEAM_WIDTH, 52);
        }
      }
      for (i = 0, p = state->particles; i != MEGAERA_BILLOWS; i++, p++) {
        if ((u32)p->variant < MEGAERA_BILLOW_FRAMES) {
          s32 row = p->variant / 4;
          s32 which = i & 1;
          u8 *smoke = state->sheet + MEGAERA_BILLOW_PICTURES +
                      row * PICTURE_BYTES(FLASH_BURST);

          rects[which](canvas, smoke,
                       FIXED_INTEGER_PART(p->x) - FLASH_BURST_WIDTH / 2,
                       FIXED_INTEGER_PART(p->y) - FLASH_BURST_HEIGHT / 2,
                       FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
          Particle_AdvanceWithGravity2D(p, 62, -toFIXED(1) / 4);
        }
        p->variant++;
        // Megaera's coming blows the fire aside; until then each billow rises
        // again from the field when it has burnt out.
        if (frame == MEGAERA_SWOOP_FRAME) {
          p->velocityX += toFIXED(2);
          p->velocityY /= 4;
        }
        if (frame < MEGAERA_SWOOP_FRAME) {
          s32 v = p->variant;

          if (v == MEGAERA_BILLOW_FRAMES) {
            p->variant = v = 0;
          }
          if (v == 0) {
            p->x = INTtoFIXED((Random16() & 31) + 28);
            p->y = INTtoFIXED((Random16() % 48) + 56);
            p->velocityX = ((Random16() & 255) - 128) << 12;
            p->velocityY = (16 - (Random16() & 127)) << 10;
          }
        }
      }
    }
    if (frame == MEGAERA_FIRE_FRAME) {
      Sound_PlayFar(SE_WAVE_HIGH);
    }
    // The fire crackles eight times, four frames apart.
    {
      s32 crackleFrame;

      for (i = 0, crackleFrame = 75; i != 8; i++, crackleFrame += 4) {
        if (frame == crackleFrame) {
          Sound_PlayFar(SE_HEAVY_IMPACT);
        }
      }
    }
    // As the first sword lands the foes flash white.
    if (frame == 10) {
      state->shakeFrames = 8;
      for (i = 0; i != args->targetCount; i++) {
        BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, i, 16);
      }
    }
    // The sparkles twinkle through SparkleDots' nine pictures, drifting down
    // and to the left.
    if (frame >= MEGAERA_SWOOP_FRAME) {
      for (i = 0, p = PARTICLES; i != MEGAERA_SPARK_COUNT; i++, p++) {
        s32 k;

        if ((u32)p->variant >= MEGAERA_SPARKLE_FRAMES) {
          continue;
        }
        k = (p->variant / 2) % 9;
        blitters[0](
            canvas,
            (MEGAERA_PICTURES + MEGAERA_SPARKLE_DOTS_PICTURES) +
                gBattleEffect_SparklePictures[k],
            FIXED_INTEGER_PART(p->x) - gBattleEffect_SparkleSizes[k] / 2,
            FIXED_INTEGER_PART(p->y) - gBattleEffect_SparkleSizes[k] / 2,
            gBattleEffect_SparkleSizes[k], gBattleEffect_SparkleSizes[k]);
        Particle_AdvanceWithGravity2D(p, 58, 0);
        if (i & 1) {
          p->velocityX += -toFIXED(1) / 32;
          p->velocityY += toFIXED(1) / 16;
        } else {
          p->velocityX += -toFIXED(1) / 16;
          p->velocityY += toFIXED(1) / 32;
        }
        p->variant++;
      }
    }

    // The two swords: each falls 16 pixels a frame, and lands with a flash, a
    // bolt of lightning, a burst of sparks and glints.
    if (frame < MEGAERA_FIRE_FRAME) {
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
      for (i = 0; i != MEGAERA_SWORDS; i++) {
        s32 fallFrame = i * 16;
        s32 column = i * 32 + 24;

        if (frame == fallFrame + 5) {
          Sound_PlayFar(SE_POWER_UP);
        }
        if (frame == fallFrame + 9) {
          state->shakeFrames = 8;
          Iwram_FillWordsEntry(canvas, CANVAS_VIEW_BYTES,
                               CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
          Sound_PlayFar(SE_CLATTER);
        }
        if (frame >= fallFrame) {
          s32 t = frame - fallFrame;
          s32 fallY = t * 16 - 16;

          scale = gMegaera_UnitScale;
          if (fallY > MEGAERA_SWORD_GROUND_Y - 1) {
            fallY = MEGAERA_SWORD_GROUND_Y;
          }
          placement[3] = 0;
          placement[1] = 0;
          placement[0] = INTtoFIXED(column * 2);
          placement[2] = INTtoFIXED(fallY);
          Render_ApplyProjectedPlacementFar(state->sprites[i], placement,
                                            &scale.x, 0);
          // The sword's bottom row of tiles; a sprite's tile number counts
          // tiles of 16 colours, even in 256.
          if (t == 9) {
            Iwram_ClearWordsEntry(
                (u8 *)OBJ_VRAM0 +
                    (OBJ_64X64_8BPP_BYTES - MEGAERA_SWORD_POINT_BYTES) +
                    ((struct Sprite *)state->sprites[i])->image.tile *
                        TILE_SIZE_4BPP,
                MEGAERA_SWORD_POINT_BYTES);
          }
          if (frame == fallFrame + 10) {
            for (j = 0; j != MEGAERA_SPARK_COUNT / MEGAERA_SWORDS; j++) {
              struct Particle *spark =
                  &PARTICLES[i * (MEGAERA_SPARK_COUNT / MEGAERA_SWORDS) + j];
              s32 angle = Random16() & 0x7fff;
              s32 speed = Random16() & 255;

              spark->x = INTtoFIXED(column);
              spark->y = toFIXED(MEGAERA_BOLT_HEIGHT);
              spark->velocityY = (-Trig_Sin(angle) * speed) >> 5;
              spark->velocityX = (Trig_Cos(angle) * speed) >> 5;
              spark->variant = 0;
            }
          }
          if (frame >= fallFrame + 10 && frame < fallFrame + 18) {
            blitters[0](canvas,
                        (MEGAERA_PICTURES + MEGAERA_LIGHTNING_BOLT_PICTURES) +
                            (((frame + i) / 4) & 3) *
                                LIGHTNING_BOLT_PICTURE_BYTES,
                        column - LIGHTNING_BOLT_WIDTH / 2, 0,
                        LIGHTNING_BOLT_WIDTH, MEGAERA_BOLT_HEIGHT);
          }
          if (frame >= fallFrame + 10) {
            for (j = 0; j != 2; j++) {
              s32 k = j & 3;
              s32 glintX = column + (Random16() & 7) -
                           (gBattleEffect_GlintPictureWidths[k] >> 1);
              s32 glintY = (Random16() & 63) -
                           (gBattleEffect_GlintPictureHeights[k] >> 1);
              u32 flip = Random16();
              EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                                  EFFECT_CLIP_BOUNDS |
                                      gMegaera_GlintFlips[flip & 3],
                                  gMegaera_GlintModes[args->variant]);
              ((DrawRectangle)Ram_HeapSlots->ramCodePaired)(
                  canvas, MEGAERA_PICTURES + gBattleEffect_GlintPictures[k],
                  glintX - 4, glintY + 40, gBattleEffect_GlintPictureWidths[k],
                  gBattleEffect_GlintPictureHeights[k]);
              HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
            }
          }
        }
      }
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                          BLITTER_ADDITIVE);
      rects[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;
      if (frame <= 69) {
        for (i = 0, p = PARTICLES; i != MEGAERA_SPARK_COUNT; i++, p++) {
          s32 size;

          if (p->variant < 0) {
            continue;
          }
          size = i % 3 + 1;
          blitters[p->velocityY > 0](
              canvas, sprites + gBattleEffect_MotePictures[size - 1],
              FIXED_INTEGER_PART(p->x) - size / 2,
              FIXED_INTEGER_PART(p->y) - size, size, size * 2);
          Particle_AdvanceWithGravity2D(p, 62, toFIXED(1) / 4);
          p->variant++;
          if (p->velocityY > 0 &&
              FIXED_INTEGER_PART(p->y) > MEGAERA_SPARK_FLOOR) {
            p->variant = -1;
          }
        }
      }
    }
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(state);
  }
  Task_Remove(BattleEffect_TransferCanvas);
  EFFECT_RELEASE_BLITTERS();
  BattleEffect_RunImpactBurst(SUMMON_FORMATION_MEGAERA, position.x, position.y);
  for (i = 0; i != MEGAERA_OBJECTS; i++) {
    Sprite_DestroyFar(state->sprites[i]);
  }
  BattleEffect_EndCanvasLayer();
}

#define VOID_BEAM_FRAMES 88
#define VOID_BEAM_SEGMENTS 10
/// The hexagon of motes is traced eight motes a frame from frame 42,
/// VOID_BEAM_SIDE_MOTES to a side; the ring behind the caster gains three
/// motes every two frames up to VOID_BEAM_RING_MOTES, 40 pixels out.
#define VOID_BEAM_SIDE_MOTES 36
#define VOID_BEAM_RING_MOTES 64
#define VOID_BEAM_RING_RADIUS 40
/// The beam fires at VOID_BEAM_FIRE_FRAME, and fades from VOID_BEAM_FADE_FRAME.
#define VOID_BEAM_FIRE_FRAME 72
#define VOID_BEAM_FADE_FRAME 80

/// Where CometSheetB and RuneSheet are decoded in the effect's pictures.
#define VOID_BEAM_COMET_B_PICTURES 4096
#define VOID_BEAM_RUNE_PICTURES 4096
/// The beam's body, a 32x32 gradient built after EmberStreakSheet, and
/// CometSheetB's comet, its head.
#define VOID_BEAM_BODY 6688
#define VOID_BEAM_BODY_SIZE 32
#define VOID_BEAM_COMET_WIDTH 32
#define VOID_BEAM_COMET_HEIGHT 36

/// A hexagon of motes is traced, side by side, around the caster while a
/// ring of motes gathers behind; at VOID_BEAM_FIRE_FRAME a beam, the comet's
/// head and a 32-line gradient repeated nine times, shoots from the caster
/// toward the targets' side, fading out from VOID_BEAM_FADE_FRAME.
///
/// Abilities: Void Beam.
void BattleEffect_RunVoidBeam(struct BattleEffectParams *args) {
  struct BattleEffectState *state;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  struct BattleCamera *camera;
  u8 *sprites;
  s32 point[3];
  struct ScreenPoint corners[11];
  struct ScreenPoint ring;
  struct ScreenPoint segment;
  s32 flip;
  s32 count;
  u8 *row;
  s32 i;
  s32 k;
  u8 *buffer;
  s32 size;

  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  camera = Ram_HeapSlots->cameraState;
  sprites = Ram_HeapSlots->battleSheet;
  buffer = Ram_MapCellBuffer;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  Resource_LoadAndDecompress(RESOURCE_EMBER_STREAK_SHEET, state->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_COMET_SHEET_B,
                             state->sheet + VOID_BEAM_COMET_B_PICTURES, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprites, 0, 0);

  // The motes, one shade dimmer.
  for (i = 0; i != MOTE_SET_BYTES; i++) {
    s32 pixel = sprites[i];

    if (pixel != 0) {
      pixel -= 8;
      if (pixel <= 0) {
        pixel = 1;
      }
      sprites[i] = pixel;
    }
  }
  Resource_LoadAndDecompress(RESOURCE_RUNE_SHEET,
                             buffer + VOID_BEAM_RUNE_PICTURES, 1, 0);
  Iwram_ClearWordsEntry(buffer, RUNE_TEXTURE_SIZE * RUNE_TEXTURE_SIZE);

  // The beam's head: the rune, mirrored to 48 columns.
  for (i = 0; i != RUNE_HALF_HEIGHT; i++) {
    s32 j;

    for (j = 0; j != RUNE_HALF_WIDTH; j++) {
      buffer[RUNE_TEXTURE_RUNE + i * RUNE_TEXTURE_SIZE + j] =
          buffer[VOID_BEAM_RUNE_PICTURES + i * RUNE_HALF_WIDTH + j];
      buffer[RUNE_TEXTURE_RUNE + 2 * RUNE_HALF_WIDTH - 1 +
             i * RUNE_TEXTURE_SIZE - j] =
          buffer[VOID_BEAM_RUNE_PICTURES + i * RUNE_HALF_WIDTH + j];
    }
  }
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry(RESOURCE_LIGHTNING_BOLT_SHEET),
                  SHEET_PALETTE_BYTES);

  // The beam's body: a 32-line gradient, rising over a quarter turn of a sine.
  row = state->sheet + VOID_BEAM_BODY;
  for (i = 0; i != VOID_BEAM_BODY_SIZE; i++) {
    s32 shade = FIXEDtoINT(Trig_Sin(i * (DEGtoANG(90) / VOID_BEAM_BODY_SIZE)) *
                           CANVAS_LEVEL_BRIGHTEST);
    s32 j;

    for (j = 0; j != VOID_BEAM_BODY_SIZE; j++) {
      *row++ = shade;
    }
  }
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  // Each picture of the beam is drawn above its line and again upside down
  // below it, turned to face the targets.
  flip = COMPOSITE_FLIP_X;
  if (args->side != EFFECT_SIDE_ENEMIES) {
    flip = 0;
  }
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, flip | EFFECT_CLIP_BOUNDS,
                      BLITTER_MAX);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      flip | EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y,
                      BLITTER_MAX);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  draw[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;
  state->cameraDelta = 0;
  state->cameraPhase = 0;
  Task_Add(BattleCamera_ApplyPhasedDelta, TASK_PASS_FRAME);
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  Sound_PlayFar(SE_SPIN);
  size = 4;

  for (frame = 0; frame != VOID_BEAM_FRAMES; frame++) {
    // The camera turns a 256th of a turn a frame from frame 9, and eases to a
    // stop from 64 to 79; the other way round over the party.
    if ((u32)(frame - 9) <= 54) {
      state->cameraDelta = -(TRIG_TURN / 256);
    } else if ((u32)(frame - 64) <= 15) {
      state->cameraDelta = frame * 16 - 1280;
    } else {
      state->cameraDelta = 0;
    }
    if (args->side == EFFECT_SIDE_PARTY) {
      state->cameraDelta = -state->cameraDelta;
    }
    if (frame > 79) {
      s32 fade = frame * 2 - 160;

      REG_BLDALPHA = BLEND_ALPHA(16 - fade, BLEND_WEIGHT_MAX);
    }
    BattleCamera_SetViewFar(0, 0, 0, BATTLE_ZOOM_NORMAL);
    Matrix_Initialize();
    Matrix_SetLookAt(camera->eye, camera->focus);
    {
      struct MotionObject *caster;
      s32 traced;

      caster = BattleObject_GetFar(args->user)->object;
      if (caster->x > 0) {
        point[0] = caster->x - toFIXED(8);
      } else {
        point[0] = caster->x + toFIXED(8);
      }

      traced = frame * 8 - 336;
      if (traced >= 0) {
        for (i = 0; i != 6; i++) {
          s32 corner = (i + 4) % 6;

          point[1] = INTtoFIXED(gVoidBeam_Corners[corner][1]) + toFIXED(30);
          point[2] = INTtoFIXED(gVoidBeam_Corners[corner][0]) + caster->z;
          EffectPosition_ProjectPoint(point, &corners[i]);
          corners[i].x >>= 1;
        }
        for (i = 0; i != 6; i++) {
          s32 motes = traced - i * VOID_BEAM_SIDE_MOTES;

          if (motes > VOID_BEAM_SIDE_MOTES) {
            motes = VOID_BEAM_SIDE_MOTES;
          }
          if (motes < 0) {
            continue;
          }
          for (k = 0; k != motes; k++) {
            s32 x = corners[i].x +
                    Iwram_MulFX(k * (corners[(i + 1) % 6].x - corners[i].x),
                                FIXED_ONE / VOID_BEAM_SIDE_MOTES);
            s32 y = corners[i].y +
                    Iwram_MulFX(k * (corners[(i + 1) % 6].y - corners[i].y),
                                FIXED_ONE / VOID_BEAM_SIDE_MOTES);

            draw[0](canvas, sprites + gBattleEffect_MotePictures[size - 1],
                    x - size / 2, y - size, size, size * 2);
          }
        }
      }

      // The ring behind the caster.
      count = frame * 3 / 2;
      if (count > VOID_BEAM_RING_MOTES) {
        count = VOID_BEAM_RING_MOTES;
      }
      for (i = 0; i != count; i++) {
        s32 angle = i * (TRIG_TURN / VOID_BEAM_RING_MOTES);

        point[1] =
            caster->y - Trig_Cos(angle) * VOID_BEAM_RING_RADIUS + toFIXED(32);
        point[2] = caster->z + Trig_Sin(angle) * VOID_BEAM_RING_RADIUS;
        EffectPosition_ProjectPoint(point, &ring);
        ring.x >>= 1;
        // DEAD: the depth clamped to the range other effects size motes by;
        // nothing reads it, and the ROM keeps the tests and stores.
        CLAMP(ring.depth, 160, 799);
        draw[1](canvas, sprites + gBattleEffect_MotePictures[size - 1],
                ring.x - size / 2, ring.y - size, size, size * 2);
      }

      if (frame >= VOID_BEAM_FIRE_FRAME) {
        s32 fade;

        if (frame == VOID_BEAM_FIRE_FRAME) {
          state->particles[0].x = point[0];
          if (state->particles[0].x > 0) {
            state->particles[0].velocityX = -toFIXED(32);
          } else {
            state->particles[0].velocityX = toFIXED(32);
          }
          REG_BG2CNT =
              BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
        }
        if (frame == VOID_BEAM_FIRE_FRAME) {
          state->shakeFrames = 8;
          EFFECT_SHOW_DAMAGE(SE_CRASH);
          BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_HURL);
          BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, 0, 8);
        }
        for (i = 0; i != VOID_BEAM_SEGMENTS; i++) {
          point[0] = state->particles[0].x + state->particles[0].velocityX * i;
          point[1] = toFIXED(30);
          point[2] = caster->z;
          EffectPosition_ProjectPoint(point, &segment);
          segment.x >>= 1;
          if (args->side == EFFECT_SIDE_ENEMIES) {
            segment.x -= 32;
          }
          if (i > 0) {
            draw[0](canvas, state->sheet + VOID_BEAM_BODY, segment.x,
                    segment.y - VOID_BEAM_BODY_SIZE, VOID_BEAM_BODY_SIZE,
                    VOID_BEAM_BODY_SIZE);
            draw[1](canvas, state->sheet + VOID_BEAM_BODY, segment.x, segment.y,
                    VOID_BEAM_BODY_SIZE, VOID_BEAM_BODY_SIZE);
          } else if (frame <= 81) {
            draw[0](canvas, state->sheet + VOID_BEAM_COMET_B_PICTURES,
                    segment.x, segment.y - VOID_BEAM_COMET_HEIGHT,
                    VOID_BEAM_COMET_WIDTH, VOID_BEAM_COMET_HEIGHT);
            draw[1](canvas, state->sheet + VOID_BEAM_COMET_B_PICTURES,
                    segment.x, segment.y, VOID_BEAM_COMET_WIDTH,
                    VOID_BEAM_COMET_HEIGHT);
          }
        }
        // The body fades from VOID_BEAM_FADE_FRAME.
        row = state->sheet + VOID_BEAM_BODY;
        fade = 0;
        if (frame >= VOID_BEAM_FADE_FRAME) {
          fade = frame * 8 - 640;
        }
        for (i = 0; i != VOID_BEAM_BODY_SIZE; i++) {
          s32 shade =
              (FIXEDtoINT(Trig_Sin(i * (DEGtoANG(90) / VOID_BEAM_BODY_SIZE)) *
                          CANVAS_LEVEL_BRIGHTEST)) -
              fade;
          s32 j;

          if (shade < 0) {
            shade = 0;
          }
          for (j = 0; j != VOID_BEAM_BODY_SIZE; j++) {
            *row++ = shade;
          }
        }
      }
    }
    if (state->cameraPhase == 0) {
      state->cameraPhase = 1;
    }
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(state);
  }
  EFFECT_RELEASE_BLITTERS();
  Task_Remove(BattleEffect_TransferCanvas);
  Task_Remove(BattleCamera_ApplyPhasedDelta);
  BattleEffect_EndCanvasLayer();
}
#undef MEGAERA_PICTURES
#undef VOID_BEAM_FRAMES
#undef VOID_BEAM_SEGMENTS
#undef VOID_BEAM_SIDE_MOTES
#undef VOID_BEAM_RING_MOTES
#undef VOID_BEAM_RING_RADIUS
#undef VOID_BEAM_FIRE_FRAME
#undef VOID_BEAM_FADE_FRAME
#undef BALL_LIGHTNING_SPARKS_CLEARED
#undef BALL_LIGHTNING_SPARK_COUNT
#undef BALL_LIGHTNING_SPARK_FLOOR
#undef BALL_LIGHTNING_TORNADO_PICTURES
#undef BALL_LIGHTNING_BOLT_CELS
#undef BALL_LIGHTNING_BOLT_PICTURE_COUNT
#undef BALL_LIGHTNING_BOLT_CEL_WIDTH
#undef BALL_LIGHTNING_BOLT_CEL_HEIGHT
#undef BALL_LIGHTNING_BOLT_CEL_INSET
#undef BALL_LIGHTNING_BOLTS_SET_UP
#undef BALL_LIGHTNING_BOLTS
#undef BALL_LIGHTNING_FRAMES
#undef BALL_LIGHTNING_SIZE
#undef EARTH_FORCE_BALL_SIZE
#undef MEGAERA_SPARK_COUNT
#undef MEGAERA_FLASH_BURST_PICTURES
#undef MEGAERA_LIGHTNING_BOLT_PICTURES
#undef MEGAERA_SPARKLE_DOTS_PICTURES
#undef MEGAERA_FRAMES
#undef MEGAERA_SWORDS
#undef MEGAERA_SWORD_GROUND_Y
#undef MEGAERA_SWORD_POINT_BYTES
#undef MEGAERA_SPARK_FLOOR
#undef MEGAERA_BOLT_HEIGHT
#undef MEGAERA_FIRE_FRAME
#undef WHIRL_BEAM_WIDTH
#undef WHIRL_BEAM_HEIGHT
#undef MEGAERA_BILLOWS_SET_UP
#undef MEGAERA_BILLOWS
#undef MEGAERA_BILLOW_FRAMES
#undef MEGAERA_BILLOW_PICTURES
#undef MEGAERA_UNREAD_FIRST
#undef MEGAERA_UNREAD_COUNT
#undef MEGAERA_SWOOP_FRAME
#undef MEGAERA_TRAIL_FRAMES
#undef MEGAERA_SPARKLE_FRAMES
#undef VOID_BEAM_COMET_B_PICTURES
#undef VOID_BEAM_RUNE_PICTURES
#undef VOID_BEAM_BODY
#undef VOID_BEAM_BODY_SIZE
#undef VOID_BEAM_COMET_WIDTH
#undef VOID_BEAM_COMET_HEIGHT
