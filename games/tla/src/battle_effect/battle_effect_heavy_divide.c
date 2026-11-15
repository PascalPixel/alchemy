/// @file
/// The body of Heavy Divide and Strong Hit, then Cool, Wyrd Curse, Radiant Fire
/// and Hurricane.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

struct StripQuad;

/// The body of Heavy Divide (mode 0) and Strong Hit (mode 1). Heavy Divide:
/// a great sword (with four fading after-images, each a dimmer copy of the
/// sheet) swings down from over the caster, striking at frame
/// gHeavyBlow_StrikeFrames[0]. Strong Hit: an axe is thrown, spinning, from the
/// caster to the target. Either way chips of stone fly from the target and
/// bounce on the ground.
void BattleEffect_RunHeavyBlow(struct BattleEffectParams *args, s32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  // Unused: blit[1] gives the stack frame its 4 bytes.
  DrawRectangle blit[2];
  s32 frame;
  void *caster;
  s32 frames;
  struct ScreenPoint hit;
  struct ScreenPoint source;
  struct ScreenPoint target;
  struct ModelTexture texture;
  struct RenderModel *model;
  void *vertices;
  s32 i;
  s32 k;
  s32 zero; // One constant, rebuilt at each store.

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  REG_BLDALPHA = BLEND_ALPHA(16, 4);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      BLITTER_TRANSPARENT);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  blit[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  caster = BattleObject_GetFar(args->user)->object;
  if (mode == HEAVY_BLOW_HEAVY_DIVIDE) {
    frames = 80;
    Resource_LoadAndDecompress((s32)&ResourceId_GreatSwordSheet, work->sheet, 1,
                               1);

    // Four after-images, each in its own dimmer colour.
    for (k = 1; k != 5; k++) {
      ((u16 *)BG_PLTT)[59 + k] = (4 - k) * 0x1000 + 0x3c00;
      for (i = 0; i != 0x1000; i++) {
        if (work->sheet[i] == 0) {
          work->sheet[k * 0x1000 + i] = 0;
        } else {
          work->sheet[k * 0x1000 + i] = k + 59;
        }
      }
    }
  } else {
    Resource_LoadAndDecompress((s32)&ResourceId_AxeSheet, work->sheet, 1, 1);
    frames = 64;
  }
  Sound_PlayFar(SE_POWER_UP);
  zero = 0;

  for (frame = 0; frame != frames; frame++) {
    if (frame == 0) {
      s32 actor = args->targets[0];
      struct ScreenPoint *at = &hit;
      struct Particle *chip;

      /* FAKEMATCH(dead): the store in a once-only block. */
      do {
        chip = (struct Particle *)gMapGridCellBuffer;
      } while (0);
      EffectPosition_ProjectBattler(actor, at);
      for (i = 0; i != 256; i++, chip++) {
        s32 angle = Random16();
        s32 speed = 255;

        speed &= Random16();
        chip->x = INTtoFIXED(hit.x / 2);
        chip->y = INTtoFIXED(hit.y - 16);
        chip->velocityX = (Trig_Sin(angle) * (speed + 128)) >> 6;
        chip->velocityY = (Trig_Cos(angle) * (speed + 128)) >> 6;
        chip->variant = (Random16() & 15) + 16;
      }
    }
    if (mode == HEAVY_BLOW_HEAVY_DIVIDE) {
      if (frame == 8) {
        BattleEffect_TintBattler(args->user, TINT_KEEP, 2, 0, -1);
      }
    } else if (frame == 0) {
      BattleEffect_TintBattler(args->user, TINT_KEEP, 2, 0, -1);
      Actor_SetAnimationSpeedFar(caster, ANIMATION_STEP_NORMAL * 2);
    }

    vertices = Heap_AllocEwram(32);
    model = Render_AllocateModelList(1);
    EffectPosition_ProjectBattler(args->user, &source);
    EffectPosition_ProjectBattler(args->targets[0], &target);
    model->kind = MODEL_SHADED;
    model->texture = &texture;
    model->vertices = vertices;
    model->shade = zero;
    if (mode == HEAVY_BLOW_HEAVY_DIVIDE) {
      s32 size;
      s32 angle;

      texture.logWidth = 5;
      texture.logHeight = 7;
      model->mesh = gHeavyBlow_SwordMesh;
      if (frame > 59) {
        size = 68 - frame;
      } else {
        size = frame + 2;
      }
      if (size > 8) {
        size = 8;
      }
      if (size > 0) {
        if (frame > 38) {
          angle = DEGtoANG(90) - Trig_Sin((frame << 9) - 0x4e00) / 16;
        } else if (frame > 15) {
          angle = (TRIG_TURN - Trig_Cos((frame << 9) - DEGtoANG(45))) / 2;
        } else {
          angle = 0;
        }

        // The after-images keep the sword's last four angles.
        for (i = 4; i != 0; i--) {
          work->particles[i].variant = work->particles[i - 1].variant;
        }
        work->particles[0].variant = angle;
        for (i = 4; i != -1; i--) {
          struct Particle *step = &work->particles[i];

          if (i != 0 && frame > i + 38) {
            continue;
          }
          if (frame < i * 2) {
            continue;
          }
          texture.pixels = work->sheet + i * 0x1000;
          Matrix_Initialize();
          Matrix_Translate(INTtoFIXED(source.x / 2 - 64),
                           INTtoFIXED(source.y - 88), 0);
          Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
          if (args->side == EFFECT_SIDE_ENEMIES) {
            Matrix_RotateZ(DEGtoANG(180) - step->variant);
          } else {
            Matrix_RotateZ(step->variant + DEGtoANG(180));
          }
          Matrix_Translate(0, 0xfff00000, 0);
          Matrix_Scale(size << 15, size * 13192, size << 13);
          if (args->side == EFFECT_SIDE_ENEMIES) {
            Matrix_RotateY(DEGtoANG(180));
          }
          Render_TransformVertices(gBattleEffect_BeamCorners, vertices, 4);
          Render_DrawModelList(model);
        }
      }
    } else {
      model->mesh = gBattleEffect_SquareMesh;
      texture.logWidth = 6;
      texture.logHeight = 6;
      if (frame == 0) {
        for (i = 0; i != 5; i++) {
          work->particles[i].x = INTtoFIXED(source.x / 2);
          work->particles[i].y = INTtoFIXED(source.y - 32);
          work->particles[i].velocityX = (target.x / 2 - source.x / 2) << 11;
          work->particles[i].velocityY = (target.y - source.y) << 11;
          work->particles[i].z = zero;
          if (args->side == EFFECT_SIDE_ENEMIES) {
            work->particles[i].velocityZ = -toFIXED(1) / 16;
          } else {
            work->particles[i].velocityZ = toFIXED(1) / 16;
          }
        }
      }
      if (frame > 7) {
        FIXED scale = 80000;

        texture.pixels = work->sheet;
        Matrix_Initialize();
        Matrix_Translate(work->particles[0].x - toFIXED(64),
                         work->particles[0].y - toFIXED(64), 0);
        Matrix_Scale(scale >> 1, scale, FIXED_ONE / 2);
        Matrix_RotateZ(work->particles[0].z);
        if (args->side == EFFECT_SIDE_ENEMIES) {
          Matrix_RotateY(DEGtoANG(180));
        }
        Render_TransformVertices(gBattleEffect_SquareCorners, vertices, 4);
        Render_DrawModelList(model);
        BattleEffectKernels_IntegrateVector3(work->particles,
                                             PARTICLE_DAMPING_NONE, 0);
      }
    }
    Heap_Release(model);
    Heap_Release(vertices);

    if (frame == gHeavyBlow_StrikeFrames[mode]) {
      EFFECT_SHOW_DAMAGE(SE_CLATTER);
      BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_HURL);
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 8);
      work->shakeFrames = 8;
    }
    if (frame >= gHeavyBlow_StrikeFrames[mode]) {
      i = 0;
      if (i != gHeavyBlow_ChipCounts[mode]) {
        DrawRectangle blitter = blit[0];
        struct Particle *chip = (struct Particle *)gMapGridCellBuffer;

        do {
          if (chip->variant >= 0) {
            blitter(canvas, gHeavyBlow_ChipPixels, FIXED_INTEGER_PART(chip->x),
                    FIXED_INTEGER_PART(chip->y) - 1, 1, 2);
            BattleEffectKernels_IntegrateVector2(chip, 62, toFIXED(1) / 16);
            blitter(canvas, gHeavyBlow_ChipPixels, FIXED_INTEGER_PART(chip->x),
                    FIXED_INTEGER_PART(chip->y) - 1, 1, 2);
            BattleEffectKernels_IntegrateVector2(chip, 62, toFIXED(1) / 16);
            blitter(canvas, gHeavyBlow_ChipPixels, FIXED_INTEGER_PART(chip->x),
                    FIXED_INTEGER_PART(chip->y) - 1, 1, 2);
            BattleEffectKernels_IntegrateVector2(chip, 62, toFIXED(1) / 16);
            if (FIXED_INTEGER_PART(chip->y) > 104) {
              chip->velocityY = -chip->velocityY;
              chip->variant -= 8;
            }
            chip->variant--;
          }
          i++;
          chip++;
        } while (i != gHeavyBlow_ChipCounts[mode]);
      }
    }
    if (mode == HEAVY_BLOW_HEAVY_DIVIDE) {
      BattleCamera_Shake(4, 16);
    } else {
      BattleCamera_Shake(8, 8);
    }
    EFFECT_END_FRAME(work);
  }
  Actor_SetAnimationSpeedFar(caster, ANIMATION_STEP_NORMAL);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

