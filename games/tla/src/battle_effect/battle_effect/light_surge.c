/// @file
/// Light Surge.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "macros.h"

struct FxPartyList;
struct Model;
struct StripQuad;

#define BLITTER(mode, flags) ((mode) << 4 | (flags))

struct SurgeTarget {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
};

#define LIGHT_SURGE_FRAMES 74
#define SURGE_BOLTS 14

// Abilities: Light Surge.
//
// The camera swings around the target while 14 bolts of light rain onto
// it, four frames apart; each lands with a flash and a ring of light (two
// rings of shaded circles), and four pillars of light rise and sway over
// the target.
void BattleEffect_RunLightSurge(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 i;
  /* FAKEMATCH(dead): seven locals nothing uses. */
  s32 unused1;
  s32 unused2;
  s32 unused3;
  s32 unused4;
  s32 unused5;
  s32 unused6;
  s32 unused7;
  struct BattleCamera *camera;
  s16 savedYaw;
  struct SurgeTarget *target;
  s16 *circles;
  s32 x;
  s32 y;
  struct ScreenPoint screen;
  struct ScreenPoint *pos;
  s32 head[3];
  struct ScreenPoint center;
  struct ModelTexture texture;
  struct RenderModel *model;
  struct Particle *fall;
  struct Particle *bolt;
  struct Particle *ring;
  struct Particle *flash;
  struct Particle *glow;
  s16 *vertices;
  s32 offset;
  s32 size;
  s32 shade;
  u8 *sprites;
  s32 cells;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  sprites = Ram_HeapSlots->battleSheet;
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C4, work->sheet + 0x1000, 0,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C9, work->sheet + 0x4000, 0,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sprites, 0, 0);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry((s32)&ResourceId_EffectPalette02),
                       128);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  camera = Ram_HeapSlots->cameraWork;
  savedYaw = camera->yaw;
  target = BattleObject_GetFar(args->targets[0])->object;

  // Shaded circles of 2 to 64 pixels, packed in the map buffer.
  cells = (s32)gMapGridCellBuffer;
  circles = (s16 *)work->sheet;
  offset = 0;
  size = 2;
  for (i = 0; i != 32; i++) {
    BattleEffect_DrawShadedCircles((u8 *)(offset + cells), size, 0x10000);
    circles[i] = offset;
    offset += (i * 2 + 3) * size / 2;
    size += 2;
  }

  frame = 0;
  /* FAKEMATCH(dead): pos first takes the camera's look-at point. */
  pos = (struct ScreenPoint *)camera->focus;
  /* FAKEMATCH(dead): the texture's address, computed as center's address
   * less 8. */
  vertices = (s16 *)((s32)&center - 8);
  for (pos = &screen; frame != LIGHT_SURGE_FRAMES; frame++) {
    if (frame == 64) {
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    }
    if (frame > 31 && (frame & 3) == 0) {
      BattleEffect_StepPaletteToResource((s32)&ResourceId_MercuryDjinnSheet);
    }
    if (frame == 0) {
      camera->yaw = savedYaw;
      for (i = 0; i != 32; i++) {
        work->particles[i].variant = -1;
      }
      for (i = 0; i != 32; i++) {
        s32 radius;
        s32 angle;
        s32 z;

        fall = &work->particles[32 + i];
        radius = (Random16() & 63) + 32;
        angle = Random16() & 0xffff;

        fall->x = Trig_Sin(angle) * radius;
        fall->z = Trig_Cos(angle) * radius;
        fall->y = px(110);
        radius = (Random16() & 31) + 4;
        fall->velocityX = target->x + Trig_Sin(angle) * radius;
        z = target->z + Trig_Cos(angle) * radius;
        fall->velocityZ = z;
        fall->velocityY = -(px(8));
        fall->velocityX = (fall->velocityX - fall->x) / 6;
        fall->velocityY = (-0x80000 - fall->y) / 6;
        fall->velocityZ = (z - fall->z) / 6;
        fall->variant = -i * 4;
      }
    }
    if ((u32)(frame - 20) <= 59) {
      s32 turn;

      if (frame <= 71) {
        turn = 0x100;
      } else {
        turn = 0xa00 - (frame << 5);
      }
      if (args->side == EFFECT_SIDE_ENEMIES) {
        camera->yaw += turn;
      } else {
        camera->yaw -= turn;
      }
    }
    Transform_Reset();
    Transform_SetLookAt(camera->eye, camera->focus);

    // The rings: four quarter circles each, upper halves for the first 16.
    for (i = 0, ring = work->particles; i != 28; ring++, i++) {
      void *cel;
      s32 half;
      s32 index;
      s16 *entry;
      s32 left;
      s32 size;

      if ((u32)ring->variant > 15) {
        continue;
      }
      size = ((Trig_Sin(ring->variant << 11) * ring->velocityX) >> 16) * 2;
      index = size - 2;
      ring->variant++;
      if ((u32)index > 61) {
        continue;
      }
      EffectPosition_ProjectPoint(ring, pos);
      index >>= 1;
      x = pos->x / 2;
      y = pos->y;
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS,
                          BLITTER_MAX);
      index <<= 1;
      index += (s32)circles;
      half = size >> 1;
      cel = (void *)(*(s16 *)index + cells);
      entry = (s16 *)index;
      left = x - half;
      index = y - size;
      ((DrawRectangle)Ram_HeapSlots->ramCode2)(canvas, cel, left, index, half,
                                               size);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                          EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
      ((DrawRectangle)Ram_HeapSlots->ramCode2)(canvas, (void *)(*entry + cells),
                                               x, index, half, size);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
      if (i > 15) {
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                            EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y, BLITTER_MAX);
        ((DrawRectangle)Ram_HeapSlots->ramCode2)(
            canvas, (void *)(*entry + cells), x - half, y, half, size);
        HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2,
                            EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP, BLITTER_MAX);
        ((DrawRectangle)Ram_HeapSlots->ramCode2)(
            canvas, (void *)(*entry + cells), x, y, half, size);
        HeapSlot_Release(HEAP_SLOT_RAM_CODE_2);
      }
    }

    // The bolts: a streak from the sky to the ground, six frames long.
    vertices = Heap_AllocEwram(32);
    model = Render_AllocateModelList(1);
    shade = 0;
    texture.logWidth = 5;
    texture.logHeight = 2;
    model->texture = &texture;
    model->mesh = gBattleEffect_SmallSquareMeshReversed;
    model->vertices = vertices;
    model->shade = shade;
    model->kind = 7;
    texture.pixels = work->sheet + 0x4000;
    for (i = 0, flash = work->particles, glow = &work->particles[16],
        bolt = &work->particles[32];
         i != SURGE_BOLTS; flash++, glow++, bolt++, i++) {
      s32 t = ++bolt->variant;
      struct ScreenPoint *out;

      if ((u32)t > 17) {
        continue;
      }
      if (t > 11) {
        head[0] = bolt->x + bolt->velocityX * (t - 12);
        head[1] = bolt->y + bolt->velocityY * (bolt->variant - 12);
        head[2] = bolt->z + bolt->velocityZ * (bolt->variant - 12);
        if (bolt->variant == 16) {
          SET_POSITION(glow, head[0], head[1], head[2]);
          glow->velocityX = (Random16() & 15) + 8;
          glow->variant = 0;
        }
        out = pos;
        EffectPosition_ProjectPoint((struct Particle *)head, out);
      } else {
        out = &screen;
        EffectPosition_ProjectPoint(bolt, out);
      }
      vertices[0] = out->x / 2 - 2;
      vertices[1] = out->y;
      vertices[4] = out->x / 2 + 2;
      vertices[5] = out->y;
      if (bolt->variant <= 5) {
        head[0] = bolt->x + bolt->velocityX * bolt->variant;
        head[1] = bolt->y + bolt->velocityY * bolt->variant;
        head[2] = bolt->z + bolt->velocityZ * bolt->variant;
      } else {
        head[0] = bolt->x + bolt->velocityX * 6;
        head[1] = bolt->y + bolt->velocityY * 6;
        head[2] = bolt->z + bolt->velocityZ * 6;
      }
      if (bolt->variant == 6) {
        SET_POSITION(flash, head[0], head[1], head[2]);
        flash->velocityX = (Random16() & 7) + 24;
        flash->variant = 0;
        work->shakeFrames = 4;
        Sound_PlayFar(SE_CRASH);
        BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_NUDGE);
        BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, 0, 2);
      }
      EffectPosition_ProjectPoint((struct Particle *)head, out);
      vertices[8] = out->x / 2 - 2;
      vertices[9] = out->y;
      vertices[12] = out->x / 2 + 2;
      vertices[13] = out->y;
      Render_DrawModelList(model);
    }

    // The pillars.
    if ((u32)(frame - 16) <= 63) {
      EffectPosition_ProjectBattler(args->targets[0], &center);
      texture.logWidth = 5;
      texture.logHeight = 5;
      texture.pixels = work->sheet + 0x1000;
      model->kind = 6;
      model->mesh = gBattleEffect_SmallSquareMeshReversed;
      for (i = 0; i != 4; i++) {
        s32 sway = Trig_Sin((frame << 8) + TRIG_TURN / 16);
        s32 x = ((center.x / 2 - 64) << 16) -
                (gLightSurge_PillarOffsets[i] * sway / (TRIG_TURN)) *
                    Trig_Sin((s16)camera->yaw);
        s32 z = -((gLightSurge_PillarOffsets[i] * sway / (TRIG_TURN)) *
                  Trig_Cos((s16)camera->yaw));
        s32 growth;
        u32 scale;

        /* FAKEMATCH(dead): a store both arms below overwrite. */
        if (i == 300)
          growth = 0;
        if (frame <= 47) {
          growth = Trig_Sin((frame << 9) - TRIG_TURN / 8);
        } else {
          growth = 0x10000;
        }
        scale = (u32)(gLightSurge_PillarSizes[i] * growth) >> 16;
        if (frame > 63) {
          shade = gLightSurge_PillarShades[i] - frame * 8 + 512;
        } else if (frame <= 23) {
          shade = gLightSurge_PillarShades[i] + frame * 8 - 192;
        } else {
          shade = gLightSurge_PillarShades[i];
        }
        model->shade = shade;
        Transform_Reset();
        SceneTransform_ApplyPosition3(x, z, 0);
        SceneTransform_ApplyScale3(scale, scale * 2, scale);
        Render_TransformVertices(gBattleEffect_SquareCorners, vertices, 4);
        Render_DrawModelList(model);
      }
    }
    Heap_Release(model);
    Heap_Release(vertices);
    /* FAKEMATCH(dead): a read of shade the compiler folds to nothing. */
    BattleCamera_Shake(4 + ((shade & 255) >> 8), 4);
    EFFECT_END_FRAME(work);
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

/// Abilities: Whorl (kind 232).
void BattleEffect_RunWhorl(s32 a0) { BattleEffect_RunWaterColumn(a0, 0); }

/// Abilities: Gale (kind 233).
void BattleEffect_RunGale(s32 a0) { BattleEffect_RunWaterColumn(a0, 1); }

/// Abilities: Lethe Albion (kind 107).
void BattleEffect_RunLetheAlbion(s32 a0) { BattleEffect_RunWaterColumn(a0, 2); }

/// Abilities: Poison Flow (kind 81).
void BattleEffect_RunPoisonFlow(s32 a0) { BattleEffect_RunWaterColumn(a0, 3); }

/// Abilities: Raging Flood (kind 392).
void BattleEffect_RunRagingFlood(s32 a0) { BattleEffect_RunWaterColumn(a0, 4); }
#undef BLITTER
#undef LIGHT_SURGE_FRAMES
#undef SURGE_BOLTS
