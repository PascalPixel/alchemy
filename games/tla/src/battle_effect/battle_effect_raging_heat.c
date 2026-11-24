/// @file
/// Raging Heat and Juggle.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// The ring of fire's texture, 128x128, after the quarter of the circle of
/// runes (64x64) that the pillar's picture then replaces.
#define FIRE_RING_PIXELS (Ram_MapCellBuffer + 64 * 64)
/// The embers, after the ring of fire's texture.
#define EMBERS ((struct Particle *)(FIRE_RING_PIXELS + 128 * 128))
#define RAGING_HEAT_EMBER_COUNT 256

#define RAGING_HEAT_FRAMES 96
/// The pillar starts to fall and the ring of fire to spread.
#define RAGING_HEAT_PILLAR_FRAME 56
/// FlashBurstSheet's pictures, after the circle of runes (128x128) in the
/// effect's sprite sheet.
#define RAGING_HEAT_FLASH_BURST_PICTURES (128 * 128)
/// The bursts the pillar sets out where it lands; the variant draws the first
/// RAGING_HEAT_BURSTS of them.
#define RAGING_HEAT_BURSTS_SET_OUT 32
/// A burst shows each of FlashBurstSheet's six pictures for four frames.
#define RAGING_HEAT_BURST_FRAMES 24
/// The rings' tilt toward the viewer.
#define RAGING_HEAT_TILT 2000
/// Until this frame the circle of runes is tipped further and rights itself.
#define RUNE_CIRCLE_RIGHTED_FRAME 48

/// gRagingHeat_Styles[variant]: four bytes for each strength.
enum {
  RAGING_HEAT_BURSTS,    ///< bursts of fire drawn where the pillar lands
  RAGING_HEAT_SPREAD,    ///< how far apart they fly
  RAGING_HEAT_RING_SIZE, ///< the rings' size, in 64ths of their textures'
  RAGING_HEAT_EMBERS     ///< embers that fly from the landing
};

