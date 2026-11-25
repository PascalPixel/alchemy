/// @file
/// Fume.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

/// One corner of the plume: signed x, y and depth.
struct Corner {
  s8 x;
  s8 y;
  s8 z;
};

#define FUME_FRAMES 50
/// The frame the plume's head reaches the target and the target is hit.
#define FUME_HIT_FRAME 22
/// The plume is FUME_SEGMENTS pairs of corners, laid in the map cell buffer:
/// the corners, those Render_TransformVertices makes of them at
/// FUME_STRIP_VERTICES, and the plume's mesh at FUME_STRIP_MESH, whose quads
/// each take 4 texels of SmokeBandSheet's flames across and its 32 down.
#define FUME_SEGMENTS 33
#define FUME_CORNERS (FUME_SEGMENTS * 2)
#define FUME_QUADS (FUME_SEGMENTS - 1)
#define FUME_STRIP_VERTICES (Ram_MapCellBuffer + 4096)
#define FUME_STRIP_MESH (Ram_MapCellBuffer + 8192)
/// The plume arcs from the caster over to the target. Its head goes
/// FUME_ARC_GROWTH further each frame, and from the level's FUME_TAIL_START
/// its tail follows, each up to FUME_ARC_SPAN; a point's angle on the arc is
/// FUME_SEGMENTS times its place.
#define FUME_ARC_GROWTH 44
#define FUME_ARC_SPAN 988
/// The plume thickens 4 pixels a segment from its tail up to FUME_THICKNESS,
/// times the level's width.
#define FUME_THICKNESS 24
/// The plume sways up to FUME_SWAY pixels at its head, less towards its tail,
/// a wave that runs along it a segment a frame.
#define FUME_SWAY 4
/// From this frame the plume dims, 8 shades a frame.
#define FUME_DIM_FRAME 34
/// The head's scale, seven eighths of the plume's width.
#define FUME_HEAD_SCALE(width) ((width) * 28 / 32)

/// The fireballs that burst from the target: FUME_BURSTS_MAX are set out at
/// the hit, four more showing each frame, and a level draws gFume_Levels'
/// FUME_SPARKS of them. Each shows FlashBurstSheet's six pictures in turn,
/// FUME_BURST_PICTURE_FRAMES apiece, for FUME_BURST_FRAMES.
#define FUME_BURSTS_MAX 32
#define FUME_BURST_PICTURE_FRAMES 4
#define FUME_BURST_FRAMES 24

/// Where FlashBurstSheet is decoded in the effect's pictures.
#define FUME_FLASH_BURST_PICTURES 2048
/// Where ClawSlashSheet, a dragon's head of flame, is decoded in the effect's
/// pictures.
#define FUME_DRAGON_HEAD_PICTURES 16384

