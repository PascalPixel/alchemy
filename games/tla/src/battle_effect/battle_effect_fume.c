/// @file
/// Fume.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

struct StripQuad;

/// One corner of the plume: signed x, y and depth.
struct Corner {
  s8 x;
  s8 y;
  s8 z;
};

#define FUME_FRAMES 50
#define FUME_SEGMENTS 33
/// The arc grows by FUME_ARC_GROWTH a frame at its head, and later its tail,
/// up to its whole span.
#define FUME_ARC_GROWTH 44
#define FUME_ARC_SPAN 988
/// The strip's mesh and its transformed corners, in the map cell buffer.
#define FUME_STRIP_VERTICES (Ram_MapCellBuffer + 0x1000)
#define FUME_STRIP_MESH (Ram_MapCellBuffer + 0x2000)

/// Where FlashBurstSheet is decoded in the effect's pictures.
#define FUME_FLASH_BURST_PICTURES 0x800
/// Where ClawSlashSheet is decoded in the effect's pictures.
#define FUME_CLAW_SLASH_PICTURES 0x4000

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
  /* LEFTOVER(frame): blit[1], the pair's idle second blitter: the frame (sub
   * sp, #116) reserves it at sp + 68 and no instruction addresses it; as one
   * DrawRectangle, 74 of 627 instructions differ in tla-en. */
  DrawRectangle blit[2];
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

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  blit[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  endX = 0;
  endY = 0;
  EffectPosition_ProjectBattler(args->user, &caster);
  EffectPosition_ProjectBattler(args->targets[0], &target);
  Resource_LoadAndDecompress(RESOURCE_SMOKE_BAND_SHEET, work->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET,
                             work->sheet + FUME_FLASH_BURST_PICTURES, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_CLAW_SLASH_SHEET,
                             work->sheet + FUME_CLAW_SLASH_PICTURES, 1, 0);
  REG_BG2PA = Q_8_8(0.5);
  Render_BuildQuadStrip(FUME_STRIP_MESH, 4, 32, 32);

  for (frame = 0; frame != FUME_FRAMES; frame++) {
    if (frame == 0) {
      arc.head = 0;
      arc.tail = 0;
      for (i = 0; i != ARRAY_COUNT(work->particles); i++) {
        work->particles[i].variant = 24;
      }
    }
    if (frame == 22) {
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                           CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
      EFFECT_SHOW_DAMAGE(SE_CLATTER);
      work->shakeFrames = 8;
      BattleEffect_StartActorMotion(args->targets[0], 1, toFIXED(2), toFIXED(4),
                                    toFIXED(2), 120);
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 20);
      for (i = 0; i != 32; i++) {
        spark = &work->particles[i];
        spark->variant = -i / 4 + 2;
        spark->x = INTtoFIXED(target.x / 2);
        spark->y = INTtoFIXED(target.y + (Random16() & 15) - 32);
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
          blit[0](canvas,
                  work->sheet + FUME_FLASH_BURST_PICTURES +
                      ((particle->variant / 4) << 11),
                  FIXED_INTEGER_PART(particle->x) - 16,
                  FIXED_INTEGER_PART(particle->y) - 32, FLASH_BURST_WIDTH,
                  FLASH_BURST_HEIGHT);
        }
        Particle_AdvanceWithGravity2D(particle, 60,
                                      -gFume_Levels[args->variant][FUME_LIFT]);
      }
      particle->variant++;
    }

    buffer = FUME_STRIP_VERTICES;
    // The strip mesh held in a per-frame local, as the vertex buffer is.
    strip = FUME_STRIP_MESH;
    corners = (struct Corner *)gMapGridCellBuffer;
    // DEAD: frame never falls below 0; the ROM keeps the compare and branch.
    if (frame >= 0) {
      s32 width;
      s32 step;
      s32 sweep;
      s32 vertex;
      s32 shade;

      width = gFume_Levels[args->variant][FUME_WIDTH];
      arc.head = frame * FUME_ARC_GROWTH;
      if (arc.head > FUME_ARC_SPAN) {
        arc.head = FUME_ARC_SPAN;
      }
      if (frame >= gFume_Levels[args->variant][FUME_TAIL_START]) {
        arc.tail = (frame - gFume_Levels[args->variant][FUME_TAIL_START]) *
                   FUME_ARC_GROWTH;
        if (arc.tail > FUME_ARC_SPAN) {
          arc.tail = FUME_ARC_SPAN;
        }
      }

      // The arc's vertices, two per segment, widening over the first six.
      for (j = 0, step = arc.head - arc.tail, sweep = 0, vertex = 0;
           j != FUME_SEGMENTS; sweep += step, vertex += 2, j++) {
        s32 distance = caster.x / 8 - target.x / 8;
        s32 angle = sweep + arc.tail * 33;
        s32 wobble = (FIXEDtoINT(Trig_Sin((frame + j) << 12) << 2)) * j / 32;
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
        thickness = thickness * width / FIXED_ONE;
        i = 0;
        half = thickness / -2;
        baseY = height + half;
        radiusY = baseY + wobble;
        radiusX = distance + half + wobble;
        for (; i != 2; i++) {
          corners[vertex + i].x =
              (FIXEDtoINT(Trig_Cos(angle) * radiusX)) - distance;
          corners[vertex + i].y = -(FIXEDtoINT(Trig_Sin(angle) * radiusY));
          corners[vertex + i].z = 0;
          radiusY += thickness;
          radiusX += thickness;
        }
        if (j == FUME_SEGMENTS - 1) {
          endX = (FIXEDtoINT((distance + wobble) * Trig_Cos(angle))) - distance;
          endY = -(FIXEDtoINT((height + wobble) * Trig_Sin(angle)));
        }
      }

      shade = 0;
      if (frame > 33) {
        shade = (34 - frame) * 8;
      }
      if (shade > MODEL_SHADE_INVISIBLE) {
        model = Render_AllocateModelList(1);
        model->kind = MODEL_SHADED_MAX;
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
          Matrix_Initialize();
          Matrix_Translate(INTtoFIXED(caster.x / 2 - 64),
                           INTtoFIXED(caster.y) - toFIXED(64), 0);
          Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
          if (args->side == EFFECT_SIDE_PARTY) {
            Matrix_RotateY(DEGtoANG(180));
          }
          Matrix_ScaleUniform(FIXED_ONE * 4);
          Render_TransformVertices(corners, buffer, 66);
          Render_DrawModelList(model);

          // The claw at the arc's end.
          if (frame <= 22) {
            model->mesh = gBattleEffect_SquareMesh;
            texture.logWidth = 6;
            texture.logHeight = 6;
            model->scrollU = 0;
            texture.pixels = work->sheet + FUME_CLAW_SLASH_PICTURES;
            Matrix_Translate(INTtoFIXED(endX), INTtoFIXED(endY), 0);
            Matrix_RotateZ(DEGtoANG(90) - arc.head * 32);
            Matrix_ScaleUniform(width * 28 / 32);
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
#undef FUME_ARC_GROWTH
#undef FUME_ARC_SPAN
#undef FUME_STRIP_VERTICES
#undef FUME_STRIP_MESH
