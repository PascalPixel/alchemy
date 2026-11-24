/// @file
/// Light Surge, then the handlers that play the water column for Whorl, Gale,
/// Lethe Albion, Poison Flow and Raging Flood.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

#define LIGHT_SURGE_FRAMES 74
/// The frame the battle shows the damage.
#define LIGHT_SURGE_DAMAGE_FRAME 64
/// From this frame the colours step towards the Mercury Djinn sprite sheet's,
/// one step every fourth frame.
#define LIGHT_SURGE_RECOLOR_FRAME 32

/// The state's particles: each bolt's flash from the first, its glow from
/// SURGE_FIRST_GLOW and the bolt itself from SURGE_FIRST_BOLT. The first frame
/// sets out a bolt in every particle from SURGE_FIRST_BOLT on, each starting
/// SURGE_BOLT_INTERVAL frames after the last; the first SURGE_BOLTS fall.
#define SURGE_FIRST_GLOW 16
#define SURGE_FIRST_BOLT 32
#define SURGE_BOLTS 14
#define SURGE_BOLT_INTERVAL 4

/// A bolt's head falls from the sky to the target's feet in SURGE_BOLT_FALL
/// frames and lands with a flash; its tail leaves the sky at SURGE_BOLT_TAIL
/// and follows it down, leaves a glow where it is at SURGE_BOLT_GLOW, and the
/// bolt is gone after SURGE_BOLT_FRAMES.
#define SURGE_BOLT_FALL 6
#define SURGE_BOLT_TAIL 12
#define SURGE_BOLT_GLOW 16
#define SURGE_BOLT_FRAMES 18
/// Half a bolt's width on the canvas.
#define SURGE_BOLT_HALF_WIDTH 2

/// The shaded discs the flashes and glows are drawn from: SURGE_CIRCLES
/// quarter discs, 2 to SURGE_CIRCLE_SIZE_MAX pixels, packed in the map cell
/// buffer.
#define SURGE_CIRCLES 32
#define SURGE_CIRCLE_SIZE_MAX (SURGE_CIRCLES * 2)
/// The discs a frame draws: the flashes, and the glows of all but the last two
/// bolts. Each swells and shrinks over SURGE_DISC_FRAMES, half a turn of a
/// sine.
#define SURGE_DISCS 28
#define SURGE_DISC_FRAMES 16

/// The camera swings around the target SURGE_SWING_TURN a frame from
/// SURGE_SWING_START, slowing over SURGE_SWING_SLOWING frames to a stop at
/// SURGE_SWING_END, after the effect has ended.
#define SURGE_SWING_START 20
#define SURGE_SWING_END 80
#define SURGE_SWING_TURN (TRIG_TURN / 256)
#define SURGE_SWING_SLOWING 8

/// The four rings of light about the target: they show from
/// SURGE_RINGS_START, swell over a quarter turn of a sine to their full size
/// at SURGE_RINGS_GROWN, brighten until SURGE_RINGS_SHOWN and dim from
/// SURGE_RINGS_DIMMING, 8 shades a frame.
#define SURGE_RINGS 4
#define SURGE_RINGS_START 16
#define SURGE_RINGS_SHOWN 24
#define SURGE_RINGS_GROWN 48
#define SURGE_RINGS_DIMMING 64
#define SURGE_RINGS_END 80

/// Where GlowOrbTexture is decoded in the effect's pictures.
#define LIGHT_SURGE_GLOW_ORB 4096

/// Where GlowBandTextureA is decoded in the effect's pictures.
#define LIGHT_SURGE_GLOW_BAND 16384