/// A circle of runes spins over the targets' middle and swells; from frame 56
/// a pillar of flame (two mirrored halves) falls from it onto them and bursts
/// into fire and embers where it lands, under a ring of fire that spreads and
/// fades.
///
/// Abilities: Raging Heat.
void BattleEffect_RunRagingHeat(struct BattleEffectParams *args) {
  struct BattleEffectState *state;
  void *canvas;
  s32 frame;
  BlitterPair draw;
  u8 *sprites;
  struct ScreenPoint targets[8];
  struct ModelTexture texture;
  u8 *buffer;
  s32 centerX;
  s32 baseY;
  struct RenderModel *model;
  struct Particle *burst;
  struct Particle *particle;
  s32 i;

  buffer = Ram_MapCellBuffer;
  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sprites = Ram_HeapSlots->battleSheet;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  REG_BLDALPHA = BLEND_ALPHA(16, 10);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                      EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_ADDITIVE);
  draw[1] = (DrawRectangle)Ram_HeapSlots->ramCodePaired;
  for (i = 0; i != args->targetCount; i++) {
    EffectPosition_ProjectScaledBattler(args->targets[i], &targets[i]);
  }
  centerX =
      targets[0].x + (targets[args->targetCount - 1].x - targets[0].x) / 2;
  baseY = targets[0].y - 48;
  Resource_LoadAndDecompress(RESOURCE_CORNER_RUNE_B_SHEET, buffer, 1, 1);
  Picture_MirrorIntoQuadrants(buffer, state->sheet, 64, 64);
  Resource_LoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprites, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET,
                             state->sheet + RAGING_HEAT_FLASH_BURST_PICTURES, 1,
                             0);
  Resource_LoadAndDecompress(RESOURCE_PINK_BURST_SHEET, buffer, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE, FIRE_RING_PIXELS, 0,
                             0);
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);

  for (frame = 0; frame != RAGING_HEAT_FRAMES; frame++) {
    if (frame == 0) {
      for (i = 0; i != ARRAY_COUNT(state->particles); i++) {
        state->particles[i].variant = RAGING_HEAT_BURST_FRAMES;
      }
      EFFECT_FREE_PARTICLES(i, EMBERS, RAGING_HEAT_EMBER_COUNT);
      Sound_PlayFar(SE_SPIN);
    }
    // The colours turn to the orange ramp over the 16 frames before the fall.
    if ((u32)(frame - (RAGING_HEAT_PILLAR_FRAME - 16)) < 16) {
      BattleEffect_StepPaletteToResource(RESOURCE_ORANGE_PALETTE_B);
    }
    if (frame == RAGING_HEAT_PILLAR_FRAME) {
      Sound_PlayFar(SE_CLATTER);
    }

    // The pillar falls eight lines a frame and bursts where it lands.
    if (frame >= RAGING_HEAT_PILLAR_FRAME) {
      s32 y = baseY + frame * 8 - 480;

      if ((u32)(y - 52) <= 7) {
        for (i = 0, burst = state->particles; i != RAGING_HEAT_BURSTS_SET_OUT;
             i++, burst++) {
          u32 random;
          u32 spread;

          burst->variant = -i / 4 + 2;
          burst->x = INTtoFIXED(centerX / 2);
          burst->y = INTtoFIXED((Random16() & 15) + y + 16);
          random = Random16();
          spread = gRagingHeat_Styles[args->variant][RAGING_HEAT_SPREAD];
          burst->velocityX = ((random % spread) - (spread >> 1)) << 12;
          burst->velocityY = (192 - (Random16() & 255)) << 10;
        }
        for (i = 0; i != gRagingHeat_Styles[args->variant][RAGING_HEAT_EMBERS];
             i++) {
          s32 angle;
          s32 speed;

          EMBERS[i].x = INTtoFIXED(centerX / 2);
          EMBERS[i].y = INTtoFIXED((Random16() & 15) + y + 24);
          angle = (Random16() & 0x7fff) + DEGtoANG(90);
          speed = (Random16() & 255) + 127;
          EMBERS[i].velocityX = (Trig_Sin(angle) * speed) >> 5;
          EMBERS[i].velocityY = (Trig_Cos(angle) * speed) >> 5;
          EMBERS[i].variant = (Random16() & 15) + 16;
        }
        EFFECT_SHOW_DAMAGE(SE_CRASH);
        for (i = 0; i != args->targetCount; i++) {
          BattleMotion_ApplyKnockbackFar(args->targets[i], KNOCKBACK_NUDGE);
          BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                   POSE_DAMAGE, i, 16);
        }
        state->shakeFrames = 16;
      }
      if (y <= 52) {
        draw[0](canvas, buffer, centerX / 2 - 24, y, 24, 64);
        draw[1](canvas, buffer, centerX / 2, y, 24, 64);
      }
      i = 0;
      /* FAKEMATCH(dead): the test repeats the loop's own. */
      if (i != gRagingHeat_Styles[args->variant][RAGING_HEAT_BURSTS]) {
        for (particle = state->particles;
             i != gRagingHeat_Styles[args->variant][RAGING_HEAT_BURSTS];
             i++, particle++) {
          if ((u32)particle->variant < RAGING_HEAT_BURST_FRAMES) {
            if (particle->variant > 1) {
              draw[0](canvas,
                      state->sheet + RAGING_HEAT_FLASH_BURST_PICTURES +
                          (particle->variant / 4) * PICTURE_BYTES(FLASH_BURST),
                      FIXED_INTEGER_PART(particle->x) - FLASH_BURST_WIDTH / 2,
                      FIXED_INTEGER_PART(particle->y) - FLASH_BURST_HEIGHT / 2,
                      FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
            }
            Particle_AdvanceWithGravity2D(particle, 60, -toFIXED(1) / 16);
          }
          particle->variant++;
        }
      }
      i = 0;
      if (i != gRagingHeat_Styles[args->variant][RAGING_HEAT_EMBERS]) {
        particle = EMBERS;
        do {
          if (particle->variant >= 0) {
            s32 size = (particle->variant >> 3) + 1;

            EFFECT_DRAW_MOTE(draw[0], canvas, sprites,
                             FIXED_INTEGER_PART(particle->x),
                             FIXED_INTEGER_PART(particle->y), size);
            Particle_AdvanceWithGravity2D(particle, 60, toFIXED(1) / 16);
            particle->variant--;
          }
          i++;
          particle++;
        } while (i != gRagingHeat_Styles[args->variant][RAGING_HEAT_EMBERS]);
      }
    }

    // The circle of runes, and from the pillar's fall the ring of fire.
    {
      void *vertices;
      s32 shade;

      vertices = Heap_AllocEwram(4 * sizeof(struct ProjectedVertex));
      model = Render_AllocateModelList(1);
      shade = 0;
      model->kind = MODEL_SHADED_ADDITIVE;
      model->texture = &texture;
      model->mesh = gBattleEffect_LargeSquareMesh;
      model->vertices = vertices;
      model->shade = shade;
      texture.logWidth = 7;
      texture.logHeight = 7;
      for (i = 0; i != 2; i++) {
        s32 size = gRagingHeat_Styles[args->variant][RAGING_HEAT_RING_SIZE];
        s32 largest = size * (FIXED_ONE / 64);
        s32 step = size << 5;
        s32 scale;

        Matrix_Initialize();
        Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
        if (i == 0) {
          Matrix_Translate(INTtoFIXED(centerX) - toFIXED(128),
                           INTtoFIXED(baseY) - toFIXED(64), 0);
        } else {
          Matrix_Translate(INTtoFIXED(centerX) - toFIXED(128),
                           INTtoFIXED(baseY + 8) - toFIXED(64), 0);
        }
        Matrix_RotateX(RAGING_HEAT_TILT);
        if (frame < RUNE_CIRCLE_RIGHTED_FRAME) {
          Matrix_RotateX((RUNE_CIRCLE_RIGHTED_FRAME - frame) << 9);
        }
        if (i == 0) {
          scale = (frame - 16) * step;
          if (scale > largest) {
            scale = largest;
          }
          if (frame > 60) {
            scale = largest - step * (frame * 2 - 120);
          }
          texture.pixels = state->sheet;
          Matrix_RotateY(-frame * 3 << 8);
        } else {
          scale = (frame - RAGING_HEAT_PILLAR_FRAME) * 4 * step;
          if (scale < 0) {
            continue;
          }
          scale += FIXED_ONE / 4;
          shade = 512 - frame * 8;
          model->shade = shade;
          texture.pixels = FIRE_RING_PIXELS;
        }
        if (scale > 0 && shade > MODEL_SHADE_INVISIBLE) {
          Matrix_ScaleUniform(scale);
          Render_TransformVertices(gBattleEffect_GroundSquareCorners, vertices,
                                   4);
          Render_DrawModelList(model);
        }
      }
      Heap_Release(model);
      Heap_Release(vertices);
    }
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(state);
  }
  EFFECT_FINISH_CANVAS();
}

