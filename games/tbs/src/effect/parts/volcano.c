/// @file
/// Part of effect.c, the battle effect object, compiled only through it.
/// Volcano.

#include "../effect_internal.h"

// The fire pillars stand at random distances on the targets' side and scroll
// a column of fire up with a flickering flame at its foot. Each erupts with a
// fountain of 200 sparks and keeps spitting 4 a frame into one pool of 1024
// in PARTICLES. The sparks move in 24.8 fixed point, with z holding the speed
// across, and bounce off y = 120; a spark's variant is its remaining life, 0
// when idle.

#define VOLCANO_FRAMES 96
#define VOLCANO_ERUPTION 200            ///< sparks thrown as a pillar starts
#define VOLCANO_TRICKLE 4               ///< and each frame after
#define VOLCANO_SPARK_POOL 1024         ///< sparks in PARTICLES
#define VOLCANO_FLAME_IMAGE_ODD 0xdd0   ///< the flame on odd frames
#define VOLCANO_FLAME_IMAGE_EVEN 0x10b4 ///< the flame on even frames

/// Abilities: Volcano, Eruption, Pyroclasm, Lava Shower, Molten Bath, Magma
/// Storm (kind 13).
///
/// One, two or four columns of fire (gVolcano_Counts by effect->variant) rise
/// near the far edge on the targets' side, at the depths of gVolcano_Depths,
/// starting at gVolcano_StartFrames; each erupts with a fountain of 200 sparks
/// and keeps spitting more, and each eruption knocks the targets back. The
/// battle shows the damage on frame 16. Variant 0 is red, 2 orange; variant 2
/// also swings the camera round.
void BattleFx_RunVolcano(struct BattleEffectArgument *effect) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 pillar;
  u8 *sheet;
  struct EffectPosition screen;
  BlitterPair draw;
  struct MotionObject *object;
  struct BattleCamera *camera;
  s32 i;

  slots = &gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
  cursor = slots;
  EFFECT_GET_WORK_AND_CANVAS(cursor, work, canvas);
  sheet = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(1);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  Resource_LoadAndDecompress((s32)&ResourceId_FirePillarSheetA, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  if (work->effect->variant == 0)
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_RedPaletteB), 128);
  else if (work->effect->variant == 2)
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                    128);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER, EFFECT_CLIP_BOUNDS, COMPOSITE_MAX);
  draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
  EFFECT_LOAD_BLITTER(HEAP_SLOT_BLITTER_ALTERNATE,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_MAX);
  draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];

  for (i = 0; i != VOLCANO_SPARK_POOL; i++)
    PARTICLES[i].variant = 0;

  // The pillars stand 72 to 87 pixels out on the first target's side.
  object = GetBattleObjectSlotFar(work->effect->actors[0])->object;
  for (i = 0; i != 4; i++) {
    struct EffectStep *base = &work->particles[i];
    s32 x;

    x = ((Random16() & 15) + 72) << 16;
    base->y = 0;
    base->x = x;
    base->z = gVolcano_Depths[work->effect->variant][i] << 16;
    if (object->x < 0)
      base->x = -x;
  }

  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE,
                        50); // trails keep half a frame

  for (frame = 0; frame != VOLCANO_FRAMES; frame++) {
    camera = gCameraWork;
    if (work->effect->variant == 2 && frame < 64) {
      if (work->effect->side == 0)
        camera->yaw = camera->yaw + 192;
      else
        camera->yaw = (s16)camera->yaw - 192;
    }
    if (frame == 16)
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    EFFECT_VIEW_FROM_CAMERA(camera);

    if (frame < 64) {
      for (pillar = 0; pillar != gVolcano_Counts[work->effect->variant];
           pillar++) {
        EffectPosition_ProjectPoint(&work->particles[pillar], &screen);
        screen.x /= 2;
        screen.y -= 8;
        if (frame == gVolcano_StartFrames[pillar])
          AudioCommand_PlayFar(SE_145);
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
        // Sparks: 200 at the start, then 4 a frame from 16 frames on.
        if (frame == gVolcano_StartFrames[pillar] ||
            frame >= gVolcano_StartFrames[pillar] + 16) {
          s32 spawned = 0;

          for (i = 0; i != VOLCANO_SPARK_POOL; i++) {
            struct EffectStep *spark = &PARTICLES[i];

            if (spark->variant == 0) {
              s32 speed;
              s32 angle;

              speed = (Random16() & 0x3ff) + 32;
              angle = (Random16() & 0x7fff) - 0x4000;
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
          EFFECT_FOR_EACH_TARGET(i, work->effect) {
            ObjectGroup_UpdateMembers(work->effect->actors[i],
                                      SPRITE_COLOR_RED_TINT, POSE_DAMAGE, i, 8);
            BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 1);
          }
        }
      }
    }

    // The sparks, in 24.8 fixed point (z holds the speed across): slow to
    // 60/64 a frame, fall, and bounce off y 120 at half speed.
    for (i = 0; i != VOLCANO_SPARK_POOL; i++) {
      struct EffectStep *spark = &PARTICLES[i];
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
    Camera_ApplyShake(16, 16);
    EFFECT_END_FRAME(work);
  }

  EFFECT_RELEASE_BLITTERS();
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  BattleFx_EndCanvasLayer();
}

#undef VOLCANO_FRAMES
#undef VOLCANO_ERUPTION
#undef VOLCANO_TRICKLE
#undef VOLCANO_SPARK_POOL
#undef VOLCANO_FLAME_IMAGE_ODD
#undef VOLCANO_FLAME_IMAGE_EVEN
