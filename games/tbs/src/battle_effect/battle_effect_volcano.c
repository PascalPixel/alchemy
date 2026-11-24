/// @file
/// Volcano.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

#define VOLCANO_FRAMES 96
#define VOLCANO_MOST_PILLARS 4 ///< the strongest variant's
/// The frames the pillars burn, and the strongest variant's camera turns.
#define VOLCANO_PILLAR_FRAMES 64
/// The camera's turn each of those frames: three sixteenths of a turn in all.
#define VOLCANO_CAMERA_SPIN (TRIG_TURN * 3 / 16 / VOLCANO_PILLAR_FRAMES)
#define VOLCANO_ERUPTION 200 ///< sparks thrown as a pillar starts
#define VOLCANO_TRICKLE 4    ///< and each frame after
#define VOLCANO_SPARK_POOL PARTICLE_POOL_COUNT ///< sparks in PARTICLES
/// The flame on odd frames, then on even frames, after the 34x104 column.
#define VOLCANO_FLAME_PICTURE_ODD PICTURE_BYTES(FIRE_PILLAR)
#define VOLCANO_FLAME_PICTURE_EVEN                                             \
  (VOLCANO_FLAME_PICTURE_ODD + PICTURE_BYTES(FIRE_PILLAR_FLAME))
/// How much faster a spark falls each frame: a sixteenth of a pixel a frame.
#define VOLCANO_SPARK_GRAVITY (Q_8_8(1) / 16)
/// The mote size a spark is drawn at: a size smaller every 8 frames of its
/// life, and the smallest for its last 32, which is all of a trickled spark's.
#define VOLCANO_SPARK_SIZE(life) (((life) - 17) / 8)