/// The column pictures: 36 pixels wide, and a slim 17-pixel one after them.
#define SLIM_COLUMN_OFFSET 0x870
#define SPARK_OFFSET 0x3060

// How a column leans as it grows.
enum { LEAN_LEFT, LEAN_RIGHT, SLIM_LEAN_LEFT, SLIM_LEAN_RIGHT };

// Cool: ice columns burst up around the targets one after another, leaning
// as they grow, and stand until they sink back; sparks of frost fly up
// from the ground and drift upward.
//
// Setup: canvas layer 1 at half blend and full width, centred between the
// first and last target; the column sheet, the spark sheet after it, and
// the blue palette.
// Frame loop (54 frames and 4 more per level): the sparks start at frame
// 0; column i rises from its start frame 10 pixels a frame, strikes a
// target two frames in, and sinks 10 pixels a frame from frame 32 + 4 per
// level. The columns are drawn with max blitters until frame 24 on the
// higher levels, with additive ones otherwise.
/// Cool: leaning ice columns and sparks of frost.
void BattleEffect_RunCool(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 sinkFrame;
  DrawRectangle draw[2];
  struct ScreenPoint ends[2];
  s32 frame;
  s32 i;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  EFFECT_START_NARROW_CANVAS(BGCNT_PRIORITY(1));
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  Resource_LoadAndDecompress((s32)&ResourceId_CrestSheet, work->sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet,
                             work->sheet + SPARK_OFFSET, 1, 0);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry((s32)&ResourceId_BlueFlameSheet),
                       SHEET_PALETTE_BYTES);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  {
    s32 stagger;

    stagger = args->variant * 4;
    sinkFrame = stagger + 32;

    // Centre the canvas between the first and the last target.
    EffectPosition_ProjectBattler(args->targets[0], &ends[0]);
    EffectPosition_ProjectBattler(args->targets[args->targetCount - 1],
                                  &ends[1]);
    ends[0].x += (ends[1].x - ends[0].x) / 2;
    REG_BG2X = (64 - ends[0].x) << 8;

    frame = 0;
    if (stagger != -54) {
    top:
      // Frame 0: 16 sparks leave the ground in random directions.
      if (frame == 0) {
        struct Particle *spark;

        for (i = 0, spark = work->particles; i != 16; i++, spark++) {
          s32 speed = (Random16() & 0x1ff) + 128;
          s32 angle = Random16() & TRIG_ANGLE_MASK;

          spark->x = INTtoFIXED((Random16() & 63) + 32);
          spark->y = toFIXED(96);
          spark->velocityX = (Trig_Sin(angle) * speed) >> 8;
          spark->velocityY = (Trig_Cos(angle) * speed) >> 8;
          spark->variant = 8;
        }
      }
      if (frame == 32)
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);

      if (frame >= 0) {
        u32 mode = BLITTER_MAX;

        if (frame <= 23 && args->variant > 0)
          mode = BLITTER_ADDITIVE;
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                            COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, mode);
        draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                            EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, mode);
        // At its address: through the slots' base it would share the base
        // above.
        draw[1] = *(DrawRectangle *)&Ram_HeapSlots->ramCodePaired;

        for (i = 0; i != args->variant * 3 + 4; i++) {
          s32 age = frame - gCool_ColumnStarts[i];
          s32 x = gCool_ColumnX[i];
          s32 scale;
          s32 height;

          // Two frames after it starts, a column strikes the next target.
          if (age == 2) {
            s32 member = i % args->targetCount;

            EFFECT_STRIKE_TARGET(args, member, 3, KNOCKBACK_NUDGE);
            Sound_PlayFar(SE_POWER_UP);
            work->shakeFrames = 3;
          }

          // The slim columns stand a little over half as high.
          scale = 60;
          if (gCool_ColumnLeans[i] > LEAN_RIGHT)
            scale = 35;
          scale = gCool_ColumnHeights[i] * scale / 64;
          if (frame < sinkFrame)
            height = age * 10;
          else
            height = scale - (frame - sinkFrame) * 10;
          if (height > scale)
            height = scale;

          switch (gCool_ColumnLeans[i]) {
          case LEAN_LEFT:
            x -= height / 2;
            break;
          case LEAN_RIGHT:
            x += height / 2;
            break;
          case SLIM_LEAN_LEFT:
            x -= height / 4;
            break;
          case SLIM_LEAN_RIGHT:
            x += height / 4;
            break;
          }
          if (gCool_ColumnLeans[i] <= LEAN_RIGHT)
            draw[gCool_ColumnLeans[i] & 1](canvas, work->sheet, x - 18,
                                           112 - height, 36, height);
          else
            draw[gCool_ColumnLeans[i] & 1](canvas,
                                           work->sheet + SLIM_COLUMN_OFFSET,
                                           x - 9, 108 - height, 17, height);
        }
        HeapSlot_Release(HEAP_SLOT_RAM_CODE);
        HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
      }

      // Spark i rises from frame 8 + 2i through seven pictures, three frames
      // apiece; past the first 16 they wait until the columns sink.
      BattleEffect_FetchRectangleBlitters(args->side, draw);
      {
        for (i = 0; i != args->variant * 4 + 8; i++) {
          struct Particle *spark = &work->particles[i];

          if ((i <= 15 || frame >= sinkFrame) && frame >= i * 2 + 8 &&
              spark->variant <= 28) {
            s32 cell = spark->variant / 3;
            s32 x = FIXED_INTEGER_PART(spark->x);
            s32 y = FIXED_INTEGER_PART(spark->y);
            u32 size;
            u8 *src;

            if (cell > 6)
              cell = 6;
            src = work->sheet + gCool_SparkPictures[cell] + SPARK_OFFSET;
            size = gCool_SparkSizes[cell];
            draw[0](canvas, src, x - size / 2, y - size / 2, size, size);
            spark->variant++;
            BattleEffectKernels_IntegrateVector2(spark, 62, -toFIXED(1) / 8);
          }
        }
      }
      EFFECT_RELEASE_BLITTERS();

      BattleCamera_Shake(4, 4);
      EFFECT_END_FRAME(work);
      if (++frame != stagger + 54)
        goto top;
    }
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