#undef RAGING_HEAT_EMBER_COUNT
#undef RAGING_HEAT_PILLAR_FRAME
#undef RAGING_HEAT_FLASH_BURST_PICTURES
#undef RAGING_HEAT_BURSTS_SET_OUT
#undef RAGING_HEAT_BURST_FRAMES
#undef RAGING_HEAT_TILT
#undef RUNE_CIRCLE_RIGHTED_FRAME

#define JUGGLE_FRAMES 122
#define JUGGLE_RING_RISE_FRAME 8   ///< the ring starts to rise over the caster
#define JUGGLE_RING_CUE_FRAME 16   ///< the launch is heard
#define JUGGLE_RING_THROW_FRAME 64 ///< it flies at the targets
#define JUGGLE_RING_HIT_FRAME 88   ///< it strikes; sparks burst
#define JUGGLE_SPARKS_PER_TARGET 128
#define JUGGLE_SPARK_COUNT 768
/// The ring's tilt toward the viewer, beside its turn.
#define JUGGLE_TILT 500
/// SmallFlameSheet's four pictures of a flame.
#define JUGGLE_FLAME_WIDTH 16
#define JUGGLE_FLAME_HEIGHT 64

/// gJuggle_Styles[variant]: seven bytes for each strength.
enum {
  JUGGLE_RING_LIFT_STEP,     ///< pixels the ring rises each frame
  JUGGLE_RING_LIFT_MAX,      ///< and its highest
  JUGGLE_RING_FLAMES,        ///< flames in the ring
  JUGGLE_RING_FLAME_SPACING, ///< their spacing round it
  JUGGLE_RING_SPARKS,        ///< sparks at each target
  JUGGLE_RING_SHAKE,         ///< frames the screen shakes at the hit
  JUGGLE_RING_BLITTER        ///< which of the pair draws the flames
};

