/// @file
/// Fume.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

struct StripQuad;

struct Corner {
  s8 x;
  s8 y;
  s8 z;
};

// Columns of gFume_Levels, one row per Fume level (effect->variant): the
// plume's width, the frame its tail starts, its arc's height, the sparks'
// spread, count and lift.
enum {
  FUME_WIDTH,
  FUME_TAIL_START,
  FUME_HEIGHT,
  FUME_SPREAD,
  FUME_SPARKS,
  FUME_LIFT,
  FUME_COLUMNS
};

extern const u16 gFume_Levels[][FUME_COLUMNS];

#define FUME_FRAMES 50
#define FUME_SEGMENTS 33

/// A band of smoke arcs from the caster to the target (33 segments of two
/// vertices each, a scrolling textured strip), its head growing 44 units a
/// frame and its tail following from the level's start frame; a claw strikes
/// at the arc's end until frame 22, when the target is hit, pushed back and
/// sparks burst from it. The camera shakes harder at higher levels.
///
/// Abilities: Fume, Serpent Fume, Dragon Fume.
void BattleEffect_RunFume(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  // Unused: the second word gives the stack frame its 4 bytes (as a plain
  // local, 106 differ).
  struct {
    DrawRectangle draw;
    s32 unused;
  } blit;
  s32 j;
  s32 frame;
  s32 endX;
  s32 endY;
  struct {
    s32 head;
    s32 tail;
  } arc;
  struct ScreenPoint caster;
  struct ScreenPoint target;
  struct ModelTexture texture;
  struct RenderModel *model;
  struct Particle *spark;
  struct Particle *particle;
  s32 i;
  void *buffer;
  u8 *strip;
  struct Corner *corners;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  BattleEffect_StartCanvasLayer(1);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  blit.draw = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  endX = 0;
  endY = 0;
  EffectPosition_ProjectBattler(args->user, &caster);
  EffectPosition_ProjectBattler(args->targets[0], &target);
  Resource_LoadAndDecompress((s32)&ResourceId_SmokeBandSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             work->sheet + 0x800, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ClawSlashSheet,
                             work->sheet + 0x4000, 1, 0);
  REG_BG2PA = Q_8_8(0.5);
  Render_BuildQuadStrip(Data_02012000, 4, 32, 32);

  for (frame = 0; frame != FUME_FRAMES; frame++) {
    if (frame == 0) {
      arc.head = 0;
      arc.tail = 0;
      for (i = 0; i != 64; i++) {
        work->particles[i].variant = 24;
      }
    }
    if (frame == 22) {
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(63));
      EFFECT_SHOW_DAMAGE(SE_CLATTER);
      work->shakeFrames = 8;
      BattleEffect_StartActorMotion(args->targets[0], 1, 0x20000, 0x40000,
                                    0x20000, 120);
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 20);
      for (i = 0; i != 32; i++) {
        spark = &work->particles[i];
        spark->variant = -i / 4 + 2;
        spark->x = px(target.x / 2);
        spark->y = px(target.y + (Random16() & 15) - 32);
        spark->velocityX =
            ((Random16() % gFume_Levels[args->variant][FUME_SPREAD]) -
             (gFume_Levels[args->variant][FUME_SPREAD] >> 1))
            << 12;
        spark->velocityY = (192 - (Random16() & 255)) << 10;
      }
    }
    for (i = 0; i != gFume_Levels[args->variant][FUME_SPARKS]; i++) {
      particle = &work->particles[i];
      if ((u32)particle->variant <= 23) {
        if (particle->variant > 1) {
          blit.draw(canvas,
                    work->sheet + 0x800 + ((particle->variant / 4) << 11),
                    Q_16_16_UPPER_HALF(particle->x) - 16,
                    Q_16_16_UPPER_HALF(particle->y) - 32, 32, 64);
        }
        BattleEffectKernels_IntegrateVector2(
            particle, 60, -gFume_Levels[args->variant][FUME_LIFT]);
      }
      particle->variant++;
    }

    buffer = Data_02011000;
    // The strip mesh held in a per-frame local, as the vertex buffer is.
    strip = Data_02012000;
    corners = (struct Corner *)gMapGridCellBuffer;
    // DEAD: frame never falls below 0; the source keeps the test.
    if (frame >= 0) {
      s32 width;
      s32 step;
      s32 sweep;
      s32 vertex;
      s32 shade;

      width = gFume_Levels[args->variant][FUME_WIDTH];
      arc.head = frame * 44;
      if (arc.head > 988) {
        arc.head = 988;
      }
      if (frame >= gFume_Levels[args->variant][FUME_TAIL_START]) {
        arc.tail = (frame - gFume_Levels[args->variant][FUME_TAIL_START]) * 44;
        if (arc.tail > 988) {
          arc.tail = 988;
        }
      }

      // The arc's vertices, two per segment, widening over the first six.
      for (j = 0, step = arc.head - arc.tail, sweep = 0, vertex = 0;
           j != FUME_SEGMENTS; sweep += step, vertex += 2, j++) {
        s32 distance = caster.x / 8 - target.x / 8;
        s32 angle = sweep + arc.tail * 33;
        s32 wobble = ((Trig_Sin((frame + j) << 12) << 2) >> 16) * j / 32;
        s32 height;
        s32 thickness;
        s32 radiusX;
        s32 radiusY;
        s32 half;
        s32 baseY;

        if (distance < 0) {
          distance = -distance;
        }
        height = gFume_Levels[args->variant][FUME_HEIGHT];
        thickness = j * 4;
        if (thickness > 24) {
          thickness = 24;
        }
        thickness = thickness * width / Q16_ONE;
        i = 0;
        half = thickness / -2;
        baseY = height + half;
        radiusY = baseY + wobble;
        radiusX = distance + half + wobble;
        for (; i != 2; i++) {
          corners[vertex + i].x =
              ((Trig_Cos(angle) * radiusX) >> 16) - distance;
          corners[vertex + i].y = -((Trig_Sin(angle) * radiusY) >> 16);
          corners[vertex + i].z = 0;
          radiusY += thickness;
          radiusX += thickness;
        }
        if (j == 32) {
          endX = (((distance + wobble) * Trig_Cos(angle)) >> 16) - distance;
          endY = -(((height + wobble) * Trig_Sin(angle)) >> 16);
        }
      }

      shade = 0;
      if (frame > 33) {
        shade = (34 - frame) * 8;
      }
      if (shade > -64) {
        model = Render_AllocateModelList(1);
        model->kind = 6;
        model->texture = &texture;
        model->shade = shade;
        model->scrollV = 0;
        model->vertices = buffer;
        if (arc.head > arc.tail) {
          texture.logWidth = 6;
          texture.logHeight = 5;
          texture.pixels = work->sheet;
          model->mesh = strip;
          model->scrollU = 127 & ~(frame << 3);
          Transform_Reset();
          SceneTransform_ApplyPosition3(px(caster.x / 2 - 64),
                                        px(caster.y) + 0xffc00000, 0);
          SceneTransform_ApplyScale3(Q16_ONE / 2, Q16_ONE, Q16_ONE / 2);
          if (args->side == EFFECT_SIDE_PARTY) {
            Transform_ApplyYaw(TRIG_TURN / 2);
          }
          SceneTransform_ApplyUniformScale(Q16_ONE * 4);
          Render_TransformVertices(corners, buffer, 66);
          Render_DrawModelList(model);

          // The claw at the arc's end.
          if (frame <= 22) {
            model->mesh = gBattleEffect_SquareMesh;
            texture.logWidth = 6;
            texture.logHeight = 6;
            model->scrollU = 0;
            texture.pixels = work->sheet + 0x4000;
            SceneTransform_ApplyPosition3(px(endX), px(endY), 0);
            Transform_ApplyRoll(TRIG_TURN / 4 - arc.head * 32);
            SceneTransform_ApplyUniformScale(width * 28 / 32);
            Render_TransformVertices(gBattleEffect_SquareCorners, buffer, 4);
            Render_DrawModelList(model);
          }
        }
        Heap_Release(model);
      }
    }
    BattleEffect_CountDownBattlerTints();
    BattleCamera_Shake(args->variant * 2 + 2, args->variant * 2 + 2);
    EFFECT_PRESENT_CANVAS(work);
  }
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}
#undef BLITTER
#undef FUME_FRAMES
#undef FUME_SEGMENTS
