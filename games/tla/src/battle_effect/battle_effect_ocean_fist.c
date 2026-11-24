/// @file
/// Ocean Fist.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

#define OCEAN_FIST_FRAMES 92
/// The drops stop rising and the target is struck; the column fades from here
/// for OCEAN_FIST_FADE_FRAMES.
#define OCEAN_FIST_STRIKE_FRAME 32
#define OCEAN_FIST_FADE_FRAMES 12
/// The damage is shown, and the camera's long shake gives way to a harder one.
#define OCEAN_FIST_DAMAGE_FRAME 38

/// The column: WhirlBeamSheet's three pictures of a swirling torrent, one a
/// frame in turn, each drawn in up to three bands one above another from its
/// foot up to its top, which rises OCEAN_FIST_COLUMN_RISE lines a frame from
/// the bottom of the view.
#define OCEAN_FIST_COLUMN_WIDTH 96
#define OCEAN_FIST_COLUMN_HEIGHT 64
#define OCEAN_FIST_COLUMN_PICTURES 3
#define OCEAN_FIST_COLUMN_BANDS 3
#define OCEAN_FIST_COLUMN_FRAMES 42
#define OCEAN_FIST_COLUMN_RISE 8
/// The canvas line the column stands on, where the drops leave from; a falling
/// drop is gone below OCEAN_FIST_DROP_FLOOR.
#define OCEAN_FIST_COLUMN_FOOT_Y 108
#define OCEAN_FIST_DROP_FLOOR 104

/// FlashBurstSheet, the spray, decoded after the drops in the map cell buffer.
#define OCEAN_FIST_SPRAY_SHEET (Ram_MapCellBuffer + 16384)
/// The sprays given a countdown, of which the first OCEAN_FIST_SPRAYS burst at
/// the column's foot, one a frame; each shows the sprite sheet's six pictures
/// for three frames apiece.
#define OCEAN_FIST_SPRAY_SLOTS 32
#define OCEAN_FIST_SPRAYS 24
#define OCEAN_FIST_SPRAY_FRAMES (6 * 3)

/// The drops, in the map cell buffer: up to OCEAN_FIST_DROPS_A_FRAME leave the
/// column's foot each frame until the strike, and at OCEAN_FIST_FOUNTAIN_FRAME
/// every free one of the first OCEAN_FIST_FOUNTAIN_DROPS bursts up at once.
#define OCEAN_FIST_DROPS 512
#define OCEAN_FIST_DROPS_A_FRAME 16
#define OCEAN_FIST_FOUNTAIN_DROPS 256
#define OCEAN_FIST_FOUNTAIN_FRAME 28

