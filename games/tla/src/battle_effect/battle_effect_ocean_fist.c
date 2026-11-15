/// @file
/// Ocean Fist.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

#define BLITTER ((DrawRectangle)Ram_HeapSlots->ramCode)
#define OCEAN_FIST_FRAMES 92
#define OCEAN_DROPS 512

/// A column of water (three pictures of the work's sheet, each halved in
/// brightness from frame 32 to 43) rises at the target and is drawn top-down
/// for 42 frames; spray bursts at its foot, and drops of the map cell
/// buffer's 512 fly up from the target for the first 32 frames, with a
/// fountain of 256 at frame 28. The target is struck at frame 32 and pushed
/// back.
///
/// Abilities: Ocean Fist.
void BattleEffect_RunOceanFist(struct BattleEffectParams *args) {
  u8 *spraySheet = gData_02014000;
  void *canvas;
  struct BattleEffectWork *work;
  u8 *sprites;
  DrawRectangle blitters[2];
  struct ScreenPoint target;
  struct Particle *drops = (struct Particle *)gMapGridCellBuffer;
  struct Particle *drop;
  struct Particle *spray;
  s32 frame;
  s32 halfWidth;
  s32 width;
  u8 *sheet;
  s32 i;

  canvas = Ram_HeapSlots->battleCanvas;
  work = Ram_HeapSlots->battleEffect;
  sprites = Ram_HeapSlots->battleSheet;
  BattleEffect_ConfigureEffectDisplay();
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  BattleEffect_FetchRectangleBlitters(0, (void **)blitters);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, spraySheet, 1,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_WhirlBeamSheet, work->sheet, 1,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry((s32)&ResourceId_BlueFlameSheet),
                       128);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  EffectPosition_ProjectBattler(args->targets[0], &target);

  {
    s32 scroll = 0;
    for (frame = 0; frame != OCEAN_FIST_FRAMES; scroll += 8, frame++) {
      if (frame == 0) {
        s32 *life = Data_02010018;

        for (i = 0; i != OCEAN_DROPS; i++) {
          *life = -1;
          life += 7;
        }
        Resource_LoadAndDecompress((s32)&ResourceId_WhirlBeamSheet, work->sheet,
                                   1, 0);
        Sound_PlayFar(SE_CRASH);
        work->shakeFrames = 4;
        halfWidth = 48;
        width = 96;
        sheet = gData_02014000;
      }
      if (frame == 32) {
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, 0, 8);
        BattleEffect_StartActorMotion(args->targets[0], 1, 0, 0, 0xf0000, 100);
      }
      if (frame == 38) {
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
        work->shakeFrames = 16;
      }

      // Frames 32 to 43: each in turn, the column's pictures dim.
      if ((u32)(frame - 32) <= 11) {
        u8 *pixel = work->sheet + (frame % 3) * 3 * 0x800;

        for (i = 0; i != 0x1800; i++) {
          s32 value = *pixel;

          *pixel = value >> 1;
          pixel++;
        }
      }

      // The column, three 64-line bands scrolling up from line 120.
      if (frame >= 0 && frame <= 41) {
        s32 top = 120 - scroll;
        s32 picture = frame % 3;
        s32 x = target.x / 2;
        s32 phase = scroll % 64;

        if (top < 0) {
          top = 0;
        }
        for (i = 0; i != 3; i++) {
          s32 y = i * 64 - phase;
          s32 height = 108 - y;

          if (height > 64) {
            height = 64;
          }
          if (height > 0 && y >= top - 64) {
            if (y < top) {
              height -= top - y;
              y = top;
            }
            blitters[1](canvas, work->sheet + picture * 3 * 0x800,
                        x - halfWidth, y, width, height);
          }
        }
      }

      // The spray at the column's foot.
      if (frame == 0) {
        for (i = 0; i != 32; i++) {
          work->particles[i].variant = -i - 4;
        }
      }
      for (i = 0, spray = work->particles; i != 24; i++, spray++) {
        if ((u32)spray->variant <= 17) {
          blitters[0](canvas, (u8 *)(((spray->variant / 3) << 11) + (s32)sheet),
                      Q_16_16_UPPER_HALF(spray->x) - 16,
                      Q_16_16_UPPER_HALF(spray->y) - 32, 32, 64);
          BattleEffectKernels_IntegrateVector2(spray, 62, -px(1) / 8);
        }
        if (++spray->variant == 0) {
          spray->x = px((Random16() & 31) + target.x / 2 - 16);
          spray->y = px((Random16() & 7) + 88);
          spray->velocityX = ((Random16() & 255) - 128) << 11;
          spray->velocityY = 0;
        }
      }

      // The drops: a fountain at frame 28, sixteen more a frame until 31.
      if (frame == 28) {
        struct Particle *fountain;

        for (i = 0, fountain = drops; i != 256; i++, fountain++) {
          if (fountain->variant == -1) {
            s32 radius = Random16() & 63;
            s32 angle = Random16() & 0xffff;

            fountain->x = ((Trig_Sin(angle) * radius) >> 3) + 0x200000;
            fountain->y = ((Trig_Cos(angle) * radius) >> 2) + 0x600000;
            fountain->velocityX = ((Random16() & 63) - 32) << 14;
            fountain->velocityY = (-(Random16() & 63) - 8) << 13;
            fountain->variant = 0;
          }
        }
      }
      if (frame >= 0 && frame <= 31) {
        s32 spawned = 0;

        for (i = 0, drop = drops; i != OCEAN_DROPS; i++, drop++) {
          if (drop->variant == -1) {
            drop->x = px((Random16() & 31) + target.x / 2 - 16);
            drop->y = px(108);
            drop->velocityX = ((Random16() & 63) - 32) << 14;
            drop->velocityY = (-(Random16() & 63) - 8) << 13;
            drop->variant = 0;
            if (++spawned == 16) {
              break;
            }
          }
        }
      }
      for (i = 0, drop = drops; i != OCEAN_DROPS; i++, drop++) {
        if (drop->variant >= 0) {
          s32 size = i % 3 + 3;

          blitters[i & 1](canvas,
                          sprites + gBattleEffect_MotePictures[size - 1],
                          Q_16_16_UPPER_HALF(drop->x) - size / 2,
                          Q_16_16_UPPER_HALF(drop->y) - size, size, size * 2);
          if (frame > 31) {
            BattleEffectKernels_IntegrateVector2(drop, 62, px(1) / 4);
          } else {
            BattleEffectKernels_IntegrateVector2(drop, 62,
                                                 gOceanFist_DropGravity[i & 3]);
          }
          drop->variant++;
          if (drop->velocityY > 0 && (drop->y >> 16) > 104) {
            drop->variant = -1;
          }
          if (drop->y < -size) {
            drop->variant = -1;
          }
        }
      }
      if (frame <= 37) {
        work->shakeFrames = 16;
      }
      if (frame <= 7) {
        BattleCamera_Shake(8, 8);
      } else if (frame <= 37) {
        BattleCamera_Shake(4, 4);
      } else {
        BattleCamera_Shake(8, 8);
      }
      EFFECT_END_FRAME(work);
    }
  }
  EFFECT_FINISH_CANVAS();
}
#undef BLITTER
#undef OCEAN_FIST_FRAMES
#undef OCEAN_DROPS