/// The camera swings around the target while SURGE_BOLTS bolts of light rain
/// onto it, SURGE_BOLT_INTERVAL frames apart; each lands with a flash and
/// leaves a glow, and SURGE_RINGS rings of light swell about the target and
/// sway.
///
/// Abilities: Light Surge.
void BattleEffect_RunLightSurge(struct BattleEffectParams *args) {
  struct BattleEffectState *state;
  void *canvas;
  s32 frame;
  s32 i;
  s32 radius;
  s32 angle;
  s32 z;
  s32 turn;
  s32 half;
  s32 index;
  s16 *entry;
  /* FAKEMATCH(temporary): out, a second pointer holding &screen, taken from
   * pos in one arm and as &screen in the other. */
  struct ScreenPoint *out;
  s32 growth;
  u32 scale;
  struct BattleCamera *camera;
  s16 savedYaw;
  struct MotionObject *target;
  s16 *circles;
  s32 x;
  s32 y;
  struct ScreenPoint screen;
  struct ScreenPoint *pos;
  s32 point[3];
  struct ScreenPoint center;
  struct ModelTexture texture;
  struct RenderModel *model;
  struct Particle *fall;
  struct Particle *bolt;
  struct Particle *disc;
  struct Particle *flash;
  struct Particle *glow;
  struct ProjectedVertex *vertices;
  s32 offset;
  s32 size;
  s32 shade;
  u8 *sprites;
  u8 *cells;

  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sprites = Ram_HeapSlots->battleSheet;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  Resource_LoadAndDecompress(RESOURCE_GLOW_ORB_TEXTURE,
                             state->sheet + LIGHT_SURGE_GLOW_ORB, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_GLOW_BAND_TEXTURE_A,
                             state->sheet + LIGHT_SURGE_GLOW_BAND, 0, 0);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, sprites, 0, 0);
  LOAD_BG_PALETTE(RESOURCE_VIOLET_PALETTE_F);
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  camera = Ram_HeapSlots->cameraState;
  savedYaw = camera->yaw;
  target = BattleObject_GetFar(args->targets[0])->object;

  // The discs' quarters, packed in the map cell buffer; the sheet's start
  // keeps where each lies.
  cells = gMapGridCellBuffer;
  circles = (s16 *)state->sheet;
  offset = 0;
  size = 2;
  for (i = 0; i != SURGE_CIRCLES; i++) {
    BattleEffect_DrawShadedCircles((u8 *)(offset + (s32)cells), size,
                                   FIXED_ONE);
    circles[i] = offset;
    offset += (i * 2 + 3) * size / 2;
    size += 2;
  }

  frame = 0;
  /* FAKEMATCH(dead): pos first takes the camera's look-at point. */
  pos = (struct ScreenPoint *)camera->focus;
  /* FAKEMATCH(dead): the texture's address, computed as center's address
   * less 8. */
  vertices = (struct ProjectedVertex *)((s32)&center - 8);
  /* FAKEMATCH(temporary): pos holds &screen. */
  pos = &screen;
  for (; frame != LIGHT_SURGE_FRAMES; frame++) {
    if (frame == LIGHT_SURGE_DAMAGE_FRAME) {
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    }
    if (frame >= LIGHT_SURGE_RECOLOR_FRAME && (frame & 3) == 0) {
      BattleEffect_StepPaletteToResource(RESOURCE_MERCURY_DJINN_SHEET);
    }
    if (frame == 0) {
      camera->yaw = savedYaw;
      EFFECT_FREE_PARTICLES(i, state->particles, SURGE_FIRST_BOLT);
      // Each bolt falls from 110 pixels up, 32 to 95 pixels out from the
      // middle of the battlefield, to the ground 4 to 35 pixels out from the
      // target in the same direction.
      for (i = 0; i != ARRAY_COUNT(state->particles) - SURGE_FIRST_BOLT; i++) {
        fall = &state->particles[SURGE_FIRST_BOLT + i];
        radius = (Random16() & 63) + 32;
        angle = Random16() & TRIG_ANGLE_MASK;

        fall->x = Trig_Sin(angle) * radius;
        fall->z = Trig_Cos(angle) * radius;
        fall->y = toFIXED(110);
        radius = (Random16() & 31) + 4;
        fall->velocityX = target->x + Trig_Sin(angle) * radius;
        z = target->z + Trig_Cos(angle) * radius;
        // DEAD: the z and y speeds are overwritten below before any read; the
        // ROM keeps the stores.
        fall->velocityZ = z;
        fall->velocityY = -toFIXED(8);
        fall->velocityX = (fall->velocityX - fall->x) / SURGE_BOLT_FALL;
        fall->velocityY = (-toFIXED(8) - fall->y) / SURGE_BOLT_FALL;
        fall->velocityZ = (z - fall->z) / SURGE_BOLT_FALL;
        fall->variant = -i * SURGE_BOLT_INTERVAL;
      }
    }
    if ((u32)(frame - SURGE_SWING_START) <
        SURGE_SWING_END - SURGE_SWING_START) {
      if (frame < SURGE_SWING_END - SURGE_SWING_SLOWING) {
        turn = SURGE_SWING_TURN;
      } else {
        turn = SURGE_SWING_TURN -
               (frame - (SURGE_SWING_END - SURGE_SWING_SLOWING)) *
                   (SURGE_SWING_TURN / SURGE_SWING_SLOWING);
      }
      if (args->side == EFFECT_SIDE_ENEMIES) {
        camera->yaw += turn;
      } else {
        camera->yaw -= turn;
      }
    }
    Matrix_Initialize();
    Matrix_SetLookAt(camera->eye, camera->focus);

    // The flashes and glows, each a shaded disc of four quarters: a flash shows
    // only its upper half.
    for (i = 0, disc = state->particles; i != SURGE_DISCS; disc++, i++) {
      s32 size;

      if ((u32)disc->variant >= SURGE_DISC_FRAMES) {
        continue;
      }
      // Its size swells to twice the radius in velocityX and shrinks back.
      size = (FIXEDtoINT(
                 Trig_Sin(disc->variant * (DEGtoANG(180) / SURGE_DISC_FRAMES)) *
                 disc->velocityX)) *
             2;
      index = size - 2;
      disc->variant++;
      // A disc with no size, or as large as the largest circle, is not drawn.
      if ((u32)index >= SURGE_CIRCLE_SIZE_MAX - 2) {
        continue;
      }
      EffectPosition_ProjectPoint(disc, pos);
      index >>= 1;
      x = pos->x / 2;
      y = pos->y;
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                          BLITTER_MAX);
      index <<= 1;
      entry = (s16 *)(index + (s32)circles);
      half = size >> 1;
      ((DrawRectangle)Ram_HeapSlots->ramCodePaired)(
          canvas, (void *)(*entry + (s32)cells), x - half, y - size, half,
          size);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
      ((DrawRectangle)Ram_HeapSlots->ramCodePaired)(
          canvas, (void *)(*entry + (s32)cells), x, y - size, half, size);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
      if (i >= SURGE_FIRST_GLOW) {
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                            EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, BLITTER_MAX);
        ((DrawRectangle)Ram_HeapSlots->ramCodePaired)(
            canvas, (void *)(*entry + (s32)cells), x - half, y, half, size);
        HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
                            EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, BLITTER_MAX);
        ((DrawRectangle)Ram_HeapSlots->ramCodePaired)(
            canvas, (void *)(*entry + (s32)cells), x, y, half, size);
        HeapSlot_Release(HEAP_SLOT_RAM_CODE_PAIRED);
      }
    }

    // The bolts: GlowBandTextureA across a quad from its tail to its head.
    vertices = Heap_AllocEwram(4 * sizeof(struct ProjectedVertex));
    model = Render_AllocateModelList(1);
    shade = 0;
    texture.logWidth = 5;
    texture.logHeight = 2;
    model->texture = &texture;
    model->mesh = gBattleEffect_SmallSquareMeshReversed;
    model->vertices = (s16 *)vertices;
    model->shade = shade;
    model->kind = MODEL_SHADED_ADDITIVE;
    texture.pixels = state->sheet + LIGHT_SURGE_GLOW_BAND;
    for (i = 0, flash = state->particles,
        glow = &state->particles[SURGE_FIRST_GLOW],
        bolt = &state->particles[SURGE_FIRST_BOLT];
         i != SURGE_BOLTS; flash++, glow++, bolt++, i++) {
      s32 t = ++bolt->variant;

      if ((u32)t >= SURGE_BOLT_FRAMES) {
        continue;
      }
      if (t >= SURGE_BOLT_TAIL) {
        point[0] = bolt->x + bolt->velocityX * (t - SURGE_BOLT_TAIL);
        point[1] =
            bolt->y + bolt->velocityY * (bolt->variant - SURGE_BOLT_TAIL);
        point[2] =
            bolt->z + bolt->velocityZ * (bolt->variant - SURGE_BOLT_TAIL);
        if (bolt->variant == SURGE_BOLT_GLOW) {
          SET_POSITION(glow, point[0], point[1], point[2]);
          glow->velocityX = (Random16() & 15) + 8;
          glow->variant = 0;
        }
        out = pos;
        EffectPosition_ProjectPoint(point, out);
      } else {
        out = &screen;
        EffectPosition_ProjectPoint(bolt, out);
      }
      vertices[0].x = out->x / 2 - SURGE_BOLT_HALF_WIDTH;
      vertices[0].y = out->y;
      vertices[1].x = out->x / 2 + SURGE_BOLT_HALF_WIDTH;
      vertices[1].y = out->y;
      if (bolt->variant < SURGE_BOLT_FALL) {
        point[0] = bolt->x + bolt->velocityX * bolt->variant;
        point[1] = bolt->y + bolt->velocityY * bolt->variant;
        point[2] = bolt->z + bolt->velocityZ * bolt->variant;
      } else {
        point[0] = bolt->x + bolt->velocityX * SURGE_BOLT_FALL;
        point[1] = bolt->y + bolt->velocityY * SURGE_BOLT_FALL;
        point[2] = bolt->z + bolt->velocityZ * SURGE_BOLT_FALL;
      }
      if (bolt->variant == SURGE_BOLT_FALL) {
        SET_POSITION(flash, point[0], point[1], point[2]);
        flash->velocityX = (Random16() & 7) + 24;
        flash->variant = 0;
        state->shakeFrames = 4;
        Sound_PlayFar(SE_CRASH);
        BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_NUDGE);
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, 0, 2);
      }
      EffectPosition_ProjectPoint(point, out);
      vertices[2].x = out->x / 2 - SURGE_BOLT_HALF_WIDTH;
      vertices[2].y = out->y;
      vertices[3].x = out->x / 2 + SURGE_BOLT_HALF_WIDTH;
      vertices[3].y = out->y;
      Render_DrawModelList(model);
    }

    // The rings: GlowOrbTexture on a square, twice as tall on the canvas as it
    // is wide, so round on the screen.
    if ((u32)(frame - SURGE_RINGS_START) <
        SURGE_RINGS_END - SURGE_RINGS_START) {
      EffectPosition_ProjectBattler(args->targets[0], &center);
      texture.logWidth = 5;
      texture.logHeight = 5;
      texture.pixels = state->sheet + LIGHT_SURGE_GLOW_ORB;
      model->kind = MODEL_SHADED_MAX;
      model->mesh = gBattleEffect_SmallSquareMeshReversed;
      for (i = 0; i != SURGE_RINGS; i++) {
        // Each sways along a line through the target, turned with the camera:
        // across the screen, and into it drawn as up and down.
        FIXED sway = Trig_Sin((frame << 8) + TRIG_TURN / 16);
        s32 x = INTtoFIXED(center.x / 2 - CANVAS_SIZE / 2) -
                (gLightSurge_PillarOffsets[i] * sway / FIXED_ONE) *
                    Trig_Sin((s16)camera->yaw);
        s32 z = -((gLightSurge_PillarOffsets[i] * sway / FIXED_ONE) *
                  Trig_Cos((s16)camera->yaw));

        /* FAKEMATCH(dead): a store both arms below overwrite. */
        if (i == 300)
          growth = 0;
        if (frame < SURGE_RINGS_GROWN) {
          growth = Trig_Sin(
              (frame - SURGE_RINGS_START) *
              (DEGtoANG(90) / (SURGE_RINGS_GROWN - SURGE_RINGS_START)));
        } else {
          growth = FIXED_ONE;
        }
        scale = (u32)(gLightSurge_PillarSizes[i] * growth) / FIXED_ONE;
        if (frame >= SURGE_RINGS_DIMMING) {
          shade =
              gLightSurge_PillarShades[i] - (frame - SURGE_RINGS_DIMMING) * 8;
        } else if (frame < SURGE_RINGS_SHOWN) {
          shade = gLightSurge_PillarShades[i] + (frame - SURGE_RINGS_SHOWN) * 8;
        } else {
          shade = gLightSurge_PillarShades[i];
        }
        model->shade = shade;
        Matrix_Initialize();
        Matrix_Translate(x, z, 0);
        Matrix_Scale(scale, scale * 2, scale);
        Render_TransformVertices(gBattleEffect_SquareCorners, vertices, 4);
        Render_DrawModelList(model);
      }
    }
    Heap_Release(model);
    Heap_Release(vertices);
    /* FAKEMATCH(dead): a read of shade that does nothing. */
    BattleCamera_Shake(4 + ((shade & 255) >> 8), 4);
    EFFECT_END_FRAME(state);
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

