/// @file
/// Volcano.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

// The fire pillars stand among the targets and scroll a column of fire up over
// a flickering flame; each erupts in a fountain of sparks and keeps spitting
// more.

#define VOLCANO_FRAMES 96
#define VOLCANO_ERUPTION 200    ///< sparks thrown as a pillar starts
#define VOLCANO_TRICKLE 4       ///< and each frame after
#define VOLCANO_SPARK_POOL 1024 ///< sparks in PARTICLES
/// The flame on odd frames, then on even frames, after the 34x104 column.
#define VOLCANO_FLAME_PICTURE_ODD (34 * 104)
#define VOLCANO_FLAME_PICTURE_EVEN (VOLCANO_FLAME_PICTURE_ODD + 20 * 37)

/// One, two or four columns of fire rise among the targets, each erupting in a
/// fountain of sparks and knocking the targets back.
///
/// Abilities: Volcano, Eruption, Pyroclasm, Lava Shower, Molten Bath, Magma
/// Storm.
void BattleEffect_RunVolcano(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 pillar;
  u8 *sheet;
  struct ScreenPoint screen;
  BlitterPair draw;
  struct Actor *actor;
  s32 i;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  sheet = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  work->args = args;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  Resource_LoadAndDecompress(RESOURCE_FIRE_PILLAR_SHEET_A, work->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sheet, 0, 0);
  if (work->args->variant == EFFECT_VARIANT_BASIC)
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry(RESOURCE_RED_PALETTE_B), 128);
  else if (work->args->variant == EFFECT_VARIANT_STRONGEST)
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry(RESOURCE_ORANGE_PALETTE_B), 128);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_LIGHTEN);
  draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_2);

  for (i = 0; i != VOLCANO_SPARK_POOL; i++)
    PARTICLES[i].variant = 0;

  actor = GetBattleObjectSlotFar(work->args->targets[0])->actor;
  for (i = 0; i != 4; i++) {
    struct Particle *base = &work->particles[i];
    s32 x;

    x = ((Random16() & 15) + 72) << 16;
    base->y = 0;
    base->x = x;
    base->z = px(gVolcano_Depths[work->args->variant][i]);
    if (actor->x.fixed < 0)
      base->x = -x;
  }

  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  for (frame = 0; frame != VOLCANO_FRAMES; frame++) {
    struct BattleCamera *camera = gBattleCamera;

    if (work->args->variant == EFFECT_VARIANT_STRONGEST && frame < 64) {
      if (work->args->side == EFFECT_SIDE_ENEMIES)
        camera->yaw = camera->yaw + 192;
      else
        camera->yaw = (s16)camera->yaw - 192;
    }
    if (frame == 16)
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    EFFECT_VIEW_FROM_CAMERA(camera);

    if (frame < 64) {
      for (pillar = 0; pillar != gVolcano_Counts[work->args->variant];
           pillar++) {
        Projection_Point(&work->particles[pillar], &screen);
        screen.x /= 2;
        screen.y -= 8;
        if (frame == gVolcano_StartFrames[pillar])
          Sound_PlayFar(SE_CRASH);
        // The column scrolls up over its flickering flame.
        if (frame >= gVolcano_StartFrames[pillar] + 4) {
          s32 scroll = (frame * 16 + pillar * 25) % 104;

          draw[pillar & 1](canvas, work->sheet, screen.x - 17,
                           screen.y - scroll - 104, 34, 104);
          draw[pillar & 1](canvas, work->sheet, screen.x - 17,
                           screen.y - scroll, 34, scroll);
          if (frame & 1) {
            draw[0](canvas, work->sheet + VOLCANO_FLAME_PICTURE_ODD,
                    screen.x - 20, screen.y - 24, 20, 37);
            draw[1](canvas, work->sheet + VOLCANO_FLAME_PICTURE_ODD, screen.x,
                    screen.y - 24, 20, 37);
          } else {
            draw[0](canvas, work->sheet + VOLCANO_FLAME_PICTURE_EVEN,
                    screen.x - 20, screen.y - 24, 20, 37);
            draw[1](canvas, work->sheet + VOLCANO_FLAME_PICTURE_EVEN, screen.x,
                    screen.y - 24, 20, 37);
          }
        }
        // Sparks: a fountain at the start, then a few a frame.
        if (frame == gVolcano_StartFrames[pillar] ||
            frame >= gVolcano_StartFrames[pillar] + 16) {
          s32 spawned = 0;

          for (i = 0; i != VOLCANO_SPARK_POOL; i++) {
            struct Particle *spark = &PARTICLES[i];

            if (spark->variant == 0) {
              s32 speed;
              s32 angle;

              speed = (Random16() & 0x3ff) + 32;
              angle = (Random16() & 0x7fff) - TRIG_TURN / 4;
              SET_POSITION(spark, screen.x << 8, (screen.y << 8) + (16 << 8),
                           (Trig_Sin(angle) * speed) >> 15);
              spark->velocityY = -(Trig_Cos(angle) * speed * 2) >> 15;
              spawned++;
              if (frame == gVolcano_StartFrames[pillar]) {
                spark->variant = (Random16() & 7) + 48;
                if (spawned == VOLCANO_ERUPTION)
                  break;
              } else {
                spark->variant = (Random16() & 7) + 24;
                if (spawned == VOLCANO_TRICKLE)
                  break;
              }
            }
          }
        }
        if (frame == gVolcano_StartFrames[pillar]) {
          work->shakeFrames = 2;
          EFFECT_FOR_EACH_TARGET(i, work->args) {
            BattleEffect_TintBattler(work->args->targets[i],
                                     SPRITE_COLOR_RED_TINT, POSE_DAMAGE, i, 8);
            BattleMotion_ApplyKnockbackFar(work->args->targets[i],
                                           KNOCKBACK_HOP);
          }
        }
      }
    }

    // The sparks, in 24.8 fixed point (z holds the speed across).
    for (i = 0; i != VOLCANO_SPARK_POOL; i++) {
      struct Particle *spark = &PARTICLES[i];
      s32 life = spark->variant;

      if (life > 0) {
        s32 x;
        s32 y;
        s32 drawY;

        spark->variant = life - 1;
        x = spark->x += spark->z;
        y = spark->y += spark->velocityY;
        spark->z = spark->z * 60 / 64;
        spark->velocityY = spark->velocityY * 60 / 64 - 16;
        drawY = y / 256;
        if (drawY > 120) {
          spark->velocityY = -spark->velocityY / 2;
        } else if (x >= 0 && x >> 8 <= 126 && y >= 0) {
          s32 drawX = x >> 8;
          s32 size = (life - 17) / 8;

          if (size <= 0)
            size = 1;
          EFFECT_DRAW_MOTE(draw[i & 1], canvas, sheet, drawX, drawY, size);
        }
      }
    }
    BattleCamera_Shake(16, 16);
    EFFECT_END_FRAME(work);
  }

  EFFECT_RELEASE_BLITTERS();
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

#undef VOLCANO_FRAMES
#undef VOLCANO_ERUPTION
#undef VOLCANO_TRICKLE
#undef VOLCANO_SPARK_POOL
#undef VOLCANO_FLAME_PICTURE_ODD
#undef VOLCANO_FLAME_PICTURE_EVEN