#undef BLITTER

// Wyrd Curse: drops fall on the target and bounce away, feeding a glow that
// grows over it, then splashes rise and the target is struck six times.

#define BLITTER ((DrawRectangle)Ram_HeapSlots->ramCode)

#define WYRD_FRAMES 120
#define WYRD_DROPS 16

/// Where BurningSkullSheet is decoded in the effect's pictures.
#define WYRD_CURSE_BURNING_SKULLS 0x4000

/// Where the 32x32 glow is mirrored from its shaded disc in the effect's
/// pictures.
#define WYRD_CURSE_GLOW 0x1000

/// Abilities: Wyrd Curse.
///
/// A 32x32 glow is mirrored from a shaded disc, and BG palette 0 becomes a
/// dark ramp. Sixteen drops form over the target; from frame 0 one falls
/// every eight frames, and on reaching the ground it bounces away and adds
/// its weight to the glow (the last particle), which grows over the target
/// and fades from frame 64. From frame 64 six splashes, four frames apart,
/// rise from the drops, and from frame 80 the target is struck six times.
void BattleEffect_RunWyrdCurse(struct BattleEffectParams *args) {
  struct ModelTexture texture;
  struct ScreenPoint screen;
  void *canvas;
  // Unused: only draw[0] is read; draw[1] gives the stack frame its size.
  DrawRectangle draw[2];
  s32 frame;
  s32 zero;
  u8 *sprites;
  void *caster;
  struct BattleEffectWork *work;
  s32 i;
  struct Particle *drop;
  struct Particle *last;
  u16 *palette;
  s32 size;
  s32 width;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sprites = Ram_HeapSlots->battleSheet;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  REG_BLDCNT = 0;
  if (args->side == EFFECT_SIDE_ENEMIES) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
  }
  draw[0] = BLITTER;
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  width = 32;
  palette = (u16 *)&BG_PLTT_COLOR(0, 1);
  caster = BattleObject_GetFar(args->user)->object;
  Iwram_ClearWordsEntry((void *)BG_PLTT, 128);
  for (i = 0; i != 63; i++) {
    *palette++ = i / 3 + 8;
  }

  // The glow: a 16-pixel disc mirrored into the four quarters of 32x32.
  {
    s32 y;
    s32 x;
    s32 half;
    s32 offset;
    s32 (*clear)(void *destination, s32 size) = Iwram_ClearWordsEntry;

    size = width / 2;
    half = size / 2;
    clear(work->sheet + WYRD_CURSE_GLOW, 0x800);
    BattleEffect_DrawShadedCircles(work->sheet, size, FIXED_ONE);
    y = 0;
    offset = 0;
  nextRow:
    for (x = 0; x != half; x++) {
      u8 v = work->sheet[offset / 2 + x];

      if (v != 0) {
        s32 mirrorRow;
        s32 row;

        row = y * 32;
        work->sheet[WYRD_CURSE_GLOW + row + x * 2] = v;
        work->sheet[WYRD_CURSE_GLOW + row + x + x + 1] = v;
        work->sheet[WYRD_CURSE_GLOW + (offset + (size - x - 1)) * 2] = v;
        work->sheet[WYRD_CURSE_GLOW + (offset + (size - x - 1)) * 2 + 1] = v;
        mirrorRow = (32 - y) * 32;
        mirrorRow -= 32;
        work->sheet[WYRD_CURSE_GLOW + mirrorRow + x * 2] = v;
        work->sheet[WYRD_CURSE_GLOW + mirrorRow + x * 2 + 1] = v;
        work->sheet[WYRD_CURSE_GLOW + mirrorRow + (size - x - 1) * 2] = v;
        work->sheet[WYRD_CURSE_GLOW + mirrorRow + (size - x - 1) * 2 + 1] = v;
      }
    }
    y++;
    offset += size;
    if (y != size) {
      goto nextRow;
    }
  }
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_BurningSkullSheet,
                             work->sheet + WYRD_CURSE_BURNING_SKULLS, 0, 0);

  zero = 0;
  for (frame = 0; frame != WYRD_FRAMES; frame++) {
    struct RenderModel *model;
    void *vertices;

    s32 level;
    FIXED splash;

    if (frame == 0) {
      s32 still;

      EffectPosition_ProjectBattler(args->targets[0], &screen);
      still = 0;
      for (i = 0, drop = work->particles; i != WYRD_DROPS; i++, drop++) {
        drop->x = INTtoFIXED(screen.x / 2 + (Random16() & 15) - 8);
        drop->y = -8;
        drop->velocityX = still;
        drop->velocityY = still;
        drop->z = (Random16() & 3) + 4;
        drop->velocityZ = 160 - i * 16;
        drop->variant = still;
      }
      {
        struct Particle *glow = &work->particles[63];

        glow->x = INTtoFIXED(screen.x / 2);
        glow->y = toFIXED(112);
        glow->variant = zero;
      }
    }

    // The falling drops, one more every eight frames.
    if (frame <= 63) {
      for (i = 0, drop = work->particles; i != 6; i++, drop++) {
        if (frame > i * 8 && drop->variant == 0) {
          s32 dropSize = drop->z;

          draw[0](canvas, sprites + gBattleEffect_MotePictures[dropSize - 1],
                  FIXED_INTEGER_PART(drop->x) - dropSize / 2,
                  FIXED_INTEGER_PART(drop->y) - dropSize, dropSize,
                  dropSize * 2);
          BattleEffectKernels_IntegrateVector2(drop, PARTICLE_DAMPING_NONE,
                                               toFIXED(1) / 2);
          if (FIXED_INTEGER_PART(drop->y) > 111) {
            drop->variant = 1;
            // DEAD: overwritten by the next store; the ROM keeps the call, its
            // arithmetic and the store.
            drop->velocityX = ((Random16() & 31) - 16) << 12;
            drop->velocityX = gWyrdCurse_DropBounces[i % 4] << 13;
            drop->velocityY = 0;
          }
        }
      }
    }

    vertices = Heap_AllocEwram(32);
    model = Render_AllocateModelList(1);
    texture.logWidth = 5;
    texture.logHeight = 5;
    texture.pixels = work->sheet + WYRD_CURSE_GLOW;
    model->kind = MODEL_SHADED_MAX;
    model->mesh = gBattleEffect_SmallSquareMeshReversed;
    model->texture = &texture;
    model->vertices = vertices;
    model->shade = zero;

    // The glow gathers what the landed drops bring.
    {
      struct Particle *glow;
      struct Particle *landed;

      i = 0;
      glow = &work->particles[63];
      landed = work->particles;
      for (; i != 8; i++, landed++) {
        if (landed->variant > 0) {
          glow->variant += landed->velocityZ;
          landed->velocityZ /= 2;
        }
      }
    }
    last = &work->particles[63];
    level = last->variant;
    if (level > 0) {
      s32 scale = (level << 13) / 824;

      if (frame > 63) {
        last->variant = level - 24;
      }
      Matrix_Initialize();
      Matrix_Translate(last->x - toFIXED(64), last->y - toFIXED(64), 0);
      Matrix_Scale(scale * 2, scale, scale * 2);
      Matrix_RotateX(-DEGtoANG(45));
      Render_TransformVertices(gBattleEffect_GroundSquareCorners, vertices, 4);
      Render_DrawModelList(model);
    }

    // The splashes, from frame 64.
    texture.logWidth = 5;
    texture.logHeight = 5;
    model->kind = MODEL_SHADED_ADDITIVE;
    model->mesh = gBattleEffect_SmallSquareMeshReversed;
    splash = 25000;
    /* FAKEMATCH(barrier): an empty asm through splash hides its constant. */
    asm("" : "+r"(splash));
    i = 0;
    drop = work->particles;
  nextSplash:
    if (frame > i * 4 + 64) {
      model->shade = zero;
      texture.pixels =
          work->sheet + WYRD_CURSE_BURNING_SKULLS + ((i % 2) << 10);
      Matrix_Initialize();
      Matrix_Translate(drop->x - toFIXED(64), drop->y - toFIXED(48), 0);
      Matrix_Scale(splash >> 1, splash, splash >> 1);
      Matrix_RotateX(-DEGtoANG(90));
      Render_TransformVertices(gBattleEffect_GroundSquareCorners, vertices, 4);
      Render_DrawModelList(model);
      BattleEffectKernels_IntegrateVector2(drop, 60, -toFIXED(1) / 2);
    }
    i++;
    drop++;
    if (i != 6) {
      goto nextSplash;
    }
    Heap_Release(model);
    Heap_Release(vertices);

    if (frame == 64) {
      Sound_PlayFar(SE_BEAST_CRY);
    }
    for (i = 0; i != 6; i++) {
      if (frame == i * 4 + 80) {
        work->shakeFrames = 2;
        BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_NUDGE);
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, 0, 2);
        if (i == 0) {
          EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
        } else {
          Sound_PlayFar(SE_IMPACT);
        }
      }
    }
    BattleCamera_Shake(4, 4);
    EFFECT_END_FRAME(work);
  }
  Actor_SetAnimationSpeedFar(caster, ANIMATION_STEP_NORMAL);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