/// A swirling blue torrent rises from the ground at the target, spray bursting
/// at its foot, while drops shoot up through it and a fountain of them bursts
/// at once. At OCEAN_FIST_STRIKE_FRAME the target is struck and thrown up, the
/// column fades and the drops fall back.
///
/// Abilities: Ocean Fist.
void BattleEffect_RunOceanFist(struct BattleEffectParams *args) {
  u8 *spraySheet = OCEAN_FIST_SPRAY_SHEET;
  void *canvas;
  struct BattleEffectState *state;
  u8 *sprites;
  BlitterPair blitters;
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
  state = Ram_HeapSlots->battleEffect;
  sprites = Ram_HeapSlots->battleSheet;
  BattleEffect_ConfigureEffectDisplay();
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  BattleEffect_BuildCanvasBlitters(0, (void **)blitters);
  Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET, spraySheet, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_WHIRL_BEAM_SHEET, state->sheet, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprites, 0, 0);
  LOAD_BG_PALETTE(RESOURCE_BLUE_FLAME_SHEET);
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  EffectPosition_ProjectBattler(args->targets[0], &target);

  {
    s32 scroll = 0;
    for (frame = 0; frame != OCEAN_FIST_FRAMES;
         scroll += OCEAN_FIST_COLUMN_RISE, frame++) {
      if (frame == 0) {
        EFFECT_FREE_PARTICLES(i, PARTICLES, OCEAN_FIST_DROPS);
        // DEAD: the column's pictures are already decoded; the ROM keeps the
        // second load.
        Resource_LoadAndDecompress(RESOURCE_WHIRL_BEAM_SHEET, state->sheet, 1,
                                   0);
        Sound_PlayFar(SE_CRASH);
        state->shakeFrames = 4;
        halfWidth = OCEAN_FIST_COLUMN_WIDTH / 2;
        width = OCEAN_FIST_COLUMN_WIDTH;
        sheet = OCEAN_FIST_SPRAY_SHEET;
      }
      if (frame == OCEAN_FIST_STRIKE_FRAME) {
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, 0, 8);
        BattleEffect_StartActorMotion(args->targets[0], 1, 0, 0, toFIXED(15),
                                      100);
      }
      if (frame == OCEAN_FIST_DAMAGE_FRAME) {
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
        state->shakeFrames = 16;
      }

      // From the strike, each frame halves the brightness of the canvas
      // shown, so each of the three fades to a sixteenth.
      if (frame >= OCEAN_FIST_STRIKE_FRAME &&
          frame < OCEAN_FIST_STRIKE_FRAME + OCEAN_FIST_FADE_FRAMES) {
        u8 *pixel = state->sheet + (frame % OCEAN_FIST_COLUMN_PICTURES) *
                                       PICTURE_BYTES(OCEAN_FIST_COLUMN);

        for (i = 0; i != PICTURE_BYTES(OCEAN_FIST_COLUMN); i++) {
          s32 value = *pixel;

          *pixel = value >> 1;
          pixel++;
        }
      }

      // The column, its bands scrolling up as its top rises.
      if (frame >= 0 && frame < OCEAN_FIST_COLUMN_FRAMES) {
        s32 top = BATTLE_VIEW_HEIGHT - scroll;
        s32 picture = frame % OCEAN_FIST_COLUMN_PICTURES;
        s32 x = target.x / 2;
        s32 phase = scroll % OCEAN_FIST_COLUMN_HEIGHT;

        if (top < 0) {
          top = 0;
        }
        for (i = 0; i != OCEAN_FIST_COLUMN_BANDS; i++) {
          s32 y = i * OCEAN_FIST_COLUMN_HEIGHT - phase;
          s32 height = OCEAN_FIST_COLUMN_FOOT_Y - y;

          if (height > OCEAN_FIST_COLUMN_HEIGHT) {
            height = OCEAN_FIST_COLUMN_HEIGHT;
          }
          if (height > 0 && y >= top - OCEAN_FIST_COLUMN_HEIGHT) {
            if (y < top) {
              height -= top - y;
              y = top;
            }
            blitters[1](canvas,
                        state->sheet +
                            picture * PICTURE_BYTES(OCEAN_FIST_COLUMN),
                        x - halfWidth, y, width, height);
          }
        }
      }

      // The spray at the column's foot: spray i sets out at frame i + 3 and
      // rises as it burns out.
      if (frame == 0) {
        for (i = 0; i != OCEAN_FIST_SPRAY_SLOTS; i++) {
          state->particles[i].variant = -i - 4;
        }
      }
      for (i = 0, spray = state->particles; i != OCEAN_FIST_SPRAYS;
           i++, spray++) {
        if ((u32)spray->variant < OCEAN_FIST_SPRAY_FRAMES) {
          blitters[0](canvas,
                      (u8 *)((spray->variant / 3) * PICTURE_BYTES(FLASH_BURST) +
                             (s32)sheet),
                      FIXED_INTEGER_PART(spray->x) - FLASH_BURST_WIDTH / 2,
                      FIXED_INTEGER_PART(spray->y) - FLASH_BURST_HEIGHT / 2,
                      FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
          Particle_AdvanceWithGravity2D(spray, 62, -toFIXED(1) / 8);
        }
        if (++spray->variant == 0) {
          spray->x = INTtoFIXED((Random16() & 31) + target.x / 2 - 16);
          spray->y = INTtoFIXED((Random16() & 7) + 88);
          spray->velocityX = ((Random16() & 255) - 128) << 11;
          spray->velocityY = 0;
        }
      }

      // The fountain: free drops set out in an oval about canvas (32, 96)
      // and fly up and out.
      if (frame == OCEAN_FIST_FOUNTAIN_FRAME) {
        struct Particle *fountain;

        for (i = 0, fountain = drops; i != OCEAN_FIST_FOUNTAIN_DROPS;
             i++, fountain++) {
          if (fountain->variant == -1) {
            s32 radius = Random16() & 63;
            s32 angle = Random16() & TRIG_ANGLE_MASK;

            fountain->x = ((Trig_Sin(angle) * radius) >> 3) + toFIXED(32);
            fountain->y = ((Trig_Cos(angle) * radius) >> 2) + toFIXED(96);
            fountain->velocityX = ((Random16() & 63) - 32) << 14;
            fountain->velocityY = (-(Random16() & 63) - 8) << 13;
            fountain->variant = 0;
          }
        }
      }

      // Until the strike, drops leave the column's foot and are pulled up,
      // each by one of gOceanFist_DropGravity's four; then they fall.
      if (frame >= 0 && frame < OCEAN_FIST_STRIKE_FRAME) {
        s32 spawned = 0;

        for (i = 0, drop = drops; i != OCEAN_FIST_DROPS; i++, drop++) {
          if (drop->variant == -1) {
            drop->x = INTtoFIXED((Random16() & 31) + target.x / 2 - 16);
            drop->y = toFIXED(OCEAN_FIST_COLUMN_FOOT_Y);
            drop->velocityX = ((Random16() & 63) - 32) << 14;
            drop->velocityY = (-(Random16() & 63) - 8) << 13;
            drop->variant = 0;
            if (++spawned == OCEAN_FIST_DROPS_A_FRAME) {
              break;
            }
          }
        }
      }
      for (i = 0, drop = drops; i != OCEAN_FIST_DROPS; i++, drop++) {
        if (drop->variant >= 0) {
          s32 size = i % 3 + 3;

          EFFECT_DRAW_MOTE(blitters[i & 1], canvas, sprites,
                           FIXED_INTEGER_PART(drop->x),
                           FIXED_INTEGER_PART(drop->y), size);
          if (frame >= OCEAN_FIST_STRIKE_FRAME) {
            Particle_AdvanceWithGravity2D(drop, 62, toFIXED(1) / 4);
          } else {
            Particle_AdvanceWithGravity2D(drop, 62,
                                          gOceanFist_DropGravity[i & 3]);
          }
          drop->variant++;
          if (drop->velocityY > 0 &&
              FIXEDtoINT(drop->y) > OCEAN_FIST_DROP_FLOOR) {
            drop->variant = -1;
          }
          // BUG: the drop's 16.16 y is compared with its size in whole
          // pixels, so a rising drop goes as it crosses the canvas's top line
          // rather than once it is past it.
#ifdef BUGFIX
          if (drop->y < toFIXED(-size)) {
#else
          if (drop->y < -size) {
#endif
            drop->variant = -1;
          }
        }
      }
      if (frame < OCEAN_FIST_DAMAGE_FRAME) {
        state->shakeFrames = 16;
      }
      if (frame <= 7) {
        BattleCamera_Shake(8, 8);
      } else if (frame < OCEAN_FIST_DAMAGE_FRAME) {
        BattleCamera_Shake(4, 4);
      } else {
        BattleCamera_Shake(8, 8);
      }
      EFFECT_END_FRAME(state);
    }
  }
  EFFECT_FINISH_CANVAS();
}
#undef OCEAN_FIST_FRAMES
#undef OCEAN_FIST_STRIKE_FRAME
#undef OCEAN_FIST_FADE_FRAMES
#undef OCEAN_FIST_DAMAGE_FRAME
#undef OCEAN_FIST_COLUMN_WIDTH
#undef OCEAN_FIST_COLUMN_HEIGHT
#undef OCEAN_FIST_COLUMN_PICTURES
#undef OCEAN_FIST_COLUMN_BANDS
#undef OCEAN_FIST_COLUMN_FRAMES
#undef OCEAN_FIST_COLUMN_RISE
#undef OCEAN_FIST_COLUMN_FOOT_Y
#undef OCEAN_FIST_DROP_FLOOR
#undef OCEAN_FIST_SPRAY_SHEET
#undef OCEAN_FIST_SPRAY_SLOTS
#undef OCEAN_FIST_SPRAYS
#undef OCEAN_FIST_SPRAY_FRAMES
#undef OCEAN_FIST_DROPS
#undef OCEAN_FIST_DROPS_A_FRAME
#undef OCEAN_FIST_FOUNTAIN_DROPS
#undef OCEAN_FIST_FOUNTAIN_FRAME
