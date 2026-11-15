/// @file
/// Trident, the leaping strikes of Planet Diver and Planetary, and Meteor
/// Blow's palette DMA.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// The leaping strike's modes (BattleEffect_RunLeapingStrike).
enum LeapingStrikeMode {
  LEAPING_STRIKE_PLANET_DIVER,
  LEAPING_STRIKE_PLANETARY
};

struct StripQuad;

extern const struct Scale gTrident_UnitScale;

#define TRIDENT_FRAMES 104
/// The sparks the trident throws, in the map cell buffer.
#define TRIDENT_SPARKS ((struct Particle *)(Ram_MapCellBuffer + 0x2964))
#define TRIDENT_SPARK_COUNT 128

/// Where JaggedStreakSheet is decoded in the effect's pictures.
#define TRIDENT_JAGGED_STREAKS 0xc56

/// Where FlameZigzagTexture is decoded in the map cell buffer.
#define TRIDENT_FLAME_ZIGZAG 0x1000

/// Where BillowTexture is decoded in the map cell buffer.
#define TRIDENT_BILLOW 0x1400

/// Where FlashBurstSheet is decoded in the map cell buffer.
#define TRIDENT_FLASH_BURST 0x5000

/// Poseidon (four objects) rises behind the targets as rings of foam
/// spread across the water; seven spinning tridents fly in from frame 18,
/// spray bursts up around them, and at 64 the waters strike every target.
void BattleEffect_RunTrident(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle blit[2]; // Unused: [1] is the rest of its two-word pseudo.
  u8 *sprites;
  s32 centerX;
  s16 *circles;
  s32 center[3];
  FIXED placement[4];
  struct Scale source;
  struct ModelTexture texture;
  struct RenderModel *model;
  void *vertices;
  struct Particle *p;
  s32 frame;
  s32 i;
  s32 size;
  s32 offset;
  s32 x;
  s32 grow; // Also each trident's first frame: one local, reused.
  s32 palette;
  s32 cells; // The cell buffer's address, held for the whole function.
  s32 orange;
  s32 orangeBase;
  s32 paletteBase;
  DrawRectangle draw;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sprites = Ram_HeapSlots->battleSheet;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  BattleEffect_SelectLivingTargets(work->args);
  BattleEffect_SpawnObjects(4, SPRITE_TRIDENT, 1);
  REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG1 |
              WININ_WIN1_BG2 | WININ_WIN1_CLR;
  REG_WINOUT = WINOUT_WIN01_BG0 | WINOUT_WIN01_CLR | WINOUT_WINOBJ_ALL;
  REG_WIN0V = WIN_RANGE(16, 124);
  REG_WIN1V = WIN_RANGE(BATTLE_VIEW_TOP, BATTLE_VIEW_BOTTOM);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  draw = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  blit[0] = draw;
  /* FAKEMATCH(temporary): the orange palette's id in a local. */
  orangeBase = (s32)&ResourceId_OrangePaletteB - 4;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  circles = (s16 *)(work->sheet + 0x4c56);
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet, 1, 0);
  // offset first holds the cell buffer's address, for the FlashBurst load.
  offset = (s32)gMapGridCellBuffer;
  Resource_LoadAndDecompress((s32)&ResourceId_JaggedStreakSheet,
                             work->sheet + TRIDENT_JAGGED_STREAKS, 0, 0);
  orange = orangeBase + 4;
  size = 2;
  /* FAKEMATCH(dead): a test on the blitter. */
  if (draw) {
    /* FAKEMATCH(temporary): the effect palette's id in a local. */
    paletteBase = (s32)&ResourceId_VioletPaletteF - 4;
    cells = (s32)gMapGridCellBuffer;
    palette = paletteBase + 4;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             (u8 *)offset + TRIDENT_FLASH_BURST, 1, 0);
  offset = 0x2400;
  // Twelve shaded droplets, 2 to 24 wide.
  for (i = 0; i != 12; i++) {
    BattleEffect_DrawShadedCircles((u8 *)(offset + cells), size, FIXED_ONE);
    circles[i] = offset;
    offset += (i * 2 + 3) * size / 2;
    size += 2;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_FlameBandTexture, (u8 *)cells, 0,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_FlameZigzagTexture,
                             Ram_MapCellBuffer + TRIDENT_FLAME_ZIGZAG, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_BillowTexture,
                             Ram_MapCellBuffer + TRIDENT_BILLOW, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry((s32)&ResourceId_VioletPaletteF),
                       SHEET_PALETTE_BYTES);
  for (i = 0, p = work->particles; i != 32; i++, p++) {
    s32 radius = i * 2;
    s32 angle = Random16() & TRIG_ANGLE_MASK;

    p->x = Trig_Sin(angle) * radius;
    p->y = -(Trig_Cos(angle) * radius);
    p->variant = i / 2 + 25;
  }
  BattleEffect_GetTargetsMidpoint(center);
  centerX = center[0] / 2;

  for (frame = 0; frame != TRIDENT_FRAMES; frame++) {
    if (frame == 0) {
      Iwram_CopyWordsEntry((void *)BG_PLTT, Resource_GetTableEntry(palette),
                           SHEET_PALETTE_BYTES);
    }
    if (frame > 76) {
      BattleEffect_StepPaletteToResource(palette);
    } else if (frame > 64) {
      BattleEffect_StepPaletteToResource(orange);
    } else if (frame > 23) {
      BattleEffect_StepPaletteToResource(palette);
    } else if (frame > 8) {
      BattleEffect_StepPaletteToResource(orange);
    }
    if (frame == 8) {
      work->shakeFrames = frame;
      for (i = 0; i != args->targetCount; i++) {
        BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, i, 16);
      }
    }
    if (frame == 48) {
      work->shakeFrames = 8;
    }
    if (frame == 64) {
      work->shakeFrames = 16;
      EFFECT_SHOW_DAMAGE(SE_CLATTER);
      for (i = 0; i != args->targetCount; i++) {
        BattleEffect_StartActorMotion(args->targets[i], 1, toFIXED(2),
                                      toFIXED(8), toFIXED(8), 120);
      }
    }
    if (frame == 0) {
      Sound_PlayFar(SE_POWER_UP);
    }
    if (frame == 4) {
      Sound_PlayFar(SE_CLATTER);
    }
    if (frame == 16) {
      Sound_PlayFar(SE_SCATTER);
    }
    if (frame == 64) {
      Sound_PlayFar(SE_CRASH);
    }

    // Poseidon rises.
    if (frame <= 63) {
      s32 left = centerX - 8;
      s32 visible = 128;
      s32 rise;

      if (frame <= 9) {
        rise = frame * 16 - 144;
      } else {
        rise = 0;
      }
      if (rise + 128 > 108) {
        visible = visible - rise - 20;
      }
      if (visible > 0) {
        source = gTrident_UnitScale;
        placement[3] = 0;
        placement[1] = toFIXED(255);
        for (i = 0; i != 4; i++) {
          placement[0] = INTtoFIXED(left * 2) + toFIXED(16);
          placement[2] = INTtoFIXED(rise + 40 + i * 32);
          Render_ApplyProjectedPlacementFar(work->sprites[i], placement,
                                            (s32 *)&source, 0);
        }
      }
    }

    // Spray: a pool of foam, then droplets thrown up.
    if ((u32)(frame - 12) <= 52) {
      if (frame == 12) {
        for (i = 0; i != 32; i++) {
          work->particles[i].variant = -2 * i;
        }
      }
      for (i = 0, p = work->particles, grow = frame * 4 - 156; i != 14;
           i++, p++) {
        p->variant++;
        if (i == 0) {
          if (frame > 39) {
            u8 *pool = work->sheet + 0x4c86;

            BattleEffect_DrawShadedCircles(pool, grow, FIXED_ONE);
            Canvas_BlitAtFoot(pool, centerX, 100, grow);
          }
          continue;
        }
        if (p->variant == 0) {
          s32 angle = Random16() & 0x7fff;
          s32 heading = angle + DEGtoANG(90);
          s32 speed = (Random16() & 511) + 512;
          FIXED sine;

          sine = Trig_Sin(heading) * speed;
          x = INTtoFIXED(centerX);
          p->x = (sine >> 4) + x;
          p->y = ((Trig_Cos(heading) * speed) >> 3) + toFIXED(104);
          angle += TRIG_TURN * 3 / 4;
          p->velocityX = (Trig_Sin(angle) * speed) >> 9;
          p->velocityY = (Trig_Cos(angle) * speed) >> 8;
          p->velocityZ = (Random16() & 7) + 4;
        }
        if ((u32)p->variant <= 31) {
          s32 drop =
              (FIXEDtoINT(Trig_Sin(p->variant << 10) * p->velocityZ)) * 2;
          s32 index = drop - 2;

          if ((u32)index <= 61) {
            s32 dropX = FIXED_INTEGER_PART(p->x);
            s32 dropY = FIXED_INTEGER_PART(p->y);
            s32 tier = index >> 1;

            BattleEffectKernels_IntegrateVector2(p, PARTICLE_DAMPING_NONE, 0);
            Canvas_BlitAtFoot((u8 *)(circles[tier] + cells), dropX, dropY,
                              drop);
          }
        }
      }
    }

    if (frame > 7) {
      s32 scale;

      vertices = Heap_AllocEwram(0x400);
      model = Render_AllocateModelList(1);
      texture.logWidth = 7;
      texture.logHeight = 7;
      texture.pixels = (void *)cells;
      model->kind = MODEL_SHADED_ADDITIVE;
      model->texture = &texture;
      model->flags = 0;
      model->mesh = gBattleEffect_CylinderMesh;
      model->vertices = vertices;
      model->scrollU = 0;
      model->scrollV = 0;

      // Eight rings of foam.
      for (i = 0; i != 8; i++) {
        s32 start = gTrident_RingStarts[i];
        s32 shade;

        if (frame <= start) {
          continue;
        }
        scale = (frame - start) * gTrident_RingGrowth[i] * 1500 + 0x4180;
        shade = (start - frame) * 8 + 56;
        if (shade > 0) {
          shade = 0;
        }
        if (shade <= MODEL_SHADE_INVISIBLE) {
          continue;
        }
        model->shade = shade;
        Matrix_Initialize();
        Matrix_Translate(INTtoFIXED(centerX - 64),
                         INTtoFIXED(gTrident_RingHeights[i]), 0);
        Matrix_Scale(scale, scale * 2, scale);
        model->scrollU = ((frame + i * 5) & 7) << 4;
        Matrix_RotateX(3000);
        Render_TransformVertices(gBattleEffect_CylinderCorners, vertices, 32);
        Render_DrawModelList(model);
      }

      // The tridents.
      if (frame <= 71) {
        const s32 *spin;
        struct Particle *t;
        struct Particle *spinner;

        texture.logWidth = 5;
        texture.logHeight = 7;
        model->texture = &texture;
        model->kind = MODEL_SHADED_ADDITIVE;
        model->mesh = gBattleEffect_ColumnMesh;
        model->vertices = vertices;
        model->scrollU = 0;
        model->scrollV = 0;
        if (frame == 8) {
          for (i = 0, t = &work->particles[32], spin = gTrident_Spins; i != 8;
               i++, t++, spin += 2) {
            t->z = spin[0];
            t->velocityZ = spin[1];
            t->variant = (i % 6) * 4;
          }
        }
        for (i = 0, grow = 18, spinner = &work->particles[32]; i != 7;
             grow += 4, i++, spinner++) {

          scale = i * 4;
          if (frame < grow || frame >= scale + 42) {
            continue;
          }
          x = (spinner->variant / 8) % 3;
          /* FAKEMATCH(dead): a test on x with both arms alike. */
          if (x) {
            model->shade = -16;
          } else {
            model->shade = -16;
          }
          texture.pixels = work->sheet + (x << 12) + TRIDENT_JAGGED_STREAKS;
          Matrix_Initialize();
          Matrix_Translate(INTtoFIXED(centerX - 64), toFIXED(32), 0);
          Matrix_RotateZ(spinner->z);
          if (i & 1) {
            Matrix_RotateY(DEGtoANG(180));
          }
          spinner->z += spinner->velocityZ;
          Matrix_Scale(FIXED_ONE * 2, 0x12710, FIXED_ONE);
          Render_TransformVertices(gTrident_ProngCorners + x * 4, vertices, 4);
          Render_DrawModelList(model);
        }
      }
      Heap_Release(model);
      Heap_Release(vertices);
    }

    // The strike: flashes and a fountain of sparks.
    if (frame > 63) {
      struct Particle *drawn;
      const u8 *cellTable;

      if (frame == 64) {
        for (i = 0, p = work->particles, x = INTtoFIXED(centerX); i != 32;
             i++, p++) {
          s32 angle = Random16();
          s32 speed = (Random16() & 511) + 512;
          s32 velocityY;

          p->velocityX = (Trig_Sin(angle) * speed) >> 8;
          velocityY = (Trig_Cos(angle) * speed) >> 7;
          p->velocityY = velocityY;
          if (i > 15) {
            p->x = (p->velocityX << 2) + x;
            p->y = (velocityY << 2) + toFIXED(100);
            p->variant = (16 - i) / 4;
          } else {
            p->x = x;
            p->y = INTtoFIXED(Random16() % 104);
            p->variant = -i / 4;
          }
        }
        for (i = 0, p = TRIDENT_SPARKS; i != TRIDENT_SPARK_COUNT; i++, p++) {
          s32 angle = (Random16() & 0x7fff) + DEGtoANG(90);
          s32 speed = (Random16() & 511) + 512;

          p->x = x;
          p->y = toFIXED(100);
          p->velocityX = (Trig_Sin(angle) * speed) >> 6;
          p->velocityY = (Trig_Cos(angle) * speed) >> 5;
          p->variant = (Random16() & 15) + 16;
        }
      }
      for (i = 0, drawn = work->particles; i != 32; i++, drawn++) {
        if ((u32)drawn->variant <= 23) {
          blit[0](canvas,
                  Ram_MapCellBuffer + TRIDENT_FLASH_BURST +
                      ((drawn->variant / 4) << 11),
                  FIXED_INTEGER_PART(drawn->x) - 16,
                  FIXED_INTEGER_PART(drawn->y) - 32, FLASH_BURST_WIDTH,
                  FLASH_BURST_HEIGHT);
          BattleEffectKernels_IntegrateVector2(drawn, 62, -toFIXED(1) / 4);
        }
        drawn->variant++;
      }
      for (i = 0, cellTable = (const u8 *)gBattleEffect_MotePictures,
          drawn = TRIDENT_SPARKS;
           i != 128; i++, drawn++) {
        s32 spark;
        s32 at;

        if (drawn->variant < 0) {
          continue;
        }
        spark = (drawn->variant >> 3) + 1;
        at = (spark - 1) * 2;
        blit[0](canvas, sprites + *(const u16 *)(cellTable + at),
                FIXED_INTEGER_PART(drawn->x) - spark / 2,
                FIXED_INTEGER_PART(drawn->y) - spark, spark, spark * 2);
        drawn->variant--;
        BattleEffectKernels_IntegrateVector2(drawn, 56, toFIXED(1) / 8);
      }
    }

    // Glints over the water.
    if ((u32)(frame - 24) <= 39) {
      for (i = 0; i != 4; i++) {
        s32 k = i & 3;
        s32 angle = Random16();
        s32 x = centerX + (FIXEDtoINT(Trig_Sin(angle) << 3)) -
                (gBattleEffect_GlintPictureWidths[k] >> 1);
        s32 y = (i << 5) + (FIXEDtoINT(Trig_Cos(angle) << 4)) -
                (gBattleEffect_GlintPictureHeights[k] >> 1);
        u32 flip = Random16();

        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                            gTrident_GlintFlips[flip & 3] | 3, 1);
        ((DrawRectangle)Ram_HeapSlots->ramCodePaired)(
            canvas, work->sheet + gBattleEffect_GlintPictures[k], x, y,
            gBattleEffect_GlintPictureWidths[k],
            gBattleEffect_GlintPictureHeights[k]);
        HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
      }
    }
    BattleCamera_Shake(16, 16);
    EFFECT_END_FRAME(work);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  for (i = 0; i != 4; i++) {
    Sprite_DestroyFar(work->sprites[i]);
  }
  BattleEffect_EndCanvasLayer();
}

