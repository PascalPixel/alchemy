/// @file
/// Crystal.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

#define CRYSTAL_FRAMES 103
/// From this frame the canvas fades out, two sixteenths a frame, from the
/// level it is shown at.
#define CRYSTAL_FADE_FRAME 96
#define CRYSTAL_BLEND_LEVEL ((CRYSTAL_FRAMES - CRYSTAL_FADE_FRAME) * 2)
/// The orbs of light that rise around the target.
#define CRYSTAL_ORBS 32
/// Sixteen shaded discs, 2 to 32 pixels across, the orbs' pictures, drawn into
/// the map cell buffer; where each starts is kept in the effect's pictures at
/// CRYSTAL_DISC_OFFSETS.
#define CRYSTAL_DISCS 16
#define CRYSTAL_DISC_OFFSETS 10000
/// Where the cog's picture is made in the map cell buffer: CogTexture's
/// quarter mirrored into a picture twice as wide and as tall.
#define CRYSTAL_COG_PICTURE 16384
/// The cog's largest scale, about 1.74, which the model is drawn at half of.
#define CRYSTAL_COG_SCALE_MAX 114000
/// The cog's tilt about the x axis, about 16 degrees.
#define CRYSTAL_COG_TILT 3000

/// Where VenusDjinnSmallSheet is decoded in the effect's pictures.
#define CRYSTAL_VENUS_DJINN_SMALL_PICTURES 26048