#undef BLITTER

#define FLAMES 32

#define RADIANT_FIRE_FRAMES 66

/// Where FireRingTexture is decoded in the effect's pictures.
#define RADIANT_FIRE_FIRE_RING 0x1000

/// Radiant Fire: eight bursts of fire bloom one after another around the
/// target, each a shaded disc that swells and shrinks under a spinning
/// square of flame, with a white flash of the canvas as the first lands.
///
/// Setup: canvas layer 0 at half blend; one max blitter; the flame texture
/// (resource 0x0c2) and the orange palette; 32 shaded discs of 2 to 64
/// pixels drawn into the map cell buffer, their offsets kept in the work
/// sheet.
/// Frame loop (66 frames): at frame 0 the bursts are placed at random
/// around the target and the user closes in; from frame 8 burst i lands at
/// frame 16 + 4i and burns for 24 frames.
///
/// Abilities: Chill, Radiant Fire.
void BattleEffect_RunRadiantFire(struct BattleEffectParams *args) {
  struct ScreenPoint screen;
  struct ModelTexture texture;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  struct BattleCamera *camera;
  s32 savedYaw;
  struct MotionObject *target;
  s16 *discs;
  struct Particle *flame;
  struct Particle *burst;
  struct Particle *square;
  struct RenderModel *model;
  void *vertices;
  s32 offset;
  s32 size;
  s32 cells;
  s32 i;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  Resource_LoadAndDecompress((s32)&ResourceId_FireRingTexture,
                             work->sheet + RADIANT_FIRE_FIRE_RING, 0, 0);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                       SHEET_PALETTE_BYTES);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  camera = Ram_HeapSlots->cameraWork;
  offset = 0;
  savedYaw = (s16)camera->yaw;
  size = 2;
  target = BattleObject_GetFar(args->targets[0])->object;

  // The discs, smallest first, one after another in the map cell buffer.
  cells = (s32)Ram_MapCellBuffer;
  discs = (s16 *)work->sheet;
  for (i = 0; i != FLAMES; i++) {
    BattleEffect_DrawShadedCircles((void *)(offset + cells), size, 0x14e20);
    discs[i] = offset;
    offset += (i * 2 + 3) * size / 2;
    size += 2;
  }

  for (frame = 0; frame != RADIANT_FIRE_FRAMES; frame++) {
    // Frame 0: each burst takes a random place on a ring about the target
    // that widens with its number, 48 pixels up, and is projected to the
    // screen; it waits 4 frames more than the one before it.
    if (frame == 0) {
      camera->yaw = savedYaw;
      Matrix_Initialize();
      Matrix_SetLookAt(camera->eye, camera->focus);
      flame = work->particles;
      for (i = 0; i != FLAMES; i++, flame++) {
        s32 angle;
        s32 radius;

        Random16();
        angle = Random16() & TRIG_ANGLE_MASK;
        radius = (Random16() & 7) + i * 4;
        SET_POSITION(flame, target->x + Trig_Sin(angle) * radius * 2,
                     target->y + Trig_Cos(angle) * radius + toFIXED(48),
                     target->z);
        EffectPosition_ProjectPoint(&flame->x, &screen);
        flame->x = screen.x / 2;
        flame->y = screen.y;
        Random16();
        flame->velocityX = 32 - i;
        flame->velocityY = Random16();
        flame->velocityZ = Random16();
        flame->variant = -i * 4;
      }
    }
    if (frame == 0)
      BattleEffect_SetApproachMotion(args->user, args->targets[0], 8, 80);

    // Frame 8: the canvas flashes white and the target is struck.
    if (frame == 8) {
      Iwram_FillWordsEntry(canvas, CANVAS_VIEW_BYTES,
                           CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
      work->shakeFrames = 16;
      BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_NUDGE);
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 8);
      EFFECT_SHOW_DAMAGE(SE_CRASH);
    }

    if (frame >= 8) {
      // Each burst strikes the target as it lands, then its disc swells
      // and shrinks with a sine over 32 frames.
      i = 0;
      burst = work->particles;
      for (; i != 8; i++, burst++) {
        burst->variant++;
        if (burst->variant == 8) {
          Sound_PlayFar(SE_CLATTER);
          work->shakeFrames = 8;
          BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_NUDGE);
          BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, 0, 8);
        }
        if (burst->variant >= 0 && burst->variant < 32) {
          s32 diameter;
          s32 index;

          diameter =
              (FIXEDtoINT(Trig_Sin(burst->variant << 10) * burst->velocityX)) *
              2;
          index = diameter - 2;
          if ((u32)index <= 61)
            Canvas_BlitAtFoot((void *)(*(s16 *)(index + (s32)discs) + cells),
                              burst->x, burst->y, diameter);
        }
      }

      // The square of flame turns over each burst for 24 frames, fading
      // over the last eight.
      vertices = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      texture.logWidth = 7;
      texture.logHeight = 7;
      texture.pixels = work->sheet + RADIANT_FIRE_FIRE_RING;
      model->kind = MODEL_SHADED_MAX;
      model->vertices = vertices;
      model->mesh = gBattleEffect_LargeSquareMesh;
      model->texture = &texture;
      i = 0;
      square = work->particles;
      for (; i != 8; i++, square++) {
        if (square->variant >= 0 && square->variant < 24) {
          s32 brightness = 0;
          s32 roll = square->variant * 3 << 11;

          if (square->variant > 15)
            brightness = (16 - square->variant) * 8;
          model->shade = brightness;
          Matrix_Initialize();
          Matrix_Translate(INTtoFIXED(square->x - 64),
                           INTtoFIXED(square->y - 64), 0);
          Matrix_RotateZ(square->velocityZ);
          Matrix_RotateX(square->velocityY);
          Matrix_ScaleUniform(roll);
          Render_TransformVertices(gBattleEffect_SquareCorners, vertices, 4);
          Render_DrawModelList(model);
        }
      }
      Heap_Release(model);
      Heap_Release(vertices);
    }

    BattleCamera_Shake(4, 4);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