/// A plume of flames rises from the caster and arcs over onto the target, a
/// dragon's head of fire at its front. As the head strikes, the screen flashes
/// white, the target is thrown back and fireballs burst from it; the plume
/// follows its head down and fades. The stronger the level, the thicker the
/// plume, the more fireballs and the harder the shake.
///
/// Abilities: Fume, Serpent Fume, Dragon Fume.
void BattleEffect_RunFume(struct BattleEffectParams *args) {
  struct BattleEffectState *state;
  void *canvas;
  /* LEFTOVER(frame): blit[1], the pair's idle second blitter, which the ROM's
   * frame reserves and never touches; as one DrawRectangle. */
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
  struct Particle *burst;
  struct Particle *particle;
  s32 i;
  void *vertices;
  u8 *mesh;
  struct Corner *corners;

  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  BattleEffect_StartCanvasLayer(BGCNT_PRIORITY(1));
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  blit[0] = Ram_HeapSlots->ramCode;
  EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  // The head's place, set at the plume's last segment.
  endX = 0;
  endY = 0;
  EffectPosition_ProjectBattler(args->user, &caster);
  EffectPosition_ProjectBattler(args->targets[0], &target);
  Resource_LoadAndDecompress(RESOURCE_SMOKE_BAND_SHEET, state->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_FLASH_BURST_SHEET,
                             state->sheet + FUME_FLASH_BURST_PICTURES, 1, 0);
  Resource_LoadAndDecompress(RESOURCE_CLAW_SLASH_SHEET,
                             state->sheet + FUME_DRAGON_HEAD_PICTURES, 1, 0);
  REG_BG2PA = Q_8_8(0.5);
  Render_BuildQuadStrip(FUME_STRIP_MESH, 4, 32, FUME_QUADS);

  for (frame = 0; frame != FUME_FRAMES; frame++) {
    if (frame == 0) {
      arc.head = 0;
      arc.tail = 0;
      // No fireball shows until the hit.
      for (i = 0; i != ARRAY_COUNT(state->particles); i++) {
        state->particles[i].variant = FUME_BURST_FRAMES;
      }
    }
    if (frame == FUME_HIT_FRAME) {
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                           CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
      EFFECT_SHOW_DAMAGE(SE_CLATTER);
      state->shakeFrames = 8;
      BattleEffect_StartActorMotion(args->targets[0], 1, toFIXED(2), toFIXED(4),
                                    toFIXED(2), 120);
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 20);
      for (i = 0; i != FUME_BURSTS_MAX; i++) {
        burst = &state->particles[i];
        burst->variant = -i / 4 + 2;
        burst->x = INTtoFIXED(target.x / 2);
        burst->y = INTtoFIXED(target.y + (Random16() & 15) - 32);
        burst->velocityX =
            ((Random16() % gFume_Levels[args->variant][FUME_SPREAD]) -
             (gFume_Levels[args->variant][FUME_SPREAD] >> 1))
            << 12;
        burst->velocityY = (192 - (Random16() & 255)) << 10;
      }
    }
    for (i = 0; i != gFume_Levels[args->variant][FUME_SPARKS]; i++) {
      particle = &state->particles[i];
      if ((u32)particle->variant < FUME_BURST_FRAMES) {
        if (particle->variant > 1) {
          blit[0](canvas,
                  state->sheet + FUME_FLASH_BURST_PICTURES +
                      particle->variant / FUME_BURST_PICTURE_FRAMES *
                          PICTURE_BYTES(FLASH_BURST),
                  FIXED_INTEGER_PART(particle->x) - FLASH_BURST_WIDTH / 2,
                  FIXED_INTEGER_PART(particle->y) - FLASH_BURST_HEIGHT / 2,
                  FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
        }
        Particle_AdvanceWithGravity2D(particle, 60,
                                      -gFume_Levels[args->variant][FUME_LIFT]);
      }
      particle->variant++;
    }

    vertices = FUME_STRIP_VERTICES;
    mesh = FUME_STRIP_MESH;
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

      // The plume's corners, two a segment, stepping evenly from the tail's
      // angle towards the head's on an ellipse from the caster to the target.
      for (j = 0, step = arc.head - arc.tail, sweep = 0, vertex = 0;
           j != FUME_SEGMENTS; sweep += step, vertex += 2, j++) {
        s32 distance = caster.x / 8 - target.x / 8;
        s32 angle = sweep + arc.tail * FUME_SEGMENTS;
        s32 sway =
            (FIXEDtoINT(Trig_Sin((frame + j) * (TRIG_TURN / 16)) * FUME_SWAY)) *
            j / (FUME_SEGMENTS - 1);
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
        if (thickness > FUME_THICKNESS) {
          thickness = FUME_THICKNESS;
        }
        thickness = thickness * width / FIXED_ONE;
        i = 0;
        half = thickness / -2;
        baseY = height + half;
        radiusY = baseY + sway;
        radiusX = distance + half + sway;
        for (; i != 2; i++) {
          corners[vertex + i].x =
              (FIXEDtoINT(Trig_Cos(angle) * radiusX)) - distance;
          corners[vertex + i].y = -(FIXEDtoINT(Trig_Sin(angle) * radiusY));
          corners[vertex + i].z = 0;
          radiusY += thickness;
          radiusX += thickness;
        }
        if (j == FUME_SEGMENTS - 1) {
          endX = (FIXEDtoINT((distance + sway) * Trig_Cos(angle))) - distance;
          endY = -(FIXEDtoINT((height + sway) * Trig_Sin(angle)));
        }
      }

      shade = 0;
      if (frame >= FUME_DIM_FRAME) {
        shade = (FUME_DIM_FRAME - frame) * 8;
      }
      if (shade > MODEL_SHADE_INVISIBLE) {
        model = Render_AllocateModelList(1);
        model->kind = MODEL_SHADED_MAX;
        model->texture = &texture;
        model->shade = shade;
        model->scrollV = 0;
        model->vertices = vertices;
        if (arc.head > arc.tail) {
          // The plume, its flames running along it 8 texels a frame.
          texture.logWidth = 6;
          texture.logHeight = 5;
          texture.pixels = state->sheet;
          model->mesh = mesh;
          model->scrollU = TEXTURE_SCROLL_MASK & ~(frame << 3);
          Matrix_Initialize();
          Matrix_Translate(INTtoFIXED(caster.x / 2 - CANVAS_SIZE / 2),
                           INTtoFIXED(caster.y) - toFIXED(64), 0);
          Matrix_Scale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
          if (args->side == EFFECT_SIDE_PARTY) {
            Matrix_RotateY(DEGtoANG(180));
          }
          Matrix_ScaleUniform(FIXED_ONE * 4);
          Render_TransformVertices(corners, vertices, FUME_CORNERS);
          Render_DrawModelList(model);

          // The head at the plume's end, turned with the arc, until it strikes.
          if (frame <= FUME_HIT_FRAME) {
            model->mesh = gBattleEffect_SquareMesh;
            texture.logWidth = 6;
            texture.logHeight = 6;
            model->scrollU = 0;
            texture.pixels = state->sheet + FUME_DRAGON_HEAD_PICTURES;
            Matrix_Translate(INTtoFIXED(endX), INTtoFIXED(endY), 0);
            Matrix_RotateZ(DEGtoANG(90) - arc.head * (FUME_SEGMENTS - 1));
            Matrix_ScaleUniform(FUME_HEAD_SCALE(width));
            Render_TransformVertices(gBattleEffect_SquareCorners, vertices, 4);
            Render_DrawModelList(model);
          }
        }
        Heap_Release(model);
      }
    }
    BattleEffect_CountDownBattlerTints();
    BattleCamera_Shake(args->variant * 2 + 2, args->variant * 2 + 2);
    EFFECT_PRESENT_CANVAS(state);
  }
  HeapSlot_Release(HEAP_SLOT_RAM_CODE);
  Task_Remove(BattleEffect_TransferCanvas);
  BattleEffect_EndCanvasLayer();
}
#undef FUME_FRAMES
#undef FUME_HIT_FRAME
#undef FUME_SEGMENTS
#undef FUME_CORNERS
#undef FUME_QUADS
#undef FUME_STRIP_VERTICES
#undef FUME_STRIP_MESH
#undef FUME_ARC_GROWTH
#undef FUME_ARC_SPAN
#undef FUME_THICKNESS
#undef FUME_SWAY
#undef FUME_DIM_FRAME
#undef FUME_HEAD_SCALE
#undef FUME_BURSTS_MAX
#undef FUME_BURST_PICTURE_FRAMES
#undef FUME_BURST_FRAMES
#undef FUME_FLASH_BURST_PICTURES
#undef FUME_DRAGON_HEAD_PICTURES