/// Abilities: Planet Diver.
void BattleEffect_RunPlanetDiver(struct BattleEffectParams *args) {
  BattleEffect_RunLeapingStrike(args, LEAPING_STRIKE_PLANET_DIVER);
}

/// Abilities: Planetary.
void BattleEffect_RunPlanetary(struct BattleEffectParams *args) {
  BattleEffect_RunLeapingStrike(args, LEAPING_STRIKE_PLANETARY);
}

#define LEAP_FRAMES 88
#define SPARKS ((struct Particle *)(Ram_MapCellBuffer + 0x6000))

/// Where FlameballSheet (Planet Diver) or ClawSlashSheet (Planetary) is decoded
/// in the effect's pictures.
#define LEAPING_STRIKE_PICTURES 0x3000

/// Where BillowTexture is decoded in the effect's pictures.
#define LEAPING_STRIKE_BILLOW 0x4000

/// Where FlameBandTexture is decoded in the effect's pictures.
#define LEAPING_STRIKE_FLAME_BAND 0x5000

/// The caster flashes and sixteen flames ring it (frames 2-15), leaps up at
/// frame 4 and comes down on the target at frame 16, sliding to it over the
/// frames after; on landing 256 sparks burst from it. For Planet Diver the
/// caster then shows a fireball and one target is struck; for Planetary a claw
/// slashes in 3D, three rings and two pillars spread, every target is struck
/// and at frame 50 all are thrown back.
void BattleEffect_RunLeapingStrike(struct BattleEffectParams *args, s32 mode) {
  struct ScreenPoint pos;
  struct ModelTexture texture;
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw[2];
  u8 *graphics;
  struct MotionObject *caster;
  struct MotionObject *target;
  s32 stepX;
  s32 stepY;
  s32 homeX;
  s32 homeZ;
  s32 landed;
  struct Particle *spark;
  s32 frame;
  s32 i;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  graphics = Ram_HeapSlots->battleSheet;
  BattleEffect_ConfigureEffectDisplay();
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, graphics, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet, 1,
                             0);
  if (mode == LEAPING_STRIKE_PLANET_DIVER) {
    Resource_LoadAndDecompress((s32)&ResourceId_FlameballSheet,
                               work->sheet + LEAPING_STRIKE_PICTURES, 1, 0);
  } else {
    Resource_LoadAndDecompress((s32)&ResourceId_ClawSlashSheet,
                               work->sheet + LEAPING_STRIKE_PICTURES, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_BillowTexture,
                               work->sheet + LEAPING_STRIKE_BILLOW, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_FlameBandTexture,
                               work->sheet + LEAPING_STRIKE_FLAME_BAND, 0, 0);
  }
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                       SHEET_PALETTE_BYTES);
  // The blitter faces the caster's side.
  if (args->side == EFFECT_SIDE_ENEMIES)
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  else
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  work->transferMode = CANVAS_COPY_AND_FADE;
  if (mode == LEAPING_STRIKE_PLANET_DIVER)
    work->transferValue = CANVAS_FADE_THREE_QUARTERS;
  else
    work->transferValue = CANVAS_FADE_HALF;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  for (i = 0; i != 256; i++)
    SPARKS[i].variant = 0;

  caster = BattleObject_GetFar(args->user)->object;
  target = BattleObject_GetFar(args->targets[0])->object;
  stepX = 0;
  stepY = 0;
  homeX = caster->x;
  homeZ = caster->z;
  landed = 0;

  for (frame = 0; frame != LEAP_FRAMES; frame++) {
    if (frame == 0) {
      SET_POSITION(caster, homeX, 0, homeZ);
      landed = 0;
    }
    if (frame > 17 || frame == 0) {
      EffectPosition_ProjectBattler(args->user, &pos);
      pos.x /= 2;
    }

    // Frames 2-15: a flash, then sixteen flames ringing the caster.
    if ((u32)(frame - 2) <= 1)
      draw[0](canvas, work->sheet, pos.x - 16, pos.y - FLASH_BURST_HEIGHT,
              FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
    if ((u32)(frame - 4) <= 11) {
      for (i = 0; i != 16; i++) {
        s32 x = pos.x + (FIXEDtoINT(frame * Trig_Sin(i << 12)));
        s32 y = pos.y + (FIXEDtoINT(frame * Trig_Cos(i << 12))) - frame;

        draw[0](canvas, work->sheet + ((frame - 4) / 2 << 11), x - 16,
                y - FLASH_BURST_HEIGHT, FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
      }
    }

    // Frame 4: the leap, away from the caster's own side.
    if (frame == 4) {
      caster->autoFaceMotion = 0;
      caster->verticalMotionStrength = 0x9999;
      caster->velocityY = toFIXED(15);
      if (caster->x < 0)
        caster->velocityX = -toFIXED(8);
      else
        caster->velocityX = toFIXED(8);
      if (mode == LEAPING_STRIKE_PLANETARY)
        caster->velocityX = -caster->velocityX;
      Actor_SelectAnimationFar(caster, 2);
      work->shakeFrames = 4;
      Sound_PlayFar(SE_WHOOSH);
    }

    // Frame 16: down at the target's depth, then sliding to it over 16
    // frames.
    if (frame == 16) {
      s32 slide = 16;

      caster->verticalMotionStrength = 0;
      if (mode == LEAPING_STRIKE_PLANET_DIVER)
        caster->z = target->z;
      else
        caster->z = 0;
      caster->z = target->z;
      caster->velocityX = 0;
      caster->velocityY = 0;
      caster->velocityZ = 0;
      stepX = (target->x * 60 / 100 - caster->x) / slide;
      stepY = (target->y - caster->y) / slide;
      Actor_StartIdleFar(caster);
    }

    if (frame > 17) {
      // Sliding to the target while in the air.
      if (caster->y > 0) {
        caster->x += stepX;
        caster->y += stepY;
      }
      if (caster->y <= 0) {
        if (!landed) {
          // The landing: sparks burst from the caster.
          landed = 1;
          caster->y = 0;
          spark = SPARKS;
          for (i = 0; i != 256; i++) {
            s32 speed = 0x3ff;
            s32 angle;

            // Masked in place, the mask assigned first.
            speed &= Random16();
            angle = Random16() & TRIG_ANGLE_MASK;

            speed += 32;
            spark->x = INTtoFIXED(pos.x);
            spark->y = INTtoFIXED(pos.y - 24);
            spark->velocityX = (Trig_Sin(angle) * speed) >> 6;
            spark->velocityY = -((Trig_Cos(angle) * speed) << 1) >> 6;
            spark->variant = (Random16() & 7) + 32;
            spark++;
          }
          work->shakeFrames = 4;
          if (mode == LEAPING_STRIKE_PLANET_DIVER) {
            EFFECT_SHOW_DAMAGE(SE_CRASH);
            BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_HURL);
            BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                     POSE_DAMAGE, 0, 8);
          } else {
            Sound_PlayFar(SE_CRASH);
            for (i = 0; i != args->targetCount; i++) {
              BattleMotion_ApplyKnockbackFar(args->targets[i],
                                             KNOCKBACK_LAUNCH);
              BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                       POSE_DAMAGE, i, 16);
            }
          }
        }
      }

      if (mode == LEAPING_STRIKE_PLANET_DIVER) {
        // The fireball over the caster while it is in the air.
        if (caster->y > 0)
          draw[0](canvas, work->sheet + LEAPING_STRIKE_PICTURES, pos.x - 20,
                  pos.y - 52, FLAMEBALL_WIDTH, FLAMEBALL_HEIGHT);
      } else {
        struct RenderModel *model;
        void *projected;
        s32 kind;
        s32 shade;

        projected = Heap_AllocEwram(1024);
        model = Render_AllocateModelList(1);
        shade = 0;
        if (frame == 40)
          work->shakeFrames = 14;

        // The claw, slashing until frame 45.
        // A set-once local: it gets no register and reloads at each use.
        kind = 6;
        texture.logWidth = 6;
        texture.logHeight = 6;
        texture.pixels = work->sheet + LEAPING_STRIKE_PICTURES;
        model->kind = kind;
        model->mesh = gBattleEffect_SquareMesh;
        model->texture = &texture;
        model->vertices = projected;
        model->shade = shade;
        if (frame <= 45) {
          Matrix_Initialize();
          Matrix_Translate(INTtoFIXED(pos.x - 60), INTtoFIXED(pos.y - 80), 0);
          Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
          if (args->side == EFFECT_SIDE_ENEMIES) {
            Matrix_RotateZ(-0x3388);
          } else {
            Matrix_RotateY(DEGtoANG(180));
            Matrix_RotateZ(-0x3388);
          }
          Matrix_ScaleUniform(0x286a0);
          Render_TransformVertices(gBattleEffect_SquareCorners, projected, 4);
          Render_DrawModelList(model);
        }

        // Three rings, each from its own frame, spinning as they spread.
        texture.logWidth = 6;
        texture.logHeight = 6;
        texture.pixels = work->sheet + LEAPING_STRIKE_BILLOW;
        model->kind = MODEL_SHADED_ADDITIVE;
        model->mesh = gBattleEffect_RingMesh;
        for (i = 0; i != 3; i++) {
          s32 start = gLeapingStrike_WaveStarts[i] + 40;

          if (frame > start) {
            shade = (start - frame) * 8 + 56;

            if (shade > 0)
              shade = 0;
            if (shade > MODEL_SHADE_INVISIBLE) {
              s32 scale;

              start = frame - start;
              scale =
                  start * gLeapingStrike_WaveGrowth[i] * 2000 + FIXED_ONE / 2;

              model->shade = shade;
              Matrix_Initialize();
              model->scrollV = (frame * 8 + i * 16) & 63;
              Matrix_Translate(INTtoFIXED(pos.x - 60),
                               INTtoFIXED(gLeapingStrike_WaveHeights[i]), 0);
              Matrix_Scale(scale >> 1, scale << 2, scale / 4);
              Matrix_RotateX(1000);
              if (i & 1)
                Matrix_RotateY((i * 4 + frame) << 11);
              else
                Matrix_RotateY((i * 4 - frame) << 11);
              Render_TransformVertices(gBattleEffect_RingCorners, projected,
                                       32);
              Render_DrawModelList(model);
            }
          }
        }

        // Two pillars where the caster landed, from their own frames.
        model->scrollV = 0;
        model->mesh = gBattleEffect_CylinderMesh;
        texture.pixels = work->sheet + LEAPING_STRIKE_FLAME_BAND;
        texture.logWidth = 7;
        texture.logHeight = 7;
        for (i = 0; i != 2; i++) {
          s32 start = gLeapingStrike_RingStarts[i] + 34;
          struct Particle *base = &work->particles[i];

          if (frame == start) {
            base->x = pos.x;
            base->y = pos.y;
          }
          if (frame > start) {
            shade = (start - frame) * 8 + 40;

            if (shade > 0)
              shade = 0;
            if (shade > MODEL_SHADE_INVISIBLE) {
              s32 scale;
              s32 scroll;

              start = frame - start;
              scale = start * gLeapingStrike_RingGrowth[i] * 1500 + 0x4180;

              model->shade = shade;
              // Masked in place, the mask assigned first.
              scroll = 7;
              scroll &= frame + i * 5;
              model->scrollU = scroll << 4;
              Matrix_Initialize();
              Matrix_Translate(INTtoFIXED(base->x - 60),
                               INTtoFIXED(gLeapingStrike_RingHeights[i]), 0);
              Matrix_Scale(scale, scale * 2, scale);
              Matrix_RotateX(2000);
              Render_TransformVertices(gBattleEffect_CylinderCorners, projected,
                                       32);
              Render_DrawModelList(model);
            }
          }
        }
        Heap_Release(model);
        Heap_Release(projected);
      }
    }

    if (mode == LEAPING_STRIKE_PLANETARY && frame == 50) {
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      for (i = 0; i != args->targetCount; i++)
        BattleEffect_StartActorMotion(args->targets[i], 1, toFIXED(4),
                                      toFIXED(16), toFIXED(1), 250);
    }

    // The sparks fall, bounce off the ground at 112 and shrink.
    spark = SPARKS;
    for (i = 0; i != 256; i++) {
      if (spark->variant > 0) {
        spark->variant--;
        BattleEffectKernels_IntegrateVector2(spark, 56, toFIXED(1) / 8);
        if (spark->y > toFIXED(112)) {
          spark->velocityY = -spark->velocityY / 2;
        } else if ((u32)spark->x < toFIXED(127) && spark->y >= 0) {
          s32 size = spark->variant / 8 + 1;

          draw[0](canvas, graphics + gBattleEffect_MotePictures[size - 1],
                  (FIXEDtoINT(spark->x)) - size / 2,
                  (FIXEDtoINT(spark->y)) - size, size, size * 2);
        }
      }
      spark++;
    }

    BattleCamera_Shake(12, 12);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

