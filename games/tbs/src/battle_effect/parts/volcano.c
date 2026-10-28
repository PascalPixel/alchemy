/// @file
/// Volcano.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

// The fire pillars stand at random distances on the targets' side and scroll
// a column of fire up with a flickering flame at its foot. Each erupts with a
// fountain of 200 sparks and keeps spitting 4 a frame into one pool of 1024
// in PARTICLES. The sparks move in 24.8 fixed point, with z holding the speed
// across, and drift upwards; a spark's variant is its remaining life, 0 when
// idle.

#define VOLCANO_FRAMES 96
#define VOLCANO_ERUPTION 200            ///< sparks thrown as a pillar starts
#define VOLCANO_TRICKLE 4               ///< and each frame after
#define VOLCANO_SPARK_POOL 1024         ///< sparks in PARTICLES
#define VOLCANO_FLAME_IMAGE_ODD 0xdd0   ///< the flame on odd frames
#define VOLCANO_FLAME_IMAGE_EVEN 0x10b4 ///< the flame on even frames

/// One, two or four columns of fire (gVolcano_Counts by args->variant) rise
/// near the far edge on the targets' side, at the depths of gVolcano_Depths,
/// starting at gVolcano_StartFrames; each erupts with a fountain of 200 sparks
/// and keeps spitting more, and each eruption knocks the targets back.
///
/// Abilities: Volcano, Eruption, Pyroclasm, Lava Shower, Molten Bath, Magma
/// Storm (BATTLE_EFFECT_VOLCANO).
///
/// The battle shows the damage on frame 16. Volcano and Lava Shower are red,
/// Pyroclasm and Magma Storm orange, and those two also swing the camera round
/// over the first 64 frames.
///
/// work->particles 0 to 3 are the pillars' feet, battle positions in 16.16;
/// only gVolcano_Counts' first one, two or four are drawn.
/// Sets REG_BLDALPHA, loads the red or orange palette into the first 64
/// background colours (Eruption and Molten Bath keep the fire pillar sheet's),
/// turns the camera's yaw (the strongest variant) and sets work->shakeFrames
/// at each eruption.
void BattleEffect_RunVolcano(struct BattleEffectParams *args) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 pillar;
  u8 *sheet;
  struct ScreenPoint screen;
  BlitterPair draw;
  struct Actor *actor;
  struct BattleCamera *camera;
  s32 i;

  slots = &gHeapSlots[HEAP_SLOT_BATTLE_EFFECT];
  cursor = slots;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  sheet = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->args = args;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  Resource_LoadAndDecompress((s32)&ResourceId_FirePillarSheetA, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  if (work->args->variant == EFFECT_VARIANT_BASIC)
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_RedPaletteB), 128);
  else if (work->args->variant == EFFECT_VARIANT_STRONGEST)
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                    128);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  draw[0] = (DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE];
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_LIGHTEN);
  draw[1] = (DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE_2];

  for (i = 0; i != VOLCANO_SPARK_POOL; i++)
    PARTICLES[i].variant = 0;

  // The pillars stand 72 to 87 pixels out on the first target's side.
  actor = GetBattleObjectSlotFar(work->args->targets[0])->actor;
  for (i = 0; i != 4; i++) {
    struct Particle *base = &work->particles[i];
    s32 x;

    x = ((Random16() & 15) + 72) << 16;
    base->y = 0;
    base->x = x;
    base->z = gVolcano_Depths[work->args->variant][i] << 16;
    if (actor->x.fixed < 0)
      base->x = -x;
  }

  // Trails keep half of each pixel a frame.
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  for (frame = 0; frame != VOLCANO_FRAMES; frame++) {
    camera = gBattleCamera;
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
        // From 4 frames after it starts: the column, scrolling up 16
        // pixels a frame, and a flickering flame at its foot.
        if (frame >= gVolcano_StartFrames[pillar] + 4) {
          s32 scroll = (frame * 16 + pillar * 25) % 104;

          draw[pillar & 1](canvas, work->sheet, screen.x - 17,
                           screen.y - scroll - 104, 34, 104);
          draw[pillar & 1](canvas, work->sheet, screen.x - 17,
                           screen.y - scroll, 34, scroll);
          if (frame & 1) {
            draw[0](canvas, work->sheet + VOLCANO_FLAME_IMAGE_ODD,
                    screen.x - 20, screen.y - 24, 20, 37);
            draw[1](canvas, work->sheet + VOLCANO_FLAME_IMAGE_ODD, screen.x,
                    screen.y - 24, 20, 37);
          } else {
            draw[0](canvas, work->sheet + VOLCANO_FLAME_IMAGE_EVEN,
                    screen.x - 20, screen.y - 24, 20, 37);
            draw[1](canvas, work->sheet + VOLCANO_FLAME_IMAGE_EVEN, screen.x,
                    screen.y - 24, 20, 37);
          }
        }
        // Sparks: 200 at the start, living 48 to 55 frames, then 4 a frame
        // from 16 frames on, living 24 to 31; anywhere in the upper half
        // circle, twice as hard up as across: up to about 8 pixels a frame
        // across and 16 up. They start 16 pixels below the column's foot.
        if (frame == gVolcano_StartFrames[pillar] ||
            frame >= gVolcano_StartFrames[pillar] + 16) {
          s32 spawned = 0;

          for (i = 0; i != VOLCANO_SPARK_POOL; i++) {
            struct Particle *spark = &PARTICLES[i];

            if (spark->variant == 0) {
              s32 speed;
              s32 angle;

              speed = (Random16() & 0x3ff) + 32;
              angle = (Random16() & 0x7fff) - EFFECT_TURN / 4;
              SET_POSITION(spark, screen.x << 8, (screen.y << 8) + 0x1000,
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

    // The sparks, in 24.8 fixed point (z holds the speed across): slow to
    // 60/64 a frame while pulled up by 1/16 pixel a frame, so they settle
    // at a rise of 1 pixel a frame; one below y 120 bounces back at half
    // speed instead of being drawn. A spark shrinks with its life.
    for (i = 0; i != VOLCANO_SPARK_POOL; i++) {
      struct Particle *spark = &PARTICLES[i];
      s32 life = spark->variant;

      if (life > 0) {
        s32 x;
        s32 y;
        s32 py;

        spark->variant = life - 1;
        x = spark->x += spark->z;
        y = spark->y += spark->velocityY;
        spark->z = spark->z * 60 / 64;
        spark->velocityY = spark->velocityY * 60 / 64 - 16;
        py = y / 256;
        if (py > 120) {
          spark->velocityY = -spark->velocityY / 2;
        } else if (x >= 0 && x >> 8 <= 126 && y >= 0) {
          s32 px = x >> 8;
          s32 size = (life - 17) / 8;

          if (size <= 0)
            size = 1;
          EFFECT_DRAW_MOTE(draw[i & 1], canvas, sheet, px, py, size);
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
#undef VOLCANO_FLAME_IMAGE_ODD
#undef VOLCANO_FLAME_IMAGE_EVEN