/// A ring of small flames. The flames circle the caster in a ring that rises
/// over its head from frame 8, each turning its own way through four cels.
/// At frame 64 the ring flies at the targets' midpoint, slowing as it
/// nears; at frame 88 it strikes: every target is knocked back and flashes,
/// sparks burst from each, and the screen shakes. The variant sets the
/// ring's rise, its flames and sparks and the shake; variant 2 also takes
/// the orange palette.
///
/// Abilities: Juggle, Heat Juggle, Fiery Juggle.
void BattleEffect_RunJuggle(struct BattleEffectParams *args) {
  struct ScreenPoint caster;
  struct ScreenPoint midpoint;
  s32 vector[3];
  struct ScreenPoint target;
  DrawRectangle draw[2];
  struct BattleEffectState *state = Ram_HeapSlots->battleEffect;
  void *canvas = Ram_HeapSlots->battleCanvas;
  s32 frame;
  u8 *sparkCels = Ram_HeapSlots->battleSheet;
  struct Particle *ring = state->particles;
  DrawRectangle *blitters;
  s32 lift;
  s32 j;
  s32 i;

  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  Resource_LoadAndDecompress(RESOURCE_SMALL_FLAME_SHEET, state->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sparkCels, 0, 0);
  if (args->variant == EFFECT_VARIANT_STRONGEST) {
    LOAD_BG_PALETTE(RESOURCE_ORANGE_PALETTE_B);
  }
  blitters = draw;
  BattleEffect_FetchRectangleBlitters(0, blitters);
  EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  Task_Add((s32)BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
  lift = 0;
  {
    s32 *life = &((struct Particle *)Ram_MapCellBuffer)->variant;

    for (i = 0; i != JUGGLE_SPARK_COUNT; i++) {
      *life = -1;
      life += sizeof(struct Particle) / sizeof(s32);
    }
  }

  for (frame = 0; frame != JUGGLE_FRAMES; frame++) {
    if (frame == JUGGLE_RING_CUE_FRAME)
      Sound_PlayFar(SE_LAUNCH);
    if (frame == 0)
      lift = 0;

    // The ring rises over the caster, and follows it until it is thrown.
    EffectPosition_ProjectBattler(args->user, &caster);
    if (frame >= JUGGLE_RING_RISE_FRAME)
      lift += gJuggle_Styles[args->variant][JUGGLE_RING_LIFT_STEP];
    if (lift > gJuggle_Styles[args->variant][JUGGLE_RING_LIFT_MAX])
      lift = gJuggle_Styles[args->variant][JUGGLE_RING_LIFT_MAX];
    if (frame < JUGGLE_RING_THROW_FRAME) {
      ring->x = INTtoFIXED(caster.x);
      ring->y = INTtoFIXED(caster.y - lift - 32);
    } else {
      // The throw: z and velocityZ hold the pull toward the midpoint, and
      // the ring keeps three quarters of its speed each frame.
      if (frame == JUGGLE_RING_THROW_FRAME) {
        BattleEffect_GetTargetsMidpoint(&midpoint);
        ring->x = INTtoFIXED(caster.x);
        ring->y = INTtoFIXED(caster.y - lift - 32);
        ring->velocityX = 0;
        ring->velocityY = 0;
        ring->z = (INTtoFIXED(midpoint.x) - ring->x) / 64;
        ring->velocityZ = (INTtoFIXED(midpoint.y - 32) - ring->y) / 64;
      }
      ring->x += ring->velocityX;
      ring->y += ring->velocityY;
      ring->velocityX += ring->z;
      ring->velocityY += ring->velocityZ;
      ring->velocityX = ring->velocityX * 48 / DAMPING_SCALE;
      ring->velocityY = ring->velocityY * 48 / DAMPING_SCALE;
    }

    // The flames: each one's place on the ring turned into the canvas, the
    // second eight on a ring tipped on its side.
    for (i = 0; i != gJuggle_Styles[args->variant][JUGGLE_RING_FLAMES]; i++) {
      s32 cel = (frame / 2 + i * 4) & 3;
      s32 *place;
      s32 x;
      s32 y;

      Matrix_Initialize();
      Matrix_Translate(ring->x - toFIXED(128), ring->y - toFIXED(64), 0);
      Matrix_RotateZ(frame * (TRIG_TURN / 256));
      Matrix_RotateX(frame * (TRIG_TURN / 256) + JUGGLE_TILT);
      if (i > 7)
        Matrix_RotateX(DEGtoANG(90));
      Matrix_RotateY(
          (frame * 4 +
           i * gJuggle_Styles[args->variant][JUGGLE_RING_FLAME_SPACING])
          << 8);
      place = vector;
      place[0] = 0;
      place[1] = 0;
      place[2] = INTtoFIXED(lift);
      Iwram_TransformVector(place, place);
      // DEAD: place keeps the flame's place on the canvas, never read; the
      // ROM keeps the two stores.
      x = FIXEDtoINT(place[0]) >> 1;
      place[0] = x + 64;
      y = FIXED_INTEGER_PART(place[1]);
      place[1] = y + 64;
      blitters[gJuggle_Styles[args->variant][JUGGLE_RING_BLITTER]](
          canvas, state->sheet + cel * PICTURE_BYTES(JUGGLE_FLAME),
          x + CANVAS_SIZE / 2 - JUGGLE_FLAME_WIDTH / 2, y + 16,
          JUGGLE_FLAME_WIDTH, JUGGLE_FLAME_HEIGHT);
    }

    // The strike: sparks burst from every target.
    if (frame == JUGGLE_RING_HIT_FRAME) {
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      for (i = 0; i != args->targetCount; i++) {
        EffectPosition_ProjectSprite(args->targets[i], &target);
        BattleMotion_ApplyKnockbackFar(args->targets[i], KNOCKBACK_HURL);
        BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, i, 16);
        for (j = 0; j != gJuggle_Styles[args->variant][JUGGLE_RING_SPARKS];
             j++) {
          struct Particle *spark =
              &((struct Particle *)
                    Ram_MapCellBuffer)[i * JUGGLE_SPARKS_PER_TARGET + j];
          s32 angle;
          s32 speed;

          spark->x = INTtoFIXED(target.x / 2);
          spark->y = INTtoFIXED(target.y);
          angle = Random16();
          speed = (Random16() & 255) + 127;
          spark->velocityX = (Trig_Sin(angle) * speed) >> 6;
          spark->velocityY = (Trig_Cos(angle) * speed) >> 6;
          spark->variant = (Random16() & 15) + 24;
        }
      }
      state->shakeFrames = gJuggle_Styles[args->variant][JUGGLE_RING_SHAKE];
    }

    // The sparks fly and burn down, drawn by turns with the two blitters.
    for (i = 0; i != JUGGLE_SPARK_COUNT; i++) {
      struct Particle *spark = &((struct Particle *)Ram_MapCellBuffer)[i];

      if (spark->variant >= 0) {
        s32 size = (spark->variant >> 3) + 1;

        blitters[i & 1](canvas,
                        sparkCels + gBattleEffect_MotePictures[size - 1],
                        FIXED_INTEGER_PART(spark->x) - size / 2,
                        FIXED_INTEGER_PART(spark->y) - size, size, size * 2);
        Particle_AdvanceWithGravity2D(spark, 60, -toFIXED(1) / 4);
        spark->variant--;
      }
    }

    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(state);
  }

  EFFECT_RELEASE_BLITTERS();
  Task_Remove((s32)BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

/// BG2PA for the white band: the canvas's 128 columns stretched across about
/// 420 screen pixels.
#define WHITE_BAND_STRETCH Q_8_8(78.0 / 256)

/// Loads the grey ramp and fills the canvas with a white band, its left and
/// right 32 columns fading out, stretched across the screen on BG2: the
/// summons sweep it in from the right to white the battle out.
void BattleEffect_FillWhiteBand(void) {
  u8 *canvas = Ram_HeapSlots->battleCanvas;
  s32 x;
  s32 y;

  Graphics_LoadGreyRamp();
  REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
  PLTT_BACKDROP = 0;
  REG_BG2PA = WHITE_BAND_STRETCH;
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  for (x = 0; x != CANVAS_SIZE; x++) {
    for (y = 0; y != CANVAS_SIZE; y++) {
      s32 level;

      if (x < 32)
        level = x * 2;
      else if (x > 95)
        level = 255 - x * 2;
      else
        level = CANVAS_LEVEL_BRIGHTEST;
      canvas[CANVAS_PIXEL_OFFSET(x, y)] = level;
    }
  }
}

#undef WHITE_BAND_STRETCH
#undef FIRE_RING_PIXELS
#undef EMBERS
#undef RAGING_HEAT_FRAMES
#undef JUGGLE_FRAMES
#undef JUGGLE_RING_CUE_FRAME
#undef JUGGLE_RING_RISE_FRAME
#undef JUGGLE_RING_THROW_FRAME
#undef JUGGLE_RING_HIT_FRAME
#undef JUGGLE_SPARKS_PER_TARGET
#undef JUGGLE_SPARK_COUNT
#undef JUGGLE_TILT
#undef JUGGLE_FLAME_WIDTH
#undef JUGGLE_FLAME_HEIGHT