/// Meteor Blow's horizontal-blank callback, the same routine as
/// actor_move_random.c's BattleEffect_ArmPaletteHBlankDma: stops DMA 0,
/// then arms it to copy one BG palette word per scanline from the line table in
/// the effect work, so the backdrop's colour changes down the screen.
void BattleEffect_ArmMeteorBlowPaletteDma(void) {
  u8 *work = Ram_HeapSlots->battleEffect;
  volatile u16 *channel = REG_DMA0;
  u32 palette;

  channel[DMA_CONTROL_HALF] &= (u16)~DMA_START_TIMING_DRQ_REPEAT;
  channel[DMA_CONTROL_HALF] &= (u16)~DMA_ENABLE;
  palette = 0xa0;
  (void)channel[DMA_CONTROL_HALF];
  Dma_Set(work + 0x7640, (void *)(palette << 19), DMA_HBLANK_HALFWORD_PER_LINE,
          (volatile u32 *)channel);
}

/// Fills a grid of columns by rows vertices with two triangles a cell, each
/// cell cellWidth by cellHeight texels of the texture, and ends the list with
/// an all-zero triangle.
void Mesh_BuildGrid(struct MeshTriangle *triangles, s32 columns, s32 rows,
                    s32 cellWidth, s32 cellHeight) {
  s32 row;
  s32 column;
  struct MeshTriangle *second;

  for (row = 0; row != rows; row++) {
    for (column = 0; column != columns; column++) {
      if (row < rows - 1) {
        struct MeshTriangle *triangle =
            (struct MeshTriangle *)((row * columns + column) * 24 +
                                    (u32)triangles);
        s32 u0 = column * cellWidth;
        s32 u1 = u0 + cellWidth - 1;
        s32 v0 = row * cellHeight;
        s32 v1 = v0 + cellHeight - 1;

        triangle[0].texCoord[0][0] = u1;
        triangle[0].texCoord[0][1] = v0;
        triangle[0].texCoord[1][0] = u0;
        triangle[0].texCoord[1][1] = v1;
        triangle[0].texCoord[2][0] = u1;
        triangle[0].texCoord[2][1] = v1;
        second = triangle + 1;
        second->texCoord[0][0] = u0;
        second->texCoord[0][1] = v0;
        second->texCoord[1][0] = u0;
        second->texCoord[1][1] = v1;
        second->texCoord[2][0] = u1;
        second->texCoord[2][1] = v0;
        triangle->vertex[0] = (column + 1) % columns + row * columns;
        triangle->vertex[1] = column % columns + columns + row * columns;
        triangle->vertex[2] = (column + 1) % columns + columns + row * columns;
        second->vertex[0] = column % columns + row * columns;
        second->vertex[1] = column % columns + columns + row * columns;
        second->vertex[2] = (column + 1) % columns + row * columns;
      }
    }
  }
  triangles[(rows - 1) * columns * 2].vertex[0] = 0;
  triangles[(rows - 1) * columns * 2].vertex[1] = 0;
  triangles[(rows - 1) * columns * 2].vertex[2] = 0;
}
#undef BLITTER
#undef LEAP_FRAMES
#undef SPARKS