/// Abilities: Whorl.
void BattleEffect_RunWhorl(struct BattleEffectParams *args) {
  BattleEffect_RunWaterColumn(args, WATER_COLUMN_WHORL);
}

/// Abilities: Gale.
void BattleEffect_RunGale(struct BattleEffectParams *args) {
  BattleEffect_RunWaterColumn(args, WATER_COLUMN_GALE);
}

/// Abilities: Lethe Albion.
void BattleEffect_RunLetheAlbion(struct BattleEffectParams *args) {
  BattleEffect_RunWaterColumn(args, WATER_COLUMN_LETHE_ALBION);
}

/// Abilities: Poison Flow.
void BattleEffect_RunPoisonFlow(struct BattleEffectParams *args) {
  BattleEffect_RunWaterColumn(args, WATER_COLUMN_POISON_FLOW);
}

/// Abilities: Raging Flood.
void BattleEffect_RunRagingFlood(struct BattleEffectParams *args) {
  BattleEffect_RunWaterColumn(args, WATER_COLUMN_RAGING_FLOOD);
}
#undef LIGHT_SURGE_FRAMES
#undef LIGHT_SURGE_DAMAGE_FRAME
#undef LIGHT_SURGE_RECOLOR_FRAME
#undef SURGE_FIRST_GLOW
#undef SURGE_FIRST_BOLT
#undef SURGE_BOLTS
#undef SURGE_BOLT_INTERVAL
#undef SURGE_BOLT_FALL
#undef SURGE_BOLT_TAIL
#undef SURGE_BOLT_GLOW
#undef SURGE_BOLT_FRAMES
#undef SURGE_BOLT_HALF_WIDTH
#undef SURGE_CIRCLES
#undef SURGE_CIRCLE_SIZE_MAX
#undef SURGE_DISCS
#undef SURGE_DISC_FRAMES
#undef SURGE_SWING_START
#undef SURGE_SWING_END
#undef SURGE_SWING_TURN
#undef SURGE_SWING_SLOWING
#undef SURGE_RINGS
#undef SURGE_RINGS_START
#undef SURGE_RINGS_SHOWN
#undef SURGE_RINGS_GROWN
#undef SURGE_RINGS_DIMMING
#undef SURGE_RINGS_END
#undef LIGHT_SURGE_GLOW_ORB
#undef LIGHT_SURGE_GLOW_BAND