/// One, two or four columns of fire rise among the targets, each erupting in a
/// fountain of sparks and knocking the targets back.
///
/// Abilities: Volcano, Eruption, Pyroclasm, Lava Shower, Molten Bath, Magma
/// Storm.
void BattleEffect_RunVolcano(struct BattleEffectParams *args) {
  struct BattleEffectState *state;
  void *canvas;
  s32 frame;
  s32 pillar;
  u8 *sheet;
  struct ScreenPoint screen;
  BlitterPair draw;
  struct Actor *actor;
  s32 i;

  state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  sheet = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  state->args = args;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  Resource_LoadAndDecompress(RESOURCE_FIRE_PILLAR_SHEET_A, state->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sheet, 0, 0);
  if (state->args->variant == EFFECT_VARIANT_BASIC)
    Iwram_CopyWords(BG_PLTT,
                    Resource_GetTableEntry(RESOURCE_VOLCANO_RED_PALETTE),
                    SHEET_PALETTE_BYTES);
  else if (state->args->variant == EFFECT_VARIANT_STRONGEST)
    Iwram_CopyWords(BG_PLTT,
                    Resource_GetTableEntry(RESOURCE_VOLCANO_ORANGE_PALETTE),
                    SHEET_PALETTE_BYTES);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_LIGHTEN);
  draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);

  for (i = 0; i != VOLCANO_SPARK_POOL; i++)
    PARTICLES[i].variant = 0;

  actor = BattleObject_GetFar(state->args->targets[0])->actor;
  // Each pillar stands 72 to 87 pixels out on the first target's side of the
  // field, at its own depth.
  for (i = 0; i != VOLCANO_MOST_PILLARS; i++) {
    struct Particle *foot = &state->particles[i];
    s32 x;

    x = INTtoFIXED((Random16() & 15) + 72);
    foot->y = 0;
    foot->x = x;
    foot->z = INTtoFIXED(gVolcano_Depths[state->args->variant][i]);
    if (actor->x.fixed < 0)
      foot->x = -x;
  }

  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  for (frame = 0; frame != VOLCANO_FRAMES; frame++) {
    struct BattleCamera *camera = gBattleCamera;

    if (state->args->variant == EFFECT_VARIANT_STRONGEST &&
        frame < VOLCANO_PILLAR_FRAMES) {
      if (state->args->side == EFFECT_SIDE_ENEMIES)
        camera->yaw = camera->yaw + VOLCANO_CAMERA_SPIN;
      else
        camera->yaw = (s16)camera->yaw - VOLCANO_CAMERA_SPIN;
    }
    if (frame == 16)
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    EFFECT_VIEW_FROM_CAMERA(camera);

    if (frame < VOLCANO_PILLAR_FRAMES) {
      for (pillar = 0; pillar != gVolcano_Counts[state->args->variant];
           pillar++) {
        Projection_Point(&state->particles[pillar], &screen);
        screen.x /= 2;
        screen.y -= 8;
        if (frame == gVolcano_StartFrames[pillar])
          Sound_PlayFar(SE_CRASH);
        // The column scrolls up over its flickering flame.
        if (frame >= gVolcano_StartFrames[pillar] + 4) {
          s32 scroll = (frame * 16 + pillar * 25) % FIRE_PILLAR_HEIGHT;

          draw[pillar & 1](canvas, state->sheet,
                           screen.x - FIRE_PILLAR_WIDTH / 2,
                           screen.y - scroll - FIRE_PILLAR_HEIGHT,
                           FIRE_PILLAR_WIDTH, FIRE_PILLAR_HEIGHT);
          draw[pillar & 1](canvas, state->sheet,
                           screen.x - FIRE_PILLAR_WIDTH / 2, screen.y - scroll,
                           FIRE_PILLAR_WIDTH, scroll);
          if (frame & 1) {
            draw[0](canvas, state->sheet + VOLCANO_FLAME_PICTURE_ODD,
                    screen.x - FIRE_PILLAR_FLAME_WIDTH, screen.y - 24,
                    FIRE_PILLAR_FLAME_WIDTH, FIRE_PILLAR_FLAME_HEIGHT);
            draw[1](canvas, state->sheet + VOLCANO_FLAME_PICTURE_ODD, screen.x,
                    screen.y - 24, FIRE_PILLAR_FLAME_WIDTH,
                    FIRE_PILLAR_FLAME_HEIGHT);
          } else {
            draw[0](canvas, state->sheet + VOLCANO_FLAME_PICTURE_EVEN,
                    screen.x - FIRE_PILLAR_FLAME_WIDTH, screen.y - 24,
                    FIRE_PILLAR_FLAME_WIDTH, FIRE_PILLAR_FLAME_HEIGHT);
            draw[1](canvas, state->sheet + VOLCANO_FLAME_PICTURE_EVEN, screen.x,
                    screen.y - 24, FIRE_PILLAR_FLAME_WIDTH,
                    FIRE_PILLAR_FLAME_HEIGHT);
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
              angle = (Random16() & 0x7fff) - DEGtoANG(90);
              SET_POSITION(spark, screen.x << 8, (screen.y << 8) + (16 << 8),
                           FIXEDtoINT_TWICE(Trig_Sin(angle) * speed));
              spark->velocityY =
                  FIXEDtoINT_TWICE(-(Trig_Cos(angle) * speed * 2));
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
          state->shakeFrames = 2;
          EFFECT_FOR_EACH_TARGET(i, state->args) {
            BattleEffect_TintBattler(state->args->targets[i],
                                     SPRITE_COLOR_RED_TINT, POSE_DAMAGE, i, 8);
            BattleMotion_ApplyKnockbackFar(state->args->targets[i],
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
        FIXED x;
        s32 y;
        s32 drawY;

        spark->variant = life - 1;
        x = spark->x += spark->z;
        y = spark->y += spark->velocityY;
        spark->z = spark->z * 60 / DAMPING_SCALE;
        spark->velocityY =
            spark->velocityY * 60 / DAMPING_SCALE - VOLCANO_SPARK_GRAVITY;
        drawY = y / 256;
        // A spark bounces at half its speed off the foot of the battle view.
        if (drawY > BATTLE_VIEW_HEIGHT) {
          spark->velocityY = -spark->velocityY / 2;
        } else if (x >= 0 && x >> 8 <= 126 && y >= 0) {
          s32 drawX = x >> 8;
          s32 size = VOLCANO_SPARK_SIZE(life);

          if (size <= 0)
            size = 1;
          EFFECT_DRAW_MOTE(draw[i & 1], canvas, sheet, drawX, drawY, size);
        }
      }
    }
    BattleCamera_Shake(16, 16);
    EFFECT_END_FRAME(state);
  }

  EFFECT_RELEASE_BLITTERS();
  Task_Remove(BattleEffect_TransferCanvas);
  BattleEffect_EndCanvasLayer();
}

#undef VOLCANO_FRAMES
#undef VOLCANO_MOST_PILLARS
#undef VOLCANO_PILLAR_FRAMES
#undef VOLCANO_CAMERA_SPIN
#undef VOLCANO_SPARK_GRAVITY
#undef VOLCANO_SPARK_SIZE
#undef VOLCANO_ERUPTION
#undef VOLCANO_TRICKLE
#undef VOLCANO_SPARK_POOL
#undef VOLCANO_FLAME_PICTURE_ODD
#undef VOLCANO_FLAME_PICTURE_EVEN