/// The map cell buffer holds 1,024 more particles while an effect runs.

#define STREAKS 64

/// The splash: six 24x48 pictures in the work sheet after the streak.
#define SPLASH_OFFSET 0x3200
#define SPLASH_SIZE (24 * 48)

#define HURRICANE_FRAMES 68

/// Hurricane: driving rain slants down over the target, each drop bursting
/// into a splash where it lands, while the target is struck eight times; the
/// user rushes in at frame 32. The canvas then hands over to the summon
/// canvas effect in mode 3.
///
/// Setup: canvas layer 1 at half blend and full width, centred on the
/// target; one additive blitter; the cyan palette; a 4x64 streak drawn into
/// the work sheet as a slanted line, leaning with the wind; the splash
/// pictures after it.
/// Frame loop (68 frames): 64 streaks start above the screen at frame 0 and
/// fall in turn, one more each frame; a streak that falls below line 48
/// starts its splash.
///
/// Abilities: Scorch, Hurricane.
void BattleEffect_RunHurricane(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle blit[2];
  struct ScreenPoint position;
  struct Particle *streak;
  struct Particle *splash;
  s32 size;
  s32 frame;
  s32 i;

  size = 0x3c00;
  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
  blit[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry((s32)&ResourceId_CyanPalette),
                       SHEET_PALETTE_BYTES);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  EffectPosition_ProjectBattler(args->targets[0], &position);
  {
    s32 x = 64 - position.x;

    REG_BG2X = x << 8;
  }

  // The streak: one pixel of colour 24 a row, stepping a pixel sideways
  // every 16 rows against the wind.
  Iwram_ClearWordsEntry(work->sheet, 256);
  for (i = 0; i != STREAKS; i++) {
    if (args->side == EFFECT_SIDE_ENEMIES)
      work->sheet[i * 4 - i / 16 + 3] = 24;
    else
      work->sheet[i * 4 + i / 16] = 24;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet,
                             work->sheet + SPLASH_OFFSET, 1, 0);
  REG_BG2PA = Q_8_8(1);

  for (frame = 0; frame != HURRICANE_FRAMES; frame++) {
    // Frame 0: the streaks wait above the screen, drifting with the wind.
    if (frame == 0) {
      Sound_PlayFar(SE_AMBIENT_WATER);
      for (i = 0; i != 256; i++)
        PARTICLES[i].variant = -1;
      for (i = 0; i != STREAKS; i++) {
        streak = &work->particles[i];
        streak->y = INTtoFIXED((Random16() & 31) - 64);
        if (args->side == EFFECT_SIDE_ENEMIES) {
          streak->x = INTtoFIXED((Random16() & 63) + 40);
          streak->velocityX = -toFIXED(1);
        } else {
          streak->x = INTtoFIXED((Random16() & 63) + 24);
          streak->velocityX = toFIXED(1);
        }
        streak->velocityY = toFIXED(16);
        streak->variant = -1;
      }
    }
    if (frame == 32)
      BattleMotion_ApproachTargetFar(args->user, args->targets[0], 8,
                                     toFIXED(3));
    if (frame == 46) {
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_HURL);
    }

    // Each splash rises and spreads through its six pictures, four frames
    // apiece.
    splash = PARTICLES;
    for (i = 0; i != 256; i++, splash++) {
      if (splash->variant >= 0 && splash->variant < 24) {
        blit[0](canvas,
                work->sheet + SPLASH_OFFSET + splash->variant / 4 * SPLASH_SIZE,
                FIXED_INTEGER_PART(splash->x) - 12,
                FIXED_INTEGER_PART(splash->y) - 32, SMOKE_PUFF_WIDTH,
                SMOKE_PUFF_HEIGHT);
        BattleEffectKernels_IntegrateVector2(splash, 56, -toFIXED(1) / 4);
        splash->variant++;
      }
    }

    // The target is struck every four frames from frame 6.
    for (i = 0; i != 8; i++) {
      if (frame == i * 4 + 6) {
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, 0, 3);
        Sound_PlayFar(SE_WATER);
      }
    }

    // Streak i falls from frame i, shortened as it reaches the ground at
    // line 112; below line 48 it starts splash i.
    for (i = 0; i != 32; i++) {
      s32 y;
      s32 height;
      s32 x;

      streak = &work->particles[i];
      if (frame < i)
        continue;
      y = FIXED_INTEGER_PART(streak->y);
      if (y > 111)
        continue;
      height = 64;
      if (y > 48)
        height = 112 - y;
      blit[0](canvas, work->sheet, FIXED_INTEGER_PART(streak->x), y, 4, height);
      x = streak->x + streak->velocityX;
      streak->x = x;
      streak->y += streak->velocityY;
      if (FIXEDtoINT(streak->y) > 48) {
        /* FAKEMATCH(temporary): landing set to the buffer, then advanced. */
        struct Particle *landing = PARTICLES;

        landing += i;
        if (landing->variant == -1) {
          landing->y = toFIXED(112);
          landing->variant = 0;
          landing->x = x;
          landing->velocityX = (Random16() % 12 - 6) << 15;
          landing->velocityY = (-Random16() & 7) << 10;
        }
      }
    }

    BattleCamera_Shake(4, 4);
    EFFECT_END_FRAME(work);
  }

  // Clear the canvas and its characters and hand the layer over.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  /* FAKEMATCH(barrier): an empty asm. */
  __asm__("");
  Iwram_ClearWordsEntry((void *)BG_CHAR_BLOCK(1), size);
  Iwram_ClearWordsEntry(canvas, size);
  Task_Remove(Palette_StepFadeTransfer);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_RunDull(args);
}

/// Abilities: Earth Force.
void BattleEffect_RunEarthForce(struct BattleEffectParams *args) {
  BattleEffect_RunBoltTrail(args, BOLT_TRAIL_EARTH_FORCE);
}

/// Abilities: Thunder Mine.
void BattleEffect_RunThunderMine(struct BattleEffectParams *args) {
  BattleEffect_RunBoltTrail(args, BOLT_TRAIL_THUNDER_MINE);
}
#undef BLITTER
#undef SLIM_COLUMN_OFFSET
#undef SPARK_OFFSET
#undef WYRD_FRAMES
#undef WYRD_DROPS
#undef FLAMES
#undef STREAKS
#undef SPLASH_OFFSET
#undef SPLASH_SIZE