/// A Venus Djinni appears and its small sprite dives onto the target; a
/// glowing cog spins and grows flat on the ground under the targets while
/// orbs of light rise around them, pulsing, and each target flashes white as
/// it is healed.
///
/// Abilities: Crystal.
void BattleEffect_RunCrystal(struct BattleEffectParams *args) {
  struct BattleEffectState *state;
  void *canvas;
  /* LEFTOVER(frame): draw[1], the pair's idle second blitter, which the ROM's
   * frame reserves and never touches; as one DrawRectangle. */
  DrawRectangle draw[2];
  u16 *discOffsets;
  s32 center[3];
  s32 other[3];
  struct ScreenPoint screen;
  struct ModelTexture texture;
  s32 frame;
  s32 i;
  s32 size;
  s32 offset;
  u8 *cells;

  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  discOffsets = (u16 *)(state->sheet + CRYSTAL_DISC_OFFSETS);
  cells = gMapGridCellBuffer;
  Iwram_ClearWords(cells, 32768);
  for (offset = 0, i = 0, size = 2; i != CRYSTAL_DISCS; size += 2, i++) {
    // Each disc is drawn after the one before it in the map cell buffer.
    BattleEffect_DrawShadedCircles((u8 *)(offset + (s32)cells), size,
                                   FIXED_ONE);
    discOffsets[i] = offset;
    offset += (i * 2 + 3) * size / 2;
  }
  Resource_LoadAndDecompress(RESOURCE_COG_TEXTURE, state->sheet, 0, 0);
  Picture_MirrorIntoQuadrants(state->sheet,
                              Ram_MapCellBuffer + CRYSTAL_COG_PICTURE, 64, 64);
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_RunDjinniEntrance(
      args,
      DJINNI_ENTRANCE(ELEMENT_VENUS, args->side ^ 1, DJINNI_ANCHOR_TARGET_X),
      center, other);
  Resource_LoadAndDecompress(RESOURCE_VENUS_DJINN_SMALL_SHEET,
                             state->sheet + CRYSTAL_VENUS_DJINN_SMALL_PICTURES,
                             1, 0);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry(RESOURCE_BLAST_SHEET),
                       SHEET_PALETTE_BYTES);
  REG_BLDALPHA = EFFECT_BLEND_CANVAS(CRYSTAL_BLEND_LEVEL);
  if (args->side == EFFECT_SIDE_PARTY) {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                        BLITTER_ADDITIVE);
  } else {
    EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                        EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                        BLITTER_ADDITIVE);
  }
  draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
  {
    struct BattleCamera *camera;
    struct MotionObject *target;

    EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE,
                        CANVAS_FADE_THREE_QUARTERS);
    Task_Add(BattleEffect_TransferCanvas, TASK_PASS_VBLANK);
    camera = Ram_HeapSlots->cameraState;
    target = BattleObject_GetFar(args->targets[0])->object;

    for (frame = 0; frame != CRYSTAL_FRAMES; frame++) {
      // The orbs start in a ring around the first target: velocityX holds the
      // phase of an orb's pulse, velocityY its rising speed, velocityZ its
      // size.
      if (frame == 0) {
        struct Particle *orb;
        for (i = 0, orb = state->particles; i != CRYSTAL_ORBS; i++, orb++) {
          s32 angle = Random16();
          s32 radius = (Random16() & 63) + 80;

          SET_POSITION(orb, target->x + ((Trig_Sin(angle) * radius) >> 1),
                       toFIXED(8), (Trig_Cos(angle) * radius) >> 1);
          orb->velocityX = Random16();
          orb->velocityY = (Random16() & (FIXED_ONE - 1)) + toFIXED(4);
          orb->velocityZ = (Random16() & 7) + 4;
          orb->variant = -1;
        }
      }
      if (frame >= CRYSTAL_FADE_FRAME) {
        REG_BLDALPHA = EFFECT_BLEND_CANVAS(CRYSTAL_FRAMES * 2 - frame * 2);
      }
      Matrix_Initialize();
      Matrix_SetLookAt(camera->eye, camera->focus);

      // The small Djinni dives, swaying, onto the target, drawn twice over
      // its first four frames.
      if (frame <= 19) {
        s32 x = (FIXEDtoINT(Trig_Sin(frame * (TRIG_TURN / 32)) * 8)) +
                center[0] / 2 - 10;
        s32 y = (FIXEDtoINT(Trig_Cos(frame * (TRIG_TURN / 32)) * 4)) +
                center[1] - 24;

        y += frame * 3;

        draw[0](canvas, state->sheet + CRYSTAL_VENUS_DJINN_SMALL_PICTURES, x, y,
                SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
        if (frame <= 3) {
          draw[0](canvas, state->sheet + CRYSTAL_VENUS_DJINN_SMALL_PICTURES, x,
                  y, SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
        }
      }

      // The orbs rise, one more every two frames from frame 48, their sizes
      // pulsing.
      if (frame > 47) {
        struct Particle *orb;
        for (i = 0, orb = state->particles; i != CRYSTAL_ORBS; i++, orb++) {
          if (frame >= i * 2 + 48) {
            s32 width;

            EffectPosition_ProjectPoint(&orb->x, &screen);
            screen.x >>= 1;
            width = orb->velocityZ;
            width += FIXEDtoINT(Trig_Sin(orb->velocityX) * width / 2);
            orb->velocityX += toFIXED(1) / 64;
            width *= 2;
            if ((u32)(width - 2) <= 61) {
              s32 disc = (width - 2) / 2;
              s32 x = screen.x;
              s32 y = screen.y;

              // DEAD: never true, as every orb's variant stays -1; the ROM
              // keeps the test and the jitter.
              if (orb->variant == 0) {
                x += (Random16() & 7) - 4;
                y += (Random16() & 7) - 4;
              }
              Canvas_BlitMirroredQuarters(
                  (u8 *)((s16)discOffsets[disc] + (s32)cells), x, y, width);
            }
            orb->y += orb->velocityY;
          }
        }
      }

      EFFECT_FOR_EACH_TARGET(i, args) {
        if (frame == i * 4 + 64) {
          Sound_PlayFar(SE_RECOVERY);
          BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_WHITE,
                                   TINT_KEEP, i, 24);
        }
      }

      // The cog, spinning and growing from frame 16.
      Matrix_Initialize();
      gProjection.centerY = 104;
      {
        void *vertices = Heap_AllocEwram(32);
        struct RenderModel *model = Render_AllocateModelList(1);

        model->kind = MODEL_SHADED_MAX;
        model->texture = &texture;
        model->mesh = gBattleEffect_LargeSquareMesh;
        model->vertices = vertices;
        texture.logWidth = 7;
        texture.logHeight = 7;
        texture.pixels = Ram_MapCellBuffer + CRYSTAL_COG_PICTURE;
        if (frame > 15) {
          FIXED scale = frame * (FIXED_ONE / 16) - FIXED_ONE;

          if (scale > CRYSTAL_COG_SCALE_MAX) {
            scale = CRYSTAL_COG_SCALE_MAX;
          }
          model->shade = 0;
          Matrix_RotateX(-CRYSTAL_COG_TILT);
          Matrix_Translate(INTtoFIXED(center[0] / 2 - CANVAS_SIZE / 2),
                           toFIXED(40), 0);
          Matrix_ScaleUniform(scale >> 1);
          Matrix_RotateY(frame * (TRIG_TURN / 128));
          Render_TransformVertices(gBattleEffect_GroundSquareCorners, vertices,
                                   4);
          Render_DrawModelList(model);
        }
        Heap_Release(model);
        Heap_Release(vertices);
      }
      gProjection.centerX = BATTLE_PROJECTION_CENTER_X;
      gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;
      EFFECT_END_FRAME(state);
    }
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#undef CRYSTAL_FRAMES
#undef CRYSTAL_FADE_FRAME
#undef CRYSTAL_BLEND_LEVEL
#undef CRYSTAL_ORBS
#undef CRYSTAL_DISCS
#undef CRYSTAL_DISC_OFFSETS
#undef CRYSTAL_COG_PICTURE
#undef CRYSTAL_COG_SCALE_MAX
#undef CRYSTAL_COG_TILT
#undef CRYSTAL_VENUS_DJINN_SMALL_PICTURES
